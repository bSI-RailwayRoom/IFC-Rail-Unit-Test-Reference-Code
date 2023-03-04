#pragma once

#include <math.h>

#include "generic.h"

#include "ifcaxis2placement3d.h"
#include "ifcclothoid.h"
#include "ifccosinespiral.h"
#include "ifcline.h"
#include "ifcproductdefinitionshape.h"
#include "ifcsecondorderpolynomialspiral.h"
#include "ifcseventhorderpolynomialspiral.h"
#include "ifcsinespiral.h"
#include "ifcthirdorderpolynomialspiral.h"




static	inline		double	PolynomialCoefficientValue__DIFFERENTIAL(
									double				* pCoefficients,
									int_t				coefficientsCard,
									double				t
								)
{
	double	value =
				(coefficientsCard > 1) ?
					pCoefficients[1] :
					0.,
			multiplicator = 1.;
	for (int_t i = 2; i < coefficientsCard; i++) {
		multiplicator *= t;
		value += i * pCoefficients[i] * multiplicator;
	}

	return	value;
}

static	inline		double	GetPolynomialCurveAngle(
									double				* pCoefficientsX,
									int_t				coefficientsXCard,
									double				* pCoefficientsY,
									int_t				coefficientsYCard,
									double  			t
								)
{
    ___VECTOR2  tangent = {
		                PolynomialCoefficientValue__DIFFERENTIAL(
                                pCoefficientsX,
                                coefficientsXCard,
				                t
			                ),
		                PolynomialCoefficientValue__DIFFERENTIAL(
                                pCoefficientsY,
                                coefficientsYCard,
                                t
			                )
                    };

	___Vec2Normalize(
            &tangent
		);

    return   std::atan2(tangent.y, tangent.x);
}

static  inline  int_t   ___SegmentCount__alignmentCant(
                                int_t   ifcCantAlignmentInstance
                            )
{
	int_t	* aggrSegments = nullptr;

    sdaiGetAttrBN(ifcCantAlignmentInstance, "Segments", sdaiAGGR, &aggrSegments);

    return  sdaiGetMemberCount(aggrSegments);
}

