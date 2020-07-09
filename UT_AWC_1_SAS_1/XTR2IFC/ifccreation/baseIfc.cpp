////////////////////////////////////////////////////////////////////////
//	Author:		Peter Bonsma
//	Date:		20 February 2019
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

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>

#include	<ctime>
#include	<iostream>


int_t	model = 0,
		timeStamp,
		* aggrRelatedElements,
		* aggr_ifcRelDeclaresInstance_RelatedDefinitions_ProjectLibrary,
		ifcApplicationInstance,
//		ifcBuildingInstance,
//		ifcBuildingInstancePlacement,
//		ifcBuildingStoreyInstance,
//		ifcBuildingStoreyInstancePlacement,
		ifcBuildOwnerHistoryInstance,
		ifcConversionBasedUnitInstance,
		ifcDimensionalExponentsInstance,
		ifcGeometricRepresentationContextInstance,
		ifcOrganizationInstance,
		ifcPersonAndOrganizationInstance,
		ifcPersonInstance,
		ifcPostalAddressInstance,
		ifcProjectInstance,
		ifcRelDeclaresInstance,
		ifcSiteInstance,
		ifcSiteInstancePlacement,
		ifcUnitAssignmentInstance;


int_t	createEmptyIfcFile(wchar_t * ifcSchemaName, char * lengthUnitConversion)
{
	model = sdaiCreateModelBNUnicode(1, 0, ifcSchemaName);

	if (model) {
		MATRIX	matrix;
		identityMatrix(&matrix);

		timeStamp = 0;
		aggrRelatedElements = nullptr;
		aggr_ifcRelDeclaresInstance_RelatedDefinitions_ProjectLibrary = nullptr;
		ifcApplicationInstance = 0;
//		ifcBuildingInstance = 0;
//		ifcBuildingInstancePlacement = 0;
//		ifcBuildingStoreyInstance = 0;
//		ifcBuildingStoreyInstancePlacement = 0;
		ifcBuildOwnerHistoryInstance = 0;
		ifcConversionBasedUnitInstance = 0;
		ifcDimensionalExponentsInstance = 0;
		ifcGeometricRepresentationContextInstance = 0;
		ifcOrganizationInstance = 0;
		ifcPersonAndOrganizationInstance = 0;
		ifcPersonInstance = 0;
		ifcPostalAddressInstance = 0;
		ifcProjectInstance = 0;
		ifcRelDeclaresInstance = 0;
		ifcSiteInstance = 0;
		ifcSiteInstancePlacement = 0;
		ifcUnitAssignmentInstance = 0;

		//
		//	Build standard IFC structures
		//

		//
		//		Build standard IFC objects
		//
		ifcProjectInstance = getProjectInstance(lengthUnitConversion);
		ifcSiteInstance = buildSiteInstance(&matrix, 0, &ifcSiteInstancePlacement);
//		ifcBuildingInstance = buildBuildingInstance(&matrix, ifcSiteInstancePlacement, &ifcBuildingInstancePlacement);
//		ifcBuildingStoreyInstance = buildBuildingStoreyInstance(&matrix, ifcBuildingInstancePlacement, &ifcBuildingStoreyInstancePlacement);

		//
		//		Build relations
		//
//		buildRelAggregatesInstance((char*)"BuildingContainer",(char*)"BuildingContainer for BuildigStories", ifcBuildingInstance, ifcBuildingStoreyInstance);
//		buildRelAggregatesInstance((char*)"SiteContainer",(char*)"SiteContainer For Buildings", ifcSiteInstance, ifcBuildingInstance);
		buildRelAggregatesInstance((char*) "ProjectContainer", (char*) "ProjectContainer for Sites", ifcProjectInstance, ifcSiteInstance);

		buildRelContainedInSpatialStructureInstance((char*) "SiteContainer", (char*) "siteContainer for Alignment Elements", ifcSiteInstance, &aggrRelatedElements);

		return	model;
	}
	else {
		return	0;
	}
}

bool	saveIfcFile(wchar_t * ifcFileName)
{
	if (model) {
		//
		//	Save the created configuration
		//
		sdaiSaveModelBNUnicode(model, ifcFileName);

		return	true;
	}
	else {
		return	false;
	}
}

