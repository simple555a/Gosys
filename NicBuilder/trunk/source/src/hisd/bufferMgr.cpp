/*
	buffer managerment code
	jhunter, 2004/7/30
*/
//added by Zander 20110527
#pragma warning(disable : 4786)

#include "precomp.h"

/* we keep the recent 8 queried archives opened in memory */
#define CACHE_ENTRIES 8

struct archive_list_entry_t{
	RTK_LIST_ENTRY link;
	CArchiveBuffer * object;
};

typedef map<hislog_key_t, archive_list_entry_t> archive_list_t;

archive_list_t archiveList;
static CRWLock archiveListLock;
static RTK_LIST_ENTRY activeArchiveListHead;

CBufferManager bufferMgr;

static CInMemoryBuffer buffers[2];
static int curBuffer = 0;
static RTDB_DISPATCH_TABLE s_DbEventHandler;


class CFooLock{
public:
	void ReadLock()
	{
	}
	void WriteLock()
	{
	}
	void Release()
	{
	}
};

// seems no point to protect the in-memory buffer list 
// modified by cj @ 2010-2-6
//static CFooLock bufferLock;
static CRWLock bufferLock;

static void _activateArchive(archive_list_entry_t * t);
static void _deActivateArchive();
static __bool addArchive(CArchiveBuffer * b);
static void saveThread(void *ptm);
static __bool openArchiveBuffer(
	class CBufferCursor * cursor,
	hislog_key_t time,
	hislog_key_t end
	);
static __bool openInMemoryBuffer(
	class CBufferCursor * cursor,
	hislog_key_t t1,
	hislog_key_t t2
	);

/*
	insert an archive into the archive list.
	
	Environment :
	the archive list lock must be held already.
*/
static __bool addArchive(CArchiveBuffer * b)
{
	archive_list_t::iterator it;
	pair<archive_list_t::iterator, bool> pr;
	archive_list_entry_t t;
	
	it = archiveList.find(b->m_startTime);
	if(it != archiveList.end()){
		return __false;
	}
	
	t.object = b;
	pr = archiveList.insert(archive_list_t::value_type(b->m_startTime, t));
	if(!pr.second){
		return __false;
	}
	RtkInitializeListHead(&pr.first->second.link);
	return __true;
}

/*
	remove archive files with a timestamp older than 3 moths
	2004-9-7 jackie
*/
static void cropArchives()
{
	archive_list_t::iterator it, next;
	hislog_key_t now;
	archive_list_entry_t * e;
//	ULARGE_INTEGER u1, u2;
	const char * wk = get_working_dir();
	char path[5];

	strncpy(path, wk, 3);
	path[3] = 0;
	//GetDiskFreeSpaceExA(path, &u1, &u2, NULL);
	//if(u1.QuadPart > hislogDiskWaterLevel*1024*1024){
	//	return;
	//}

	// the disk space is below water level, remove archive files
	
	now = hislogGetCurrentTime();
	it = archiveList.begin();
	while(it != archiveList.end()){
		next = it;
		next++;
		if(now - it->first > hislogDay * 86400){
			e = &it->second;
			if(!RtkIsListEmpty(&e->link)){
//				_deActivateArchive(e);
			}
			utils_trace("Removing archive file %s\n", e->object->m_fileName.c_str());
			DeleteFile(e->object->m_fileName.c_str());
			// remove folder
			char path_buffer[_MAX_PATH];
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
		    _splitpath(e->object->m_fileName.c_str(), drive, dir, fname, ext );
		    sprintf(path_buffer, "%s%s", drive, dir);
			_rmdir(path_buffer);
			delete e->object;
			archiveList.erase(it);
			
			//GetDiskFreeSpaceExA(path, &u1, &u2, NULL);
			//if(u1.QuadPart > hislogDiskWaterLevel*1024*1024){
			//	return;
			//}
		}else{
			break;
		}
		it = next;		
	}
}

