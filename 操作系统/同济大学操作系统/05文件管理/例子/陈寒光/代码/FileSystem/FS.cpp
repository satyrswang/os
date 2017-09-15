// FS.cpp: implementation of the FS class.
//
//////////////////////////////////////////////////////////////////////

#include "FS.h"
#include "File.h"
#include <windows.h>
#include "MyList.h"
#include <iostream>

#include "FS_Macro.h"
#include <assert.h>

using namespace std;
using namespace FS_STD;


/*******************************************************************
 Construction
********************************************************************/
FS::FS( char * onDiskFileName )
{
 this->onDiskFileName = onDiskFileName;
 this->pFSConfig = NULL;
 this->pFAT16   = NULL;
 this->pFAT16_2  = NULL;
 this->pROOTDIR = NULL;

 this->openedFileNum  = 0;
 this->curWorkDir = NULL;
 this->hFileOnDisk= NULL;
 
 if ( (this->hFileOnDisk = ::CreateFile( this->onDiskFileName,
										   GENERIC_READ|GENERIC_WRITE,
										   0, 
										   NULL,
										   OPEN_EXISTING ,
										   FILE_ATTRIBUTE_NORMAL,
										   NULL))
						== INVALID_HANDLE_VALUE  )
 {//ԭ�ļ�������
	 //if open the file false! then create the fiel on real disk!
	 this->hFileOnDisk = ::CreateFile(this->onDiskFileName ,
									   GENERIC_READ|GENERIC_WRITE,
									   0, 
									   NULL, 
									   CREATE_NEW,
									   FILE_ATTRIBUTE_NORMAL, 
									   NULL 
									  );
	 cout<<"New A Disk File !!" <<endl;
	 cout<<"Format the Disk File ...."<<endl;
	 this->FSFormat();
	 cout<<"Formating the Disk File has Finished "<<endl;
 }else{//ԭ�����ļ����ڵĻ�
	cout<<"Use the Old Disk File !!"<<endl;
	 
 }

this->InitFSParameters();



}

void FS::InitFSParameters(){
	openedFileNum = 0;//ֻ��򿪵��ļ�, ����Ŀ¼
	if( pFSConfig == NULL ){
		this->pFSConfig = new BYTE[ 512 *(1 + 9 + 9 + 14 ) ];
	}
	for(int  i = 0; i < (1 + 9 + 9 + 14 ) ; i ++){
		this->FSReadSector( (pFSConfig + i * 512),  512, i, 0 );
	}

	this->pFAT16   = (WORD*) (this->pFSConfig + (1)*512);
	this->pFAT16_2 = (WORD*) (this->pFSConfig + (1+9)*512);
	this->pROOTDIR = (ENTRY*)(this->pFSConfig + (1+9+9)*512);

	cout<<endl<<"*******************************************************************"<<endl;
	cout<<"the Boot Record's size:\t\t"<<"512\tBYTES"<<endl;
	cout<<"the FAT1's size : \t\t"<<(BYTE*)pFAT16_2-(BYTE*)pFAT16<<"\tBYTES "<<endl;
	cout<<"the FAT2's size : \t\t"<<(BYTE*)pROOTDIR-(BYTE*)pFAT16_2<<"\tBYTES"<<endl;
	cout<<"the Root Directory's size:\t"<<14*512 <<"\tBYTES"<<endl;
	cout<<endl<<"*******************************************************************"<<endl;

	/************************
	 �����ʼ����ǰ����Ŀ¼
	 ************************/
	curWorkDir = (File *)malloc( sizeof(File));
	curWorkDir->pFileEntry = new ENTRY();
	curWorkDir->pFileEntry->startCluster = 0;
	this->curWorkDir->openMode = OPEN_MODE_READ_WRITE;
	fullPathName.clear();
	fullPathName.push_back("/");
	fullPath.clear();
	fullPath.push_back(curWorkDir->pFileEntry);
	

}

/**************************************
�����ļ��ĸ�ʽ������
************************************/
void FS::FSFormat(){
	BYTE * pBytes = new BYTE[512] ;
	int counter = 0;
	//���ȸ�ʽ��boot sector;
	::memset( (void *) pBytes, 0x00000000, 512 );
	this->FSWriteSector( pBytes, 512, counter ++ , 0);//when write , counter == 0;
	//�����FAT16����, ��ʹ��һ����FAT16.
	pBytes[0] = (BYTE)0xf0;
	pBytes[1] = (BYTE)0xff;
	pBytes[2] = (BYTE)0xff;
	pBytes[3] = (BYTE)0xff;
	this->FSWriteSector( pBytes, 512, counter++ , 0);//when write sector, counter == 1;
	
	::memset( (void *) pBytes, 0x00000000, 512 ); 
	for( int j =1; j < 9; j ++, counter++){
		this->FSWriteSector( pBytes, 512, counter , 0);
		::OutputDebugString("format fat16 sectors\n\r");
	}
	/////////////////////////////////////////////////
	//���ǵڶ���FAT16��.
	for( j =0; j < 9; j ++, counter++){
		this->FSWriteSector( pBytes, 512, counter , 0);
		::OutputDebugString("format fat16 sectors\n\r");
	}
	//Ȼ���Ǹ�Ŀ¼�� 
	for( int i = 0; i < 14; i ++, counter ++){
		this->FSWriteSector( pBytes, 512, counter, 0);
		::OutputDebugString("format dir sectors\n\r");
	}

    //����� content sector, ����ǿ��ٸ�ʽ���� �˲�����ʡ��.
	::memset( (void *) pBytes, 0x00000000, 512);
	int contentSector = ( 512 * 9 / 2 ) * SECTOR_PER_CLUSTER;
	for( int k = 0 ; k < contentSector; k ++, counter ++){
		this->FSWriteSector( pBytes, 512, counter , 0);
	}

	delete [] pBytes;

}

