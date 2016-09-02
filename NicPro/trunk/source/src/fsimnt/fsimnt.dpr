program fsimnt;

uses
  Forms,
  librtk,
  libutils,
  windows,
  unt_frmMain in 'unt_frmMain.pas' {Form1},
  unt_framDevice in 'unt_framDevice.pas' {framDevice: TFrame};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  SetCurrentDirectory(get_working_dir);
  Application.Run;
end.
