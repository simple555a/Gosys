unit untFormTargetConfig;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Menus,
  f8,
  librtk,
  ComCtrls;

type
  TfrmTargetConfig = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button4: TButton;
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    GroupBox3: TGroupBox;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Edit3: TEdit;
    Edit4: TEdit;
    Edit5: TEdit;
    Edit6: TEdit;
    Edit7: TEdit;
    Edit8: TEdit;
    lvDevices: TListView;
    ppDevices: TPopupMenu;
    Label1: TLabel;
    procedure lvDevicesMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormCreate(Sender: TObject);
    procedure ppDevicesPopup(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure lvDevicesDblClick(Sender: TObject);
  private
    { Private declarations }
    pbs : array [0..5] of TProgressBar;
    procedure popupMenuClick(Sender: TObject);
    procedure updateDeviceView(li : tListItem; d : Pio_device);
    procedure commitConfig;
  public
    { Public declarations }
  end;

var
  frmTargetConfig: TfrmTargetConfig;

implementation

uses
  libutils,
  rtkdefs,
  common;
{$R *.dfm}
var
  devices : array[0..io_max_devices-1] of io_device;
  configChanged : array[0..IO_MAX_DEVICES-1] of boolean;
  nonDeviceId : f8_uuid;
  resDeviceId : f8_uuid;

procedure TfrmTargetConfig.lvDevicesMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
var
  pt : TPoint;
begin
  if Button<>mbRight then
    exit;
  if assigned(lvDevices.selected) then
  begin
    pt.X := x;
    pt.y := y;
    pt := lvDevices.clientToScreen(pt);
    ppDevices.Popup(pt.x, pt.y);
  end;
end;

function onAddMenuEntry(dev : Pdevlib_ent_t; con : f8_uint):f8_bool;stdcall;
var
  pp : TfrmTargetConfig;
  mnu : TMenuItem;
begin
  pp := TfrmTargetConfig(con);
  mnu := TMenuItem.Create(pp);
  mnu.Name := 'mnu' + dev^.name;
  mnu.Tag := Integer(dev);
  mnu.OnClick := pp.popupMenuClick;
  mnu.Caption := dev^.description;
  pp.ppDevices.Items.Add(mnu);
  result := __true;
end;

procedure TfrmTargetConfig.FormCreate(Sender: TObject);
procedure placeAt(a:TProgressBar;b:TEdit);
begin
  a.left := b.left;
  a.top := b.top;
  a.Width := b.Width;
  a.Height := b.height;
  a.Smooth := true;
end;
var
  d : Pio_device;
  i : Integer;
  li : TListItem;
  sizes : array[0..KERN_NUM_SECTIONS*2-1] of f8_u32;
begin
  for i:=0 to 5 do
  begin
    pbs[i] := TProgressBar.Create(self);
    pbs[i].Visible := true;
    pbs[i].Parent := groupBox3;
  end;

  placeAt(pbs[0],edit3);
  placeAt(pbs[1],edit4);
  placeAt(pbs[2],edit5);
  placeAt(pbs[3],edit6);
  placeAt(pbs[4],edit7);
  placeAt(pbs[5],edit8);

  nonDeviceId := blklib_get_dev_by_name('NONE')^.dev.clsid;
  resDeviceId := blklib_get_dev_by_name('RESERVED')^.dev.clsid;

  d := ke_get_device_list(g_kernel);
  copyMemory(@devices[0], d, sizeof(io_device)*io_max_devices);
  d := @devices[0];
  for i:=0 to io_max_devices-1 do
  begin
    if (d^.features and io_device_present)=0 then
      d^.clsid := nonDeviceId;
    li := lvdevices.Items.Add;
    updateDeviceView(li, d);
    configChanged[i] := false;
    Inc(d);
  end;

  blklib_enum_available_devices(onAddMenuEntry, f8_uint(self));

  kproxy_get_mem_usage(sizes);
  label4.caption := '字节变量区:'+IntToStr(sizes[1] shr 10)+'K/'+IntToStr(sizes[0] shr 10)+'K';
  label5.caption := '短整型变量区:'+IntToStr(sizes[3] shr 10)+'K/'+IntToStr(sizes[2] shr 10)+'K';
  label6.caption := '4字节变量区:'+IntToStr(sizes[5] shr 10)+'K/'+IntToStr(sizes[4] shr 10)+'K';
  label7.caption := '8字节变量区:'+IntToStr(sizes[7] shr 10)+'K/'+IntToStr(sizes[6] shr 10)+'K';
  label8.caption := '任意长度变量区:'+IntToStr(sizes[9] shr 10)+'K/'+IntToStr(sizes[8] shr 10)+'K';
  label9.caption := '布尔变量区:'+IntToStr(sizes[11] shr 10)+'K/'+IntToStr(sizes[10] shr 10)+'K';

  for i:=0 to 5 do
    if sizes[i*2]<>0 then pbs[i].Position := sizes[i*2+1]*100 div sizes[i*2];
end;

procedure TfrmTargetConfig.ppDevicesPopup(Sender: TObject);
var
  d : Pio_device;
  i : Integer;
  mnu : TMenuItem;
begin
  if not assigned(lvdevices.Selected) then
    exit;
  d := @devices[0];
  inc(d, lvdevices.Selected.Index);
  for i:=0 to ppDevices.Items.Count-1 do
  begin
    mnu := ppdevices.items[i];
    {
    if getDebug then
      writeln('Emh???', mnu.caption);
    }
    if mnu.tag<>0 then
    begin
      if Pdevlib_ent_t(mnu.Tag)^.dev.clsid = d^.clsid then
        mnu.Checked := true
      else
        mnu.Checked := false;
    end else begin
    end;
  end;
end;

procedure TfrmTargetConfig.popupMenuClick(Sender: TObject);
var
  mnu : TMenuItem;
  d : Pio_device;
begin
  if not assigned(lvdevices.Selected) then
    exit;
  mnu := tmenuItem(sender);
  d := @devices[0];
  inc(d, lvdevices.Selected.Index);
  if d^.clsid <> Pdevlib_ent_t(mnu.Tag)^.dev.clsid then
    configChanged[lvdevices.Selected.Index] := true;
  d^.clsid := Pdevlib_ent_t(mnu.Tag)^.dev.clsid;
  if d^.clsid=nonDeviceId then
    d^.features := 0
  else
    d^.features := d^.features or io_device_present;
  updateDeviceView(lvdevices.Selected, d);
end;

procedure TfrmTargetConfig.updateDeviceView(li: TListItem; d: Pio_device);
var
  dd : Pdevlib_ent_t;
begin
  li.subItems.clear;
  li.SubItems.Add(IntToStr(li.index));
  if (d^.features and io_device_present)=0 then
  begin
    li.SubItems.Add('None');
    li.SubItems.Add('空设备');
  end else begin
    dd := blklib_get_dev_by_id(@d^.clsid);
    if assigned(dd) then
    begin
      li.SubItems.Add(dd^.name);
      li.SubItems.Add(dd^.description);
    end else begin
      li.SubItems.Add('Unknown');
      li.SubItems.Add('未知设备-' + F8UuidToString(d^.clsid));
    end;
  end;
end;

procedure TfrmTargetConfig.Button1Click(Sender: TObject);
begin
  commitConfig;
end;

procedure TfrmTargetConfig.commitConfig;
var
  i : Integer;
  d1, d2 : Pio_device;
  cmd : string;
  j : integer;
  buf : array [0..128] of char;
  pp : Pchar;
begin
  d1 := @devices[0];
  d2 := ke_get_device_list(g_kernel);
  for i:=0 to io_max_devices-1 do
  begin
    if configChanged[i] then
    begin
      cmd := 'device ' + intToStr(i) + ' ' + f8UuidToString(d1^.clsid);
      pp := @buf[0];
      for j:=0 to DEV_CONFIG_LEN-1 do
        inc(pp,my_wsprintf(pp,'%02x',d1^.configData[j],1,2,3,4,5));
      cmd := cmd + ' ' + buf;
      KProxyShellWithError(Handle, cmd);
    end;
    inc(d1);
    inc(d2);
  end;
end;

{
  module config
  load the config-ui dll, and call config procedure
}
procedure TfrmTargetConfig.lvDevicesDblClick(Sender: TObject);
var
  e : Pdevlib_ent_t;
  mnu : TMenuItem;
  d : Pio_device;
  buf : array[0..256] of char;
  pp : integer;
  dllName,procName : string;
begin
  // detailed config
  if not assigned(lvdevices.Selected) then
    exit;
  mnu := tmenuItem(sender);
  d := @devices[0];
  inc(d, lvdevices.Selected.Index);
  e := blklib_get_dev_by_id(@d^.clsid);
  if not assigned(e) then exit;
  if not assigned(e^.configUiDll) and (e^.configUiDll<>pointer(-2)) then
  begin
    getPrivateProfileString(pchar(e^.name),'ConfigHandler','',buf,sizeof(buf),
      pchar(get_working_dir()+'config/f8-devices.inf'));
    if buf[0]=chr(0) then exit;
    pp:=Pos(':',buf);
    dllName:=copy(buf,1,pp-1);
    procName := copy(buf,pp+1,length(buf));
    e^.configUiDll := pointer(LoadLibrary(pchar(dllName)));
    if not assigned(e^.configUiDll) then
    begin
      // e^.configUiDll := pointer(-2);
    end;
    e^.configProc := GetProcAddress(HMODULE(e^.configUiDll), pchar(procName));
  end;

  IF assigned(e^.configProc) then
    if e^.configProc(handle, @d^.configData[0], DEV_CONFIG_LEN, 0)<>0 then
      configChanged[lvdevices.Selected.Index]:=true;
end;

end.
