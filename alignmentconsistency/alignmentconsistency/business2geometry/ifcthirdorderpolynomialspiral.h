#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"

#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreateBlossCurve(
                                int_t       model,
                                double      cubicTerm,
                                double      quadraticTerm,
                                double      linearTerm,
                                double      constantTerm
                            )
{
    int_t	ifcThirdOrderPolynomialSpiralInstance = sdaiCreateInstanceBN(model, (char*) "IFCTHIRDORDERPOLYNOMIALSPIRAL");

    sdaiPutAttrBN(ifcThirdOrderPolynomialSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2D(model));

    sdaiPutAttrBN(ifcThirdOrderPolynomialSpiralInstance, "CubicTerm", sdaiREAL, &cubicTerm);

    if (quadraticTerm) {
        sdaiPutAttrBN(ifcThirdOrderPolynomialSpiralInstance, "QuadraticTerm", sdaiREAL, &quadraticTerm);
    }

    if (linearTerm) {
        sdaiPutAttrBN(ifcThirdOrderPolynomialSpiralInstance, "LinearTerm", sdaiREAL, &linearTerm);
    }

    if (constantTerm) {
        sdaiPutAttrBN(ifcThirdOrderPolynomialSpiralInstance, "ConstantTerm", sdaiREAL, &constantTerm);
    }

    assert(ifcThirdOrderPolynomialSpiralInstance);

    return	ifcThirdOrderPolynomialSpiralInstance;
}