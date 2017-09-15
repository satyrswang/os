#include <iostream>
#include <string>
using namespace std;


//	FAT��ṹ
struct FAT
{	
	bool used;			// λͼ
	int  nextBlock;     // ��һ���������
	int  offset;		// �ڿ��ڵ�ƫ��
	FAT()				//��ʼ��
	{
		used = false;
		nextBlock = -1;
		offset = 0;
	}
};

// ��Ľṹ
struct BLOCK
{
	char content[1024];  //һ����Ϊ1024���ֽ�
};

// ����һЩ����
static int BLOCKSIZE = 1024;	// ��ĳ���
static int NAMESIZE = 64;		// �ļ�������
static int BLOCKNUMSIZE = 5;    // ��ʾ��ı����ռ�Ŀռ�
static int TAGSIZE = 2;			// ��ʾ ���ļ�����Ŀ¼�ı�ʶ
static int RECORDSIZE = NAMESIZE+BLOCKNUMSIZE; //�ڿ���һ����¼�ĳ���

// ��ʾ��ǰ·�������е��ļ���Ŀ¼
void dispCurrentDirectory(FAT *pFat, BLOCK *pBlock, int currentDirectory)
{

	char* p = (char*)(pBlock+currentDirectory);

	char blocknum[5]; // ���ŵĳ���
	char name[64];    // �ļ����ĳ���
	int cnt = 0;

	memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);

	while (strcmp(blocknum,"-1")!=0)
	{
		memcpy(name,p,NAMESIZE);
		p = p+RECORDSIZE;
		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
		++ cnt;
		cout  << name << endl;
	}
	if(cnt ==0)
	{
		cout << "        " << "no file or directory in this directory" << endl<< endl;
	}
	else
	{
		cout << "        " << cnt << " files and directories"  << endl << endl;
	}
}

// ��FAT���в��ҿ��е���
int findFreeSpace(FAT *pFat)
{
	FAT *pfat = pFat;

	int blockID = 0;
	while(pfat->used==true)
	{
		++blockID;
		++pfat;
	}
	pfat->used = true;

	return blockID;
}

// �Ӽ��̵õ��û������룬��Ϊ�ļ�����
void getFileContent(string &filecontent)
{
	cout << "Input the file content: " << endl;
	cout << "(Press Ctrl+Z at the new line to end input)" << endl;
	char c;
	while((c = getchar())!=-1)
	{
		filecontent = filecontent + c;
	}
	int length = filecontent.size();
	filecontent = filecontent.substr(0, length-1);
}

// ��һ���ļ��ĳ��ȳ���һ����Ĵ�Сʱ���޸�FAT���γɼ���������ӣ���ʽ���ӣ�
void modifyFAT(FAT *pFat, int oldBlockID, int blockID)
{
	FAT *pfat = pFat;
	pfat[oldBlockID].nextBlock = blockID;
}

// �ж��Ƿ����㹻�Ŀռ�
bool hasEnoughBlocks(FAT *pFat, int blockNumNeeded)
{
	FAT *pfat = pFat;
	int i = 0;
	int cntFreeBlock = 0;

	while(i < 1024)
	{
		if(pfat->used == false)
		{
			cntFreeBlock++;
			if(cntFreeBlock>blockNumNeeded)
			{
				return true;
			}
		}
		++pfat;
		++i;
	}
	return false;
}

