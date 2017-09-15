// DisplayView.cpp : implementation file
//

#include "stdafx.h"
#include "WinManageFile.h"
#include "DisplayView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayView

IMPLEMENT_DYNCREATE(CDisplayView, CScrollView)

CDisplayView::CDisplayView()
{
	m_Mode = NOREQUEST;
	m_pMemory = NULL;
	vec_.clear();
	::ZeroMemory(m_Name, FILENAME);
}

CDisplayView::~CDisplayView()
{
}


BEGIN_MESSAGE_MAP(CDisplayView, CScrollView)
//{{AFX_MSG_MAP(CDisplayView)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplayView drawing

void CDisplayView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDisplayView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	this->ShowContent(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CDisplayView diagnostics

#ifdef _DEBUG
void CDisplayView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDisplayView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDisplayView message handlers

void CDisplayView::AcceptRequest(ManageMemory* pMemory,int Mode, const char *Name)
{
	this->m_Mode = Mode;
	strcpy(this->m_Name, Name);
	this->m_pMemory = pMemory;
	this->Invalidate();
}



void CDisplayView::ShowDirInfo(CDC *pDC)
{
	int DirID = INVALID_HANDLE;
	CString str;
	FileFCB* pFCB = NULL;
	FileFCB FCB;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	if (this->m_pMemory == NULL)
		return ;
	FileOperate FOp(this->m_pMemory);

	DirID = FOp.GetFileHandle(this->m_Name);
	if (DirID == INVALID_HANDLE)
		return;
	
	pFCB = this->m_pMemory->GetFileFCB();
	this->vec_.clear();
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFCB[i].ParentDirID == DirID)
		{
			FCB.Access = pFCB[i].Access;
			FCB.FileHandle = pFCB[i].FileHandle;
			FCB.FileLength = pFCB[i].FileLength;
			strcpy(FCB.FileName, pFCB[i].FileName);
			FCB.NumDataArea = pFCB[i].NumDataArea;
			FCB.ParentDirID = pFCB[i].ParentDirID;
			FCB.StartID = pFCB[i].StartID;
			FCB.Type = pFCB[i].Type;
			strcpy(FCB.UserName, pFCB[i].UserName);

			this->vec_.push_back(FCB);
		}
	}
	
	CPoint ptDirName(5, 5);
	CPoint ptFileInfo(5, ptDirName.y+LINEHEIGHT+20);
	this->SetScrollSizes(MM_TEXT, CSize(600, LINEHEIGHT*vec_.size()+ptFileInfo.y));
	//show DirName;
	pDC->SetTextColor(RGB(70, 70, 150));
	str.Format("当前文件夹: %s", this->m_Name);
	pDC->TextOut(ptDirName.x, ptDirName.y, str);
	CPen pen(PS_SOLID, 3, RGB(85,55,85));
	pDC->SelectObject(pen);
	pDC->MoveTo(CPoint(0, ptDirName.y+LINEHEIGHT+3));
	pDC->LineTo(600, ptDirName.y+LINEHEIGHT+3);
	//show AllFileInfo
	CPen penLine(PS_SOLID, 1, RGB(120,50,120));
	pDC->SelectObject(penLine);
	for (i = 0; i < vec_.size(); i++)
	{
		CString Type;
		CString Access;
		switch(vec_[i].Type)
		{
		case TYPE_D:
			Type.Format("文件夹");
			break;

		case TYPE_N:
			Type.Format("普通文件");
			break;

		default:
			break;
		}
		switch(vec_[i].Access)
		{
		case ACCESS_O:
			Access.Format("只读");
			break;

		case ACCESS_H:
			Access.Format("隐藏");
			break;

		case ACCESS_N:
			Access.Format("正常");
			break;

		default:
			break;
		}
		str.Format("文件名字: %s      文件类型: %s    文件长度: %d    存取权限:%s    文件句柄: %d",vec_[i].FileName,Type,vec_[i].FileLength,Access,vec_[i].FileHandle);
		pDC->TextOut(ptFileInfo.x, ptFileInfo.y+i*LINEHEIGHT,str);
		pDC->MoveTo(CPoint(0,ptFileInfo.y+i*LINEHEIGHT+17));
		pDC->LineTo(600, ptFileInfo.y+i*LINEHEIGHT+17);
	}
	return;
}

void CDisplayView::ReadFileInfo(CDC *pDC)
{
	int FileHandle = INVALID_HANDLE;
	CString str;
	CString strTemp;
	int len = 127;
	int temp = 0;
	char buffer[128];
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	if (this->m_pMemory == NULL)
		return ;
	FileOperate FOp(this->m_pMemory);

	FileHandle = FOp.OpenFile_(this->m_Name);
	if (FileHandle == INVALID_HANDLE)
		return;

	str.Empty();
	while(true)
	{
		temp = FOp.ReadFile_(FileHandle, buffer, len);
		buffer[temp] = '\0';
		str += buffer;
		if (temp < len)
			break;
	}

	CPoint ptFileName(5,5);
	CPoint ptContent(5,30);
	CPen pen(PS_SOLID, 3, RGB(85,55,85));
	pDC->SelectObject(pen);
	pDC->SetTextColor(RGB(90, 70, 115));
	strTemp.Format("文件名: %s", this->m_Name);
	pDC->TextOut(ptFileName.x, ptFileName.y, strTemp);
	pDC->MoveTo(0, ptFileName.y+20);
	pDC->LineTo(600, ptFileName.y+20);
	pDC->TextOut(ptContent.x, ptContent.y, str);

}

void CDisplayView::ShowContent(CDC *pDC)
{
	switch(this->m_Mode)
	{
	case SHOWDIR:
		this->ShowDirInfo(pDC);
		break;
	case READFILE:
		this->ReadFileInfo(pDC);
		break;
	case NOREQUEST:
		break;
	default:
		break;
	}
	return;
}
