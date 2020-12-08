#pragma once

#include "mathematics.h"
#include "ifcengine.h"


static	inline	int_t	CreateDirection(
							   int_t   model,
							   VECTOR2 * point
							)
{
	int_t	ifcDirectionInstance, * aggrDirectionRatios;

	ifcDirectionInstance = sdaiCreateInstanceBN(model, "IFCDIRECTION");

	aggrDirectionRatios = sdaiCreateAggrBN(ifcDirectionInstance, "DirectionRatios");

	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &point->x);
	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &point->y);

	assert(ifcDirectionInstance);

	return	ifcDirectionInstance;
}
