object frmAddTag: TfrmAddTag
  Left = 310
  Top = 159
  BorderStyle = bsDialog
  Caption = #28155#21152#26631#31614
  ClientHeight = 476
  ClientWidth = 402
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object lblHint: TLabel
    Left = 236
    Top = 452
    Width = 42
    Height = 12
    Caption = 'lblHint'
  end
  inline framTag: TframTag
    Left = -16
    Top = -10
    Width = 409
    Height = 451
    AutoScroll = False
    Color = clBtnFace
    Ctl3D = False
    ParentColor = False
    ParentCtl3D = False
    TabOrder = 0
    inherited pnlAdvanced: TPanel
      Width = 397
      Color = clBtnFace
      inherited Label1: TLabel
        Height = 12
      end
      inherited Label2: TLabel
        Height = 12
      end
      inherited Label3: TLabel
        Height = 12
      end
      inherited Label4: TLabel
        Height = 12
      end
      inherited Label11: TLabel
        Height = 12
      end
      inherited lblStatus: TLabel
        Width = 54
        Height = 12
      end
      inherited Label18: TLabel
        Height = 12
      end
      inherited Label19: TLabel
        Height = 12
      end
      inherited grpSwitchProperty: TGroupBox
        inherited Label15: TLabel
          Height = 12
        end
        inherited Label16: TLabel
          Height = 12
        end
        inherited edtOnMsg: TEdit
          Height = 18
        end
        inherited edtOffMsg: TEdit
          Height = 18
        end
      end
      inherited grpAnalogProperty_1: TGroupBox
        Ctl3D = True
        ParentCtl3D = False
        inherited Label5: TLabel
          Height = 12
        end
        inherited Label6: TLabel
          Height = 12
        end
        inherited Label9: TLabel
          Height = 12
        end
        inherited Label7: TLabel
          Height = 12
        end
        inherited Label8: TLabel
          Height = 12
        end
        inherited Label12: TLabel
          Width = 42
          Height = 12
        end
        inherited Label13: TLabel
          Height = 12
        end
        inherited Label14: TLabel
          Height = 12
        end
        inherited Label17: TLabel
          Width = 42
          Height = 12
        end
        inherited edtAlarmDeadband: TEdit
          Height = 20
        end
        inherited edtLo: TEdit
          Height = 20
        end
        inherited edtHi: TEdit
          Height = 20
        end
        inherited edtMinValue: TEdit
          Height = 20
        end
        inherited edtRate: TEdit
          Height = 20
        end
        inherited edtHiHi: TEdit
          Height = 20
        end
        inherited edtLoLo: TEdit
          Height = 20
        end
        inherited edtEU: TEdit
          Height = 20
        end
        inherited edtMaxValue: TEdit
          Height = 20
        end
      end
      inherited edtAlarmRank: TEdit
        Height = 20
        Ctl3D = True
        ParentCtl3D = False
      end
      inherited edtTagName: TEdit
        Height = 20
        Ctl3D = True
        ParentCtl3D = False
      end
      inherited edtCName: TEdit
        Height = 20
        Ctl3D = True
        ParentCtl3D = False
      end
      inherited edtAddr: TEdit
        Height = 20
        Ctl3D = True
        ParentCtl3D = False
      end
      inherited edtValue: TEdit
        Height = 18
      end
      inherited grpCutOff: TGroupBox
        inherited Label10: TLabel
          Height = 12
        end
        inherited edtCutOff: TEdit
          Height = 18
        end
      end
      inherited grpAnalogProperty_2: TGroupBox
        Ctl3D = True
        ParentCtl3D = False
      end
      inherited cmbDevice: TComboBox
        Height = 20
        ItemHeight = 12
      end
    end
    inherited tmrUpdater: TTimer
      Enabled = False
    end
  end
  object btnAdd: TButton
    Left = 8
    Top = 447
    Width = 75
    Height = 25
    Caption = #19979#19968#20010'(&S)'
    Default = True
    TabOrder = 1
  end
  object btnClose: TButton
    Left = 84
    Top = 447
    Width = 75
    Height = 25
    Caption = #20851#38381'(&X)'
    ModalResult = 2
    TabOrder = 2
    OnClick = btnCloseClick
  end
  object Button1: TButton
    Left = 160
    Top = 447
    Width = 75
    Height = 25
    Caption = #24110#21161'(&H)'
    ModalResult = 2
    TabOrder = 3
  end
end
