#include <iostream>
#include <string>
#include <vector>
using namespace std;

class FatBlock {
private:
	bool use;
	int lengthInUse;
	int nextBlock;
public:
	FatBlock();
	bool getUse() {return use;}
	int getLengthInUse() {return lengthInUse;}
	int getNextBlock() {return nextBlock;}
	void setUse(bool u) {use = u;}
	void setLengthInUse(int l) {lengthInUse = l;}
	void setNextBlock(int n) {nextBlock = n;}
};

class FatTable {
public:
	FatBlock fatBlockList[1024];
	FatTable();
};

class Block {
public:
	char content[1024];
	Block();
};

class Memory {
public:
	Block * blockList;
	Memory();
	~Memory();
};

class FCB {
public:
	char name[60];
	int size;
	bool folderOrFile;
	int contentBlock;
	FCB();
	FCB(string name, int size, bool folderOrFile, int contentBlock);
};

class FileRecord {
public:
	char name[60];
	int FCBBlock;
	FileRecord();
	FileRecord(string name, int FCBBlock);
};

class FolderBlock {
public:
	FileRecord fileList[16];
	FolderBlock();
};

class FileSystem {
private:
	FatTable fatTable;
	Memory memory;
	int currentFolder;
public:
	FileSystem();
	Memory & getMemory() {return memory;}
	void setMemory(Block * list) {memory.blockList = list;}
	bool checkName(string name);
	bool enoughBlock(int &blockForFCB, int &blockForContent);
	bool enoughFileBlock(int &blockForFCB, int &blockForContent);
	bool enoughInBlock(int &blockForRecord);
	bool createFolder(string foldName);
	bool createFile(string fileName);
	void dir();
	bool cd(string path);
	bool del(string fileName);
	bool deltree(string folderName);
	bool type(string fileName);
	bool edit(string fileName);
	void instruction();
	string getCurrentPath();
};