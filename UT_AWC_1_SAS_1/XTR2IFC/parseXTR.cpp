// Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "parseXTR.h"

#include <assert.h>


enum enum_geometry_type
{
	__STRAIGHT_LINE__,
	__ARC__,
	__CLOTHOID__		//	Transition Curve
};

//
//	Horizontal Alignment
//
size_t				linesHorizontalAlignmentCnt = 0;
enum_geometry_type	* HA_geometry_type = nullptr;
double				* HA_easting = nullptr,
					* HA_northing = nullptr,
					* HA_azimuth = nullptr,
					* HA_element_length = nullptr,
					* HA_start_radius = nullptr,
					* HA_end_radius = nullptr;

//
//	Vertical Alignment
//
size_t				linesVerticalAlignmentCnt = 0;
enum_geometry_type	* VA_geometry_type = nullptr;
double				* VA_metraege = nullptr,
					* VA_element_length = nullptr,
					* VA_gradient = nullptr,
					* VA_radius = nullptr,
					* VA_altitude = nullptr;

//
//	Cant Alignment
//
size_t				linesCantAlignmentCnt = 0;
double				* C_metraege = nullptr,
					* C_element_length = nullptr,
					* C_start_cant = nullptr,
					* C_end_cant = nullptr,
					* C_speed = nullptr;


char	* GetElement(
				char	** pLine
			)
{
	size_t	i = 0;
	while ((*pLine)[i] == '\t' || (*pLine)[i] == ' ') { i++; }

	char	* element = &(*pLine)[i];
	while ((*pLine)[i] && (*pLine)[i] != '\t' && (*pLine)[i] != ' ') {
		i++;
	}

	if ((*pLine)[i]) {
		(*pLine)[i] = 0;
		(*pLine) = &(*pLine)[i + 1];
	}
	else {
		(*pLine) = &(*pLine)[i];
	}
	
	return	element;
}

void	ParseHorizontalAlignment(
				char				* line,
				enum_geometry_type	* geometry_type,
				double				* easting,
				double				* northing,
				double				* azimuth,
				double				* element_length,
				double				* start_radius,
				double				* end_radius
			)
{
	char	* element = GetElement(&line);

	if (element[0] == 'D') {
		(*geometry_type) = __STRAIGHT_LINE__;
	}
	else if (element[0] == 'C') {
		(*geometry_type) = __ARC__;
	}
	else {
		assert(element[0] == 'R');
		(*geometry_type) = __CLOTHOID__;		//	Transition Curve	
	}
	assert(element[1] == 0);

	element = GetElement(&line);
	(*easting) = atof(element);

	element = GetElement(&line);
	(*northing) = atof(element);

	element = GetElement(&line);
	(*azimuth) = atof(element);

	element = GetElement(&line);
	(*element_length) = atof(element);

	element = GetElement(&line);
	(*start_radius) = atof(element);

	element = GetElement(&line);
	(*end_radius) = atof(element);

	assert(line[0] == 0);
}

void	ParseVerticalAlignment(
				char				* line,
				enum_geometry_type	* geometry_type,
				double				* metraege,
				double				* element_length,
				double				* gradient,
				double				* radius,
				double				* altitude
			)
{
	char	* element = GetElement(&line);

	if (element[0] == 'c') {
		(*geometry_type) = __ARC__;
	}
	else {
		assert(element[0] == 'd');
		(*geometry_type) = __STRAIGHT_LINE__;
	}
	assert(element[1] == 0);

	element = GetElement(&line);
	(*metraege) = atof(element);

	element = GetElement(&line);
	(*element_length) = atof(element);

	element = GetElement(&line);
	(*gradient) = atof(element);

	element = GetElement(&line);
	(*radius) = atof(element);

	element = GetElement(&line);
	(*altitude) = atof(element);

	assert(line[0] == 0);
}

