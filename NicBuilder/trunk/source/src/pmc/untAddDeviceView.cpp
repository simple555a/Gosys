//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop
#include <ioss.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TframAddDevice *framAddDevice;

class _CVendor : public CTreeNode
{
public:
	_CVendor(PVENDOR_INFO v)
	{
		data = *v;
	}
	VENDOR_INFO data;
};

class CDevType : public CTreeNode
{
public:
	CDevType(PDEVTYPE_INFO t)
	{
		data = *t;
	}
	DEVTYPE_INFO data;
};

static CTreeNode *devices;

static void _add_devices(_CVendor *vnode, const char * inf)
{
	char dev[1024];
	CArgs args;
	GetPrivateProfileString(
		"oem",
		"devices",
		"",
		dev,
		sizeof(dev),
		inf
		);
	parse_arg_ex(dev, &args, ",");
	for(int i=0; i<args.argc; i++){
		DEVTYPE_INFO t;
		ZeroMemory(&t, sizeof(t));
		t.key = (DEVTYPE_KEY)CDevTypeName(args.argv[i]);
		GetPrivateProfileString(
			args.argv[i],
			"description",
			"",
			t.description,
			sizeof(t.description),
			inf
			);
		CDevType *tnode = new CDevType(&t);
		if(!tnode){
			continue;
		}
		vnode->AddChild(tnode);
	}
}

static void _add_vendor(const AnsiString & inf)
{
	VENDOR_INFO v;
	CArgs args;
	parse_arg_ex(inf.c_str(), &args, ":/\\");
	AnsiString sn;
	sn = args.argv[args.argc - 1];
	assert(sn.Length() > 4);
	sn[sn.Length() - 3] = 0;
	ZeroMemory(&v, sizeof(v));
	v.key = (VENDOR_KEY)CVendorName(sn.c_str());
	GetPrivateProfileString(
		"oem",
		"vendor",
		"",
		v.description,
		sizeof(v.description),
		inf.c_str()
		);
	_CVendor * vnode;
	vnode = new _CVendor(&v);
	if(!vnode){
		return;
	}
	devices->AddChild(vnode);
	_add_devices(vnode, inf.c_str());
}

static void _build_device_tree()
{
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	BOOL ret;
	AnsiString infPath;

	devices = new CTreeNode;
	if(!devices){
		return;
	}
	infPath = get_exe_dir();
	infPath += "infs/";
	// utils_debug("path is `%s'\n", infPath.c_str());
	hFind = FindFirstFile((infPath + "*.inf").c_str(), &fd);
	if(INVALID_HANDLE_VALUE == hFind){
		return;
	}
	do{
		_add_vendor(infPath + fd.cFileName);
	}while(FindNextFile(hFind, &fd));
	FindClose(hFind);
}

//---------------------------------------------------------------------------
__fastcall TframAddDevice::TframAddDevice(TComponent* Owner)
	: TFrame(Owner)
{
	if(!devices){
		_build_device_tree();
	}
	lstVendors->Ctl3D = false;
	lstTypes->Ctl3D = false;
}
//---------------------------------------------------------------------------

void TframAddDevice::InitView()
{
	_CVendor     *vnode;
	if(!devices){
		return;
	}
	lstVendors->Items->Clear();
	vnode = dynamic_cast<_CVendor*>(devices->GetFirstChild());
	while(vnode){
		lstVendors->Items->Add(vnode->data.description);
		vnode = dynamic_cast<_CVendor*>(devices->GetNextChild(vnode));
	}
	edtName->Text = "";
	edtAddress->Text = "";
	edtInit->Text = "";
}

#define err(s) Application->MessageBox(s, "错误")

void __fastcall TframAddDevice::Button1Click(TObject *Sender)
{
	DEVICE_KEY key;

	if(!is_valid_name(edtName->Text.c_str()) ){
		err("设备名只能包含字母、数字和下划线");
		return;
	}
	int i;
	//
	CDeviceItems *ditems;
	AnsiString Url;
	Url = (char*)CNodeName(node);
	Url += ".DEVICES";
	ditems = dynamic_cast<CDeviceItems*>(g_DCS.Search(Url));
	if(!ditems){
		return;
	}
	CDeviceItem * ditem;
	if(ditems->GetItems()){
		ditem = dynamic_cast<CDeviceItem*>(ditems->GetFirstChild());
		while(ditem){
			if(edtName->Text == (char *)CDeviceName(ditem->info.k) ){
				err("指定的设备已经存在.");
				return;
			}
			/*
			if(edtAddress->Text == ditem->info.address && edtAddress->Text.Length()){
				err("指定的地址已经链接有设备.");
				return;
			}
			*/
			ditem = dynamic_cast<CDeviceItem*>(ditems->GetNextChild(ditem));
		}
	}
	i = lstVendors->ItemIndex;
	if(i<0){
		err("请选择厂商和设备类型");
		return;
	}
	_CVendor *vnode;
	vnode = dynamic_cast<_CVendor*>(devices->GetFirstChild());
	while(i--){
		vnode = dynamic_cast<_CVendor*>(devices->GetNextChild(vnode));
		assert(vnode);
	}
	i = lstTypes->ItemIndex;
	if(i<0){
		err("请选择设备类型");
		return;
	}
	CDevType *tnode;
	tnode = dynamic_cast<CDevType*>(vnode->GetFirstChild());
	while(i--){
		tnode = dynamic_cast<CDevType*>(vnode->GetNextChild(tnode));
		assert(tnode);
	}
	key = (DEVICE_KEY)CDeviceName(edtName->Text.c_str());
	if(proxy_add_device(
		&node,
		&key,
		&vnode->data.key,
		&tnode->data.key,
		edtAddress->Text.c_str(),
		edtInit->Text.c_str(),
		1000))
	{
		DEVICE_INFO info;
		ZeroMemory(&info, sizeof(info));
		info.k = key;
		info.v = vnode->data;
		info.t = tnode->data;
		strncpy(info.address, edtAddress->Text.c_str(), sizeof(info.address));
		info.address[sizeof(info.address) - 1] = 0;
		strncpy(info.parameter, edtInit->Text.c_str(), sizeof(info.parameter));
		info.parameter[sizeof(info.parameter) - 1] = 0;
		CDeviceItem *it;
		it = new CDeviceItem(&node, &info);
		ditems->AddChild(it);
	}else{
		err("添加失败");
	}
}
//---------------------------------------------------------------------------

void __fastcall TframAddDevice::Button2Click(TObject *Sender)
{
	//    
}
//---------------------------------------------------------------------------

void __fastcall TframAddDevice::lstVendorsClick(TObject *Sender)
{
	CDevType *tnode;
	lstTypes->Items->Clear();
	int i = lstVendors->ItemIndex;
	if(i<0){
		return;
	}
	_CVendor *vnode;
	vnode = dynamic_cast<_CVendor*>(devices->GetFirstChild());
	while(i--){
		vnode = dynamic_cast<_CVendor*>(devices->GetNextChild(vnode));
		assert(vnode);
	}
	tnode = dynamic_cast<CDevType*>(vnode->GetFirstChild());
	while(tnode){
		lstTypes->Items->Add(tnode->data.description);
		tnode = dynamic_cast<CDevType*>(vnode->GetNextChild(tnode));
	}
}
//---------------------------------------------------------------------------
