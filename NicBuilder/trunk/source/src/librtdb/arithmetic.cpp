#include "precomp.h"

static __i8 precedences[]={
	-1, // dt_null = 0,
	11, // dt_bool = 1,
	2, // dt_real4 = 2,
	5, // dt_int32 = 3,	
	1, // dt_real8 = 4,
	1, // dt_date = 5,
	
	9, // dt_int8 = 6,
	7, // dt_int16 = 7,
	3, // dt_int64 = 8,
	
	10, // dt_uint8 = 9,
	8, // dt_uint16 = 10,
	6, // dt_uint32 = 11,
	4, // dt_uint64 = 12,

	0, // dt_string = 13,
	-1, // dt_bstr = 14, 

	-1, // dt_array = 15,
	-1, // dt_dispatch = 16,

	// dt_MaxType = 17,
};

RTDB_API int PMC_API pmc_value_add(
	pmc_value_t * d, 
	const pmc_value_t * o1, 
	const pmc_value_t * o2
	)
{
	pmc_value_t v;
	__u8 t1, t2;

	t1 = get_value_type(o1->Flags);
	t2 = get_value_type(o2->Flags);
	
	if(precedences[t1] < 0 || precedences[t2] < 0){
		return -1;
	}

	if(precedences[t1] > precedences[t2]){
		// swap o1 and o2
		const pmc_value_t * vv;
		vv = o2;
		o2 = o1;
		o1 = vv;
		t1 = get_value_type(o1->Flags);
		t2 = get_value_type(o2->Flags);
	}

	// ok, now o1 is assured to be at least as precedent as o2
	if(precedences[t1] < precedences[t2]){
		set_value_type(v.Flags, t1);
		if(!pmc_type_cast(o2, &v)){
			return -2;
		}
		o2 = &v;
	}else{
		assert(precedences[t1] == precedences[t2]);
	}

	set_value_type(d->Flags, t1);
	switch(t1){
	case dt_string:
		d->Value.str = (char *)alloc_buffer_v(
			o1->Value.str, 
			strlen(o1->Value.str), 
			o2->Value.str,
			strlen(o2->Value.str) + 1,
			0
			);
		break;
	case dt_bool:
		d->Value.b = o1->Value.b || o2->Value.b;
		break;
	case dt_int8:
	case dt_uint8:
		d->Value.i8 = o1->Value.i8 + o2->Value.i8;
		break;
	case dt_int16:
	case dt_uint16:
		d->Value.i16 = o1->Value.i16 + o2->Value.i16;
	break;
	case dt_int32:
	case dt_uint32:
		d->Value.i32 = o1->Value.i32 + o2->Value.i32;
	break;
	case dt_int64:
	case dt_uint64:
		d->Value.i64 = o1->Value.i64 + o2->Value.i64;
	break;
	case dt_real4:
		d->Value.flt = o1->Value.flt + o2->Value.flt;
	break;
	case dt_real8:
	case dt_date:
		d->Value.dbl = o1->Value.dbl + o2->Value.dbl;
	break;
	}

	return 0;
}

RTDB_API int PMC_API pmc_value_sub(
	pmc_value_t * d, 
	const pmc_value_t * o1, 
	const pmc_value_t * o2
	)
{
	pmc_value_t v;
	__u8 t1, t2;

	t1 = get_value_type(o1->Flags);
	t2 = get_value_type(o2->Flags);
	
	if(precedences[t1] < 1 || precedences[t2] < 1){
		return -1;
	}

	if(precedences[t1] > precedences[t2]){
		// swap o1 and o2
		const pmc_value_t * vv;
		vv = o2;
		o2 = o1;
		o1 = vv;
		t1 = get_value_type(o1->Flags);
		t2 = get_value_type(o2->Flags);
	}

	// ok, now o1 is assured to be at least as precedent as o2
	if(precedences[t1] < precedences[t2]){
		set_value_type(v.Flags, t1);
		if(!pmc_type_cast(o2, &v)){
			return -2;
		}
		o2 = &v;
	}else{
		assert(precedences[t1] == precedences[t2]);
	}

	set_value_type(d->Flags, t1);
	switch(t1){
	case dt_bool:
		d->Value.b = o1->Value.b ^ o2->Value.b;
		break;
	case dt_int8:
	case dt_uint8:
		d->Value.i8 = o1->Value.i8 - o2->Value.i8;
		break;
	case dt_int16:
	case dt_uint16:
		d->Value.i16 = o1->Value.i16 - o2->Value.i16;
	break;
	case dt_int32:
	case dt_uint32:
		d->Value.i32 = o1->Value.i32 - o2->Value.i32;
	break;
	case dt_int64:
	case dt_uint64:
		d->Value.i64 = o1->Value.i64 - o2->Value.i64;
	break;
	case dt_real4:
		d->Value.flt = o1->Value.flt - o2->Value.flt;
	break;
	case dt_real8:
	case dt_date:
		d->Value.dbl = o1->Value.dbl - o2->Value.dbl;
	break;
	}

	return 0;
}

