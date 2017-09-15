// FileOperate.cpp: implementation of the FileOperate class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <windows.h>
#include "FileOperate.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FileOperate::FileOperate(ManageMemory* pMemory)
{
	this->SetMemory(pMemory);
}

FileOperate::FileOperate()
{
	this->pMemory_ = NULL;
}

FileOperate::~FileOperate()
{
	this->pMemory_ = NULL;
}


void FileOperate::SetMemory(ManageMemory* pMemory)
{
	this->pMemory_ = pMemory;	
}

int FileOperate::GetUniqueHandle()
{
	int handle = 0;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	FileFCB* pFCB = this->pMemory_->GetFileFCB();
	
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		handle = MaxValue(handle, pFCB[i].FileHandle);
	}
	return ++handle;
	
}

int FileOperate::GetFileHandle(const char* FileName)
{
	int handle = INVALID_HANDLE;
	
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	FileFCB* pFCB= this->pMemory_->GetFileFCB();
	
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (strcmp(FileName, pFCB[i].FileName) == 0)
		{
			handle = pFCB[i].FileHandle;
			break;
		}
	}
	return handle;
}

bool FileOperate::IsFileInDirectory(char* DirName)
{
	int DirHandle = INVALID_HANDLE;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	FileFCB* pFCB = this->pMemory_->GetFileFCB();
	DirHandle = this->GetFileHandle(DirName);
	
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFCB[i].ParentDirID == DirHandle)
			return true;
	}
	
	return false;
}

bool FileOperate::IsFileExist(const char* FileName)
{
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	FileFCB* pFCB = this->pMemory_->GetFileFCB();	
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (strcmp(FileName, pFCB[i].FileName) == 0)
			return true;
	}
	return false;
}

bool FileOperate::CreateDIR(char* DirName, int ParentDirID)
{
	//文件夹占一个DataArea
	//建立FCB,填充FCB结构
	//建立FAT,填充FAT结构
	bool bRet = true;
	int FreeFCBID = FCB_FULL;
	int FreeFATID = FAT_FULL;
	FileFCB* pFCB = NULL;
	FileFAT* pFAT = NULL;
	
	if (this->pMemory_ == NULL)
		return false;
	if (this->IsFileExist(DirName))
		return false;
	FreeFCBID = this->pMemory_->GetFreeFCBID();
	if (FreeFCBID == FCB_FULL)
		return false;
	FreeFATID = this->pMemory_->GetFreeFATID();
	if (FreeFATID == FAT_FULL)
		return false;
	
	//建立FCB
	pFCB = this->pMemory_->GetFileFCB();
	pFCB[FreeFCBID].Access = ACCESS_N;
	pFCB[FreeFCBID].FileHandle = this->GetUniqueHandle();
	pFCB[FreeFCBID].FileLength = 0;   //文件夹的文件大小为0
	strcpy(pFCB[FreeFCBID].FileName, DirName);
	pFCB[FreeFCBID].NumDataArea = 1;  //占的DataArea为1
	pFCB[FreeFCBID].ParentDirID = ParentDirID;
	pFCB[FreeFCBID].StartID = FreeFATID;
	pFCB[FreeFCBID].Type = TYPE_D;
	::ZeroMemory(pFCB[FreeFCBID].UserName, USERNAME); 
	
	//建立FAT
	pFAT = this->pMemory_->GetFileFAT();
	pFAT[FreeFATID].bUsed = true;
	pFAT[FreeFATID].LeaveNum = DATALENGTH;
	pFAT[FreeFATID].NextID = FAT_NEXTEND;
	
	return bRet;
}

