#pragma once


#include "ifccartesianpoint.h"
#include "ifcdirection.h"


static	inline	SdaiInstance	___CreateAxis2Placement3DInstance(
										SdaiModel	model
									)
{
	SdaiInstance	ifcAxis2Placement3DInstance = sdaiCreateInstanceBN(model, "IFCAXIS2PLACEMENT3D");

	___VECTOR3	vecLocation = { 0., 0., 0. };
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Location", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, &vecLocation));

	assert(ifcAxis2Placement3DInstance);

	return	ifcAxis2Placement3DInstance;
}

static	inline	SdaiInstance	___CreateAxis2Placement3DInstance(
										SdaiModel	model,
										___MATRIX	* matrix
									)
{
	SdaiInstance	ifcAxis2Placement3DInstance = sdaiCreateInstanceBN(model, "IFCAXIS2PLACEMENT3D");

	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Location", sdaiINSTANCE, (void*) ___CreateCartesianPointInstance(model, (___VECTOR3*) &matrix->_41));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Axis", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, (___VECTOR3*) &matrix->_31));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "RefDirection", sdaiINSTANCE, (void* ) ___CreateDirectionInstance(model, (___VECTOR3*) &matrix->_11));

	assert(ifcAxis2Placement3DInstance);

	return	ifcAxis2Placement3DInstance;
}