void	ParseCantAlignment(
				char	* line,
				double	* metraege,
				double	* element_length,
				double	* start_cant,
				double	* end_cant,
				double	* speed
			)
{
	char	* element = GetElement(&line);
	assert(element[0] == 'Q');
	assert(element[1] == 0);

	element = GetElement(&line);
	(*metraege) = atof(element);

	element = GetElement(&line);
	(*element_length) = atof(element);

	element = GetElement(&line);
	(*start_cant) = atof(element);

	element = GetElement(&line);
	(*end_cant) = atof(element);

	element = GetElement(&line);
	(*speed) = atof(element);

	assert(line[0] == 0);
}

size_t	GetLines(
				char	* buffer,
				char	*** lines
			)
{
	size_t	len = strlen(buffer),
			lineCnt = 1;
	for (size_t i = 0; i < len; i++) {
		if (buffer[i] == 10 || buffer[i] == 13) {
			lineCnt++;
		}
	}

	(*lines) = new char*[lineCnt];
	(*lines)[0] = buffer;

	lineCnt = 1;
	for (size_t i = 0; i < len; i++) {
		if (buffer[i] == 10 || buffer[i] == 13) {
			buffer[i] = 0;
			if (i && buffer[i - 1] == 0) {
				lineCnt--;
			}
			(*lines)[lineCnt++] = &buffer[i + 1];
		}
	}

	return	lineCnt;
}

size_t	GetLinesHA(
				char	** lines,
				size_t	lineCnt,
				char	*** linesHorizontalAlignments
			)
{
	(*linesHorizontalAlignments) = new char*[lineCnt];

	size_t	i = 0;
	while ((i < lineCnt) &&
		   ((lines[i])[0] != 'D') &&
		   ((lines[i])[0] != 'C') &&
		   ((lines[i])[0] != 'R')) {
		i++;
	}

	size_t	linesHorizonAlignmentCnt = 0;
	while ((i < lineCnt) &&
		   (((lines[i])[0] == 'D') ||
		    ((lines[i])[0] == 'C') ||
		    ((lines[i])[0] == 'R'))) {
		(*linesHorizontalAlignments)[linesHorizonAlignmentCnt++] = lines[i];
		i++;
	}

	return	linesHorizonAlignmentCnt;
}

size_t	GetLinesVA(
				char	** lines,
				size_t	lineCnt,
				char	*** linesVerticalAlignments
			)
{
	(*linesVerticalAlignments) = new char*[lineCnt];

	size_t	i = 0;
	while ((i < lineCnt) &&
		   ((lines[i])[0] != 'd') &&
		   ((lines[i])[0] != 'c')) {
		i++;
	}

	size_t	linesVerticalAlignmentCnt = 0;
	while ((i < lineCnt) &&
		   (((lines[i])[0] == 'd') ||
		    ((lines[i])[0] == 'c'))) {
		(*linesVerticalAlignments)[linesVerticalAlignmentCnt++] = lines[i];
		i++;
	}

	return	linesVerticalAlignmentCnt;
}

size_t	GetLinesCant(
				char	** lines,
				size_t	lineCnt,
				char	*** linesCants
			)
{
	(*linesCants) = new char*[lineCnt];

	size_t	i = 0;
	while ((i < lineCnt) &&
		   ((lines[i])[0] != 'Q')) {
		i++;
	}

	size_t	linesCantCnt = 0;
	while ((i < lineCnt) &&
		   ((lines[i])[0] == 'Q')) {
		(*linesCants)[linesCantCnt++] = lines[i];
		i++;
	}

	return	linesCantCnt;
}

