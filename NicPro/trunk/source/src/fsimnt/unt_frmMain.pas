unit unt_frmMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ComCtrls, StdCtrls, ExtCtrls,
  f8,
  ShellApi, 
  Menus,
  Buttons,
  winsock,
  ImgList;

const
//my call back constant I could of just used wm_user
WM_CAllBack = WM_USER;

type
  TForm1 = class(TForm)
    Panel1: TPanel;
    Timer1: TTimer;
    PopupMenu1: TPopupMenu;
    About1: TMenuItem;
    Exit1: TMenuItem;
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    lbDevices: TScrollBox;
    mnuShow: TMenuItem;
    Panel2: TPanel;
    ledRun: TPanel;
    ledStop: TPanel;
    ledStandby: TPanel;
    ledAct: TPanel;
    ImageList1: TImageList;
    TabSheet2: TTabSheet;
    Label1: TLabel;
    Memo1: TMemo;
    memAddress: TEdit;
    Label2: TLabel;
    Button4: TButton;
    ledDbg1: TPanel;
    ledDbg2: TPanel;
    led7: TPanel;
    GroupBox1: TGroupBox;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    GroupBox2: TGroupBox;
    RadioButton3: TRadioButton;
    RadioButton4: TRadioButton;
    RadioButton5: TRadioButton;
    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure About1Click(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure mnuShowClick(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure updateKeyStatus(Sender: TObject);
  private
    { Private declarations }
    bAssigningOptions : boolean;
    
    procedure UpdateStatus;
    procedure Hide2Tray;
    procedure WM_CALLBACKPRO(var msg : TMessage); message wm_callBack;
    procedure WM_Hide2Tray(var msg : TMessage); message wm_callBack + 1;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation
uses
  unt_framDevice;
const
  displaed_devices = 4;

  FKERN_LED_RUN = 8;
  FKERN_LED_STOP =9    ;
  FKERN_LED_STANDBY= 10  ;
  FKERN_LED_BUSA =11      ;
  FKERN_LED_BUSB =12       ;
  FKERN_LED_ACT =13         ;
  FKERN_LED_DBG1 =14      ;
  FKERN_LED_DBG2 =15      ;
  FKERN_KEY_A_B = 16 ;
  FKERN_KEY_RUN =17   ;
  FKERN_KEY_STOP= 18  ;
  FKERN_KEY_LOCK= 19;

var
  blah : HICON;
  TrayIcon : TNotifyIconData;
  g_kernel : Pkernel_t;
{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
var
  i : Integer;
  f : TframDevice;
  wsa : WSAData;
begin
  WSAStartup($202, wsa);
  kserver_main(pchar(string(GetCommandLine()) + ' mode=headless'));
  init_blklib;
  g_kernel := ke_get_default_kernel;
  lbDevices.DoubleBuffered := true;
  for i:=0 to displaed_devices-1 do
  begin
    f := TframDevice.Create(lbDevices);
    f.Name := 'DevSlot' + IntToStr(i);
    f.addr := i;
    f.Parent := lbDevices;
    f.Left := 0;
    f.Top := i * (f.Height+2);
    f.updateFirst;
    f.Visible := true;
  end;
  radioButton1.checked := true;
  radioButton3.checked := true;
  // start_kserver;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var
  i : Integer;
  f : TframDevice;
begin
  UpdateStatus;
  for i:=0 to displaed_devices-1 do
  begin
    f := TframDevice(lbDevices.FindComponent('DevSlot' + InttoStr(i)));
    f.updateView;
  end;
  Button4Click(sender);
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  uninit_blklib;
  kserver_trivial;
  WSACleanup;
end;

procedure TForm1.UpdateStatus;
begin
  // RUN LED
  if ke_get_flag(FKERN_LED_RUN)<>0 then
    ledRun.Color := clLime
  else
    ledRun.Color := clGray;

  // stop
  if ke_get_flag(FKERN_LED_STOP)<>0 then
    ledStop.Color := clLime
  else
    ledStop.Color := clGray;

  // STANDBY LED
  if ke_get_flag(FKERN_LED_STANDBY)<>0 then
    ledStandby.Color := clLime
  else
    ledStandby.Color := clGray;

  // act
  if ke_get_flag(FKERN_LED_ACT)<>0 then
    ledAct.Color := clLime
  else
    ledAct.Color := clGray;

  // Dbg1
  if ke_get_flag(FKERN_LED_DBG1)<>0 then
    ledDbg1.Color := clLime
  else
    ledDbg1.Color := clGray;

  // DBG2
  if ke_get_flag(FKERN_LED_STOP)<>0 then
    ledDbg2.Color := clLime
  else
    ledDbg2.Color := clGray;
end;

procedure TForm1.Hide2Tray;
begin
  //hide the form
  Visible := False;
  //give the application icon handle to blah
  blah := application.Icon.Handle;
  //Get The Size Of Byte Of TNotifyIconData
  Trayicon.cbSize := SizeOf(TNotifyIconData);
  //Say The Icon Is Coming From The Form
  Trayicon.Wnd := handle;
  //Give A Tip Message With Mouse Over
  Trayicon.szTip := 'F8 Simulator - x86';
  //Give It A Identifier of your icon
  Trayicon.uID := 1;
  //give the hicon to the tray icon
  TrayIcon.hIcon := blah;
  //my callback function so when click it does something
  TrayIcon.uCallbackMessage := WM_CAllBack;
  //TrayIcon flags to say what it can do
  Trayicon.uFlags := NIF_MESSAGE or NIF_ICON or NIF_TIP;
  //Add The Icon To The Tray
  Shell_NotifyIcon(NIM_ADD,@trayicon);
end;

procedure TForm1.WM_CALLBACKPRO(var msg: TMessage);
var
p : TPoint;
begin
  //WM_LBUTTONDOWN is from the lparam so if left mouse button is pressed it does something
  case msg.LParam of
  WM_RBUTTONDOWN,WM_LBUTTONDOWN :
  begin
    GetCursorPos(p);
    PopupMenu1.Popup(p.X,p.Y);
  end;
  end;
end;

procedure TForm1.Exit1Click(Sender: TObject);
begin
  Close;
end;

procedure TForm1.About1Click(Sender: TObject);
begin
  MessageBox(Handle,
    'F8现场总线控制器仿真程序, Conductor Suite 版本1.0',
    'Actech-F8',
    mb_ok
    );
end;

procedure TForm1.WM_Hide2Tray(var msg: TMessage);
begin
  Hide2Tray;
end;

procedure TForm1.FormResize(Sender: TObject);
begin
  //
end;

procedure TForm1.mnuShowClick(Sender: TObject);
begin
  Shell_NotifyIcon(NIM_DELETE,@TrayIcon);
  Form1.Visible := True;
end;

function my_printf(buf,format:pchar;a1,a2,a3,a4:integer):integer;cdecl;external 'user32.dll' name 'wsprintfA';

procedure TForm1.Button4Click(Sender: TObject);
procedure dumpMemory(sec,ad:integer);
var
  sp,spp : Pf8_u8;
  i,j : integer;
  buf : array[0..1024] of char;
  p : pchar;
begin
  sp := ke_get_section(g_kernel,f8_u8(sec));
  if not assigned(sp) then
    exit;
  dec(ad);
  if (ad+128 > ke_get_section_size(g_kernel,sec)) then
    exit;
  inc(sp,ad);
  memo1.Lines.Clear;
  for i:=0 to 7 do
  begin
    p:=buf;
    spp:=sp;
    p := p + my_printf(p,'%d%05d: ', sec,ad+1+i*16,3,4);
    for j:= 0 to 15 do
    begin
      p := p + my_printf(p,'%02x ',spp^,2,3,4);
      inc(spp);
    end;
    spp:=sp;
    for j:= 0 to 15 do
    begin
      if (spp^>=16) then
        p := p + my_printf(p,'%c ',spp^,2,3,4)
      else
        p := p + my_printf(p,'.',1,2,3,4);
      inc(spp);
    end;
    memo1.Lines.Add(buf);
    inc(sp,16);
  end;
end;
var
  sec : integer;
  ad : integer;
  i : integer;
  ads : array [0..31] of char;
begin
  lstrcpy(@ads[0],pchar(memAddress.Text));
  if length(ads)<2 then
    exit;
  try
    sec := strToInt(ads[0]);
    ad := strToInt(ads+1);
  except
    exit;
  end;
  dumpMemory(sec,ad);
end;

procedure TForm1.updateKeyStatus(Sender: TObject);
begin
  if bAssigningOptions then
    exit;
  case (sender as TRadioButton).tag of
  0 : ke_set_flag(FKERN_KEY_A_B,0);
  1 : ke_set_flag(FKERN_KEY_A_B,1);
  2 :
    begin
      ke_set_flag(FKERN_KEY_RUN,1);
      ke_set_flag(FKERN_KEY_STOP,0);
      ke_set_flag(FKERN_KEY_LOCK,0);
    end;
  3:
    begin
      ke_set_flag(FKERN_KEY_RUN,0);
      ke_set_flag(FKERN_KEY_STOP,1);
      ke_set_flag(FKERN_KEY_LOCK,0);
    end;
  4:
    begin
      ke_set_flag(FKERN_KEY_RUN,0);
      ke_set_flag(FKERN_KEY_STOP,0);
      ke_set_flag(FKERN_KEY_LOCK,1);
    end;
  end;
end;

end.
