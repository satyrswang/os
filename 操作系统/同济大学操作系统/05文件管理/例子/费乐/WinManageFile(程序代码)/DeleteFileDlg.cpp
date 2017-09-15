// DeleteFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinManageFile.h"
#include "DeleteFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeleteFileDlg dialog
CString CDeleteFileDlg::s_FileName;

CDeleteFileDlg::CDeleteFileDlg(ManageMemory* pMemory,CString strDir,CWnd* pParent /*=NULL*/)
	: CDialog(CDeleteFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->s_FileName.Empty();
	this->strDir_ = strDir;
	this->pMemory_ = pMemory;

}


void CDeleteFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteFileDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeleteFileDlg, CDialog)
	//{{AFX_MSG_MAP(CDeleteFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteFileDlg message handlers

void CDeleteFileDlg::ShowAllFile()
{

	CComboBox* pWnd = NULL;
	FileFCB* pFCB = NULL;
	FileFAT* pFAT = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();

	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBOFILE1);
	pWnd->ResetContent();
	if (this->pMemory_ != NULL)
	{
		pFCB = pMemory_->GetFileFCB();
		pFAT = pMemory_->GetFileFAT();
		
		for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
		{
			if (pFCB[i].Type == TYPE_N)
			{
				pWnd->AddString(pFCB[i].FileName);
			}
		}
		if (pWnd->GetCount() > 0)
			pWnd->SetCurSel(0);
	}
}

void CDeleteFileDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->s_FileName.Empty();
	CWnd* pWnd = NULL;
	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBOFILE1);
	pWnd->GetWindowText(this->s_FileName);
	CDialog::OnOK();
}

BOOL CDeleteFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->ShowAllFile();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
