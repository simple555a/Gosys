#include "precomp.h"

class strcmp_nocase{
public:
	bool operator()(const string & a, const string & b) const
	{
		return strcmpi(a.data(), b.data())<0? true : false;
	}
};

/* internal representation of blk library entry */
struct blk_lib_ent{
	f8_u16	magic;
	string	name;
	string	rawFileName;
	string	fileName;
	f8_uuid	id;
	f8_uint	resOffset;
	struct blk_lib_dir * dir;
};

typedef map<string, blk_lib_ent, strcmp_nocase> lib_ent_list;
typedef map<f8_uuid, blk_lib_ent*> uuid_index_map;

/* internal representation of blk library directory */
struct blk_lib_dir{
	string	name; /* F8 name */ 
	string	dirName; /* file system directory name */
	lib_ent_list elist; /* contained block items */
};

typedef map<string, blk_lib_dir, strcmp_nocase> blk_lib;

static char  g_blkpaths[4096];
static blk_lib g_blklib;
static uuid_index_map uindices;
static CArgs g_blkpath_args;
static string g_sProjectPath;

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
		p++;
	}
	s = fullPath;
	// supress compiler warning in LINUX
	goto __failed;
__failed:
	return s;
}

static string blk_file_by_type(const char * type)
{
	CArgs a;
	blk_lib::iterator it;
	lib_ent_list::iterator lit;

	parse_arg_ex(type, &a, ".");
	
	if(a.argc == 2){
		it = g_blklib.find(a.argv[0]);
		if(it == g_blklib.end()){
			return "";
		}
		lit = it->second.elist.find(a.argv[1]);
		if(lit == it->second.elist.end()){
			return "";
		}
		return lit->second.fileName;
	}

	// only type name is specified, match it in all libraries
	it = g_blklib.begin();
	while(it != g_blklib.end()){
		lit = it->second.elist.find(a.argv[0]);
		if(lit != it->second.elist.end()){
			return lit->second.fileName;
		}
		it++;
	}

	// cannot find a match in all libraries
	return "";
}

static void _load_directory(string s)
{
	blk_lib_dir	bl, *bbl;
	blk_lib_ent bent;
	blk_lib::iterator it;
	pair<lib_ent_list::iterator,bool> ppi;
	pair<blk_lib::iterator,bool> pi;
	int		i;
	int		count;
	char	lbuf[1024];
	CArgs	aa;
	string	path;
	bool	bIsAbsolutePath;
	
#ifdef _WIN32
	ExpandEnvironmentStrings(s.data(), lbuf, sizeof(lbuf));
	s = lbuf;
#endif

#ifdef _WIN32
	if(s[0] == '.'){
		bIsAbsolutePath = false;
	}else{
		if(s.length() < 2){
			return;
		}
		bIsAbsolutePath = s[1] == ':'? true : false;
	}
#else
	if(s[0] == '/'){
		bIsAbsolutePath = true;
	}
#endif

	if(!bIsAbsolutePath){
		s = string(get_working_dir()) + s;
	}

	if(strchr("/\\", s[s.length() - 1])){
		path = s;
		s = s + "f8.dir";		
	}else{
		path = s + "/";
		s = s + "/f8.dir";
	}
	count = GetPrivateProfileInt("library", "count", 0, s.data());
	if(!count){
		// return;
	}
	GetPrivateProfileString(
		"library", 
		"catagory",
		"base",
		lbuf,
		sizeof(lbuf),
		s.data()
		);
	it = g_blklib.find(lbuf);
	if(it == g_blklib.end()){
		bl.name = lbuf;
		bl.dirName = path;
		pi = g_blklib.insert(blk_lib::value_type(lbuf, bl));
		if(!pi.second){
			utils_error(
				"Error loading %s, insufficient memory.\n", 
				lbuf
				);
			return;
		}
		bbl = &pi.first->second;
	}else{
		bbl = &it->second;
	}
	for(i=0; i<count; i++){
		char key[16];
		sprintf(key, "%d", i);
		GetPrivateProfileString(
			"library", 
			key,
			"",
			lbuf,
			sizeof(lbuf),
			s.data()
			);
		parse_arg_ex(lbuf, &aa, ",");
		if(aa.argc < 5){
			continue;
		}		
		
		/*
		key=<type>,<magic>,<file>,<uuid>,<res_offset>
		*/
		bent.name = aa.argv[0];
		bent.magic = (aa.argv[1][0] << 8) + aa.argv[1][1];
		bent.fileName = path + aa.argv[2];
		bent.rawFileName = aa.argv[2];
		bent.dir = bbl;
		f8_uuid_from_string(&bent.id, aa.argv[3]);
		bent.resOffset = atoi(aa.argv[4]);

		// utils_trace("Loading function block %s\n", bent.rawFileName.data());

		ppi = bbl->elist.insert(
			lib_ent_list::value_type(bent.name, bent)
			);
		if(ppi.second){
			uindices.insert(
				uuid_index_map::value_type(bent.id, &(ppi.first->second))
				);
		}		
	}

#if 0
	utils_debug(
		"%d blocks in directory %s loaded.\n", 
		count, 
		s.data()
		);
#endif
}