void	parseXTR(wchar_t * XTR)
{
	FILE	* fileStream = 0;
	_wfopen_s(&fileStream, XTR, L"r");
	if (fileStream) {
		char	buffer[16 * 512];
		size_t	numread = fread(buffer, sizeof(char), 16 * 512, fileStream);

		fclose(fileStream);

		char	** lines = nullptr;
		size_t	lineCnt = GetLines(buffer, &lines);

		char	** linesHorizontalAlignments = nullptr;
		linesHorizontalAlignmentCnt = GetLinesHA(lines, lineCnt, &linesHorizontalAlignments);

		char	** linesVerticalAlignments = nullptr;
		linesVerticalAlignmentCnt = GetLinesVA(lines, lineCnt, &linesVerticalAlignments);

		char	** linesCantAlignments = nullptr;
		linesCantAlignmentCnt = GetLinesCant(lines, lineCnt, &linesCantAlignments);

		//
		//	Horizontal Alignment
		//
		HA_geometry_type  = new enum_geometry_type[linesHorizontalAlignmentCnt];
		HA_easting		  = new double[linesHorizontalAlignmentCnt],
		HA_northing		  = new double[linesHorizontalAlignmentCnt],
		HA_azimuth		  = new double[linesHorizontalAlignmentCnt],
		HA_element_length = new double[linesHorizontalAlignmentCnt],
		HA_start_radius	  = new double[linesHorizontalAlignmentCnt],
		HA_end_radius	  = new double[linesHorizontalAlignmentCnt];

		for (size_t i = 0; i < linesHorizontalAlignmentCnt; i++) {
			assert(linesHorizontalAlignments[i]);
			ParseHorizontalAlignment(
					linesHorizontalAlignments[i],
					&HA_geometry_type[i],
					&HA_easting[i],
					&HA_northing[i],
					&HA_azimuth[i],
					&HA_element_length[i],
					&HA_start_radius[i],
					&HA_end_radius[i]
				);
		}

		//
		//	Vertical Alignment
		//
		VA_geometry_type  = new enum_geometry_type[linesVerticalAlignmentCnt];
		VA_metraege		  = new double[linesVerticalAlignmentCnt],
		VA_element_length = new double[linesVerticalAlignmentCnt],
		VA_gradient		  = new double[linesVerticalAlignmentCnt],
		VA_radius		  = new double[linesVerticalAlignmentCnt],
		VA_altitude		  = new double[linesVerticalAlignmentCnt];

		for (size_t i = 0; i < linesVerticalAlignmentCnt; i++) {
			assert(linesVerticalAlignments[i]);
			ParseVerticalAlignment(
					linesVerticalAlignments[i],
					&VA_geometry_type[i],
					&VA_metraege[i],
					&VA_element_length[i],
					&VA_gradient[i],
					&VA_radius[i],
					&VA_altitude[i]
				);
		}

		//
		//	Cant Alignment
		//
		C_metraege		  = new double[linesCantAlignmentCnt],
		C_element_length  = new double[linesCantAlignmentCnt],
		C_start_cant	  = new double[linesCantAlignmentCnt],
		C_end_cant		  = new double[linesCantAlignmentCnt],
		C_speed			  = new double[linesCantAlignmentCnt];

		for (size_t i = 0; i < linesCantAlignmentCnt; i++) {
			assert(linesCantAlignments[i]);
			ParseCantAlignment(
					linesCantAlignments[i],
					&C_metraege[i],
					&C_element_length[i],
					&C_start_cant[i],
					&C_end_cant[i],
					&C_speed[i]
				);
		}
	}
}

