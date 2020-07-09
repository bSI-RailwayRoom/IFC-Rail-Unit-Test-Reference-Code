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


#include "boundingBoxIfc.h"


struct	POINT2D {
	double			x;
	double			y;
};

struct	POLYGON2D {
	POINT2D			* point;
	POLYGON2D		* next;
};



void	createIfcExtrudedPolygonShape(POLYGON2D * polygon, int_t ifcCurveInstance, int_t ifcAxisLateralInclinationInstance, double factor);
void	createIfcPolylineShape(double p0x, double p0y, double p1x, double p1y);


//
//
//		ShapeRepresentation
//
//


int_t	buildShapeRepresentationInstance(int_t type, POLYGON2D * polygon, int_t ifcCurveInstance, int_t ifcAxisLateralInclinationInstance);
int_t	buildShapeRepresentationInstance(int_t type, double p0x, double p0y, double p1x, double p1y);


//
//
//		ArbitraryClosedProfileDef, CartesianPoint(2D), ExtrudedAreaSolid, Polyline
//
//


int_t	buildArbitraryClosedProfileDefInstance(POLYGON2D * polygon);
int_t	buildCartesianPointInstance(double x, double y);
int_t	buildInclinedReferenceSweptAreaSolidInstance(POLYGON2D * polygon, int_t ifcCurveInstance, int_t ifcAxisLateralInclinationInstance);
int_t	buildPolylineInstance(POLYGON2D * polygon);
int_t	buildPolylineInstance(double p0x, double p0y, double p1x, double p1y);
