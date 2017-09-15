// Filesystem.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Filesystem.h"
#include <stdlib.h>
#include <string>
#include <vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// The one and only application object
CWinApp theApp;
//������һЩ����������
int retKongxianFat(int);
int ISZGKongxianFat(int);
int QingKongFat(int tmp[20],int index);
void DeleteFile(int index);
const int TypeCatalog=2;//Ŀ¼����
const int TypeFile=1;//�ļ�����
const int Block=1000;//ÿ�������Ĵ�С
const int idlesse=0;//���б�־
const int noidlesse=1;//�ǿ��б�־
const int fail=-1;//ʧ�ܱ�־
const int End=-2;//�ļ������Ӧ���������һ��
const int FatLength=100;//������ܵĿ�����FAT��ĳ���
//FCB�ļ�/Ŀ¼���ƽṹ
struct fcb{
	char szName[256];//�ļ���Ŀ¼��
	int  iIndex;//����
	int  iFatherIndex;//�ϼ�Ŀ¼������ ��Ŀ¼Ϊ-1
	//int  iShared;//�������
	int  Type;//catalog��file������һ��
	//bool IsRoot;//�Ƿ�Ϊ��Ŀ¼
    int  iNextBlock;//��ָ��һ���ڴ��ַ�� ����ַ  Ŀ¼�Ļ���Ϊ0
	int  iLength;//�ļ�����  Ŀ¼Ϊ0
	int  state;//ʹ�����
	int  IsOpend;//�Ƿ�Ϊ��״̬��ֻ����stateΪ1��TypeΪ �ļ�ʱ����Ч����Ϊ1
}FCB[FatLength];//�ļ���Ŀ¼�Ŀ�����Ϣ

int Fat[FatLength];//FAT���λʾͼ�Ľ�� ���ڷ��������Ϳ��п�Ļ���
char *p=NULL;//�ܵ�������ڴ�������ļ�ϵͳ�õ�ͷָ��
char pwd[256]="root";//��ǰĿ¼��
string precommand=" ";//��һ��ʹ�õ�����
const int ShenqingLength=sizeof(Fat)+sizeof(FCB)+FatLength*Block;//�����ڴ�Ĵ�С

void init(void){//��ʼ������
	p=(char *)malloc(ShenqingLength);
	for(int i=0;i<FatLength;i++)
		FCB[i].iFatherIndex=-2;
	::memset(p+sizeof(Fat)+sizeof(FCB),'\0',FatLength*Block);
	::memset(pwd,'\0',5);
	::memcpy(pwd,"root",4);
}

int IsEqual(char s1[256],char s2[256]){//�ж����ַ������Ƿ���ȣ�ȥ���ո�
	int ret=1; 
	CString ss1(s1);
	CString ss2(s2);
	ss1.TrimRight();
	ss2.TrimRight();
	ret=ss1.Compare(ss2);
 return ret;
}

int retIndex(string s){//����һ���ļ���/Ŀ¼����FCB�е�������
	CString ss1(s.c_str());
	for(int i=0;i<FatLength;i++){
		CString ss2(FCB[i].szName);
		ss1.TrimRight();
		ss2.TrimRight();
		if(ss1.Compare(ss2)==0)
			return FCB[i].iIndex;
	}
	CString ss3("root");
	if(ss1.Compare(ss3)==0) return -1;
	return -2;//�д���û�д�����
}

int parsePath(char path[1000]){//����ȫ·��Ϊ����Ŀ¼���������·���Ƿ����
	vector<string> vec;
	char temp[256];
	int j=0;
	for(int i=0;(i<1000&&path[i]!='\0');i++)
		if(path[i]!='/') 
			temp[j++]=path[i];
		else
		{temp[j]='\0';j=0;string s=temp;
		vec.push_back(s);}
		
		for(i=vec.size()-1;i>0;i--) 
			if(FCB[retIndex(vec[i])].iFatherIndex!=retIndex(vec[i-1]))
				return -2;//·������

		if(retIndex(vec[0])!=-1)
			return -2;
		vector<string>::iterator it=vec.begin();
		cout<<"·������Ϊ:\n";
		for(;it!=vec.end();it++)
			cout<<*it<<endl;
	return retIndex(vec[vec.size()-1]);//�������һ������index
}

