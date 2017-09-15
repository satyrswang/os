// WinManageFileDlg.h : header file
//

#if !defined(AFX_WINMANAGEFILEDLG_H__BAC0D277_B1AD_4963_A081_76DBD6FD66B3__INCLUDED_)
#define AFX_WINMANAGEFILEDLG_H__BAC0D277_B1AD_4963_A081_76DBD6FD66B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWinManageFileDlg dialog
#include "DisplayView.h"
#include "ManageMemory.h"

class CWinManageFileDlg : public CDialog
{
	// Construction
public:
	CWinManageFileDlg(CWnd* pParent = NULL);	// standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CWinManageFileDlg)
	enum { IDD = IDD_WINMANAGEFILE_DIALOG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinManageFileDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void AddComboxDir();
	void ShowSystemInfo();
	CDisplayView* m_pView;
	ManageMemory* m_pMemory;
	// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CWinManageFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCreatenewsystem();
	afx_msg void OnOpensystem();
	afx_msg void OnSavesystem();
	afx_msg void OnCreateNewDIR();
	afx_msg void OnDeleteDIR();
	afx_msg void OnShowDIRInfo();
	afx_msg void OnCreateNewFile();
	afx_msg void OnDeleteFile();
	afx_msg void OnReadFile();
	afx_msg void OnWriteFile();
	afx_msg void OnAbout();
	afx_msg void OnModifyDir();
	afx_msg void OnModifyFile();
	afx_msg void OnSelchangeComboDir();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINMANAGEFILEDLG_H__BAC0D277_B1AD_4963_A081_76DBD6FD66B3__INCLUDED_)
