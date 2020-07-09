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

#include	"baseIfcObject.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>


extern	int_t	* aggrRelatedElements,
				ifcSiteInstancePlacement;


//
//
//		BuildingElementProxy
//
//


int_t	buildBuildingElementProxyInstance(int_t ifcModel, char * name, char * description)
{
	int_t	ifcBuildingElementProxyInstance;

	ifcBuildingElementProxyInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCBUILDINGELEMENTPROXY");

	sdaiPutAttrBN(ifcBuildingElementProxyInstance, (char*) "GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcBuildingElementProxyInstance, (char*) "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcBuildingElementProxyInstance, (char*) "Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcBuildingElementProxyInstance, (char*) "Description", sdaiSTRING, description);

	MATRIX	matrix;
	identityMatrix(&matrix);

	int_t	ifcInstancePlacement = buildLocalPlacementInstance(&matrix, ifcSiteInstancePlacement);
	sdaiPutAttrBN(ifcBuildingElementProxyInstance, (char*) "ObjectPlacement", sdaiINSTANCE, (void*) ifcInstancePlacement);
	sdaiPutAttrBN(ifcBuildingElementProxyInstance, (char*) "Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	sdaiAppend((int_t) aggrRelatedElements, sdaiINSTANCE, (void*) ifcBuildingElementProxyInstance);

	assert(ifcBuildingElementProxyInstance);

	return	ifcBuildingElementProxyInstance;
}


//
//
//		Alignment, AlignmentCurve
//
//


int_t	buildAlignmentInstance(int_t ifcModel, char * name, char * description)
{
	int_t	ifcAlignmentInstance;

	ifcAlignmentInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCALIGNMENT");

	sdaiPutAttrBN(ifcAlignmentInstance, (char*) "GlobalId", engiGLOBALID, (void*) 0);
	sdaiPutAttrBN(ifcAlignmentInstance, (char*) "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcAlignmentInstance, (char*) "Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcAlignmentInstance, (char*) "Description", sdaiSTRING, description);

	MATRIX	matrix;
	identityMatrix(&matrix);

	int_t	ifcInstancePlacement = buildLocalPlacementInstance(&matrix, ifcSiteInstancePlacement);
	sdaiPutAttrBN(ifcAlignmentInstance, (char*) "ObjectPlacement", sdaiINSTANCE, (void*) ifcInstancePlacement);

	sdaiAppend((int_t) aggrRelatedElements, sdaiINSTANCE, (void*) ifcAlignmentInstance);

	assert(ifcAlignmentInstance);

	return	ifcAlignmentInstance;
}

int_t	buildAlignmentCurveInstance(int_t ifcModel, int_t ifcAlignmentInstance)
{
	int_t	ifcAlignmentCurveInstance;

	ifcAlignmentCurveInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCALIGNMENTCURVE");

	sdaiPutAttrBN(ifcAlignmentInstance, (char*) "Axis", sdaiINSTANCE, (void*) ifcAlignmentCurveInstance);

	assert(ifcAlignmentCurveInstance);

	return	ifcAlignmentCurveInstance;
}

int_t	buildLinearAxisWithInclinationInstance(int_t ifcModel, int_t ifcAlignmentCurveInstance, int_t ifcAxisLateralInclinationInstance)
{
	int_t	ifcLinearAxisWithInclinationInstance;

	ifcLinearAxisWithInclinationInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCLINEARAXISWITHINCLINATION");

	sdaiPutAttrBN(ifcLinearAxisWithInclinationInstance, (char*) "Directrix", sdaiINSTANCE, (void*) ifcAlignmentCurveInstance);
	sdaiPutAttrBN(ifcLinearAxisWithInclinationInstance, (char*) "Inclinating", sdaiINSTANCE, (void*) ifcAxisLateralInclinationInstance);

	assert(ifcLinearAxisWithInclinationInstance);

	return	ifcLinearAxisWithInclinationInstance;
}


//
//
//		Alignment2DHorizontal, Alignment2DHorizontalSegment
//
//


int_t	buildAlignment2DHorizontalInstance(int_t ifcModel, int_t ifcAlignmentCurveInstance, int_t ** pAggrSegments)
{
	int_t	ifcAlignment2DHorizontalInstance;

	ifcAlignment2DHorizontalInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCALIGNMENT2DHORIZONTAL");

	sdaiPutAttrBN(ifcAlignmentCurveInstance, (char*) "Horizontal", sdaiINSTANCE, (void*) ifcAlignment2DHorizontalInstance);

	(*pAggrSegments) = sdaiCreateAggrBN(ifcAlignment2DHorizontalInstance, (char*) "Segments");

	assert(ifcAlignment2DHorizontalInstance);

	return	ifcAlignment2DHorizontalInstance;
}

int_t	buildAlignment2DHorizontalSegmentInstance(int_t ifcModel, int_t ifcAlignmentCurveInstance, int_t * aggrSegments)
{
	int_t	ifcAlignment2DHorizontalSegmentInstance;

	ifcAlignment2DHorizontalSegmentInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCALIGNMENT2DHORIZONTALSEGMENT");
		
	sdaiPutAttrBN(ifcAlignment2DHorizontalSegmentInstance, (char*) "CurveGeometry", sdaiINSTANCE, (void*) ifcAlignmentCurveInstance);
	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) ifcAlignment2DHorizontalSegmentInstance);

	assert(ifcAlignment2DHorizontalSegmentInstance);

	return	ifcAlignment2DHorizontalSegmentInstance;
}

