#if !defined(AFX_FILEDLG_H__E6E5D6C0_C9FA_4A7A_A741_44C384DED326__INCLUDED_)
#define AFX_FILEDLG_H__E6E5D6C0_C9FA_4A7A_A741_44C384DED326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog

class CFileDlg : public CDialog
{
// Construction
public:
	CFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileDlg)
	enum { IDD = IDD_FILENAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	static CString s_FileName;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDLG_H__E6E5D6C0_C9FA_4A7A_A741_44C384DED326__INCLUDED_)