// �����ļ�
bool mkfile(FAT *pFat, BLOCK *pBlock, int currentDirectory,string filename, string& msg, int cur)
{

	char* p = (char*)(pBlock+currentDirectory);

	// �ж�ͬһ��Ŀ¼���Ƿ�����
	char blocknum[5];
	char existedname[64];
	memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
	memcpy(existedname,p,NAMESIZE);
	while (strcmp(blocknum,"-1")!=0)
	{
		if(strcmp(existedname,filename.c_str())==0)
		{
			msg = "The name is used, please choose another name";
			return false;
		}

		p = p+RECORDSIZE;

		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
		memcpy(existedname,p,NAMESIZE);
	}


	// �õ��ļ�����
	string filecontent;
	getFileContent(filecontent);


	int filesize = filecontent.size();

	int blockNumNeeded = filesize / (BLOCKSIZE+1) + 1;

	// �����ļ����ȣ��õ���Ҫ��block��
//	cout << "blockNumNeeded: " << blockNumNeeded << endl;
	
	if(!hasEnoughBlocks(pFat,blockNumNeeded))
	{
		msg = "No enough Space!";
		return false;
	}
	
	// �õ�FCB��block
	int FCBBlockID = findFreeSpace(pFat);
	char strFCBBlockID[5];
	sprintf(strFCBBlockID,"%d",FCBBlockID);

	// ��дĿ¼�ṹ

	memcpy(p,filename.c_str(),NAMESIZE);
	memcpy(p+NAMESIZE,strFCBBlockID,BLOCKNUMSIZE);
	memcpy(p+RECORDSIZE+NAMESIZE,"-1\0",BLOCKNUMSIZE);


	// �õ����п��
	int firstBlockID = findFreeSpace(pFat);
	// int ת��Ϊ char
	char strFirstBlockID[5];
	sprintf(strFirstBlockID,"%d",firstBlockID);

	p = (char*)(pBlock+FCBBlockID);
	memcpy(p,filename.c_str(),NAMESIZE);
	memcpy(p+NAMESIZE,"0\0",TAGSIZE);
	memcpy(p+NAMESIZE+TAGSIZE,strFirstBlockID,BLOCKNUMSIZE);

		
	int blockID = firstBlockID;
	int index = 0;
	while(blockNumNeeded!=0)
	{
		//���ļ����ݰ���Ĵ�С�ָ�
		string smallcontent;
	
		smallcontent = filecontent.substr(5*index,BLOCKSIZE);
		++index;
	
		// ��block����д�ļ�����
		p = (char*)pBlock;
		memcpy(p+blockID*sizeof(BLOCK),smallcontent.c_str(),BLOCKSIZE);

		// ������һ��
		--blockNumNeeded;
		if(blockNumNeeded>0)
		{
			int oldBlockID = blockID;
			blockID = findFreeSpace(pFat);
			modifyFAT(pFat, oldBlockID, blockID);
		}
	}
    cout << "The file is saved!" << endl << endl;
	return true;
}


// ���ļ��ĳ��ȳ���һ����ʱ����FAT���еõ���һ����
int getNextBlock(FAT *pFat, int blocknumber)
{
	FAT *pfat = pFat;
	int nextblock = pfat[blocknumber].nextBlock;
	return nextblock;
}

// �ж����ļ�����Ŀ¼
bool isFile(BLOCK *pBlock, char* blocknum)
{
	int blockNumber = atoi(blocknum);
	char* p = (char*)(pBlock+blockNumber);
	char tag[2];

	memcpy(tag, p+NAMESIZE,TAGSIZE);
	if((strcmp(tag,"1"))==0)
	{
		return false;
	}
		return true;
}


// ɾ���ļ�
bool deleteFile(FAT *pFat, BLOCK *pBlock, int currentDirectory,string filename,string& msg, int cur)
{

	char* p = (char*)(pBlock+currentDirectory);
	FAT *pfat = pFat;


	char blocknum[5];
	char existedname[64];
	memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
	memcpy(existedname,p,NAMESIZE);

	while (strcmp(blocknum,"-1")!=0 )
	{
		if(strcmp(existedname,filename.c_str())==0)
		{
		
			if(!isFile(pBlock, blocknum))
			{
				msg = "This is not a file!";
				return false;
			}
			break;
		}

		p = p+RECORDSIZE;

		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
		memcpy(existedname,p,NAMESIZE);
		 
	}

	int blocknumber = atoi( blocknum );
	if(!strcmp(existedname,filename.c_str())==0 || (strcmp(blocknum,"-1"))==0)
	{
		// δ�ҵ�Ҫɾ�����ļ�
		msg = "cannot find the file";
		return false;
	}
	else
	{
		// �ҵ�Ҫɾ�����ļ�
		char* pOld = p;
		p = p+RECORDSIZE;
		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);

		while (strcmp(blocknum,"-1")!=0)
		{
			memcpy(pOld,p,NAMESIZE);
			memcpy(pOld+NAMESIZE,p+NAMESIZE,BLOCKNUMSIZE);
			pOld = p;
			p = p+RECORDSIZE;
			memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);

		}
		memcpy(pOld+NAMESIZE,p+NAMESIZE,BLOCKNUMSIZE);
	}
	
	 
	//  ����FAT��

	pfat[blocknumber].used = false;
	char* pp = (char*)(pBlock+blocknumber);
	char startblocknum[5];

	memcpy(startblocknum,pp+NAMESIZE+TAGSIZE,BLOCKNUMSIZE);
	int startblocknumber = atoi( startblocknum );
	int nextblock = getNextBlock(pFat,startblocknumber);


	while (nextblock!=-1)
	{
		pfat[startblocknumber].used = false;
	
		startblocknumber=nextblock;
		nextblock = getNextBlock(pFat,startblocknumber);
	}

	pfat[startblocknumber].used = false;
	
	cout << "The file is deleted!" << endl << endl;

	return true;
}


