//---------------------------------------------------------------------------

#ifndef untFormAddDriverH
#define untFormAddDriverH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TfrmAddDriver : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TListView *lv;
    TLabel *Label2;
    TEdit *edtInit;
    TButton *btnSave;
    TButton *Button2;
    TButton *Button3;
    TLabel *Label3;
    TImage *Image1;
    TImageList *ImageList1;
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall lvChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmAddDriver(TComponent* Owner);
    void __fastcall UpdateView();
    NODE_KEY node;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddDriver *frmAddDriver;
//---------------------------------------------------------------------------
#endif
