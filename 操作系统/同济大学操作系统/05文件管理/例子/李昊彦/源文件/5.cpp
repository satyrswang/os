#pragma warning (disable:4786)
#include <iostream>
#include <vector>
#include <cstring>
#include <ostream>
#include <istream>
#include <string>
using namespace std;

typedef struct BLOCK {
	char array[80];
	int size;
	int next;
	int number;
	int isUse;
}BLOCK,*PBLOCK;

struct LIST;

typedef struct END {
	int flag;
	char name[80];
	PBLOCK bbegin;
	LIST* lbegin;
	int isUse;
	int bbeginnum;
	int lbeginnum;
	int number;
}END,*PEND;

typedef struct LIST {
	PEND element[50];
	int elementnum[50];
	int isUse;
	int number;
	int digit;
}LIST,*PLIST;

typedef struct FAT {
	int flag;
	char name[80];
	int isUse;
	PLIST list;
	int listnum;
}FAT,*PFAT;

void travel(PEND pEnd)
{
	int flag = pEnd->flag;
	pEnd->isUse = 0;
	if(flag == 0)
	{
		(pEnd->bbegin)->isUse = 0;
	}
	if(flag == 1)
	{
		int count = 0;
		while(count < ((pEnd->lbegin)->digit))
		{
			if(((pEnd->lbegin)->element)[count]->flag == 0)
			{
				(((pEnd->lbegin)->element)[count]->bbegin)->isUse = 0;
			}
			if(((pEnd->lbegin)->element)[count]->flag == 1)
			{
				travel(((pEnd->lbegin)->element)[count]);
			}
			count++;
		}
	}
}

PEND FileFound(vector<string> currentPos,PFAT pFat,PLIST pList,PBLOCK pBlock,PEND pEnd)
{
	string sname;
	char* cname;
	if(currentPos.size() != 0)
	{		
		string name = currentPos[0];
		char* fname = (char*)name.c_str();
		int num = 0;
		PFAT pf = pFat;
		while(1)
		{
			if(pFat->isUse != 0)
			{
				if(strcmp((pFat->name),fname) == 0)
				{
					num = 1;
					break;
				}
			}
			pFat++;
			if((pFat-pf) == 50)
			{
				cout<<"File not found!"<<endl;
				return NULL;
			}
		}
		if(num == 0)
		{
			cout<<"no such directory!"<<endl;
			return NULL;
		}
		PLIST p = pFat->list;
		PEND it;
		int n;
		int digit = 0;
		int pos = -1;
		int lastpos;
		if(currentPos.size() == 1)
		{
			if(((p->element)[0])->flag == 0) 
			{
				cout<<"File found!"<<endl;
				cout<<((p->element)[0])->name<<endl;
				return ((p->element)[0]);
			}
			else
			{
				cout<<"File not found!"<<endl;
				return NULL;
			}
		}
		for(int i = 1;i<=currentPos.size();i++)
		{
			if(i <= currentPos.size()-1)
			{
				for(n=0;n<(p->digit);n++)
				{
					it = (p->element)[n];
					sname = currentPos[i];
					cname = (char*)sname.c_str();
					if(strcmp(it->name,cname) == 0)
					{
						digit = 1;
						pos++;
						break;
					}
				}
				if(digit == 0)
				{
					cout<<"File not found!"<<endl;
					return NULL;
				}
				if((p->element)[pos]->flag != 0)
				{
					p = (p->element)[pos]->lbegin;
				}
				digit = 0;
				lastpos = pos;
				pos = -1;
			}
			else
			{
				cout<<"File found!"<<endl;
				cout<<(p->element)[lastpos]->name<<endl;
				return (p->element)[lastpos];
			}
		}
	}
	else
	{
		cout<<"File not found!"<<endl;
		return NULL;
	}
	return NULL;
}

