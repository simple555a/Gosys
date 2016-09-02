//= Update Record
//= Update for     Version       Update by       Date            Update                           
//= (19-1)         1.0.0.19      Zander Sun      2013-9-22       增加DPUDB_CURSOR的成员delay_small_switch()、last_val、delay_num

#ifndef __NICSYS_DEVDPU_DEVICE_H_
#define __NICSYS_DEVDPU_DEVICE_H_

#include "ONetType.h"
//#include "nicIOSSComm.h"
//#include "pmc_value_t.h"
#include "NetTask.h"


class CONetPctlDepository;

class DPUDB_CURSOR;

class CDPUDevice
{
public:
	friend DPUDB_CURSOR;
	CDPUDevice();
	~CDPUDevice();
	__i32 init_device(PDEVICE_INFO handle,ACE_Reactor * reactor);
	__i32 uninit_device();
	__i32 get_value(__u8 ioAddr,__u32 channel,DBData );

	//2015.03.17 GM 属性由private更改为public
	CNetTask * _pTask;		//协议仓库

private:
	ONet_DB _db;
	
	CONetPctlDepository *_pPctls;//协议仓库


};

//游标类型
enum DBCURSOR
{
	DBCURSOR_null =0,		//无效游标
	DBCURSOR_iovalue,		//IO值游标
	DBCURSOR_dpustatus,		//DPU状态游标
	DBCURSOR_iostatus,		//IO卡状态游标
	DBCURSOR_chstatus,		//通道状态游标
};
//数据缓冲区的游标
struct DPUDB_CURSOR_IOVALUE
{
	__u8 ioAddr;	//IO卡逻辑地址
	__i32 chAddr;	//通道编号
};
struct DPUDB_CURSOR_DPUSTATUS
{
	__u8 dpu;		//0,为主DPU，1为备
	__u32 statusNum;	//状态编号
};
struct DPUDB_CURSOR_IOSTATUS
{
	__u8 ioAddr;	//IO卡逻辑地址
	__u32 statusNum;	//状态编号
};
struct DPUDB_CURSOR_CHSTATUS
{
	__u8 ioAddr;	//IO卡逻辑地址
	__i32 chAddr;	//通道编号
};

class DPUDB_CURSOR
{
public:

	DPUDB_CURSOR(CDPUDevice *device):_device(device)
	{	

	}
	~DPUDB_CURSOR()
	{	
	}
	CDPUDevice * const _device;//指针为const不能更改指针指向
	__u8  curType;	//游标类型
	union
	{
		DPUDB_CURSOR_IOVALUE iovalue_cur;
		DPUDB_CURSOR_DPUSTATUS  dpustatus_cur;
		DPUDB_CURSOR_IOSTATUS  iostatus_cur;
		DPUDB_CURSOR_CHSTATUS  chstatus_cur;
	};
	__bool get_dpu_status(DBData *val)
	{
	//	_device->_db._data_mutex.acquire_read();
		// DPU类的指针为空，则返回
		if (!_device)
		{
			return __false;
		}

		if (!_device->_pTask->m_is_dpu_exist)
		{
			set_value_type(val->value.Flags, dt_null);
			val->value.Value.u8 = 0;
			//	_device->_db._data_mutex.release();
			return __false; // Zander
		}

		set_value_type(val->value.Flags, dt_uint8);
		switch(dpustatus_cur.statusNum)
		{
		case 0:			//DPU主备状态

			//状态信息已经在接收的时候被处理过
			if ((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x01 )
			{
				val->value.Value.u8 = 0; //主
			}
			else if((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x02 )
			{
				val->value.Value.u8 = 1; //备
			}
			else 
			{
				val->value.Value.u8 = 128; // 其他
				return __false;
			}
			break;
		case 1:			//DPU的CPU
			//val->Value.DataType = DBDataType_UInt8;
			//set_value_type(val->value.Flags, dt_uint8);
			val->value.Value.u8 = _device->_db._status[dpustatus_cur.dpu].cpu;
			break;
		case 2:			//DPU的MEM
			//val->Value.DataType = DBDataType_UInt8;
			//set_value_type(val->value.Flags, dt_uint8);
			val->value.Value.u8 =  _device->_db._status[dpustatus_cur.dpu].mem;
			break;
		case 3:			//DPU的温度
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 =  _device->_db._status[dpustatus_cur.dpu].temp;
			break;
			break;
		case 4:			//dpbus1的状态
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 = _device->_db._status[dpustatus_cur.dpu].dpBus1;
			break;
		case 5:			//dpbus1的状态
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 = _device->_db._status[dpustatus_cur.dpu].dpBus2;
			break;
		case 6:        //DPU的ERROR灯是否亮起
			if ((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x04 )
			{
				val->value.Value.u8 = 1; //故障
			}else
			{
				val->value.Value.u8 = 0;
			}
			break;
		case 7:   // DPU是否配置了备控制器
			if ((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x08)
			{
				val->value.Value.u8 = 1; // 已配置另一控制器为该控制器的冗余
			}else
			{
				val->value.Value.u8 = 0;
			}
			break;
		case 8:  // DPU是否切换锁死
			if ((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x10)
			{
				val->value.Value.u8 = 1; // 切换锁定，无法冗余切换
			}else
			{
				val->value.Value.u8 = 0;
			}
			break;
		default://其他的DPU状态信息暂时不提供
			val->value.Value.u8 = 128;
			return __false;
			break;
		}
	//	_device->_db._data_mutex.release();
		return __true;

	}
	//IO卡的状态初始化定义：初始态0x01; 配置态0x02; 运行态0x11; 死态0x4F; 错误态0x41; 通讯中断0x42; 配置错误0x44 */
	__bool get_iocard_status(DBData *val)
	{
		if (!_device)
		{
			return __false;
		}

	   if (!_device->_pTask->m_is_dpu_exist)
		{
			set_value_type(val->value.Flags, dt_null);
			val->value.Value.u8 = 0;
			//	_device->_db._data_mutex.release();
			return __false; // Zander
		}

	
		//	_device->_db._data_mutex.acquire_read();
		set_value_type(val->value.Flags, dt_uint8);
		switch(iostatus_cur.statusNum)
		{
		case 0:			//IO卡状态 0为正常，1为错误，2为停电或者被拔不存在状态
			//val->Value.DataType = DBDataType_UInt8;
			if(_device->_db.ioCards[iostatus_cur.ioAddr].isExist == __false)
			{
				val->value.Value.u8 = 2;
	//			_device->_db._data_mutex.release();
				return __false;
			}
			if ( _device->_db.ioCards[iostatus_cur.ioAddr].ioStatus & 0x11)
			{
				val->value.Value.u8 = 0;
			}
			else if( 
				_device->_db.ioCards[iostatus_cur.ioAddr].ioStatus & 0x01 ||
				_device->_db.ioCards[iostatus_cur.ioAddr].ioStatus & 0x02 ||	
				_device->_db.ioCards[iostatus_cur.ioAddr].ioStatus & 0x41 || 
				_device->_db.ioCards[iostatus_cur.ioAddr].ioStatus & 0x4F||
				_device->_db.ioCards[iostatus_cur.ioAddr].ioStatus & 0x42||
				_device->_db.ioCards[iostatus_cur.ioAddr].ioStatus & 0x44)
			{
				val->value.Value.u8 = 1;
			}
			else	//其他的都算2
			{
				val->value.Value.u8 = 2;
			}
			break;
		default:	//IO卡其他的参数暂时不提供
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 = 0;
			break;
		}
	//	_device->_db._data_mutex.release();
		return __true;

	}
	__bool get_channel_status(DBData *val)
	{
		if (!_device)
		{
			return __false;
		}
		if (!_device->_pTask->m_is_dpu_exist)
		{
			set_value_type(val->value.Flags, dt_null);
			val->value.Value.u8 = 0;
			//	_device->_db._data_mutex.release();
			return __false; // Zander
		}
	    

		set_value_type(val->value.Flags, dt_uint8);
		//当卡件被拔出时，应该置无效,卡件不正常运行通道也无效
		if(_device->_db.ioCards[iostatus_cur.ioAddr].ioStatus != 0x11)
		{
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 = 1;
			return __true;
		}
	//	_device->_db._data_mutex.acquire_read();
		//当通道号大于IO卡的当前有的通道号时，返回错误态
		if ( chstatus_cur.chAddr > _device->_db.ioCards[chstatus_cur.ioAddr].ioCurNum || (!(_device->_db.ioCards[iovalue_cur.ioAddr].channels)) )
		{
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 = 1;
	//		_device->_db._data_mutex.release();
			return __true;
		}
		//val->Value.DataType = DBDataType_UInt8;	//接收上来的质量戳为1是有效，要转换成无故障的0
		val->value.Value.u8 = (_device-> _db.ioCards[chstatus_cur.ioAddr].channels[chstatus_cur.chAddr].status & 0x01)== 1? 0 :1;
	//	_device->_db._data_mutex.release();
		return __true;

	}
	// Zander  void返回值改为了__bool
	__bool get_value(DBData *val)
	{
		if (!_device)
		{
			// VV
			printf("NO device!!!\n");
			return __false;
		}

		if (!_device->_pTask->m_is_dpu_exist)
		{
			set_value_type(val->value.Flags, dt_null);
			val->value.Value.u8 = 0;
			//VV
			printf("NO DPU EXIST!!!!\n");
			return __false; // Zander
		}

	//	_device->_db._data_mutex.acquire_read();
		if (iovalue_cur.chAddr >  _device->_db.ioCards[iovalue_cur.ioAddr].ioCurNum|| (!(_device->_db.ioCards[iovalue_cur.ioAddr].channels ))
			|| _device->_db.ioCards[iovalue_cur.ioAddr].isExist == __false)//Zander 2013-7-30
		{
			//val->Value.DataType = DBDataType_Empty;
			set_value_type(val->value.Flags, dt_null);
			val->value.Value.u8 = 0;
		//	_device->_db._data_mutex.release();
		//	printf("NO IO addr = %d \n", iovalue_cur.ioAddr);
			return __false; // Zander
		}
		*val =  _device-> _db.ioCards[iovalue_cur.ioAddr].channels[iovalue_cur.chAddr].rData;
		return __true; // Zander
	//	_device->_db._data_mutex.release();
	}
	int set_value(pmc_value_t val)
	{
		if (!_device)
		{
			return __false;
		}

	//	_device->_db._data_mutex.acquire_read();
		if (iovalue_cur.chAddr >  _device->_db.ioCards[iovalue_cur.ioAddr].ioCurNum ||
			(!(_device->_db.ioCards[iovalue_cur.ioAddr].channels)) )
		{
	//		_device->_db._data_mutex.release();
			return RET_Fail;
		}
		if(_device->_db.ioCards[iovalue_cur.ioAddr].channels[iovalue_cur.chAddr].ioAddr != VPTGROUP &&
			_device->_db.ioCards[iovalue_cur.ioAddr].channels[iovalue_cur.chAddr].ioAddr  != RETVPTGROUP)
		{
		//	_device->_db._data_mutex.release();
			return RET_Fail;//暂时只支持对VPT的写操作
		}
		_device->_pTask->send_set_vpt_value(
			_device->_db.ioCards[iovalue_cur.ioAddr].channels[iovalue_cur.chAddr].dataType,
			_device->_db.ioCards[iovalue_cur.ioAddr].channels[iovalue_cur.chAddr].ioAddr,
			_device->_db.ioCards[iovalue_cur.ioAddr].channels[iovalue_cur.chAddr].offAddr,
			_device->_db.ioCards[iovalue_cur.ioAddr].channels[iovalue_cur.chAddr].subOffAddr,
			val
			);
	//	_device->_db._data_mutex.release();
		return RET_OK;
	}

	// Zander 2013-9-22
	// 小IP由备切到主的延时判断
	__bool delay_small_switch(DBData* val);
	DBData last_val;  // 上一次读取的状态或数值
	__u8 delay_num;   // 连续读取小IP为主的次数
};


#endif