// ��ʾ�ļ�����
bool dispFile(FAT *pFat, BLOCK *pBlock, int currentDirectory,string filename,string& msg, int cur)
{
	char* p = (char*)(pBlock+currentDirectory);

	char blocknum[5];
	char existedname[64];
	memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
	memcpy(existedname,p,NAMESIZE);

	while (strcmp(blocknum,"-1")!=0)
	{
		if(strcmp(existedname,filename.c_str())==0)
		{
			break;
		}

		p = p+RECORDSIZE;

		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
		memcpy(existedname,p,NAMESIZE);
	}

	if(!strcmp(existedname,filename.c_str())==0 || (strcmp(blocknum,"-1"))==0)
	{
		msg = "cannot find the file";
		return false;
	}

	char fileContent[1024];
	

	char startBlocknum[5];
	memcpy(startBlocknum, p+NAMESIZE, BLOCKNUMSIZE);
	int blocknumber2 = atoi( startBlocknum );
	p = (char*)(pBlock+blocknumber2);
	
	memcpy(startBlocknum, p+NAMESIZE+TAGSIZE, BLOCKNUMSIZE);

	int blocknumber = atoi( startBlocknum );


	int nextblock = getNextBlock(pFat,blocknumber);
	
	while (nextblock!=-1)
	{

		char* pp = (char*) pBlock;
		memcpy(fileContent,pp+BLOCKSIZE*blocknumber,BLOCKSIZE);

		fileContent[BLOCKSIZE]='\0';
		cout << fileContent << endl ;

		blocknumber=nextblock;
		nextblock = getNextBlock(pFat,blocknumber);
	}
	char* pp = (char*) pBlock;
	memcpy(fileContent,pp+BLOCKSIZE*blocknumber,BLOCKSIZE);
	fileContent[BLOCKSIZE]='\0';
	cout << fileContent << endl ;

	return true;
}

// ������Ŀ¼
bool mkDirectory(FAT *pFat, BLOCK *pBlock, int currentDirectory,string directoryName,string& msg, int cur)
{
	char* p = (char *)(pBlock+currentDirectory);
	// �ж�ͬһ��Ŀ¼���Ƿ�����
	char blocknum[5];
	char existedname[64];
	memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
	memcpy(existedname,p,NAMESIZE);
	while (strcmp(blocknum,"-1")!=0)
	{
		if(strcmp(existedname,directoryName.c_str())==0)
		{
			msg = "The name is used, please choose another name";
			return false;
		}

		p = p+RECORDSIZE;

		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
		memcpy(existedname,p,NAMESIZE);
	}


	if(!hasEnoughBlocks(pFat,2))
	{
		msg = "No enough Space!";
		return false;
	}


	// �õ�FCB��block
	int FCBBlockID = findFreeSpace(pFat);
	char strFCBBlockID[5];
	sprintf(strFCBBlockID,"%d",FCBBlockID);

	// ��дĿ¼�ṹ
	memcpy(p,directoryName.c_str(),NAMESIZE);
	memcpy(p+NAMESIZE,strFCBBlockID,BLOCKNUMSIZE);
	memcpy(p+RECORDSIZE+NAMESIZE,"-1\0",BLOCKNUMSIZE);

	// �õ����п��
	int firstBlockID = findFreeSpace(pFat);
	// int ת��Ϊ char
	char strFirstBlockID[5];
	sprintf(strFirstBlockID,"%d",firstBlockID);


	p = (char*)(pBlock+FCBBlockID);
	memcpy(p, directoryName.c_str(),NAMESIZE);
	memcpy(p+NAMESIZE,"1\0",TAGSIZE);
	memcpy(p+NAMESIZE+TAGSIZE,strFirstBlockID,BLOCKNUMSIZE);

	

	char* pp = (char*) (pBlock+firstBlockID);

	char strCur[5];
	sprintf(strCur,"%d", currentDirectory);
	memcpy(pp,".",NAMESIZE);
	memcpy(pp+NAMESIZE,strCur,BLOCKNUMSIZE);
	
	sprintf(strCur,"%d", cur);
	memcpy(pp+RECORDSIZE,"..",NAMESIZE);
	memcpy(pp+RECORDSIZE+NAMESIZE,strCur,BLOCKNUMSIZE);

	memcpy(pp+2*RECORDSIZE+NAMESIZE,"-1\0",BLOCKNUMSIZE);

	return true;
}

