/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	e:\vss\pmc2\src\include\tagtable.h
	file path:	e:\vss\pmc2\src\include
	author:	Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Real-time database types and procedures	
	history:
	Created:	Mar. 2001
	created:	10:8:2002   14:07
*********************************************************************/
#ifndef __tagtable_h__
#define __tagtable_h__

#include <librtdb.h>

#pragma warning (disable : 4786)

    /* internal layout of real-time system database */
    /*
        GroupTable and TagTable construct the three-tier
        in-memory real-time database structure.
    */
#ifndef RTK_USE_HASHMAP
    #include <map>
    typedef std::map<TAG_KEY, RTK_TAG>   TAG_TABLE;
    typedef std::map<GROUP_KEY, RTK_GROUP> GROUP_TABLE;
    typedef std::map<NODE_KEY, RTK_NODE> NODE_TABLE;	
#else
	#include <hash_map>

	inline size_t rtk_nhash_string(const char* obj, __uint len)
	{
		const char *p=obj;
		char c;
		size_t hash=0;
		while(len-- && *p){
			c=toupper(*p);
			p++;
			hash=hash*3+(hash>>1)+c;
		}
		return hash % 37;
	}

	template<class key> 
		struct _RTK_HASHER
	{
		size_t operator()(const key & n) const
		{
			return rtk_nhash_string(n.Data, sizeof(n.Data));
		}
	};

	template<class _T>
		struct _RTK_EQUALER
	{
		bool operator()(const _T & c1, const _T & c2) const
		{
			return !memcmp(c1.Data, c2.Data, sizeof(c1.Data));
		}
	};

	typedef hash_map
		<TAG_KEY, RTK_TAG, 
		_RTK_HASHER<TAG_KEY>, 
		_RTK_EQUALER<TAG_KEY> 
		>TAG_TABLE;
	typedef hash_map
		<GROUP_KEY, 
		RTK_GROUP, 
		_RTK_HASHER<GROUP_KEY>, 
		_RTK_EQUALER<GROUP_KEY> 
		>GROUP_TABLE;
	typedef hash_map
		<NODE_KEY, 
		RTK_NODE, 
		_RTK_HASHER<NODE_KEY>, 
		_RTK_EQUALER<NODE_KEY> 
		> NODE_TABLE;
#endif	// RTK_NO_HASHMAP

#endif
