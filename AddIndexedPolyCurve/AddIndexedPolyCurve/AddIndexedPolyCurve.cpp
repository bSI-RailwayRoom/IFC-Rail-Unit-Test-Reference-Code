// AddIndexedPolyCurve.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "./addCheck/include/addCheck.h"

int wmain(int argc, wchar_t* argv[])
{
    if (argc >= 3) {
        if (argc >= 4) {
            AddChecks(
                    argv[1],
                    argv[2],
                    _wtoi(argv[3])
                );
        }
        else {
            AddChecks(
                    argv[1],
                    argv[2],
                   100
                );
        }
    }
}
