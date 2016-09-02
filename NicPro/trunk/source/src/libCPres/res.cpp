/*
	Resource manager
	by Jackie, 2003/9/9
*/
#include "precomp.h"

struct F8_RESITEM{
	void	*buffer;
	int		length;
};

typedef map<string, F8_RESITEM> ITEMMAP;

struct F8_RESOURCE{
	f8_uuid			id;
	int				refcount;
	ITEMMAP			*pItems;
	_DEL_RES_PROC	delProc;
};

typedef map<f8_uuid, F8_RESOURCE> RESMAP;

RESMAP g_Resources;

static void _free_items(ITEMMAP * imap)
{
	ITEMMAP::iterator it;

	it = imap->begin();
	while(it != imap->end()){
		if(it->second.buffer){
			__free__(it->second.buffer);
		}
		it->second.buffer = 0;
		it++;
	}
}

/*
	release memory consumed by the contents
	of a resource.
	NOTE : the memory of the resource entry itself
	is not released(that is released by one or more calls
	to unload_resource).
*/
static __bool _free_resource(
	F8_RESOURCE * res
	)
{		
	ITEMMAP	* imap = res->pItems;

	if(!imap){
		return __true;
	}

	_free_items(imap);

	delete res->pItems;
	res->pItems = 0;

	return __true;
}

static F8_RESOURCE * _get_res(const f8_uuid * id)
{
	RESMAP::iterator rit;

	rit = g_Resources.find(*id);
	if(rit == g_Resources.end()){
		return 0;
	}

	return &rit->second;
}

static F8_RESITEM * _get_item(
	F8_RESOURCE * res,
	const char * key
	)
{
	ITEMMAP::iterator  it;
	ITEMMAP * imap = res->pItems;
	it = imap->find(key);
	if(it == imap->end()){
		return 0;
	}
	return &it->second;
}

F8_RESITEM * _new_item(F8_RESOURCE * res, const char * k)
{
	struct F8_RESITEM item;
	pair<ITEMMAP::iterator, bool> pib;

	memset(&item, 0, sizeof(item));
	pib = res->pItems->insert(ITEMMAP::value_type(k, item));
	if(pib.second){
		return &pib.first->second;
	}
	return 0;
}

static F8_RESOURCE * create_resource_bibibobo(
	const f8_uuid * id,
	__bool bIncrementUsageCount
	)
{
	F8_RESOURCE foo, *r;
	pair<RESMAP::iterator,bool> pr;

	if(r = _get_res(id)){
		if(bIncrementUsageCount){
			r->refcount++;
		}		
		return r;
	}
	if(bIncrementUsageCount){
		foo.refcount = 1;
	}else{
		foo.refcount = 0;
	}
	foo.id = *id;
	foo.pItems = new ITEMMAP;
	foo.delProc = 0;
	if(!foo.pItems){
		return 0;
	}
	pr = g_Resources.insert(RESMAP::value_type(*id, foo));
	if(!pr.second){
		delete foo.pItems;
		return 0;
	}
	r = &pr.first->second;
	
	return r;
}

/*
	create a resouce entry with a reference count
	preset to 1(or increment it if already exists)
*/
F8RES_API __bool create_resource(
	const f8_uuid *id
	)
{
	return create_resource_bibibobo(id, __true)? __true : __false;
}

