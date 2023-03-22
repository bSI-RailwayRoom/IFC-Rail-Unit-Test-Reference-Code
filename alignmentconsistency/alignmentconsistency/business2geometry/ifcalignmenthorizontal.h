#pragma once

#include <stdio.h>
#include <string.h>

#include "generic.h"
#include "spiral.h"
#include "ifccircle.h"
#include "ifcclothoid.h"
#include "ifccosinespiral.h"
#include "ifcline.h"
#include "ifcpolynomialcurve.h"
#include "ifcproductdefinitionshape.h"
#include "ifcsinespiral.h"
#include "ifcsecondorderpolynomialspiral.h"
#include "ifcseventhorderpolynomialspiral.h"
#include "ifcthirdorderpolynomialspiral.h"


static	const	double		___Pi = 3.14159265358979323846;

#ifdef _DEBUG
extern  int_t   horizontalAlignmentParentCurveI, horizontalAlignmentParentCurveII;
#endif // _DEBUG


inline  static  SdaiInstance     ___GetAlignmentCant(
                                        SdaiModel       model,
                                        SdaiInstance    ifcAlignmentInstance,
                                        bool            * hasIssue
                                    );

static  inline  double   GetCantStart(
                                SdaiModel       model,
                                SdaiInstance    ifcAlignmentInstance,
                                double          offset,
                                double          length,
                                double          * pStartCantRight,
                                double          * pStartCantLeft,
                                double          * pRailHeadDistance
                            )
{
    (*pStartCantRight)   = 0.;
    (*pStartCantLeft)    = 0.;
    (*pRailHeadDistance) = 1.;

    SdaiInstance    ifcCantAlignmentInstance =
                        ___GetAlignmentCant(
                                model,
                                ifcAlignmentInstance,
                                nullptr
                            );

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcCantAlignmentInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
        SdaiInstance    * segmentInstances = new SdaiInstance[noSegmentInstances];

        ___GetAlignmentSegments(
                model,
                ifcCantAlignmentInstance,
                segmentInstances
            );

        for (int_t i = 0; i < noSegmentInstances; i++) {
            SdaiInstance    ifcAlignmentSegmentInstance = segmentInstances[i],
                            ifcAlignmentCantSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentCantSegmentInstance);

            double  startDistAlong = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &startDistAlong);

            double  horizontalLength = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &horizontalLength);
            if (startDistAlong == offset && horizontalLength == length) {
                double  startCantLeft = 0., startCantRight = 0.;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantLeft", sdaiREAL, (void*) &startCantLeft);
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantRight", sdaiREAL, (void*) &startCantRight);

                double  railHeadDistance = 0.;
                sdaiGetAttrBN(ifcCantAlignmentInstance, "RailHeadDistance", sdaiREAL, (void*) &railHeadDistance);
                if (railHeadDistance) {
                    (*pStartCantRight)   = startCantRight;
                    (*pStartCantLeft)    = startCantLeft;
                    (*pRailHeadDistance) = railHeadDistance;

                    return  (startCantRight - startCantLeft) / railHeadDistance;
                }
            }
        }

        delete[] segmentInstances;
    }

    assert(false);
    return  0.;
}

static  inline  double   GetCantAngleStart(
                                int_t   model,
                                int_t   ifcAlignmentInstance,
                                double  offset,
                                double  length
                            )
{
    SdaiInstance    ifcCantAlignmentInstance =
                        ___GetAlignmentCant(
                                model,
                                ifcAlignmentInstance,
                                nullptr
                            );

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcCantAlignmentInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
        SdaiInstance    * segmentInstances = new SdaiInstance[noSegmentInstances];

        ___GetAlignmentSegments(
                model,
                ifcCantAlignmentInstance,
                segmentInstances
            );

        for (int_t i = 0; i < noSegmentInstances; i++) {
            SdaiInstance    ifcAlignmentSegmentInstance = segmentInstances[i],
                            ifcAlignmentCantSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentCantSegmentInstance);

            double  startDistAlong = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &startDistAlong);

            double  horizontalLength = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &horizontalLength);
            if (startDistAlong == offset && horizontalLength == length) {
                double  startCantLeft = 0., startCantRight = 0.;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantLeft", sdaiREAL, (void*) &startCantLeft);
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantRight", sdaiREAL, (void*) &startCantRight);

                double  railHeadDistance = 0.;
                sdaiGetAttrBN(ifcCantAlignmentInstance, "RailHeadDistance", sdaiREAL, (void*) &railHeadDistance);
                if (railHeadDistance) {
                    return  (startCantRight - startCantLeft) / railHeadDistance;
                }
            }
        }

        delete[] segmentInstances;
    }

    assert(false);
    return  0.;
}

static  inline  double   GetCantEnd(
                                SdaiModel       model,
                                SdaiInstance    ifcAlignmentInstance,
                                double          offset,
                                double          length,
                                double          * pEndCantRight,
                                double          * pEndCantLeft,
                                double          * pRailHeadDistance
                            )
{
    (*pEndCantRight)     = 0.;
    (*pEndCantLeft)      = 0.;
    (*pRailHeadDistance) = 1.;

    SdaiInstance    ifcCantAlignmentInstance =
                        ___GetAlignmentCant(
                                model,
                                ifcAlignmentInstance,
                                nullptr
                            );

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcCantAlignmentInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
        SdaiInstance    * segmentInstances = new SdaiInstance[noSegmentInstances];

        ___GetAlignmentSegments(
                model,
                ifcCantAlignmentInstance,
                segmentInstances
            );

        for (int_t i = 0; i < noSegmentInstances; i++) {
            SdaiInstance    ifcAlignmentSegmentInstance = segmentInstances[i],
                            ifcAlignmentCantSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentCantSegmentInstance);

            double  startDistAlong = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &startDistAlong);

            double  horizontalLength = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &horizontalLength);
            if (startDistAlong == offset && horizontalLength == length) {
                double  endCantLeft = 0., endCantRight = 0.;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantLeft", sdaiREAL, (void*) &endCantLeft);
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantRight", sdaiREAL, (void*) &endCantRight);

                double  railHeadDistance = 0.;
                sdaiGetAttrBN(ifcCantAlignmentInstance, "RailHeadDistance", sdaiREAL, (void*) &railHeadDistance);
                if (railHeadDistance) {
                    (*pEndCantRight)     = endCantRight;
                    (*pEndCantLeft)      = endCantLeft;
                    (*pRailHeadDistance) = railHeadDistance;

                    return  (endCantRight - endCantLeft) / railHeadDistance;
                }
            }
        }

        delete[] segmentInstances;
    }

    assert(false);
    return  0.;
}

