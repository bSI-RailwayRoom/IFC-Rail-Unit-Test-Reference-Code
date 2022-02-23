#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"


#include "ifcaxis2placement2d.h"
#include "ifcpolynomialcurve.h"


static	inline	int_t   ___CreateVienneseBendCant(
                                int_t       model,
                                double      * polynomialConstants,
                                int_t       polynomialConstantsCnt,
                                double      horizontalLength
                            )
{
    double  xValues[2] = { 0., horizontalLength };
    return  ___CreatePolynomialCurve__woRotation(
                    model,
                    xValues,
                    2,
                    polynomialConstants,
                    polynomialConstantsCnt,
                    nullptr,
                    0
                );
}

static	inline	int_t   ___CreateVienneseBend(
                                int_t       model,
                                double      septicTerm,
                                double      sexticTerm,
                                double      quinticTerm,
                                double      quarticTerm,
                                double      cubicTerm,
                                double      quadraticTerm,
                                double      linearTerm,
                                double      constantTerm
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

    sdaiPutAttrBN(ifcSeventhOrderPolynomialSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2D(model));

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