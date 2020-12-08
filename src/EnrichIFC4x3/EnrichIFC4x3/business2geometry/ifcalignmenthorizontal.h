#pragma once

#include "generic.h"

#include "ifccircle.h"
#include "ifcclothoid.h"
#include "ifcline.h"


static	const	double		Pi = 3.14159265358979323846;


static  inline   int_t  CreateCompositeCurve__alignmentHorizontal(
                                int_t   model,
                                int_t   ifcHorizontalAlignmentInstance
                            )
{
	int_t	ifcCompositeCurveInstance = sdaiCreateInstanceBN(model, "IFCCOMPOSITECURVE"),
            * aggrCurveSegment = sdaiCreateAggrBN(ifcCompositeCurveInstance, "Segments"),
        	* aggrSegments = nullptr;

    sdaiGetAttrBN(ifcHorizontalAlignmentInstance, "Segments", sdaiAGGR, &aggrSegments);
    int_t   noAggrSegments = sdaiGetMemberCount(aggrSegments);
    for (int_t i = 0; i < noAggrSegments; i++) {
        int_t   ifcAlignmentHorizontalSegmentInstance = 0;
        engiGetAggrElement(aggrSegments, i, sdaiINSTANCE, &ifcAlignmentHorizontalSegmentInstance);
        assert(sdaiGetInstanceType(ifcAlignmentHorizontalSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTHORIZONTALSEGMENT"));

        {
            int_t   ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

            //
            //  SegmentLength
            //
            double  segmentLength = 0.;
            sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "SegmentLength", sdaiREAL, &segmentLength);
            assert(segmentLength > 0.);

            //
            //  Transition
            //
            char    transitionCode[30] = "CONTSAMEGRADIENTSAMECURVATURE";
            sdaiPutAttrBN(ifcCurveSegmentInstance, "Transition", sdaiENUM, (void*) transitionCode);

            //
            //  Placement
            //
            MATRIX  matrix;
            MatrixIdentity(&matrix);

            int_t   ifcCartesianPointInstance = 0;
            sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, &ifcCartesianPointInstance);

            GetCartesianPointCoordinates(ifcCartesianPointInstance, (VECTOR3*) &matrix._41);

            double   startDirection = 0.;
            sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartDirection", sdaiREAL, &startDirection);
            matrix._11 = cos(startDirection);
            matrix._12 = sin(startDirection);

            sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) CreateAxis2Placement2D(model, &matrix));

            //
            //  Parse the individual segments
            //
            char    * predefinedType = nullptr;
            sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
            if (equals(predefinedType, (char*) "CIRCULARARC")) {
                double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);
                assert(startRadiusOfCurvature && startRadiusOfCurvature == endRadiusOfCurvature);

                int_t   ifcCircleInstance = CreateCircle(model, startRadiusOfCurvature);
                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCircleInstance);

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
                    segmentLength = 2. * Pi * segmentLength / std::fabs(startRadiusOfCurvature);
                }

                //
                //  SegmentLength
                //
                void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*)segmentLengthADB);
            }
            else if (equals(predefinedType, (char*) "CLOTHOID")) {
                double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);

                double  offset = 0.;
                int_t   ifcClothoidInstance = CreateClothoid(model, startRadiusOfCurvature, endRadiusOfCurvature, segmentLength, &offset);
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
                sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*)segmentLengthADB);
            }
            else if (equals(predefinedType, (char*) "LINE")) {
                int_t   ifcLineInstance = CreateLine(model);
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
                sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*)segmentLengthADB);
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
                sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*)segmentLengthADB);
            }

            sdaiAppend((int_t) aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);
        }
    }

    return  ifcCompositeCurveInstance;
}

static  inline  int_t   GetAlignmentHorizontal(
                                int_t   model,
                                int_t   ifcAlignmentInstance
                            )
{
    int_t   ifcAlignmentHorizontalInstance = 0;

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
            }
        }
    }

    assert(ifcAlignmentHorizontalInstance);
    return  ifcAlignmentHorizontalInstance;
}
