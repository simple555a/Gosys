object Form3: TForm3
  Left = 479
  Top = 229
  BorderStyle = bsDialog
  Caption = #36873#25321#25480#26435#30424
  ClientHeight = 107
  ClientWidth = 254
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 11
  object Label1: TLabel
    Left = 4
    Top = 4
    Width = 99
    Height = 11
    Caption = #36873#25321#23384#25918#25480#26435#30340#30913#30424
  end
  object lst: TComboBox
    Left = 4
    Top = 24
    Width = 241
    Height = 19
    Style = csDropDownList
    ItemHeight = 11
    TabOrder = 0
    OnDropDown = lstDropDown
  end
  object Button1: TButton
    Left = 168
    Top = 52
    Width = 75
    Height = 21
    Caption = #30830#23450'(&O)'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object Button2: TButton
    Left = 168
    Top = 80
    Width = 75
    Height = 21
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 2
  end
  object CheckBox1: TCheckBox
    Left = 4
    Top = 52
    Width = 145
    Height = 17
    Caption = #21482#26174#31034#21487#31227#21160#30913#30424
    Checked = True
    State = cbChecked
    TabOrder = 3
  end
end