bool	saveIfcFileAsXml(wchar_t * ifcFileName)
{
	if (model) {
		//
		//  Save the created configuration
		//
		sdaiSaveModelAsXmlBNUnicode(model, ifcFileName);

		return	true;
	}
	else {
		return	false;
	}
}

void	identityMatrix(MATRIX * matrix)
{
	matrix->_11 = 1;
	matrix->_12 = 0;
	matrix->_13 = 0;
	matrix->_21 = 0;
	matrix->_22 = 1;
	matrix->_23 = 0;
	matrix->_31 = 0;
	matrix->_32 = 0;
	matrix->_33 = 1;
	matrix->_41 = 0;
	matrix->_42 = 0;
	matrix->_43 = 0;
}

void	identityPoint(POINT3D * point)
{
	point->x = 0;
	point->y = 0;
	point->z = 0;
}

int_t	* getTimeStamp()
{
	timeStamp = (int_t) time(0);

	return	&timeStamp;
}


//
//
//		Application, Organization, Person (OwnerHistory, PersonAndOrganization)
//
//


int_t	getApplicationInstance()
{
	if (!ifcApplicationInstance) {
		ifcApplicationInstance = sdaiCreateInstanceBN(model,(char*)"IFCAPPLICATION");

		sdaiPutAttrBN(ifcApplicationInstance,(char*)"ApplicationDeveloper", sdaiINSTANCE, (void*) getOrganizationInstance());
		sdaiPutAttrBN(ifcApplicationInstance,(char*)"Version", sdaiSTRING,(char*)"0.10");
		sdaiPutAttrBN(ifcApplicationInstance,(char*)"ApplicationFullName", sdaiSTRING,(char*)"Test Application");
		sdaiPutAttrBN(ifcApplicationInstance,(char*)"ApplicationIdentifier", sdaiSTRING,(char*)"TA 1001");
	}

	assert(ifcApplicationInstance);

	return	ifcApplicationInstance;
}

int_t	getOrganizationInstance()
{
	if (!ifcOrganizationInstance) {
		ifcOrganizationInstance = sdaiCreateInstanceBN(model,(char*)"IFCORGANIZATION");

		sdaiPutAttrBN(ifcOrganizationInstance,(char*)"Name", sdaiSTRING,(char*)"RDF");
		sdaiPutAttrBN(ifcOrganizationInstance,(char*)"Description", sdaiSTRING,(char*)"RDF Ltd.");
	}

	assert(ifcOrganizationInstance);

	return	ifcOrganizationInstance;
}

int_t	getOwnerHistoryInstance()
{
	if (!ifcBuildOwnerHistoryInstance) {
		ifcBuildOwnerHistoryInstance = sdaiCreateInstanceBN(model,(char*)"IFCOWNERHISTORY");

		sdaiPutAttrBN(ifcBuildOwnerHistoryInstance,(char*)"OwningUser", sdaiINSTANCE, (void*) getPersonAndOrganizationInstance());
		sdaiPutAttrBN(ifcBuildOwnerHistoryInstance,(char*)"OwningApplication", sdaiINSTANCE, (void*) getApplicationInstance());
		sdaiPutAttrBN(ifcBuildOwnerHistoryInstance,(char*)"ChangeAction", sdaiENUM,(char*)"ADDED");
		sdaiPutAttrBN(ifcBuildOwnerHistoryInstance,(char*)"CreationDate", sdaiINTEGER, (void*) getTimeStamp());
	}

	assert(ifcBuildOwnerHistoryInstance);

	return	ifcBuildOwnerHistoryInstance;
}

int_t	getPersonAndOrganizationInstance()
{
	if (!ifcPersonAndOrganizationInstance) {
		ifcPersonAndOrganizationInstance = sdaiCreateInstanceBN(model,(char*)"IFCPERSONANDORGANIZATION");

		sdaiPutAttrBN(ifcPersonAndOrganizationInstance,(char*)"ThePerson", sdaiINSTANCE, (void*) getPersonInstance());
		sdaiPutAttrBN(ifcPersonAndOrganizationInstance,(char*)"TheOrganization", sdaiINSTANCE, (void*) getOrganizationInstance());
	}

	assert(ifcPersonAndOrganizationInstance);

	return	ifcPersonAndOrganizationInstance;
}

