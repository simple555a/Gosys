#ifndef __API_NICSYS_STRINGFUNCTION__
#define __API_NICSYS_STRINGFUNCTION__


#include "nicDefs.h"
#include "nicUtils.h"

//����ǩ���Ƿ�Ϸ���ֻ������ĸ�����֣����ַ���'_'��ɡ�
UTILS_API nicBool  NICSYS_API  Utils_CheckNameAscii( nicChar* str );

UTILS_API nicBool NICSYS_API Utils_StrMatch(nicChar* source, nicChar* pattern);


#define NICSYS_KEY_STORED_WITH_CASE



#ifndef NICSYS_KEY_STORED_WITH_CASE
//�ַ�����Сд����
#define MakeUnifiedKey(k)\
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
#define MakeUnifiedKey(k) 0
#endif

template<typename T> 
int rtk_compare_key(const T & k1, const T & k2)
{
#ifdef NICSYS_KEY_STORED_WITH_CASE
	return strnicmp((const char *)&k1, (const char *)&k2, sizeof(T));
#else
	return memcmp((const char *)&k1, (const char *)&k2, sizeof(T));
#endif
}

//��һ����ַ�����ת��Ϊ�ṹ��
template <class __Type>
class  CAlphaName{
public:
	CAlphaName(const char * alpha)
	{
		memset(buf, 0, sizeof(buf));
		strncpy(buf, alpha, sizeof(buf) - 1);
#ifndef NICSYS_KEY_STORED_WITH_CASE
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



#endif
