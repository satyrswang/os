#include "FileSystem.h"

int main(int argc, char *argv[ ], char *envp[ ] )
{
	FileSystem fileSystem;

	FILE * stream;

	if((stream = fopen("data.dat","r+b")) != NULL)
	{
		fread(&fileSystem,sizeof(FileSystem),1,stream);
		Block * blockList = new Block[1024];
		fread(blockList,sizeof(Block[1024]),1,stream);
		fileSystem.setMemory(blockList);
		fclose(stream);
	}

	cout << "                              欢迎进入迷你文件系统" << endl;
	cout << "                                   姓名：沈呈" << endl;
	cout << "                                   学号：010124" << endl;
	
	char c;
	char s[2];
	while(true)
	{
		string in = "";
		cout << endl;
		cout << fileSystem.getCurrentPath();
		while((c = getchar()) != '\n')
		{
			s[0] = c;
			s[1] = '\0';

			in = in.append(s);
		}
		if(in == "exit")
		{
			stream = NULL;
			if((stream = fopen("data.dat","w+b")) != NULL)
			{		
				fwrite(&fileSystem,sizeof(FileSystem),1,stream);
				fwrite(fileSystem.getMemory().blockList,sizeof(Block[1024]),1,stream);
				fclose(stream);
			}
			break;
		}
		else if(in == "?")
			fileSystem.instruction();
		else if(in == "dir")
			fileSystem.dir();
		else if(in == "cd..")
			fileSystem.cd("..");
		else if(in == "format")
			cout << "format" << endl;
		else 
		{
			string cmd = in.substr(0,in.find_first_of(" "));
			string parameter = in.substr(in.find_first_of(" ") + 1,in.length());
			if(cmd == "cd")
				fileSystem.cd(parameter);
			else if(cmd == "md")
				fileSystem.createFolder(parameter);
			else if(cmd == "mf")
				fileSystem.createFile(parameter);	
			else if(cmd == "deltree")
				fileSystem.deltree(parameter);
			else if(cmd == "del")
				fileSystem.del(parameter);
			else if(cmd == "type")
				fileSystem.type(parameter);
			else if(cmd == "edit")
				fileSystem.edit(parameter);
			else
				cout << cmd << "为不合法操作， 如需查看基本操作,请输入?";
		}
		cout << endl << endl;
	}

	return 0;
}