static  inline  int_t   ___CreateSegmentedReferenceCurve__alignmentCant(
                                int_t   model,
                                int_t   ifcCantAlignmentInstance,
                                double  startDistAlongHorizontalAlignment
                            )
{
    assert(startDistAlongHorizontalAlignment == 0.);

    if (ifcCantAlignmentInstance == 0) {
        return  0;
    }

    double  railHeadDistance = 0.;
    sdaiGetAttrBN(ifcCantAlignmentInstance, "RailHeadDistance", sdaiREAL, &railHeadDistance);
    if (railHeadDistance == 0.) {
        assert(false);
        railHeadDistance = 1.;
    }

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcCantAlignmentInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
	    int_t	ifcSegmentedReferenceCurveInstance = sdaiCreateInstanceBN(model, "IFCSEGMENTEDREFERENCECURVE"),
	            * aggrCurveSegment = sdaiCreateAggrBN(ifcSegmentedReferenceCurveInstance, "Segments");

	    char    selfIntersect[2] = "F";
	    sdaiPutAttrBN(ifcSegmentedReferenceCurveInstance, "SelfIntersect", sdaiENUM, (void*) selfIntersect);

        int_t   * segmentInstances = new int_t[noSegmentInstances];

        ___GetAlignmentSegments(
                model,
                ifcCantAlignmentInstance,
                segmentInstances
            );

        double  mostRecentStartDistAlong = 0.,
                mostRecentLength         = 0.,
                mostRecentendCantLeft    = 0.,
                mostRecentendCantRight   = 0.;

        ___POINT4D  previousEndPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } };

        for (int_t i = 0; i < noSegmentInstances; i++) {
            int_t   ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));

            int_t   ifcAlignmentCantSegmentInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) &ifcAlignmentCantSegmentInstance);
            assert(sdaiGetInstanceType(ifcAlignmentCantSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTCANTSEGMENT"));

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
					
                assert(ifcCurveSegmentInstance && ifcAlignmentSegmentInstance);
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
                //  ENTITY IfcAlignmentCantSegment
                //      StartDistAlong      IfcLengthMeasure
                //      HorizontalLength    IfcPositiveLengthMeasure
                //      StartCantLeft       IfcLengthMeasure
                //      EndCantLeft         OPTIONAL IfcLengthMeasure
                //      StartCantRight      IfcLengthMeasure
                //      EndCantRight        OPTIONAL IfcLengthMeasure
                //      SmoothingLength     OPTIONAL IfcPositiveLengthMeasure
                //      PredefinedType      IfcAlignmentCantSegmentTypeEnum
                //  END_ENTITY
                //

                //
                //  StartDistAlong
                //
                double  startDistAlong = 0.;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartDistAlong", sdaiREAL, &startDistAlong);

                //
                //  HorizontalLength
                //
                double  horizontalLength = 0.;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "HorizontalLength", sdaiREAL, &horizontalLength);
                assert(horizontalLength >= 0.);

                //
                //  StartCantLeft
                //
                double  startCantLeft = 0.;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantLeft", sdaiREAL, &startCantLeft);

                //
                //  EndCantLeft
                //
                double  endCantLeft = startCantLeft;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantLeft", sdaiREAL, &endCantLeft);

                //
                //  StartCantRight
                //
                double  startCantRight = 0.;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantRight", sdaiREAL, &startCantRight);

                //
                //  EndCantRight
                //
                double  endCantRight = startCantRight;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantRight", sdaiREAL, &endCantRight);

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

                mostRecentStartDistAlong = startDistAlong;
                mostRecentLength         = horizontalLength;
                mostRecentendCantLeft    = endCantLeft;
                mostRecentendCantRight   = endCantRight;

                //
                //  Placement
                //
                ___MATRIX  matrix;
                ___MatrixIdentity(&matrix);

                matrix._41 = startDistAlong - startDistAlongHorizontalAlignment;
                matrix._42 = (startCantLeft + startCantRight) / 2.;
                matrix._43 = 0.;

                if (horizontalLength) {
                    matrix._11 =  horizontalLength;
                    matrix._12 = (endCantLeft + endCantRight) / 2. - (startCantLeft + startCantRight) / 2.;
                    matrix._13 =  0.;
                    ___Vec3Normalize((___VECTOR3*) &matrix._11);
                }
                else {
                    matrix._11 = 1.;
                    matrix._12 = 0.;
                    matrix._13 = 0.;
                }

                matrix._21 = - matrix._12;
                matrix._22 =   matrix._11;

                double  _factor = - (startCantLeft - startCantRight);
                matrix._31 = - _factor * matrix._12;
                matrix._32 = _factor * matrix._11;
                matrix._33 = railHeadDistance;
                ___Vec3Normalize((___VECTOR3*) &matrix._31);

                ___Vec3Cross((___VECTOR3*) &matrix._21, (___VECTOR3*) &matrix._31, (___VECTOR3*) &matrix._11);

                sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model, &matrix));

                if ((horizontalLength == 0.) &&
                    (i == noSegmentInstances - 1)) {
                    sdaiPutAttrBN(ifcSegmentedReferenceCurveInstance, "EndPoint", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model, &matrix));
                }
                else {
                    assert(horizontalLength > 0.);
                }

                //
                //  Parse the individual segments
                //      BLOSSCURVE
                //      CONSTANTCANT
                //      COSINECURVE
                //      LINEARTRANSITION
                //      HELMERTCURVE
                //      SINECURVE
                //      VIENNESEBEND
                //

                char    * predefinedType = nullptr;
                sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
                if (___equals(predefinedType, "CONSTANTCANT")) {
                    ___VECTOR2  pnt = { 0., (startCantLeft + startCantRight) / 2. },
								dir = { 1., 0. };
                    int_t   ifcConstantCantParentCurve =
                                ___CreateLineInstance(
                                        model,
                                        &pnt,
                                        &dir
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcConstantCantParentCurve);

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    double  segmentLength = horizontalLength;

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*)segmentLengthADB);
                }
                else if (___equals(predefinedType, "LINEARTRANSITION")) {
                    double  factor =
                                ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) ?
                                    ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) / 2. :
                                    1.,
                            constantTerm  =   0. * factor + (startCantLeft + startCantRight) / 2.,
                            linearTerm    =   1. * factor;

                    ___MATRIX   myMatrix;
                    ___MatrixIdentity(&myMatrix);
                    myMatrix._42 = (startCantLeft + startCantRight) / (2. * horizontalLength);

                    int_t   ifcLinearTransitionParentCurve =
                                ___CreateClothoidInstance(
                                        model,
                                        linearTerm ? horizontalLength * pow(std::fabs(linearTerm), -1. / 2.) * linearTerm / std::fabs(linearTerm) : 0.,
                                        &myMatrix
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLinearTransitionParentCurve);

                    //
                    //  SegmentStart
                    //
                    double  offset = 0.;
                    void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    double  segmentLength = horizontalLength;

                    //
                    //  SegmentLength
                    //
                    void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "BLOSSCURVE")) {
                    matrix._11 = 1.;
                    matrix._12 = 0.;
                    matrix._13 = 0.;

                    double  factor__ = -(startCantLeft - startCantRight);
                    matrix._31 = - factor__ * matrix._12;
                    matrix._32 = factor__ * matrix._11;
                    matrix._33 = railHeadDistance;
                    ___Vec3Normalize((___VECTOR3*) &matrix._31);

                    ___Vec3Cross((___VECTOR3*) &matrix._21, (___VECTOR3*) &matrix._31, (___VECTOR3*) &matrix._11);

                    sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model, &matrix));

                    double  factor =
                                ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) ?
                                    ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) / 2. :
                                    1.,
                            constantTerm  =   0. * factor + (startCantLeft + startCantRight) / 2.,
                            linearTerm    =   0. * factor,
                            quadraticTerm =   3. * factor,
                            cubicTerm     = - 2. * factor;

                    int_t   ifcBlossCurveParentCurve =
                                ___CreateThirdOrderPolynomialSpiralInstance(
                                        model,
                                        cubicTerm     ? horizontalLength * pow(std::fabs(cubicTerm),     -1. / 4.) * cubicTerm     / std::fabs(cubicTerm)     : 0.,
                                        quadraticTerm ? horizontalLength * pow(std::fabs(quadraticTerm), -1. / 3.) * quadraticTerm / std::fabs(quadraticTerm) : 0.,
                                        linearTerm    ? horizontalLength * pow(std::fabs(linearTerm),    -1. / 2.) * linearTerm    / std::fabs(linearTerm)    : 0.,
                                        constantTerm  ? horizontalLength * pow(std::fabs(constantTerm),  -1. / 1.) * constantTerm  / std::fabs(constantTerm)  : 0.,
                                        nullptr
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcBlossCurveParentCurve);

                    //
                    //  SegmentStart
                    //
                    double  offsetAsParameter = 0.;
                    void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offsetAsParameter);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void    * segmentLengthADB = sdaiCreateADB(sdaiREAL, &horizontalLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "COSINECURVE")) {
                    matrix._11 = 1.;
                    matrix._12 = 0.;
                    matrix._13 = 0.;

                    double  factor__ = -(startCantLeft - startCantRight);
                    matrix._31 = - factor__ * matrix._12;
                    matrix._32 = factor__ * matrix._11;
                    matrix._33 = railHeadDistance;
                    ___Vec3Normalize((___VECTOR3*) &matrix._31);

                    ___Vec3Cross((___VECTOR3*) &matrix._21, (___VECTOR3*) &matrix._31, (___VECTOR3*) &matrix._11);

                    sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model, &matrix));

                    double  factor =
                                ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) ?
                                    ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) / 2. :
                                    1.,
                            cosineTerm   = - 0.5 * factor,
                            constantTerm =   0.5 * factor + (startCantLeft + startCantRight) / 2.;

                    int_t   ifcCosineCurveParentCurve =
                                ___CreateCosineSpiralInstance(
                                        model,
                                        constantTerm ? horizontalLength * pow(std::fabs(constantTerm), -1. / 1.) * constantTerm / std::fabs(constantTerm) : 0.,
                                        cosineTerm   ? horizontalLength * pow(std::fabs(cosineTerm),   -1. / 1.) * cosineTerm   / std::fabs(cosineTerm)   : 0.,
                                        nullptr
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCosineCurveParentCurve);

                    //
                    //  SegmentStart
                    //
                    double  offsetAsParameter = 0.;
                    void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offsetAsParameter);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void    * segmentLengthADB = sdaiCreateADB(sdaiREAL, &horizontalLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "SINECURVE")) {
                    matrix._11 = 1.;
                    matrix._12 = 0.;
                    matrix._13 = 0.;

                    double  factor__ = -(startCantLeft - startCantRight);
                    matrix._31 = - factor__ * matrix._12;
                    matrix._32 = factor__ * matrix._11;
                    matrix._33 = railHeadDistance;
                    ___Vec3Normalize((___VECTOR3*) &matrix._31);

                    ___Vec3Cross((___VECTOR3*) &matrix._21, (___VECTOR3*) &matrix._31, (___VECTOR3*) &matrix._11);

                    sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model, &matrix));

                    double  factor =
                                ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) ?
                                    ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) / 2. :
                                    1.,
                            sineTerm     = - (1. / (2. * Pi)) * factor,
                            constantTerm =    0.              * factor + (startCantLeft + startCantRight) / 2.,
                            linearTerm   =    1.              * factor;

                    int_t   ifcSineCurveParentCurve =
                                ___CreateSineSpiralInstance(
                                        model,
                                        linearTerm   ? horizontalLength * pow(std::fabs(linearTerm),   -1. / 2.) * linearTerm   / std::fabs(linearTerm)   : 0.,
                                        constantTerm ? horizontalLength * pow(std::fabs(constantTerm), -1. / 1.) * constantTerm / std::fabs(constantTerm) : 0.,
                                        sineTerm     ? horizontalLength * pow(std::fabs(sineTerm),     -1. / 1.) * sineTerm     / std::fabs(sineTerm)     : 0.,
                                        nullptr
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcSineCurveParentCurve);

                    //
                    //  SegmentStart
                    //
                    double  offsetAsParameter = 0.;
                    void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offsetAsParameter);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void    * segmentLengthADB = sdaiCreateADB(sdaiREAL, &horizontalLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "VIENNESEBEND")) {
                    matrix._11 = 1.;
                    matrix._12 = 0.;
                    matrix._13 = 0.;

                    double  factor__ = -(startCantLeft - startCantRight);
                    matrix._31 = - factor__ * matrix._12;
                    matrix._32 = factor__ * matrix._11;
                    matrix._33 = railHeadDistance;
                    ___Vec3Normalize((___VECTOR3*) &matrix._31);

                    ___Vec3Cross((___VECTOR3*) &matrix._21, (___VECTOR3*) &matrix._31, (___VECTOR3*) &matrix._11);

                    sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model, &matrix));

                    assert(horizontalLength > 0.);
                    double  factor =
                                ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) ?
                                    ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) / 2. :
                                    1.,
                            constantTerm  =    0. * factor + (startCantLeft + startCantRight) / 2.,
                            linearTerm    =    0. * factor,
                            quadraticTerm =    0. * factor,
                            cubicTerm     =    0. * factor,
                            quarticTerm   =   35. * factor,
                            quinticTerm   = - 84. * factor,
                            sexticTerm    =   70. * factor,
                            septicTerm    = - 20. * factor;

                    int_t   ifcVienneseBendParentCurve =
                                ___CreateSeventhOrderPolynomialSpiralInstance(
                                        model,
                                        septicTerm    ? horizontalLength * pow(std::fabs(septicTerm),    -1. / 8.) * septicTerm    / std::fabs(septicTerm)    : 0.,
                                        sexticTerm    ? horizontalLength * pow(std::fabs(sexticTerm),    -1. / 7.) * sexticTerm    / std::fabs(sexticTerm)    : 0.,
                                        quinticTerm   ? horizontalLength * pow(std::fabs(quinticTerm),   -1. / 6.) * quinticTerm   / std::fabs(quinticTerm)   : 0.,
                                        quarticTerm   ? horizontalLength * pow(std::fabs(quarticTerm),   -1. / 5.) * quarticTerm   / std::fabs(quarticTerm)   : 0.,
                                        cubicTerm     ? horizontalLength * pow(std::fabs(cubicTerm),     -1. / 4.) * cubicTerm     / std::fabs(cubicTerm)     : 0.,
                                        quadraticTerm ? horizontalLength * pow(std::fabs(quadraticTerm), -1. / 3.) * quadraticTerm / std::fabs(quadraticTerm) : 0.,
                                        linearTerm    ? horizontalLength * pow(std::fabs(linearTerm),    -1. / 2.) * linearTerm    / std::fabs(linearTerm)    : 0.,
                                        constantTerm  ? horizontalLength * pow(std::fabs(constantTerm),  -1. / 1.) * constantTerm  / std::fabs(constantTerm)  : 0.,
                                        nullptr
                                    );
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcVienneseBendParentCurve);

                    //
                    //  SegmentStart
                    //
                    double  offsetAsParameter = 0.;
                    void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offsetAsParameter);
                    sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                    //
                    //  SegmentLength
                    //
                    void    * segmentLengthADB = sdaiCreateADB(sdaiREAL, &horizontalLength);
                    sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                    sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                }
                else if (___equals(predefinedType, "HELMERTCURVE")) {
                    double  factor =
                                ((endCantRight + endCantLeft) - (startCantRight + startCantLeft)) ?
                                    ((endCantRight + endCantLeft) - (startCantRight + startCantLeft)) / 1. :
                                    1.,
                            constantTermFirstHalf   =   0. * factor + (startCantRight + startCantLeft) / 1.,
                            linearTermFirstHalf     =   0. * factor,
                            quadraticTermFirstHalf  =   2. * factor,
                            constantTermSecondHalf  = - 1. * factor + (startCantRight + startCantLeft) / 1.,
                            linearTermSecondHalf    =   4. * factor,
                            quadraticTermSecondHalf = - 2. * factor;

                    {
                        matrix._11 = 1.;
                        matrix._12 = 0.;
                        matrix._13 = 0.;

                        double  factor__ = (startCantRight - startCantLeft);
                        matrix._31 = - factor__ * matrix._12;
                        matrix._32 = factor__ * matrix._11;
                        matrix._33 = railHeadDistance;
                        ___Vec3Normalize((___VECTOR3*) &matrix._31);

                        ___Vec3Cross((___VECTOR3*) &matrix._21, (___VECTOR3*) &matrix._31, (___VECTOR3*) &matrix._11);

                        sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model, &matrix));

                        int_t   ifcHelmertCurveParentCurve =
                                    ___CreatSecondOrderPolynomialSpiralInstance(
                                            model,
                                            quadraticTermFirstHalf ? horizontalLength * pow(std::fabs(quadraticTermFirstHalf), -1. / 3.) * quadraticTermFirstHalf / std::fabs(quadraticTermFirstHalf) : 0.,
                                            linearTermFirstHalf    ? horizontalLength * pow(std::fabs(linearTermFirstHalf),    -1. / 2.) * linearTermFirstHalf    / std::fabs(linearTermFirstHalf)    : 0.,
                                            constantTermFirstHalf  ? horizontalLength * pow(std::fabs(constantTermFirstHalf),  -1. / 1.) * constantTermFirstHalf  / std::fabs(constantTermFirstHalf)  : 0.,
                                            nullptr
                                        );
                        sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcHelmertCurveParentCurve);

                        //
                        //  SegmentStart
                        //
                        double  offsetAsParameter = 0.;
                        void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offsetAsParameter);
                        sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                        sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                        //
                        //  SegmentLength
                        //
                        double  segmentLengthAsLength = horizontalLength / 2.;
                        void    * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLengthAsLength);
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                        sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);

                        sdaiAppend((int_t) aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);
                    }



                    {
                        double  quadraticTerm = quadraticTermFirstHalf ? horizontalLength * pow(std::fabs(quadraticTermFirstHalf), -1. / 3.) * quadraticTermFirstHalf / std::fabs(quadraticTermFirstHalf) : 0.,
                                linearTerm    = linearTermFirstHalf    ? horizontalLength * pow(std::fabs(linearTermFirstHalf),    -1. / 2.) * linearTermFirstHalf    / std::fabs(linearTermFirstHalf)    : 0.,
                                constantTerm  = constantTermFirstHalf  ? horizontalLength * pow(std::fabs(constantTermFirstHalf),  -1. / 1.) * constantTermFirstHalf  / std::fabs(constantTermFirstHalf)  : 0.;
                     
                        double	FACTOR = horizontalLength / 2.;
                        quadraticTerm /= FACTOR;
                        linearTerm    /= FACTOR;
                        constantTerm  /= FACTOR;

                        {
		                    double	iFACTOR = 1. / FACTOR;

		                    const	int_t	coefficientsXCard = 2;
		                    double	pCoefficientsX[coefficientsXCard] = {
						                    0,
						                    1.
					                    };

		                    const	int_t	coefficientsYCard = 3;
		                    double	pCoefficientsY[coefficientsYCard] = {
						                    constantTerm ?
							                    iFACTOR / (constantTerm) :
							                    0.,
						                    linearTerm ?
							                    iFACTOR * linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
							                    0.,
						                    quadraticTerm ?
							                    iFACTOR / (quadraticTerm * quadraticTerm * quadraticTerm) :
							                    0.
					                    };

                            double  angle =
                                        GetPolynomialCurveAngle(
				                            pCoefficientsX,
				                            coefficientsXCard,
				                            pCoefficientsY,
				                            coefficientsYCard,
				                            0.5
			                            );

                            matrix._11 = cos(2. * angle);
                            matrix._12 = sin(2. * angle);
                            matrix._13 = 0.;
                        }
                    }

                    {
                        ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

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
                                                false
                                            )
                            );
                    }

                    matrix._41 += horizontalLength / 2.;
                    matrix._42 += ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) ? 
                                    ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) / 4. :
                                    0.5 ;

                    {

        //                matrix._11 = 1.;
        //                matrix._12 = 0.;
        //                matrix._13 = 0.;

                        double  factor__ = (startCantLeft - startCantRight) + ((endCantLeft + endCantRight) - (startCantLeft + startCantRight)) / 2.;
                        matrix._31 = - factor__ * matrix._12;
                        matrix._32 = factor__ * matrix._11;
                        matrix._33 = railHeadDistance;
                        ___Vec3Normalize((___VECTOR3*) &matrix._31);

                        ___Vec3Cross((___VECTOR3*) &matrix._21, (___VECTOR3*) &matrix._31, (___VECTOR3*) &matrix._11);

                        sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model, &matrix));

                        int_t   ifcHelmertCurveInstance =
                                    ___CreatSecondOrderPolynomialSpiralInstance(
                                            model,
                                            quadraticTermSecondHalf ? horizontalLength * pow(std::fabs(quadraticTermSecondHalf), -1. / 3.) * quadraticTermSecondHalf / std::fabs(quadraticTermSecondHalf) : 0.,
                                            linearTermSecondHalf    ? horizontalLength * pow(std::fabs(linearTermSecondHalf),    -1. / 2.) * linearTermSecondHalf    / std::fabs(linearTermSecondHalf)    : 0.,
                                            constantTermSecondHalf  ? horizontalLength * pow(std::fabs(constantTermSecondHalf),  -1. / 1.) * constantTermSecondHalf  / std::fabs(constantTermSecondHalf)  : 0.,
                                            nullptr
                                        );
                        sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcHelmertCurveInstance);

                        //
                        //  SegmentStart
                        //
                        double  offsetAsParameter = horizontalLength / 2.;
                        void    * segmentStartADB = sdaiCreateADB(sdaiREAL, &offsetAsParameter);
                        sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                        sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                        //
                        //  SegmentLength
                        //
                        double  segmentLengthAsLength = horizontalLength / 2.;
                        void    * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLengthAsLength);
                        sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                        sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
                    }
                }
                else {
                    assert(false);
                }

                sdaiAppend((int_t) aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);

