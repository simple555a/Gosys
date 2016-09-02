/*
* OS routines needed by f8 executive
* by jackie pan, panxiaohui@actech-mc.com
* Sep 13, 2006
**/
int sysDebugTrace(int code);
void sysStallExecutionProcessor(int microseconds);
int sysDisplayString(const char * s);
int sysPrintf(const char * fmt, ...);
int sysBugCheck(int code);
void sysDelayMs(int milliseconds);
int sysGetA_BSetting();
void sysEnableWatchdog(int timeout);
void sysRefreshWatchdog(int timeout);
void sysDisableWatchdog();

#ifdef __VXWORKS__
STATUS sysIoChannelEnable(int c);
STATUS sysIoChannelDisable(int c);
STATUS sysWriteNe2000(int c, void * d, int len,int timeout);
void sysNe2000SetAddr(int c, char * a);
STATUS sysIoChannelHook(int c, STATUS (*hook)(int, char *, int));
STATUS sysIoChannelUnhook(int c);
#endif

extern int microsecondCount;
double sysTimeMark();

