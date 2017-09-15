// FileSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinManageFile.h"
#include "FileSystemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileSystemDlg dialog


CFileSystemDlg::CFileSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileSystemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileSystemDlg)
	m_nFCB = 0;
	m_nFAT = 0;
	//}}AFX_DATA_INIT
}


void CFileSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileSystemDlg)
	DDX_Text(pDX, IDC_FCB, m_nFCB);
	DDV_MinMaxInt(pDX, m_nFCB, 1, 16384);
	DDX_Text(pDX, IDC_FAT, m_nFAT);
	DDV_MinMaxInt(pDX, m_nFAT, 1, 65536);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CFileSystemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSystemDlg message handlers

void CFileSystemDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData(TRUE);
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();

	pFileInfo->SetNumFCB(this->m_nFCB);
	pFileInfo->SetNumDataArea(this->m_nFAT);
	CDialog::OnOK();
}

BOOL CFileSystemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_nFCB = 512;	
	this->m_nFAT = 2048;
	this->UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
