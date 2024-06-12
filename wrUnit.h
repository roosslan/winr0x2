#ifndef wrUnitH
#define wrUnitH

#include <filectrl.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <string>
#include <deque>

#include "functions.h"
#include "dynaCRUD.h"

class dynaCRUD;
class funcs;

class TWRForm : public TForm
{
__published:	// IDE-managed Components
//  TCoolTrayIcon CoolTrayIcon1;
	TShape *pnlMain;
	TLabel *StaticTextTitle;
	TShape *pnlWindowTitle;
	TShape *sidebarMenu;
	TShape *sidebarMisc;
	TShape *sidebarWindows;
	TRadioButton *rbHelp;
	TShape *sidebarAccess;
	TShape *sidebarAppwiz;
	TShape *Shape9;
	TRadioButton *rbAbout;
	TShape *sidebarHelp;
	TImageList *imgList;
	TButton *btnExit;
	TColorDialog *ColorPicker;
        TOpenDialog *OpenDialog1;
	TPanel *pnlEditBoxPanel;
	TShape *pnlHorizontal;
	TLabel *lblChange;
	TButton *btnClosePanel;
	TShape *pnlVertical;
	TEdit *edtRegistryValue;
	TButton *btnSettingSave;
	TLabel *StaticTextPnl_edtBox;
      	TImage *imgBrowse;
      	TButton *btnBrowse;
	TPanel *pnlHelp;
	TImage *imgHelp;
	TLabel *StaticTextMenu;
	TLabel *StaticTextMisc;
	TLabel *StaticTextWindows;
	TLabel *StaticTextAccess;
	TLabel *StaticTextInfo;
	TBevel *pnlBevelHelp;
	TPanel *pnlAbout;
	TImage *imgLogo;
	TLabel *StaticTextAbout;
	TRichEdit *StaticTextWinrus;
	TRichEdit *StaticTextThanks;
	TLabel *StaticTextName;
	TLabel *StaticTextDate;
	TLabel *StaticTextFreeware;
	TLabel *StaticTextCopyright;
	TButton *btnHide;
	TButton *btnMore;
	TLabel *StaticTextMore;
	TPanel *pnlAppwiz;
	TLabel *StaticTextAppwiz;
	TLabel *StaticTextRemove;
	TListBox *lstAppwiz;
	TButton *btnDel;
	TRadioButton *rbAppwiz_cpl;
	TShape *sidebarBuiltin;
	TShape *sidebarAbout;
	TRadioButton *rbBuiltin;
	TPanel *pnlBuiltin;
	TImage *imgRemoveRecycleBin;
	TLabel *StaticTextRemoveRecycleBin;
	TImage *imgAsWebView;
	TLabel *StaticTextWebView;
	TLabel *StaticTextTinyMenu;
	TImage *imgTinyMenu;
	TLabel *StaticTextOpenNewWindow;
	TImage *imgOpenNewWindow;
	TImage *imgShortcutNoArrow;
	TLabel *StaticTextShortcutNoArrow;
	TLabel *StaticText3dElements;
	TImage *img3dElements;
	TButton *btnCleanCreateMenu;
	TLabel *StaticTextCleanCreateMenu;
	TButton *btnCreateShutdownLink;
	TLabel *StaticTextCreateShutdownLnk;
	TLabel *StaticTextCreateRebootLnk;
	TButton *btnCreateRebootLink;
	TImage *imgHideStartupFolder;
	TLabel *StaticTextStartup;
	TButton *btnColorUCursor;
	TLabel *StaticTextColorUCursor;
	TButton *btnColorTooltips;
	TLabel *StaticTextColorTooltips;
	TLabel *StaticTextColorWinBackg;
	TLabel *StaticTextColorShadow;
	TButton *btnColorShadow;
	TButton *btnColorWinBackg;
	TLabel *StaticTextColorTooltipText;
	TButton *btnColorTooltipText;
	TImage *imgLinkTo;
	TLabel *StaticTextLinkTo;
        void __fastcall FormMouseDown(TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y );
        void __fastcall FormCreate(TObject* Sender );
        void __fastcall btnExitClick(TObject* Sender );
        void __fastcall btnColorShadowClick(TObject* Sender );
        void __fastcall btnColorWinBackgClick(TObject* Sender );
        void __fastcall btnColorTooltipsClick(TObject* Sender );
        void __fastcall imgTinyMenuMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y );
        void __fastcall fillAppwizLst( );
        void __fastcall ViewMainPanel(bool browseBtn, String sValue);
        void __fastcall HideMainPanel( );
        void __fastcall btnClosePanelClick( TObject* Sender );
        void __fastcall btnSettingSaveClick( TObject* Sender );
        void __fastcall imgLinkToMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y );
        void __fastcall btnBrowseClick( TObject* Sender );
        void __fastcall imgOpenNewWindowMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y );
        void __fastcall rbAppwiz_cplClick( TObject* Sender );
        void __fastcall rbHelpClick( TObject* Sender );
        void __fastcall rbAboutClick( TObject* Sender );
        void __fastcall img3dElementsMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y );
        void __fastcall lstAppwizClick( TObject* Sender );
        void __fastcall btnDelClick( TObject* Sender );
        void __fastcall btnColorUCursorClick( TObject* Sender );
        void __fastcall imgRemoveRecycleBinMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y );
        void __fastcall imgAsWebViewMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y );
        void __fastcall btnColorTooltipTextClick( TObject* Sender );
	void __fastcall btnCreateShutdownLinkClick( TObject* Sender );
        void __fastcall btnCreateRebootLinkClick( TObject* Sender );
        void __fastcall imgShortcutNoArrowMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y );
        void __fastcall CleanCreateMenu( );
        void __fastcall btnCleanCreateMenuClick( TObject* Sender );
        void __fastcall imgHideStartupFolderMouseUp( TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y );
  	void __fastcall HelpPanelHint( TObject* Sender );
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall rbBuiltinClick(TObject *Sender);
	void __fastcall btnMoreClick(TObject *Sender);
 
public:		// User declarations
        __fastcall TWRForm(TComponent* Owner);
	int weWorkingOn;	// current processing setting
        TDateTime BegTime;
        dynaCRUD *dyn;
	funcs *func;
	std::string exePath, // GP,
            WindowsStartupDirectory,      // AStartup,
            UserDesktopDirectory,         // ADesktop,
            WindowsDirectory,             // AFolder,
            WindowsStartMenuDirectory;    // APrograms

	// true если нужна перезагрузка
	bool Reboot;
};

extern PACKAGE TWRForm *WRForm;



#endif //  wrUnitH