int_t	getPersonInstance()
{
	if (!ifcPersonInstance) {
		ifcPersonInstance = sdaiCreateInstanceBN(model,(char*)"IFCPERSON");

		sdaiPutAttrBN(ifcPersonInstance,(char*)"GivenName", sdaiSTRING,(char*)"Peter");
		sdaiPutAttrBN(ifcPersonInstance,(char*)"FamilyName", sdaiSTRING,(char*)"Bonsma");
	}

	assert(ifcPersonInstance);

	return	ifcPersonInstance;
}


//
//
//		CartesianPoint, Direction, LocalPlacement (Axis2Placement)
//
//


int_t	buildAxis2Placement3DInstance(MATRIX * matrix)
{
	int_t	ifcAxis2Placement3DInstance;

	ifcAxis2Placement3DInstance = sdaiCreateInstanceBN(model,(char*)"IFCAXIS2PLACEMENT3D");

	sdaiPutAttrBN(ifcAxis2Placement3DInstance,(char*)"Location", sdaiINSTANCE, (void*) buildCartesianPointInstance((POINT3D*) &matrix->_41));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance,(char*)"Axis", sdaiINSTANCE, (void*) buildDirectionInstance((POINT3D*) &matrix->_31));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance,(char*)"RefDirection", sdaiINSTANCE, (void*) buildDirectionInstance((POINT3D*) &matrix->_11));

	assert(ifcAxis2Placement3DInstance);

	return	ifcAxis2Placement3DInstance;
}

int_t	buildCartesianPointInstance(POINT3D * point)
{
	int_t	ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model,(char*)"IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance,(char*)"Coordinates");

	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->x);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->y);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &point->z);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}


int_t	buildCartesianPointInstance(double x, double y)
{
	int_t	ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, (char*)"IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, (char*)"Coordinates");

	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &x);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &y);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

int_t	buildDirectionInstance(POINT3D * point)
{
	int_t	ifcDirectionInstance, * aggrDirectionRatios;

	ifcDirectionInstance = sdaiCreateInstanceBN(model,(char*)"IFCDIRECTION");

	aggrDirectionRatios = sdaiCreateAggrBN(ifcDirectionInstance,(char*)"DirectionRatios");

	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &point->x);
	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &point->y);
	sdaiAppend((int_t) aggrDirectionRatios, sdaiREAL, &point->z);

	assert(ifcDirectionInstance);

	return	ifcDirectionInstance;
}

int_t	buildLocalPlacementInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo)
{
	int_t	ifcLocalPlacementInstance;

	ifcLocalPlacementInstance = sdaiCreateInstanceBN(model,(char*)"IFCLOCALPLACEMENT");

	if (ifcPlacementRelativeTo) {
		sdaiPutAttrBN(ifcLocalPlacementInstance,(char*)"PlacementRelTo", sdaiINSTANCE, (void*) ifcPlacementRelativeTo);
	}
	sdaiPutAttrBN(ifcLocalPlacementInstance,(char*)"RelativePlacement", sdaiINSTANCE, (void*) buildAxis2Placement3DInstance(matrix));

	assert(ifcLocalPlacementInstance);

	return	ifcLocalPlacementInstance;
}


//
//
//		ConversionBasedUnit, DimensionalExponents, MeasureWithUnit, SIUnit, UnitAssignment
//
//


int_t	getConversionBasedUnitInstance()
{
	if (!ifcConversionBasedUnitInstance) {
		ifcConversionBasedUnitInstance = sdaiCreateInstanceBN(model,(char*)"IFCCONVERSIONBASEDUNIT");

		sdaiPutAttrBN(ifcConversionBasedUnitInstance,(char*)"Dimensions", sdaiINSTANCE, (void*) getDimensionalExponentsInstance());
		sdaiPutAttrBN(ifcConversionBasedUnitInstance,(char*)"UnitType", sdaiENUM,(char*)"PLANEANGLEUNIT");
		sdaiPutAttrBN(ifcConversionBasedUnitInstance,(char*)"Name", sdaiSTRING,(char*)"DEGREE");
		sdaiPutAttrBN(ifcConversionBasedUnitInstance,(char*)"ConversionFactor", sdaiINSTANCE, (void*) buildMeasureWithUnitInstance());
	}

	assert(ifcConversionBasedUnitInstance);

	return	ifcConversionBasedUnitInstance;
}

