#include "precomp.h"
#pragma hdrstop

#include <algorithm>
#include <time.h>

#if 0
#define WORKER_DEBUG(x) DEBUG_PRINTF(x)
#else
#define WORKER_DEBUG(x) utils_debug x
#endif

/* convert time_t to string */
/* use the "YYYY/MM/DD hh:mm:ss" format */

static char *strtime (time_t *t)
{
	struct tm	*local;
	static char result[32];

	local = localtime(t);
	sprintf(result,"%4d/%02d/%02d %02d:%02d:%02d",
		  local->tm_year+1900, local->tm_mon+1, local->tm_mday,
		  local->tm_hour, local->tm_min, local->tm_sec);
	return result;
}

/*
 *function: Gets the system time and convert to string(YYYY/MM/DD hh:mm:ss)
 */
static char * now()
{
	time_t t;
	time(&t);
	return strtime(&t);
}


__uint CProxyWorker::m_MaxBgLoads;

/*
 *函数功能：比较a和b的tag是否相等
 *
 */
bool operator ==(const struct task_item_t & a, const struct task_item_t & b)
{
	return a.tag == b.tag;
}

/*
	determine if a loading requesting is pending
	environment : db lock held 
*/
/*
 *函数功能：查找CWaitingList中是否存有指定的标签名
 *输入参数：标签名
 *返    回：有返回1；没有返回0
 */
__bool CProxyWorker::is_loading(PCTAG_NAME n)
{
#if 1
	CWaitingList::reverse_iterator p;
	task_item_t key;
	key.tag = *n;
	p = find(m_Tasks.rbegin(), m_Tasks.rend(), key);
	return p == m_Tasks.rend()? __false : __true;
#else
	WORKER_DEBUG((
		">>T%08x, %d, checking: '%s.%s.%s'\n",
		id(),
		m_Tasks.size(),
		(char*)CNodeName(n->node),
		(char*)CGroupName(n->sname.group),
		(char*)CTagName(n->sname.tag)
		));

	CWaitingList::iterator p;
	task_item_t key;
	key.tag = *n;
	p = m_Tasks.begin();
	for(int i=m_Tasks.size(); i; i--, p++){
		if(*p == key){
			return __true;
		}
	}
	return __false;
#endif
}

/*
	....
	cancel_load : remove an entry from the loading list
	Parameters:
	...
	Environment :
	database unlocked.
*/
/*
 *函数功能：在CWaitingList中找到含有指定标签名tk的task_item_t型的元素，并将其erase
 *输入参数：指定的标签名
 *返    回：erase成功，返回1；锁定数据库不成功，返回0
 */
__bool CProxyWorker::cancel_load(PCTAG_NAME tk)
{
	bool ret = __false;
	CWaitingList::iterator p;
	
	if(!lock_rtdb(__true, 1000)){
		return __false;
	}
	task_item_t key;
	key.tag = *tk;
	p = find(m_Tasks.begin(), m_Tasks.end(), key);
	if(p != m_Tasks.end()){
		m_Tasks.erase(p);
		ret = __true;
	}
	unlock_rtdb();

	if(ret){
		WORKER_DEBUG((
			">>T%08x, task cancelled: '%s.%s.%s'\n",
			id(),
			(char*)CNodeName(tk->node),
			(char*)CGroupName(tk->sname.group),
			(char*)CTagName(tk->sname.tag)
			));
	}

	return ret;
}

/*
	....
	Environment :
	database unlocked.
*/
/*
 *函数功能：将count个标签名的task_item_t插入到CWaitingList中。
 *输入参数：指定的标签名，指定的标签名的总数，强制插入标识
 *说    明：强制插入是不管CWaitingList是否有指定的标签名，直接插入CWaitingList中；非强制插入是若CWaitingList中已经存有指定的标签名则不插入，没有才插入
 *返    回：失败（主要是线程同步操作失败的情况）返回0；成功（包括插入或不插入的情况）返回1
 */