#ifdef _DEBUG
                ___POINT4D  startPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } },
                            endPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } };
                ___GetBorderPoints(
                        ifcCurveSegmentInstance,
                        sdaiGetEntity(model, "IFCSEGMENTEDREFERENCECURVE"),
                        &startPnt,
                        &endPnt
                    );
//                ...
double  minDist = ___Vec3Distance(&startPnt.point, &endPnt.point);

                assert(startPnt.point.x == matrix._41 &&
                       startPnt.point.y == matrix._42 &&
                       startPnt.point.z == matrix._43);

                ___VECTOR3  tangent = {
                                    matrix._11,
                                    matrix._12,
                                    0.
                                };
                assert(matrix._13 == 0.);

                assert(std::fabs(startPnt.tangent.x - tangent.x) < 0.0000000001 &&
                       std::fabs(startPnt.tangent.y - tangent.y) < 0.0000000001 &&
                       startPnt.tangent.z == 0.);

                if (i) {
                    assert((std::fabs(startPnt.point.x - previousEndPnt.point.x < 0.001) &&
                            std::fabs(startPnt.point.y - previousEndPnt.point.y < 0.001) &&
                            startPnt.point.z == previousEndPnt.point.z));

/*                    assert(std::fabs(startPnt.tangent.x - previousEndPnt.tangent.x < 0.0005) &&
                           std::fabs(startPnt.tangent.y - previousEndPnt.tangent.y < 0.0005) &&
                           startPnt.tangent.z == previousEndPnt.tangent.z); //  */
                }

                previousEndPnt = endPnt;
