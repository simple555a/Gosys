/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	e:\vss\pmc2\src\librtdb\sysdb_object.h
	file path:	e:\vss\pmc2\src\librtdb
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Objects used in realtime database	
	history:
	created:	10:9:2002   10:03
*********************************************************************/
#ifndef __sysdb_object_h__
#define __sysdb_object_h__

#include <assert.h>
#include <librtdb.h>
#include <object.h>

//#define RTK_USE_SGI_STL

#ifdef _MSC_VER
#ifdef RTK_USE_SGI_STL
#define referent_type data_type &
#endif
#define TYPENAME typename         // 增加了typename  Zander 2013-1-23
#else
#define TYPENAME typename
#define referent_type data_type &
#endif

/*
	_CURSOR object acts as a proxy between the outer RTK_CURSOR type 
	and the inner iterator class, for example, between the RTK_CURSOR and 
	TAG_TABLE::iterator. It does this by encapsulating and exposing 
	the smallest necessary interface of the inner class to the
	outside world.

	_CURSOR object automatically manages the reference counting of
	the memory pointed by iterator, note!! this behaviour assumes 
	that it->second	has a data member named refcount.

	proxy_class is a class which derives from _CURSOR;
	outer_class is RTK_CURSOR;
	table_class is a container class which hosts the cursor;
*/
//#ifndef RTK_USE_HASHMAP
//#include <map>
//typedef std::map<key_class, rtk_cursor_class> table_class
//#endif

template<
	class table_class, 
	class outer_class, 
	class proxy_class
	> 
	class _CURSOR
{
public:
	_CURSOR(){
		assigned = __false;
	};
	virtual ~_CURSOR()
	{
		if(assigned){
			unassign();
		}
	}
	inline _CURSOR(const TYPENAME table_class::iterator & i, table_class * t)
	{
		assign(i, t);
	}
	/*
		assign() is the ONLY procedure that can be used to bind a _CURSOR
		object to an internal iterator object!! this must be satisfied
		for the reference couting mechanism to work properly.
	*/
	/*
	*将形参i,t赋给类模板cursor的成员it,tb，并把链表table_class的second的成员，即RTK_NODE\RTK_GROUP\RTK_TAG的成员refcount加1
	*/
	inline void assign(const TYPENAME table_class::iterator & i, table_class * t)
	{
		if(assigned){
			unassign();
		}
		it = i;
		tb = t;
		reference();
		assigned = __true;
	}
	inline void unassign()
	{
		/*
			Possible bug!!
			2003/3/31, Jackie
			seems the memory pointed by it.second has already been freeed, 
			so VC's RTL can detected a 'free block modified after freeing' 
			bug. Potentially harmful.
		*/
		// dereference();
		assigned = __false;
	}
	inline void detach()
	{
		assigned = __false;
	}
	inline __bool is_end()
	{
		return it == tb->end();
	}
	inline __bool is_begin()
	{
		return it == tb->begin();
	}
	inline __bool operator++(int)
	{
		if(it == tb->end()){
			throw *this;
		}
		it++;
		return !is_begin();
	}
	inline __bool operator--(int)
	{
		if(it == tb->end()){
			throw *this;
		}
		it++;
		return !is_end();
	}
	inline __bool move_begin()
	{
		it = tb->begin();
		return !is_end();
	}
	inline __bool move_end()
	{
		it = tb->end();
		return !is_begin();
	}
	table_class * table()
	{
		return tb;
	}
	operator TYPENAME table_class::iterator &()
	{
		return it;
	}
	operator outer_class ()
	{
		if(it == tb->end()){
			return 0;
		}
		return (outer_class)(&((*it).second));
	}
	//RTK_NODE的成员refcount加1？？
	void reference()
	{
#ifdef _WIN32
		__uint * c = &(((*it).second).refcount);
		assert(!IsBadWritePtr(c, 4));
		__asm{
			mov		ebx, c
			mov		eax, 1
			lock	xadd [ebx], eax
			mov		c, eax
		}
		// assert( (__int)c >= 0 );
#endif
	}
	//RTK_NODE的成员refcount减1？？
	void dereference()
	{
#ifdef _WIN32
		__uint *c = &(((*it).second).refcount);
		if(it == tb->end()){
			return;
		}
		__asm{
			mov  ebx, c
			mov  eax, -1
			lock xadd [ebx], eax
			mov  c, eax
		}
		// assert( (__int)c >= 0 );
#endif
	}
	static inline proxy_class * create(
		table_class *table,
		const TYPENAME table_class::key_type &key, 
		const TYPENAME table_class::mapped_type  ref
		)
	{
		std::pair<TYPENAME table_class::iterator, bool> pr;
		pr = table->insert(TYPENAME table_class::value_type(key, ref));
		if( !pr.second ){
			return 0;
		}
		proxy_class * cursor;
		cursor = new proxy_class;
		if(!cursor){
			return 0;
		}
		cursor->assign(pr.first, table);
		return cursor;
	}
	//在table中寻找key对应的值，找到了就new一个proxy_class，并assign这个proxy_class，返回这个proxy_class
	static inline proxy_class * find(
		table_class *table,
		const TYPENAME table_class::key_type & key
		)
	{
		proxy_class * cursor;
		TYPENAME table_class::iterator it;
		it = table->find(key);
		if(it == table->end()){
			return 0;
		}
		cursor = new proxy_class;
		if(!cursor){
			return 0;
		}
		cursor->assign(it, table);
		return cursor;
	}
	static inline outer_class find_item(
		table_class * table,
		const TYPENAME table_class::key_type & key
		)
	{
		TYPENAME table_class::iterator it;
		it = table->find(key);
		if(it == table->end()){
			return 0;
		}
		return (outer_class)&it->second;
	}
	static inline proxy_class * first_item(
		table_class * table
		)
	{
		proxy_class * cursor;
		if(!table->size()){
			return 0;
		}
		cursor = new proxy_class;
		if(!cursor){
			return 0;
		}
		cursor->assign(table->begin(), table);
		return cursor;
	}
	void erase()
	{
		tb->erase(it);
	}
public:
	__uint		cursor_type;
protected:	
	TYPENAME table_class::iterator it;
	table_class			*tb;
	__bool				assigned;
};

