// WinManageFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileSystemDlg.h"
#include "WinManageFile.h"
#include "WinManageFileDlg.h"
#include "DirNameDlg.h"
#include "FileDlg.h"
#include "DeleteFileDlg.h"
#include "WriteFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinManageFileDlg dialog

CWinManageFileDlg::CWinManageFileDlg(CWnd* pParent /*=NULL*/)
: CDialog(CWinManageFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWinManageFileDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->m_pView = NULL;
	this->m_pMemory = NULL;
}

void CWinManageFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinManageFileDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinManageFileDlg, CDialog)
//{{AFX_MSG_MAP(CWinManageFileDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_CREATENEWSYSTEM, OnCreatenewsystem)
ON_BN_CLICKED(IDC_OPENSYSTEM, OnOpensystem)
ON_BN_CLICKED(IDC_SAVESYSTEM, OnSavesystem)
ON_BN_CLICKED(IDC_CREATENEWDIR, OnCreateNewDIR)
ON_BN_CLICKED(IDC_DELETEDIR, OnDeleteDIR)
ON_BN_CLICKED(IDC_SHOWDIRINFO, OnShowDIRInfo)
ON_BN_CLICKED(IDC_CREATENEWFILE, OnCreateNewFile)
ON_BN_CLICKED(IDC_DELETEFILE, OnDeleteFile)
ON_BN_CLICKED(IDC_READFILE, OnReadFile)
ON_BN_CLICKED(IDC_WRITEFILE, OnWriteFile)
ON_BN_CLICKED(IDC_ABOUT, OnAbout)
ON_BN_CLICKED(IDC_MODIFYDIR, OnModifyDir)
ON_BN_CLICKED(IDC_MODIFYFILE, OnModifyFile)
	ON_CBN_SELCHANGE(IDC_COMBODIR, OnSelchangeComboDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinManageFileDlg message handlers

BOOL CWinManageFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CRect rc;
	CWnd* pWnd  = NULL;
	CRuntimeClass* prt = RUNTIME_CLASS( CDisplayView );
	m_pView = (CDisplayView*)prt->CreateObject();
	pWnd = this->GetDlgItem(IDC_PLACEHOLDER);
	pWnd->GetClientRect(&rc);
	pWnd->ClientToScreen(&rc);
	this->ScreenToClient(&rc);
	m_pView->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rc, this, 10001, NULL);
	m_pView->SetScrollSizes(MM_TEXT, CSize(100,100));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinManageFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinManageFileDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinManageFileDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWinManageFileDlg::OnCreatenewsystem() 
{
	// TODO: Add your control notification handler code here
	CFileSystemDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		if (m_pMemory != NULL)
			delete m_pMemory;
		
		m_pMemory = new ManageMemory;
		if (m_pMemory != NULL)
		{
			FileOperate FileOp(this->m_pMemory);
			FileOp.InitMemory();
			this->AddComboxDir();
			ShowSystemInfo();
			this->OnShowDIRInfo();
		}
	}
	
}

void CWinManageFileDlg::OnOpensystem() 
{
	// TODO: Add your control notification handler code here
	CString strName;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	CFileDialog dlg(true,NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_FILEMUSTEXIST, "FileSystem (*.fs)|*.fs|All Files (*.*)|*.*||", this); 
	if (dlg.DoModal() == IDOK)
	{
		strName = dlg.GetPathName();
		//this->MessageBox(strName);
		if (this->m_pMemory != NULL)
		{
			delete this->m_pMemory;
			this->m_pMemory = NULL;
		}
		pFileInfo->SetFileName(strName.GetBuffer(strName.GetLength()));
		this->m_pMemory = new ManageMemory;
		this->AddComboxDir();
		ShowSystemInfo();
		this->OnShowDIRInfo();		
	}
	
}

