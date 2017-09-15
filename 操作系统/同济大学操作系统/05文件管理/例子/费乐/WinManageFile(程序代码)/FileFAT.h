// FileFAT.h: interface for the FileFAT class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#pragma once
#pragma pack(push , 1)

struct FileFAT  
{
int ID;
int NextID;
int LeaveNum;
bool bUsed;
};


#pragma pack(pop)