#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	SdaiInstance    ___CreateSineSpiralInstance(
                                        SdaiModel   model,
                                        double      linearTerm,
                                        double      constantTerm,
                                        double      sineTerm,
                                        ___MATRIX   * matrix
                                    )
{
    SdaiInstance	ifcSineSpiralInstance = sdaiCreateInstanceBN(model, "IFCSINESPIRAL");

    sdaiPutAttrBN(ifcSineSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));

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

static	inline	double  ___SineSpiralLengthMeasureToParameterValue(
                                double      linearTerm,
                                double      constantTerm,
                                double      sineTerm,
                                double      lengthMeasure
                            )
{
    assert(false);  //  parameter conversion not clear yet, currently assume 1 : 1 conversion with length
    return  lengthMeasure;
}
