////////////////////////////////////////////////////////////////////////
//	Author:		Peter Bonsma
//	Date:		20 February 2015
//	Project:	IFC Engine Series (example using DLL)
//
//	This code may be used and edited freely,
//	also for commercial projects in open and closed source software
//
//	In case of use of the DLL:
//	be aware of license fee for use of this DLL when used commercially
//	more info for commercial use:	contact@rdf.bg
//
//	more info for using the IFC Engine DLL in other languages
//	and creation of specific code examples:
//									peter.bonsma@rdf.bg
////////////////////////////////////////////////////////////////////////


#include	"stdafx.h"
#include	"baseIfc.h"
#include	"propertiesIfc.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>


extern	int_t	model;

//
//
//		PropertySet, PropertySingleValue
//
//


int_t	buildPropertySet(char * name, int_t ** aggrHasProperties)
{
	int_t	ifcPropertySetInstance;

	ifcPropertySetInstance = sdaiCreateInstanceBN(model, (char*)"IFCPROPERTYSET");

	sdaiPutAttrBN(ifcPropertySetInstance, (char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcPropertySetInstance, (char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcPropertySetInstance, (char*)"Name", sdaiSTRING, name);

	(* aggrHasProperties) = sdaiCreateAggrBN(ifcPropertySetInstance, (char*)"HasProperties");

	assert(ifcPropertySetInstance);

	return	ifcPropertySetInstance;
}

int_t	buildPropertySingleValue(char * name, char * description, bool nominalValue)
{
	return	buildPropertySingleValue(name, description, nominalValue, (char*)"IFCBOOLEAN");
}

int_t	buildPropertySingleValue(char * name, char * description, bool nominalValue, char * typePath)
{
	int_t	ifcPropertySingleValueInstance;
	void	* nominalValueADB;
	char	bTrue[2] = "T", bFalse[2] = "F"; 

	ifcPropertySingleValueInstance = sdaiCreateInstanceBN(model, (char*)"IFCPROPERTYSINGLEVALUE");

	sdaiPutAttrBN(ifcPropertySingleValueInstance, (char*)"Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcPropertySingleValueInstance, (char*)"Description", sdaiSTRING, description);

	if (nominalValue) {
		nominalValueADB = sdaiCreateADB(sdaiENUM, bTrue);
	}
	else {
		nominalValueADB = sdaiCreateADB(sdaiENUM, bFalse);
	}
	sdaiPutADBTypePath(nominalValueADB, 1, typePath); 
	sdaiPutAttrBN(ifcPropertySingleValueInstance, (char*)"NominalValue", sdaiADB, (void*) nominalValueADB);

	assert(ifcPropertySingleValueInstance);

	return	ifcPropertySingleValueInstance;
}

int_t	buildPropertySingleValue(char * name, char * description, double nominalValue)
{
	return	buildPropertySingleValue(name, description, nominalValue, (char*)"IFCREAL");
}

int_t	buildPropertySingleValue(char * name, char * description, double nominalValue, char * typePath)
{
	int_t	ifcPropertySingleValueInstance;
	void	* nominalValueADB;

	ifcPropertySingleValueInstance = sdaiCreateInstanceBN(model,(char*)"IFCPROPERTYSINGLEVALUE");

	sdaiPutAttrBN(ifcPropertySingleValueInstance,(char*)"Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcPropertySingleValueInstance,(char*)"Description", sdaiSTRING, description);

	nominalValueADB = sdaiCreateADB(sdaiREAL, (int_t*) &nominalValue);
	sdaiPutADBTypePath(nominalValueADB, 1, typePath); 
	sdaiPutAttrBN(ifcPropertySingleValueInstance,(char*)"NominalValue", sdaiADB, (void*) nominalValueADB);

	assert(ifcPropertySingleValueInstance);

	return	ifcPropertySingleValueInstance;
}


int_t	buildPropertySingleValue(char * name, char * description, char * nominalValue)
{
	return	buildPropertySingleValue(name, description, nominalValue,(char*)"IFCTEXT");
}

int_t	buildPropertySingleValue(char * name, char * description, char * nominalValue, char * typePath)
{
	int_t	ifcPropertySingleValueInstance;
	void	* nominalValueADB;

	ifcPropertySingleValueInstance = sdaiCreateInstanceBN(model,(char*)"IFCPROPERTYSINGLEVALUE");

	sdaiPutAttrBN(ifcPropertySingleValueInstance,(char*)"Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcPropertySingleValueInstance,(char*)"Description", sdaiSTRING, description);

	nominalValueADB = sdaiCreateADB(sdaiSTRING, nominalValue);
	sdaiPutADBTypePath(nominalValueADB, 1, typePath); 
	sdaiPutAttrBN(ifcPropertySingleValueInstance,(char*)"NominalValue", sdaiADB, (void*) nominalValueADB);

	assert(ifcPropertySingleValueInstance);

	return	ifcPropertySingleValueInstance;
}


//
//
//		ElementQuantity, QuantityLength, QuantityArea, QuantityVolume
//
//


int_t	buildElementQuantity(char * name, int_t ** aggrQuantities)
{
	int_t	ifcElementQuantityInstance;

	ifcElementQuantityInstance = sdaiCreateInstanceBN(model,(char*)"IFCELEMENTQUANTITY");

	sdaiPutAttrBN(ifcElementQuantityInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcElementQuantityInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcElementQuantityInstance,(char*)"Name", sdaiSTRING, name);

	(* aggrQuantities) = sdaiCreateAggrBN(ifcElementQuantityInstance,(char*)"Quantities");

	assert(ifcElementQuantityInstance);

	return	ifcElementQuantityInstance;
}

int_t	buildQuantityLength(char * name, char * description, double length)
{
	int_t	ifcQuantityLengthInstance;

	ifcQuantityLengthInstance = sdaiCreateInstanceBN(model,(char*)"IFCQUANTITYLENGTH");

	sdaiPutAttrBN(ifcQuantityLengthInstance,(char*)"Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcQuantityLengthInstance,(char*)"Description", sdaiSTRING, description);
	sdaiPutAttrBN(ifcQuantityLengthInstance,(char*)"LengthValue", sdaiREAL, &length);

	assert(ifcQuantityLengthInstance);

	return	ifcQuantityLengthInstance;
}

int_t	buildQuantityArea(char * name, char * description, double area)
{
	int_t	ifcQuantityAreaInstance;

	ifcQuantityAreaInstance = sdaiCreateInstanceBN(model,(char*)"IFCQUANTITYAREA");

	sdaiPutAttrBN(ifcQuantityAreaInstance,(char*)"Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcQuantityAreaInstance,(char*)"Description", sdaiSTRING, description);
	sdaiPutAttrBN(ifcQuantityAreaInstance,(char*)"AreaValue", sdaiREAL, &area);

	assert(ifcQuantityAreaInstance);

	return	ifcQuantityAreaInstance;
}

int_t	buildQuantityVolume(char * name, char * description, double volume)
{
	int_t	ifcQuantityVolumeInstance;

	ifcQuantityVolumeInstance = sdaiCreateInstanceBN(model,(char*)"IFCQUANTITYVOLUME");

	sdaiPutAttrBN(ifcQuantityVolumeInstance,(char*)"Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcQuantityVolumeInstance,(char*)"Description", sdaiSTRING, description);
	sdaiPutAttrBN(ifcQuantityVolumeInstance,(char*)"VolumeValue", sdaiREAL, &volume);

	assert(ifcQuantityVolumeInstance);

	return	ifcQuantityVolumeInstance;
}


//
//
//		Pset_WallCommon, BaseQuantities_Wall, BaseQuantities_WallStandardCase, BaseQuantities_Opening, Pset_WindowCommon, BaseQuantities_Window
//
//


int_t	buildPset_WallCommon()
{
	int_t	ifcPropertySetInstance, * aggrHasProperties;


	ifcPropertySetInstance = buildPropertySet((char*)"Pset_WallCommon", &aggrHasProperties);

	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"Reference",(char*)"Reference",(char*)"",(char*)"IFCIDENTIFIER"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"AcousticRating",(char*)"AcousticRating",(char*)"",(char*)"IFCLABEL"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"FireRating",(char*)"FireRating",(char*)"",(char*)"IFCLABEL"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"Combustible",(char*)"Combustible", false));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"SurfaceSpreadOfFlame",(char*)"SurfaceSpreadOfFlame",(char*)"",(char*)"IFCLABEL"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"ThermalTransmittance",(char*)"ThermalTransmittance", 0.24,(char*)"IFCTHERMALTRANSMITTANCEMEASURE"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"IsExternal",(char*)"IsExternal", true));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"ExtendToStructure",(char*)"ExtendToStructure", false));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"LoadBearing",(char*)"LoadBearing", false));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"Compartmentation",(char*)"Compartmentation", false));

	assert(ifcPropertySetInstance);

	return	ifcPropertySetInstance;
}

