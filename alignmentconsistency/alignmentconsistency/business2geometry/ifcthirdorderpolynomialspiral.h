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

static	inline	double  ___ThirdOrderPolynomialSpiralLengthMeasureToParameterValue(
                                double      cubicTerm,
                                double      quadraticTerm,
                                double      linearTerm,
                                double      constantTerm,
                                double      lengthMeasure
                            )
{
    assert(false);  //  parameter conversion not clear yet, currently assume 1 : 1 conversion with length
    return  lengthMeasure;	// / (std::fabs(constantTerm) * pow(Pi, 1. / 1.) + lengthMeasure / std::fabs(linearTerm) * pow(Pi, 1. / 2.) + lengthMeasure / std::fabs(quadraticTerm) * pow(Pi, 1. / 3.) + lengthMeasure / std::fabs(cubicTerm) * pow(Pi, 1. / 4.));
}
