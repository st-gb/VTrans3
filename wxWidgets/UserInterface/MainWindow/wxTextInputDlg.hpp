///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wxTextInputDlg__
#define __wxTextInputDlg__

//#include <wx/gdicmn.h>
//#include <wx/font.h>
//#include <wx/button.h>
//#include <wx/colour.h>
#include <wx/dialog.h> //base class wxDialog
//#include <wx/panel.h>
//#include <wx/settings.h>
#include <wx/string.h> //class wxString
//#include <wx/textctrl.h>
#include <wxWidgets/UserInterface/MainWindow/MainWindowBase.hpp>
#include <wxWidgets/VTransApp.hpp> //wxGetApp()

#include <Parse/ParseByRise.hpp> //class ParseByRise
//#include <wxWidgets/wxParseTreePanel.hpp> //class wxParseTreePanel

#include <vector> //class std::vector

///////////////////////////////////////////////////////////////////////////

//Forward declarations (faster than #include)
class TranslationControllerBase;
class TranslationAndGrammarPart ;
//class wxHtmlWindow ;
class wxBoxSizer ;
class wxButton ;
//class wxWebKitCtrl ;

///////////////////////////////////////////////////////////////////////////////
/// Class wxTextInputDlg
///////////////////////////////////////////////////////////////////////////////
class wxTextInputDlg
  : public wxDialog, public wxWidgets::MainWindowBase
{
private:
  wxButton * mp_wxbutton ;
  wxBoxSizer * m_p_boxsizerToolButtons;

  wxToolBarToolBase * AddBitmapButtonAndTooltip(user_interface_control_actions itemID,
    /**constant array of constant char arrays*/
    const char * const * XPMbitmapData, const wxChar toolTip []//, wxSizer * p_wxsizer
    );
protected:
public:
    wxTextInputDlg(
      wxWindow * p_wxwindowParent,
      TranslationControllerBase & r_translationcontrollerbase,
      wxWindowID wxwindow_id = wxID_ANY,
      const wxString & cr_wxstrTitle = wxEmptyString,
      const wxPoint & cr_wxpointWindowPosition = wxDefaultPosition,
      const wxSize & cr_wxsizeWindow = wxSize(690, 557),
      long  lStyle = wxCLOSE_BOX | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    ~wxTextInputDlg();
    std::string GetAllTranslationPossibilites(
      const std::vector<std::string> & stdvec_stdstrWholeTransl,
      const std::vector<std::vector<TranslationAndGrammarPart> > &
        stdvec_stdvecTranslationAndGrammarPart);
    bool Show(bool show)
    {
      wxDialog::Show(show);
      if(show)
        //If this dialog is closed the app should exit.
        wxGetApp().SetTopWindow( (wxDialog*) this) ;
      //TODO senseful return value
      return true;
    }
};

#endif //__wxTextInputDlg__