void Save(void){//save�ļ�ϵͳ
	::memcpy(p,&Fat,sizeof(Fat));
	::memcpy(p+sizeof(Fat),&FCB,sizeof(FCB));

	FILE *fp;
	if((fp=fopen("FS","wb"))==NULL){
		printf("Cannot open file!.\n");
			return ;
	}
	for(int i=0;i<sizeof(Fat)+sizeof(FCB)+FatLength*Block;i++) 
		if(fwrite(p+i,1,1,fp)!=1){
			printf("File read error.\n");
		}
	fclose(fp);
}

void Format(void){//��ʽ���ļ�ϵͳ
	::memset(p,sizeof(Fat)+sizeof(FCB)+FatLength*Block,'\0');
	::memset(&Fat,'\0',sizeof(Fat));
	::memset(&FCB,'\0',sizeof(FCB));
	init();
	Save();
	printf("��ʽ���ļ�ϵͳ�ɹ�!\n");
}

int Load(void){//load�ļ�ϵͳ
	FILE *fp;
	if((fp=fopen("FS","rwb"))==NULL){
			return -1;
	}
	for(int i=0;i<sizeof(Fat)+sizeof(FCB)+FatLength*Block;i++) 
		if(fread(p+i,1,1,fp)!=1){
			if(feof(fp)) break;
			printf("File read error.\n");
		}
	fclose(fp);

	::memcpy(&Fat,p,sizeof(Fat));
	::memcpy(&FCB,p+sizeof(Fat),sizeof(FCB));
	return 0;
}

void DisplayCatalog(int Father){//��ʾ����Ŀ¼�ļ�
	printf("��ǰ��Ŀ¼����:\n");
	for(int i=0;i<FatLength;i++)
		if(FCB[i].Type==TypeCatalog&&FCB[i].iFatherIndex==Father) 
			printf("%s %d\n",FCB[i].szName,FCB[i].iIndex);
	
}

void DisplayFile(int Father){//��ʾ�����ļ�
	printf("��ǰ�ļ�Ϊ:\n");
	for(int i=0;i<FatLength;i++)
		if(FCB[i].Type==TypeFile&&FCB[i].iFatherIndex==Father) 
			printf("%s %d\n",FCB[i].szName,FCB[i].iIndex);
}

void Create(char fileName[256],int length,int Type,int father){//�������������ļ�/Ŀ¼
	int flag=0;//�Ƿ��п���Ŀ¼
	string szPWD(pwd);
	for(int i=0;i<FatLength;i++)
		if(!IsEqual(FCB[i].szName,fileName)&&FCB[i].iFatherIndex==retIndex(szPWD)) {printf("�ļ���Ŀ¼ͬ��,����ʧ��\n");return;} 
	if(Type==TypeFile)
		if(ISZGKongxianFat(length)==-1) {printf("û���㹻���������\n");return ;} 
	if(father==-2)
	{printf("û�д˸����");return ;}

	for(i=0;i<FatLength;i++)
		if(FCB[i].state==idlesse) 
			{
				flag=1;
				::memcpy(FCB[i].szName,fileName,256);
				FCB[i].state=noidlesse;
				FCB[i].Type=Type;
				FCB[i].iFatherIndex=father;
				if(Type==TypeFile) FCB[i].iNextBlock=retKongxianFat(length);
				FCB[i].iIndex=i;
				FCB[i].iLength=length;
				break;
			}
	if(flag==1)
		printf("create victory\n");
}

void DeleteCatalog(int index){//��������ɾ��Ŀ¼
	if(index==-1)//�൱�ڸ�ʽ��
		Format();
	if(FCB[index].Type==TypeCatalog){
		::memset(&FCB[index],'-2',sizeof(FCB[index]));
		for(int i=0;i<FatLength;i++){
			if(FCB[i].Type==TypeFile&&FCB[i].iFatherIndex==index)
				DeleteFile(i);
			if(FCB[i].Type==TypeCatalog&&FCB[i].iFatherIndex==index)
				DeleteCatalog(i);
		}

	}
}

void DeleteFile(int index){//��������ɾ���ļ�
	if(FCB[index].Type==TypeFile){
		int tmp[20];	
		int num=QingKongFat(tmp,FCB[index].iNextBlock);
		::memset(&FCB[index],'-2',sizeof(FCB[index]));
		for(int i=0;i<num;i++)
			::memset(&Fat[tmp[i]],'\0',sizeof(Fat[tmp[i]]));
	}
}

int QingKongFat(int tmp[20],int index){//Ҫ���Fat�����к���
	tmp[0]=index;
	int k=0;
	while(Fat[tmp[k]]!=End) tmp[k+1]=Fat[tmp[k++]];
	return k+1;
}

