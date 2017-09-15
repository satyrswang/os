// Tokenizer.h: interface for the Tokenizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOKENIZER_H__265947FD_FAA0_4E9E_BA65_A56AEADAE94B__INCLUDED_)
#define AFX_TOKENIZER_H__265947FD_FAA0_4E9E_BA65_A56AEADAE94B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyList.h"
using namespace FS_STD;

class Tokenizer  
{
private:
	
	List<char * > container;
public:
	char * Next();
	bool HasNext();
	Tokenizer(char * str );
	char * operator[]( int index );
	virtual ~Tokenizer();
	int size();
};

#endif // !defined(AFX_TOKENIZER_H__265947FD_FAA0_4E9E_BA65_A56AEADAE94B__INCLUDED_)
