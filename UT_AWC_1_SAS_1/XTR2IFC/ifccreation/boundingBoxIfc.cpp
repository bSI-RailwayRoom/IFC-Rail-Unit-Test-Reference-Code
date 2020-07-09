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
#include	"boundingBoxIfc.h"
#include	"extrudedPolygonIfc.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>


extern	int_t	model,
				* aggrRelatedElements,
				* aggrRepresentations,
				ifcBuildingStoreyInstancePlacement;


void	createIfcBoundingBoxShape(double width, double thickness, double height, char * representationIdentifier)
{
	assert(aggrRepresentations);

	sdaiAppend((int_t) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(BODY_BOUNDINGBOX_REPRESENTATION, width, thickness, height, representationIdentifier));
}


//
//
//		BoundingBox, ShapeRepresentation
//
//


int_t	buildBoundingBoxInstance(double width, double thickness, double height)
{
	int_t	ifcBoundingBoxInstance;

	ifcBoundingBoxInstance = sdaiCreateInstanceBN(model,(char*)"IFCBOUNDINGBOX");

	sdaiPutAttrBN(ifcBoundingBoxInstance,(char*)"Corner", sdaiINSTANCE, (void*) buildCartesianPointInstance(0, 0, 0));
	sdaiPutAttrBN(ifcBoundingBoxInstance,(char*)"XDim", sdaiREAL, &width);
	sdaiPutAttrBN(ifcBoundingBoxInstance,(char*)"YDim", sdaiREAL, &thickness);
	sdaiPutAttrBN(ifcBoundingBoxInstance,(char*)"ZDim", sdaiREAL, &height);

	assert(ifcBoundingBoxInstance);

	return	ifcBoundingBoxInstance;
}

int_t	buildShapeRepresentationInstance(int_t type, double width, double thickness, double height, char * representationIdentifier)
{
	int_t	ifcShapeRepresentationInstance, *aggrItems;

	ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model,(char*)"IFCSHAPEREPRESENTATION");

	sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"ContextOfItems", sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());
	switch  (type) {
		case  BODY_BOUNDINGBOX_REPRESENTATION:
			aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance,(char*)"Items");
			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"RepresentationIdentifier", sdaiSTRING, representationIdentifier);
			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"RepresentationType", sdaiSTRING,(char*)"BoundingBox");
			sdaiAppend((int_t) aggrItems, sdaiINSTANCE, (void*) buildBoundingBoxInstance(width, thickness, height));
			break;
		default:
			assert(false);
			break;
	}

	assert(ifcShapeRepresentationInstance);

	return	ifcShapeRepresentationInstance;
}


//
//
//		CartesianPoint(3D)
//
//



int_t	buildCartesianPointInstance(double x, double y, double z)
{
	int_t	ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model,(char*)"IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance,(char*)"Coordinates");
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &x);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &y);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &z);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}
