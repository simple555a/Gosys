#include "precomp.h"

static void dumpBblk(IBlk * b, FILE * of)
{
	const char * name;
	char path[F8_MAX_PATH];
	char buf[1024];
	char idName[128];
	struct blk_ent_t be;
	struct blk_pin_t * p;

	*buf=0;
	if(!(parent_blk(b)->h.flags & BLK_F_READONLY)){
		name = IBlk_name(b);
		IBlk_path_by_blk(parent_blk(b), path, sizeof(buf));
		if(blklib_query(&b->h.uuid, &be)){
			sprintf(buf, "mount  path=\"%s\" type=\"%s.%s\" name=\"%s\"", path, be.dir, be.name, name);
		}else{
			f8_uuid_to_string(&b->h.uuid, idName, sizeof(idName));
			sprintf(buf, "mount  path=\"%s\" id=%s name=\"%s\" ", path, idName, name);
		}
		if(of==stdout)
			utils_trace("%s\n", buf);
		else
			fprintf(of, "%s\n", buf);
		IBlk_path_by_blk(b, path, sizeof(path));
		sprintf(buf, "place block \"%s\" (%d,%d)", path, get_res_int(&b->uuid, "x", 0), get_res_int(&b->uuid, "y", 0));
		if (of==stdout)
			utils_trace("%s\n", buf);
		else
			fprintf(of, "%s\n", buf);		
	}
	
	for(p = __vcall__(b, first_pin, (b)); p; p = __vcall__(b, next_pin, (p))){
		if(p->_class != PIN_CLASS_DI && p->_class != PIN_CLASS_DO){
			continue;
		}
		IBlk_path_by_pin(p, path, sizeof(path));
		// dump binding
		*buf = 0;
		if(p->binding == PIN_B_CONST){
			switch(p->type){
			case  PIN_T_INTEGER	:
				if(p->dp->u.d.i32 != 0){
					sprintf(buf, "bind \"%s\" %d", path, p->dp->u.d.i32);
				}
				break;
			case  PIN_T_BOOL		:
				if(p->dp->u.d.ui8 != 0){
					sprintf(buf, "bind \"%s\" %d", path, p->dp->u.d.ui8);
				}
				break;
			case  PIN_T_FLOAT		:
				if(p->dp->u.d.flt != 0){
					sprintf(buf, "bind \"%s\" %f", path, p->dp->u.d.flt);
				}
				break;
			case  PIN_T_BYTE		:
				if(p->dp->u.d.ui8 != 0){
					sprintf(buf, "bind \"%s\" %d", path, p->dp->u.d.ui8);
				}
				break;
			case  PIN_T_WORD		:
				if(p->dp->u.d.ui16 != 0){
					sprintf(buf, "bind \"%s\" %d", path, p->dp->u.d.ui16);
				}
				break;
			case  PIN_T_DOUBLE	:
				if(p->dp->u.d.dbl != 0){
					sprintf(buf, "bind \"%s\" %.15f", path, p->dp->u.d.dbl);
				}
				break;
			case  PIN_T_DATE		:
				if(fabs(p->dp->u.d.dbl) > 1e-9){
					if(p->dp->u.d.dbl > 2.0){
						sprintf(buf, "bind \"%s\" t#%fs", path, p->dp->u.d.dbl);
					}else{
						sprintf(buf, "bind \"%s\" t#%fms", path, p->dp->u.d.dbl*1000);
					}
				}
				break;
			case  PIN_T_CHAR		:
				if(p->dp->u.d.i8 != 0){
					sprintf(buf, "bind \"%s\" %d", path, p->dp->u.d.i8);
				}
				break;
			case  PIN_T_SHORT		:
				if(p->dp->u.d.i16 != 0){
					sprintf(buf, "bind \"%s\" %d", path, p->dp->u.d.ui16);
				}
				break;
			case  PIN_T_DWORD		:
				if(p->dp->u.d.ui32 != 0){
					sprintf(buf, "bind \"%s\" %d", path, p->dp->u.d.ui32);
				}
				break;
			}
		}else {
			struct blk_var_t * v;
			v = query_var_by_pin(p);
			if(v){
				char vpath[F8_MAX_PATH];
				IBlk_path_by_blk(v->blk, vpath, sizeof(vpath));
				sprintf(buf, "bind \"%s\" %s.%s", path, vpath, v->name);
				// on_bind
			}
		}

		if(*buf){
			if(of==stdout)
				utils_trace("%s\n", buf);
			else
				fprintf(of, "%s\n", buf);
		}
		
		if(p->type==PIN_T_BOOL && (p->flags & PIN_F_INVERTED)){
			sprintf(buf, "invert \"%s\"", path);
			if(of==stdout)
				utils_trace("%s\n", buf);
			else
				fprintf(of, "%s\n", buf);
		}
		
	}
}

