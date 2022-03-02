#pragma once


#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreatePolynomialCurve__woRotation(
                                int_t   model,
                                double  * pCoefficientsX,
                                int_t   coefficientsXCard,
                                double  * pCoefficientsY,
                                int_t   coefficientsYCard,
                                double  * pCoefficientsZ,
                                int_t   coefficientsZCard
                            )
{
    int_t	ifcPolynomialCurveInstance = sdaiCreateInstanceBN(model, (char*) "IFCPOLYNOMIALCURVE");

    sdaiPutAttrBN(ifcPolynomialCurveInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2D(model));

    if (coefficientsXCard) {
	    int_t   * aggrCoefficientsX = sdaiCreateAggrBN(ifcPolynomialCurveInstance, (char*) "CoefficientsX");
        
        for (int_t i = 0; i < coefficientsXCard; i++) {
            sdaiAppend((int_t) aggrCoefficientsX, sdaiREAL, &pCoefficientsX[i]);
        }
    }

    if (coefficientsYCard) {
	    int_t   * aggrCoefficientsY = sdaiCreateAggrBN(ifcPolynomialCurveInstance, (char*) "CoefficientsY");
        
        for (int_t i = 0; i < coefficientsYCard; i++) {
            sdaiAppend((int_t) aggrCoefficientsY, sdaiREAL, &pCoefficientsY[i]);
        }
    }

    if (coefficientsZCard) {
	    int_t   * aggrCoefficientsZ = sdaiCreateAggrBN(ifcPolynomialCurveInstance, (char*) "CoefficientsZ");
        
        for (int_t i = 0; i < coefficientsZCard; i++) {
            sdaiAppend((int_t) aggrCoefficientsZ, sdaiREAL, &pCoefficientsZ[i]);
        }
    }

    assert(ifcPolynomialCurveInstance);
    return	ifcPolynomialCurveInstance;
}

static	inline	int_t   ___CreateBiquadraticParabola(
                                int_t       model,
                                double      polynomialConstantsCant_III,
                                double      polynomialConstantsCant_II,
                                double      polynomialConstantsCant_I
                            )
{
    int_t	ifcPolynomialCurveInstance = sdaiCreateInstanceBN(model, (char*) "IFCPOLYNOMIALCURVE");

//    sdaiPutAttrBN(ifcPolynomialCurveInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2D(model));
    
    {
	    int_t   * aggrCoefficientsX = sdaiCreateAggrBN(ifcPolynomialCurveInstance, (char*) "CoefficientsX");
        
        double  zero = 0.,
                linearConstant = 1.;
        sdaiAppend((int_t) aggrCoefficientsX, sdaiREAL, &zero);
        sdaiAppend((int_t) aggrCoefficientsX, sdaiREAL, &linearConstant);
    }

    {
	    int_t   * aggrCoefficientsY = sdaiCreateAggrBN(ifcPolynomialCurveInstance, (char*) "CoefficientsY");
        
        double  zero = 0.;
        sdaiAppend((int_t) aggrCoefficientsY, sdaiREAL, &polynomialConstantsCant_I);
        sdaiAppend((int_t) aggrCoefficientsY, sdaiREAL, &polynomialConstantsCant_II);
        sdaiAppend((int_t) aggrCoefficientsY, sdaiREAL, &polynomialConstantsCant_III);
    }

    assert(ifcPolynomialCurveInstance);

    return	ifcPolynomialCurveInstance;
}

static	inline	int_t   ___CreateCubic(
                                int_t       model,
                                double      cubicConstant
                            )
{
    int_t	ifcPolynomialCurveInstance = sdaiCreateInstanceBN(model, (char*) "IFCPOLYNOMIALCURVE");

//    sdaiPutAttrBN(ifcPolynomialCurveInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2D(model));
    
    {
	    int_t   * aggrCoefficientsX = sdaiCreateAggrBN(ifcPolynomialCurveInstance, (char*) "CoefficientsX");
        
        double  zero = 0.,
                linearConstant = 1.;
        sdaiAppend((int_t) aggrCoefficientsX, sdaiREAL, &zero);
        sdaiAppend((int_t) aggrCoefficientsX, sdaiREAL, &linearConstant);
    }

    {
	    int_t   * aggrCoefficientsY = sdaiCreateAggrBN(ifcPolynomialCurveInstance, (char*) "CoefficientsY");
        
        double  zero = 0.;
        sdaiAppend((int_t) aggrCoefficientsY, sdaiREAL, &zero);
        sdaiAppend((int_t) aggrCoefficientsY, sdaiREAL, &zero);
        sdaiAppend((int_t) aggrCoefficientsY, sdaiREAL, &zero);
        sdaiAppend((int_t) aggrCoefficientsY, sdaiREAL, &cubicConstant);
    }

    assert(ifcPolynomialCurveInstance);

    return	ifcPolynomialCurveInstance;
}
