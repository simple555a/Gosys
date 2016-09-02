/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Configuration parameters	
    This file contains system-wide configurable parameters.
    Modification on these variables will significantly affect
    runtime system efficiency, so, take care. 
	history:
	Created : May 31, 2001, version 1.0
	created:	10:10:2002   8:02, version 2.03
*********************************************************************/
#ifndef __pmc_config_h__
#define __pmc_config_h__

#include <rtkdefs.h>

// Real time kernel
// the network layer can only communicate to other 
// node of the same RTK version
/* 
	Version encoding :
	0x 0 2  0 3  0 0 2 5
	   ---  ---  -------
	   |    |      |______ Build number, increase each build
	   |    |_____________ Minor version
	   |__________________ Major version
*/
#ifndef RTK_VERSION
#define RTK_VERSION	0x03510816
#endif
/*
  one and only one of the following macros must be defined
 */
#if !defined(_WIN32) && !defined(__LINUX__) && !defined(__VXWORKS__)
#error "Unexpected platform."
#endif

#ifdef PMC_1_0_COMPATIBLE
//#define rtkm_node_key_length    8
//#define rtkm_group_key_length   8
//#define rtkm_tag_key_length     8
#else
/* constants in librtdb.dll */
#define rtkm_node_key_length    16
#define rtkm_group_key_length   16
#define rtkm_tag_key_length     32
#endif

#define rtkm_pict_key_length	64

/*
#define rtkm_max_nodes          256
#define rtkm_max_groups         512
#define rtkm_max_tags           (1024L*100)
*/
#define RTK_DEFAULT_NODE_KEY    "DEFAULT"
#define RTK_DEFAULT_GROUP_KEY   "DEFAULT"
#define rtkm_onmsg_length       64
#define rtkm_eu_length          64
#define rtkm_description_length 64
#define rtkm_tag_address_length 128

/* constants in rtknet.dll */
#define MAX_PACKET_SIZE				8192
#define rtkm_delay_between_fragments	10

// db refresher wakes up every 50 milliseconds
#define rtkm_refresh_quantum			50

/*
    The three macros below controls the threading model used in network
    listening.
    If rtkm_use_event_select is defined, WSAEventSelect is used to select
    incoming packets.
    If rtkm_use_msg_select is defined, WSAAsyncSelect is used to select
    incoming packets.
    If rtkm_use_no_select is defined, the threads blindly wait on incoming
    packets, and will be killed (TerminateThread) when the program is about to
    quit.
*/
//#define rtkm_use_event_select
#define rtkm_use_msg_select
//#define rtkm_use_no_select
#define rtkm_default_transaction_life 300

/* constants in rtkproxy.dll */
#define rtkm_default_node_life		6000
#define rtkm_default_tag_life		60		// 60 seconds before tag expires
#define rtkm_default_change_rate	 0.025
#define ALARM_ACK_RANK				0
#define ALARM_WRITE_DEVICE_RANK		0
#define rtkm_system_msg_length		4096

/*
	2002/10/15, virtual bus's broadcast property can only be simulated through
	socket layer's multicast mechanism.
*/
#define USE_MULTICAST 
#define RTKNET_PORT_BASE			15426
#define RTKNET_MULTICAST_IP_PREFIX	"230.10.10"

#define HISLOG_NOTIFY_EVENT	"MCHISLOG-CONFIG"
#define HISLOG_EXIT_EVENT	"MCHISLOG-EXIT"
#define SPAC_RESET_EVENT	"PMC-SPAC-RESET"

#ifdef _WIN32
#define PMC_API _stdcall
#else
#define PMC_API
#endif

#endif
