#include "precomp.h"
#pragma hdrstop

#if 0
int dump_memory(DWORD PID,PBYTE pbStartingAddress,DWORD iLength,char * szFile)
{
	FILE * fp=fopen(szFile,"wb+");
	void * pbMemory;
	DWORD iRead;
	HANDLE hProcess;
	
	utils_debug("Dumping Process : %08x, from %08x, length %08x, to file %s.\n",PID,pbStartingAddress,iLength,szFile);
	if(!fp){
		utils_error("Open %s(read/write) failed.\n",fp);
		return 0;
	};
	pbMemory=malloc(iLength);
	if(!pbMemory){
		utils_error("Memory allocation failed (%d bytes).\n",iLength);
		fclose(fp);
		return 0;
	};

	if( !(hProcess=OpenProcess(PROCESS_ALL_ACCESS,0,PID)) ){
		wperror("OpenProcess failed:");
		free(pbMemory);
		fclose(fp);
		return 0;
	}
	if( !ReadProcessMemory(hProcess,pbStartingAddress,pbMemory,iLength,&iRead)){
		wperror("ReadProcessMemory failed:");
		free(pbMemory);
		fclose(fp);
		return 0;
	};

	fwrite(pbMemory,1,iLength,fp);

	utils_trace("Ok, %d bytes written.\n",iLength);

	free(pbMemory);
	fclose(fp);
	return 1;
};

void main(int argc,char **argv)
{
	DWORD iLength;
	void * pvStartingAddress;
	DWORD PID;
	static char * szPid;
	static char * szAddress;
	static char * szLength;
	static char * szFile;

	static BEGIN_ARGS	
		ARG("f:",szFile,"char *","Output file name.")
		ARG("p:",szPid,"char *","PID of the destination process(hex)")
		ARG("a:",szAddress,"char *","Starting address(hex).")
		ARG("l:",szLength,"char *","Length of memory to dump(hex).")
	END_ARGS

	process_args_1(argc,argv);
	if(!szFile || !szPid || !szAddress || !szLength){
		print_usage(__CmdLineArgValues);
		return;
	};
	sscanf(szPid,"%x",&PID);
	sscanf(szLength,"%x",&iLength);
	sscanf(szAddress,"%x",&pvStartingAddress);
	dump_memory(PID,pvStartingAddress,iLength,szFile);
}

#endif