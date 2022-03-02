#pragma once


#include "ifcunitassignment.h"


static	inline	double  ___GetPlaneAngleUnitConversionFactor(
                                int_t   model
                            )
{
    int_t   * ifcProjectInstances = sdaiGetEntityExtentBN(model, "IFCPROJECT"),
            noIfcProjectInstances = sdaiGetMemberCount(ifcProjectInstances);
    if (noIfcProjectInstances) {
        assert(noIfcProjectInstances == 1);

        int_t   ifcProjectInstance = 0;
        engiGetAggrElement(ifcProjectInstances, 0, sdaiINSTANCE, &ifcProjectInstance);

        int_t   ifcUnitAssignmentInstance = 0;
        sdaiGetAttrBN(ifcProjectInstance, "UnitsInContext", sdaiINSTANCE, &ifcUnitAssignmentInstance);

        return  ___GetPlaneAngleUnitConversionFactor(
                        model,
                        ifcUnitAssignmentInstance
                    );
    }

    assert(false);
    return	1.;
}

static  inline  int_t   ___GetProductRepresentationItem(
                                int_t   ifcProductInstance
                            )
{
    if (ifcProductInstance) {
        int_t   ifcProductRepresentationInstance = 0;
        sdaiGetAttrBN(ifcProductInstance, "Representation", sdaiINSTANCE, &ifcProductRepresentationInstance);
        if (ifcProductRepresentationInstance) {
            int_t   * aggrRepresentations = nullptr;
            sdaiGetAttrBN(ifcProductRepresentationInstance, "Representations", sdaiAGGR, &aggrRepresentations);
            if (aggrRepresentations && sdaiGetMemberCount(aggrRepresentations) == 1) {
                int_t   ifcShapeRepresentationInstance = 0;
                engiGetAggrElement(aggrRepresentations, 0, sdaiINSTANCE, &ifcShapeRepresentationInstance);

                int_t   * aggrItems = nullptr;
                sdaiGetAttrBN(ifcShapeRepresentationInstance, "Items", sdaiAGGR, &aggrItems);
                if (aggrItems && sdaiGetMemberCount(aggrItems) == 1) {
                    int_t   ifcRepresentationItemInstance = 0;
                    engiGetAggrElement(aggrItems, 0, sdaiINSTANCE, &ifcRepresentationItemInstance);

                    return  ifcRepresentationItemInstance;
                }
            }
        }
    }

    assert(false);
    return  0;
}