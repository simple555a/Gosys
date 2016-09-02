/*
	Database refresher
	2003/5/22 master/slave supported added

	REvision:
	2003/7/20, Jackie, found bug : tag is not invalidated immediately
	after failure in updation, corrected.
*/
#include "precomp.h"

static RTDB_DISPATCH_TABLE s_Handler;
static int	last_phase;

CDBRefresher g_Refresher;

CDBRefresher::CDBRefresher()
{
	m_dwPeriod = rtkm_refresh_quantum;
	m_bMsgLoops = __false;	
}

CDBRefresher::~CDBRefresher()
{
}

/*
函数功能：将本地节点下的所有组添加到g_Refresher.rlist中，所有标签点关联到各自的设备驱动中
参数说明： 无
返回值：  无
*/
void CDBRefresher::post_db_init()
{
	RTK_CURSOR handle;
	PRTK_GROUP p;
    //找到本地节点的游标CURSOR
	handle = cursor_open_first_subitem(HNODE_LOCAL_MACHINE);
	//遍历本地节点下的所有组，用其更新内存中的组信息
	while(!cursor_is_end(handle)){
		p = (PRTK_GROUP)cursor_get_item(handle);
		OnAddGroup(p);
		RTK_CURSOR hTag;
		PRTK_TAG   pTag;
		hTag = cursor_open_first_subitem(handle);
		//遍历组中的所有标签点,
		while(pTag = (PRTK_TAG)cursor_get_item(hTag)){
			OnAddTag(pTag);
			cursor_move_next(hTag);
		}
		close_handle(hTag);
		cursor_move_next(handle);
	}
	close_handle(handle);
}
/*
函数功能：add本地节点、组和标签；对g_Handlers[]中的函数指针赋值；开启一个线程。即初始化数据刷新过程
参数说明：无
返回值：  0 __false      锁定实时库内存失败
                     或  开启线程失败
		  1 __true       初始化成功
*/
__bool CDBRefresher::init()
{
	if(!lock_rtdb(__false, 1000)){
		return __false;
	}
    
	//1 将所有标签关联到设备驱动
	post_db_init();
    
	//2 RTDB_DISPATCH_TABLE中的函数指针赋值
	s_Handler.OnAddGroup = OnAddGroup;
	s_Handler.OnDropGroup = OnDropGroup;
	s_Handler.OnAddTag = OnAddTag;
	s_Handler.OnModifyTag = OnAddTag;
	s_Handler.OnModifyGroup = OnEditGroup;
	s_Handler.OnDropTag	= OnDropTag;
    
	//3 将实时库分派结构s_Handler添加到librtdb.dll的实时库分派结构的数组中
	hook_rtdb_events(&s_Handler);
    
	//4 开启DB Refresher线程
	__bool ret = g_Refresher.start();
	utils_trace(
		"DB Refresher started(0x%08x)...%s\n", 
		g_Refresher.id(), 
		ret? "Ok" : "Failed."
		);
	unlock_rtdb();
	
	return ret;
}

/*
函数功能：注销数据刷新过程--删除实时库分派结构、关闭线程、清空类CDBRefresher中的组信息列表
参数说明：无
返 回 值：只能返回__true  1
          没有错误值返回
*/
__bool CDBRefresher::uninit()
{
	//1 从实时库分派结构数组中删除s_Handler
	unhook_rtdb_events(&s_Handler);
	//2 关闭DB refresher线程
	if (g_Refresher.stop(5000)){
		utils_trace("DB refresher stopped.\n");
	}else{
		utils_trace("DB refresher stopping timeout.\n");
	}
	//3 清空组信息列表
	g_Refresher.rlist.clear();	
	return __true;
}

