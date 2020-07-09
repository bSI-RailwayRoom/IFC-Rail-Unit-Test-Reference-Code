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


struct	MATRIX {
	double			_11;
	double			_12;
	double			_13;
	double			_21;
	double			_22;
	double			_23;
	double			_31;
	double			_32;
	double			_33;
	double			_41;
	double			_42;
	double			_43;
};

struct	POINT3D {
	double			x;
	double			y;
	double			z;
	int_t			ifcCartesianPointInstance;
};


void	identityMatrix(MATRIX * matrix);
void	identityPoint(POINT3D * point);
int_t	* getTimeStamp();


//
//
//	GUID
//
//


int_t	createEmptyIfcFile(wchar_t * ifcSchemaName, char * lengthUnitConversion);
bool	saveIfcFile(wchar_t * ifcFileName);
bool	saveIfcFileAsXml(wchar_t * ifcFileName);


//
//
//		Application, Organization, Person (OwnerHistory, PersonAndOrganization)
//
//


int_t	getApplicationInstance();
int_t	getOrganizationInstance();
int_t	getOwnerHistoryInstance();
int_t	getPersonAndOrganizationInstance();
int_t	getPersonInstance();


//
//
//		CartesianPoint, Direction, LocalPlacement (Axis2Placement)
//
//


int_t	buildAxis2Placement3DInstance(MATRIX * matrix);
int_t	buildCartesianPointInstance(POINT3D * point);
int_t	buildCartesianPointInstance(double x, double y);
int_t	buildDirectionInstance(POINT3D * point);
int_t	buildLocalPlacementInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo);


//
//
//		ConversionBasedUnit, DimensionalExponents, MeasureWithUnit, SIUnit, UnitAssignment
//
//


int_t	getConversionBasedUnitInstance();
int_t	getDimensionalExponentsInstance();
int_t	buildMeasureWithUnitInstance();
int_t	buildSIUnitInstance(char * UnitType, char * Prefix, char * Name);
int_t	getUnitAssignmentInstance(char * lengthUnitConversion);


//
//
//		RelAggregates, RelContainedInSpatialStructure
//
//


int_t	buildRelAggregatesInstance(char * name, char * description, int_t ifcRelatingObjectInstance, int_t ifcRelatedObjectInstance);
int_t	buildRelContainedInSpatialStructureInstance(char * name, char * description, int_t ifcRelatingStructureInstance, int_t ** aggrRelatedElements);


//
//
//		Building, BuildingStorey, Project, Site
//
//


int_t	buildBuildingInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcBuildingInstancePlacement);
int_t	buildBuildingStoreyInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcBuildingStoreyInstancePlacement);
int_t	getProjectInstance(char * lengthUnitConversion);
int_t	buildSiteInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcSiteInstancePlacement);


//
//
//		WorldCoordinateSystem, GeometricRepresentationContext
//
//


int_t	getWorldCoordinateSystemInstance();
int_t	getGeometricRepresentationContextInstance();


//
//
//		PostalAddress
//
//


int_t	buildPostalAddress();
