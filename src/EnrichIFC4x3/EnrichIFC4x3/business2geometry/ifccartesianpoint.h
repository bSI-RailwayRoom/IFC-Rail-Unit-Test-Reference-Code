#pragma once

#include "mathematics.h"
#include "ifcengine.h"




static	inline	int_t	CreateCartesianPoint2D(
                                int_t   model
                            )
{
	int_t	ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model,(char*) "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance,(char*) "Coordinates");

    VECTOR2 point = { 0., 0. };
    sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point.x);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point.y);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

static	inline	int_t	CreateCartesianPoint2D(
                                int_t   model,
                                VECTOR2 * point
                            )
{
	int_t	ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model,(char*) "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance,(char*) "Coordinates");

	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->x);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->y);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

static	inline	void    GetCartesianPointCoordinates(
                                int_t   ifcCartesianPointInstance,
                                VECTOR3 * vec
                            )
{
    int_t	* aggrCoordinates = nullptr, noAggrCoordinates;
    sdaiGetAttrBN(ifcCartesianPointInstance, "Coordinates", sdaiAGGR, &aggrCoordinates);
    noAggrCoordinates = sdaiGetMemberCount(aggrCoordinates);

    if (noAggrCoordinates >= 1) {
        engiGetAggrElement(aggrCoordinates, 0, sdaiREAL, &vec->x);
    }
    if (noAggrCoordinates >= 2) {
        engiGetAggrElement(aggrCoordinates, 1, sdaiREAL, &vec->y);
    }
    if (noAggrCoordinates >= 3) {
        engiGetAggrElement(aggrCoordinates, 2, sdaiREAL, &vec->z);
    }

    assert(noAggrCoordinates == 2);
}
