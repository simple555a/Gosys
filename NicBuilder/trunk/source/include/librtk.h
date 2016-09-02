/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	e:\vss\pmc2\src\include\librtk.h
	file path:	e:\vss\pmc2\src\include
	author:	Jackie Pan, jhunter@tsinghua.org.cn

	purpose:	Common procedures used by real time kernel.
	history:
	created:	10:8:2002   13:35
*********************************************************************/
#ifndef __librtk_h_
#define __librtk_h_

#pragma warning(disable:4996)

#include <string.h>

#include <utils.h>
#include <assert.h>
#include <rtkdefs.h>
#include <rtk_compat.h>
#include <rtk_error.h>
#include <rtk_config.h>
#include <rtk_list_entry.h>
#include <time.h>

#ifdef __LINUX__
// be sure to define XOPEN_SOURCE >= 500, or pthread_rwlock_t won't be defined
#include <pthread.h>
#endif

#ifdef _WIN32
#ifdef LIBRTK_EXPORTS
#define LIBRTK_API      __declspec(dllexport)
#else
#define LIBRTK_API      __declspec(dllimport)
#endif
#define RTK_API __stdcall
#else
#define LIBRTK_API
#define RTK_API
#endif

#include <rtk_error.h>

/*
	2004/8, we don't provide case-insensitive storage any longer
*/
#define rtkm_key_stored_with_case

CDECL_BEGIN

LIBRTK_API int RTK_API rtk_set_last_error(int e);
LIBRTK_API int RTK_API rtk_get_last_error();

CDECL_END


typedef	struct __tag_RTK_TIME{
	__u64	Data;
}RTK_TIME;

typedef RTK_TIME * PRTK_TIME;
typedef const RTK_TIME * PCRTK_TIME;

typedef struct __tag__RTK_BUF{
	char	*buf;
	int	len;
}RTK_BUF, *PRTK_BUF;
typedef const RTK_BUF* PCRTK_BUF;

typedef void * RTK_HANDLE;

#ifndef rtkm_key_stored_with_case
/*
	case insensitive storage
*/
#define make_unified_key(k)\
	do{\
		int i=0;\
		char *p=(k).Data;\
		while(*p && i<sizeof((k).Data)){\
			*p = toupper(*p);\
			i++, p++;\
		}\
	}while(0)
#else
// case sensitive storage
#define make_unified_key(k) 0
#endif

CDECL_BEGIN

LIBRTK_API void RTK_API rtk_time_mark(PRTK_TIME t);
// return difference of two RTK_TIMEs, in SECONDS
LIBRTK_API double RTK_API rtk_time_diff(PCRTK_TIME end, PCRTK_TIME start);
LIBRTK_API void RTK_API rtk_sleep(__uint miliseconds);
LIBRTK_API void * RTK_API alloc_buffer(int count, PCRTK_BUF bufs);
LIBRTK_API char * RTK_API rtk_alloc_string(const char * str);
LIBRTK_API void * RTK_API rtk_alloc_memory(__uint size);
LIBRTK_API void * alloc_buffer_v(const void *buf, int len, ...);
LIBRTK_API void   RTK_API free_buffer(void *buf);
LIBRTK_API void * RTK_API fill_buffer(int count, void *heap, PCRTK_BUF bufs);
LIBRTK_API void * fill_buffer_v(void *buf,  ...);
LIBRTK_API void RTK_API close_handle(RTK_HANDLE);

LIBRTK_API int RTK_API rtk_logged_puts(MSG_LEVEL level, char *s);

CDECL_END

// seems only Win32 defines this constant
#ifndef _WIN32
#define INFINITE        ((__uint)-1)
#define WAIT_SUCCESS    WAIT_OBJECT_0
#define WAIT_OBJECT_0   256
#define WAIT_TIMEOUT	((__uint)-2)
#define WAIT_FAILED     ((__uint)-1)
#define TRUE            1
#define FALSE           0
#endif

#ifdef __cplusplus

#include <pshpack1.h>

class LIBRTK_API RTK_GUID{
public:
	__u8	data[16];
	// randomly generately a uuid
	void create();
	// string size
	inline int name_size() const
	{
		return 43;
	}
	// display the uuid in a readable manner
	int to_string(char * buffer) const;
};
typedef RTK_GUID *PRTK_GUID;
typedef const RTK_GUID *PCRTK_GUID;

template<typename T> 
int rtk_compare_key(const T & k1, const T & k2)
{
#ifdef rtkm_key_stored_with_case
	return strnicmp((const char *)&k1, (const char *)&k2, sizeof(T));
#else
	return memcmp((const char *)&k1, (const char *)&k2, sizeof(T));
#endif
}

template<typename T> 
int rtk_compare_key_case(const T & k1, const T & k2)
{
	return strncmp((const char *)&k1, (const char *)&k2, sizeof(T));
}

// the following overloaded operators is useful when implementating
// a map class based on key-type of RTK_GUID
inline bool operator < (const RTK_GUID & g1, const RTK_GUID & g2)
{
	return memcmp(g1.data, g2.data, sizeof(g1.data))<0? true : false;
}
inline bool operator == (const RTK_GUID & g1, const RTK_GUID & g2)
{
	return memcmp(g1.data, g2.data, sizeof(g1.data))==0? true : false;
}

template <class __Type>
class CAlphaName{
public:
	CAlphaName(const char * alpha)
	{
		memset(buf, 0, sizeof(buf));
		strncpy(buf, alpha, sizeof(buf) - 1);
#ifndef rtkm_key_stored_with_case
		strupr(buf);
#endif
		m_theType = *this;
	}
	CAlphaName(const __Type &t)
	{
		memset(buf, 0, sizeof(buf));
		memcpy(buf, &t, sizeof(t.Data));
		m_theType = *this;
	}
	inline operator char *()
	{
		return buf;
	}
	inline operator __Type ()
	{
		__Type t;
		memcpy(t.Data, buf, sizeof(t.Data));
		return t;
	}
	inline operator const __Type * ()
	{
		return &m_theType;
	}
private:
	__Type m_theType;
	char buf[sizeof(__Type) + 1];
};

#include <poppack.h>

#else	// __cplusplus
typedef struct __tag_RTK_GUID{
	__u8	data[16];
}RTK_GUID;
typedef RTK_GUID *PRTK_GUID;
typedef const RTK_GUID *PCRTK_GUID;

#endif	// __cplusplus

CDECL_BEGIN

LIBRTK_API char * RTK_API get_config_file();
LIBRTK_API char * RTK_API get_working_dir();
LIBRTK_API char * RTK_API get_exe_dir();
LIBRTK_API void RTK_API set_working_dir(const char * dir);
LIBRTK_API void * RTK_API html_help(
						 void * hwndCaller,
						 const char * pszFile,
						 __uint uCommand,
						 __uint * dwData
						 );

LIBRTK_API void RTK_API qsort_r(
	void *base, 
	unsigned num, 
	unsigned width, 
	int (*comp)(void * context, const void *, const void *),
	void * context
	);

LIBRTK_API void * RTK_API bsearch_r(
	const void *key,
	const void *base,
	size_t num,
	size_t width,
	int (*compare)(void * context, const void *, const void *),
	void * context
	);

LIBRTK_API int RTK_API rtk_format_time(time_t tmt,char *buf,int len);

CDECL_END

#endif	// __librtk_h_

