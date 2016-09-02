//[1] add 'netd.exe'.  2011-11-02, Zander
unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    GroupBox1: TGroupBox;
    Label1: TLabel;
    pmcStatus: TLabel;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Label2: TLabel;
    Label3: TLabel;
    Button5: TButton;
    Button6: TButton;
    Label4: TLabel;
    Label5: TLabel;
    Button7: TButton;
    Button8: TButton;
    Label6: TLabel;
    Label7: TLabel;
    Timer1: TTimer;
    Label8: TLabel;
    Edit1: TEdit;
    Button9: TButton;
    CheckBox1: TCheckBox;
    Label9: TLabel;
    Label10: TLabel;
    Button10: TButton;
    Button11: TButton;
    Label11: TLabel;
    Button12: TButton;
    Button13: TButton;
    Label12: TLabel;
    procedure Timer1Timer(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button9Click(Sender: TObject);
    procedure CheckBox1Click(Sender: TObject);
  private
    { Private declarations }
    procedure updateStatus;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

uses
  librtk;
  
function insmod_main(cmdLine : pchar):integer;stdcall;external 'libinsmod.dll';

//[1]
var
  btns : array [0..5,0..1] of TButton;
  keys : array[0..5] of string = ('pmcd',
                                  'spacd',
                                  'evtd',
                                  'hisd',
                                  'netd',
                                  'timesvr');
  names : array[0..5] of string = ('NicBuilder 实时数据库服务',
                                   'NicBuilder 串口访问服务',
                                   'NicBuilder 事件记录服务',
                                   'NicBuilder 历史数据库服务',
                                   'NicBuilder 网络监测服务',
                                   'NicBuilder 校时服务');
  lbls : array[0..5] of TLabel;
{$R *.dfm}

procedure TForm1.Timer1Timer(Sender: TObject);
begin
  updateStatus;
end;

procedure TForm1.FormCreate(Sender: TObject);
var
  aa : array[0..255] of char;
begin
  btns[0,0] := button1;
  btns[0,1] := button2;
  btns[1,0] := button3;
  btns[1,1] := button4;
  btns[2,0] := button5;
  btns[2,1] := button6;
  btns[3,0] := button7;
  btns[3,1] := button8;
  btns[4,0] := button10;    //[1]
  btns[4,1] := button11;    //[1]
  btns[5,0] := button12;
  btns[5,1] := button13;

  lbls[0] := pmcStatus;
  lbls[1] := label3;
  lbls[2] := label5;
  lbls[3] := label7;
  lbls[4] := label10;       //[1]
  lbls[5] := label12;

  getPrivateProfileString('pmc', 'serverName', '', aa, sizeof(aa), get_config_file);
  edit1.Text := aa;

  checkbox1.onClick := nil;
  if getPrivateProfileInt('pmc', 'AllowBackup', 0, get_config_file)<>0 then
    checkbox1.Checked := true
  else
    checkbox1.Checked := false;
  checkbox1.onClick := checkbox1Click;

  updateStatus;
end;

procedure TForm1.updateStatus;
var
  i : integer;
  s : string;
  res : integer;
  st : integer;
begin
  //
  for i:=0 to 5 do
  begin
    s := 'insmod -s:' + keys[i] + ' -a:q -f:qqqq';
    res := insmod_main(pchar(s));
    if res<>0 then
    begin
      btns[i,0].caption := '卸载';
      btns[i,1].Enabled := true;
      if res=1 then
      begin
        btns[i,1].Caption := '启动';
        lbls[i].Caption := '已安装, 未启动';
      end else begin
        btns[i,1].Caption := '停止';
        lbls[i].Caption := '已安装, 已启动';
        if (i=0) then
        begin
          st := getPrivateProfileInt('pmc','serverStatus',0,get_config_file());
          case st of
            0:lbls[i].caption := lbls[i].caption + ', 停止方式';
            1:lbls[i].caption := lbls[i].caption + ', 主站方式';
            2:lbls[i].caption := lbls[i].caption + ', 备站方式';
          end;
        end;

        //
        if (i=5) then
        begin
          st := getPrivateProfileInt('timesvr','serverStatus',0,get_config_file());
          case st of
            1:lbls[i].caption := lbls[i].caption + ', 时钟源';
          end;
        end;

      end;
    end else begin
      btns[i,0].caption := '安装';
      btns[i,1].Enabled := false;
      lbls[i].Caption := '未安装';
    end;
  end;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
  btn : tbutton;
  s : string;
  s2 : string;
begin
  //
  btn := sender as tbutton;
  if btn.Caption = '卸载' then
    s2 := 'u'
  else
    s2 := 'i';
  s := 'insmod -a:' + s2 + ' -s:' + keys[btn.tag] + ' -f:' +
    extractFilePath(application.ExeName) + keys[btn.tag] +
    '.exe -nc -m:2 -tp:16 -dn:"' + names[btn.tag] + '"';
  insmod_main(pchar(s));
end;

procedure TForm1.Button2Click(Sender: TObject);
var
  btn : tbutton;
  s : string;
  s2 : string;
begin
  //
  btn := sender as tbutton;
  if btn.Caption = '启动' then
    s2 := 's'
  else
    s2 := 'S';
  s := 'insmod -a:' + s2 + ' -s:' + keys[btn.tag] + ' -f:' +
    extractFilePath(application.ExeName) + keys[btn.tag] + '.exe -nc -tp:16';
  insmod_main(pchar(s));
end;

procedure TForm1.Button9Click(Sender: TObject);
begin
  WritePrivateProfileString('pmc', 'serverName', pchar(edit1.text), get_config_file);
end;

procedure TForm1.CheckBox1Click(Sender: TObject);
begin
  if checkbox1.Checked then
    WritePrivateProfileString('pmc', 'AllowBackup', '1', get_config_file)
  else
    WritePrivateProfileString('pmc', 'AllowBackup', '0', get_config_file);
end;

end.
