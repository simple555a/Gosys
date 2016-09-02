/*
	PMC 2.0 source file
	by Jackie Hunter, jhunter@tsinghua.org.cn
*/

#ifndef __rtk_error_h__
#define __rtk_error_h__

/*
	an error number is composed by a module number and an error
	number, simply concatenating them by a shift operator.
*/

#define PMC_ERROR(modNum, errNum) (((__u16)(modNum)) << 16 | (__u16)(errNum))

#define PMC_SUCCESS(errCode) (((errCode)&0x8000)? __false : __true)
#define PMC_FAILED(errCode) (((errCode)&0x8000)? __true : __false)

#define RTK_NO_ERROR				0

#endif

