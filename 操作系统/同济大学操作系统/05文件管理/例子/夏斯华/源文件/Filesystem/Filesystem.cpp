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
//函数和一些常量的声明
int retKongxianFat(int);
int ISZGKongxianFat(int);
int QingKongFat(int tmp[20],int index);
void DeleteFile(int index);
const int TypeCatalog=2;//目录类型
const int TypeFile=1;//文件类型
const int Block=1000;//每个物理块的大小
const int idlesse=0;//空闲标志
const int noidlesse=1;//非空闲标志
const int fail=-1;//失败标志
const int End=-2;//文件申请对应物理块的最后一个
const int FatLength=100;//物理块总的块数、FAT表的长度
//FCB文件/目录控制结构
struct fcb{
	char szName[256];//文件、目录名
	int  iIndex;//索引
	int  iFatherIndex;//上级目录的索引 根目录为-1
	//int  iShared;//共享计数
	int  Type;//catalog、file两者中一个
	//bool IsRoot;//是否为根目录
    int  iNextBlock;//所指第一块内存地址块 即首址  目录的话就为0
	int  iLength;//文件长度  目录为0
	int  state;//使用情况
	int  IsOpend;//是否为打开状态（只有在state为1，Type为 文件时才有效）打开为1
}FCB[FatLength];//文件或目录的控制信息

int Fat[FatLength];//FAT表和位示图的结合 用于分配物理块和空闲块的回收
char *p=NULL;//总的申请的内存区域给文件系统用的头指针
char pwd[256]="root";//当前目录名
string precommand=" ";//上一次使用的命令
const int ShenqingLength=sizeof(Fat)+sizeof(FCB)+FatLength*Block;//申请内存的大小

void init(void){//初始化环境
	p=(char *)malloc(ShenqingLength);
	for(int i=0;i<FatLength;i++)
		FCB[i].iFatherIndex=-2;
	::memset(p+sizeof(Fat)+sizeof(FCB),'\0',FatLength*Block);
	::memset(pwd,'\0',5);
	::memcpy(pwd,"root",4);
}

int IsEqual(char s1[256],char s2[256]){//判断两字符数组是否相等（去除空格）
	int ret=1; 
	CString ss1(s1);
	CString ss2(s2);
	ss1.TrimRight();
	ss2.TrimRight();
	ret=ss1.Compare(ss2);
 return ret;
}

int retIndex(string s){//返回一个文件名/目录名在FCB中的索引号
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
	return -2;//有错误，没有此名字
}

int parsePath(char path[1000]){//解析全路径为单个目录名，并检查路径是否存在
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
				return -2;//路径有误

		if(retIndex(vec[0])!=-1)
			return -2;
		vector<string>::iterator it=vec.begin();
		cout<<"路径解析为:\n";
		for(;it!=vec.end();it++)
			cout<<*it<<endl;
	return retIndex(vec[vec.size()-1]);//返回最后一个名的index
}

