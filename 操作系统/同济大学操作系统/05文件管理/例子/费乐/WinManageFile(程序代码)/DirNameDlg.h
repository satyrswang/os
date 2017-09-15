#if !defined(AFX_DIRNAMEDLG_H__1D214EF6_4A9B_43AF_8A70_3B27108C92B4__INCLUDED_)
#define AFX_DIRNAMEDLG_H__1D214EF6_4A9B_43AF_8A70_3B27108C92B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirNameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDirNameDlg dialog

class CDirNameDlg : public CDialog
{
// Construction
public:
	CDirNameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDirNameDlg)
	enum { IDD = IDD_DIRNAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	static CString s_DirName;

protected:

	// Generated message map functions
	//{{AFX_MSG(CDirNameDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRNAMEDLG_H__1D214EF6_4A9B_43AF_8A70_3B27108C92B4__INCLUDED_)
