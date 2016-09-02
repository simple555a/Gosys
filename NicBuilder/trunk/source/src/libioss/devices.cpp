#include "precomp.h"

DEVICE_LIST	g_Devices;


/*
�������ܣ�������ǩ�Ķ�̬�������侲̬������ָ�����豸
����˵����
          tag    �������ı�ǩ����Ϣ
		  d      �������豸����Ϣ
�� �� ֵ����
*/
void attach_tag_to_device(PRTK_TAG tag, PDEVICE_INFO d)
{
	//1 �˺�������֮ǰ��tag�Ķ�̬�����б���û���豸��Ϣ��û�й����豸
	assert(!tag->d.DeviceObj);
	assert(RtkIsListEmpty(&tag->d.DeviceLink));
	
	//2 ������ǩ���豸
	tag->d.DeviceObj = d;
	
	//3 ���豸��Ϣ�е�������ϢΪ�գ���������
	if(!d->d){
		return;
	}

	/*
		2005-7-30
		the address translation interface is supersuded by 
		the newer DISP_DB_ADD_TAG message passed to
		the dispatch interface, if implemented.
	*/
	//4 ���豸��������ӱ�ǩ���address_translate
	if(d->d->dispatch){
		d->d->dispatch(d, DISP_DB_ADD_TAG, (int)tag);
		tag->d.Value.Flags |= TF_Translated;
	}else{
		tag->d.Value.Flags &= ~TF_Translated;
		if(d->d->address_translate){
			/*
				note, during address translation, the device driver
				might set other flags in tag->d.Value.Flags
			*/
			if(d->d->address_translate(tag)){
				tag->d.Value.Flags |= TF_Translated;
			}
		}else{
			// device tag address needs no translation
			tag->d.Value.Flags |= TF_Translated;
		}
	}
    
	//5 ���豸��Ϣ���ǩ�໥�������Թ���
	RtkInsertTailList(&d->tags, &tag->d.DeviceLink);
}

/*
�������ܣ������豸��������dispatch()��ȥ���豸�е�ָ����ǩ��������ǩ�й������豸��Ϣָ����0
����˵����
          tag   ָ���ɾ���ı�ǩ�����ݽṹ(RTK_TAG)��ָ��
����ֵ��  ��
*/
void detach_tag_from_device(PRTK_TAG tag)
{
	PDRIVER_INFO drv;

	// TODO: notify the device we're depriving this tag
	//1 
	if(tag->d.DeviceObj){
		drv = tag->d.DeviceObj->d;
		if(drv && drv->dispatch){
			drv->dispatch(tag->d.DeviceObj, DISP_DB_DROP_TAG, (int)tag);
		}
		// detach tag from the old device
		RtkRemoveEntryList(&tag->d.DeviceLink);
		RtkInitializeListHead(&tag->d.DeviceLink);
	}else{
		assert(RtkIsListEmpty(&tag->d.DeviceLink));
	}

	tag->d.DeviceObj = 0;
}

/*
���ܣ��������б�ǩ�㣬�ҵ���̬���õ��豸��ָ���豸��ͬ�ı�ǩ�㣬�������豸����Ҳ��������
������
      d     ָ�����豸�ṹ��
���أ���
*/
void attach_device_tags(PDEVICE_INFO d)
{
	RTK_CURSOR handle;
	PRTK_GROUP p;

	handle = cursor_open_first_subitem(HNODE_LOCAL_MACHINE);
	while(!cursor_is_end(handle)){
		p = (PRTK_GROUP)cursor_get_item(handle);
		RTK_CURSOR hTag;
		PRTK_TAG   pTag;
		hTag = cursor_open_first_subitem(handle);
		while(pTag = (PRTK_TAG)cursor_get_item(hTag)){
			if(pTag->s.Device == d->k){
				attach_tag_to_device(pTag, d);
			}
			cursor_move_next(hTag);
		}
		close_handle(hTag);
		cursor_move_next(handle);
	}
	close_handle(handle);
}


