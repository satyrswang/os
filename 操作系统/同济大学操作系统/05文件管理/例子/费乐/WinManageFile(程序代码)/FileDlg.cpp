// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinManageFile.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog

CString CFileDlg::s_FileName;
CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->s_FileName.Empty();
}


void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
	//{{AFX_MSG_MAP(CFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDlg message handlers

void CFileDlg::OnOK() 
{
	// TODO: Add extra validation here
	CWnd* pWnd = NULL;
	pWnd = this->GetDlgItem(IDC_FILENAME);
	pWnd->GetWindowText(this->s_FileName);
	CDialog::OnOK();
}
