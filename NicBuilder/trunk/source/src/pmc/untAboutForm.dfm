object AboutBox: TAboutBox
  Left = 397
  Top = 364
  BorderStyle = bsDialog
  Caption = #20851#20110
  ClientHeight = 140
  ClientWidth = 320
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 12
  object Comments: TLabel
    Left = 30
    Top = 90
    Width = 251
    Height = 12
    AutoSize = False
    Caption = 'Comments'
    WordWrap = True
    IsControl = True
  end
  object Copyright: TLabel
    Left = 30
    Top = 66
    Width = 251
    Height = 12
    AutoSize = False
    Caption = 'Copyright'
    IsControl = True
  end
  object Version: TLabel
    Left = 30
    Top = 43
    Width = 251
    Height = 12
    AutoSize = False
    Caption = 'Version'
    IsControl = True
  end
  object ProductName: TLabel
    Left = 30
    Top = 20
    Width = 251
    Height = 16
    AutoSize = False
    Caption = 'Product Name'
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = #40657#20307
    Font.Style = []
    ParentFont = False
    IsControl = True
  end
  object OKButton: TButton
    Left = 100
    Top = 112
    Width = 88
    Height = 21
    Caption = #20851#38381
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
end
