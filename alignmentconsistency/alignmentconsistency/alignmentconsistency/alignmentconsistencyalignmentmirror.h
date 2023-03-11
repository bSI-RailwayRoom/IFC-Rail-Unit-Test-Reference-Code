// alignmentconsistency.h : main header file for the alignmentconsistency DLL
//

#pragma once

#include "issues.h"


int_t	CreateMirror(
				int_t	mirrorModel,
				int_t	model,
				int_t	ifcAlignmentInstance
			);

void	EnrichMirror(
				int_t	mirrorModel,
				int_t	mirrorIfcAlignmentInstance,
				int_t	ifcAlignmentInstance
			);

void	CompareMirror(
				int_t	mirrorModel,
				int_t	mirrorIfcAlignmentInstance,
				int_t	model,
				int_t	ifcAlignmentInstance,
				double	relativeEpsilon
			);

void	CheckMirrorGeometry(
				int_t	mirrorModel,
				int_t	mirrorIfcAlignmentInstance,
				int_t	ifcAlignmentInstance
			);
