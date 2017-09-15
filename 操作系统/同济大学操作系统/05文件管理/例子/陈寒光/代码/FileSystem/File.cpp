// File.cpp: implementation of the File class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include "File.h"
using namespace std;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


int File::FRead( PBYTE pBuffer, int BYTEToRead ){//返回值为实际读的字节数


	return 0;
}

int File::FWrite( PBYTE pBuffer, int BYTEToWrite ){ //返回值为实际写的字节数
	return 0;
}

int File::FSeek( int BYTEToMove, int fromWhereToMove ){

	return 0 ;
}


File::~File()
{

}

ostream & operator<<( ostream & os,  File & file ){
	os<<file.GetFileName();
	return os;
}
