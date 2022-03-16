#pragma once


#include "generic.h"
#include "mathematics.h"
#include "ifcpolynomialcurve.h"
#include "ifcproductdefinitionshape.h"


enum class enum_segment_type : unsigned char
{
    CIRCULARARC,
	CLOTHOID,
    CONSTANTGRADIENT,
    PARABOLICARC
};

static  inline  int_t   ___SegmentCount__alignmentVertical(
                                int_t   ifcVerticalAlignmentInstance
                            )
{
	int_t	* aggrSegments = nullptr;

    sdaiGetAttrBN(ifcVerticalAlignmentInstance, "Segments", sdaiAGGR, &aggrSegments);

    return  sdaiGetMemberCount(aggrSegments);
}

static  inline  int_t   ___CreateGradientCurve__alignmentVertical(
                                int_t   model,
                                int_t   ifcVerticalAlignmentInstance,
                                double  startDistAlongHorizontalAlignment
                            )
{
#ifdef _DEBUG
    double  epsilon = 0.0000001;
#endif // _DEBUG

	int_t	ifcGradientCurveInstance = sdaiCreateInstanceBN(model, "IFCGRADIENTCURVE"),
            * aggrCurveSegment = sdaiCreateAggrBN(ifcGradientCurveInstance, "Segments");
    char    selfIntersect[2] = "F";
    sdaiPutAttrBN(ifcGradientCurveInstance, "SelfIntersect", sdaiENUM, (void*) selfIntersect);

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcVerticalAlignmentInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
        int_t   * segmentInstances = new int_t[noSegmentInstances];

        ___GetAlignmentSegments(
                model,
                ifcVerticalAlignmentInstance,
                segmentInstances
            );

        double               * pStartGradient = new double[noSegmentInstances],
                             * pEndGradient = new double[noSegmentInstances],
                             * pStartHeight = new double[noSegmentInstances],
                             * pRadiusOfCurvature = new double[noSegmentInstances];
        enum_segment_type    * pSegmentType = new enum_segment_type[noSegmentInstances];

        for (int_t i = 0; i < noSegmentInstances; i++) {
            int_t   ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));

            int_t   ifcAlignmentVerticalSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentVerticalSegmentInstance);

            //
            //  StartHeight
            //
            double  startHeight = 0.;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, &startHeight);
            pStartHeight[i] = startHeight;

            char    * predefinedType = nullptr;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
            if (___equals(predefinedType, (char*) "CIRCULARARC")) {
                //
                //  StartGradient
                //
                double  startGradient__ = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, &startGradient__);
                double  startAngle = std::atan(startGradient__);
                pStartGradient[i] = startGradient__;

                //
                //  EndGradient
                //
                double  endGradient__ = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "EndGradient", sdaiREAL, &endGradient__);
                double  endAngle = std::atan(endGradient__);
                pEndGradient[i] = endGradient__;

                double  horizontalLength = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, &horizontalLength);

                pRadiusOfCurvature[i] = horizontalLength / (sin(endAngle) - sin(startAngle));
                pSegmentType[i] = enum_segment_type::CIRCULARARC;
            }
            else if (___equals(predefinedType, (char*) "CLOTHOID")) {
                pStartGradient[i] = 0.;
                pEndGradient[i] = 0.;

                pRadiusOfCurvature[i] = 0.;
                pSegmentType[i] = enum_segment_type::CLOTHOID;
            }
            else if (___equals(predefinedType, (char*) "CONSTANTGRADIENT")) {
                double  startGradient__ = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, &startGradient__);
