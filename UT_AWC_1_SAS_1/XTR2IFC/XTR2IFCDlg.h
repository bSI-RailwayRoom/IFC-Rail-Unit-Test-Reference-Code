
// XTR2IFCDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CXTR2IFCDlg dialog
class CXTR2IFCDlg : public CDialogEx
{
// Construction
public:
	CXTR2IFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_XTR2IFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit p_XTRfile;
	CEdit p_IFCfile;
	CButton p_HA;
	CButton p_VA;
	CButton p_CA;
	CEdit p_SchemaFile;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	CButton p_SimpleSolid;
	CEdit pFactor;
	CEdit pFactorCant;
};
