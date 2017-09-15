#if !defined(AFX_DISPLAYVIEW_H__16069569_2AA0_4BEF_A48C_D18D606D054E__INCLUDED_)
#define AFX_DISPLAYVIEW_H__16069569_2AA0_4BEF_A48C_D18D606D054E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplayView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplayView view

class CDisplayView : public CScrollView
{
protected:
	CDisplayView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDisplayView)
		
		// Attributes
public:
	
	// Operations
public:
	void AcceptRequest(ManageMemory* pMemory,int Mode, const char* Name);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplayView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL
private:
	int m_Mode;
	char m_Name[FILENAME];
	vector<FileFCB> vec_;
	ManageMemory* m_pMemory;

	void ShowContent(CDC* pDC);
	void ShowDirInfo(CDC* pDC);
	void ReadFileInfo(CDC *pDC);
	
	// Implementation
protected:
	virtual ~CDisplayView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// Generated message map functions
	//{{AFX_MSG(CDisplayView)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYVIEW_H__16069569_2AA0_4BEF_A48C_D18D606D054E__INCLUDED_)
