///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wxTextInputDlg__
#define __wxTextInputDlg__

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/button.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/dialog.h>

#include <Parse/ParseByRise.hpp>
#include <wxWidgets/wxParseTreePanel.hpp>

///////////////////////////////////////////////////////////////////////////

class wxHtmlWindow ;
//class wxWebKitCtrl ;

///////////////////////////////////////////////////////////////////////////////
/// Class wxTextInputDlg
///////////////////////////////////////////////////////////////////////////////
class wxTextInputDlg : public wxDialog 
{
	private:
	
	protected:
  wxHtmlWindow * mp_wxhtmlwindow ;
//  wxWebKitCtrl * mp_wxwebkitctrl;
    ParseByRise m_parsebyrise ;
		wxTextCtrl * mp_textctrlEnglishText;
		wxTextCtrl * mp_textctrlGermanText ;
		//wxPanel * mp_wxparsetreepanel;
		wxParseTreePanel * mp_wxparsetreepanel;
//    wxPanel * mp_wxpanelTop;
    wxSplitterWindow * mp_wxsplitterwindow ;
	
	public:
		wxTextInputDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 690,557 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~wxTextInputDlg();

  //std::string
		void GetEntireInputText(std::string & r_stdstrInputText) ;
	void OnClose( wxCloseEvent & wxcmd ) ;
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