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
#include	"extrudedPolygonIfc.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>


extern	int_t	model,
				* aggrRelatedElements,
				* aggrRepresentations;
	
double	global_factor = 1;

void	createIfcExtrudedPolygonShape(POLYGON2D * polygon, int_t ifcCurveInstance, int_t ifcAxisLateralInclinationInstance, double factor)
{
	global_factor = factor / 1000.;
	assert(aggrRepresentations);

	sdaiAppend((int_t)aggrRepresentations, sdaiINSTANCE, (void*)buildShapeRepresentationInstance(BODY_SWEPTSOLID_REPRESENTATION, polygon, ifcCurveInstance, ifcAxisLateralInclinationInstance));
}

void	createIfcPolylineShape(double p0x, double p0y, double p1x, double p1y)
{
	assert(aggrRepresentations);

	sdaiAppend((int_t) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(AXIS_CURVE2D_REPRESENTATION, p0x, p0y, p1x, p1y));
}


//
//
//		ShapeRepresentation
//
//


int_t	buildShapeRepresentationInstance(int_t type, POLYGON2D * polygon, int_t ifcCurveInstance, int_t ifcAxisLateralInclinationInstance)
{
	int_t	ifcShapeRepresentationInstance, * aggrItems;

	ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model,(char*)"IFCSHAPEREPRESENTATION");

	aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance,(char*)"Items");
	switch	(type) {
		case  BODY_SWEPTSOLID_REPRESENTATION:
			sdaiAppend((int_t)aggrItems, sdaiINSTANCE, (void*) buildInclinedReferenceSweptAreaSolidInstance(polygon, ifcCurveInstance, ifcAxisLateralInclinationInstance));

			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"RepresentationIdentifier", sdaiSTRING,(char*)"Body");
			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"RepresentationType", sdaiSTRING,(char*)"SweptSolid");
			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"ContextOfItems", sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());
			break;
		default:
			assert(false);
			break;
	}

	assert(ifcShapeRepresentationInstance);

	return	ifcShapeRepresentationInstance;
}

int_t	buildShapeRepresentationInstance(int_t type, double p0x, double p0y, double p1x, double p1y)
{
	int_t	ifcShapeRepresentationInstance, * aggrItems;

	ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model,(char*)"IFCSHAPEREPRESENTATION");

	aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance,(char*)"Items");
	switch  (type) {
		case  AXIS_CURVE2D_REPRESENTATION:
			sdaiAppend((int_t) aggrItems, sdaiINSTANCE, (void*) buildPolylineInstance(p0x, p0y, p1x, p1y));

			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"RepresentationIdentifier", sdaiSTRING,(char*)"Axis");
			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"RepresentationType", sdaiSTRING,(char*)"Curve2D");
			sdaiPutAttrBN(ifcShapeRepresentationInstance,(char*)"ContextOfItems", sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());
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
//		ArbitraryClosedProfileDef, CartesianPoint(2D), ExtrudedAreaSolid, Polyline
//
//

struct VECTOR2
{
	double	x, y;
};

int_t	buildCartesianPointInstance(VECTOR2 pnt)
{
	int_t	ifcCartesianPointInstance, * aggrCoordinates;

	pnt.x *= global_factor;
	pnt.y *= global_factor;

	//
	//	rotate
	//
	double	tmp = pnt.x;
	pnt.x = pnt.y;
	pnt.y = -tmp;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, (char*) "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, (char*) "Coordinates");

	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, (void*) &pnt.x);
	sdaiAppend((int_t) aggrCoordinates, sdaiREAL, (void*) &pnt.y);

	assert(ifcCartesianPointInstance);

	return	ifcCartesianPointInstance;
}

int_t	buildPolylineInstance(VECTOR2 pnt1, VECTOR2 pnt2)
{
	int_t	ifcPolylineInstance, * aggrPoints;

	ifcPolylineInstance = sdaiCreateInstanceBN(model, (char*) "IFCPOLYLINE");

	aggrPoints = sdaiCreateAggrBN(ifcPolylineInstance, (char*) "Points");

	sdaiAppend((int_t) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(pnt1));
	sdaiAppend((int_t) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(pnt2));

	assert(ifcPolylineInstance);

	return	ifcPolylineInstance;
}

