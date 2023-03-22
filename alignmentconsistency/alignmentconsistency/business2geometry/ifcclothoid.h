//  These sources are licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http ://www.apache.org/licenses/LICENSE-2.0
//  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

#pragma once


#include <cmath>

#include "ifcaxis2placement2d.h"


static	inline	SdaiInstance    ___CreateClothoidInstance(
                                        SdaiModel   model,
                                        double      linearTerm,
                                        ___MATRIX   * matrix
                                    )
{
    SdaiInstance    ifcClothoidInstance = sdaiCreateInstanceBN(model, "IFCCLOTHOID");

    sdaiPutAttrBN(ifcClothoidInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));

    sdaiPutAttrBN(ifcClothoidInstance, "ClothoidConstant", sdaiREAL, &linearTerm);

    assert(ifcClothoidInstance);

    return	ifcClothoidInstance;
}
