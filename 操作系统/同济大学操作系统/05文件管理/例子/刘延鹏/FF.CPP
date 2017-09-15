#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct FATITEM
{
	int ifill;
	int next;
	int offset;

	FATITEM()
	{
		ifill = 0; //0 is not; 1 is y
		next  = -1;
		offset = 0;
	}
};

struct BLOCK
{
	char block[1024];
};

struct ROOTITEM
{
	char name[11];
	int  head;
	char flag ;

	void changeStringtoChar(string n)
	{
		for(int i = 0 ;i<n.length();i++)
		{
			name[i] = n[i];
		}
		name[i] = '\0';
	}

};

struct MENU
{
	string name;
	int   offset;
};
vector<MENU> vecM;
MENU first;

FATITEM* pFAT = new FATITEM[1024];
BLOCK* pBLOCK = new BLOCK[1024];

BLOCK* p = pBLOCK;

void input(string &command)
{
	cout<<"Virtual OS:\\";
	for(int i = 0; i<vecM.size();i++)
	{
		cout<<vecM[i].name;
		cout<<"\\";
	}
	cout<<">";
	cin>>command;

	for(int j =0;j<command.length();j++)
	{
		if(command[j] >= 'A' && command[j] <= 'Z')
		{
			command[j] = command[j] + 32;
		}
	}
}

void searchEmpty(int &insert)
{
	for(int i = 1 ;i<1024;i++)
	{
		if(pFAT[i].ifill == 0)
		{
			pFAT[i].ifill = 1;
			insert = i;
			return;
		}
	}
	cout<<"No enough Memory"<<endl;
}

void addD(string name)
{
	if(name.size() >10)
	{
		cout<<"the length of this directory name is more than 10"<<endl;
		return ;
	}
	int insert = 0;
	searchEmpty(insert);
	
	//是否有重复的名字
	vector<ROOTITEM> temp;
	int offset = 0;
	ROOTITEM *tp = (ROOTITEM*)p;
	
	char tempname[15];
	bool flag = true;
	while(offset != pFAT[p-pBLOCK].offset)
	{
		temp.push_back(*tp);
		tp++;
		offset++;
	}	
	if(offset != 0)
	{
		for(int j = 0; j<name.length();j++)
		{
			tempname[j] = name[j];
		}
		tempname[j] = '\0';

		for(int i = 0; i<temp.size();i++)
		{
			if(strcmp(tempname,temp[i].name)==0&&temp[i].flag == '1')
			{
				flag = false;
				break;
			}
		}
		if(flag == false)
		{
			cout<<endl<<"      a duplicate directory name"<<endl<<endl;
			temp.clear();
			return ;
		}
	}
	temp.clear();
	////////////
	int doffset = pFAT[p-pBLOCK].offset;
	ROOTITEM *ttemp = (ROOTITEM*)p;
	ttemp = ttemp + doffset;

	ttemp->changeStringtoChar(name);
	ttemp->head = insert;
	ttemp->flag = '1';

	pFAT[p-pBLOCK].offset++;
}

void cdD(string name)
{
	vector<ROOTITEM> temp;
	int offset = 0;
	ROOTITEM *tp = (ROOTITEM*)p;
	
	char tempname[15];
	bool flag = false;

	while(offset != pFAT[p-pBLOCK].offset)
	{
		temp.push_back(*tp);
		tp++;
		offset++;
	}
	
	for(int j = 0; j<name.length();j++)
	{
		tempname[j] = name[j];
	}
	tempname[j] = '\0';

	for(int i = 0; i<temp.size();i++)
	{
		if(strcmp(tempname,temp[i].name)==0&&temp[i].flag == '1')
		{
			flag = true;
			p = p + temp[i].head;
			MENU item;
			item.name = name;
			item.offset = p - pBLOCK;
			vecM.push_back(item);
			break;
		}
	}
	


	if(flag == false)
	{
		cout<<"No such Dictionary"<<endl;
	}

	
	temp.clear();
}

void removeD(string name)
{
	vector<ROOTITEM> temp;
	int offset = 0;
	ROOTITEM *tp = (ROOTITEM*)p;
	
	char tempname[15];
	bool flag = false;

	while(offset != pFAT[p-pBLOCK].offset)
	{
		temp.push_back(*tp);
		tp++;
		offset++;
	}
	
	for(int j = 0; j<name.length();j++)
	{
		tempname[j] = name[j];
	}
	tempname[j] = '\0';
	
	vector<ROOTITEM>::iterator it;

	int eraseroffset = 0;
	for(it = temp.begin();it!=temp.end();it++)
	{
		if(strcmp(it->name,tempname)==0&&it->flag == '1')
		{
			flag = true;
			eraseroffset++;
		//	temp.erase(it);
		//	pFAT[p - pBLOCK].offset--;
			break;
		}
	}
	
	if(flag == false)
	{
		cout<<"No such Dictionary"<<endl;
		return ;
	}

	tp = (ROOTITEM*)p;
	tp = tp + eraseroffset -1;
	tp->flag = '3';

//	for(it = temp.begin();it!=temp.end();it++)
//	{
//		memcpy(tp->name,it->name,11);
//		tp->head = it->head;
//		tp->flag = it->flag;
//	}
}

