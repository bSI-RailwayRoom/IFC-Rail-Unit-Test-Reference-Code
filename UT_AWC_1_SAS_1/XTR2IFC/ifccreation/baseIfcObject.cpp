////////////////////////////////////////////////////////////////////////
//	Author:		Peter Bonsma
//	Date:		
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
#include	"baseIfcObject.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>


extern	int_t	model,
				* aggrRelatedElements,
				* aggr_ifcRelDeclaresInstance_RelatedDefinitions_ProjectLibrary,
//				ifcBuildingInstancePlacement,
//				ifcBuildingStoreyInstancePlacement,
				ifcProjectInstance,
				ifcRelDeclaresInstance;

int_t			* aggrRepresentations,
				ifcOpeningElementInstancePlacement,
				ifcWallInstancePlacement,
				* aggr_ifcRelDeclaresInstance_RelatedDefinitions;


int_t	createIfcOpeningElement(wchar_t * pOpeningElementName, double xOffset, double yOffset, double zOffset, bool representation)
{
	MATRIX	matrix;
	int_t	ifcOpeningElementInstance;

	identityMatrix(&matrix);
	matrix._41 = xOffset;
	matrix._42 = yOffset;
	matrix._43 = zOffset;

	//
	//		Build Opening Element
	//
	ifcOpeningElementInstance = buildOpeningElementInstance(&matrix, ifcWallInstancePlacement, &ifcOpeningElementInstancePlacement, pOpeningElementName, representation);

	assert(ifcOpeningElementInstance);

	return	ifcOpeningElementInstance;
}

int_t	createIfcOpeningElement(wchar_t * pOpeningElementName,
								double xRefDirection, double yRefDirection, double zRefDirection,
								double xAxis, double yAxis, double zAxis,
								double xOffset, double yOffset, double zOffset,
								bool representation)
{
	MATRIX	matrix;
	int_t	ifcOpeningElementInstance;

	identityMatrix(&matrix);
	matrix._11 = xRefDirection;
	matrix._12 = yRefDirection;
	matrix._13 = zRefDirection;
	matrix._31 = xAxis;
	matrix._32 = yAxis;
	matrix._33 = zAxis;
	matrix._41 = xOffset;
	matrix._42 = yOffset;
	matrix._43 = zOffset;

	//
	//		Build Opening Element
	//
	ifcOpeningElementInstance = buildOpeningElementInstance(&matrix, ifcWallInstancePlacement, &ifcOpeningElementInstancePlacement, pOpeningElementName, representation);

	assert(ifcOpeningElementInstance);

	return	ifcOpeningElementInstance;
}


//
//
//		ProductDefinitionShape
//
//


int_t	buildProductDefinitionShapeInstance()
{
	int_t	ifcProductDefinitionShapeInstance;

	ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model,(char*)"IFCPRODUCTDEFINITIONSHAPE");

	aggrRepresentations = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance,(char*)"Representations");

	assert(ifcProductDefinitionShapeInstance);

	return	ifcProductDefinitionShapeInstance;
}


//
//
//		IfcWall, IfcWallStandardCase, IfcOpeningElement, IfcWindow
//
//


