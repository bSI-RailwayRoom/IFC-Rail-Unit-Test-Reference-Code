#pragma once

#include	<stdio.h>
#include	<time.h>
#include	<stdint.h>

#include	<assert.h>


char	* _itoa_int(
				int_t		value,
				char		* result,
				int_t		base
			)
{
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	
	char* ptr = result, *ptr1 = result, tmp_char;
	int_t tmp_value;
	
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	
	// Apply negative sign
	if (tmp_value < 0) {
		*ptr++ = '-';
	}
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

size_t	printInternal(char * fpc, char * str)
{
	size_t	size = strlen(str);

	if (fpc) {
		memcpy(fpc, str, strlen(str));
	}

	return	size;
}

size_t	printInternal(char * fpc, int_t value)
{

	char	str[100];

	_itoa_int(value, str, 10);

	return	printInternal(fpc, str);
}

size_t	WriteHeader(char * fpc, char * ifcFileName, int issueCnt, double absoluteEpsilon, double relativeEpsilon)
{
	size_t	size = 0;

	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "<html lang=\"en\"><head>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "<title>Roundcube Webmail :: </title>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "<!-- meta ignored -->\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "    <!-- meta ignored -->\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "	\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "    <link href=\"./?_task=utils&amp;_action=modcss&amp;_u=tmp-4c587b2867b0b915aa383cab9f390143.css\" rel=\"stylesheet\">\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "    <style type=\"text/css\">body { font-family: 'Arial', sans-serif; padding: 10px 40px; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        span.time { color: #999; font-style: italic; float: right; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        span.step-time { float: right; color: #555; font-size: 0.8em; font-style: italic; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        span.success { background-color: #97cc64; padding: 5px; border-radius: 5px; color: #FFF; font-weight: bold; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        span.failure { background-color: #fb5a3e; padding: 5px; border-radius: 5px; color: #FFF; font-weight: bold; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        p.failure { background-color: #fb5a3e; padding: 5px; border-radius: 5px; color: #fff; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        p.unspecified { background-color: #994f00; padding: 5px; border-radius: 5px; color: #fff; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        p.skipped { background-color: #8b8d8f; padding: 5px; border-radius: 5px; color: #fff; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        p.description { background-color: #eee; border-radius: 5px; padding: 20px; margin-left: auto; margin-right: auto; display: inline-block; font-weight: bold;}\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        li { padding: 10px; font-family: monospace; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        li.success { background-color: #b6cca1; color: #333; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        li.failure { background-color: #fbb4a8; color: #900; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        li.unspecified { background-color: #ffd37f; color: #a30; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        li.skipped { background-color: #f5f5f5; color: #333; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        li p { margin-bottom: 0px; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        footer { color: #999; font-size: 0.8em; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        header { text-align: center; }\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        hr { margin: 20px; margin-left: 0px; margin-right: 0px;  border: none; border-top: 1px solid #ccc; }</style>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "<link rel=\"stylesheet\" type=\"text/css\" href=\"skins/elastic/styles/embed.css?s = 1612812581\">\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "<script type=\"text/javascript\">\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "/* <![CDATA[ */\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "rcmail.set_env({\"orig_task\":null});\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "/* ]]> */\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "</script>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "\n\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "</head>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "<body>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "    <!-- header ignored --><!-- meta ignored -->\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <h1>");
	size += printInternal(fpc ? &fpc[size] : nullptr, ifcFileName);
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "</h1>\n");

	char		timeStamp[512];
	time_t		t;
//	struct tm	tInfo;

	time(&t);

	struct tm	* timeInfo =
#ifdef SAFE
					new tm();
	localtime_s(timeInfo, &t);
#else 
					localtime(&t);
#endif // SAFE


//	localtime_s(&tInfo, &t);

//	_itoa_s(1900 + tInfo.tm_year, &timeStamp[0], 512, 10);
//	_itoa_s(100 + 1 + tInfo.tm_mon, &timeStamp[4], 512 - 4, 10);
//	_itoa_s(100 + tInfo.tm_mday, &timeStamp[7], 512 - 7, 10);
	_itoa_int(1900 + timeInfo->tm_year, &timeStamp[0], 10);
	_itoa_int(100 + 1 + timeInfo->tm_mon, &timeStamp[4], 10);
	_itoa_int(100 + timeInfo->tm_mday, &timeStamp[7], 10);
	timeStamp[4] = '-';
	timeStamp[7] = '-';
//	_itoa_s(100 + tInfo.tm_hour, &timeStamp[10], 512 - 10, 10);
//	_itoa_s(100 + tInfo.tm_min, &timeStamp[13], 512 - 13, 10);
//	_itoa_s(100 + tInfo.tm_sec, &timeStamp[16], 512 - 16, 10);
	_itoa_int(100 + timeInfo->tm_hour, &timeStamp[10], 10);
	_itoa_int(100 + timeInfo->tm_min, &timeStamp[13], 10);
	_itoa_int(100 + timeInfo->tm_sec, &timeStamp[16], 10);
	timeStamp[10] = ' ';
	timeStamp[13] = ':';
	timeStamp[16] = ':';
	timeStamp[19] = 0;

//	fprintf(fp, "        <p><strong>%s </strong></p>\n", timeStamp);
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <p><strong>");
	size += printInternal(fpc ? &fpc[size] : nullptr, timeStamp);
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) " </strong></p>\n");

//	fprintf(fp, "        <hr />\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <hr />\n");


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
	}	//	*/

	if (checkTypeErrorCnt) {
		size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <span class=\"failure\">Failure</span>\n");
	}
	else {
		size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <span class=\"success\">Success</span>\n");
	}
	int percetage = (int) (100. * (double) (checkTypeCnt - checkTypeErrorCnt) / (double) checkTypeCnt);
