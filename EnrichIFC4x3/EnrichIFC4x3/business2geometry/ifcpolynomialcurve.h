#pragma once


#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreatePolynomialCurveInstance(
                                int_t   model,
                                double  * pCoefficientsX,
                                int_t   coefficientsXCard,
                                double  * pCoefficientsY,
                                int_t   coefficientsYCard,
                                double  * pCoefficientsZ,
                                int_t   coefficientsZCard
                            )
{
    int_t	ifcPolynomialCurveInstance = sdaiCreateInstanceBN(model, "IFCPOLYNOMIALCURVE");

    sdaiPutAttrBN(ifcPolynomialCurveInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model));

    if (coefficientsXCard) {
	    int_t   * aggrCoefficientsX = sdaiCreateAggrBN(ifcPolynomialCurveInstance, "CoefficientsX");
        
        for (int_t i = 0; i < coefficientsXCard; i++) {
            sdaiAppend((int_t) aggrCoefficientsX, sdaiREAL, &pCoefficientsX[i]);
        }
    }

    if (coefficientsYCard) {
	    int_t   * aggrCoefficientsY = sdaiCreateAggrBN(ifcPolynomialCurveInstance, "CoefficientsY");
        
        for (int_t i = 0; i < coefficientsYCard; i++) {
            sdaiAppend((int_t) aggrCoefficientsY, sdaiREAL, &pCoefficientsY[i]);
        }
    }

    if (coefficientsZCard) {
	    int_t   * aggrCoefficientsZ = sdaiCreateAggrBN(ifcPolynomialCurveInstance, "CoefficientsZ");
        
        for (int_t i = 0; i < coefficientsZCard; i++) {
            sdaiAppend((int_t) aggrCoefficientsZ, sdaiREAL, &pCoefficientsZ[i]);
        }
    }

    assert(ifcPolynomialCurveInstance);

    return	ifcPolynomialCurveInstance;
}