void CreatFile(vector<string> currentPos,PFAT pFat,PLIST pList,PBLOCK pBlock,PEND pEnd,char* name,int flag)
{
	PFAT pf = pFat;
	PLIST pl = pList;
	PBLOCK pbl = pBlock;
	PEND pe = pEnd;
	string sname;
	char* cname;
	if(currentPos.empty())
	{
		if(flag == 0)
		{	
			while(1)
			{
				if(pFat->isUse == 0)
					break;
				pFat++;
				if((pFat-pf) == 50)
				{
					cout<<"No enough space!"<<endl;
					return;
				}
			}
			strcpy(pFat->name,name);
			pFat->flag = 0;
			pFat->isUse = 1;
			while(1)
			{
				if(pList->isUse == 0)
					break;
				pList++;
				if((pList-pl) == 50)
				{
					cout<<"No enough space!"<<endl;
					return;
				}
			}
			pFat->list = pList;
			pFat->listnum = pList->number;
			(pFat->list)->isUse = 1;
			while(1)
			{
				if(pEnd->isUse == 0)
					break;
				pEnd++;
				if((pEnd-pe) == 50)
				{
					cout<<"No enough space!"<<endl;
					return;
				}
			}
			((pFat->list)->element)[(pFat->list)->digit] = pEnd;
			((pFat->list)->elementnum)[(pFat->list)->digit] = pEnd->number;
			((pFat->list)->digit) += 1;
			while(1)
			{
				if(pBlock->isUse == 0)
					break;
				pBlock++;
				if((pBlock-pbl) == 50)
				{
					cout<<"No enough space!"<<endl;
					return;
				}
			}
			strcpy((((pFat->list)->element)[0])->name,name);
			(((pFat->list)->element)[0])->flag = 0;
			(((pFat->list)->element)[0])->isUse = 1;
			(((pFat->list)->element)[0])->bbegin = pBlock;
			(((pFat->list)->element)[0])->bbeginnum = pBlock->number;
			pBlock->isUse = 1;
			pBlock->size = 0;
			pBlock->next = -1;
		}
		else
		{
			while(1)
			{
				if(pFat->isUse == 0)
					break;
				pFat++;
				if((pFat-pf) == 50)
				{
					cout<<"No enough space!"<<endl;
					return;
				}
			}
			strcpy(pFat->name,name);
			pFat->flag = 1;
			pFat->isUse = 1;
			while(1)
			{
				if(pList->isUse == 0)
					break;
				pList++;
				if((pList-pl) == 50)
				{
					cout<<"No enough space!"<<endl;
					return;
				}
			}
			pFat->list = pList;
			pFat->listnum = pList->number;
			(pFat->list)->isUse = 1;
		}
	}
	else
	{	
		string tname = currentPos[0];
		char* fname = (char*)tname.c_str();
		int num = 0;
		while(1)
		{
			if(pFat->isUse != 0)
			{
				if(strcmp((pFat->name),fname) == 0)
				{
					num = 1;
					break;
				}
			}
			pFat++;
			if((pFat-pf) == 50)
			{
				cout<<"No enough space!"<<endl;
				return;
			}
		}
		if(num == 0)
		{
			cout<<"no such directory!"<<endl;
			return;
		}
		PLIST p = pFat->list;
		PEND it;
		int n;
		int digit = 0;
		int pos = -1;
		int all = 0;
		for(int i = 0;i<currentPos.size();i++)
		{
			if(i < currentPos.size()-1)
			{
				for(n=0;n<(p->digit);n++)
				{
					it = (p->element)[n];
					sname = currentPos[i+1];
					cname = (char*)sname.c_str();
					if(strcmp(it->name,cname) == 0)
					{
						digit = 1;
						pos++;
						break;
					}
				}
				if(digit == 0)
				{
					cout<<"No such directory!"<<endl;
					return;
				}
				p = ((p->element)[pos])->lbegin;
				digit = 0;
				pos = -1;
			}
			else
			{
				while(1)
				{
					if(pEnd->isUse == 0)
						break;
					pEnd++;
					if((pEnd-pe) == 50)
					{
						cout<<"No enough space!"<<endl;
						return;
					}
				}
				(p->element)[p->digit] = pEnd;
				(p->elementnum)[p->digit] = pEnd->number;
				(p->digit) += 1;
				((p->element)[(p->digit)-1])->isUse = 1;
				strcpy(((p->element)[(p->digit)-1])->name,name);
				((p->element)[(p->digit)-1])->flag = flag;
				p->isUse = 1;
				if(flag == 1)
				{
					while(1)
					{
						if(pList->isUse == 0)
							break;
						pList++;
						if((pList-pl) == 50)
						{
							cout<<"No enough space!"<<endl;
							return;
						}
					}
					pList->isUse = 1;
					((p->element)[(p->digit)-1])->lbegin = pList;
					((p->element)[(p->digit)-1])->lbeginnum = pList->number;
				}
				if(flag == 0)
				{
					while(1)
					{
						if(pBlock->isUse == 0)
							break;
						pBlock++;
						if((pBlock-pbl) == 50)
						{
							cout<<"No enough space!"<<endl;
							return;
						}
					}
					pBlock->isUse = 1;
					pBlock->size = 0;
					pBlock->next = -1;
					((p->element)[(p->digit)-1])->bbegin = pBlock;
					((p->element)[(p->digit)-1])->bbeginnum = pBlock->number;
				}
			}
		}
	}
}

