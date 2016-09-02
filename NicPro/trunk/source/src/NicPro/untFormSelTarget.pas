unit untFormSelTarget;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, f8ui, f8, libutils, ActnList;

type
  TfrmSelTarget = class(TForm)
    Label2: TLabel;
    cmbTarget: TComboBox;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    procedure Button2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure actHelpExecute(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSelTarget: TfrmSelTarget;

implementation

{$R *.dfm}

procedure TfrmSelTarget.Button2Click(Sender: TObject);
begin
  ModalResult := mrCancel;
end;

procedure TfrmSelTarget.Button1Click(Sender: TObject);
begin
  ModalResult := mrOk;
end;

procedure TfrmSelTarget.actHelpExecute(Sender: TObject);
begin
  unimplemented(handle);
end;

procedure TfrmSelTarget.FormCreate(Sender: TObject);
var
  tgtName : pchar;
  compName : array[0..1024] of char;
  size : dword;
begin
  tgtName := get_res_buf(@blk_id_root, nil, '!target0');
  if not assigned(tgtName) then
  begin
    size := sizeof(compName);
    GetComputerName(@compName[0], size);
    tgtName := compName;
  end;
  cmbTarget.Text := tgtName
end;

end.
