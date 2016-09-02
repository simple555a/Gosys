#include"pmc.h"
#include"SeiralScheduler.h"

#pragma comment(lib,"librtk.lib")
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtdb.lib")

extern CLock commlocker;

 CRPNScheduler *pnt = 0;

 __bool load(
	PDRIVER_INFO driverObj
	)
 {
	 return __true;
 }

 __bool unload()
 {
	 return __true;
 }

 //设备地址为"COM##"
 //设备参数为"57600,m,8,1" 波特率，奇偶校验，字长，停止位
 __bool start_device(
	PDEVICE_INFO handle
	)
 {
	 char temp[64] = {0};
	 memcpy(temp,handle->address,64);
	 char *p = strstr(temp,"COM");
	 int pn = atoi(p+3);
	 //端口号在0和6范围内
	 if(pn <= 0 || pn >6) return __false;

	 if(pnt==0)
	 {
		 pnt = new CRPNScheduler(pn,handle->parameter);
		 pnt->open();
		 pnt->wait(CRtkThread::EVT_INIT_COMPLETE,500);
		 pnt->start();
	 }


	 return __true;
 }

 __bool stop_device(
	PDEVICE_INFO handle
	)
 {
	 if(pnt)
	 {
		 pnt->stop(5000);
		delete pnt;
		pnt = 0;
	 }

		 return __true;
 }

 __bool write_device(
	PRTK_TAG tte, 
	PCTAG_VALUE value
	)
 {
	 	 return __true;
 }
 //设备地址为“S/M/P:##:%%”
 //S：源量程
 //M:中间量程
 //P:功率量程
 //##:偏移首地址
 //%%:开关量在字节中的偏移量
 void update_tag(
	PCRTK_TAG tte, 
	struct pmc_value_t * pt, 
	PCRTK_TIME now
	)
	{
		pmc_value_t flVal;
		pmc_value_t blVal;
		CArgs args;
		parse_arg_ex(tte->s.Address, &args, ":,;-");

		pt->Flags &= ~TF_Valid;
		flVal.Flags &= ~TF_Valid;
		blVal.Flags &= ~TF_Valid;
		set_value_type(flVal.Flags, dt_real4);
		set_value_type(blVal.Flags, dt_bool);

		commlocker.Lock();
		//模拟量
		if(args.argc ==2)
		{
			//模拟量偏移地址
			int index = atoi(args.argv[1]);
			if(index>96 || index < 5)
				commlocker.UnLock();
				return;
			switch(args.argv[0][0])
			{
			case 'S':
				flVal.Value.fltValue = *(__r4*)(pnt->rpn_rng_buff+index);
				flVal.Flags |= TF_Valid;
				break;
			case 'M':
				flVal.Value.fltValue = *(__r4*)(pnt->rpn_mid_buff+index);
				flVal.Flags |= TF_Valid;
				break;
			case 'P':
				flVal.Value.fltValue = *(__r4*)(pnt->rpn_pow_buff+index);
				flVal.Flags |= TF_Valid;
				break;
			default:
				break;
			}
			pmc_type_cast(&flVal, pt);
		}
		else if(args.argc ==3)
		{
			//开关量偏移地址
			int index = atoi(args.argv[1]);
			int offset = atoi(args.argv[2]);
			if(index>15 || index < 6||offset <0 ||offset>7)
			{
					commlocker.UnLock();
					return;
			}

			switch(args.argv[0][0])
			{
			case 'S':
				blVal.Value.b = (pnt->rpn_rng_buff[index] & (0x01<<offset))?__true:__false;
				blVal.Flags |= TF_Valid;
				break;
			case 'M':
				blVal.Value.b = (pnt->rpn_mid_buff[index]& (0x01<<offset))?__true:__false;
				blVal.Flags |= TF_Valid;
				break;
			case 'P':
				blVal.Value.b = (pnt->rpn_pow_buff[index]& (0x01<<offset))?__true:__false;
				blVal.Flags |= TF_Valid;
				break;
			default:
				break;
			}
			pmc_type_cast(&blVal, pt);
		}

	commlocker.UnLock();

	}


/*
 __bool address_translate(
	PRTK_TAG tte
	)
 {
		 return __true;
 }

 IOSS_STATUS PMC_API dispatch(
	PDEVICE_INFO device,
	int majorCode,
	int param
	)
{
		 return IOSS_SUCCESS;
}*/