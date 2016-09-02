object frmSearch: TfrmSearch
  Left = 344
  Top = 171
  BorderStyle = bsDialog
  Caption = #26597#25214'...'
  ClientHeight = 438
  ClientWidth = 425
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object Label1: TLabel
    Left = 4
    Top = 4
    Width = 48
    Height = 12
    Caption = #26597#25214#23545#35937
  end
  object Edit1: TEdit
    Left = 4
    Top = 22
    Width = 421
    Height = 20
    TabOrder = 0
  end
  object GroupBox1: TGroupBox
    Left = 4
    Top = 46
    Width = 217
    Height = 59
    Caption = #31867#22411
    TabOrder = 1
    Visible = False
    object RadioButton2: TRadioButton
      Left = 12
      Top = 36
      Width = 113
      Height = 17
      Caption = #21464#37327
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object RadioButton1: TRadioButton
      Left = 12
      Top = 18
      Width = 113
      Height = 17
      Caption = #21151#33021#22359
      TabOrder = 1
    end
  end
  object Button1: TButton
    Left = 346
    Top = 48
    Width = 75
    Height = 21
    Caption = #28165#38500#32467#26524
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 270
    Top = 48
    Width = 75
    Height = 21
    Caption = #26597#25214
    Default = True
    TabOrder = 3
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 347
    Top = 76
    Width = 75
    Height = 21
    Cancel = True
    Caption = #20851#38381
    TabOrder = 4
    OnClick = Button3Click
  end
  object lb: TListBox
    Left = 4
    Top = 112
    Width = 421
    Height = 325
    Color = clInactiveCaptionText
    ItemHeight = 12
    TabOrder = 5
    OnDblClick = lbDblClick
  end
  object Button4: TButton
    Left = 271
    Top = 76
    Width = 75
    Height = 21
    Caption = #26597#30475
    TabOrder = 6
    OnClick = Button4Click
  end
end