/*
	caller must have DB locked exclusively.
*/
/*
���ܣ�ɾ��ָ���豸���������б�ǩ��
������
      dev   ָ�����豸��Ϣ
���أ���
*/
static void detach_device_tags(PDEVICE_INFO dev)
{
	PRTK_TAG tag;
	PRTK_LIST_ENTRY head, entry, next;
	head = &dev->tags;
	entry = head->Flink;
	while(entry != head){
		next = entry->Flink;
		tag = RTK_CONTAINING_RECORD(entry, RTK_TAG, d.DeviceLink);//���豸��Ϣ�е������Ƴ����豸�����ı�ǩ���RTKTAG
#if 0
		assert(tag->d.DeviceObj == dev);
		RtkRemoveEntryList(entry);
		RtkInitializeListHead(entry);
		tag->d.DeviceObj = 0;
#else
		detach_tag_from_device(tag);
#endif
		entry = next;
	}
}

/*
	create a device object and insert it into system's
	device list.

	Enrironment :
	db lock acquired exclusively
*/
/**/
/*
���ܣ����豸���ƺ�������Ϣ�����豸��Ϣ������DEVICE_LIST  g_Devices��
������
      diverObj         ������Ϣ
	  key              �豸����
	  extension_size   Ԥ����
���أ�
*/
IO_API PDEVICE_INFO PMC_API io_create_device(
	PDRIVER_INFO driverObj,
	PCDEVICE_KEY key,
	__uint extension_size
	)
{
	PDEVICE_INFO dev;
	pair<DEVICE_LIST::iterator, bool> pr;
	//1 ���ٴ洢�豸��Ϣ���ڴ�
	dev = (PDEVICE_INFO)new char[sizeof(DEVICE_INFO) + extension_size];
	if(!dev){
		return 0;
	}
	ZeroMemory(dev, sizeof(DEVICE_INFO) + extension_size);
	//2 �����豸����
	dev->k = *key;
	make_unified_key(dev->k);
	//3 �����豸������Ϣ
	dev->d = driverObj;
	//4 �����������豸����+1
	if(driverObj){
		driverObj->device_count++;
	}



	//5 ��ʼ���豸��Ϣ������
	RtkInitializeListHead(&dev->tags);

	//6 �����ɵ��豸��Ϣ�����豸�б�g_Device
	pr = g_Devices.insert(DEVICE_LIST::value_type(*key, dev));
	return dev;
}

/*
	remove a device object from system's device list

	Environment:

	db lock acquired exclusively.
*/
/*
�������ܣ����豸��ϵͳ�豸�б���ɾ��
��    ����
          dev  ��ɾ�����豸��Ϣ�ṹ��
����ֵ��  ֻ��__true��û�д���ֵ����
*/
IO_API __bool PMC_API io_delete_device(PDEVICE_INFO dev)
{
	PDRIVER_INFO driverObj;
	DEVICE_LIST::iterator it;
    
	//1 ����豸����ر�ǩ�������
	detach_device_tags(dev);
    
	//2 �ͷ�g_Devices�и��豸��Ϣ
	it = g_Devices.find(dev->k);
	assert(it != g_Devices.end());
	//3 �ͷ�ǰ���ҵ����豸��Ϣ������������Ϣ�ĵ�ַ
	driverObj = dev->d;
	g_Devices.erase(it);
	delete dev;

	//4 ������Ϣ�Ĵ���
	if(driverObj){
		//4.1 ������Ϣ�еĹ����豸������1
		driverObj->device_count--;
		assert(driverObj->device_count >= 0);
		//4.2 ������������Ϣ�еĹ����豸����Ϊ0����д������DLL
		if(!driverObj->device_count){
			io_unload_driver(driverObj);
		}
	}	
	
	return __true;
}

