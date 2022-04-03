#pragma once


#include "generic.h"

#include "spiral.h"
#include "ifccircle.h"
#include "ifcclothoid.h"
#include "ifccosine.h"
#include "ifcline.h"
#include "ifcpolynomialcurve.h"
#include "ifcproductdefinitionshape.h"
#include "ifcsine.h"
#include "ifcsecondorderpolynomialspiral.h"
#include "ifcseventhorderpolynomialspiral.h"
#include "ifcthirdorderpolynomialspiral.h"


static  const   uint64_t    __flagbit0 = 1;                           // 2^^0                          0000.0000..0000.0001
static  const   uint64_t    __flagbit1 = 2;                           // 2^^1                          0000.0000..0000.0010
static  const   uint64_t    __flagbit2 = 4;                           // 2^^2                          0000.0000..0000.0100
static  const   uint64_t    __flagbit3 = 8;                           // 2^^3                          0000.0000..0000.1000

static  const   uint64_t    __flagbit4 = 16;                          // 2^^4                          0000.0000..0001.0000
static  const   uint64_t    __flagbit5 = 32;                          // 2^^5                          0000.0000..0010.0000
static  const   uint64_t    __flagbit6 = 64;                          // 2^^6                          0000.0000..0100.0000
static  const   uint64_t    __flagbit7 = 128;                         // 2^^7                          0000.0000..1000.0000

static  const   uint64_t    __flagbit8 = 256;                         // 2^^8                          0000.0001..0000.0000
static  const   uint64_t    __flagbit9 = 512;                         // 2^^9                          0000.0010..0000.0000
static  const   uint64_t    __flagbit10 = 1024;                       // 2^^10                         0000.0100..0000.0000
static  const   uint64_t    __flagbit11 = 2048;                       // 2^^11                         0000.1000..0000.0000

static  const   uint64_t    __flagbit12 = 4096;                       // 2^^12                         0001.0000..0000.0000
static  const   uint64_t    __flagbit13 = 8192;                       // 2^^13                         0010.0000..0000.0000
static  const   uint64_t    __flagbit14 = 16384;                      // 2^^14                         0100.0000..0000.0000
static  const   uint64_t    __flagbit15 = 32768;                      // 2^^15                         1000.0000..0000.0000

static	const	double		___Pi = 3.14159265358979323846;


inline  static  int_t   ___GetAlignmentCant(
                                int_t   model,
                                int_t   ifcAlignmentInstance,
                                bool    * hasIssue
                            );

