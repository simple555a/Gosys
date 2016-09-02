//= Author: Zander Sun
//= Date:2013-9-22
//= File Function: ��DPUDB_CURSOR�ĳ�Ա�������塣

//= UPDATE RECORD
//= Update for                    Version     Update by     Date         Update (function names modified)                            
//= (22-1)����״̬��ʾ����        v1.0.0.22   Zander Sun    2014-10-10   delay_small_switch()
//= 


#include "DPUDevice.h"

// СIP�ɱ��е��������е��ӳ��жϣ�״̬�������Ϊ�����Ű����õ�״̬������Ϊ����
// ���룺״̬��Ϣ����ֵ�ṹ
__bool DPUDB_CURSOR::delay_small_switch(DBData* val)
{
	// �����ǿ�����״̬����
	assert(curType == DBCURSOR_dpustatus);

	// �ж���СIP������
	if (dpustatus_cur.dpu != 0)
	{
		return __true;
	}

	// �ж�������״̬��Ϣ
	if (dpustatus_cur.statusNum != 0)
	{
		return __true;
	}

	// ����״̬Ϊ����������һʱ������״̬��ͬ
	if (val->value.Value.u8 == 0 && val->value.Value.u8 != last_val.value.Value.u8)
	{
		//
		if (delay_num > SMALL_IP_SWITCH_DELAY_NUM)  // Zander 2014-10-10   OLD: "=="
		{
			delay_num = 0;
		}else
		{
			val->value.Value.u8 = 1;
			delay_num++;
		}

	}else
	{
		if (val->value.Value.u8 == 1)
		{
			delay_num = 0;
		}
	}

	// ������һ�ζ�ȡ��״̬
	last_val = *val;

	return __true;
}