void Save(void){//save文件系统
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

void Format(void){//格式化文件系统
	::memset(p,sizeof(Fat)+sizeof(FCB)+FatLength*Block,'\0');
	::memset(&Fat,'\0',sizeof(Fat));
	::memset(&FCB,'\0',sizeof(FCB));
	init();
	Save();
	printf("格式化文件系统成功!\n");
}

int Load(void){//load文件系统
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

void DisplayCatalog(int Father){//显示所有目录文件
	printf("当前有目录如下:\n");
	for(int i=0;i<FatLength;i++)
		if(FCB[i].Type==TypeCatalog&&FCB[i].iFatherIndex==Father) 
			printf("%s %d\n",FCB[i].szName,FCB[i].iIndex);
	
}

void DisplayFile(int Father){//显示所有文件
	printf("当前文件为:\n");
	for(int i=0;i<FatLength;i++)
		if(FCB[i].Type==TypeFile&&FCB[i].iFatherIndex==Father) 
			printf("%s %d\n",FCB[i].szName,FCB[i].iIndex);
}

void Create(char fileName[256],int length,int Type,int father){//根据条件建立文件/目录
	int flag=0;//是否有空闲目录
	string szPWD(pwd);
	for(int i=0;i<FatLength;i++)
		if(!IsEqual(FCB[i].szName,fileName)&&FCB[i].iFatherIndex==retIndex(szPWD)) {printf("文件或目录同名,创建失败\n");return;} 
	if(Type==TypeFile)
		if(ISZGKongxianFat(length)==-1) {printf("没有足够空闲物理块\n");return ;} 
	if(father==-2)
	{printf("没有此父结点");return ;}

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

void DeleteCatalog(int index){//根据索引删除目录
	if(index==-1)//相当于格式化
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

void DeleteFile(int index){//根据索引删除文件
	if(FCB[index].Type==TypeFile){
		int tmp[20];	
		int num=QingKongFat(tmp,FCB[index].iNextBlock);
		::memset(&FCB[index],'-2',sizeof(FCB[index]));
		for(int i=0;i<num;i++)
			::memset(&Fat[tmp[i]],'\0',sizeof(Fat[tmp[i]]));
	}
}

int QingKongFat(int tmp[20],int index){//要清空Fat表序列号码
	tmp[0]=index;
	int k=0;
	while(Fat[tmp[k]]!=End) tmp[k+1]=Fat[tmp[k++]];
	return k+1;
}

int ISZGKongxianFat(int length){//判断要申请空间大小是否能满足要求
	int iNumBlock;//需要分配的物理块数
	int ret=-1;//表示分配不成功
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

int retKongxianFat(int length){//根据要分配空间大小分配空闲物理块号码
	int iNumBlock;//需要分配的物理块数
	int ret=-1;//表示分配不成功
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

void displayPWD(void){//显示当前目录名
	printf("%s\n",pwd);
}
void LS(void){//列出当前目录下的目录和文件的信息
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

void HELP(void){//显示该文件系统支持的命令清单
	printf("help/?   显示命令帮助信息\n");
	printf("pwd      列出当前目录\n");
	printf("ls       列出当前目录下的目录和文件\n");
	printf("rm       删除文件 用法:rm 文件名\n");
	printf("rmdir    删除目录 用法:rmdir 目录名\n");
	printf("mk       建立文件 用法:mk 文件名 或者 mk 文件名 文件大小\n");
	printf("mkdir    建立目录 用法:mkdir 目录名\n");
	printf("open     打开已经存在文件 用法:open 文件名\n");
	printf("close    关闭已经存在文件 用法:close 文件名\n");
	printf("vi       写文件 用法:vi 文件名 以$结束.文件须已打开.此命令会覆盖以前内容\n");
	printf("cat      读文件 用法:cat 文件名,读文件前文件须已打开\n");
	printf("format   格式化文件系统\n");
	printf("exit     退出此文件系统\n");
}

void MK(char fileName[256],int length){ //在当前目录下建文件
	if(length<=0) length=1000;
	string szPWD(pwd);
	string Name(fileName);
	if(Name.compare("root")!=0)
		Create(fileName,length,TypeFile,retIndex(szPWD));
	else printf("不能用root作为文件名\n");
}

void MKIDR(char directoryName[256]){ //在当前目录下建目录
	string szPWD(pwd);
	string Name(directoryName);
	if(Name.compare("root")!=0)
		Create(directoryName,0,TypeCatalog,retIndex(szPWD));
	else printf("不能用root作为目录名\n");
}

void RM(char fileName[256]){ //在当前目录下删除文件
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type!=TypeFile) printf("不能用此命令删除目录\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
		DeleteFile(retIndex(szName));
	else
		printf("此文件不在当前目录下!\n");
	}
	else
		printf("此文件不存在!\n");
}

void RMDIR(char directoryName[256]){ //在当前目录下删除下级目录
	string szName(directoryName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type==TypeFile) printf("不能用此命令删除文件\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
		DeleteCatalog(retIndex(szName));
	else
		printf("此目录不在当前目录下!\n");
	}
	else
		printf("此目录不存在!\n");

}

void ReadFile(int index){//读文件
	int base=sizeof(FCB)+sizeof(Fat);
	int tmp[20];
	int k=QingKongFat(tmp,FCB[index].iNextBlock);
	int j=0;
	for(int i=0;i<k;i++)
		for(int j=0;j<Block&&*(p+base+tmp[i]*Block+j)!='\0';j++)  
			cout<<*(p+base+tmp[i]*Block+j);
	printf("\n");
}

void WriteFile(int index){//写文件
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

void OPENFILE(char fileName[256]){//打开文件
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
		if(FCB[retIndex(szName)].Type!=TypeFile) printf("目录不存在打开\n");
		else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
		{
			if(FCB[retIndex(szName)].IsOpend==1)
				printf("此文件已经被打开\n");
			else
				FCB[retIndex(szName)].IsOpend=1;
		}
		else
			printf("此文件不在当前目录下!\n");
	}
	else
		printf("此文件不存在!\n");
}

void CLOSEFILE(char fileName[256]){//关闭文件
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type!=TypeFile) printf("目录不存在关闭\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
	{
		if(FCB[retIndex(szName)].IsOpend!=1)
			printf("此文件已经被被关闭\n");
		else
			FCB[retIndex(szName)].IsOpend=0;
	}
	else
		printf("此文件不在当前目录下!\n");
	}
	else
		printf("此文件不存在!\n");
}

void CAT(char fileName[256]){ //显示文件的内容
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type!=TypeFile) printf("不能用此命令读目录\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
	{
		if(FCB[retIndex(szName)].IsOpend==1)
			ReadFile(retIndex(szName));
		else
			printf("此文件未打开,请先open it\n");
	}
	else
		printf("此文件不在当前目录下!\n");
	}
	else
		printf("此文件不存在!\n");

}

void VI(char fileName[256]){ //修改文件的内容
	string szName(fileName);
	string szPWD(pwd);
	if(retIndex(szName)>=0){
	if(FCB[retIndex(szName)].Type!=TypeFile) printf("不能用此命令读目录\n");
	else if(FCB[retIndex(szName)].iFatherIndex==retIndex(szPWD))
	{
		if(FCB[retIndex(szName)].IsOpend==1)
			WriteFile(retIndex(szName));
		else
			printf("此文件未打开,请先open it\n");
	}
	else
		printf("此文件不在当前目录下!\n");
	}
	else
		printf("此文件不存在!\n");

}

void CD(char directoryName[256]){//cd 下一节目录
	string s=directoryName;
	if(retIndex(s)==-2||retIndex(s)!=-2&&FCB[retIndex(s)].iFatherIndex!=retIndex(pwd)||FCB[retIndex(s)].Type==TypeFile)
	{printf("不存在此目录\n");return;}
	::memset(pwd,'\0',s.size()+1);
	::memcpy(pwd,directoryName,s.size());
}

void CDDDot(void){//回到上级目录
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

int parseCommand(string command,string name,int length){//解析执行命令
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

void InputCommand(void){//接受命令输入、解析命令、显示当前目录路径
	char command[256];
	printf("欢迎使用简单文件系统v1.0.0\n");
	printf("(c) 版权所有  夏斯华 010155 \n");
	cout<<"********************************************************************************"<<endl;
	cout<<"简介:假定系统格式化后有根目录root,在此下可以建文件以及目录.                   "<<endl;
	cout<<"     目录下面还可以再建文件和目录，以此类推。直至没有空间为止.                "<<endl;
	cout<<"     目录结构似象Linux的树型.命令采用了Linux的名字.具体的可以打?/help回车查看 "<<endl;
	cout<<"预设:每个物理块大小:1000字节;最多文件和目录数为100个;文件和目录及相互不要同名 "<<endl;
	cout<<"********************************************************************************"<<endl;
	cout<<"                                                              "<<endl;
		
	while(true){
		//显示到当前目录的路径
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
		init();//初始化文件系统
		if(Load()==-1) Save();
		InputCommand();		
	}
	return nRetCode;
}


