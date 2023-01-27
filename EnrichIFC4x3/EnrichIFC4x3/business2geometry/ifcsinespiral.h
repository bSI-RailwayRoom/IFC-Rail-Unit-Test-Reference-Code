#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreateSineSpiralInstance(
                                int_t       model,
                                double      linearTerm,
                                double      constantTerm,
                                double      sineTerm,
                                ___MATRIX   * matrix
                            )
{
    int_t	ifcSineSpiralInstance = sdaiCreateInstanceBN(model, "IFCSINESPIRAL");

    if (matrix) {
        sdaiPutAttrBN(ifcSineSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));
    }

    sdaiPutAttrBN(ifcSineSpiralInstance, "SineTerm", sdaiREAL, &sineTerm);

    if (linearTerm) {
        sdaiPutAttrBN(ifcSineSpiralInstance, "LinearTerm", sdaiREAL, &linearTerm);
    }

    if (constantTerm) {
        sdaiPutAttrBN(ifcSineSpiralInstance, "ConstantTerm", sdaiREAL, &constantTerm);
    }

    assert(ifcSineSpiralInstance);

    return	ifcSineSpiralInstance;
}
