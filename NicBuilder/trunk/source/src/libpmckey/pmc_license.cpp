#pragma warning(disable:4996)

#include <pmc.h>
#define MAKE_LICENSE
#include "d3des.h"
#include "pmc_license.h"




/* a wrapper function */
static void transform(const __u8 * key, int direction, __uint len, const void * plain, void * ciphered)
{
	__uint i;
	const __u8 *from;
	__u8 *to;
	assert(!(len & 0xf));
	des2key(key, direction);
	from=(const unsigned char *)plain, to=(unsigned char *)ciphered;
	memset(ciphered, 0, len);
	i=0;
	while(i < len){
		D2des(from,to);
		i+=16;
		from+=16;
		to+=16;
	}
}

extern BOOL getHardDiskId(int diskNo, char * idBuffer, int bufferLen);

static __u8 * getSiteKey()
{
	static unsigned char localKey[32];
	strcpy((char *)localKey, "ACTECH-KEY");
	if(getHardDiskId(0, (char*)localKey, sizeof localKey)){
		/* shift off leading spaces */
		memmove(localKey, localKey+4, 16);
		localKey[16]=0;
	}
	return localKey;
}

static bool isLicenseFileValid(const char * file)
{
	HANDLE h;
	BY_HANDLE_FILE_INFORMATION bhfi;
	h=CreateFile(file, GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	int err = GetLastError();
	if(h==INVALID_HANDLE_VALUE){
		return false;
	}
	FILETIME t[3];
	GetFileTime(h,t,t+1,t+2);
	GetFileInformationByHandle(h, &bhfi);
	CloseHandle(h);
	if(memcmp(&t[0],&t[2],sizeof t[0])!=0){
		return false;
	}
	if((bhfi.nFileSizeLow-16) % sizeof(pmc_license_t)){
		return false;
	}
	return true;
}

static void makeLicenseFileValid(const char * file)
{
	HANDLE h;
	h=CreateFile(file, GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(h==INVALID_HANDLE_VALUE){
		return;
	}
	FILETIME t[3];
	GetFileTime(h,t,t+1,t+2);
	t[0]=t[2];
	SetFileTime(h,t,t+1,t+2);
	CloseHandle(h);
}

#if defined(MAKE_LICENSE)
BOOL genLicense(const char * file, struct pmc_license_t * lic)
{
	FILE * fp;
	int i;
	struct pmc_license_t lic2;
	__u8 localKey[17];
	
	fp=fopen(file, "wb");
	memcpy(localKey, getSiteKey(), 16);
	localKey[16]=0;
	printf("ok, using key '%s'\n", localKey);
	if(!fp){
		wperror("fopen");
		return FALSE;
	}
	/* run and dev */
	lic->sig=SIGNATURE;
	transform(localKey, EN0, sizeof *lic, (const __u8*)lic, (__u8*)&lic2);
	/* ok, write key and contents to file */
	for(i=0; i<16; i++){localKey[i]=~localKey[i];}
	fwrite(localKey, 1, 16, fp);
	fwrite(&lic2,1,sizeof lic2, fp);
	fclose(fp);
	printf("ok, license file '%s' written\n", file);

	makeLicenseFileValid(file);
	
	return TRUE;
}
#endif

LIBPMCKEY_API void createNullLicenseFile(const char * file)
{
	FILE * fp;
	char localKey[16];
	int i;
	memcpy(localKey, getSiteKey(), 16);
	for(i=0; i<16; i++){localKey[i]=~localKey[i];}
	fp=fopen(file,"wb");
	if(fp){
		fwrite(localKey,1,16,fp);
		fclose(fp);
	}
	makeLicenseFileValid(file);
}

/*
	add an entry of license to a file
*/
LIBPMCKEY_API BOOL insertLicense(const char * file, struct pmc_license_t * lic)
{
	FILE * fp;
	int i;
	unsigned char key[16];
	if(!isLicenseFileValid(file)){
		createNullLicenseFile(file);
	}
	fp=fopen(file,"rb+");
	if(!fp){
		return FALSE;
	}
	fread(key, 1, 16, fp);
	for(i=0; i<16; i++){key[i]=~key[i];}
	/* find an empty slot */
	struct pmc_license_t lic2,lic3;
	while(!feof(fp)){
		if(fread(&lic2, 1, sizeof lic2, fp) != sizeof(lic2)){
			assert(feof(fp));
			break;
		}
		transform(key,DE1,sizeof lic2, &lic2,&lic3);
		if(lic3.sig!=SIGNATURE || lic3.count==0){
			fseek(fp,(0-sizeof lic2), SEEK_CUR);// Zander 2013-1-22
			break;
		}
	}
	lic->sig=SIGNATURE;
	transform(key,EN0,sizeof lic2, lic, &lic2);
	fwrite(&lic2,1,sizeof lic2, fp);
	fclose(fp);

	makeLicenseFileValid(file);
	
	return TRUE;
}

/*
	remove an amount of license from a file
*/
LIBPMCKEY_API BOOL removeLicense(const char * file, int id, int count)
{
	if(!isLicenseFileValid(file)){
		return FALSE;
	}
	FILE * fp;
	/* count is ignored for now, the whole entry is deleted */	
	struct pmc_license_t lic2, lic3;
	fp=fopen(file, "rb+");
	if(!fp){
		return FALSE;
	}
	unsigned char key[16];
	fread(key,16,1,fp);
	int i;
	for(i=0; i<16;i++){key[i]=~key[i];}
	int entries;
	fseek(fp,0,SEEK_END);
	entries=(ftell(fp)-16)/sizeof(pmc_license_t);
	id--;
	if(id>=entries){
		fclose(fp);
		return FALSE;
	}
	fseek(fp,16+sizeof(pmc_license_t)*id,SEEK_SET);
	/* place a null entry here */
	memset(&lic2, 0, sizeof lic2);
	transform(key,EN0,sizeof(pmc_license_t), &lic2, &lic3);
	fwrite(&lic3, 1, sizeof lic3, fp);
	fclose(fp);

	makeLicenseFileValid(file);
	
	return TRUE;
}

/*
*/
LIBPMCKEY_API int enumLicense(const char * file, ENUM_LIC_PROC p, int con)
{
	int i,id;
	__u8 key[16];
	int count=0;
	struct pmc_license_t lic, lic2;

	if(!isLicenseFileValid(file)){
		return 0;
	}
	HANDLE h;
	h=CreateFile(file, GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(h==INVALID_HANDLE_VALUE){
		return 0;
	}
	DWORD r;
	if(!ReadFile(h,key,16,&r,0)){
		CloseHandle(h);
		return 0;
	}
	for(i=0; i<16; i++){key[i]=~key[i];}
	id=0;
	while(1){
		if(!ReadFile(h,&lic,sizeof lic,&r,0)){
			// return count;
			break;
		}
		if(r==0){
			break;
		}
		id++;
		transform(key, DE1, sizeof lic, &lic, &lic2);
		if(lic2.sig != SIGNATURE || !lic2.count){
			continue;
		}
		if(p){
			if(!p(file, id, &lic2, con)){
				break;
			}
		}
		count++;
	}
	CloseHandle(h);
	
	return count;
}

/*
	get local licenses
*/
int enumLocalLicense(const char * file, ENUM_LIC_PROC p, int con)
{
	int i,id;
	__u8 key[16];
	int count=0;
	struct pmc_license_t lic, lic2;
	__u8 * localKey;

	if(!isLicenseFileValid(file)){
		return 0;
	}
	HANDLE h;
	h=CreateFile(file, GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(h==INVALID_HANDLE_VALUE){
		return 0;
	}
	DWORD r;
	if(!ReadFile(h,key,16,&r,0)){
		CloseHandle(h);
		return 0;
	}
	/* stored key is read but ignored, site key is used instead */
	for(i=0; i<16; i++){key[i]=~key[i];}
	id=0;
	/* use site key to extract messages */
	localKey=getSiteKey();
	while(1){
		if(!ReadFile(h,&lic,sizeof lic,&r,0)){
			// return count;
			break;
		}
		if(r==0){
			break;
		}
		id++;
		transform(localKey, DE1, sizeof lic, &lic, &lic2);
		if(lic2.sig != SIGNATURE || !lic2.count){
			continue;
		}
		if(p){
			if(!p(file, id, &lic2, con)){
				break;
			}
		}
		count++;
	}
	CloseHandle(h);
	
	return count;
}