// �õ���һ��FCB���ڵĿ��
int getPrevFCBNum(BLOCK *pBlock, char* blocknum,string& prompt)
{
	int blockNumber = atoi(blocknum);
	char* p = (char*)(pBlock+blockNumber);
	
	char block[5];
	memcpy(block, p+NAMESIZE+TAGSIZE, BLOCKNUMSIZE);

	int pos = prompt.find_first_of("\\",0);
	int pos1 = prompt.find_last_of("\\");
	string temp1 = prompt.substr(0,pos1);
	int pos2 = temp1.find_last_of("\\");
	if(pos2==-1)
	{
		prompt = "MyFS:\\>";
	}
	else
	{
		prompt = prompt.substr(0,pos1)+">";
	}

	int num = atoi(block);
	return num;
}

// �õ�FCB���ڵĿ��
int getFCBBlockNum(BLOCK *pBlock, char* blocknum,string& prompt)
{
	int blockNumber = atoi(blocknum);

	char* p = (char*)(pBlock+blockNumber);
	char strResult[5];
	char name[64];
	memcpy(name,p,NAMESIZE);
	memcpy(strResult, p+NAMESIZE+TAGSIZE,BLOCKNUMSIZE);
	int result = atoi(strResult);
	

	int length = prompt.size();
	string temp;
	char c = prompt.at(length-2);
	if(c == '\\')
	{
		temp = prompt.substr(0, length-2);
	}
	else
	{
		temp = prompt.substr(0, length-1);
	}
	prompt = temp+"\\"+name+">";

	return result;

}

// �ı���Ŀ¼
bool changeDirectory(FAT *pFat, BLOCK *pBlock, int& currentDirectory,string directoryName,string& msg,string& prompt, int& cur)
{
	char* p = (char*)(pBlock+currentDirectory);

	if(directoryName == ".")
	{
		return true;
	}
	else if(directoryName == ".." && currentDirectory == 1)
	{
		return true;
	}
	else if(directoryName == "..")
	{
		char* ppp = (char*)(pBlock+1);

		char blocknum[5];
		char existedname[64];
		int nextFCBBlock;
		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
		memcpy(existedname,p,NAMESIZE);
		while (strcmp(blocknum,"-1")!=0)
		{
		if(strcmp(existedname,directoryName.c_str())==0)
		{
			
			nextFCBBlock = atoi(blocknum);
			break;
		}
		p = p+RECORDSIZE;
		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
		memcpy(existedname,p,NAMESIZE);
		}

		int blocknumber = atoi( blocknum );

	if(!strcmp(existedname,directoryName.c_str())==0 || strcmp(blocknum,"-1")==0)
	{
		// δ�ҵ�Ҫɾ�����ļ�
		msg = "cannot find the directory!";
		return false;
	}
	else
	{
	}
	
	int num = getPrevFCBNum(pBlock, blocknum,prompt);

	cur = blocknumber;
	currentDirectory = num;

	return true;
	}

	char blocknum[5];
	char existedname[64];
	int nextFCBBlock;
	memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
	memcpy(existedname,p,NAMESIZE);

	while (strcmp(blocknum,"-1")!=0)
	{
		if(strcmp(existedname,directoryName.c_str())==0)
		{
			
			// Ҫ�ж����ļ�����Ŀ¼
			
			if(isFile(pBlock, blocknum))
			{
				msg = "This is not a directory!";
				return false;
			}
			nextFCBBlock = atoi(blocknum);
	
			break;
		}

		p = p+RECORDSIZE;

		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
		memcpy(existedname,p,NAMESIZE);
		 
	}

	int blocknumber = atoi( blocknum );
	if(!strcmp(existedname,directoryName.c_str())==0)
	{
		// δ�ҵ�Ҫɾ�����ļ�
		msg = "cannot find the directory!";
		return false;
	}
	else
	{
	}

	int num = getFCBBlockNum(pBlock, blocknum, prompt);
	cur = nextFCBBlock;
	currentDirectory = num;
	return true;
}

