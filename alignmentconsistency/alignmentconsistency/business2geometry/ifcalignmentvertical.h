#pragma once


#include "generic.h"
#include "mathematics.h"
#include "ifcpolynomialcurve.h"
#include "ifcproductdefinitionshape.h"

#include "../alignmentconsistency/alignmentconsistencyalignmentmirror.h"


extern  double	absEpsilon;


static  inline  SdaiInstance   GetAlignmentSegmentFromVerticalAlignment(SdaiInstance ifcVerticalAlignmentInstance, SdaiInteger index)
{
    SdaiModel   ifcModel = engiGetEntityModel(sdaiGetInstanceType(ifcVerticalAlignmentInstance));

    SdaiAggr    ifcRelNestsAGGR = nullptr;
    sdaiGetAttrBN(ifcVerticalAlignmentInstance, "IsNestedBy", sdaiAGGR, &ifcRelNestsAGGR);

    SdaiInstance    ifcRelNestsInstance = 0;
    sdaiGetAggrByIndex(ifcRelNestsAGGR, 0, sdaiINSTANCE, &ifcRelNestsInstance);

    SdaiAggr ifcAlignmentSegmentAGGR = nullptr;
    sdaiGetAttrBN(ifcRelNestsInstance, "RelatedObjects", sdaiAGGR, &ifcAlignmentSegmentAGGR);

    SdaiInstance    ifcAlignmentSegmentInstance = 0;
    sdaiGetAggrByIndex(ifcAlignmentSegmentAGGR, index, sdaiINSTANCE, &ifcAlignmentSegmentInstance);

    return  ifcAlignmentSegmentInstance;
}

void	assert__error__DERIVED_ISSUE(
				int_t		ifcInstance,
				char		* attributeName,  
                double      foundValue,
                double      expectedValue
			);


enum class enum_segment_type : unsigned char
{
    CIRCULARARC,
	CLOTHOID,
    CONSTANTGRADIENT,
    PARABOLICARC
};

static  inline  int_t   ___SegmentCount__alignmentVertical(
                                SdaiInstance    ifcVerticalAlignmentInstance
                            )
{
	SdaiAggr	aggrSegments = nullptr;

    sdaiGetAttrBN(ifcVerticalAlignmentInstance, "Segments", sdaiAGGR, &aggrSegments);

    return  sdaiGetMemberCount(aggrSegments);
}

static  inline  SdaiInstance    ___CreateGradientCurve__alignmentVertical(
                                        SdaiModel       model,
                                        SdaiInstance    ifcVerticalAlignmentInstance,
                                        double          startDistAlongHorizontalAlignment
                                    )
{
#ifdef _DEBUG
    double  epsilon = 0.0000001;
#endif // _DEBUG

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcVerticalAlignmentInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
        SdaiInstance	ifcGradientCurveInstance = sdaiCreateInstanceBN(model, "IFCGRADIENTCURVE");
	    SdaiAggr        aggrCurveSegment = sdaiCreateAggrBN(ifcGradientCurveInstance, "Segments");
	    char    selfIntersect[2] = "F";
	    sdaiPutAttrBN(ifcGradientCurveInstance, "SelfIntersect", sdaiENUM, (void*) selfIntersect);

        SdaiInstance    * segmentInstances = new SdaiInstance[noSegmentInstances];

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
            SdaiInstance    ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));

            SdaiInstance    ifcAlignmentVerticalSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentVerticalSegmentInstance);

            //
            //  StartHeight
            //
            double  startHeight = 0.;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, &startHeight);
            pStartHeight[i] = startHeight;

            char    * predefinedType = nullptr;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
            if (___equals(predefinedType, "CIRCULARARC")) {
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
            else if (___equals(predefinedType, "CLOTHOID")) {
                pStartGradient[i] = 0.;
                pEndGradient[i] = 0.;

                pRadiusOfCurvature[i] = 0.;
                pSegmentType[i] = enum_segment_type::CLOTHOID;
            }
            else if (___equals(predefinedType, "CONSTANTGRADIENT")) {
                double  startGradient__ = 0.;
                sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, &startGradient__);
//                double  angle = std::atan(startGradient__);
                pStartGradient[i] = startGradient__;
                pEndGradient[i] = startGradient__;

                pRadiusOfCurvature[i] = 0.;
                pSegmentType[i] = enum_segment_type::CONSTANTGRADIENT;
            }
            else {
                assert(___equals(predefinedType, "PARABOLICARC"));

                pStartGradient[i] = 0.;
                pEndGradient[i] = 0.;

                pRadiusOfCurvature[i] = 0.;
                pSegmentType[i] = enum_segment_type::PARABOLICARC;
            }
        }
        
        double          mostRecentStartDistAlong       = 0.,
                        mostRecentHorizontalLength     = 0.,
                        mostRecentEndGradient          = 0.;
        SdaiInstance    mostRecentCurveSegmentInstance = 0;
