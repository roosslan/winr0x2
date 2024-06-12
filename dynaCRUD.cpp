//---------------------------------------------------------------------------
#pragma hdrstop
#include "dynaCRUD.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

dynaCRUD::dynaCRUD(TWRForm *ParentWindow)
{
   parentWindow = ParentWindow;
   btns_n_chbxs = new std::vector<workingControl>;
};

bool dynaCRUD::LoadPanels(TWRForm *ParentForm)
{
	TIniFile *winrusIni = new TIniFile(GetCurrentDir() + "\\winr0x2.conf");

        TStringList *panelsList = new TStringList();
        winrusIni->ReadSectionValues("Panels", panelsList);

        TStringList *sectionsList = new TStringList();
        winrusIni->ReadSections(sectionsList);

	// int elemCount = sectionsList->Count;

	// осталось с версии 0.01:
        int controlLeft = winrusIni->ReadInteger("Panels.First", "Left", 112);
        int controlTop = winrusIni->ReadInteger("Panels.First", "Top", 48);
        int controlHeight = winrusIni->ReadInteger("Panels.First", "Height", 289);
        int controlWidth = winrusIni->ReadInteger("Panels.First", "Width", 345);


	std::pair<String, TPanel*> rbPair;

        // Dyn. creating Left Slider widget and Panels
	for (int i = 0; i < panelsList->Count; ++i)
        {
           	TPanel *panelFromIni = new TPanel(*ParentForm);

		panelFromIni->Parent = ParentForm;
		panelFromIni->Left = controlLeft;
		panelFromIni->Top = controlTop;
		panelFromIni->Height = controlHeight;
		panelFromIni->Width = controlWidth;
		panelFromIni->SendToBack();
                panelFromIni->Font->Color = clWhite;
                panelFromIni->Color = clNavy;
                panelFromIni->BorderStyle = bsSingle;
		panelFromIni->Name = panelsList->Strings[i].SubString(0, panelsList->Strings[i].Pos("=")-1);

                panelFromIni->Caption = "";
                panelFromIni->SendToBack();

	       	TRadioButton *rbFromIni = new TRadioButton(*ParentForm);
		rbFromIni->Parent = ParentForm;

                byte lineLength = panelsList->Strings[i].Length();		// "PanelWindows = Windows" | 23
                byte rbCaption_eqSignPos = panelsList->Strings[i].Pos("=") + 1; // "PanelWindows =| 13
                byte rbCaptionLength = lineLength - rbCaption_eqSignPos + 1;    // we've get "| Windows"

                rbFromIni->Caption = " " + panelsList->Strings[i].SubString(rbCaption_eqSignPos, rbCaptionLength) + " ";

        	rbFromIni->Top = 48 + i*24;
	        rbFromIni->Left = 8;
                rbFromIni->Color = clNavy;
                rbFromIni->Font->Color = clWhite;

                AnsiString rbName = "rb" + (String)i;
        	rbFromIni->Name = rbName;

        	rbPair.first = rbName;
	        rbPair.second = panelFromIni;

	        rbFromIni->OnClick = parentWindow->func->RadioButtonClick;

	        rbPanels.insert(std::make_pair(rbName, panelFromIni));

                // ќрганизаци€ Tooltip
                TBevel *bevelHoriz = new TBevel(panelFromIni);
                bevelHoriz->Parent = panelFromIni;
                bevelHoriz->Top = 248;
                bevelHoriz->Height = 9;
                bevelHoriz->Left = 112;
                bevelHoriz->Width = 225;
                bevelHoriz->Shape = bsBottomLine;

                TLabel *textDynTooltip = new TLabel(panelFromIni);
                textDynTooltip->Left = 120;
                textDynTooltip->Top = 264;
                textDynTooltip->Font->Color = clWhite;
                textDynTooltip->Parent = panelFromIni;
                textTooltip = textDynTooltip;
	        pair_rb_n_TooltipLabels.insert(std::make_pair(rbName, textDynTooltip));

                radioButtons.push_back(rbFromIni);
        };

        delete winrusIni;
	return false;
}

