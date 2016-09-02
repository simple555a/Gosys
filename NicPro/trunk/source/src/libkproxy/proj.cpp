/*
	This module manages configurations project-wide.
	by J.Hunter, 2003/9/18
*/
#include "precomp.h"

#define EMPTY_STRING	" "

static string _find_proj(const char * url, bool bAllowCreate=false)
{
	string s;
	char path[F8_MAX_PATH];
	char *filePart;

	s = url;
	if(strlen(url) < 3){
		s = s + ".ce";
	}else{
		if(strcmpi(url + strlen(url) - 3, ".ce")){
			s = s + ".ce";
		}
	}

#ifdef _WIN32
	HANDLE h;
	WIN32_FIND_DATA fd;
	h = FindFirstFile(s.data(), &fd);
	if(INVALID_HANDLE_VALUE == h){
		if(!bAllowCreate){
			return EMPTY_STRING;
		}
		HANDLE hFile = CreateFile(
			s.data(), 
			GENERIC_WRITE,
			0, 
			0, 
			OPEN_ALWAYS, 
			FILE_ATTRIBUTE_TEMPORARY, 
			0);
		if(INVALID_HANDLE_VALUE == hFile){
			return EMPTY_STRING;
		}
		CloseHandle(hFile);
		h = FindFirstFile(s.data(), &fd);
		if(INVALID_HANDLE_VALUE == h){
			DeleteFile(s.data());
			return EMPTY_STRING;
		}
	}
	GetFullPathName(
		s.data(),
		sizeof(path),
		path,
		&filePart
		);
	s = path;
	FindClose(h);
#else
#endif

	return s;
}

static string _full_name(const char * s)
{
	char path[F8_MAX_PATH];

#ifdef _WIN32
	GetCurrentDirectory(sizeof(path), path);
#else
	strcpy(path, ".");
#endif
	merge_path(path, (char*)s);

	return path;
}

static string _get_path(const char * url)
{
	char * p;
	char fullPath[F8_MAX_PATH];
	string s;

	g_sProjectPath = url;

#ifdef _WIN32
	if(!GetFullPathName(url, sizeof(fullPath), fullPath, &p)){
		s = "";
		goto __failed;
	}
	*p = 0;
#else
	strncpy(fullPath, url, sizeof(fullPath));
	fullPath[sizeof(fullPath) - 1] = 0;
#endif		
	p = fullPath + strlen(fullPath) - 1;
	while(p != fullPath){
		if(strchr("/\\", *p)){
			*p = 0;
			break;
		}
		p--;
	}
	s = fullPath;
	// supress compiler warning in LINUX
	goto __failed;
__failed:
	return s;
}

static int _open_proj(const char * url)
{
	string st;
	IMemStream *ms;
	IF8Stream * s;
	struct kern_conf_cmd cmd;	
	struct blk_hdr_t *hdr;
	__bool resLoaded;

	DEBUG_PRINTF((
		"RootRef=%d,RootClsRef=%d\n", 
		query_resource(&sections->uuid),
		query_resource(&sections->h.uuid)
		));

	st = _find_proj(url);
	if(st == EMPTY_STRING){
		return F8_FILE_NOT_FOUND;
	}

	/* 
		load resource before hand, so that we can dump
		debug message in IKern layer
	*/
	st = st.substr(0, st.length() - 3);
	st = st + ".res";

	resLoaded = __false;
	ms = IMemStream_load_file(st.data());
	if(ms){
		s = __ucast__(IMemStream, IF8Stream, (ms));
		if(load_res_stream(s)){
			resLoaded = __true;
		}
		__delete__(ms);
	}else{
		utils_error(
			"Warning %d : resource not found.\n", 
			F8_RES_NOT_FOUND
			);		
	}

	DEBUG_PRINTF((
		"RootRef=%d,RootClsRef=%d\n", 
		query_resource(&sections->uuid),
		query_resource(&sections->h.uuid)
		));

	st = st.substr(0, st.length() - 4);
	st = st + ".ce";

	strncpy(fileName, st.data(), sizeof(fileName));
	fileName[sizeof(fileName) - 1] = 0;	

	ms = IMemStream_load_file(st.data());
	if(!ms){
		return F8_FILE_NOT_FOUND;
	}
	if(ms->length < sizeof(struct blk_hdr_t)){
		__delete__(ms);
		return F8_INVALID_DATA;
	}
	hdr = (struct blk_hdr_t*)ms->buffer;

	memset(&cmd, 0, sizeof(cmd));
	cmd.cmd = F8_PUT_PROG;
	NEW_ID(&cmd) = hdr->uuid;
	cmd.isize = ms->length;
	cmd.ibuffer = ms->buffer;
	cmd.obuffer = 0;
	cmd.osize = 0;

	__vcall__(proxy_adapter, exec_local, (proxy_adapter, &cmd));
	
	__delete__(ms);

	if(cmd.retcode != F8_SUCCESS){
		return cmd.retcode;
	}	
	
	{
		g_sProjectPath = _get_path(url);
		set_blklib_project_path(g_sProjectPath.data());
	}

	/* 
		we must probe the resources for ourselves, because
		we bypassed the ``blklib_load_by_file'' interface, which
		will handle the resource loading automatically.
	*/
	probe_blk_resources(sections);
	
	DEBUG_PRINTF((
		"RootRef=%d,RootClsRef=%d\n", 
		query_resource(&sections->uuid),
		query_resource(&sections->h.uuid)
		));

	load_config();
	reset_reg_map();
	load_variables();
	build_reg_map();
	
	return F8_SUCCESS;
}