static  inline  double   GetCantAngle(
                                int_t   model,
                                int_t   ifcAlignmentInstance,
                                double  offset,
                                double  length
                            )
{
    int_t   ifcCantAlignmentInstance =
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
        int_t   * segmentInstances = new int_t[noSegmentInstances];

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
                double  startCantLeft = 0., endCantLeft = 0., startCantRight = 0., endCantRight = 0.;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantLeft", sdaiREAL, (void*) &startCantLeft);
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantLeft", sdaiREAL, (void*) &endCantLeft);
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantRight", sdaiREAL, (void*) &startCantRight);
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantRight", sdaiREAL, (void*) &endCantRight);

                double  startCant = startCantRight - startCantLeft,
                        endCant = endCantRight - endCantLeft;

                double  railHeadDistance = 0.;
                sdaiGetAttrBN(ifcCantAlignmentInstance, "RailHeadDistance", sdaiREAL, (void*) &railHeadDistance);
                if (railHeadDistance) {
//                    return  std::fabs(startCant - endCant) / railHeadDistance;
                    return  (startCant - endCant) / railHeadDistance;
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

static  inline  int_t   ___CreateCompositeCurve__alignmentHorizontal(
                                int_t   model,
                                int_t   ifcAlignmentInstance,
                                int_t   ifcHorizontalAlignmentInstance,
                                double  planeAngleUnitConversionFactor
                            )
{
	int_t	ifcCompositeCurveInstance = sdaiCreateInstanceBN(model, "IFCCOMPOSITECURVE"),
            * aggrCurveSegment = sdaiCreateAggrBN(ifcCompositeCurveInstance, "Segments");

    char    selfIntersect[2] = "F";
    sdaiPutAttrBN(ifcCompositeCurveInstance, "SelfIntersect", sdaiENUM, (void*) selfIntersect);

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcHorizontalAlignmentInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
        int_t   * segmentInstances = new int_t[noSegmentInstances];

        ___GetAlignmentSegments(
                model,
                ifcHorizontalAlignmentInstance,
                segmentInstances
            );

        ___VECTOR3  * pStartPoint = new ___VECTOR3[noSegmentInstances];
        double      * pStartDirection = new double[noSegmentInstances];
        for (int_t i = 0; i < noSegmentInstances; i++) {
            int_t   ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));

            int_t   ifcAlignmentHorizontalSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentHorizontalSegmentInstance);
            assert(sdaiGetInstanceType(ifcAlignmentHorizontalSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTHORIZONTALSEGMENT"));

            {
                int_t   ifcCartesianPointInstance = 0;
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

        double  compositeCurveLength = 0.;
        for (int_t i = 0; i < noSegmentInstances; i++) {
            int_t   ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));

            int_t   ifcAlignmentHorizontalSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentHorizontalSegmentInstance);
            assert(sdaiGetInstanceType(ifcAlignmentHorizontalSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTHORIZONTALSEGMENT"));

            {
                int_t   ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

                //
                //  Add geometry for Ifc...Alignment...
                //
                int_t   * aggrItems = nullptr;
                sdaiPutAttrBN(
                        ifcAlignmentSegmentInstance,
                        "ObjectPlacement",
                        sdaiINSTANCE,
                        (void*) ___CreateObjectPlacement(
                                        model
                                    )
                    );
                sdaiPutAttrBN(
                        ifcAlignmentSegmentInstance,
                        "Representation",
                        sdaiINSTANCE,
                        (void*) ___CreateProductDefinitionShape(
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
                assert(segmentLength >= 0.);

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
                ___MATRIX  matrix;
                ___MatrixIdentity(&matrix);

//                int_t   ifcCartesianPointInstance = 0;
//                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, &ifcCartesianPointInstance);

//                ___GetCartesianPointCoordinates(ifcCartesianPointInstance, (___VECTOR3*) &matrix._41);
                matrix._41 = pStartPoint[i].x;
                matrix._42 = pStartPoint[i].y;
                matrix._43 = pStartPoint[i].z;

//                double   startDirection = 0.;
//                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartDirection", sdaiREAL, &startDirection);

//                startDirection *= planeAngleUnitConversionFactor;

                matrix._11 = cos(pStartDirection[i]);
                matrix._12 = sin(pStartDirection[i]);

                sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement2D(model, &matrix));

                //
                //  Parse the individual segments
                //
                char    * predefinedType = nullptr;
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
                if (___equals(predefinedType, (char*) "CIRCULARARC")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);
//                    assert(startRadiusOfCurvature && startRadiusOfCurvature == endRadiusOfCurvature);

                    int_t   ifcCircleInstance = ___CreateCircle__woRotation(model, startRadiusOfCurvature);
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCircleInstance);

                    if (startRadiusOfCurvature == 0.) {
                        startRadiusOfCurvature = endRadiusOfCurvature;
                    }

                    if (startRadiusOfCurvature < 0) {
                        segmentLength = -segmentLength;
                    }

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    if (startRadiusOfCurvature) {
                        segmentLength = segmentLength / std::fabs(startRadiusOfCurvature);
                    }

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
//                    if (segmentLength < 0.) {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
//                    }
//                    else {
//                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
//                    }
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, (char*) "CLOTHOID")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    double  offset = 0.;
                    int_t   ifcClothoidInstance = ___CreateClothoid(model, startRadiusOfCurvature, endRadiusOfCurvature, segmentLength, &offset);
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcClothoidInstance);

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
                else if (___equals(predefinedType, (char*) "COSINECURVE")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    double  offset = 0.;
                    double  cosineTerm,
                            constantTerm;

                    assert(segmentLength > 0.);
                    if (startRadiusOfCurvature == 0.) {
                        assert(endRadiusOfCurvature);
                        cosineTerm = endRadiusOfCurvature;
                        constantTerm = 0;
                    }
                    else if (endRadiusOfCurvature == 0.) {
                        assert(startRadiusOfCurvature);
                        cosineTerm = -startRadiusOfCurvature;
                        constantTerm = 0;
                        offset = segmentLength;
                        segmentLength = -segmentLength;
                    }
                    else if (std::fabs(startRadiusOfCurvature) > std::fabs(endRadiusOfCurvature)) {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        double  factor = (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);
                        cosineTerm = 1. / factor;
                        constantTerm = segmentLength / startRadiusOfCurvature;
                        constantTerm = 1. / startRadiusOfCurvature;
                    }
                    else {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        double  factor = (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);
                        cosineTerm = 1. / factor;
                        constantTerm = segmentLength / startRadiusOfCurvature;
                        constantTerm = 1. / startRadiusOfCurvature;
                    }

                    int_t   ifcCosineCurveInstance = ___CreateCosineCurve(model, cosineTerm, constantTerm);
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCosineCurveInstance);

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
                else if (___equals(predefinedType, (char*) "SINECURVE")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    double  offset = 0.;
                    double  sineTerm,
//                            linearTerm,
                            constantTerm;

                    assert(segmentLength > 0.);
                    if (startRadiusOfCurvature == 0.) {
                        assert(endRadiusOfCurvature);
                        sineTerm = endRadiusOfCurvature;
                        constantTerm = 0.;
                    }
                    else if (endRadiusOfCurvature == 0.) {
                        assert(startRadiusOfCurvature);
                        sineTerm = -startRadiusOfCurvature;
                        constantTerm = 0.;
                        offset = segmentLength;
                        segmentLength = -segmentLength;
                    }
                    else if (std::fabs(startRadiusOfCurvature) > std::fabs(endRadiusOfCurvature)) {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        double  factor = (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);
                        sineTerm = 1. / factor;
                        constantTerm = 1. / startRadiusOfCurvature;
                    }
                    else {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        double  factor = (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);
                        sineTerm = 1. / factor;
                        constantTerm = 1. / startRadiusOfCurvature;
                    }

                    int_t   ifcSineCurveInstance = ___CreateSineCurve(model, sineTerm, constantTerm, segmentLength);
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcSineCurveInstance);

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
                else if (___equals(predefinedType, (char*) "BLOSSCURVE")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    double  offset = 0.,
                            constantTerm,
                            linearTerm,
                            quadraticTerm,
                            cubicTerm;

                    assert(segmentLength > 0.);
                    if (startRadiusOfCurvature == 0.) {
                        assert(endRadiusOfCurvature);
                        double  factor =
                                    endRadiusOfCurvature ?
                                        1. / endRadiusOfCurvature :
                                        0.;
                        constantTerm  =   0. * factor;
                        linearTerm    =   0. * factor;
                        quadraticTerm =   3. * factor;
                        cubicTerm     = - 2. * factor;
                    }
                    else if (endRadiusOfCurvature == 0.) {
                        assert(startRadiusOfCurvature);
                        double  factor =
                                    startRadiusOfCurvature ?
                                        1. / startRadiusOfCurvature :
                                        0.;
                        constantTerm  =   0. * factor;
                        linearTerm    =   0. * factor;
                        quadraticTerm =   3. * factor;
                        cubicTerm     = - 2. * factor;
                        offset        =   segmentLength;
                        segmentLength = - segmentLength;
                    }
                    else if (std::fabs(startRadiusOfCurvature) > std::fabs(endRadiusOfCurvature)) {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        double  factor =
                                    (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);
                        constantTerm  =   0. * factor + 1. / startRadiusOfCurvature;
                        linearTerm    =   0. * factor;
                        quadraticTerm =   3. * factor;
                        cubicTerm     = - 2. * factor;
                    }
                    else {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        double  factor =
                                    (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);
                        constantTerm  =   0. * factor + 1. / startRadiusOfCurvature;
                        linearTerm    =   0. * factor;
                        quadraticTerm =   3. * factor;
                        cubicTerm     = - 2. * factor;
                    }


                    int_t   ifcBlossCurveInstance =
                                ___CreateBlossCurve(
                                        model,
                                        cubicTerm     ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * cubicTerm),     -1. / 4.) * cubicTerm     / std::fabs(cubicTerm)     : 0.,
                                        quadraticTerm ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * quadraticTerm), -1. / 3.) * quadraticTerm / std::fabs(quadraticTerm) : 0.,
                                        linearTerm    ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * linearTerm),    -1. / 2.) * linearTerm    / std::fabs(linearTerm)    : 0.,
                                        constantTerm  ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * constantTerm),  -1. / 1.) * constantTerm  / std::fabs(constantTerm)  : 0.
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcBlossCurveInstance);

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
                else if (___equals(predefinedType, (char*) "HELMERTCURVE")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    double  as = 2., bs = 0., cs = 0.;
                    double  ae = -2., be = 4., ce = -1.;

                    double  offsetI = 0., offsetII = segmentLength / 2.;

                    assert(segmentLength > 0.);
                    if (startRadiusOfCurvature == 0.) {
                        assert(endRadiusOfCurvature);
                        as /= endRadiusOfCurvature;
                        bs /= endRadiusOfCurvature;
                        cs /= endRadiusOfCurvature;

                        ae /= endRadiusOfCurvature;
                        be /= endRadiusOfCurvature;
                        ce /= endRadiusOfCurvature;
                    }
                    else if (endRadiusOfCurvature == 0.) {
                        as = -2.; bs = 4.; cs = -1.;
                        ae = 2.; be = 0.; ce = 0.;

                        assert(startRadiusOfCurvature);
                        as /= startRadiusOfCurvature;
                        bs /= startRadiusOfCurvature;
                        cs /= startRadiusOfCurvature;

                        ae /= startRadiusOfCurvature;
                        be /= startRadiusOfCurvature;
                        ce /= startRadiusOfCurvature;

                        offsetI = segmentLength;
                        offsetII = segmentLength / 2.;
                        segmentLength = -segmentLength;
                    }
                    else if (std::fabs(startRadiusOfCurvature) > std::fabs(endRadiusOfCurvature)) {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        double  factor = (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);

                        as *= factor;
                        bs *= factor;
                        cs *= factor;
                        ae *= factor;
                        be *= factor;
                        ce *= factor;

                        double  constant = 1. / startRadiusOfCurvature;
                        cs += constant;
                        ce += constant;
                    }
                    else {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        double  factor = (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);
                        as *= factor;
                        bs *= factor;
                        cs *= factor;
                        ae *= factor;
                        be *= factor;
                        ce *= factor;

                        double  constant = 1. / startRadiusOfCurvature;
                        cs += constant;
                        ce += constant;
                    }

                    double  asH = as ? segmentLength * pow(std::fabs(segmentLength * as), -1. / 3.) * as / std::fabs(as) : 0.;
                    double  bsH = bs ? segmentLength * pow(std::fabs(segmentLength * bs), -1. / 2.) * bs / std::fabs(bs) : 0.;
                    double  csH = cs ? segmentLength * pow(std::fabs(segmentLength * cs), -1. / 1.) * cs / std::fabs(cs) : 0.;

                    double  aeH = ae ? segmentLength * pow(std::fabs(segmentLength * ae), -1. / 3.) * ae / std::fabs(ae) : 0.;
                    double  beH = be ? segmentLength * pow(std::fabs(segmentLength * be), -1. / 2.) * be / std::fabs(be) : 0.;
                    double  ceH = ce ? segmentLength * pow(std::fabs(segmentLength * ce), -1. / 1.) * ce / std::fabs(ce) : 0.;


                    as *= segmentLength;
                    bs *= segmentLength;
                    cs *= segmentLength;

                    ae *= segmentLength;
                    be *= segmentLength;
                    ce *= segmentLength;

                    ___MATRIX  matrix___;
                    ___MatrixIdentity(&matrix___);

                    int_t   ifcHelmertInstance = ___CreateHelmert(model, asH, bsH, csH, &matrix___);
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcHelmertInstance);


                    //
                    //  SegmentStart
                    //
                    void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offsetI);
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

                    sdaiAppend((int_t) aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);

                    double  x_firstHalf = 0., y_firstHalf = 0.;

                    double  x_secondHalf = 0., y_secondHalf = 0.;

                    x_firstHalf = segmentLength * ___XbyAngleDeviationPolynomial(0., as, bs, cs, 0.5);
                    y_firstHalf = segmentLength * ___YbyAngleDeviationPolynomial(0., as, bs, cs, 0.5);

                    x_secondHalf = segmentLength * ___XbyAngleDeviationPolynomial(0., ae, be, ce, 0.5);
                    y_secondHalf = segmentLength * ___YbyAngleDeviationPolynomial(0., ae, be, ce, 0.5);

                    //
                    //  SECOND INSTANCE
                    //

                    ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");
                	sdaiAppend((int_t) aggrItems, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);

                    {
                        //
                        //  Add geometry for Ifc...Alignment...
                        //
                        sdaiPutAttrBN(
                                ifcAlignmentSegmentInstance,
                                "ObjectPlacement",
                                sdaiINSTANCE,
                                (void*) ___CreateObjectPlacement(
                                                model
                                            )
                            );
                        sdaiPutAttrBN(
                                ifcAlignmentSegmentInstance,
                                "Representation",
                                sdaiINSTANCE,
                                (void*) ___CreateProductDefinitionShape(
                                                model,
                                                ifcCurveSegmentInstance,
                                                false
                                            )
                            );
                        char    transitionCode[30] = "CONTSAMEGRADIENTSAMECURVATURE";
                        sdaiPutAttrBN(ifcCurveSegmentInstance, "Transition", sdaiENUM, (void*) transitionCode);

                        int_t   ifcCartesianPointInstance = 0;
                        sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, &ifcCartesianPointInstance);


