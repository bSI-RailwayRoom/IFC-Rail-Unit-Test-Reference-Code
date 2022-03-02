#pragma once


#include "mathematics.h"
#include "ifcengine.h"


static	inline	int_t	___CreateCartesianTransformationOperator2D(
							    int_t   model,
                                double  offset
							)
{
	int_t	ifcCartesianTransformationOperator2DInstance;

    ifcCartesianTransformationOperator2DInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANTRANSFORMATIONOPERATOR2D");

	___VECTOR3	localOrigin = { 0., offset, 0. };
    sdaiPutAttrBN(ifcCartesianTransformationOperator2DInstance, "LocalOrigin", sdaiINSTANCE, (void*) ___CreateCartesianPoint3D(model, &localOrigin));

	assert(ifcCartesianTransformationOperator2DInstance);

	return	ifcCartesianTransformationOperator2DInstance;
}
