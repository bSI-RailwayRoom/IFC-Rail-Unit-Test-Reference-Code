#pragma once


#include "mathematics.h"


static	inline	SdaiInstance	___CreateCartesianTransformationOperator2DInstance(
										SdaiModel   model,
										double		offset
									)
{
	SdaiInstance	ifcCartesianTransformationOperator2DInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANTRANSFORMATIONOPERATOR2D");

	___VECTOR2	localOrigin = { 0., offset };
    sdaiPutAttrBN(ifcCartesianTransformationOperator2DInstance, "LocalOrigin", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, &localOrigin));

	assert(ifcCartesianTransformationOperator2DInstance);

	return	ifcCartesianTransformationOperator2DInstance;
}
