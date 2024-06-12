#include <vcl.h>
#pragma hdrstop

#include "wrUnit.h"
#include "rebUnit.h"

#include "registry_funcs.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TWRForm *WRForm;

__fastcall TWRForm::TWRForm(TComponent* Owner): TForm(Owner)
{
   // bitset<4> bitvec = {0001};

   dyn = new dynaCRUD(WRForm);
   func = new funcs(WRForm);
}

void __fastcall TWRForm::HelpPanelHint( TObject* Sender )
{
  // Хинт панели Справка
  StaticTextInfo->Caption = Application->Hint;
}


void __fastcall TWRForm::FormMouseDown( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y )
{
  // const int SC_DRAGMOVE = 0xF012;
  // Мою форму можно перетаскивать за поле
  ReleaseCapture;
  // perform(WM_SYSCOMMAND, SC_DRAGMOVE, 0);
}

void __fastcall TWRForm::FormCreate( TObject* Sender )
{
   imgList->GetIcon(1, imgTinyMenu->Picture->Icon);
   imgList->GetIcon(1, imgHideStartupFolder->Picture->Icon);
   imgList->GetIcon(1, imgAsWebView->Picture->Icon);
   imgList->GetIcon(1, imgShortcutNoArrow->Picture->Icon);
   imgList->GetIcon(1, img3dElements->Picture->Icon);
   imgList->GetIcon(1, imgOpenNewWindow->Picture->Icon);
   imgList->GetIcon(1, imgRemoveRecycleBin->Picture->Icon);
   imgList->GetIcon(1, imgLinkTo->Picture->Icon);

   std::string bs, bh;
//   TSearchRec DInfo;
//   TRegistry *Registry;

  Reboot = false;

  Application->OnHint = &HelpPanelHint;

  fillAppwizLst();

 // Получaю имя папки Windows
  WindowsDirectory = reg::ReadString(HKLM, "Software\\Microsoft\\Windows\\CurrentVersion\\Setup", "WinDir", "C:\\Windows");

 // Получaю в AStartup имя папки Автозагрузка
  WindowsStartupDirectory = reg::ReadString(HKCU, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Startup", "C:\\Startup");

// Информация в реестре о программе
  bh = reg::ReadString(HKCU, "Control Panel\\Colors", "ButtonHilight", "1");
  bs = reg::ReadString(HKCU, "Control Panel\\Colors", "ButtonShadow", "1");

  reg::WriteString(HKCU, "SOFTWARE\\RUS\\WinRusSE", "", "Настройщик операционной системы");
  reg::WriteString(HKCU, "SOFTWARE\\RUS\\WinRusSE", "Copyright", "RUSlan");
  reg::WriteString(HKCU, "SOFTWARE\\RUS\\WinRusSE", "LCD", "7.03.2001"); // Last compiled date
  reg::WriteString(HKCU, "SOFTWARE\\RUS\\WinRusSE", "Full", "WinRus 2001 Second Edition for Windows 98");

  exePath = ParamStr(0).c_str();
  //GP = ParamStr(0);


  if (!reg::ParamExists(HKCU, "SOFTWARE\\RUS\\WinRusSE", "LastInstallPath"))
         reg::WriteString(HKCU, "SOFTWARE\\RUS\\WinRusSE", "LastInstallPath", ParamStr(0).c_str());

    // Получaю в ADesktop имя папки Рабочего стола
  UserDesktopDirectory = reg::ReadString(HKCU, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Desktop", "C:\\");

  func->CreateLNKFile("WinRus", UserDesktopDirectory, ParamStr(0).c_str());


  // Получaю в APrograms имя папки Главное меню /  Программы
  WindowsStartMenuDirectory = reg::ReadString(HKCU, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Programs", "C:\\");

/*
  ->CreateLNKDir(WindowsStartMenuDirectory + "\\WinRus SE 2001");
  ->CreateLNKFile("WinRus SE 2001", WindowsStartMenuDirectory + "\\WinRus SE 2001", ParamStr(0).c_str());
  ->CreateLNKFile("WinRus SE Help", WindowsStartMenuDirectory + "\\WinRus SE 2001", ExtractFilePath(ParamStr(0)) + "\\WinRus.hlp");
  ->CreateLNKFile("WinRus SE Readme", WindowsStartMenuDirectory + "\\WinRus SE 2001", ExtractFilePath(ParamStr(0)) + "\\ReadMe.txt");
*/
   // Стрелки на ярлыках
  if (!reg::ParamExists(HKR, "lnkfile", "IsShortcut"))
    imgList->GetIcon(0, imgShortcutNoArrow->Picture->Icon);

  // Проверка WindowsStartupDirectory
  if ( ( FileGetAttr( WindowsStartupDirectory.c_str() ) & faHidden ) != 0 )
    imgList->GetIcon(0, imgHideStartupFolder->Picture->Icon);

  // Проверка 'Ярлык для'
  if (reg::ParamExists(HKCU, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer", "Link" ))
     imgList->GetIcon(0, imgLinkTo->Picture->Icon );

  // Проверка видимости Корзины
  if (reg::ParamExists(HKLM, "Software\\Microsoft\\Windows\\CurrentVersion\\explorer\\Desktop\\NameSpace\\{645FF040-5081-101B-9F08-00AA002F954E}", "Attributes"))
    imgList->GetIcon( 0, imgRemoveRecycleBin->Picture->Icon );

  // Проверка видимости меню Как веб-страница
  if (reg::ParamExists(HKLM, "Software\\Microsoft\\Windows\\CurrentVersion\\ExtShellViews\\{5984FFE0-28D4-11CF-AE66-08002B2E1262}", ""))
    imgList->GetIcon(0, imgAsWebView->Picture->Icon);

  // Проверка на 3D - образность
    if (reg::ReadString(HKCU, "Control Panel\\Colors", "3D", "0") == "1") // &&
     if (reg::ReadString(HKCU, "Control Panel\\Colors", "Swap", "0") == "1" )
       imgList->GetIcon(0, img3dElements->Picture->Icon);


  // Проверка существования меню Открыть в новом окне
  if (reg::ReadString(HKR, "Directory\\shell\\OpenNew\\Command", "", "") == "explorer %1")
    imgList->GetIcon( 0, imgOpenNewWindow->Picture->Icon );


  // Проверка меню на широту и высоту
  if (reg::ReadString(HKCU, "Control Panel\\Desktop\\WindowMetrics", "MenuHeight", "-260") == "10")
    if (reg::ReadString(HKCU, "Control Panel\\Desktop\\WindowMetrics", "MenuWidth", "-260") == "10" )
     imgList->GetIcon(0, imgTinyMenu->Picture->Icon);

  dyn->LoadPanels(WRForm);
  dyn->CreateButtons_nCheckboxes(WRForm);
}


void __fastcall TWRForm::btnExitClick( TObject* Sender )
{
 // Проверяю нужна ли перезагрузка
  if (!Reboot)
  {
     // Закрываю файл справки и потом приложение
    func->Winhelp(Handle, "WinRus.HLP", HELP_QUIT, 0);

    Application->Terminate();
  }
  else
    RebootForm->ShowModal();
}

void __fastcall TWRForm::btnColorShadowClick( TObject* Sender )
{
  if ( ColorPicker->Execute() )
  {
     TColor colr = ColorPicker->Color;
     reg::WriteString(HKCU, "Control Panel\\Colors", "ButtonDKShadow",
     			      ( IntToStr(GetRValue(colr)) + " " +
	    		 	IntToStr(GetGValue(colr)) + " " +
                                IntToStr(GetBValue(colr))).c_str()    );
    Reboot = true;
  }
}


void __fastcall TWRForm::btnColorWinBackgClick( TObject* Sender )
{
  if ( ColorPicker->Execute() )
  {
     TColor colr = ColorPicker->Color;
     reg::WriteString(HKCU, "Control Panel\\Colors", "AppWorkspace",
     			      ( IntToStr(GetRValue(colr)) + " " +
	    		 	IntToStr(GetGValue(colr)) + " " +
                                IntToStr(GetBValue(colr))).c_str()    );
    Reboot = true;
  }
}


void __fastcall TWRForm::btnColorTooltipsClick( TObject* Sender )
{
  if ( ColorPicker->Execute() )
  {
     TColor colr = ColorPicker->Color;
     reg::WriteString(HKCU, "Control Panel\\Colors", "InfoWindow",
     			      ( IntToStr(GetRValue(colr)) + " " +
	    		 	IntToStr(GetGValue(colr)) + " " +
                                IntToStr(GetBValue(colr))).c_str()    );
    Reboot = true;
  }
}


void __fastcall TWRForm::imgTinyMenuMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y )
{
   // Set
   dynaCRUD::workingControl menusz;
   menusz.DataType = REG_SZ;
   menusz.RootKey = HKCU;
   menusz.Key = "Control Panel\\Desktop\\WindowMetrics";

   menusz.Param = "MenuWidth";
   reg::WriteString(menusz.RootKey, menusz.Key, menusz.Param, "10");

   menusz.Param = "MenuHeight";
   reg::WriteString(menusz.RootKey, menusz.Key, menusz.Param, "10");

   // Unset
   // reg::WriteString(menusz.RootKey, menusz.Key, menusz.Param, "-260");
}

void __fastcall TWRForm::ViewMainPanel(bool browseBtn, String sValue)
{
   dynaCRUD::workingControl elm = dyn->btns_n_chbxs[0][weWorkingOn];

   pnlEditBoxPanel->BringToFront();

   edtRegistryValue->Text = sValue;


  pnlEditBoxPanel->Visible = true;

 
  if (browseBtn)
  {
    imgBrowse->Visible = true;
    btnBrowse->Visible = true;
  }
  WRForm->ActiveControl = edtRegistryValue;
}


void __fastcall TWRForm::HideMainPanel( )
{
//MainForm
  func->enableFormControls(WRForm);

  pnlEditBoxPanel->Visible = false;

  edtRegistryValue->Text = "";

 
}


void __fastcall TWRForm::btnClosePanelClick( TObject* Sender )
{
  HideMainPanel();
}

void __fastcall TWRForm::btnSettingSaveClick( TObject* Sender )
{
   dynaCRUD::workingControl Element = dyn->btns_n_chbxs[0][weWorkingOn];
   dyn->HandlerSetter(Element, static_cast<std::string>(edtRegistryValue->Text.c_str()) );
   HideMainPanel();
}
void __fastcall TWRForm::imgLinkToMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y )
{
   byte buff[4] = {0, 0, 0, 0};
   // Set  reg::Write_REG_BINARY(HKCU, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer", "Link", buff);

   // Unset   ->DeleteValue( "Link" );
}



void __fastcall TWRForm::btnBrowseClick( TObject* Sender )
{
  String S;
  S = "";
  if ( SelectDirectory( "", "", S ) );
      edtRegistryValue->Text = S;
};

void __fastcall TWRForm::imgOpenNewWindowMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y )
{
   dynaCRUD::workingControl opn;
   opn.DataType = REG_SZ;
   opn.RootKey = HKR;
   opn.Key = "Directory\\shell\\OpenNew";
   opn.Param = ""; // Default
   reg::WriteString(opn.RootKey, opn.Key, opn.Param, "Открыть в новом окне");

   opn.Key = "Directory\\shell\\OpenNew\\Command";
   reg::WriteString(opn.RootKey, opn.Key, opn.Param, "explorer %1");

   // Убираем
   // ->DeleteKey( "Directory\\shell\\OpenNew" );

}


void __fastcall TWRForm::rbAppwiz_cplClick( TObject* Sender )
{
   // Установка и удаление программ
   pnlAppwiz->BringToFront();
}


void __fastcall TWRForm::rbHelpClick( TObject* Sender )
{
  pnlHelp->BringToFront();
}


void __fastcall TWRForm::rbAboutClick( TObject* Sender )
{
   pnlAbout->BringToFront();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* void __fastcall TWRForm::CoolTrayIcon1DblClick( TObject* Sender )
{
  CoolTrayIcon1->ShowMainForm;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void __fastcall TWRForm::img3dElementsMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y )
{
   dynaCRUD::workingControl s3d;
   s3d.DataType = REG_SZ;
   s3d.RootKey = HKCU;
   s3d.Key = "Control Panel\\Colors";
   s3d.Param = "ButtonHilight";
// reg::WriteString(s3d.RootKey, s3d.Key, s3d.Param, bhs);

   s3d.Param = "ButtonShadow";
// reg::WriteString(s3d.RootKey, s3d.Key, s3d.Param, bss);

   s3d.Param = "3D";
   reg::WriteString(s3d.RootKey, s3d.Key, s3d.Param, "1");

   s3d.Param = "Swap";
   reg::WriteString(s3d.RootKey, s3d.Key, s3d.Param, "1");

   // ->DeleteValue( "3D" );
   // ->DeleteValue( "Swap" );
}

void __fastcall TWRForm::lstAppwizClick( TObject* Sender )
{
  if ( (lstAppwiz->SelCount > lstAppwiz->ItemIndex ) || (lstAppwiz->SelCount < 0))
  {
    btnDel->Enabled = true;
    btnDel->Default = true;
  }
  else
    btnDel->Enabled = false;
}

void __fastcall TWRForm::fillAppwizLst( )
{
   lstAppwiz->Clear();
   std::list<std::string> lstSubkeys = reg::getSubkeys(HKLM, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

   std::vector<std::string> *displayNames = new std::vector<std::string>;
   for (unsigned int i = 0; i < lstSubkeys.size(); ++i)
   {
      displayNames->push_back(reg::ReadString(HKLM, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + lstSubkeys.front(), "DisplayName", ""));
      lstSubkeys.pop_front();
   }

   for (unsigned int i = 0; i < displayNames->size(); ++i)
   {
     if (!displayNames[0][i].empty())
         lstAppwiz->Items->Add(displayNames[0][i].c_str());
   }

   delete displayNames;
}

void __fastcall TWRForm::btnDelClick(TObject* Sender)
{
  std::list<std::string> lstSubkeys = reg::getSubkeys(HKLM, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

  std::map<std::string, std::string> Path_n_Names;
  for (unsigned int i = 0; i < lstSubkeys.size(); ++i)
  {
     Path_n_Names.insert(std::make_pair("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + lstSubkeys.front(),
	     reg::ReadString(HKLM, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + lstSubkeys.front(), "DisplayName", "")));
     lstSubkeys.pop_front();
  }

  // reg::DeleteKey(HKLM, Path_n_Names[])
  fillAppwizLst();
}


void __fastcall TWRForm::btnColorUCursorClick( TObject* Sender )
{
   TColor colr = ColorPicker->Color;
     reg::WriteString(HKCU, "Control Panel\\Colors", "HotTrackingColor",
     			      ( IntToStr(GetRValue(colr)) + " " +
	    		 	IntToStr(GetGValue(colr)) + " " +
                                IntToStr(GetBValue(colr))).c_str()    );
}


void __fastcall TWRForm::imgRemoveRecycleBinMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y )
{
   dynaCRUD::workingControl recb;
   recb.DataType = REG_SZ;
   recb.RootKey = HKU;
   recb.Key = ".DEFAULT\\SOFTWARE\\Classes\\CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}";
   recb.Param = ""; // Default

   reg::WriteString(recb.RootKey, recb.Key, recb.Param, "");

   recb.RootKey = HKLM;
   recb.Key = "Software\\Microsoft\\Windows\\CurrentVersion\\explorer\\Desktop\\NameSpace\\{645FF040-5081-101B-9F08-00AA002F954E}";
   recb.Param = ""; // Default

   reg::WriteString(recb.RootKey, recb.Key, recb.Param, "");
  // Убрать->DeleteKey( "Software\\Microsoft\\Windows\\CurrentVersion\\explorer\\Desktop\\NameSpace\\{645FF040-5081-101B-9F08-00AA002F954E}" );
}


void __fastcall TWRForm::imgAsWebViewMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y )
{
   dynaCRUD::workingControl imgasw;
   imgasw.DataType = REG_SZ;
   imgasw.RootKey = HKLM;
   imgasw.Key = "Software\\Microsoft\\Windows\\CurrentVersion\\ExtShellViews\\{5984FFE0-28D4-11CF-AE66-08002B2E1262}";

   reg::WriteString(imgasw.RootKey, imgasw.Key, "HelpText", "Отображение элементов в веб-виде" );
   reg::WriteString(imgasw.RootKey, imgasw.Key, "MenuName", "Как &веб-страница" );
   reg::WriteString(imgasw.RootKey, imgasw.Key, "ToolTipText", "Web" );
   // Убрать    ->DeleteKey("Software\\Microsoft\\Windows\\CurrentVersion\\ExtShellViews";
}
 
void __fastcall TWRForm::btnColorTooltipTextClick( TObject* Sender )
{
     TColor colr = ColorPicker->Color;
     reg::WriteString(HKCU, "Control Panel\\Colors", "InfoText",
     			      ( IntToStr(GetRValue(colr)) + " " +
	    		 	IntToStr(GetGValue(colr)) + " " +
                                IntToStr(GetBValue(colr))).c_str()    );
}

void __fastcall TWRForm::btnCreateShutdownLinkClick( TObject* Sender )
{
   // ->CreateLNKFileEx( "Завершить работу", ADesktop, AFolder + "\\RUNDLL.EXE", "", " user.exe,exitwindows", AFolder + "\\System\\user.exe", 1 );
   func->CreateLNKFile( "Завершить работу",  "\\RUNDLL.EXE", "");
}


void __fastcall TWRForm::btnCreateRebootLinkClick( TObject* Sender )
{
   // ->CreateLNKFileEx( "Перезагрузить Windows", ADesktop, AFolder + "\\RUNDLL.EXE", "", " user.exe,exitwindowsexec", AFolder + "\\System\\user.exe", 6 );
   func->CreateLNKFile( "Перезагрузить Windows", "\\RUNDLL.EXE", "");
}



void __fastcall TWRForm::imgShortcutNoArrowMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y )
{
   // imgList->GetIcon(1, imgShortcutNoArrow->Picture->Icon );
   dynaCRUD::workingControl elnk;
   elnk.DataType = REG_SZ;
   elnk.RootKey = HKR;
   elnk.Key = "lnkfile";
   elnk.Param = "IsShortcut";

   reg::WriteString(elnk.RootKey, elnk.Key, elnk.Param, "");

   elnk.Key = "piffile";
   reg::WriteString(elnk.RootKey, elnk.Key, elnk.Param, "");

/*
    imgList->GetIcon(0, imgShortcutNoArrow->Picture->Icon);
    Registry->DeleteValue( "IsShortcut" );
    Registry->OpenKey( "piffile", true );
    Registry->DeleteValue( "IsShortcut" );
*/
}


void __fastcall TWRForm::CleanCreateMenu( )
{
  // reg::DeleteKey(HKR, buff + "\\ShellNew"
}


void __fastcall TWRForm::btnCleanCreateMenuClick( TObject* Sender )
{
   CleanCreateMenu();
}

void __fastcall TWRForm::imgHideStartupFolderMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y )
{
  imgHideStartupFolder->Tag = imgHideStartupFolder->Tag + 1;
  if ( imgHideStartupFolder->Tag - 1 == 0 )
  {  // Не выбрано
    imgList->GetIcon(1, imgHideStartupFolder->Picture->Icon);
    imgHideStartupFolder->Tag = 1;
    FileSetAttr(WindowsStartupDirectory.c_str(), faArchive);
  }
  else
  {  // Выбрано
    imgList->GetIcon(0, imgHideStartupFolder->Picture->Icon);
    imgHideStartupFolder->Tag = 0;
    FileSetAttr(WindowsStartupDirectory.c_str(), faHidden);
  }
}

void __fastcall TWRForm::FormKeyPress(TObject *Sender, char &Key)
{
	if( 27 == Key)
          Close();
}

void __fastcall TWRForm::FormShow(TObject *Sender)
{
   dyn->rbPanels["rb0"]->BringToFront();
   dyn->radioButtons[0]->SetFocus();	
}
//---------------------------------------------------------------------------

void __fastcall TWRForm::rbBuiltinClick(TObject *Sender)
{
	pnlBuiltin->BringToFront();	
}
//---------------------------------------------------------------------------

void __fastcall TWRForm::btnMoreClick(TObject *Sender)
{
	Application->HelpCommand( HELP_FINDER, 0 );	
}
//---------------------------------------------------------------------------

