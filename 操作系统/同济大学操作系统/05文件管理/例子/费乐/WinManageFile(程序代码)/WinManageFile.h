// WinManageFile.h : main header file for the WINMANAGEFILE application
//

#if !defined(AFX_WINMANAGEFILE_H__0CF1A4BF_5078_480C_A5E3_D5BE8F32939B__INCLUDED_)
#define AFX_WINMANAGEFILE_H__0CF1A4BF_5078_480C_A5E3_D5BE8F32939B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinManageFileApp:
// See WinManageFile.cpp for the implementation of this class
//

class CWinManageFileApp : public CWinApp
{
public:
	CWinManageFileApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinManageFileApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinManageFileApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINMANAGEFILE_H__0CF1A4BF_5078_480C_A5E3_D5BE8F32939B__INCLUDED_)