bool FileOperate::DeleteDIR(char* DirName)
{
	//模仿DOS下的RD操作,若文件夹中有文件就使操作失败
	//文件夹占一个DataArea
	//释放FCB
	//释放FAT
	//释放DataArea
	int DirHandle = INVALID_HANDLE;
	FileFCB* pFCB = NULL;
	FileFAT* pFAT = NULL;
	DataArea* pDataArea = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	if (this->pMemory_ == NULL)
		return false;	
	
	pFCB = this->pMemory_->GetFileFCB();
	pFAT = this->pMemory_->GetFileFAT();
	pDataArea = this->pMemory_->GetDataArea();
	DirHandle = this->GetFileHandle(DirName);
	if (INVALID_HANDLE == DirHandle)
		return false;
	if (this->IsFileInDirectory(DirName))
		return false;
	
	//进行删除
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFCB[i].FileHandle == DirHandle)
		{
			int FATID = pFCB[i].StartID;
			//释放FCB
			ZeroMemory(&pFCB[i], sizeof(FileFCB));
			pFCB[i].StartID = FCB_UNUSED;
			pFCB[i].FileHandle = FCB_UNUSED;
			pFCB[i].ParentDirID = FCB_UNUSED;
			//释放FAT
			pFAT[FATID].bUsed = false;
			pFAT[FATID].ID = FATID;
			pFAT[FATID].LeaveNum = DATALENGTH;
			pFAT[FATID].NextID = FAT_NEXTEND;
			//释放DataArea
			ZeroMemory(&pDataArea[FATID], sizeof(DataArea));
			
			return true;
		}
	}
	return false;
}

bool FileOperate::CreateDIR(char* DirName)
{
	int MainDirID = this->GetFileHandle(MainName);
	return this->CreateDIR(DirName, MainDirID);
}

bool FileOperate::CreateDIR(char* DirName, char* ParentName)
{
	if (!this->IsFileExist(ParentName))
		return false;
	int handle = this->GetFileHandle(ParentName);
	return this->CreateDIR(DirName, handle);
}


bool FileOperate::CreateMainDIR()
{
	return this->CreateDIR(MainName, PARENTMAINDIRID);
}

bool FileOperate::ModifyDIR(const char* DirName, const char* newName, int newAccess)
{

	if (this->pMemory_ == NULL)
		return false;

	int DirHandle;
	FileFCB* pFCB = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();	
	pFCB = this->pMemory_->GetFileFCB();
	DirHandle = this->GetFileHandle(DirName);

	if (DirHandle == INVALID_HANDLE)
		return false;

	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFCB[i].FileHandle == DirHandle)
		{
			if (pFCB[i].Type != TYPE_D)
				return false;
			if (newName != NULL)
				strcpy(pFCB[i].FileName, newName);
			pFCB[i].Access = newAccess;
			break;
		}
	}	
	return true;
}

bool FileOperate::ShowDIR(char* DirName,vector<int>& vec)
{
	//将所有在该Dir下的文件的ID都返回,并且保存在vec中
	
	int DirID = INVALID_HANDLE;
	FileFCB* pFCB = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	if (this->pMemory_ == NULL)
		return false;
	
	DirID = this->GetFileHandle(DirName);
	if (DirID == INVALID_HANDLE)
		return false;
	
	pFCB = this->pMemory_->GetFileFCB();
	vec.clear();
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFCB[i].ParentDirID == DirID)
			vec.push_back(pFCB[i].FileHandle);
	}
	
	return true;
}

bool FileOperate::CreateFile_(const char* FileName, int ParentDirID)
{
	//文件初始占一个DataArea
	//建立FCB,填充FCB结构
	//建立FAT,填充FAT结构
	int FreeFCBID = FCB_FULL;
	int FreeFATID = FAT_FULL;
	FileFCB* pFCB = NULL;
	FileFAT* pFAT = NULL;
	
	if (this->pMemory_ == NULL)
		return false;
	if (this->IsFileExist(FileName))
		return false;
	FreeFCBID = this->pMemory_->GetFreeFCBID();
	if (FreeFCBID == FCB_FULL)
		return false;
	FreeFATID = this->pMemory_->GetFreeFATID();
	if (FreeFATID == FAT_FULL)
		return false;
	
	//建立FCB
	pFCB = this->pMemory_->GetFileFCB();
	pFCB[FreeFCBID].Access = ACCESS_N;
	pFCB[FreeFCBID].FileHandle = this->GetUniqueHandle();
	pFCB[FreeFCBID].FileLength = 0;   //文件初始大小为0
	strcpy(pFCB[FreeFCBID].FileName, FileName);
	pFCB[FreeFCBID].NumDataArea = 1;  //文件初始占的DataArea数目为1
	pFCB[FreeFCBID].ParentDirID = ParentDirID;
	pFCB[FreeFCBID].StartID = FreeFATID;
	pFCB[FreeFCBID].Type = TYPE_N;
	::ZeroMemory(pFCB[FreeFCBID].UserName, USERNAME); 
	
	//建立FAT
	pFAT = this->pMemory_->GetFileFAT();
	pFAT[FreeFATID].bUsed = true;
	pFAT[FreeFATID].LeaveNum = DATALENGTH;
	pFAT[FreeFATID].NextID = FAT_NEXTEND;
	
	return true;
}

