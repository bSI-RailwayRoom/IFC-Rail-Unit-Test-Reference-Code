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


#include "baseIfcObject.h"


void	createIfcBoundingBoxShape(double width, double thickness, double height, char * representationIdentifier);


//
//
//		BoundingBox, ShapeRepresentation
//
//


int_t	buildBoundingBoxInstance(double width, double thickness, double height);
int_t	buildShapeRepresentationInstance(int_t type, double width, double thickness, double height, char * representationIdentifier);


//
//
//		CartesianPoint(3D)
//
//


int_t	buildCartesianPointInstance(double x, double y, double z);