/*
	construct a resource object in memory from a stream
	the stream is composed of:
	1) a magic
	2) uuid
	3) item count
	4) array of items

	NOTE!! 
	1) On success, the resource item is not reference counted. 
	If the client won't addref on the loaded resource, then the 
	item is exposed to the garbage collector.
	More exactly, this proc won't addref on the resource when
	the resource already exists.
*/
static F8_RESOURCE * _load_res_stream(IF8Stream * str)
{
	__u16	magic;
	RESMAP::iterator it;
	__u32 itemcount;
	c_bstr_t cbstr;
	f8_uuid id;
	__int	length;
	void	* buffer;
	F8_RESOURCE * res;
	__bool ret;

	// DEBUG_PRINTF(("stream %08x\n", __vcall__(str, tell, (str))));
	
	if(!__vcall__(str, get, (str, &magic, sizeof(magic)))){
		return 0;
	}

	if(magic != F8_RES_MAGIC && magic != F8_RES_MAGIC_2){
		__vcall__(str, seek, (str, 0-sizeof(magic), SEEK_CUR));
		return 0;
	}

	if(!__vcall__(str, get, (str, &id, sizeof(id)))){
		return 0;
	}

	res = create_resource_bibibobo(&id, __false);
	if(!res){
		return 0;
	}
	
	/* the resource has not been loaded yet */
	if(!__vcall__(str, get, (str, &itemcount, sizeof(itemcount)))){
		return 0;
	}

	if(res->refcount){
		/* note if the resource is already present we silently
		eat the stream without modifing the in-memory version
		*/
		for(; itemcount; itemcount--){
			if(!__vcall__(str, get, (str, &cbstr.count, sizeof(cbstr.count)))){
				goto __failed;
			}
			__vcall__(str, seek, (str, cbstr.count, SEEK_CUR));
			if(!__vcall__(str, get, (str, &length, sizeof(length)))){
				goto __failed;
			}
			__vcall__(str, seek, (str, length, SEEK_CUR));
		}	
	}else{
		for(; itemcount; itemcount--){	
			if(!bstr_from_stream(&cbstr, str)){
				goto __failed;
			}

			if(!__vcall__(str, get, (str, &length, sizeof(length)))){
				goto __failed;
			}
			if(length){
				buffer = __malloc__(length);
				if(!buffer){
					goto __failed;
				}
				if(!__vcall__(str, get, (str, buffer, length))){
					goto __failed;
				}
			}else{
				buffer = 0;
			}
			
			ret = set_res_buf(&id, buffer, length, cbstr.buffer);
			__free__(buffer);
			c_free_bstr(&cbstr);
			if(!ret){
				goto __failed;
			}
		}
	}
	return res;	

__failed:
	/* the unload_resource will decrease the refcount */
	res->refcount++;
	unload_resource(&id);
	
	return 0;
}

/*
	import a resource from disk, and if the resource has
	already been loaded, increment its reference count.

	2005/6/24
	the load_from_disk branch is obsolete.
*/
F8RES_API __bool load_resource(
	const f8_uuid * id
	)
{
	RESMAP::iterator it;
	it = g_Resources.find(*id);
	if(it != g_Resources.end()){
		it->second.refcount++;
		return __true;
	}
	return __false;
#if 0
	IFileStream *fs;
	IF8Stream * str;
	char buf[43];
	
	f8_uuid_to_string(id, buf, sizeof(buf));
	strcat(buf, ".res");
	fs = IFileStream_from_file(buf, "rb");
	if(!fs){
		return __false;
	}
	str = __ucast__(IFileStream, IF8Stream, fs);
	h = _load_res_stream(str);
	if(h){
		if(*id != h->id){
			/* error in library */
			unload_resource(&h->id);
			h = 0;
		}		
	}

	__delete__(fs);
	return h? __true : __false;
#endif
}

/*
	decrement the reference count of a resource.
	remove the resource entry if the reference count
	reaches zero.
*/
F8RES_API __bool unload_resource(
	const f8_uuid * id
	)
{
	RESMAP::iterator it;
	F8_RESOURCE * res;

	it = g_Resources.find(*id);
	if(it == g_Resources.end()){
		return __false;
	}
	res = &it->second;
	assert(res->refcount > 0);
	res->refcount--;
	
	if(0 == res->refcount){
		/* garbage collection */
		if(res->delProc){
			res->delProc(id);
		}
		_free_resource(res);
		g_Resources.erase(it);
	}
	
	return __true;
}

