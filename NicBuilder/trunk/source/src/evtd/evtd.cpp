#include "precomp.h"
#include "evtsrv.h"

CEvtSrv serv;

//#define INVALID_FILE_ATTRIBUTES  -1
int main(int c,char **v)
{
	int i;
	char * projDir;
	DWORD fileAttrib;
	
	for(i=1; i<c; i++){
		if(!strnicmp(v[i], "-p:", 3)){
			projDir = v[i] + 3;
			fileAttrib = GetFileAttributes(projDir);
			if(fileAttrib == INVALID_FILE_ATTRIBUTES || 
				!(fileAttrib & FILE_ATTRIBUTE_DIRECTORY)
			){
				utils_error("Invalid project directory : %s\n", v[i] + 3);
			}else{
				set_working_dir(projDir);
			}
		}
	}
	
	serv.RegisterService(c, v);

	return 1;
}