static  inline  double   GetCantAngleEnd(
                                SdaiModel       model,
                                SdaiInstance    ifcAlignmentInstance,
                                double          offset,
                                double          length
                            )
{
    SdaiInstance    ifcCantAlignmentInstance =
                        ___GetAlignmentCant(
                                model,
                                ifcAlignmentInstance,
                                nullptr
                            );

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcCantAlignmentInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
        SdaiInstance    * segmentInstances = new SdaiInstance[noSegmentInstances];

        ___GetAlignmentSegments(
                model,
                ifcCantAlignmentInstance,
                segmentInstances
            );

        for (int_t i = 0; i < noSegmentInstances; i++) {
            int_t   ifcAlignmentSegmentInstance = segmentInstances[i],
                    ifcAlignmentCantSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentCantSegmentInstance);

            double  startDistAlong = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &startDistAlong);

            double  horizontalLength = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &horizontalLength);
            if (startDistAlong == offset && horizontalLength == length) {
                double  endCantLeft = 0., endCantRight = 0.;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantLeft", sdaiREAL, (void*) &endCantLeft);
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantRight", sdaiREAL, (void*) &endCantRight);

                double  railHeadDistance = 0.;
                sdaiGetAttrBN(ifcCantAlignmentInstance, "RailHeadDistance", sdaiREAL, (void*) &railHeadDistance);
                if (railHeadDistance) {
                    return  (endCantRight - endCantLeft) / railHeadDistance;
                }
            }
        }

        delete[] segmentInstances;
    }

    assert(false);
    return  0.;
}

static  inline  double  CalculateCubicArcLength(double horizontalLength, double cubicConstant)
{
    int_t   segmentationParts = 10000;
    double  partLength = horizontalLength / (double) segmentationParts, L = 0.;
    for (int_t i = 0; i < segmentationParts; i++) {
        double  offset = i * partLength;
        ___VECTOR3  startVec = {
                            offset,
                            cubicConstant * pow(offset, 3),
                            0.
                        },
                    endVec = {
                            offset + partLength,
                            cubicConstant * pow(offset + partLength, 3),
                            0.
                        };
        L += ___Vec3Distance(&startVec, &endVec);
    }
    return  L;
}

static  inline  SdaiInstance    ___CreateCompositeCurve__alignmentHorizontal(
                                        SdaiModel       model,
                                        SdaiInstance    ifcAlignmentInstance,
                                        SdaiInstance    ifcHorizontalAlignmentInstance,
                                        double          planeAngleUnitConversionFactor
                                    )
{
    SdaiInstance	ifcCompositeCurveInstance = sdaiCreateInstanceBN(model, "IFCCOMPOSITECURVE");
    SdaiAggr        aggrCurveSegment = sdaiCreateAggrBN(ifcCompositeCurveInstance, "Segments");

    char    selfIntersect[2] = "F";
    sdaiPutAttrBN(ifcCompositeCurveInstance, "SelfIntersect", sdaiENUM, (void*) selfIntersect);

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcHorizontalAlignmentInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
        SdaiInstance    * segmentInstances = new SdaiInstance[noSegmentInstances];

        ___GetAlignmentSegments(
                model,
                ifcHorizontalAlignmentInstance,
                segmentInstances
            );

        ___VECTOR3  * pStartPoint = new ___VECTOR3[noSegmentInstances];
        double      * pStartDirection = new double[noSegmentInstances];
        for (int_t i = 0; i < noSegmentInstances; i++) {
            SdaiInstance    ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));

            SdaiInstance    ifcAlignmentHorizontalSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentHorizontalSegmentInstance);
            assert(sdaiGetInstanceType(ifcAlignmentHorizontalSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTHORIZONTALSEGMENT"));

            {
                SdaiInstance    ifcCartesianPointInstance = 0;
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, &ifcCartesianPointInstance);

                pStartPoint[i].x = 0.;
                pStartPoint[i].y = 0.;
                pStartPoint[i].z = 0.;
                ___GetCartesianPointCoordinates(ifcCartesianPointInstance, &pStartPoint[i]);

                double   startDirection = 0.;
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartDirection", sdaiREAL, &startDirection);

                startDirection *= planeAngleUnitConversionFactor;
                pStartDirection[i] = startDirection;
            }
        }

        double          mostRecentRadius = 0.,
                        mostRecentLength = 0.;
        SdaiInstance    mostRecentCurveSegmentInstance = 0;
#ifdef _DEBUG
        ___VECTOR2  mostRecentLocation = { 0., 0. };
#endif // _DEBUG

#ifdef _DEBUG
        ___POINT4D  previousEndPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } };

        char    * previousPredefinedType = nullptr;
