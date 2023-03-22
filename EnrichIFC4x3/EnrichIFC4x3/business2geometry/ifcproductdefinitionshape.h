#pragma once


#include "ifcshaperepresentation.h"


static	inline	SdaiInstance	___CreateProductDefinitionShapeInstance(
										SdaiModel		model,
										SdaiInstance	ifcRepresentationItemInstance,
										bool			is3DCurve
									)
{
	SdaiInstance	ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model, "IFCPRODUCTDEFINITIONSHAPE");

	SdaiAggr		aggrRepresentations = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance, "Representations");

	sdaiAppend(aggrRepresentations, sdaiINSTANCE, (void*) ___CreateShapeRepresentationInstance(model, ifcRepresentationItemInstance, is3DCurve));

	assert(ifcRepresentationItemInstance);

	assert(ifcProductDefinitionShapeInstance);
	return	ifcProductDefinitionShapeInstance;
}

static	inline	SdaiInstance	___CreateProductDefinitionShapeInstance(
										SdaiModel		model,
										SdaiInstance	ifcRepresentationItemInstance,
										SdaiAggr		* pAggrItems,
										bool			is3DCurve
									)
{
	SdaiInstance	ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model, "IFCPRODUCTDEFINITIONSHAPE");

	SdaiAggr		aggrRepresentations = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance, "Representations");

	sdaiAppend(aggrRepresentations, sdaiINSTANCE, (void*) ___CreateShapeRepresentationInstance(model, ifcRepresentationItemInstance, pAggrItems, is3DCurve));

	assert(ifcProductDefinitionShapeInstance);
	return	ifcProductDefinitionShapeInstance;
}
