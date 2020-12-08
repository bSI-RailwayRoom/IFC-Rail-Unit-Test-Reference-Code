#pragma once

#include "ifcshaperepresentation.h"


static	inline	int_t   CreateProductDefinitionShape(
								int_t   model,
								int_t   ifcRepresentationItemInstance
							)
{
	int_t	ifcProductDefinitionShapeInstance, * aggrRepresentations;

    ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model, "IFCPRODUCTDEFINITIONSHAPE");

	aggrRepresentations = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance, "Representations");

	sdaiAppend((int_t) aggrRepresentations, sdaiINSTANCE, (void*) CreateShapeRepresentation(model, ifcRepresentationItemInstance));

	assert(ifcProductDefinitionShapeInstance);
	return	ifcProductDefinitionShapeInstance;
}

