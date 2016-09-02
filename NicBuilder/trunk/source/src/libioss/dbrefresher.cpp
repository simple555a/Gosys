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
�������ܣ������ؽڵ��µ���������ӵ�g_Refresher.rlist�У����б�ǩ����������Ե��豸������
����˵���� ��
����ֵ��  ��
*/
void CDBRefresher::post_db_init()
{
	RTK_CURSOR handle;
	PRTK_GROUP p;
    //�ҵ����ؽڵ���α�CURSOR
	handle = cursor_open_first_subitem(HNODE_LOCAL_MACHINE);
	//�������ؽڵ��µ������飬��������ڴ��е�����Ϣ
	while(!cursor_is_end(handle)){
		p = (PRTK_GROUP)cursor_get_item(handle);
		OnAddGroup(p);
		RTK_CURSOR hTag;
		PRTK_TAG   pTag;
		hTag = cursor_open_first_subitem(handle);
		//�������е����б�ǩ��,
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
�������ܣ�add���ؽڵ㡢��ͱ�ǩ����g_Handlers[]�еĺ���ָ�븳ֵ������һ���̡߳�����ʼ������ˢ�¹���
����˵������
����ֵ��  0 __false      ����ʵʱ���ڴ�ʧ��
                     ��  �����߳�ʧ��
		  1 __true       ��ʼ���ɹ�
*/
__bool CDBRefresher::init()
{
	if(!lock_rtdb(__false, 1000)){
		return __false;
	}
    
	//1 �����б�ǩ�������豸����
	post_db_init();
    
	//2 RTDB_DISPATCH_TABLE�еĺ���ָ�븳ֵ
	s_Handler.OnAddGroup = OnAddGroup;
	s_Handler.OnDropGroup = OnDropGroup;
	s_Handler.OnAddTag = OnAddTag;
	s_Handler.OnModifyTag = OnAddTag;
	s_Handler.OnModifyGroup = OnEditGroup;
	s_Handler.OnDropTag	= OnDropTag;
    
	//3 ��ʵʱ����ɽṹs_Handler��ӵ�librtdb.dll��ʵʱ����ɽṹ��������
	hook_rtdb_events(&s_Handler);
    
	//4 ����DB Refresher�߳�
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
�������ܣ�ע������ˢ�¹���--ɾ��ʵʱ����ɽṹ���ر��̡߳������CDBRefresher�е�����Ϣ�б�
����˵������
�� �� ֵ��ֻ�ܷ���__true  1
          û�д���ֵ����
*/
__bool CDBRefresher::uninit()
{
	//1 ��ʵʱ����ɽṹ������ɾ��s_Handler
	unhook_rtdb_events(&s_Handler);
	//2 �ر�DB refresher�߳�
	if (g_Refresher.stop(5000)){
		utils_trace("DB refresher stopped.\n");
	}else{
		utils_trace("DB refresher stopping timeout.\n");
	}
	//3 �������Ϣ�б�
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
�������ܣ�����ʵʱֵ�ı�����Ϣ�Ͷ�̬���ԣ����ͱ������ݱ��������µ�ʵʱֵ��¼��CChangedTagManager��
����˵����
          gk      ���µ�ʵʱֵ��ǩ���ڵ�����
		  tte     ʵʱֵ�ṹ��
		  tv      ���豸�õ�������ʵʱֵ
		  now     ʱ��
�� �� ֵ����
*/
void CDBRefresher::check_exception(
	const GROUP_KEY *gk, 
	PRTK_TAG tte, 
	pmc_value_t *tv, 
	PCRTK_TIME now
	)
{
	//1 ����ʵʱֵtv���õ�����״̬��Ϣ�����㲥��������
	get_exception(gk, tte, tv, now);
    
	//2 ���±�ǩ��ʵʱ��̬����
	// ���¶�ֵ̬
	tte->d.Value.Value = tv->Value;
	// ���¶�̬��ʶ
	tte->d.Value.Flags &= ~TF_Valid;
	tte->d.Value.Flags |= (tv->Flags & TF_Valid);

	if(tv->Flags & TF_Valid){
		// clear the first-unavailable flag
		tte->d.Value.Flags &= ~TF_Shutdown;
	}
	// ���¶�̬ʱ��
	tte->d.Time = *now;
	tv->Flags = tte->d.Value.Flags;
    // ���¹㲥��ʱ���ֵ
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
		//3 ���������ʵʱֵ��¼����CChangedTagManager��
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
�������ܣ�ִ�и��±�ǩ�������񣨸������б�ǩ�㣬����������ʵʱֵ�ȸ�����Ϣ�㲥��ȥ��
����˵������
����ֵ����
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
    
	//1 ����Դ״̬������������PWR_RUNNING�����ȱ�PWR_BACKUP��ֹͣPWR_STOPPED��
	if(get_power_state() != PWR_RUNNING){
		return;
	}
    
	//2 ����ʵʱ���ݿ��ڴ�
	if( !lock_rtdb(__false, 100) ){
		return;
	}
    
	//3 forѭ����ͨ�����б�rlist, ���������飬Ƕ��whileѭ�������¸������ǩ��ʵʱֵ
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

		//��ʼ��CChangedTagManager
		ctm.init(count, &ge->key);
		
		//whileѭ��������һ�����е����б�ǩ��
		while(pTag = (PRTK_TAG)cursor_get_item(hTag)){			
			if((pTag->d.Value.Flags & (TF_Translated | TF_ConfigOk))
				==
				(TF_Translated | TF_ConfigOk)
			){
				assert(get_value_type(pTag->d.Value.Flags) ==
					get_value_type(pTag->s.Flags));								
				//����ʵʱֵ��tv��
				io_update_tag(pTag, &tv, &now);
			}else{
				tv.Flags = 0;
				set_value_type(tv.Flags, get_value_type(pTag->s.Flags));
			}
			//�����豸�и������õ�ʵʱֵtv����pTag�������±�����Ϣ������tv��¼��ctm��
			check_exception(&ge->key, pTag, &tv, &now);
			cursor_move_next(hTag);
		}
		close_handle(hTag);
		close_handle(hGroup);
        
		//���������и��¹��ı�ǩ��Ϣ�㲥�����ϣ������ݸ�����Ϣ�����ݱ�����������Ϊ��λ��
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
    
	//���ʵʱ���ڴ�����
	unlock_rtdb();
}

/*
�������ܣ�������Ϣ�б�(g_Refresher.rlist)���������Ϣ
������
          pgrp   ָ�����ӵ�����Ϣ�ṹ�壨RTK_GROUP��ָ��
����ֵ����
*/
void  CDBRefresher::OnAddGroup(PRTK_GROUP pgrp)
{
	GROUP_R_EXT ext;
	R_LIST & rlist = g_Refresher.rlist;
	R_LIST::iterator it;
    
	//1 ����g_Refresher.rlist���ҵ���pgrp->key�����rlist
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
	/*2 ��û�ҵ�����rlist��ĩβ����һ���µ�Ԫ��GROUP_EXT*/
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
���ܣ�����ָ������ �� ���б� �е�������Ϣ
������
      grp   ��Ҫ�����б��и��µ� ��
���أ���
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
���ܣ�ɾ�� ���б� �е�ָ����
������
      grp   ��ɾ�������� 
���أ���
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
�������ܣ����ݱ�ǩ��̬���������õ��豸��Ϣ������ǩ��ӵ���Ӧ���豸������
����˵����
          tag  ָ�����ӱ�ǩ��ʵʱ���ݽṹ(RTK_TAG)
����ֵ��  ��
*/
void  CDBRefresher::OnAddTag(PRTK_TAG tag)
{
	PDEVICE_INFO d;
    
	//1 �Ա�ǩ�Ķ�̬����������
	if(!tag->d.DeviceObj){
		RtkInitializeListHead(&tag->d.DeviceLink);
	}else{
		assert(!RtkIsListEmpty(&tag->d.DeviceLink));
	}

	// preset dynamic portion
	//2 �ñ�ǩ�ľ�̬���Ե�Flags�����䶯̬���Ե�Flags
	set_value_type(tag->d.Value.Flags, get_value_type(tag->s.Flags));
	
	// re-validate the static configuration
	//validate_tag()�����⣿������ֻ�ܷ���0
	//3 ����ǩ��Flags����Ӹñ�ǩ��������ȷ�ı�ʶ
	if(validate_tag(tag) == 0){
		tag->d.Value.Flags |= TF_ConfigOk;
	}else{
		tag->d.Value.Flags &= ~TF_ConfigOk;
	}

	// handling refresh coloring
	//
	
	//4 ���豸�б��ж�ȡ�ñ�ǩ��̬�����й������豸��Ϣ
	d = io_open_device(&tag->s.Device);
	
	//5 ����ǩ�Ķ�̬�����й������豸��Ϣ����Ӹ��豸������ɾ����ǩ
	if(tag->d.DeviceObj){
		detach_tag_from_device(tag);
	}
    
	//6 ���ݱ�ǩ��̬�����е��豸��Ϣ�ҵ��豸�����������ӱ�ǩ
	if(d){
		attach_tag_to_device(tag, d);
	}
    
	//7 ���ñ�ǩ��ֵ̬�ı�ʶ
	tag->d.Value.Flags |= TF_Shutdown;
}

/*�����豸��������dispatch()��ȥ���豸�е�ָ����ǩ��������ǩ�й������豸��Ϣָ����0*/
void CDBRefresher::OnDropTag(PRTK_TAG tag)
{
	// BUGBUG: commented out temporarily, 2003/12/11, Jackie
	// FIXED: 2004/8/18
	detach_tag_from_device(tag);
}

