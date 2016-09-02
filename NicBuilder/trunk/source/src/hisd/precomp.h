#include <pmc.h>

#include <rpc.h>
#include <objbase.h>
#include <process.h>
#include <direct.h>

#pragma warning(disable:4786)

#include <string>
#include <map>

#pragma warning(disable:4786)

#include <ntservice.h>
#include <rtk_list_entry.h>

#include <object.h>
#include <pmc_hislog.h>

using namespace std;

#pragma warning(disable:4786)

extern __u32 hislogRecordInterval;
extern __u32 hislogRecordColors;
extern __u32 hislogArchiveTimeSpan; 
extern __u32 hislogAutoSaveInterval;
extern __u32 hislogMaxArchiveSize;
extern __u32 hislogMaxActiveArchive;
extern __uint hislogNumQueryThreads;
extern __uint hislogDiskWaterLevel;
extern __uint hislogDay;
extern __uint activeArchives;

extern __bool g_bNoArchive;
extern __bool g_bNoBuffer;

#define ARCHIVE_MAGIC 'ARCV'
#define ARCHIVE_MAGIC_BIG_ENDIAN 'VCRA'

#define ARCHIVER_VERSION 0x0300
	
struct archive_hdr_t{
	__u32 magic;
	__u32 version;
	__u32 tagCount;
	__u32 valueCount;	//3 cached value, can also be computed by walking thru index table
	hislog_key_t startTime;
	hislog_key_t endTime;
	__u32 reserved[32];
}PACKED;

struct archive_index_table_item_t{
	TAG_NAME	tagName;
	__u32		dataOffset;
	__u32		valueCount;
};

struct archive_item_t{
	hislog_key_t	time;
	hislog_item_t	value;
};

extern hislog_key_t hislogGetCurrentTime();
extern hislog_key_t getKeyByTime(FILETIME * tm);
extern FILETIME getTimeByKey(hislog_key_t t);
extern string timeAsString(hislog_key_t t);
extern string valueAsString(hislog_item_t * v);

class CRecorder : public CRtkThread
{
public:
	virtual __uint run();
};

class CHislogService : public CNTService {
public:
	CHislogService();
	virtual void Run(DWORD, LPTSTR *);
	virtual void Stop();

	void uninit();
	void init();
public:
	HANDLE m_EvtConfigChanged;
	HANDLE m_EvtExit;
	HANDLE m_EvtRestart;
};


class CBuffer;

CBuffer * getBufferByTime(hislog_key_t time);

class CHislogStream{
public:
	typedef map<hislog_key_t, hislog_item_t> hislog_value_list;
	hislog_value_list m_values;
	TAG_TYPE m_type;
	TAG_NAME m_name;

	/* for sparse-saving */
	int m_color;
	
	bool m_deletePending;
	
	class CInMemoryBuffer * m_parent;

	/*
		compressing support
	*/
	archive_item_t m_recentValues[2];

	CHislogStream(CInMemoryBuffer * parent);
	
	void clear();
	__bool getValue(hislog_key_t time, hislog_item_t * item);
	__bool putSnapshot(
		hislog_key_t time, 
		const hislog_item_t * item = NULL,
		__bool forceSave = __false
		);
	__uint flush(
		__u8 * startAddress, 
		hislog_key_t * startTime, 
		hislog_key_t * endTime
	);
	__uint size();
	__uint getBufferValues(
		hislog_key_t startTime,
		__uint count,
		__uint step,
		hislog_item_t * buffer
		);
	void _debugDump(int n);	
private:
	inline void linearCompress(
		hislog_key_t time,
		const hislog_item_t * snapShot, 
		__uint compMax, 
		__r8 compDev,
		__bool & bSave,
		__bool & bReplace
		);
};

/*
	CBuffer is the container of CHislogStream

	2004/11/12 CRWLock is now re-implemented using NT's
	file locking mechanism, this is a more robust implementation.
	however, the new approach imposes a one-handle-per-lock
	overhead, which might become a problem when there're
	thousands of buffers.
*/
class CBuffer
{
public:
	hislog_key_t m_startTime;	// inclusive
	hislog_key_t m_endTime;	// inclusive

	CBuffer()
	{
		theLock = 0;
		deleteLockPending = __false;
	}
	
	virtual ~CBuffer()
	{
	}

	__uint valueCount()
	{
		return (__uint)(m_endTime - m_startTime + 1);
	}

	bool contains(hislog_key_t time)
	{
		return time>=m_startTime && time<=m_endTime? true : false;
	}

	bool intersects(hislog_key_t t1, hislog_key_t t2)
	{
		if(t2 < m_startTime){
			return false;
		}
		if(t1 > m_endTime){
			return false;
		}
		return true;
	}
	
	virtual bool isValid() = 0;
	
	virtual __uint getBufferValues(
		PCTAG_NAME tag, 
		hislog_key_t startTime,
		__uint count,
		__uint step,
		hislog_item_t * buffer
		) = 0;

