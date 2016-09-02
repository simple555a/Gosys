/*
	internal header used by fcc.exe
	do not include this file.
	this file is included from source files of controller-executables.
*/
#ifndef __kprocs_h__
#define __kprocs_h__

#ifndef F8_BASE_TYPE_DEFINED
#define F8_BASE_TYPE_DEFINED

typedef char f8_char;
typedef int f8_int;
typedef int f8_long;
typedef unsigned char f8_byte;

typedef long long int f8_i64;

typedef char f8_i8;
typedef short f8_i16;
typedef int f8_i32;

typedef unsigned char f8_u8;
typedef unsigned short f8_u16;
typedef unsigned int f8_u32;	
typedef unsigned long long int f8_u64;

typedef unsigned char f8_uchar;
typedef unsigned short f8_ushort;
typedef unsigned int f8_uint;
typedef unsigned int f8_ulong;
typedef f8_u8 f8_bool;

typedef float f8_r4;

typedef double f8_r8;

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef int integer;
typedef double date;

typedef f8_u32 f8_ptr;

typedef f8_u64 f8_uuid;

#ifndef __cplusplus
typedef unsigned char bool;

#define true 1
#define false 0
#endif /*__cplusplus */

#define TRUE 1
#define FALSE 0

#endif /*F8_BASE_TYPE_DEFINED*/

#ifndef BLK_SVC_PROC_DEFINED
#define BLK_SVC_PROC_DEFINED
typedef f8_u32 BLK_SERVICE_PROC;
typedef BLK_SERVICE_PROC BLK_SERVICE_TABLE[256];
#endif

/*
	pin is a 8-byte data structure that represent a pin's
	dynamic state, this will contribute to the bulk of the information
	that are transferred on the control network.
*/
struct pin{
	union {			
		f8_i8		i8;
		f8_i16		i16;
		f8_i32		i32;
		f8_i64		i64;
		f8_u8		ui8;
		f8_u16		ui16;
		f8_u32		ui32;
		f8_u64		ui64;
		f8_r4	flt;
		f8_r8	dbl;
	};
};

struct event_pin{
	int foo;
};

#define function(__id__)\
	void __id__()

#define SCHED_T_ONESHOT	0
#define SCHED_T_PERIODIC	1

#define sched(t, p, tp)	((int   (*)(double, struct event_pin *, f8_u8))__blk_services__[0])(t, p, tp)
#define cancel(sid, p)	((bool (*)(int, struct event_pin *))__blk_services__[1])(sid, p)

#define abs(i)		((int (*)(int))   __blk_services__[2])(i)
#define acos(f)		((double(*)(double))__blk_services__[3])(f)
#define asin(f)		((double(*)(double))__blk_services__[4])(f)
#define atan(f)		((double(*)(double))__blk_services__[5])(f)
#define atan2(f)	((double(*)(double))__blk_services__[6])(f)
#define cos(f)		((double(*)(double))__blk_services__[7])(f)
#define cosh(f)		((double(*)(double))__blk_services__[8])(f)
#define exp(f)		((double(*)(double))__blk_services__[9])(f)
#define fabs(f)		((double(*)(double))__blk_services__[10])(f)
#define fmod(f)		((double(*)(double))__blk_services__[11])(f)
#define log(f)		((double(*)(double))__blk_services__[12])(f)
#define log10(f)	((double(*)(double))__blk_services__[13])(f)
#define pow(f)		((double(*)(double))__blk_services__[14])(f)
#define sin(f)		((double(*)(double))__blk_services__[15])(f)
#define sinh(f)		((double(*)(double))__blk_services__[16])(f)
#define tan(f)		((double(*)(double))__blk_services__[17])(f)
#define tanh(f)		((double(*)(double))__blk_services__[18])(f)
#define sqrt(f)		((double(*)(double))__blk_services__[19])(f)

#define gettime()	((double(*)(void  ))__blk_services__[20])()
#define year(t)		((int (*)(double))__blk_services__[21])(t)
#define month(t)	((int   (*)(double))__blk_services__[22])(t)
#define day(t)		((int   (*)(double))__blk_services__[23])(t)

#define halt(e)		((void (*)(short))__blk_services__[24])(e)

#define hour(t)		((int (*)(double))__blk_services__[25])(t)
#define minute(t)	((int (*)(double))__blk_services__[26])(t)
#define second(t)	((int (*)(double))__blk_services__[27])(t)

#define fprecision()((double(*)(void))  __blk_services__[28])()
#define rand(a)		((int (*)(int))__blk_services__[29])(a)
#define io_mem(n)	((byte*(*)(byte))__blk_services__[30])(n)
#define x_mem(n)	((byte*(*)(byte))__blk_services__[31])(n)
#define dev_cfg(n)	((byte* (*)(word))__blk_services__[32])(n)

#define id_of_var(name) ((f8_uuid(*)(const char*))__blk_services__[100])(name)
#define read_var(id,p) ((f8_bool(*)(f8_uuid,void *))__blk_services__[101])(id,p) 
#define write_var(id,p) ((f8_bool(*)(f8_uuid,void *))__blk_services__[102])(id,p) 

#define f8_sprintf ((int(*)(char *,int,char*,...))__blk_services__[103])
#define f8_printf ((int(*)(char *,...))__blk_services__[104])
#define f8_strcpy
#define f8_strcmp
#define f8_strcmpi
#define f8_memcpy
#define f8_memset

/*
internal procedure calls
*/
#define F8_SYSTEM_CALL_IDX 200
#define f8_system_call(a,b,c,d) ((int)(*)(int,int,int,int))__blk_services__[F8_SYSTEM_CALL_IDX](a,b,c,d)

#define RS_TRIGGER(q,r,s) do{\
	if(r) \
		(q)=0;\
	else if(s)\
		(q)=1;\
}while(0)

#define SR_TRIGGER(q,r,s) do{\
	if(s) \
		(q)=1;\
	else if(r)\
		(r)=0;\
}while(0)

#endif

