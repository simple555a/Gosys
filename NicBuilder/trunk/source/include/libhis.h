#ifndef __libhis_h_
#define __libhis_h_

#ifdef _WIN32
#ifdef LIBHIS_EXPORTS
#define HIS_API      __declspec(dllexport)
#else
#define HIS_API      __declspec(dllimport)
#pragma comment (lib, "libhis.lib")
#endif
#else
#define HIS_API       
#endif

typedef __int64 PMCTIME;
typedef unsigned short uint16;
struct PMCSYSTIME{
	uint16 year;
	uint16 month;
	uint16 day;
	uint16 hour;
	uint16 minute;
	uint16 second;
	uint16 milliSeconds;
};

HIS_API void tm2systm(const PMCTIME *tm, PMCSYSTIME *sysTm);
HIS_API void systm2tm(const PMCSYSTIME *sysTm, PMCTIME *tm);
HIS_API void time2str(PMCTIME tm, char str[15]);
HIS_API void time2str_x(PMCTIME tm, char str[30]);
HIS_API void str2time(char str[15], PMCTIME *tm);
HIS_API PMCTIME getCurrentTime();
HIS_API bool getCurrentPool(char path[_MAX_PATH], char poolCurrent[_MAX_FNAME]);
HIS_API bool moveNextPool(char path[_MAX_PATH], unsigned int poolCount);
HIS_API void markNewTimeId(char pool[_MAX_FNAME]);
HIS_API bool getPoolByTime(PMCTIME *tm, char path[_MAX_PATH], char pool[_MAX_FNAME]);

template <class rec, unsigned int len> 
class CPMCArchive{
private:
public:
	CPMCArchive()
	{
		use1 = true; 
		p2 = 0;
		memset(snapShot2, 0, sizeof(snapShot2));
		p1 = 0; 
		memset(snapShot1, 0, sizeof(snapShot1));
	};

	void initialize_r(const char *p, unsigned int pc)
	{
		memset(path, 0, _MAX_PATH);
		strncpy(path, p, _MAX_PATH);
		poolCount = pc;
	}
	
	void initialize(const char *p, unsigned int pc)
	{
		char fullName[_MAX_PATH], pool[_MAX_FNAME];
		FILE *fp;
		int i = 0;

		memset(path, 0, _MAX_PATH);
		strncpy(path, p, _MAX_PATH);
		poolCount = pc;
	
		if(getCurrentPool(path, pool)){
			sprintf(fullName, "%s/%s", path, pool);
			fp = fopen(fullName, "rb");
			if(fp){
				fread(snapShot1, sizeof(rec), len, fp);
				p1 = 0;
				while(snapShot1[i].valid){
					i++;
					p1++;
				}
				fclose(fp);
			}			
			if(p1 == len){ // 上次evtd退出时当前pool恰恰是满的，小概率事件
				p1 = 0;
				flush2file();
				moveNextPool(path, poolCount);
				memset(snapShot1, 0, sizeof(snapShot1));
				use1 = false;
			}
		}
	}

