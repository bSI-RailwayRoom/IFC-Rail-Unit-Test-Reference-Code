
// XTR2IFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CXTR2IFCApp:
// See XTR2IFC.cpp for the implementation of this class
//

class CXTR2IFCApp : public CWinApp
{
public:
	CXTR2IFCApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual BOOL InitApplication();
	virtual int ExitInstance();
};

extern CXTR2IFCApp theApp;