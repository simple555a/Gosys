#include "precomp.h"

DEVICE_LIST	g_Devices;


/*
函数功能：关联标签的动态属性与其静态属性中指定的设备
参数说明：
          tag    待关联的标签的信息
		  d      待关联设备的信息
返 回 值：无
*/
void attach_tag_to_device(PRTK_TAG tag, PDEVICE_INFO d)
{
	//1 此函数操作之前，tag的动态属性中必须没有设备信息，没有关联设备
	assert(!tag->d.DeviceObj);
	assert(RtkIsListEmpty(&tag->d.DeviceLink));
	
	//2 关联标签和设备
	tag->d.DeviceObj = d;
	
	//3 若设备信息中的驱动信息为空，则函数返回
	if(!d->d){
		return;
	}

	/*
		2005-7-30
		the address translation interface is supersuded by 
		the newer DISP_DB_ADD_TAG message passed to
		the dispatch interface, if implemented.
	*/
	//4 在设备驱动中添加标签点或address_translate
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
    
	//5 将设备信息与标签相互关联，自关联
	RtkInsertTailList(&d->tags, &tag->d.DeviceLink);
}

/*
函数功能：调用设备驱动函数dispatch()，去掉设备中的指定标签，并将标签中关联的设备信息指针置0
参数说明：
          tag   指向待删除的标签的数据结构(RTK_TAG)的指针
返回值：  无
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
功能：遍历所有标签点，找到静态配置的设备与指定设备相同的标签点，将其与设备驱动也关联起来
参数：
      d     指定的设备结构体
返回：无
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
功能：删除指定设备关联的所有标签点
参数：
      dev   指定的设备信息
返回：无
*/
static void detach_device_tags(PDEVICE_INFO dev)
{
	PRTK_TAG tag;
	PRTK_LIST_ENTRY head, entry, next;
	head = &dev->tags;
	entry = head->Flink;
	while(entry != head){
		next = entry->Flink;
		tag = RTK_CONTAINING_RECORD(entry, RTK_TAG, d.DeviceLink);//由设备信息中的链表反推出该设备关联的标签点的RTKTAG
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
功能：由设备名称和驱动信息生成设备信息，插入DEVICE_LIST  g_Devices中
参数：
      diverObj         驱动信息
	  key              设备名称
	  extension_size   预留区
返回：
*/
IO_API PDEVICE_INFO PMC_API io_create_device(
	PDRIVER_INFO driverObj,
	PCDEVICE_KEY key,
	__uint extension_size
	)
{
	PDEVICE_INFO dev;
	pair<DEVICE_LIST::iterator, bool> pr;
	//1 开辟存储设备信息的内存
	dev = (PDEVICE_INFO)new char[sizeof(DEVICE_INFO) + extension_size];
	if(!dev){
		return 0;
	}
	ZeroMemory(dev, sizeof(DEVICE_INFO) + extension_size);
	//2 填入设备名称
	dev->k = *key;
	make_unified_key(dev->k);
	//3 填入设备驱动信息
	dev->d = driverObj;
	//4 驱动关联的设备数量+1
	if(driverObj){
		driverObj->device_count++;
	}



	//5 初始化设备信息的链接
	RtkInitializeListHead(&dev->tags);

	//6 将生成的设备信息插入设备列表g_Device
	pr = g_Devices.insert(DEVICE_LIST::value_type(*key, dev));
	return dev;
}

/*
	remove a device object from system's device list

	Environment:

	db lock acquired exclusively.
*/
/*
函数功能：将设备从系统设备列表中删除
参    数：
          dev  待删除的设备信息结构体
返回值：  只有__true，没有错误值返回
*/
IO_API __bool PMC_API io_delete_device(PDEVICE_INFO dev)
{
	PDRIVER_INFO driverObj;
	DEVICE_LIST::iterator it;
    
	//1 解除设备与相关标签点的连接
	detach_device_tags(dev);
    
	//2 释放g_Devices中该设备信息
	it = g_Devices.find(dev->k);
	assert(it != g_Devices.end());
	//3 释放前需找到该设备信息关联的驱动信息的地址
	driverObj = dev->d;
	g_Devices.erase(it);
	delete dev;

	//4 驱动信息的处理
	if(driverObj){
		//4.1 驱动信息中的关联设备数量减1
		driverObj->device_count--;
		assert(driverObj->device_count >= 0);
		//4.2 若发现驱动信息中的关联设备数量为0，则写在驱动DLL
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
函数功能：从设备列表中取出设备信息
参数：
         buffer    用于存放取出的设备信息的DEVICE_INFO数组     [IN]
		 maxcount  设备列表中的设备总数                        [OUT]
返回值： 实际取出的设备信息的个数(设备列表中元素的个数和maxcount的较小值)
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
函数功能：从设备列表中得到指定名称的设备信息
参数：
          key   指向设备名称的指针
返回值：  取出的设备信息结构体的指针
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
函数功能：调用设备驱动的导出函数，启动设备
参数：
          dev  待启动的设备 
返回值：  0         设备驱动未加载
          1         设备驱动中用于启动设备的导出函数指针为0
			   或   启动成功   
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
			r = __true;             //表示已经启动了？？所以返回__true?
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
功能：调用设备驱动导出的函数，停止指定的设备
参数：
      dev    待停止的设备
返回：0      停止设备的导出函数执行失败
      1      设备已停止
	      或 停止设备操作成功
*/
/*
	caller must have DB locked exclusively.
*/
IO_API __bool PMC_API io_stop_device(PDEVICE_INFO dev)
{	
	if( !(dev->flags & DF_Active) ){
		return __true;//设备已经停止
	}
	utils_trace("Stopping device %s\n", (char*)CDeviceName(dev->k));
	// newly created devices might not be attached to driver yet
	if(dev->d){
		if(dev->d->stop_device){
	        //stop_device()返回0，则表示停止设备失败
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

