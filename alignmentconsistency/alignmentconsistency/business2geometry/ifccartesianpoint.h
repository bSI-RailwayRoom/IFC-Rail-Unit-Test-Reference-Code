#pragma once


#include "mathematics.h"


static	inline	int_t	___CreateCartesianPointInstance_2D(
                                int_t       model
                            )
{
	int_t	ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");

    ___VECTOR2  point = { 0., 0. };
    sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point.x);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point.y);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

static	inline	int_t	___CreateCartesianPointInstance(
                                int_t       model,
                                ___VECTOR2  * point
                            )
{
	int_t	ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");

	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->x);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->y);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

static	inline	int_t	___CreateCartesianPointInstance(
                                int_t       model,
                                ___VECTOR3  * point
                            )
{
	int_t	ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");

	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->x);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->y);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->z);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

static	inline	void    ___GetCartesianPointCoordinates(
                                int_t       ifcCartesianPointInstance,
                                ___VECTOR3  * vec
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

    assert(noAggrCoordinates == 2 || (noAggrCoordinates == 3 && vec->z == 0.));
}
