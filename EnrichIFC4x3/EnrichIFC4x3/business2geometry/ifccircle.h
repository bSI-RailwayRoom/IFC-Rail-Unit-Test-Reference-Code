#pragma once


#include "ifcaxis2placement2d.h"


static	inline	int_t   ___CreateCircleInstance(
                                int_t       model,
                                double      radiusOfCurvature,
                                ___MATRIX   * matrix
                            )
{
    int_t	ifcCircleInstance = sdaiCreateInstanceBN(model, (char*) "IFCCIRCLE");

    sdaiPutAttrBN(ifcCircleInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, matrix));

    double  radius = std::fabs(radiusOfCurvature);
    sdaiPutAttrBN(ifcCircleInstance, "Radius", sdaiREAL, &radius);

    assert(ifcCircleInstance);
    return	ifcCircleInstance;
}

static	inline	int_t   ___CreateCircleInstance(
                                int_t       model,
                                double      radiusOfCurvature
                            )
{
    int_t	ifcCircleInstance = sdaiCreateInstanceBN(model, (char*) "IFCCIRCLE");

    ___MATRIX  matrix;
    ___MatrixIdentity(&matrix);

    if (radiusOfCurvature > 0.) {
        matrix._11 = 0.;
        matrix._12 = -1.;
        assert(matrix._13 == 0.);
    }
    else {
        matrix._11 = 0.;
        matrix._12 = 1.;
        assert(matrix._13 == 0.);
    }

    matrix._42 = radiusOfCurvature;
    sdaiPutAttrBN(ifcCircleInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &matrix));

    double  radius = std::fabs(radiusOfCurvature);
    sdaiPutAttrBN(ifcCircleInstance, "Radius", sdaiREAL, &radius);

    assert(ifcCircleInstance);
    return	ifcCircleInstance;
}

static	inline	int_t   ___CreateCircleInstance(
                                int_t       model,
                                ___VECTOR2  * origin,
                                double      radius
                            )
{
    int_t	ifcCircleInstance = sdaiCreateInstanceBN(model, (char*) "IFCCIRCLE");

    ___MATRIX  matrix;
    ___MatrixIdentity(&matrix);
    matrix._41 = origin->x;
    matrix._42 = origin->y;

    sdaiPutAttrBN(ifcCircleInstance, "Position", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &matrix));

    sdaiPutAttrBN(ifcCircleInstance, "Radius", sdaiREAL, &radius);

    assert(ifcCircleInstance);
    return	ifcCircleInstance;
}
