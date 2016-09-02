//---------------------------------------------------------------------------


#ifndef untTagViewH
#define untTagViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <pmc.h>
#include <ComCtrls.hpp>
#include "configurable.h"
#include <Graphics.hpp>
#include "PERFGRAP.h"

//---------------------------------------------------------------------------
class TframTag : public TFrame
{
__published:	// IDE-managed Components
    TPanel *pnlAdvanced;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *edtTagName;
    TEdit *edtCName;
    TEdit *edtAddr;
    TEdit *edtValue;
    TTimer *tmrUpdater;
    TButton *btnWriteDevice;
    TLabel *lblStatus;
    TLabel *Label18;
    TLabel *Label19;
    TComboBox *cmbDevice;
    TButton *btnSave;
    TPageControl *PageControl1;
    TTabSheet *tsAnalog;
    TTabSheet *tsLogical;
    TTabSheet *tsArchive;
    TGroupBox *grpAnalogProperty_1;
    TLabel *Label9;
    TLabel *Label13;
    TLabel *Label14;
    TEdit *edtMinValue;
    TEdit *edtEU;
    TEdit *edtMaxValue;
    TGroupBox *grpSwitchProperty;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *edtOnMessage;
    TEdit *edtOffMessage;
    TGroupBox *grpSwitchProperty_2;
    TCheckBox *chkOff2On;
    TCheckBox *chkOn2Off;
    TGroupBox *grpAnalogProperty_2;
    TLabel *Label11;
    TLabel *Label12;
    TCheckBox *chkHiHi;
    TCheckBox *chkLoLo;
    TCheckBox *chkHi;
    TCheckBox *chkLo;
    TCheckBox *chkRate;
    TEdit *edtAlarmRank;
    TEdit *edtHiHi;
    TEdit *edtLoLo;
    TEdit *edtHi;
    TEdit *edtLo;
    TEdit *edtAlarmDeadband;
    TEdit *edtRate;
    TCheckBox *chkSaveToHistory;
    TLabel *Label7;
    TComboBox *cmbType;
    TGroupBox *GroupBox1;
    TCheckBox *chkCompressing;
    TEdit *edtCompDev;
    TEdit *edtCompDevPercent;
    TEdit *edtCompMax;
    TTabSheet *tsException;
    TGroupBox *GroupBox2;
    TEdit *edtExcDev;
    TEdit *excExcDevPercent;
    TEdit *edtExcMax;
    TLabel *Label8;
    TLabel *Label10;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label20;
        TLabel *Label21;
    TEdit *edtAlarmRank2;
    TCheckBox *chkStep;
    void __fastcall tmrUpdaterTimer(TObject *Sender);
    void __fastcall btnWriteDeviceClick(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnReadClick(TObject *Sender);
    void __fastcall cmbTypeChange(TObject *Sender);
    void __fastcall edtCompDevChange(TObject *Sender);
    void __fastcall edtCompDevPercentChange(TObject *Sender);
public:
    TAG_NAME    m_TagName;
    RTK_TAG     m_Tag;
    AnsiString  m_Hint;
public:		// User declarations
    __fastcall TframTag(TComponent* Owner);
    void __fastcall setTag(PCTAG_NAME name);
    void __fastcall UpdateView();
    AnsiString __fastcall UpdateData();
    bool ValidateView();
};

//---------------------------------------------------------------------------
extern PACKAGE TframTag *framTag;
//---------------------------------------------------------------------------
#endif
