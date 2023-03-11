// alignmentconsistency.h : main header file for the alignmentconsistency DLL
//

#pragma once

#include "../include/ifcengine.h"


void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText
			);

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstance
			);

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				char	* issueTextADD
			);

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstanceI,
				int_t	ifcInstanceII
			);

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstance,
				char	* itemText
			);

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstance,
				char	* itemText,
				double	givenValue,
				double	expectedValue
			);

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstanceI,
				int_t	ifcInstanceII,
				char	* itemText
			);

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstanceI,
				int_t	ifcInstanceII,
				double	value
			);
