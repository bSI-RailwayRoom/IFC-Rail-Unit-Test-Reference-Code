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


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../ifcengine/include/ifcengine.h"


//
//
//		PropertySet, PropertySingleValue
//
//


int_t	buildPropertySet(char * name, int_t ** aggrHasProperties);
int_t	buildPropertySingleValue(char * name, char * description, bool nominalValue);
int_t	buildPropertySingleValue(char * name, char * description, bool nominalValue, char * typePath);
int_t	buildPropertySingleValue(char * name, char * description, double nominalValue);
int_t	buildPropertySingleValue(char * name, char * description, double nominalValue, char * typePath);
int_t	buildPropertySingleValue(char * name, char * description, char * nominalValue);
int_t	buildPropertySingleValue(char * name, char * description, char * nominalValue, char * typePath);


//
//
//		ElementQuantity, QuantityLength, QuantityArea, QuantityVolume
//
//


int_t	buildElementQuantity(char * name, int_t ** aggrQuantities);
int_t	buildQuantityLength(char * name, char * description, double length);
int_t	buildQuantityArea(char * name, char * description, double area);
int_t	buildQuantityVolume(char * name, char * description, double volume);


//
//
//		Pset_WallCommon, BaseQuantities_Wall, BaseQuantities_WallStandardCase, BaseQuantities_Opening, Pset_WindowCommon, BaseQuantities_Window
//
//


int_t	buildPset_WallCommon();
int_t	buildBaseQuantities_Wall(double width, double length, double height, double openingArea, double linearConversionFactor);
int_t	buildBaseQuantities_WallStandardCase(double width, double length, double height, double openingArea, double linearConversionFactor);
int_t	buildBaseQuantities_Opening(double depth, double height, double width);
int_t	buildPset_WindowCommon();
int_t	buildBaseQuantities_Window(double height, double width);


//
//
//		RelDefinesByProperties
//
//


int_t	buildRelDefinesByProperties(int_t relatedObject, int_t relatingPropertyDefinition);
