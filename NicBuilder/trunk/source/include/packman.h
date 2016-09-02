/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\include\rtk\packman.h
	file path:	E:\vss\pmc2\src\include\rtk
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Packet manager	
	history:
	created:	10:14:2002   10:08
*********************************************************************/
#include <map>
#include <rtk_list_entry.h>
#include <rtksocket.h>

#pragma warning(disable: 4786 4275)

/*
	a receive item is an object representing the incoming
	data packets as a bunch of fragments. The fragments
	firstly belong to a group, and the groups in turn belong
	to a RECEIVE_ITEM.
*/
struct RECEIVE_ITEM{
	__uint		unreceived_fragments;
	PRTK_PACKET	header;	
	RTK_TIME	birthday;
	double		life;
	__uint		max_frgmt_id_received;
	
	// packet group, a circular linked list
	RTK_LIST_ENTRY	li;
	
	__u8		bitmap[MAX_PACKET_SIZE>>2];

};

struct SEND_ITEM{
	PRTK_PACKET	header;    //第一个分包包头的地址
	RTK_LIST_ENTRY li;
	RTK_TIME	birthday;  //分包的时间
	double		life;
};

// BUGBUG: Why should the CLock be here?
// BUGBUG: actually CPacketMan never use the lock itself
class CPacketMan : public CLock
{
	friend class CRtkVBus;
public:
	static __uint m_mtu;				/* maximum transmission unit */ //可传输的数据包最大容量
	static __uint m_dwEffectiveLoad;	// 可传输的最大数据包data之后的容量
	static __uint m_dwHeaderSize;       // 可传输的最大数据包的包头的容量

public:
	CPacketMan();
	virtual ~CPacketMan();
	
	/* packet reconstruction */
	__bool IsFragment(RTK_PACKET * packet);
	RECEIVE_ITEM * MergePacket(RTK_PACKET * input_packet);
	void DeleteReceiveItem(RECEIVE_ITEM * item);

	/* packet fragmentation */
	__bool IsFragmentationNeeded(const RTK_PACKET * input);
	SEND_ITEM * SplitPacket(const RTK_PACKET *input);
	SEND_ITEM * GetSendItem(const RTK_GUID & Guid, __uint grpId);
	virtual __uint SendPacket(PRTK_PACKET p)=0;
	void FlushQueue();
	
	void reap_packets();
	void dbg_dump();

protected:
	void * Alloc(__uint size);
	void Free(void * mem);
	
private:
	void request_packets(RECEIVE_ITEM * h);
	void	reap_received(PCRTK_TIME);
	void	reap_sent(PCRTK_TIME);	
	
	typedef std::map<RTK_GUID, RTK_LIST_ENTRY> RITEM_LIST;
	typedef std::map<RTK_GUID, RTK_LIST_ENTRY> SITEM_LIST;

	RITEM_LIST	r_lst;
	SITEM_LIST	s_lst;
};

