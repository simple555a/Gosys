unit untFormOptions;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls,
  f8ui,
  blklayout,
  ComCtrls, ExtCtrls;

type
  TfrmOptions = class(TForm)
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    ComboBox1: TComboBox;
    Edit1: TEdit;
    Edit2: TEdit;
    CheckBox1: TCheckBox;
    GroupBox2: TGroupBox;
    Label4: TLabel;
    Panel1: TPanel;
    Panel2: TPanel;
    Label5: TLabel;
    Panel3: TPanel;
    Label6: TLabel;
    Label7: TLabel;
    cmdFont: TButton;
    fdlg: TFontDialog;
    GroupBox3: TGroupBox;
    Label8: TLabel;
    edtLibPath: TEdit;
    procedure Button1Click(Sender: TObject);
    procedure cmdFontClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmOptions: TfrmOptions;

implementation

{$R *.dfm}

procedure TfrmOptions.Button1Click(Sender: TObject);
begin
  BlkUnimplemented(Handle);
end;

procedure TfrmOptions.cmdFontClick(Sender: TObject);
begin
  if fdlg.Execute then
  begin
    cmdfont.Caption := fdlg.Font.Name + ' ' + inttostr(fdlg.font.Size) + ' (&F) ...';
  end;
end;

end.