__bool CProxyWorker::inform_load(PCTAG_NAME jobs, __uint count, bool bForce)
{
	__uint		i;
	TAG_NAME	tk;
	PCTAG_NAME	j;
	task_item_t task;

	if(!lock_rtdb(__true, 1000)){
		return __false;
	}

	if(m_Tasks.size() > m_MaxBgLoads){
		unlock_rtdb();
		return __false;
	}
	
	for(i=0, j=jobs; i<count; i++, j++){
		memset(&tk, 0, sizeof(TAG_NAME));
		if(!bForce){
			tk.node = j->node;
			// check if pertaining node is being loaded
			if(is_loading(&tk)){
				continue;
			}
			// check if pertaining group is being loaded
			if(j->sname.group.Data[0]){
				tk.sname.group = j->sname.group;
				if(is_loading(&tk)){
					continue;
				}
				// check if already being loaded
				if(j->sname.tag.Data[0]){
					tk.sname.tag = j->sname.tag;
					if(is_loading(&tk)){
						continue;
					}
				}
			}else{
			}
		}else{
			tk = *j;
		}
		WORKER_DEBUG((
			">>T%08x, %d, inform loading: '%s.%s.%s'\n",
			id(),
			m_Tasks.size(),
			(char*)CNodeName(tk.node),
			(char*)CGroupName(tk.sname.group),
			(char*)CTagName(tk.sname.tag)
			));
		task.tag = tk;
		task.failCount = 0;
		m_Tasks.insert(m_Tasks.end(), task);
	}

	unlock_rtdb();
	
	return __true;
}

/*
 *函数功能：将指定node下的各个标签？？强制插入CWaitingList
 *输入参数：指定的节点名
 */
__bool CProxyWorker::load_node(PNODE_KEY nk)
{
	RTK_CURSOR hNode;

	if(!nk){
		return __false;
	}

	WORKER_DEBUG((
		">>BGLoader, %d, loading node '%s'\n",
		m_Tasks.size(),
		(char*)CNodeName(*nk)
		));

	if(!lock_rtdb(__true, 2000)){
		return __false;
	}
	hNode = create_node(nk, 0);
	close_handle(hNode);
	unlock_rtdb();
	if(!hNode){
		return __false;
	}

	discover_groups_ex(nk, 3000, NULL, __false);      //discover.cpp
	
	RTK_CURSOR hGroup;
	PRTK_GROUP pGroup;
	int i, count;
	PTAG_NAME pNames = 0;

	if(!lock_rtdb(__false, 2000)){
		return __false;
	}
	hNode = open_node(nk);
	if(hNode){
		count = cursor_get_subitem_count(hNode);      //librtdb.dll  ??
		pNames = new TAG_NAME[count];
		memset(pNames, 0, sizeof(TAG_NAME) * count);
		i = 0;
		if(pNames){
			hGroup = cursor_open_first_subitem(hNode);     //librtdb.dll ??
			while(!cursor_is_end(hGroup)){
				pGroup = (PRTK_GROUP)cursor_get_item(hGroup);
				pNames[i].node = *nk;
				pNames[i].sname.group = pGroup->key;
				cursor_move_next(hGroup);//librtdb.dll
				i++;
			}
			close_handle(hGroup);
			assert(i == count);
		}
		close_handle(hNode);
	}
	unlock_rtdb();
	if(pNames){
		inform_load(pNames, count, true);   //强制插入
		delete pNames;
	}

	return __true;
}

/*
 *函数功能：需先理解discover_tags_ex()
 *
 */
__bool CProxyWorker::load_group(PRTK_GROUP_NAME gn)
{
	__bool ret = __false;
	
	if(!gn){
		return __false;
	}

	WORKER_DEBUG((
		">>BGLoader, %d, loading group '%s.%s'\n",
		m_Tasks.size(),
		(char*)CNodeName(gn->node),
		(char*)CGroupName(gn->group)
		));
	ret = discover_tags_ex(&gn->node, &gn->group, 5000, NULL, __false);

	return ret;
}

//需先理解reload_tags()
__bool CProxyWorker::load_tag(PCTAG_NAME n)
{
	WORKER_DEBUG((
		">>BGLoader, %d, loading tag : '%s.%s.%s'\n",
		m_Tasks.size(),
		(char*)CNodeName(n->node),
		(char*)CGroupName(n->sname.group),
		(char*)CTagName(n->sname.tag)
		));
	reload_tags(1, n, 1000);
	return __true;
}

/*
 *函数功能：1、装载CWaitingList中的标签到CWaitingList中？？？？？???
            2、涉及到其他线程也操作了CWaitingList的情况
 *
 */