/*
	load resources in the stream, note the reference
	count of the resources are not affected, or set
	to zero if not already present.
*/
F8RES_API __bool load_res_stream(
	IF8Stream * s
	)
{
	F8_RESOURCE *h;
	
	do{
		h = _load_res_stream(s);
	}while(h);
	
	/* we should have consumed all the stream contents */
	if(!__vcall__(s, eos, (s))){
		return __false;
	}
	
	return __true;
}

/*
	load resource from stream, but in another name.
	again, the reference count is not affected.
*/
F8RES_API __bool load_res_stream_as(
	IF8Stream * s,
	const f8_uuid * newId
	)
{
	F8_RESOURCE *h;
	int ret;
	h = _load_res_stream(s);
	if(!h){
		return __false;
	}
	ret = copy_resource(&h->id, newId);
	if(F8_FAILED(ret)){
		return __false;
	}
	/*
		to be compatible with load_res_stream, we
		remove a reference count from the one incremented
		by copy_resource
	*/
	h = _get_res(newId);
	h->refcount--;
	return __true;
}

/*
	save resource to stream with garbage collection.
	the resources with reference count of zero are not saved.
*/
static __bool _save_res_stream(IF8Stream * str, F8_RESOURCE * res)
{
	__uint itemcount;
	void * itembuf = 0;
	c_bstr_t cbstr;
	ITEMMAP::iterator it;
	__u16 magic = F8_RES_MAGIC;
	ITEMMAP & imap = *res->pItems;

	if(!res->refcount){
		return __false;
	}

	itemcount = imap.size();
	// 2003-11-20, save non-null resources only
	if(itemcount){
		__vcall__(str, put, (str, &magic, sizeof(magic)));
		__vcall__(str, put, (str, (void*)&res->id, sizeof(f8_uuid)));
		__vcall__(str, put, (str, &itemcount, sizeof(itemcount)));

		it = imap.begin();
		while(it != imap.end()){
			cbstr.buffer = (char*)it->first.data();
			cbstr.count = it->first.length();
			bstr_to_stream(&cbstr, str);
			__vcall__(str, put, (str, &it->second.length, sizeof(__uint)));		
			__vcall__(str, put, (str, it->second.buffer, it->second.length));
			it++;
		}
	}

	return __true;
}

F8RES_API __bool save_res_stream(
	const f8_uuid * id,
	IF8Stream * s
	)
{
	F8_RESOURCE * res;

	if(id){
		res = _get_res(id);
		if(!res){
			return __false;
		}

		return _save_res_stream(s, res);
	}else{
		RESMAP::iterator it;
		it = g_Resources.begin();
		while(it != g_Resources.end()){
			_save_res_stream(s, &it->second);
			it++;
		}
		g_Resources.clear();
	}

	return __true;
}

static __bool _svr_proc(const f8_uuid * uuid, __int context)
{
	IF8Stream * s;

	s = (IF8Stream*)context;
	if(!get_res_int(uuid, "!readonly", 0)){
		save_res_stream(uuid, s);
	}
	return __true;
}

F8RES_API __bool save_volatile_res(
	IF8Stream *s
	)
{
	enum_resources(_svr_proc, (__int)s);
	return __true;
}

F8RES_API __bool set_res_buf_v(
	const f8_uuid * id,
	const void * buffer,
	int length,
	const char * keyFormat,
	...
	)
{
	char key[128];
	va_list args;

	va_start(args, keyFormat);
	_vsnprintf(key, sizeof(key), keyFormat, args);
	va_end(args);

	return set_res_buf(id, buffer, length, key);
}

