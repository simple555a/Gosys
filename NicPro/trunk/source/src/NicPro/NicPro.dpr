program NicPro;

uses
  forms,
  activex,
  untFormMain in 'untFormMain.pas' {frmMain},
  About in 'ABOUT.PAS' {AboutBox},
  f8,
  untFormBlkBrowser in 'untFormBlkBrowser.pas' {frmBlkBrowser},
  untFormSelTarget in 'untFormSelTarget.pas' {frmSelTarget},
  sysutils,
  common in 'common.pas',
  untFormPinAttrib in 'untFormPinAttrib.pas' {frmPinAttrib},
  untFormBlkAttrib in 'untFormBlkAttrib.pas' {frmBlkAttrib},
  untFormOptions in 'untFormOptions.pas' {frmOptions},
  untFormVariables in 'untFormVariables.pas' {frmVariables},
  untFormTargetConfig in 'untFormTargetConfig.pas' {frmTargetConfig},
  untFormDefineVar in 'untFormDefineVar.pas' {frmDefineVar},
  untFramVariables in 'untFramVariables.pas' {framVariables: TFrame},
  untCopyPaste in 'untCopyPaste.pas',
  untFormSearch in 'untFormSearch.pas' {frmSearch},
  untProgress in 'untProgress.pas' {frmProgress};

{$R *.RES}

begin
  Application.Initialize;
  g_AppPath := ExtractFilePath(application.ExeName);
  CoInitialize(nil);

  Application.Title := 'Actech Conductor';
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TAboutBox, AboutBox);
  Application.CreateForm(TfrmSearch, frmSearch);
  Application.CreateForm(TfrmProgress, frmProgress);
  {Application.CreateForm(TfrmDefineVar, frmDefineVar);
  Application.CreateForm(TfrmBlkBrowser, frmBlkBrowser);
  Application.CreateForm(TfrmSelTarget, frmSelTarget);
  Application.CreateForm(TfrmPinAttrib, frmPinAttrib);
  Application.CreateForm(TfrmBlkAttrib, frmBlkAttrib);
  Application.CreateForm(TfrmOptions, frmOptions);
  Application.CreateForm(TfrmTargetConfig, frmTargetConfig);
  }
  Application.Run;
end.
 
