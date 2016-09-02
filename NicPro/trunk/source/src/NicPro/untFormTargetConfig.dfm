object frmTargetConfig: TfrmTargetConfig
  Left = 273
  Top = 234
  Width = 529
  Height = 311
  Caption = #30446#26631#31995#32479#37197#32622
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  DesignSize = (
    521
    284)
  PixelsPerInch = 96
  TextHeight = 12
  object Label1: TLabel
    Left = 8
    Top = 260
    Width = 156
    Height = 12
    Anchors = [akLeft, akBottom]
    Caption = #21491#38190#20462#25913#31867#22411', '#21452#20987#35774#32622#21442#25968
  end
  object Button1: TButton
    Left = 285
    Top = 258
    Width = 75
    Height = 21
    Anchors = [akRight, akBottom]
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 362
    Top = 258
    Width = 75
    Height = 21
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 1
  end
  object Button4: TButton
    Left = 440
    Top = 258
    Width = 75
    Height = 21
    Anchors = [akRight, akBottom]
    Caption = #24110#21161
    TabOrder = 2
  end
  object PageControl1: TPageControl
    Left = 6
    Top = 5
    Width = 511
    Height = 247
    ActivePage = TabSheet1
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 3
    object TabSheet1: TTabSheet
      Caption = #35774#22791
      object lvDevices: TListView
        Left = 0
        Top = 0
        Width = 503
        Height = 220
        Align = alClient
        Columns = <
          item
            Width = 18
          end
          item
            Caption = #22320#22336
          end
          item
            Caption = #22411#21495
            Width = 150
          end
          item
            Caption = #22791#27880
            Width = 350
          end>
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnDblClick = lvDevicesDblClick
        OnMouseDown = lvDevicesMouseDown
      end
    end
    object TabSheet2: TTabSheet
      Caption = #20869#23384
      ImageIndex = 1
      object GroupBox3: TGroupBox
        Left = 5
        Top = 4
        Width = 363
        Height = 161
        Caption = #20869#23384#20351#29992#24773#20917
        TabOrder = 0
        object Label4: TLabel
          Left = 24
          Top = 17
          Width = 24
          Height = 12
          Caption = '0 '#21306
        end
        object Label5: TLabel
          Left = 24
          Top = 62
          Width = 24
          Height = 12
          Caption = '1 '#21306
        end
        object Label6: TLabel
          Left = 24
          Top = 106
          Width = 24
          Height = 12
          Caption = '2 '#21306
        end
        object Label7: TLabel
          Left = 182
          Top = 13
          Width = 24
          Height = 12
          Caption = '3 '#21306
        end
        object Label8: TLabel
          Left = 190
          Top = 66
          Width = 24
          Height = 12
          Caption = '4 '#21306
        end
        object Label9: TLabel
          Left = 186
          Top = 110
          Width = 24
          Height = 12
          Caption = '5 '#21306
        end
        object Edit3: TEdit
          Left = 25
          Top = 37
          Width = 121
          Height = 20
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 0
          Text = '4096'
        end
        object Edit4: TEdit
          Left = 25
          Top = 82
          Width = 121
          Height = 20
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 1
          Text = '4096'
        end
        object Edit5: TEdit
          Left = 25
          Top = 126
          Width = 121
          Height = 20
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 2
          Text = '4096'
        end
        object Edit6: TEdit
          Left = 183
          Top = 37
          Width = 121
          Height = 20
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 3
          Text = '4096'
        end
        object Edit7: TEdit
          Left = 183
          Top = 82
          Width = 121
          Height = 20
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 4
          Text = '4096'
        end
        object Edit8: TEdit
          Left = 183
          Top = 126
          Width = 121
          Height = 20
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 5
          Text = '4096'
        end
      end
    end
  end
  object ppDevices: TPopupMenu
    AutoHotkeys = maManual
    OnPopup = ppDevicesPopup
    Left = 131
    Top = 214
  end
end