static void _save_directory(string s)
{
	/*
	// purge user blocks entries
	blk_lib::iterator blit;
	lib_ent_list::iterator leit;
	char buf[1024];
	char * p;
	int i;

	blit = g_blklib.find(s);
	if(blit == g_blklib.end()){
		return;
	}

	leit = blit->second.elist.begin();
	i = 0;
	while(leit != blit->second.elist.end()){
		p = buf;
		p += sprintf(p, leit->second.)
		i++;
		leit++;
	}
	*/
}

/*
	alpha/digit/underscore and DBCSs are allowed for blk names
*/
FBDAR_API __bool is_valid_blk_name(const char * name)
{
	if(!*name){
		return false;
	}
	while(*name){
		// letters/numbers/_ and DBCS are valid
		if(!isalpha(*name) && !isdigit(*name) && !strchr("_-()#!~%&*)",*name) &&!(*name & 0x80)){
			return false;
		}
		name++;
	}
	return true;
}

FBDAR_API __bool is_valid_pin_name(const char * name)
{
	if(!*name){
		return false;
	}
	while(*name){
		// letters/numbers/_ and DBCS are valid
		if(!isalpha(*name) && !isdigit(*name) && !strchr("_-()#!~%&*)",*name) &&!(*name & 0x80)){
			return false;
		}
		name++;
	}
	return true;
}

FBDAR_API f8_status blklib_load_by_type(const char *type, IBlk **blk)
{
	string s;
	s = blk_file_by_type(type);
	if(s == ""){
		*blk = 0;
		return F8_PATH_NOT_FOUND;
	}
	return blklib_load_by_file(s.data(), blk);
}

FBDAR_API f8_status blklib_load_by_id(const char *id, IBlk **blk)
{
	f8_uuid bid;
	struct blk_ent_t ent;
	f8_uuid_from_string(&bid, id);
	if(!blklib_query(&bid, &ent)){
		return F8_OBJECT_NOT_FOUND;
	}
	return blklib_load_by_file(ent.fileName, blk);
}

FBDAR_API f8_status blklib_load_by_file(const char * file, IBlk **b)
{
	IFileStream * fs;
	IF8Stream	* s;
	IBlk * blk;
	__u16	magic;

	*b = 0;
	fs = IFileStream_from_file(file, "rb");
	if( !fs ){
		// fprintf(stderr, "Cannot open %s for reading.\n", file);
		return F8_FILE_NOT_FOUND;
	}
	s = __ucast__(IFileStream, IF8Stream, fs);
	blk = IBlk_from_stream(s);

	if(blk){
		if(__vcall__(s, get, (s, &magic, sizeof(magic)))){
			__vcall__(s, seek, (s, -2, SEEK_CUR));
			if(magic == F8_RES_MAGIC || magic == F8_RES_MAGIC_2){
				load_res_stream(s);
			}
		}
		probe_blk_resources(blk);
	}

	__delete__(fs);

	*b = blk;
	return F8_SUCCESS;
}

/*
	export a block to the user block library
*/
FBDAR_API int blklib_save(const char *type, IBlk *blk, int reserved)
{
	string s;
	__bool rt;

	mkdir((g_sProjectPath + "/user").data());

	s = g_sProjectPath + "/user/" + type + ".blk";
	rt = blk_to_file(blk, s.data());
	if(!rt){
		return F8_INVALID_DATA;
	}
	
	s = get_working_dir();
	s = s + "mkf8dir -c:user -d:" + g_sProjectPath + "/user";
#ifdef _WIN32
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		utils_debug("Launching `%s'\n", s.data());
		if( !CreateProcess(
			NULL,
			(char*)s.data(),
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi
			)
			)
		{
			return F8_X_PROG_NOT_FOUND;
		}
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		reload_user_blklib();
	}
#endif
	return rt ? F8_SUCCESS : F8_FILE_NOT_FOUND;
}

