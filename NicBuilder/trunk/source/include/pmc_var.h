/*
	Plant Monitoring & Controlling System source
	
	jhunter,2004/8

	this file defines the structure holding the realtime data
	as well as its type and attributes.
*/
#ifndef __pmc_var_h__
#define __pmc_var_h__

#include <oaidl.h>

#include <rtkdefs.h>
#include <pshpack1.h>

/*
	tag's general flag (TF_ constants)
	
	Various system components use these flags, including
	io subsystem, device drivers, rtk_proxy, configuration ui,
	archieving subsystem. We mess them up into a 32-bit
	integer simply for performance. Keep in mind that these
	components generally have little common logic, if any, 
	when accessing these flags. Although some connections 
	do exist, e.g., the TF_Valid is commonly set in the ioss
	component while accessed through almost all other
	components.
*/

/*
	the tag's configuration is validated through two stages,
	firstly through the RTDB layer, and the result of the
	validation is stored in the bit flag : TF_ConfigOk. 

	the next validation is done in the device driver layer,
	the Address field is checked, the result for this step
	is stored in the TF_Translated flag.

	A tag must have both flags set to be evaluated by IOSS.
*/
#define TF_HiHi 			(0x00000001 << 8)
#define TF_Hi				(0x00000001 << 9)
#define TF_Lo				(0x00000001 << 10)
#define TF_LoLo 			(0x00000001 << 11)
#define TF_On2Off			(0x00000001 << 12)
#define TF_Off2On			(0x00000001 << 13)
#define TF_Rate 			(0x00000001 << 14)
#define TF_Alarm			(0x00000001 << 15)
#define TF_Valid			(0x00000001 << 16)
#define TF_Expired		(0x00000001 << 17)
#define TF_SharpChange	(0x00000001 << 18)	// 
#define TF_ReadOnly 		(0x00000001 << 19)
#define TF_Translated		(0x00000001 << 20)
#define TF_SaveToHistory	(0x00000001 << 21)
#define TF_ConfigOk 		(0x00000001 << 22)
#define TF_System			(0x00000001 << 23)
#define TF_Shutdown		(0x00000001 << 24)
#define TF_AlarmIfNA		(0x00000001 << 25)
#define TF_StaticAlloc		(0x00000001 << 26)
#define TF_Step			(0x00000001 << 27)
//#define TF_ACKAlarm			(0x00000001 << 30)         //2015.03.20 GM   点击确认报警按钮后，控制声音播放标志位

/*
	Note : we pack the type and flags into a 4-byte 
	integer.
*/
typedef __u8 TAG_TYPE;
typedef __u32 TAG_FLAGS;

#define get_value_type(flags) ((flags) & 0xff)
#define set_value_type(flags, t) ((flags) = ((flags) & 0xffffff00) | (t))

enum tag_type_t{
/*
	original data types
*/
	dt_null = 0,
	dt_bool = 1,
	dt_real4 = 2,
	dt_int32 = 3,	
	dt_real8 = 4,
	dt_date = 5,
	
	dt_int8 = 6,
	dt_int16 = 7,
	dt_int64 = 8,
	
	dt_uint8 = 9,
	dt_uint16 = 10,
	dt_uint32 = 11,
	dt_uint64 = 12,

	dt_string = 13,
	dt_bstr = 14, 

	dt_array = 15,
	dt_dispatch = 16,

	dt_MaxType = 17,
};

/*
	change this to port to 64-bit or 16-bit platforms
*/
#define dt_int dt_int32
#define dt_uint dt_uint32
#define dt_float dt_real4
#define dt_double dt_real8

/*
	the PMC equivalence of OLE's variant structure
*/
typedef struct __tag_TAG_VALUE{
	union	{
		__r4		flt;
		__r4		fltValue;	// alias for flt
		__bool	b;
		__int		i;
		__int		iValue; // alias for i
		
		__i8		i8;
		__i16	i16;
		__i32	i32;	
		__i64	i64;
		__i64	i64Value; // alias for i64
		
		__u8	u8;
		__u16	u16;
		__u32	u32;
		__u64	u64;

		__r8		dbl;
		__r8		dblValue;// alias for dbl

		/* number of seconds since Jan 1, 1601 */
		__r8		date;
		
		/* partially supported types(used only as interface types to OLE) */
		char *	str;
		wchar_t * wstr;

		void * array;
		/* a dispatch interface pointer, semantics of the pointer
		is yet un-defined, 2005-5-20
		*/
		void	* dispatch;
	};
}PACKED TAG_VALUE, *PTAG_VALUE;
typedef const TAG_VALUE * PCTAG_VALUE;

/*
	This struct is used in client/server communication
*/
typedef struct pmc_value_t{
	TAG_VALUE Value;
	TAG_FLAGS  Flags;
}*Ppmc_value_t;
typedef const struct pmc_value_t *PCpmc_value_t;

#include <poppack.h>

CDECL_BEGIN
	
RTDB_API __bool PMC_API pmc_type_cast(PCpmc_value_t s, Ppmc_value_t d);

RTDB_API void PMC_API pmc_empty(Ppmc_value_t v);

RTDB_API void PMC_API pmc_clear(Ppmc_value_t v);

RTDB_API char * PMC_API pmc_value_string(PCpmc_value_t v);

RTDB_API __bool PMC_API pmc_value_to_variant(
	PCpmc_value_t p, 
	VARIANT* tagValue
	);

RTDB_API __bool PMC_API pmc_variant_to_value(
	const VARIANT* tagValue,
	Ppmc_value_t p
	);

RTDB_API __u16 PMC_API pmc_type_to_variant_type(__u8 t);

RTDB_API __u8 PMC_API pmc_type_from_variant_type(__u16 vt);

RTDB_API int PMC_API pmc_value_add(
	struct pmc_value_t * d, 
	const struct pmc_value_t * o1, 
	const struct pmc_value_t * o2
	);
RTDB_API int PMC_API pmc_value_sub(
	struct pmc_value_t * d, 
	const struct pmc_value_t * o1, 
	const struct pmc_value_t * o2
	);
RTDB_API int PMC_API pmc_value_mul(
	struct pmc_value_t * d, 
	const struct pmc_value_t * o1, 
	const struct pmc_value_t * o2
	);
RTDB_API int PMC_API pmc_value_div(
	struct pmc_value_t * d, 
	const struct pmc_value_t * o1, 
	const struct pmc_value_t * o2
	);

CDECL_END
	
#endif