static __bool defineVariable(struct blk_var_t * v, __uint context)
{
	FILE * of = (FILE*)context;
	char buf[1024];
	char path[F8_MAX_PATH];
	const char *tp = _type_name(v->type);
	IBlk_path_by_blk(v->blk, path, sizeof(path));
	sprintf(buf, "set type=%s name=\"%s\" comment=\"%s\" scope=\"%s\"", tp, v->name, v->comment, path);
	if (of==stdout)
		utils_trace("%s\n", buf);
	else
		fprintf(of, "%s\n", buf);
	return __true;
}

static void dumpCblk(IBlk * b, __bool firstVisit, FILE * of)
{
	const char * name;
	char path[F8_MAX_PATH];
	char buf[1024];
	char idName[128];
	struct blk_ent_t be;

	if(firstVisit){
		name = IBlk_name(b);
		IBlk_path_by_blk(parent_blk(b), path, sizeof(path));
		if(parent_blk(b) && !(parent_blk(b)->h.flags & BLK_F_READONLY)){
			if(b->h.flags & BLK_F_READONLY){
				if(blklib_query(&b->h.uuid, &be)){
					sprintf(buf, "mount  path=\"%s\" type=\"%s.%s\" name=\"%s\"", path, be.dir, be.name, name);
				}else{
					f8_uuid_to_string(&b->h.uuid, idName, sizeof(idName));
					sprintf(buf, "mount  path=\"%s\" id=%s name=\"%s\"", path, idName, name);
				}
				if(of==stdout)
					utils_trace("%s\n",buf);
				else
					fprintf(of, "%s\n", buf);
				return;
			}else{
				sprintf(buf, "mount path=\"%s\" file=blank.blk name=\"%s\" rw", path, name);
			}
			if (of==stdout)
				utils_trace("%s\n",buf);
			else
				fprintf(of, "%s\n", buf);
			IBlk_path_by_blk(b, path, sizeof(path));
			sprintf(buf, "place block \"%s\" (%d,%d)", path, get_res_int(&b->uuid, "x", 0), get_res_int(&b->uuid, "y", 0));
			if(of==stdout)
				utils_trace("%s\n",buf);
			else
				fprintf(of, "%s\n", buf);
		}

		enum_variables(b, defineVariable, (__uint)of);
	}else{
		/*
			dump links positions/export pins
		*/
		if(b->h.flags & BLK_F_READONLY){
			return;
		}
		ICBlk * cb = __dcast__(IBlk, ICBlk, b);
		struct cblk_link_t * l;
		for(l=ICBlk_first_link(cb); l; l=ICBlk_next_link(l)){
			char p1[F8_MAX_PATH];
			char p2[F8_MAX_PATH];
			IBlk_path_by_pin(l->s.pin, p1, sizeof(p1));
			IBlk_path_by_pin(l->t.pin, p2, sizeof(p2));
			sprintf(buf, "link \"%s\" \"%s\"", p1, p2);
			if(of==stdout)
				utils_trace("%s\n",buf);
			else
				fprintf(of, "%s\n", buf);

			TLinkMetrics *m;
			m = (TLinkMetrics *)get_res_buf(&l->uuid, 0, "metrics");
			if(m){
				char *p;
				p = buf;
				p += sprintf(p, "place link \"%s\" %d ", p2, m->count);
				int i;
				for(i=0; i<m->count; i++){
					p += sprintf(p, "(%d,%d) ", m->coord[i].x, m->coord[i].y);
				}
				if(of==stdout)
					utils_trace("%s\n",buf);
				else
					fprintf(of, "%s\n", buf);
			}
		}
	}
}

static __bool dumpBlk(ITreeNode * nd, __bool firstVisit, __uint context)
{
	IBlk * b;
	FILE * of;
	b = __dcast__(ITreeNode, IBlk, nd);
	of = (FILE*)context;
	static int count;

#if 0
	fprintf(of, "#\n");
	fprintf(of, "#-------BLK NO.%d----------------------\n", count++);
	fprintf(of, "#\n");
#endif

	if(b->h.magic == BBLK_MAGIC){
		dumpBblk(b, of);
	}else{
		dumpCblk(b, firstVisit, of);
	}

	return __true;
}