static int UTILS_API _efn(
	const char * szBaseDir,
	const char * szFullPath,
	const char * szRelativePath,
	const void * pvSearchContext
	)
{
	string s;
	char *p1, *p2;
	char sz[128];

	memcpy(sz, szFullPath, strlen(szFullPath));
	sz[strlen(szFullPath)+1] = '\0';

	s = sz;
	p1 = strrchr(sz,  '/');
#ifdef _WIN32
	p2 = strrchr(sz, '\\');
#else
	p2 = p1;
#endif
	if(p2 > p1){
		p1 = p2;
	}
	s = s.substr(0, p1 - sz);
	_load_directory(s);
	return __true;
}

static void reload_blklibs()
{
	string s;
	
#if 0
	g_blklib.clear();
	uindices.clear();

	for(i=0; i<g_blkpath_args.argc; i++){
		s = g_blkpath_args.argv[i];
		_load_directory(s);
	}
#else
	s = get_exe_dir();
	s = s + "F8/X86/TARGET";
	enumerate_files(
		s.data(),
		"f8.dir",
		_efn,
		SEARCH_ALL_FILE,
		0);
#endif
}

FBDAR_API void set_blklib_paths(const char * paths)
{
	strncpy(g_blkpaths, paths, sizeof(g_blkpaths));
	g_blkpaths[ sizeof(g_blkpaths) - 1] = 0;
	parse_arg_ex(g_blkpaths, &g_blkpath_args, PATH_SEPRATOR);
	WritePrivateProfileString(
		"f8", 
		"libpath", 
		g_blkpaths, 
		get_config_file()
		);
}

FBDAR_API const char * get_blklib_paths()
{
	return g_blkpaths;
}

FBDAR_API void enumerate_blk_dir(
	ENUM_BLKDIR_PROC proc, 
	__uint context
	)
{
	blk_lib::iterator it;
	struct	blk_dir_t bd;

	if(!proc){
		return;
	}
	it = g_blklib.begin();
	while(it != g_blklib.end()){
		bd.name = (char*)it->second.name.data();
		if(!proc(&bd, context)){
			return;
		}
		it++;
	}
}

FBDAR_API void enumerate_blk_ent(
	ENUM_BLKENT_PROC proc, 
	const char * dirName,
	__uint context
	)
{
	blk_lib::iterator	dit;
	lib_ent_list::iterator it;
	blk_ent_t	ble;

	if(!proc){
		return;
	}
	dit = g_blklib.find(dirName);
	if(dit == g_blklib.end()){
		return;
	}
	it = dit->second.elist.begin();
	while(it != dit->second.elist.end()){
		ble.name = (char*)it->second.name.data();
		ble.fileName = (char*)it->second.fileName.data();
		ble.magic = it->second.magic;
		if(!proc(&ble, context)){
			return;
		}
		it++;
	}
}

FBDAR_API __bool  blklib_query(const f8_uuid * id, struct blk_ent_t * ent)
{
	blk_lib_ent * e;
	uuid_index_map::iterator it;
	it = uindices.find(*id);
	if(it == uindices.end()){
		return __false;
	}
	e = it->second;
	ent->dir = e->dir->name.c_str();
	ent->fileName = e->fileName.c_str();
	ent->magic = e->magic;
	ent->name = e->name.c_str();
	ent->pad = 0;
	return __true;
}

/*
	load the resource from block file and increment the usage count
	of the resource.
*/
FBDAR_API __bool blklib_load_res(const f8_uuid * id)
{
	blk_lib_ent * ent;
	uuid_index_map::iterator it;
	__bool ret;

	if(query_resource(id) >= 0){
		load_resource(id);
		return __true;
	}
	it = uindices.find(*id);
	if(it == uindices.end()){
		return __false;
	}

	IFileStream * fs;
	IF8Stream *s;
	ent = it->second;
	fs = 	IFileStream_from_file(ent->fileName.data(), "rb");
	if(!fs){
		return __false;
	}
	s = __ucast__(IFileStream, IF8Stream, (fs));
	__vcall__(s, seek, (s, ent->resOffset, SEEK_SET));
	ret = load_res_stream(s);
	if(!ret){
		// we don't have appropriate handler for such errors
		/*
			2005-8-11, this must be handled in case
			user forget to invoke mkf8dir after modifying
			block sources.

			for now we ignore the error and continue.
		*/
		// assert(0);
		char buf[128];
		f8_uuid_to_string(id, buf, sizeof(buf));
		utils_error(
			"file %s, {%s} - resource not loaded."
			"invoke mkf8dir on the directory to correct.\n",
			ent->fileName.c_str(),
			buf
			);
	}
	
	// initialize reference count to 1
	load_resource(id);

	__delete__(fs);
	return __true;
}

FBDAR_API void unload_user_blklib()
{
	// purge user blocks entries
	blk_lib::iterator blit;
	lib_ent_list::iterator leit;

	blit = g_blklib.find("user");
	if(!(blit == g_blklib.end())){
		leit = blit->second.elist.begin();
		while(leit != blit->second.elist.end()){
			uindices.erase(leit->second.id);
			leit++;
		}
		blit->second.elist.clear();
	}
}