/*
	enumerate device list

	Environment:

	db lock acquired either exclusively or non-exclusively
*/
/*
�������ܣ����豸�б���ȡ���豸��Ϣ
������
         buffer    ���ڴ��ȡ�����豸��Ϣ��DEVICE_INFO����     [IN]
		 maxcount  �豸�б��е��豸����                        [OUT]
����ֵ�� ʵ��ȡ�����豸��Ϣ�ĸ���(�豸�б���Ԫ�صĸ�����maxcount�Ľ�Сֵ)
*/
IO_API __uint PMC_API io_get_devices(
	DEVICE_INFO * buffer, 
	__uint *maxcount
	)
{
	__uint count, i;
	count = min(g_Devices.size(), *maxcount);
	DEVICE_LIST::iterator p;
	p = g_Devices.begin();
	for(i=0; i<count; i++, p++){
		buffer[i] = *(p->second);
	}
	if(*maxcount < g_Devices.size()){
		*maxcount = g_Devices.size();
	}
	return count;
}

/*
	search for a device

	Environment :
	
	db lock acquired, either read or write
*/
/*
�������ܣ����豸�б��еõ�ָ�����Ƶ��豸��Ϣ
������
          key   ָ���豸���Ƶ�ָ��
����ֵ��  ȡ�����豸��Ϣ�ṹ���ָ��
*/
IO_API PDEVICE_INFO PMC_API io_open_device(PCDEVICE_KEY key)
{
	DEVICE_LIST::iterator it;
	it = g_Devices.find(*key);
	if(it == g_Devices.end()){
		return 0;
	}
	return it->second;
}

/*
	caller must have DB locked exclusively
*/
/*
�������ܣ������豸�����ĵ��������������豸
������
          dev  ���������豸 
����ֵ��  0         �豸����δ����
          1         �豸���������������豸�ĵ�������ָ��Ϊ0
			   ��   �����ɹ�   
*/
IO_API __bool PMC_API io_start_device(PDEVICE_INFO dev)
{
	__bool r;	

	// a fake driver is created even if the module cannot be loaded
	assert(dev->d);
	
	//
	if(!(dev->d->flags & DRIVER_FLAG_LOADED)){
		dev->error = IOSS_DRIVER_NOT_LOADED;
		r = __false;
	}else{
		if(!dev->d->start_device){
			r = __true;             //��ʾ�Ѿ������ˣ������Է���__true?
		}else{
			r = dev->d->start_device(dev);
			if(!r){
				dev->error = IOSS_DEVICE_NOT_STARTED;
			}
		}
	}
	
	if(!r){
		dev->flags &= ~DF_Active;
	}else{
		dev->flags |= DF_Active;
		dev->error = 0;
	}
	
	if(r){
		utils_trace(
			"Device %s started.\n", 
			(char*)CDeviceName(dev->k)
			);
	}else{
		utils_error(
			"Cannot start device %s.\n", 
			(char*)CDeviceName(dev->k)
			);
	}
	
	return r;
}

/*
���ܣ������豸���������ĺ�����ָֹͣ�����豸
������
      dev    ��ֹͣ���豸
���أ�0      ֹͣ�豸�ĵ�������ִ��ʧ��
      1      �豸��ֹͣ
	      �� ֹͣ�豸�����ɹ�
*/
/*
	caller must have DB locked exclusively.
*/
IO_API __bool PMC_API io_stop_device(PDEVICE_INFO dev)
{	
	if( !(dev->flags & DF_Active) ){
		return __true;//�豸�Ѿ�ֹͣ
	}
	utils_trace("Stopping device %s\n", (char*)CDeviceName(dev->k));
	// newly created devices might not be attached to driver yet
	if(dev->d){
		if(dev->d->stop_device){
	        //stop_device()����0�����ʾֹͣ�豸ʧ��
			if(!dev->d->stop_device(dev)){
				dev->flags &= ~DF_Active;
				utils_error(
					"Device %s not responding to STOP command.\n", 
					(char*)CDeviceName(dev->k)
					);
				return __false;
			}
		}
	}
	dev->flags &= ~DF_Active;
	utils_trace("Device %s stopped\n", (char*)CDeviceName(dev->k));
	
	return __true;
}

