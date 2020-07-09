
// XTR2IFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XTR2IFC.h"
#include "XTR2IFCDlg.h"
#include "afxdialogex.h"

#include "parseXTR.h"
#include "ifccreation/baseIfc.h"

#include "ifcengine/include/engine.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern	wchar_t	* xtrFileName, * ifcFileName, * ifcSchemaName;


// CXTR2IFCDlg dialog


CXTR2IFCDlg::CXTR2IFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CXTR2IFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXTR2IFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, p_XTRfile);
	DDX_Control(pDX, IDC_EDIT2, p_IFCfile);
	DDX_Control(pDX, IDC_CHECK1, p_HA);
	DDX_Control(pDX, IDC_CHECK2, p_VA);
	DDX_Control(pDX, IDC_CHECK3, p_CA);
	DDX_Control(pDX, IDC_EDIT3, p_SchemaFile);
	DDX_Control(pDX, IDC_CHECK4, p_SimpleSolid);
	DDX_Control(pDX, IDC_EDIT4, pFactor);
	DDX_Control(pDX, IDC_EDIT5, pFactorCant);
}

BEGIN_MESSAGE_MAP(CXTR2IFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CXTR2IFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CXTR2IFCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CXTR2IFCDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CXTR2IFCDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CXTR2IFCDlg message handlers

BOOL CXTR2IFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	p_XTRfile.SetWindowTextW(xtrFileName);
	p_IFCfile.SetWindowTextW(ifcFileName);
	p_SchemaFile.SetWindowTextW(ifcSchemaName);

	p_HA.SetCheck(0);
	p_VA.SetCheck(0);
	p_CA.SetCheck(1);

	p_SimpleSolid.SetCheck(1);

	pFactor.SetWindowTextW(L"500");
	pFactorCant.SetWindowTextW(L"3");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXTR2IFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXTR2IFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CXTR2IFCDlg::OnBnClickedButton1()
{
	wchar_t	buff[512];
	p_XTRfile.GetWindowTextW(buff, 511);

	CFileDialog dlgFile(true);
	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.lpstrFilter = _T("XTR Files (*.xtr)\0*.XTR\0All Files (*.*)\0*.*\0\0");
	ofn.lpstrTitle = _T("Please, select an XTR file");
	ofn.lpstrFile = buff;
	ofn.nMaxFile = 512;

	if (dlgFile.DoModal() == IDOK) {
		CString	name = dlgFile.GetPathName();
		p_XTRfile.SetWindowTextW(name);
	}
}

void CXTR2IFCDlg::OnBnClickedButton3()
{
	wchar_t	buff[512];
	p_IFCfile.GetWindowTextW(buff, 511);

	CFileDialog dlgFile(true);
	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.lpstrFilter = _T("IFC Files (*.ifc)\0*.IFC\0All Files (*.*)\0*.*\0\0");
	ofn.lpstrTitle = _T("Please, select an IFC file");
	ofn.lpstrFile = buff;
	ofn.nMaxFile = 512;

	if (dlgFile.DoModal() == IDOK) {
		CString	name = dlgFile.GetPathName();
		p_IFCfile.SetWindowTextW(name);
	}
}


void CXTR2IFCDlg::OnBnClickedButton4()
{
	wchar_t	buff[512];
	p_SchemaFile.GetWindowTextW(buff, 511);

	CFileDialog dlgFile(true);
	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.lpstrFilter = _T("EXP Files (*.exp)\0*.EXP\0All Files (*.*)\0*.*\0\0");
	ofn.lpstrTitle = _T("Please, select an schema (EXP) file");
	ofn.lpstrFile = buff;
	ofn.nMaxFile = 512;

	if (dlgFile.DoModal() == IDOK) {
		CString	name = dlgFile.GetPathName();
		p_SchemaFile.SetWindowTextW(name);
	}
}


void CXTR2IFCDlg::OnBnClickedButton2()
{
	wchar_t	ifcSchemaName[512];
	p_SchemaFile.GetWindowTextW(ifcSchemaName, 511);

	int_t	ifcModel = createEmptyIfcFile(ifcSchemaName, nullptr);

	if (ifcModel) {
		wchar_t	factorStr[512];
		pFactor.GetWindowTextW(factorStr, 511);

		int_t	factor = _wtoi(factorStr);

		wchar_t	factorCantStr[512];
		pFactorCant.GetWindowTextW(factorCantStr, 511);

		int_t	factorCant = _wtoi(factorCantStr);

		wchar_t	xtrFileaName[512];
		p_XTRfile.GetWindowTextW(xtrFileaName, 511);

		parseXTR(xtrFileaName);

		if (p_HA.GetCheck()) {
			int_t	ifcCurveInstance = 0,
					ifcAlignmentInstance = buildAlignment__H(ifcModel, &ifcCurveInstance);

			if (p_SimpleSolid.GetCheck()) {
				buildProxy(ifcModel, ifcCurveInstance, 0, (double) factor);
			}
		}

		if (p_VA.GetCheck()) {
			int_t	ifcCurveInstance = 0,
					ifcAlignmentInstance = buildAlignment__HV(ifcModel, &ifcCurveInstance);

			if (p_SimpleSolid.GetCheck()) {
				buildProxy(ifcModel, ifcCurveInstance, 0, (double) factor);
			}
		}

		if (p_CA.GetCheck()) {
			int_t	ifcCurveInstance = 0,
					ifcAxisLateralInclinationInstance = 0,
					ifcAlignmentInstance = buildAlignment__HVC(ifcModel, &ifcCurveInstance, &ifcAxisLateralInclinationInstance, (double) factorCant);

			if (p_SimpleSolid.GetCheck()) {
				buildProxy(ifcModel, ifcCurveInstance, ifcAxisLateralInclinationInstance, (double) factor);
			}
		}

		wchar_t	ifcFileName[512];
		p_IFCfile.GetWindowTextW(ifcFileName, 511);

		char		timeStamp[512];
		time_t		t;
		struct tm	tInfo;

		time(&t);
		localtime_s(&tInfo, &t);

		int_t	i = 0, j = 0;
		while (ifcFileName[i]) {
			if (ifcFileName[i++] == '\\') {
				j = i;
			}
		}

		_itoa_s(1900 + tInfo.tm_year, &timeStamp[0], 512, 10);
		_itoa_s(100 + 1 + tInfo.tm_mon, &timeStamp[4], 512 - 4, 10);
		_itoa_s(100 + tInfo.tm_mday, &timeStamp[7], 512 - 7, 10);
		timeStamp[4] = '-';
		timeStamp[7] = '-';
		_itoa_s(100 + tInfo.tm_hour, &timeStamp[10], 512 - 10, 10);
		_itoa_s(100 + tInfo.tm_min, &timeStamp[13], 512 - 13, 10);
		_itoa_s(100 + tInfo.tm_sec, &timeStamp[16], 512 - 16, 10);
		timeStamp[10] = 'T';
		timeStamp[13] = ':';
		timeStamp[16] = ':';
		timeStamp[19] = 0;

		//	set Description
		SetSPFFHeaderItem(ifcModel, 0, 0, sdaiSTRING, "ViewDefinition [CoordinationView]");
		SetSPFFHeaderItem(ifcModel, 0, 1, sdaiSTRING, 0);
		//	set Implementation Level
		SetSPFFHeaderItem(ifcModel, 1, 0, sdaiSTRING, "2;1");
		//	set Name
		SetSPFFHeaderItem(ifcModel, 2, 0, sdaiUNICODE, (char*) &ifcFileName[j]);
		//	set Time Stamp
		SetSPFFHeaderItem(ifcModel, 3, 0, sdaiSTRING, &timeStamp[0]);
		//	set	Author
		SetSPFFHeaderItem(ifcModel, 4, 0, sdaiSTRING, "Architect");
		SetSPFFHeaderItem(ifcModel, 4, 1, sdaiSTRING, 0);
		//	set Organization
		SetSPFFHeaderItem(ifcModel, 5, 0, sdaiSTRING, "Building Designer Office");
		//	set Preprocessor Version
		SetSPFFHeaderItem(ifcModel, 6, 0, sdaiSTRING, "IFC Engine Library 2020");
		//	set Originating System
		SetSPFFHeaderItem(ifcModel, 7, 0, sdaiSTRING, "Alignment example (Cant Alignment)");
		//	set Authorization
		SetSPFFHeaderItem(ifcModel, 8, 0, sdaiSTRING, "The authorising person");
		//	set File Schema
		SetSPFFHeaderItem(ifcModel, 9, 0, sdaiSTRING, "IFC4x3");
		SetSPFFHeaderItem(ifcModel, 9, 1, sdaiSTRING, 0);

		sdaiSaveModelBNUnicode(ifcModel, ifcFileName);

		sdaiCloseModel(ifcModel);
		ifcModel = 0;
	}
	else {
		assert(false);
	}
}
