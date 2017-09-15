#if !defined(AFX_WRITEFILEDLG_H__CFF207AE_C52E_4FBA_AE44_F4CCDB4EE445__INCLUDED_)
#define AFX_WRITEFILEDLG_H__CFF207AE_C52E_4FBA_AE44_F4CCDB4EE445__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WriteFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWriteFileDlg dialog

class CWriteFileDlg : public CDialog
{
// Construction
public:
	CWriteFileDlg(ManageMemory* pMemory, CString FileName, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWriteFileDlg)
	enum { IDD = IDD_WRITEFILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWriteFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CString strFile_;
	ManageMemory* pMemory_;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWriteFileDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITEFILEDLG_H__CFF207AE_C52E_4FBA_AE44_F4CCDB4EE445__INCLUDED_)
