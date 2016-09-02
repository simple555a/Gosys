
libhisdATLps.dll: dlldata.obj libhisdATL_p.obj libhisdATL_i.obj
	link /dll /out:libhisdATLps.dll /def:libhisdATLps.def /entry:DllMain dlldata.obj libhisdATL_p.obj libhisdATL_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del libhisdATLps.dll
	@del libhisdATLps.lib
	@del libhisdATLps.exp
	@del dlldata.obj
	@del libhisdATL_p.obj
	@del libhisdATL_i.obj
