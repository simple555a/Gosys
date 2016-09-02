object frmAddDevice: TfrmAddDevice
  Left = 268
  Top = 205
  BorderStyle = bsDialog
  Caption = #28155#21152#35774#22791
  ClientHeight = 291
  ClientWidth = 482
  Color = clCaptionText
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 12
  object Label1: TLabel
    Left = 24
    Top = 52
    Width = 48
    Height = 12
    Caption = #35774#22791#22320#22336
  end
  object Label2: TLabel
    Left = 24
    Top = 97
    Width = 60
    Height = 12
    Caption = #21021#22987#21270#21442#25968
  end
  object Image1: TImage
    Left = 24
    Top = 36
    Width = 401
    Height = 9
    AutoSize = True
  end
  object Label4: TLabel
    Left = 28
    Top = 16
    Width = 102
    Height = 16
    Caption = #25351#23450#35774#22791#21442#25968
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #40657#20307
    Font.Style = [fsBold]
    ParentFont = False
  end
  object edtAddress: TEdit
    Left = 24
    Top = 74
    Width = 445
    Height = 20
    TabOrder = 0
    Text = 'edtAddress'
  end
  object edtInit: TEdit
    Left = 24
    Top = 120
    Width = 445
    Height = 21
    TabOrder = 1
    Text = 'edtInit'
  end
  object chkAutoGen: TCheckBox
    Left = 24
    Top = 146
    Width = 149
    Height = 21
    Caption = #33258#21160#29983#25104#23454#26102#25968#25454
    TabOrder = 2
  end
  object Button1: TButton
    Left = 236
    Top = 264
    Width = 75
    Height = 21
    Caption = #19979#19968#20010'(&S)'
    Default = True
    TabOrder = 3
  end
  object Button2: TButton
    Left = 312
    Top = 264
    Width = 75
    Height = 21
    Caption = #20851#38381'(&C)'
    TabOrder = 4
    OnClick = Button2Click
  end
  object GroupBox1: TGroupBox
    Left = 24
    Top = 176
    Width = 445
    Height = 81
    TabOrder = 5
    object Label3: TLabel
      Left = 12
      Top = 25
      Width = 108
      Height = 12
      Caption = #23454#26102#25968#25454#20998#32452#20851#38190#23383
    end
    object edtGroupKey: TEdit
      Left = 12
      Top = 45
      Width = 405
      Height = 20
      TabOrder = 0
      Text = 'edtGroupKey'
    end
  end
  object Button3: TButton
    Left = 388
    Top = 264
    Width = 75
    Height = 21
    Caption = #24110#21161'(&H)'
    TabOrder = 6
  end
end
