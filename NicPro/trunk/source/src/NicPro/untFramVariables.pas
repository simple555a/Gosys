unit untFramVariables;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, ComCtrls,
  f8, Menus, Buttons;

type
  TframVariables = class(TFrame)
    lvVariables: TListView;
    Panel1: TPanel;
    Button4: TButton;
    Button1: TButton;
    Button5: TButton;
    Timer1: TTimer;
    Button2: TButton;
    ppm: TPopupMenu;
    ListBox1: TListBox;
    Edit1: TEdit;
    SpeedButton1: TSpeedButton;
    procedure Button4Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure lvVariablesCompare(Sender: TObject; Item1, Item2: TListItem;
      Data: Integer; var Compare: Integer);
    procedure lvVariablesColumnClick(Sender: TObject; Column: TListColumn);
    procedure SpeedButton1Click(Sender: TObject);
    procedure ListBox1Click(Sender: TObject);
    procedure Edit1KeyPress(Sender: TObject; var Key: Char);
    procedure lvVariablesDblClick(Sender: TObject);
  private
    { Private declarations }
    newItem:TListItem;
  public
    { Public declarations }
    blk : Pblk;
    findStr : string;
    procedure ReloadVariables;
    procedure onAddVar(v : Pblk_var_t);
    procedure onDelVar(v : Pblk_var_t);
    procedure adjustApperence;
  end;

implementation
uses
  common, untFormDefineVar, f8ui, rtkdefs,libutils;
{$R *.dfm}

var
  sortIdx : integer;
  invertCompare : boolean;

function _enum_var(v : Pblk_var_t; context : f8_uint):__bool;cdecl;forward;

procedure TframVariables.Button4Click(Sender: TObject);
var
  frm : TfrmDefineVar;
  v : Pblk_var_t;
begin
  frm := TfrmDefineVar.Create(nil);
  frm.Tag := 0;
  if frm.ShowModal=mrOK then begin
    v := query_var(g_sections,pchar(frm.Edit1.text));
    assert(assigned(v));
    onAddVar(v);
    newItem.Selected :=true;
    newItem.MakeVisible(false);
    newItem := nil;
  end;
  frm.Free;
end;

procedure TframVariables.Button1Click(Sender: TObject);
var
  cmd : string;
  v : Pblk_var_t;
begin
  if not assigned(lvVariables.Selected) then
    Exit;
  v := Pblk_var_t(lvVariables.Selected.Data);
  cmd := 'uset ' + GetBlkPath(v.blk) + '.' + v.name;
  if KProxyShellWithError(Handle, cmd)=f8_success then
    lvVariables.DeleteSelected;
end;

function _enum_var(v : Pblk_var_t; context : f8_uint):__bool;cdecl;
var
  frm : TframVariables;
begin
  frm := TframVariables(context);
  frm.onAddVar(v);
  result := __true;
end;

procedure TframVariables.ReloadVariables;
begin
  lvVariables.Items.Clear;
  lvVariables.Items.BeginUpdate;
  enum_variables(g_sections, _enum_var, __uint(self));
{$IFDEF ENABLE_LOCAL_VARIABLES}
  if assigned(blk) and (blk <> g_sections) then
    enum_variables(blk, _enum_var, __uint(lvVariables));
{$ENDIF}
  lvVariables.Items.EndUpdate;
end;

procedure TframVariables.Button2Click(Sender: TObject);
begin
  reloadVariables;
end;

procedure TframVariables.Button5Click(Sender: TObject);
var
  frm : TfrmDefineVar;
  v : Pblk_var_t;
  li : TListItem;
begin
  if not assigned(lvVariables.Selected) then
    exit;
  li := lvVariables.Selected;
  v := Pblk_var_t(lvVariables.Selected.Data);
  frm := TfrmDefineVar.Create(nil);
  frm.Tag := 1;
  frm.savedName := v.name;
  frm.v := v;
  frm.savedComment := v.comment;
  if frm.ShowModal=mrOK then begin
    li.SubItems[0] := v.name;
    li.SubItems[4] := valuebyDpin(v._type,@v.initVal,false);
    if getDebug then
      li.SubItems[6] := v.comment
    else
      li.subitems[5] := v.comment;
  end;
  frm.Free;
