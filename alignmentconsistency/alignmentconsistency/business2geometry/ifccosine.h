#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"


#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreateCosineCurve__NESTED(
                                int_t       model,
                                double      cosineTerm,
                                double      constantTerm
                            )
{
    int_t	ifcCosineSpiralInstance = sdaiCreateInstanceBN(model, (char*) "IFCCOSINESPIRAL");

//    sdaiPutAttrBN(ifcCosineSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2D(model));

    sdaiPutAttrBN(ifcCosineSpiralInstance, "CosineTerm", sdaiREAL, &cosineTerm);

    if (constantTerm) {
        sdaiPutAttrBN(ifcCosineSpiralInstance, "ConstantTerm", sdaiREAL, &constantTerm);
    }

    assert(ifcCosineSpiralInstance);

    return	ifcCosineSpiralInstance;
}

static	inline	int_t   ___CreateCosineCurve(
                                int_t       model,
                                double      cosineTerm,
                                double      constantTerm
                            )
{
    if (cosineTerm) { cosineTerm = 1. / cosineTerm; }

    constantTerm += cosineTerm / 2.;
    cosineTerm *= -1. / 2.;

//    double  aB = a ? segmentLength * pow(std::fabs(segmentLength * a), -1. / 4.) * a / std::fabs(a) : 0.;
//    double  bB = b ? segmentLength * pow(std::fabs(segmentLength * b), -1. / 3.) * b / std::fabs(b) : 0.;
//    double  cB = c ? segmentLength * pow(std::fabs(segmentLength * c), -1. / 2.) * c / std::fabs(c) : 0.;
//    double  dB = d ? segmentLength * pow(std::fabs(segmentLength * d), -1. / 1.) * d / std::fabs(d) : 0.;

    return  ___CreateCosineCurve__NESTED(
                    model,
                    cosineTerm ? 1. / cosineTerm : 0.,
                    constantTerm ? 1. / constantTerm : 0.
                );
}