//	size += printInternal(fpc ? &fpc[size] : nullptr, "        Tests passed: <strong>%i / %i</strong> (%i%%)\n", checkTypeCnt - checkTypeErrorCnt, checkTypeCnt, percetage);
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        Tests passed: <strong>");
	size += printInternal(fpc ? &fpc[size] : nullptr, checkTypeCnt - checkTypeErrorCnt);
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) " / ");
	size += printInternal(fpc ? &fpc[size] : nullptr, checkTypeCnt);
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "</strong> (");
	size += printInternal(fpc ? &fpc[size] : nullptr, percetage);
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "%%)\n");

	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <br />\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <p class=\"description\">\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            This checker checks geometry that is defined by alignment segments.<br />\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        </p>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <hr />\n");

	return	size;
}

size_t	WriteBody(char* fpc, int issueCnt, double absoluteEpsilon, double relativeEpsilon)
{
	size_t	size = 0;

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

			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "    <!-- section ignored -->\n");

//			size += printInternal(fpc ? &fpc[size] : nullptr, "        <h2>%s</h2>\n", GetGroupNameC(group, -1));
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <h2>");
			size += printInternal(fpc ? &fpc[size] : nullptr, GetGroupNameC(group, -1));
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "</h2>\n");

			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <p> \n");
			if (checkErrorCnt) {
				size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            <span class=\"failure\">Failure</span>\n");
			}
			else {
				size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            <span class=\"success\">Success</span>\n");
			}
			int percetage = (int) (100. * (double) (checkCnt - checkErrorCnt) / (double) checkCnt);

//			size += printInternal(fpc ? &fpc[size] : nullptr, "            Tests passed: <strong>%i / %i</strong> (%i%%)\n", checkCnt - checkErrorCnt, checkCnt, percetage);
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            Tests passed: <strong>");
			size += printInternal(fpc ? &fpc[size] : nullptr, checkCnt - checkErrorCnt);
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) " / ");
			size += printInternal(fpc ? &fpc[size] : nullptr, checkCnt);
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "</strong> (");
			size += printInternal(fpc ? &fpc[size] : nullptr, percetage);
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "%%)\n");

			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            <span class=\"time\"> \n");
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            Duration: 0.01s\n");
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            </span> \n");
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        </p> \n");
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <ol> \n");
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
							size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "			<li class=\"failure\">\n");
						}
						else {
							size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            <li class=\"success\">\n");
						}

//						size += printInternal(fpc ? &fpc[size] : nullptr, "                %s\n", GetGroupNameC(group, subGroup));
						size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "                ");
						size += printInternal(fpc ? &fpc[size] : nullptr, GetGroupNameC(group, subGroup));
						size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "\n");

						size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "                <span class=\"step - time\">0.01s</span>\n");

						for (int i = 0; i < issueCnt; i++) {
							bool	isError = false;
							char	* myIssue = GetIssueC(i, group, subGroup, absoluteEpsilon, relativeEpsilon, &isError);
							if (myIssue) {
								if (isError) {
									size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "				<p class=\"failure\"> \n");

//									size += printInternal(fpc ? &fpc[size] : nullptr, "					Assertion Failed: %s <br />\n", myIssue);
									size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "					Assertion Failed: ");
									size += printInternal(fpc ? &fpc[size] : nullptr, myIssue);
									size += printInternal(fpc ? &fpc[size] : nullptr, (char*) " <br />\n");

									size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "                </p>\n");
								}
								else {
									size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "				<p class=\"success\">\n");

//									size += printInternal(fpc ? &fpc[size] : nullptr, "                    For info: %s <br />\n", myIssue);
									size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "                    For info: ");
									size += printInternal(fpc ? &fpc[size] : nullptr, myIssue);
									size += printInternal(fpc ? &fpc[size] : nullptr, (char*) " <br />\n");

									size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "				</p> \n");
								}
							}
						}

						size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            </li> \n");
						subGroup++;
					}
					else {
						subGroup = -1;
					}
				}
			}
			size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        </ol> \n");

			group++;
		}
		else {
			group = -1;
		}
	}

	return	size;
}

size_t	WriteFooter(char * fpc)
{
	size_t	size = 0;
	
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "    <hr/>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "    <!-- footer ignored -->\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        <p>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "            The checker is developed based on <a href=\"http://rdf.bg/product-list/ifc-engine/\" target=\"_blank\" rel=\"noreferrer\">IfcEngine</a>.\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "        </p>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "		\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "</body>\n");
	size += printInternal(fpc ? &fpc[size] : nullptr, (char*) "</html>\n");

	return	size;
}

static	inline	char	* GetPage(
								int_t	model
							)
{
	size_t	len;

double	absoluteEpsilon = 0.0001,
		relativeEpsilon = 0.0001;
char	ifcFileName[19] = "test-file-name.ifc";

	int		issueCnt = CheckConsistencyAlignment((void*) model, relativeEpsilon);

	{
		size_t	size = 0;

		size += WriteHeader(nullptr, ifcFileName, issueCnt, absoluteEpsilon, relativeEpsilon);

		size += WriteBody(nullptr, issueCnt, absoluteEpsilon, relativeEpsilon);

		size += WriteFooter(nullptr);

		len = size;
	}

	char * fpc = new char[len + 1];

	{
		size_t	size = 0;

		size += WriteHeader(&fpc[size], ifcFileName, issueCnt, absoluteEpsilon, relativeEpsilon);

		size += WriteBody(&fpc[size], issueCnt, absoluteEpsilon, relativeEpsilon);

		size += WriteFooter(&fpc[size]);

		assert(len == size);
	}

	return	fpc;
}