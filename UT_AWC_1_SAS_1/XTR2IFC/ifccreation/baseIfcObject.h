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


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "baseIfc.h"

#define		AXIS_CURVE2D_REPRESENTATION			101
#define		BODY_BOUNDINGBOX_REPRESENTATION		111
#define		BODY_BREP_REPRESENTATION			112
#define		BODY_SWEPTSOLID_REPRESENTATION		113


int_t	createIfcWall(wchar_t * pWallName, double xOffset, double yOffset, double zOffset);
int_t	createIfcWallStandardCase(wchar_t * pWallName, double xOffset, double yOffset, double zOffset);
int_t	createIfcOpeningElement(wchar_t * pOpeningElementName, double xOffset, double yOffset, double zOffset, bool representation);
int_t	createIfcOpeningElement(wchar_t * pOpeningElementName,
								double xRefDirection, double yRefDirection, double zRefDirection,
								double xAxis, double yAxis, double zAxis,
								double xOffset, double yOffset, double zOffset,
								bool representation);
int_t	createIfcWindow(wchar_t * pWindowName, double xOffset, double yOffset, double zOffset, bool insideOpening, double overallHeight, double overallWidth);
int_t	createIfcWindow(wchar_t * pWindowName,
						double xRefDirection, double yRefDirection, double zRefDirection,
						double xAxis, double yAxis, double zAxis,
						double xOffset, double yOffset, double zOffset,
						bool insideOpening,
						double overallHeight,
						double overallWidth);


//
//
//		ProductDefinitionShape
//
//


int_t	buildProductDefinitionShapeInstance();


//
//
//		IfcWall, IfcWallStandardCase, IfcOpeningElement, IfcWindow
//
//


int_t	buildWallInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcWallInstancePlacement, wchar_t * pWallName);
int_t	buildWallStandardCaseInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcWallInstancePlacement, wchar_t * pWallName);
int_t	buildOpeningElementInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcOpeningElementInstancePlacement, wchar_t * pOpeningElementName, bool representation);
int_t	buildWindowInstance(MATRIX * matrix, int_t ifcPlacementRelativeTo, int_t * ifcWindowInstancePlacement, wchar_t * pWindowName, double overallHeight, double overallWidth);


//
//
//		RelVoidsElement, RelFillsElement
//
//


int_t	buildRelVoidsElementInstance(int_t ifcBuildingElementInstance, int_t ifcOpeningElementInstance);
int_t	buildRelFillsElementInstance(int_t ifcOpeningElementInstance, int_t ifcBuildingElementInstance);


//
//
//		ProjectLibrary, RelDeclares
//
//


int_t	buildProjectLibrary();
int_t	buildRelDeclares_ProjectLibrary(int_t ifcProjectLibraryInstance);
int_t	buildRelDeclares_WallType(int_t ifcWallTypeInstance);


//
//
//		MaterialConstituentSet, MaterialConstituent
//
//


int_t	buildMaterialConstituentSet();
int_t	buildMaterialConstituent(char * material);


//
//
//		RelAssociatesMaterial, MaterialLayerSetUsage, MaterialLayerSet, MaterialLayer
//
//


int_t	buildRelAssociatesMaterial(int_t ifcBuildingElementInstance, double thickness);
int_t	buildRelAssociatesMaterial(int_t ifcBuildingElementInstance);
int_t	buildMaterialLayerSetUsage(double thickness);
int_t	buildMaterialLayerSet(double thickness);
int_t	buildMaterialLayer(double thickness);
int_t	buildMaterial();
int_t	buildMaterial(char * name);
