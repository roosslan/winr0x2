#include <vcl.h>
#pragma hdrstop

#include "rebUnit.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TRebootForm* RebootForm;


__fastcall TRebootForm::TRebootForm(TComponent* Owner)
        : TForm(Owner)
{
}

//---------------------------------------------------------------------------


void __fastcall TRebootForm::btnYesClick(TObject* Sender)
{
  ExitWindowsEx( EWX_REBOOT, 0 );
}


void __fastcall TRebootForm::btnNoClick(TObject* Sender)
{
   // ->Winhelp( WRForm->Handle, "WinRus.HLP", HELP_QUIT, 0 );
  Application->Terminate();
}

////////////////////////////////////////////////////////////////////////////////////////////
void __fastcall TRebootForm::FormClose(TObject* Sender, TCloseAction& Action )
{
//  WRForm->CoolTrayIcon1.Enabled = true;
}

////////////////////////////////////////////////////////////////////////////////////////////
void __fastcall TRebootForm::FormShow(TObject* Sender )
{
//  WRForm.CoolTrayIcon1.Enabled = false;
}
////////////////////////////////////////////////////////////////////////////////////////////
