#pragma once


#include "mathematics.h"
#include "ifcengine.h"


static	inline	int_t	___CreateDirectionInstance(
								int_t		model,
								___VECTOR2	* vector
							)
{
	int_t	ifcDirectionInstance, * aggrDirectionRatios;

	ifcDirectionInstance = sdaiCreateInstanceBN(model, "IFCDIRECTION");

	aggrDirectionRatios = sdaiCreateAggrBN(ifcDirectionInstance, "DirectionRatios");

	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &vector->x);
	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &vector->y);

	assert(ifcDirectionInstance);

	return	ifcDirectionInstance;
}

static	inline	int_t	___CreateDirectionInstance(
								int_t		model,
								___VECTOR3	* vector
							)
{
	int_t	ifcDirectionInstance, * aggrDirectionRatios;

	ifcDirectionInstance = sdaiCreateInstanceBN(model, "IFCDIRECTION");

	if (internalGetP21Line(ifcDirectionInstance) == 758) {
		int u = 0;
	}

	aggrDirectionRatios = sdaiCreateAggrBN(ifcDirectionInstance, "DirectionRatios");

	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &vector->x);
	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &vector->y);
	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &vector->z);

	assert(ifcDirectionInstance);

	return	ifcDirectionInstance;
}