#ifdef _DEBUG
        ___VECTOR2  mostRecentLocation = { 0., 0. };
#endif // _DEBUG

#ifdef _DEBUG
        ___POINT4D  previousEndPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0.} };
#endif // _DEBUG

        for (int_t i = 0; i < noSegmentInstances; i++) {
            SdaiInstance    ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));

            SdaiInstance    ifcAlignmentVerticalSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentVerticalSegmentInstance);
            assert(sdaiGetInstanceType(ifcAlignmentVerticalSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICALSEGMENT"));

            {
                SdaiInstance    ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

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
					
                assert(ifcCurveSegmentInstance && ifcAlignmentSegmentInstance);
                sdaiPutAttrBN(
                        ifcAlignmentSegmentInstance,
                        "Representation",
                        sdaiINSTANCE,
                        (void*) ___CreateProductDefinitionShapeInstance(
                                        model,
                                        ifcCurveSegmentInstance,
                                        "Segment"
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

                if (___equals(predefinedType, "PARABOLICARC")) {
                    if (startGradient__ == 0. && i) {
                        startGradient__ = pEndGradient[i - 1];
                    }
                    if (endGradient__ == 0. && i < noSegmentInstances - 1) {
                        endGradient__ = pStartGradient[i + 1];
                    }
                }

                mostRecentStartDistAlong   = startDistAlong;
                mostRecentHorizontalLength = horizontalLength;
                mostRecentEndGradient      = endGradient__;

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
#ifdef _DEBUG
                mostRecentLocation.x = location.x;
                mostRecentLocation.y = location.y;
#endif // _DEBUG

                double  heightDeviation = 0.;

                //
                //  Parse the individual segments
                //      CONSTANTGRADIENT
                //      CIRCULARARC
                //      PARABOLICARC
                //      CLOTHOID
                //
                if (___equals(predefinedType, "CIRCULARARC")) {
                    double  startAngle = std::atan(startGradient__),
                            endAngle = std::atan(endGradient__);
                    assert(startAngle > -___Pi && startAngle < ___Pi && endAngle > -___Pi && endAngle < ___Pi);

                    if (radiusOfCurvature &&
                        std::fabs(radiusOfCurvature - pRadiusOfCurvature[i]) > absEpsilon) {

                        int_t   ifcAlignmentSegmentInstance = GetAlignmentSegmentFromVerticalAlignment(ifcVerticalAlignmentInstance, i),
                                ifcAlignmentVerticalSegmentInstance = 0;

                        sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, &ifcAlignmentVerticalSegmentInstance);

                        assert__error__DERIVED_ISSUE(ifcAlignmentVerticalSegmentInstance, "RadiusOfCurvature", radiusOfCurvature, pRadiusOfCurvature[i]);
                    }

                    double      radius;
                    ___VECTOR2  origin;
                    if (startAngle < endAngle) {
 ///                       assert(radiusOfCurvature < 0.);
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
                        //  heightDeviation = (cos( startAngle ) - cos( endAngle )) * radius;
                        //
                        heightDeviation = (cos(startAngle) - cos(endAngle)) * radius;

                        origin.x = -sin(startAngle) * radius;
                        origin.y = cos(startAngle) * radius;

                        assert(std::fabs(origin.x - (horizontalLength - sin(endAngle) * radius)) < epsilon);
                        assert(std::fabs(origin.y - (heightDeviation + cos(endAngle) * radius)) < epsilon);

                        startAngle += 3. * ___Pi / 2.;
                        endAngle += 3. * ___Pi / 2.;

//                        origin.x = -cos(startAngle) * radius;
//                        origin.y = -sin(startAngle) * radius;
                    }
                    else {
                        assert(startAngle > endAngle);
///                        assert(radiusOfCurvature > 0.);
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
                        //  heightDeviation = (cos( endAngle ) - cos( startAngle )) * radius;
                        //
                        heightDeviation = (cos(endAngle) - cos(startAngle)) * radius;

                        origin.x = sin(startAngle) * radius;
                        origin.y = -cos(startAngle) * radius;

                        assert(std::fabs(origin.x - (horizontalLength + sin(endAngle) * radius)) < epsilon);
                        assert(std::fabs(origin.y - (heightDeviation - cos(endAngle) * radius)) < epsilon);

                        startAngle += ___Pi / 2.;
                        endAngle += ___Pi / 2.;

                        origin.x = -cos(startAngle) * radius;
                        origin.y = -sin(startAngle) * radius;
                    }

                    ___MATRIX   myMatrix;
                    ___MatrixIdentity(&myMatrix);
                    myMatrix._41 = origin.x;
                    myMatrix._42 = origin.y;

                    SdaiInstance    ifcCircularArcParentCurve =
                                        ___CreateCircleInstance(
                                                model,
                                                radius,
                                                &myMatrix
                                            );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCircularArcParentCurve);

                    //
                    //  SegmentStart
                    //
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &startAngle);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    double  segmentLengthAsParameter = endAngle - startAngle;
                    assert(std::fabs(segmentLengthAsParameter * radius) > horizontalLength * 0.9999999);
                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLengthAsParameter);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);

                    double  radiusOfCurvature__ = 0.;
                    sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "RadiusOfCurvature", sdaiREAL, &radiusOfCurvature__);

                    if (radiusOfCurvature__ == 0. && radius) {
                        double  radiusOfCurvature = radius;
                        sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "RadiusOfCurvature", sdaiREAL, &radiusOfCurvature);
                    }
                }
                else if (___equals(predefinedType, "CLOTHOID")) {
                    double  startAngle = std::atan(startGradient__),
                            endAngle = std::atan(endGradient__);

                    {
                        //
                        //  new definition where the context defines the radius
                        //
                        double  startRadiusOfCurvature = i ? pRadiusOfCurvature[i - 1] : pRadiusOfCurvature[i];
#ifdef _DEBUG
                        double  endRadiusOfCurvature = (i + 1 < noSegmentInstances) ? pRadiusOfCurvature[i + 1] : pRadiusOfCurvature[i];;
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

                                SdaiInstance    ifcClothoidParentCurve =
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
                else if (___equals(predefinedType, "CONSTANTGRADIENT")) {
                    ___VECTOR2  dir = {
                                        refDirection.x,
                                        refDirection.y
                                    };

                    heightDeviation = refDirection.x ? refDirection.y * horizontalLength / refDirection.x : 0.;

//                    if (horizontalLength) {
                    if (true) {
                        SdaiInstance    ifcLineParentCurve =
                                            ___CreateLineInstance(
                                                    model,
                                                    &dir
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
                        assert(segmentLength >= 0.);

                        //
                        //  SegmentLength
                        //
                        void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                        sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                    }
                    else {
                        assert(i == noSegmentInstances - 1);
                        sdaiDeleteInstance(ifcCurveSegmentInstance);
                        ifcCurveSegmentInstance = 0;
                    }
                }
                else {
                    assert(___equals(predefinedType, "PARABOLICARC"));

                    if (startGradient__ == 0. && i) {
                        startGradient__ = pEndGradient[i - 1];
                    }
                    
                    if (endGradient__ == 0. && i < noSegmentInstances - 1) {
                        endGradient__ = pStartGradient[i + 1];
                    }

//                    double  startAngle = std::atan(startGradient__),
//                            endAngle = std::atan(endGradient__);

                    //
                    //  y = a * x^2 + b * x^1 + c * x^0
                    //    = a * x^2 + b * x   + c
                    //
                    //  direction
                    //      startAngle = 2 * a * x1 + b
                    //      endAngle = 2 * a * x2 + b
                    //      x2 - x1 = horizontalLength
                    //
                    //      horizontalLength = (endAngle - startAngle) / (2 * a)
                    //      a = (endAngle - startAngle) / (2 * horizontalLength)
                    // 
                    //      y' = 2ax + b
                    // 
                    //  start point (x1, y1) where x1 = 0
                    //      startAngle = 2 * a * x1 + b => b = startAngle
                    // 
                    //  (x, y) in x1 => (0, startHeight)
                    //       y = a * x^2 + b * x + c
                    //      startHeight = a * 0.^2 + b * 0. + c => c = startHeight
                    //

//                    double  a = (endAngle - startAngle) / (2. * horizontalLength),
                    double  a = (endGradient__ - startGradient__) / (2. * horizontalLength),
//                            b = startAngle,
                            b = startGradient__,
                            c = startHeight;

                    double  pCoefficientsX[] = { 0., 1. },
                            pCoefficientsY[] = { c, b, a };
                    SdaiInstance    ifcParabolicArcParentCurve =
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
                    double  offset = 0.;
                    void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
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

                    double  radiusOfCurvature__ = 0.;
                    sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "RadiusOfCurvature", sdaiREAL, &radiusOfCurvature__);

                    if (radiusOfCurvature__ == 0. && a) {
                        double  radiusOfCurvature = 1. / (2. * a);
                        sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "RadiusOfCurvature", sdaiREAL, &radiusOfCurvature);
                    }
                }

#ifdef _DEBUG
                if (ifcCurveSegmentInstance) {
                    ___POINT4D  startPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } },
                                endPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } };
                    ___GetBorderPoints(
                            ifcCurveSegmentInstance,
                            sdaiGetEntity(model, "IFCGRADIENTCURVE"),
                            &startPnt,
                            &endPnt
                        );

                    assert(startPnt.point.x == startDistAlong &&
                           startPnt.point.y == startHeight &&
                           startPnt.point.z == 0.);

                    ___VECTOR3  tangent = {
                                        1.,
                                        startGradient__,
                                        0.
                                    };
                    ___Vec3Normalize(&tangent);

                    assert(std::fabs(startPnt.tangent.x - tangent.x) < 0.0000000001 &&
                           std::fabs(startPnt.tangent.y - tangent.y) < 0.0000000001 &&
                           startPnt.tangent.z == 0.);

                    if (i) {
        //                assert(std::fabs(startPnt.point.x - previousEndPnt.point.x < 0.0000000001) &&
        //                       std::fabs(startPnt.point.y - previousEndPnt.point.y < 0.0000000001) &&
        //                       startPnt.point.z == previousEndPnt.point.z);

        //                assert(std::fabs(startPnt.tangent.x - previousEndPnt.tangent.x < 0.00001) &&
        //                       std::fabs(startPnt.tangent.y - previousEndPnt.tangent.y < 0.00001) &&
        //                       startPnt.tangent.z == previousEndPnt.tangent.z); //  */
                    }

                    previousEndPnt = endPnt;
                }