RTDB_API int PMC_API pmc_value_mul(
	pmc_value_t * d, 
	const pmc_value_t * o1, 
	const pmc_value_t * o2
	)
{
	pmc_value_t v;
	__u8 t1, t2;

	t1 = get_value_type(o1->Flags);
	t2 = get_value_type(o2->Flags);
	
	if(precedences[t1] < 1 || precedences[t2] < 1){
		return -1;
	}

	if(precedences[t1] > precedences[t2]){
		// swap o1 and o2
		const pmc_value_t * vv;
		vv = o2;
		o2 = o1;
		o1 = vv;
		t1 = get_value_type(o1->Flags);
		t2 = get_value_type(o2->Flags);
	}

	// ok, now o1 is assured to be at least as precedent as o2
	if(precedences[t1] < precedences[t2]){
		set_value_type(v.Flags, t1);
		if(!pmc_type_cast(o2, &v)){
			return -2;
		}
		o2 = &v;
	}else{
		assert(precedences[t1] == precedences[t2]);
	}

	set_value_type(d->Flags, t1);
	switch(t1){
	case dt_bool:
		d->Value.b = o1->Value.b && o2->Value.b;
		break;
	case dt_int8:
	case dt_uint8:
		d->Value.i8 = o1->Value.i8 * o2->Value.i8;
		break;
	case dt_int16:
	case dt_uint16:
		d->Value.i16 = o1->Value.i16 * o2->Value.i16;
	break;
	case dt_int32:
	case dt_uint32:
		d->Value.i32 = o1->Value.i32 * o2->Value.i32;
	break;
	case dt_int64:
	case dt_uint64:
		d->Value.i64 = o1->Value.i64 * o2->Value.i64;
	break;
	case dt_real4:
		d->Value.flt = o1->Value.flt * o2->Value.flt;
	break;
	case dt_real8:
	case dt_date:
		d->Value.dbl = o1->Value.dbl * o2->Value.dbl;
	break;
	}

	return 0;
}

RTDB_API int PMC_API pmc_value_div(
	pmc_value_t * d, 
	const pmc_value_t * o1, 
	const pmc_value_t * o2
	)
{
	pmc_value_t v;
	__u8 t1, t2;

	t1 = get_value_type(o1->Flags);
	t2 = get_value_type(o2->Flags);
	
	if(precedences[t1] < 1 || precedences[t2] < 1){
		return -1;
	}

	if(precedences[t1] > precedences[t2]){
		// swap o1 and o2
		const pmc_value_t * vv;
		vv = o2;
		o2 = o1;
		o1 = vv;
		t1 = get_value_type(o1->Flags);
		t2 = get_value_type(o2->Flags);
	}

	// ok, now o1 is assured to be at least as precedent as o2
	if(precedences[t1] < precedences[t2]){
		set_value_type(v.Flags, t1);
		if(!pmc_type_cast(o2, &v)){
			return -2;
		}
		o2 = &v;
	}else{
		assert(precedences[t1] == precedences[t2]);
	}

	set_value_type(d->Flags, t1);
	switch(t1){
	case dt_bool:
		d->Value.b = o1->Value.b ^ o2->Value.b;
		break;
	case dt_int8:
	case dt_uint8:
		d->Value.i8 = o1->Value.i8 / o2->Value.i8;
		break;
	case dt_int16:
	case dt_uint16:
		d->Value.i16 = o1->Value.i16 / o2->Value.i16;
	break;
	case dt_int32:
	case dt_uint32:
		d->Value.i32 = o1->Value.i32 / o2->Value.i32;
	break;
	case dt_int64:
	case dt_uint64:
		d->Value.i64 = o1->Value.i64 / o2->Value.i64;
	break;
	case dt_real4:
		d->Value.flt = o1->Value.flt / o2->Value.flt;
	break;
	case dt_real8:
	case dt_date:
		d->Value.dbl = o1->Value.dbl / o2->Value.dbl;
	break;
	}

	return 0;
}