/*
	associate a buffer with a guid and a key
	the old buffer will be freeed automatically.
*/
F8RES_API __bool set_res_buf_ex(
	const f8_uuid * id,
	const void * buffer,
	int length,
	const char * key,
	__bool bForce
	)
{
	F8_RESOURCE * res;
	F8_RESITEM * item;

	res = _get_res(id);
	if(!res && bForce){
		create_resource(id);		
		res = _get_res(id);
	}
	if(!res){
		assert(0);
		return __false;
	}
	item = _get_item(res, key);
	if(!item){
		item = _new_item(res, key);
	}

	if(!item){
		return __false;
	}
	
	if(length == -1){
		length = strlen((const char*)buffer) + 1;
	}

	if(item->buffer == buffer){
		item->length = length;
		return __true;
	}

	if(item->buffer){
		__free__(item->buffer);
	}

	if(length){
		item->buffer = __malloc__(length);
		item->length = length;
		if(!item->buffer){
			return __false;
		}
		memcpy(item->buffer, buffer, length);
	}else{
		item->buffer = 0;
		item->length = 0;
	}

	return __true;
}

F8RES_API __bool set_res_buf(
	const f8_uuid * id,
	const void * buffer,
	int length,
	const char * key
	)
{
	return set_res_buf_ex(id, buffer, length, key, __false);
}

F8RES_API __bool set_res_name(
	const f8_uuid * id,
	const char * name
	)
{
	return set_res_buf(id, name, strlen(name) + 1, "name");
}

F8RES_API const char * get_res_name(
	const f8_uuid * id
	)
{
	return get_res_buf(id, 0, "name");
}

F8RES_API char * get_res_buf_v(
	const f8_uuid * id,
	int *length,
	const char * keyFormat,
	...
	)
{
	char key[128];
	va_list args;

	va_start(args, keyFormat);
	_vsnprintf(key, sizeof(key), keyFormat, args);
	va_end(args);

	return get_res_buf(id, length, key);
}

F8RES_API char * get_res_buf(
	const f8_uuid * id,
	int *length,
	const char * key
	)
{
	F8_RESOURCE * res;
	F8_RESITEM * item;

	res = _get_res(id);
	if(!res){
		return __false;
	}
	item = _get_item(res, key);

	if(!item){
		if(length){
			*length = 0;
		}
		return __false;
	}
	if(length){
		*length = item->length;
	}
	return (char*)item->buffer;
}

F8RES_API __bool del_res_item(
	const f8_uuid * id, 
	const char * key
	)
{
	F8_RESOURCE * res;
	ITEMMAP::iterator it;

	res = _get_res(id);
	if(!res){
		return __false;
	}
	it = res->pItems->find(key);
	if(it == res->pItems->end()){
		return __false;
	}
	if(it->second.buffer){
		__free__(it->second.buffer);
	}
	res->pItems->erase(it);

	return __true;
}

F8RES_API __int get_res_int(
	const f8_uuid * id,
	const char * key,
	__int defvalue
	)
{
	char * buf;
	buf = get_res_buf(id, 0, key);
	if(!buf){
		return defvalue;
	}
	return atoi(buf);
}

F8RES_API __int get_res_int_v(
	const f8_uuid * id,
	const char * keyFormat,
	__int defvalue,
	...
	)
{
	char key[128];
	va_list args;

	va_start(args, keyFormat);
	_vsnprintf(key, sizeof(key), keyFormat, args);
	va_end(args);

	return get_res_int(id, key, defvalue);
}

F8RES_API __bool set_res_int(
	const f8_uuid * id,
	const char * key,
	__int val
	)
{
	char ibuf[32];
	sprintf(ibuf, "%d", val);
	return set_res_buf(id, ibuf, strlen(ibuf) + 1, key);
}

F8RES_API __bool set_res_int_v(
	const f8_uuid * id,
	const char * keyFormat,
	__int val,
	...
	)
{
	char key[128];
	va_list args;

	va_start(args, keyFormat);
	_vsnprintf(key, sizeof(key), keyFormat, args);
	va_end(args);

	return set_res_int(id, key, val);
}

