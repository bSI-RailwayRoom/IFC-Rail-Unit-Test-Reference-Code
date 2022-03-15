#pragma once


#include "ifccartesianpoint.h"
#include "ifcdirection.h"


static	inline	int_t	___CreateAxis2Placement3DInstance(
								int_t		model
							)
{
	int_t	ifcAxis2Placement3DInstance;

    ifcAxis2Placement3DInstance = sdaiCreateInstanceBN(model,(char*) "IFCAXIS2PLACEMENT3D");

	___VECTOR3	vecLocation = { 0., 0., 0. };
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Location", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, &vecLocation));

	assert(ifcAxis2Placement3DInstance);

	return	ifcAxis2Placement3DInstance;
}

static	inline	int_t	___CreateAxis2Placement3DInstance(
								int_t		model,
								___MATRIX	* matrix
							)
{
	int_t	ifcAxis2Placement3DInstance;

    ifcAxis2Placement3DInstance = sdaiCreateInstanceBN(model,(char*) "IFCAXIS2PLACEMENT3D");

	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Location", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, (___VECTOR3*) &matrix->_41));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Axis", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, (___VECTOR3*) &matrix->_31));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "RefDirection", sdaiINSTANCE, (void* ) ___CreateDirectionInstance(model, (___VECTOR3*) &matrix->_11));

	assert(ifcAxis2Placement3DInstance);

	return	ifcAxis2Placement3DInstance;
}
