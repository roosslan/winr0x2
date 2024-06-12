object RebootForm: TRebootForm
  Left = 367
  Top = 117
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  ClientHeight = 131
  ClientWidth = 303
  Color = clSilver
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblQuestion: TLabel
    Left = 32
    Top = 16
    Width = 231
    Height = 45
    Caption = 
      #1044#1083#1103' '#1074#1089#1090#1091#1087#1083#1077#1085#1080#1103' '#1085#1086#1074#1099#1093' '#1087#1072#1088#1072#1084#1077#1090#1088#1086#1074' '#1074' '#1089#1080#1083#1091'      '#1082#1086#1084#1087#1100#1102#1090#1077#1088' '#1090#1088#1077#1073#1091#1077#1090#1089#1103' ' +
      #1087#1077#1088#1077#1079#1072#1075#1088#1091#1079#1080#1090#1100'.                 '#1042#1099' '#1093#1086#1090#1080#1090#1077' '#1089#1076#1077#1083#1072#1090#1100' '#1101#1090#1086' '#1089#1077#1081#1095#1072#1089'?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Lucida Sans Unicode'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object lblNo: TLabel
    Left = 48
    Top = 96
    Width = 215
    Height = 15
    Caption = '  '#1053#1077#1090', '#1103' '#1087#1077#1088#1077#1079#1072#1075#1088#1091#1078#1091' '#1082#1086#1084#1087#1100#1102#1090#1077#1088' '#1087#1086#1079#1078#1077'  '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Lucida Sans Unicode'
    Font.Style = []
    ParentFont = False
  end
  object lblYes: TLabel
    Left = 48
    Top = 72
    Width = 252
    Height = 15
    Caption = '  '#1044#1072', '#1103' '#1093#1086#1095#1091' '#1087#1077#1088#1077#1079#1072#1075#1088#1091#1079#1080#1090#1100' '#1082#1086#1084#1087#1100#1102#1090#1077#1088' '#1089#1077#1081#1095#1072#1089'  '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Lucida Sans Unicode'
    Font.Style = []
    ParentFont = False
  end
  object btnYes: TButton
    Left = 8
    Top = 72
    Width = 25
    Height = 17
    TabOrder = 0
    OnClick = btnYesClick
  end
  object btnNo: TButton
    Left = 8
    Top = 96
    Width = 25
    Height = 17
    TabOrder = 1
    OnClick = btnNoClick
  end
end
