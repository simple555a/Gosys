#include "precomp.h"

#define NO_HASH_DEBUG

#ifndef HASH_BUCKETS
#define HASH_BUCKETS	(1009)
#endif

typedef struct __tag_hash_entry_t{
	RTK_LIST_ENTRY link;
	hash_key_t k;
	hash_val_t v;
}ent_t;

static __inline ent_t * _hash_get_entry(RTK_LIST_ENTRY *buckets, const hash_key_t * t)
{
	int idx;
	ent_t * e;
	RTK_LIST_ENTRY *li, *lk;

	idx = (f8_u32)((*t & 0xffffffff)) % HASH_BUCKETS;
	li = &buckets[idx];
	if(RtkIsListEmpty(li)){
		return 0;
	}
	lk = li->Flink;
	while(lk != li){
		e = (ent_t*)lk;
		if(e->k == *t){
			return e;
		}
		lk = lk->Flink;
	}
	return 0;
}

FKERN_API hash_val_t ke_get_hash(RTK_LIST_ENTRY *buckets, const hash_key_t * t)
{
	ent_t * e;
#ifndef NO_HASH_DEBUG
	char uuidname[64];
#endif
	e = _hash_get_entry(buckets, t);
#ifndef NO_HASH_DEBUG	
	f8_uuid_to_string(t, uuidname, sizeof(uuidname));
	KD_PRINT(("gethash(%s)=%p\n", uuidname, e? e->v : 0));
#endif
	if(!e){
		return 0;
	}
	return e->v;
}

FKERN_API __bool ke_set_hash(RTK_LIST_ENTRY *buckets, const hash_key_t * t, hash_val_t v)
{
	int idx;
	ent_t * e;
	RTK_LIST_ENTRY *li, *lk;

#ifndef NO_HASH_DEBUG
	char uuidname[64];
	f8_uuid_to_string(t, uuidname, sizeof(uuidname));
	KD_PRINT(("sethash(%s, %p)\n", uuidname, v));
#endif
	idx = ((f8_u32)(*t & 0xffffffff)) % HASH_BUCKETS;
	li = &buckets[idx];
	lk = li->Flink;
	while(lk != li){
		e = (ent_t*)lk;
		if(e->k == *t){
			e->v = v;
			return __true;
		}
		lk = lk->Flink;
	}
	e = (ent_t*)mm_alloc(kern_heap, sizeof(ent_t), 0);
	e->k = *t;
	e->v = v;
	if(!e){
		return __false;
	}
	RtkInsertTailList(li, &e->link);

	return __true;
}

FKERN_API __bool ke_del_hash(RTK_LIST_ENTRY * buckets, const hash_key_t * t)
{
	ent_t * e;
	e = _hash_get_entry(buckets, t);
	if(e){
		RtkRemoveEntryList(&e->link);
		mm_free(kern_heap, e);
		return __true;
	}
	return __false;
}

FKERN_API RTK_LIST_ENTRY * ke_init_hash()
{
	int i;
	PRTK_LIST_ENTRY buckets;
	PRTK_LIST_ENTRY li;

	buckets = (PRTK_LIST_ENTRY)mm_alloc(kern_heap, HASH_BUCKETS*sizeof(RTK_LIST_ENTRY), 0);
	if(!buckets){
		return 0;
	}
	li = &buckets[0];
	for(i=0; i<HASH_BUCKETS; i++){
		RtkInitializeListHead(li);
		li++;
	}
	return buckets;
}

FKERN_API void ke_uninit_hash(RTK_LIST_ENTRY * buckets)
{
	ke_clear_hash(buckets);
	mm_free(kern_heap, buckets);
}

FKERN_API void ke_clear_hash(RTK_LIST_ENTRY * buckets)
{
	int i;
	PRTK_LIST_ENTRY li, lk;

	li = buckets;
	for(i=0; i<HASH_BUCKETS; i++, li++){
		while(!RtkIsListEmpty(li)){
			lk = li->Flink;
			RtkRemoveEntryList(lk);
			mm_free(kern_heap, lk);
		}
	}
}

FKERN_API f8_uint ke_enum_hash(
	RTK_LIST_ENTRY * buckets, 
	__bool (*visitor)(f8_uint context, const hash_key_t * k, const hash_val_t v),
	f8_uint context
	)
{
	int i;
	f8_uint count;
	PRTK_LIST_ENTRY li, lk;
	ent_t * e;

	li = buckets;
	count = 0;
	for(i=0; i<HASH_BUCKETS; i++, li++){
		lk = li->Flink;
		while(lk != li){
			e = RTK_CONTAINING_RECORD(lk, ent_t, link);
			count++;
			if(!visitor(context, &e->k, e->v)){
				break;
			}
			lk = lk->Flink;
		}
	}

	return count;
}