int_t	getDimensionalExponentsInstance()
{
	if (!ifcDimensionalExponentsInstance) {
		int_t	LengthExponent = 0,
				MassExponent = 0,
				TimeExponent = 0,
				ElectricCurrentExponent = 0,
				ThermodynamicTemperatureExponent = 0,
				AmountOfSubstanceExponent = 0,
				LuminousIntensityExponent = 0;

		ifcDimensionalExponentsInstance = sdaiCreateInstanceBN(model,(char*)"IFCDIMENSIONALEXPONENTS");

		sdaiPutAttrBN(ifcDimensionalExponentsInstance,(char*)"LengthExponent", sdaiINTEGER, &LengthExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance,(char*)"MassExponent", sdaiINTEGER, &MassExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance,(char*)"TimeExponent", sdaiINTEGER, &TimeExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance,(char*)"ElectricCurrentExponent", sdaiINTEGER, &ElectricCurrentExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance,(char*)"ThermodynamicTemperatureExponent", sdaiINTEGER, &ThermodynamicTemperatureExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance,(char*)"AmountOfSubstanceExponent", sdaiINTEGER, &AmountOfSubstanceExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance,(char*)"LuminousIntensityExponent", sdaiINTEGER, &LuminousIntensityExponent);
	}

	assert(ifcDimensionalExponentsInstance);

	return	ifcDimensionalExponentsInstance;
}

int_t	buildMeasureWithUnitInstance()
{
	int_t	ifcMeasureWithUnitInstance;
	void	* valueComponentADB;
	double	valueComponent= 0.01745;

	ifcMeasureWithUnitInstance = sdaiCreateInstanceBN(model,(char*)"IFCMEASUREWITHUNIT");

	valueComponentADB = sdaiCreateADB(sdaiREAL, &valueComponent);
	sdaiPutADBTypePath(valueComponentADB, 1,(char*)"IFCPLANEANGLEMEASURE"); 
	sdaiPutAttrBN(ifcMeasureWithUnitInstance,(char*)"ValueComponent", sdaiADB, (void*) valueComponentADB);

	sdaiPutAttrBN(ifcMeasureWithUnitInstance,(char*)"UnitComponent", sdaiINSTANCE, (void*) buildSIUnitInstance((char*)"PLANEANGLEUNIT", 0,(char*)"RADIAN"));

	assert(ifcMeasureWithUnitInstance);

	return	ifcMeasureWithUnitInstance;
}

int_t	buildSIUnitInstance(char * UnitType, char * Prefix, char * Name)
{
	int_t	ifcSIUnitInstance;

	ifcSIUnitInstance = sdaiCreateInstanceBN(model,(char*)"IFCSIUNIT");

	sdaiPutAttrBN(ifcSIUnitInstance,(char*)"Dimensions", sdaiINTEGER, 0);
	sdaiPutAttrBN(ifcSIUnitInstance,(char*)"UnitType", sdaiENUM, UnitType);
	if (Prefix) {
		sdaiPutAttrBN(ifcSIUnitInstance,(char*)"Prefix", sdaiENUM, Prefix);
	}
	sdaiPutAttrBN(ifcSIUnitInstance,(char*)"Name", sdaiENUM, Name);

	assert(ifcSIUnitInstance);

	return	ifcSIUnitInstance;
}