end;

function typeIconIndex(tp : f8_u8):integer;
begin
  case tp of
  pin_t_bool:
    result := 0;
  pin_t_byte:
    result := 0;
  pin_t_word:
    result := 0;
  pin_t_dword:
    result := 0;
  pin_t_char:
    result := 0;
  pin_t_short:
    result := 0;
  pin_t_integer:
    result := 0;
  pin_t_float:
    result := 0;
  pin_t_double:
    result := 0;
  pin_t_date:
    result := 0;
  else
    result := 0;
  end;
end;

procedure TframVariables.onAddVar(v: Pblk_var_t);
var
  li : TListItem;
  lv : TListView;
begin
  lv := lvVariables;
  li := lv.Items.Add;
  with li do
  begin
    // li.ImageIndex := typeIconIndex(v._type);
    subitems.Add(v.name);
    subitems.Add(_typeName(v._type));
    subitems.Add(inttostr(v.ref_count));
    subitems.Add(valueByVar(g_kernel, v));
    subitems.Add(valuebyDpin(v._type,@v.initVal,false));
    if getDebug then
      subitems.Add(AddressStrByPin(v.addr));
    if assigned(v.comment) then
      subitems.Add(v.comment)
    else
      subitems.Add('');
    li.Data := v;
  end;
  newItem := li;
end;

procedure TframVariables.onDelVar(v: Pblk_var_t);
begin

end;

procedure TframVariables.lvVariablesCompare(Sender: TObject; Item1,
  Item2: TListItem; Data: Integer; var Compare: Integer);
begin
  Compare := stricomp(pchar(item1.SubItems[sortIdx]),pchar(item2.SubItems[sortIdx]));
  if invertCompare then
    compare := -compare;
end;

procedure TframVariables.lvVariablesColumnClick(Sender: TObject;
  Column: TListColumn);
begin
  if column.index=0 then
    exit;
  if column.Index-1=sortIdx then
    invertCompare := not invertCompare
  else
    invertCompare := false;
  sortIdx := column.Index-1;
  reloadVariables;
end;

function _find_var(v : Pblk_var_t; context : f8_uint):__bool;cdecl;
var
  frm : TframVariables;
begin
  frm := TframVariables(context);
  if (pos(frm.findStr,uppercase(v.name))>0) or (pos(frm.findStr,uppercase(v.comment))>0) then
    frm.ListBox1.AddItem(v.name+','+v.comment,TObject(v));
  result := __true;
end;

procedure TframVariables.SpeedButton1Click(Sender: TObject);
var
  i : integer;
  li : tlistitem;
begin
  findStr := uppercase(edit1.Text);
  listbox1.Clear;
  enum_variables(g_sections, _find_var, __uint(self));
end;

procedure TframVariables.ListBox1Click(Sender: TObject);
var
  i : integer;
  v : Pblk_var_t;
begin
  v := Pblk_var_t(listbox1.items.objects[listbox1.ItemIndex]);
  for i:=0 to  lvVariables.Items.count-1 do begin
    if lvVariables.Items[i].Data=v then begin
      lvvariables.Items[i].MakeVisible(false);
      lvvariables.Items[i].Selected:=true;
      exit;
    end;
  end;
end;

procedure TframVariables.Edit1KeyPress(Sender: TObject; var Key: Char);
begin
  if ord(key)=vk_return then begin
    SpeedButton1Click(edit1);
  end;
end;

procedure TframVariables.adjustApperence;
begin
  if not getDebug and (lvVariables.Columns.count>5) then with lvVariables.Columns do begin
    Delete(6);
  end;
end;

procedure TframVariables.lvVariablesDblClick(Sender: TObject);
begin
  Button5Click(sender);
end;

end.
