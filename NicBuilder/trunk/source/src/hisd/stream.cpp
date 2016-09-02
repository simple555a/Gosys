//added by Zander on May 27th 2011
#pragma warning(disable : 4786)
#pragma warning(disable : 4018)

#include "precomp.h"

CHislogStream::CHislogStream(CInMemoryBuffer * p)
{
	m_deletePending = false;
	m_parent = p;
}

void CHislogStream::_debugDump(int n)
{
	hislog_value_list::iterator it;
	hislog_item_t * v;

	it = m_values.begin();
	if(n == -1){
		while(it != m_values.end()){
			v = &it->second;
			printf(
				"%s [0x%I64x] -- %s\n", 
				timeAsString(it->first).c_str(), 
				it->first,
				valueAsString(&it->second).c_str()
				);
			it++;
		}
		putchar('\n');
	}else{
		if(n < 0 || n > m_values.size()){
			return;
		}
		while(n--){
			it++;
		}
	}
}

/*
	the timestamp-value pair is stored in a red-black tree, maybe we 
	should try a B-tree implementation.
*/
__bool CHislogStream::getValue(hislog_key_t time, hislog_item_t * item)
{
	pair<hislog_value_list::iterator, hislog_value_list::iterator> pii;
	hislog_item_t *v1, *v2;
	hislog_key_t t1, t2;

	item->Flags = 0;

	/*
		get the value range so we can do an inner interpolate
	*/
	pii = m_values.equal_range(time);
	
	if(pii.first != m_values.end()){
		if(pii.first->first == time){
			*item = pii.first->second;
			return __true;
		}
		// move back by one item
		hislog_key_t tt = pii.first->first;
		assert(tt > time);
		//assert(pii.first->first > time);
		pii.first--;
	}

	/*
		now pii.first is the item with an earlier timestamp
		or m_values.end() if no such item.
		
		pii.second is the item with a later timestamp or
		m_values.end() if no such item.

		so we can do an inner interpolate now.
	*/
	if(pii.first != m_values.end()){
		t1 = pii.first->first;
		assert(time != t1);
		v1 = &((pii.first)->second);
		if(pii.second != m_values.end()){
			t2 = pii.second->first;
			assert(time != t2);
			
			if(t1 == t2){
				/* this is an exception, 
					pii.first--
				has no effect when pii.first is already m_values.begin()
				*/
				_debugDump(0);
				assert(t1 > time);
				*item = *v1;
				return __false;
			}
			
			// interpolate
			v2 = &((pii.second)->second);
			interpolate(v1, v2, item, t1, t2, time);
		}else{
			// use v1
			*item = *v1;
		}
	}else{
		if(pii.second != m_values.end()){
			*item = (pii.second)->second;
			return __true;
		}else{
			return __false;
		}
	}

	return __true;
}

void CHislogStream::linearCompress(
	hislog_key_t time,
	const hislog_item_t * snapShot, 
	__uint compMax, 
	__r8 compDev,
	__bool & bSave,
	__bool & bReplace
	)
{
	__r8 dev, dev2;
	//hislog_item_t v;
	/*
		linear compressing
	*/
	bReplace = __true;
	/*
	interpolate(
		&m_recentValues[0].value,
		snapShot,
		&v,
		m_recentValues[0].time,
		time,
		m_recentValues[1].time
		);
	*/
	if((snapShot->Flags & TF_Valid) != (m_recentValues[1].value.Flags & TF_Valid)){
		bSave = __true;
		bReplace = __false;
		return;
	}

	switch(get_value_type(snapShot->Flags)){
	case dt_bool:
		//if(!(m_recentValues[1].value.Value.b ^ snapShot->Value.b)
		//	&&
		//!(m_recentValues[0].value.Value.b ^ snapShot->Value.b)
		//){
		if(m_recentValues[0].value.Value.b == snapShot->Value.b
			&& m_recentValues[0].value.Value.b == m_recentValues[1].value.Value.b
			){
			bSave = __false;
			bReplace = __true;
		}else{
			bSave = __true;
			bReplace = __false;
		}
		return;
		break;
		
	case dt_real4:
		//dev = fabs(v.Value.fltValue - m_recentValues[1].value.Value.fltValue);  
		//dev2 = fabs(v.Value.fltValue - m_recentValues[0].value.Value.fltValue);
		dev = snapShot->Value.fltValue - m_recentValues[1].value.Value.fltValue;
		dev2 = m_recentValues[1].value.Value.fltValue - m_recentValues[0].value.Value.fltValue;
		break;
	case dt_real8:
	case dt_date:
		//dev = fabs(v.Value.dblValue - m_recentValues[1].value.Value.dblValue);
		//dev2 = fabs(v.Value.dblValue - m_recentValues[0].value.Value.dblValue);
		dev = snapShot->Value.dblValue - m_recentValues[1].value.Value.dblValue;
		dev2 = m_recentValues[1].value.Value.dblValue - m_recentValues[0].value.Value.dblValue;
		break;
	case dt_int8:
	case dt_int16:
	case dt_int32:
		//dev = abs(v.Value.i32 - m_recentValues[1].value.Value.i32);
		//dev2 = abs(v.Value.i32 - m_recentValues[0].value.Value.i32);
		dev = snapShot->Value.i32 - m_recentValues[1].value.Value.i32;
		dev2 = m_recentValues[1].value.Value.i32 - m_recentValues[0].value.Value.i32;
		break;
	case dt_uint8:
	case dt_uint16:
	case dt_uint32:
		//dev = abs(v.Value.u32 - m_recentValues[1].value.Value.u32);
		//dev2 = abs(v.Value.u32 - m_recentValues[0].value.Value.u32);
		dev = snapShot->Value.u32 - m_recentValues[1].value.Value.u32;
		dev2 = m_recentValues[1].value.Value.u32 - m_recentValues[0].value.Value.u32;
		break;
	default:
		/* we dont provide compressing for other types for now */
		return;
	}

	if((dev*dev2) < 0 || fabs(dev) > compDev || fabs(dev2) > compDev){
		bSave = __true;
		bReplace = __false;
	}else{
		bSave = __false;
		bReplace = __true;
	}
	//if(dev2 > compDev){
	//	bSave = __true;
	//	bReplace = __false;
	//}else if(dev < compDev){
	//	bSave = __false;
	//	bReplace = __true;
	//}
}

