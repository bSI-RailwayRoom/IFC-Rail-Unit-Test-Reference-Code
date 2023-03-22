#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	SdaiInstance    ___CreateThirdOrderPolynomialSpiralInstance(
                                        SdaiModel   model,
                                        double      cubicTerm,
                                        double      quadraticTerm,
                                        double      linearTerm,
                                        double      constantTerm,
                                        ___MATRIX   * matrix
                                    )
{
    SdaiInstance	ifcThirdOrderPolynomialSpiralInstance = sdaiCreateInstanceBN(model, "IFCTHIRDORDERPOLYNOMIALSPIRAL");

    sdaiPutAttrBN(ifcThirdOrderPolynomialSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));

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