int ISZGKongxianFat(int length){//�ж�Ҫ����ռ��С�Ƿ�������Ҫ��
	int iNumBlock;//��Ҫ������������
	int ret=-1;//��ʾ���䲻�ɹ�
	if(length%Block==0) iNumBlock=length/Block;
	else iNumBlock=length/Block+1;
	int inum=iNumBlock;
	int j=0;

	for(int i=0;i<FatLength;i++)
		if(Fat[i]==idlesse) j++;
	if(j>=iNumBlock)
		ret=0;
	else
		ret=-1;
	return ret;
} 

int retKongxianFat(int length){//����Ҫ����ռ��С���������������
	int iNumBlock;//��Ҫ������������
	int ret=-1;//��ʾ���䲻�ɹ�
	if(length%Block==0) iNumBlock=length/Block;
	else iNumBlock=length/Block+1;
	int inum=iNumBlock;
	int j=0;
	int tmp[10];
	for(int i=0;i<FatLength;i++)
		if(Fat[i]==idlesse) j++;
	if(j>=iNumBlock)
	{
		int k=0;
		for(i=0;i<FatLength&&inum>0;i++)
			if(Fat[i]==idlesse) 
			{tmp[k++]=i;inum--;}
		for(i=0;i<iNumBlock-1;i++)
			Fat[tmp[i]]=tmp[i+1];
		Fat[tmp[i]]=End;
		ret=tmp[0];
	}
	else
		ret=-1;
	return ret;
}

void displayPWD(void){//��ʾ��ǰĿ¼��
	printf("%s\n",pwd);
}
void LS(void){//�г���ǰĿ¼�µ�Ŀ¼���ļ�����Ϣ
	string szpwd=pwd;
	for(int i=0;i<FatLength;i++)
		if(FCB[i].iFatherIndex==retIndex(szpwd)){
			if(FCB[i].Type==TypeCatalog) 
				printf("<DIR>          %s    \n",FCB[i].szName);
		 else   
		 {printf("       %d    %s    ",FCB[i].iLength,FCB[i].szName);
		 if(FCB[i].IsOpend==1) printf("Open\n");
		 else printf("Close\n");
		 }
		}
}

void HELP(void){//��ʾ���ļ�ϵͳ֧�ֵ������嵥
	printf("help/?   ��ʾ���������Ϣ\n");
	printf("pwd      �г���ǰĿ¼\n");
	printf("ls       �г���ǰĿ¼�µ�Ŀ¼���ļ�\n");
	printf("rm       ɾ���ļ� �÷�:rm �ļ���\n");
	printf("rmdir    ɾ��Ŀ¼ �÷�:rmdir Ŀ¼��\n");
	printf("mk       �����ļ� �÷�:mk �ļ��� ���� mk �ļ��� �ļ���С\n");
	printf("mkdir    ����Ŀ¼ �÷�:mkdir Ŀ¼��\n");
	printf("open     ���Ѿ������ļ� �÷�:open �ļ���\n");
	printf("close    �ر��Ѿ������ļ� �÷�:close �ļ���\n");
	printf("vi       д�ļ� �÷�:vi �ļ��� ��$����.�ļ����Ѵ�.������Ḳ����ǰ����\n");
	printf("cat      ���ļ� �÷�:cat �ļ���,���ļ�ǰ�ļ����Ѵ�\n");
	printf("format   ��ʽ���ļ�ϵͳ\n");
	printf("exit     �˳����ļ�ϵͳ\n");
}

void MK(char fileName[256],int length){ //�ڵ�ǰĿ¼�½��ļ�
	if(length<=0) length=1000;
	string szPWD(pwd);
	string Name(fileName);
	if(Name.compare("root")!=0)
		Create(fileName,length,TypeFile,retIndex(szPWD));
	else printf("������root��Ϊ�ļ���\n");
}

void MKIDR(char directoryName[256]){ //�ڵ�ǰĿ¼�½�Ŀ¼
	string szPWD(pwd);
	string Name(directoryName);
	if(Name.compare("root")!=0)
		Create(directoryName,0,TypeCatalog,retIndex(szPWD));
	else printf("������root��ΪĿ¼��\n");
}

void RM(char fileName[256]){ //�ڵ�ǰĿ¼��ɾ���ļ�
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type!=TypeFile) printf("�����ô�����ɾ��Ŀ¼\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
		DeleteFile(retIndex(szName));
	else
		printf("���ļ����ڵ�ǰĿ¼��!\n");
	}
	else
		printf("���ļ�������!\n");
}

