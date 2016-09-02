#include <windows.h>
#include <stdio.h>
#include "libhis.h"

static inline void name2id(char name[_MAX_FNAME], unsigned int *id)
{
	char temp[_MAX_FNAME];
	memset(temp, 0 , _MAX_FNAME);
	int len = strlen(name);
	strncpy(temp, name, len-19);
	*id = atoi(temp);
}

static PMCTIME inline getPoolTime(char pool[_MAX_FNAME])
{
	char temp[_MAX_FNAME];
	int i = 0;
	while(pool[i] != '-'){
		i++;
	}
	i++;
	memset(temp, 0, sizeof(temp));
	strncpy(temp, &pool[i], 14);

	PMCTIME tm;
	str2time(temp, &tm);
	return tm;
}

HIS_API void time2str(PMCTIME tm, char str[])
{
	PMCSYSTIME sysTm;
	tm2systm(&tm, &sysTm);
	sprintf(str, "%04d%02d%02d%02d%02d%02d", sysTm.year, sysTm.month, sysTm.day, 
			sysTm.hour, sysTm.minute, sysTm.second);
}

HIS_API void time2str_x(PMCTIME tm, char str[])//mox
{
	PMCSYSTIME sysTm;
	tm2systm(&tm, &sysTm);
	sprintf(str, "%04dÄê%02dÔÂ%02dÈÕ%02d:%02d:%02d", sysTm.year, sysTm.month, sysTm.day, 
		sysTm.hour, sysTm.minute, sysTm.second);
}

HIS_API void str2time(char str[], PMCTIME *tm)
{
	PMCSYSTIME sysTm;
	char temp[5];
	
	memset(temp, 0, 5);
	strncpy(temp, &str[0], 4);
	sysTm.year = atoi(temp);
	memset(temp, 0, 5);
	strncpy(temp, &str[4], 2);
	sysTm.month = atoi(temp);
	memset(temp, 0, 5);
	strncpy(temp, &str[6], 2);
	sysTm.day = atoi(temp);
	memset(temp, 0, 5);
	strncpy(temp, &str[8], 2);
	sysTm.hour = atoi(temp);
	memset(temp, 0, 5);
	strncpy(temp, &str[10], 2);
	sysTm.minute = atoi(temp);
	memset(temp, 0, 5);
	strncpy(temp, &str[12], 2);
	sysTm.second = atoi(temp);
	sysTm.milliSeconds = 0;
	systm2tm(&sysTm, tm);
}

HIS_API void tm2systm(const PMCTIME *tm, PMCSYSTIME *sysTm)
{
	FILETIME ftm;
	SYSTEMTIME stm;
	memcpy(&ftm, tm, sizeof(ftm));
	FileTimeToSystemTime(&ftm, &stm);
	sysTm->year = stm.wYear;
	sysTm->month = stm.wMonth;
	sysTm->day = stm.wDay;
	sysTm->hour = stm.wHour;
	sysTm->minute = stm.wMinute;
	sysTm->second = stm.wSecond;
	sysTm->milliSeconds = stm.wMilliseconds; 
}

HIS_API void systm2tm(const PMCSYSTIME *sysTm, PMCTIME *tm)
{
	FILETIME ftm;
	SYSTEMTIME stm;
	stm.wYear = sysTm->year;
	stm.wMonth = sysTm->month;
	stm.wDay = sysTm->day;
	stm.wHour = sysTm->hour;
	stm.wMinute = sysTm->minute;
	stm.wSecond = sysTm->second;
	stm.wMilliseconds = sysTm->milliSeconds;
	SystemTimeToFileTime(&stm, &ftm);
	memcpy(tm, &ftm, sizeof(ftm));
}

HIS_API PMCTIME getCurrentTime()
{
	SYSTEMTIME stm;
	FILETIME ftm;
	GetLocalTime(&stm);
	SystemTimeToFileTime(&stm, &ftm);

	PMCTIME tm;
	memcpy(&tm, &ftm, sizeof(ftm));
	return tm;
}

