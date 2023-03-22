#pragma once


#include "ifcunitassignment.h"


static	inline	double  ___GetPlaneAngleUnitConversionFactor(
                                SdaiModel       model
                            )
{
    SdaiAggr    ifcProjectInstances = sdaiGetEntityExtentBN(model, "IFCPROJECT");
    SdaiInteger noIfcProjectInstances = sdaiGetMemberCount(ifcProjectInstances);
    if (noIfcProjectInstances) {
        assert(noIfcProjectInstances == 1);

        SdaiInstance    ifcProjectInstance = 0;
        sdaiGetAggrByIndex(ifcProjectInstances, 0, sdaiINSTANCE, &ifcProjectInstance);

        SdaiInstance    ifcUnitAssignmentInstance = 0;
        sdaiGetAttrBN(ifcProjectInstance, "UnitsInContext", sdaiINSTANCE, &ifcUnitAssignmentInstance);

        return  ___GetPlaneAngleUnitConversionFactor(
                        model,
                        ifcUnitAssignmentInstance
                    );
    }

    assert(false);
    return	1.;
}

static  inline  SdaiInstance    ___GetProductRepresentationItem(
                                        SdaiInstance    ifcProductInstance
                                    )
{
    if (ifcProductInstance) {
        SdaiInstance    ifcProductRepresentationInstance = 0;
        sdaiGetAttrBN(ifcProductInstance, "Representation", sdaiINSTANCE, &ifcProductRepresentationInstance);
        if (ifcProductRepresentationInstance) {
            SdaiAggr    aggrRepresentations = nullptr;
            sdaiGetAttrBN(ifcProductRepresentationInstance, "Representations", sdaiAGGR, &aggrRepresentations);
            if (aggrRepresentations && sdaiGetMemberCount(aggrRepresentations) == 1) {
                SdaiInstance    ifcShapeRepresentationInstance = 0;
                sdaiGetAggrByIndex(aggrRepresentations, 0, sdaiINSTANCE, &ifcShapeRepresentationInstance);

                SdaiAggr    aggrItems = nullptr;
                sdaiGetAttrBN(ifcShapeRepresentationInstance, "Items", sdaiAGGR, &aggrItems);
                if (aggrItems && sdaiGetMemberCount(aggrItems) == 1) {
                    SdaiInstance    ifcRepresentationItemInstance = 0;
                    sdaiGetAggrByIndex(aggrItems, 0, sdaiINSTANCE, &ifcRepresentationItemInstance);

                    return  ifcRepresentationItemInstance;
                }
            }
        }
    }

    assert(false);
    return  0;
}