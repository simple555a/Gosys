#ifndef _convert_type_h_
#define _convert_type_h_

#include "precomp.h"

#define RET_Fail -1
#define RET_OK 0


typedef struct 
{
	//nicTime				Time ;				// ʱ���
	pmc_value_t        value;
	RTK_TIME           time;
	//pmc_value_t			Value ;				// ֵ
	__u8				Quality ;			// ������
} DBData;	


#endif //_convert_type_h_