vector<char*> LookMenu(vector<string> currentPos,PFAT pFat,PLIST pList,PBLOCK pBlock,PEND pEnd,int flag)
{
	PFAT pf = pFat;
	string sname;
	char* cname;
	if(currentPos.size() != 0)
	{
		string name = currentPos[0];
		char* fname = (char*)name.c_str();
		int num = 0;
		while(1)
		{
			if(pFat->isUse != 0)
			{
				if(strcmp((pFat->name),fname) == 0)
				{
					num = 1;
					break;
				}
			}
			pFat++;
			if((pFat-pf) == 50)
			{
				cout<<"No enough space!"<<endl;
				vector<char*> s;
				return s;
			}
		}
		if(num == 0)
		{
			cout<<"no such directory!"<<endl;
			vector<char*> s;
			return s;
		}
		PLIST p = pFat->list;
		PEND it;
		int n;
		int digit = 0;
		int pos = -1;
		if(currentPos.size() == 1)
		{
			vector<char*> s;
			for(int j = 0;j<(p->digit);j++)
			{
				if(flag == 1)
				{
					if(((p->element)[j])->flag == 1)
					{
						cout<<"<DIR>"<<"	";
					}
					else
					{
						cout<<"<FILE>"<<"	";
					}
					cout<<((p->element)[j])->name<<endl;
				}
				if(((p->element)[j])->flag == 1)
				{
					s.push_back(((p->element)[j])->name);
				}
			}
			return s;
		}
		for(int i = 1;i<=currentPos.size();i++)
		{
			if(i <= currentPos.size()-1)
			{
				for(n=0;n<(p->digit);n++)
				{
					it = (p->element)[n];
					sname = currentPos[i];
					cname = (char*)sname.c_str();
					if(strcmp(it->name,cname) == 0)
					{
						digit = 1;
						pos++;
						break;
					}
				}
				if(digit == 0)
				{
					cout<<"No such directory!"<<endl;
					vector<char*> s;
					return s;
				}
				if((p->element)[pos]->flag == 0)
				{
					cout<<"No such directory!"<<endl;
					vector<char*> s;
					return s;
				}
				p = (p->element)[pos]->lbegin;
				digit = 0;
				pos = -1;
			}
			else
			{
				vector<char*> s;
				for(int j=0;j<(p->digit);j++)
				{
					if(flag == 1)
					{
						if(((p->element)[j])->flag == 1)
						{
							cout<<"<DIR>"<<"	";
						}
						else
						{
							cout<<"<FILE>"<<"	";
						}
						cout<<((p->element)[j])->name<<endl;
					}
					if(((p->element)[j])->flag == 1)
					{
						s.push_back(((p->element)[j])->name);
					}
				}
				return s;
			}
		}
	}
	else
	{
		vector<char*> s;
		while(1)
		{
			if(pFat->isUse != 0)
			{
				if(flag == 1)
				{
					if(pFat->flag == 1)
					{
						cout<<"<DIR>"<<"	";
					}
					else
					{
						cout<<"<FILE>"<<"	";
					}
					cout<<pFat->name<<endl;
				}
				if(pFat->flag == 1)
				{
					s.push_back(pFat->name);
				}
			}
			pFat++;
			if(pFat-pf == 50)
			{
				break;
			}
		}
		return s;
	}
	return NULL;
}