int_t	buildCompositeCurveSegmentInstance(bool sameSense, VECTOR2 pnt1, VECTOR2 pnt2)
{
	int_t	ifcCompositeCurveSegmentInstance;

	ifcCompositeCurveSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCCOMPOSITECURVESEGMENT");

	sdaiPutAttrBN(ifcCompositeCurveSegmentInstance, (char*) "Transition", sdaiENUM, (char*) "CONTINOUS");
	if (sameSense) {
		sdaiPutAttrBN(ifcCompositeCurveSegmentInstance, (char*) "SameSense", sdaiENUM, (char*) "T");
	}
	else {
		sdaiPutAttrBN(ifcCompositeCurveSegmentInstance, (char*) "SameSense", sdaiENUM, (char*) "F");
	}
	sdaiPutAttrBN(ifcCompositeCurveSegmentInstance, (char*) "ParentCurve", sdaiINSTANCE, (void*) buildPolylineInstance(pnt1, pnt2));

	assert(ifcCompositeCurveSegmentInstance);

	return	ifcCompositeCurveSegmentInstance;
}

int_t	buildProfile()
{
	int_t	ifcCompositeCurveInstance, * aggrSegments;

	ifcCompositeCurveInstance = sdaiCreateInstanceBN(model, (char*) "IFCCOMPOSITECURVE");

	aggrSegments = sdaiCreateAggrBN(ifcCompositeCurveInstance, (char*) "Segments");

	//	143
	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(true, { -9.98015093595997,7.1399999999993 }, { -24.5200347039908,8.38272510837836 }));
	//	154
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	164
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	171
	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(true, { -36.5547740966101,14.5074202858492 }, { -41.0194805834387,32.3772430471736 }));
	//	181
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	188
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { -42.0090617451543,33.1500000000023 }, { -64.2441509359607,33.1500000000019 }));
	//	198
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	208
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	218
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	228
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	238
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	245
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { -64.2441509359619,-33.1499999999991 }, { -42.0090617451567,-33.1499999999981 }));
	//	253
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	260
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { -41.0194805834404,-32.3772430471739 }, { -36.5547740966108,-14.5074202858471 }));
	//	270
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	280
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	287
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { -24.5200347039921,-8.38272510837894 }, { -9.98015093595934,-7.14000000000091 }));
	//	294
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { -9.98015093595928,-7.14000000000094 }, { 20.6198490640388,-7.14000000000042 }));
	//	301
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { 20.6198490640388,-7.14000000000039 }, { 37.7098304117724,-10.0613643329462 }));
	//	311
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	321
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	328
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { 47.5647585096521,-16.0052461763333 }, { 59.09597432816,-62.2040275004898 }));
	//	338
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	345
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { 61.0752511730617,-63.7500000000013 }, { 68.049849064039,-63.750000000003 }));
	//	353
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	360
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { 69.0698490640398,-62.7300000000012 }, { 69.0698490640404,62.7299999999984 }));
	//	368
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	375
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { 68.0498490640411,63.7499999999992 }, { 61.0752511730629,63.7499999999977 }));
	//	383
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	390
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { 59.0959743281626,62.2040275004861 }, { 47.5647585096522,16.0052461763318 }));
	//	400
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	410
//	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(???));
	//	417
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)buildCompositeCurveSegmentInstance(true, { 37.7098304117731,10.0613643329451 }, { 20.6198490640395,7.13999999999922 } ));
	//	424
	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) buildCompositeCurveSegmentInstance(true, { 20.6198490640395,7.13999999999922 }, { -9.98015093595991,7.13999999999927 }));

	assert(ifcCompositeCurveInstance);

	return	ifcCompositeCurveInstance;
}

