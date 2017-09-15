// FileSystemInfo.h: interface for the FileSystemInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Global.h"
#pragma once

class FileSystemInfo  
{
public:
	FileSystemInfo();
	virtual ~FileSystemInfo();

	int GetNumFCB();
	void SetNumFCB(int NumFCB);

	int GetNumDataArea();
	void SetNumDataArea(int NumDataArea);

	char* GetFileName();
	void SetFileName(char* FileName);

	bool isFromFile();

	static FileSystemInfo* GetFileInfo();
private:	
	int NumFCB_;
	int NumDataArea_;    //这两个数字要最后写入文件的

	bool bFromFile_;
	char FileName_[128];

	
	void SetFromFile(bool bFromFile);

};