void DeleteFile(vector<string> currentPos,PFAT pFat,PLIST pList,PBLOCK pBlock,PEND pEnd)
{
	PFAT pf = pFat;
	string sname;
	char* cname;
	if(currentPos.size() != 0)
	{
		string name = currentPos[0];
		char* fname = (char*)name.c_str();
		int num = 0;
		while(1)
		{
			if(pFat->isUse != 0)
			{
				if(strcmp((pFat->name),fname) == 0)
				{
					num = 1;
					break;
				}
			}
			pFat++;
			if((pFat-pf) == 50)
			{
				cout<<"Not found!"<<endl;
				return;
			}
		}
		if(num == 0)
		{
			cout<<"Not found!"<<endl;
			return;
		}
		PLIST p = pFat->list;
		int n;
		PEND it;
		int lastpos = 0;
		int digit = 0;
		int pos = 0;
		for(int i = 1;i<=currentPos.size();i++)
		{
			if(i <= currentPos.size()-1)
			{
				for(n=0;n<(p->digit);n++)
				{
					it = (p->element)[n];
					sname = currentPos[i];
					cname = (char*)sname.c_str();
					if(strcmp(it->name,cname) == 0)
					{
						digit = 1;
						pos++;
						break;
					}
				}
				if(digit == 0)
				{
					cout<<"Not found!"<<endl;
					return;
				}
				lastpos = pos;
				if((p->element)[pos]->flag != 0 && i < currentPos.size()-1)
				{
					p = (p->element)[pos]->lbegin;
				}
				digit = 0;
				pos = 0;
			}
			else
			{
				if(i > 1)
				{
					travel((p->element)[lastpos]);
					for(int h = lastpos+1;h<(p->digit);h++)
					{
						(p->element)[h-1] = (p->element)[h];
					}
					(p->element)[(p->digit)-1] = NULL;
					(p->digit) -= 1;
				}
				if(i == 1)
				{
					pFat = pf;
					while(1)
					{
						if(strcmp(pFat->name,fname) == 0)
						{
							pFat->isUse = 0;
							return;
						}
						pFat++;
						if((pFat-pf) == 50)
						{
							cout<<"Not found!"<<endl;
							return;
						}
					}
				}
			}
		}
	}
	else
	{
		cout<<"Not found!"<<endl;
		return;
	}
}