//next four functions are base services!
/********************************************************************
��ĳһ����������ȡ����
�ڽ���˺���ǰ�� ��֤����Խ��������������ļ�
*********************************************************************/

int FS::FSReadSector( PBYTE pBuf, int size,   int whichSector, int begPosInSector ){
	unsigned long readBytes = 0;

	if (  begPosInSector + size > 512 ){
		size = 512 - begPosInSector;
	}

	::SetFilePointer( this->hFileOnDisk, whichSector*512 + begPosInSector  , NULL, FILE_BEGIN);
	::ReadFile( this->hFileOnDisk, (void *)pBuf, size,  &readBytes, NULL );
	
	return readBytes;

}

/********************************************************************
��ĳһ��������д���ݣ�
�ڽ���˺���ǰ�� ��֤����Խ��������������ļ�
*********************************************************************/

int FS::FSWriteSector( PBYTE pBuf, int size , int whichSector ,int begPosInSector){
	unsigned long writeBytes = 0;
	int diskFileSize = ::GetFileSize( this->hFileOnDisk, NULL );
	if ( begPosInSector + size >= 512 ){
		 size = 512 - begPosInSector;
	}
	::SetFilePointer( this->hFileOnDisk, whichSector*512 + begPosInSector, NULL, FILE_BEGIN);
	::WriteFile( this->hFileOnDisk, (void *)pBuf, size,  &writeBytes, NULL );

	return writeBytes;
}

/********************************************************************
дDATA AREA����
�ڽ���˺���ǰ�� ��֤����Խ��������������ļ�
*********************************************************************/
int FS::FSReadCluster( PBYTE pBuf, int size, int whichCluster, int begPosInCluster ){
	assert ( whichCluster >= 2 );
	assert ( begPosInCluster >= 0 && size > 0 );
	assert ( pBuf != NULL );

	whichCluster -= 2;
	unsigned long readBytes = 0;
	int baseLine = (1 + 9 + 9 + 14)*512;
	if ( begPosInCluster + size > 512 * SECTOR_PER_CLUSTER ){
		size =  512 * SECTOR_PER_CLUSTER - begPosInCluster ;
	}

	::SetFilePointer( this->hFileOnDisk,
							baseLine + whichCluster*(512* SECTOR_PER_CLUSTER ) + begPosInCluster  , 
							NULL,
							FILE_BEGIN
					);
	::ReadFile( this->hFileOnDisk, (void *)pBuf, size,  &readBytes, NULL );

	return readBytes;

}

/***************************************************************
��pBuf�еĴ�СΪsize������д��Content����whichCluster�е���begPosInCluster��λ����ȥ
�ڽ���˺���ǰ�� ��֤����Խ��������������ļ�
******************************************************************/
int FS::FSWriteCluster( PBYTE pBuf, int size, int whichCluster, int begPosInCluster ){
	assert ( pBuf != NULL );
	assert ( whichCluster >= 2 );
	assert ( begPosInCluster >= 0 && size > 0 );

	whichCluster -= 2;

	unsigned long writeBytes = 0;
	int baseLine = (1 + 9 + 9 + 14)*512;
	if ( begPosInCluster + size > CLUSTER_SIZE ){
		size =  CLUSTER_SIZE - begPosInCluster ;
	}
	::SetFilePointer( this->hFileOnDisk,
						baseLine + whichCluster*(CLUSTER_SIZE) + begPosInCluster  , 
						NULL,
						FILE_BEGIN
					);
	::WriteFile( this->hFileOnDisk, (void *)pBuf, size,  &writeBytes, NULL );

	return writeBytes;
}

/*******************************************************

*******************************************************/


/*************************************************************************
*��FAT16���У� 0x0000��ʾδ��ռ�ã� 0xffff��ʾһ��FAT16���Ľ���.
****************************************************************************/
WORD FS::AllocClusterFromFAT16(WORD curLastCluster ){
	if( curLastCluster == 0 ){//����һ���ļ��ĵ�һ�������
		for( int i = 2 ; i < 512 * 9/2; i ++){
			if( *(this->pFAT16+i) == (WORD)0x0000 ){
				*(this->pFAT16+i) = (WORD)0xffff;
				return i;
			}
		}
		return -1;//��ʾû������ɹ�
	}else{
		WORD *  pTail = this->pFAT16 + curLastCluster;
		for( int i = 2; i < 512 * 9/2; i ++){
			if( *(this->pFAT16+i) == (WORD)0x0000 ){
				*(pTail) = (WORD)i;
				*(this->pFAT16+i) = (WORD)0xffff;
				return i;
			}
		}
		return -1;

	}

}

/******************************************************************
����һ���ļ��Ĵ�ͷ�����������ļ���ռ�õĴش�FAT16�������
********************************************************************/
void FS::FreeClusterFromFAT16(WORD head)
{
	while( head != WORD(0xffff) ) {
		WORD tmp = pFAT16[head] ;
		pFAT16[head] =(WORD) 0x0000;
		head = tmp;
	}

}

