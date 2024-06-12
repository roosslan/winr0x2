#ifndef functionsH
#define functionsH

//---2001--------------------------------------------------------------------
#define ctCheckbox "Checkbox"
#define ctButton "Button"

#define tStr   "REG_SZ"
#define tDWORD "REG_DWORD"
#define tBin   "REG_BINARY"

#define dtBinary  0
#define dtString  1
#define dtDWORD   2




#include <vcl.h>
#include <stdio.h>
#include <vector>
#include <typeinfo>
#include <map>
#include <IniFiles.hpp>

#include "dynaCRUD.h"
#include "wrUnit.h"
//---------------------------------------------------------------------------

class TWRForm;
class dynaCRUD;

class funcs
{
 	TWRForm *parentWindow;
        dynaCRUD *dyn;
        TLabel *textTooltip;
public:

template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
};
//   	std::vector<workingControl> *btns_n_chbxs;
        funcs(TWRForm *ParentWindow);
	__fastcall ~funcs();

	void __fastcall RadioButtonClick(TObject *Sender);
        void disableFormControls(TForm *ParentForm);
        void enableFormControls(TForm *ParentForm);
	
	void __fastcall CheckBoxClick(TObject* Sender);
	void __fastcall ButtonClick(TObject* Sender);
        //void __fastcall WndProc(TMessage &Message);
	void __fastcall BtnMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall cbMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

        byte toDataType(std::string DataType);


      	bool Winhelp(void *THandle, std::string WinHLP, long a, int b){ return false; };			// Скопировать winhlp.exe
	bool CreateLNKFile(std::string Name, std::string Where, std::string PathToFile){ return false; }	// Реализовать в виде отдельного include
   };

#endif //  functionsH
