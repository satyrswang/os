// WriteFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinManageFile.h"
#include "WriteFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWriteFileDlg dialog


CWriteFileDlg::CWriteFileDlg(ManageMemory* pMemory, CString FileName,CWnd* pParent /*=NULL*/)
: CDialog(CWriteFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWriteFileDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->pMemory_ = pMemory;
	this->strFile_ = FileName;
}


void CWriteFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWriteFileDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWriteFileDlg, CDialog)
//{{AFX_MSG_MAP(CWriteFileDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWriteFileDlg message handlers

BOOL CWriteFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;
	int FileHandle = INVALID_HANDLE;
	int len = 127;
	int temp = 0;
	char buffer[128];
	// TODO: Add extra initialization here
	if (this->pMemory_ == NULL)
		return true;
	
	FileOperate FOp(this->pMemory_);
	FileHandle = FOp.OpenFile_(this->strFile_);
	if (FileHandle == INVALID_HANDLE)
		return true;
	
	str.Empty();
	while(true)
	{
		temp = FOp.ReadFile_(FileHandle, buffer, len);
		buffer[temp] = '\0';
		str += buffer;
		if (temp < len)
			break;
	}
	
	CWnd* pWnd = NULL;
	pWnd = this->GetDlgItem(IDC_CONTENT);
	pWnd->SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWriteFileDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	int FileHandle = INVALID_HANDLE;
	CWnd* pWnd = NULL;
	pWnd = this->GetDlgItem(IDC_CONTENT);
	pWnd->GetWindowText(str);

	if (this->pMemory_ != NULL)
	{
		FileOperate FOp(this->pMemory_);
		FileHandle = FOp.OpenFile_(this->strFile_);
		if (FileHandle != INVALID_HANDLE)
		{
			FOp.WriteFile_(FileHandle, str, str.GetLength());
		}		
	}
	CDialog::OnOK();
}
