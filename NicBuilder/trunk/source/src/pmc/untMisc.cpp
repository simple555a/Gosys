#include "precomp.h"
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

AnsiString ToUrl(PCTAG_NAME name)
{
    AnsiString Url;
    Url = "";
    Url += (char*)CNodeName(name->node);
    Url += ".DB.";
    Url += (char*)CGroupName(name->sname.group);
    Url += ".";
    Url += (char*)CTagName(name->sname.tag);
    return Url;
};

AnsiString ToUrl(PCNODE_KEY nd)
{
    AnsiString Url;
    Url = (char*)CNodeName(*nd);
    return Url;
}

AnsiString ToUrl(PCNODE_KEY nd, PCGROUP_KEY grp)
{
    AnsiString Url;
    Url = "";
    Url += (char*)CNodeName(*nd);
    Url += ".DB.";
    Url += (char*)CGroupName(*grp);
    return Url;
}

AnsiString ToUrl(PCNODE_KEY nd, PCDEVICE_KEY d)
{
    AnsiString Url;
    Url = "";
    Url += (char*)CNodeName(*nd);
    Url += ".DEVICES.";
    Url += (char*)CDeviceName(*d);
    return Url;
}

bool is_valid_name(const char * p)
{
    if(! *p){
        return false;
    }
    while(*p){
        if(!isalpha(*p) && !isdigit(*p) && !strchr("_-$@", *p) && (*p)>0){
            return false;
        }
        p++;
    }
    return true;
}

