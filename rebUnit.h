#ifndef rebUnitH
#define rebUnitH

#include <System.hpp>
#include <windows.hpp>
#include <messages.hpp>
#include <sysutils.hpp>

#include <graphics.hpp>

#include <Forms.hpp>
#include <dialogs.hpp>
#include <stdctrls.hpp>
#include <registry.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>

class TRebootForm: public TForm
{

__published:
  // IDE-managed Components
	TLabel *lblQuestion;
	TLabel *lblNo;
	TLabel *lblYes;
	TButton *btnYes;
	TButton *btnNo;
  void __fastcall btnYesClick(TObject* Sender);
  void __fastcall btnNoClick(TObject* Sender);
  void __fastcall FormClose(TObject* Sender, TCloseAction& Action);
  void __fastcall FormShow(TObject* Sender);
public:
   __fastcall TRebootForm (TComponent* Owner);
};


extern PACKAGE TRebootForm *RebootForm;

#endif //  rebUnitH