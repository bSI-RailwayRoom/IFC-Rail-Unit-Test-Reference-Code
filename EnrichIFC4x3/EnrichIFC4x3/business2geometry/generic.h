#pragma once

#include <assert.h>


static	inline	bool    ___equals(
                                char    * txtI,
                                char    * txtII
                            )
{
    if (txtI && txtII) {
        int_t   i = 0;
        while (txtI[i]) {
            if (txtI[i] != txtII[i]) {
                return  false;
            }
            i++;
        }
        if (txtII[i] == 0) {
            return  true;
        }
    }
    return  false;
}

static  inline  int_t   ___GetAlignmentSegments(
                                int_t   model,
                                int_t   ifcAlignmentXyzInstance,
                                int_t   * pAlignmentSegment
                            )
{
    int_t   alignmentSegmentCnt = 0;

    int_t   expressID = internalGetP21Line(ifcAlignmentXyzInstance);

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentXyzInstance, "IsNestedBy", sdaiAGGR, &aggrIfcRelAggregates);
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

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT")) {
                    if (pAlignmentSegment) {
                        pAlignmentSegment[alignmentSegmentCnt] = ifcObjectDefinitionInstance;
                    }
                    alignmentSegmentCnt++;
                }
            }
        }
    }

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentXyzInstance, "IsDecomposedBy", sdaiAGGR, &aggrIfcRelAggregates);
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

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT")) {
                    if (pAlignmentSegment) {
                        pAlignmentSegment[alignmentSegmentCnt] = ifcObjectDefinitionInstance;
                    }
                    alignmentSegmentCnt++;

                    assert(false);  //  should be a nested relations
                }
            }
        }
    }

 //   assert(alignmentSegmentCnt || ifcAlignmentXyzInstance == 0);
    return  alignmentSegmentCnt;
}