int_t	getUnitAssignmentInstance(char * lengthUnitConversion)
{
	int_t	* aggrUnits;

	if (!ifcUnitAssignmentInstance) {
		ifcUnitAssignmentInstance = sdaiCreateInstanceBN(model,(char*)"IFCUNITASSIGNMENT");

		aggrUnits = sdaiCreateAggrBN(ifcUnitAssignmentInstance,(char*)"Units");
		sdaiAppend((int_t) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance((char*)"LENGTHUNIT", lengthUnitConversion,(char*)"METRE"));
		sdaiAppend((int_t) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance((char*)"AREAUNIT", 0,(char*)"SQUARE_METRE"));
		sdaiAppend((int_t) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance((char*)"VOLUMEUNIT", 0,(char*)"CUBIC_METRE"));
		sdaiAppend((int_t) aggrUnits, sdaiINSTANCE, (void*) getConversionBasedUnitInstance());
		sdaiAppend((int_t) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance((char*)"SOLIDANGLEUNIT", 0,(char*)"STERADIAN"));
		sdaiAppend((int_t) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance((char*)"MASSUNIT", 0,(char*)"GRAM"));
		sdaiAppend((int_t) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance((char*)"TIMEUNIT", 0,(char*)"SECOND"));
		sdaiAppend((int_t) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance((char*)"THERMODYNAMICTEMPERATUREUNIT", 0,(char*)"DEGREE_CELSIUS"));
		sdaiAppend((int_t) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance((char*)"LUMINOUSINTENSITYUNIT", 0,(char*)"LUMEN"));
	}

	assert(ifcUnitAssignmentInstance);

	return	ifcUnitAssignmentInstance;
}


//
//
//		RelAggregates, RelContainedInSpatialStructure
//
//


int_t	buildRelAggregatesInstance(char * name, char * description, int_t ifcRelatingObjectInstance, int_t ifcRelatedObjectInstance)
{
	assert(ifcRelatingObjectInstance  &&  ifcRelatedObjectInstance);

	int_t	ifcRelAggregatesInstance, * aggrRelatedObjects;

	ifcRelAggregatesInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELAGGREGATES");

	sdaiPutAttrBN(ifcRelAggregatesInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcRelAggregatesInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcRelAggregatesInstance,(char*)"Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcRelAggregatesInstance,(char*)"Description", sdaiSTRING, description);
	sdaiPutAttrBN(ifcRelAggregatesInstance,(char*)"RelatingObject", sdaiINSTANCE, (void*) ifcRelatingObjectInstance);
	aggrRelatedObjects = sdaiCreateAggrBN(ifcRelAggregatesInstance,(char*)"RelatedObjects");
	sdaiAppend((int_t) aggrRelatedObjects, sdaiINSTANCE, (void*) ifcRelatedObjectInstance);

	assert(ifcRelAggregatesInstance);

	return	ifcRelAggregatesInstance;
}

int_t	buildRelContainedInSpatialStructureInstance(char * name, char * description, int_t ifcRelatingStructureInstance, int_t ** aggrRelatedElements)
{
	assert(ifcRelatingStructureInstance);

	int_t	ifcRelContainedInSpatialStructureInstance;

	ifcRelContainedInSpatialStructureInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELCONTAINEDINSPATIALSTRUCTURE");

	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance,(char*)"Name", sdaiSTRING,(char*)"Default Building");
	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance,(char*)"Description", sdaiSTRING,(char*)"Contents of Building Storey");
	(* aggrRelatedElements) = sdaiCreateAggrBN(ifcRelContainedInSpatialStructureInstance,(char*)"RelatedElements");
	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance,(char*)"RelatingStructure", sdaiINSTANCE, (void*) ifcRelatingStructureInstance);

	assert(ifcRelContainedInSpatialStructureInstance);

	return	ifcRelContainedInSpatialStructureInstance;
}


//
//
//		Building, BuildingStorey, Project, Site
//
//


