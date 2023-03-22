#pragma once


#include "ifcaxis2placement2d.h"


static	inline	SdaiInstance    ___CreatePolynomialCurveInstance(
                                        SdaiModel   model,
                                        double      * pCoefficientsX,
                                        int_t       coefficientsXCard,
                                        double      * pCoefficientsY,
                                        int_t       coefficientsYCard,
                                        double      * pCoefficientsZ,
                                        int_t       coefficientsZCard
                                    )
{
    SdaiInstance    ifcPolynomialCurveInstance = sdaiCreateInstanceBN(model, "IFCPOLYNOMIALCURVE");

    sdaiPutAttrBN(ifcPolynomialCurveInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model));

    if (coefficientsXCard) {
	    SdaiAggr    aggrCoefficientsX = sdaiCreateAggrBN(ifcPolynomialCurveInstance, "CoefficientsX");
        
        for (int_t i = 0; i < coefficientsXCard; i++) {
            sdaiAppend(aggrCoefficientsX, sdaiREAL, &pCoefficientsX[i]);
        }
    }

    if (coefficientsYCard) {
        SdaiAggr    aggrCoefficientsY = sdaiCreateAggrBN(ifcPolynomialCurveInstance, "CoefficientsY");
        
        for (int_t i = 0; i < coefficientsYCard; i++) {
            sdaiAppend(aggrCoefficientsY, sdaiREAL, &pCoefficientsY[i]);
        }
    }

    if (coefficientsZCard) {
        SdaiAggr    aggrCoefficientsZ = sdaiCreateAggrBN(ifcPolynomialCurveInstance, "CoefficientsZ");
        
        for (int_t i = 0; i < coefficientsZCard; i++) {
            sdaiAppend(aggrCoefficientsZ, sdaiREAL, &pCoefficientsZ[i]);
        }
    }

    assert(ifcPolynomialCurveInstance);

    return	ifcPolynomialCurveInstance;
}
