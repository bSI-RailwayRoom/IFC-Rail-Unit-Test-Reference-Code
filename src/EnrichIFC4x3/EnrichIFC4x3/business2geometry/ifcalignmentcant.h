#pragma once


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
        int_t   ifcAlignmentVerticalSegmentInstance = 0;
        engiGetAggrElement(aggrSegments, i, sdaiINSTANCE, &ifcAlignmentVerticalSegmentInstance);
        assert(sdaiGetInstanceType(ifcAlignmentVerticalSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICALSEGMENT"));

        {
            int_t   ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

/*            //
            //  SegmentLength
            //
            double  segmentLength = 0.;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "SegmentLength", sdaiREAL, &segmentLength);
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
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartPoint", sdaiINSTANCE, &ifcCartesianPointInstance);

            GetCartesianPointCoordinates(ifcCartesianPointInstance, (VECTOR3*) &matrix._41);

            double   startDirection = 0.;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartDirection", sdaiREAL, &startDirection);
            matrix._11 = cos(startDirection);
            matrix._12 = sin(startDirection);

            sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) CreateAxis2Placement2D(model, &matrix));

            //
            //  Parse the individual segments
            //
            char    * predefinedType = nullptr;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
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
            }
            else if (equals(predefinedType, (char*) "CLOTHOID")) {
                double  startRadiusOfCurvature = 0., endRadiusOfCurvature = 0.;
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
                sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);
    //            assert((startRadiusOfCurvature && endRadiusOfCurvature == 0.) || (startRadiusOfCurvature == 0. && endRadiusOfCurvature));

                int_t   ifcClothoidInstance = CreateClothoid(model, startRadiusOfCurvature, endRadiusOfCurvature, segmentLength, ifcCurveSegmentInstance, &segmentLength);
                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcClothoidInstance);
            }
            else if (equals(predefinedType, (char*) "LINE")) {
                int_t   ifcLineInstance = CreateLine(model);
                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLineInstance);
            }
            else {
                assert(false);
            }

            //
            //  SegmentLength
            //
            void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
            sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
            sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
//  */
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
