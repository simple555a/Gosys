/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename:	E:\vss\pmc2\src\calcnpu\npu.cpp
	file path:	E:\vss\pmc2\src\calcnpu
	author:	Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose : Calc NPU plugin
	
	history : 
	
	created : by J.Hunter, Jul. 2001

	modified : 10:9:2002   15:36
	interface upgrade to version 2.0
	
	modified : by Jackie, Feb. 2004
	added support for ``span'' function

	modified : by jackie, Aug. 2004
	multiple tag-type supported.

*********************************************************************/
#include <pmc.h>
#include "expression.h"

#pragma comment(linker, "/base:0x76500000")
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "libdevmath.lib")

struct hw_buf{
	TAG_VALUE	tv;
	/* the following field is for libmath.dll use only */
	RTK_TIME	last_update_time;
	char		write_pending;
	char		padding[3];
}PACKED;

IOSS_DLL_EXPORT __bool write_device(PRTK_TAG tte, PCTAG_VALUE value)
{
	hw_buf * hb;

	hb = (hw_buf*)tte->d.BinaryAddress;
	hb->tv = *value;
	hb->write_pending = __true;

	return __true;
}

IOSS_DLL_EXPORT void update_tag(PCRTK_TAG tte, pmc_value_t * pt, PCRTK_TIME)
{
	hw_buf * hb;
	pmc_value_t dblVal;

	CArgs args;
	parse_arg_ex(tte->s.Address, &args, "#");

	pt->Flags &= ~TF_Valid;

	set_value_type(dblVal.Flags, dt_real8);
	
	hb = (hw_buf*)tte->d.BinaryAddress;
	if(hb->write_pending){
		pt->Value = hb->tv;
		hb->write_pending = __false;
	}else{
		if(!tte->s.Address[0]){
			return;
		}
		if( !evaluate(tte->s.Address, &dblVal.Value.dbl) ){
			return;
		}
		// type-cast
		pmc_type_cast(&dblVal, pt);
	}

	pt->Flags |= TF_Valid;
}

/*
*/
IOSS_DLL_EXPORT __bool address_translate(PRTK_TAG tte)
{
	assert(sizeof(struct hw_buf) <= sizeof(tte->d.BinaryAddress));
	return __true;
}

