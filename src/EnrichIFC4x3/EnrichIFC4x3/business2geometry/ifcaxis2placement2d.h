#pragma once

#include "ifccartesianpoint.h"
#include "ifcdirection.h"


static	inline	int_t	CreateAxis2Placement2D(
								int_t   model,
								MATRIX  * matrix
							)
{
	int_t	ifcAxis2Placement2DInstance;

    ifcAxis2Placement2DInstance = sdaiCreateInstanceBN(model,(char*) "IFCAXIS2PLACEMENT2D");

	sdaiPutAttrBN(ifcAxis2Placement2DInstance, "Location", sdaiINSTANCE, (void*) CreateCartesianPoint2D(model, (VECTOR2*) &matrix->_41));
	sdaiPutAttrBN(ifcAxis2Placement2DInstance, "RefDirection", sdaiINSTANCE, (void*) CreateDirection(model, (VECTOR2*) &matrix->_11));

	assert(ifcAxis2Placement2DInstance);

	return	ifcAxis2Placement2DInstance;
}
