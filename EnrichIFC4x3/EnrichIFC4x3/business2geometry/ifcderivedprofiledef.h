#pragma once


#include "ifccartesiantransformationoperator2d.h"


static	inline	int_t	___CreateDerivedProfileDef(
							    int_t   model,
                                int_t   ifcProfileDefInstance,
                                double  offset
							)
{
	int_t	ifcDerivedProfileDef;

    ifcDerivedProfileDef = sdaiCreateInstanceBN(model, "IFCDERIVEDPROFILEDEF");

    sdaiPutAttrBN(ifcDerivedProfileDef, "ParentProfile", sdaiINSTANCE, (void*) ifcProfileDefInstance);
    sdaiPutAttrBN(ifcDerivedProfileDef, "Operator", sdaiINSTANCE, (void*) ___CreateCartesianTransformationOperator2DInstance(model, offset));

	assert(ifcDerivedProfileDef);

	return	ifcDerivedProfileDef;
}