#define TAG_HANDLE_TAG ((__uint)('G' << 24) + ('A' << 16) + ('T' << 8) + 'H')                   //"GATH"
#define GRP_HANDLE_TAG ((__uint)('P' << 24) + ('R' << 16) + ('G' << 8) + 'H')                  //"PRGH"
#define NOD_HANDLE_TAG ((__uint)('D' << 24) + ('O' << 16) + ('N' << 8) + 'H')                  //"DONH"

class _HTAG : 
	public _CURSOR<TAG_TABLE, RTK_CURSOR, _HTAG>,
	public CRTKObject
{
public:
	inline _HTAG() { cursor_type = TAG_HANDLE_TAG; }
};

class _HGROUP :
	public _CURSOR<GROUP_TABLE, RTK_CURSOR, _HGROUP>,
	public CRTKObject
{
public:
	inline _HGROUP() { cursor_type = GRP_HANDLE_TAG; }
};

class _HNODE :
	public _CURSOR<NODE_TABLE, RTK_CURSOR, _HNODE>,
	public CRTKObject
{
public:	
	inline _HNODE() { cursor_type = NOD_HANDLE_TAG; }
};

#define is_htag(handle)		(((_HTAG*)(handle))->cursor_type == TAG_HANDLE_TAG)
#define is_hgroup(handle)	(((_HTAG*)(handle))->cursor_type == GRP_HANDLE_TAG)
#define is_hnode(handle)	(((_HTAG*)(handle))->cursor_type == NOD_HANDLE_TAG)

#endif  // __sysdb_object_h__
