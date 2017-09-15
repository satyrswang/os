// FS.h: interface for the FS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FS_H__9CF7502A_6045_45C6_BFF1_4CBB240AC217__INCLUDED_)
#define AFX_FS_H__9CF7502A_6045_45C6_BFF1_4CBB240AC217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef FS_H
	#define FS_H
	#include "MyList.h"
	#include <windows.h>
	#include "fs_macro.h"
	using namespace std;
	using namespace FS_STD;
	class File;
#endif


class FS  
{

//friend class File;
	friend class Shell;
private:
	//模拟层之下的数据结构
	HANDLE hFileOnDisk;
	char * onDiskFileName ;//
	//模拟层之上的数据结构
	BYTE *  pFSConfig;
	WORD *  pFAT16; 
	WORD *  pFAT16_2;//备分的那个FAT16表， 在很多地方， 并不用这个区域， 只是保留而已.
	ENTRY * pROOTDIR;
	
	List<char*> fullPathName;
	List<ENTRY *> fullPath;
	File * curWorkDir;  //文件系统现在活跃的文件， 当然包括子目录.
	short  openedFileNum;//已打开的文件数目
	List<File *> openedFileList;
public:
	ENTRY * FindFile( char * fileName );
	bool FSFillFile( char * fileName, char * src );
	char * GetCurWorkDirName();
	void PrintFAT16Usage(WORD head );
	bool FSDeleteDir( char * dirName );
	bool FSDeleteFile( char * fileName );
	void PrintCurWorkDirList();
	void PrintFullPathName();
	FS(char * onDiskFileName  );
	void FSFormat();

	ENTRY * FSCreateFile  ( char  * fileName, BYTE type );
	ENTRY * FSCreateDir   ( char  * dirName );

	bool    FSChangeWorkDir  ( char  * dirName );
	File  * FSOpen ( ENTRY * pDir, char * fileName, int openMode);
	bool    FSClose( File * file );
	void flush();
	void InitFSParameters();
	virtual ~FS();

private://base services, 在FS类的外部不可用
	int MemReadSector(  PBYTE pBuf, int size, int whichSector, int begPosInSector);
	int MemWriteSector( PBYTE pBuf, int size, int whichSector, int begPosInSector);

	int FSReadSector  ( PBYTE pBuf, int size, int whichSector,  int begPosInSector );
	int FSWriteSector ( PBYTE pBuf, int size, int whichSector,  int begPosInSector  );
	int FSReadCluster ( PBYTE pBuf, int size, int whichCluster, int begPosInCluster );
	int FSWriteCluster( PBYTE pBuf, int size, int whichCluster, int begPosInCluster );



private:
	bool    FSDelete( ENTRY * pFir,  ENTRY * pFileEntry );
	WORD AllocClusterFromFAT16    ( WORD lastCluster);
	void FreeClusterFromFAT16    ( WORD head );
	int FindDirCluster           ( char * fileName, int cluster);
	char * GetFileMainName       ( char * fileName );
	char * GetFileExtensionName  ( char *  fileName);
	bool CheckFileNameFormat     ( char * fileName );
private:
	bool CheckDirNameFormat( char * dirName );
	ENTRY * InRootFindDir ( char  * dirName );
	ENTRY * InRootFindFile( char  * fileName);
	ENTRY * InDirFindFile ( ENTRY * pDir , char * fileName, WORD * inWhichCluster, int * offset ); 
	ENTRY * InDirFindDir  ( ENTRY * pDir , char * dirName , WORD * inWhichCluster, int * offset );
	void SetFileTime( FILE_TIME * ft );
		
	

};

#endif // !defined(AFX_FS_H__9CF7502A_6045_45C6_BFF1_4CBB240AC217__INCLUDED_)
