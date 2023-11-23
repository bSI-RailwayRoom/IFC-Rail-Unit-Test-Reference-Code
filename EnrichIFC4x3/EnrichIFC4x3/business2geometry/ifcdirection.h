#pragma once


#include "mathematics.h"


static	inline	SdaiInstance	___CreateDirectionInstance(
										SdaiModel			model,
										const ___VECTOR2	* vector
									)
{
	SdaiInstance	ifcDirectionInstance = sdaiCreateInstanceBN(model, "IFCDIRECTION");

	SdaiAggr		aggrDirectionRatios = sdaiCreateAggrBN(ifcDirectionInstance, "DirectionRatios");

	sdaiAppend(aggrDirectionRatios, sdaiREAL, &vector->u);
	sdaiAppend(aggrDirectionRatios, sdaiREAL, &vector->v);

	assert(ifcDirectionInstance);

	return	ifcDirectionInstance;
}

static	inline	SdaiInstance	___CreateDirectionInstance(
										SdaiModel			model,
										const ___VECTOR3	* vector
									)
{
	SdaiInstance	ifcDirectionInstance = sdaiCreateInstanceBN(model, "IFCDIRECTION");

	SdaiAggr		aggrDirectionRatios = sdaiCreateAggrBN(ifcDirectionInstance, "DirectionRatios");

	sdaiAppend(aggrDirectionRatios, sdaiREAL, &vector->x);
	sdaiAppend(aggrDirectionRatios, sdaiREAL, &vector->y);
	sdaiAppend(aggrDirectionRatios, sdaiREAL, &vector->z);

	assert(ifcDirectionInstance);

	return	ifcDirectionInstance;
}
