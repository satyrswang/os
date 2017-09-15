// Tokenizer.cpp: implementation of the Tokenizer class.
//
//////////////////////////////////////////////////////////////////////

#include "Tokenizer.h"
#include "MyList.h"
using namespace FS_STD;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Tokenizer::Tokenizer(char * str)
{
	char * pSpace = str;
	char * pPre = str;
	while( *pSpace != '\0' ){
		//过滤空格
		while( *pSpace == ' ' ) pSpace ++;
		pPre = pSpace;
		//提取单词
		while( *pSpace != ' '  && *pSpace != '\0' ) pSpace ++;
		int size = pSpace - pPre + 1;
		char * pWord = new char[size];
		::memcpy( pWord, pPre, pSpace - pPre );
		pWord[size-1] = '\0';
		container.push_back( pWord );
		//过滤空格
		while( *pSpace == ' ' ) pSpace ++;
		pPre = pSpace;
	}

}

Tokenizer::~Tokenizer()
{
	char *pWord ;
	while( container.size() > 0 ){
		pWord = container.front();
		delete [] pWord;
		container.pop_front();
	}

}

char *
Tokenizer::operator []( int index )
{
	if( index < 0 || index >= container.size() ){
		return NULL;
	}else{
		List<char*>::iterator iter = container.begin();
		while( -- index   >= 0 ){
			iter ++;
		}
		return *iter;
	}

}

int Tokenizer::size(){

	return container.size();
}

bool Tokenizer::HasNext()
{
	//wait to implements
	return true;
}

char * Tokenizer::Next()
{
	//wait to implements
	return NULL;
}
