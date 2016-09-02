unit untFormDefineVar;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls,f8;

type
  TfrmDefineVar = class(TForm)
    Label1: TLabel;
    Edit1: TEdit;
    Label2: TLabel;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    Button1: TButton;
    Button2: TButton;
    lbType: TListBox;
    Label3: TLabel;
    edtComment: TEdit;
    edtInitVal: TEdit;
    Label4: TLabel;
    procedure Button1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
    procedure addVariable;
    procedure editVariable;
  public
    { Public declarations }
    savedType : integer;
    savedName : string;
    savedLocal : boolean;
    savedComment : string;

    v : Pblk_var_t;
  end;

var
  frmDefineVar: TfrmDefineVar;

implementation

{$R *.dfm}
uses
  rtkdefs, f8ui, common;

procedure TfrmDefineVar.Button1Click(Sender: TObject);
begin
  if tag=0 then
    addVariable
  else
    editVariable;
end;

procedure TfrmDefineVar.addVariable;
var
  s : string;
begin
  lbType.Enabled := false;
  savedName := edit1.Text;
  savedLocal := radioButton2.checked;
  savedType := integer(lbType.Items.Objects[lbType.ItemIndex]);
  savedComment := edtComment.Text;

  s := 'set name=' + edit1.text + ' type=' +
    IntToStr(Integer(lbType.Items.Objects[lbType.ItemIndex])) +
    ' comment="' + edtComment.Text + '"' + ' init='+edtInitVal.Text;
  if radiobutton2.Checked then
    s := s + ' scope=' + GetBlkPath(g_layout.Block);
  if KProxyShellWithError(Handle, s, 0)=0 then begin
    ModalResult := mrOk;
  end;
end;

procedure TfrmDefineVar.editVariable;
var
  s : string;
  path : string;
begin
  if radiobutton2.Checked then
    path:=GetBlkPath(g_layout.Block)
  else
  path:='/';
  s := 'editvar name=' + path + '.' + savedName + ' init='+edtInitVal.Text;
  if edit1.text<>savedName then
    s := s + ' newname=' + edit1.text;
  s := s + ' comment="' + edtComment.Text + '"';
  if KProxyShellWithError(Handle, s, 0)=0 then
    ModalResult := mrOk;
end;

procedure TfrmDefineVar.FormShow(Sender: TObject);
var
  tp : f8_byte;
  i : integer;
begin
  edit1.Text := savedName;

  if savedLocal then
    radiobutton2.Checked := true
  else
    radioButton1.checked := true;

  lbType.Items.Clear;
  tp := pin_t_char;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));
  tp := pin_t_short;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));
  tp := pin_t_integer;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));
  tp := pin_t_byte;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));
  tp := pin_t_word;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));
  tp := pin_t_dword;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));
  tp := pin_t_float;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));
  tp := pin_t_double;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));
  tp := pin_t_bool;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));
  tp := pin_t_date;
  lbType.Items.AddObject(_typeName(tp),TObject(tp));

  edtInitVal.text := valueByDpin(tp,@v^.initVal,false);

  if tag=1 then
    savedType:=v^._type;
    
  i := lbType.Items.IndexOfObject(TObject(savedType));
  if i>0 then
    lbType.ItemIndex := i
  else
    lbType.ItemIndex := 0;

  edtComment.Text := savedComment;

  if tag=0 then
    caption := '定义变量'
  else
    caption := '修改变量';

  lbType.Enabled := tag=0;
end;

end.
