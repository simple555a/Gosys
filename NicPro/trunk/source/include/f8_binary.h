/*
	Controller binary specification header file
	by panxh, panxiaohui@actech-mc.com
	Create : Mar. 7, 2003
	Mar. 14, 2003, c-simulated object service support added, Jackie.
*/

#ifndef __ctrller_binary_h__
#define __ctrller_binary_h__

#include <f8_vm.h>
// #include <f8_resource.h>
#include <rtk_list_entry.h>
#include <streamable.h>
#include <treelist.h>
#include <c_bstr.h>

struct mem_addr_t{
	f8_u16	addr;
	f8_u8	section;
	f8_u8	padding_0;
}PACKED;

#ifndef BLK_SVC_PROC_DEFINED
#define BLK_SVC_PROC_DEFINED
typedef __u32 BLK_SERVICE_PROC;
typedef BLK_SERVICE_PROC BLK_SERVICE_TABLE[256];
#endif

// pin class constants
#define PIN_CLASS_EI	'e'		// event input
#define PIN_CLASS_EO	'E'		// event output
#define PIN_CLASS_DI	'd'		// data input
#define PIN_CLASS_DO	'D'		// data output
#define PIN_CLASS_EXP	'x'		// staticaly exported pins

// data pin type constants
#define PIN_T_EVENT		'E'

#define PIN_T_NONE		'0'
#define PIN_T_INTEGER	'I'
#define PIN_T_BOOL		'B'
#define PIN_T_FLOAT		'F'
#define PIN_T_TIMER		'T'
#define PIN_T_BYTE		'Y'
#define PIN_T_WORD		'W'
#define PIN_T_DOUBLE	'D'
#define PIN_T_DATE		'A'
#define PIN_T_CHAR		'C'
#define PIN_T_SHORT		'S'
#define PIN_T_DWORD		'L'

#define PIN_T_STRUCT	'R'
#define PIN_T_ARRAY		'V'
#define PIN_T_POINTER	'P'
#define PIN_T_ANY		'N'

/* flag modifiers on data pins */
#define PIN_F_VALID		(0x1<<0)	/* for IO pins, is current value valid? */
#define PIN_F_INVERTED	(0x1<<2)	/* for PIN_T_BOOL, invert logic before in/out */
#define PIN_F_HIDDEN	(0x1<<1)	/* pin is for internal use only */

/* binding enumeration */
#define PIN_B_CONST		0	/* if pin's value is an immediate(in local dyn_pin_t) */
#define PIN_B_MEMORY	1	/* pin's value in exchange memory, section 0/1/2/3/4/5 */

#include <f8_fbd.h>
#include <f8_ld.h>
#include <f8_sfc.h>
#include <f8_st.h>
#include <f8_il.h>

#endif	/* dual-inclusion guard */

