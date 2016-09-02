unit ABOUT;

interface

uses Windows, Classes, Graphics, Forms, Controls, StdCtrls,
  Buttons, ExtCtrls;

type
  TAboutBox = class(TForm)
    ProgramIcon: TImage;
    Label1: TLabel;
    procedure ProgramIconClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormKeyPress(Sender: TObject; var Key: Char);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  AboutBox: TAboutBox;

implementation

{$R *.dfm}
uses common;

procedure TAboutBox.ProgramIconClick(Sender: TObject);
begin
  Close;
end;

procedure TAboutBox.FormCreate(Sender: TObject);
begin
  programIcon.Picture.LoadFromFile(g_appPath + 'res/f8.bmp');
  programIcon.AutoSize := true;
  autosize := true;
  Position := poScreenCenter;
  Label1.Caption := '';
end;

procedure TAboutBox.FormKeyPress(Sender: TObject; var Key: Char);
begin
  if (Ord(key)=VK_RETURN) or (ord(key)=VK_ESCAPE) THen
    close;
end;

end.
 
