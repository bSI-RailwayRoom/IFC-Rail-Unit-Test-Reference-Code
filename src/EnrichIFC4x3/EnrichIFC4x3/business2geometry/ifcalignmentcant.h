#pragma once

#include "generic.h"

#include "ifcaxis2placement2d.h"
#include "ifcline.h"


static  inline  int_t   CreateSegmentedReferenceCurve__alignmentCant(
                                int_t   model,
                                int_t   ifcCantAlignmentInstance
                            )
{
    if (ifcCantAlignmentInstance == 0) {
        return  0;
    }

	int_t	ifcSegmentedReferenceCurveInstance = sdaiCreateInstanceBN(model, "IFCSEGMENTEDREFERENCECURVE"),
            * aggrCurveSegment = sdaiCreateAggrBN(ifcSegmentedReferenceCurveInstance, "Segments"),
        	* aggrSegments = nullptr;

    sdaiGetAttrBN(ifcSegmentedReferenceCurveInstance, "Segments", sdaiAGGR, &aggrSegments);
    int_t   noAggrSegments = sdaiGetMemberCount(aggrSegments);
    for (int_t i = 0; i < noAggrSegments; i++) {
        int_t   ifcAlignmentCantSegmentInstance = 0;
        engiGetAggrElement(aggrSegments, i, sdaiINSTANCE, &ifcAlignmentCantSegmentInstance);
        assert(sdaiGetInstanceType(ifcAlignmentCantSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICALSEGMENT"));

        int_t   expressLine = internalGetP21Line(ifcAlignmentCantSegmentInstance);

        {
            int_t   ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

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
            assert(horizontalLength > 0.);

            //
            //  StartCantLeft
            //
            double  startCantLeft = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantLeft", sdaiREAL, &startCantLeft);

            //
            //  EndCantLeft
            //
            double  endCantLeft = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantLeft", sdaiREAL, &endCantLeft);

            //
            //  StartCantRight
            //
            double  startCantRight = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "StartCantRight", sdaiREAL, &startCantRight);

            //
            //  EndCantRight
            //
            double  endCantRight = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "EndCantRight", sdaiREAL, &endCantRight);

            //
            //  SmoothingLength
            //
            double  smoothingLength = 0.;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "SmoothingLength", sdaiREAL, &smoothingLength);

            //
            //  Transition
            //
            char    transitionCode[30] = "CONTSAMEGRADIENTSAMECURVATURE";
            sdaiPutAttrBN(ifcCurveSegmentInstance, "Transition", sdaiENUM, (void*)transitionCode);

            //
            //  Placement
            //
            MATRIX  matrix;
            MatrixIdentity(&matrix);

            matrix._41 = startDistAlong;
            matrix._42 = (startCantLeft + startCantRight) / 2.;

            sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*)CreateAxis2Placement2D(model, &matrix));

            //
            //  Parse the individual segments
            //      CONSTANTCANT
            //      LINEARTRANSITION
            //      BIQUADRATICPARABOLA
            //      BLOSSCURVE
            //      COSINECURVE
            //      SINECURVE
            //      VIENNESEBEND
            //
            char    * predefinedType = nullptr;
            sdaiGetAttrBN(ifcAlignmentCantSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
            if (equals(predefinedType, (char*) "CONSTANTCANT")) {
                int_t   ifcLineInstance = CreateLine(model);
                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLineInstance);

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
            else if (equals(predefinedType, (char*) "LINEARTRANSITION")) {
                VECTOR2 orientation = {
                                horizontalLength,
                                (endCantLeft + endCantRight) / 2. - (startCantLeft + startCantRight) / 2.
                            };
                Vec2Normalize(&orientation);
                int_t   ifcLineInstance = CreateLine(model);
                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLineInstance);

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
            else if (equals(predefinedType, (char*) "BIQUADRATICPARABOLA")) {
                assert(false);
            }
            else if (equals(predefinedType, (char*) "BLOSSCURVE")) {
                assert(false);
            }
            else if (equals(predefinedType, (char*) "COSINECURVE")) {
                assert(false);
            }
            else if (equals(predefinedType, (char*) "SINECURVE")) {
                assert(false);
            }
            else if (equals(predefinedType, (char*) "VIENNESEBEND")) {
                assert(false);
            }
            else {
                assert(false);
            }

            sdaiAppend((int_t) aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);
        }
    }

    return  ifcSegmentedReferenceCurveInstance;
}

inline  static  int_t   GetAlignmentCant(
                                int_t   model,
                                int_t   ifcAlignmentInstance
                            )
{
    int_t   ifcAlignmentCantInstance = 0;

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
            }
        }
    }

    return  ifcAlignmentCantInstance;
}
