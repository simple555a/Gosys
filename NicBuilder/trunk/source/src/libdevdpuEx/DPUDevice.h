//= Update Record
//= Update for     Version       Update by       Date            Update                           
//= (19-1)         1.0.0.19      Zander Sun      2013-9-22       ����DPUDB_CURSOR�ĳ�Աdelay_small_switch()��last_val��delay_num

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

	//2015.03.17 GM ������private����Ϊpublic
	CNetTask * _pTask;		//Э��ֿ�

private:
	ONet_DB _db;
	
	CONetPctlDepository *_pPctls;//Э��ֿ�


};

//�α�����
enum DBCURSOR
{
	DBCURSOR_null =0,		//��Ч�α�
	DBCURSOR_iovalue,		//IOֵ�α�
	DBCURSOR_dpustatus,		//DPU״̬�α�
	DBCURSOR_iostatus,		//IO��״̬�α�
	DBCURSOR_chstatus,		//ͨ��״̬�α�
};
//���ݻ��������α�
struct DPUDB_CURSOR_IOVALUE
{
	__u8 ioAddr;	//IO���߼���ַ
	__i32 chAddr;	//ͨ�����
};
struct DPUDB_CURSOR_DPUSTATUS
{
	__u8 dpu;		//0,Ϊ��DPU��1Ϊ��
	__u32 statusNum;	//״̬���
};
struct DPUDB_CURSOR_IOSTATUS
{
	__u8 ioAddr;	//IO���߼���ַ
	__u32 statusNum;	//״̬���
};
struct DPUDB_CURSOR_CHSTATUS
{
	__u8 ioAddr;	//IO���߼���ַ
	__i32 chAddr;	//ͨ�����
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
	CDPUDevice * const _device;//ָ��Ϊconst���ܸ���ָ��ָ��
	__u8  curType;	//�α�����
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
		// DPU���ָ��Ϊ�գ��򷵻�
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
		case 0:			//DPU����״̬

			//״̬��Ϣ�Ѿ��ڽ��յ�ʱ�򱻴����
			if ((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x01 )
			{
				val->value.Value.u8 = 0; //��
			}
			else if((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x02 )
			{
				val->value.Value.u8 = 1; //��
			}
			else 
			{
				val->value.Value.u8 = 128; // ����
				return __false;
			}
			break;
		case 1:			//DPU��CPU
			//val->Value.DataType = DBDataType_UInt8;
			//set_value_type(val->value.Flags, dt_uint8);
			val->value.Value.u8 = _device->_db._status[dpustatus_cur.dpu].cpu;
			break;
		case 2:			//DPU��MEM
			//val->Value.DataType = DBDataType_UInt8;
			//set_value_type(val->value.Flags, dt_uint8);
			val->value.Value.u8 =  _device->_db._status[dpustatus_cur.dpu].mem;
			break;
		case 3:			//DPU���¶�
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 =  _device->_db._status[dpustatus_cur.dpu].temp;
			break;
			break;
		case 4:			//dpbus1��״̬
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 = _device->_db._status[dpustatus_cur.dpu].dpBus1;
			break;
		case 5:			//dpbus1��״̬
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 = _device->_db._status[dpustatus_cur.dpu].dpBus2;
			break;
		case 6:        //DPU��ERROR���Ƿ�����
			if ((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x04 )
			{
				val->value.Value.u8 = 1; //����
			}else
			{
				val->value.Value.u8 = 0;
			}
			break;
		case 7:   // DPU�Ƿ������˱�������
			if ((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x08)
			{
				val->value.Value.u8 = 1; // ��������һ������Ϊ�ÿ�����������
			}else
			{
				val->value.Value.u8 = 0;
			}
			break;
		case 8:  // DPU�Ƿ��л�����
			if ((__u8)(_device->_db._staticAttr[dpustatus_cur.dpu].dpuStatus) & 0x10)
			{
				val->value.Value.u8 = 1; // �л��������޷������л�
			}else
			{
				val->value.Value.u8 = 0;
			}
			break;
		default://������DPU״̬��Ϣ��ʱ���ṩ
			val->value.Value.u8 = 128;
			return __false;
			break;
		}
	//	_device->_db._data_mutex.release();
		return __true;

	}
	//IO����״̬��ʼ�����壺��ʼ̬0x01; ����̬0x02; ����̬0x11; ��̬0x4F; ����̬0x41; ͨѶ�ж�0x42; ���ô���0x44 */
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
		case 0:			//IO��״̬ 0Ϊ������1Ϊ����2Ϊͣ����߱��β�����״̬
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
			else	//�����Ķ���2
			{
				val->value.Value.u8 = 2;
			}
			break;
		default:	//IO�������Ĳ�����ʱ���ṩ
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
		//���������γ�ʱ��Ӧ������Ч,��������������ͨ��Ҳ��Ч
		if(_device->_db.ioCards[iostatus_cur.ioAddr].ioStatus != 0x11)
		{
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 = 1;
			return __true;
		}
	//	_device->_db._data_mutex.acquire_read();
		//��ͨ���Ŵ���IO���ĵ�ǰ�е�ͨ����ʱ�����ش���̬
		if ( chstatus_cur.chAddr > _device->_db.ioCards[chstatus_cur.ioAddr].ioCurNum || (!(_device->_db.ioCards[iovalue_cur.ioAddr].channels)) )
		{
			//val->Value.DataType = DBDataType_UInt8;
			val->value.Value.u8 = 1;
	//		_device->_db._data_mutex.release();
			return __true;
		}
		//val->Value.DataType = DBDataType_UInt8;	//����������������Ϊ1����Ч��Ҫת�����޹��ϵ�0
		val->value.Value.u8 = (_device-> _db.ioCards[chstatus_cur.ioAddr].channels[chstatus_cur.chAddr].status & 0x01)== 1? 0 :1;
	//	_device->_db._data_mutex.release();
		return __true;

	}
	// Zander  void����ֵ��Ϊ��__bool
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
			return RET_Fail;//��ʱֻ֧�ֶ�VPT��д����
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
	// СIP�ɱ��е�������ʱ�ж�
	__bool delay_small_switch(DBData* val);
	DBData last_val;  // ��һ�ζ�ȡ��״̬����ֵ
	__u8 delay_num;   // ������ȡСIPΪ���Ĵ���
};


#endif