// ɾ����Ŀ¼
bool deleteDirectory(FAT *pFat, BLOCK *pBlock, int& currentDirectory,string directoryName,string& msg, int cur)
{
	char* p = (char*)(pBlock+currentDirectory);
	FAT *pfat = pFat;
	char blocknum[5];
	char existedname[64];
	memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
	memcpy(existedname,p,NAMESIZE);

	while (strcmp(blocknum,"-1")!=0)
	{
		if(strcmp(existedname,directoryName.c_str())==0)
		{
			// Ҫ�ж����ļ�����Ŀ¼
			
			if(isFile(pBlock, blocknum))
			{
				msg = "This is not a directory!";
				return false;
			}
	
			break;
		}

		p = p+RECORDSIZE;

		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
		memcpy(existedname,p,NAMESIZE);
		 
	}

	int blocknumber = atoi( blocknum );
	if(!strcmp(existedname,directoryName.c_str())==0)
	{
		// δ�ҵ�Ҫɾ�����ļ�
		msg = "cannot find the directory!";
		return false;
	}
	else
	{
	}

	cout << "delete all files and directories in this directorie? (Y/N)" ;
	char c; 
	cin >> c;
	if (c =='y' || c =='Y')
	{
		char* pOld = p;
		p = p+RECORDSIZE;
		memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
//		memcpy(existedname,p,NAMESIZE);
		while (strcmp(blocknum,"-1")!=0)
		{
			memcpy(pOld,p,NAMESIZE);
			memcpy(pOld+NAMESIZE,p+NAMESIZE,BLOCKNUMSIZE);
			pOld = p;
			p = p+RECORDSIZE;
			memcpy(blocknum,p+NAMESIZE,BLOCKNUMSIZE);
//		memcpy(existedname,p,NAMESIZE);
		}
		memcpy(pOld+NAMESIZE,p+NAMESIZE,BLOCKNUMSIZE);
		

		pfat[blocknumber].used = false;
		char* pp = (char*)(pBlock+blocknumber);

		char startblocknum[5];

	memcpy(startblocknum,pp+NAMESIZE+TAGSIZE,BLOCKNUMSIZE);
	int startblocknumber = atoi( startblocknum );
	pfat[startblocknumber].used = false;
	getchar();
	cout << "The directory is deleted! " << endl << endl;
	}
	else
	{

		getchar();
//		return true;
	}


	return true;
}

// ���浽����
void SavetoDisk(FAT *pFat, BLOCK *pBlock)
{

	FILE *stream;
	if((stream = fopen("block.dat","w+b")) != NULL)
	{
		fwrite(pBlock,sizeof(BLOCK),1024,stream);
		fclose(stream);
	}	
	stream = NULL;
	if((stream = fopen("fat.dat","w+b")) != NULL)
	{
		fwrite(pFat,sizeof(FAT),1024,stream);
		fclose(stream);
	}
	cout << "save the file system successfully!" << endl;


}

bool LoadFromDisk(FAT *pFat, BLOCK *pBlock)
{
		bool failed = false;
		FILE *stream;
		// ���ļ�����block����Ϣ
		if((stream = fopen("block.dat","r+b")) != NULL)
		{
			fread(pBlock,sizeof(BLOCK),1024,stream);
			fclose(stream);
		}	
		else
		{
			failed = true;
		}
		stream = NULL;
		// ���ļ�����FAT�����Ϣ
		if((stream = fopen("fat.dat","r+b")) != NULL)
		{

			fread(pFat,sizeof(FAT),100,stream);
			fclose(stream);
		}
		else
		{
			failed = true;
		}
		if(failed == true)
		{
			cout << "fail to load the file system! Maybe the file doesn't exist!" << endl;
		}
		else
		{
			cout << "load the file system successfully!" << endl;
		}
//		getchar();
		return failed;
}

