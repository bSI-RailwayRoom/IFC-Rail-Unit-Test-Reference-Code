#pragma once


#include "ifcdirection.h"


static	inline	SdaiInstance	___CreateVector(
                                        SdaiModel   model
                                    )
{
    SdaiInstance	ifcVectorInstance = sdaiCreateInstanceBN(model, "IFCVECTOR");

    ___VECTOR2  orientation = { 1., 0. };
    sdaiPutAttrBN(ifcVectorInstance, "Orientation", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, &orientation));

    double  magnitude = 1.;
    sdaiPutAttrBN(ifcVectorInstance, "Magnitude", sdaiREAL, &magnitude);

    assert(ifcVectorInstance);

    return	ifcVectorInstance;
}

static	inline	SdaiInstance	___CreateVector(
                                        SdaiModel   model,
                                        ___VECTOR2  * orientation
                                    )
{
    SdaiInstance	ifcVectorInstance = sdaiCreateInstanceBN(model, "IFCVECTOR");

    sdaiPutAttrBN(ifcVectorInstance, "Orientation", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, orientation));

    double  magnitude = 1.;
    sdaiPutAttrBN(ifcVectorInstance, "Magnitude", sdaiREAL, &magnitude);

    assert(ifcVectorInstance);

    return	ifcVectorInstance;
}