bool dynaCRUD::CreateButtons_nCheckboxes(TForm *ParentForm)
{
   	TIniFile *winrusIni = new TIniFile(GetCurrentDir() + "\\winr0x2.conf");

        TStringList *panelsList = new TStringList();
        winrusIni->ReadSectionValues("Panels", panelsList);

        TStringList *sectionsList = new TStringList();
        winrusIni->ReadSections(sectionsList);

	int elemCount = sectionsList->Count;

	// Reading the whole INI elements-file
        for (int i = 1; i < elemCount; ++i) 		// '1' cause skip "Panels" section
        {
           workingControl *wElement = new workingControl();
           wElement->Index = i-1;

           AnsiString Section = sectionsList->Strings[i].c_str();

	   wElement->ControlName = "contrl" + parentWindow->func->toString<int>(i);
       	   wElement->ComponentType = winrusIni->ReadString(Section, "ControlType", ctCheckbox).c_str();
       	   wElement->Top = winrusIni->ReadInteger(Section, "Top", 0);

           wElement->DataType = parentWindow->func->toDataType( winrusIni->ReadString(Section, "DataType", "String").c_str() );


           std::string sParentPanel = winrusIni->ReadString(Section, "Parent", "ParentForm").c_str();
           wElement->parentPanel = StringToTPanel(ParentForm, sParentPanel);

           wElement->Left     = winrusIni->ReadInteger(Section,  "Left",      18);
       	   wElement->Title     = winrusIni->ReadString(Section,  "Title",     Section + "Title error").c_str();
       	   wElement->ToolTip   = winrusIni->ReadString(Section,  "ToolTip",   "No Tooltip").c_str();
	   wElement->RootKey   = winrusIni->ReadString(Section,  "RootKey",   Section + "RootKey error").c_str();
	   wElement->Key       = winrusIni->ReadString(Section,  "Key",       Section + "Key error").c_str();
	   wElement->Param     = winrusIni->ReadString(Section,  "Param",     Section + "Param error").c_str();
	   wElement->Set       = winrusIni->ReadString(Section,  "Set",       Section + "Set error").c_str();
	   wElement->Unset     = winrusIni->ReadString(Section,  "Unset",     Section + "Unset error").c_str();
       	   wElement->BrowseOn  = winrusIni->ReadBool(Section,    "BrowseOn",  false);

           btns_n_chbxs->push_back(*wElement);
        }

        // Dynamically creating checkboxes and buttons
	for (unsigned short i = 0; i < elemCount-1; ++i)		// '-1' cause skip "Panels" section
        {
                workingControl wcElement = btns_n_chbxs[0][i];

                if (wcElement.ComponentType == ctCheckbox)
                {
                   TCheckBox *cbHandle = createControl<TCheckBox>(&wcElement);
                   cbHandle->OnMouseMove = &(parentWindow->func->cbMouseMove);

                   std::string readSet;
                   // ≈сли Set-данные совпадают с тем, что в реестре - ставим галочку:
                   switch (wcElement.DataType)
                   {
                     case dtString:
                       readSet = reg::ReadString(wcElement.RootKey, wcElement.Key, wcElement.Param, wcElement.Unset);;
                       break;
                     case dtDWORD:
                       readSet = reg::ReadInt(wcElement.RootKey, wcElement.Key, wcElement.Param, wcElement.Unset);
                       break;
                    case dtBinary:
                       readSet = reg::ReadBinary(wcElement.RootKey, wcElement.Key, wcElement.Param, wcElement.Unset);
                       break;
                   }
                   if(readSet == wcElement.Set)
                      cbHandle->State = cbChecked;

                   //cbHandle->WindowProc = &WndProc;

                   cbHandle->OnClick = parentWindow->func->CheckBoxClick;
                }
                if (wcElement.ComponentType == ctButton)
                {
	                TButton *btnHandle = createControl<TButton>(&wcElement);
                	// createControl<TButton>(&wcElement)->OnClick = ButtonClick;
                        btnHandle->OnMouseMove = &(parentWindow->func->BtnMouseMove);
                        btnHandle->OnClick = parentWindow->func->ButtonClick;
        	}
        };

        delete winrusIni;
	return false;
}

void dynaCRUD::HandlerSetter(workingControl elm, int Value)
{
	// WriteDecimalInt?
}

void dynaCRUD::HandlerSetter(workingControl elm, std::string Value)
{
   reg::WriteString(elm.RootKey, elm.Key, elm.Param, parentWindow->edtRegistryValue->Text.c_str());
}

void dynaCRUD::HandlerSetter(workingControl elm, bool Value)      // for Checkboxes
{
   //String sValueToSet = IsChecked ? elm.Set : elm.Unset;

   switch (elm.DataType)
   {
     case dtString:
        reg::WriteString(elm.RootKey, elm.Key, elm.Param, Value ? elm.Set : elm.Unset);
        break;
     case dtBinary:
	// bitset<8> RValue = Value ? reg::stringToBitset(elm.Set) : reg::stringToBitset(elm.Unset);
	reg::Write_REG_BINARY(elm.RootKey, elm.Key, elm.Param, Value ? elm.Set : elm.Unset);
        break;
     case dtDWORD:
        reg::WriteInt(elm.RootKey, elm.Key, elm.Param, Value ? atoi(elm.Set.c_str()) : atoi(elm.Unset.c_str()));
        break;
   }

}

TPanel* dynaCRUD::StringToTPanel(TForm *ParentForm, std::string str_PanelFromIniFile)
{
	for (int i = 0; i < ParentForm->ControlCount; ++i)
	{
    		// Let us extract the control at index i
    		TControl *control = ParentForm->Controls[i];

    		// Now we want to test whether this is any specific type, for instance a
		// TEdit or any TEdit descendant.
	    	TPanel *panel = dynamic_cast<TPanel *>(control);

		// If this control is a TPanel or any control deriving from it, we will have
		// a pointer to it, if this is any other type, we will be left with a zero
		// pointer.
		if (panel)
		{
			// This is a TPanel control... we can now access TPanel's only properties
		        if(str_PanelFromIniFile == panel->Name.c_str())
                        	return panel;
		}
	}
        return NULL;
};