void printInstruction()
{
	cout << "         **************************************************************     " << endl;
	cout << "         *                     Instructions                           *     " << endl;
	cout << "         *                                                            *     " << endl;
	cout << "         *       mkfile <fname>  --------  create a file              *     " << endl;
	cout << "         *       del    <fname>  --------  delete a file              *     " << endl;
	cout << "         *       md     <dname>  --------  create a directory         *     " << endl;
	cout << "         *       rd     <dname>  --------  delete a directory         *     " << endl;
	cout << "         *       cd     <dname>  --------  change a directory         *     " << endl;
	cout << "         *       type   <fname>  --------  disp   a file content      *     " << endl;
	cout << "         *       dir    <dname>  --------  disp files and directories *     " << endl;
	cout << "         *       save            --------  save the file system       *     " << endl;
	cout << "         *       load            --------  load the file system       *     " << endl;
	cout << "         *       exit            --------  exit the system            *     " << endl;
	cout << "         **************************************************************     " << endl;
	cout << endl;
	cout << "                                    1.5+2.5        �����      012574       " << endl;
	cout << endl << endl;
}
 



// ������
int main()
{
	printInstruction();


	FAT *pFat = new FAT[1024];
	BLOCK *pBlock = new BLOCK[1024];

	FAT* pfat=pFat;
	BLOCK *pblock = pBlock;

	string prompt = "MyFS:\\>";

	bool failed = false;
	char choice;
	// �Ƿ���ļ������ļ�ϵͳ
	cout<<"Load the file system from the HardDisk? (Y/N) ";
	cin.get(choice);
	
	if(choice == 'Y' || choice == 'y')
	{
		failed = LoadFromDisk(pFat, pBlock);
		
	}
	getchar();
	if((choice != 'Y' && choice != 'y')|| failed == true)
	{
	// ���ļ�ʧ�ܣ����߲�Ҫ���ļ�ʱ�����ļ�ϵͳ���г�ʼ��	
	pFat = new FAT[1024];
	pBlock = new BLOCK[1024];

	char* p = (char*)pBlock;
	pfat = pFat;
// ��ʼ��FAT���0��
	pfat[0].used = true;
	

// ��ʼ����0�����ݿ� FCB
	memcpy(p,prompt.c_str(),NAMESIZE);
	memcpy(p+NAMESIZE,"1\0",TAGSIZE);
	memcpy(p+NAMESIZE+TAGSIZE,"1\0",BLOCKNUMSIZE);

// ��ʼ��FAT��1��

	pfat=pFat;
	pfat[1].used=true;

// ��ʼ����1�����ݿ� 
	p = (char*)(pBlock+1);
	memcpy(p+NAMESIZE,"-1\0",BLOCKNUMSIZE);

	}
// ��ʼ����ǰĿ¼ָ��
	int cur	= 0;
	int currentDirectory = 1;

//	dispCurrentDirectory(pFat, pBlock, currentDirectory);

// ѭ����ʼ�������û�����
	while(true)
	{
	string input;
	string command;
	cout << prompt;
	char c;
	while ((c = getchar()) != '\n')
	{
		input = input + c;
	}
 
	int pos = input.find_first_of(" ");
	
	command = input.substr(0,pos);

	if (command == "mkfile")	// �½��ļ�
	{
		if(pos==-1)
		{
			cout << "please input a file name!" << endl;
			continue;
		}
		int pos1 = input.find_first_not_of(" ",pos);
		if(pos1 == -1)
		{
			cout << "please input a file name!" << endl;
			continue;
		}
		int pos2 = input.find_first_of(" ",pos1);

		string filename = input.substr(pos1,pos2);

		if(filename == "")
		{
			cout << "please input a file name!" << endl;
		}

		string msg;
		if(!mkfile(pFat, pBlock, currentDirectory, filename,msg,cur))
		{
			cout << msg << endl;
			continue;
		}

		
	}
	else if (command == "dir")		// ��ʾ��ǰĿ¼�µ������ļ���Ŀ¼
	{
		pblock = pBlock;
		dispCurrentDirectory(pFat, pblock, currentDirectory);
	}
	else if (command == "type")		// ��ʾ�ļ�����
	{
		if(pos==-1)
		{
			cout << "please input a file name!" << endl;
			continue;
		}
		int pos1 = input.find_first_not_of(" ",pos);
		if(pos1 == -1)
		{
			cout << "please input a file name!" << endl;
			continue;
		}
		int pos2 = input.find_first_of(" ",pos1);

		string filename = input.substr(pos1,pos2);

		if(filename == "")
		{
			cout << "please input a file name!" << endl;
		}
		string msg;
		if(!dispFile(pFat, pBlock, currentDirectory, filename,msg,cur))
		{
			cout << msg << endl;
			continue;
		}

	}
	else if(command == "del")		//ɾ���ļ�
	{
		if(pos==-1)
		{
			cout << "please input a file name!" << endl;
			continue;
		}
		int pos1 = input.find_first_not_of(" ",pos);
		if(pos1 == -1)
		{
			cout << "please input a file name!" << endl;
			continue;
		}
		int pos2 = input.find_first_of(" ",pos1);
		string filename = input.substr(pos1,pos2);

		
		string msg;
		if(!deleteFile(pFat, pBlock, currentDirectory, filename,msg,cur))
		{
			cout << msg << endl;
			continue;
		}
	}
	else if(command == "md")		//������Ŀ¼
	{
		if(pos==-1)
		{
			cout << "please input a file name!" << endl;
			continue;
		}
		int pos1 = input.find_first_not_of(" ",pos);
		if(pos1 == -1)
		{
			cout << "please input a file name!" << endl;
			continue;
		}
		int pos2 = input.find_first_of(" ",pos1);
		string directoryName = input.substr(pos1,pos2);
		string msg;
		if(!mkDirectory(pFat, pBlock, currentDirectory, directoryName, msg,cur))
		{
			cout << msg << endl;
			continue;
		}
		
	}
	else if(command == "cd")		// �任��Ŀ¼
	{
		if(pos==-1)
		{
			cout << "please input a directory name!" << endl;
			continue;
		}
		int pos1 = input.find_first_not_of(" ",pos);
		if(pos1 == -1)
		{
			cout << "please input a directory name!" << endl;
			continue;
		}
		int pos2 = input.find_first_of(" ",pos1);
		string directoryName = input.substr(pos1,pos2);

		string msg;
		int pos3 = directoryName.find_first_of(" ",0);
		if(pos3 != -1)
		{

		}
		
		else
		{
		if(!changeDirectory(pFat, pBlock, currentDirectory, directoryName, msg, prompt,cur))
		{
			cout << msg << endl;
			continue;
		}
		}
	}
	else if(command == "rd")			//ɾ����Ŀ¼
	{
		if(pos==-1)
		{
			cout << "please input a directory name!" << endl;
			continue;
		}
		int pos1 = input.find_first_not_of(" ",pos);
		if(pos1 == -1)
		{
			cout << "please input a directory name!" << endl;
			continue;
		}
		int pos2 = input.find_first_of(" ",pos1);
		string directoryName = input.substr(pos1,pos2);
		string msg;
		if(!deleteDirectory(pFat, pBlock, currentDirectory, directoryName, msg,cur))
		{
			cout << msg << endl;
			continue;
		}

	}
	else if (command == "save")			// �ļ�ϵͳ����
	{
		char c;
		cout << "Save the file system to the HardDisk? (Y/N) ";
		cin >> c;
		if(c == 'Y' || c == 'y')
		{
			SavetoDisk(pFat,pBlock);		// �Ƿ���ļ�ϵͳ����
		}
		getchar();
		continue;
	}
	else if (command == "load")			// �����ļ�ϵͳ
	{
		char c;
		cout << "Load the file system from the HardDisk? (Y/N) ";
		cin >> c;
		if(c == 'Y' || c == 'y')
		{
//			pFat *ptempFat = 
			bool failed = LoadFromDisk(pFat,pBlock);		// �Ƿ���ļ�ϵͳ����
			
			if(!failed)
			{
				cur	= 0;
				currentDirectory = 1;
				prompt = "MyFS:\\>";
			}
		}
		getchar();
		continue;
	}
	else if (command == "exit")			// �˳�ϵͳ
	{	
		char c;
		cout << "Save the file system to the HardDisk? (Y/N) ";
		cin >> c;
		if(c == 'Y' || c == 'y')
		{
			SavetoDisk(pFat,pBlock);		// �Ƿ���ļ�ϵͳ����
		}
		free(pFat);
		free(pBlock);
		return 0;
		
	}
	else if(command =="")
	{
	}
	else
	{
		cout << "No such a command!" << endl;
		continue;
	}
	}
	char c;
	cout << "Save the file system to the HardDisk? (Y/N) ";
	cin >> c;
	if(c == 'Y' || c == 'y')
	{
		SavetoDisk(pFat,pBlock);		// �Ƿ���ļ�ϵͳ����
	}
	
	free(pFat);
	free(pBlock);
	return 0;
}