void Initialize(PFAT pFat,PLIST pList,PEND pEnd,PBLOCK pBlock,int size)
{
	for(int i = 0;i<size;i++)
	{
		pFat->isUse = 0;
		pFat->flag = -1;
		pFat->list = NULL;
		pFat->listnum = -1;
		for(int h = 0;h<80;h++)
		{
			(pFat->name)[h] = '\0';
		}
		pFat++;
	}
	for(int j = 0;j<size;j++)
	{
		pList->isUse = 0;
		pList->number = j;
		pList->digit = 0;
		for(int q = 0;q<50;q++)
		{
			(pList->elementnum)[q] = -1;
			(pList->element)[q] = NULL;
		}
		pList++;
	}
	for(int k = 0;k<size;k++)
	{
		pBlock->isUse = 0;
		pBlock->next = -1;
		pBlock->number = k;
		pBlock->size = 0;
		for(int h = 0;h<80;h++)
		{
			(pBlock->array)[h] = '\0';
		}
		pBlock++;
	}
	for(int m = 0;m<size;m++)
	{
		pEnd->isUse = 0;
		pEnd->flag = -1;
		for(int h = 0;h<80;h++)
		{
			(pEnd->name)[h] = '\0';
		}
		pEnd->lbegin = NULL;
		pEnd->bbegin = NULL;
		pEnd->bbeginnum = -1;
		pEnd->lbeginnum = -1;
		pEnd->number = m;
		pEnd++;
	}
}

void OpenFile(vector<string> currentPos,PFAT pFat,PLIST pList,PBLOCK pBlock,PEND pEnd)
{
	PEND p = FileFound(currentPos,pFat,pList,pBlock,pEnd);
	if(p != NULL)
	{
		cout<<"Content:";
		PBLOCK pB = p->bbegin;
		int number;
		do
		{
			for(int k = 0;k<50;k++)
			{
				if(pB->array[k] == '\0')
					break;
				cout<<pB->array[k];
			}
			if(pB->next != -1)
			{
				number = pB->next;
				while(1)
				{
					if(pBlock->number == number)
						break;
					pBlock++;
				}
				pB = pBlock;
			}
			else
			{
				cout<<endl;
				break;
			}
		}
		while(pB->size != 0);
	}
	else
	{
		cout<<"File not found!"<<endl;
	}
}

