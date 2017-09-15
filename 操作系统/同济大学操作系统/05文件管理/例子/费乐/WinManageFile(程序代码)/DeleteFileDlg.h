#if !defined(AFX_DELETEFILEDLG_H__0EA8AF69_DCE2_4A7B_9972_A44CAC36988D__INCLUDED_)
#define AFX_DELETEFILEDLG_H__0EA8AF69_DCE2_4A7B_9972_A44CAC36988D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeleteFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeleteFileDlg dialog

class CDeleteFileDlg : public CDialog
{
	// Construction
public:
	CDeleteFileDlg(ManageMemory* pMemory,CString strDir, CWnd* pParent = NULL);   // standard constructor
	static CString s_FileName;
	// Dialog Data
	//{{AFX_DATA(CDeleteFileDlg)
	enum { IDD = IDD_DELETEFILE };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeleteFileDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void ShowAllFile();
	CString strDir_;
	ManageMemory* pMemory_;
		// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDeleteFileDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELETEFILEDLG_H__0EA8AF69_DCE2_4A7B_9972_A44CAC36988D__INCLUDED_)
