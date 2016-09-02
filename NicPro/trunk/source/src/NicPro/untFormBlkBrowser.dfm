object frmBlkBrowser: TfrmBlkBrowser
  Left = 346
  Top = 182
  Width = 472
  Height = 372
  Caption = #21151#33021#22359#27983#35272#22120
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 171
    Width = 464
    Height = 174
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object Label2: TLabel
      Left = 0
      Top = 0
      Width = 464
      Height = 13
      Align = alTop
      Caption = '  '#39044#35272
    end
    object Panel3: TPanel
      Left = 0
      Top = 149
      Width = 464
      Height = 25
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 0
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 464
    Height = 171
    Align = alTop
    Caption = 'Panel2'
    TabOrder = 1
    object Panel4: TPanel
      Left = 1
      Top = 1
      Width = 462
      Height = 26
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      DesignSize = (
        462
        26)
      object Label1: TLabel
        Left = 3
        Top = 7
        Width = 74
        Height = 18
        AutoSize = False
        Caption = '  '#36873#25321#21151#33021#22359
      end
      object Button1: TButton
        Left = 353
        Top = 3
        Width = 105
        Height = 22
        Anchors = [akRight, akBottom]
        Caption = '(&C)'#20851#38381
        TabOrder = 0
      end
    end
    object lv: TListView
      Left = 1
      Top = 27
      Width = 462
      Height = 143
      Align = alClient
      Columns = <
        item
          Width = 18
        end
        item
          Caption = #21517#31216
          Width = 150
        end
        item
          Caption = #35828#26126
          Width = 350
        end>
      DragCursor = crSizeAll
      DragMode = dmAutomatic
      ReadOnly = True
      RowSelect = True
      TabOrder = 1
      ViewStyle = vsReport
      OnChange = lvChange
      OnStartDrag = lvStartDrag
    end
  end
end