#endif // _DEBUG

        double  compositeCurveLength = 0.;
        for (int_t i = 0; i < noSegmentInstances; i++) {
            SdaiInstance    ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));

            SdaiInstance    ifcAlignmentHorizontalSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentHorizontalSegmentInstance);
            assert(sdaiGetInstanceType(ifcAlignmentHorizontalSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTHORIZONTALSEGMENT"));

            {
                SdaiInstance    ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

                //
                //  Add geometry for Ifc...Alignment...
                //
                SdaiAggr    aggrItems = nullptr;
                sdaiPutAttrBN(
                        ifcAlignmentSegmentInstance,
                        "ObjectPlacement",
                        sdaiINSTANCE,
                        (void*) ___CreateObjectPlacement(
                                        model
                                    )
                    );
					
                assert(ifcCurveSegmentInstance && ifcAlignmentSegmentInstance);
                sdaiPutAttrBN(
                        ifcAlignmentSegmentInstance,
                        "Representation",
                        sdaiINSTANCE,
                        (void*) ___CreateProductDefinitionShapeInstance(
                                        model,
                                        ifcCurveSegmentInstance,
                                        &aggrItems,
                                        false
                                    )
                    );

                //
                //  SegmentLength
                //
                double  segmentLength = 0.;
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "SegmentLength", sdaiREAL, &segmentLength);
 ////////               assert(segmentLength >= 0.);
segmentLength = std::fabs(segmentLength);

                mostRecentLength = segmentLength;

                //
                //  Transition
                //
                if (i == noSegmentInstances - 1) {
                    char    transitionCode[14] = "DISCONTINUOUS";
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "Transition", sdaiENUM, (void*) transitionCode);
                }
                else {
                    char    transitionCode[30] = "CONTSAMEGRADIENTSAMECURVATURE";
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "Transition", sdaiENUM, (void*) transitionCode);
                }

                //
                //  Placement
                //
                ___VECTOR2  refDirection = {
                                    cos(pStartDirection[i]),
                                    sin(pStartDirection[i])
                                },
                            location = {
                                    pStartPoint[i].x,
                                    pStartPoint[i].y
                                };
                assert(pStartPoint[i].z == 0.);

                sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &location, &refDirection));
#ifdef _DEBUG
                mostRecentLocation.x = location.x;
                mostRecentLocation.y = location.y;
#endif // _DEBUG

                //
                //  Parse the individual segments
                //
                char    * predefinedType = nullptr;
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
                if (___equals(predefinedType, "CIRCULARARC")) {
                    double  radiusOfCurvature;

                    {
                        double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                        sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                        sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                        if (startRadiusOfCurvature == 0.) {
                            radiusOfCurvature = endRadiusOfCurvature;
                        }
                        else {
                            radiusOfCurvature = startRadiusOfCurvature;
                        }
                    }

                    ___MATRIX   myMatrix;
                    ___MatrixIdentity(&myMatrix);
                    myMatrix._11 =   refDirection.y * radiusOfCurvature / std::fabs(radiusOfCurvature);
                    myMatrix._12 = - refDirection.x * radiusOfCurvature / std::fabs(radiusOfCurvature);
                    myMatrix._21 = - myMatrix._12;
                    myMatrix._22 =   myMatrix._11;

                    ___VECTOR3  myOffset = { - std::fabs(radiusOfCurvature), 0., 0. };
                    ___Vec3Transform(&myOffset, &myOffset, &myMatrix);

                    myMatrix._41 = myOffset.x;
                    myMatrix._42 = myOffset.y;
                    assert(myOffset.z == 0.);

                    SdaiInstance    ifcCircularArcParentCurve =
                                        ___CreateCircleInstance(
                                                model,
                                                radiusOfCurvature,
                                                &myMatrix
                                            );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCircularArcParentCurve);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveI = ifcCircularArcParentCurve;