void cdpp()
{
	vecM.pop_back();
	if(vecM.empty() == true)
	{
		p = pBLOCK;
	}
	else
	{
		p = pBLOCK + vecM[vecM.size()-1].offset;
	}
}

void creatFile(string name)
{
	if(name.size() >10)
	{
		cout<<"the length of this file name is more than 10"<<endl;
		return ;
	}
	int insert = 0;
	searchEmpty(insert);
	
	//是否有重复的名字
	vector<ROOTITEM> temp;
	int offset = 0;
	ROOTITEM *tp = (ROOTITEM*)p;
	
	char tempname[15];
	bool flag = true;
	while(offset != pFAT[p-pBLOCK].offset)
	{
		temp.push_back(*tp);
		tp++;
		offset++;
	}	
	if(offset != 0)
	{
		for(int j = 0; j<name.length();j++)
		{
			tempname[j] = name[j];
		}
		tempname[j] = '\0';

		for(int i = 0; i<temp.size();i++)
		{
			if(strcmp(tempname,temp[i].name)==0&&temp[i].flag == '2')
			{
				flag = false;
				break;
			}
		}
		if(flag == false)
		{
			cout<<endl<<"      a duplicate directory name"<<endl<<endl;
			temp.clear();
			return ;
		}
	}
	temp.clear();
	////////////
	int doffset = pFAT[p-pBLOCK].offset;
	ROOTITEM *ttemp = (ROOTITEM*)p;
	ttemp = ttemp + doffset;

	ttemp->changeStringtoChar(name);
	ttemp->head = insert;
	ttemp->flag = '2';

	pFAT[p-pBLOCK].offset++;
}

void del(string name)
{
	vector<ROOTITEM> temp;
	int offset = 0;
	ROOTITEM *tp = (ROOTITEM*)p;
	
	char tempname[15];
	bool flag = false;

	while(offset != pFAT[p-pBLOCK].offset)
	{
		temp.push_back(*tp);
		tp++;
		offset++;
	}
	
	for(int j = 0; j<name.length();j++)
	{
		tempname[j] = name[j];
	}
	tempname[j] = '\0';
	
	vector<ROOTITEM>::iterator it;

	int eraseroffset = 0;
	for(it = temp.begin();it!=temp.end();it++)
	{
		if(strcmp(it->name,tempname)==0&&it->flag == '2')
		{
			flag = true;
			eraseroffset++;
			temp.erase(it);
			pFAT[p - pBLOCK].offset--;
			break;
		}
	}
	
	if(flag == false)
	{
		cout<<"No such Dictionary"<<endl;
		return ;
	}

	tp = (ROOTITEM*)p;
	tp = tp + eraseroffset -1;
	tp->flag = '3';

	for(it = temp.begin();it!=temp.end();it++)
	{
		memcpy(tp->name,it->name,11);
		tp->head = it->head;
		tp->flag = it->flag;
	}
}
void type(string name)
{
	if(name.size() >10)
	{
		cout<<"the length of this file name is more than 10"<<endl;
		return ;
	}
	vector<ROOTITEM> temp;
	int offset = 0;
	ROOTITEM *tp = (ROOTITEM*)p;
	BLOCK* tempp = p;
	int firsthead = 0;

	char tempname[15];
	bool flag = false;
	while(offset != pFAT[p-pBLOCK].offset)
	{
		temp.push_back(*tp);
		tp++;
		offset++;
	}	
	if(offset != 0)
	{
		for(int j = 0; j<name.length();j++)
		{
			tempname[j] = name[j];
		}
		tempname[j] = '\0';

		for(int i = 0; i<temp.size();i++)
		{
			if(strcmp(tempname,temp[i].name)==0&&temp[i].flag == '2')
			{
				flag = true;
				//p = p + temp[i].head;
				firsthead = temp[i].head;
				break;
			}
		}
		if(flag == false)
		{
			cout<<endl<<"    no such file"<<endl<<endl;
			temp.clear();
			return ;
		}
	}
	else
	{
		cout<<endl<<"    no such file"<<endl<<endl;
		return ;

	}
	temp.clear();
	//文件内容
	vector<char> vec;
	char c;
	c = getchar();
	while((c=getchar())!='1')
	{
		vec.push_back(c);
	}

	int num = 0;
//	int lastoffset = 0;
	if(vec.size()/1024 == 0)
	{
		num = 1;
	}
	else
	{
		if(vec.size()%1024 == 0)
		{
			num = vec.size()/1024;
		}
		else
		{
			num = vec.size()/1024 + 1;
		}
	}
	//写文件
	char* pp = (char*)pBLOCK;
	//BLOCK *pp = pBLOCK;
	vector<int> numvec;

	numvec.push_back(firsthead);
	int insert = 0 ,count = 0;
	int point = 0;

	for(int m = 0 ;m<num-1;m++)
	{
		searchEmpty(insert);
		numvec.push_back(insert);
	}

	for(int n = 0; n<numvec.size();n++)
	{
		pp = (char *)(pBLOCK + numvec[n]);
		pFAT[numvec[n]].offset = 0;
		for(int k = 0;;k++)
		{
			*pp = vec[point];
			pp++;
			point++;
			count++;
			pFAT[numvec[n]].offset++;
			if(count == 1024)
			{
				if(n<numvec.size()-1)
				{
					pFAT[numvec[n]].next = numvec[n+1];
				}
				count = 0;
				break;
			}
			if(count == vec.size())
			{
				break;
			}
		}
	}
	
	p = tempp;
}