void	buildHorizontalAlignment(int_t ifcModel, int_t ifcAlignmentCurveInstance)
{
	int_t * aggrSegments = nullptr;
	buildAlignment2DHorizontalInstance(ifcModel, ifcAlignmentCurveInstance, &aggrSegments);
	assert(aggrSegments);

	for (size_t i = 0; i < linesHorizontalAlignmentCnt; i++) {
		double	HA_azimuth_adjusted = 9 * HA_azimuth[i] / 10;
		HA_azimuth_adjusted = -(HA_azimuth_adjusted - 90);

		switch (HA_geometry_type[i]) {
			case  __STRAIGHT_LINE__:
				{
					int_t	ifcCurveSegment2DInstance =
								buildLineSegment2DInstance(
										ifcModel,
										HA_easting[i],
										HA_northing[i],
										HA_azimuth_adjusted,
										HA_element_length[i],
										HA_start_radius[i],
										HA_end_radius[i]
									);
					buildAlignment2DHorizontalSegmentInstance(ifcModel, ifcCurveSegment2DInstance, aggrSegments);
				}
				break;
			case  __ARC__:
				{
					int_t	ifcCurveSegment2DInstance =
								buildCircularArcSegment2DInstance(
										ifcModel,
										HA_easting[i],
										HA_northing[i],
										HA_azimuth_adjusted,
										HA_element_length[i],
										HA_start_radius[i],
										HA_end_radius[i]
									);
					buildAlignment2DHorizontalSegmentInstance(ifcModel, ifcCurveSegment2DInstance, aggrSegments);
				}
				break;
			case  __CLOTHOID__:
				{
					int_t	ifcCurveSegment2DInstance =
								buildTransitionCurveSegment2DInstance(
										ifcModel,
										HA_easting[i],
										HA_northing[i],
										HA_azimuth_adjusted,
										HA_element_length[i],
										HA_start_radius[i],
										HA_end_radius[i]
									);
					buildAlignment2DHorizontalSegmentInstance(ifcModel, ifcCurveSegment2DInstance, aggrSegments);
				}
				break;
			default:
				assert(false);
				break;
		}
	}
}

void	buildVerticalAlignment(int_t ifcModel, int_t ifcAlignmentCurveInstance)
{
	int_t * aggrSegments = nullptr;
	buildAlignment2DVerticalInstance(ifcModel, ifcAlignmentCurveInstance, &aggrSegments);
	assert(aggrSegments);

	for (size_t i = 0; i < linesVerticalAlignmentCnt; i++) {
		double	VA_gradient_adjusted = VA_gradient[i] / 1000;

		switch (VA_geometry_type[i]) {
			case  __STRAIGHT_LINE__:
				{
					int_t	ifcAlignment2DVerticalSegmentInstance =
								buildAlignment2DVerSegLineInstance(
										ifcModel,
										VA_metraege[i],
										VA_element_length[i],
										VA_gradient_adjusted,
										VA_radius[i],
										VA_altitude[i]
									);
					appendAlignment2DVerticalSegmentInstance(ifcAlignment2DVerticalSegmentInstance, aggrSegments);
				}
				break;
			case  __ARC__:
				{
					int_t	ifcAlignment2DVerticalSegmentInstance =
								buildAlignment2DVerSegCircularArcInstance(
										ifcModel,
										VA_metraege[i],
										VA_element_length[i],
										VA_gradient_adjusted,
										VA_radius[i],
										VA_altitude[i]
									);
					appendAlignment2DVerticalSegmentInstance(ifcAlignment2DVerticalSegmentInstance, aggrSegments);
				}
				break;
			case  __CLOTHOID__:
				assert(false);
				break;
			default:
				assert(false);
				break;
		}
	}
}

int_t	buildAlignment2DCant(int_t ifcModel, double factorCant)
{
	int_t	* aggrSegments = nullptr,
			ifcCantAlignmentInstance = buildAlignment2DCantInstance(ifcModel, &aggrSegments, 1.5);
	assert(aggrSegments);

	for (size_t i = 0; i < linesCantAlignmentCnt; i++) {
		double	C_start_cant_adjusted = factorCant * C_start_cant[i] / 1000,
				C_end_cant_adjusted = factorCant * C_end_cant[i] / 1000;

		int_t	ifcAlignment2DCantSegmentInstance =
					buildAlignment2DCantSegLineInstance(
							ifcModel,
							C_metraege[i],
							C_element_length[i],
							C_start_cant_adjusted / 2,
							C_end_cant_adjusted / 2,
							-(C_start_cant_adjusted / 2),
							-(C_end_cant_adjusted / 2),
							C_speed[i]
						);
		appendAlignment2DCantSegmentInstance(ifcAlignment2DCantSegmentInstance, aggrSegments);
	}

	return	ifcCantAlignmentInstance;
}

