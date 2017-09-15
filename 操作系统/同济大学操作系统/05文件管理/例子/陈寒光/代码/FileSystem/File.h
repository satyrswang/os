// File.h: interface for the File class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILE_H__0C405274_BEFF_46AB_9568_D56030AF855E__INCLUDED_)
#define AFX_FILE_H__0C405274_BEFF_46AB_9568_D56030AF855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

	#include "FS_MACRO.h"
	#include "FS.h"
	#include <windows.h>



class File  
{
	friend class FS;
	friend class Shell;
	friend ostream & operator<<(ostream & os ,  File & file );

private:
	ENTRY * pFileEntry;
	int   openMode; //��readOnly, writeOnly, readWrite.
	int   curPos;   //0----fileSize, ���ΪfileSize, ���ʾ�Ѿ����ļ���ĩβ��
	BYTE * curActivePtr;//
				//00h---normal file
			   //01h---file that can only be read
			   // 02h---hidden file 
			   // 03h---system file
	           // 08h---reserved 
			   // 10h -- subdirectory
			   // 20h  archive file

private:
	File(){ }//���ù���ģʽ�� ��FS�ฺ�����File����
public:
	int FRead( PBYTE pBuffer, int BYTEToRead );//����ֵΪʵ�ʶ����ֽ���
	int FWrite( PBYTE pBuffer, int BYTEToWrite ); //����ֵΪʵ��д���ֽ���
	int FSeek( int BYTEToMove, int fromWhereToMove );
	short GetType(){
			return pFileEntry->type;
	}
public:

	char * GetFileName(){
		if( pFileEntry->type == SUBDIRECTORY ){
			return pFileEntry->fileMainName ;
		}else{
			char * r = new char[12];
			int i;
			for( i = 0 ; i < 8; i ++){
				if( pFileEntry->fileMainName[i] == '\0' ) break;
				r[i] = pFileEntry->fileMainName[i];
			}
			for( int j = 0; j < 3; j ++){
				if( pFileEntry->extension[j] == '\0' ) break;
				r[i++] = pFileEntry->extension[j];
			}
		 r[i++] = '\0';
		 return r;
		}
	
	}



	virtual ~File();

};

#endif // !defined(AFX_FILE_H__0C405274_BEFF_46AB_9568_D56030AF855E__INCLUDED_)
