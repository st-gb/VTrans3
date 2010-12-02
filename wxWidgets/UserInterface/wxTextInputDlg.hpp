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

#include <Parse/ParseByRise.hpp> //class ParseByRise
//#include <wxWidgets/wxParseTreePanel.hpp> //class wxParseTreePanel

#include <vector> //class std::vector

///////////////////////////////////////////////////////////////////////////

//Forward declarations (faster than #include)
class TranslationAndGrammarPart ;
//class wxHtmlWindow ;
class wxBoxSizer ;
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
  void AddButtons() ;
	protected:
//  wxHtmlWindow * mp_wxhtmlwindow ;
//  wxWebKitCtrl * mp_wxwebkitctrl;
//    ParseByRise m_parsebyrise ;
    ParseByRise & m_parsebyrise ;
    wxBoxSizer * p_boxsizerOuter ;
		wxTextCtrl * mp_textctrlEnglishText;
		wxTextCtrl * mp_textctrlGermanText ;
		//wxPanel * mp_wxparsetreepanel;
		wxParseTreePanel * mp_wxparsetreepanel;
//    wxPanel * mp_wxpanelTop;
    wxSplitterWindow * mp_wxsplitterwindow ;
	
	public:
		wxTextInputDlg(
		  wxWindow * p_wxwindowParent,
		  wxWindowID wxwindow_id = wxID_ANY,
		  const wxString & cr_wxstrTitle = wxEmptyString,
		  const wxPoint & cr_wxpointWindowPosition = wxDefaultPosition,
		  const wxSize & cr_wxsizeWindow = wxSize( 690,557 ),
		  long lStyle = wxCLOSE_BOX | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
		  );
		~wxTextInputDlg();

  std::string  GetAllTranslationPossibilites(
    const std::vector<std::string> & stdvec_stdstrWholeTransl ,
    const std::vector<std::vector<TranslationAndGrammarPart> > &
      stdvec_stdvecTranslationAndGrammarPart
    ) ;
  //std::string
		void GetEntireInputText(std::string & r_stdstrInputText) ;
	void OnClose( wxCloseEvent & wxcmd ) ;
	void OnInfoButton( wxCommandEvent & wxcmd ) ;
	void OnLoadDictionaryButton(wxCommandEvent & wxcmd ) ;
	void OnReInitGrammarRulesButton( wxCommandEvent & wxcmd ) ;
	void OnResolveSuperclassGrammarParts( wxCommandEvent & wxcmd ) ;
	void OnResolve1ParseLevelButton( wxCommandEvent & wxcmd ) ;
	void OnShowTokenIndex2GrammarPartButton( wxCommandEvent & wxcmd ) ;
  void OnTranslateButton( wxCommandEvent & wxcmd ) ;
  //Neccessary in order to get scroll events; to avoid compilation errors.
  DECLARE_EVENT_TABLE()
};

#endif //__wxTextInputDlg__