void RMDIR(char directoryName[256]){ //�ڵ�ǰĿ¼��ɾ���¼�Ŀ¼
	string szName(directoryName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type==TypeFile) printf("�����ô�����ɾ���ļ�\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
		DeleteCatalog(retIndex(szName));
	else
		printf("��Ŀ¼���ڵ�ǰĿ¼��!\n");
	}
	else
		printf("��Ŀ¼������!\n");

}

void ReadFile(int index){//���ļ�
	int base=sizeof(FCB)+sizeof(Fat);
	int tmp[20];
	int k=QingKongFat(tmp,FCB[index].iNextBlock);
	int j=0;
	for(int i=0;i<k;i++)
		for(int j=0;j<Block&&*(p+base+tmp[i]*Block+j)!='\0';j++)  
			cout<<*(p+base+tmp[i]*Block+j);
	printf("\n");
}

void WriteFile(int index){//д�ļ�
	ReadFile(index);
	int base=sizeof(FCB)+sizeof(Fat);
	int tmp[20];
	int k=QingKongFat(tmp,FCB[index].iNextBlock);
	int j=0;
	char ch;
	for(int i=0;i<k;i++)
		for(int j=0;j<Block;j++)
		{
			//cin.get(c);
			//scanf("%c",&c);
			ch=getchar();
			if(ch=='$')  return;
			*(p+base+tmp[i]*Block+j)=ch;
		}
}

void OPENFILE(char fileName[256]){//���ļ�
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
		if(FCB[retIndex(szName)].Type!=TypeFile) printf("Ŀ¼�����ڴ�\n");
		else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
		{
			if(FCB[retIndex(szName)].IsOpend==1)
				printf("���ļ��Ѿ�����\n");
			else
				FCB[retIndex(szName)].IsOpend=1;
		}
		else
			printf("���ļ����ڵ�ǰĿ¼��!\n");
	}
	else
		printf("���ļ�������!\n");
}

void CLOSEFILE(char fileName[256]){//�ر��ļ�
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type!=TypeFile) printf("Ŀ¼�����ڹر�\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
	{
		if(FCB[retIndex(szName)].IsOpend!=1)
			printf("���ļ��Ѿ������ر�\n");
		else
			FCB[retIndex(szName)].IsOpend=0;
	}
	else
		printf("���ļ����ڵ�ǰĿ¼��!\n");
	}
	else
		printf("���ļ�������!\n");
}

void CAT(char fileName[256]){ //��ʾ�ļ�������
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type!=TypeFile) printf("�����ô������Ŀ¼\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
	{
		if(FCB[retIndex(szName)].IsOpend==1)
			ReadFile(retIndex(szName));
		else
			printf("���ļ�δ��,����open it\n");
	}
	else
		printf("���ļ����ڵ�ǰĿ¼��!\n");
	}
	else
		printf("���ļ�������!\n");

}

void VI(char fileName[256]){ //�޸��ļ�������
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type!=TypeFile) printf("�����ô������Ŀ¼\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
	{
		if(FCB[retIndex(szName)].IsOpend==1)
			WriteFile(retIndex(szName));
		else
			printf("���ļ�δ��,����open it\n");
	}
	else
		printf("���ļ����ڵ�ǰĿ¼��!\n");
	}
	else
		printf("���ļ�������!\n");

}

void CD(char directoryName[256]){//cd ��һ��Ŀ¼
	string s=directoryName;
	if(retIndex(s)==-2||retIndex(s)!=-2&&FCB[retIndex(s)].iFatherIndex!=retIndex(pwd)||FCB[retIndex(s)].Type==TypeFile)
	{printf("�����ڴ�Ŀ¼\n");return;}
	::memset(pwd,'\0',s.size()+1);
	::memcpy(pwd,directoryName,s.size());
}

void CDDDot(void){//�ص��ϼ�Ŀ¼
	string szPWD(pwd);
	if(retIndex(szPWD)>=0){
		if(FCB[retIndex(szPWD)].iFatherIndex==-1)
		{
			::memset(pwd,'\0',5);
			::memcpy(pwd,"root",4);
		}
		else{
			string szPWD2=FCB[FCB[retIndex(szPWD)].iFatherIndex].szName;
			::memset(pwd,'\0',szPWD2.size()+1);
			::memcpy(pwd,FCB[FCB[retIndex(szPWD)].iFatherIndex].szName,szPWD2.size());
		}
	}
}

