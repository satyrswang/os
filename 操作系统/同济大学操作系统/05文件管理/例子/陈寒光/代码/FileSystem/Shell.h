// Shell.h: interface for the Shell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELL_H__314E339A_F57F_4B61_9ECC_C239E47EB996__INCLUDED_)
#define AFX_SHELL_H__314E339A_F57F_4B61_9ECC_C239E47EB996__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef SHELL_H
	#define SHELL_H
	#include "FS_MACRO.h"
	#include "FS.h"
	#include "File.h"
	#include "Tokenizer.h"
#endif

class Shell  
{
private:
static const char * help;

/*"
*/
private:
	FS * pFS;
	char userName[16];
	char promt[64];
public:
	bool Login();
	bool Logout();
	int CommandDispatcher( Tokenizer & praser );
	Shell( FS * fs );
	void Run( );

	virtual ~Shell();

};

#endif // !defined(AFX_SHELL_H__314E339A_F57F_4B61_9ECC_C239E47EB996__INCLUDED_)
