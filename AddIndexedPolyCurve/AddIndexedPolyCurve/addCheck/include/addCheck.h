// addCheck.h : main header file for the addCheck DLL
//

#pragma once



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


#ifdef __cplusplus
	extern "C" {
#endif


void	DECL STDC	AddChecks(
							const wchar_t	* fileNameIN,
							const wchar_t	* fileNameOUT,
							int				segmentationParts
						);


#ifdef __cplusplus
	}
#endif


#undef DECL
#undef STDC