int parseCommand(string command,string name,int length){//����ִ������
	CString szCommand(command.c_str());
	CString szprecommand(precommand.c_str());
	char *p=(char *)name.c_str();
	char Name[256];
	for(int i=0; i<name.size();i++)
		Name[i]=*p++;
	Name[i]='\0';

	if(!szCommand.CompareNoCase("pwd")) displayPWD(); 
	else if(!szCommand.CompareNoCase("ls")) LS();
	else if(!szCommand.CompareNoCase("?")||!szCommand.CompareNoCase("help"))  HELP();
	else if(!szCommand.CompareNoCase("mk")) MK(Name,length);
	else if(!szCommand.CompareNoCase("mkdir")) MKIDR(Name);
	else if(!szCommand.CompareNoCase("rm")) RM(Name);
	else if(!szCommand.CompareNoCase("rmdir")) RMDIR(Name);
	else if(!szCommand.CompareNoCase("cat")) CAT(Name);
	else if(!szCommand.CompareNoCase("vi")) VI(Name);
	else if(!szCommand.CompareNoCase("cd"))  CD(Name);
	else if(!szCommand.CompareNoCase("cd.."))  CDDDot();
	else if(!szCommand.CompareNoCase("format"))  Format();
	else if(!szCommand.CompareNoCase("exit"))  {for(int j=0;j<FatLength;j++)FCB[j].IsOpend=0;Save();exit(0);}
	else if(!szprecommand.CompareNoCase("vi"))  return -2;
	else if(!szCommand.CompareNoCase("open"))  OPENFILE(Name);
	else if(!szCommand.CompareNoCase("close"))  CLOSEFILE(Name);
	else printf("wrong command!\n");
	Save();
	precommand=command;
	return 0;
}

void InputCommand(void){//�����������롢���������ʾ��ǰĿ¼·��
	char command[256];
	printf("��ӭʹ�ü��ļ�ϵͳv1.0.0\n");
	printf("(c) ��Ȩ����  ��˹�� 010155 \n");
	cout<<"********************************************************************************"<<endl;
	cout<<"���:�ٶ�ϵͳ��ʽ�����и�Ŀ¼root,�ڴ��¿��Խ��ļ��Լ�Ŀ¼.                   "<<endl;
	cout<<"     Ŀ¼���滹�����ٽ��ļ���Ŀ¼���Դ����ơ�ֱ��û�пռ�Ϊֹ.                "<<endl;
	cout<<"     Ŀ¼�ṹ����Linux������.���������Linux������.����Ŀ��Դ�?/help�س��鿴 "<<endl;
	cout<<"Ԥ��:ÿ��������С:1000�ֽ�;����ļ���Ŀ¼��Ϊ100��;�ļ���Ŀ¼���໥��Ҫͬ�� "<<endl;
	cout<<"********************************************************************************"<<endl;
	cout<<"                                                              "<<endl;
		
	while(true){
		//��ʾ����ǰĿ¼��·��
		string szPWD(pwd),s;
		vector<string> svec;
		int index=retIndex(szPWD);
		if(index!=-1){
			while(true){
				string stmp(FCB[index].szName);
				if(FCB[index].iFatherIndex!=-1)
				{	
					svec.push_back(stmp);
					index=FCB[index].iFatherIndex;
				}
				else
				{
					svec.push_back(stmp);
					break;
				}
			}		
		}
		svec.push_back("root");
		vector<string>::reverse_iterator li;
		for(li=svec.rbegin();li!=svec.rend();li++)
		{
			s+="/";
			s+=*li;
		}
		cout<<s<<">";

Label:	gets(command);

		vector<string> vec;
		char temp[256];
		int j=0;
		for(int i=0;i<256;i++){
			if(command[i]!=' '&&command[i]!='\0') 
				temp[j++]=command[i];
			else
			{
				temp[j]='\0';j=0;string s=temp;
				vec.push_back(s);
				if(command[i]=='\0') break;
			}
		}
		int flag=0;
		if(vec.size()==1)
			flag=parseCommand(vec[0]," ",0);
		else if(vec.size()==2) 
			flag=parseCommand(vec[0],vec[1],0);
		else {
			char c[10];
			::memset(c,'\0',11);
			::memcpy(c,vec[2].c_str(),vec[2].length());
			flag=parseCommand(vec[0],vec[1],atoi(c));
		}
		svec.clear();
		vec.clear();
		if(flag==-2) goto Label;
	}
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		init();//��ʼ���ļ�ϵͳ
		if(Load()==-1) Save();
		InputCommand();		
	}
	return nRetCode;
}