int_t	buildBaseQuantities_Wall(double width, double length, double height, double openingArea, double linearConversionFactor)
{
	int_t	ifcElementQuantityInstance, * aggrQuantities;
	double	grossSideArea = (length / linearConversionFactor) * (height / linearConversionFactor),
			netSideArea = grossSideArea - openingArea;

	ifcElementQuantityInstance = buildElementQuantity((char*)"BaseQuantities", &aggrQuantities);

	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Width",(char*)"Width", width));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Length",(char*)"Length", length));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityArea((char*)"GrossFootprintArea",(char*)"GrossFootprintArea", (length / linearConversionFactor) * (width / linearConversionFactor)));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityArea((char*)"NetFootprintArea",(char*)"NetFootprintArea", (length / linearConversionFactor) * (width / linearConversionFactor)));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityArea((char*)"GrossSideArea",(char*)"GrossSideArea", grossSideArea));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityArea((char*)"NetSideArea",(char*)"NetSideArea", netSideArea));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityVolume((char*)"GrossVolume",(char*)"GrossVolume", grossSideArea * (width / linearConversionFactor)));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityVolume((char*)"NetVolume",(char*)"NetVolume", netSideArea * (width / linearConversionFactor)));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Height",(char*)"Height", height));

	assert(ifcElementQuantityInstance);

	return	ifcElementQuantityInstance;
}

