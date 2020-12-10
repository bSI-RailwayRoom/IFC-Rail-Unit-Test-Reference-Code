#pragma once

#include "ifccartesianpoint.h"
#include "ifcvector.h"


static	inline	int_t   CreateLine(
                                int_t   model
                            )
{
    int_t	ifcLineInstance = sdaiCreateInstanceBN(model, (char*) "IFCLINE");

    sdaiPutAttrBN(ifcLineInstance, "Pnt", sdaiINSTANCE, (void*) CreateCartesianPoint2D(model));
    sdaiPutAttrBN(ifcLineInstance, "Dir", sdaiINSTANCE, (void*) CreateVector(model));

    assert(ifcLineInstance);

    return	ifcLineInstance;
}

static	inline	int_t   CreateLine(
                                int_t   model,
                                VECTOR2 * orientation
                            )
{
    int_t	ifcLineInstance = sdaiCreateInstanceBN(model, (char*) "IFCLINE");

    sdaiPutAttrBN(ifcLineInstance, "Pnt", sdaiINSTANCE, (void*) CreateCartesianPoint2D(model));
    sdaiPutAttrBN(ifcLineInstance, "Dir", sdaiINSTANCE, (void*) CreateVector(model, orientation));

    assert(ifcLineInstance);

    return	ifcLineInstance;
}