int_t	buildRelDefinesByType(int_t relatedObjectInstance, int_t relatingTypeInstance)
{
	int_t	ifcRelDefinesByTypeInstance, * aggrRelatedObjects;

	ifcRelDefinesByTypeInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELDEFINESBYTYPE");

	sdaiPutAttrBN(ifcRelDefinesByTypeInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcRelDefinesByTypeInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	aggrRelatedObjects = sdaiCreateAggrBN(ifcRelDefinesByTypeInstance,(char*)"RelatedObjects");
	sdaiAppend((int_t) aggrRelatedObjects, sdaiINSTANCE, (void*) relatedObjectInstance);
	sdaiPutAttrBN(ifcRelDefinesByTypeInstance,(char*)"RelatingType", sdaiINSTANCE, (void*) relatingTypeInstance);

	assert(ifcRelDefinesByTypeInstance);

	return	ifcRelDefinesByTypeInstance;
}

int_t	buildWallTypeInstance(int_t ifcWallInstance, wchar_t * pWindowName, char * predefinedType)
{
	assert(ifcWallInstance);

	int_t	ifcWallTypeInstance;

	ifcWallTypeInstance = sdaiCreateInstanceBN(model,(char*)"IFCWALLTYPE");

	sdaiPutAttrBN(ifcWallTypeInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcWallTypeInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcWallTypeInstance,(char*)"Name", sdaiUNICODE, pWindowName);
	sdaiPutAttrBN(ifcWallTypeInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Window Type");

	sdaiPutAttrBN(ifcWallTypeInstance,(char*)"PredefinedType", sdaiENUM, predefinedType);

	buildRelDefinesByType(ifcWallInstance, ifcWallTypeInstance);

	assert(ifcWallTypeInstance);

#ifdef	IFC4
	//
	//	Add to IfcRelDeclares
	//
	buildRelDeclares_WallType(ifcWallTypeInstance);
#endif

	return	ifcWallTypeInstance;
}

int_t	buildWallInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcWallInstancePlacement, wchar_t * pWallName)
{
	assert(ifcPlacementRelativeTo);

	int_t	ifcWallInstance;

	ifcWallInstance = sdaiCreateInstanceBN(model,(char*)"IFCWALL");

	sdaiPutAttrBN(ifcWallInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcWallInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcWallInstance,(char*)"Name", sdaiUNICODE, pWallName);
	sdaiPutAttrBN(ifcWallInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Wall");

	(*ifcWallInstancePlacement) = buildLocalPlacementInstance(matrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcWallInstance,(char*)"ObjectPlacement", sdaiINSTANCE, (void*) (*ifcWallInstancePlacement));
	sdaiPutAttrBN(ifcWallInstance,(char*)"Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	char	predefinedType[9] = "STANDARD";
	buildWallTypeInstance(ifcWallInstance, pWallName, predefinedType);
	sdaiPutAttrBN(ifcWallInstance,(char*)"PredefinedType", sdaiENUM, predefinedType);

	assert(ifcWallInstance);

	return	ifcWallInstance;
}

int_t	buildWallStandardCaseInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcWallInstancePlacement, wchar_t * pWallName)
{
	assert(ifcPlacementRelativeTo);

	int_t	ifcWallInstance;

	ifcWallInstance = sdaiCreateInstanceBN(model,(char*)"IFCWALLSTANDARDCASE");

	sdaiPutAttrBN(ifcWallInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcWallInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcWallInstance,(char*)"Name", sdaiUNICODE, pWallName);
	sdaiPutAttrBN(ifcWallInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Wall");

	(*ifcWallInstancePlacement) = buildLocalPlacementInstance(matrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcWallInstance,(char*)"ObjectPlacement", sdaiINSTANCE, (void*) (*ifcWallInstancePlacement));
	sdaiPutAttrBN(ifcWallInstance,(char*)"Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	char	predefinedType[9] = "STANDARD";
	buildWallTypeInstance(ifcWallInstance, pWallName, predefinedType);
	sdaiPutAttrBN(ifcWallInstance,(char*)"PredefinedType", sdaiENUM, predefinedType);

	assert(ifcWallInstance);

	return	ifcWallInstance;
}

int_t	buildOpeningElementInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcOpeningElementInstancePlacement, wchar_t * pOpeningElementName, bool representation)
{
	assert(ifcPlacementRelativeTo);

	int_t	ifcOpeningElementInstance;

	ifcOpeningElementInstance = sdaiCreateInstanceBN(model,(char*)"IFCOPENINGELEMENT");

	sdaiPutAttrBN(ifcOpeningElementInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcOpeningElementInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcOpeningElementInstance,(char*)"Name", sdaiUNICODE, pOpeningElementName);
	sdaiPutAttrBN(ifcOpeningElementInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Opening");

	char	predefinedType[8] = "OPENING";
	sdaiPutAttrBN(ifcOpeningElementInstance,(char*)"PredefinedType", sdaiENUM, predefinedType);

	(*ifcOpeningElementInstancePlacement) = buildLocalPlacementInstance(matrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcOpeningElementInstance,(char*)"ObjectPlacement", sdaiINSTANCE, (void*) (*ifcOpeningElementInstancePlacement));
	if (representation) {
		sdaiPutAttrBN(ifcOpeningElementInstance,(char*)"Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());
	}

	assert(ifcOpeningElementInstance);

	return	ifcOpeningElementInstance;
}

int_t	buildWindowTypeInstance(int_t ifcWindowInstance, wchar_t * pWindowName, char * predefinedType, char * partitioningType)
{
	assert(ifcWindowInstance);

	int_t	ifcWindowTypeInstance;

	ifcWindowTypeInstance = sdaiCreateInstanceBN(model,(char*)"IFCWINDOWTYPE");

	sdaiPutAttrBN(ifcWindowTypeInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcWindowTypeInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcWindowTypeInstance,(char*)"Name", sdaiUNICODE, pWindowName);
	sdaiPutAttrBN(ifcWindowTypeInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Window Type");

	sdaiPutAttrBN(ifcWindowTypeInstance,(char*)"PredefinedType", sdaiENUM, predefinedType);
	sdaiPutAttrBN(ifcWindowTypeInstance,(char*)"PartitioningType", sdaiENUM, partitioningType);

	buildRelDefinesByType(ifcWindowInstance, ifcWindowTypeInstance);

	//
	//	Add to IfcRelDeclares
	//
	buildRelDeclares_WallType(ifcWindowTypeInstance);

	assert(ifcWindowTypeInstance);

	return	ifcWindowTypeInstance;
}

int_t	buildWindowInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcWindowInstancePlacement, wchar_t * pWindowName, double overallHeight, double overallWidth)
{
	assert(ifcPlacementRelativeTo);

	int_t	ifcWindowInstance;

	ifcWindowInstance = sdaiCreateInstanceBN(model,(char*)"IFCWINDOW");

	sdaiPutAttrBN(ifcWindowInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcWindowInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcWindowInstance,(char*)"Name", sdaiUNICODE, pWindowName);
	sdaiPutAttrBN(ifcWindowInstance,(char*)"Description", sdaiSTRING,(char*)"Description of Window");

	(*ifcWindowInstancePlacement) = buildLocalPlacementInstance(matrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcWindowInstance,(char*)"ObjectPlacement", sdaiINSTANCE, (void*) (*ifcWindowInstancePlacement));
	sdaiPutAttrBN(ifcWindowInstance,(char*)"Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	char	predefinedType[7] = "WINDOW", partitioningType[13] = "SINGLE_PANEL";
#ifdef IFC4
	buildWindowTypeInstance(ifcWindowInstance, pWindowName, predefinedType, partitioningType);
#endif
	sdaiPutAttrBN(ifcWindowInstance,(char*)"PredefinedType", sdaiENUM, predefinedType);
	sdaiPutAttrBN(ifcWindowInstance,(char*)"PartitioningType", sdaiENUM, partitioningType);

	sdaiPutAttrBN(ifcWindowInstance,(char*)"OverallHeight", sdaiREAL, &overallHeight);
	sdaiPutAttrBN(ifcWindowInstance,(char*)"OverallWidth", sdaiREAL, &overallWidth);

	assert(ifcWindowInstance);

	return	ifcWindowInstance;
}


//
//
//		RelVoidsElement, RelFillsElement
//
//


int_t	buildRelVoidsElementInstance(int_t ifcBuildingElementInstance, int_t ifcOpeningElementInstance)
{
	assert(ifcBuildingElementInstance  &&  ifcOpeningElementInstance);

	int_t	ifcRelVoidsElementInstance;

	ifcRelVoidsElementInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELVOIDSELEMENT");

	sdaiPutAttrBN(ifcRelVoidsElementInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcRelVoidsElementInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	sdaiPutAttrBN(ifcRelVoidsElementInstance,(char*)"RelatingBuildingElement", sdaiINSTANCE, (void*) ifcBuildingElementInstance);
	sdaiPutAttrBN(ifcRelVoidsElementInstance,(char*)"RelatedOpeningElement", sdaiINSTANCE, (void*) ifcOpeningElementInstance);

	assert(ifcRelVoidsElementInstance);

	return	ifcRelVoidsElementInstance;
}

int_t	buildRelFillsElementInstance(int_t ifcOpeningElementInstance, int_t ifcBuildingElementInstance)
{
	assert(ifcOpeningElementInstance  &&  ifcBuildingElementInstance);

	int_t	ifcRelFillsElementInstance;

	ifcRelFillsElementInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELFILLSELEMENT");

	sdaiPutAttrBN(ifcRelFillsElementInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcRelFillsElementInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	sdaiPutAttrBN(ifcRelFillsElementInstance,(char*)"RelatingOpeningElement", sdaiINSTANCE, (void*) ifcOpeningElementInstance);
	sdaiPutAttrBN(ifcRelFillsElementInstance,(char*)"RelatedBuildingElement", sdaiINSTANCE, (void*) ifcBuildingElementInstance);

	assert(ifcRelFillsElementInstance);

	return	ifcRelFillsElementInstance;
}


//
//
//		ProjectLibrary, RelDeclares
//
//


int_t	buildProjectLibrary()
{
	int_t	ifcProjectLibraryInstance;

	ifcProjectLibraryInstance = sdaiCreateInstanceBN(model,(char*)"IFCPROJECTLIBRARY");

	sdaiPutAttrBN(ifcProjectLibraryInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcProjectLibraryInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	buildRelDeclares_ProjectLibrary(ifcProjectLibraryInstance);

	assert(ifcProjectLibraryInstance);

	return	ifcProjectLibraryInstance;
}

int_t	buildRelDeclares_ProjectLibrary(int_t ifcProjectLibraryInstance)
{
	int_t	ifcRelDeclaresInstance = 0;

	assert(ifcProjectLibraryInstance);

	if (aggr_ifcRelDeclaresInstance_RelatedDefinitions_ProjectLibrary == 0) {
		ifcRelDeclaresInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELDECLARES");

		sdaiPutAttrBN(ifcRelDeclaresInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
		sdaiPutAttrBN(ifcRelDeclaresInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

		aggr_ifcRelDeclaresInstance_RelatedDefinitions_ProjectLibrary = sdaiCreateAggrBN(ifcRelDeclaresInstance,(char*)"RelatedDefinitions");
		sdaiAppend((int_t) aggr_ifcRelDeclaresInstance_RelatedDefinitions_ProjectLibrary, sdaiINSTANCE, (void*) ifcProjectInstance);
		sdaiPutAttrBN(ifcRelDeclaresInstance,(char*)"RelatingContext", sdaiINSTANCE, (void*) ifcProjectLibraryInstance);
	}
	else {
		sdaiAppend((int_t) aggr_ifcRelDeclaresInstance_RelatedDefinitions_ProjectLibrary, sdaiINSTANCE, (void*) ifcProjectInstance);
	}

	assert(ifcRelDeclaresInstance);

	return	ifcRelDeclaresInstance;
}

int_t	buildRelDeclares_WallType(int_t ifcWallTypeInstance)
{
	assert(ifcWallTypeInstance);

	if (ifcRelDeclaresInstance == 0) {
		ifcRelDeclaresInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELDECLARES");

		sdaiPutAttrBN(ifcRelDeclaresInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
		sdaiPutAttrBN(ifcRelDeclaresInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

		aggr_ifcRelDeclaresInstance_RelatedDefinitions = sdaiCreateAggrBN(ifcRelDeclaresInstance,(char*)"RelatedDefinitions");
		sdaiAppend((int_t) aggr_ifcRelDeclaresInstance_RelatedDefinitions, sdaiINSTANCE, (void*) ifcWallTypeInstance);
		sdaiPutAttrBN(ifcRelDeclaresInstance,(char*)"RelatingContext", sdaiINSTANCE, (void*) buildProjectLibrary());
	}
	else {
		sdaiAppend((int_t) aggr_ifcRelDeclaresInstance_RelatedDefinitions, sdaiINSTANCE, (void*) ifcWallTypeInstance);
	}

	assert(ifcRelDeclaresInstance);

	return	ifcRelDeclaresInstance;
}



//
//
//		MaterialConstituentSet, MaterialConstituent
//
//


int_t	buildMaterialConstituentSet()
{
	int_t	ifcMaterialConstituentSetInstance, * aggrMaterialConstituents;

	ifcMaterialConstituentSetInstance = sdaiCreateInstanceBN(model,(char*)"IFCMATERIALCONSTITUENTSET");

	sdaiPutAttrBN(ifcMaterialConstituentSetInstance,(char*)"Name", sdaiSTRING,(char*)"Constituent Set for Window");

	aggrMaterialConstituents = sdaiCreateAggrBN(ifcMaterialConstituentSetInstance,(char*)"MaterialConstituents");
	sdaiAppend((int_t) aggrMaterialConstituents, sdaiINSTANCE, (void*) buildMaterialConstituent((char*)"Glass"));
	sdaiAppend((int_t) aggrMaterialConstituents, sdaiINSTANCE, (void*) buildMaterialConstituent((char*)"Wood"));

	assert(ifcMaterialConstituentSetInstance);

	return	ifcMaterialConstituentSetInstance;
}

int_t	buildMaterialConstituent(char * material)
{
	int_t	ifcMaterialConstituentInstance;

	ifcMaterialConstituentInstance = sdaiCreateInstanceBN(model,(char*)"IFCMATERIALCONSTITUENT");

	sdaiPutAttrBN(ifcMaterialConstituentInstance,(char*)"Name", sdaiSTRING,(char*)"Framing");
	sdaiPutAttrBN(ifcMaterialConstituentInstance,(char*)"Material", sdaiINSTANCE, (void*) buildMaterial(material));

	assert(ifcMaterialConstituentInstance);

	return	ifcMaterialConstituentInstance;
}



//
//
//		RelAssociatesMaterial, MaterialLayerSetUsage, MaterialLayerSet, MaterialLayer
//
//


int_t	buildRelAssociatesMaterial(int_t ifcBuildingElementInstance, double thickness)
{
	assert(ifcBuildingElementInstance);

	int_t	ifcRelAssociatesMaterialInstance, * aggrRelatedObjects;

	ifcRelAssociatesMaterialInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELASSOCIATESMATERIAL");

	sdaiPutAttrBN(ifcRelAssociatesMaterialInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcRelAssociatesMaterialInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	aggrRelatedObjects = sdaiCreateAggrBN(ifcRelAssociatesMaterialInstance,(char*)"RelatedObjects");
	sdaiAppend((int_t) aggrRelatedObjects, sdaiINSTANCE, (void*) ifcBuildingElementInstance);
	sdaiPutAttrBN(ifcRelAssociatesMaterialInstance,(char*)"RelatingMaterial", sdaiINSTANCE, (void*) buildMaterialLayerSetUsage(thickness));

	assert(ifcRelAssociatesMaterialInstance);

	return	ifcRelAssociatesMaterialInstance;
}

int_t	buildRelAssociatesMaterial(int_t ifcBuildingElementInstance)
{
	assert(ifcBuildingElementInstance);

	int_t	ifcRelAssociatesMaterialInstance, * aggrRelatedObjects;

	ifcRelAssociatesMaterialInstance = sdaiCreateInstanceBN(model,(char*)"IFCRELASSOCIATESMATERIAL");

	sdaiPutAttrBN(ifcRelAssociatesMaterialInstance,(char*)"GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcRelAssociatesMaterialInstance,(char*)"OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	aggrRelatedObjects = sdaiCreateAggrBN(ifcRelAssociatesMaterialInstance,(char*)"RelatedObjects");
	sdaiAppend((int_t) aggrRelatedObjects, sdaiINSTANCE, (void*) ifcBuildingElementInstance);
	sdaiPutAttrBN(ifcRelAssociatesMaterialInstance,(char*)"RelatingMaterial", sdaiINSTANCE, (void*) buildMaterialConstituentSet());

	assert(ifcRelAssociatesMaterialInstance);

	return	ifcRelAssociatesMaterialInstance;
}

int_t	buildMaterialLayerSetUsage(double thickness)
{
	int_t	ifcMaterialLayerSetUsageInstance;
	double	offsetFromReferenceLine = -thickness/2;

	ifcMaterialLayerSetUsageInstance = sdaiCreateInstanceBN(model,(char*)"IFCMATERIALLAYERSETUSAGE");

	sdaiPutAttrBN(ifcMaterialLayerSetUsageInstance,(char*)"ForLayerSet", sdaiINSTANCE, (void*) buildMaterialLayerSet(thickness));
	sdaiPutAttrBN(ifcMaterialLayerSetUsageInstance,(char*)"LayerSetDirection", sdaiENUM,(char*)"AXIS2");
	sdaiPutAttrBN(ifcMaterialLayerSetUsageInstance,(char*)"DirectionSense", sdaiENUM,(char*)"POSITIVE");
	sdaiPutAttrBN(ifcMaterialLayerSetUsageInstance,(char*)"OffsetFromReferenceLine", sdaiREAL, &offsetFromReferenceLine);

	assert(ifcMaterialLayerSetUsageInstance);

	return	ifcMaterialLayerSetUsageInstance;
}

int_t	buildMaterialLayerSet(double thickness)
{
	int_t	ifcMaterialLayerSetInstance, * aggrMaterialLayers;

	ifcMaterialLayerSetInstance = sdaiCreateInstanceBN(model,(char*)"IFCMATERIALLAYERSET");

	aggrMaterialLayers = sdaiCreateAggrBN(ifcMaterialLayerSetInstance,(char*)"MaterialLayers");
	sdaiAppend((int_t) aggrMaterialLayers, sdaiINSTANCE, (void*) buildMaterialLayer(thickness));

	assert(ifcMaterialLayerSetInstance);

	return	ifcMaterialLayerSetInstance;
}

int_t	buildMaterialLayer(double thickness)
{
	int_t	ifcMaterialLayerInstance;

	ifcMaterialLayerInstance = sdaiCreateInstanceBN(model,(char*)"IFCMATERIALLAYER");

	sdaiPutAttrBN(ifcMaterialLayerInstance,(char*)"Material", sdaiINSTANCE, (void*) buildMaterial());
	sdaiPutAttrBN(ifcMaterialLayerInstance,(char*)"LayerThickness", sdaiREAL, &thickness);

	assert(ifcMaterialLayerInstance);

	return	ifcMaterialLayerInstance;
}

int_t	buildMaterial()
{
	return	buildMaterial((char*)"Name of the material used for the wall");
}

int_t	buildMaterial(char * name)
{
	int_t	ifcMaterialInstance;

	ifcMaterialInstance = sdaiCreateInstanceBN(model,(char*)"IFCMATERIAL");

	sdaiPutAttrBN(ifcMaterialInstance,(char*)"Name", sdaiSTRING, (void*) name);

	assert(ifcMaterialInstance);

	return	ifcMaterialInstance;
}
