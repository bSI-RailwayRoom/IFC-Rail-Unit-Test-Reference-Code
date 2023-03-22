#pragma once


#include "generic.h"
#include "ifcnamedunit.h"


static	inline	double  ___GetPlaneAngleUnitConversionFactor(
                                SdaiModel       model,
                                SdaiInstance    ifcUnitAssignmentInstance
                            )
{
    if (sdaiGetInstanceType(ifcUnitAssignmentInstance) == sdaiGetEntity(model, "IFCUNITASSIGNMENT")) {
        SdaiAggr    aggrUnits = nullptr;
        sdaiGetAttrBN(ifcUnitAssignmentInstance, "Units", sdaiAGGR, &aggrUnits);
        SdaiInteger noAggrUnits = sdaiGetMemberCount(aggrUnits);

        for (int_t i = 0; i < noAggrUnits; i++) {
            int_t	ifcNamedUnitInstance = 0;
            sdaiGetAggrByIndex(aggrUnits, i, sdaiINSTANCE, &ifcNamedUnitInstance);

            char    * unitType = nullptr;
            sdaiGetAttrBN(ifcNamedUnitInstance, "UnitType", sdaiENUM, &unitType);
            if (___equals(unitType, "PLANEANGLEUNIT")) {
                return  ___GetUnitConversionFactor(
                                model,
                                ifcNamedUnitInstance
                            );
            }
        }
    }

    assert(false);
    return	1.;
}
