// File.cpp: implementation of the File class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include "File.h"
using namespace std;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


int File::FRead( PBYTE pBuffer, int BYTEToRead ){//����ֵΪʵ�ʶ����ֽ���


	return 0;
}

int File::FWrite( PBYTE pBuffer, int BYTEToWrite ){ //����ֵΪʵ��д���ֽ���
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