int_t	buildAlignment__H(int_t ifcModel, int_t * ifcCurveInstance)
{
	int_t	ifcAlignmentInstance = buildAlignmentInstance(ifcModel, "a", "b"),
			ifcAlignmentCurveInstance = buildAlignmentCurveInstance(ifcModel, ifcAlignmentInstance);

	buildHorizontalAlignment(ifcModel, ifcAlignmentCurveInstance);

	(*ifcCurveInstance) = ifcAlignmentCurveInstance;

	return	ifcAlignmentInstance;
}

int_t	buildAlignment__HV(int_t ifcModel, int_t * ifcCurveInstance)
{
	int_t	ifcAlignmentInstance = buildAlignmentInstance(ifcModel, "a", "b"),
			ifcAlignmentCurveInstance = buildAlignmentCurveInstance(ifcModel, ifcAlignmentInstance);

	buildHorizontalAlignment(ifcModel, ifcAlignmentCurveInstance);
	buildVerticalAlignment(ifcModel, ifcAlignmentCurveInstance);

	(*ifcCurveInstance) = ifcAlignmentCurveInstance;

	return	ifcAlignmentInstance;
}

int_t	buildAlignment__HVC(int_t ifcModel, int_t * ifcCurveInstance, int_t * ifcAxisLateralInclinationInstance, double factorCant)
{
	int_t	ifcAlignment2DCantInstance = buildAlignment2DCant(ifcModel, factorCant);

	int_t	ifcAlignmentInstance = buildAlignmentInstance(ifcModel, "a", "b"),
			ifcAlignmentCurveInstance = buildAlignmentCurveInstance(ifcModel, ifcAlignmentInstance),
			ifcAlignmentCurveWithInclinationInstance = buildLinearAxisWithInclinationInstance(ifcModel, ifcAlignmentCurveInstance, ifcAlignment2DCantInstance);

	buildHorizontalAlignment(ifcModel, ifcAlignmentCurveInstance);
	buildVerticalAlignment(ifcModel, ifcAlignmentCurveInstance);

	(*ifcCurveInstance) = ifcAlignmentCurveInstance;
	(*ifcAxisLateralInclinationInstance) = ifcAlignment2DCantInstance;

	return	ifcAlignmentInstance;
}

/*POLYGON2D	* localCreatePolygonStructureForSquare(double min_x, double min_y, double max_x, double max_y)
{
	POLYGON2D	* polygon = new POLYGON2D;
	polygon->point = new POINT2D;
	polygon->point->x = min_x;
	polygon->point->y = min_y;
	polygon->next = new POLYGON2D;
	polygon->next->point = new POINT2D;
	polygon->next->point->x = min_x;
	polygon->next->point->y = max_y;
	polygon->next->next = new POLYGON2D;
	polygon->next->next->point = new POINT2D;
	polygon->next->next->point->x = max_x;
	polygon->next->next->point->y = max_y;
	polygon->next->next->next = new POLYGON2D;
	polygon->next->next->next->point = new POINT2D;
	polygon->next->next->next->point->x = max_x;
	polygon->next->next->next->point->y = min_y;
	polygon->next->next->next->next = 0;

	return	polygon;
}	//	*/

int_t	buildProxy(int_t ifcModel, int_t ifcCurveInstance, int_t ifcAxisLateralInclinationInstance, double factor)
{
	int_t	ifcProxyInstance = buildBuildingElementProxyInstance(ifcModel, "a", "b");

	double	profileWidth = 1.5,
			profileThickness = 0.2;

//	POLYGON2D	* polygon = localCreatePolygonStructureForSquare(-profileWidth / 2, 0, profileWidth / 2, profileThickness);
	createIfcExtrudedPolygonShape(0, ifcCurveInstance, ifcAxisLateralInclinationInstance, factor);

	return	ifcProxyInstance;
}
