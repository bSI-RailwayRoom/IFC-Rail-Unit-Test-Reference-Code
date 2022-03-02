// alignmentconsistency.cpp : Defines the initialization routines for the DLL.
//

#include <stdio.h>
#include <string.h>

#include "alignmentconsistency.h"
#include "alignmentconsistencyalignment.h"
#include "alignmentconsistencyfile.h"
#include "alignmentconsistencyPage.h"
#include "alignmentconsistencyTEXT.h"


#ifdef _WINDOWS
	#ifdef _USRDLL
		#define DECL //__declspec(dllexport)
		//	The use of the Microsoft-specific
		//	__declspec(dllexport)
		//	is needed only when
		//	not using a .def file.
	#else
		#define DECL __declspec(dllimport) 
	#endif
	#define STDC __stdcall
#else
	#define DECL /*nothing*/
	#define STDC /*nothing*/
#endif


extern	int_t	myIssueCnt;
extern	char	** myIssues;
extern	int		* myIssuesGroup;
extern	int		* myIssuesSubGroup;
extern	double	* myIssuesDeviation;

bool	PARSE_GEOMETRY = true;

int_t		AC_MODEL = 0;

void	DECL * STDC	OpenModelC(
							char	* filename,
							bool	parseGeometry
						)
{
	PARSE_GEOMETRY = parseGeometry;

	myIssueCnt = 0;
	return	(void*) sdaiOpenModelBN(0, (const char*) filename, (const char*) "");
}

void	DECL * STDC	OpenModelW(
							wchar_t	* filename,
							bool	parseGeometry
						)
{
	PARSE_GEOMETRY = parseGeometry;

	myIssueCnt = 0;
	return	(void*) sdaiOpenModelBNUnicode(0, (const wchar_t*) filename, (const wchar_t*) L"");
}

void	DECL STDC	CloseModel(
							void	* model
						)
{
	return	sdaiCloseModel((int_t) model);
}

int		DECL STDC	CheckConsistencyFile(
							void	* model
						)
{
	AC_MODEL = (int_t) model;

	CheckConsistencyFile__internal((int_t) model);

	return	(int) myIssueCnt;
}

int		DECL STDC	CheckConsistencyAlignment(
							void	* model,
							bool	relativeEpsilon
						)
{
	AC_MODEL = (int_t) model;

	CheckConsistencyAlignment__internal((int_t) model, relativeEpsilon);

	return	(int) myIssueCnt;
}

char	DECL * STDC	GetIssueC(
							int		index,
							int		group,
							int		subGroup,
							double	absoluteEpsilon,
							double	relativeEpsilon,
							bool	* isError
						)
{
	if (myIssuesGroup[index] == group &&
		myIssuesSubGroup[index] == subGroup) {
		if (absoluteEpsilon < myIssuesDeviation[index]) {
			(*isError) = true;
		}
		else {
			(*isError) = false;
		}
		return	myIssues[index];
	}
	return	nullptr;
}

wchar_t	DECL * STDC	GetIssueW(
							int		index,
							int		group,
							int		subGroup,
							double	absoluteEpsilon,
							double	relativeEpsilon,
							bool	* isError
						)
{
	char	* rValue = GetIssueC(index, group, subGroup, absoluteEpsilon, relativeEpsilon, isError);
	if (rValue) {
		size_t	len = strlen(rValue);
		wchar_t	* rValueW = new wchar_t[len + 1];
		for (size_t i = 0; i < len; i++) {
			rValueW[i] = (wchar_t) rValue[i];
		}
		rValueW[len] = 0;
		return	rValueW;
	}
	return	nullptr;
}

char	DECL * STDC	GetGroupNameC(
							int		group,
							int		subGroup
						)
{
	return	GetGroupText(group, subGroup, /*isName*/ true, PARSE_GEOMETRY);
}

wchar_t	DECL * STDC	GetGroupNameW(
							int		group,
							int		subGroup
						)
{
	char	* rValue = GetGroupNameC(group, subGroup);
	if (rValue) {
		size_t	len = strlen(rValue);
		wchar_t	* rValueW = new wchar_t[len + 1];
		for (size_t i = 0; i < len; i++) {
			rValueW[i] = (wchar_t)rValue[i];
		}
		rValueW[len] = 0;
		return	rValueW;
	}
	return	nullptr;
}

char	DECL * STDC	GetGroupDescriptionC(
							int		group,
							int		subGroup
						)
{
	return	GetGroupText(group, subGroup, /*isName*/ false, PARSE_GEOMETRY);
}

wchar_t	DECL * STDC	GetGroupDescriptionW(
							int		group,
							int		subGroup
						)
{
	char	* rValue = GetGroupDescriptionC(group, subGroup);
	if (rValue) {
		size_t	len = strlen(rValue);
		wchar_t	* rValueW = new wchar_t[len + 1];
		for (size_t i = 0; i < len; i++) {
			rValueW[i] = (wchar_t)rValue[i];
		}
		rValueW[len] = 0;
		return	rValueW;
	}
	return	nullptr;
}

char	DECL * STDC	GetPageC(
						)
{
	return	GetPage(AC_MODEL);
}

wchar_t	DECL * STDC	GetPageW(
						)
{
	char	* rValue = GetPageC();
	if (rValue) {
		size_t	len = strlen(rValue);
		wchar_t	* rValueW = new wchar_t[len + 1];
		for (size_t i = 0; i < len; i++) {
			rValueW[i] = (wchar_t)rValue[i];
		}
		rValueW[len] = 0;
		return	rValueW;
	}
	return	nullptr;
}


#undef DECL
#undef STDC
