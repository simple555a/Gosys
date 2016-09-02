//---------------------------------------------------------------------------


#ifndef untPictureViewH
#define untPictureViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TframPictureView : public TFrame
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TListView *lv;
    TEdit *edtName;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TImageList *ImageList1;
    void __fastcall lvClick(TObject *Sender);
    void __fastcall lvDblClick(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframPictureView(TComponent* Owner);
    void UpdateView();
    NODE_KEY node;
};
//---------------------------------------------------------------------------
extern PACKAGE TframPictureView *framPictureView;
//---------------------------------------------------------------------------
#endif