#endif // _DEBUG

                    if (radiusOfCurvature < 0) {
                        segmentLength = - segmentLength;
                    }

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    if (radiusOfCurvature) {
                        segmentLength = segmentLength / std::fabs(radiusOfCurvature);
                    }

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "CLOTHOID")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    if (std::fabs(startRadiusOfCurvature) + std::fabs(endRadiusOfCurvature) > 500 && std::fabs(startRadiusOfCurvature - endRadiusOfCurvature) < 0.5)
                        predefinedType = (char*) "VIENNESEBEND";

                    double  factor = 
                                  (endRadiusOfCurvature ? segmentLength / endRadiusOfCurvature : 0.)
                                - (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            constantTerm  =   0. * factor + (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            linearTerm    =   1. * factor;


double __offset = constantTerm ? segmentLength * pow(std::fabs(constantTerm),    -1. / 1.) * constantTerm / std::fabs(constantTerm)    : 0.;

double   LT = segmentLength * pow(std::fabs(linearTerm), -1. / 2.) * linearTerm / std::fabs(linearTerm);

                    double sign = (startRadiusOfCurvature + endRadiusOfCurvature) / std::fabs(startRadiusOfCurvature + endRadiusOfCurvature);
                    double  offset, __linearTernm;
                    if ((std::fabs(startRadiusOfCurvature) < std::fabs(endRadiusOfCurvature) && startRadiusOfCurvature) || endRadiusOfCurvature == 0.) {
                        double  factor = segmentLength * (1. / startRadiusOfCurvature + (endRadiusOfCurvature ? 1. / (endRadiusOfCurvature - startRadiusOfCurvature) : 0.));
assert(segmentLength > 0. && factor * sign > 0.);
                        __linearTernm = - startRadiusOfCurvature * std::sqrt(std::fabs(factor));
                        offset = - segmentLength - (endRadiusOfCurvature ? segmentLength * startRadiusOfCurvature / (endRadiusOfCurvature - startRadiusOfCurvature) : 0.);


__offset *= - factor;
assert(std::fabs(offset - __offset) < 0.000000001);
                        double  O = - startRadiusOfCurvature * factor;
                        assert(std::fabs(O - offset) < 0.0000001);
                    }
                    else {
                        assert(std::fabs(startRadiusOfCurvature) > std::fabs(endRadiusOfCurvature) || startRadiusOfCurvature == 0.);
                        double  factor = segmentLength * (1. / endRadiusOfCurvature + (startRadiusOfCurvature ? 1. / (startRadiusOfCurvature - endRadiusOfCurvature) : 0.));
assert(segmentLength > 0. && factor * sign > 0.);
                        __linearTernm = endRadiusOfCurvature * std::sqrt(std::fabs(factor));
                        offset = startRadiusOfCurvature ? segmentLength * endRadiusOfCurvature / (startRadiusOfCurvature - endRadiusOfCurvature) : 0.;
                    }

 assert(std::fabs(__linearTernm - LT) < 0.00001);

                    ___MATRIX   myMatrix;
                    ___MatrixIdentity(&myMatrix);

                    ___VECTOR3  myVec = {
                                        ___XclothoidByConstant(__linearTernm, offset),
                                        ___YclothoidByConstant(__linearTernm, offset),
                                        0.
                                    };

                    double  angleDir = atan2(refDirection.y, refDirection.x);
                    double  angle = ___AngleClothoidByConstant(__linearTernm, offset);
                    myMatrix._11 = cos(-(angle - angleDir));
                    myMatrix._12 = sin(-(angle - angleDir));
                    myMatrix._21 = - myMatrix._12;
                    myMatrix._22 =   myMatrix._11;

                    ___Vec3Transform(&myVec, &myVec, &myMatrix);
                    myMatrix._41 = - myVec.x;
                    myMatrix._42 = - myVec.y;
                    assert(myVec.z == 0.);

                    SdaiInstance    ifcClothoidParentCurve =
                                        ___CreateClothoidInstance(
                                                model,
                                                linearTerm ? segmentLength * pow(std::fabs(linearTerm), -1. / 2.) * linearTerm / std::fabs(linearTerm) : 0.,
                                                &myMatrix
                                            );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcClothoidParentCurve);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveI = ifcClothoidParentCurve;
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    if (segmentLength < 0.) {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    }
                    else {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    }
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "COSINECURVE")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    assert(segmentLength > 0.);
                    double  factor = 
                                  (endRadiusOfCurvature ? segmentLength / endRadiusOfCurvature : 0.)
                                - (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            cosineTerm   = - 0.5 * factor,
                            constantTerm =   0.5 * factor + (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.);

                    SdaiInstance    ifcCosineCurveParentCurve =
                                        ___CreateCosineSpiralInstance(
                                                model,
                                                constantTerm ? segmentLength * pow(std::fabs(constantTerm), -1. / 1.) * constantTerm / std::fabs(constantTerm) : 0.,
                                                cosineTerm   ? segmentLength * pow(std::fabs(cosineTerm),   -1. / 1.) * cosineTerm   / std::fabs(cosineTerm)   : 0.,
                                                nullptr
                                            );

                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCosineCurveParentCurve);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveI = ifcCosineCurveParentCurve;
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "SINECURVE")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    assert(segmentLength > 0.);
                    double  factor = 
                                  (endRadiusOfCurvature ? segmentLength / endRadiusOfCurvature : 0.)
                                - (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            sineTerm     = - (1. / (2. * Pi)) * factor,
                            constantTerm =    0.              * factor + (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            linearTerm   =    1.              * factor;

                    SdaiInstance    ifcSineCurveParentCurve =
                                        ___CreateSineSpiralInstance(
                                                model,
                                                linearTerm   ? segmentLength * pow(std::fabs(linearTerm),   -1. / 2.) * linearTerm   / std::fabs(linearTerm)   : 0.,
                                                constantTerm ? segmentLength * pow(std::fabs(constantTerm), -1. / 1.) * constantTerm / std::fabs(constantTerm) : 0.,
                                                sineTerm     ? segmentLength * pow(std::fabs(sineTerm),     -1. / 1.) * sineTerm     / std::fabs(sineTerm)     : 0.,
                                                nullptr
                                            );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcSineCurveParentCurve);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveI = ifcSineCurveParentCurve;
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    if (segmentLength < 0.) {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    }
                    else {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    }
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "BLOSSCURVE")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    assert(segmentLength > 0.);
                    double  factor = 
                                  (endRadiusOfCurvature ? segmentLength / endRadiusOfCurvature : 0.)
                                - (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            constantTerm  =   0. * factor + (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            linearTerm    =   0. * factor,
                            quadraticTerm =   3. * factor,
                            cubicTerm     = - 2. * factor;

                    SdaiInstance    ifcBlossCurveParentCurve =
                                        ___CreateThirdOrderPolynomialSpiralInstance(
                                                model,
                                                cubicTerm     ? segmentLength * pow(std::fabs(cubicTerm),     -1. / 4.) * cubicTerm     / std::fabs(cubicTerm)     : 0.,
                                                quadraticTerm ? segmentLength * pow(std::fabs(quadraticTerm), -1. / 3.) * quadraticTerm / std::fabs(quadraticTerm) : 0.,
                                                linearTerm    ? segmentLength * pow(std::fabs(linearTerm),    -1. / 2.) * linearTerm    / std::fabs(linearTerm)    : 0.,
                                                constantTerm  ? segmentLength * pow(std::fabs(constantTerm),  -1. / 1.) * constantTerm  / std::fabs(constantTerm)  : 0.,
                                                nullptr
                                            );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcBlossCurveParentCurve);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveI = ifcBlossCurveParentCurve;
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "HELMERTCURVE")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    assert(segmentLength > 0.);
                    double  factor = 
                                  (endRadiusOfCurvature ? segmentLength / endRadiusOfCurvature : 0.)
                                - (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            constantTermFirstHalf   =   0. * factor + (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            linearTermFirstHalf     =   0. * factor,
                            quadraticTermFirstHalf  =   2. * factor,
                            constantTermSecondHalf  = - 1. * factor + (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            linearTermSecondHalf    =   4. * factor,
                            quadraticTermSecondHalf = - 2. * factor;

                    SdaiInstance    ifcHelmertInstance =
                                        ___CreatSecondOrderPolynomialSpiralInstance(
                                                model,
                                                quadraticTermFirstHalf ? segmentLength * pow(std::fabs(quadraticTermFirstHalf), -1. / 3.) * quadraticTermFirstHalf / std::fabs(quadraticTermFirstHalf) : 0.,
                                                linearTermFirstHalf    ? segmentLength * pow(std::fabs(linearTermFirstHalf),    -1. / 2.) * linearTermFirstHalf    / std::fabs(linearTermFirstHalf)    : 0.,
                                                constantTermFirstHalf  ? segmentLength * pow(std::fabs(constantTermFirstHalf),  -1. / 1.) * constantTermFirstHalf  / std::fabs(constantTermFirstHalf)  : 0.,
                                                nullptr
                                            );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcHelmertInstance);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveI = ifcHelmertInstance;
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    double  segmentLengthHalf = segmentLength / 2.;
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLengthHalf);
                    if (segmentLengthHalf < 0.) {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    }
                    else {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    }
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);

                    sdaiAppend(aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);

                    ___VECTOR2  endPointFirstHalf = {
                                        segmentLength * ___XbyAngleDeviationPolynomial(0., quadraticTermFirstHalf, linearTermFirstHalf, constantTermFirstHalf, 0.5),
                                        segmentLength * ___YbyAngleDeviationPolynomial(0., quadraticTermFirstHalf, linearTermFirstHalf, constantTermFirstHalf, 0.5)
                                    };
                    ___VECTOR2  startPointSecondHalf = {
                                        segmentLength * ___XbyAngleDeviationPolynomial(0., quadraticTermSecondHalf, linearTermSecondHalf, constantTermSecondHalf, 0.5),
                                        segmentLength * ___YbyAngleDeviationPolynomial(0., quadraticTermSecondHalf, linearTermSecondHalf, constantTermSecondHalf, 0.5)
                                    };

