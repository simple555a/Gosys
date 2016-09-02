/*
	controller side variable table representation
	07/08/2005, panxiaohui@actech-mc.com
*/
#include "precomp.h"

static PRTK_LIST_ENTRY varTable;
static f8_uint varCount;

void ki_init_var_table()
{
	if(!varTable){
		varTable = ke_init_hash();
		varCount = 0;
	}
}

void ki_uninit_var_table()
{
	ki_reset_var_table();
	ke_uninit_hash(varTable);
	varTable = 0;
}

static __bool _remove_var(f8_uint context, const hash_key_t * k, const hash_val_t v)
{
	mm_free(kern_heap, v);
	return __true;
}

struct _find_id_context{
	const char * name;
	f8_uuid id;
	f8_bool found;
};

static int my_toupper(int c)
{
	return toupper(c);
}

static int my_strcmpi
    (
    const char * s1,   /* string to compare */
    const char * s2    /* string to compare <s1> to */
    )
    {
    while (my_toupper(*s1++) == my_toupper(*s2++))
	if (s1 [-1] == 0)
	    return (0);

    return (my_toupper(s1 [-1]) - my_toupper(s2 [-1]));
}

static __bool _find_id(f8_uint context, const hash_key_t * k, const hash_val_t v)
{
	struct _find_id_context * c;
	c=(struct _find_id_context*)context;
	
	if(!my_strcmpi(((struct blk_var_t*)v)->name,c->name)){
		c->found=f8_true;
		c->id=*k;
		return __false;
	}
	return __true;
}

f8_uuid ki_id_of_var(const char * name)
{
	struct _find_id_context c;
	c.name=name;
	c.found=f8_false;
	ke_enum_hash(varTable,_find_id,(f8_uint)&c);
	if(c.found)
		return c.id;
	return 0;
}

struct blk_var_t * ki_var_of_id(f8_uuid id)
{
	return (struct blk_var_t *)ke_get_hash(varTable,&id);
}

void ki_reset_var_table()
{
	if(varTable){
		ke_enum_hash(varTable, _remove_var, 0);
		ke_clear_hash(varTable);
	}
	varCount = 0;
}

f8_status ki_add_var(struct blk_var_t * v)
{
	struct blk_var_t *nv;
	nv = mm_alloc(kern_heap, sizeof(struct blk_var_t), 0);
	if(!nv){
		return F8_LOW_MEMORY;
	}

	// only global variables are defined
	*nv=*v;
	// zero id meas root block
	nv->blk_uuid=0;
	if(kernelState == KERN_S_RUNNING)
		memcpy(ke_get_section(&g_kernel,v->addr.section)+v->addr.addr,&v->initVal,_type_size(v->type));
	
	ke_set_hash(varTable, &v->uuid, nv);
	varCount++;
	assert(varCount >= 0);
	return F8_SUCCESS;
}

f8_status ki_remove_var(const f8_uuid * id)
{
	struct blk_var_t * nv;
	nv = (struct blk_var_t*)ke_get_hash(varTable, id);
	if(!nv){
		return F8_OBJECT_NOT_FOUND;
	}
	mm_free(kern_heap, nv);
	ke_del_hash(varTable, id);
	varCount--;
	assert(varCount >= 0);
	return F8_SUCCESS;
}

f8_status ki_edit_var(struct blk_var_t * v)
{
	struct blk_var_t * nv;
	nv = (struct blk_var_t*)ke_get_hash(varTable, &v->uuid);
	if(!nv){
		return F8_OBJECT_NOT_FOUND;
	}
	*nv = *v;
	// only global variables are defined
	nv->blk_uuid=0;
	if(kernelState == KERN_S_RUNNING)
		memcpy(ke_get_section(&g_kernel,v->addr.section)+v->addr.addr,&v->initVal,_type_size(v->type));
	return F8_SUCCESS;
}

f8_status ki_load_var_table(f8_uint count, struct blk_var_t * defs)
{
	f8_uint i;
	f8_status code;
	struct blk_var_t *v;
	v = defs;
	ki_reset_var_table();
	for(i=0; i<count; i++, v++){
		code = ki_add_var(v);
		if(F8_FAILED(code)){
			return code;
		}
		
	}
	return F8_SUCCESS;
}

static __bool _load_value(f8_uint context, const hash_key_t * k, const hash_val_t v)
{
	struct blk_var_t *var=(struct blk_var_t*)v;
	memcpy(ke_get_section(&g_kernel,var->addr.section)+var->addr.addr,&var->initVal,_type_size(var->type));
	return __true;
}

void ki_load_variable_values()
{
	ke_enum_hash(varTable,_load_value,0);
}

static __bool _save_var(f8_uint context, const hash_key_t * k, const hash_val_t v)
{
	IF8Stream * s;
	struct blk_var_t * nv;
	s = (IF8Stream*)context;
	nv = (struct blk_var_t*)v;
	if(!__vcall__(s, put, (s, nv, sizeof(struct blk_var_t)))){
		return __false;
	}
	return __true;
}

f8_status ki_save_var_table(IF8Stream * s)
{
	f8_uint count;
	count = ke_enum_hash(varTable, _save_var, (f8_uint)s);
	if(count == varCount){
		return F8_SUCCESS;
	}
	return F8_LOW_MEMORY;
}