/*
	write contents of the in-memory archive to disk,
	then convert the in-memory buffer into an
	on-disk archive buffer.
*/
static void saveThread(void *ptm)
{	
	assert(!g_bNoBuffer);
	
	CInMemoryBuffer * theBuffer = (CInMemoryBuffer*)ptm;

	// SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
	
	if(!theBuffer->flushToDisk()){\
		return;
	}

	if(!g_bNoArchive){		
		// ok, convert the in-memory buffer to an archiveList
		CArchiveBuffer * b;
		
		b = new CArchiveBuffer();

		if(!b){
			return;
		}

		if(b->quickInit(theBuffer->m_fileName.c_str())){	
			// SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
			archiveListLock.WriteLock();
			if(!addArchive(b)){
				delete b;
			}
			cropArchives();
			archiveListLock.Release();
		}
	}
}

static void _OnAddTag(PRTK_TAG pTag)
{
	TAG_NAME tn;

	if(!(pTag->s.Flags & TF_SaveToHistory)){
		return;
	}
	
	tn.node = pTag->node;
	tn.sname.group = pTag->group;
	tn.sname.tag = pTag->key;
	
	for(int i=0; i<2; i++){
		buffers[i].addTagSafe(&tn);
	}
}

static void _OnDropTag(PRTK_TAG pTag)
{
	// do nothing for avoid bugs.
	// return;

	TAG_NAME tn;

	if(!(pTag->s.Flags & TF_SaveToHistory)){
		return;
	}
	
	
	tn.node = pTag->node;
	tn.sname.group = pTag->group;
	tn.sname.tag = pTag->key;
	
	for(int i=0; i<2; i++){
		buffers[i].dropTagSafe(&tn);
	}
}

static void _OnModifyTag(PRTK_TAG pTag)
{
	// do nothing for avoid bugs.
	// return;

	TAG_NAME tn;

	tn.node = pTag->node;
	tn.sname.group = pTag->group;
	tn.sname.tag = pTag->key;

	if(pTag->s.Flags & TF_SaveToHistory){
		for(int i=0; i<2; i++){
			buffers[i].addTagSafe(&tn);
		}
	}else{
		for(int i=0; i<2; i++){
			buffers[i].dropTagSafe(&tn);
		}
	}
}

static int UTILS_API _e_file(
	const char * szBaseDir,
	const char * szFullPath,
	const char * szRelativePath,
	const void * pvSearchContext
	)
{
	// insert a node into archiveList
	CArchiveBuffer * b;
	
	b = new CArchiveBuffer();

	if(!b){
		return 1;
	}

	if(!b->quickInit(szFullPath)){
		delete b;
		return 1;
	}
	
	if(!addArchive(b)){
		delete b;
		return 1;
	}

	return 1;
}

/*
	move archive to the head of 
*/
static void _activateArchive(archive_list_entry_t * t)
{
	static CLock lock;

	lock.Lock();
	
	if(!RtkIsListEmpty(&t->link)){
		// if the object is already valid
		RtkRemoveEntryList(&t->link);
	}else{
		// not already active, activate it
		activeArchives++;
	
	}
	
	// move the buffer into the activate buffer list
	RtkInsertHeadList(&activeArchiveListHead, &t->link);

	if(!t->object->isValid())
		// this load will not ensure that the buffer be valid
		// the client should re-check it
		t->object->load();

	lock.UnLock();
}

void _deActivateArchive()
{
// 	if(t->object->isValid()){
// 		t->object->unload();
// 	}
// 	RtkRemoveEntryList(&t->link);
// 	RtkInitializeListHead(&t->link);
// 	activeArchives--;
	archive_list_t::iterator it;
	it = archiveList.begin();
	while (it != archiveList.end())
	{
         if ((&it->second)->object->isValid())
         {
			 (&it->second)->object->unload();
         }		 
	}
}

class CBufferCursor : public CRTKObject
{
public:
	CBufferCursor()
	{
		it = archiveList.end();
		buffer = 0;
		type = 0;
	}

	virtual ~CBufferCursor()
	{
		if(type == 1){
			// delete @ 2007-10-31, chenj
			//archiveListLock.Release();
			buffer = 0;
		}else{
			// delete @ 2007-10-31, chenj
			//bufferLock.Release();
		}
	}