#ifdef _DEBUG
                    mostRecentLocation.x = endPointFirstHalf.x;
                    mostRecentLocation.y = endPointFirstHalf.y;
#endif // _DEBUG

                    //
                    //  SECOND INSTANCE
                    //

                    ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");
                	sdaiAppend(aggrItems, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);

                    {
                        //
                        //  Add geometry for Ifc...Alignment...
                        //
                        assert(ifcCurveSegmentInstance && ifcAlignmentSegmentInstance);
                        sdaiPutAttrBN(								//									!!!!!!!!!!!!!!! IS CREATING ISSUE IN VALIDATION
                                ifcAlignmentSegmentInstance,
                                "Representation",
                                sdaiINSTANCE,
                                (void*) ___CreateProductDefinitionShapeInstance(
                                                model,
                                                ifcCurveSegmentInstance,
                                                false
                                            )
                            );
                        char    transitionCode[30] = "CONTSAMEGRADIENTSAMECURVATURE";
                        sdaiPutAttrBN(ifcCurveSegmentInstance, "Transition", sdaiENUM, (void*) transitionCode);

                        double   startDirection = 0.;
                        sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartDirection", sdaiREAL, &startDirection);
                        startDirection *= planeAngleUnitConversionFactor;

                        double      angle = startDirection + ___AngleByAngleDeviationPolynomial(0., quadraticTermFirstHalf, linearTermFirstHalf, constantTermFirstHalf, 0.5);
 
                        ___VECTOR2  refDirection__2 = {
                                            cos(angle),
                                            sin(angle)
                                        },
		                            location__2 = {
		                                    location.x + cos(startDirection) * endPointFirstHalf.x - sin(startDirection) * endPointFirstHalf.y,
		                                    location.y + sin(startDirection) * endPointFirstHalf.x + cos(startDirection) * endPointFirstHalf.y
		                                };

                        sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &location__2, &refDirection__2));
                    }

                    {
                        double      angle =
                                          ___AngleByAngleDeviationPolynomial(0., quadraticTermFirstHalf, linearTermFirstHalf, constantTermFirstHalf, 0.5)
                                        - ___AngleByAngleDeviationPolynomial(0., quadraticTermSecondHalf, linearTermSecondHalf, constantTermSecondHalf, 0.5);
                        ___MATRIX   matrix = {
                                            cos(angle),          sin(angle),          0.,
                                            -sin(angle),         cos(angle),          0.,
                                            0.,                  0.,                  1.,
                                            endPointFirstHalf.x, endPointFirstHalf.y, 0.
                                        },
                                    matrixCorrection = {
                                            1.,                      0.,                      0.,
                                            0.,                      1.,                      0.,
                                            0.,                      0.,                      1.,
                                            -startPointSecondHalf.x, -startPointSecondHalf.y, 0.
                                        };
                        ___MatrixMultiply(&matrix, &matrixCorrection, &matrix);

                        ifcHelmertInstance =
                                ___CreatSecondOrderPolynomialSpiralInstance(model,
                                        quadraticTermSecondHalf ? segmentLength * pow(std::fabs(quadraticTermSecondHalf), -1. / 3.) * quadraticTermSecondHalf / std::fabs(quadraticTermSecondHalf) : 0.,
                                        linearTermSecondHalf    ? segmentLength * pow(std::fabs(linearTermSecondHalf),    -1. / 2.) * linearTermSecondHalf    / std::fabs(linearTermSecondHalf)    : 0.,
                                        constantTermSecondHalf  ? segmentLength * pow(std::fabs(constantTermSecondHalf),  -1. / 1.) * constantTermSecondHalf  / std::fabs(constantTermSecondHalf)  : 0.,
                                        &matrix
                                    );
                    }
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcHelmertInstance);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveII = ifcHelmertInstance;
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    offset = segmentLength / 2.;
                    segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLengthHalf);
                    if (segmentLength < 0.) {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    }
                    else {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    }
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "LINE")) {
                    ___VECTOR2  dir = {
                                        refDirection.x,
                                        refDirection.y
                                    };

                    SdaiInstance    ifcLineParentCurveInstance =
                                        ___CreateLineInstance(
                                                model,
                                                &dir
                                            );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLineParentCurveInstance);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveI = ifcLineParentCurveInstance;
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "VIENNESEBEND")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    double  gravityCenterLineHeight = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "GravityCenterLineHeight", sdaiREAL, &gravityCenterLineHeight);

                    double  startCantRight = 0., startCantLeft = 0., endCantRight = 0., endCantLeft = 0., railHeadDistance = 0.;
                    GetCantStart(model, ifcAlignmentInstance, compositeCurveLength, segmentLength, &startCantRight, &startCantLeft, &railHeadDistance);
                    GetCantEnd(model, ifcAlignmentInstance, compositeCurveLength, segmentLength, &endCantRight, &endCantLeft, &railHeadDistance);

                    double  cantAngleStart = (startCantRight - startCantLeft) / railHeadDistance,
                            cantAngleEnd   = (endCantRight - endCantLeft) / railHeadDistance,
                            cantComponent  = -420. * (gravityCenterLineHeight / segmentLength) * (cantAngleEnd - cantAngleStart);

                    double  factor = 
                                  (endRadiusOfCurvature ? segmentLength / endRadiusOfCurvature : 0.)
                                - (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            constantTerm   =                         0. * factor + (startRadiusOfCurvature ? segmentLength / startRadiusOfCurvature : 0.),
                            linearTerm     =                         0. * factor,
                            quadraticTerm  =   1. * cantComponent +  0. * factor,
                            cubicTerm      = - 4. * cantComponent +  0. * factor,
                            quarticTerm    =   5. * cantComponent + 35. * factor,
                            quinticTerm    = - 2. * cantComponent - 84. * factor,
                            sexticTerm     =                        70. * factor,
                            septicTerm     =                      - 20. * factor;

                    SdaiInstance    ifcVienneseBendParentCurve =
                                        ___CreateSeventhOrderPolynomialSpiralInstance(
                                                model,
                                                septicTerm    ? segmentLength * pow(std::fabs(septicTerm),    -1. / 8.) * septicTerm    / std::fabs(septicTerm)    : 0.,
                                                sexticTerm    ? segmentLength * pow(std::fabs(sexticTerm),    -1. / 7.) * sexticTerm    / std::fabs(sexticTerm)    : 0.,
                                                quinticTerm   ? segmentLength * pow(std::fabs(quinticTerm),   -1. / 6.) * quinticTerm   / std::fabs(quinticTerm)   : 0.,
                                                quarticTerm   ? segmentLength * pow(std::fabs(quarticTerm),   -1. / 5.) * quarticTerm   / std::fabs(quarticTerm)   : 0.,
                                                cubicTerm     ? segmentLength * pow(std::fabs(cubicTerm),     -1. / 4.) * cubicTerm     / std::fabs(cubicTerm)     : 0.,
                                                quadraticTerm ? segmentLength * pow(std::fabs(quadraticTerm), -1. / 3.) * quadraticTerm / std::fabs(quadraticTerm) : 0.,
                                                linearTerm    ? segmentLength * pow(std::fabs(linearTerm),    -1. / 2.) * linearTerm    / std::fabs(linearTerm)    : 0.,
                                                constantTerm  ? segmentLength * pow(std::fabs(constantTerm),  -1. / 1.) * constantTerm  / std::fabs(constantTerm)  : 0.,
                                                nullptr
                                            );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcVienneseBendParentCurve);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveI = ifcVienneseBendParentCurve;
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    if (segmentLength < 0.) {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    }
                    else {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    }
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "CUBIC")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    double  cubicConstant = 0.,
                            offset = 0.;
                    if (startRadiusOfCurvature == 0. && i < noSegmentInstances - 1) {
                        assert(endRadiusOfCurvature);
                        ___VECTOR3  * inputStartPoint = &pStartPoint[i],
                                    * inputEndPoint = &pStartPoint[i + 1];
                        double  startDirection = pStartDirection[i];
                        assert(inputStartPoint->z == 0. && inputEndPoint->z == 0.);

                        ___VECTOR3  originCubic = { 0., 0., 0. },
                                    refDirectionCubic = { cos(startDirection), sin(startDirection), 0. },
                                    endPoint = { inputEndPoint->x - inputStartPoint->x, inputEndPoint->y - inputStartPoint->y, 0. },
                                    correctedEndPoint;

                        double  D = ___PointLineDistance(&correctedEndPoint, &endPoint, &originCubic, &refDirectionCubic);

                        double  correctedHorizontalLength = ___Vec3Distance(&originCubic, &correctedEndPoint);

                        ___MATRIX   mat;
                        ___MatrixIdentity(&mat);
                        ___Vec3Subtract((___VECTOR3*) &mat._11, &correctedEndPoint, &originCubic);
                        ___Vec3Normalize((___VECTOR3*) &mat._11);
                        ___Vec3Cross((___VECTOR3*) &mat._21, (___VECTOR3*) &mat._31, (___VECTOR3*) &mat._11);

                        ___VECTOR3  axis;
                        ___Vec3Subtract(&axis, &correctedEndPoint, &endPoint);
                        ___Vec3Normalize(&axis);

                        double  dotProduct = ___Vec3Dot((___VECTOR3*) &mat._21, &axis);
                        assert(std::fabs(std::fabs(dotProduct) - 1.) < 0.00000001);


                        //
                        //  y = a * x^3 where
                        //      0 = a * 0^3
                        //      D = a * correctedHorizontalLength^3
                        //
                        cubicConstant = D / pow(correctedHorizontalLength, 3);
                        if (dotProduct > 0.) {
                            cubicConstant = -cubicConstant;
                        }

//                        double  L = CalculateCubicArcLength(correctedHorizontalLength, cubicConstant);
                        segmentLength = correctedHorizontalLength;
                    }
                    else if (endRadiusOfCurvature == 0. && i < noSegmentInstances - 1) {
                        assert(startRadiusOfCurvature);
                        ___VECTOR3  * inputStartPoint = &pStartPoint[i],
                                    * inputEndPoint = &pStartPoint[i + 1];
                        double  startDirection = pStartDirection[i + 1];
                        assert(inputStartPoint->z == 0. && inputEndPoint->z == 0.);

                        ___VECTOR3  originCubic = { 0., 0., 0. },
                                    refDirectionCubic = { cos(startDirection), sin(startDirection), 0. },
                                    endPoint = { -(inputEndPoint->x - inputStartPoint->x), -(inputEndPoint->y - inputStartPoint->y), 0. },
                                    correctedEndPoint;

                        ___VECTOR3  secondPointCubic = {
                                            originCubic.x + 10. * refDirectionCubic.x,
                                            originCubic.y + 10. * refDirectionCubic.y,
                                            originCubic.z + 10. * refDirectionCubic.z
                                        };

                        double  D = ___PointLineDistance(&correctedEndPoint, &endPoint, &originCubic, &secondPointCubic);

                        double  correctedHorizontalLength = ___Vec3Distance(&originCubic, &correctedEndPoint);

                        ___MATRIX   mat;
                        ___MatrixIdentity(&mat);
                        ___Vec3Subtract((___VECTOR3*) &mat._11, &correctedEndPoint, &originCubic);
                        ___Vec3Normalize((___VECTOR3*) &mat._11);
                        ___Vec3Cross((___VECTOR3*) &mat._21, (___VECTOR3*) &mat._31, (___VECTOR3*) &mat._11);

                        ___VECTOR3  axis;
                        ___Vec3Subtract(&axis, &correctedEndPoint, &endPoint);
                        ___Vec3Normalize(&axis);

                        double  dotProduct = ___Vec3Dot((___VECTOR3*) &mat._21, &axis);
                        assert(std::fabs(std::fabs(dotProduct) - 1.) < 0.00000001);


                        //
                        //  y = a * x^3 where
                        //      0 = a * 0^3
                        //      D = a * correctedHorizontalLength^3
                        //
                        cubicConstant = D / pow(correctedHorizontalLength, 3);
                        if (dotProduct > 0.) {
                            cubicConstant = -cubicConstant;
                        }

//                        double  L = CalculateCubicArcLength(correctedHorizontalLength, cubicConstant);
                        segmentLength = correctedHorizontalLength;

                        offset = -segmentLength;
                    }
                    else if (std::fabs(startRadiusOfCurvature) > std::fabs(endRadiusOfCurvature)) {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        assert(false);
                    }
                    else {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        assert(false);
                    }

                    double  coefficientsX[2] = {
                                    0.,
                                    1.
                                },
                            coefficientsY[4] = {
                                    0.,
                                    0.,
                                    0.,
                                    cubicConstant
                                };

                    SdaiInstance	ifcCubicParentCurve =
                                        ___CreatePolynomialCurveInstance(
                                                model,
                                                coefficientsX,
                                                2,
                                                coefficientsY,
                                                4,
                                                nullptr,
                                                0
                                            );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCubicParentCurve);
