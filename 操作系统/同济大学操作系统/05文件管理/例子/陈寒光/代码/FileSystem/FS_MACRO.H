#ifndef FS_MACRO_H
#define FS_MACRO_H
#include <windows.h>
//*************下面是文件的存贮方式
	#define NORMAL_FILE				0x00
	#define READ_ONLY				0x01
	#define HIDDEN_FILE				0x02
	#define SYSTEM_FILE				0x04
	#define SUBDIRECTORY			0x10
	#define ARCHIVE_FILE			0x20
	#define SECTOR_PER_CLUSTER		0x01
	#define CLUSTER_SIZE			512 * SECTOR_PER_CLUSTER
//***************下面是文件的打开方式
	#define OPEN_MODE_READ_ONLY		0x00
	#define OPEN_MODE_WRITE_ONLY	0x01
	#define OPEN_MODE_READ_WRITE	0x02
//***************下面是Shell的宏
	#define SHELL_EXIT				0x00
	#define SHELL_NORMAL			0x01
	/***************************
		文件时间结构
	****************************/
	  typedef struct tagFILE_TIME{
		short wYear;
		short wMonth;
		short wDay;
		short wHour;
		short wMinute;
		short wSecond;
	} FILE_TIME, *PFILE_TIME;

	/************************
	文件的索引结构
	************************/
	  typedef struct tagEntry{// sizeof( ENTRY ) == 32 
		char fileMainName[9]; //9
		char extension[4];    //4
		BYTE type;            //1
		FILE_TIME ft;         //12
		WORD startCluster;    //2
		int sizeOfFile;       //4
	}ENTRY, *PENTRY;


#endif