int_t	buildBaseQuantities_WallStandardCase(double width, double length, double height, double openingArea, double linearConversionFactor)
{
	int_t	ifcElementQuantityInstance, * aggrQuantities;

	double	grossSideArea = (length / linearConversionFactor) * (height / linearConversionFactor),
			netSideArea = grossSideArea - openingArea;


	ifcElementQuantityInstance = buildElementQuantity((char*)"BaseQuantities", &aggrQuantities);

	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Width",(char*)"Width", width));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Length",(char*)"Length", length));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityArea((char*)"GrossFootprintArea",(char*)"GrossFootprintArea", (length / linearConversionFactor) * (width / linearConversionFactor)));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityArea((char*)"NetFootprintArea",(char*)"NetFootprintArea", (length / linearConversionFactor) * (width / linearConversionFactor)));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityArea((char*)"GrossSideArea",(char*)"GrossSideArea", grossSideArea));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityArea((char*)"NetSideArea",(char*)"NetSideArea", netSideArea));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityVolume((char*)"GrossVolume",(char*)"GrossVolume", grossSideArea * (width / linearConversionFactor)));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityVolume((char*)"NetVolume",(char*)"NetVolume", netSideArea * (width / linearConversionFactor)));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Height",(char*)"Height", height));

	assert(ifcElementQuantityInstance);

	return	ifcElementQuantityInstance;
}

int_t	buildBaseQuantities_Opening(double depth, double height, double width)
{
	int_t	ifcElementQuantityInstance, * aggrQuantities;

	ifcElementQuantityInstance = buildElementQuantity((char*)"BaseQuantities", &aggrQuantities);

	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Depth",(char*)"Depth", depth));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Height",(char*)"Height", height));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Width",(char*)"Width", width));

	assert(ifcElementQuantityInstance);

	return	ifcElementQuantityInstance;
}

int_t	buildPset_WindowCommon()
{
	int_t	ifcPropertySetInstance, * aggrHasProperties;

	ifcPropertySetInstance = buildPropertySet((char*)"Pset_WindowCommon", &aggrHasProperties);

	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"Reference",(char*)"Reference",(char*)"",(char*)"IFCIDENTIFIER"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"FireRating",(char*)"FireRating",(char*)"",(char*)"IFCLABEL"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"AcousticRating",(char*)"AcousticRating",(char*)"",(char*)"IFCLABEL"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"SecurityRating",(char*)"SecurityRating",(char*)"",(char*)"IFCLABEL"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"IsExternal",(char*)"IsExternal", true));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"Infiltration",(char*)"Infiltration", 0.3,(char*)"IFCVOLUMETRICFLOWRATEMEASURE"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"ThermalTransmittance",(char*)"ThermalTransmittance", 0.24,(char*)"IFCTHERMALTRANSMITTANCEMEASURE"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"GlazingAreaFraction",(char*)"GlazingAreaFraction", 0.7,(char*)"IFCPOSITIVERATIOMEASURE"));
	sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValue((char*)"SmokeStop",(char*)"SmokeStop", false));

	assert(ifcPropertySetInstance);

	return	ifcPropertySetInstance;
}

int_t	buildBaseQuantities_Window(double height, double width)
{
	int_t	ifcElementQuantityInstance, * aggrQuantities;

	ifcElementQuantityInstance = buildElementQuantity((char*)"BaseQuantities", &aggrQuantities);

	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Height",(char*)"Height", height));
	sdaiAppend((int_t) aggrQuantities, sdaiINSTANCE, (void*) buildQuantityLength((char*)"Width",(char*)"Width", width));

	assert(ifcElementQuantityInstance);

	return	ifcElementQuantityInstance;
}


//
//
//		RelDefinesByProperties
//
//


int_t	buildRelDefinesByProperties(int_t relatedObject, int_t relatingPropertyDefinition)
{
	int_t	ifcRelDefinesByPropertiesInstance, * aggrRelatedObjects;

	ifcRelDefinesByPropertiesInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELDEFINESBYPROPERTIES");

	sdaiPutAttrBN(ifcRelDefinesByPropertiesInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcRelDefinesByPropertiesInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	aggrRelatedObjects = sdaiCreateAggrBN(ifcRelDefinesByPropertiesInstance,(char*)"RelatedObjects");
	sdaiAppend((int_t) aggrRelatedObjects, sdaiINSTANCE, (void*) relatedObject);
	sdaiPutAttrBN(ifcRelDefinesByPropertiesInstance,(char*)"RelatingPropertyDefinition", sdaiINSTANCE, (void*) relatingPropertyDefinition);

	assert(ifcRelDefinesByPropertiesInstance);

	return	ifcRelDefinesByPropertiesInstance;
}
