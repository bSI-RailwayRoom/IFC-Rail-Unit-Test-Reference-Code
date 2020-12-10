#pragma once

#include "ifcengine.h"


static	inline	int_t   CreateShapeRepresentation(
                                int_t   model,
                                int_t   ifcRepresentationItemInstance
                            )
{
	int_t	ifcShapeRepresentationInstance, * aggrItems;

    ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model, "IFCSHAPEREPRESENTATION");

    char    representationIdentifier[5] = "Axis";
    sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationIdentifier", sdaiSTRING, representationIdentifier);

	aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance, "Items");
	sdaiAppend((int_t) aggrItems, sdaiINSTANCE, (void*) ifcRepresentationItemInstance);

	assert(ifcShapeRepresentationInstance);
	return	ifcShapeRepresentationInstance;
}
