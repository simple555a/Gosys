object frmDefineVar: TfrmDefineVar
  Left = 313
  Top = 287
  BorderStyle = bsDialog
  Caption = #23450#20041#21464#37327
  ClientHeight = 283
  ClientWidth = 365
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
    Left = 22
    Top = 8
    Width = 36
    Height = 12
    Caption = #21464#37327#21517
  end
  object Label2: TLabel
    Left = 10
    Top = 30
    Width = 48
    Height = 12
    Caption = #21464#37327#31867#22411
  end
  object Label3: TLabel
    Left = 34
    Top = 181
    Width = 24
    Height = 12
    Caption = #27880#37322
  end
  object Label4: TLabel
    Left = 18
    Top = 205
    Width = 36
    Height = 12
    Caption = #21021#22987#20540
  end
  object Edit1: TEdit
    Left = 62
    Top = 4
    Width = 295
    Height = 20
    TabOrder = 0
    Text = 'Edit1'
  end
  object RadioButton1: TRadioButton
    Left = 60
    Top = 232
    Width = 90
    Height = 17
    Caption = #20840#23616#21464#37327
    Checked = True
    TabOrder = 1
    TabStop = True
    Visible = False
  end
  object RadioButton2: TRadioButton
    Left = 175
    Top = 232
    Width = 113
    Height = 17
    Caption = #23616#37096#21464#37327
    TabOrder = 2
    Visible = False
  end
  object Button1: TButton
    Left = 184
    Top = 259
    Width = 83
    Height = 21
    Caption = #30830#23450'(&O)'
    Default = True
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 268
    Top = 259
    Width = 83
    Height = 21
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 4
  end
  object lbType: TListBox
    Left = 62
    Top = 28
    Width = 295
    Height = 145
    ItemHeight = 12
    TabOrder = 5
  end
  object edtComment: TEdit
    Left = 62
    Top = 176
    Width = 297
    Height = 20
    TabOrder = 6
    Text = 'edtComment'
  end
  object edtInitVal: TEdit
    Left = 62
    Top = 200
    Width = 297
    Height = 20
    TabOrder = 7
    Text = 'edtComment'
  end
end
