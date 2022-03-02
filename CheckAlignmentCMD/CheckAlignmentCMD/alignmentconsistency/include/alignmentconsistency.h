// alignmentconsistency.h : main header file for the alignmentconsistency DLL
//

#pragma once

#ifdef _WINDOWS
	#ifdef _USRDLL
		#define DECL __declspec(dllexport)
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


void	DECL * STDC	OpenModelC(
							char	* filename,
							bool	parseGeometry
						);

void	DECL * STDC	OpenModelW(
							wchar_t	* filename,
							bool	parseGeometry
						);

void	DECL STDC	CloseModel(
							void	* model
						);

int		DECL STDC	CheckConsistencyFile(
							void	* model
						);

int		DECL STDC	CheckConsistencyAlignment(
							void	* model,
							bool	relativeEpsilon
						);

char	DECL * STDC	GetIssueC(
							int		index,
							int		group,
							int		subgroup,
							double	absoluteEpsilon,
							double	relativeEpsilon,
							bool	* isError
						);

wchar_t	DECL * STDC	GetIssueW(
							int		index,
							int		group,
							int		subgroup,
							double	absoluteEpsilon,
							double	relativeEpsilon,
							bool	* isError
						);

char	DECL * STDC	GetGroupNameC(
							int		group,
							int		subGroup
						);

wchar_t	DECL * STDC	GetGroupNameW(
							int		group,
							int		subGroup
						);

char	DECL * STDC	GetGroupDescriptionC(
							int		group,
							int		subGroup
						);

wchar_t	DECL * STDC	GetGroupDescriptionW(
							int		group,
							int		subGroup
						);

char	DECL * STDC	GetPageC(
						);

wchar_t	DECL * STDC	GetPageW(
						);


#undef DECL
#undef STDC
