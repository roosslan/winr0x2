//---Ruslan 17.03.2001-------------------------------------------------------

// #define _STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS // Для to_string()

#pragma hdrstop
#include "functions.h"
#include <sstream>
#include "registry_funcs.h"

//---------------------------------------------------------------------------

funcs::funcs(TWRForm *ParentWindow)
{
   parentWindow = ParentWindow;
   dyn = parentWindow->dyn;
   dyn->btns_n_chbxs = new std::vector<dynaCRUD::workingControl>;
};




void __fastcall funcs::RadioButtonClick(TObject * Sender)
{
        TRadioButton *rb = static_cast<TRadioButton*>(Sender);
	// Bring to front the Panel, related to selected radio button
        dyn->rbPanels[rb->Name]->BringToFront();
        textTooltip = dyn->pair_rb_n_TooltipLabels[rb->Name];

        // Очистим Hint
        textTooltip->Caption = "";
}

void funcs::enableFormControls(TForm *ParentForm)
{
	for (int i = 0; i < ParentForm->ControlCount; ++i)
	{
           TControl *control = ParentForm->Controls[i];
           control->Enabled = true;
	}
        return;
}

void funcs::disableFormControls(TForm *ParentForm)
{
	for (int i = 0; i < ParentForm->ControlCount; ++i)
	{
    		// Let us extract the control at index i

    		TControl *control = ParentForm->Controls[i];

                if(control->Parent->Name != "pnlEditBoxPanel")
                	control->Enabled = false;
                if(control->Name == "pnlEditBoxPanel")
                	control->Enabled = true;
	}
        return;
}



__fastcall funcs::~funcs()
{
/*
   for (int i; i < wcElements->size(); ++i)
   	delete wcElements[0];
*/
	delete dyn->btns_n_chbxs;
}

byte funcs::toDataType(std::string DataType)
{
   if (DataType == tBin)
        return dtBinary;
        
   if (DataType == tStr)
        return dtString;

   if (DataType == tDWORD)
        return dtDWORD;

   return NULL;
};



void __fastcall funcs::CheckBoxClick(TObject* Sender)
{
   int gWorkingOn = static_cast<TButtonControl*>(Sender)->Tag;
   parentWindow->weWorkingOn = gWorkingOn;
   dynaCRUD::workingControl elm = dyn->btns_n_chbxs[0][gWorkingOn];

   bool IsChecked = static_cast<TCheckBox*>(Sender)->State;
   dyn->HandlerSetter(elm, IsChecked);
}

void __fastcall funcs::BtnMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
   dynaCRUD::workingControl tt_elm = dyn->btns_n_chbxs[0][static_cast<TButtonControl*>(Sender)->Tag];
   //parentWindow->Label17->Caption = static_cast<TButtonControl*>(Sender)->Hint;
   textTooltip->Caption = static_cast<TButtonControl*>(Sender)->Hint;
}

void __fastcall funcs::cbMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
   dynaCRUD::workingControl tt_elm = dyn->btns_n_chbxs[0][static_cast<TCheckBox*>(Sender)->Tag];
   textTooltip->Caption = static_cast<TCheckBox*>(Sender)->Hint;
}



void __fastcall funcs::ButtonClick(TObject* Sender)
{
   	disableFormControls(WRForm);
   	int gWorkingOn = static_cast<TButtonControl*>(Sender)->Tag;
	parentWindow->weWorkingOn = gWorkingOn;
	dynaCRUD::workingControl elm = dyn->btns_n_chbxs[0][gWorkingOn];
        std::string edtText;
        switch (elm.DataType)
        {
        case dtString:
           edtText = reg::ReadString(elm.RootKey, elm.Key, elm.Param, elm.Param);
           parentWindow->ViewMainPanel(false, edtText.c_str());
           break;
        case dtBinary:
           edtText = reg::ReadBinary(elm.RootKey, elm.Key, elm.Param, "");
           parentWindow->ViewMainPanel(false, edtText.c_str());
           break;
        case dtDWORD:
           edtText = reg::ReadInt(elm.RootKey, elm.Key, elm.Param, 0);
           parentWindow->ViewMainPanel(false, edtText.c_str());
           break;
        }
};