FBDAR_API void reload_user_blklib()
{
	string s;
	// purge user blocks entries
	unload_user_blklib();
	s = g_sProjectPath + "/user";
	_load_directory(s);
}

FBDAR_API void init_blklib()
{
	char oldRtkf8[F8_MAX_PATH];
	string s;

	static bool inited = false;
	if(inited){
		return;
	}

	inited = true;
	
#ifdef _WIN32
	s = get_working_dir();
	if(strchr("/\\", s[s.length() - 1])){
		s[s.length() - 1] = 0;
	}
	if(!GetEnvironmentVariable("PMCF8", oldRtkf8, sizeof(oldRtkf8))){
		SetEnvironmentVariable("PMCF8", s.data());
	}	
#endif
	
	GetPrivateProfileString(
		"f8", 
		"libpath",
#ifdef _WIN32
		"%PMCF8%/f8lib/1499;%PMCF8%/f8lib/1131;%PMCF8%/f8lib/base",
#else
		"$PMCF8/f8lib/1499:$PMCF8/f8lib/1131:$PMCF8/f8lib/base",
#endif
		g_blkpaths,
		sizeof(g_blkpaths),
		get_config_file()
		);
	parse_arg_ex(g_blkpaths, &g_blkpath_args, PATH_SEPRATOR);	
	reload_blklibs();

	blklib_load_devices();
}

FBDAR_API void uninit_blklib()
{
	blklib_unload_devices();
}

__inline void _ENSURE_RES_EXISTS(f8_uuid * r)
{
	__int rcount;
	rcount = query_resource(r);
	if(rcount < 0){
		create_resource(r);
	}else{
		load_resource(r);
	}
}

/*
	this proc has two duties:
	1) load the instance resources, create them if necessary
	2) increment usage count of the class resources
*/
static __bool _ld_res_proc(ITreeNode * nd, __bool firstVisit, __uint root_blk)
{
	IBlk * blk;
	ICBlk * cblk;
	struct cblk_link_t * lk;
	struct blk_pin_t  * p;
	struct cblk_pin_t * cp;
	
	blk = __dcast__(ITreeNode, IBlk, nd);

	_ENSURE_RES_EXISTS(&blk->uuid);
	
	blklib_load_res(&blk->h.uuid);
	
	if(blk->h.magic == CBLK_MAGIC){
		cblk = __dcast__(IBlk, ICBlk, (blk));
		
		lk = ICBlk_first_link(cblk);
		while(lk){
			_ENSURE_RES_EXISTS(&lk->uuid);
			lk = ICBlk_next_link(lk);
		}

		p = __vcall__(blk, first_pin, (blk));
		while(p){
			cp = SAFE_CONTAINING_RECORD(p, struct cblk_pin_t, p);
			_ENSURE_RES_EXISTS(&cp->uuid);
			p = __vcall__(blk, next_pin, (p));
		}
	}
	
	return __true;
}

/*
	this proc fixups resource usage count for block hive `b'.
*/
FBDAR_API void probe_blk_resources(IBlk * b)
{
	_traverse(b, _ld_res_proc, (__uint)b);
}

FBDAR_API __bool blk_to_file(IBlk * blk, const char * file)
{
	IFileStream * fs;
	IF8Stream	* s;
	__bool ret;
	
	fs = IFileStream_from_file(file, "wb");
	if( !fs ){
		// fprintf(stderr, "Cannot open %s for writing.\n", file);
		return 0;
	}
	s = __ucast__(IFileStream, IF8Stream, fs);
	ret = __vcall__(blk, save, (blk, s));
	if(query_resource(&blk->h.uuid) > 0){
		save_res_stream(&blk->h.uuid, s);
	}

	__delete__(fs);
	return ret;
}

FBDAR_API void set_blklib_project_path(const char * p)
{
	char end;
	g_sProjectPath = p;
	end=g_sProjectPath.at(g_sProjectPath.length()-1);
	if(end=='/' || end=='\\')
		g_sProjectPath=g_sProjectPath.substr(0,g_sProjectPath.length()-1);
	reload_user_blklib();
}

#ifdef _WIN32
BOOL WINAPI DllMain(HMODULE h,DWORD r,LPVOID)
{
	switch(r){
	case DLL_PROCESS_ATTACH:
		// init_blklib();
		DisableThreadLibraryCalls(h);
		break;
	case DLL_PROCESS_DETACH:
		// uninit_blklib();
		break;
	}
	return TRUE;
}
#endif
