#include <pmc.h>
#define MAKE_LICENSE
#include "../pmc_license.h"

static __bool __cdecl  eProc(const char * file, int entryId, struct pmc_license_t * lic, int context)
{
	printf("%d\t%d\t%d\t%s\n", lic->count, lic->runLevel, lic->devLevel, lic->comment);
	return __true;
}

static BOOL chkLicenseFile(const char * file)
{
	int count;
	count=enumLicense(file, 0, 0);
	if(count){
		printf("Count\tRun\tDev\tComment\n");
		enumLicense(file, eProc, 0);
	}else{
		printf("No valid license found.\n");
	}
	return TRUE;
}

static __bool __cdecl  e2(const char * file, int entryId, struct pmc_license_t * lic, int context)
{
	char * f2;
	f2=(char *)context;
	insertLicense(f2, lic);
	return __true;
}


static void moveLicenseFile(const char * f1, const char * f2)
{
	enumLicense(f1,e2,(int)f2);
	createNullLicenseFile(f1);
}

int main(int c, char **v)
{
	char * fileName="license.dat";
	char * fileName2="/license.dat";
	int runLevel=128;
	int devLevel=128;
	__bool chk=__false;
	char * comment="Actech generic PMC License";
	struct pmc_license_t lic;
	int count=1;
	__bool move=false;
	int i;
	
	BEGIN_ARGS
		ARG("-f:", fileName, "char *", "license file name")
		ARG("-f2:", fileName, "char *", "license file name 2")
		ARG("-r:", runLevel, "int", "run level")
		ARG("-d:", devLevel, "int", "development level")
		ARG("-chk", chk, "tbool", "check license file")
		ARG("-move", move, "tbool", "move license file")
		ARG("-count:", count, "int", "license count")
		ARG("-comment:", comment, "char *", "comment")
	END_ARGS

	if(process_args_1(c, v) < 0){
		print_usage_1();
		return -1;
	}

	if(chk){
		chkLicenseFile(fileName);
	}else if(move){
		moveLicenseFile(fileName, fileName2);
	}else{
		/* ok, generate license file */
		memset(&lic, 0, sizeof lic);
		lic.devLevel=devLevel;
		lic.runLevel=runLevel;
		lic.count=1;
		strncpy(lic.comment, comment, sizeof lic.comment);
		for(i=0;i<count;i++){
			insertLicense(fileName, &lic);
		}
	}
	return 0;
}