int_t	buildAlignment2DVerticalInstance(int_t ifcModel, int_t ifcAlignmentCurveInstance, int_t ** pAggrSegments)
{
	int_t	ifcAlignment2DVerticalInstance;

	ifcAlignment2DVerticalInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCALIGNMENT2DVERTICAL");

	sdaiPutAttrBN(ifcAlignmentCurveInstance, (char*) "Vertical", sdaiINSTANCE, (void*) ifcAlignment2DVerticalInstance);

	(*pAggrSegments) = sdaiCreateAggrBN(ifcAlignment2DVerticalInstance, (char*) "Segments");

	assert(ifcAlignment2DVerticalInstance);

	return	ifcAlignment2DVerticalInstance;
}

int_t	buildAlignment2DCantInstance(int_t ifcModel, int_t ** pAggrSegments, double trackGauge)
{
	int_t	ifcAlignment2DCantInstance;

	ifcAlignment2DCantInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCALIGNMENT2DCANT");

//	sdaiPutAttrBN(ifcAlignmentCurveInstance, (char*) "Vertical", sdaiINSTANCE, (void*)ifcAlignment2DVerticalInstance);
	sdaiPutAttrBN(ifcAlignment2DCantInstance, (char*) "RailHeadDistance", sdaiREAL, (void*) &trackGauge);

	(*pAggrSegments) = sdaiCreateAggrBN(ifcAlignment2DCantInstance, (char*) "Segments");

	assert(ifcAlignment2DCantInstance);

	return	ifcAlignment2DCantInstance;
}

int_t	buildLineSegment2DInstance(
				int_t	ifcModel,
				double	easting,
				double	northing,
				double	azimuth,
				double	element_length,
				double	start_radius,
				double	end_radius
			)
{
	int_t	ifcLineSegment2DInstance;

	ifcLineSegment2DInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCLINESEGMENT2D");

	double	x = easting,
			y = northing;
	sdaiPutAttrBN(ifcLineSegment2DInstance, (char*) "StartPoint", sdaiINSTANCE, (void*)buildCartesianPointInstance(x, y));
	sdaiPutAttrBN(ifcLineSegment2DInstance, (char*) "StartDirection", sdaiREAL, (void*)&azimuth);
	sdaiPutAttrBN(ifcLineSegment2DInstance, (char*) "SegmentLength", sdaiREAL, (void*)&element_length);

	assert(start_radius == 0 && end_radius == 0);
	assert(ifcLineSegment2DInstance);

	return	ifcLineSegment2DInstance;
}

