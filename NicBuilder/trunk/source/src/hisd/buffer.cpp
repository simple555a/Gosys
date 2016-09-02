//added by Zander 20110527
#pragma warning(disable : 4786)

#include "precomp.h"

CInMemoryBuffer::CInMemoryBuffer()
{
	m_state = bs_ready;
	theLock = new CRWLock;
}

CInMemoryBuffer::~CInMemoryBuffer()
{
	clearStreamList();
	delete theLock;
	theLock = 0;
}

void CInMemoryBuffer::startRecord(hislog_key_t now)
{
	SYSTEMTIME st;
	FILETIME ft;
	char buf[_MAX_PATH];
	hislog_stream_list::iterator it, next;
	CHislogStream * str;
	
	//utils_debug("wlock 1\n");
	WriteLock();
	
	if(m_state != bs_ready){
		//utils_debug("release 1\n");
		Release();
		throw(m_state);
	}

	ft = getTimeByKey(now);
	FileTimeToSystemTime(&ft, &st);
	_snprintf(
		buf,
		sizeof(buf),
		"%svar/hisd/%04d-%02d-%02d", 
		get_working_dir(),
		st.wYear,
		st.wMonth,
		st.wDay
		);
	m_pathName = buf;
	
	_snprintf(
		buf,
		sizeof(buf),
		"%svar/hisd/%04d-%02d-%02d/%02d-%02d-%02d.hsd", 
		get_working_dir(),
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond
		);
	m_fileName = buf;
#if 1
	{
		DEBUG_PRINTF((
			"Recording started on %s\n",
			timeAsString(now).c_str()
			));
	}
#endif

	m_startTime = now;
	m_endTime = now;

	// handle postponed deletion
	it = m_streams.begin();
	while(it != m_streams.end()){
		next = it;
		next++;
		str = it->second;
		if(str->m_deletePending){
			delete str;
			m_streams.erase(it);
		}else{
			str->clear();
		}
		it = next;
	}
	
	m_state = bs_logging;
	
	//utils_debug("release 2\n");
	Release();
}

void CInMemoryBuffer::stopRecord(hislog_key_t now)
{
	__uint size;
	hislog_stream_list::iterator it;

#if 1
	{
		DEBUG_PRINTF((
			"Recording stopped on %s\n",
			timeAsString(now).c_str()
			));
	}
#endif

	//utils_debug("wlock 2\n");
	WriteLock();
	
	if(m_state != bs_logging){
		//utils_debug("release 3\n");
		Release();
		throw(m_state);
	}
	
	if(!lock_rtdb(__false, 100)){
		//utils_debug("release 4\n");
		Release();
		return;
	}
	
	for(size = m_streams.size(), it = m_streams.begin(); size; it++, size--){
		it->second->putSnapshot(now, NULL, __true);
	}

	m_state = bs_ready;
	
	unlock_rtdb();

	//utils_debug("release 5\n");
	Release();
}

/*
	determine which tags should be saved to history database.
*/
void CInMemoryBuffer::buildStreamList()
{
	RTK_CURSOR hNode;
	RTK_CURSOR hGroup;
	RTK_CURSOR hTag;
	PCRTK_TAG  pTag;
	TAG_NAME tn;
	char nodeName[rtkm_node_key_length + 1];
	
	GetPrivateProfileString(
		"PMC",
		"ServerName",
		"LocalServer",
		nodeName,
		sizeof(nodeName),
		get_config_file()
		);
	CNodeName nodeKey(nodeName);

	//utils_debug("wlock 3\n");
	WriteLock();
	
	if(!lock_rtdb(__false, 100)){
		//utils_debug("release 6\n");
		Release();
		return;
	}
	
	// clear list
	clearStreamList();
	
	hNode = open_node(nodeKey);
	if(hNode){
		hGroup = cursor_open_first_subitem(hNode);
		while(!cursor_is_end(hGroup)){
			hTag = cursor_open_first_subitem(hGroup);
			while(!cursor_is_end(hTag)){
				pTag = (PCRTK_TAG)cursor_get_item(hTag);
				if(pTag->s.Flags & TF_SaveToHistory){
					tn.node = pTag->node;
					tn.sname.group = pTag->group;
					tn.sname.tag = pTag->key;
					addTag(&tn);
				}
				cursor_move_next(hTag);
			}
			close_handle(hTag);
			cursor_move_next(hGroup);
		}
		close_handle(hGroup);
	}

	unlock_rtdb();
	
	//utils_debug("release 7\n");
	Release();
}

