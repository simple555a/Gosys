/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\librtk\object.cpp
	file path:	E:\vss\pmc2\src\librtk
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Object service support	
	history:
	created:	10:9:2002   9:36
*********************************************************************/
#include "precomp.h"

CRTKObject::CRTKObject()
{
	m_iTag  = 0;
	m_iObjectType = (('O' << 24) | ('K' << 16) | ('T' << 8) | 'R');
	CreateObject(this);
}

CRTKObject::~CRTKObject()
{
	DestroyObject(this);
}

void CRTKObject::CreateObject(CRTKObject * obj)
{
}

void CRTKObject::DestroyObject(CRTKObject * obj)
{
}

LIBRTK_API void RTK_API close_handle(RTK_HANDLE handle)
{
	CRTKObject * obj;
	obj = (CRTKObject*)handle;
	if(handle){
		delete obj;
	}
}

