#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"
#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreateCosineSpiralInstance(
                                int_t       model,
                                double      constantTerm,
                                double      cosineTerm
                            )
{
    int_t	ifcCosineSpiralInstance = sdaiCreateInstanceBN(model, (char*) "IFCCOSINESPIRAL");

    sdaiPutAttrBN(ifcCosineSpiralInstance, "CosineTerm", sdaiREAL, &cosineTerm);

    if (constantTerm) {
        sdaiPutAttrBN(ifcCosineSpiralInstance, "ConstantTerm", sdaiREAL, &constantTerm);
    }

    assert(ifcCosineSpiralInstance);

    return	ifcCosineSpiralInstance;
}
