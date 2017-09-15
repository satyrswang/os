// ManageMemory.h: interface for the ManageMemory class.
//
//////////////////////////////////////////////////////////////////////
#include "FileFCB.h"
#include "FileFAT.h"
#include "DataArea.h"
#include "FileSystemInfo.h"

#pragma once

class ManageMemory  
{
public:
	ManageMemory();
	virtual ~ManageMemory();

private:
	char* pMemory;
	FileFCB* pFileFCB;
	FileFAT* pFileFAT;
	DataArea* pDataArea;

private:
	bool AllocMemory();
	void DeleteMemory();
		
	bool CreateFCB();
	bool CreateDataArea();
	bool CreateFAT();
	
public:
	bool InitMemory();
	FileFCB* GetFileFCB();
	FileFAT* GetFileFAT();
	DataArea* GetDataArea();

	int GetFreeFATID();
	int GetFreeFCBID();
	int GetNumFreeDataArea();
	int GetNummFreeFCB();
	bool WirteFile(char* SaveName);

	//static ManageMemory* GetManageMemory();
	

};

