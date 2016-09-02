#ifndef _NICSYS_ALARMKERNEL_H__
#define _NICSYS_ALARMKERNEL_H__

#include "nicDefs.h"

#ifdef _WIN32
#if defined	LIB_ALARMKERNEL_EXPORTS || defined LIB_ALARMCELL_EXPORTS
#define ALARMKERNEL_API	  __declspec(dllexport)
#else
#define ALARMKERNEL_API	  __declspec(dllimport)
#endif
#else
#define ALARMKERNEL_API
#endif




#endif
