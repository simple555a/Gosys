//=========================================================================
//  Nicsys1000   版本:  2.0
//-------------------------------------------------------------------------
//  
//  实时库的通用错误码定义文件
//
//-------------------------------------------------------------------------
//  版权 (C) 2012 - 北京中核控制系统工程有限公司(CNCS)
//=========================================================================
// 
//  改动历史：
//  	1. 2012/02/27 : 	CNCS-0136 建立文件
//=========================================================================
#ifndef __Nicsys_ERROR_H__
#define __Nicsys_ERROR_H__

#include "nicPrecomp.h"
CDECL_BEGIN

//=====================================================================
// 
// 函数返回值及错误号定义
// 
//=====================================================================
/// <summary> 
///	pSpace错误号码，范围从-20000至-10000
/// </summary> 
/// 
typedef enum
{	
	nicERR_TAG_MIN = -15000,						// 接口返回最小错误号
	nicPOINT_NOT_NULL,
	nicERR_TAG_START,						
	nicERR_TAG_ID_ALLOT,						// ID分配失败
	nicERR_PROP_SETERR,
	nicERR_PROP_GETERR,
	FAIL_DEL_FILE,
	nicERR_PROP_DATATYPE,
} nicTagBaseErrorCodeEnum;

typedef enum
{
	nicERR_MIN = -20000 ,						// 接口返回最小错误号

	nicERR_COMMON_BASE = -20000 ,						// 通用错误

	nicERR_DB_CANNOT_START_BASE,                 //实时库基本属性模块无法启动
	nicERR_DB_CANNOT_START_DIGITAL,            //实时库开关量模块无法启动
	nicERR_DB_CANNOT_START_ANALOG,				//实时库模拟量模块无法启动
	nicERR_DB_CANNOT_START_REAL,					//实时库实时值部分无法启动
	nicERR_DB_CANNOT_START_NOTIFY,				//实时库通知模块无法启动

	nicERR_DB_CANNOT_STOP,                            //实时库无法正常停止
	nicERR_DB_CANNOT_STOP_BASE,					//实时库基本属性模块停止出错
	nicERR_DB_CANNOT_STOP_DIGITAL,              //实时库开关量模块停止出错
	nicERR_DB_CANNOT_STOP_ANALOG,              //实时库模拟量模块停止出错
	nicERR_DB_CANNOT_STOP_REAL,                  //实时库实时值部分停止出错
	nicERR_DB_CANNOT_STOP_NOTIFY,			   //实时库通知模块停止出错

	nicERR_COMMON_FUNCTION_NOT_IMPLEMENT ,				// 功能未实现
	nicERR_COMMON_PARAMETER_INVALID ,					// 参数错误
	nicERR_FAIL_IN_BATCH ,						// 操作集中有部分操作出错，部分成功
	nicERR_COMMON_NO_MEMORY ,							// 无足够内存空间
	nicERR_COMMON_NO_AUHTORITY,							// 无操作权限
	nicERR_COMMON_STRING_TOO_LONG,						// 字符串太长
	nicERR_COMMON_DATACHANGE_FAIL,                       // 数据转换出错
	nicERR_COMM_NOMQ,									// 同步操作没有发现相关消息队列

	nicERR_INTERFACE_BASE = -19900 ,						// 接口动态库相关错误

	nicERR_INTERFACE_REGISTER_CALLBACK_FAIL,				// 回调函数注册失败

	nicERR_NET_BASE = -19800 ,					// 通讯相关错误

	nicERR_NET_GETQ_TIMEOUT,						// 操作消息队列超时
	nicERR_NET_MQ_SHUTDOWN,						// 消息队列关闭
	nicERR_NET_MAPUNBIND,						// 连接信息解除绑定失败
	nicERR_NET_STATUSERR,						// 网络连接状态查找失败,没有对应的状态信息保存
	nicERR_NET_CONNECT_FAILED,					// 连接失败
	nicERR_NET_SEndID_N_ERR,						// 发送失败
	nicERR_NET_RECVN_FAILED,                     // 接受失败
	nicERR_NET_NEWTHREAD_SEndID_,					// 线程失败
	nicERR_NET_NO_CONNECTED,						// 没有连接
	nicERR_NET_NOHANDLE_FIND,					// 没有保存的handle信息
	nicERR_NET_MAX_CONNECTION,					// 最大连接数
	nicERR_NET_NEEDRESET,						// 服务器端已经重启，客户端需要重启
	nicERR_NET_CONNECTING,					//网络正在连接中

	nicERR_SCHUDULER_BASE = -19700 ,				// 服务器相关错误

	nicERR_SCHUDULER_STATUS_ERROR,				// 服务器处在错误状态
	nicERR_SCHUDULER_STATUS_STOP,				// 服务器处在停止状态
	nicERR_SCHUDULER_STATUS_BUSY,				// 服务器处在正忙状态
	nicERR_SCHUDULER_PARA_FILE_FAIL,				// 调度模块读取配置文件失败
	nicERR_SCHEDULER_CONNECT_HASHMAP_ERROR, 		// 读取hash表失败
	nicERR_SCHEDULER_StartID__ERROR,				// 调度模块启动失败
	nicERR_SCHEDULER_CONNECT_NOT_FOUND,  		// 调度模块中未发现连接信息
	nicERR_SCHEDULER_API_CONNECT_NOT_FOUND,		// 调度模块中未发现应用连接
	nicERR_SCHEDULER_NET_CONNECT_NOT_FOUND,		// 调度模块中未发现网络连接
	nicERR_CONNECTMGR_STOPED,					// 连接管理器已经停止

	nicERR_TAGTYPEMANAGER_BASE = -19600 ,		// 测点类型相关错误

	nicERR_TAGTYPEMANAGER_CANNOT_StartID_,			// 测点类型管理模块不能启动
	nicERR_TAGTYPEMANAGER_MANAGER_FILE_NOT_FOUND,	// 测点类型管理文件不存在
	nicERR_TAGTYPEMAMAGER_MAMAGER_FILE_FAIL,		// 测点类型管理文件错误
	nicERR_TAGTYPEMANAGER_TOTAL_PROP_FILE_NOT_FOUND,	// 测点类型完整属性文件不存在
	nicERR_TAGTYPEMANAGER_TOTAL_PROP_FILE_FAIL,		// 测点类型完整属性文件错误

	nicERR_TAGTYPE_BASE = -19500 ,				// 测点类型相关错误

	nicERR_TAGTYPE_CANNOT_SRART,					// 测点类型动态库不能启动
	nicERR_TAGTYPE_HAVE_StartID_ED,					// 测点类型动态库已经启动
	nicERR_TAGTYPE_INFO_FILE_FAIL,				// 测点类型信息文件错误
	nicERR_TAGTYPE_PROP_FILE_FAIL,				// 测点类型属性文件错误

	nicERR_TAG_BASE = -19400 ,					// 测点相关错误

	nicERR_TAG_HAS_PARENT,						// ???
	nicERR_TAG_WRONG_NAME,						//名字里的字符不合法
	nicERR_TAG_ADD,								// ???
	nicERR_TAG_NAME,								// ???
	nicERR_TAGTREE_DEL,							// ???
	nicERR_TAG_GET_ID,							// ???
	nicERR_TAG_NAMEHASH_UNBIND,					// ???
	nicERR_TAG_GETLONGNAME,						// ???
	nicERR_TAG_NAMEHASH,							// ???
	nicERR_TAG_NAME_TOOLONG,						// ???
	nicERR_TAG_GETSHORTNAME,						// ???
	nicERR_TAGTREE_TRAVERSE,						// ???
	nicERR_TAG_ISANCESTOR,						// ???
	nicERR_TAG_INFOFILE,							// 不能打开测点信息文件
	nicERR_TAG_DATAFILE,							// 不能打开测点数据文件
	nicERR_TAG_DELETE_ROOT,						// ???
	nicERR_TAG_HAVE_DELETED,						// 测点已经被删除
	nicERR_TAG_HAVE_CHILD,							//测点有子节点，不能被删除
	nicERR_TAG_PROP_REDUPLICATE,					// 属性重复
	nicERR_TAG_NAME_REDUPLICATE,					// 测点名称重复
	nicERR_TAG_NAME_NOT_EXIST,					// 测点名不存在
	nicERR_TAG_NAMEHASH_FAILURE,					// 名称HASH表出错
	nicERR_TAG_NOT_NODE,							// 指定的点不是结点类型
	nicERR_TAG_CANNOT_MOVE,						// 测点不能移动
	nicERR_TAG_CANNOT_COPY,						// 测点不能复制
	nicERR_TAG_NOT_EXIST,						// 测点不存在
	nicERR_TAG_PROP_NOT_EXIST,					// 属性不存在
	nicERR_TAG_PROP_NOT_CHANGED,					// 测点值未攺变
	nicERR_TAG_PROP_FILTER,						// 测点值被过滤
	nicERR_TAG_CANNOT_GETPROP,					// 不能读属性
	nicERR_TAG_CANNOT_SETPROP,					// 不能写属性		
	nicERR_TAG_LACK_PROP,						// 增加测点时缺少必要属性

	nicERR_DATATYPE,								// 数据类型错误 ???
	nicERR_DATATYPE_NOT_MATCH,
	nicERR_TAG_NOT_SAVE_HIS,

	nicERR_TAGNOTIFY_BASE = -19300,

	nicERR_TAGNOTIFY_SUBCRIBE_FAILED,			// 注册失败
	nicERR_TAGNOTIFY_CANNOT_StartID_,				// 测点订阅发布模块不能启动
	nicERR_TAGNOTIFY_SUBSCRIBE_MAP_ERROR,		// 测点订阅发布模块的订阅表错误
	nicERR_TAGNOTIFY_PBULISH_MAP_ERROR,			// 测点订阅发布模块的发布者表错误
	nicERR_TAGNOTIFY_TAG_NOT_SUBSCRIBED,			// 测点未被订阅
	nicERR_TAGNOTIFY_CONNECT_NOT_FOUND,			// 连接不存在

	nicERR_REAL_BASE = -19200 ,					// 实时相关错误

	nicERR_REAL_CANNOT_READ,						// 不能读实时值
	nicERR_REAL_CANNOT_WRITE,					// 不能写实时值
	nicERR_REAL_DATA_FILE_FAIL,					// 实时数据保存文件错误

	nicERR_REALNOTIFY_BASE = -19100 ,			// 实时相关错误

	nicERR_REALNOTIFY_CANNOT_StartID_,				// 实时订阅发布模块不能启动
	nicERR_REALNOTIFY_SUBSCRIBE_MAP_ERROR,		// 实时订阅发布模块的订阅表错误
	nicERR_REALNOTIFY_PUBLISH_MAP_ERROR,			// 实时订阅发布模块的发布者表错误
	nicERR_REALNOTIFY_TAG_NOTSUBSCRIBED,			// 实时未被订阅
	nicERR_REALNOTIFY_CONNECT_NOT_FOUND,			// 连接不存在

	nicERR_AUTHORITY_BASE = -19000,				// 权限相关错误

	nicERR_AUTHORITY_SECURITY_AREA_NOT_MATCH,	// 安全区不匹配
	nicERR_AUTHORITY_WRITE_READONLY_PROPERTY,	// 写只读属性
	nicERR_TAG_BEYOND_MAX,						// 总点数超过授权
	nicERR_LICENSE_UPDATA_ERROR,					// 更新License信息失败，停止运行（刚性保护）
	nicERR_LICENSE_TYPE_NO,						// 没有任何许可证，Server停止运行。（刚性保护）
	nicERR_LICENSE_DEMO_RUN_TIMEOUT,				// 演示版和开发试用版运行超时，Server停止运行。（刚性保护）
	nicERR_LICENSE_NOT_PSPACE5,					// 没有获取pSpace 5.x版本的License，Server停止运行。（刚性保护）
	nicERR_LICENSE_NOT_SIMPLIFIED_CHINESE,		// 没有获取简体中文版本的License，Server停止运行。（刚性保护）
	nicERR_LICENSE_SITRAP_StartID_,					// SITrap启动。（刚性保护）
	nicERR_LICENSE_CONNECT_BEYOND_USERAPP_MAX,	//用户自编写的应用程序客户端连接数超过限制值，不能再建立连接。（弹性保护）
	nicERR_LICENSE_CONNECT_BEYOND_VIEW_MAX,		//View客户端数连接数超过限制值，不能再建立连接。（弹性保护）
	nicERR_LICENSE_CONNECT_BEYOND_NETVIEW_MAX,	//NetView客户端数连接数超过限制值，不能再建立连接。（弹性保护）
	nicERR_LICENSE_CONNECT_BEYOND_IO_MAX,		//NetView客户端数连接数超过限制值，不能再建立连接。（弹性保护）
	nicERR_AUTHORITY_StartID__LICENSE_FAIL,			//启动License失败（刚性保护）
	nicERR_LICENSE_EVALUATION_TIMEOUT,			//评估版运行超时（刚性保护）
	nicERR_LICENSE_NO_EX_LICENSE,				//扩展组件没有License授权（弹性保护）

	nicERR_HIS_BASE = -18900 ,					// 历史和历史文件相关错误

	nicERR_HIS_NO_DISK_SPACE,					// 无足够磁盘空间
	nicERR_HIS_CACHE_OPENED,					    // 缓冲区已经打开
	nicERR_HIS_CACHE_CLOSED,						// 缓冲区没有打开
	// 历史管理文件模块错误号
	nicERR_HIS_NOT_INI_PATH,						// 没有初始化路径
	nicERR_HIS_MANAGER_FILE_OPENED,				// 历史管理文件已经打开
	nicERR_HIS_MANAGER_FILE_CLOSED,				// 历史管理文件没有打开
	nicERR_HIS_DATA_FILE_IN_LIST,				// 文件已在历史数据文件列表，不能被注册
	nicERR_HIS_MANAGER_FILE_VERSION,				// 管理文件文件版本无效
	nicERR_HIS_MANAGER_FILE_LABEL,				// 管理文件文件标识无效
	nicERR_HIS_MANAGER_FILE_INVALID,				// 管理文件文件格式错误
	nicERR_HIS_UNREGIST_CURRENT_FILE,			// 不能注消当前文件
	nicERR_HIS_BACKUP_CURRENT_FILE,				// 不能备份当前文件
	nicERR_HIS_DATA_FILE_NO_DATA,				// 数据文件无数据，不能备份
	nicERR_HIS_DATA_FILE_TIME_BEYOND,			// 恢复数据文件时，文件时间比当前时间还大，出错
	nicERR_HIS_DATA_FILE_TIME_OVERLAP,			// 恢复数据文件时，文件时间与现有文件列表重叠
	nicERR_HIS_DATA_FILE_COUNT_LESS,				// 在替换模式下，历史数据文件的数量过少
	nicERR_HIS_CAN_NOT_REGIST,					// 不能注册
	// 历史数据文件模块错误号
	nicERR_HIS_DATA_FILE_VERSION,				// 管理文件文件版本无效
	nicERR_HIS_DATA_FILE_LABEL,					// 管理文件文件标识无效
	nicERR_HIS_CRC,					 		    // 校验错误
	nicERR_HIS_PAGE_FULL,				 		// 页中数据满了
	nicERR_HIS_DATA_PAGE_BEYOND_MIN,				// 查找时，时间比数据页内最小时间还小
	nicERR_HIS_EndID__OF_INDEX_PAGE,					
	nicERR_HIS_FILE_NOT_IN_LIST,					// 文件ID号在历史管理文件的数据文件列表中找不到
	nicERR_HIS_SOME_DATA_NOT_SAVED,				// 还有一部分数据因为没有空间，不能插入
	nicERR_HIS_NEED_PACK_DATA,					// 还余一部分空间，需要压缩历史数据
	// 游标模块错误号
	nicERR_HIS_FIND_ERROR,						// 查询出现异常情况
	nicERR_HIS_BEYOND_MIN,					 	// 比第一个数据的时间还小
	nicERR_HIS_BEYOND_MAX, 			 			// 比最后一个数据的时间还大
	nicERR_HIS_NO_EQUAL,							// 不精确匹配
	nicERR_HIS_PAGE_EMPTY,						// 页空
	nicERR_HIS_DATA_FILE_NOT_EXIST,				// 所查询的数据位于已移出系统的文件中
	nicERR_HIS_NO_DATA_FILE,						// 没有历史数据文件
	nicERR_HIS_BEYOND_COMPRESS,					// 时间比压缩结构中数据时间还要大
	nicERR_HIS_EQUAL_COMPRESS,					// 数据与压缩结构中数据时间相同
	nicERR_HIS_PAGE_NOT_OPEN,					// 页未打开
	nicERR_HIS_NO_DATA,							// 测点在文件中无对应的数据
	nicERR_HIS_NO_DATA_IN_INDEX_PAGE,			// 索引页无数据
	nicERR_HIS_NO_DATA_IN_DATA_PAGE,				// 数据页无数据
	// 读模块错误号
	nicERR_HIS_NEED_ADJUST_EndID_,					// 需要调整最后一个数据
	nicERR_HIS_NEED_FOUND_COMPRESS,				// 需要在压缩结构中查询数据	
	nicERR_HIS_BEYOND_MAX_READ_COUNT,			// 读取数据的个数超出最大读个数	
	//删除函数错误号
	nicERR_HIS_DELTIME_LESSTHAN_MIN,   			//删除函数的指定时间段比该测点最小时间还小
	nicERR_HIS_DELTIME_MORETHAN_MAX,   			//删除函数的指定时间段比该测点最大时间还大
	nicERR_HIS_BEYOND_NUMBER,					// 统计间隔数不应超过100      

	nicERR_EVENT_BASE = -18800 ,					// 事件相关错误

	nicERR_EVENTNOTIFY_BASE = -18700 ,			// 事件发布相关错误

	nicERR_EVENTNOTIFY_CANNOT_StartID_,				// 事件订阅发布模块不能启动
	nicERR_EVENTNOTIFY_SUBSCRIBE_MAP_ERROR,		// 事件订阅发布模块的订阅表错误
	nicERR_EVENTNOTIFY_PUBLISH_MAP_ERROR,		// 事件订阅发布模块的发布者表错误
	nicERR_EVENTNOTIFY_CONNECT_NOT_FOUND,		// 连接不存在

	nicERR_USER_BASE = -18600 ,					// 用户和用户组相关错误

	nicERR_USER_GROUPNAME_CONFLICT,              //用户组名冲突
	nicERR_USER_USERNAME_CONFLICT,               //用户名冲突
	nicERR_USER_USER_IN_GROUP,                   //向用户组添加组中存在的用户
	nicERR_USER_USER_NOT_IN_GROUP,               //从用户组删除不存在的用户
	nicERR_USER_REIGISTE_USERNAME_FAIL,          //向系统注册用户名出错
	nicERR_USER_REIGISTE_GROUPNAME_FAIL,         //向系统注册用户组名出错
	nicERR_USER_USERGROUP_NOT_EXIST,             //操作不存在的用户组
	nicERR_USER_USER_NOT_EXIST,                  //操作不存在的用户
	nicERR_USER_USERNAME_OR_PASSWORD_NOTMATCH,   //用户名和密码不匹配
	nicERR_USER_READ_PSUSERCONF_FAIL,            //用户和用户组配置文件读出错
	nicERR_USER_WRITE_PSUSERCONF_FAIL,           //用户和用户组配置文件写错误
	nicERR_USER_USERGROUP_ONLYREAD,              //用户组只读
	nicERR_USER_USER_ONLYREAD,                   //用户只读
	nicERR_USER_LOST_VITAL_DATA ,               //缺失必须字段
	nicERR_USER_NAME_LENGTH_WRONG,              //用户名或用户组名长度错误；
	nicERR_USER_NAME_ILLEGAL,                   //用户名或用户组名非法
	nicERR_USER_CONF_DESTROY,                   //用户配置文件被破坏
	nicERR_USER_CONF_BACKUP_FAIL,                   //用户配置文件备份失败

	nicERR_IOSS_BASE = -18500 ,					// IOSS相关错误号
	nicERR_IOSS_OBJECT_NOT_FOUND,
	nicERR_IOSS_DEVICE_NOT_STARTED,
	nicERR_IOSS_DRIVER_NOT_LOADED,

	nicERR_MAX = -10000 , 						// 接口返回最大错误号
} nicErrorCodeEnum;

CDECL_END

#endif ///  __Nicsys_ERROR_H__
