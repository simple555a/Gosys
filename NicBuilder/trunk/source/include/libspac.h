/*
    PC based Monitoring System source file
    version 1.0
    by J.Hunter,jhunter@263.net,July 2001

	This is the proxy declaration for the Serial Port Access Coordinator(SPAC).
*/
#ifndef __libspac_h__
#define __libspac_h__

#include <pmc_config.h>

//#define LIBSPAC_EXPORTS

#ifdef _WIN32
    #ifdef LIBSPAC_EXPORTS
        #define LIBSPAC_API      __declspec(dllexport)
    #else
        #define LIBSPAC_API      __declspec(dllimport)
    #endif  
#else
    #define LIBSPAC_API
#endif

#ifdef __cplusplus
extern "C"{
#endif

#define SPAC_READ	0
#define SPAC_WRITE	1
#define SPAC_FLUSH	2
#define SPAC_OPEN	3
#define SPAC_CLOSE	4

#define SPAC_DEFAULT_SETTING "57600,m,8,1"

int LIBSPAC_API PMC_API init_spac();
int LIBSPAC_API PMC_API uninit_spac();
int LIBSPAC_API PMC_API lock_port(int PortNum, int TimeOut);
int LIBSPAC_API PMC_API unlock_port(int PortNum);
int LIBSPAC_API PMC_API flush_port(int pno, int timeout);
int LIBSPAC_API PMC_API open_port(int pno, int timeout);
int LIBSPAC_API PMC_API close_port(int pno, int timeout);
int LIBSPAC_API PMC_API read_port(int pno, char * buf, int *size,int timeout);
int LIBSPAC_API PMC_API write_port(int pno, char * buf, int size,int timeout);

#ifdef __cplusplus
};
#endif

#endif
