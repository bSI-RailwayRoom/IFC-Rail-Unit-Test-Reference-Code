#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	SdaiInstance    ___CreateCosineSpiralInstance(
                                        SdaiModel   model,
                                        double      constantTerm,
                                        double      cosineTerm,
                                        ___MATRIX   * matrix
                                    )
{
    SdaiInstance	ifcCosineSpiralInstance = sdaiCreateInstanceBN(model, "IFCCOSINESPIRAL");

    sdaiPutAttrBN(ifcCosineSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));

    sdaiPutAttrBN(ifcCosineSpiralInstance, "CosineTerm", sdaiREAL, &cosineTerm);

    if (constantTerm) {
        sdaiPutAttrBN(ifcCosineSpiralInstance, "ConstantTerm", sdaiREAL, &constantTerm);
    }

    assert(ifcCosineSpiralInstance);

    return	ifcCosineSpiralInstance;
}

static	inline	double  ___CosineSpiralLengthMeasureToParameterValue(
                                double      constantTerm,
                                double      cosineTerm,
                                double      lengthMeasure
                            )
{
    assert(false);  //  parameter conversion not clear yet, currently assume 1 : 1 conversion with length
    return  lengthMeasure;
}
