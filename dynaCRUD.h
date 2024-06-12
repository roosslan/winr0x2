#ifndef dynacrudH
#define dynacrudH

#include "wrUnit.h"
#include "functions.h"
#include "registry_funcs.h"

#include <map>
#include <vector>
#include <string>

class TWRForm;

//---------------------------------------------------------------------------

class dynaCRUD
{
       	TWRForm *parentWindow;
        TLabel *textTooltip;
public:
std::map<String, TPanel*> rbPanels;
std::vector<TRadioButton*> radioButtons;
std::map<String, TLabel*> pair_rb_n_TooltipLabels;

typedef struct wC{
        unsigned short Index;           // Порядковый номер в массиве
        void *onClick;
        unsigned short Left;
        unsigned short Top;
        byte DataType;    		// REG_SZ / DWORD
        bool BrowseOn;
	std::string ComponentType; 	// CheckBox/Button
	std::string RootKey;
	std::string Key;
	std::string Param;
        std::string ControlName;
	std::string Set;
	std::string Unset;
        std::string Title;
        std::string ToolTip;
        TPanel *parentPanel;
} workingControl;

   public:
   	std::vector<workingControl> *btns_n_chbxs;

	dynaCRUD(TWRForm *ParentWindow);
	bool LoadPanels(TWRForm *ParentForm);
	bool CreateButtons_nCheckboxes(TForm *ParentForm);
	TPanel* StringToTPanel(TForm *ParentForm, std::string str_PanelFromIniFile);
        void HandlerSetter(workingControl elm, bool Value);      // for Checkboxes
        void HandlerSetter(workingControl elm, int Value);
        void HandlerSetter(workingControl elm, std::string Value);

        template<typename T>
        T* createControl(workingControl *wcElement)
        {
           T *controlFromIniList = new T(wcElement->parentPanel);
	   controlFromIniList->Parent = wcElement->parentPanel;

           controlFromIniList->Top = wcElement->Top;
           controlFromIniList->Width = 300;		// Width of checkbox

	   controlFromIniList->Left = 18;

           controlFromIniList->Tag = wcElement->Index;
           controlFromIniList->Font->Color = clWhite;

           controlFromIniList->Name = wcElement->ControlName.c_str();


                if (wcElement->ComponentType == ctButton)			// лучше бы spec template, но
                {
                   controlFromIniList->Height = 12;
                   controlFromIniList->Width = 20;
                   controlFromIniList->Caption = "";

                   TLabel *textForButton = new TLabel(wcElement->parentPanel);
                   textForButton->Parent = wcElement->parentPanel;

                   textForButton->Caption = wcElement->Title.c_str();
                   textForButton->Left = 56;
                   textForButton->Top = wcElement->Top;

                   textForButton->Font->Color = clWhite;

                   controlFromIniList->Hint = wcElement->ToolTip.c_str();
                   textForButton->Hint = wcElement->ToolTip.c_str();
                   textForButton->OnMouseMove = parentWindow->func->BtnMouseMove;
                }
                else // Элемент - чекбокс
                {
                   controlFromIniList->Caption = wcElement->Title.c_str();
                   controlFromIniList->Left = wcElement->Left;
                   controlFromIniList->Hint = wcElement->ToolTip.c_str();

                }
                return controlFromIniList;
         };

};

#endif
