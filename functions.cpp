//---------------------------------------------------------------------------
#pragma hdrstop
#include "functions.h"

//---------------------------------------------------------------------------


RegHelper::RegHelper(TWRForm *ParentWindow)
{
   parentWindow = ParentWindow;
   btns_n_chbxs = new std::vector<workingControl>;
}

  String RegHelper::RegGetValue(String rootKey, String Key, String paramName, String retIfNExists)
  {
    String ret = retIfNExists;
    TRegistry *reg = new TRegistry(KEY_READ);
    try
    {
      reg->RootKey = toHKEY(rootKey);
      if(reg->OpenKeyReadOnly(Key))
      {
         if(!reg->ValueExists(paramName))
         	ret = retIfNExists; // "ParamNotExists"
         else
           ret = reg->ReadString(paramName);
      }
      reg->CloseKey();
    }
    __finally
    {
      delete reg;
    }
    return ret;
  }

  bool RegHelper::RegParamExists(String rootKey, String Key, String paramName)
  {
    bool ret = true;
    TRegistry *reg = new TRegistry(KEY_READ);
    try
    {
      reg->RootKey = toHKEY(rootKey);
      reg->OpenKey(Key, false);
      if (reg->ReadString(paramName) == NULL)
          ret = false;
      reg->CloseKey();
    }
    __finally
    {
      delete reg;
    }
    return ret;
  }

  bool RegHelper::RegSetValue(String rootKey, String Key, String paramName, String Value)
  {
    bool ret = false;
    TRegistry *reg = new TRegistry(KEY_WRITE);
    try
    {
      reg->RootKey = toHKEY(rootKey);
      reg->OpenKey(Key, true);
      reg->WriteString(paramName, Value);
      reg->CloseKey();
    }
    __finally
    {
      delete reg;
    }
    return ret;
  }

  byte RegHelper::GetBinaryValue(String rootKey, String Key, String paramName)
  {
     byte buff[4];
     TRegistry *reg = new TRegistry(KEY_WRITE);
     try
     {
        reg->RootKey = toHKEY(rootKey);
        reg->OpenKey(Key, true);
        reg->ReadBinaryData(paramName, buff, sizeof(buff));
        // byte buff[]={0x24, 0, 0, 0, 0x2E, 0xA0};

        /*
           if (buff[0] == 0x01) // && (buff[1] == 0x00) && (buff[2] == 0x00 ) && (buff[3] == 0x00 )
        */
        reg->CloseKey();
     }
     __finally
     {
        delete reg;
     }
     return *buff;
}

bool RegHelper::LoadPanels(TForm *ParentForm)
{
	TIniFile *winrusIni = new TIniFile(GetCurrentDir() + "\\winr0x2.conf");

        TStringList *panelsList = new TStringList();
        winrusIni->ReadSectionValues("Panels", panelsList);

        TStringList *sectionsList = new TStringList();
        winrusIni->ReadSections(sectionsList);

	int elemCount = sectionsList->Count;
	// --elemCount; // remove "Panels" sections


/*      String controlTitle = winrusIni->ReadSetring("Panels.First", "Title", "e");
        String controlType = winrusIni->ReadString("Panels.First", "Type", "e");
*/
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
		panelFromIni->Name = panelsList->Strings[i].SubString(0, panelsList->Strings[i].Pos("=")-1);

                panelFromIni->Caption = panelFromIni->Name;
                panelFromIni->SendToBack();

	       	TRadioButton *rbFromIni = new TRadioButton(*ParentForm);
		rbFromIni->Parent = ParentForm;

                byte lineLength = panelsList->Strings[i].Length();		// "PanelWindows = Windows" | 23
                byte rbCaption_eqSignPos = panelsList->Strings[i].Pos("=") + 1; // "PanelWindows =| 13
                byte rbCaptionLength = lineLength - rbCaption_eqSignPos + 1;    // we've get "| Windows"

                rbFromIni->Caption = panelsList->Strings[i].SubString(rbCaption_eqSignPos, rbCaptionLength);

        	rbFromIni->Top = 48 + i*24;
	        rbFromIni->Left = 8;
                rbFromIni->Color = clNavy;
                rbFromIni->Font->Color = clWhite;

                AnsiString rbName = "radiobutton" + (String)i;
        	rbFromIni->Name = rbName;

        	rbPair.first = rbName;
	        rbPair.second = panelFromIni;

	        rbFromIni->OnClick = RadioButtonClick;

	        rButtons.insert(std::make_pair(rbName, panelFromIni));
        };