	/*
		move to the buffer later in timestamp than current buffer.
		the algorithm is complicated by the fact that we've two
		types of buffer: in-memory buffers and archive buffers,
		and sometimes we must move from one type into another.
	*/
	void moveNext()
	{
		archive_list_entry_t * t;
		CBuffer * next;
		
		/*
		The two program sentence below figure out the bug 
		that hisq.exe can just show history data for about 40 days , added by Zander Sun on Feburay 2011
		*/
	//	UnmapViewOfFile((PVOID)(&it->second)->object->m_archive);
	//	(&it->second)->object->m_hdr = 0; 
//	if((&it->second)->object->isValid()){
//			(&it->second)->object->unload();
//		}
		//(&it->second)->object->m_archive.UnInit();
	//	(&it->second)->object->unload();
		
		if(type == 0){
			if(!buffer){
				// at the very end already
				return;
			}
			if(buffer == &buffers[0]){
				next = &buffers[1];
			}else if(buffer == &buffers[1]){
				next = &buffers[0];
			}else{
				assert(0);
			}
		}else{
			
			//added by Zander on May 27th 2011
			if((&it->second)->object->isValid())
				(&it->second)->object->unload();
			
			assert(it != archiveList.end());
			it++;
			
			if(it != archiveList.end()){
				t = &it->second;
				//deleted by Zander on May 27th 2011
				//_activateArchive(t);
				next = t->object;
			}else{
				// check in-memory buffer
				// since we don't need the archive list any more,
				// we had better release the lock immediately
				// delete @ 2007-10-31, chenj
				//archiveListLock.Release();

				type = 0;
				
				/* select an in-memory buffer */
				if(buffers[1 - curBuffer].m_startTime){
					next = &buffers[1 - curBuffer];
				}else{
					next = &buffers[curBuffer];
				}
			}
		}
		// note that the time range of the host might
		// been erroneously set, so that the current
		// time is previous to timestamp of some archive 
		// buffers, we must handle this case
		if(next->m_startTime < buffer->m_startTime){
			next = 0;
		}
		buffer = next;
	}

	bool isEnd()
	{
		return buffer? false : true;
	}
	
	archive_list_t::iterator it;
	int type;
	CBuffer * buffer;
};

/*
	get an earliest buffer containing specified data
*/
static __bool openArchiveBuffer(
	CBufferCursor * cursor,
	hislog_key_t time,
	hislog_key_t end
	)
{
	archive_list_entry_t * t;
	pair<archive_list_t::iterator, archive_list_t::iterator> pii;
	archive_list_t::iterator it1;
	archive_list_entry_t * t1, * t2;

	/*
		i don't know how to search for an item in a rb-tree
		matching the criteria that the key is smaller than but
		closest to the specified value, equal_range is, however,
		apparently a quick dirty alternative.
	*/
	pii = archiveList.equal_range(time);

	if(!archiveList.size()){
		return __false;
	}
	
	if(pii.first == archiveList.end()){
		// the specified start time is later than any time-stamp 
		// of all archives, but odd is that the last item still has 
		// data in specified range
		it1 = pii.first;
		if(it1 != archiveList.begin()){
			it1--;
		}
		t = &it1->second;

		if(t->object->intersects(time, end)){
			goto __done;
		}else{
			// run out of buffers
		}
		
		return __false;
	}

	it1 = pii.first;
	if(it1 != archiveList.begin()){
		it1--;
	}
	t1 = &it1->second;
	t2 = &pii.first->second;
	
	if(t1->object->intersects(time, end)){
		cursor->it = it1;
		t = t1;
	}else if(t2->object->intersects(time, end)){
		it1 = pii.first;
		t = t2;
	}else{
		return __false;
	}

__done:

	// ok, now we get an actual buffer, activate it if necessary
    
	//deleted by Zander on May 27th 2011
	//_activateArchive(t);

	// attach the cursor to this buffer
	cursor->it = it1;
	cursor->buffer = t->object;
	cursor->type = 1;
		
	return __true;
}

