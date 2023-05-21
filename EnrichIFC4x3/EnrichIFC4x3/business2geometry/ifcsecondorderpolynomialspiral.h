#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	SdaiInstance    ___CreatSecondOrderPolynomialSpiralInstance(
                                        SdaiModel   model,
                                        double      quadraticTerm,
                                        double      linearTerm,
                                        double      constantTerm,
                                        ___MATRIX   * matrix
                                    )
{
    SdaiInstance	ifcSecondOrderPolynomialSpiralInstance = sdaiCreateInstanceBN(model, "IFCSECONDORDERPOLYNOMIALSPIRAL");

    sdaiPutAttrBN(ifcSecondOrderPolynomialSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));

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

static	inline	double  ___SecondOrderPolynomialSpiralLengthMeasureToParameterValue(
                                double      quadraticTerm,
                                double      linearTerm,
                                double      constantTerm,
                                double      lengthMeasure
                            )
{
    assert(false);  //  parameter conversion not clear yet, currently assume 1 : 1 conversion with length
    return  lengthMeasure;	// / (std::fabs(constantTerm) * pow(Pi, 1. / 1.) + lengthMeasure / std::fabs(linearTerm) * pow(Pi, 1. / 2.) + lengthMeasure / std::fabs(quadraticTerm) * pow(Pi, 1. / 3.));
}
