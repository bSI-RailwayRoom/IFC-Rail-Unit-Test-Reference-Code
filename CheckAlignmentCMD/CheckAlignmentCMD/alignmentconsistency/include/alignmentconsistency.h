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


void			DECL * STDC	OpenModelC(
									const char		* filename,
									bool			parseGeometry
								);

void			DECL * STDC	OpenModelW(
									const wchar_t	* filename,
									bool			parseGeometry
								);

void			DECL STDC	CloseModel(
									void			* model
								);

int				DECL STDC	CheckConsistencyFile(
									void			* model
								);

int				DECL STDC	CheckConsistencyAlignment(
									void			* model,
									double			relativeEpsilon
								);

const char		DECL * STDC	GetIssueC(
									int				index,
									int				group,
									int				subgroup,
									double			absoluteEpsilon,
									double			relativeEpsilon,
									bool			* isError,
									int				* expressID_1		= nullptr,
									int				* expressID_2		= nullptr
								);

const wchar_t	DECL * STDC	GetIssueW(
									int				index,
									int				group,
									int				subgroup,
									double			absoluteEpsilon,
									double			relativeEpsilon,
									bool			* isError
								);

const char		DECL * STDC	GetGroupNameC(
									int				group,
									int				subGroup
								);

const wchar_t	DECL * STDC	GetGroupNameW(
									int				group,
									int				subGroup
								);

const char		DECL * STDC	GetGroupDescriptionC(
									int				group,
									int				subGroup
								);

const wchar_t	DECL * STDC	GetGroupDescriptionW(
									int				group,
									int				subGroup
								);

const char		DECL * STDC	GetPageC(
									double			absoluteEpsilon,
									double			relativeEpsilon
								);

const wchar_t	DECL * STDC	GetPageW(
									double			absoluteEpsilon,
									double			relativeEpsilon
								);

const char		DECL * STDC	GetPageJSON(
									const char		* fileName,
									double			absoluteEpsilon,
									double			relativeEpsilon
								);



#undef DECL
#undef STDC
