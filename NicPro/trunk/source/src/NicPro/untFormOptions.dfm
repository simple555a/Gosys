object frmOptions: TfrmOptions
  Left = 387
  Top = 218
  BorderStyle = bsDialog
  Caption = #36873#39033
  ClientHeight = 283
  ClientWidth = 405
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 12
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 405
    Height = 253
    ActivePage = TabSheet1
    Align = alTop
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #22270#24418
      object Label7: TLabel
        Left = 9
        Top = 178
        Width = 24
        Height = 12
        Caption = #23383#20307
      end
      object GroupBox1: TGroupBox
        Left = 1
        Top = 4
        Width = 395
        Height = 71
        Caption = #39029#38754#23610#23544
        TabOrder = 0
        object Label1: TLabel
          Left = 15
          Top = 45
          Width = 24
          Height = 12
          Caption = #23485#24230
        end
        object Label2: TLabel
          Left = 206
          Top = 49
          Width = 24
          Height = 12
          Caption = #39640#24230
        end
        object Label3: TLabel
          Left = 15
          Top = 21
          Width = 24
          Height = 12
          Caption = #24133#38754
        end
        object Edit1: TEdit
          Left = 49
          Top = 44
          Width = 147
          Height = 20
          TabOrder = 1
          Text = '210 mm'
        end
        object ComboBox1: TComboBox
          Left = 49
          Top = 19
          Width = 145
          Height = 20
          ItemHeight = 12
          ItemIndex = 4
          TabOrder = 0
          Text = 'A4(210mm'#215'297mm)'
          Items.Strings = (
            'A0(1153mm'#215'897mm)'
            'A1'
            'A2'
            'A3'
            'A4(210mm'#215'297mm)'
            'B0'
            'B1'
            'B2'
            'B3'
            'B4'
            'B5')
        end
        object Edit2: TEdit
          Left = 238
          Top = 44
          Width = 147
          Height = 20
          TabOrder = 2
          Text = '297 mm'
        end
      end
      object CheckBox1: TCheckBox
        Left = 4
        Top = 81
        Width = 97
        Height = 17
        Caption = #26174#31034#32593#26684
        TabOrder = 1
      end
      object GroupBox2: TGroupBox
        Left = 2
        Top = 104
        Width = 393
        Height = 66
        Caption = #37197#33394#26041#26696
        TabOrder = 2
        object Label4: TLabel
          Left = 10
          Top = 24
          Width = 60
          Height = 12
          Caption = #32452#21512#21151#33021#22359
        end
        object Label5: TLabel
          Left = 130
          Top = 24
          Width = 60
          Height = 12
          Caption = #22522#26412#21151#33021#22359
        end
        object Label6: TLabel
          Left = 268
          Top = 23
          Width = 24
          Height = 12
          Caption = #32972#26223
        end
        object Panel1: TPanel
          Left = 80
          Top = 21
          Width = 21
          Height = 19
          BevelOuter = bvLowered
          Color = clHotLight
          TabOrder = 0
        end
        object Panel2: TPanel
          Left = 200
          Top = 21
          Width = 21
          Height = 19
          BevelOuter = bvLowered
          Color = clHotLight
          TabOrder = 1
        end
        object Panel3: TPanel
          Left = 306
          Top = 21
          Width = 21
          Height = 19
          BevelOuter = bvLowered
          Color = clHotLight
          TabOrder = 2
        end
      end
      object cmdFont: TButton
        Left = 50
        Top = 174
        Width = 247
        Height = 21
        Caption = #26032#23435#20307' 9'#21495' (&F) ...'
        TabOrder = 3
        OnClick = cmdFontClick
      end
    end
    object TabSheet2: TTabSheet
      Caption = #24037#31243
      ImageIndex = 1
      object GroupBox3: TGroupBox
        Left = 0
        Top = 4
        Width = 397
        Height = 53
        Caption = #30446#24405
        TabOrder = 0
        object Label8: TLabel
          Left = 10
          Top = 19
          Width = 84
          Height = 12
          Caption = #31574#30053#24211#25628#32034#36335#24452
        end
        object edtLibPath: TEdit
          Left = 101
          Top = 17
          Width = 274
          Height = 20
          TabOrder = 0
          Text = 'edtLibPath'
        end
      end
    end
  end
  object Button1: TButton
    Left = 80
    Top = 258
    Width = 75
    Height = 21
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 157
    Top = 258
    Width = 75
    Height = 21
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 2
  end
  object Button3: TButton
    Left = 234
    Top = 258
    Width = 75
    Height = 21
    Caption = #24212#29992
    TabOrder = 3
  end
  object Button4: TButton
    Left = 312
    Top = 257
    Width = 75
    Height = 21
    Caption = #24110#21161
    TabOrder = 4
  end
  object fdlg: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [fdNoSimulations]
    Left = 368
    Top = 196
  end
end
