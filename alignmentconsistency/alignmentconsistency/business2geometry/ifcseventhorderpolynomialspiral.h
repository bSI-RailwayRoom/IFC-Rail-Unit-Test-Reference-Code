#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"
#include "ifcpolynomialcurve.h"


static	inline	int_t   ___CreateSeventhOrderPolynomialSpiralInstance(
                                int_t       model,
                                double      septicTerm,
                                double      sexticTerm,
                                double      quinticTerm,
                                double      quarticTerm,
                                double      cubicTerm,
                                double      quadraticTerm,
                                double      linearTerm,
                                double      constantTerm,
                                ___MATRIX   * matrix
                            )
{
    //		SepticTerm
    //		SexticTerm
    //		QuinticTerm
    //		QuarticTerm
    //		CubicTerm
    //		QuadraticTerm
    //		LinearTerm
    //		ConstantTerm

    int_t	ifcSeventhOrderPolynomialSpiralInstance = sdaiCreateInstanceBN(model, (char*) "IFCSEVENTHORDERPOLYNOMIALSPIRAL");

    if (matrix) {
        sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));
    }

    sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "SepticTerm", sdaiREAL, &septicTerm);

    if (sexticTerm) {
        sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "SexticTerm", sdaiREAL, &sexticTerm);
    }

    if (quinticTerm) {
        sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "QuinticTerm", sdaiREAL, &quinticTerm);
    }

    if (quarticTerm) {
        sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "QuarticTerm", sdaiREAL, &quarticTerm);
    }

    if (cubicTerm) {
        sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "CubicTerm", sdaiREAL, &cubicTerm);
    }

    if (quadraticTerm) {
        sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "QuadraticTerm", sdaiREAL, &quadraticTerm);
    }

    if (linearTerm) {
        sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "LinearTerm", sdaiREAL, &linearTerm);
    }

    if (constantTerm) {
        sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "ConstantTerm", sdaiREAL, &constantTerm);
    }

    assert(ifcSeventhOrderPolynomialSpiralInstance);

    return	ifcSeventhOrderPolynomialSpiralInstance;
}