void CWinManageFileDlg::OnSavesystem() 
{
	// TODO: Add your control notification handler code here
	CString strName;
	CFileDialog dlg(false,NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "FileSystem (*.fs)|*.fs|All Files (*.*)|*.*||", this); 
	if (dlg.DoModal() == IDOK)
	{
		strName = dlg.GetPathName();
		//this->MessageBox(strName);
		if (this->m_pMemory != NULL)
			this->m_pMemory->WirteFile(strName.GetBuffer(strName.GetLength()));
	}
}



void CWinManageFileDlg::ShowSystemInfo()
{
	CWnd* pWnd = NULL;
	FileFCB* pFCB = NULL;
	FileFAT* pFAT = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	CString str;
	
	int NumFile = 0;
	int NumDir = 0;
	int SystemSize = 0;
	int NumLeft = 0;
	
	if (this->m_pMemory != NULL)
	{
		pFCB = m_pMemory->GetFileFCB();
		pFAT = m_pMemory->GetFileFAT();
		SystemSize = sizeof(FileFCB)*pFileInfo->GetNumFCB()+sizeof(FileFAT)*pFileInfo->GetNumDataArea()+sizeof(DataArea)*pFileInfo->GetNumDataArea();
		for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
		{
			if (pFCB[i].Type == TYPE_D)
				NumDir++;
			if (pFCB[i].Type == TYPE_N)
				NumFile++;
		}
		for (i = 0; i < pFileInfo->GetNumDataArea(); i++)
		{
			if (!pFAT[i].bUsed)
			{
				NumLeft++;
			}	
		}
		NumLeft *= sizeof(DataArea);
		pWnd = this->GetDlgItem(IDC_NUMFILE);
		str.Format("总文件数目：%d",NumFile);
		pWnd->SetWindowText(str);
		pWnd = this->GetDlgItem(IDC_NUMDIR);
		str.Format("总文件夹数目：%d",NumDir);
		pWnd->SetWindowText(str);
		pWnd = this->GetDlgItem(IDC_SIZESYSTEM);
		str.Format("文件系统大小：%d",SystemSize);
		pWnd->SetWindowText(str);
		pWnd = this->GetDlgItem(IDC_NUMLEFT);
		str.Format("剩余字节数：%d",NumLeft);
		pWnd->SetWindowText(str);
	}
	else
	{
		pWnd = this->GetDlgItem(IDC_NUMFILE);
		str.Format("总文件数目：");
		pWnd->SetWindowText(str);
		pWnd = this->GetDlgItem(IDC_NUMDIR);
		str.Format("总文件夹数目：");
		pWnd->SetWindowText(str);
		pWnd = this->GetDlgItem(IDC_SIZESYSTEM);
		str.Format("文件系统大小：");
		pWnd->SetWindowText(str);
		pWnd = this->GetDlgItem(IDC_NUMLEFT);
		str.Format("剩余字节数：");
		pWnd->SetWindowText(str);
	}
}

void CWinManageFileDlg::AddComboxDir()
{
	CComboBox* pWnd = NULL;
	FileFCB* pFCB = NULL;
	FileFAT* pFAT = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	
	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBODIR);
	pWnd->ResetContent();
	if (this->m_pMemory != NULL)
	{
		pFCB = m_pMemory->GetFileFCB();
		pFAT = m_pMemory->GetFileFAT();
		
		for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
		{
			if (pFCB[i].Type == TYPE_D)
			{
				pWnd->AddString(pFCB[i].FileName);
			}
		}
		if (pWnd->GetCount() > 0)
			pWnd->SetCurSel(0);
	}
}

void CWinManageFileDlg::OnCreateNewDIR() 
{
	// TODO: Add your control notification handler code here
	CDirNameDlg dlg;
	CComboBox* pWnd = NULL;
	CString strParent;
	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBODIR);
	if (IDOK == dlg.DoModal())
	{
		if (this->m_pMemory != NULL)
		{
			FileOperate FOp(this->m_pMemory);
			pWnd->GetWindowText(strParent);
			FOp.CreateDIR(dlg.s_DirName.GetBuffer(dlg.s_DirName.GetLength()),  strParent.GetBuffer(strParent.GetLength()));
			this->AddComboxDir();
			ShowSystemInfo();
			this->OnShowDIRInfo();
		}
	}	
}