//                double  angle = std::atan(startGradient__);
                pStartGradient[i] = startGradient__;
                pEndGradient[i] = startGradient__;

                pRadiusOfCurvature[i] = 0.;
                pSegmentType[i] = enum_segment_type::CONSTANTGRADIENT;
            }
            else {
                assert(___equals(predefinedType, (char*) "PARABOLICARC"));

                pStartGradient[i] = 0.;
                pEndGradient[i] = 0.;

                pRadiusOfCurvature[i] = 0.;
                pSegmentType[i] = enum_segment_type::PARABOLICARC;
            }
        }

        for (int_t i = 0; i < noSegmentInstances; i++) {
            int_t   ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));

            int_t   ifcAlignmentVerticalSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentVerticalSegmentInstance);
            assert(sdaiGetInstanceType(ifcAlignmentVerticalSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICALSEGMENT"));

            {
                int_t   ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

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
                        (void*) ___CreateProductDefinitionShapeInstance(
                                        model,
                                        ifcCurveSegmentInstance,
                                        false
                                    )
                    );

                //
                //  ENTITY IfcAlignmentVerticalSegment
                //      StartDistAlong      IfcLengthMeasure
                //      HorizontalLength    IfcPositiveLengthMeasure
                //      StartHeight         IfcLengthMeasure
                //      StartGradient       IfcLengthMeasure
                //      EndGradient         IfcLengthMeasure
                //      RadiusOfCurvature   OPTIONAL IfcPositiveLengthMeasure
                //      PredefinedType      IfcAlignmentVerticalSegmentTypeEnum
                //  END_ENTITY
                //

                //
                //  StartDistAlong
                //
                double  startDistAlong = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartDistAlong", sdaiREAL, &startDistAlong);

                //
                //  HorizontalLength
                //
                double  horizontalLength = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, &horizontalLength);

                //
                //  StartHeight
                //
                double  startHeight = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, &startHeight);

                //
                //  StartGradient
                //
                double  startGradient__ = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, &startGradient__);

                //
                //  EndGradient
                //
                double  endGradient__ = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "EndGradient", sdaiREAL, &endGradient__);

                char    * predefinedType = nullptr;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);

                if (___equals(predefinedType, (char*) "PARABOLICARC")) {
                    if (startGradient__ == 0. && i) {
                        startGradient__ = pEndGradient[i - 1];
                    }
                    if (endGradient__ == 0. && i < noSegmentInstances - 1) {
                        endGradient__ = pStartGradient[i + 1];
                    }
                }

                //
                //  RadiusOfCurvature
                //
                double  radiusOfCurvature = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "RadiusOfCurvature", sdaiREAL, &radiusOfCurvature);

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

                ___VECTOR2  refDirection = {
                                    1.,
                                    startGradient__
                                },
                            location = {
                                    startDistAlong - startDistAlongHorizontalAlignment,
                                    startHeight
                                };
                ___Vec2Normalize(&refDirection);
                sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &location, &refDirection));

                if ((horizontalLength == 0.) &&
                    (i == noSegmentInstances - 1)) {
                    sdaiPutAttrBN(ifcGradientCurveInstance, "EndPoint", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &location, &refDirection));
                }
                else {
                    assert(horizontalLength > 0.);
                }

                //
                //  Parse the individual segments
                //      CONSTANTGRADIENT
                //      CIRCULARARC
                //      PARABOLICARC
                //      CLOTHOID
                //
                if (___equals(predefinedType, (char*) "CIRCULARARC")) {
                    double  startAngle = std::atan(startGradient__),
                            endAngle = std::atan(endGradient__);
                    assert(startAngle > -___Pi && startAngle < ___Pi && endAngle > -___Pi && endAngle < ___Pi);

                    double  radius;
#ifdef _DEBUG
                    ___VECTOR2 origin;
#endif // _DEBUG
                    if (startAngle < endAngle) {
 ///                       assert(radiusOfCurvature > 0.);
                        //
                        //  Ox = -sin( startAngle ) * radius         Ox = horizontalLength - sin( endAngle ) * radius
                        //  Oy = cos( startAngle ) * radius          Oy = offsetY + cos( endAngle ) * radius
                        //
                        //  horizontalLength = (sin( endAngle ) - sin( startAngle )) * radius
                        //  radius = horizontalLength / (sin( endAngle ) - sin( startAngle ));
                        //
                        radius = horizontalLength / (sin(endAngle) - sin(startAngle));
 ///////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!????????????????????                       assert(radius > 0. && std::fabs(radius - radiusOfCurvature) < 0.001);

                        //
                        //  offsetY = (cos( startAngle ) - cos( endAngle )) * radius;
                        //
#ifdef _DEBUG
                        double  offsetY = (cos(startAngle) - cos(endAngle)) * radius;

                        origin.x = -sin(startAngle) * radius;
                        origin.y = cos(startAngle) * radius;
#endif // _DEBUG

                        assert(std::fabs(origin.x - (horizontalLength - sin(endAngle) * radius)) < epsilon);
                        assert(std::fabs(origin.y - (offsetY + cos(endAngle) * radius)) < epsilon);

                        startAngle += 3. * ___Pi / 2.;
                        endAngle += 3. * ___Pi / 2.;

//                        origin.x = -cos(startAngle) * radius;
//                        origin.y = -sin(startAngle) * radius;
                    }
                    else {
                        assert(startAngle > endAngle);
 ///                       assert(radiusOfCurvature < 0.);
                        //
                        //  Ox = sin( startAngle ) * radius         Ox = horizontalLength + sin( endAngle ) * radius
                        //  Oy = -cos( startAngle ) * radius        Oy = offsetY - cos( endAngle ) * radius
                        //
                        //  horizontalLength = (sin( startAngle ) - sin( endAngle )) * radius
                        //  radius = horizontalLength / (sin( startAngle ) - sin( endAngle ));
                        //
                        radius = horizontalLength / (sin(startAngle) - sin(endAngle));
///                        assert(radius > 0. && std::fabs(radius + radiusOfCurvature) < 0.001);
 
                        //
                        //  offsetY = (cos( endAngle ) - cos( startAngle )) * radius;
                        //
#ifdef _DEBUG
                        double  offsetY = (cos(endAngle) - cos(startAngle)) * radius;

                        origin.x = sin(startAngle) * radius;
                        origin.y = -cos(startAngle) * radius;
#endif // _DEBUG

                        assert(std::fabs(origin.x - (horizontalLength + sin(endAngle) * radius)) < epsilon);
                        assert(std::fabs(origin.y - (offsetY - cos(endAngle) * radius)) < epsilon);

                        startAngle += ___Pi / 2.;
                        endAngle += ___Pi / 2.;

#ifdef _DEBUG
                        origin.x = -cos(startAngle) * radius;
                        origin.y = -sin(startAngle) * radius;
#endif // _DEBUG
                    }

                    int_t   ifcCircularArcParentCurve =
                                ___CreateCircleInstance(
                                        model,
                                        radius
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCircularArcParentCurve);

                    //
                    //  SegmentStart
                    //
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &startAngle);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    double  segmentLengthAsParameter = endAngle - startAngle;
                    assert(std::fabs(segmentLengthAsParameter * radius) > horizontalLength);
                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLengthAsParameter);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, (char*) "CLOTHOID")) {
                    double  startAngle = std::atan(startGradient__),
                            endAngle = std::atan(endGradient__);

                    {
                        //
                        //  new definition where the context defines the radius
                        //
                        double  startRadiusOfCurvature = pRadiusOfCurvature[i - 1];
#ifdef _DEBUG
                        double  endRadiusOfCurvature = pRadiusOfCurvature[i + 1];
#endif // _DEBUG

                        //
                        //  HorizontalLength
                        //
                        double  horizontalLength__ = 0.;
                        sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, &horizontalLength__);
                        assert(horizontalLength__ == horizontalLength);

                        if (startRadiusOfCurvature == 0.) {
                            if (true) {
//                            double  gradientSpiral = pEndGradient[i - 1];

                            ___VECTOR3 	originSpiral = { 0., 0., 0. },
                                    	correctedEndPoint;

#ifdef _DEBUG
                            ___VECTOR3		refDirectionSpiral = { 1., startGradient__, 0. },
                                    		endPoint = { horizontalLength, pStartHeight[i + 1] - pStartHeight[i], 0. };
                            double	D = ___PointLineDistance(&correctedEndPoint, &endPoint, &originSpiral, &refDirectionSpiral);
#endif // _DEBUG

                            double  correctedHorizontalLength = ___Vec3Distance(&originSpiral, &correctedEndPoint),
                                    correctedStartAngle = 0.,
                                    correctedEndAngle = endAngle - startAngle;

                            double  L1 = correctedStartAngle ? (correctedStartAngle / std::fabs(correctedStartAngle)) * sqrt(2. * std::fabs(correctedStartAngle) / 1.) : 0.,
                                    L2 = correctedEndAngle ? (correctedEndAngle / std::fabs(correctedEndAngle)) * sqrt(1. * std::fabs(correctedEndAngle) / 1.) : 0.;

                            double  polynomialConstants[3] = { 0., 0., 1. },
                                    x1 = ___IntegralTaylorSeriesCos(polynomialConstants, 3, L1),
                                    x2 = ___IntegralTaylorSeriesCos(polynomialConstants, 3, L2);

                            {
                                double  distance = x2 - x1;
//                                assert(distance > 0.);

                                double  a = std::pow(distance / correctedHorizontalLength, 2);
                                polynomialConstants[2] = a;
                                L1 = correctedStartAngle ? (correctedStartAngle / std::fabs(correctedStartAngle)) * sqrt(2. * std::fabs(correctedStartAngle) / a) : 0.;
                                L2 = correctedEndAngle ? (correctedEndAngle / std::fabs(correctedEndAngle)) * sqrt(1. * std::fabs(correctedEndAngle) / a) : 0.;
//                                assert(L1 < L2);

                                x1 = ___IntegralTaylorSeriesCos(polynomialConstants, 3, L1);
                                x2 = ___IntegralTaylorSeriesCos(polynomialConstants, 3, L2);
//                                double  dist = x2 - x1;
    //                            assert(std::fabs(dist - correctedHorizontalLength) < 0.0000001);

#ifdef _DEBUG
                                double  y2 = ___IntegralTaylorSeriesSin(polynomialConstants, 3, L2);
#endif // _DEBUG
                                assert(std::fabs(std::fabs(y2) - std::fabs(D)) < 0.0000001);

                                double  segmentLength = L2 - L1,
                                        offset = L1;

                                double  linearTerm = (correctedEndAngle / std::fabs(correctedEndAngle)) / (2. * a);

#ifdef _DEBUG
                                double  angle1 = ___AngleByAngleDeviationPolynomial(0., 0., (linearTerm) ? 1. / linearTerm : 0., 0., L1);
                                double  angle2 = ___AngleByAngleDeviationPolynomial(0., 0., (linearTerm) ? 1. / linearTerm : 0., 0., L2);
#endif // _DEBUG
                                assert(std::fabs(correctedStartAngle - angle1) < 0.000001);
                                assert(std::fabs(correctedEndAngle - angle2) < 0.000001);

                                int_t   ifcClothoidInstance =
                                            ___CreateClothoidInstance(
                                                    model,
//                                                    linearTerm ? segmentLength * pow(std::fabs(linearTerm), -1. / 2.) * linearTerm / std::fabs(linearTerm) : 0.
                                                    linearTerm ? 1. * pow(std::fabs(linearTerm), -1. / 2.) * linearTerm / std::fabs(linearTerm) : 0.,
                                                    nullptr
                                               );
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
                                sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                            }
                            }
                        }
                        else {
                            if (true) {
                            assert(endRadiusOfCurvature == 0.);
#ifdef _DEBUG
                            double  gradientSpiral = pStartGradient[i + 1];
#endif // _DEBUG

                            ___VECTOR3 	originSpiral = { horizontalLength, 0., 0. },
#ifdef _DEBUG
                                    refDirectionSpiral = { 1., gradientSpiral, 0. },
                                    endPoint = { 0., pStartHeight[i] - pStartHeight[i + 1], 0. },
#endif // _DEBUG
                                    correctedEndPoint;

#ifdef _DEBUG
                            ___VECTOR3 	secondPointSpiral = {
	                                            originSpiral.x + 10. * refDirectionSpiral.x,
	                                            originSpiral.y + 10. * refDirectionSpiral.y,
	                                            originSpiral.z + 10. * refDirectionSpiral.z
	                                        };
                            double	D = ___PointLineDistance(&correctedEndPoint, &endPoint, &originSpiral, &secondPointSpiral);
#endif // _DEBUG

                            double  correctedHorizontalLength = ___Vec3Distance(&originSpiral, &correctedEndPoint),
                                    correctedStartAngle = startAngle - endAngle,
                                    correctedEndAngle = 0.;

                            double  L1 = correctedStartAngle ? (correctedStartAngle / std::fabs(correctedStartAngle)) * sqrt(1. * std::fabs(correctedStartAngle) / 1.) : 0.,
                                    L2 = correctedEndAngle ? (correctedEndAngle / std::fabs(correctedEndAngle)) * sqrt(1. * std::fabs(correctedEndAngle) / 1.) : 0.;

                            double  polynomialConstants[3] = { 0., 0., 1. },
                                    x1 = ___IntegralTaylorSeriesCos(polynomialConstants, 3, L1),
                                    x2 = ___IntegralTaylorSeriesCos(polynomialConstants, 3, L2);

                            {
                                double  distance = x2 - x1;
    //                            assert(distance > 0.);

                                double  a = std::pow(distance / correctedHorizontalLength, 2);
                                polynomialConstants[2] = a;
                                L1 = correctedStartAngle ? (correctedStartAngle / std::fabs(correctedStartAngle)) * sqrt(1. * std::fabs(correctedStartAngle) / a) : 0.;
                                L2 = correctedEndAngle ? (correctedEndAngle / std::fabs(correctedEndAngle)) * sqrt(1. * std::fabs(correctedEndAngle) / a) : 0.;
    //                            assert(L1 < L2);

                                x1 = ___IntegralTaylorSeriesCos(polynomialConstants, 3, L1);
                                x2 = ___IntegralTaylorSeriesCos(polynomialConstants, 3, L2);
//                                double  dist = x2 - x1;
    //                            assert(std::fabs(dist - correctedHorizontalLength) < 0.0000001);

#ifdef _DEBUG
                                double  y1 = ___IntegralTaylorSeriesSin(polynomialConstants, 3, L1);
#endif // _DEBUG
                                assert(std::fabs(std::fabs(y1) - std::fabs(D)) < 0.0000001);

                                double  segmentLength = L2 - L1,
                                        offset = L1;

                                double  linearTerm = (correctedStartAngle / std::fabs(correctedStartAngle)) / (2. * a);

#ifdef _DEBUG
                                double  angle1 = ___AngleByAngleDeviationPolynomial(0., 0., (linearTerm) ? 1. / linearTerm : 0., 0., L1);
                                double  angle2 = ___AngleByAngleDeviationPolynomial(0., 0., (linearTerm) ? 1. / linearTerm : 0., 0., L2);
#endif // _DEBUG
                                assert(std::fabs(correctedStartAngle - angle1) < 0.000001);
                                assert(std::fabs(correctedEndAngle - angle2) < 0.000001);

                                int_t   ifcClothoidParentCurve =
                                            ___CreateClothoidInstance(
                                                    model,
                                                    linearTerm ? segmentLength * pow(std::fabs(linearTerm), -1. / 2.) * linearTerm / std::fabs(linearTerm) : 0.,
                                                    nullptr
                                                );
                                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcClothoidParentCurve);

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
                            }
                        }
                    }
                }
                else if (___equals(predefinedType, (char*) "CONSTANTGRADIENT")) {
                    ___VECTOR2  orientation = {
                                        1.,
                                        0.
                                    };
                    int_t   ifcLineParentCurve =
                                ___CreateLineInstance(
                                        model,
                                        &orientation
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLineParentCurve);

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    double  segmentLength = horizontalLength * std::sqrt(1. + startGradient__ * startGradient__);

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else {
                    assert(___equals(predefinedType, (char*) "PARABOLICARC"));

                    if (startGradient__ == 0. && i) {
                        startGradient__ = pEndGradient[i - 1];
                    }
                    
                    if (endGradient__ == 0. && i < noSegmentInstances - 1) {
                        endGradient__ = pStartGradient[i + 1];
                    }

                    double  startAngle = std::atan(startGradient__),
                            endAngle = std::atan(endGradient__);

                    //
                    //  y = a * x^2
                    //
                    //  direction
                    //      startAngle = 2 * a * x1
                    //      endAngle = 2 * a * x2
                    //      x2 - x1 = horizontalLength
                    //
                    //      horizontalLength = (endAngle - startAngle) / (2 * a)
                    //      a = (endAngle - startAngle) / (2 * horizontalLength)
                    // 
                    //      x1 = startAngle / (2 * a);
                    //

                    double  a = (endAngle - startAngle) / (2. * horizontalLength),
                            offset = startAngle / (2 * a);

                    double  pCoefficientsX[] = { 0., 1. },
                            pCoefficientsY[] = { 0., 0., a };
                    int_t   ifcParabolicArcParentCurve =
                                ___CreatePolynomialCurveInstance(
                                        model,
                                        pCoefficientsX, sizeof(pCoefficientsX) / sizeof(double),
                                        pCoefficientsY, sizeof(pCoefficientsY) / sizeof(double),
                                        nullptr, 0
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcParabolicArcParentCurve);

                    //
                    //  SegmentStart
                    //
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    assert(horizontalLength > 0.);
                    double  segmentLength = horizontalLength;

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }

                sdaiAppend((int_t) aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);
            }
        }

        delete[] pSegmentType;
        delete[] pRadiusOfCurvature;
        delete[] pStartHeight;
        delete[] pEndGradient;
        delete[] pStartGradient;

        delete[] segmentInstances;
    }

    return  ifcGradientCurveInstance;
}

static  inline  int_t   ___GetAlignmentVertical(
                                int_t   model,
                                int_t   ifcAlignmentInstance,
                                bool    * hasIssue
                            )
{
    int_t   ifcAlignmentVerticalInstance = 0;

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

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICAL")) {
                    if (ifcAlignmentVerticalInstance && hasIssue) {
                        (*hasIssue) = true;
                    }

                    assert(ifcAlignmentVerticalInstance == 0);
                    ifcAlignmentVerticalInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    if (ifcAlignmentVerticalInstance == 0) {
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

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICAL")) {
                    assert(ifcAlignmentVerticalInstance == 0);
                    ifcAlignmentVerticalInstance = ifcObjectDefinitionInstance;

                    if (hasIssue) {
                        (*hasIssue) = true;
                    }
                }
            }
        }
    }

    return  ifcAlignmentVerticalInstance;
}