/*************************************************
		<�� �Ǹ�Ŀ¼ �����ļ���>
����:   pDir:ENTRY*      ------���Ǹ�Ŀ¼����
		dirName:char*   ------���ҵ��ļ���
���:   ������ڵ�ǰĿ¼���ҵ����Ŀ¼, ����һ��32�ֽڵ�ENTRY 
		û���ҵ��� �򷵻�NULL

		!!!!!!���ص�ENTRY ���ڸú����ڲ�new ������!!!!!
6.29
**********************************************************************/
ENTRY * FS::InDirFindDir(ENTRY *pDir, char * dirName, WORD * inWhichCluster, int * offset )
{
	assert( pDir != NULL );
	assert( pDir->startCluster != 0 );
	assert( dirName != NULL && strlen(dirName) <= 8 );


	WORD curCluster = pDir->startCluster;
	BYTE  * pClusterPool = new BYTE[ CLUSTER_SIZE];
	ENTRY * pSearchEntry = (ENTRY *)pClusterPool;
	do{
		this->FSReadCluster( pClusterPool, CLUSTER_SIZE, curCluster, 0);
		for( int i = 0; i < CLUSTER_SIZE/sizeof(ENTRY); i ++){
			if(	(WORD)0x0000 != *(WORD*)(pSearchEntry + i )
				&&
				SUBDIRECTORY == (pSearchEntry + i )->type
				&&
				strcmp( (pSearchEntry + i )->fileMainName, 	dirName ) == 0
			  )
			{
				ENTRY * pResult = new ENTRY();
				::memcpy( (void*)pResult, (void*)(pSearchEntry + i ), sizeof(ENTRY));
				if( inWhichCluster ){ 
					*inWhichCluster = curCluster;
				}
				if( offset ){
					*offset = i*sizeof(ENTRY);
				}

				delete [] pClusterPool; pClusterPool = NULL;
				return pResult;
			}
		}

		curCluster = pFAT16[curCluster];
	}while( curCluster != WORD(0xffff) );
	
	delete [] pClusterPool; pClusterPool = NULL;
	return NULL;

}

/**********************************************************************
			<�� �Ǹ�Ŀ¼ �����ļ�>��
����:   pDir:ENTRY*      ------���Ǹ�Ŀ¼����
		fileName:char*   ------���ҵ��ļ���
���:   ������ڵ�ǰĿ¼���ҵ����Ŀ¼, ����һ��32�ֽڵ�ENTRY 
		û���ҵ��� �򷵻�NULL

		!!!!!!���ص�ENTRY ���ڸú����ڲ�new ������!!!!!
6.29
**********************************************************************/
ENTRY * FS::InDirFindFile( ENTRY * pDir, char * fileName, WORD * inWhichCluster, int * offset ){
	assert( pDir != NULL );
	assert( pDir->startCluster != 0 );
	

	char * fileMainName = this->GetFileMainName( fileName );
	char * extension    = this->GetFileExtensionName( fileName );

	WORD curCluster = pDir->startCluster;
	BYTE  * pClusterPool = new BYTE[ CLUSTER_SIZE];
	ENTRY * pSearchEntry = (ENTRY *)pClusterPool;
	do{
		this->FSReadCluster( pClusterPool, CLUSTER_SIZE, curCluster, 0);
		for( int i = 0; i < CLUSTER_SIZE/sizeof(ENTRY); i ++){
			if( (WORD)0x0000  != *(WORD*)(pSearchEntry + i )
				&&
				SUBDIRECTORY  != (pSearchEntry + i )->type
				&&
				strcmp( (pSearchEntry + i )->fileMainName, 	fileMainName ) == 0
				&& 
				strcmp( (pSearchEntry + i )->extension , extension ) == 0
			  )
			{
				ENTRY * pResult = new ENTRY();
				::memcpy( (void*)pResult, (void*)(pSearchEntry + i ), sizeof(ENTRY));
				if( inWhichCluster ){ 
					*inWhichCluster = curCluster;
				}
				if( offset ){
					*offset = i*sizeof(ENTRY);
				}
				delete [] pClusterPool;pClusterPool = NULL;
				return pResult;
			}
		}

		curCluster = pFAT16[curCluster];
	}while( curCluster != WORD(0xffff) );
	
	delete [] pClusterPool; pClusterPool = NULL;
	return NULL;
}


/*********************************************************
�����ļ���������������Ŀ¼���������ڵĴأ���������еݹ���ù���
����ֵ��-1��ʾ���ļ�����Ŀ¼Ϊ��Ŀ¼
		��Ϊ-1, ��ʾ���Ǹ�Ŀ¼��
**********************************************************/
int FS::FindDirCluster( char * fileName, int cluster){
	return 0;
}



void FS::SetFileTime(FILE_TIME *ft)
{
	assert( ft != NULL );
	SYSTEMTIME st;
	::GetLocalTime( &st );
	ft->wYear   = st.wYear;
	ft->wMonth  = st.wMonth;
	ft->wDay    = st.wDay;
	ft->wHour   = st.wHour;
	ft->wMinute = st.wMinute;
	ft->wSecond = st.wSecond;
}

