/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\librtk\guid.cpp
	file path:	E:\vss\pmc2\src\librtk
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Guid support routines	
	history:
	created:	10:9:2002   11:41
Notes : 
	needs e2fsprogs package when compiled under linux, which can
	found at http://e2fsprogs.sourceforge.net
*********************************************************************/

#include "precomp.h"
#pragma hdrstop

void RTK_GUID::create()
{
#ifdef _WIN32
    UuidCreate((UUID*)this);
#elif defined(__LINUX__) && 0
	uuid_generate(data);
#else
	assert(0);
#endif
};

int RTK_GUID::to_string(char * buffer) const
{
	// uint32 * l=(uint32*)Data;
	unsigned * l=(unsigned*)data;
	int i;
	i=sprintf(buffer,"%08x-",*l);
	l++;
	//uint16 * s=(uint16*)l;
	unsigned short * s=(unsigned short*)l;
	i+=sprintf(buffer+i,"%04x-",*s);
	s++;
	i+=sprintf(buffer+i,"%04x-",*s);
	s++;
	//uchar * c=(uchar*)s;
	unsigned char *c=(unsigned char*)s;
	i+=sprintf(buffer+i,"%02x",*c);
	c++;
	for(int j=1;j<8;j++,c++){
	    i+=sprintf(buffer+i,"%02x",*c);
	}
	return i;
}
