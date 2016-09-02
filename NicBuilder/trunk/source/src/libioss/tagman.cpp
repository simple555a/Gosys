#include "precomp.h"

/*
功能：开辟一片内存，初始化CChangedTagManager的成员
参数：
      maxitems     组所包含的标签数，即需更新的标签数
	  gk           组名指针
返回：0     失败     开辟内存操作失败
      1     成功  
*/
__bool CChangedTagManager::init(__uint maxitems, PCGROUP_KEY gk)
{
	//1 分配buffer内存
	if(this->maxitems < maxitems){
		buffer = realloc(
			buffer, 
			maxitems * sizeof(CHANGED_DATA_ENTRY) + 
			sizeof(RTK_PACKET) - 
			sizeof(packet->data) +
			sizeof(CHANGED_DATA)
			);
		if(!buffer){
			cds = 0;
			return __false;
		}
	}

	//2 初始化  更新数据组指针  更新数数据标签指针  数据报指针  最大更新数？？
	//  以上组成一个连续的内存结构buffer，即由第1步分配所得
	cds = (CHANGED_DATA*)
		((char*)buffer + sizeof(RTK_PACKET) - sizeof(packet->data));
	cds->group = *gk;
	cds->count = 0;
	entries = (CHANGED_DATA_ENTRY*)&cds[1];
	packet = (RTK_PACKET*)buffer;
	this->maxitems = maxitems;
	return __true;
}

/*
函数功能：将更新的实时值信息存储在类成员数组entries[]中，并cds->count+1
参数说明：
          gk      组名指针
		  tte     实时值数据结构
		  tv      更新的实时值
返 回 值：无
*/
void CChangedTagManager::add_tag(
	PCGROUP_KEY gk, 
	PCRTK_TAG tte, 
	PCpmc_value_t tv
	)
{
	//1 buffer未分配内存，则返回
	if(!buffer){
		return;
	}

	//2 存储的新实时值总数不能溢出
	__uint items = cds->count;
	if(items>=maxitems){
		return;
	}

	//3 将更新的实时值赋给更新数组entries[],更新实时值的数量加1
	entries[items].name	 = tte->key;
	entries[items].value = *tv;
	cds->count++;
}

/*
功能：将一个组的标签更新的实时值数据报广播出去
参数：无
返回：0       开辟数据报内存失败
          或  该组没有标签
		  或  发送数据报失败
	  1      成功
*/
__bool CChangedTagManager::broadcast()
{
	__uint byteCount;
	if(!buffer){
		return __false;
	}
	if(!cds->count){
		return __false;
	}
	packet->guid.create();
	mk_bcast_addr(&packet->dest.host);
	packet->dest.port = PORT_ALL_CLIENT;
	packet->packet_type = PT_ChangedData;
	packet->data_size = cds->count*sizeof(CHANGED_DATA_ENTRY) 
		+ sizeof(CHANGED_DATA);
	packet->offset = 0;	
	packet->frgmt_id = 0;	
	packet->total_frag = 1;
	packet->grp_id = 0;
	byteCount = send_rtk_packet(vbus_rtdb, packet);//发送，返回发送的字节数
	return byteCount >= 0? __true : __false;
}

/*
功能：释放标签实时值更新的数据报内存
参数：无
返回：无
*/
void CChangedTagManager::destroy()
{
	if(buffer){
		free(buffer);
		buffer   = 0;
		entries  = 0;
		maxitems = 0;
		cds      = 0;
	}
}


