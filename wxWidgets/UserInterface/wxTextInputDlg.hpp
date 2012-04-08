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
#include <wxWidgets/UserInterface/wxGermanTranslationPanel.hpp>

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
class wxParseTreePanel ;
class wxSplitterWindow ;
class wxTextCtrl ;
//class wxWebKitCtrl ;

///////////////////////////////////////////////////////////////////////////////
/// Class wxTextInputDlg
///////////////////////////////////////////////////////////////////////////////
class wxTextInputDlg
  : public wxDialog
{
private:
  wxButton * mp_wxbutton ;
  TranslationControllerBase & m_translationcontrollerbase;

  void AddButtons() ;
  void AddAddGrammarRulesButton( wxSizer * p_sizer ) ;
  void AddDrawParseTreeButton(wxSizer * p_sizer ) ;
  void AddLoadDictionaryButton( wxSizer * p_sizer);
  void AddRemoveGrammarRulesButton( wxSizer * p_sizer ) ;
  void AddRemoveVocAttrDefsButton( wxSizer * p_sizer ) ;
  void AddAddTranslationRulesButton( wxSizer * p_sizer ) ;
  void AddAddTransformationRulesButton( wxSizer * p_sizer ) ;
  void AddAddVocAttrDefsButton( wxSizer * p_sizer ) ;
  void AddRemoveTranslationRulesButton( wxSizer * p_sizer ) ;
  void AddRemoveTransformationRulesButton( wxSizer * p_sizer ) ;
  void AddResolve1ParseLevelButton( wxSizer * p_sizer ) ;
  void AddResolveSuperClassesButton( wxSizer * p_sizer ) ;
  void AddShowInformationButton( wxSizer * p_sizer ) ;
  void AddTranslateButton( wxSizer * p_sizer ) ;
  void AddTruncateLogFileButton( wxSizer * p_sizer ) ;
  void AddReInitializeGrammarButton(wxSizer * p_wxsizer);
protected:
    ParseByRise & m_parsebyrise;
    wxBoxSizer * p_boxsizerOuter;
    wxTextCtrl * mp_textctrlEnglishText;
    wxGermanTranslationPanel * m_p_wxgermantranslationpanel;
    wxParseTreePanel * mp_wxparsetreepanel;
    wxSplitterWindow * mp_wxsplitterwindow;
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
    void GetEntireInputText(std::string & r_stdstrInputText);
    void OnAddGrammarRules(wxCommandEvent & wxcmd);
    void OnAddTransformationRules(wxCommandEvent & wxcmd);
    void OnAddTranslationRules(wxCommandEvent & wxcmd);
    void OnAddVocAttrDefs(wxCommandEvent & wxcmd);
    void OnClose(wxCloseEvent & wxcmd);
    void OnDrawParseTreeButton(wxCommandEvent & wxcmd);
    void OnInfoButton(wxCommandEvent & wxcmd);
    void OnLoadDictionaryButton(wxCommandEvent & wxcmd);
    void OnReInitGrammarRulesButton(wxCommandEvent & wxcmd);
    void OnRemoveGrammarRules(wxCommandEvent & wxcmd);
    void OnRemoveTransformationRules(wxCommandEvent & wxcmd);
    void OnRemoveTranslationRules(wxCommandEvent & wxcmd);
    void OnRemoveVocAttrDefs(wxCommandEvent & wxcmd);
    void OnResolveSuperclassGrammarParts(wxCommandEvent & wxcmd);
    void OnResolve1ParseLevelButton(wxCommandEvent & wxcmd);
    void OnShowTokenIndex2GrammarPartButton(wxCommandEvent & wxcmd);
    void OnTranslateButton(wxCommandEvent & wxcmd);
    void OnTruncateLogFileButton( wxCommandEvent & wxcmd );
  DECLARE_EVENT_TABLE()
};

#endif //__wxTextInputDlg__
