unit untFormBlkAttrib;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ComCtrls,
  f8,
  common,
  blklayout,
  StdCtrls, untFramVariables;

type
  TfrmBlkAttrib = class(TForm)
    Button1: TButton;
    Button2: TButton;
    edtName: TEdit;
    edtCName: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    mmComment: TMemo;
    Label3: TLabel;
    Button4: TButton;
    btnViewSource: TButton;
    chkDisableBlk: TCheckBox;
    Label4: TLabel;
    Memo1: TMemo;
    procedure FormShow(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure btnViewSourceClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    blk : Pblk;
  end;

var
  frmBlkAttrib: TfrmBlkAttrib;

implementation
{$R *.dfm}
uses
  rtkdefs, shellapi;

procedure TfrmBlkAttrib.FormShow(Sender: TObject);
var
  pb : Pblk;
  be : blk_ent_t;
  txt : pchar;
begin
  edtName.text := Iblk_name(blk);
  txt := get_res_name(@blk^.h.uuid);
  if assigned(txt) then
  begin
    edtCName.text := Iblk_class_name(blk);
    label2.Visible := true;
    edtCName.visible := true;
  end else begin
    label2.Visible := false;
    edtCName.visible := false;
  end;
  frmBlkAttrib := Self;

  if blk.h.magic=bblk_magic then
  begin
    chkDisableBlk.Enabled := true;
    chkDisableBlk.Checked := (blk.h.flags and BLK_F_DISABLED)<>0
  end else
    chkDisableBlk.Enabled := false;
    
  if blk=g_sections then
  begin
    edtName.Readonly := true;
    edtCName.ReadOnly := true;
    mmComment.ReadOnly := false;
  end else begin
    pb := delphi_parent(blk);
    if (pb.h.flags and blk_f_readonly)<>0 then
    begin
      edtName.Readonly := true;
      edtCName.ReadOnly := true;
      mmComment.ReadOnly := true;
    end else begin
      edtName.ReadOnly := false;
      mmComment.ReadOnly := false;
      if (blk.h.magic=cblk_magic) and ((blk.h.flags and blk_f_readonly)=0) then
        edtCName.readonly := false
      else
        edtCName.readonly := true;
    end;
  end;

  if blklib_query(@blk^.h.uuid, @be)<>0 then
  begin
    btnViewSource.Enabled := true;
  end else begin
    btnViewSource.Enabled := false;
  end;

  mmComment.Text := IBlk_get_data(blk, 'comment', nil);
  txt := get_res_buf(@blk^.h.uuid, nil, 'description');
  if assigned(txt) then
    memo1.text := txt;

  Caption := '功能块属性-' + GetBlkPath(blk);
end;

procedure TfrmBlkAttrib.Button1Click(Sender: TObject);
var
  b : Pblk;
begin
  if not edtname.ReadOnly then
  begin
    if not is_valid_blk_name(pchar(edtName.Text)) then
    begin
      MessageBox(Handle, '功能块名不合法', pchar(Application.Title), mb_ok);
      Exit;
    end;
    b := Iblk_blk_by_path(delphi_parent(blk), pchar(edtName.Text));
    if assigned(b) and (b<>blk) then
    begin
      messageBox(Handle, '功能块重名', pchar(application.title), mb_ok);
      exit;
    end;
    set_res_name(@blk.uuid, pchar(edtName.Text));
  end;

  if not edtcname.ReadOnly then
  begin
    if not is_valid_blk_name(pchar(edtCName.Text)) then
    begin
      MessageBox(Handle, '功能块类名不合法', pchar(Application.Title), mb_ok);
      Exit;
    end;
    set_res_name(@blk.h.uuid, pchar(edtCName.Text));
  end;

  if not mmcomment.ReadOnly then
  begin
    IBlk_set_data(blk, 'comment', pchar(mmComment.text), -1);
  end;

  ModalResult := mrOk;
end;

procedure TfrmBlkAttrib.Button4Click(Sender: TObject);
var
  k : string;
  e : blk_ent_t;
begin
  // invoke help on the current block
  if blklib_query(@blk.h.uuid, @e)=0 then
    Exit;
  k := ChangeFileExt(e.fileName, '.html');
  ShellExecute(
    Handle,
    'open',
    pchar(k),
    nil,
    nil,
    SW_SHOWNORMAL
    );
end;

procedure TfrmBlkAttrib.btnViewSourceClick(Sender: TObject);
var
  be : blk_ent_t;
  ret : __bool;
  s : string;
  editor : string;
  cmd : string;
  pi : Process_Information;
  si : StartupInfo;
begin
  ret := blklib_query(@blk^.h.uuid, @be);
  assert(ret<>0);
  s := be.fileName;
  s := changeFileExt(s, '.f');
  editor := 'notepad.exe';
  cmd := editor + ' ' + s;
  zeroMemory(@si, sizeof(si));
  si.cb := sizeof(si);
  if createProcess(nil, pchar(cmd), nil, nil, false, 0, nil, nil, si, pi) then
  begin
    closeHandle(pi.hProcess);
    closeHandle(pi.hThread);
  end;
end;

end.