bool FileOperate::CreateFile_( char const* DirName)
{
	int MainDirID = this->GetFileHandle(MainName);
	return this->CreateFile_(DirName, MainDirID);
}

bool FileOperate::CreateFile_(const char* DirName, const char* ParentName)
{
	if (!this->IsFileExist(ParentName))
		return false;
	int handle = this->GetFileHandle(ParentName);
	return this->CreateFile_(DirName, handle);
}
int FileOperate::OpenFile_(const char* FileName)
{
	if (this->pMemory_ == NULL)
		return INVALID_HANDLE;
	return this->GetFileHandle(FileName);
}

bool FileOperate::ModifyFile_(const char* FileName, const char* newName, int newAccess)
{
	if (this->pMemory_ == NULL)
		return false;

	int FileHandle;
	FileFCB* pFCB = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();	
	pFCB = this->pMemory_->GetFileFCB();
	FileHandle = this->OpenFile_(FileName);
	if (FileHandle == INVALID_HANDLE)
		return false;

	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFCB[i].FileHandle == FileHandle)
		{
			if (pFCB[i].Type != TYPE_N)
				return false;
			if (newName != NULL)
				strcpy(pFCB[i].FileName, newName);
			pFCB[i].Access = newAccess;
			break;
		}
	}	
	return true;
}

bool FileOperate::CloseFile_(int FileHandle)
{
	//未实现,只是提供接口
	if (this->pMemory_ == NULL)
		return false;
	return true;
}

int FileOperate::WriteFile_(int FileHandle, const char* pSrc, int num)
{
	int FreeFCBID = FCB_FULL;
	int FreeFATID = FAT_FULL;
	int len = 0;
	int len1;
	int DataAreaStartID;
	int temp;
	int templen;
	int next;
	FileFCB* pFCB = NULL;
	FileFAT* pFAT = NULL;
	DataArea* pDataArea = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	if (this->pMemory_ == NULL)
		return 0;
	if (FileHandle == INVALID_HANDLE)
		return 0;
	len = MinValue(strlen(pSrc), num);
	if (len == 0)
		return 0;
	
	pFCB = this->pMemory_->GetFileFCB();
	pFAT = this->pMemory_->GetFileFAT();
	pDataArea = this->pMemory_->GetDataArea();
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFCB[i].FileHandle == FileHandle)
		{
			DataAreaStartID = pFCB[i].StartID;
			if (pFCB[i].Type != TYPE_N)
				return 0;
			break;
		}
	}
	
	temp = DataAreaStartID;
	while (pFAT[temp].NextID != FAT_NEXTEND)
		temp = pFAT[temp].NextID;
	
	templen = len;
	while (templen > 0)
	{
		len1 = templen;
		templen -= pFAT[temp].LeaveNum;
		if (templen > 0)
		{
			//写入Data
			strncpy(pDataArea[temp].Data+DATALENGTH-pFAT[temp].LeaveNum, pSrc,pFAT[temp].LeaveNum); 
			pSrc += pFAT[temp].LeaveNum;
			pFAT[temp].LeaveNum = 0;
			next = this->pMemory_->GetFreeFATID();
			if (next == FAT_FULL)
				return len-templen;
			pFAT[next].bUsed = true;
			pFAT[temp].NextID = next;
			temp = next;
		}
		else
		{	
			strncpy(pDataArea[temp].Data+DATALENGTH-pFAT[temp].LeaveNum, pSrc,len1); 
			pFAT[temp].LeaveNum -= len1;
			pSrc += len1;
		}
		
	}
	return len;
}

