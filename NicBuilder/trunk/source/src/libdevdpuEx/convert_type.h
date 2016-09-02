#ifndef _convert_type_h_
#define _convert_type_h_

#include "precomp.h"

#define RET_Fail -1
#define RET_OK 0


typedef struct 
{
	//nicTime				Time ;				// 时间戳
	pmc_value_t        value;
	RTK_TIME           time;
	//pmc_value_t			Value ;				// 值
	__u8				Quality ;			// 质量戳
} DBData;	


#endif //_convert_type_h_
