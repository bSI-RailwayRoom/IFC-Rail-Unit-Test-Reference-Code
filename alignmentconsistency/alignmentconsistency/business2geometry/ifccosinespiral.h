#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreateCosineSpiralInstance(
                                int_t       model,
                                double      constantTerm,
                                double      cosineTerm,
                                ___MATRIX   * matrix
                            )
{
    int_t	ifcCosineSpiralInstance = sdaiCreateInstanceBN(model, "IFCCOSINESPIRAL");

    if (matrix) {
        sdaiPutAttrBN(ifcCosineSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));
    }

    sdaiPutAttrBN(ifcCosineSpiralInstance, "CosineTerm", sdaiREAL, &cosineTerm);

    if (constantTerm) {
        sdaiPutAttrBN(ifcCosineSpiralInstance, "ConstantTerm", sdaiREAL, &constantTerm);
    }

    assert(ifcCosineSpiralInstance);

    return	ifcCosineSpiralInstance;
}