bool CInMemoryBuffer::addTagSafe(PCTAG_NAME tn)
{
	bool ret;

	//utils_debug("wlock 4\n");
	WriteLock();
	if(!lock_rtdb(__false, 100)){
		//utils_debug("release 8\n");
		Release();
		return __false;
	}
	ret = addTag(tn);
	unlock_rtdb();

	//utils_debug("release 9\n");
	Release();
	return ret;
}

/*
	addTag()
	insert a tag into the logging stream list.
	caller must have rtdb locked.
*/
bool CInMemoryBuffer::addTag(PCTAG_NAME name)
{
	CHislogStream * str;
	hislog_stream_list::iterator it;
	pair<hislog_stream_list::iterator,bool> pib;
	PCRTK_TAG p;
	
	it = m_streams.find(*name);
	if(it != m_streams.end()){
		// already logged, reset the pending-deletion flag
		it->second->m_deletePending = false;
		return true;
	}

	p = query_tag_f(name);
	if(!p){
		return false;
	}
	
	str = new CHislogStream(this);
	if(!str){
		return false;
	}

	str->m_type = get_value_type(p->s.Flags);
	str->m_name = *name;
	str->m_color = rng_rand(0, hislogRecordColors);
	
	pib = m_streams.insert(hislog_stream_list::value_type(*name, str));
	if(!pib.second){
		delete str;
		return false;
	}

	return true;
}

/*
	addTag()
	remove a tag from the logging stream list.
	caller must have rtdb locked.
*/
bool CInMemoryBuffer::dropTag(PCTAG_NAME name)
{
	CHislogStream * str;
	hislog_stream_list::iterator it;
	
	it = m_streams.find(*name);
	if(it == m_streams.end()){
		return true;
	}
	str = it->second;
	str->m_deletePending = true;
	
	return true;
}

bool CInMemoryBuffer::dropTagSafe(PCTAG_NAME tn)
{
	bool ret;
	
	//utils_debug("wlock 5\n");
	WriteLock();
	if(!lock_rtdb(__false, 100)){
		Release();
		return __false;
	}
	ret = dropTag(tn);
	unlock_rtdb();

	//utils_debug("release 10\n");
	Release();
	
	return ret;
}

void CInMemoryBuffer::archive(hislog_key_t now, int color)
{
	__uint size;
	hislog_stream_list::iterator it;
	CHislogStream * str;

	if(now < m_endTime){
		return;
	}

	//utils_debug("wlock 6\n");
	WriteLock();

	m_endTime = now;
	
	if(m_state != bs_logging){
		//utils_debug("release 11\n");
		Release();
		return;
	}
	
	if(!lock_rtdb(__false, 100)){
		//utils_debug("release 12\n");
		Release();
		return;
	}
	
	for(size = m_streams.size(), it = m_streams.begin(); size; it++, size--){
		str = it->second;
		// if(str->m_color == color)
		{
			it->second->putSnapshot(now, NULL, false);
		}
	}

	unlock_rtdb();

	//utils_debug("release 13\n");
	Release();
}

void CInMemoryBuffer::clearStreamList()
{
	// clear list
	int size;
	hislog_stream_list::iterator it;
	it = m_streams.begin();
	for(size = m_streams.size(); size; size--){
		delete it->second;
		it++;
	} 
	m_streams.clear();
}

bool CInMemoryBuffer::isValid()
{
	return true;
}

