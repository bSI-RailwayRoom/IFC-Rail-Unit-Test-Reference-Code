#pragma once

#include <cmath>

#include "clothoid.h"
#include "ifcaxis2placement2d.h"


int_t   CreateClothoid(
                int_t   model,
                double  startRadiusOfCurvature,
                double  endRadiusOfCurvature,
                double  segmentLength,
                double  * pOffset
            )
{
    int_t	ifcClothoidInstance = sdaiCreateInstanceBN(model, (char*) "IFCCLOTHOID");

    MATRIX  matrix;
    MatrixIdentity(&matrix);

    double  A;

    if (startRadiusOfCurvature == 0.) {
        assert(endRadiusOfCurvature);
        A = std::sqrt(std::fabs(endRadiusOfCurvature) * segmentLength);
        if (endRadiusOfCurvature < 0) { A = -A; }

        assert(*pOffset == 0.);
    }
    else if (endRadiusOfCurvature == 0.) {
        A = std::sqrt(std::fabs(startRadiusOfCurvature) * segmentLength);
        if (startRadiusOfCurvature > 0) { A = -A; }

        (*pOffset) = -segmentLength;
    }
    else if (startRadiusOfCurvature < 0. && endRadiusOfCurvature < 0.) {
        if (startRadiusOfCurvature < endRadiusOfCurvature) {
            //  Calculate clothoidConstant
            //      endRadius * (len + offset) = startRadius * offset
            //      startRadius * offset - endRadius * offset = endRadius * len
            //      offset = (endRadius * len) / (startRadius - endRadius)
            double  offsetLength = (endRadiusOfCurvature * segmentLength) / (startRadiusOfCurvature - endRadiusOfCurvature);
            A = std::sqrt(std::fabs(endRadiusOfCurvature) * (segmentLength + offsetLength));
            assert(A == std::sqrt(std::fabs(startRadiusOfCurvature) * offsetLength));

            A = -A;

            (*pOffset) = offsetLength;
        }
        else {
            assert(startRadiusOfCurvature > endRadiusOfCurvature);
            //  Calculate clothoidConstant
            //      startRadius * (len + offset) = endRadius * offset
            //      endRadius * offset - startRadius * offset = startRadius * len
            //      offset = (startRadius * len) / (endRadius - startRadius)
            double  offsetLength = (startRadiusOfCurvature * segmentLength) / (endRadiusOfCurvature - startRadiusOfCurvature);
            A = std::sqrt(std::fabs(startRadiusOfCurvature) * (segmentLength + offsetLength));
            assert(A == std::sqrt(std::fabs(endRadiusOfCurvature) * offsetLength));

            (*pOffset) = -(segmentLength + offsetLength);
        }
    }
    else if (startRadiusOfCurvature > 0. && endRadiusOfCurvature > 0.) {
        if (startRadiusOfCurvature < endRadiusOfCurvature) {
            //  Calculate clothoidConstant
            //      startRadius * (len + offset) = endRadius * offset
            //      endRadius * offset - startRadius * offset = startRadius * len
            //      offset = (startRadius * len) / (endRadius - startRadius)
            double  offsetLength = (startRadiusOfCurvature * segmentLength) / (endRadiusOfCurvature - startRadiusOfCurvature);
            A = std::sqrt(std::fabs(startRadiusOfCurvature) * (segmentLength + offsetLength));
            assert(A == std::sqrt(std::fabs(endRadiusOfCurvature) * offsetLength));

            A = -A;

            (*pOffset) = -(segmentLength + offsetLength);
        }
        else {
            assert(startRadiusOfCurvature > endRadiusOfCurvature);
            //  Calculate clothoidConstant
            //      endRadius * (len + offset) = startRadius * offset
            //      startRadius * offset - endRadius * offset = endRadius * len
            //      offset = (endRadius * len) / (startRadius - endRadius)
            double  offsetLength = (endRadiusOfCurvature * segmentLength) / (startRadiusOfCurvature - endRadiusOfCurvature);
            A = std::sqrt(std::fabs(endRadiusOfCurvature) * (segmentLength + offsetLength));
            assert(A == std::sqrt(std::fabs(startRadiusOfCurvature) * offsetLength));

            (*pOffset) = offsetLength;
        }
    }
    else {
        assert(false);
        A = 1.;
        assert(*pOffset == 0.);
    }

    if ((*pOffset)) {
        CalculateClothoidDirection(
                -A,
                -(*pOffset),
                (VECTOR3*) &matrix._11
            );

        Vec3Cross((VECTOR3*) &matrix._21, (VECTOR3*) &matrix._31, (VECTOR3*) &matrix._11);

        VECTOR3 pos;
        CalculateClothoidPosition(
                A,
                -(*pOffset),
                (VECTOR3*) &pos
            );

        Vec3Transform((VECTOR3*) &matrix._41, (VECTOR3*) &pos, &matrix);
    }

    sdaiPutAttrBN(ifcClothoidInstance, "Position", sdaiINSTANCE, (void*) CreateAxis2Placement2D(model, &matrix));

    sdaiPutAttrBN(ifcClothoidInstance, "ClothoidConstant", sdaiREAL, &A);

    assert(ifcClothoidInstance);

    return	ifcClothoidInstance;
}
