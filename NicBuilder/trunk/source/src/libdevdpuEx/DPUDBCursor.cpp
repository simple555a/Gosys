//= Author: Zander Sun
//= Date:2013-9-22
//= File Function: 类DPUDB_CURSOR的成员函数定义。

//= UPDATE RECORD
//= Update for                    Version     Update by     Date         Update (function names modified)                            
//= (22-1)主备状态显示错误        v1.0.0.22   Zander Sun    2014-10-10   delay_small_switch()
//= 


#include "DPUDevice.h"

// 小IP由备切到主过程中的延迟判断：状态连续多次为主，才把所得的状态变量置为主。
// 输入：状态信息变量值结构
__bool DPUDB_CURSOR::delay_small_switch(DBData* val)
{
	// 必须是控制器状态类型
	assert(curType == DBCURSOR_dpustatus);

	// 判断是小IP控制器
	if (dpustatus_cur.dpu != 0)
	{
		return __true;
	}

	// 判断是主备状态信息
	if (dpustatus_cur.statusNum != 0)
	{
		return __true;
	}

	// 主备状态为主，且与上一时刻主备状态不同
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

	// 更新上一次读取的状态
	last_val = *val;

	return __true;
}