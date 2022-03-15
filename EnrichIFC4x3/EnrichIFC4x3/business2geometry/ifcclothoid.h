//  These sources are licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http ://www.apache.org/licenses/LICENSE-2.0
//  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

#pragma once


#include <cmath>

#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreateClothoidInstance(
                                int_t       model,
                                double      linearTerm
                            )
{
    int_t	ifcClothoidInstance = sdaiCreateInstanceBN(model, (char*) "IFCCLOTHOID");

    sdaiPutAttrBN(ifcClothoidInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model));

    sdaiPutAttrBN(ifcClothoidInstance, "ClothoidConstant", sdaiREAL, &linearTerm);

    assert(ifcClothoidInstance);

    return	ifcClothoidInstance;
}

static  inline  int_t   ___CreateClothoid(
                                int_t   model,
                                double  startRadiusOfCurvature,
                                double  endRadiusOfCurvature,
                                double  segmentLength,
                                double  * pOffset
                           )
{
    double  A;

    if (startRadiusOfCurvature == 0.) {
        assert(endRadiusOfCurvature);
        A = std::sqrt(std::fabs(endRadiusOfCurvature) * segmentLength);
        if (endRadiusOfCurvature < 0) { A = -A; }

        assert(*pOffset == 0.);
    }
    else if (endRadiusOfCurvature == 0.) {
        A = std::sqrt(std::fabs(startRadiusOfCurvature) * segmentLength);
        if (startRadiusOfCurvature > 0) { A = -A; }

        (*pOffset) = -segmentLength;
    }
    else if (startRadiusOfCurvature < 0. && endRadiusOfCurvature < 0.) {
        if (startRadiusOfCurvature < endRadiusOfCurvature) {
            //  Calculate clothoidConstant
            //      endRadius * (len + offset) = startRadius * offset
            //      startRadius * offset - endRadius * offset = endRadius * len
            //      offset = (endRadius * len) / (startRadius - endRadius)
            double  offsetLength = (endRadiusOfCurvature * segmentLength) / (startRadiusOfCurvature - endRadiusOfCurvature);
            A = std::sqrt(std::fabs(endRadiusOfCurvature) * (segmentLength + offsetLength));
            assert(A == std::sqrt(std::fabs(startRadiusOfCurvature) * offsetLength));

            A = -A;

            (*pOffset) = offsetLength;
        }
        else {
            assert(startRadiusOfCurvature > endRadiusOfCurvature);
            //  Calculate clothoidConstant
            //      startRadius * (len + offset) = endRadius * offset
            //      endRadius * offset - startRadius * offset = startRadius * len
            //      offset = (startRadius * len) / (endRadius - startRadius)
            double  offsetLength = (startRadiusOfCurvature * segmentLength) / (endRadiusOfCurvature - startRadiusOfCurvature);
            A = std::sqrt(std::fabs(startRadiusOfCurvature) * (segmentLength + offsetLength));
            assert(A == std::sqrt(std::fabs(endRadiusOfCurvature) * offsetLength));

            (*pOffset) = -(segmentLength + offsetLength);
        }
    }
    else if (startRadiusOfCurvature > 0. && endRadiusOfCurvature > 0.) {
        if (startRadiusOfCurvature < endRadiusOfCurvature) {
            //  Calculate clothoidConstant
            //      startRadius * (len + offset) = endRadius * offset
            //      endRadius * offset - startRadius * offset = startRadius * len
            //      offset = (startRadius * len) / (endRadius - startRadius)
            double  offsetLength = (startRadiusOfCurvature * segmentLength) / (endRadiusOfCurvature - startRadiusOfCurvature);
            A = std::sqrt(std::fabs(startRadiusOfCurvature) * (segmentLength + offsetLength));
            assert(A == std::sqrt(std::fabs(endRadiusOfCurvature) * offsetLength));

            A = -A;

            (*pOffset) = -(segmentLength + offsetLength);
        }
        else {
            assert(startRadiusOfCurvature > endRadiusOfCurvature);
            //  Calculate clothoidConstant
            //      endRadius * (len + offset) = startRadius * offset
            //      startRadius * offset - endRadius * offset = endRadius * len
            //      offset = (endRadius * len) / (startRadius - endRadius)
            double  offsetLength = (endRadiusOfCurvature * segmentLength) / (startRadiusOfCurvature - endRadiusOfCurvature);
            A = std::sqrt(std::fabs(endRadiusOfCurvature) * (segmentLength + offsetLength));
            assert(A == std::sqrt(std::fabs(startRadiusOfCurvature) * offsetLength));

            (*pOffset) = offsetLength;
        }
    }
    else {
        assert(false);
        A = 1.;
        assert(*pOffset == 0.);
    }

    return  ___CreateClothoidInstance(
                    model,
                    A
                );
}
