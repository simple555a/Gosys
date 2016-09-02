#include "precomp.h"
#pragma hdrstop

#pragma comment (lib, "librtk.lib")
#pragma comment (lib, "libutils.lib")
#pragma comment (lib, "librtdb.lib")


IOSS_DLL_EXPORT __bool start_device(PDEVICE_INFO handle)
{
	COpcDevice * d;
	
	d = new COpcDevice();

	if(!d){
		utils_error("Error(opc): allocating opc object.\n");
		return __false;
	}

	int n;
	n = MultiByteToWideChar(
		0, 
		0, 
		handle->address, 
		-1, 
		d->m_opcSource, 
		OPC_SRC_LENGTH
		);
	
	if(!d->config(handle->parameter)){
		utils_error("Device `%s' configuration error.\n", (char *)CDeviceName(handle->k));
		return __false;
	}

	// ok, start the thread
	handle->OwnerField = d;

	// d->on_msg
	if(!d->start()){
		goto __failed;
	}

	if(d->wait(CRtkThread::EVT_INIT_COMPLETE, 15000)!=WAIT_OBJECT_0){
		utils_error(
			"OPC>> Warning : device %s initialization timeout.\n", 
			(char*)CDeviceName(handle->k)
			);
	}

	return __true;

__failed:
	handle->OwnerField = 0;
	d->destructor();
	return __false;
}

IOSS_DLL_EXPORT __bool stop_device(PDEVICE_INFO handle)
{
	COpcDevice * dev;
	dev = (COpcDevice *)handle->OwnerField;

	if(!dev){
		return 0;
	}

	if(!dev->stop(5000)){
		utils_error(
			"Warning : opc device on '%s' didn't stop in 5 seconds.\n",
			handle->address
			);
	}
	dev->destructor();
	handle->OwnerField = 0;
	
	return __true;
}

IOSS_DLL_EXPORT void update_tag(
	PCRTK_TAG tte, 
	pmc_value_t * pt, 
	PCRTK_TIME)
{
	COpcDevice * d;
	d = (COpcDevice*)tte->d.DeviceObj->OwnerField;
	if(d){
		d->readOneTag(tte, pt);
	}
}

static void _translate(PRTK_TAG tte)
{
	COpcDevice * d;
	d = (COpcDevice*)tte->d.DeviceObj->OwnerField;
	if(!d){
		return;
	}

	if(d->addOneTag(tte)){
		tte->d.Value.Flags |= TF_Translated;
	}
}

IOSS_DLL_EXPORT __bool write_device(PRTK_TAG tte, PCTAG_VALUE value)
{
	COpcDevice * d;
	d = (COpcDevice*)tte->d.DeviceObj->OwnerField;
	if(!d){
		return __false;
	}

	PMC_OPC_TAG * tag;
	tag = *((PMC_OPC_TAG**)tte->d.BinaryAddress);
	if(!tag){
		return __false;
	}
	if(!tag->pItemResult){
		return __false;
	}
	
	write_device_parameter * wdp;
	wdp=new write_device_parameter;
	if(!wdp){
		DEBUG_PRINTF((
			">> opc , Shit!! error new write_device_parameter\n"
			));
		return __false;
	}

	wdp->hItem = tag->pItemResult->hServer;

	pmc_value_t v;
	v.Flags = tte->d.Value.Flags;
	v.Value = *value;
	pmc_value_to_variant(&v, &wdp->newValue);

	__bool ret = d->post_message(WM_USER+2, (WPARAM)wdp, 0);
	if(!ret){
		delete wdp;
	}
	return ret;
}

IOSS_DLL_EXPORT IOSS_STATUS PMC_API dispatch(
	PDEVICE_INFO device,
	int majorCode,
	int param
	)
{
	PRTK_TAG tte;
	COpcDevice * dev;
		
	switch(majorCode){
	case DISP_DB_ADD_TAG:
		_translate((PRTK_TAG)param);
		break;
	case DISP_DB_DROP_TAG:
		tte = (PRTK_TAG)param;
		dev = (COpcDevice*)tte->d.DeviceObj->OwnerField;
		if(!dev){
			return IOSS_FAILED;
		}
		dev->RemoveOneTag(tte);
		break;
	}

	return IOSS_SUCCESS;
}

