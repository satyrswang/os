#include "fs.h"
#include "file.h"
#include "Shell.h"


void main(){

	FS * pFS = new FS("FileSystem.dat");

	Shell * pShell = new Shell ( pFS );
	pShell->Run();
	
	delete pShell;
	delete pFS;
	
}