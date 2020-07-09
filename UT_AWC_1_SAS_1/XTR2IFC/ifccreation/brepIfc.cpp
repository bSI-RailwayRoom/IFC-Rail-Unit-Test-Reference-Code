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
#include	"BRepIfc.h"
#include	"materialIfc.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>


extern	int_t	model,
				* aggrRelatedElements,
				* aggrRepresentations;


void	createIfcBRepShape(SHELL * shell)
{
	assert(aggrRepresentations);

	sdaiAppend((int_t) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(BODY_BREP_REPRESENTATION, shell));
}

void	createIfcBRepShape(SHELL * shell, double p0x, double p0y, double p1x, double p1y)
{
	assert(aggrRepresentations);

	sdaiAppend((int_t) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(AXIS_CURVE2D_REPRESENTATION, p0x, p0y, p1x, p1y));
	sdaiAppend((int_t) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(BODY_BREP_REPRESENTATION, shell));
}


//
//
//		ShapeRepresentation
//
//


int_t	buildShapeRepresentationInstance(int_t type, SHELL * shell)
{
	int_t	ifcShapeRepresentationInstance, * aggrItems;

	ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model,(char*)"IFCSHAPEREPRESENTATION");

	aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance,(char*)"Items");

	switch (type) {
		case  BODY_BREP_REPRESENTATION:
			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"RepresentationIdentifier", sdaiSTRING,(char*)"Body");
			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"RepresentationType", sdaiSTRING,(char*)"Brep");
			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"ContextOfItems", sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());

			while (shell) {
				FACE		* faces = shell->faces;
				MATERIAL3D	* material = &faces->material;
				while (faces) {
					int_t	ifcFacetedBrepInstance, ifcClosedShellInstance, *aggrCfsFaces;

					ifcClosedShellInstance = sdaiCreateInstanceBN(model, (char*)"IFCCLOSEDSHELL");
					aggrCfsFaces = sdaiCreateAggrBN(ifcClosedShellInstance, (char*)"CfsFaces");

					while ((faces) &&
						   (faces->material.R == material->R) &&
						   (faces->material.G == material->G) &&
						   (faces->material.B == material->B) &&
						   (faces->material.A == material->A)) {
						int_t	ifcPolyLoopInstance, * aggrPolygon,
								ifcFaceOuterBoundInstance,
								ifcFaceInstance, * aggrBounds;

						ifcPolyLoopInstance = sdaiCreateInstanceBN(model,(char*)"IFCPOLYLOOP");
						aggrPolygon = sdaiCreateAggrBN(ifcPolyLoopInstance,(char*)"Polygon");

						POLYGON		* outerPolygon = faces->outerPolygon;
						while (outerPolygon) {
							POINT3D	* point = outerPolygon->point;
							//
							//	Check if point is already written
							//
							if (!point->ifcCartesianPointInstance) {
								point->ifcCartesianPointInstance = buildCartesianPointInstance(point->x, point->y, point->z);
							}

							sdaiAppend((int_t) aggrPolygon, sdaiINSTANCE, (void*) point->ifcCartesianPointInstance);
							outerPolygon = outerPolygon->next;
						}

						ifcFaceOuterBoundInstance = sdaiCreateInstanceBN(model,(char*)"IFCFACEOUTERBOUND");
						sdaiPutAttrBN(ifcFaceOuterBoundInstance,(char*)"Bound", sdaiINSTANCE, (void*) ifcPolyLoopInstance);
						sdaiPutAttrBN(ifcFaceOuterBoundInstance,(char*)"Orientation", sdaiENUM,(char*)"T");

						ifcFaceInstance = sdaiCreateInstanceBN(model,(char*)"IFCFACE");
						aggrBounds = sdaiCreateAggrBN(ifcFaceInstance,(char*)"Bounds");
						sdaiAppend((int_t) aggrBounds, sdaiINSTANCE, (void*) ifcFaceOuterBoundInstance);

						sdaiAppend((int_t) aggrCfsFaces, sdaiINSTANCE, (void*) ifcFaceInstance);

						POLYGONS	* innerPolygons = faces->innerPolygons;
						while (innerPolygons) {
							int_t	ifcFaceBoundInstance;

							ifcPolyLoopInstance = sdaiCreateInstanceBN(model,(char*)"IFCPOLYLOOP");
							aggrPolygon = sdaiCreateAggrBN(ifcPolyLoopInstance,(char*)"Polygon");

							POLYGON	* innerPolygon = innerPolygons->polygon;
							while (innerPolygon) {
								POINT3D	* point = innerPolygon->point;
								//
								//	Check if point is already written
								//
								if (!point->ifcCartesianPointInstance) {
									point->ifcCartesianPointInstance = buildCartesianPointInstance(point->x, point->y, point->z);
								}

								sdaiAppend((int_t) aggrPolygon, sdaiINSTANCE, (void*) point->ifcCartesianPointInstance);

								innerPolygon = innerPolygon->next;
							}

							ifcFaceBoundInstance = sdaiCreateInstanceBN(model,(char*)"IFCFACEBOUND");
							sdaiPutAttrBN(ifcFaceBoundInstance,(char*)"Bound", sdaiINSTANCE, (void*) ifcPolyLoopInstance);
							sdaiPutAttrBN(ifcFaceBoundInstance,(char*)"Orientation", sdaiENUM,(char*)"T");

							sdaiAppend((int_t) aggrBounds, sdaiINSTANCE, (void*) ifcFaceBoundInstance);

							innerPolygons = innerPolygons->next;
						}

						faces = faces->next;
					}

					ifcFacetedBrepInstance = sdaiCreateInstanceBN(model,(char*)"IFCFACETEDBREP");
					sdaiPutAttrBN(ifcFacetedBrepInstance,(char*)"Outer", sdaiINSTANCE, (void*) ifcClosedShellInstance);

					buildStyledItemInstance(material, ifcFacetedBrepInstance);

					sdaiAppend((int_t) aggrItems, sdaiINSTANCE, (void*) ifcFacetedBrepInstance);

					if (faces) {
						material = &faces->material;
					}
				}

				shell = shell->next;
			}
			break;
		default:
			assert(false);
			break;
	}

	assert(ifcShapeRepresentationInstance);

	return	ifcShapeRepresentationInstance;
}
