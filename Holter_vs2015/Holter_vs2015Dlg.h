
// Holter_vs2015Dlg.h : header file
#pragma once
#include "stdafx.h"
#include "curlapi.h"

// CHolter_vs2015Dlg dialog
class CHolter_vs2015Dlg : public CDialogEx
{
// Construction
public:
	CHolter_vs2015Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOLTER_VS2015_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_uid;
	CString m_password;
	CString m_fpath;
	CString m_dlurl;
	curl_interface curl_imp;

	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedupload();
	afx_msg void OnClickedDownLoad();

	afx_msg void OnBnClickedButton3();
};
