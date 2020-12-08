#pragma once


static  inline  int_t   CreateGradientCurve__alignmentVertical(
                                int_t   model,
                                int_t   ifcVerticalAlignmentInstance
                            )
{
    double  epsilon = 0.0000001;

	int_t	ifcGradientCurveInstance = sdaiCreateInstanceBN(model, "IFCGRADIENTCURVE"),
            * aggrCurveSegment = sdaiCreateAggrBN(ifcGradientCurveInstance, "Segments"),
        	* aggrSegments = nullptr;

    sdaiGetAttrBN(ifcVerticalAlignmentInstance, "Segments", sdaiAGGR, &aggrSegments);
    int_t   noAggrSegments = sdaiGetMemberCount(aggrSegments);
    for (int_t i = 0; i < noAggrSegments; i++) {
        int_t   ifcAlignmentVerticalSegmentInstance = 0;
        engiGetAggrElement(aggrSegments, i, sdaiINSTANCE, &ifcAlignmentVerticalSegmentInstance);
        assert(sdaiGetInstanceType(ifcAlignmentVerticalSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICALSEGMENT"));

        {
            int_t   ifcCurveSegmentInstance = sdaiCreateInstanceBN(model, "IFCCURVESEGMENT");

            //
            //  ENTITY IfcAlignmentVerticalSegment
            //      StartDistAlong      IfcLengthMeasure;
            //      HorizontalLength    IfcPositiveLengthMeasure;
            //      StartHeight         IfcLengthMeasure;
            //      StartGradient       IfcLengthMeasure;
            //      EndGradient         IfcLengthMeasure;
            //      RadiusOfCurvature   OPTIONAL IfcPositiveLengthMeasure;
            //      PredefinedType      IfcAlignmentVerticalSegmentTypeEnum;
            //  END_ENTITY;
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
            assert(horizontalLength > 0.);

            //
            //  StartHeight
            //
            double  startHeight = 0.;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, &startHeight);

            //
            //  StartGradient
            //
            double  startGradient = 0.;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, &startGradient);

            //
            //  EndGradient
            //
            double  endGradient = 0.;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "EndGradient", sdaiREAL, &endGradient);

            //
            //  RadiusOfCurvature
            //
            double  radiusOfCurvature = 0.;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "RadiusOfCurvature", sdaiREAL, &radiusOfCurvature);

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

            matrix._41 = startDistAlong;
            matrix._42 = startHeight;

            sdaiPutAttrBN(ifcCurveSegmentInstance, "Placement", sdaiINSTANCE, (void*) CreateAxis2Placement2D(model, &matrix));

            //
            //  Parse the individual segments
            //      CONSTANTGRADIENT
            //      CIRCULARARC
            //      PARABOLICARC
            //      CLOTHOID
            //
            char    * predefinedType = nullptr;
            sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
            if (equals(predefinedType, (char*) "CIRCULARARC")) {
                assert(startGradient > -Pi && startGradient < Pi && endGradient > -Pi && endGradient < Pi);

                double  radius;
                VECTOR2 origin;
                if (startGradient < endGradient) {
                    //
                    //  Ox = -sin( startGradient ) * radius         Ox = horizontalLength - sin( endGradient ) * radius
                    //  Oy = cos( startGradient ) * radius          Oy = offsetY + cos( endGradient ) * radius
                    //
                    //  horizontalLength = (sin( endGradient ) - sin( startGradient )) * radius
                    //  radius = horizontalLength / (sin( endGradient ) - sin( startGradient ));
                    //
                    radius = horizontalLength / (sin(endGradient) - sin(startGradient));
                    assert(radius > 0. && (radius - std::fabs(radiusOfCurvature) < .05 * radius || radiusOfCurvature == 0.));

                    //
                    //  offsetY = (cos( startGradient ) - cos( endGradient )) * radius;
                    //
                    double  offsetY = (cos(startGradient) - cos(endGradient)) * radius;

                    origin.x = -sin(startGradient) * radius;
                    origin.y = cos(startGradient) * radius;

                    assert(std::fabs(origin.x - (horizontalLength - sin(endGradient) * radius)) < epsilon);
                    assert(std::fabs(origin.y - (offsetY + cos(endGradient) * radius)) < epsilon);

                    startGradient += 3. * Pi / 2.;
                    endGradient += 3 * Pi / 2.;

                    origin.x = -cos(startGradient) * radius;
                    origin.y = -sin(startGradient) * radius;
                }
                else {
                    assert(startGradient > endGradient);
                    assert(radiusOfCurvature < 0.);
                    //
                    //  Ox = sin( startGradient ) * radius         Ox = horizontalLength + sin( endGradient ) * radius
                    //  Oy = -cos( startGradient ) * radius        Oy = offsetY - cos( endGradient ) * radius
                    //
                    //  horizontalLength = (sin( startGradient ) - sin( endGradient )) * radius
                    //  radius = horizontalLength / (sin( startGradient ) - sin( endGradient ));
                    //
                    radius = horizontalLength / (sin(startGradient) - sin(endGradient));
                    assert(radius > 0. && radius - std::fabs(radiusOfCurvature) < .05 * radius);
 
                    //
                    //  offsetY = (cos( endGradient ) - cos( startGradient )) * radius;
                    //
                    double  offsetY = (cos(endGradient) - cos(startGradient)) * radius;

                    origin.x = sin(startGradient) * radius;
                    origin.y = -cos(startGradient) * radius;

                    assert(std::fabs(origin.x - (horizontalLength + sin(endGradient) * radius)) < epsilon);
                    assert(std::fabs(origin.y - (offsetY - cos(endGradient) * radius)) < epsilon);

                    startGradient += Pi / 2.;
                    endGradient += Pi / 2.;

                    origin.x = -cos(startGradient) * radius;
                    origin.y = -sin(startGradient) * radius;
                }

                int_t   ifcCircleInstance = CreateCircle(model, &origin, radius);
                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcCircleInstance);

                //
                //  SegmentStart
                //
                void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &startGradient);
                sdaiPutADBTypePath(segmentStartADB, 1, "IFCPARAMETERVALUE");
                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                double  segmentLengthAsParameter = endGradient - startGradient;
                assert(std::fabs(segmentLengthAsParameter * radius) > horizontalLength);
                //
                //  SegmentLength
                //
                void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLengthAsParameter);
                sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
            }
            else if (equals(predefinedType, (char*) "CLOTHOID")) {
assert(false);
//                int_t   ifcClothoidInstance = CreateClothoid(model, startRadiusOfCurvature, endRadiusOfCurvature, segmentLength, ifcCurveSegmentInstance, &segmentLength);
//                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcClothoidInstance);
            }
            else if (equals(predefinedType, (char*) "CONSTANTGRADIENT")) {
                VECTOR2 orientation = {
                                cos(startGradient),
                                sin(startGradient)
                            };
                int_t   ifcLineInstance = CreateLine(model, &orientation);
                sdaiPutAttrBN(ifcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, (void*) ifcLineInstance);

                //
                //  SegmentStart
                //
                double  offset = 0.;
                void   * segmentStartADB = sdaiCreateADB(sdaiREAL, &offset);
                sdaiPutADBTypePath(segmentStartADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentStart", sdaiADB, (void*) segmentStartADB);

                double  segmentLength =
                            (orientation.x) ?
                                horizontalLength / orientation.x :
                                0.;

                //
                //  SegmentLength
                //
                void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &segmentLength);
                sdaiPutADBTypePath(segmentLengthADB, 1, "IFCPARAMETERVALUE");
                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*)segmentLengthADB);
            }
            else {
assert(false);
                assert(equals(predefinedType, (char*) "PARABOLICARC"));

                void   * segmentLengthADB = sdaiCreateADB(sdaiREAL, &horizontalLength);
                sdaiPutADBTypePath(segmentLengthADB, 1, "IFCNONNEGATIVELENGTHMEASURE");
                sdaiPutAttrBN(ifcCurveSegmentInstance, "SegmentLength", sdaiADB, (void*) segmentLengthADB);
            }

            //
            //  horizontalLength
            //

            sdaiAppend((int_t) aggrCurveSegment, sdaiINSTANCE, (void*) ifcCurveSegmentInstance);
        }
    }

    return  ifcGradientCurveInstance;
}

static  inline  int_t   GetAlignmentVertical(
                                int_t   model,
                                int_t   ifcAlignmentInstance
                            )
{
    int_t   ifcAlignmentVerticalInstance = 0;

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
            }
        }
    }

//    assert(ifcAlignmentVerticalInstance);
    return  ifcAlignmentVerticalInstance;
}