#ifdef _DEBUG
int_t   * aggrItems__DEBUG = nullptr;
sdaiPutAttrBN(
        sdaiCreateInstanceBN(
                model,
                "IFCPROXY"
            ),
        "Representation",
        sdaiINSTANCE,
        (void*) ___CreateProductDefinitionShape(
                        model,
                        ifcHelmertInstance,
                        &aggrItems__DEBUG,
                        true
                    )
    );
{
    ___VECTOR2  pnt = { -1., 0. }, dir = { 1., 0. };
    sdaiAppend(
            (int_t) aggrItems__DEBUG,
            sdaiINSTANCE,
            (void*) ___CreateLine(
                            model,
                            &pnt,
                            &dir
                        )
        );
    pnt.x = 0.;
    sdaiAppend(
            (int_t) aggrItems__DEBUG,
            sdaiINSTANCE,
            (void*) ___CreateLine(
                            model,
                            &pnt,
                            &dir
                        )
        );
}
{
    ___VECTOR2  pnt = { 0., -1. }, dir = { 0., 1. };
    sdaiAppend(
            (int_t) aggrItems__DEBUG,
            sdaiINSTANCE,
            (void*) ___CreateLine(
                            model,
                            &pnt,
                            &dir
                        )
        );
    pnt.y = 0.;
    sdaiAppend(
            (int_t) aggrItems__DEBUG,
            sdaiINSTANCE,
            (void*) ___CreateLine(
                            model,
                            &pnt,
                            &dir
                        )
        );
}
#endif // _DEBUG

                        double   startDirection = 0.;
                        sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartDirection", sdaiREAL, &startDirection);

                        startDirection *= planeAngleUnitConversionFactor;

                        matrix._11 = cos(startDirection);
                        matrix._12 = sin(startDirection);

