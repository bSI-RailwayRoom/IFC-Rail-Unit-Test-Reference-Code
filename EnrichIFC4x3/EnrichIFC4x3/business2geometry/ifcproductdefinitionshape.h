#pragma once


#include "ifcshaperepresentation.h"


static	inline	SdaiInstance	___CreateProductDefinitionShapeInstance(
										SdaiModel		model,
                                		SdaiAggr		* pAggrRepresentations
									)
{
	SdaiInstance	ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model, "IFCPRODUCTDEFINITIONSHAPE");

	assert(pAggrRepresentations && (*pAggrRepresentations) == nullptr);
	(*pAggrRepresentations) = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance, "Representations");

	assert(ifcProductDefinitionShapeInstance);
	return ifcProductDefinitionShapeInstance;
}

static	inline	SdaiInstance	___CreateProductDefinitionShapeInstance(
										SdaiModel		model,
										SdaiInstance	ifcRepresentationItemInstance,
										const char		* representationType
									)
{
	SdaiInstance	ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model, "IFCPRODUCTDEFINITIONSHAPE");

	SdaiAggr		aggrRepresentations = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance, "Representations");

	sdaiAppend(aggrRepresentations, sdaiINSTANCE, (void*) ___CreateShapeRepresentationInstance(model, ifcRepresentationItemInstance, representationType));

	assert(ifcRepresentationItemInstance);

	assert(ifcProductDefinitionShapeInstance);
	return	ifcProductDefinitionShapeInstance;
}

static	inline	SdaiInstance	___CreateProductDefinitionShapeInstance(
										SdaiModel		model,
										SdaiInstance	ifcRepresentationItemInstance,
										SdaiAggr		* pAggrItems,
										const char		* representationType
									)
{
	SdaiInstance	ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model, "IFCPRODUCTDEFINITIONSHAPE");

	SdaiAggr		aggrRepresentations = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance, "Representations");

	sdaiAppend(aggrRepresentations, sdaiINSTANCE, (void*) ___CreateShapeRepresentationInstance(model, ifcRepresentationItemInstance, pAggrItems, representationType));

	assert(ifcProductDefinitionShapeInstance);
	return	ifcProductDefinitionShapeInstance;
}
