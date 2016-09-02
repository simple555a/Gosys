unit untFormPinAttrib;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls,
  f8ui,
  blklayout,
  common,
  f8,
  untFormVariables,
  ExtCtrls, ComCtrls, ImgList, Buttons, untFramVariables;

type
  TfrmPinAttrib = class(TForm)
    Label1: TLabel;
    grpBinding: TGroupBox;
    Button2: TButton;
    Button3: TButton;
    chkInvert: TCheckBox;
    Label3: TLabel;
    edtName: TEdit;
    rbConst: TRadioButton;
    rbAuto: TRadioButton;
    edtConst: TEdit;
    Button1: TButton;
    edtAuto: TEdit;
    framVariables1: TframVariables;
    procedure FormShow(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure rbAutoClick(Sender: TObject);
    procedure edtConstEnter(Sender: TObject);
    procedure edtAutoChange(Sender: TObject);
    procedure edtAutoEnter(Sender: TObject);
    procedure framVariables1lvVariablesChange(Sender: TObject;
      Item: TListItem; Change: TItemChange);
    procedure framVariables1Button4Click(Sender: TObject);
  private
    { Private declarations }
    procedure Load;
    procedure ShowHide;
  public
    p : TPin;
    { Public declarations }
  end;

var
  frmPinAttrib: TfrmPinAttrib;

implementation

{$R *.dfm}
uses
  rtkdefs, untFormDefineVar;
var
  g_bShow : boolean;

procedure TfrmPinAttrib.FormShow(Sender: TObject);
begin
  Load;
end;

procedure TfrmPinAttrib.Load;
begin
  Caption := '引脚属性 - ' + p.FPinPath;

  edtName.text := p.Fpinname;
  
  if (p.p._class=pin_class_ei) or (p.p._class=pin_class_eo) then
  begin
    label1.Caption := '事件型';
    grpBinding.Enabled := false;
  end else begin
    label1.Caption := _typeName(p.rp._type);
    label1.Caption := label1.Caption + ',当前值=' + valuebypin(p.rp);
  end;

  if (p.rp._class=PIN_CLASS_DI) or (p.rp._class=PIN_CLASS_DO) then
  case p.rp.binding of
  PIN_B_CONST:
  begin
    rbConst.Checked := true;
    edtConst.Text := p.ValueStr;
  end;
  PIN_B_MEMORY:
  begin
    rbAuto.Checked := true;
    edtAuto.Text := GetBindingStr(p.rp);
  end;
  end;

  if (p.rp._class=PIN_CLASS_DI) then
  begin
    if (p.rp.u1.link.Flink <> @p.rp.u1.link) then
    begin
      rbAuto.Checked := true;
      grpBinding.Enabled := false;
    end;
  end;

  if (p.rp._class=PIN_CLASS_DO) then
  begin
    rbConst.Enabled := false;
    edtConst.Enabled := false;
  end;

  if (p.rp._type=pin_t_bool) then
  begin
    chkInvert.Enabled := true;
    chkInvert.Checked := (p.rp.flags and pin_f_inverted)<>0;
  end else begin
    chkInvert.Enabled := false;
  end;

  framVariables1.blk := delphi_parent(p.p.blk);
  framVariables1.ReloadVariables;
end;

procedure TfrmPinAttrib.Button3Click(Sender: TObject);
var
  s : string;
  frm : TfrmDefineVar;
  res : Integer;
begin
  if (p.rp._class<>PIN_CLASS_DI) and (p.rp._class<>PIN_CLASS_DO) then
    Exit;
  if rbConst.Checked then
  begin
    s := edtConst.Text;
    KProxyShellWithError(
      Handle,
      pchar('bind ' + GetPinPath(p.p) + ' ' + s)
      );
  end else begin
    s := edtAuto.Text;
    if (s<>'') and not assigned(query_var(g_sections,pchar(s))) and not assigned(query_var(g_layout.block,pchar(s))) then begin
      frm := TfrmDefineVar.Create(nil);
      frm.Tag := 0;
      frm.savedName := s;
      frm.savedType := p.rp._type;
      res := frm.ShowModal;
      frm.Free;
      if res=mrOK then
        self.framVariables1.ReloadVariables
      else
        exit;
    end;
    if length(s)<>0 then
    begin
      if KProxyShellWithError(
        Handle,
        pchar('bind ' + GetPinPath(p.p) + ' ' + s)
        )<>F8_SUCCESS then
        Exit;
    end else begin
      if KProxyShellWithError(
        Handle,
        pchar('ubind ' + GetPinPath(p.p))
        )<>F8_SUCCESS then
        Exit;
    end;
  end;
  if (p.rp._type = PIN_T_BOOL) AND
    chkInvert.Checked <> LongBool(p.rp.flags and PIN_F_INVERTED)
  then
    KProxyShellWithError(
      Handle,
      'invert ' + GetPinPath(p.p)
    );

  g_Layout.PrepareCanvas;
  p.UiInvalidate;
  g_layout.ReleaseCanvas;

  Close;
end;


procedure TfrmPinAttrib.Button1Click(Sender: TObject);
begin
  g_bShow := not g_bShow;
  ShowHide;
end;

procedure TfrmPinAttrib.FormCreate(Sender: TObject);
begin
  edtConst.Text := '';
  edtAuto.Text := '';
  ShowHide;
end;

procedure TfrmPinAttrib.rbAutoClick(Sender: TObject);
begin
  edtAuto.SetFocus;
end;

procedure TfrmPinAttrib.edtConstEnter(Sender: TObject);
begin
  rbConst.Checked := true;
end;

procedure TfrmPinAttrib.ShowHide;
begin
  if g_bShow then begin
    clientHeight := framVariables1.top + framVariables1.height + 5;
    clientWidth := framVariables1.Width + 10;
    button1.Caption := '隐藏变量<<(&V)';
    framVariables1.adjustApperence;
  end else begin
    clientHeight := framVariables1.Top - 3;
    clientWidth := Button3.Left + button3.Width + 3;
    button1.Caption := '查找变量>>(&V)';
  end;
end;

procedure TfrmPinAttrib.edtAutoChange(Sender: TObject);
begin
  rbAuto.OnClick := nil;
  rbAuto.Checked := true;
  rbAuto.OnClick := rbAutoClick;
end;

procedure TfrmPinAttrib.edtAutoEnter(Sender: TObject);
begin
  rbAuto.OnClick := nil;
  rbAuto.Checked := true;
  rbAuto.OnClick := rbAutoClick;
end;

procedure TfrmPinAttrib.framVariables1lvVariablesChange(Sender: TObject;
  Item: TListItem; Change: TItemChange);
begin
  if not assigned(framVariables1.lvVariables.Selected) then
    Exit;
  edtAuto.Text := framVariables1.lvVariables.Selected.SubItems[0];
end;

procedure TfrmPinAttrib.framVariables1Button4Click(Sender: TObject);
begin
  framVariables1.Button4Click(Sender);
end;

end.
