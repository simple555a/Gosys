/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose : Echo Device Driver
	history : 
	created : by J.Hunter, Jul.,2001
	created : 10:9:2002	  15:36
*********************************************************************/
#include <ddk.h>

#pragma comment(linker, "/base:0x76520000")


IOSS_DLL_EXPORT __bool write_device(PRTK_TAG tte, PCTAG_VALUE value)
{
	*((PTAG_VALUE)tte->d.BinaryAddress) = *value;
	// tte->d.Value = *value;
	return __true;
}

IOSS_DLL_EXPORT void update_tag(PCRTK_TAG tte, pmc_value_t * pt, PCRTK_TIME)
{
	pt->Flags |= TF_Valid;
	// pt->Value = tte->d.Value;
	pt->Value = *((PTAG_VALUE)tte->d.BinaryAddress);
}

