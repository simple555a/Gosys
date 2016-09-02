#pragma warning(disable:4996)
#include <pmc.h>
#include <stdio.h>

#define DFP_GET_VERSION 0x00074080
#define DFP_SEND_DRIVE_COMMAND 0x0007c084
#define DFP_RECEIVE_DRIVE_DATA 0x0007c088

#pragma pack(1)
typedef struct _GETVERSIONOUTPARAMS {
	BYTE bVersion;  // Binary driver version.
	BYTE bRevision;  // Binary driver revision.
	BYTE bReserved;  // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

typedef struct _IDEREGS {
	BYTE bFeaturesReg;  // Used for specifying SMART "commands".
	BYTE bSectorCountReg; // IDE sector count register
	BYTE bSectorNumberReg; // IDE sector number register
	BYTE bCylLowReg;   // IDE low order cylinder value
	BYTE bCylHighReg;  // IDE high order cylinder value
	BYTE bDriveHeadReg;  // IDE drive/head register
	BYTE bCommandReg;  // Actual IDE command.
	BYTE bReserved;   // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;

typedef struct _SENDCMDINPARAMS {
	DWORD cBufferSize;  // Buffer size in bytes
	IDEREGS irDriveRegs;  // Structure with drive register values.
	BYTE bDriveNumber;  // Physical drive number to send
	// command to (0,1,2,3).
	BYTE bReserved[3];  // Reserved for future expansion.
	DWORD dwReserved[4];  // For future use.
	//BYTE  bBuffer[1];   // Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

typedef struct _DRIVERSTATUS {
	BYTE bDriverError;  // Error code from driver,
	// or 0 if no error.
	BYTE bIDEStatus;   // Contents of IDE Error register.
	// Only valid when bDriverError
	// is SMART_IDE_ERROR.
	BYTE bReserved[2];  // Reserved for future expansion.
	DWORD dwReserved[2];  // Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

typedef struct _SENDCMDOUTPARAMS {
	DWORD    cBufferSize;  // Size of bBuffer in bytes
	DRIVERSTATUS DriverStatus;  // Driver status structure.
	BYTE   bBuffer[512];   // Buffer of arbitrary length
	// in which to store the data read from the drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

typedef struct _IDSECTOR {
	USHORT wGenConfig;
	USHORT wNumCyls;
	USHORT wReserved;
	USHORT wNumHeads;
	USHORT wBytesPerTrack;
	USHORT wBytesPerSector;
	USHORT wSectorsPerTrack;
	USHORT wVendorUnique[3];
	CHAR sSerialNumber[20];
	USHORT wBufferType;
	USHORT wBufferSize;
	USHORT wECCSize;
	CHAR sFirmwareRev[8];
	CHAR sModelNumber[40];
	USHORT wMoreVendorUnique;
	USHORT wDoubleWordIO;
	USHORT wCapabilities;
	USHORT wReserved1;
	USHORT wPIOTiming;
	USHORT wDMATiming;
	USHORT wBS;
	USHORT wNumCurrentCyls;
	USHORT wNumCurrentHeads;
	USHORT wNumCurrentSectorsPerTrack;
	ULONG ulCurrentSectorCapacity;
	USHORT wMultSectorStuff;
	ULONG ulTotalAddressableSectors;
	USHORT wSingleWordDMA;
	USHORT wMultiWordDMA;
	BYTE bReserved[128];
} IDSECTOR, *PIDSECTOR;

/**//*+++
Global vars
---*/
static GETVERSIONOUTPARAMS vers;
static SENDCMDINPARAMS in;
static SENDCMDOUTPARAMS out;
static HANDLE h;
static BYTE j;
static DWORD i;

#if 0
void CopyRight(){
	cerr<<endl<<"HDD identifier v1.0 for WIN95/98/Me/NT/2000. written by Lu Lin");
	cerr<<"For more information, please visit Inside Programming: http://lu0.126.com");
	cerr<<"2000.11.3"<<endl);
}
#endif

static VOID ChangeByteOrder(PCHAR szString, USHORT uscStrSize)
{

	USHORT i;
	CHAR temp;

	for (i = 0; i < uscStrSize; i+=2)
	{
		temp = szString[i];
		szString[i] = szString[i+1];
		szString[i+1] = temp;
	}
}

#define __msg(x)

static void DetectIDE(BYTE bIDEDeviceMap){
	if (bIDEDeviceMap&1){
		if (bIDEDeviceMap&16){
			__msg(("ATAPI device is attached to primary controller, drive 0."));
		}else{
			__msg(("IDE device is attached to primary controller, drive 0."));
		}
	}
	if (bIDEDeviceMap&2){
		if (bIDEDeviceMap&32){
			__msg(("ATAPI device is attached to primary controller, drive 1."));
		}else{
			__msg(("IDE device is attached to primary controller, drive 1."));
		}
	}
	if (bIDEDeviceMap&4){
		if (bIDEDeviceMap&64){
			__msg(("ATAPI device is attached to secondary controller, drive 0."));
		}else{
			__msg(("IDE device is attached to secondary controller, drive 0."));
		}
	}
	if (bIDEDeviceMap&8){
		if (bIDEDeviceMap&128){
			__msg(("ATAPI device is attached to secondary controller, drive 1."));
		}else{
			__msg(("IDE device is attached to secondary controller, drive 1."));
		}
	}
}

#if 0
static void hdid9x(){
	ZeroMemory(&vers,sizeof(vers));
	//We start in 95/98/Me
	h=CreateFile("\\\\.\\Smartvsd",0,0,0,CREATE_NEW,0,0);
	if (!h){
		__msg(("open smartvsd.vxd failed");
		return;
	}

	if (!DeviceIoControl(h,DFP_GET_VERSION,0,0,&vers,sizeof(vers),&i,0)){
		__msg(("DeviceIoControl failed:DFP_GET_VERSION");
		CloseHandle(h);
		return;
	}
	//If IDE identify command not supported, fails
	if (!(vers.fCapabilities&1)){
		__msg(("Error: IDE identify command not supported.";
		CloseHandle(h);
		return;
	}
	//Display IDE drive number detected
	DetectIDE(vers.bIDEDeviceMap);
	//Identify the IDE drives
	for (j=0;j<4;j++){
		PIDSECTOR phdinfo;
		char s[41];

		ZeroMemory(&in,sizeof(in));
		ZeroMemory(&out,sizeof(out));
		if (j&1){
			in.irDriveRegs.bDriveHeadReg=0xb0;
		}else{
			in.irDriveRegs.bDriveHeadReg=0xa0;
		}
		if (vers.fCapabilities&(16>>j)){
			//We don't detect a ATAPI device.
			__msg(("Drive ", (int)(j+1), " is a ATAPI device, we don't detect it");
			continue;
		}else{
			in.irDriveRegs.bCommandReg=0xec;
		}
		in.bDriveNumber=j;
		in.irDriveRegs.bSectorCountReg=1;
		in.irDriveRegs.bSectorNumberReg=1;
		in.cBufferSize=512;
		if (!DeviceIoControl(h,DFP_RECEIVE_DRIVE_DATA,&in,sizeof(in),&out,sizeof(out),&i,0)){
			__msg(("DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA");
			CloseHandle(h);
			return;
		}
		phdinfo=(PIDSECTOR)out.bBuffer;
		memcpy(s,phdinfo->sModelNumber,40);
		s[40]=0;
		ChangeByteOrder(s,40);
		__msg(endl<<"Module Number:"<<s);
		memcpy(s,phdinfo->sFirmwareRev,8);
		s[8]=0;
		ChangeByteOrder(s,8);
		__msg(("\tFirmware rev:"<<s);
		memcpy(s,phdinfo->sSerialNumber,20);
		s[20]=0;
		ChangeByteOrder(s,20);
		__msg(("\tSerial Number:"<<s);
		__msg(("\tCapacity:"<<phdinfo->ulTotalAddressableSectors/2/1024<<"M"<<endl);
	}

	//Close handle before quit
	CloseHandle(h);
	CopyRight();

}
#endif

static BOOL hdidnt(int j, char * sid, int sidLen){
	char hd[80];
	PIDSECTOR phdinfo;
	char s[41];
	HANDLE h;

	ZeroMemory(&vers,sizeof(vers));

	//We start in NT/Win2000
	{
		sprintf(hd,"\\\\.\\PhysicalDrive%d",j);
		h=CreateFile(hd,GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
		if (!h){
			return FALSE;
		}
		if (!DeviceIoControl(h,DFP_GET_VERSION,0,0,&vers,sizeof(vers),&i,0)){
			CloseHandle(h);
			return FALSE;
		}
		//If IDE identify command not supported, fails
		if (!(vers.fCapabilities&1)){
			__msg(("Error: IDE identify command not supported."));
			CloseHandle(h);
			return FALSE;
		}
		//Identify the IDE drives
		ZeroMemory(&in,sizeof(in));
		ZeroMemory(&out,sizeof(out));
		if (j&1){
			in.irDriveRegs.bDriveHeadReg=0xb0;
		}else{
			in.irDriveRegs.bDriveHeadReg=0xa0;
		}
		if (vers.fCapabilities&(16>>j)){
			//We don't detect a ATAPI device.
			__msg(("Drive %d %s\n",(int)(j+1)," is a ATAPI device, we don't detect it"));
			return FALSE;
		}else{
			in.irDriveRegs.bCommandReg=0xec;
		}
		in.bDriveNumber=j;
		in.irDriveRegs.bSectorCountReg=1;
		in.irDriveRegs.bSectorNumberReg=1;
		in.cBufferSize=512;
		if (!DeviceIoControl(h,DFP_RECEIVE_DRIVE_DATA,&in,sizeof(in),&out,sizeof(out),&i,0)){
			__msg(("DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA\n"));
			CloseHandle(h);
			return FALSE;
		}
		phdinfo=(PIDSECTOR)out.bBuffer;
		memcpy(s,phdinfo->sModelNumber,40);
		s[40]=0;
		ChangeByteOrder(s,40);
		__msg(("Module Number:%s\n", s));
		memcpy(s,phdinfo->sFirmwareRev,8);
		s[8]=0;
		ChangeByteOrder(s,8);
		__msg(("\tFirmware rev:%s\n", s));
		memcpy(s,phdinfo->sSerialNumber,20);
		s[20]=0;
		ChangeByteOrder(s,20);
		__msg(("\tSerial Number:%s\n", s));
		strncpy(sid, s, min(20, sidLen));
		sid[sidLen-1] = 0;
		__msg(("\tCapacity:%dM\n", phdinfo->ulTotalAddressableSectors/2/1024));
		CloseHandle(h);
	}

	return TRUE;
}

#if 0
void main(){
	OSVERSIONINFO VersionInfo;

	ZeroMemory(&VersionInfo,sizeof(VersionInfo));
	VersionInfo.dwOSVersionInfoSize=sizeof(VersionInfo);
	GetVersionEx(&VersionInfo);

	switch (VersionInfo.dwPlatformId){
 case VER_PLATFORM_WIN32s:
	 __msg(("Win32s is not supported by this programm.");
	 return;
 case VER_PLATFORM_WIN32_WINDOWS:
	 hdid9x();
	 return;
 case VER_PLATFORM_WIN32_NT:
	 hdidnt();
	 return;
	}
}
#endif

BOOL getHardDiskId(int diskNo, char * idBuffer, int bufferLen)
{
	return hdidnt(diskNo, idBuffer, bufferLen);
}

