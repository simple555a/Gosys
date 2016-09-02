#pragma warning(disable:4996)
#include "precomp.h"

#if 0

FBD_API f8_int blk_traverse(IBlk * root, BLK_TRAVERSE_PROC proc, f8_uint context)
{
	return ITreeNode_traverse(
			__ucast__(IBlk, ITreeNode, root),
			proc,
			__true,
			ITreeNode_post_order,
			(__uint)(context)
		);
}

#endif


/*
	copy first part of path into part, and return a marker which can be used
	in a second	call to _first_part().
	if path is led by '/', then '/' is copied.
	else the substring of path before the first occurencf of '/' is returned.
*/
static const char * _first_part(
	const char * path, 
	char part[F8_MAX_PATH]
	)
{
	const char *p;
	
	if('/' == *path){
		*part = '/';
		part[1] = 0;
		return path + 1;
	}

	*part = 0;
	p = path;
	/* 
		path doesn't begin with '/', copy every thing before 
		first occurence of '/'
	*/
	while(*p){
		if('/' == *p){
			// ok, copy the substring before p and return p+1
			p++;
			break;
		}
		*part++ = *p++;
	}
	*part = 0;
	return p;
}


FBD_API IBlk * IBlk_subblk_by_name(IBlk * me, const char * n)
{
	IBlk * b;
	ITreeNode * c;
	const char * name;

	c = __ucast__(IBlk, ITreeNode, me);
	if( !strcmp(n, "..") ){
		return __dcast__(ITreeNode, IBlk, c->uplink);
	}else if( n[0] == '/' && n[1] == 0){
		while(c->uplink){
			c = c->uplink;
		}
		b = __dcast__(ITreeNode, IBlk, c);
		return b;
	}else{
		c = c->first_child;
		while(c){
			b = __dcast__(ITreeNode, IBlk, c);
			name = IBlk_name(b);
			assert(name);
			if( !strcmpi(name, n) ){
				return b;
			}
			c = ITreeNode_next(c);
		}
	}
	return 0;
}

/*
// test of _first_part()
int main()
{
	char line[256], part[256];
	char *nextpart;
	int i;

	while(1){
		gets(line);
		nextpart = line;
		i = 0;
		while(*nextpart){
			nextpart = _first_part(nextpart, part);			
			if(*part){
				i++;
				printf("Part %d=%s\n", i, part);
			}
		}
	}
	return 1;
}
*/


FBD_API struct blk_pin_t * IBlk_pin_by_name(IBlk * blk, const char * name)
{
	int i;
	struct blk_pin_t * p;
	const char *pname;
	__bool bGot = __false;

	p = __vcall__(blk, first_pin, (blk));
	for(i=0; i<blk->h.n_pins; i++){
		pname = IBlk_pin_name(p);
		if(!pname){
			break;
		}
		if(!stricmp(pname, name)){
			bGot = __true;
			break;
		}
		p = __vcall__(blk, next_pin, (p));
	}		
	
	return bGot? p : 0;
}

FBD_API struct blk_pin_t * IBlk_pin_by_path(IBlk * cur, const char * path)
{
	char part[F8_MAX_PATH];
	const char *nextpart;
	struct blk_pin_t * pin = 0;

	if(strlen(path) > F8_MAX_PATH){
		return 0;
	}

	nextpart = path;
	while( *nextpart ){
		nextpart = _first_part(nextpart, part);
		if(*nextpart){
			// this is the block name part
			cur = IBlk_subblk_by_name(cur, part);
			if(!cur){
				return 0;
			}
		}else{
			// this is the pin name part
			pin = IBlk_pin_by_name(cur, part);
		}
	}

	return pin;
}

/*
	IBlk::blk_from_path
*/
FBD_API IBlk * IBlk_blk_by_path(IBlk * cur, const char * path)
{
	char part[F8_MAX_PATH];
	const char *nextpart;

	if(strlen(path) > F8_MAX_PATH){
		return 0;
	}

	nextpart = path;
	while(*nextpart){
		nextpart = _first_part(nextpart, part);
		cur = IBlk_subblk_by_name(cur, part);
		if(!cur){
			return 0;
		}
	}

	return cur;
}

FBD_API const char * IBlk_pin_name(struct blk_pin_t * p)
{
	int i;
	IBlk * blk;
	struct cblk_pin_t * cp;
	const char *pname;
	static char buf[43];

	blk = p->blk;
	if(blk->h.magic == BBLK_MAGIC || blk->h.flags & BLK_F_READONLY){
		i = __vcall__(blk, id_by_pin, (p));
		pname = get_res_buf_v(&blk->h.uuid, 0, "!p%d", i);
		if(!pname){
			pname = get_res_buf_v(&blk->h.uuid, 0, "!p%d!name", i);
		}
	}else{
		cp = SAFE_CONTAINING_RECORD(p, struct cblk_pin_t, p);
		pname = get_res_name(&cp->uuid);
	}
	if(!pname){
		sprintf(buf, "auto_%08x", p);
		pname = buf;
	}
	return pname;
}

FBD_API __uint IBlk_path_by_pin(struct blk_pin_t * p, char *buf, __uint len)
{
	__uint l1, l2;

	if(!p || !buf || !len){
		return 0;
	}
	l1 = IBlk_path_by_blk(p->blk, buf, len);
	if(!l1){
		return 0;
	}
	l2 = strlen(IBlk_pin_name(p));
	if(l1 + l2 + 2> len){
		return 0;
	}
	strcat(buf, "/");
	strcat(buf, IBlk_pin_name(p));
	return l1 + l2;
}

