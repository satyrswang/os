
#pragma once
//////////////////////
//Global Macro
//////////////////////
#define FILENAME 30
#define USERNAME 20

#define DATALENGTH 1024

#define ACCESS_O 1
#define ACCESS_H 2
#define ACCESS_N 3

#define TYPE_D 1
#define TYPE_N 2
#define TYPE_S 3

#define FCB_UNUSED -1
#define FCB_FULL -2

#define FAT_NEXTEND -1
#define FAT_FULL -2

#define PARENTMAINDIRID -3
extern char MainName[FILENAME];

#define INVALID_HANDLE -4

#define NOREQUEST 0
#define SHOWDIR 1
#define READFILE 2

#define LINEHEIGHT 20
//////////////////////
//Global Function
//////////////////////
int MaxValue(int value1, int value2);
int MinValue(int value1, int value2);

