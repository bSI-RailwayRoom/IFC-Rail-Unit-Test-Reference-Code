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


#include "../ifcengine/include/ifcengine.h"


//
//
//		BuildingElementProxy
//
//


int_t	buildBuildingElementProxyInstance(int_t ifcModel, char * name, char * description);


//
//
//		Alignment, AlignmentCurve
//
//


int_t	buildAlignmentInstance(int_t ifcModel, char * name, char * description);

int_t	buildAlignmentCurveInstance(int_t ifcModel, int_t ifcAlignmentInstance);

int_t	buildLinearAxisWithInclinationInstance(int_t ifcModel, int_t ifcAlignmentCurveInstance, int_t ifcAxisLateralInclinationInstance);


//
//
//		Alignment2DHorizontal, Alignment2DHorizontalSegment
//
//


int_t	buildAlignment2DHorizontalInstance(int_t ifcModel, int_t ifcAlignmentCurveInstance, int_t ** pAggrSegments);

int_t	buildAlignment2DHorizontalSegmentInstance(int_t ifcModel, int_t ifcAlignmentCurveInstance, int_t * aggrSegments);

int_t	buildAlignment2DVerticalInstance(int_t ifcModel, int_t ifcAlignmentCurveInstance, int_t ** pAggrSegments);

int_t	buildAlignment2DCantInstance(int_t ifcModel, int_t ** pAggrSegments, double trackGauge);

int_t	buildLineSegment2DInstance(
				int_t	ifcModel,
				double	easting,
				double	northing,
				double	azimuth,
				double	element_length,
				double	start_radius,
				double	end_radius
			);

int_t	buildCircularArcSegment2DInstance(
				int_t	ifcModel,
				double	easting,
				double	northing,
				double	azimuth,
				double	element_length,
				double	start_radius,
				double	end_radius
			);

int_t	buildTransitionCurveSegment2DInstance(
				int_t	ifcModel,
				double	easting,
				double	northing,
				double	azimuth,
				double	element_length,
				double	start_radius,
				double	end_radius
			);

int_t	buildAlignment2DVerSegLineInstance(
				int_t	ifcModel,
				double	metraege,
				double	element_length,
				double	gradient,
				double	radius,
				double	altitude
			);

int_t	buildAlignment2DVerSegCircularArcInstance(
				int_t	ifcModel,
				double	metraege,
				double	element_length,
				double	gradient,
				double	radius,
				double	altitude
			);

void	appendAlignment2DVerticalSegmentInstance(int_t ifcAlignment2DVerticalSegmentInstance, int_t * aggrSegments);

int_t	buildAlignment2DCantSegLineInstance(
				int_t	ifcModel,
				double	metraege,
				double	element_length,
				double	start_cant_left,
				double	end_cant_left,
				double	start_cant_right,
				double	end_cant_right,
				double	speed
			);

void	appendAlignment2DCantSegmentInstance(int_t ifcAlignment2DVerticalSegmentInstance, int_t * aggrSegments);
