#pragma once


#include "ifcshaperepresentation.h"


static	inline	int_t   ___CreateProductDefinitionShape(
								int_t   model,
								int_t   ifcRepresentationItemInstance,
								bool	is3DCurve
							)
{
	int_t	ifcProductDefinitionShapeInstance, * aggrRepresentations;

    ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model, "IFCPRODUCTDEFINITIONSHAPE");

	aggrRepresentations = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance, "Representations");

	sdaiAppend((int_t) aggrRepresentations, sdaiINSTANCE, (void*) ___CreateShapeRepresentation(model, ifcRepresentationItemInstance, is3DCurve));

	assert(ifcProductDefinitionShapeInstance);
	return	ifcProductDefinitionShapeInstance;
}

static	inline	int_t   ___CreateProductDefinitionShape(
								int_t   model,
								int_t   ifcRepresentationItemInstance,
                                int_t   ** pAggrItems,
								bool	is3DCurve
							)
{
	int_t	ifcProductDefinitionShapeInstance, * aggrRepresentations;

    ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model, "IFCPRODUCTDEFINITIONSHAPE");

	aggrRepresentations = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance, "Representations");

	sdaiAppend((int_t) aggrRepresentations, sdaiINSTANCE, (void*) ___CreateShapeRepresentation(model, ifcRepresentationItemInstance, pAggrItems, is3DCurve));

	assert(ifcProductDefinitionShapeInstance);
	return	ifcProductDefinitionShapeInstance;
}