int_t	buildBuildingInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcBuildingInstancePlacement)
{
	assert(ifcPlacementRelativeTo);

	int_t	ifcBuildingInstance;

	ifcBuildingInstance = sdaiCreateInstanceBN(model,(char*)"IFCBUILDING");

	sdaiPutAttrBN(ifcBuildingInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcBuildingInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcBuildingInstance,(char*)"Name", sdaiSTRING,(char*)"Default Building");
	sdaiPutAttrBN(ifcBuildingInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Default Building");

	(*ifcBuildingInstancePlacement) = buildLocalPlacementInstance(matrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcBuildingInstance,(char*)"ObjectPlacement", sdaiINSTANCE, (void*) (*ifcBuildingInstancePlacement));
	sdaiPutAttrBN(ifcBuildingInstance,(char*)"CompositionType", sdaiENUM,(char*)"ELEMENT");

	sdaiPutAttrBN(ifcBuildingInstance,(char*)"BuildingAddress", sdaiINSTANCE, (void*) buildPostalAddress());

	assert(ifcBuildingInstance);

	return	ifcBuildingInstance;
}

int_t	buildBuildingStoreyInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcBuildingStoreyInstancePlacement)
{
	assert(ifcPlacementRelativeTo);

	int_t	ifcBuildingStoreyInstance;
	double	elevation = 0;

	ifcBuildingStoreyInstance = sdaiCreateInstanceBN(model,(char*)"IFCBUILDINGSTOREY");

	sdaiPutAttrBN(ifcBuildingStoreyInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcBuildingStoreyInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcBuildingStoreyInstance,(char*)"Name", sdaiSTRING,(char*)"Default Building Storey");
	sdaiPutAttrBN(ifcBuildingStoreyInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Default Building Storey");

	(*ifcBuildingStoreyInstancePlacement) = buildLocalPlacementInstance(matrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcBuildingStoreyInstance,(char*)"ObjectPlacement", sdaiINSTANCE, (void*) (*ifcBuildingStoreyInstancePlacement));
	sdaiPutAttrBN(ifcBuildingStoreyInstance,(char*)"CompositionType", sdaiENUM,(char*)"ELEMENT");
	sdaiPutAttrBN(ifcBuildingStoreyInstance,(char*)"Elevation", sdaiREAL, &elevation);

	assert(ifcBuildingStoreyInstance);

	return	ifcBuildingStoreyInstance;
}

int_t	getProjectInstance(char * lengthUnitConversion)
{
	int_t	* aggrRepresentationContexts = 0;

	if (!ifcProjectInstance) {
		ifcProjectInstance = sdaiCreateInstanceBN(model,(char*)"IFCPROJECT");

		sdaiPutAttrBN(ifcProjectInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
		sdaiPutAttrBN(ifcProjectInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
		sdaiPutAttrBN(ifcProjectInstance,(char*)"Name", sdaiSTRING,(char*)"Default Project");
		sdaiPutAttrBN(ifcProjectInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Default Project");
		sdaiPutAttrBN(ifcProjectInstance,(char*)"UnitsInContext", sdaiINSTANCE, (void*) getUnitAssignmentInstance(lengthUnitConversion));
		aggrRepresentationContexts = sdaiCreateAggrBN(ifcProjectInstance,(char*)"RepresentationContexts");
		sdaiAppend((int_t) aggrRepresentationContexts, sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());
	}

	assert(ifcProjectInstance);

	return	ifcProjectInstance;
}

int_t	buildSiteInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcSiteInstancePlacement)
{
	assert(ifcPlacementRelativeTo == 0);

	int_t	ifcSiteInstance, * aggrRefLatitude, * aggrRefLongitude,
			refLat_x = 24, refLat_y = 28, refLat_z = 0, refLong_x = 54, refLong_y = 25, refLong_z = 0;

	ifcSiteInstance = sdaiCreateInstanceBN(model,(char*)"IFCSITE");

	sdaiPutAttrBN(ifcSiteInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcSiteInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcSiteInstance,(char*)"Name", sdaiSTRING,(char*)"Default Site");
	sdaiPutAttrBN(ifcSiteInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Default Site");

	(*ifcSiteInstancePlacement) = buildLocalPlacementInstance(matrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcSiteInstance,(char*)"ObjectPlacement", sdaiINSTANCE, (void*) (*ifcSiteInstancePlacement));
	sdaiPutAttrBN(ifcSiteInstance,(char*)"CompositionType", sdaiENUM,(char*)"ELEMENT");

	aggrRefLatitude = sdaiCreateAggrBN(ifcSiteInstance,(char*)"RefLatitude");
	sdaiAppend((int_t) aggrRefLatitude, sdaiINTEGER, &refLat_x);
	sdaiAppend((int_t) aggrRefLatitude, sdaiINTEGER, &refLat_y);
	sdaiAppend((int_t) aggrRefLatitude, sdaiINTEGER, &refLat_z);

	aggrRefLongitude = sdaiCreateAggrBN(ifcSiteInstance,(char*)"RefLongitude");
	sdaiAppend((int_t) aggrRefLongitude, sdaiINTEGER, &refLong_x);
	sdaiAppend((int_t) aggrRefLongitude, sdaiINTEGER, &refLong_y);
	sdaiAppend((int_t) aggrRefLongitude, sdaiINTEGER, &refLong_z);

	double	refElevation = 10;
	sdaiPutAttrBN(ifcSiteInstance,(char*)"RefElevation", sdaiREAL, &refElevation);

	assert(ifcSiteInstance);

	return	ifcSiteInstance;
}


//
//
//		WorldCoordinateSystem, GeometricRepresentationContext
//
//


int_t	getWorldCoordinateSystemInstance()
{
	POINT3D	point;
	identityPoint(&point);

	int_t	ifcWorldCoordinateSystemInstance;

	ifcWorldCoordinateSystemInstance = sdaiCreateInstanceBN(model,(char*)"IFCAXIS2PLACEMENT3D");
	
	sdaiPutAttrBN(ifcWorldCoordinateSystemInstance,(char*)"Location", sdaiINSTANCE, (void*) buildCartesianPointInstance(&point));

	assert(ifcWorldCoordinateSystemInstance);

	return	ifcWorldCoordinateSystemInstance;
}

int_t	getGeometricRepresentationContextInstance()
{
	if (!ifcGeometricRepresentationContextInstance) {
		double	precision = 0.00001;
		int_t	coordinateSpaceDimension = 3;

		ifcGeometricRepresentationContextInstance = sdaiCreateInstanceBN(model,(char*)"IFCGEOMETRICREPRESENTATIONCONTEXT");

		sdaiPutAttrBN(ifcGeometricRepresentationContextInstance,(char*)"ContextType", sdaiSTRING,(char*)"Model");
		sdaiPutAttrBN(ifcGeometricRepresentationContextInstance,(char*)"CoordinateSpaceDimension", sdaiINTEGER, &coordinateSpaceDimension);
		sdaiPutAttrBN(ifcGeometricRepresentationContextInstance,(char*)"Precision", sdaiREAL, &precision);
		sdaiPutAttrBN(ifcGeometricRepresentationContextInstance,(char*)"WorldCoordinateSystem", sdaiINSTANCE, (void*) getWorldCoordinateSystemInstance());

		POINT3D	point;
		point.ifcCartesianPointInstance = 0;
		point.x = 0;
		point.y = 1;
		point.z = 0;

		sdaiPutAttrBN(ifcGeometricRepresentationContextInstance,(char*)"TrueNorth", sdaiINSTANCE, (void*) buildDirectionInstance(&point));
	}

	assert(ifcGeometricRepresentationContextInstance);

	return	ifcGeometricRepresentationContextInstance;
}


//
//
//		PostalAddress
//
//


int_t	buildPostalAddress()
{
	if (!ifcPostalAddressInstance) {
		int_t	* addressLines;

		ifcPostalAddressInstance = sdaiCreateInstanceBN(model,(char*)"IFCPOSTALADDRESS");

		addressLines = sdaiCreateAggrBN(ifcPostalAddressInstance,(char*)"AddressLines");
		sdaiAppend((int_t) addressLines, sdaiSTRING,(char*)"RDF Ltd.");
		sdaiAppend((int_t) addressLines, sdaiSTRING,(char*)"Main Office");

		sdaiPutAttrBN(ifcPostalAddressInstance,(char*)"PostalBox", sdaiSTRING,(char*)"32");
		sdaiPutAttrBN(ifcPostalAddressInstance,(char*)"Town", sdaiSTRING,(char*)"Bankya");
		sdaiPutAttrBN(ifcPostalAddressInstance,(char*)"Region", sdaiSTRING,(char*)"Sofia");
		sdaiPutAttrBN(ifcPostalAddressInstance,(char*)"PostalCode", sdaiSTRING,(char*)"1320");
		sdaiPutAttrBN(ifcPostalAddressInstance,(char*)"Country", sdaiSTRING,(char*)"Bulgaria");
	}

	assert(ifcPostalAddressInstance);

	return	ifcPostalAddressInstance;
}
