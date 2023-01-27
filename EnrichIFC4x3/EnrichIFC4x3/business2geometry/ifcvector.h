#pragma once


#include "ifcdirection.h"


static	inline	int_t	___CreateVector(
                                int_t       model
                            )
{
    int_t	ifcVectorInstance = sdaiCreateInstanceBN(model, "IFCVECTOR");

    ___VECTOR2  orientation = { 1., 0. };
    sdaiPutAttrBN(ifcVectorInstance, "Orientation", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, &orientation));

    double  magnitude = 1.;
    sdaiPutAttrBN(ifcVectorInstance, "Magnitude", sdaiREAL, &magnitude);

    assert(ifcVectorInstance);

    return	ifcVectorInstance;
}

static	inline	int_t	___CreateVector(
                                int_t       model,
                                ___VECTOR2  * orientation
                            )
{
    int_t	ifcVectorInstance = sdaiCreateInstanceBN(model, "IFCVECTOR");

    sdaiPutAttrBN(ifcVectorInstance, "Orientation", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, orientation));

    double  magnitude = 1.;
    sdaiPutAttrBN(ifcVectorInstance, "Magnitude", sdaiREAL, &magnitude);

    assert(ifcVectorInstance);

    return	ifcVectorInstance;
}
