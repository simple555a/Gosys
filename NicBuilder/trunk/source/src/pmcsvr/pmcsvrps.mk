
pmcsvrps.dll: dlldata.obj pmcsvr_p.obj pmcsvr_i.obj
	link /dll /out:pmcsvrps.dll /def:pmcsvrps.def /entry:DllMain dlldata.obj pmcsvr_p.obj pmcsvr_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del pmcsvrps.dll
	@del pmcsvrps.lib
	@del pmcsvrps.exp
	@del dlldata.obj
	@del pmcsvr_p.obj
	@del pmcsvr_i.obj