#ifdef _DEBUG
                    horizontalAlignmentParentCurveI = ifcCubicParentCurve;
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else {
                    assert(false);

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    if (segmentLength < 0.) {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    }
                    else {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    }
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }

#ifdef _DEBUG
if (i == 2) {
    int uu = 0;
}

                ___POINT4D  startPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } },
                            endPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } };
                ___GetBorderPoints(
                        ifcCurveSegmentInstance,
                        sdaiGetEntity(model, "IFCCOMPOSITECURVE"),
                        &startPnt,
                        &endPnt
                    );

double  minDist = ___Vec3Distance(&startPnt.point, &endPnt.point);

                assert(startPnt.point.x == location.x &&
                       startPnt.point.y == location.y &&
                       startPnt.point.z == 0.);

                ___VECTOR3  tangent = {
                                    refDirection.x,
                                    refDirection.y,
                                    0.
                                };

                assert(std::fabs(startPnt.tangent.x - tangent.x) < 0.0000000001 &&
                       std::fabs(startPnt.tangent.y - tangent.y) < 0.0000000001 &&
                       startPnt.tangent.z == 0.);

                if (i) {
                    assert(___equals(predefinedType,         "VIENNESEBEND") ||
                           ___equals(previousPredefinedType, "VIENNESEBEND") ||
                           (std::fabs(startPnt.point.x - previousEndPnt.point.x < 0.001) &&
                            std::fabs(startPnt.point.y - previousEndPnt.point.y < 0.001) &&
                            startPnt.point.z == previousEndPnt.point.z));

                    assert(___equals(predefinedType,         "VIENNESEBEND") || 
                           ___equals(previousPredefinedType, "VIENNESEBEND") ||
                           (___equals(predefinedType, "LINE") && ___equals(previousPredefinedType, "LINE")) ||
                           (std::fabs(startPnt.tangent.x - previousEndPnt.tangent.x < 0.0005) &&
                            std::fabs(startPnt.tangent.y - previousEndPnt.tangent.y < 0.0005) &&
                            startPnt.tangent.z == previousEndPnt.tangent.z)); //  */
                }

                previousPredefinedType = predefinedType;
                previousEndPnt = endPnt;