/**********************************************************************

************************************************************************/
ENTRY * FS::FSCreateDir(char *dirName)
{
	assert (dirName != NULL );
	assert ( strlen(dirName ) <= 8 );
	
    //////////////////////////////////////////////////////
	//ENTRY.fileMainName���������ļ�����˵�����ļ��е����֡�

	WORD startCluster = this->curWorkDir->pFileEntry->startCluster;
	if( startCluster == 0 ){// ˵�����ڵ�Ŀ¼Ϊ��Ŀ¼������û�дصĸ��
		//first make sure there is not such dir in curWorkDir
		ENTRY * pTest ; 
		if( (pTest = InRootFindDir( dirName ) ) != NULL ) {
			cout<<"the dir "<<dirName<<" alread exist"<<endl;
			cout<<"You can not create it in current directory"<<endl;
			return NULL;
		}

		ENTRY * pSearchEntry = this->pROOTDIR;
			//����һ����е�ENTRY���� ȥ��䡣
			for(int i = 0 ;  i < 14*512/sizeof(ENTRY); i ++ ){
				if( *(WORD*)(pSearchEntry ) == (WORD)0x0000 ){
					break;
				}else{
					pSearchEntry ++;
				}
			}
			if( i >= 14*512/sizeof(ENTRY)  ){
				cout<<"root directory is full, can not create file in root directory!!"<<endl;
				//��ʾ����û�п��и�Ŀ¼�ռ���
				return NULL;
			}
			//�ҵ�һ����е�entry�ռ䣮
			pSearchEntry->startCluster = this->AllocClusterFromFAT16(0);
			SetFileTime( &pSearchEntry->ft);
			pSearchEntry->type = SUBDIRECTORY;
			pSearchEntry->sizeOfFile = 0;
			memcpy( pSearchEntry->fileMainName, dirName, 9 );
			memcpy( pSearchEntry->extension   , "\0\0\0\0", 4);
			cout<<dirName<<" has been create !"<<endl; 
			return pSearchEntry;
	}else{//˵�����ڵ�Ŀ¼���Ǹ�Ŀ¼�����������ڴ���
		ENTRY * pTest ; 
		if( (pTest = InDirFindDir( curWorkDir->pFileEntry, dirName , NULL, NULL )) != NULL ) {
			cout<<"the dir "<<dirName<<" alread exist"<<endl;
			cout<<"You can not create it in current directory"<<endl;
			return NULL;
		}
			ENTRY * pClusterPool = (ENTRY*)new BYTE[CLUSTER_SIZE];
			WORD curCluster   = startCluster;
			int i;
			do{
				this->FSReadCluster((PBYTE) pClusterPool, CLUSTER_SIZE, curCluster, 0);
				for( i = 0; i < CLUSTER_SIZE/sizeof(ENTRY); i ++){
					if( *(WORD*)(pClusterPool + i ) == (WORD)0x0000 ){
						goto _OUT;
					}
				}
				
				curCluster = pFAT16[curCluster];
			}while( curCluster != WORD(0xffff) );
		
			//����ܹ����е���һ���� ���ʾ���Ŀ¼�����ڵĴ�û���㹻�Ŀռ�, ��Ҫ������һ��.
			curCluster = startCluster;
			while( pFAT16[curCluster] != WORD(0xffff)){
				curCluster = pFAT16[curCluster];
			}
			curCluster = this->AllocClusterFromFAT16( curCluster );
						
	   _OUT:
			ENTRY *pEntry = new ENTRY();
			ENTRY & entry = *pEntry;
			SetFileTime( &entry.ft);
			entry.sizeOfFile = 0;
			entry.type = SUBDIRECTORY;
			memcpy( entry.fileMainName, dirName, 9 );
			memcpy( entry.extension   , "\0\0\0\0", 4);
		    entry.startCluster = this->AllocClusterFromFAT16(0);
			FSWriteCluster ((PBYTE)&entry, sizeof(ENTRY), curCluster, i*sizeof(ENTRY)%512);
			
			delete [] pClusterPool;
			cout<<dirName<<" has been create !"<<endl; 
			return pEntry;

	}

}

