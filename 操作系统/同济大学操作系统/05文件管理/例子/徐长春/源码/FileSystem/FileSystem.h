#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#include <string.h>

#define false 0
#define true 1
#define n 5 /*ģ��ʵ����ϵͳ������ļ����������*/


class Content /*Ŀ¼�ṹ*/
{
	public:
	char name[3]; /*�ļ���Ŀ¼��*/
	char type[3]; /*�ļ�������*/
	char attribute; /*����*/
	char address; /*�ļ���Ŀ¼����ʼ�̿��*/
	char length; /*�ļ����ȣ����̿�Ϊ��λ*/
};

/*�Ѵ��ļ����ж�дָ��Ľṹ*/
class Pointer 
{
public:
	int dnum; /*�����̿��*/
	int bnum; /*�̿��ڵڼ���*/
}; 

/*�Ѵ��ļ��������Ͷ���*/
class OFILE
{
public:
	char name[20]; /*�ļ�����·����*/
	char attribute;/*�ļ������ԣ���1���ֽڱ�ʾ����������char����*/
	int number; /*�ļ���ʼ�̿��*/
	int length; /*�ļ����ȣ��ļ�ռ�õ��ֽ���*/
	int flag; /*�������ͣ���"0"��ʾ�Զ�������ʽ���ļ�����"1"��ʾд������ʽ���ļ�*/
	Pointer read; /*���ļ���λ�ã��ļ��մ�ʱdnumΪ�ļ���ʼ�̿��,bnumΪ"0"*/
	Pointer write; /*д�ļ���λ�ã��ļ�����ʱdnumΪ�ļ���ʼ�̿��,bnumΪ"0"����ʱΪ�ļ�ĩβ*/
}; 

/*�Ѵ��ļ�����*/
class OpenFile
{
public:
   OFILE file[n]; /*�Ѵ��ļ���*/
   int length; /*�Ѵ��ļ����еǼǵ��ļ�����*/
}; 
char buffer1[64];/*ģ�⻺��1*/
Content buffer2[8];/*ģ�⻺��2*/
FILE *fc; /*ģ����̵��ļ�ָ��*/