___MATRIX  matrixI;
___MatrixIdentity(&matrixI);

double  ANGLE = 0.;
if (segmentLength > 0.) {
    matrixI._41 = x_firstHalf;
    matrixI._42 = y_firstHalf;
}
else {
    double  angleFirst = ___AngleByAngleDeviationPolynomial(0., as, bs, cs, 1.);// segmentLengthHalf);
    double  angle = -angleFirst + startDirection;

    matrixI._11 = cos(angle);
    matrixI._12 = sin(angle);
    matrixI._21 = -sin(angle);
    matrixI._22 = cos(angle);

    ANGLE = -angleFirst;

    double  x_firstHalfStart = segmentLength * ___XbyAngleDeviationPolynomial(0., as, bs, cs, 1.);// segmentLengthHalf);
    double  y_firstHalfStart = segmentLength * ___YbyAngleDeviationPolynomial(0., as, bs, cs, 1.);// segmentLengthHalf);


    ___VECTOR3   vec = { x_firstHalf - x_firstHalfStart, y_firstHalf - y_firstHalfStart, 0. };
    ___Vec3Transform(&vec, &vec, &matrixI);

    matrixI._41 = vec.x;
    matrixI._42 = vec.y;

}

double  angleFirst = ___AngleByAngleDeviationPolynomial(0., as, bs, cs, 0.5);// segmentLengthHalf);
double  angle = angleFirst + startDirection + ANGLE;

