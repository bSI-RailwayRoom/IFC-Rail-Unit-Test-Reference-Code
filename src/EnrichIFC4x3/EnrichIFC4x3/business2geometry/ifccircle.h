#pragma once

#include "ifcaxis2placement2d.h"


static	inline	int_t   CreateCircle(
                                int_t   model,
                                double  radiusOfCurvature
                            )
{
    int_t	ifcCircleInstance = sdaiCreateInstanceBN(model, (char*) "IFCCIRCLE");

    MATRIX  matrix;
    MatrixIdentity(&matrix);

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
    sdaiPutAttrBN(ifcCircleInstance, "Position", sdaiINSTANCE, (void*) CreateAxis2Placement2D(model, &matrix));

    double  radius = std::fabs(radiusOfCurvature);
    sdaiPutAttrBN(ifcCircleInstance, "Radius", sdaiREAL, &radius);

    assert(ifcCircleInstance);
    return	ifcCircleInstance;
}

static	inline	int_t   CreateCircle(
                                int_t   model,
                                VECTOR2 * origin,
                                double  radius
                            )
{
    int_t	ifcCircleInstance = sdaiCreateInstanceBN(model, (char*) "IFCCIRCLE");

    MATRIX  matrix;
    MatrixIdentity(&matrix);
    matrix._41 = origin->x;
    matrix._42 = origin->y;

    sdaiPutAttrBN(ifcCircleInstance, "Position", sdaiINSTANCE, (void*) CreateAxis2Placement2D(model, &matrix));

    sdaiPutAttrBN(ifcCircleInstance, "Radius", sdaiREAL, &radius);

    assert(ifcCircleInstance);
    return	ifcCircleInstance;
}