static __bool openInMemoryBuffer(
	CBufferCursor * cursor,
	hislog_key_t t1,
	hislog_key_t t2
	)
{
	// is it current buffer?
	
	//bufferLock.ReadLock();
	// modified by cj @ 2010-2-6
	//bufferLock.WriteLock();
	
	cursor->type = 0;
	
	if(buffers[0].intersects(t1, t2)){
		if(buffers[1].intersects(t1, t2)){
			if(buffers[0].m_startTime < buffers[1].m_startTime){
				cursor->buffer = &buffers[0];
			}else{
				cursor->buffer = &buffers[1];
			}
		}else{
			cursor->buffer = &buffers[0];
		}
	}else{
		if(buffers[1].intersects(t1, t2)){
			cursor->buffer = &buffers[1];
		}else{
			cursor->buffer = 0;
		}
	}
	
	// modified by cj @ 2010-2-6
	//bufferLock.Release();

	return cursor->buffer? __true : __false;
}


void CBufferManager::init()
{	
	if(!g_bNoBuffer){
		s_DbEventHandler.OnAddTag = _OnAddTag;
		s_DbEventHandler.OnDropTag = _OnDropTag;
		s_DbEventHandler.OnModifyTag = _OnModifyTag;
		hook_rtdb_events(&s_DbEventHandler);

		CInMemoryBuffer * buf = &buffers[0];
		for(int i=0; i<2; i++, buf++){
			buf->clearStreamList();
			//buf->buildStreamList();
		}
	}

	archiveListLock.WriteLock();
	
	if(!g_bNoArchive){
		archive_list_t::iterator it;
		while(archiveList.size()){
			it = archiveList.begin();
			delete it->second.object;
			archiveList.erase(it);
		}
		
		// load archive buffers
		enumerate_files(
			(string(get_working_dir()) + "var/hisd").c_str(),
			"*.hsd",
			_e_file,
			SEARCH_RECURSIVELY | SEARCH_HIDDEN,
			0
			);
	}
	
	activeArchives = 0;
	RtkInitializeListHead(&activeArchiveListHead);

	cropArchives();
	
	archiveListLock.Release();
}

void CBufferManager::uninit()
{
	archiveListLock.WriteLock();
	
	archive_list_t::iterator it;
	while(archiveList.size()){
		it = archiveList.begin();
		delete it->second.object;
		archiveList.erase(it);
	}
	
	activeArchives = 0;
	RtkInitializeListHead(&activeArchiveListHead);
	
	archiveListLock.Release();
}

void CBufferManager::unloadAgedBuffers()
{
// 	RTK_TIME now;
// 	PRTK_LIST_ENTRY entry;
 //	archive_list_entry_t * t;
	
// 	if(activeArchives <= hislogMaxActiveArchive){
// 		return;
// 	}
	
	archiveListLock.WriteLock();
	
// 	rtk_time_mark(&now);
// 	while(activeArchives > hislogMaxActiveArchive){
// 		entry = activeArchiveListHead.Blink;
// 		assert(entry != &activeArchiveListHead);
// 		t = RTK_CONTAINING_RECORD(entry, archive_list_entry_t, link);
		_deActivateArchive();
//	}
	
	archiveListLock.Release();
}

/*
	CBuffer * openBuffer(t1,t2)

	locate a buffer object, either from current in-memory buffer,
	or load it from disk files.

	The buffer must contains data stamped within the specified
	time range. if multiple buffer exists, select the one with the
	smallest m_startTime.
*/
//3 Note the time must be given as multiples of ArchiveTimeSpan
HBUFFER_CURSOR CBufferManager::openBuffer(
	hislog_key_t t1,
	hislog_key_t t2
	)
{
	CBufferCursor * cursor;

	cursor = new CBufferCursor;
	if(!cursor){
		return 0;
	}

	// remove by cj @ 2009-6-2
	//archiveListLock.ReadLock();
	if(openArchiveBuffer(cursor, t1, t2)){
		// leave the list locked
		//archiveListLock.Release();
		return cursor;
	}
	//archiveListLock.Release();

	openInMemoryBuffer(cursor, t1, t2);
	
	return (HBUFFER_CURSOR)cursor;
}

CBuffer * CBufferManager::getBuffer(HBUFFER_CURSOR c)
{
	CBufferCursor * _c = (CBufferCursor*)c;
	return _c->buffer;
}

void CBufferManager::cursorMoveNext(HBUFFER_CURSOR c)
{
	CBufferCursor * _c;
	_c = (CBufferCursor*)c;
	_c->moveNext();
}

