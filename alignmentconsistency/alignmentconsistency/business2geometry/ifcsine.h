#pragma once


#include "ifccartesianpoint.h"
#include "ifcvector.h"


#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreateSineCurve__NESTED(
                                int_t       model,
                                double      sineTerm,
                                double      linearTerm,
                                double      constantTerm
                            )
{
    int_t	ifcSineSpiralInstance = sdaiCreateInstanceBN(model, (char*) "IFCSINESPIRAL");

//    sdaiPutAttrBN(ifcSineSpiralInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2D(model));

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

static	inline	int_t   ___CreateSineCurve(
                                int_t       model,
                                double      sineTerm,
                                double      constantTerm,
                                double      segmentLength
                            )
{
    if (sineTerm) { sineTerm = 1. / sineTerm; }

    double  linearTerm = 1. * sineTerm;
//    linearTerm = std::fabs(segmentLength) * pow(std::fabs(linearTerm), -1. / 1.) * linearTerm / std::fabs(linearTerm);
    linearTerm = (linearTerm) ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * linearTerm), -1. / 2.) * linearTerm / std::fabs(linearTerm) : 0.;

//    sineTerm *= -1. / (2. * Pi);
    sineTerm = -(2. * Pi) / sineTerm;

//    double  aB = a ? segmentLength * pow(std::fabs(segmentLength * a), -1. / 4.) * a / std::fabs(a) : 0.;
//    double  bB = b ? segmentLength * pow(std::fabs(segmentLength * b), -1. / 3.) * b / std::fabs(b) : 0.;
//    double  cB = c ? segmentLength * pow(std::fabs(segmentLength * c), -1. / 2.) * c / std::fabs(c) : 0.;
//    double  dB = d ? segmentLength * pow(std::fabs(segmentLength * d), -1. / 1.) * d / std::fabs(d) : 0.;

    return  ___CreateSineCurve__NESTED(
                    model,
                    sineTerm,
                    linearTerm,
                    constantTerm ? 1. / constantTerm : 0.
                );
}