/***********************************************************************
�ڵ�ǰ�Ĺ���Ŀ¼�д���һ���ļ���
***********************************************************************/
ENTRY * FS::FSCreateFile( char * fileName, BYTE type ){
	if(  CheckFileNameFormat( fileName ) == false ){
		cout<<"the input fileName: "<<fileName<<" does not apply to "
			<<"the 8.3 rule"<<endl;
		return NULL;
	}
	      
  	   //////////////////////////////////////////////////////
		char * fileMainName = this->GetFileMainName(fileName );
		char * fileExtensionName = this->GetFileExtensionName( fileName);
		////////////////////////////////////////////////////////////////////

		WORD startCluster = this->curWorkDir->pFileEntry->startCluster;
		if( startCluster == 0 ){// ˵���ļ����ڵ�Ŀ¼Ϊ��Ŀ¼������û�дصĸ��
				ENTRY * pTest ; 
				if( (pTest = InRootFindFile( fileName )) != NULL ) {
					cout<<"the file "<<fileName<<" alread exist"<<endl;
					cout<<"You can not create it in current directory"<<endl;
					return NULL;
				}
			ENTRY * pSearchEntry = this->pROOTDIR;
			//����һ����е�ENTRY���� ȥ��䡣
			for(int i = 0 ;  i < 14*512/sizeof(ENTRY); i ++ ){
				if( *(WORD*)(pSearchEntry ) == (WORD)0x0000 ){
					break;
				}else{
					pSearchEntry ++;
				}
			}
			if( i >= 14*512/sizeof(ENTRY)  ){
				cout<<"root directory is full, can not create file in root directory!!"<<endl;
				//��ʾ����û�п��и�Ŀ¼�ռ���
				return NULL;
			}
			//�ҵ�һ����е�entry�ռ䣮
			pSearchEntry->startCluster = this->AllocClusterFromFAT16(0);
			SetFileTime( &pSearchEntry->ft);
			pSearchEntry->type = type;
			pSearchEntry->sizeOfFile = 0;
			memcpy( pSearchEntry->fileMainName, fileMainName, 9 );
			memcpy( pSearchEntry->extension   , fileExtensionName, 4);
			cout<<fileName<<" has been create !"<<endl; 
			return pSearchEntry;
		}else{//˵���ļ����ڵ�Ŀ¼���Ǹ�Ŀ¼�����������ڴ���
			ENTRY * pTest ; 
			if( (pTest = InDirFindFile(curWorkDir->pFileEntry,  fileName , NULL, NULL )) != NULL ) {
				cout<<"the dir "<<fileName<<" alread exist"<<endl;
				cout<<"You can not create it in current directory"<<endl;
				return NULL;
			}
			ENTRY * pClusterPool = (ENTRY*)new BYTE[CLUSTER_SIZE];
			WORD curCluster   = startCluster;
			int i;
			do{
				this->FSReadCluster((PBYTE) pClusterPool, CLUSTER_SIZE, curCluster, 0);
				for( i = 0; i < CLUSTER_SIZE/sizeof(ENTRY); i ++){
					if( *(WORD*)(pClusterPool + i ) == (WORD)0x0000 ){
						goto _OUT;
					}
				}
				
				curCluster = pFAT16[curCluster];
			}while( curCluster != WORD(0xffff) );
		
			//����ܹ����е���һ���� ���ʾ���Ŀ¼�����ڵĴ�û���㹻�Ŀռ�, ��Ҫ������һ��.
			curCluster = startCluster;
			while( pFAT16[curCluster] != WORD(0xffff)){
				curCluster = pFAT16[curCluster];
			}
			curCluster = this->AllocClusterFromFAT16( curCluster );
						
	   _OUT:
			ENTRY *pEntry = new ENTRY();
			ENTRY & entry = *pEntry;
			SetFileTime( &entry.ft);
			entry.sizeOfFile = 0;
			entry.type = type;
			memcpy( entry.fileMainName, fileMainName, 9 );
			memcpy( entry.extension, fileExtensionName, 4);
			entry.startCluster = this->AllocClusterFromFAT16(0);
			FSWriteCluster ((PBYTE)&entry, sizeof(ENTRY), curCluster, i*sizeof(ENTRY)%512);
			
			delete [] pClusterPool;
			cout<<fileName<<" has been create !"<<endl; 
			return pEntry;
		}

				
}

/*************************************************************************************
��src��ָ����ļ�����д���ļ���ȥ
**************************************************************************************/
bool FS::FSFillFile(char *fileName, char * src )
{

	ENTRY * pSearchEntry = this->FindFile( fileName );
	if( pSearchEntry == NULL ){
		cout<<"Such File: "<<fileName<<" Not Found In Current Directory!"<<endl;
		return false;
	}
	int srcLen = strlen( src );
	pSearchEntry->sizeOfFile += srcLen;
	if( srcLen <= CLUSTER_SIZE ){
		this->FSWriteCluster( (BYTE*)src, CLUSTER_SIZE, pSearchEntry->startCluster, 0 );
	}else {
		WORD curCluster = pSearchEntry->startCluster;
		do{
			this->FSWriteCluster( (BYTE*)src, CLUSTER_SIZE, curCluster, 0 );
			srcLen -= CLUSTER_SIZE;
			
		}while( srcLen > 0 && (curCluster = this->AllocClusterFromFAT16( curCluster )));
	}
	return true;
}

bool FS::FSDeleteFile(char * fileName)
{
	if(  CheckFileNameFormat( fileName ) == false ){
		cout<<"the input fileName: "<<fileName<<" does not apply to "
			<<"the 8.3 rule"<<endl;
		return false;
	}

	ENTRY * pFileEntry = NULL;
	if( fullPath.size() == 1 ){//��ǰΪ��Ŀ¼
		pFileEntry = this->InRootFindFile( fileName );
	}else if ( fullPath.size() > 1 ){
		pFileEntry = this->InDirFindFile(curWorkDir->pFileEntry, fileName, NULL, NULL );
	}
	if( pFileEntry == NULL ){
		cout<<"such file: '"<<fileName<<"' not exist in current directory!"<<endl;
		return false ;
	}else{
		return  FSDelete(curWorkDir->pFileEntry, pFileEntry ) ;
	}

}

bool FS::FSDeleteDir(char * dirName)
{
	if(  CheckDirNameFormat( dirName ) == false ){
		cout<<"the Directory's Name: "<<dirName<<" is not apply to the naming rule!!"<<endl;
		return false;
	}

	ENTRY * pFileEntry = NULL;
	if( fullPath.size() == 1 ){//��ǰΪ��Ŀ¼
		pFileEntry = this->InRootFindDir( dirName );
	}else if ( fullPath.size() > 1 ){
		pFileEntry = this->InDirFindDir(curWorkDir->pFileEntry, dirName, NULL, NULL );
	}

	if( pFileEntry == NULL ){
		cout<<"such directory: '"<<dirName<<"' not exist in current directory!"<<endl;
		return false ;
	}else{
		return  FSDelete(curWorkDir->pFileEntry, pFileEntry ) ;
	}

}

