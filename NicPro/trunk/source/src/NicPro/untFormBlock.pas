unit untFormBlock;

interface

uses Windows, Classes, Graphics, Forms, Controls, StdCtrls, f8, ComCtrls;

type
  TfrmBlock = class(TForm)
    tv: TTreeView;
    Button1: TButton;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    blk : PBlk;
  end;

implementation

{$R *.dfm}

procedure TfrmBlock.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  Action := caFree;
end;


procedure TfrmBlock.Button1Click(Sender: TObject);
begin
end;

end.