void showFile(string name)
{
	vector<ROOTITEM> temp;
	int offset = 0;
	ROOTITEM *tp = (ROOTITEM*)p;
	BLOCK* tempp = p;

	while(offset != pFAT[p-pBLOCK].offset)
	{	
		temp.push_back(*tp);
		tp++;
		offset++;
	}

	char tempname[15];
	bool flag = false;
	int head = 0;

	for(int j = 0; j<name.length();j++)
	{
		tempname[j] = name[j];
	}
	tempname[j] = '\0';

	for(int i = 0; i<temp.size();i++)
	{
		if(strcmp(tempname,temp[i].name)==0&&temp[i].flag == '2')
		{
			flag = true;
			//p = p + temp[i].head;	
			head = temp[i].head;
			break;
		}
	}
	
	if(flag == false)
	{
		cout<<"No such File"<<endl;
		//temp.clear();
		return;
	}

	temp.clear();
	//显示
	vector<char> disp;
	vector<int> numvec;
	for(int m = 0; ;m++)
	{
		if(head == -1)
		{
			break;
		}
		numvec.push_back(head);
		head = pFAT[head].next;
	}
	
	char* ppp = (char*)pBLOCK;
	int count = 0;
	for(int mm = 0;mm<numvec.size();mm++)
	{
		ppp = (char*)(pBLOCK + numvec[mm]);
		for(;;)
		{
			disp.push_back(*ppp);
			ppp++;
			count++;
			if(count == pFAT[numvec[mm]].offset)
			{
				break;
			}
		}
	}

	for(int nn =0;nn<disp.size();nn++)
	{
		cout<<disp[nn];
	}
	cout<<endl;
	p = tempp;
}

void cdBacktoRoot()
{
	p = pBLOCK;
	vecM.clear();
}

void show()
{
	vector<ROOTITEM> temp;
	int offset = 0;
	ROOTITEM *tp = (ROOTITEM*)p;
	
	if(p != pBLOCK)
	{
		cout<<"."<<endl;
		cout<<".."<<endl;
	}
	while(offset != pFAT[p-pBLOCK].offset)
	{	
		temp.push_back(*tp);
		tp++;
		offset++;
	}
	int ddd = 0;
	for(int i = 0; i<temp.size();i++)
	{
		if(temp[i].flag != '3')
		cout<<temp[i].name<<"\t";
		if(temp[i].flag == '1')
		{
			cout<<"dictionary";
		}
		else if(temp[i].flag == '2')
		{
			cout<<"file";
		}
		else if(temp[i].flag == '3')
		{
			ddd++;
		}
		cout<<endl;
	}
	cout<<endl<<(temp.size()-ddd)<<"  item(s)"<<endl;
	temp.clear();
}

void shell()
{
	string command;
	string name;
	input(command);	
	

	while(command != "exit")
	{
		if(command == "md")
		{
			cin>>name;
			addD(name);
		}
		else if(command == "rd")
		{
			cin>>name;
			removeD(name);
		}
		else if(command == "cd")
		{
			cin>>name;
			cdD(name);
		}
		else if(command == "cd\\")
		{
			cdBacktoRoot();
		}
		else if(command == "cd..")
		{
			cdpp();
		}
		else if(command == "dir")
		{
			show();
		}
		else if(command == "mkfile")
		{
			cin>>name;
			creatFile(name);
		}
		else if(command == "type")
		{
			cin>>name;
			type(name);
		}
		else if(command == "del")
		{
			cin>>name;
			del(name);
		}
		else if(command == "showfile")
		{
			cin>>name;
			showFile(name);
		}
		else
		{
			cout<<"No such command"<<endl;
		}
		input(command);
	}
}



int main()
{
	shell();
	delete[] pFAT;
	delete[] pBLOCK;
	return 0;
}