/**************************************************************************

***************************************************************************/
bool FS::FSDelete(ENTRY * pDir,  ENTRY * pFileEntry ){
		
	////////////////////////////////////////////////////////
	ENTRY * pSearchEntry = 	pFileEntry;
	////////////////////////////////////////////////////////
	if( pSearchEntry >= pROOTDIR
		&&
		pSearchEntry < pROOTDIR + 14*512/sizeof(ENTRY) 
	  ){//��ʾ���ָ������PROOTDIR�ķ�Χ��
		//***************************�Ƚ�FAT16���и��ļ���ռ�Ŀ����
		FreeClusterFromFAT16( pSearchEntry->startCluster );
		cout<<pFileEntry->fileMainName<<pFileEntry->extension<<" has been delete\n";
		memset((void*) pSearchEntry, 0x00, sizeof(ENTRY) );
	}else{//��ʾ���ENTRY���ڴ����У�����û�����ڴ��У�
		if ( SUBDIRECTORY == pSearchEntry->type ){//��Ŀ¼.
			WORD whichCluster; int offset;
			if( (pSearchEntry = this->InDirFindDir( pDir, pSearchEntry->fileMainName, &whichCluster, &offset) ) == NULL ){
				cout<<"not found the file "<<endl;
				return false;
			}else {

				cout<<pSearchEntry->fileMainName<<pSearchEntry->extension<<" has been delete\n";
				FreeClusterFromFAT16( pSearchEntry->startCluster );
				memset( (void*) pSearchEntry, 0x00, sizeof(ENTRY) );
				FSWriteCluster( (BYTE*)pSearchEntry, sizeof( ENTRY),  whichCluster, offset);
			}

		}else{//���ļ�
			WORD  whichCluster; int  offset;
			char fileName[13];
			memset( fileName, 0x0000, 13);
			strcat( fileName, pSearchEntry->fileMainName);
			strcat( fileName, "." );
			strcat( fileName, pSearchEntry->extension );
			if( (pSearchEntry = this->InDirFindFile( pDir, fileName, &whichCluster, &offset) ) == NULL ){
				cout<<"not found the file "<<endl;
				return false;
			}else{
				cout<<pSearchEntry->fileMainName<<"."<<pSearchEntry->extension<<" has been delete\n";
				FreeClusterFromFAT16( pSearchEntry->startCluster );
				memset( (void*) pSearchEntry, 0x00,   sizeof(ENTRY) );
				FSWriteCluster( (BYTE*)pSearchEntry,  sizeof( ENTRY),  whichCluster, offset);
			}

		}

	}
	flush();
	return true;

}

/*********************************************************************************
		��pDirĿ¼���ҵ�fileName����ļ�, �����ظ��ļ�����Ӧ��ENTRY, 
		��Ҫ�����ļ��ӵ�openedFileList��

**********************************************************************************/
File * FS::FSOpen( ENTRY * pDir , char * fileName, int openMode){
	ENTRY *pResultEntry = this->InDirFindFile( pDir, fileName , NULL, NULL);
	if( pResultEntry == NULL ){//��ʾû���ҵ����ļ�
		cout<<"in FS::FSOpen(): can not find the file:"<<fileName
			<<" in this directory: "<<pDir->fileMainName<<endl;
		return NULL;
	}else {
		File * pResultFile = (File *) malloc( sizeof(File) );
		pResultFile->pFileEntry = pResultEntry;
		pResultFile->openMode = openMode;
		pResultFile->curPos = 0;
		
		this->openedFileNum ++;
		this->openedFileList.push_back( pResultFile);

		return pResultFile;
	}
}

/************************************************************************
		���Ѵ򿪵��ļ������ҵ�File * file ��ָ����Ǹ��ļ�, Ȼ������ر�
����ֵ: true-----------��ʾ�ҵ�, ���ر��������ļ�
		false----------��ʾû���ҵ�������ļ�.
*************************************************************************/
bool FS::FSClose(  File * file ){

	List<File*>::iterator iter = openedFileList.find( &file );
	if( iter == NULL ){
		cout<<"In FS::FSClose() : the file "<<file<<" you want to close not found !";
		return false;
	}
	this->openedFileList.remove( file );
	this->openedFileNum --;
	
	return false;	
}

bool FS::CheckDirNameFormat(char *dirName)
{
	assert( dirName != NULL );
	char * pDest = strchr( dirName, '.' );
	int len      = strlen( dirName );

	if( len > 8 ) return false;
	if( pDest != NULL ) return false;

	return true;

}

/********************************************************
	���� XXXXXXXX.YYY ��ʽ���ļ���
***********************************************************/
bool FS::CheckFileNameFormat( char * fileName ){
	assert( fileName != NULL );
	char * pDest = strchr( fileName, '.' );
	int len      = strlen( fileName );

	if( len > 12 ) return false;
	if( pDest == NULL ) return false;
	if( pDest - fileName > 8 ) return false;
	if( (fileName+len) - pDest  > 4) return false;

	return true;
	
}

/************************************************
���� XXXXXXXX.YYY ��ʽ���ļ���
*************************************************/
char * FS::GetFileMainName(char * fileName ){

	char * pMainFileName = new char [9];
	memset( pMainFileName, 0x00, 9);
	for( int i = 0 ;i <= 8 ; i ++){
		if( fileName[i] =='.' ) break;
		pMainFileName[i] = fileName[i];
	}
//	pMainFileName[i] = '\0';
	return pMainFileName;
}

/************************************************
���� XXXXXXXX.YYY ��ʽ���ļ���
*************************************************/
char * FS::GetFileExtensionName(char *  fileName){

	char * extension = new char [4];
	memset( extension, 0x00, 4);
	char * pDest = strchr(fileName, '.' );
	int i = 1;
	do{
		extension[i-1] = pDest[i];
		i++;
	}while( i < 4 && pDest[i] != '\0' );
//	extension[i] = '\0';
	return extension;

}




