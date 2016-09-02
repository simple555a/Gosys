//---------------------------------------------------------------------------

#ifndef untMiscH
#define untMiscH

#include <system.hpp>
#include <pmc.h>

//---------------------------------------------------------------------------
bool is_valid_name(const char * p);
extern AnsiString ToUrl(PCTAG_NAME TagName);
extern AnsiString ToUrl(PCNODE_KEY nd);
extern AnsiString ToUrl(PCNODE_KEY, PCGROUP_KEY);
extern AnsiString ToUrl(PCNODE_KEY, PCDEVICE_KEY);

//---------------------------------------------------------------------------

#endif