int FileOperate::ReadFile_(int FileHandle, char* pDest, int num)
{
	
	int len = 0;
	int len1;
	int DataAreaStartID;
	int temp;
	FileFCB* pFCB = NULL;
	FileFAT* pFAT = NULL;
	DataArea* pDataArea = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	if (this->pMemory_ == NULL)
		return 0;
	if (FileHandle == INVALID_HANDLE)
		return 0;
	len = num;
	if (len == 0)
		return 0;
	
	pFCB = this->pMemory_->GetFileFCB();
	pFAT = this->pMemory_->GetFileFAT();
	pDataArea = this->pMemory_->GetDataArea();
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (pFCB[i].FileHandle == FileHandle)
		{
			DataAreaStartID = pFCB[i].StartID;
			if (pFCB[i].Type != TYPE_N)
				return 0;
			break;
		}
	}
	
	temp = DataAreaStartID;
	while (true)
	{
		len1 = len;
		len -= DATALENGTH - pFAT[temp].LeaveNum;
		if (len > 0)
		{
			strncpy(pDest, pDataArea[temp].Data, DATALENGTH - pFAT[temp].LeaveNum);
			pDest += DATALENGTH - pFAT[temp].LeaveNum;
			
			temp = pFAT[temp].NextID;
			if (temp == FAT_NEXTEND)
				return num - len;
		}
		else
		{
			strncpy(pDest, pDataArea[temp].Data, len1);
			return num;
		}
		
	}
	
}

bool FileOperate::DeleteFile_(const char* FileName)
{
	//释放FAT
	//释放FCB
	//释放DataArea
	int FCBID = FCB_UNUSED;
	int DataAreaStartID;
	int TempID;
	int NextID;
	FileFCB* pFCB = NULL;
	FileFAT* pFAT = NULL;
	DataArea* pDataArea = NULL;
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	
	if (this->pMemory_ == NULL)
		return false;
	if (!this->IsFileExist(FileName))
		return false;
	
	
	pFCB = this->pMemory_->GetFileFCB();
	pFAT = this->pMemory_->GetFileFAT();
	pDataArea = this->pMemory_->GetDataArea();
	for (int i = 0; i < pFileInfo->GetNumFCB(); i++)
	{
		if (strcmp(pFCB[i].FileName, FileName) == 0)
		{
			DataAreaStartID = pFCB[i].StartID;
			FCBID = i;
			break;
		}
	}
	
	//释放FAT和DataArea
	TempID = DataAreaStartID;
	while(true)
	{
		NextID = pFAT[TempID].NextID;
		pFAT[TempID].bUsed = false;
		pFAT[TempID].LeaveNum = DATALENGTH;
		pFAT[TempID].NextID = FAT_NEXTEND;
		::ZeroMemory(&pDataArea[TempID], DATALENGTH);
		
		TempID = NextID;
		if (TempID == FAT_NEXTEND)
			break;
	}
	
	//释放FCB
	ZeroMemory(&pFCB[FCBID], sizeof(FileFCB));	
	pFCB[FCBID].StartID = FCB_UNUSED;
	pFCB[FCBID].FileHandle = FCB_UNUSED;
	pFCB[FCBID].ParentDirID = FCB_UNUSED;
	return true;
}

void FileOperate::InitMemory()
{
	FileSystemInfo* pFileInfo = NULL;
	pFileInfo = FileSystemInfo::GetFileInfo();
	if (this->pMemory_ != NULL && !pFileInfo->isFromFile())
		this->CreateMainDIR();   //建立主目录
}