static int dumpDevices(FILE * of)
{
	int i;
	struct io_device * d;
	devlib_ent_t * dd;
	char configStr[128];
	bool nullConfig;

	d = ke_get_device_list(proxy_adapter->kernel);

	for(i=0; i<IO_MAX_DEVICES; i++, d++){
		if(!(d->features & IO_DEVICE_PRESENT)){
			continue;
		}
		dd = blklib_get_dev_by_id(&d->clsid);
		if(dd && !strcmpi(dd->name, "none")){
			continue;
		}

		{
			int j;
			char * p;
			p=configStr;
			nullConfig=true;
			for(j=0;j<DEV_CONFIG_LEN;j++){
				p+=sprintf(p,"%02x", d->configData[j]);
				if(d->configData[j]){
					nullConfig=false;
				}
			}
		}
		
		if(dd){
			if(of==stdout)
				utils_trace("device %d %s %s\n", i, dd->name, nullConfig? "" : configStr);
			else
				fprintf(of, "device %d %s %s\n", i, dd->name, nullConfig? "" : configStr);
		}else{
			char uid[128];
			f8_uuid_to_string(&d->clsid, uid, sizeof(uid));
			if(of==stdout)
				utils_trace("device %d %s %s\n", i, uid, nullConfig? "" : configStr);
			else
				fprintf(of, "device %d %s %s\n", i, uid, nullConfig? "" : configStr);
		}
	}

	return 0;
}

static void dumpSource(FILE * of, IBlk * b)
{
	ITreeNode_traverse(
		__ucast__(IBlk, ITreeNode, b),
		dumpBlk,
		__true,
		ITreeNode_both_order,
		(__uint)of
		);
	
}

int on_export(char * v, void *k)
{
	CArgsEx a;
	char * path = 0;
	char * ofname = 0;
	__bool includeConfig = 0;
	__bool includeSource = 0;

	BEGIN_ARGS
		ARG("-of:", ofname, "char *", "output file name")
		ARG("-p:", path, "char *", "root block path")
		ARG("-config", includeConfig, "tbool", "include configuration")
		ARG("-source", includeSource, "tbool", "include source")
	END_ARGS
		
	a.parse(v, FS);

	if(process_args_1(a.argc, a.argv) < 0){
		print_usage_1();
		return F8_SYNTAX;
	}

	if(!includeSource && !includeConfig){
		return F8_SUCCESS;
	}

	IBlk * b;
	if(path){
		b = IBlk_blk_by_path(sections, path);
	}else{
		b = sections;
	}
	if(!b){
		utils_error("%s - path not found.\n", path);
		return F8_PATH_NOT_FOUND;
	}
	FILE * of;
	if(ofname){
		of = fopen(ofname, "wt");
		if(!of){
			utils_error("%s - cannot open.\n", ofname);
			return F8_FILE_NOT_FOUND;
		}
	}else{
		of = stdout;
	}

	if(includeSource){
		dumpSource(of, b);
	}

	if(includeConfig){
		dumpDevices(of);
	}

	if(of != stdout){
		fclose(of);
	}
	
	return F8_SUCCESS;
}

/*
	play commands contained in a script file
*/
int on_play(char *v, void *k)
{
	CArgsExA a;
	__bool silent;
	
	a.parse(v, FS);

	if(a.argc < 2){
		utils_error("Usage : play <script file>\n");
		return F8_SYNTAX;
	}

	FILE * fp;
	fp = fopen(a.argv[1], "rt");
	if(!fp){
		utils_error("%s - cannot open.\n", a.argv[1]);
		return F8_FILE_NOT_FOUND;
	}

	char buf[2048];
	silent = proxy_silent;
	proxy_silent = __true;
	while(fgets(buf, sizeof(buf), fp), !feof(fp)){
		// buf trimming
		if(*buf == 0){
			continue;
		}
		if(*buf == '\r' || *buf=='\n' || *buf=='#'){
			continue;
		}
		char *p = buf + strlen(buf) - 1;
		if(*p == '\r' || *p == '\n'){
			*p = 0;
		}
		kproxy_shell(buf);
	}
	
	fclose(fp);
	proxy_silent = silent;
	return F8_SUCCESS;
}


