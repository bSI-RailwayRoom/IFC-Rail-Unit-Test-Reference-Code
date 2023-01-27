#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreatSecondOrderPolynomialSpiralInstance(
                                int_t       model,
                                double      quadraticTerm,
                                double      linearTerm,
                                double      constantTerm,
                                ___MATRIX   * matrix
                            )
{
    int_t	ifcSecondOrderPolynomialSpiralInstance = sdaiCreateInstanceBN(model, "IFCSECONDORDERPOLYNOMIALSPIRAL");

    if (matrix) {
        sdaiPutAttrBN(ifcSecondOrderPolynomialSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));
    }

    sdaiPutAttrBN(ifcSecondOrderPolynomialSpiralInstance, "QuadraticTerm", sdaiREAL, &quadraticTerm);

    if (linearTerm) {
        sdaiPutAttrBN(ifcSecondOrderPolynomialSpiralInstance, "LinearTerm", sdaiREAL, &linearTerm);
    }
    
    if (constantTerm) {
        sdaiPutAttrBN(ifcSecondOrderPolynomialSpiralInstance, "ConstantTerm", sdaiREAL, &constantTerm);
    }

    assert(ifcSecondOrderPolynomialSpiralInstance);

    return	ifcSecondOrderPolynomialSpiralInstance;
}
