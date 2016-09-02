/*
	archive management code
	JHunter, 2004/7/29
*/
#include "precomp.h"

static int __cdecl _compareTagName(const void * s, const void * d) 
{
	int ret;
#define _s ((PTAG_NAME)s)
#define _d ((PTAG_NAME)d)
	ret = rtk_compare_key(_s->node, _d->node);
	if(ret){
		return ret;
	}
	ret = rtk_compare_key(_s->sname.group, _d->sname.group);
	if(ret){
		return ret;
	}
	return rtk_compare_key(_s->sname.tag, _d->sname.tag);
}

/*
	find the first archive value with a timestamp of the
	smallest difference with the specified time and still
	previous to the specified time.

	Returns :
	
	NULL if not found (time out of range)
*/
archive_item_t * _locateItem(archive_item_t * lo, archive_item_t * hi, hislog_key_t key)
{
	archive_item_t *mid;
	unsigned int half;
	__uint num = hi - lo + 1;

	while (num > 2){
		half = num / 2;
		assert(lo->time <= hi->time);
		mid = lo + (num & 1 ? half : (half - 1));
		if (key == mid->time){
			return(mid);
		}else if (key < mid->time){
			hi = mid - 1;
			if(hi->time <= key){
				return hi;
			}
			num = num & 1 ? half : half-1;
		}else{
			lo = mid + 1;
			if(lo->time > key){
				return mid;
			}
			num = half;
		}
	}

	return lo;
}

CArchiveBuffer::CArchiveBuffer() : CBuffer()
{
	m_hdr = 0;
}

CArchiveBuffer::~CArchiveBuffer()
{
	unload();
}

bool CArchiveBuffer::load()
{
	return load(m_fileName.c_str());
}

/*
	find the archived designated by the time,
	load and map it into memory, so that we
	can access through normal pointer dereferences.

	note this will put the archive buffer into a usable state,
	so we initialize the internal RWLock object here.
*/
bool CArchiveBuffer::load(const char * fileName)
{
	__uint fileSize;
	bool ret = false;
	
	m_archive.UnInit();
	
	if(!theLock){
		theLock = new CRWLock;
	}
	
	if(!theLock){
		utils_error("cannot alloc lock for %s\n",fileName);
		goto __done;
	}
	
	m_archive.oFNum = oFNum;
	m_archive.StaticInit(
		fileName
		);
	oFNum = m_archive.oFNum;
	

	if(!(PVOID)m_archive){
		goto __done;
	}

	m_hdr = (archive_hdr_t*)((PVOID)m_archive);
	if(m_hdr->magic != ARCHIVE_MAGIC){
		goto __done;
	}
	
	fileSize  = sizeof(archive_hdr_t);
	fileSize += m_hdr->valueCount * sizeof(archive_item_t);
	fileSize += m_hdr->tagCount * sizeof(archive_index_table_item_t);
	
	if(m_hdr->version > ARCHIVER_VERSION){
		utils_error(
			"Warning : higher version(0x%08x) archive encountered.\n", 
			m_hdr->version
			);
		// return false;
	}
	
	if(m_archive.FileSize != fileSize){
		utils_error(
			"Warning : invalid archive encountered, size mismatch : %d, %d, 0x%08x, 0x%08x.\n", 
			m_hdr->tagCount,
			m_hdr->valueCount,
			m_archive.FileSize,
			fileSize
			);
		utils_error(
			"hdr:%d, item:%d, index%d.\n",
			sizeof(archive_hdr_t),
			sizeof(archive_item_t),
			sizeof(archive_index_table_item_t)
			);
		goto __done;
	}

	m_startTime = m_hdr->startTime;
	m_endTime = m_hdr->endTime;
	
	/*
		ok, now relocate member pointers
	*/
	m_indices = (archive_index_table_item_t*)(m_hdr + 1);
	ret = true;
__done:
	if(!ret){
		if(theLock){
			delete theLock;
			theLock = 0;
		}
		m_hdr = 0;
	}

	if(!ret)
		utils_error("Loading %s failed.\n",fileName);
	return ret;
}

