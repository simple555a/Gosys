#ifndef _DPU_h_
#define _DPU_h_

#include "pmc.h"

IOSS_DLL_EXPORT __bool start_device(PDEVICE_INFO h);
IOSS_DLL_EXPORT __bool stop_device(PDEVICE_INFO h);
IOSS_DLL_EXPORT __bool write_device(PRTK_TAG tte, PCTAG_VALUE value);





#endif//_DPU_h_