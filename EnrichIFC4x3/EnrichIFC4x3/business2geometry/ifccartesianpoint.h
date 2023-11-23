#pragma once


#include "mathematics.h"


static	inline	SdaiInstance    ___CreateCartesianPointInstance_2D(
                                        SdaiModel   model
                                    )
{
    SdaiInstance    ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

    SdaiAggr        aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");

    ___VECTOR2  point = { 0., 0. };
    sdaiAppend(aggrCoordinates, sdaiREAL, &point.u);
	sdaiAppend(aggrCoordinates, sdaiREAL, &point.v);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

static	inline	SdaiInstance	___CreateCartesianPointInstance(
                                        SdaiModel           model,
                                        const ___VECTOR2    * point
                                    )
{
    SdaiInstance    ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

    SdaiAggr        aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");

	sdaiAppend(aggrCoordinates, sdaiREAL, &point->u);
	sdaiAppend(aggrCoordinates, sdaiREAL, &point->v);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

static	inline	SdaiInstance    ___CreateCartesianPointInstance(
                                        SdaiModel           model,
                                        const ___VECTOR3    * point
                                    )
{
    SdaiInstance    ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

	SdaiAggr        aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");

	sdaiAppend(aggrCoordinates, sdaiREAL, &point->x);
	sdaiAppend(aggrCoordinates, sdaiREAL, &point->y);
	sdaiAppend(aggrCoordinates, sdaiREAL, &point->z);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

static	inline	void    ___GetCartesianPointCoordinates(
                                SdaiInstance    ifcCartesianPointInstance,
                                ___VECTOR3      * vec
                            )
{
    SdaiAggr	aggrCoordinates = nullptr;
    sdaiGetAttrBN(ifcCartesianPointInstance, "Coordinates", sdaiAGGR, &aggrCoordinates);
    SdaiInteger noAggrCoordinates = sdaiGetMemberCount(aggrCoordinates);

    if (noAggrCoordinates >= 1) {
        sdaiGetAggrByIndex(aggrCoordinates, 0, sdaiREAL, &vec->x);
    }
    if (noAggrCoordinates >= 2) {
        sdaiGetAggrByIndex(aggrCoordinates, 1, sdaiREAL, &vec->y);
    }
    if (noAggrCoordinates >= 3) {
        sdaiGetAggrByIndex(aggrCoordinates, 2, sdaiREAL, &vec->z);
    }

    assert(noAggrCoordinates == 2 || (noAggrCoordinates == 3 && vec->z == 0.));
}
