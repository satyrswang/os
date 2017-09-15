#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#include <string.h>

#define false 0
#define true 1
#define n 5 /*模拟实验中系统允许打开文件的最大数量*/


class Content /*目录结构*/
{
	public:
	char name[3]; /*文件或目录名*/
	char type[3]; /*文件类型名*/
	char attribute; /*属性*/
	char address; /*文件或目录的起始盘块号*/
	char length; /*文件长度，以盘块为单位*/
};

/*已打开文件表中读写指针的结构*/
class Pointer 
{
public:
	int dnum; /*磁盘盘块号*/
	int bnum; /*盘块内第几项*/
}; 

/*已打开文件表项类型定义*/
class OFILE
{
public:
	char name[20]; /*文件绝对路径名*/
	char attribute;/*文件的属性，用1个字节表示，所以用了char类型*/
	int number; /*文件起始盘块号*/
	int length; /*文件长度，文件占用的字节数*/
	int flag; /*操作类型，用"0"表示以读操作方式开文件，用"1"表示写操作方式打开文件*/
	Pointer read; /*读文件的位置，文件刚打开时dnum为文件起始盘块号,bnum为"0"*/
	Pointer write; /*写文件的位置，文件建立时dnum为文件起始盘块号,bnum为"0"，打开时为文件末尾*/
}; 

/*已打开文件表定义*/
class OpenFile
{
public:
   OFILE file[n]; /*已打开文件表*/
   int length; /*已打开文件表中登记的文件数量*/
}; 
char buffer1[64];/*模拟缓冲1*/
Content buffer2[8];/*模拟缓冲2*/
FILE *fc; /*模拟磁盘的文件指针*/