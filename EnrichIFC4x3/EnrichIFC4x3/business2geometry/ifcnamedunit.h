#pragma once


#include "ifcengine.h"


static	inline	double  ___GetConversionFactor(
                                int_t   model,
                                int_t   ifcMeasureWithUnitInstance
                            )
{
    if (sdaiGetInstanceType(ifcMeasureWithUnitInstance) == sdaiGetEntity(model, "IFCMEASUREWITHUNIT")) {
        int_t   * valueComponentADB = nullptr;
        sdaiGetAttrBN(ifcMeasureWithUnitInstance, "ValueComponent", sdaiADB, &valueComponentADB);

        double  valueComponent = 0.;
        sdaiGetADBValue(valueComponentADB, sdaiREAL, (void*) &valueComponent);

        if (valueComponent) {
            return  valueComponent;
        }
    }

    assert(false);
    return	1.;
}

static	inline	double  ___GetUnitConversionFactor(
                                int_t   model,
                                int_t   ifcNamedUnitInstance
                            )
{
    if (sdaiGetInstanceType(ifcNamedUnitInstance) == sdaiGetEntity(model, "IFCSIUNIT")) {
        return   1.;
    }
    else if (sdaiGetInstanceType(ifcNamedUnitInstance) == sdaiGetEntity(model, "IFCCONVERSIONBASEDUNIT")) {
        int_t   ifcMeasureWithUnitInstance = 0;
        sdaiGetAttrBN(ifcNamedUnitInstance, "ConversionFactor", sdaiINSTANCE, &ifcMeasureWithUnitInstance);

        return  ___GetConversionFactor(
                        model,
                        ifcMeasureWithUnitInstance
                    );
    }

    assert(false);
    return	1.;
}