void CProxyWorker::do_load()
{
	CWaitingList::iterator p;
	task_item_t task;
	__bool ret;

	if(!(m_TaskFlag & PROXY_ENABLE_BGLOADER)){
		return;
	}
	while(true){
		kill_nodes();

		if(!lock_rtdb(__false, 1000)){
			break;
		}
		if(!m_Tasks.size()){
			unlock_rtdb();
			break;
		}
		p = m_Tasks.begin();
		task = *p;
		unlock_rtdb();
        
		if(task.tag.sname.tag.Data[0]){
			ret = load_tag(&task.tag);
		}else if(task.tag.sname.group.Data[0]){
			RTK_GROUP_NAME	gn;
			gn.node = task.tag.node;
			gn.group = task.tag.sname.group;
			ret = load_group(&gn);
		}else if(task.tag.node.Data[0]){
			ret = load_node(&task.tag.node);
		}else{
			// assert(false);
			ret = __true;
		}

		/* 2005-10-26 this sleep proves to be vital for large 
		system(greater than 7000 points)
		*/
		rtk_sleep(g_QueryInterval);
		
		if(!lock_rtdb(__true, 1000)){
			break;
		}

		// p might have already been removed from the list by other thread
		// note the check must be made while db lock is held
		// 2003/7/14, Jackie
		p = find(m_Tasks.begin(), m_Tasks.end(), task);
		// 2004/10/15, jackie, fixed a bug here
		if(p != m_Tasks.end()){
			if(!ret){
				(*p).failCount++;
				if((*p).failCount >= 3){
					m_Tasks.erase(p);
				}
			}else{
				m_Tasks.erase(p);
			}
		}
		
		unlock_rtdb();
	}
}

/* cancel un-loaded entries for this node */
/*
 *函数功能：消除链表CWaitingList中的节点nk对应的结构体元素。
 */
void CProxyWorker::purge_load_queue(PCNODE_KEY nk)
{
	CWaitingList::iterator it;
	it = m_Tasks.begin();
	while(it != m_Tasks.end()){
		task_item_t * ti;
		ti = &(*it);
		if(ti->tag.node == *nk){
			it = m_Tasks.erase(it);
		}else{
			it++;
		}
	}
}

/*
1、打开第一个node的游标
2、得到一个node对应的RTK_NODE
3、计算RTK_NODE的成员dogcount
4、若dogcount<0,则cursor_delete，并跳回第一步执行（暂时还不知道cursor_delete的具体操作？？？）
   若dogcount>=0,则cursor_move_next()，跳到第2步取执行
5、有goto就是比较乱啊~~~~~
*/
void CProxyWorker::kill_nodes()
{
	if(!lock_rtdb(__true, 1000)){
		return;
	}
	RTK_CURSOR	hNode;
	PRTK_NODE	pNode;
	NODE_KEY	nk;
__oops_a_guy_is_murdered:
	hNode = open_first_node();
	while(!cursor_is_end(hNode)){
		pNode = (PRTK_NODE)cursor_get_item(hNode);
		pNode->dogcount -= m_dwPeriod;
		if(pNode->dogcount < 0){
			nk = pNode->key;
			utils_debug(
				"Kicking '%s' off the shelf.\n",
				(char*)CNodeName(nk)
				);
			cursor_delete(hNode);
			/*
			rtk_queue_event(
				PT_Alarm, 
				0,
				AC_Offline,
				"Site idle too long, kicked out.",
				&nk
				);				
			*/
			goto __oops_a_guy_is_murdered;
		}
		cursor_move_next(hNode);
	}
	close_handle(hNode);
	unlock_rtdb();
}

void CProxyWorker::kill_tags()
{
}

void CProxyWorker::task()
{
	extern void _load_settings(__bool bSilent);       //类中的成员函数调用类之外的普通函数需要extern一下~~
	_load_settings(__true);              //对相应的全局变量赋值，即初始化
	do_load();                           
}


/*
 *CProxyWorker构造函数功能：1、从pmc.ini中取出字段[PMC]下"BGLoaderInternal"的值，赋给CProxyWorker的父类CPeriodicThread的成员m_dwPeriod
 *                          2、用flags给其本身的成员m_TaskFlag赋值
 *                          即对两个变量m_dwPeriod和m_TaskFlag赋值
 */
CProxyWorker::CProxyWorker(int flags)
{
	m_dwPeriod = GetPrivateProfileInt(
		"PMC",
		"BGLoaderInterval",
		1000,
		get_config_file()
		);
	if(m_dwPeriod < 20 || m_dwPeriod > 30000){
		utils_error(
			"BGLoaderInterval = %d is not valid, reset to %d.\n",
			m_dwPeriod,
			1000
			);
		m_dwPeriod = 1000;
	}
	m_TaskFlag = flags;
}

