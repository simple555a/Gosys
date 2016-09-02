unit untFormVariables;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, Grids, DBGrids, ComCtrls, ToolWin,
  untFramVariables, ImgList;

type
  TfrmVariables = class(TForm)
    ImageList1: TImageList;
    framVariables1: TframVariables;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure framVariables1Button4Click(Sender: TObject);
    procedure framVariables1Button5Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmVariables: TfrmVariables;

implementation

{$R *.dfm}

procedure TfrmVariables.FormCreate(Sender: TObject);
begin
  framVariables1.blk := nil;
end;

procedure TfrmVariables.FormShow(Sender: TObject);
begin
  framVariables1.adjustApperence;
  framVariables1.ReloadVariables;
end;

procedure TfrmVariables.framVariables1Button4Click(Sender: TObject);
begin
  framVariables1.Button4Click(Sender);
end;

procedure TfrmVariables.framVariables1Button5Click(Sender: TObject);
begin
  framVariables1.Button5Click(Sender);
end;

end.
