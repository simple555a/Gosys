#ifndef __API_NICSYS_RTKTIME__
#define __API_NICSYS_RTKTIME__

#include "nicUtils.h"

#include "nicDefs.h"


typedef	struct __tag_RTK_TIME{
	nicUInt64 Data;
}RTK_TIME;

typedef RTK_TIME * PRTK_TIME;
typedef const RTK_TIME * PCRTK_TIME;

typedef struct TAG_UTILS_TIME{
	unsigned int lo;
	unsigned int hi;
}UTILS_TIME;




extern UTILS_API  double cpu_speed;


CDECL_BEGIN
/* returns CPU speed, in MHz */
UTILS_API  double NICSYS_API cpu_calibrate(void);
/*
returns time as CPU ticks, requires support of high-performance counter
in i586 or above, the built-in timer
*/
UTILS_API  void   NICSYS_API time_mark(UTILS_TIME * t);
/* returns time as CPU ticks since last reset of RDT */
UTILS_API  double NICSYS_API time_mark_d(void);
/* return time differences, in seconds */
UTILS_API  double NICSYS_API time_diff(const UTILS_TIME * _t1, const UTILS_TIME * t2);

UTILS_API void NICSYS_API rtk_time_mark(PRTK_TIME t);
UTILS_API double NICSYS_API rtk_time_diff(PCRTK_TIME end, PCRTK_TIME start);
UTILS_API void NICSYS_API rtk_sleep(nicUInt32 miliseconds);

UTILS_API int NICSYS_API rtk_format_time(time_t tmt,char *buf,int len);


#ifndef __BORLANDC__
static void randomize()
{
	UTILS_TIME t;
	time_mark(&t);
	srand(t.lo);
}
#endif
//产生随机数函数
UTILS_API int NICSYS_API rng_rand(int min,int max);
CDECL_END
#endif
