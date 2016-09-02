/*
 * PC-based monitoring & controlling system
 * Actech M&C Inc., Beijing, China
 * (c) Jackie, jhunter@tsinghua.org.cn

 Intel-x86 archietecture type definition

 * */
#ifndef __cpu_features_h__
#define __cpu_features_h__

typedef float	__r4;
typedef double	__r8;

typedef char	__i8;
typedef short	__i16;
typedef int		__i32;
#ifdef __GNUC__
typedef long long __i64;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64 __i64;
#elif defined(__LCC__)
typedef struct __tag_i64{
	__i32	hi;
	__i32	lo;
}__i64;
#else
#error "64 bit supported not found, revise me!!!"
#endif

typedef unsigned char		__u8;
typedef unsigned short		__u16;
typedef unsigned int		__u32;
#ifdef __GNUC__
typedef unsigned long long __u64;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 __u64;
#elif defined(__LCC__)
typedef __i64	__u64;
#else
#error "64 bit supported not found, revise me!!!"
#endif

typedef union __tag_r4i32{
	__r4 flt;
	__i32 i;
}r4i32_t;

typedef int		__int;
typedef unsigned int		__uint;
typedef unsigned char		__bool;
typedef unsigned long		__ulong;
typedef long				__long;

#define HOST_IS_SMALL_ENDIAN	1
// #define HOST_IS_BIG_ENDIAN 0

#endif // __cpu_features_h__