FS::~FS()
{

	flush();

}

/*************************************************************************
	��ν���ڴ����, �ڴ�д��, ��ָ�Դ������ڴ��е�root record, FAT16_1, FAT16_2, 
		ROOT directory ���ж���д
***************************************************************************/
int FS::MemWriteSector(PBYTE pBuf, int size, int whichSector, int begPosInSector)
{
	PBYTE pCfg = this->pFSConfig;
	if( begPosInSector + size > 512 ) {
		size = 512 - begPosInSector  ;
	}

	::memcpy( pCfg + whichSector*512 + begPosInSector, pBuf, size );
	
	return size;
}

/*************************************************************************
	��ν���ڴ����, �ڴ�д��, ��ָ�Դ������ڴ��е�root record, FAT16_1, FAT16_2, 
		ROOT directory ���ж���д
***************************************************************************/
int FS::MemReadSector(PBYTE pBuf, int size, int whichSector, int begPosInSector)
{
	PBYTE pCfg = this->pFSConfig;
	if( begPosInSector + size > 512) {
		size = 512 - begPosInSector  ;
	}

	::memcpy( pBuf, pCfg + whichSector*512 + begPosInSector,  size );
	return size;
	
}

/*************************************************************************
���������ڴ��е�boot record, FAT16_1, FAT16_2, ROOT directory д�ش�����ȥ
6.28
**************************************************************************/
void FS::flush()
{
	for(int i = 0 ; i < 1 + 9 + 9 + 14; i ++){
		this->FSWriteSector( pFSConfig + i * 512, 512, i, 0 );
	}
	
}

/***************************************************************************

 ������   dirName:char*, ��ʾҪ�ӵ�ǰ����Ŀ¼�����Ŀ¼
����ֵ:  true---------�ɹ��������dirNameָ����Ŀ¼
         false--------�޷������Ŀ¼, ������ָ����Ŀ¼�������ڵ�ǰ����Ŀ¼
6.29
******************************************************************************/
bool FS::FSChangeWorkDir(char * dirName)
{
	ENTRY * pTmp;

	if( curWorkDir->pFileEntry->startCluster == 0 ){// ��ǰ�Ĺ���Ŀ¼Ϊ��Ŀ¼
		if( strcmp( dirName, "..") == 0 ){//�Ǹ�Ŀ¼�� �������������ˡ�
			cout<<"can not go to upper director, this is root directory"<<endl;
			return false;
		}else{
			if( (pTmp = this->InRootFindDir( dirName ) )== NULL ){
				cout<<"such directory "<<dirName<<" does not exist!"<<endl;
				return false;
			}else {
				curWorkDir->pFileEntry = pTmp;
				fullPathName.push_back( curWorkDir->pFileEntry->fileMainName);
				fullPath.push_back    ( curWorkDir->pFileEntry );
				return true;
			}
		}
		
	}else {//��ʾ������root directory�в���.
		if ( strcmp( dirName, "..") == 0 ){
			fullPathName.pop_back();
			fullPath.pop_back();
			curWorkDir->pFileEntry = fullPath.back();
			return true;
		}else{
			if ( (pTmp = this->InDirFindDir( curWorkDir->pFileEntry, dirName, NULL, NULL)) == NULL ){
				cout<<"such directory "<<dirName<<" does not exist!"<<endl;
				return false;
			}else{
				curWorkDir->pFileEntry = pTmp;
				fullPathName.push_back( pTmp->fileMainName);
				fullPath.push_back    ( curWorkDir->pFileEntry );
				return true;	
			}
		}
	}

}

/****************************************************************8
		<�ڸ�Ŀ¼����һ��ָ�����ļ�>
����:   fileName:char * -------------Ҫ�ҵ��ļ���

���:    �ļ�����32�ֽڵ�ENTRY
		 ����NULL��ʾû���ҵ����ļ�
	    !!!!!!!!!���صĶ����ڸ��ļ���PROOTDIR�еĶ�ӦENTRY��ָ�룬 
		û��new ENTRY
  6��29
******************************************************************/
ENTRY * FS::InRootFindFile(char *fileName)
{
	char * fileMainName = this->GetFileMainName( fileName );
	char * extension    = this->GetFileExtensionName( fileName );

	ENTRY * pSearchEntry = this->pROOTDIR;
	for( int i = 0; i < 512*14/sizeof(ENTRY); i ++){
		if( *(WORD*)(pSearchEntry+i) != (WORD)0x0000
			&&
			(pSearchEntry + i )->type != SUBDIRECTORY 
			&&
			strcmp( (pSearchEntry + i )->fileMainName, fileMainName ) == 0
			&&
			strcmp( (pSearchEntry + i )->extension, extension ) == 0
		  )
		{
			return (pSearchEntry + i) ;
		}	
	}
	return NULL; //means not found;
}

