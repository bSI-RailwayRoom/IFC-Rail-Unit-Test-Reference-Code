
// XTR2IFC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "XTR2IFC.h"
#include "XTR2IFCDlg.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


wchar_t	* xtrFileName = nullptr, * ifcFileName = nullptr, * ifcSchemaName = nullptr;


// CXTR2IFCApp

BEGIN_MESSAGE_MAP(CXTR2IFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CXTR2IFCApp construction

CXTR2IFCApp::CXTR2IFCApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CXTR2IFCApp object

CXTR2IFCApp theApp;


// CXTR2IFCApp initialization

BOOL CXTR2IFCApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CXTR2IFCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CXTR2IFCApp::InitApplication()
{
	size_t	i = wcslen(this->m_pszHelpFilePath) - wcslen(this->m_pszExeName) - 4;

	assert(xtrFileName == nullptr && ifcFileName == nullptr && ifcSchemaName == nullptr && i > 0);

	xtrFileName = new wchar_t[i + wcslen(L"a_736_NET_GL_Bestand_1Kette.XTR") + 1];
	memcpy(&xtrFileName[0], this->m_pszHelpFilePath, i * sizeof(wchar_t));
	memcpy(&xtrFileName[i], L"a_736_NET_GL_Bestand_1Kette.XTR", (wcslen(L"a_736_NET_GL_Bestand_1Kette.XTR") + 1) * sizeof(wchar_t));

	ifcFileName = new wchar_t[i + wcslen(L"generated.ifc") + 1];
	memcpy(&ifcFileName[0], this->m_pszHelpFilePath, i * sizeof(wchar_t));
	memcpy(&ifcFileName[i], L"generated.ifc", (wcslen(L"generated.ifc") + 1) * sizeof(wchar_t));

	ifcSchemaName = new wchar_t[i + wcslen(L"IFC4x3_RC1.exp") + 1];
	memcpy(&ifcSchemaName[0], this->m_pszHelpFilePath, i * sizeof(wchar_t));
	memcpy(&ifcSchemaName[i], L"IFC4x3_RC1.exp", (wcslen(L"IFC4x3_RC1.exp") + 1) * sizeof(wchar_t));

	return CWinApp::InitApplication();
}

int CXTR2IFCApp::ExitInstance()
{
	if (xtrFileName) {
		delete[] xtrFileName;
		xtrFileName = nullptr;
	}

	if (ifcFileName) {
		delete[] ifcFileName;
		ifcFileName = nullptr;
	}

	if (ifcSchemaName) {
		delete[] ifcSchemaName;
		ifcSchemaName = nullptr;
	}

	return CWinApp::ExitInstance();
}
