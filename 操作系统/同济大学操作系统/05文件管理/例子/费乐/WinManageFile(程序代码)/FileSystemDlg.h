#if !defined(AFX_FILESYSTEMDLG_H__0D9F9C4D_71B3_452C_8A9B_E2C2BB79EB1C__INCLUDED_)
#define AFX_FILESYSTEMDLG_H__0D9F9C4D_71B3_452C_8A9B_E2C2BB79EB1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileSystemDlg.h : header file
//
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CFileSystemDlg dialog

class CFileSystemDlg : public CDialog
{
// Construction
public:
	CFileSystemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileSystemDlg)
	enum { IDD = IDD_FILESYSTEM };
	int		m_nFCB;
	int		m_nFAT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSystemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileSystemDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESYSTEMDLG_H__0D9F9C4D_71B3_452C_8A9B_E2C2BB79EB1C__INCLUDED_)
