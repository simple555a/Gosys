object framAddGroup: TframAddGroup
  Left = 0
  Top = 0
  Width = 385
  Height = 451
  Ctl3D = False
  ParentCtl3D = False
  TabOrder = 0
  object Label6: TLabel
    Left = 44
    Top = 68
    Width = 28
    Height = 13
    Caption = #25551#36848
  end
  object Label4: TLabel
    Left = 44
    Top = 116
    Width = 90
    Height = 13
    Caption = #37319#26679#21608#26399'('#27627#31186')'
  end
  object Label1: TLabel
    Left = 44
    Top = 24
    Width = 28
    Height = 13
    Caption = #32452#21517
  end
  object lblHint: TLabel
    Left = 46
    Top = 256
    Width = 29
    Height = 13
    Caption = 'lblHint'
  end
  object edtCName: TEdit
    Left = 44
    Top = 88
    Width = 313
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 0
    Text = 'edtCName'
  end
  object cmbPeriod: TComboBox
    Left = 44
    Top = 132
    Width = 313
    Height = 21
    Ctl3D = False
    ItemHeight = 13
    ParentCtl3D = False
    TabOrder = 1
    Text = '1000'
    Items.Strings = (
      '50'
      '100'
      '200'
      '300'
      '400'
      '500'
      '1000'
      '2000'
      '10000'
      '60000')
  end
  object edtName: TEdit
    Left = 44
    Top = 44
    Width = 313
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 2
    Text = 'edtName'
  end
  object btnNext: TButton
    Left = 146
    Top = 204
    Width = 100
    Height = 21
    Caption = #23436#25104'(&S)'
    TabOrder = 3
    OnClick = btnNextClick
  end
  object btnHelp: TButton
    Left = 254
    Top = 204
    Width = 100
    Height = 21
    Caption = #24110#21161'(&H)'
    TabOrder = 4
    OnClick = btnHelpClick
  end
end
