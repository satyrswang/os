// FileSystemInfo.cpp: implementation of the FileSystemInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "FileSystemInfo.h"
#include <windows.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FileSystemInfo::FileSystemInfo()
{
	this->NumFCB_ = 512;	
	this->NumDataArea_ = 2048;
	
	this->bFromFile_ = false;
	::ZeroMemory(this->FileName_, FILENAME);
}

FileSystemInfo::~FileSystemInfo()
{
	
}


int FileSystemInfo::GetNumFCB()
{
	return this->NumFCB_;
}

void FileSystemInfo::SetNumFCB(int NumFCB)
{
	this->NumFCB_ = NumFCB;
}

int FileSystemInfo::GetNumDataArea()
{
	return this->NumDataArea_;
}

void FileSystemInfo::SetNumDataArea(int NumDataArea)
{
	this->NumDataArea_ = NumDataArea;
}


bool FileSystemInfo::isFromFile()
{
	return this->bFromFile_;
}

void FileSystemInfo::SetFromFile(bool bFromFile)
{
	this->bFromFile_ = bFromFile;
}

char* FileSystemInfo::GetFileName()
{
	return this->FileName_;
}

void FileSystemInfo::SetFileName(char* FileName)
{
	::ZeroMemory(this->FileName_,FILENAME);
	if (FileName != NULL)
	{
		int NumFCB = 0;
		int NumFAT = 0;
		strcpy(this->FileName_, FileName);
		this->SetFromFile(true);
		FILE* hFile = ::fopen(this->GetFileName(), "rb");
		if (hFile == NULL)
			return ;
		::fread(&NumFCB, sizeof(int), 1, hFile);
		::fread(&NumFAT, sizeof(int), 1, hFile);
		this->NumFCB_ = NumFCB;
		this->NumDataArea_ = NumFAT;
		::fclose(hFile);
	}
	else
		this->SetFromFile(false);
}

FileSystemInfo* FileSystemInfo::GetFileInfo()
{
	static FileSystemInfo fileinfo;
	return &fileinfo;
}