#endif // _DEBUG

                sdaiAppend(aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);
                compositeCurveLength += segmentLength;

                mostRecentCurveSegmentInstance = ifcCurveSegmentInstance;
            }
        }

        if (mostRecentLength != 0.) {
            //
            //  Placement
            //
            ___VECTOR2  refDirection = {
                                1.,
                                0.
                            },
                        endPoint = {
                                0.,
                                0.
                            };

            ___GetEndPoint(
                    model,
                    &endPoint,
#ifdef _DEBUG
                    &mostRecentLocation,
#endif // _DEBUG
                    mostRecentCurveSegmentInstance,
                    sdaiGetInstanceType(ifcCompositeCurveInstance)
                );

            SdaiInstance    ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

            //
            //  Transition
            //
            char    transitionCode[14] = "DISCONTINUOUS";
            sdaiPutAttrBN(ifcCurveSegmentInstance, "Transition", sdaiENUM, (void*) transitionCode);

            sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &endPoint, &refDirection));

            if (mostRecentRadius) {
                //
                //  IfcCircle with length zero
                //
                SdaiInstance    ifcCircularArcParentCurve =
                                    ___CreateCircleInstance(
                                            model,
                                            mostRecentRadius
                                        );
                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCircularArcParentCurve);
            }
            else {
                //
                //  IfcLine with length zero
                //
                SdaiInstance    ifcLineParentCurveInstance =
                                    ___CreateLineInstance(
                                            model
                                        );
                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLineParentCurveInstance);
            }

            //
            //  SegmentStart
            //
            double  offset = 0.;
            void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
            sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
            sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

            //
            //  SegmentLength
            //
            double  segmentLength = 0.;
            void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
            sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
            sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);

            sdaiAppend(aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);
        }

        delete[] pStartDirection;
        delete[] pStartPoint;
        delete[] segmentInstances;
    }
    else {
        //
        //  IfcLine with length zero
        //
        SdaiInstance    ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

        //
        //  Transition
        //
        char    transitionCode[14] = "DISCONTINUOUS";
        sdaiPutAttrBN(ifcCurveSegmentInstance, "Transition", sdaiENUM, (void*) transitionCode);

        //
        //  Placement
        //
        ___VECTOR2  refDirection = {
                            1.,
                            0.
                        },
                    location = {
                            0.,
                            0.
                        };

        sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &location, &refDirection));

        SdaiInstance    ifcLineParentCurveInstance =
                            ___CreateLineInstance(
                                    model
                                );
        sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLineParentCurveInstance);

        //
        //  SegmentStart
        //
        double  offset = 0.;
        void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
        sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
        sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

        //
        //  SegmentLength
        //
        double  segmentLength = 0.;
        void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
        sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);

        sdaiAppend(aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);
    }

    return  ifcCompositeCurveInstance;
}

