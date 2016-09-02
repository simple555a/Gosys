/*
	2005-11-10 software license suppoted added, panxiaohui@actech-mc.com
*/
#pragma warning(disable:4996)

#include <pmc.h>
#include "pair.h"
#include "pmckey.h"
#include "spromeps.h"	// rainbow API的头文件
#include "pmc_license.h"

#include <string>
using namespace std;
 
#pragma comment(lib, "Spromeps.lib")
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtk.lib")
static RB_SPRO_APIPACKET ApiPacket;			//Rainbow 所定义的数据结构
static RB_DWORD qResult32;	

unsigned char readOne(int nCellId)
{
	SP_STATUS	spStatus;
	unsigned char qData[4];					//欲查询数据	
	unsigned char rData[4];					//响应数据
	unsigned char res[] = {0,0,0,0};				
	int queryLength	= 4;						//设置查询数据长度
	int cellAddress = 0x8 + nCellId*4;		//设置查询单元号
	int nPair = nCellId * 400;
	int i;

	//for(i = 0; i < 100; i++){ 为了节省时间，只读了10个
	for(i = 0; i < 10; i++){
		memcpy(qData, &query[nPair],4);
		memcpy(rData, &response[nPair],4);
		spStatus = RNBOsproQuery(ApiPacket,     //Apipacket为 Rainbow公司保留数据结构
								 cellAddress,   //查询单元号
								 &qData,		//查询数据(输入加密锁的数据)
								 &res,			//响应数据(经加密锁计算返回数据)
								 &qResult32,    //qResult32为 Rainbow公司保留数据结构
								 queryLength ); //查询数据长度

		spStatus = memcmp( res, rData, 4 );		//比较响应数据和预知数据

		if(0 != spStatus){
			break;
		}

		nPair = nPair +4;
	}

	if(0 == spStatus){
		return 1;
	}else{
		return 0;
	}

}

LIBPMCKEY_API unsigned char PMCKEY_API init_key()
{
	SP_STATUS	spStatus;
	spStatus = RNBOsproFormatPacket( ApiPacket, sizeof(ApiPacket) );//初始化数据结构（仅Win32程序使用） 
	spStatus = RNBOsproInitialize( ApiPacket );//初始化函数
	RB_WORD  developerID = 0x16F6;//开发者ID号
	//注：这里所提到的ID号均为16进制数	
	spStatus = RNBOsproFindFirstUnit( ApiPacket,developerID );//找到Developer ID指定加密锁
	//									         ▲▲▲ 
	//									       加密琐的ID号 
	
	if(0 == spStatus){
		return 1;
	}else{
		return 0;
	}
}
 

void get_one_key(unsigned char *run, int *runLevel, unsigned char *dev, int *devLevel);

static __bool __cdecl  e2(const char * file, int entryId, struct pmc_license_t * lic, int context)
{
	pmc_license_t * l2=(pmc_license_t*)context;
	if(l2->runLevel<lic->runLevel){
		l2->runLevel=lic->runLevel;
	}
	if(l2->devLevel<lic->devLevel){
		l2->devLevel=lic->devLevel;
	}
	return __true;
}

/*
	find license with the greatest runLevel and devLevel
*/
static BOOL PMCKEY_API get_software_key(unsigned char *run, int *runLevel, unsigned char *dev, int *devLevel)
{
	string s;
	pmc_license_t lic;
	
	s = get_exe_dir();
	s = s + "license.dat";
	memset(&lic,0,sizeof(lic));
	enumLocalLicense(s.c_str(),e2,(int)&lic);

	/* move on to scan root directory */
	s=s.substr(0,3)+"license.dat";
	enumLocalLicense(s.c_str(),e2,(int)&lic);
	
	/* ok, a valid license is found */
	utils_debug("License got : %d run, %d dev\n", lic.runLevel, lic.devLevel);
	*runLevel=lic.runLevel;
	*run=*runLevel?1:0;
	*devLevel=lic.devLevel;
	*dev=*devLevel?1:0;
	return TRUE;
}

LIBPMCKEY_API void PMCKEY_API get_key(unsigned char *run, int *runLevel, unsigned char *dev, int *devLevel)
{
	unsigned char r, d;
	int rl, dl;

	*run=*dev=0;

	get_one_key(run, runLevel, dev, devLevel);
	while(RNBOsproFindNextUnit(ApiPacket) == 0){
		get_one_key(&r, &rl, &d, &dl);
		if(r){
			if(!(*run)){
				*run = r;
				*runLevel = rl;
			}else{
				if(rl == -1){
					*runLevel = -1;
				}else if(rl > *runLevel){
					*runLevel = rl;
				}
			}
		}
		if(d){
			if(!(*dev)){
				*dev = d;
				*devLevel = dl;
			}else{
				if(dl == -1){
					*devLevel = -1;
				}else if(dl > *devLevel){
					*devLevel = dl;
				}
			}
		}
	}

	if(*run==0 && *dev==0){
		get_software_key(run, runLevel, dev, devLevel);
	}
	
#if 0
	if(*run==0 && *dev==0){
		*run=1,*dev=1;
		*runLevel=15000;
		*devLevel=15000;
	}
#endif

}

/*
	cell id
	0 - r64
	1 - r128
	2 - r256
	3 - r512
	4 - r1024
	5 - rnl
	6 - d64
	7 - d218
	8 - d256
	9 - d512
	10 - d1024
	11 - dnl
	12 - rsv1
	13 - rsv2
*/
void get_one_key(unsigned char *run, int *runLevel, unsigned char *dev, int *devLevel)
{
	*run = 0;
	*runLevel = 0;
	*dev = 0;
	*devLevel = 0;

	if(readOne(0)){
		*run = 1;
		*runLevel = 64;
	}
	if(readOne(1)){
		*run = 1;
		*runLevel = 128;
	}
	if(readOne(2)){
		*run = 1;
		*runLevel = 256;
	}
	if(readOne(3)){
		*run = 1;
		*runLevel = 512;
	}
	if(readOne(4)){
		*run = 1;
		*runLevel = 1024;
	}
	if(readOne(5)){
		*run = 1;
		*runLevel = -1;
	}

	if(readOne(6)){
		*dev = 1;
		*devLevel = 64;
	}
	if(readOne(7)){
		*dev = 1;
		*devLevel = 128;
	}
	if(readOne(8)){
		*dev = 1;
		*devLevel = 256;
	}
	if(readOne(9)){
		*dev = 1;
		*devLevel = 512;
	}
	if(readOne(10)){
		*dev = 1;
		*devLevel = 1024;
	}
	if(readOne(11)){
		*dev = 1;
		*devLevel = -1;
	}
}

LIBPMCKEY_API void PMCKEY_API set_product_type()
{
}

LIBPMCKEY_API int PMCKEY_API get_product_type()
{
	return 0;
}

#if 0
extern "C" void __declspec(dllexport) _stdcall dll_main(int,int,char * szCmdLine,int)
{
	setup_console_for_log();
	char buf[32];
	getHardDiskId(0, buf, 32);
	printf("Serial NO:%s\n", buf);
	utils_getc();
}
#endif