static void _close_proj()
{	
	on_offline(0, 0);
	
	__vcall__(proxy_adapter, exec0, (proxy_adapter, F8_RESET, 0));	

	assert(sections->uuid == BLK_ID_ROOT);
	fileName[0] = 0;
	
	// generate a new uuid
	create_f8_uuid(&sections->h.uuid);

	reset_reg_map();
	unload_user_blklib();
	unload_variables();

#if !defined(NDEBUG) && 0
	// detect resource leakage
	kproxy_shell("vres");
#endif

	reset_resources();
}

/*
	int on_open(char * v, void *k)
	Purpose : open a project file.
	This function will load the following:
	1) project binary, i.e., compiled CE stream;
	2) project resource;
*/
int on_open(char * v, void *k)
{
	CArgsEx a;
	int ret;

	_close_proj();

	a.parse(v, FS);

	if(a.argc != 2){
		utils_error("usage : open <filename>\n");
		return F8_SYNTAX;
	}

	ret = _open_proj(a[1]);
	
	if(ret != F8_SUCCESS){
		_close_proj();
		return ret;
	}	

	return ret;
}

int on_create(char *v, void *k)
{	
	_close_proj();
	create_resource(&sections->h.uuid);
	create_resource(&sections->uuid);	
	return F8_SUCCESS;
}

int on_close(char *v, void *k)
{
	_close_proj();
	return F8_SUCCESS;
}

static f8_status _download_config()
{
	_shproc_prolog_no_k_();
	/*
		system configuration download first.
	*/
	struct ctrller_config cfg;
	cmd.cmd = F8_GET_SYS_CONFIG;
	OLD_ID(&cmd) = BLK_ID_ANY;
	NEW_ID(&cmd) = BLK_ID_NONE;
	cmd.isize = 0;
	cmd.obuffer = (char *)&cfg;
	cmd.osize = sizeof(cfg);
	
	__vcall__(
		proxy_adapter, 
		exec_local, 
		(proxy_adapter, &cmd)
		);
	
	if(F8_FAILED(cmd.retcode)){
		return cmd.retcode;
	}
	
	cmd.cmd = F8_SET_SYS_CONFIG;
	OLD_ID(&cmd) = BLK_ID_ANY;
	NEW_ID(&cmd) = sections->h.uuid;
	cmd.ibuffer = (char *)&cfg;
	cmd.isize = sizeof(cfg);
	cmd.osize = 0;
	
	__vcall__(
		proxy_adapter, 
		exec_remote, 
		(proxy_adapter, &cmd, 5000)
		);
	
	return cmd.retcode;
}

static f8_status _download_user_program()
{
	_shproc_prolog_no_k_();
	/*
		download user program
	*/
	__vcall__(sections, save, (sections, s));

	cmd.cmd = F8_PUT_PROG;
	OLD_ID(&cmd) = BLK_ID_ANY;
	NEW_ID(&cmd) = sections->h.uuid;
	cmd.isize = ms.pointer;
	cmd.osize = 0;

	__vcall__(
		proxy_adapter, 
		exec_remote, 
		(proxy_adapter, &cmd, 5000)
		);

	return cmd.retcode;
}

/*
	put local configuration/program into target
*/
int on_dload(char *v, void *k)
{
	_shproc_prolog_();

	if(!(proxy_adapter->flags & ADAPTER_F_ONLINE)){
		return F8_ADAPTER_OFFLINE;
	}

	if(F8_FAILED(cmd.retcode = check_program())){
		return cmd.retcode;
	}

	struct kstatus_t st;
	cmd.cmd = F8_GET_STATUS;
	cmd.isize = 0;
	cmd.obuffer = (char *)&st;
	cmd.osize = sizeof(st);
	__vcall__(proxy_adapter, exec_remote, (proxy_adapter, &cmd, psp->timeout));
	if(F8_FAILED(cmd.retcode)){
		return cmd.retcode;
	}

	stop_ctrller();
	
	/*
		ctrller must be reset to go further
	*/
	cmd.cmd = F8_RESET;
	cmd.isize = cmd.osize = 0;
	__vcall__(
		proxy_adapter, 
		exec_remote,
		(proxy_adapter, &cmd, 2000)
		);
	if(!successful){
		return cmd.retcode;
	}

	cmd.retcode = _download_config();
	if(!successful){
		return cmd.retcode;
	}
	
	cmd.retcode = _download_user_program();
	if(!successful){
		return cmd.retcode;
	}

	cmd.retcode = download_var_table();
	if(!successful){
		return cmd.retcode;
	}

	if(st.state==KERN_S_RUNNING || st.state==KERN_S_STANDBY){
		start_ctrller();
	}
	
	return F8_SUCCESS;
}

