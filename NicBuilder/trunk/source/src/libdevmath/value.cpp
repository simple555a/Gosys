// Valoare.cpp: implementation of the CValue class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4251)
#include "expression.h"
#include "value.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CValue::CValue()
{
	m_value = NULL;
}

CValue::~CValue()
{
	delete m_value;
}

void CValue::UpdateValue()
{
}

double CValue::GetValue()
{
	return 0;
}

void CValue::SetValue(void* value)
{
	m_value = value;
}