void CWinManageFileDlg::OnDeleteDIR() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pWnd = NULL;
	CString strDir;
	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBODIR);
	pWnd->GetWindowText(strDir);
	if (this->m_pMemory != NULL)
	{
		if (strDir.Compare(MainName) == 0)
		{
			this->MessageBox("不能删除主目录");
			return;
		}
		FileOperate FOp(this->m_pMemory);
		FOp.DeleteDIR(strDir.GetBuffer(strDir.GetLength()));
		this->AddComboxDir();
		ShowSystemInfo();
		this->OnShowDIRInfo();
	}
}

void CWinManageFileDlg::OnShowDIRInfo() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pWnd = NULL;
	CString strDir;
	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBODIR);
	pWnd->GetWindowText(strDir);
	if (this->m_pMemory != NULL)
	{
		this->m_pView->AcceptRequest(this->m_pMemory,SHOWDIR, strDir);
	}
	
}

void CWinManageFileDlg::OnCreateNewFile() 
{
	// TODO: Add your control notification handler code here
	CFileDlg dlg;
	CComboBox* pWnd = NULL;
	CString strParent;
	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBODIR);
	pWnd->GetWindowText(strParent);
	if (IDOK == dlg.DoModal())
	{
		if (this->m_pMemory != NULL)
		{
			FileOperate FOp(this->m_pMemory);
			FOp.CreateFile_(CFileDlg.s_FileName, strParent);
			this->AddComboxDir();
			ShowSystemInfo();
			this->OnShowDIRInfo();
		}
	}
	
}

void CWinManageFileDlg::OnDeleteFile() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pWnd = NULL;
	CString strParent;
	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBODIR);
	pWnd->GetWindowText(strParent);
	CDeleteFileDlg dlg(this->m_pMemory, strParent);
	if (IDOK == dlg.DoModal())
	{
		FileOperate FOp(this->m_pMemory);
		FOp.DeleteFile_(CDeleteFileDlg::s_FileName);
		this->AddComboxDir();
		ShowSystemInfo();
		this->OnShowDIRInfo();
	}
	
}

void CWinManageFileDlg::OnReadFile() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pWnd = NULL;
	CString strParent;
	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBODIR);
	pWnd->GetWindowText(strParent);
	CDeleteFileDlg dlg(this->m_pMemory, strParent);
	if (IDOK == dlg.DoModal())
	{
		if (!CDeleteFileDlg::s_FileName.IsEmpty())
			this->m_pView->AcceptRequest(this->m_pMemory, READFILE, CDeleteFileDlg::s_FileName);
	}	
}

void CWinManageFileDlg::OnWriteFile() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pWnd = NULL;
	CString strParent;
	pWnd = (CComboBox*)this->GetDlgItem(IDC_COMBODIR);
	pWnd->GetWindowText(strParent);
	CDeleteFileDlg dlg(this->m_pMemory, strParent);
	if (IDOK == dlg.DoModal())
	{
		if (!CDeleteFileDlg::s_FileName.IsEmpty())
		{
			CWriteFileDlg WriteDlg(this->m_pMemory, CDeleteFileDlg::s_FileName);
			WriteDlg.DoModal();
		}
		this->AddComboxDir();
		ShowSystemInfo();
	}
}

void CWinManageFileDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal();
	
}

void CWinManageFileDlg::OnModifyDir() 
{
	// TODO: Add your control notification handler code here
	
}

void CWinManageFileDlg::OnModifyFile() 
{
	// TODO: Add your control notification handler code here
	
}


void CWinManageFileDlg::OnSelchangeComboDir() 
{
	// TODO: Add your control notification handler code here
	this->OnShowDIRInfo();
}
