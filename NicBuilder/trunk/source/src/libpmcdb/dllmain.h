// dllmain.h : 模块类的声明。

class ClibpmcdbModule : public ATL::CAtlDllModuleT< ClibpmcdbModule >
{
public :
	DECLARE_LIBID(LIBID_libpmcdbLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LIBPMCDB, "{D3A52179-C160-451A-88C4-55AE19D0269F}")
};

extern class ClibpmcdbModule _AtlModule;
