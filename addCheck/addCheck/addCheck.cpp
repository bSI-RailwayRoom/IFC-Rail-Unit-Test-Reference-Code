// addCheck.cpp : Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "addCheck.h"

#include "processing.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//



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



void	DECL STDC	AddChecks(
							wchar_t	* fileNameIN,
							wchar_t	* fileNameOUT,
							int		segmentationParts
						)
{
	int_t	model = sdaiOpenModelBNUnicode(0, fileNameIN, L"");
	if (model) {
		setSegmentation(model, (int_t) segmentationParts, 0.);

		WalkInstances(model);

		sdaiSaveModelBNUnicode(model, fileNameOUT);
		sdaiCloseModel(model);
	}
}



#undef DECL
#undef STDC