/*
	update a tag's value in database according the
	recently acquisited value(by device driver), and
	queue the tag and its value into the tag manager's
	broadcast list if necessary.
*/
/*
函数功能：更新实时值的报警信息和动态属性；发送报警数据报；将更新的实时值记录在CChangedTagManager中
参数说明：
          gk      更新的实时值标签所在的组名
		  tte     实时值结构体
		  tv      从设备得到的最新实时值
		  now     时间
返 回 值：无
*/
void CDBRefresher::check_exception(
	const GROUP_KEY *gk, 
	PRTK_TAG tte, 
	pmc_value_t *tv, 
	PCRTK_TIME now
	)
{
	//1 根据实时值tv，得到报警状态信息，并广播到网络中
	get_exception(gk, tte, tv, now);
    
	//2 更新标签的实时动态属性
	// 更新动态值
	tte->d.Value.Value = tv->Value;
	// 更新动态标识
	tte->d.Value.Flags &= ~TF_Valid;
	tte->d.Value.Flags |= (tv->Flags & TF_Valid);

	if(tv->Flags & TF_Valid){
		// clear the first-unavailable flag
		tte->d.Value.Flags &= ~TF_Shutdown;
	}
	// 更新动态时间
	tte->d.Time = *now;
	tv->Flags = tte->d.Value.Flags;
    // 更新广播的时间和值
	if(tte->d.Value.Flags & (TF_Rate | TF_Expired)){
		tte->d.BroadcastTime = *now;
		tte->d.BroadcastedValue = tte->d.Value.Value;
#if 0
		if(tte->s.Type == DT_Real4){
			DEBUG_PRINTF((
				"%s.%s=%f.\n", 
				(char*)CGroupName(*gk),
				(char*)CTagName(tte->key),
				tv->Value.fltValue
				));
		}else{
			DEBUG_PRINTF((
				"%s.%s=%s.\n", 
				(char*)CGroupName(*gk),
				(char*)CTagName(tte->key),
				tv->Value.iValue? "On" : "Off"
				));
		}
#endif
		//3 将更新完的实时值记录在类CChangedTagManager中
		ctm.add_tag(gk, tte, tv);
	}
}

/*
	2004/8/4
	we should use the tag level coloring scheme to smooth 
	server-side load. for now, the group-level coloring is 
	used, it seems inadequate for lage amout of tag data.
*/
/*
函数功能：执行更新标签的总任务（更新所有标签点，并将报警和实时值等更新信息广播出去）
参数说明：无
返回值：无
*/
void CDBRefresher::task()
{
	if(!rlist.size()) {
		return;
	}
	RTK_CURSOR		hGroup;
	RTK_CURSOR		hTag;
	PRTK_TAG		pTag;
	R_LIST::iterator it;
	GROUP_R_EXT		*ge;
	RTK_TIME		now;	
	__uint			count;
	pmc_value_t tv;
    
	//1 若电源状态不是正在运行PWR_RUNNING（是热备PWR_BACKUP或停止PWR_STOPPED）
	if(get_power_state() != PWR_RUNNING){
		return;
	}
    
	//2 锁定实时数据库内存
	if( !lock_rtdb(__false, 100) ){
		return;
	}
    
	//3 for循环：通过组列表rlist, 遍历所有组，嵌套while循环，更新各组各标签的实时值
	for(it=rlist.begin(); it!=rlist.end(); it++){
		ge = &(*it);
		ge->phase++;
		if(ge->phase < (__int)ge->max_quantums){
			continue;
		}
		ge->phase = 0;
		ge->refresh_count++;				
		hGroup = open_group(HNODE_LOCAL_MACHINE, &ge->key);
		if(!hGroup){
			continue;
		}
		hTag = cursor_open_first_subitem(hGroup);
		if(!hTag){
			close_handle(hGroup);
			continue;
		}
		rtk_time_mark(&now);
#if 0
		UTILS_TIME		start_time;
		time_mark(&start_time);
#endif
		count = cursor_get_subitem_count(hGroup);

		//初始化CChangedTagManager
		ctm.init(count, &ge->key);
		
		//while循环：更新一个组中的所有标签点
		while(pTag = (PRTK_TAG)cursor_get_item(hTag)){			
			if((pTag->d.Value.Flags & (TF_Translated | TF_ConfigOk))
				==
				(TF_Translated | TF_ConfigOk)
			){
				assert(get_value_type(pTag->d.Value.Flags) ==
					get_value_type(pTag->s.Flags));								
				//更新实时值到tv中
				io_update_tag(pTag, &tv, &now);
			}else{
				tv.Flags = 0;
				set_value_type(tv.Flags, get_value_type(pTag->s.Flags));
			}
			//将从设备中更新所得的实时值tv赋给pTag，并更新报警信息，并将tv记录在ctm中
			check_exception(&ge->key, pTag, &tv, &now);
			cursor_move_next(hTag);
		}
		close_handle(hTag);
		close_handle(hGroup);
        
		//将该组所有更新过的标签信息广播到网上，即数据更新信息的数据报发送是以组为单位。
		ctm.broadcast();
#if 0
		UTILS_TIME e;
		time_mark(&e);
		DEBUG_PRINTF((
			">>G %s : %d tags flushed in %.3f ms, %d broadcasted.\n",
			(char*)CGroupName(ge->key),
			count,
			time_diff(&e, &start_time)*1000,
			ctm.get_items()
			));
#endif
	}
    
	//解除实时库内存锁定
	unlock_rtdb();
}

