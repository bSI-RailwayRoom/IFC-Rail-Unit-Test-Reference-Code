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
#include	"materialIfc.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>


extern	int_t	model,
				* aggrRelatedElements,
				* aggrRepresentations,
				ifcBuildingStoreyInstancePlacement;


//
//
//		ColourRgb, SurfaceStyleRendering, SurfaceStyle
//
//


int_t	buildColourRgbInstance(MATERIAL3D * material)
{
	int_t	ifcColourRgbInstance;

	ifcColourRgbInstance = sdaiCreateInstanceBN(model, (char*)"IFCCOLOURRGB");

	sdaiPutAttrBN(ifcColourRgbInstance, (char*)"Red", sdaiREAL, &material->R);
	sdaiPutAttrBN(ifcColourRgbInstance, (char*)"Green", sdaiREAL, &material->G);
	sdaiPutAttrBN(ifcColourRgbInstance, (char*)"Blue", sdaiREAL, &material->B);

	assert(ifcColourRgbInstance);

	return	ifcColourRgbInstance;
}

int_t	buildSurfaceStyleRenderingInstance(MATERIAL3D * material)
{
	int_t	ifcSurfaceStyleRenderingInstance;

	ifcSurfaceStyleRenderingInstance = sdaiCreateInstanceBN(model, (char*)"IFCSURFACESTYLERENDERING");

	double	transparency = 1 - material->A;
	if (transparency < 0) {
		transparency = 0;
	}
	else if (transparency > 1) {
		transparency = 1;
	}

	sdaiPutAttrBN(ifcSurfaceStyleRenderingInstance, (char*)"SurfaceColour", sdaiINSTANCE, (void*)buildColourRgbInstance(material));
	sdaiPutAttrBN(ifcSurfaceStyleRenderingInstance, (char*)"SpecularColour", sdaiINSTANCE, (void*)buildColourRgbInstance(material));
	sdaiPutAttrBN(ifcSurfaceStyleRenderingInstance, (char*)"DiffuseColour", sdaiINSTANCE, (void*)buildColourRgbInstance(material));
	sdaiPutAttrBN(ifcSurfaceStyleRenderingInstance, (char*)"Transparency", sdaiREAL, &transparency);
	sdaiPutAttrBN(ifcSurfaceStyleRenderingInstance, (char*)"ReflectanceMethod", sdaiENUM, (char*)"NOTDEFINED");

//	double	diffuseColourNormalizedRatioMeasure = 0.62;
//	void	* diffuseColourADB = sdaiCreateADB(sdaiREAL, (int_t*)&diffuseColourNormalizedRatioMeasure);
//	sdaiPutADBTypePath(diffuseColourADB, 1, (char*)"IFCNORMALISEDRATIOMEASURE");
//	sdaiPutAttrBN(ifcSurfaceStyleRenderingInstance, (char*)"DiffuseColour", sdaiADB, (void*)diffuseColourADB);

//	double	specularColourNormalizedRatioMeasure = 0.65;
//	void	* specularColourADB = sdaiCreateADB(sdaiREAL, (int_t*)&diffuseColourNormalizedRatioMeasure);
//	sdaiPutADBTypePath(specularColourADB, 1, (char*)"IFCNORMALISEDRATIOMEASURE");
//	sdaiPutAttrBN(ifcSurfaceStyleRenderingInstance, (char*)"SpecularColour", sdaiADB, (void*)specularColourADB);

	assert(ifcSurfaceStyleRenderingInstance);

	return	ifcSurfaceStyleRenderingInstance;
}

int_t	buildSurfaceStyleInstance(MATERIAL3D * material)
{
	int_t	ifcSurfaceStyleInstance, * aggrStyles;

	ifcSurfaceStyleInstance = sdaiCreateInstanceBN(model, (char*)"IFCSURFACESTYLE");

	sdaiPutAttrBN(ifcSurfaceStyleInstance, (char*)"Side", sdaiENUM, (char*)"BOTH");

	aggrStyles = sdaiCreateAggrBN(ifcSurfaceStyleInstance, (char*)"Styles");
	sdaiAppend((int_t)aggrStyles, sdaiINSTANCE, (void*)buildSurfaceStyleRenderingInstance(material));

	assert(ifcSurfaceStyleInstance);

	return	ifcSurfaceStyleInstance;
}


//
//
//		PresentationStyleAssignment, StyledItem
//
//


int_t	buildPresentationStyleAssignmentInstance(MATERIAL3D * material)
{
	int_t	ifcPresentationStyleAssignmentInstance, *aggrStyles;

	ifcPresentationStyleAssignmentInstance = sdaiCreateInstanceBN(model, (char*)"IFCPRESENTATIONSTYLEASSIGNMENT");

	aggrStyles = sdaiCreateAggrBN(ifcPresentationStyleAssignmentInstance, (char*)"Styles");
	sdaiAppend((int_t)aggrStyles, sdaiINSTANCE, (void*)buildSurfaceStyleInstance(material));

	assert(ifcPresentationStyleAssignmentInstance);

	return	ifcPresentationStyleAssignmentInstance;
}

int_t	buildStyledItemInstance(MATERIAL3D * material, int_t ifcRepresentationItemInstance)
{
	int_t	ifcStyledItemInstance, *aggrStyles;

	ifcStyledItemInstance = sdaiCreateInstanceBN(model, (char*)"IFCSTYLEDITEM");

	sdaiPutAttrBN(ifcStyledItemInstance, (char*)"Item", sdaiINSTANCE, (void*)ifcRepresentationItemInstance);
	aggrStyles = sdaiCreateAggrBN(ifcStyledItemInstance, (char*)"Styles");
	sdaiAppend((int_t)aggrStyles, sdaiINSTANCE, (void*)buildPresentationStyleAssignmentInstance(material));

	assert(ifcStyledItemInstance);

	return	ifcStyledItemInstance;
}