FBD_API const char * IBlk_class_name(IBlk * blk)
{
	static char name[64];
	const char * cname;
	
	cname = get_res_name(&blk->h.uuid);
	if(!cname){
		sprintf(name, "unamed_%08x", (__u32)(blk->h.uuid >> 32));
		cname = name;
	}
	return cname;
}

FBD_API const char * IBlk_name(IBlk * blk)
{
	IBlk * pb;
	__u16 id;
	const char *name;
	static char buf[32];
	pb = parent_blk(blk);
	if(!pb){
		/* only embedded blocks can have a name */
		return 0;
	}
	if(pb->h.magic == BBLK_MAGIC || (pb->h.flags & BLK_F_READONLY)){
		id = IBlk_id_by_blk(blk);
		name = get_res_buf_v(&pb->h.uuid, 0, "!b%d", id);
		if(!name){
			name = get_res_buf_v(&pb->h.uuid, 0, "!b%d!name", id);
		}
	}else{
		name = get_res_name(&blk->uuid);
	}
	if(!name){
		sprintf(buf, "unamed_%08x", blk);
		name = buf;
	}
	return name;
}

FBD_API __uint IBlk_path_by_blk(IBlk * blk, char *buf, __uint len)
{
	__uint i;
	char *p;
	__uint partlen;
	const char * name;

	if(!len || !buf || !blk){
		return 0;
	}
	*buf = 0;
	i = 0;
	p = buf + len - 1;
	*p = 0;
	i = len -1;

#define prepend(s) \
	partlen = strlen(s);\
	if(partlen > i){\
		return 0;\
	}\
	p -= partlen;\
	i -= partlen;\
	strncpy(p, s, partlen);	

	if(!parent_blk(blk)){
		prepend("/");
	}else{
		name = IBlk_name(blk);
		while(blk){
			blk = parent_blk(blk);
			if(name){
				prepend(name);			
				prepend("/");		
			}
			if(blk){
				name = IBlk_name(blk);
			}
		}
	}

	memmove(buf, p, strlen(p) + 1);

	return strlen(p);
}

#if 0
/*
	2005/6/24 obsolete
	note the resource loading and unloading is not a semmetric
	operation. loading is cared by the specific code performing
	block construction, and is not concurrent with block construction,
	normally after that; while resource unloading is handled automatically
	when the block is unloaded.
*/
static __bool _lr_proc(ITreeNode *nd, __uint context)
{
	IBlk * blk;
	__bool * failed;

	failed = (__bool*)context;
	blk = __dcast__(ITreeNode, IBlk, nd);

	/*
		N.B. instance resource is loaded in a batch.
		client of the binary library is responsible to 
		call load_res_stream to load all instance resources.
		while class resources are managed internaly.
	*/
	if(!load_resource(&blk->h.uuid)){
		return *failed = __false;
	}
	return __true;
}

/*
	this function will load all class resources 
	for a given block, for blk and all its desendants.

	Note, a block's instance resource is not loaded this way.
	In fact, the instance resource is loaded by external loader.
*/
FBD_API __bool IBlk_load_resource(IBlk *blk)
{
	ITreeNode * nd;
	__bool	failed = __false;

	nd = __ucast__(IBlk, ITreeNode, blk);
	ITreeNode_traverse(nd, _lr_proc, __true, ITreeNode_post_order, (__uint)&failed);

	return !failed;
}

#endif

static __bool _ur_proc(ITreeNode *nd, __bool firstVisit, __uint context)
{
	IBlk * blk;

	blk = __dcast__(ITreeNode, IBlk, nd);

#ifdef F8_CONFIGURATOR
	unload_resource(&blk->h.uuid);
	unload_resource(&blk->uuid);
#endif

	return __true;
}

/*
	this function will unload all class resources 
	for a given block, for blk and all its desendants.
*/
__bool IBlk_unload_resource(IBlk *blk)
{
	ITreeNode * nd;
	__bool	failed = __false;

	nd = __ucast__(IBlk, ITreeNode, blk);
	ITreeNode_traverse(nd, _ur_proc, __true, ITreeNode_post_order, (__uint)&failed);

	return !failed;
}

static void _GetBlkIdAndPrefix(IBlk * blk, f8_uuid * id, char * pfix, int len)
{
	IBlk * pb;
	f8_u16	bid;
	pb = parent_blk(blk);
	if(pb){
		if(BLK_IS_READONLY(pb)){
			*id = pb->h.uuid;
			bid = IBlk_id_by_blk(blk);
			_snprintf(pfix, len, "!b%d", bid);
		}else{
			*id = blk->uuid;
			*pfix = 0;
		}
	}else{
		*pfix = 0;
		*id = blk->uuid;
	}
}

FBD_API void * IBlk_get_data(IBlk * blk, const char * name, int * len)
{
	char	key[256];
	f8_uuid id;
	_GetBlkIdAndPrefix(blk, &id, key, sizeof(key));
	strncat(key, name, sizeof(key));
	return get_res_buf(&id, len, key);
}

FBD_API f8_bool IBlk_set_data(IBlk * blk, const char * name, void * buf, int len)
{
	char	key[256];
	f8_uuid id;
	_GetBlkIdAndPrefix(blk, &id, key, sizeof(key));
	strncat(key, name, sizeof(key));
	return set_res_buf(&id, buf, len, key);
}