bool CBufferManager::cursorIsEnd(HBUFFER_CURSOR c)
{
	CBufferCursor * _c;
	_c = (CBufferCursor*)c;
	return _c->isEnd();
}

//added by Zander 20110527
void CBufferManager::load(HBUFFER_CURSOR c, __uint* pNum)
{
	CBufferCursor * _c;
	_c = (CBufferCursor*)c;
	CBuffer* _buffer;
	_buffer = _c->buffer;
	if (_c->type == 1)
	{
		((CArchiveBuffer*)_buffer)->oFNum = *pNum;//TEST
		if (!((CArchiveBuffer*)_buffer)->isValid())
		{
			((CArchiveBuffer*)_buffer)->load();
		}		
		*pNum = ((CArchiveBuffer*)_buffer)->oFNum;
	}

}

CInMemoryBuffer * CBufferManager::getCurrentBuffer()
{
	CInMemoryBuffer * ret;
 	//bufferLock.ReadLock();
 	// modified by cj @ 2010-2-6
 	// bufferLock.WriteLock();
	ret = &buffers[curBuffer];
 	// modified by cj @ 2010-2-6
	// bufferLock.Release();
	return ret;
}

CInMemoryBuffer * CBufferManager::switchToAlternateBuffer()
{
	int r;
	CInMemoryBuffer * ret;

	// modified by cj @ 2010-2-6
	bufferLock.WriteLock();	
	_beginthread(saveThread, 0, &buffers[curBuffer]);
	r = curBuffer;
	r++;
	r  &= 1;
	curBuffer = r;
	bufferLock.Release();
	
	ret = &buffers[r];

	return ret;
}

// debug shell hooks
/*
void dbgListBuffers(int id)
{
	archive_list_entry_t * a;
	archive_list_t::iterator it;
	int count;

	count = 0;

	if(id == -1){
		printf("list of buffers:\n");
		
		archiveListLock.ReadLock();
		it = archiveList.begin();
		while(it != archiveList.end()){
			a = &it->second;
			printf(
				"%d(%c):%s to %s, %d tags, %d values\n",
				count,
				(PVOID)a->object->m_archive? 'A' : 'U',
				timeAsString(a->object->m_startTime).c_str(),
				timeAsString(a->object->m_endTime).c_str(),
				a->object->m_summary.tagCount,
				a->object->m_summary.valueCount
				);
			count++;
			it++;
		}

		printf(
			"%d archives listed, %d active + %d in-active.\n", 
			archiveList.size(),
			activeArchives, 
			archiveList.size() - activeArchives
			);
		
		archiveListLock.Release();
		
		bufferLock.ReadLock();
		for(int i=0; i<2; i++){
			if(buffers[i].m_startTime){
				printf(
					"%d - %s to %s, memory buffer\n",
					count,
					timeAsString(buffers[i].m_startTime).c_str(),
					timeAsString(buffers[i].m_endTime).c_str()
					);
			}
			count++;
		}
		bufferLock.Release();
	}else{
		//
		printf("List of buffer NO.%d\n", id);
		archiveListLock.ReadLock();
		if(id < archiveList.size()){
			it = archiveList.begin();
			while(id--){
				it++;
			}
			a = &it->second;
			_activateArchive(a);
			a->object->dbgDump(0);
		}else{
			a = 0;
			id -= archiveList.size();
		}
		archiveListLock.Release();

		if(!a && id < 2){
			CBuffer * b;
			bufferLock.ReadLock();
			if(id == 0){
				if(buffers[0].m_startTime){
					b = &buffers[0];
				}else{
					b = &buffers[1];
				}
			}else{
				b = &buffers[1];
			}
			if(b->m_startTime){
				b->dbgDump(0);
			}
			bufferLock.Release();
		}
	}
}
*/

// added by cj @ 2009-6-2
void CBufferManager::listWriteLock()
{
	//archiveListLock.ReadLock();
	archiveListLock.WriteLock();
	bufferLock.WriteLock();
}

void CBufferManager::listUnlock()
{
	archiveListLock.Release();
	bufferLock.Release();
}
