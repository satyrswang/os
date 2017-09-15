// DirNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinManageFile.h"
#include "DirNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirNameDlg dialog

CString CDirNameDlg::s_DirName;
CDirNameDlg::CDirNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirNameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->s_DirName.Empty();
}


void CDirNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirNameDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirNameDlg, CDialog)
	//{{AFX_MSG_MAP(CDirNameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirNameDlg message handlers

void CDirNameDlg::OnOK() 
{
	// TODO: Add extra validation here
	CWnd* pWnd = NULL;
	pWnd = this->GetDlgItem(IDC_DIRNAME);
	pWnd->GetWindowText(this->s_DirName);
	CDialog::OnOK();
}