/*
        std::vector<workingComponent> wElements;

        workingComponent *whElement = new workingComponent();
        std::vector<workingComponent>  *whElements = new std::vector<workingComponent>;
*/
	// Reading the whole INI elements-file
        for (int i = 1; i < elemCount; ++i) 		// '1' cause skip "Panels" section
        {
           workingControl *wElement = new workingControl();
           wElement->Index = i-1;

           String Section = sectionsList->Strings[i];

	   wElement->ControlName = "contrl" + static_cast<String>(i);
       	   wElement->type = winrusIni->ReadString(Section, "ControlType", ctCheckbox);
       	   wElement->Top = winrusIni->ReadInteger(Section, "Top", 0);
	   wElement->DataType = winrusIni->ReadString(Section, "DataType", "String");
       	   wElement->Title = winrusIni->ReadString(Section, "Title", Section + "Title error");

           String sParentPanel = winrusIni->ReadString(Section, "Parent", "ParentForm");
           wElement->parentPanel = StringToTPanel(ParentForm, sParentPanel);

         	wElement->RootKey = winrusIni->ReadString(Section, "RootKey", Section + "RootKey error");
		wElement->Key = winrusIni->ReadString(Section, "Key", Section + "Key error");
	        wElement->Param = winrusIni->ReadString(Section, "Param", Section + "Param error");
		wElement->Set = winrusIni->ReadInteger(Section, "Set", 0);
		wElement->Unset = winrusIni->ReadInteger(Section, "Unset", 0);
                
                if (static_cast<String>("String") == wElement->DataType) {};
                btns_n_chbxs->push_back(*wElement);
        }

        // Dynamically creating checkboxes and buttons
	for (int i = 0; i < elemCount-1; ++i)		// '-1' cause skip "Panels" section
        {
                workingControl wcElement = btns_n_chbxs[0][i];

                if (wcElement.type == ctCheckbox)
                	createControl<TCheckBox>(&wcElement)->OnClick = CheckBoxClick;
                if (wcElement.type == ctButton)
                	createControl<TButton>(&wcElement)->OnClick = ButtonClick;
        };
	return false;
}

void __fastcall RegHelper::CheckBoxClick(TObject* Sender)
{
   int gWorkingOn = static_cast<TButtonControl*>(Sender)->Tag;
   parentWindow->weWorkingOn = gWorkingOn;
   workingControl elm = btns_n_chbxs[0][gWorkingOn];
     /*
       AnsiString controlType = typeid(*Sender).name();
       if( static_cast<String>("TCheckBox") == controlType )
     */
     {
          ShowMessage(typeid(*Sender).name());
     }

}
void RegHelper::HandlerSetter(workingControl elm)
{

//   workingControl elm = btns_n_chbxs[0][weWorkingOn];
   if (elm.DataType == dtString)
   {
      RegSetValue(elm.RootKey, elm.Key, elm.Param, parentWindow->edtRegistryValue->Text);
   }


}

void __fastcall RegHelper::ButtonClick(TObject* Sender)
{
   	int gWorkingOn = static_cast<TButtonControl*>(Sender)->Tag;
	parentWindow->weWorkingOn = gWorkingOn;
	workingControl elm = btns_n_chbxs[0][gWorkingOn];
	// ShowMessage(static_cast<TButtonControl*>(Sender)->Name);
        if (elm.DataType == dtString)
        {
           String edtText = RegGetValue(elm.RootKey, elm.Key, elm.Param, elm.Param);
           parentWindow->ViewMainPanel(3, false, edtText);

        }
}

void __fastcall RegHelper::RadioButtonClick(TObject * Sender)
{
        TRadioButton *rb = static_cast<TRadioButton*>(Sender);
	// Bring to front the Panel, related to selected radio button
        rButtons[rb->Name]->BringToFront();
}

TPanel* RegHelper::StringToTPanel(TForm *ParentForm, String str_PanelFromIniFile)
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
		        if(str_PanelFromIniFile == panel->Name)
                        	return panel;

		}
	}
        return NULL;
}                                                     

__fastcall RegHelper::~RegHelper()
{
/*
   for (int i; i < wcElements->size(); ++i)
   	delete wcElements[0];
*/
	delete btns_n_chbxs;
}

HKEY RegHelper::toHKEY(String hKey)
{
   if (hKey == "HKR")
   	return pHKR;

   if (hKey == "HKCU")
   	return pHKCU;

   if (hKey == "HKLM")
   	return pHKLM;
};