HIS_API bool getCurrentPool(char path[_MAX_PATH], char poolCurrent[_MAX_FNAME])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	bool ret = false;

	char find[_MAX_PATH];
	sprintf(find, "%s/*.dac", path);
	hFind = FindFirstFile(find, &FindFileData);
	if(hFind != INVALID_HANDLE_VALUE){
		sprintf(poolCurrent, "%s", FindFileData.cFileName);
		ret = true;
	}else{
		PMCTIME tm = getCurrentTime();
		char str[15];
		time2str(tm, str);
		sprintf(poolCurrent, "0-%s.dac", str);
	}
	FindClose(hFind);

	return ret;
}

HIS_API bool getPoolById(char path[_MAX_PATH], char pool[_MAX_FNAME], unsigned int id)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	bool ret;

	char find[_MAX_PATH];
	sprintf(find, "%s/%d-*.dat", path, id);
	hFind = FindFirstFile(find, &FindFileData);
	if(hFind != INVALID_HANDLE_VALUE){
		sprintf(pool, "%s", FindFileData.cFileName);
		ret = true;
	}else{
		ret = false;
	}
	FindClose(hFind);

	return ret;
}

HIS_API bool moveNextPool(char path[_MAX_PATH], unsigned int poolCount)
{
	char oldName[_MAX_PATH], newName[_MAX_PATH];
	char pool[_MAX_FNAME];
	unsigned int id;
	
	if(getCurrentPool(path, pool)){
		name2id(pool, &id);
		sprintf(oldName, "%s/%s", path, pool);
		memset(newName, 0, sizeof(newName));
		strncpy(newName, oldName, strlen(oldName)-1);
		strcat(newName, "t");
		if(rename(oldName, newName) != 0){
			return false;
		}
	}else{
		id = 0;
	}

	id++;
	if(id == poolCount){
		id = 0;
	}
	PMCTIME tm = getCurrentTime();
	char str[15];
	time2str(tm, str);
	if(getPoolById(path, pool, id)){
		sprintf(oldName, "%s/%s", path, pool);
	}else{
		sprintf(oldName, "%s/%d-%s.dat", path, id, str);
	}
	sprintf(newName, "%s/%d-%s.dac", path, id, str);
	FILE *f = fopen(oldName, "w");
	if(f == NULL){
		return false;
	}
	fclose(f);
	rename(oldName, newName);

	return true;
}

HIS_API void markNewTimeId(char pool[_MAX_FNAME])
{
	char str[15];
	PMCTIME tm = getCurrentTime();
	time2str(tm, str);

	int i = 0;
	while(pool[i] != '-'){
		i++;
	}
	i++;
	for(int j = 0; j < 14; j++, i++){
		pool[i] = str[j];
	}
}

HIS_API bool getPoolByTime(PMCTIME *tm, char path[_MAX_PATH], char pool[_MAX_FNAME])
{
	PMCTIME i, itm, itmc = 0;
	char poolc[_MAX_FNAME];
	itm = *tm;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char find[_MAX_PATH];
	sprintf(find,"%s/*-*.dat", path);
	hFind = FindFirstFile(find, &FindFileData);
	if(hFind != INVALID_HANDLE_VALUE){
		if(!(FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes)){
			i = getPoolTime(FindFileData.cFileName);
			if(i > itm){
				itmc = i;
				sprintf(poolc, "%s", FindFileData.cFileName);
			}
		}
		while(FindNextFile(hFind, &FindFileData)){
			if(!(FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes)){
				i = getPoolTime(FindFileData.cFileName);
				if(i > itm && (i < itmc || itmc == 0)){
					itmc = i;
					sprintf(poolc, "%s", FindFileData.cFileName);
				}
			}
		}
	}
	FindClose(hFind);

	if(itmc == 0){
		return false;
	}else{
		sprintf(pool, "%s", poolc);
		*tm = itmc;
		return true;
	}
}