	//virtual int dbgDump(char * buffer) = 0;

	void ReadLock()
	{
		assert(theLock);
		//utils_debug("Rlock %d\n", theLock);
		theLock->ReadLock();
	}

	void WriteLock()
	{
		assert(theLock);
		//utils_debug("Wlock %d\n", theLock);
		theLock->WriteLock();
	}
	
	void Release()
	{
		assert(theLock);
		//utils_debug("Unlock %d\n", theLock);
		theLock->Release();
	}
	
protected:
	__bool deleteLockPending;
	CRWLock * theLock;
};

enum buffer_state_t{
	bs_logging,
	bs_transition,
	bs_ready,
	bs_not_ready
};

/*
	CInMemoryBuffer
	is a class managing tagname and tagvalues, either for
	in-memory or on-disk archive.
*/
class CInMemoryBuffer : public CBuffer
{
public:
	__uint m_tagCount;
	hislog_key_t m_archiveId;
	string m_fileName;
	string m_pathName;
	buffer_state_t m_state;
	
	typedef map<TAG_NAME, CHislogStream *> hislog_stream_list;
	hislog_stream_list m_streams;

	CInMemoryBuffer();
	virtual ~CInMemoryBuffer();
	
	void stopRecord(hislog_key_t now);
	void startRecord(hislog_key_t now);
	void archive(hislog_key_t now, int color);
	
	void buildStreamList();
	void clearStreamList();
	
	bool addTag(PCTAG_NAME);
	bool addTagSafe(PCTAG_NAME);
	bool dropTag(PCTAG_NAME);
	bool dropTagSafe(PCTAG_NAME);

	bool flushToDisk();
	
	virtual bool isValid();
	virtual __uint getBufferValues(
		PCTAG_NAME tag, 
		hislog_key_t startTime,
		__uint count,
		__uint step,
		hislog_item_t * buffer
		);
	//virtual int dbgDump(char * buffer);
};

/*
	layout of archive file:
	1) header
	2) tag-value index, for each tag contained, there is an entry for it indicating
	the offset into the file for the `struct pmc_value_t' array
	3) the PACKED_VALUE arrays for each tag 
*/
class CArchiveBuffer : public CBuffer
{
public:
	CSharedMemory m_archive;
	archive_hdr_t * m_hdr;
	archive_index_table_item_t * m_indices;
	string m_fileName;
// quick inited variables
	struct {
		__uint tagCount;
		__uint valueCount;
	}m_summary;

	//TEST 2012-8-15
    __uint oFNum;

	CArchiveBuffer();
	virtual ~CArchiveBuffer();	
	bool load(const char * fileName);
	bool load();
	void unload();
	bool quickInit(const char * fileName);
	
	virtual bool isValid();
	virtual __uint getBufferValues(
		PCTAG_NAME tag, 
		hislog_key_t startTime,
		__uint count,
		__uint step,
		hislog_item_t * buffer
		);
	//virtual int dbgDump(char * buffer);
};

typedef void * HBUFFER_CURSOR;

// a singleton class managing all buffers
class CBufferManager
{
public:

	static void init();
	static void uninit();
	static CInMemoryBuffer * getCurrentBuffer();
	static CInMemoryBuffer * switchToAlternateBuffer();
	static void unloadAgedBuffers();
	
	static HBUFFER_CURSOR openBuffer(
		hislog_key_t time,
		hislog_key_t span
		);
	static CBuffer * getBuffer(HBUFFER_CURSOR c);
	static void cursorMoveNext(HBUFFER_CURSOR c);
	static bool cursorIsEnd(HBUFFER_CURSOR c);

	static void listWriteLock();
	static void listUnlock();

	static void load(HBUFFER_CURSOR c, __uint* pNum);
	
};

extern CBufferManager bufferMgr;

void initQueryProcessor();
void uninitQueryProcessor();

void initDebugShell();
void uninitDebugShell();

/*
	returns the floor value of an rational number specified
	by T/mode.
*/
inline hislog_key_t rFloor(hislog_key_t T, hislog_key_t mode)
{
	return T / mode;
}

/*
	returns the ceil value of an rational number specified
	by T/mode
*/
inline hislog_key_t rCeil(hislog_key_t T, hislog_key_t mode)
{
	return (T + mode - 1) / mode;
}

inline hislog_key_t commModeFloor(
	hislog_key_t t, 
	hislog_key_t T, 
	hislog_key_t mode
	)
{
	return t + rFloor(T - t, mode) * mode;
}

inline hislog_key_t commModeCeil(
	hislog_key_t t, 
	hislog_key_t T, 
	hislog_key_t mode
	)
{
	return t + rCeil(T - t, mode) * mode;
}

void interpolate(
	const hislog_item_t * v1, 
	const hislog_item_t * v2, 
	hislog_item_t * v, 
	const hislog_key_t t1,
	const hislog_key_t t2,
	const hislog_key_t t
	);

extern CRecorder * g_recorder;
extern CHislogService * g_service;

