// CheckAlignmentCMD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"

#include <iostream>

#include "./alignmentconsistency/include/alignmentconsistency.h"

void	WriteHeader(FILE * fp, char * ifcFileName, int issueCnt, double absoluteEpsilon, double relativeEpsilon)
{
	fprintf(fp, "<html lang=\"en\"><head>\n");
	fprintf(fp, "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
	fprintf(fp, "<title>Roundcube Webmail :: </title>\n");
	fprintf(fp, "<!-- meta ignored -->\n");
	fprintf(fp, "\n");
	fprintf(fp, "    <!-- meta ignored -->\n");
	fprintf(fp, "	\n");
	fprintf(fp, "    <link href=\"./?_task=utils&amp;_action=modcss&amp;_u=tmp-4c587b2867b0b915aa383cab9f390143.css\" rel=\"stylesheet\">\n");
	fprintf(fp, "    <style type=\"text/css\">body { font-family: 'Arial', sans-serif; padding: 10px 40px; }\n");
	fprintf(fp, "        span.time { color: #999; font-style: italic; float: right; }\n");
	fprintf(fp, "        span.step-time { float: right; color: #555; font-size: 0.8em; font-style: italic; }\n");
	fprintf(fp, "        span.success { background-color: #97cc64; padding: 5px; border-radius: 5px; color: #FFF; font-weight: bold; }\n");
	fprintf(fp, "        span.failure { background-color: #fb5a3e; padding: 5px; border-radius: 5px; color: #FFF; font-weight: bold; }\n");
	fprintf(fp, "        p.failure { background-color: #fb5a3e; padding: 5px; border-radius: 5px; color: #fff; }\n");
	fprintf(fp, "        p.unspecified { background-color: #994f00; padding: 5px; border-radius: 5px; color: #fff; }\n");
	fprintf(fp, "        p.skipped { background-color: #8b8d8f; padding: 5px; border-radius: 5px; color: #fff; }\n");
	fprintf(fp, "        p.description { background-color: #eee; border-radius: 5px; padding: 20px; margin-left: auto; margin-right: auto; display: inline-block; font-weight: bold;}\n");
	fprintf(fp, "        li { padding: 10px; font-family: monospace; }\n");
	fprintf(fp, "        li.success { background-color: #b6cca1; color: #333; }\n");
	fprintf(fp, "        li.failure { background-color: #fbb4a8; color: #900; }\n");
	fprintf(fp, "        li.unspecified { background-color: #ffd37f; color: #a30; }\n");
	fprintf(fp, "        li.skipped { background-color: #f5f5f5; color: #333; }\n");
	fprintf(fp, "        li p { margin-bottom: 0px; }\n");
	fprintf(fp, "        footer { color: #999; font-size: 0.8em; }\n");
	fprintf(fp, "        header { text-align: center; }\n");
	fprintf(fp, "        hr { margin: 20px; margin-left: 0px; margin-right: 0px;  border: none; border-top: 1px solid #ccc; }</style>\n");
	fprintf(fp, "\n");
	fprintf(fp, "<link rel=\"stylesheet\" type=\"text/css\" href=\"skins/elastic/styles/embed.css?s = 1612812581\">\n");
	fprintf(fp, "<script type=\"text/javascript\">\n");
	fprintf(fp, "/* <![CDATA[ */\n");
	fprintf(fp, "rcmail.set_env({\"orig_task\":null});\n");
	fprintf(fp, "/* ]]> */\n");
	fprintf(fp, "</script>\n");
	fprintf(fp, "\n\n");
	fprintf(fp, "</head>\n");
	fprintf(fp, "<body>\n");
	fprintf(fp, "    <!-- header ignored --><!-- meta ignored -->\n");
	fprintf(fp, "        <h1>%s</h1>\n", ifcFileName);


	char		timeStamp[512];
	time_t		t;
	struct tm	tInfo;

	time(&t);
	localtime_s(&tInfo, &t);

	_itoa_s(1900 + tInfo.tm_year, &timeStamp[0], 512, 10);
	_itoa_s(100 + 1 + tInfo.tm_mon, &timeStamp[4], 512 - 4, 10);
	_itoa_s(100 + tInfo.tm_mday, &timeStamp[7], 512 - 7, 10);
	timeStamp[4] = '-';
	timeStamp[7] = '-';
	_itoa_s(100 + tInfo.tm_hour, &timeStamp[10], 512 - 10, 10);
	_itoa_s(100 + tInfo.tm_min, &timeStamp[13], 512 - 13, 10);
	_itoa_s(100 + tInfo.tm_sec, &timeStamp[16], 512 - 16, 10);
	timeStamp[10] = ' ';
	timeStamp[13] = ':';
	timeStamp[16] = ':';
	timeStamp[19] = 0;

	fprintf(fp, "        <p><strong>%s </strong></p>\n", timeStamp);



	fprintf(fp, "        <hr />\n");

	int	group = 0, checkTypeCnt = 0, checkTypeErrorCnt = 0;
	while (group >= 0) {
		if (GetGroupNameC(group, -1)) {
			bool	subgroupHasError = false;

			int	subGroup = 0;
			while (subGroup >= 0) {
				if (GetGroupNameC(group, subGroup)) {
					for (int i = 0; i < issueCnt; i++) {
						bool	isError = false;
						GetIssueC(i, group, subGroup, absoluteEpsilon, relativeEpsilon, &isError);
						if (isError) {
							subgroupHasError = true;
						}
					}
					subGroup++;
				}
				else {
					subGroup = -1;
				}
			}

			if (subgroupHasError) {
				checkTypeErrorCnt++;
			}
			checkTypeCnt++;

			group++;
		}
		else {
			group = -1;
		}
	}

	if (checkTypeErrorCnt) {
		fprintf(fp, "        <span class=\"failure\">Failure</span>\n");
	}
	else {
		fprintf(fp, "        <span class=\"success\">Success</span>\n");
	}
	int percetage = (int) (100. * (double) (checkTypeCnt - checkTypeErrorCnt) / (double) checkTypeCnt);
	fprintf(fp, "        Tests passed: <strong>%i / %i</strong> (%i%%)\n", checkTypeCnt - checkTypeErrorCnt, checkTypeCnt, percetage);
	fprintf(fp, "        <br />\n");
	fprintf(fp, "        <p class=\"description\">\n");
	fprintf(fp, "            This checker checks geometry that is defined by alignment segments.<br />\n");
	fprintf(fp, "        </p>\n");
	fprintf(fp, "        <hr />\n");
}

void	WriteBody(FILE * fp, char * ifcFileName, int issueCnt, double absoluteEpsilon, double relativeEpsilon)
{
	int	group = 0;
	while (group >= 0) {
		if (GetGroupNameC(group, -1)) {
			int	checkCnt = 0, checkErrorCnt = 0;
			{
				int	subGroup = 0;
				while (subGroup >= 0) {
					if (GetGroupNameC(group, subGroup)) {
						bool	subgroupHasError = false;
						for (int i = 0; i < issueCnt; i++) {
							bool	isError = false;
							GetIssueC(i, group, subGroup, absoluteEpsilon, relativeEpsilon, &isError);
							if (isError) {
								subgroupHasError = true;
							}
						}

						if (subgroupHasError) checkErrorCnt++;
						checkCnt++;
						subGroup++;
					}
					else {
						subGroup = -1;
					}
				}
			}

			fprintf(fp, "    <!-- section ignored -->\n");
			fprintf(fp, "        <h2>%s</h2>\n", GetGroupNameC(group, -1));
			fprintf(fp, "        <p> \n");
			if (checkErrorCnt) {
				fprintf(fp, "            <span class=\"failure\">Failure</span>\n");
			}
			else {
				fprintf(fp, "            <span class=\"success\">Success</span>\n");
			}
			int percetage = (int) (100. * (double) (checkCnt - checkErrorCnt) / (double) checkCnt);
			fprintf(fp, "            Tests passed: <strong>%i / %i</strong> (%i%%)\n", checkCnt - checkErrorCnt, checkCnt, percetage);
			fprintf(fp, "            <span class=\"time\"> \n");
			fprintf(fp, "            Duration: 0.01s\n");
			fprintf(fp, "            </span> \n");
			fprintf(fp, "        </p> \n");
			fprintf(fp, "        <ol> \n");
			{
				int	subGroup = 0;
				while (subGroup >= 0) {
					if (GetGroupNameC(group, subGroup)) {
						bool	subgroupHasError = false;
						for (int i = 0; i < issueCnt; i++) {
							bool	isError = false;
							GetIssueC(i, group, subGroup, absoluteEpsilon, relativeEpsilon, &isError);
							if (isError) {
								subgroupHasError = true;
							}
						}

						if (subgroupHasError) {
							fprintf(fp, "			<li class=\"failure\">\n");
						}
						else {
							fprintf(fp, "            <li class=\"success\">\n");
						}

						fprintf(fp, "                %s\n", GetGroupNameC(group, subGroup));
						fprintf(fp, "                <span class=\"step - time\">0.01s</span>\n");

						for (int i = 0; i < issueCnt; i++) {
							bool	isError = false;
							char	* myIssue = GetIssueC(i, group, subGroup, absoluteEpsilon, relativeEpsilon, &isError);
							if (myIssue) {
								if (isError) {
									fprintf(fp, "				<p class=\"failure\"> \n");
									fprintf(fp, "					Assertion Failed: %s <br />\n", myIssue);
									fprintf(fp, "                </p>\n");
								}
								else {
									fprintf(fp, "				<p class=\"success\">\n");
									fprintf(fp, "                    For info: %s <br />\n", myIssue);
									fprintf(fp, "				</p> \n");
								}
							}
						}

						fprintf(fp, "            </li> \n");
						subGroup++;
					}
					else {
						subGroup = -1;
					}
				}
			}
			fprintf(fp, "        </ol> \n");

			group++;
		}
		else {
			group = -1;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc >= 3) {
		void	* model = OpenModelC(argv[1], (argc > 5) ? false : true);
		if (model) {
			double	absoluteEpsilon = 0., relativeEpsilon = 0.;
			if (argc >= 4) {
				absoluteEpsilon = atof(argv[3]);
			}

			if (argc >= 5) {
				relativeEpsilon = atof(argv[4]);
			}

			CheckConsistencyFile(model);
			int		issueCnt = CheckConsistencyAlignment(model, relativeEpsilon);

			FILE	* fp = nullptr;
			fopen_s(&fp, argv[2], "w");
			if (fp) {
				fprintf(fp, GetPageC(absoluteEpsilon, relativeEpsilon));
//				WriteHeader(fp, argv[1], issueCnt, absoluteEpsilon, relativeEpsilon);

//				WriteBody(fp, argv[1], issueCnt, absoluteEpsilon, relativeEpsilon);
				
//				WriteFooter(fp);
				fclose(fp);
			}

			CloseModel(model);
		}
		else {
			std::cout << "FILE NOT FOUND\n";
		}
	}
	else {
		std::cout << "ARGUMENT NOT FOUND\n";
	}

	std::cout << "Ready!\n";
}