/*
	put a snapshot value into the in-memory buffer
	make compression if necessary.
*/
__bool CHislogStream::putSnapshot(
	hislog_key_t time, 
	const hislog_item_t * snapShot, 
	__bool bForceSave
	)
{
	hislog_item_t item;
	PCRTK_TAG p;
	pair<hislog_value_list::iterator, bool> pib;
	comp_method_t comp;
	__r4 compDev;
	__u32 compMax;
	__bool bSave = __true;
	__bool bReplace = __false;
	__bool bRet;
	hislog_value_list::iterator theIt;
	
	if(!snapShot){
		p = query_tag_f(&m_name);
		if(!p){
			return __false;
		}
		item = p->d.Value;
		snapShot = &item;

		comp = p->s.Compressing;
		compMax = p->s.CompMax;
		compDev = p->s.CompDev;
		if(!compMax){
			// save a point every 8 hour
			compMax = 3600*8;
		}
		if(compDev < 1e-9){
			compDev = (float)1e-9;
		}
	}else{
		comp = Compress_None;
	}

	if(!bForceSave && m_values.size() >= 2 && comp != Compress_None &&
		abs((double)(time - m_recentValues[1].time)) < compMax
	){ 
		// TODO: add data compressing code here
		// determine whether we should put this value into the
		// database
		if(snapShot->Flags & TF_Valid){
			if( (m_recentValues[1].value.Flags & TF_Valid) &&
				(m_recentValues[0].value.Flags & TF_Valid)){
				switch(comp){
					case Compress_Linear:
					case Compress_Const:
						linearCompress(
							time,
							snapShot, 
							compMax, 
							compDev,
							bSave,
							bReplace
							);
						break;
					default:
						;
				}
			}
		}else{
			if(!(m_recentValues[1].value.Flags & TF_Valid)){
				bSave = false;
			}
		}
	}else{
		bSave = true;
	}

	if(bSave){
		pib = m_values.insert(hislog_value_list::value_type(time, *snapShot));
		m_recentValues[0] = m_recentValues[1];
		//if(pib.second){
			m_recentValues[1].time = time;
			m_recentValues[1].value = *snapShot;
		//}else{
			//assert(0);
		//}
		bRet = pib.second;
	/*}else if(bReplace){
		// replace the last value
		theIt = m_values.end();
		theIt--;
		m_values.erase(theIt);
		pib = m_values.insert(hislog_value_list::value_type(time, *snapShot));
		if(pib.second){
			m_recentValues[1].time = time;
			m_recentValues[1].value = *snapShot;
		}else{
			assert(0);
		}
		bRet = pib.second;
	*/
	 }else{
		// replace the last value
		theIt = m_values.end();
		theIt--;
		m_values.erase(theIt);
		pib = m_values.insert(hislog_value_list::value_type(time, *snapShot));
		if(pib.second){
			m_recentValues[1].time = time;
			m_recentValues[1].value = *snapShot;
		}else{
			assert(0);
		}
		bRet = pib.second;
	}

	return bRet;
}

/*
	serialize the value array into the stream, and return number
	of entries written.
	
	Note : it's assumed that the caller supplies a memory mapped
	file, and while the write goes beyond the allocated range, the
	caller is responsible to handle the exception and expand the 
	mapped file.
*/
__uint CHislogStream::flush(
	__u8 * startAddress, 
	hislog_key_t * startTime, 
	hislog_key_t * endTime
	)
{
	__uint i;
	hislog_value_list::iterator it;
	archive_item_t * p = (archive_item_t*)startAddress;
	it = m_values.begin();
	if(startTime){
		*startTime = it->first;
	}
	for(i = m_values.size(); i; i--, it++, p++){
		p->time = it->first;
		p->value = it->second;
		if(i == 1 && endTime){
			*endTime = it->first;
		}
	}
	return m_values.size();
}

void CHislogStream::clear()
{
	m_values.clear();
	// invalidate last cached values
}

__uint CHislogStream::size()
{
	return m_values.size();
}

__uint CHislogStream::getBufferValues(
	hislog_key_t startTime,
	__uint count,
	__uint step,
	hislog_item_t * buffer
	)
{
	hislog_key_t time;
	__uint n;
	
	assert(startTime >= m_parent->m_startTime && startTime <= m_parent->m_endTime);
	
	time = startTime;
	n = 0;
	while(time <= m_parent->m_endTime && n < count){
		getValue(time, buffer);
		time += step;
		buffer++;
		n++;
	}

	// assert(n == (m_parent->m_endTime - startTime) / step + 1);

	return n;
}
