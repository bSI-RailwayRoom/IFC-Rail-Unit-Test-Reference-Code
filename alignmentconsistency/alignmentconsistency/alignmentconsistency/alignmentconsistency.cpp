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


double	absEpsilon = 0.001;

extern	int_t	myIssueCnt;
extern	char	** myIssues;
extern	int		* myIssuesGroup;
extern	int		* myIssuesSubGroup;
extern	double	* myIssuesDeviation;
extern	int		* myIssuesExpID_1;
extern	int		* myIssuesExpID_2;


bool	PARSE_GEOMETRY = true;

int_t	AC_MODEL = 0;

void	DECL * STDC	OpenModelC(
							const char		* filename,
							bool			parseGeometry
						)
{
	PARSE_GEOMETRY = parseGeometry;

	myIssueCnt = 0;
	return	(void*) sdaiOpenModelBN(0, filename, "");
}

void	DECL * STDC	OpenModelW(
							const wchar_t	* filename,
							bool			parseGeometry
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
							double	relativeEpsilon
						)
{
	AC_MODEL = (int_t) model;

	CheckConsistencyAlignment__internal((int_t) model, relativeEpsilon);

	return	(int) myIssueCnt;
}

const char	DECL * STDC	GetIssueC(
								int			index,
								int			group,
								int			subGroup,
								double		absoluteEpsilon,
								double		relativeEpsilon,
								bool		* isError,
								int			* expressID_1,
								int			* expressID_2
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
		if (expressID_1)
			(*expressID_1) = myIssuesExpID_1[index];
		if (expressID_2)
			(*expressID_2) = myIssuesExpID_2[index];
		return	myIssues[index];
	}
	return	nullptr;
}

const wchar_t	DECL * STDC	GetIssueW(
								int		index,
								int		group,
								int		subGroup,
								double	absoluteEpsilon,
								double	relativeEpsilon,
								bool	* isError
							)
{
	const char	* rValue = GetIssueC(index, group, subGroup, absoluteEpsilon, relativeEpsilon, isError);
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

const char	DECL * STDC	GetGroupNameC(
								int		group,
								int		subGroup
							)
{
	return	GetGroupText(group, subGroup, /*isName*/ true, PARSE_GEOMETRY);
}

const wchar_t	DECL * STDC	GetGroupNameW(
									int		group,
									int		subGroup
								)
{
	const char	* rValue = GetGroupNameC(group, subGroup);
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

const char	DECL * STDC	GetGroupDescriptionC(
								int		group,
								int		subGroup
							)
{
	return	GetGroupText(group, subGroup, /*isName*/ false, PARSE_GEOMETRY);
}

const wchar_t	DECL * STDC	GetGroupDescriptionW(
									int		group,
									int		subGroup
								)
{
	const char	* rValue = GetGroupDescriptionC(group, subGroup);
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

const char	DECL * STDC	GetPageC(
								double	absoluteEpsilon,
								double	relativeEpsilon
							)
{
	absEpsilon = absoluteEpsilon;

	return	GetPage(AC_MODEL, absoluteEpsilon, relativeEpsilon);
}

const wchar_t	DECL * STDC	GetPageW(
									double	absoluteEpsilon,
									double	relativeEpsilon
								)
{
	absEpsilon = absoluteEpsilon;

	const char	* rValue = GetPageC(absoluteEpsilon, relativeEpsilon);
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

const char	DECL * STDC	GetPageJSON(
								const char	* fileName,
								double		absoluteEpsilon,
								double		relativeEpsilon
							)
{
	absEpsilon = absoluteEpsilon;

	return	GetPageJSON(AC_MODEL, fileName, absoluteEpsilon, relativeEpsilon);
}


#undef DECL
#undef STDC


