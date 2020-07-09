#pragma once

#include "resource.h"
#include "ifccreation\alignmentIfc.h"
#include "ifccreation\extrudedPolygonIfc.h"


void	parseXTR(wchar_t * XTR);

int_t	buildAlignment__H(int_t ifcModel, int_t * ifcCurveInstance);

int_t	buildAlignment__HV(int_t ifcModel, int_t * ifcCurveInstance);

int_t	buildAlignment__HVC(int_t ifcModel, int_t * ifcCurveInstance, int_t * ifcAxisLateralInclinationInstance, double factorCant);

int_t	buildProxy(int_t ifcModel, int_t ifcCurveInstance, int_t ifcAxisLateralInclinationInstance, double factor);