/****************************************************************8
		<�ڸ�Ŀ¼����һ��ָ����Ŀ¼>
����:   dirName:char * -------------Ҫ�ҵ��ļ���

���:    �ļ�����32�ֽڵ�ENTRY
		 ����NULL��ʾû���ҵ����ļ�
	    !!!!!!!!!���صĶ����ڸ��ļ���PROOTDIR�еĶ�ӦENTRY��ָ�룬 
		û��new ENTRY
  6��29
******************************************************************/
ENTRY * FS::InRootFindDir(char *dirName)
{
	assert( dirName != NULL && strlen(dirName) <= 8 );

	ENTRY * pSearchEntry = this->pROOTDIR;

	for( int i = 0; i < 512*14/sizeof(ENTRY) ; i ++){
			if( *(WORD*)(pSearchEntry+i) != (WORD)0x0000
				&&
				(pSearchEntry+i)->type == SUBDIRECTORY
				&&
				strcmp( (pSearchEntry + i )->fileMainName, 	dirName ) == 0 
			  )
			{
				return (pSearchEntry + i );
			}
	}
	return NULL;//means not found!!
}


void FS::PrintFullPathName()
{
	List<char*>::iterator iter = fullPathName.begin();
	List<char*>::iterator iter_end = fullPathName.end();
	cout<<"/";
	iter ++;
	for(; iter != iter_end; iter  ++ ){
		cout<<*iter<<"/";
	}
	cout<<endl;

}

void FS::PrintCurWorkDirList()
{
	ENTRY * pSearchEntry = NULL;
	if ( curWorkDir->pFileEntry->startCluster == 0 	){//Ϊ��Ŀ¼
		cout<<"\n\rDirectory of Root \\"<<endl<<endl;
		cout<<"Time\t\t\t"<<"Attr  "<<"Clusters "<<"size\t"<<"Name"<<endl;
		pSearchEntry = this->pROOTDIR;
		for( int i = 0; i < 512*14/sizeof(ENTRY) ; i ++ , pSearchEntry ++){
				if( *(WORD*)(pSearchEntry) != (WORD)0x0000 )
				{
						cout<<pSearchEntry->ft.wYear<<"-"<<pSearchEntry->ft.wMonth<<"-"<<pSearchEntry->ft.wDay<<" "
							<<pSearchEntry->ft.wHour<<":"<<pSearchEntry->ft.wMinute<<":"<<pSearchEntry->ft.wSecond;
					if( pSearchEntry->type == SUBDIRECTORY ) {	
						cout<<"\t"<<"<DIR> \t[";PrintFAT16Usage( pSearchEntry->startCluster );
						cout<<"]\t"<<pSearchEntry->sizeOfFile<<"\t"<<pSearchEntry->fileMainName<<endl;
					}else{
						cout<<"\t"<<"<FILE>\t[";PrintFAT16Usage( pSearchEntry->startCluster );
						cout<<"]\t"<<pSearchEntry->sizeOfFile<<"\t"<<pSearchEntry->fileMainName<<"."<<pSearchEntry->extension<<endl;
					}

				}
		}
	}else{ //���Ǹ�Ŀ¼��
		WORD curCluster = curWorkDir->pFileEntry->startCluster;
		BYTE  * pClusterPool = new BYTE[ CLUSTER_SIZE];
		cout<<"\n\rDirectory of "<<curWorkDir->pFileEntry->fileMainName<<"\\"<<endl<<endl;
		cout<<"Time\t\t\t"<<"Attr "<<"Clusters "<<"size\t"<<"Name"<<endl;
		do{
			pSearchEntry = (ENTRY *)pClusterPool;
			this->FSReadCluster( pClusterPool, CLUSTER_SIZE, curCluster, 0);
			for( int i = 0; i < CLUSTER_SIZE/sizeof(ENTRY); i ++, pSearchEntry ++ ){
				if(	(WORD)0x0000 != *(WORD*)(pSearchEntry ) )
				{
						cout<<pSearchEntry->ft.wYear<<"-"<<pSearchEntry->ft.wMonth<<"-"<<pSearchEntry->ft.wDay<<" "
							<<pSearchEntry->ft.wHour<<":"<<pSearchEntry->ft.wMinute<<":"<<pSearchEntry->ft.wSecond;
					if( pSearchEntry->type == SUBDIRECTORY ) {	
						cout<<"\t"<<"<DIR> \t[";PrintFAT16Usage( pSearchEntry->startCluster );
						cout<<"]\t"<<pSearchEntry->sizeOfFile<<"\t"<<pSearchEntry->fileMainName<<endl;
					}else{
						cout<<"\t"<<"<FILE>\t[";PrintFAT16Usage( pSearchEntry->startCluster );
						cout<<"]\t"<<pSearchEntry->sizeOfFile<<"\t"<<pSearchEntry->fileMainName<<"."<<pSearchEntry->extension<<endl;
					}
				}
			}

			curCluster = pFAT16[curCluster];
		}while( curCluster != WORD(0xffff) );
		
		delete [] pClusterPool; pClusterPool = NULL;
	}
	cout<<endl<<endl;



}


void FS::PrintFAT16Usage(WORD head )
{
	if( head == 0 ){
		cout<<"The Disk's such cluster has been used:"<<endl;
		for( int i = 2; i < 9*512/2; i ++ ){
			if(*(pFAT16+i) != 0x0000 ){
				cout<<i<<",";
			}
		}
		cout<<endl;
	}else {
		do{
			cout<<head<<",";
			head = pFAT16[head];
		}while( head != WORD(0xffff) );
	}


}

char * FS::GetCurWorkDirName()
{

	return fullPathName.back();


}

ENTRY * FS::FindFile(char *fileName)
{
	ENTRY * pSearchEntry = NULL;
	if( fullPath.size() == 1 ){
		pSearchEntry = this->InRootFindFile( fileName );
		
	}else{
		pSearchEntry = this->InDirFindFile( curWorkDir->pFileEntry, fileName, NULL, NULL );
		
	}
	return pSearchEntry;

}