/*
	how much information is stored in the controller is not
	yet determined, surely the controller must know enough
	about the function blocks and system configuration to 
	run the control logic and do device io, but that's not 
	sufficient to reconstruct a graphical representation of
	the control program. so as for now, this interface is only
	for command-line test purpose.
*/
int on_uload(char *v, void *k)
{
	_shproc_prolog_();

	if(!(proxy_adapter->flags & ADAPTER_F_ONLINE)){
		return F8_ADAPTER_OFFLINE;
	}

	/*
		load memory configuration from remote 
	*/
	struct ctrller_config cfg;
	cmd.cmd = F8_GET_SYS_CONFIG;
	cmd.osize = sizeof(struct ctrller_config);
	cmd.obuffer = (char *)&cfg;
	__vcall__(
		proxy_adapter,
		exec_remote,
		(proxy_adapter, &cmd, 5000)
		);
	if(!successful){
		return cmd.retcode;
	}

	unload_variables();
	
	cmd.cmd = F8_SET_SYS_CONFIG;
	OLD_ID(&cmd) = BLK_ID_ANY;
	cmd.isize = sizeof(cfg);
	cmd.ibuffer = (char*)&cfg;
	cmd.osize = 0;
	__vcall__(
		proxy_adapter, 
		exec_local, 
		(proxy_adapter, &cmd)
		);

	reset_reg_map();
	
	cmd.cmd = F8_GET_PROG;
	cmd.isize = 0;
	cmd.osize = x_buf_len;
	cmd.obuffer = x_buffer;
	__vcall__(
		proxy_adapter, 
		exec_remote, 
		(proxy_adapter, &cmd, 5000)
		);
	if(!successful){
		return cmd.retcode;
	}

#if 0
	{
		FILE * fp;
		fp = fopen("ttt.ce", "wb");
		if(fp){
			fwrite(cmd.obuffer, 1, cmd.osize, fp);
			fclose(fp);
		}
	}
#endif
	
	cmd.cmd = F8_PUT_PROG;
	OLD_ID(&cmd) = BLK_ID_ANY;
	cmd.isize = cmd.osize;
	cmd.ibuffer = cmd.obuffer;
	cmd.osize = 0;
	cmd.obuffer = 0;
	__vcall__(
		proxy_adapter, 
		exec_local, 
		(proxy_adapter, &cmd)
		);

	probe_blk_resources(sections);
	upload_var_table();
	build_reg_map();

	return cmd.retcode;
}

static int _save_proj(const string & url, int bAutosave=false)
{
	IFileStream *fs;
	string s = url;

	fs = IFileStream_from_file(s.data(), "wb");
	if(!fs){
		return F8_LOW_MEMORY;
	}

	__vcall__(
		sections, 
		save, 
		(sections, __ucast__(IFileStream, IF8Stream, fs))
		);
	__delete__(fs);

	save_variables();
	save_config();
	
	/*
		save volatile resources
	*/
	s = s.substr(0, s.length() - 3);
	s = s + ".res";
	fs = IFileStream_from_file(s.data(), "wb");
	if(!fs){
		return F8_RES_NOT_FOUND;
	}
	save_volatile_res(__ucast__(IFileStream, IF8Stream, fs));
	__delete__(fs);

	if(!bAutosave){
		strncpy(fileName, url.data(), sizeof(fileName));
		fileName[sizeof(fileName) - 1] = 0;
		s = _get_path(fileName);
		// if(s != g_sProjectPath)
		{
			g_sProjectPath = s;
			set_blklib_project_path(s.data());
			reload_user_blklib();
		}
	}
	
	return F8_SUCCESS;
}

int on_save(char * v, void *k)
{
	CArgsEx a;
	string	s;

	a.parse(v, FS);

	if(a.argc < 2){
		if(!fileName[0]){
			// cannot save while untitled
			return F8_ACCESS;
		}
		s = fileName;
	}else{
		s = _find_proj(a[1], true);
		if(!s.data()[0]){
			return F8_FILE_NOT_FOUND;
		}
	}

	_save_proj(s);

	return F8_SUCCESS;
}

int autoSave(char *, void *k)
{
	string s;
	string::size_type n;
	if(!*fileName)
		return F8_ACCESS;
	s=fileName;
	if(s.find("_bak.ce") !=string::npos)
		return F8_ACCESS;
	n=s.find_last_of(".");
	if(n!=string::npos)
		s=s.substr(0,n);
	s+="_bak.ce";
	return _save_proj(s,true);
}