/*
函数功能：向组信息列表(g_Refresher.rlist)中添加组信息
参数：
          pgrp   指向待添加的组信息结构体（RTK_GROUP）指针
返回值：无
*/
void  CDBRefresher::OnAddGroup(PRTK_GROUP pgrp)
{
	GROUP_R_EXT ext;
	R_LIST & rlist = g_Refresher.rlist;
	R_LIST::iterator it;
    
	//1 若在g_Refresher.rlist中找到了pgrp->key则更新rlist
	for(it = rlist.begin(); it != rlist.end(); it++){
		if(is_same_key(&it->key, &pgrp->key)){
			// editing existing group
			it->max_quantums = pgrp->period / rtkm_refresh_quantum;
			if(0 >= it->max_quantums){
				it->max_quantums = 1;
			}
			it->phase = last_phase % it->max_quantums;
			last_phase++;
			return;
		}
	}
	/*2 若没找到，在rlist的末尾插入一个新的元素GROUP_EXT*/
	ZeroMemory(&ext, sizeof(ext));
	ext.key = pgrp->key;
	ext.max_quantums = (int)ceil((double)pgrp->period / rtkm_refresh_quantum);
	if(0 >= ext.max_quantums){
		ext.max_quantums = 1;
	}
	ext.phase = last_phase % ext.max_quantums;
	last_phase++;
	rlist.insert(rlist.end(), ext);
}

/*
功能：更新指定的组 在 组列表 中的配置信息
参数：
      grp   需要在组列表中更新的 组
返回：无
*/
void  CDBRefresher::OnEditGroup(PRTK_GROUP grp)
{
	R_LIST & rlist = g_Refresher.rlist;
	R_LIST::iterator it;

	for(it = rlist.begin(); it != rlist.end(); it++){
		if(is_same_key(&it->key, &grp->key)){
			it->max_quantums = grp->period / rtkm_refresh_quantum;
			if(0 >= it->max_quantums){
				it->max_quantums = 1;
			}
			it->phase = last_phase % it->max_quantums;
			last_phase++;
			break;
		}
	}
}

/*
功能：删除 组列表 中的指定组
参数：
      grp   待删除的组名 
返回：无
*/
void  CDBRefresher::OnDropGroup(PRTK_GROUP grp)
{
	R_LIST & rlist = g_Refresher.rlist;
	R_LIST::iterator it;

	for(it = rlist.begin(); it != rlist.end(); it++){
		if(is_same_key(&it->key, &grp->key)){
			rlist.erase(it);
			break;
		}
	}
}

/*
函数功能：根据标签静态属性中配置的设备信息，将标签添加到对应的设备驱动中
函数说明：
          tag  指向待添加标签的实时数据结构(RTK_TAG)
返回值：  无
*/
void  CDBRefresher::OnAddTag(PRTK_TAG tag)
{
	PDEVICE_INFO d;
    
	//1 对标签的动态属性做链接
	if(!tag->d.DeviceObj){
		RtkInitializeListHead(&tag->d.DeviceLink);
	}else{
		assert(!RtkIsListEmpty(&tag->d.DeviceLink));
	}

	// preset dynamic portion
	//2 用标签的静态属性的Flags设置其动态属性的Flags
	set_value_type(tag->d.Value.Flags, get_value_type(tag->s.Flags));
	
	// re-validate the static configuration
	//validate_tag()有问题？？？？只能返回0
	//3 将标签的Flags中添加该标签点配置正确的标识
	if(validate_tag(tag) == 0){
		tag->d.Value.Flags |= TF_ConfigOk;
	}else{
		tag->d.Value.Flags &= ~TF_ConfigOk;
	}

	// handling refresh coloring
	//
	
	//4 从设备列表中读取该标签静态属性中关联的设备信息
	d = io_open_device(&tag->s.Device);
	
	//5 若标签的动态属性中关联着设备信息，则从该设备驱动中删除标签
	if(tag->d.DeviceObj){
		detach_tag_from_device(tag);
	}
    
	//6 根据标签静态属性中的设备信息找到设备驱动，并链接标签
	if(d){
		attach_tag_to_device(tag, d);
	}
    
	//7 设置标签动态值的标识
	tag->d.Value.Flags |= TF_Shutdown;
}

/*调用设备驱动函数dispatch()，去掉设备中的指定标签，并将标签中关联的设备信息指针置0*/
void CDBRefresher::OnDropTag(PRTK_TAG tag)
{
	// BUGBUG: commented out temporarily, 2003/12/11, Jackie
	// FIXED: 2004/8/18
	detach_tag_from_device(tag);
}

