#pragma once


#include "ifccartesiantransformationoperator2d.h"


static	inline	SdaiInstance	___CreateDerivedProfileDefInstance(
										SdaiModel		model,
										SdaiInstance	ifcProfileDefInstance,
										double			offset
									)
{
	SdaiInstance	ifcDerivedProfileDef = sdaiCreateInstanceBN(model, "IFCDERIVEDPROFILEDEF");

    sdaiPutAttrBN(ifcDerivedProfileDef, "ParentProfile", sdaiINSTANCE, (void*) ifcProfileDefInstance);
    sdaiPutAttrBN(ifcDerivedProfileDef, "Operator", sdaiINSTANCE, (void*) ___CreateCartesianTransformationOperator2DInstance(model, offset));

	assert(ifcDerivedProfileDef);

	return	ifcDerivedProfileDef;
}
