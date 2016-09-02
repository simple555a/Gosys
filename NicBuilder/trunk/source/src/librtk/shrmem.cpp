#include "precomp.h"

CSharedMemory::CSharedMemory()
{
	lpBlob=NULL;
	mhMapped=NULL;
	mhFile=INVALID_HANDLE_VALUE;		
	mErr=-1;
	InitializeCriticalSection(&cs);
}

CSharedMemory::CSharedMemory(
	const char * fname,
	unsigned offset,
	unsigned size,
	const char * shared_name,
	DWORD create_mode,
	DWORD share_mode,
	DWORD disired_access	
)
{
	lpBlob=NULL;
	mhMapped=NULL;
	mhFile=INVALID_HANDLE_VALUE;		
	mErr=-1;	
	InitializeCriticalSection(&cs);
	StaticInit(
		fname,
		offset,
		size,
		shared_name,
		create_mode,
		share_mode,
		disired_access
		);
}

CSharedMemory::~CSharedMemory()
{
	UnInit();
	DeleteCriticalSection(&cs);
}

void	CSharedMemory::UnInit()
{		
	Lock();

	if(NULL != lpBlob)	{
		UnmapViewOfFile(lpBlob);
		lpBlob=NULL;
	}
	if(NULL != mhMapped)	{
		CloseHandle(mhMapped);
		mhMapped=NULL;
	}
	if(INVALID_HANDLE_VALUE != mhFile){
		CloseHandle(mhFile);
		mhFile=INVALID_HANDLE_VALUE;
	}

	Unlock();
}

int CSharedMemory::StaticInit(
	const char * fname,
	unsigned offset,
	unsigned size,
	const char * shared_name,
	DWORD create_mode,
	DWORD share_mode,
	DWORD disired_access
	)
{
	int err,dsize;
	bool exists=false;
	FileSize=0;

	UnInit();
	
	Lock();

	try{
		if(fname){
			mhFile=CreateFile(
				fname,
				disired_access,
				share_mode,
				NULL,
				create_mode,
				FILE_ATTRIBUTE_ARCHIVE,
				NULL
			);
			
			if( INVALID_HANDLE_VALUE == mhFile)
				throw(err=-10);
			dsize=GetFileSize(mhFile,NULL);
			oFNum++;
			if(size == 0) size=dsize;
		}else{
			mhFile=INVALID_HANDLE_VALUE;
		}
		mhMapped = CreateFileMapping(
			mhFile,
			NULL,
			PAGE_READWRITE,
			offset,
			size,
			shared_name
			);
		if(ERROR_ALREADY_EXISTS == GetLastError()){
			exists=true;
		}
		if(NULL == mhMapped){
			throw(err=-20);
		}
		lpBlob=MapViewOfFile(mhMapped, FILE_MAP_ALL_ACCESS, 0,0,size);
		
		if(NULL != mhMapped){
			CloseHandle(mhMapped);
			mhMapped=NULL;
		}
		
		if(INVALID_HANDLE_VALUE != mhFile){
			CloseHandle(mhFile);
			mhFile=INVALID_HANDLE_VALUE;
		}
		if(NULL == lpBlob)	{
			throw(err=-30);
		}
		if(exists){
			memset(lpBlob,0,dsize);
		}
		FileSize=dsize;
	}catch(int err){
		mErr=err;
		err=GetLastError();
	}
	
	Unlock();

	return mErr;
}

// BUGBUG: I once thought there should be a flush routine
__bool CSharedMemory::Flush()
{
	// do nothing
	return __true;
}
