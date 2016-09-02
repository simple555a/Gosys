object frmVariables: TfrmVariables
  Left = 278
  Top = 177
  Width = 576
  Height = 483
  Caption = #25511#21046#21464#37327
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inline framVariables1: TframVariables
    Left = 0
    Top = 0
    Width = 568
    Height = 456
    Align = alClient
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 0
    DesignSize = (
      568
      456)
    inherited lvVariables: TListView
      Width = 568
      Height = 330
      Anchors = [akLeft, akTop, akRight, akBottom]
    end
    inherited Panel1: TPanel
      Left = 7
      Top = 332
      Width = 560
      Height = 118
      Anchors = [akLeft, akRight, akBottom]
      inherited SpeedButton1: TSpeedButton
        Left = 278
        Top = 0
      end
      inherited Button4: TButton
        Left = 301
        Top = 0
        OnClick = framVariables1Button4Click
      end
      inherited Button1: TButton
        Left = 365
        Top = 0
      end
      inherited Button5: TButton
        Left = 431
        Top = 0
        OnClick = framVariables1Button5Click
      end
      inherited Button2: TButton
        Left = 495
        Top = 0
      end
      inherited ListBox1: TListBox
        Left = 0
        Width = 560
        Height = 90
      end
      inherited Edit1: TEdit
        Left = 0
        Top = 4
        Width = 273
      end
    end
  end
  object ImageList1: TImageList
    Left = 184
    Top = 142
  end
end
