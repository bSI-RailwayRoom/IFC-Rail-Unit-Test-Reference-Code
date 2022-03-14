#pragma once


#include "ifccartesianpoint.h"
#include "ifcdirection.h"


static	inline	int_t	___CreateAxis2Placement2D(
								int_t		model
							)
{
	int_t	ifcAxis2Placement2DInstance;

    ifcAxis2Placement2DInstance = sdaiCreateInstanceBN(model,(char*) "IFCAXIS2PLACEMENT2D");

	___VECTOR2	vecLocation = { 0., 0. };
	sdaiPutAttrBN(ifcAxis2Placement2DInstance, "Location", sdaiINSTANCE, (void*) ___CreateCartesianPoint2D(model, (___VECTOR2*) &vecLocation));

	assert(ifcAxis2Placement2DInstance);

	return	ifcAxis2Placement2DInstance;
}

static	inline	int_t	___CreateAxis2Placement2D(
								int_t		model,
								___VECTOR2	* location,
								___VECTOR2	* refDirection
							)
{
	int_t	ifcAxis2Placement2DInstance;

    ifcAxis2Placement2DInstance = sdaiCreateInstanceBN(model,(char*) "IFCAXIS2PLACEMENT2D");

	sdaiPutAttrBN(ifcAxis2Placement2DInstance, "Location", sdaiINSTANCE, (void*) ___CreateCartesianPoint2D(model, location));
	sdaiPutAttrBN(ifcAxis2Placement2DInstance, "RefDirection", sdaiINSTANCE, (void*) ___CreateDirection_2D(model, refDirection));

	assert(ifcAxis2Placement2DInstance);

	return	ifcAxis2Placement2DInstance;
}

static	inline	int_t	___CreateAxis2Placement2D(
								int_t		model,
								___MATRIX  * matrix
							)
{
	if (matrix) {
		return	___CreateAxis2Placement2D(model, (___VECTOR2*) &matrix->_41, (___VECTOR2*) &matrix->_11);
	}
	else {
		return	___CreateAxis2Placement2D(model);
	}
}
