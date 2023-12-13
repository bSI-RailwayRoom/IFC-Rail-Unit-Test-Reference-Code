// CheckAlignmentCMD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"

#include <iostream>

#include "./alignmentconsistency/include/alignmentconsistency.h"

int main(int argc, char * argv[])
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
//				fprintf(fp, GetPageC(absoluteEpsilon, relativeEpsilon));
				fprintf(fp, GetPageJSON(argv[1], absoluteEpsilon, relativeEpsilon));
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
