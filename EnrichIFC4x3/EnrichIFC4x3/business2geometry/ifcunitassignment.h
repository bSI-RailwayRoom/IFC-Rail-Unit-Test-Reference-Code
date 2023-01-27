#pragma once


#include "generic.h"
#include "ifcnamedunit.h"


static	inline	double  ___GetPlaneAngleUnitConversionFactor(
                                int_t   model,
                                int_t   ifcUnitAssignmentInstance
                            )
{
    if (sdaiGetInstanceType(ifcUnitAssignmentInstance) == sdaiGetEntity(model, "IFCUNITASSIGNMENT")) {
    	int_t	* aggrUnits = nullptr, noAggrUnits;
        sdaiGetAttrBN(ifcUnitAssignmentInstance, "Units", sdaiAGGR, &aggrUnits);
        noAggrUnits = sdaiGetMemberCount(aggrUnits);

        for (int_t i = 0; i < noAggrUnits; i++) {
            int_t	ifcNamedUnitInstance = 0;
            engiGetAggrElement(aggrUnits, i, sdaiINSTANCE, &ifcNamedUnitInstance);

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
