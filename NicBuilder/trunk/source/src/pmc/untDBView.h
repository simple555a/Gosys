//---------------------------------------------------------------------------


#ifndef untDBViewH
#define untDBViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <pmc.h>

//---------------------------------------------------------------------------
class TframDB : public TFrame
{
__published:	// IDE-managed Components
    TLabel *Label5;
    TImage *Image2;
    TLabel *lblHint;
    TButton *btnAdd;
    TButton *btnRefresh;
    TLabel *lblTagCount;
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall btnAddClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframDB(TComponent* Owner);

    NODE_KEY key;
    void UpdateView();
};

//---------------------------------------------------------------------------
extern PACKAGE TframDB *framDB;

//---------------------------------------------------------------------------
#endif
