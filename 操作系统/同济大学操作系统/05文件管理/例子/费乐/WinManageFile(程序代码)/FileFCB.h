// FileFCB.h: interface for the FileFCB class.
//
//////////////////////////////////////////////////////////////////////
#include "Global.h"


#pragma once
#pragma pack(push, 1)
struct FileFCB  
{
char FileName[FILENAME];
char UserName[USERNAME];    //‘› ±≤ª”√

int StartID;
int FileHandle;
int FileLength;
int NumDataArea;
int Access;
int Type;
int ParentDirID;

};

#pragma pack(pop)

