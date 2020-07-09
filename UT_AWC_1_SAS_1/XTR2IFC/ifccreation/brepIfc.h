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


#include "extrudedPolygonIfc.h"

struct MATERIAL3D {
	double R, G, B, A;
};

struct POLYGON {
	POINT3D			* point;
	POLYGON			* next;
};

struct POLYGONS {
	POLYGON			* polygon;
	POLYGONS		* next;
};

struct FACE {
	POLYGON			* outerPolygon;
	POLYGONS		* innerPolygons;
	FACE			* next;
	MATERIAL3D      material;
};

struct SHELL {
	FACE			* faces;
	SHELL			* next;
};


void	createIfcBRepShape(SHELL * shell);
void	createIfcBRepShape(SHELL * shell, double p0x, double p0y, double p1x, double p1y);


//
//
//		ShapeRepresentation
//
//


int_t	buildShapeRepresentationInstance(int_t type, SHELL * shell);
