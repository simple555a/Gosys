/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	ctrller/include/f8.h
	author:	Jackie Pan, jhunter@mail.tsinghua.org.cn
	
	purpose:	F8 master include file	
	history:
	created:	12/5/2002   9:49
*********************************************************************/
#ifndef __f8_h__
#define __f8_h__

#ifdef __VXWORKS__
#include <vxworks.h>
#endif

/* import host machine basic types */
#include <cpu_features.h>

#include <f8_status.h>
#include <f8_config.h>
#include <f8_vm.h>
#include <f8_binary.h>
#include <f8_kernel.h>
#include <f8_cc.h>
#include <f8_mm.h>

#if defined(F8_CONFIGURATOR) && !defined(FCC_INVOKED)
#include <f8_resource.h>
#endif

#include <f8_fbdar.h>

#endif