F8RES_API __int enum_resources(
	_ERES_PROC p, 
	__int context
	)
{
	RESMAP::iterator it;
	f8_uuid id;
	__int count = 0;

	it = g_Resources.begin();
	while(it != g_Resources.end()){
		id = it->second.id;
		it++;
		count++;
		if(!p(&id, context)){
			break;
		}
	}

	return count;
}

F8RES_API __int enum_res_items(
	const f8_uuid *id,
	_EITEM_PROC p,
	__int context
	)
{
	F8_RESOURCE * res;
	F8_RESITEM * item;
	__int count = 0;
	const char * k;

	res = _get_res(id);
	if(!res){
		return 0;
	}
	ITEMMAP::iterator it;
	it = res->pItems->begin();
	while(it != res->pItems->end()){
		item = &it->second;
		k = it->first.data();
		it++;
		count++;
		if(!p(k, item->buffer, item->length, context)){
			break;
		}
	}

	return count;
}

/*
	remove all entries from resource database.
*/
F8RES_API void reset_resources()
{	
	F8_RESOURCE * res;
	RESMAP::iterator it;
	it = g_Resources.begin();
	while(it != g_Resources.end()){
		res = &it->second;
		_free_resource(res);
		it++;
	}
	g_Resources.clear();
}

static __bool _cp_item(
	const char * key,							 
	const void * buf, 
	__int length, 
	__int context
	)
{
	set_res_buf_ex((f8_uuid*)context, buf, length, key, __false);
	return __true;
}

/*
	make a copy of a resource, initialize the target resource's
	reference count to 1.
	if the target resource already exists, then the reference
	count is incremented but the contents are overwritten.
*/
F8RES_API int copy_resource(
	const f8_uuid * s, 
	const f8_uuid * t
	)
{	
	F8_RESOURCE * r;

	if(query_resource(s) < 0){
		return 0;
	}
	
	r = _get_res(t);
	if(r){
		// _free_items(r->pItems);
		r->refcount++;
	}else{
		create_resource(t);
	}
	r = _get_res(t);
	if(!r){
		return F8_LOW_MEMORY;
	}

	return enum_res_items(s, _cp_item, (__int)t);
}

/*
	move_resource
	will decrement usage count of the source resource,
	note this will not necessary cause the source resource
	to be deleted, only if its usage count reaches zero.
*/
F8RES_API int move_resource(
	const f8_uuid * s, 
	const f8_uuid * t
	)
{
	int r;
	F8_RESOURCE *tr, *sr;

	if(query_resource(t) >= 0){
		return F8_NAME_DUPLICATE;
	}
	sr = _get_res(s);
	if(!sr){
		return F8_OBJECT_NOT_FOUND;
	}
	if(!create_resource(t)){
		return F8_LOW_MEMORY;
	}
	if(sr->refcount == 1){
		/*
			a little optimization, if the reference count of source
			is 1, then no actual copy is made, instead, we move
			the contents of s to t, and detach s from its contents.
		*/
		tr = _get_res(t);
		delete tr->pItems;
		tr->pItems = _get_res(s)->pItems;
		sr->pItems = 0;
		_free_resource(sr);
		r = F8_SUCCESS;
	}else{
		r = copy_resource(s, t);
	}
	/*
	this will decrement the reference count, and remove the
	resource entry if necessary
	*/
	unload_resource(s);
	return r;
}

/*
	return resource reference count of a resource.
*/
F8RES_API __int query_resource(
	const f8_uuid * id
	)
{
	RESMAP::iterator it;
	it = g_Resources.find(*id);
	if(it == g_Resources.end()){
		return -100;
	}
	return it->second.refcount;
}

/*
	a seemingly useful but not yet used proc.
	2005/6/24
*/
F8RES_API __bool set_del_res_proc(
	f8_uuid * rid, 
	_DEL_RES_PROC p
	)
{
	F8_RESOURCE * res;
	res = _get_res(rid);
	if(res){
		res->delProc = p;
		return __true;
	}
	return __false;
}