/*
	getBufferValues

	Environment :

	buffer lock acquired at least non-exclusively

	Algorithm description:

	on entry, we've got three time ranges

	a) time range for which this buffer has archive data
	b) time range that client demands archive data
	c) time range of the tag data array
	
	we denote them as Ta, Tb, Tc, respectively. 

	Ta and Tb must intersect, or else there is a bug(should 
	not go here).

	Tb and Tc might intersect, in which case there will
	be some archived data returned. Or they might not
	intersect, in which case there'll be no archived data
	returned.

	Either case, the number of tag data returned is determined
	by the intersection length of Tc and Tb and irrelavant of
	time range Ta. 

	Time range Ta only serves as a hint to find a tag's
	archive data by timestamp.
*/
__uint CArchiveBuffer::getBufferValues(
	PCTAG_NAME tag, 
	hislog_key_t startTime,
	__uint count,
	__uint step,
	hislog_item_t * buffer
	)
{
	__uint ret;
	archive_index_table_item_t * ti;
	archive_item_t * values;
	archive_item_t * item, * margin;
	hislog_key_t endTime;
	hislog_key_t Tc_1, Tc_2;
	__uint n;
	
	assert(isValid());

	endTime = startTime + (count - 1) * step;
	
	__try{
		ti = (archive_index_table_item_t*)bsearch(
			tag, 
			m_indices, 
			m_hdr->tagCount, 
			sizeof(archive_index_table_item_t), 
			_compareTagName
			);
		if(!ti){
			return 0;
		}
		if(ti->valueCount < 2){
			return 0;
		}
		values = (archive_item_t *)((__u8*)m_archive.GetPointer() + ti->dataOffset);
		//3 now we gonna get the timed value from this array
		//3 A quich and dirty implementation
		ret = 0;

		// align them to grid points
		Tc_1 = commModeCeil(startTime, values[0].time, step);
		Tc_2 = commModeFloor(startTime, values[ti->valueCount - 1].time, step);
		
		// compute the intersection of range [Tc_1, Tc_2] and [startTime, endTime]
		Tc_1 = Tc_1 > startTime? Tc_1 : startTime;
		Tc_2 = Tc_2 > endTime? endTime : Tc_2;
		
		if(Tc_1 > Tc_2){
			// no intersection
			return 0;
		}

		if(Tc_1 < m_startTime || Tc_2 > m_endTime){
			// this is an invalid archive, we don't proceed for fear
			// that unexpected result might occur
			rtk_set_last_error(HISLOG_INVALID_ARCHIVE);
			return 0;
		}

		assert(!((Tc_1 - startTime) % step));
		assert(!((Tc_2 - startTime) % step));
		
		// prolog
		if(Tc_1 > startTime){
			// make compensation
			n = (__uint)((Tc_1 - startTime) / step);
			ret += n;
			startTime += n * step;
			buffer += n;
		}

		item = values;
		margin = &values[ti->valueCount - 1];
		
		// actual values in the archive
		do{
			item = _locateItem(item, margin, startTime);
			assert(item);
			assert(item->time <= startTime);
			// get value for startTime
			if(item->time == startTime || item == margin){
				*buffer = item->value;
			}else{			
				// interpolate
				interpolate(
					&item->value, 
					&item[1].value,
					buffer, 
					item->time,
					item[1].time,
					startTime
					);
			}
			buffer++;
			ret++;
			startTime += step;
		}while(startTime <= Tc_2 && ret < count);
	}__except(EXCEPTION_EXECUTE_HANDLER){
		rtk_set_last_error(HISLOG_INVALID_ARCHIVE);
		return 0;
	}

	return ret;
}

void CArchiveBuffer::unload()
{
	m_archive.UnInit();
	m_hdr = 0;
	if(theLock){
		delete theLock;
		theLock = 0;
	}
}

bool CArchiveBuffer::isValid()
{
	return m_hdr==NULL? false : true;
}

/*
	gather enough information from the on-disk file,
	while keep allocating large chunk of memory until
	some client is requesting data from the archive.
*/
bool CArchiveBuffer::quickInit(const char * fileName)
{
	m_fileName = fileName;
	m_archive.StaticInit(fileName, 0, sizeof(archive_hdr_t));
	if(!m_archive.GetPointer()){
		return false;
	}
	m_hdr = (archive_hdr_t*)m_archive.GetPointer();
	if(m_hdr->magic != ARCHIVE_MAGIC && m_hdr->magic != ARCHIVE_MAGIC_BIG_ENDIAN){
		unload();
		return false;
	}
	if(m_hdr->version > ARCHIVER_VERSION){
		unload();
		return false;
	}
	m_startTime = m_hdr->startTime;
	m_endTime = m_hdr->endTime;
	m_summary.tagCount = m_hdr->tagCount;
	m_summary.valueCount = m_hdr->valueCount;
	unload();
	return true;
}

/*
int CArchiveBuffer::dbgDump(char * buffer)
{
	__uint i, j;
	archive_index_table_item_t * ti;
	archive_item_t * values;
	
	ReadLock();
	ti = m_indices;
	try{
		for(i=0; i<m_hdr->tagCount; i++, ti++){
			printf(
				"tag %s.%s.%s \n---------------------------\n",
				(char*)CNodeName(ti->tagName.node),
				(char*)CGroupName(ti->tagName.sname.group),
				(char*)CTagName(ti->tagName.sname.tag)
				);
			values = (archive_item_t*)(ti->dataOffset + (__u8*)m_hdr);
			for(j=0; j<ti->valueCount; j++, values++){
				printf(
					"%s [0x%I64x] -- %s\n",
					timeAsString(values->time).c_str(),
					values->time,
					valueAsString(&values->value).c_str()
					);
			}
		}
	}catch(...){
	}
	Release();

	return 0;
}
*/