/*
	getBufferValues()

	return values contained.

	Environment :

	Must have the tag-list locked, preferrably read-locked.
*/
__uint CInMemoryBuffer::getBufferValues(
	PCTAG_NAME tag, 
	hislog_key_t startTime,
	__uint count,
	__uint step,
	hislog_item_t * buffer
	)
{
	__uint retVal = 0;
	hislog_stream_list::iterator it;
	CHislogStream * str;
	hislog_key_t endTime;

	assert(contains(startTime));
	endTime = startTime + step * (count - 1);
	if(endTime > m_endTime){
		endTime = m_endTime;
	}
	
	it = m_streams.find(*tag);
	if(it != m_streams.end()){
		str = it->second;
		retVal = str->getBufferValues(
			startTime,
			count,
			step,
			buffer
			);
	}else{
		retVal = (__uint)((endTime - startTime)/step + 1);
		memset(buffer, 0, sizeof(hislog_item_t)*retVal);
	}

	return retVal;
}

/*
	save archive into disk files.
*/
bool CInMemoryBuffer::flushToDisk()
{
	archive_hdr_t * hdr;
	archive_index_table_item_t * index;
	archive_item_t * itemPointer;
	//hislog_key_t * tm = &startTime;
	
	__uint i;
	hislog_stream_list::iterator it;
	__uint valueCount;
	__uint fileSize;
	CSharedMemory shm;

	//utils_debug("flush to disk begin - %s\n", m_fileName.c_str());

	//utils_debug("rlock 7\n");
	//ReadLock();
	WriteLock();

	if(m_state != bs_ready){
		//utils_debug("release 14\n");
		Release();
		throw(m_state);
	}
	
	// estimate size needed
	i = m_streams.size();
	it = m_streams.begin();
	valueCount = 0;
	for(; i; i--, it++){
		valueCount += it->second->m_values.size();
	}
	if(!valueCount){
		//utils_debug("release 15\n");
		Release();
		return false;
	}

	fileSize  = sizeof(archive_hdr_t);
	fileSize += m_streams.size() * sizeof(archive_index_table_item_t);
	fileSize += valueCount * sizeof(archive_item_t);	

	_mkdir(m_pathName.c_str());
	shm.StaticInit(
		m_fileName.c_str(),
		0,
		fileSize,
		NULL,
		OPEN_ALWAYS,
		FILE_SHARE_READ,
		GENERIC_READ | GENERIC_WRITE
		);
	if(!(PVOID)shm){
		//utils_debug("release 16\n");
		Release();
		return false;
	}

	// relocate pointers
	hdr = (archive_hdr_t*)((PVOID)shm);
	index = (archive_index_table_item_t*)(hdr + 1);
	itemPointer = (archive_item_t*)(index + m_streams.size());
	
	memset(hdr, 0, sizeof(hdr));
	hdr->version = ARCHIVER_VERSION;
	hdr->tagCount = m_streams.size();
	hdr->valueCount = valueCount;
	hdr->startTime = m_startTime;
	hdr->endTime = m_endTime;
	
	i = m_streams.size();
	it = m_streams.begin();
	for(; i; i--, it++){
		valueCount = it->second->flush((__u8*)itemPointer, NULL, NULL);
		index->dataOffset = (__u8*)itemPointer - (__u8*)hdr;
		index->tagName = it->second->m_name;
		index->valueCount = valueCount;
		itemPointer += valueCount;
		index++;
	}

	assert((__u8*)itemPointer == (__u8*)hdr + fileSize);
	
	// ok, this last write will mark the file as a valid archive
	hdr->magic = ARCHIVE_MAGIC;

	m_state = bs_ready;
	
	//utils_debug("release 17\n");
	Release();
	
	shm.UnInit();
	
	//utils_debug("flush to disk end - %s\n", m_fileName.c_str());

	return true;
}

/*
int CInMemoryBuffer::dbgDump(char * buffer)
{
	hislog_stream_list::iterator it;
	PCTAG_NAME tn;
	CHislogStream * str;
	
	ReadLock();
	it = m_streams.begin();
	while(it != m_streams.end()){
		str = it->second;
		tn = &it->first;
		printf(
			"tag %s.%s.%s \n---------------------------\n",
			(char*)CNodeName(tn->node),
			(char*)CGroupName(tn->sname.group),
			(char*)CTagName(tn->sname.tag)
			);		
		str->_debugDump(-1);
		it++;
	}
	Release();
	return 0;	
}
*/
