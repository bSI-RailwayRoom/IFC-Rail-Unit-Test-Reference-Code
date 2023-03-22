#pragma once


#include "ifccartesianpoint.h"
#include "ifcdirection.h"


static	inline	SdaiInstance	___CreateAxis2Placement2DInstance(
										SdaiModel	model
									)
{
	SdaiInstance	ifcAxis2Placement2DInstance = sdaiCreateInstanceBN(model, "IFCAXIS2PLACEMENT2D");

	___VECTOR2	vecLocation = { 0., 0. };
	sdaiPutAttrBN(ifcAxis2Placement2DInstance, "Location", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, (___VECTOR2*) &vecLocation));

	assert(ifcAxis2Placement2DInstance);

	return	ifcAxis2Placement2DInstance;
}

static	inline	SdaiInstance	___CreateAxis2Placement2DInstance(
										SdaiModel	model,
										___VECTOR2	* location,
										___VECTOR2	* refDirection
									)
{
	SdaiInstance	ifcAxis2Placement2DInstance = sdaiCreateInstanceBN(model,(char*) "IFCAXIS2PLACEMENT2D");

	sdaiPutAttrBN(ifcAxis2Placement2DInstance, "Location", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, location));
	sdaiPutAttrBN(ifcAxis2Placement2DInstance, "RefDirection", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, refDirection));

	assert(ifcAxis2Placement2DInstance);

	return	ifcAxis2Placement2DInstance;
}

static	inline	SdaiInstance	___CreateAxis2Placement2DInstance(
										SdaiModel	model,
										___MATRIX  * matrix
									)
{
	if (matrix) {
		return	___CreateAxis2Placement2DInstance(model, (___VECTOR2*) &matrix->_41, (___VECTOR2*) &matrix->_11);
	}
	else {
		return	___CreateAxis2Placement2DInstance(model);
	}
}
