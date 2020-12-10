#pragma once

#include "ifcdirection.h"


static	inline	int_t	CreateVector(
                                int_t   model
                            )
{
    int_t	ifcVectorInstance = sdaiCreateInstanceBN(model, (char*) "IFCVECTOR");

    VECTOR2 orientation = { 1., 0. };
    sdaiPutAttrBN(ifcVectorInstance, "Orientation", sdaiINSTANCE, (void*) CreateDirection(model, &orientation));

    double  magnitude = 1.;
    sdaiPutAttrBN(ifcVectorInstance, "Magnitude", sdaiREAL, &magnitude);

    assert(ifcVectorInstance);

    return	ifcVectorInstance;
}

static	inline	int_t	CreateVector(
                                int_t   model,
                                VECTOR2 * orientation
                            )
{
    int_t	ifcVectorInstance = sdaiCreateInstanceBN(model, (char*) "IFCVECTOR");

    sdaiPutAttrBN(ifcVectorInstance, "Orientation", sdaiINSTANCE, (void*) CreateDirection(model, orientation));

    double  magnitude = 1.;
    sdaiPutAttrBN(ifcVectorInstance, "Magnitude", sdaiREAL, &magnitude);

    assert(ifcVectorInstance);

    return	ifcVectorInstance;
}