void Write(vector<string> currentPos,PFAT pFat,PLIST pList,PBLOCK pBlock,PEND pEnd)
{
	PEND p = FileFound(currentPos,pFat,pList,pBlock,pEnd);
	char a;
	if(p != NULL)
	{
		cout<<"Content:";
		for(int k = 0;k<50;k++)
		{
			if((p->bbegin)->array[k] == '\0')
				break;
			cout<<(p->bbegin)->array[k];
		}
		int count = 0;
		PBLOCK pB = p->bbegin;
		PBLOCK currentp = pBlock;
		while((cin.get(a)) && (pB->size < 50))
		{
			if(a != '$')
			{
				pB->array[pB->size] = a;
				pB->size++;
				if(pB->size == 50)
				{
					while(1)
					{
						if(pBlock->isUse == 0)
							break;
						pBlock++;
						count++;
					}
					pB->next = count;
					pBlock->number = count;
					pBlock->size = 0;
					pBlock->isUse = 1;
					pBlock->next = -1;
					for(int h = 0;h<50;h++)
					{
						(pBlock->array)[h] = '\0';
					}
					pB = pBlock;
					count = 0;
					pBlock = currentp;
				}
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		cout<<"File not found!"<<endl;
	}
}

void SavetoFile(PBLOCK pBlock)
{
	FILE *stream;
	if((stream = fopen("block","w+b")) != NULL)
	{
		fwrite(pBlock,sizeof(BLOCK),100,stream);
		fclose(stream);
	}	
}

PBLOCK ReadfromFile(char* name)
{
	int size = 100;
	PBLOCK pBlock = new BLOCK[size];
	FILE *stream;
	if((stream = fopen(name,"r+b")) != NULL)
	{
		fread(pBlock,sizeof(BLOCK),100,stream);
		fclose(stream);
	}	
	return pBlock;
}

void SavetoHardDisk(PFAT pFat,PLIST pList,PBLOCK pBlock,PEND pEnd)
{
	int size = 100;
	
	FILE *stream;
	
	SavetoFile(pBlock);
	
	if((stream = fopen("list","w+b")) != NULL)
	{
		fwrite(pList,sizeof(LIST),100,stream);
		fclose(stream);
	}	
	
	if((stream = fopen("end","w+b")) != NULL)
	{
		fwrite(pEnd,sizeof(END),100,stream);
		fclose(stream);
	}	
	
	if((stream = fopen("fat","w+b")) != NULL)
	{
		fwrite(pFat,sizeof(FAT),100,stream);
		fclose(stream);
	}	
	
}

int main()
{
	int size = 100;
	PFAT pFat;
	PLIST pList;
	PEND pEnd;
	PBLOCK pBlock;
	vector<string> vec;
	cout<<"[Simple File Operation System]"<<endl;
	cout<<"2004-6-10"<<endl;
	cout<<endl;
	
	char input[80];
	char command[80];
	char array[80];
	int count = 0;
	int j = 0;
	int k = 0;
	int flag = 0;
	char* name;
	char choice;
	
	cout<<"Recover the file system? Y/N"<<endl;
	
	cin.get(choice);
	
	if(choice == 'Y' || choice == 'y')
	{
		int i;

		FILE *stream;
		
		pBlock = new BLOCK[size];
		if((stream = fopen("block","r+b")) != NULL)
		{
			fread(pBlock,sizeof(BLOCK),100,stream);
			fclose(stream);
		}	
		
		pList = new LIST[size];
		PLIST pL1 = pList;
		if((stream = fopen("list","r+b")) != NULL)
		{
			fread(pList,sizeof(LIST),100,stream);
			fclose(stream);
		}
		
		pEnd = new END[size];
		PEND pE1 = pEnd;
		if((stream = fopen("end","r+b")) != NULL)
		{
			fread(pEnd,sizeof(END),100,stream);
			fclose(stream);
		}
		for(i = 0;i<size;i++)
		{
			if(pEnd->bbeginnum != -1)
			{
				pEnd->bbegin = pBlock+(pEnd->bbeginnum);
			}
			if(pEnd->lbeginnum != -1)
			{
				pEnd->lbegin = pList+(pEnd->lbeginnum);
			}
			pEnd++;
		}
		pEnd = pE1;
		
		for(i = 0;i<size;i++)
		{
			for(int j = 0;j<(pList->digit);j++)
			{
				if((pList->elementnum)[j] != -1)
				{
					(pList->element)[j] = pEnd+(pList->elementnum)[j];
				}
			}
			pList++;
		}	
		pList = pL1; 
		
		pFat = new FAT[size];	
		if((stream = fopen("fat","r+b")) != NULL)
		{
			fread(pFat,sizeof(FAT),100,stream);
			fclose(stream);
		}
		
		PFAT pf1 = pFat;
		for(i = 0;i<size;i++)
		{
			if((pFat->listnum) != -1)
			{
				pFat->list = pList+(pFat->listnum);
			}
			else
			{
				pFat->list = NULL;
			}
			pFat++;
		}
		pFat = pf1;
		cout<<"Recover the file system successfully!"<<endl;
	}
	if(choice == 'N' || choice == 'n')
	{
		pFat = new FAT[size];
		pList = new LIST[size];
		pEnd = new END[size];
		pBlock = new BLOCK[size];
		Initialize(pFat,pList,pEnd,pBlock,size);
	}
	
	string s = "System:\\";
	s += ">";
	cout<<s;

	while(cin.getline(input,80,'\n'))
	{
		int i = 0;
		while(input[i] != '\0')
		{
			if(input[i] == ' ')
			{
				while(input[i+1] == ' ')
				{
					i++;
				}
				if(flag == 1)
				{
					count++;
				}
				if(flag == 0)
				{
					i++;
				}
			}
			if(count == 0)
			{
				flag = 1;
				command[j] = input[i];
				j++;
			}
			else if(count == 1)
			{
				i++;
				array[k] = input[i];
				k++;
			}
			else
			{
				break;
			}
			if(count == 0)
			{
				i++;
			}
		}
		if(count == 1 || count == 0)
			command[j] = '\0';
		if(count == 2)
		{
			command[j] = '\0';
			array[k] = '\0';
		}
		if(strcmp(command,"dir") == 0)
		{
			char name[80];
			strcpy(name,array);
			LookMenu(vec,pFat,pList,pBlock,pEnd,1);
		}
		else if(strcmp(command,"mkdir") == 0)
		{
			char name[80];
			strcpy(name,array);
			cout<<endl;
			CreatFile(vec,pFat,pList,pBlock,pEnd,name,1);
		}
		else if(strcmp(command,"create") == 0)
		{
			char name[80];
			strcpy(name,array);
			cout<<endl;
			CreatFile(vec,pFat,pList,pBlock,pEnd,name,0);
		}
		else if(strcmp(command,"deldir") == 0)
		{
			char name[80];
			strcpy(name,array);
			string s = name;
			vec.push_back(s);
			DeleteFile(vec,pFat,pList,pBlock,pEnd);
			vec.pop_back();
		}
		else if(strcmp(command,"delete") == 0)
		{
			char name[80];
			strcpy(name,array);
			string s = name;
			vec.push_back(s);
			DeleteFile(vec,pFat,pList,pBlock,pEnd);
			vec.pop_back();
		}
		else if(strcmp(command,"cd") == 0)
		{
			char name[80];
			strcpy(name,array);
			int flag = 0;
			if(LookMenu(vec,pFat,pList,pBlock,pEnd,0).size() != 0)
			{
				for(int i = 0;i<(LookMenu(vec,pFat,pList,pBlock,pEnd,0)).size();i++)
				{
					if(strcmp((LookMenu(vec,pFat,pList,pBlock,pEnd,0))[i],name) == 0)
					{
						flag = 1;
						vec.push_back(name);
						break;
					}
				}
			}
			if(flag == 0)
			{
				cout<<"No such directory!"<<endl;
			}
		}
		else if(strcmp(command,"cd..") == 0)
		{
			if(vec.size() != 0)
			{
				vec.pop_back();
			}
			else
			{
				cout<<"Already come to the root directory!"<<endl;
			}
		}
		else if(strcmp(command,"open") == 0)
		{
			char name[80];
			strcpy(name,array);
			string s = name;
			vec.push_back(s);
			OpenFile(vec,pFat,pList,pBlock,pEnd);
			vec.pop_back();
		}
		else if(strcmp(command,"write") == 0)
		{
			char name[80];
			strcpy(name,array);
			string s = name;
			vec.push_back(s);
			Write(vec,pFat,pList,pBlock,pEnd);
			vec.pop_back();
		}
		else if(strcmp(command,"exit") == 0)
		{
			SavetoHardDisk(pFat,pList,pBlock,pEnd);
			cout<<"Save information to harddisk successfully!"<<endl;
			cout<<"You can exit the file system safely!"<<endl;
			exit(0);
		}
		else if(strcmp(command,"help") == 0)
		{
			cout<<"    help	----	look over the help menu"<<endl;
			cout<<"    dir		----	display files in the directory"<<endl;
			cout<<"    mkdir	----	create new directory"<<endl;
			cout<<"    create	----	create new file"<<endl;
			cout<<"    deldir	----	delete the directory"<<endl;
			cout<<"    delete	----	delete the file"<<endl;
			cout<<"    cd..	----	return the last directory"<<endl;
			cout<<"    open	----	open the file"<<endl;
			cout<<"    write	----	write the file"<<endl;
			cout<<"    exit	----	exit the file operation system"<<endl;
		}
		else
		{
			cout<<"No such command!"<<endl;
		}
		i = 0;
		j = 0;
		k = 0;
		count = 0;
		flag = 0;
		name = "";
		string s = "System:\\";
		for(int j = 0;j<vec.size();j++)
		{
			s += vec[j];
			if(j<vec.size()-1)
				s += "\\";
		}
		s += ">";
		cout<<s;
	}
	
	return 0;
}