int_t	buildArbitraryClosedProfileDefInstance(POLYGON2D * polygon)
{
	int_t	ifcArbitraryClosedProfileDefInstance;

	ifcArbitraryClosedProfileDefInstance = sdaiCreateInstanceBN(model,(char*)"IFCARBITRARYCLOSEDPROFILEDEF");

	sdaiPutAttrBN(ifcArbitraryClosedProfileDefInstance,(char*)"ProfileType", sdaiENUM,(char*)"AREA");
//	sdaiPutAttrBN(ifcArbitraryClosedProfileDefInstance,(char*)"OuterCurve", sdaiINSTANCE, (void*) buildPolylineInstance(polygon));
	sdaiPutAttrBN(ifcArbitraryClosedProfileDefInstance,(char*)"OuterCurve", sdaiINSTANCE, (void*) buildProfile());

	assert(ifcArbitraryClosedProfileDefInstance);

	return	ifcArbitraryClosedProfileDefInstance;
}

int_t	buildInclinedReferenceSweptAreaSolidInstance(POLYGON2D * polygon, int_t ifcCurveInstance, int_t ifcAxisLateralInclinationInstance)
{
	MATRIX	matrix;
	int_t	ifcInclinedReferenceSweptAreaSolidInstance;

	identityMatrix(&matrix);

	ifcInclinedReferenceSweptAreaSolidInstance = sdaiCreateInstanceBN(model, (char*) "IFCINCLINEDREFERENCESWEPTAREASOLID");

	sdaiPutAttrBN(ifcInclinedReferenceSweptAreaSolidInstance, (char*) "SweptArea", sdaiINSTANCE, (void*) buildArbitraryClosedProfileDefInstance(polygon));
	sdaiPutAttrBN(ifcInclinedReferenceSweptAreaSolidInstance, (char*) "Position", sdaiINSTANCE, (void*) buildAxis2Placement3DInstance(&matrix));
	sdaiPutAttrBN(ifcInclinedReferenceSweptAreaSolidInstance, (char*) "FixedReference", sdaiINSTANCE, (void*) buildDirectionInstance((POINT3D*) &matrix._11));
	sdaiPutAttrBN(ifcInclinedReferenceSweptAreaSolidInstance, (char*) "Directrix", sdaiINSTANCE, (void*) ifcCurveInstance);

	sdaiPutAttrBN(ifcInclinedReferenceSweptAreaSolidInstance, (char*) "Directrix", sdaiINSTANCE, (void*) ifcCurveInstance);

	sdaiPutAttrBN(ifcInclinedReferenceSweptAreaSolidInstance, (char*) "Inclinating", sdaiINSTANCE, (void*) ifcAxisLateralInclinationInstance);

	assert(ifcInclinedReferenceSweptAreaSolidInstance);

	return	ifcInclinedReferenceSweptAreaSolidInstance;
}

int_t	buildPolylineInstance(POLYGON2D * polygon)
{
	int_t	ifcPolylineInstance, * aggrPoints;

	ifcPolylineInstance = sdaiCreateInstanceBN(model,(char*)"IFCPOLYLINE");

	aggrPoints = sdaiCreateAggrBN(ifcPolylineInstance,(char*)"Points");
	double	x = polygon->point->x,
			y = polygon->point->y;
	while  (polygon) {
		sdaiAppend((int_t) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(polygon->point->x, polygon->point->y));
		polygon = polygon->next;
	}
	sdaiAppend((int_t) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(x, y));

	assert(ifcPolylineInstance);

	return	ifcPolylineInstance;
}

int_t	buildPolylineInstance(double p0x, double p0y, double p1x, double p1y)
{
	int_t	ifcPolylineInstance, * aggrPoints;

	ifcPolylineInstance = sdaiCreateInstanceBN(model,(char*)"IFCPOLYLINE");

	aggrPoints = sdaiCreateAggrBN(ifcPolylineInstance,(char*)"Points");
	sdaiAppend((int_t) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(p0x, p0y));
	sdaiAppend((int_t) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(p1x, p1y));

	assert(ifcPolylineInstance);

	return	ifcPolylineInstance;
}