#endif // _DEBUG
            }
        }

        delete[] segmentInstances;

        {
            double  startCantLeft = mostRecentendCantLeft,
                    startCantRight = mostRecentendCantRight;

            //
            //  Placement
            //
            ___MATRIX  matrix;
            ___MatrixIdentity(&matrix);

            matrix._41 = (mostRecentStartDistAlong + mostRecentLength) - startDistAlongHorizontalAlignment;
            matrix._42 = (startCantLeft + startCantRight) / 2.;
            matrix._43 = 0.;

            matrix._11 = 1.;
            matrix._12 = 0.;
            matrix._13 = 0.;

            matrix._21 = -matrix._12;
            matrix._22 = matrix._11;

            double  _factor = -(startCantLeft - startCantRight);
            matrix._31 = - _factor * matrix._12;
            matrix._32 =   _factor * matrix._11;
            matrix._33 =   railHeadDistance;
            ___Vec3Normalize((___VECTOR3*) &matrix._31);

            ___Vec3Cross((___VECTOR3*) &matrix._21, (___VECTOR3*) &matrix._31, (___VECTOR3*) &matrix._11);

            sdaiPutAttrBN(ifcSegmentedReferenceCurveInstance, "EndPoint", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model, &matrix));
        }

        return  ifcSegmentedReferenceCurveInstance;
    }

    return  0;
}

inline  static  int_t   ___GetAlignmentCant(
                                int_t   model,
                                int_t   ifcAlignmentInstance,
                                bool    * hasIssue
                            )
{
    int_t   ifcAlignmentCantInstance = 0;

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

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTCANT")) {
                    if (ifcAlignmentCantInstance && hasIssue) {
                        (*hasIssue) = true;
                    }

                    assert(ifcAlignmentCantInstance == 0);
                    ifcAlignmentCantInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    if (ifcAlignmentCantInstance == 0) {
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

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTCANT")) {
                    assert(ifcAlignmentCantInstance == 0);
                    ifcAlignmentCantInstance = ifcObjectDefinitionInstance;

                    if (hasIssue) {
                        (*hasIssue) = true;
                    }
                }
            }
        }
    }

    return  ifcAlignmentCantInstance;
}