#endif // _DEBUG

                if (ifcCurveSegmentInstance)
                    sdaiAppend(aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);

                if (i == noSegmentInstances - 1) {
                    if (horizontalLength == 0.) {
                        assert(startGradient__ == endGradient__ && heightDeviation == 0.);
                        location.x = startDistAlong - startDistAlongHorizontalAlignment;
                        location.y = startHeight;
                        refDirection.x = 1.;
                        refDirection.y = startGradient__;
                        sdaiPutAttrBN(ifcGradientCurveInstance, "EndPoint", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &location, &refDirection));
                    }
                    else {
                        location.x = startDistAlong - startDistAlongHorizontalAlignment + horizontalLength;
                        location.y = startHeight + heightDeviation;
                        refDirection.x = 1.;
                        refDirection.y = endGradient__;
                        sdaiPutAttrBN(ifcGradientCurveInstance, "EndPoint", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &location, &refDirection));
                    }
                }
                else {
                    assert(horizontalLength > 0.);
                }

                mostRecentCurveSegmentInstance = ifcCurveSegmentInstance;
            }
        }

        delete[] pSegmentType;
        delete[] pRadiusOfCurvature;
        delete[] pStartHeight;
        delete[] pEndGradient;
        delete[] pStartGradient;

        delete[] segmentInstances;

        if (mostRecentCurveSegmentInstance) {
            ___VECTOR2  endPoint = { 0., 0. };

            ___GetEndPoint(
                    model,
                    &endPoint,
#ifdef _DEBUG
                    &mostRecentLocation,
#endif // _DEBUG
                    mostRecentCurveSegmentInstance,
                    sdaiGetInstanceType(ifcGradientCurveInstance)
                );

            ___VECTOR2  refDirection = {
                                1.,
                                mostRecentEndGradient
                            },
                        location = {
                                endPoint.x,
                                endPoint.y
                            };
            assert((mostRecentStartDistAlong + mostRecentHorizontalLength) - startDistAlongHorizontalAlignment == endPoint.x);

            if (mostRecentHorizontalLength == 0.) {
                assert(mostRecentLocation.x == endPoint.x &&
                       mostRecentLocation.y == endPoint.y);
            }

            ___Vec2Normalize(&refDirection);
            sdaiPutAttrBN(ifcGradientCurveInstance, "EndPoint", sdaiINSTANCE, (void*) ___CreateAxis2Placement2DInstance(model, &location, &refDirection));
        }

        return  ifcGradientCurveInstance;
    }
 
    return  0;
}

static  inline  SdaiInstance    ___GetAlignmentVertical(
                                        SdaiModel       model,
                                        SdaiInstance    ifcAlignmentInstance,
                                        bool            * hasIssue
                                    )
{
    SdaiInstance    ifcAlignmentVerticalInstance = 0;

    {
	    SdaiAggr	aggrIfcRelAggregates = nullptr;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsNestedBy", sdaiAGGR, &aggrIfcRelAggregates);
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

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICAL")) {
                    if (ifcAlignmentVerticalInstance && hasIssue)
                        (*hasIssue) = true;

                    assert(ifcAlignmentVerticalInstance == 0);
                    ifcAlignmentVerticalInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    if (ifcAlignmentVerticalInstance == 0) {
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

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICAL")) {
                    assert(ifcAlignmentVerticalInstance == 0);
                    ifcAlignmentVerticalInstance = ifcObjectDefinitionInstance;

                    if (hasIssue)
                        (*hasIssue) = true;
                }
            }
        }
    }

    return  ifcAlignmentVerticalInstance;
}