	bool flush2file()
	{
		char tempName[_MAX_PATH], targetName[_MAX_PATH], targetName1[_MAX_PATH];
		sprintf(tempName, "%s/temp", path);
		
		FILE *f = fopen(tempName, "wb");
		if(f == NULL){
			return false;
		}
		if(use1){
			fwrite(snapShot1, sizeof(rec), len, f);
		}else{
			fwrite(snapShot2, sizeof(rec), len, f);
		}
		fclose(f);

		char poolCurrent[_MAX_FNAME];
		getCurrentPool(path, poolCurrent);
		sprintf(targetName, "%s/%s", path, poolCurrent);
		CopyFile(tempName, targetName, FALSE);
		markNewTimeId(poolCurrent);
		sprintf(targetName1, "%s/%s", path, poolCurrent);
		rename(targetName, targetName1);
		return true;
	}
	void addNewRecord(rec *pNew)
	{
		if(use1){
			snapShot1[p1] = *pNew;
			p1++;
			if(p1 >= len){
				p1 = 0;
				flush2file();
				moveNextPool(path, poolCount);
				memset(snapShot1, 0, sizeof(snapShot1));
				use1 = false;
			}
		}else{
			snapShot2[p2] = *pNew;
			p2++;
			if(p2 >= len){
				p2 = 0;
				flush2file();
				moveNextPool(path, poolCount);
				memset(snapShot2, 0, sizeof(snapShot2));
				use1 = true;
			}
		}
	}
#define MAX_GET_RECORD 1024
	int readSnapShot(rec *rt, rec *rs, PMCTIME stm, unsigned int count)
	{
		int i = 0;
		while(rs->rTime < stm && i < count){
			*rt = *rs;
			rs++;
			rt++;
			i++
		}
		return i;
	}

#ifdef EVTLQ
	int getRecord_r(PMCTIME tm, PMCTIME etm, unsigned int count, rec *p, CString &filter)
	{
		if(count > MAX_GET_RECORD){
			count = MAX_GET_RECORD;
		}
		unsigned int i, ir = 0;
		rec *rs, *rt;
		rt = p;
		FILE *fp;
		rec snap[len];
		char fullName[_MAX_PATH], pool[_MAX_FNAME];
		int bComplete = 0;
		PMCTIME ptm = tm;
		CString stmp;
		while(getPoolByTime(&ptm, path, pool) && count > 0){
			sprintf(fullName, "%s/%s", path, pool);
			fp = fopen(fullName, "rb");
			if(fp){
				fread(snap, sizeof(rec), len, fp);
				rs = &snap[0];
				for(i = 0; i < len; i++){
					//if(!rs->valid || (etm !=0 && rs->rTime > etm)){
					//	bComplete = 1;
					//	break;
					//}
					if(rs->rTime > tm && ((rs->rTime < etm) || etm == 0)){
						stmp = rs->data;
						if(filter.IsEmpty() || stmp.Find(filter) >= 0){
							ir++;
							*rt = *rs;
							if(ir >= count){
								bComplete = 1;
								break;
							}
							rt++;
						}
					}
					rs++;
				}
				fclose(fp);
				if(bComplete){
					return ir;
				}
			}else{
				return ir;
			}
		}

		sprintf(fullName, "%s/temp", path);
		fp = fopen(fullName, "rb");
		if(fp){
			fread(snap, sizeof(rec), len, fp);
			rs = &snap[0];
			for(i = 0; i < len; i++){
				//if(!rs->valid || (etm !=0 && rs->rTime > etm)){
				//	bComplete = 1;
				//	break;
				//}
				if(rs->rTime > tm && ((rs->rTime < etm) || etm == 0)){
					stmp = rs->data;
					if(filter.IsEmpty() || stmp.Find(filter) >= 0){
						ir++;
						*rt = *rs;
						if(ir >= count){
							bComplete = 1;
							break;
						}
						rt++;
					}
				}
				rs++;
			}
			fclose(fp);
			if(bComplete){
				return ir;
			}
		}else{
			return ir;
		}

		return ir;
	}
#endif
	
	int getRecord(PMCTIME tm, PMCTIME etm, unsigned int count, rec *p)
	{
		if(count > MAX_GET_RECORD){
			count = MAX_GET_RECORD;
		}
		unsigned int i, ir = 0;
		rec *rs, *rt;
		rt = p;
		FILE *fp;
		rec snap[len];
		char fullName[_MAX_PATH], pool[_MAX_FNAME];
		int bComplete = 0;
		PMCTIME ptm = tm;
		while(getPoolByTime(&ptm, path, pool) && count > 0){
			sprintf(fullName, "%s/%s", path, pool);
			fp = fopen(fullName, "rb");
			if(fp){
				fread(snap, sizeof(rec), len, fp);
				rs = &snap[0];
				for(i = 0; i < len; i++){
					if(!rs->valid || (etm !=0 && rs->rTime > etm)){
						bComplete = 1;
						break;
					}
					if(rs->rTime > tm && ((rs->rTime < etm) || etm == 0)){
						ir++;
						*rt = *rs;
						if(ir >= count){
							bComplete = 1;
							break;
						}
						rt++;
					}
					rs++;
				}
				fclose(fp);
				if(bComplete){
					return ir;
				}
			}else{
				return ir;
			}
		}

		if(poolCount > 0){	
			if(use1){
				memcpy(snap, snapShot1, sizeof(snap));
			}else{
				memcpy(snap, snapShot2, sizeof(snap));
			}
			rs = &snap[0];
			for(i = 0; i < len; i++){
				if(!rs->valid || (etm !=0 && rs->rTime > etm)){
					break;
				}
				if(rs->rTime > tm && ((rs->rTime < etm) || etm == 0)){
					ir++;
					*rt = *rs;
					if(ir >= count){
						break;;
					}
					rt++;
				}
				rs++;
			}
		}

		return ir;
	}

private:
	rec snapShot1[len];
	rec snapShot2[len];
	bool use1;
	int p1, p2;
	char path[_MAX_PATH];
	unsigned int poolCount;
};

#endif