int_t	buildCircularArcSegment2DInstance(
				int_t	ifcModel,
				double	easting,
				double	northing,
				double	azimuth,
				double	element_length,
				double	start_radius,
				double	end_radius
			)
{
	int_t	ifcCircularArcSegment2DInstance;

	ifcCircularArcSegment2DInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCCIRCULARARCSEGMENT2D");

	double	x = easting,
			y = northing;
	sdaiPutAttrBN(ifcCircularArcSegment2DInstance, (char*) "StartPoint", sdaiINSTANCE, (void*) buildCartesianPointInstance(x, y));
	sdaiPutAttrBN(ifcCircularArcSegment2DInstance, (char*) "StartDirection", sdaiREAL, (void*) &azimuth);
	sdaiPutAttrBN(ifcCircularArcSegment2DInstance, (char*) "SegmentLength", sdaiREAL, (void*) &element_length);

	assert(start_radius == end_radius);
	if (start_radius < 0) {
		start_radius = -start_radius;
		sdaiPutAttrBN(ifcCircularArcSegment2DInstance, (char*) "IsCCW", sdaiENUM, (char*) "T");
	}
	else {
		sdaiPutAttrBN(ifcCircularArcSegment2DInstance, (char*) "IsCCW", sdaiENUM, (char*) "F");
	}
	sdaiPutAttrBN(ifcCircularArcSegment2DInstance, (char*) "Radius", sdaiREAL, (void*) &start_radius);

	assert(ifcCircularArcSegment2DInstance);

	return	ifcCircularArcSegment2DInstance;
}

int_t	buildTransitionCurveSegment2DInstance(
				int_t	ifcModel,
				double	easting,
				double	northing,
				double	azimuth,
				double	element_length,
				double	start_radius,
				double	end_radius
			)
{
	int_t	ifcTransitionCurveSegment2DInstance;

	ifcTransitionCurveSegment2DInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCTRANSITIONCURVESEGMENT2D");

	double	x = easting,
			y = northing;
	sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "StartPoint", sdaiINSTANCE, (void*) buildCartesianPointInstance(x, y));
	sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "StartDirection", sdaiREAL, (void*) &azimuth);
	sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "SegmentLength", sdaiREAL, (void*) &element_length);

	if (start_radius < 0) {
		start_radius = -start_radius;
		sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "IsStartRadiusCCW", sdaiENUM, (char*) "T");
	}
	else {
		sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "IsStartRadiusCCW", sdaiENUM, (char*) "F");
	}
	sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "StartRadius", sdaiREAL, (void*) &start_radius);

	if (end_radius < 0) {
		end_radius = -end_radius;
		sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "IsEndRadiusCCW", sdaiENUM, (char*) "T");
	}
	else {
		sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "IsEndRadiusCCW", sdaiENUM, (char*) "F");
	}
	sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "EndRadius", sdaiREAL, (void*) &end_radius);

	sdaiPutAttrBN(ifcTransitionCurveSegment2DInstance, (char*) "TransitionCurveType", sdaiENUM, (char*) "CLOTHOIDCURVE");

	assert(ifcTransitionCurveSegment2DInstance);

	return	ifcTransitionCurveSegment2DInstance;
}

int_t	buildAlignment2DVerSegLineInstance(
				int_t	ifcModel,
				double	metraege,
				double	element_length,
				double	gradient,
				double	radius,
				double	altitude
			)
{
	int_t	ifcAlignment2DVerSegLineInstance;

	ifcAlignment2DVerSegLineInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCALIGNMENT2DVERSEGLINE");

	sdaiPutAttrBN(ifcAlignment2DVerSegLineInstance, (char*) "StartDistAlong", sdaiREAL, (void*) &metraege);
	sdaiPutAttrBN(ifcAlignment2DVerSegLineInstance, (char*) "HorizontalLength", sdaiREAL, (void*) &element_length);
	sdaiPutAttrBN(ifcAlignment2DVerSegLineInstance, (char*) "StartHeight", sdaiREAL, (void*) &altitude);
	sdaiPutAttrBN(ifcAlignment2DVerSegLineInstance, (char*) "StartGradient", sdaiREAL, (void*) &gradient);

	assert(radius == 0);
	assert(ifcAlignment2DVerSegLineInstance);

	return	ifcAlignment2DVerSegLineInstance;
}