matrixI._11 = cos(angle);
matrixI._12 = sin(angle);
matrixI._21 = -sin(angle);
matrixI._22 = cos(angle);

                        sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement2D(model, &matrixI));
                    }

                    {
___MATRIX  mat__;
___MatrixIdentity(&mat__);
double  angleFirst = ___AngleByAngleDeviationPolynomial(0., as, bs, cs, 0.5);// segmentLengthHalf);
double  angleSecond = ___AngleByAngleDeviationPolynomial(0., ae, be, ce, 0.5);// segmentLengthHalf);
double  __angle = (angleFirst - angleSecond) * segmentLength / std::fabs(segmentLength);
mat__._11 = cos(__angle);
mat__._12 = sin(__angle);
mat__._21 = -sin(__angle);
mat__._22 = cos(__angle);
mat__._41 = (x_firstHalf);
mat__._42 = (y_firstHalf);
___MATRIX  mat____;
___MatrixIdentity(&mat____);
mat____._41 = -x_secondHalf;
mat____._42 = -y_secondHalf;
___MatrixMultiply(&mat__, &mat____, &mat__);

                        ifcHelmertInstance = ___CreateHelmert(model, aeH, beH, ceH, &mat__);
                    }
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcHelmertInstance);

#ifdef _DEBUG___
int_t   * aggrItems = nullptr;
sdaiPutAttrBN(
        sdaiCreateInstanceBN(
                model,
                "IFCPROXY"
            ),
        "Representation",
        sdaiINSTANCE,
        (void*) ___CreateProductDefinitionShape(
                        model,
                        ifcHelmertInstance,
                        &aggrItems
                    )
    );
{
    ___VECTOR2  pnt = { mat__._41, mat__._42 }, dir = { mat__._11, mat__._12 };
    sdaiAppend(
            (int_t) aggrItems,
            sdaiINSTANCE,
            (void*) ___CreateLine(
                            model,
                            &pnt,
                            &dir
                        )
        );
    dir.x = -dir.x;
    dir.y = -dir.y;
    sdaiAppend(
            (int_t) aggrItems,
            sdaiINSTANCE,
            (void*) ___CreateLine(
                            model,
                            &pnt,
                            &dir
                        )
        );
}
{
    ___VECTOR2  pnt = { mat__._41, mat__._42 }, dir = { mat__._21, mat__._22 };
    sdaiAppend(
            (int_t) aggrItems,
            sdaiINSTANCE,
            (void*) ___CreateLine(
                            model,
                            &pnt,
                            &dir
                        )
        );
    dir.x = -dir.x;
    dir.y = -dir.y;
    sdaiAppend(
            (int_t) aggrItems,
            sdaiINSTANCE,
            (void*) ___CreateLine(
                            model,
                            &pnt,
                            &dir
                        )
        );
}
#endif // _DEBUG

                    //
                    //  SegmentStart
                    //
                    segmentStartADB = sdaiCreateADB(sdaiREAL, &offsetII);
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
                else if (___equals(predefinedType, (char*) "LINE")) {
                    int_t   ifcLineInstance = ___CreateLine(model);
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLineInstance);

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
                else if (___equals(predefinedType, (char*) "VIENNESEBEND")) {
                    double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                    double  gravityCenterLineHeight = 0.;
                    sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "GravityCenterLineHeight", sdaiREAL, &gravityCenterLineHeight);

                    double  cantAngle     = GetCantAngle(model, ifcAlignmentInstance, compositeCurveLength, segmentLength),
                            cantComponent = -420. * (gravityCenterLineHeight / segmentLength) * cantAngle;

                    bool    flip = false;
                    double  offset = 0., factor = 1., constant = 0.,
                            radiusOfCurvature = 0.;
                    assert(segmentLength > 0.);
                    if (startRadiusOfCurvature == 0.) {
                        assert(endRadiusOfCurvature);
                        radiusOfCurvature = endRadiusOfCurvature;

                        cantComponent = -cantComponent;
                    }
                    else if (endRadiusOfCurvature == 0.) {
                        assert(startRadiusOfCurvature);
                        radiusOfCurvature = startRadiusOfCurvature;
                        offset = segmentLength;
                        segmentLength = -segmentLength;

                        flip = true;
                    }
                    else if (std::fabs(startRadiusOfCurvature) > std::fabs(endRadiusOfCurvature)) {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        factor = (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);

                        cantComponent = -cantComponent;
                        radiusOfCurvature = 1. / factor;
                        constant = 1. / startRadiusOfCurvature;

cantComponent *= endRadiusOfCurvature * factor;
                    }
                    else {
                        assert(startRadiusOfCurvature / std::fabs(startRadiusOfCurvature) == endRadiusOfCurvature / std::fabs(endRadiusOfCurvature));
                        factor = (1. / endRadiusOfCurvature - 1. / startRadiusOfCurvature);

                        radiusOfCurvature = 1. / factor;
                        constant = 1. / startRadiusOfCurvature;

cantComponent *= startRadiusOfCurvature * factor;
                    }


                    double   polynomialConstants[8] = {
                                      constant,
                                      0.,
                                      1. * cantComponent / std::fabs(segmentLength),
                                    - 4. * cantComponent / std::fabs(segmentLength),
                                      5. * cantComponent / std::fabs(segmentLength) + 35. / radiusOfCurvature,
                                    - 2. * cantComponent / std::fabs(segmentLength) - 84. / radiusOfCurvature,
                                                                                      70. / radiusOfCurvature,
                                                                                    - 20. / radiusOfCurvature
                                };

                    if (flip) {
                        for (int_t ii = 0; ii < 8; ii++) {
                            polynomialConstants[ii] = -polynomialConstants[ii];
                        }
                    }

                    factor = 1.;
                    double  a = polynomialConstants[7] * factor,
                            b = polynomialConstants[6] * factor,
                            c = polynomialConstants[5] * factor,
                            d = polynomialConstants[4] * factor,
                            e = polynomialConstants[3] * factor,
                            f = polynomialConstants[2] * factor,
                            g = polynomialConstants[1] * factor,
                            h = polynomialConstants[0] * factor;

                    int_t   ifcVienneseBendInstance =
                                ___CreateVienneseBend(
                                        model,
                                        a ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * a), -1. / 8.) * a / std::fabs(a) : 0.,
                                        b ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * b), -1. / 7.) * b / std::fabs(b) : 0.,
                                        c ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * c), -1. / 6.) * c / std::fabs(c) : 0.,
                                        d ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * d), -1. / 5.) * d / std::fabs(d) : 0.,
                                        e ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * e), -1. / 4.) * e / std::fabs(e) : 0.,
                                        f ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * f), -1. / 3.) * f / std::fabs(f) : 0.,
                                        g ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * g), -1. / 2.) * g / std::fabs(g) : 0.,
                                        h ? std::fabs(segmentLength) * pow(std::fabs(segmentLength * h), -1. / 1.) * h / std::fabs(h) : 0.
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcVienneseBendInstance);

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
                else if (___equals(predefinedType, (char*) "CUBIC")) {
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

                    int_t   ifcCubicInstance = ___CreateCubic(model, cubicConstant);
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCubicInstance);

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
//                    if (segmentLength < 0.) {
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
//                    }
//                    else {
//                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
//                    }
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

                sdaiAppend((int_t) aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);
                compositeCurveLength += segmentLength;
            }
        }

        delete[] pStartDirection;
        delete[] pStartPoint;
        delete[] segmentInstances;
    }

    return  ifcCompositeCurveInstance;
}

static  inline  int_t   ___GetAlignmentHorizontal(
                                int_t   model,
                                int_t   ifcAlignmentInstance,
                                bool    * hasIssue
                            )
{
    int_t   ifcAlignmentHorizontalInstance = 0;

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsNestedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

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
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsDecomposedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

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