static  inline  SdaiInstance    ___GetAlignmentHorizontal(
                                        SdaiModel       model,
                                        SdaiInstance    ifcAlignmentInstance,
                                        bool            * hasIssue
                                    )
{
    SdaiInstance    ifcAlignmentHorizontalInstance = 0;

    {
        SdaiAggr    aggrIfcRelAggregates = nullptr;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsNestedBy", sdaiAGGR, &aggrIfcRelAggregates);
        SdaiInteger noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (SdaiInteger i = 0; i < noAggrIfcRelAggregates; i++) {
            SdaiInstance    ifcRelAggregatesInstance = 0;
            sdaiGetAggrByIndex(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    SdaiAggr	aggrIfcObjectDefinition = nullptr;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            SdaiInteger noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (SdaiInteger j = 0; j < noAggrIfcObjectDefinition; j++) {
                SdaiInstance    ifcObjectDefinitionInstance = 0;
                sdaiGetAggrByIndex(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTHORIZONTAL")) {
                    if (ifcAlignmentHorizontalInstance && hasIssue) {
                        (*hasIssue) = true;
                    }

                    assert(ifcAlignmentHorizontalInstance == 0);
                    ifcAlignmentHorizontalInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    if (ifcAlignmentHorizontalInstance == 0) {
	    SdaiAggr    aggrIfcRelAggregates = nullptr;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsDecomposedBy", sdaiAGGR, &aggrIfcRelAggregates);
        SdaiInteger noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (SdaiInteger i = 0; i < noAggrIfcRelAggregates; i++) {
            SdaiInstance    ifcRelAggregatesInstance = 0;
            sdaiGetAggrByIndex(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

            SdaiAggr    aggrIfcObjectDefinition = nullptr;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            SdaiInteger noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (SdaiInteger j = 0; j < noAggrIfcObjectDefinition; j++) {
                SdaiInstance    ifcObjectDefinitionInstance = 0;
                sdaiGetAggrByIndex(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTHORIZONTAL")) {
                    assert(ifcAlignmentHorizontalInstance == 0);
                    ifcAlignmentHorizontalInstance = ifcObjectDefinitionInstance;

                    if (hasIssue) {
                        (*hasIssue) = true;
                    }
                }
            }
        }
    }

    assert(ifcAlignmentHorizontalInstance);
    return  ifcAlignmentHorizontalInstance;
}
