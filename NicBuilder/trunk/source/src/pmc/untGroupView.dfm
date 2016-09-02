object framGroup: TframGroup
  Left = 0
  Top = 0
  Width = 577
  Height = 538
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  ParentFont = False
  TabOrder = 0
  object Label5: TLabel
    Left = 20
    Top = 12
    Width = 40
    Height = 19
    Caption = #20449#24687
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = #40657#20307
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label6: TLabel
    Left = 28
    Top = 52
    Width = 36
    Height = 12
    Caption = #20013#25991#21517
  end
  object Label4: TLabel
    Left = 28
    Top = 100
    Width = 84
    Height = 12
    Caption = #37319#26679#21608#26399'('#27627#31186')'
  end
  object lblAnalog: TLabel
    Left = 32
    Top = 172
    Width = 54
    Height = 12
    Caption = 'lblAnalog'
  end
  object lblDigital: TLabel
    Left = 32
    Top = 192
    Width = 60
    Height = 12
    Caption = 'lblDigital'
  end
  object lblTagCount: TLabel
    Left = 32
    Top = 152
    Width = 66
    Height = 12
    Caption = 'lblTagCount'
  end
  object btnNewTag: TButton
    Left = 140
    Top = 274
    Width = 200
    Height = 21
    Caption = #26032#24314#26631#31614'(&A...)'
    TabOrder = 0
    OnClick = btnNewTagClick
  end
  object btnRefresh: TButton
    Left = 140
    Top = 248
    Width = 200
    Height = 21
    Caption = #21047#26032'(&R)'
    TabOrder = 1
    OnClick = btnRefreshClick
  end
  object edtCName: TEdit
    Left = 28
    Top = 72
    Width = 313
    Height = 18
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 2
    Text = 'edtCName'
  end
  object cmbPeriod: TComboBox
    Left = 28
    Top = 116
    Width = 313
    Height = 20
    Ctl3D = False
    ItemHeight = 12
    ParentCtl3D = False
    TabOrder = 3
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
  object btnSave: TButton
    Left = 140
    Top = 222
    Width = 200
    Height = 21
    Caption = #20445#23384#24403#21069#35774#32622'(&S)'
    TabOrder = 4
    OnClick = btnSaveClick
  end
end
