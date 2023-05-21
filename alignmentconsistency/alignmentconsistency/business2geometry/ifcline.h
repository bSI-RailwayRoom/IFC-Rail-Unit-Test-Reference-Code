#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	SdaiInstance    ___CreateLineInstance(
                                        SdaiModel   model
                                    )
{
    SdaiInstance    ifcLineInstance = sdaiCreateInstanceBN(model, "IFCLINE");

    sdaiPutAttrBN(ifcLineInstance, "Pnt", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance_2D(model));
    sdaiPutAttrBN(ifcLineInstance, "Dir", sdaiINSTANCE, (void*) ___CreateVector(model));

    assert(ifcLineInstance);

    return	ifcLineInstance;
}

static	inline	SdaiInstance    ___CreateLineInstance(
                                        SdaiModel   model,
                                        ___VECTOR2  * dir
                                    )
{
    SdaiInstance	ifcLineInstance = sdaiCreateInstanceBN(model, "IFCLINE");

    sdaiPutAttrBN(ifcLineInstance, "Pnt", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance_2D(model));
    sdaiPutAttrBN(ifcLineInstance, "Dir", sdaiINSTANCE, (void*) ___CreateVector(model, dir));

    assert(ifcLineInstance);

    return	ifcLineInstance;
}

static	inline	SdaiInstance    ___CreateLineInstance(
                                        SdaiModel   model,
                                        ___VECTOR2  * pnt,
                                        ___VECTOR2  * dir
                                    )
{
    SdaiInstance	ifcLineInstance = sdaiCreateInstanceBN(model, "IFCLINE");

    sdaiPutAttrBN(ifcLineInstance, "Pnt", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, pnt));
    sdaiPutAttrBN(ifcLineInstance, "Dir", sdaiINSTANCE, (void*) ___CreateVector(model, dir));

    assert(ifcLineInstance);

    return	ifcLineInstance;
}

static	inline	SdaiInstance    ___CreateVertexInstance(
                                        SdaiModel   model,
                                        ___VECTOR3  edge
                                    )
{
    SdaiInstance	ifcVertexPointInstance = sdaiCreateInstanceBN(model, "IFCVERTEXPOINT");

    sdaiPutAttrBN(ifcVertexPointInstance, "VertexGeometry", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, &edge));

    assert(ifcVertexPointInstance);

    return	ifcVertexPointInstance;
}

static	inline	SdaiInstance    ___CreateEdgeInstance(
                                        SdaiModel   model,
                                        ___VECTOR3  edgeStart,
                                        ___VECTOR3  edgeEnd
                                    )
{
    SdaiInstance	ifcEdgeInstance = sdaiCreateInstanceBN(model, "IFCEDGE");

    sdaiPutAttrBN(ifcEdgeInstance, "EdgeStart", sdaiINSTANCE, (void*) ___CreateVertexInstance(model, edgeStart));
    sdaiPutAttrBN(ifcEdgeInstance, "EdgeEnd", sdaiINSTANCE, (void*) ___CreateVertexInstance(model, edgeEnd));

    assert(ifcEdgeInstance);

    return	ifcEdgeInstance;
}

static	inline	double  ___LineLengthMeasureToParameterValue(
                                double      segmentLength,
                                double      lengthMeasure
                            )
{
    if (segmentLength)
        return  lengthMeasure / segmentLength;

    assert(false);
    return  lengthMeasure;
}
