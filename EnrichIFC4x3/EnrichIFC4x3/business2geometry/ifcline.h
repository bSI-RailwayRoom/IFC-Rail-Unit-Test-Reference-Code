#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreateLineInstance(
                                int_t       model
                            )
{
    int_t	ifcLineInstance = sdaiCreateInstanceBN(model, (char*) "IFCLINE");

    sdaiPutAttrBN(ifcLineInstance, "Pnt", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance_2D(model));
    sdaiPutAttrBN(ifcLineInstance, "Dir", sdaiINSTANCE, (void*) ___CreateVector(model));

    assert(ifcLineInstance);

    return	ifcLineInstance;
}

static	inline	int_t   ___CreateLineInstance(
                                int_t       model,
                                ___VECTOR2  * orientation
                            )
{
    int_t	ifcLineInstance = sdaiCreateInstanceBN(model, (char*) "IFCLINE");

    sdaiPutAttrBN(ifcLineInstance, "Pnt", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance_2D(model));
    sdaiPutAttrBN(ifcLineInstance, "Dir", sdaiINSTANCE, (void*) ___CreateVector(model, orientation));

    assert(ifcLineInstance);

    return	ifcLineInstance;
}

static	inline	int_t   ___CreateLineInstance(
                                int_t       model,
                                ___VECTOR2  * pnt,
                                ___VECTOR2  * dir
                            )
{
    int_t	ifcLineInstance = sdaiCreateInstanceBN(model, (char*) "IFCLINE");

    sdaiPutAttrBN(ifcLineInstance, "Pnt", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, pnt));
    sdaiPutAttrBN(ifcLineInstance, "Dir", sdaiINSTANCE, (void*) ___CreateVector(model, dir));

    assert(ifcLineInstance);

    return	ifcLineInstance;
}

static	inline	int_t   ___CreateVertexInstance(
                                int_t       model,
                                ___VECTOR3  edge
                            )
{
    int_t	ifcVertexPointInstance = sdaiCreateInstanceBN(model, (char*) "IFCVERTEXPOINT");

    sdaiPutAttrBN(ifcVertexPointInstance, "VertexGeometry", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, &edge));

    assert(ifcVertexPointInstance);

    return	ifcVertexPointInstance;
}

static	inline	int_t   ___CreateEdgeInstance(
                                int_t       model,
                                ___VECTOR3  edgeStart,
                                ___VECTOR3  edgeEnd
                            )
{
    int_t	ifcEdgeInstance = sdaiCreateInstanceBN(model, (char*) "IFCEDGE");

    sdaiPutAttrBN(ifcEdgeInstance, "EdgeStart", sdaiINSTANCE, (void*) ___CreateVertexInstance(model, edgeStart));
    sdaiPutAttrBN(ifcEdgeInstance, "EdgeEnd", sdaiINSTANCE, (void*) ___CreateVertexInstance(model, edgeEnd));

    assert(ifcEdgeInstance);

    return	ifcEdgeInstance;
}
