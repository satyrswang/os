// Shell.cpp: implementation of the Shell class.
//
//////////////////////////////////////////////////////////////////////

#include "Shell.h"
#include <iostream>
#include "Tokenizer.h"
#include "FS_Macro.h"
#include "fs.h"
#include <conio.h>

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const char * Shell::help =
 "����һ���ļ�ϵͳ��ģ����� ����������У� ֧��Linux�µĻ����ļ���������\n\
	mkdir\t\tĿ¼��...\t��mkdir os gp ���ᴴ�������ļ���\n\
	rmdir\t\tĿ¼��...\t��rmdir os gp ����ɾ���������ļ���\n\
	touch\t\t�ļ���...\t��touch a.c b.cpp ���ᴴ���������ļ�\n\
	rm\t\t �ļ���...\t��rm a.c b.cpp ����ɾ���������ļ�\n\
	ls\t\t\t ��ʾ��ǰĿ¼����������\n\
	pwd\t\t\t ��ӡ��ǰĿ¼\n\
	exit\t\t\t �˳�\n\
	cd\t\t\t �ı�Ŀ¼Ŀ¼��\n\
	help\t\t\t �������\n\
	logout\t\t\t �ǳ�\n\
	clear\t\t\t ����\n\
	usage\t\t\t �����̵�ʹ�����\n";

Shell::Shell( FS * fs)
{

	this->pFS = fs;
	memset(userName, 0x00, sizeof(userName) );
	memset(promt,    0x00, sizeof(promt)    );
	strcat( userName, "nobydy" );
	Login();	
	
}

bool Shell::Login()
{
	cout<<"Please Login In, You Just Provide Your Name, No Password Needed."<<endl;
	cout<<"Your Name: ";
	cin.getline( userName,  sizeof(userName), '\n' );
	promt[0] = '[';
	strcat(promt,userName);
	strcat(promt, "@Linux ");
	strcat(promt, "/]#" );

//	memcpy(promt+1+strlen(userName), "@Linux]#", sizeof("@Linux]#") );
	
	return true;
}

bool Shell::Logout()
{
	cout<<"You choose to logout, memory will be flush to file on disk"<<endl;
	pFS->flush();
	memset( userName, 0x00, sizeof(userName ) );
	strcat( userName, "nobody" );
	memset(promt,    0x00, sizeof(promt)    );
	promt[0] = '[';
	strcat(promt,"nobody");
	strcat(promt, "@Linux ");
	strcat(promt, "/]#" );

	return true;
}

int Shell::CommandDispatcher(Tokenizer & praser )
{
	char * command; 
	char * para;
	int paraNum;
	command = praser[0];
	if( strcmp( command, "ls") == 0 ){
		pFS->PrintCurWorkDirList();
	}else if ( strcmp( command, "format" ) == 0 ){
		cout<<"warning :Formatting Will cause The Lose Of All Data On The Disk Y/N? ";
		char c;
		cin>>c;
		
		if( 'Y' == c || 'y' == c ){
			pFS->FSFormat();
			pFS->InitFSParameters();
		}else if( 'N' == c || 'n' == c ){
			cout<<"Formatting Has been Cancelled "<<endl;
		}

	}else if ( strcmp( command, "mkdir" ) == 0 ){
			paraNum =  praser.size() - 1;
			for( int i = 1; i-1 < paraNum; i ++){
				para = praser[i];
				pFS->FSCreateDir( para );
			}

	}else if ( strcmp( command, "rmdir" ) == 0 ){
			 paraNum =  praser.size() - 1;
			for( int i = 1; i-1 < paraNum; i ++){
				para = praser[i];
				pFS->FSDeleteDir( para );
			}

	}else if ( strcmp( command, "touch" ) == 0  ){
			 paraNum =  praser.size() - 1;
			for( int i = 1; i-1 < paraNum; i ++){
				para = praser[i];
				pFS->FSCreateFile( para, NORMAL_FILE );
			}

	}else if ( strcmp( command, "rm" ) == 0 ){
			 paraNum =  praser.size() - 1;
			for( int i = 1; i-1 < paraNum; i ++){
				para = praser[i];
				pFS->FSDeleteFile( para );
			}
	}else if ( strcmp( command, "fill" ) == 0 ){
			 paraNum =  praser.size() - 1;
			for( int i = 1; i-1 < paraNum; i ++){
				para = praser[i];
				char line[1024];
				cin.getline( line, 1024 );
				pFS->FSFillFile( para, line );
			}	
	}else if ( strcmp( command, "cd" ) == 0 ){
			paraNum =  praser.size() - 1;
			if( paraNum > 1  || paraNum <= 0 ){
				cout<<"wrong synatx, type 'help' to get help"<<endl;
			}else{
				para = praser[1];
				pFS->FSChangeWorkDir( para );
				
				memset(promt,    0x00, sizeof(promt)    );
				promt[0] = '[';
				strcat(promt,userName );
				strcat(promt, "@Linux ");
				strcat(promt, pFS->GetCurWorkDirName()  );
				strcat(promt, "]#" );

			}
	}else if ( strcmp( command, "pwd" ) == 0 ){
			pFS->PrintFullPathName();
	}else if ( strcmp( command, "clear" ) == 0 ){
		    system("cls");
	}else if ( strcmp( command, "usage" ) == 0 ){
			pFS->PrintFAT16Usage(0);

	}else if ( strcmp( command, "login" ) == 0  ){
		this->Login();
	}else if ( strcmp( command, "logout" ) == 0 ){
			this->Logout();
			exit(0);

	}else if ( strcmp( command, "help") == 0 ){
		cout<<endl<<help<<endl;
	}else if (  strcmp( command, "exit")   == 0 ){
		cout<<"You are quiting..."<<endl;
		return SHELL_EXIT;
	}else{
		cout<<"Wrong Command, Type 'help' to Get Help"<<endl;
	}

	return SHELL_NORMAL;

}

void Shell::Run(){

	char line[256];

	memset( line, 0x00, 256);
	cout<<promt;
	while( cin.getline( line, sizeof(line),  '\n' ) ){
		Tokenizer praser(line);
		if( praser.size() <= 0 ){
			cout<<promt;
			continue;
		}else {
			if( strcmp( userName, "nobody" ) ==  0 ){
				cout<<"You Have n't Login"<<endl;
				continue;
			}
			if( SHELL_EXIT == CommandDispatcher(  praser ) ){
				break;
			}
		}
				
		cout<<promt;
		memset( line, 0x00, 256);
	}

	cout<<"Thank you!"<<endl;
	
	getch();
}

Shell::~Shell()
{

}

