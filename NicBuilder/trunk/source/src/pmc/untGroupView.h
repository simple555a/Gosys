//---------------------------------------------------------------------------


#ifndef untGroupViewH
#define untGroupViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <pmc.h>
#include <Chart.hpp>
#include <MXGRAPH.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>

//---------------------------------------------------------------------------
class TframGroup : public TFrame
{
__published:	// IDE-managed Components
    TLabel *Label5;
    TLabel *Label6;
    TButton *btnNewTag;
    TButton *btnRefresh;
    TEdit *edtCName;
    TLabel *Label4;
    TComboBox *cmbPeriod;
    TButton *btnSave;
    TLabel *lblAnalog;
    TLabel *lblDigital;
    TLabel *lblTagCount;
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall btnNewTagClick(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframGroup(TComponent* Owner);
    void __fastcall UpdateView();
    void __fastcall Validate(PRTK_GROUP grp);
};
//---------------------------------------------------------------------------
extern PACKAGE TframGroup *framGroup;
//---------------------------------------------------------------------------
#endif