int_t	buildAlignment2DVerSegCircularArcInstance(
				int_t	ifcModel,
				double	metraege,
				double	element_length,
				double	gradient,
				double	radius,
				double	altitude
			)
{
	int_t	ifcAlignment2DVerSegCircularArcInstance;

	ifcAlignment2DVerSegCircularArcInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCALIGNMENT2DVERSEGCIRCULARARC");
	
	sdaiPutAttrBN(ifcAlignment2DVerSegCircularArcInstance, (char*) "StartDistAlong", sdaiREAL, (void*) &metraege);
	sdaiPutAttrBN(ifcAlignment2DVerSegCircularArcInstance, (char*) "HorizontalLength", sdaiREAL, (void*) &element_length);
	sdaiPutAttrBN(ifcAlignment2DVerSegCircularArcInstance, (char*) "StartHeight", sdaiREAL, (void*) &altitude);
	sdaiPutAttrBN(ifcAlignment2DVerSegCircularArcInstance, (char*) "StartGradient", sdaiREAL, (void*) &gradient);

	if (radius < 0) {
		radius = -radius;
		sdaiPutAttrBN(ifcAlignment2DVerSegCircularArcInstance, (char*) "IsConvex", sdaiENUM, (char*) "T");
	}
	else {
		sdaiPutAttrBN(ifcAlignment2DVerSegCircularArcInstance, (char*) "IsConvex", sdaiENUM, (char*) "F");
	}
	sdaiPutAttrBN(ifcAlignment2DVerSegCircularArcInstance, (char*) "Radius", sdaiREAL, (void*) &radius);

	assert(ifcAlignment2DVerSegCircularArcInstance);

	return	ifcAlignment2DVerSegCircularArcInstance;
}

void	appendAlignment2DVerticalSegmentInstance(int_t ifcAlignment2DVerticalSegmentInstance, int_t * aggrSegments)
{
	sdaiAppend((int_t) aggrSegments, sdaiINSTANCE, (void*) ifcAlignment2DVerticalSegmentInstance);
}

void	appendAlignment2DCantSegmentInstance(int_t ifcAlignment2DVerticalSegmentInstance, int_t * aggrSegments)
{
	sdaiAppend((int_t)aggrSegments, sdaiINSTANCE, (void*)ifcAlignment2DVerticalSegmentInstance);
}

int_t	buildAlignment2DCantSegLineInstance(
				int_t	ifcModel,
				double	metraege,
				double	element_length,
				double	start_cant_left,
				double	end_cant_left,
				double	start_cant_right,
				double	end_cant_right,
				double	speed
			)
{
	int_t	ifcAlignment2DCantSegLineInstance;

	ifcAlignment2DCantSegLineInstance = sdaiCreateInstanceBN(ifcModel, (char*) "IFCALIGNMENT2DCANTSEGLINE");
	
	sdaiPutAttrBN(ifcAlignment2DCantSegLineInstance, (char*) "StartDistAlong", sdaiREAL, (void*) &metraege);
	sdaiPutAttrBN(ifcAlignment2DCantSegLineInstance, (char*) "HorizontalLength", sdaiREAL, (void*) &element_length);
	sdaiPutAttrBN(ifcAlignment2DCantSegLineInstance, (char*) "StartCantLeft", sdaiREAL, (void*) &start_cant_left);
	sdaiPutAttrBN(ifcAlignment2DCantSegLineInstance, (char*) "EndCantLeft", sdaiREAL, (void*) &end_cant_left);
	sdaiPutAttrBN(ifcAlignment2DCantSegLineInstance, (char*) "StartCantRight", sdaiREAL, (void*) &start_cant_right);
	sdaiPutAttrBN(ifcAlignment2DCantSegLineInstance, (char*) "EndCantRight", sdaiREAL, (void*) &end_cant_right);

	assert(ifcAlignment2DCantSegLineInstance);

	return	ifcAlignment2DCantSegLineInstance;
}
