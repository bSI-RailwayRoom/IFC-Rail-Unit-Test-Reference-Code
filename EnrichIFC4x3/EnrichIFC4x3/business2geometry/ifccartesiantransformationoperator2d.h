#pragma once


#include "mathematics.h"
#include "ifcengine.h"


static	inline	int_t	___CreateCartesianTransformationOperator2DInstance(
							    int_t   model,
                                double  offset
							)
{
	int_t	ifcCartesianTransformationOperator2DInstance;

    ifcCartesianTransformationOperator2DInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANTRANSFORMATIONOPERATOR2D");

	___VECTOR2	localOrigin = { 0., offset };
    sdaiPutAttrBN(ifcCartesianTransformationOperator2DInstance, "LocalOrigin", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, &localOrigin));

	assert(ifcCartesianTransformationOperator2DInstance);

	return	ifcCartesianTransformationOperator2DInstance;
}
