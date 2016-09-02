object framTag: TframTag
  Left = 0
  Top = 0
  Width = 735
  Height = 665
  Ctl3D = False
  ParentCtl3D = False
  TabOrder = 0
  object pnlAdvanced: TPanel
    Left = 16
    Top = 20
    Width = 401
    Height = 493
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 0
    object Label1: TLabel
      Left = 12
      Top = 4
      Width = 36
      Height = 12
      Caption = #26631#31614#21517
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 24
      Top = 28
      Width = 24
      Height = 12
      Caption = #25551#36848
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 24
      Top = 76
      Width = 24
      Height = 12
      Caption = #22320#22336
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 8
      Top = 416
      Width = 36
      Height = 13
      Caption = #24037#31243#20540
    end
    object lblStatus: TLabel
      Left = 64
      Top = 440
      Width = 40
      Height = 13
      Caption = 'lblStatus'
    end
    object Label18: TLabel
      Left = 12
      Top = 440
      Width = 48
      Height = 13
      Caption = #26356#26032#26102#38388
    end
    object Label19: TLabel
      Left = 24
      Top = 52
      Width = 24
      Height = 12
      Caption = #35774#22791
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 24
      Top = 100
      Width = 24
      Height = 13
      Caption = #31867#22411
    end
    object edtTagName: TEdit
      Left = 64
      Top = 4
      Width = 321
      Height = 19
      Color = clScrollBar
      ReadOnly = True
      TabOrder = 0
    end
    object edtCName: TEdit
      Left = 64
      Top = 26
      Width = 321
      Height = 19
      TabOrder = 1
    end
    object edtAddr: TEdit
      Left = 64
      Top = 72
      Width = 321
      Height = 19
      TabOrder = 2
    end
    object edtValue: TEdit
      Left = 56
      Top = 412
      Width = 155
      Height = 19
      Color = clInactiveBorder
      ReadOnly = True
      TabOrder = 3
    end
    object btnWriteDevice: TButton
      Left = 266
      Top = 411
      Width = 115
      Height = 20
      Caption = #20889#35774#22791'(&W)'
      TabOrder = 4
      OnClick = btnWriteDeviceClick
    end
    object cmbDevice: TComboBox
      Left = 64
      Top = 48
      Width = 321
      Height = 21
      ItemHeight = 13
      TabOrder = 5
      Text = 'cmbDevice'
    end
    object cmbType: TComboBox
      Left = 64
      Top = 96
      Width = 321
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 6
      OnChange = cmbTypeChange
      Items.Strings = (
        #24320#20851#37327
        #27169#25311#37327
        #39640#31934#24230#27169#25311#37327
        '8'#20301#25972#25968
        '16'#20301#25972#25968
        '32'#20301#25972#25968
        '64'#20301#25972#25968
        #26080#31526#21495'8'#20301#25972#25968
        #26080#31526#21495'16'#20301#25972#25968
        #26080#31526#21495'32'#20301#25972#25968
        #26080#31526#21495'64'#20301#25972#25968
        #26085#26399
        #26242#19981#25903#25345#27492#31867#22411)
    end
  end
  object btnSave: TButton
    Left = 244
    Top = 460
    Width = 154
    Height = 21
    Caption = #20445#23384'(&S)'
    TabOrder = 1
    OnClick = btnSaveClick
  end
  object PageControl1: TPageControl
    Left = 16
    Top = 148
    Width = 385
    Height = 277
    ActivePage = tsAnalog
    TabIndex = 0
    TabOrder = 2
    object tsAnalog: TTabSheet
      Caption = #27169#25311#37327#23646#24615
      object grpAnalogProperty_1: TGroupBox
        Left = 8
        Top = 12
        Width = 365
        Height = 77
        Caption = #23646#24615
        TabOrder = 0
        object Label9: TLabel
          Left = 20
          Top = 46
          Width = 24
          Height = 13
          Caption = #21333#20301
        end
        object Label13: TLabel
          Left = 20
          Top = 20
          Width = 24
          Height = 13
          Caption = #26368#22823
        end
        object Label14: TLabel
          Left = 190
          Top = 20
          Width = 24
          Height = 13
          Caption = #26368#23567
        end
        object edtMinValue: TEdit
          Left = 222
          Top = 16
          Width = 101
          Height = 19
          TabOrder = 0
        end
        object edtEU: TEdit
          Left = 52
          Top = 44
          Width = 101
          Height = 19
          TabOrder = 1
        end
        object edtMaxValue: TEdit
          Left = 52
          Top = 16
          Width = 101
          Height = 19
          TabOrder = 2
        end
        object chkStep: TCheckBox
          Left = 224
          Top = 46
          Width = 109
          Height = 17
          Caption = #38454#36291#22411#21464#37327
          TabOrder = 3
        end
      end
      object grpAnalogProperty_2: TGroupBox
        Left = 8
        Top = 93
        Width = 365
        Height = 153
        Caption = #25253#35686
        TabOrder = 1
        object Label11: TLabel
          Left = 243
          Top = 69
          Width = 48
          Height = 13
          Caption = #25253#35686#32423#21035
        end
        object Label12: TLabel
          Left = 243
          Top = 25
          Width = 62
          Height = 13
          Caption = #24674#22797#27515#21306'(%)'
        end
        object chkHiHi: TCheckBox
          Left = 20
          Top = 22
          Width = 49
          Height = 17
          Caption = #39640#39640
          TabOrder = 0
        end
        object chkLoLo: TCheckBox
          Tag = 3
          Left = 20
          Top = 95
          Width = 49
          Height = 17
          Caption = #20302#20302
          TabOrder = 1
        end
        object chkHi: TCheckBox
          Tag = 1
          Left = 20
          Top = 46
          Width = 41
          Height = 17
          Caption = #39640
          TabOrder = 2
        end
        object chkLo: TCheckBox
          Tag = 2
          Left = 20
          Top = 70
          Width = 37
          Height = 17
          Caption = #20302
          TabOrder = 3
        end
        object chkRate: TCheckBox
          Tag = 5
          Left = 20
          Top = 119
          Width = 65
          Height = 17
          Caption = #36895#29575'(%)'
          TabOrder = 4
        end
        object edtAlarmRank: TEdit
          Left = 243
          Top = 87
          Width = 101
          Height = 19
          TabOrder = 5
        end
        object edtHiHi: TEdit
          Left = 88
          Top = 20
          Width = 101
          Height = 19
          TabOrder = 6
        end
        object edtLoLo: TEdit
          Left = 88
          Top = 92
          Width = 101
          Height = 19
          TabOrder = 7
        end
        object edtHi: TEdit
          Left = 88
          Top = 44
          Width = 101
          Height = 19
          TabOrder = 8
        end
        object edtLo: TEdit
          Left = 88
          Top = 69
          Width = 101
          Height = 19
          TabOrder = 9
        end
        object edtAlarmDeadband: TEdit
          Left = 243
          Top = 43
          Width = 101
          Height = 19
          TabOrder = 10
        end
        object edtRate: TEdit
          Left = 88
          Top = 116
          Width = 101
          Height = 19
          TabOrder = 11
        end
      end
    end
    object tsLogical: TTabSheet
      Caption = #24320#20851#37327#23646#24615
      ImageIndex = 1
      object grpSwitchProperty: TGroupBox
        Left = 8
        Top = 12
        Width = 365
        Height = 73
        Caption = #23646#24615
        Ctl3D = False
        ParentCtl3D = False
        TabOrder = 0
        object Label5: TLabel
          Left = 17
          Top = 19
          Width = 36
          Height = 13
          Caption = #24320#28040#24687
        end
        object Label6: TLabel
          Left = 17
          Top = 47
          Width = 36
          Height = 13
          Caption = #20851#28040#24687
        end
        object edtOnMessage: TEdit
          Left = 61
          Top = 15
          Width = 288
          Height = 19
          TabOrder = 0
        end
        object edtOffMessage: TEdit
          Left = 61
          Top = 43
          Width = 288
          Height = 19
          TabOrder = 1
        end
      end
      object grpSwitchProperty_2: TGroupBox
        Left = 8
        Top = 91
        Width = 365
        Height = 134
        Caption = #25253#35686
        Ctl3D = False
        ParentCtl3D = False
        TabOrder = 1
        object Label21: TLabel
          Left = 15
          Top = 73
          Width = 48
          Height = 13
          Caption = #25253#35686#32423#21035
        end
        object chkOff2On: TCheckBox
          Tag = 5
          Left = 18
          Top = 24
          Width = 37
          Height = 17
          Caption = #24320
          TabOrder = 0
        end
        object chkOn2Off: TCheckBox
          Tag = 4
          Left = 18
          Top = 52
          Width = 37
          Height = 17
          Caption = #20851
          TabOrder = 1
        end
        object edtAlarmRank2: TEdit
          Left = 15
          Top = 91
          Width = 101
          Height = 19
          TabOrder = 2
        end
      end
    end
    object tsException: TTabSheet
      Caption = #20363#22806#25253#21578#23646#24615
      ImageIndex = 4
      object GroupBox2: TGroupBox
        Left = 8
        Top = 13
        Width = 365
        Height = 100
        Caption = #20363#22806#25253#21578#21442#25968
        TabOrder = 0
        object Label8: TLabel
          Left = 44
          Top = 20
          Width = 24
          Height = 13
          Caption = #38408#20540
        end
        object Label10: TLabel
          Left = 18
          Top = 44
          Width = 50
          Height = 13
          Caption = #30334#20998#27604'(%)'
        end
        object Label15: TLabel
          Left = 26
          Top = 68
          Width = 42
          Height = 13
          Caption = #36229#26102'('#31186')'
        end
        object edtExcDev: TEdit
          Left = 92
          Top = 16
          Width = 113
          Height = 19
          TabOrder = 0
        end
        object excExcDevPercent: TEdit
          Left = 92
          Top = 40
          Width = 113
          Height = 19
          TabOrder = 1
        end
        object edtExcMax: TEdit
          Left = 92
          Top = 65
          Width = 113
          Height = 19
          TabOrder = 2
        end
      end
    end
    object tsArchive: TTabSheet
      Caption = #23384#26723#23646#24615
      ImageIndex = 2
      object chkSaveToHistory: TCheckBox
        Left = 12
        Top = 9
        Width = 165
        Height = 17
        Caption = #23384#20837#21382#21490#26723#26696#24211
        TabOrder = 0
      end
      object GroupBox1: TGroupBox
        Left = 8
        Top = 37
        Width = 365
        Height = 153
        Caption = #23384#26723#21442#25968
        TabOrder = 1
        object Label16: TLabel
          Left = 52
          Top = 28
          Width = 48
          Height = 13
          Caption = #21387#32553#38408#20540
        end
        object Label17: TLabel
          Left = 26
          Top = 52
          Width = 74
          Height = 13
          Caption = #21387#32553#30334#20998#27604'(%)'
        end
        object Label20: TLabel
          Left = 34
          Top = 76
          Width = 66
          Height = 13
          Caption = #36229#26102#35760#24405'('#31186')'
        end
        object chkCompressing: TCheckBox
          Tag = 3
          Left = 12
          Top = 127
          Width = 193
          Height = 17
          Caption = #23384#20837#25968#25454#24211#26102#21551#29992#21387#32553
          Checked = True
          State = cbChecked
          TabOrder = 0
          Visible = False
        end
        object edtCompDev: TEdit
          Left = 116
          Top = 24
          Width = 133
          Height = 19
          TabOrder = 1
          OnChange = edtCompDevChange
        end
        object edtCompDevPercent: TEdit
          Left = 116
          Top = 48
          Width = 133
          Height = 19
          TabOrder = 2
          OnChange = edtCompDevPercentChange
        end
        object edtCompMax: TEdit
          Left = 116
          Top = 73
          Width = 133
          Height = 19
          TabOrder = 3
        end
      end
    end
  end
  object tmrUpdater: TTimer
    OnTimer = tmrUpdaterTimer
    Left = 328
    Top = 144
  end
end
