// ManageMemory.cpp: implementation of the ManageMemory class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include "ManageMemory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ManageMemory::ManageMemory()
{
	pMemory = NULL;
	pFileFCB = NULL;
	pFileFAT = NULL;
	pDataArea = NULL;
	
	InitMemory();
}

ManageMemory::~ManageMemory()
{
	this->DeleteMemory();
}



///////////
///////////
bool ManageMemory::AllocMemory()
{
	bool bRet = true;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	int len = sizeof(FileFCB)*pFileInfo->GetNumFCB()+sizeof(FileFAT)*pFileInfo->GetNumDataArea()+sizeof(DataArea)*pFileInfo->GetNumDataArea();
	this->pMemory = (char*)::malloc(len);
	if (this->pMemory == NULL)
		bRet = false;
	ZeroMemory(this->pMemory, len);
	return bRet;
}

void ManageMemory::DeleteMemory()
{
	if (pMemory != NULL)
		::free(this->pMemory);
	pMemory = NULL;
	pFileFCB = NULL;
	pFileFAT = NULL;
	pDataArea = NULL;
}

bool ManageMemory::InitMemory()
{
	bool bRet = true;
	//
	if (!this->AllocMemory())
		return false;
	this->CreateFCB();
	this->CreateFAT();
	this->CreateDataArea();	
	//
	return bRet;
}

bool ManageMemory::CreateFCB()
{
	bool bRet = true;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	this->pFileFCB = (FileFCB*)this->pMemory;
	if (pFileInfo->isFromFile())
	{
		//���ļ��н��г��Ի�
		FILE* hFile = ::fopen(pFileInfo->GetFileName(), "rb");
		if (hFile == NULL)
			return false;
		
		::fseek(hFile,sizeof(int)*2, SEEK_SET);
		::fread(this->pFileFCB, sizeof(FileFCB), pFileInfo->GetNumFCB(), hFile);
		::fclose(hFile);
	}
	else
	{
		ZeroMemory(this->pFileFCB, sizeof(FileFCB)*pFileInfo->GetNumFCB());
		FileFCB* pFCB = this->pFileFCB;
		for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
		{
			pFCB[i].StartID = FCB_UNUSED;
			pFCB[i].FileHandle = FCB_UNUSED;
			pFCB[i].ParentDirID = FCB_UNUSED;
		}
		
	}
	return bRet;
}

bool ManageMemory::CreateFAT()
{
	bool bRet = true;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	this->pFileFAT = (FileFAT*)(this->pMemory + sizeof(FileFCB)*pFileInfo->GetNumFCB());
	if (pFileInfo->isFromFile())
	{
		//���ļ��н��г��Ի�
		FILE* hFile = ::fopen(pFileInfo->GetFileName(), "rb");
		if (hFile == NULL)
			return false;
		
		::fseek(hFile,sizeof(int)*2 + sizeof(FileFCB)*pFileInfo->GetNumFCB(), SEEK_SET);
		::fread(this->pFileFAT, sizeof(FileFAT), pFileInfo->GetNumDataArea(), hFile);
		::fclose(hFile);
	}
	else
	{
		ZeroMemory(this->pFileFAT, sizeof(FileFAT)*pFileInfo->GetNumDataArea());
		FileFAT* pFAT = this->pFileFAT;
		for (int i = 0; i < pFileInfo->GetNumDataArea(); i++)
		{
			pFAT[i].bUsed = false;
			pFAT[i].ID = i;    //DataArea��0��ʼ����
			pFAT[i].LeaveNum = DATALENGTH;
			pFAT[i].NextID = FAT_NEXTEND;
		}
	}
	return bRet;
}


bool ManageMemory::CreateDataArea()
{
	bool bRet = true;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	this->pDataArea = (DataArea*)(this->pMemory + sizeof(FileFCB)*pFileInfo->GetNumFCB()+sizeof(FileFAT)*pFileInfo->GetNumDataArea());
	if (pFileInfo->isFromFile())
	{
		//���ļ��н��г��Ի�
		FILE* hFile = ::fopen(pFileInfo->GetFileName(), "rb");
		if (hFile == NULL)
			return false;		
		::fseek(hFile,sizeof(int)*2 + sizeof(FileFCB)*pFileInfo->GetNumFCB()+sizeof(FileFAT)*pFileInfo->GetNumDataArea(), SEEK_SET);
		::fread(this->pDataArea, sizeof(DataArea), pFileInfo->GetNumDataArea(), hFile);
		::fclose(hFile);
	}
	else
	{
		ZeroMemory(this->pDataArea, sizeof(DataArea)*pFileInfo->GetNumDataArea());
	}
	return bRet;
}

FileFCB* ManageMemory::GetFileFCB()
{
	return this->pFileFCB;
}

FileFAT* ManageMemory::GetFileFAT()
{
	return this->pFileFAT;
}

DataArea* ManageMemory::GetDataArea()
{
	return this->pDataArea;
}

int ManageMemory::GetFreeFATID()
{
	
	int FreeID = FAT_FULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	FileFAT* pFAT = this->pFileFAT;
	for (int i = 0; i < pFileInfo->GetNumDataArea(); i++)
	{
		if (!pFAT[i].bUsed)
		{
			FreeID = i;
			break;
		}	
	}
	return FreeID;
}
int ManageMemory::GetFreeFCBID()
{
	int FreeID = FCB_FULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	FileFCB* pFCB = this->pFileFCB;
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFCB[i].FileHandle == FCB_UNUSED)
		{
			FreeID = i;
			break;
		}	
	}
	return FreeID;
}
bool ManageMemory::WirteFile(char* SaveName)
{
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();

	FILE* hFile = ::fopen(SaveName, "wb");
	if (hFile == NULL)
		return false;
		
	::fseek(hFile,0, SEEK_SET);
	int NumFCB = pFileInfo->GetNumFCB();
	int NumDataArea = pFileInfo->GetNumDataArea();
	//дFileSystemInfo�еĲ���
	::fwrite(&NumFCB,sizeof(int), 1, hFile);
	::fwrite(&NumDataArea,sizeof(int), 1, hFile);
	//дFCB
	::fwrite(pFileFCB, sizeof(FileFCB), pFileInfo->GetNumFCB(), hFile);
	//дFAT
	::fwrite(pFileFAT, sizeof(FileFAT), pFileInfo->GetNumDataArea(), hFile);
	//дDataArea
	::fwrite(pDataArea, sizeof(DataArea), pFileInfo->GetNumDataArea(), hFile);
	::fclose(hFile);
	return true;
}

int ManageMemory::GetNumFreeDataArea()
{
	int FreeNum = 0;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	for (int i = 0; i < pFileInfo->GetNumDataArea(); i++)
	{
		if (!pFileFAT[i].bUsed)
			FreeNum++;
	}
	return FreeNum;
}

int ManageMemory::GetNummFreeFCB()
{
	int FreeNum = 0;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFileFCB[i].FileHandle == FCB_UNUSED)
			FreeNum++;
	}
	return FreeNum;
}
/*
ManageMemory* ManageMemory::GetManageMemory()
{
static ManageMemory manage;
return &manage;
}
*/