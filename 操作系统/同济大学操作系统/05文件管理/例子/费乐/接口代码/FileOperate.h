// FileOperate.h: interface for the FileOperate class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ManageMemory.h"
#include <vector>
using namespace std;
#pragma once


class FileOperate  
{
public:
	FileOperate::FileOperate();
	FileOperate(ManageMemory* pMemory);
	virtual ~FileOperate();

private:
	ManageMemory* pMemory_;

public:
	void InitMemory();
	void SetMemory(ManageMemory* pMemory);

	bool CreateDIR(char* DirName);    //默认ParentDirID为MAINDIRID
	bool CreateDIR(char* DirName, int ParentDirID);
	bool CreateDIR(char* DirName, char* ParentName);
	bool DeleteDIR(char* DirName);
	bool ModifyDIR(const char* DirName, const char* newName, int newAccess = ACCESS_N);
	bool ShowDIR(char* DirName,vector<int>& vec);
	
	bool CreateFile_(const char* FileName, int ParentDirID);
	bool CreateFile_(const char* DirName);    //默认ParentDirID为MAINDIRID
	bool CreateFile_(const char* DirName, const char* ParentName);
	bool ModifyFile_(const char* FileName, const char* newName, int newAccess = ACCESS_N);
	int OpenFile_(const char* FileName);
	bool CloseFile_(int FileHandle);
	int WriteFile_(int FileHandle, const char* pSrc, int num);
	int ReadFile_(int FileHandle, char* pDest, int num);
	bool DeleteFile_(const char* FileName);
	int GetFileHandle(const char* FileName);
	
private:
	bool CreateMainDIR();
	int GetUniqueHandle();
	bool IsFileInDirectory(char* DirName);
	bool IsFileExist(const char* FileName);
	
};
