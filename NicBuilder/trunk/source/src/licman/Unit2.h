//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
    TEdit *Edit1;
    TEdit *Edit2;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Edit3;
    TLabel *Label3;
    TEdit *Edit4;
    TLabel *Label4;
    TEdit *Edit5;
    TLabel *Label5;
    TButton *Button1;
    TButton *Button2;
    TLabel *Label6;
private:	// User declarations
public:		// User declarations
    __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
