///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxTextInputDlg.h"
#include "wxParseTreePanel.h"
#include <wx/textctrl.h>
#include <wx/event.h> //for EVT_CLOSE

///////////////////////////////////////////////////////////////////////////

//An enum guarantees a unique number for each element.
enum
{
  ID_Translate = 1,
  ID_Close
};

BEGIN_EVENT_TABLE(wxTextInputDlg, wxDialog)
  EVT_BUTTON( ID_Translate , wxTextInputDlg::OnTranslateButton)
  EVT_CLOSE( wxTextInputDlg::OnClose)
END_EVENT_TABLE()

wxTextInputDlg::wxTextInputDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style )
: wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  wxButton * p_wxbutton ;

  mp_wxsplitterwindow = new wxSplitterWindow( this, wxID_ANY ) ;

  m_panelParseTree = new //wxPanel(
    wxParseTreePanel(
    //new wxParseTreePanel(
    //this
    mp_wxsplitterwindow
    //(wxFrame*) mp_wxsplitterwindow
    , wxID_ANY
    , wxDefaultPosition
    , wxDefaultSize,
    wxTAB_TRAVERSAL
    );

	wxPanel * m_panelSplitterTop = new wxPanel(
    //parent window
    //this
    mp_wxsplitterwindow
//    , wxID_ANY,
//    wxDefaultPosition,
//    wxDefaultSize,
//    wxTAB_TRAVERSAL
    );

	mp_textctrlEnglishText = new wxTextCtrl(
    //this
    mp_wxsplitterwindow
    //m_panelSplitterTop
    , wxID_ANY, 
    wxEmptyString,
    wxDefaultPosition,
    wxDefaultSize,
    wxHSCROLL|wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB
    );

	//wxGridBagSizer * p_gridbagsizerSplitterTop = new wxGridBagSizer( ) ;
	wxFlexGridSizer * p_flexgridsizerSplitterTop = new wxFlexGridSizer( 0 ) ;
	wxBoxSizer * p_boxsizerSplitterTop = new wxBoxSizer( wxHORIZONTAL ) ;

  //p_gridbagsizerSplitterTop->Add( m_textCtrl7, 1, wxEXPAND | wxALL, 5 );
  p_flexgridsizerSplitterTop->Add(
  //p_boxsizerSplitterTop->Add(
    mp_textctrlEnglishText,
    1,
    wxEXPAND |
    wxALL
    , 5
    );
//  p_flexgridsizerSplitterTop->Add(
//  p_boxsizerSplitterTop->Add(
//    p_wxbutton ,
//    0, wxALL, 5 );

	//wxFlexGridSizer * p_flexgridsizerOuter;
	//p_flexgridsizerOuter = new wxFlexGridSizer( 2, 1, 0, 0 );
	wxGridSizer * p_gridsizerOuter;
	p_gridsizerOuter = new wxGridSizer( 
    1, //rows
    1, //cols
    0,
    0 );
  wxBoxSizer * p_boxsizerOuter = new wxBoxSizer( wxVERTICAL ) ;
	
//	gSizer3->Add( m_textCtrl7, 1, wxALL|wxEXPAND, 5 );

  m_panelSplitterTop->SetSizer(
    //p_gridbagsizerSplitterTop
    p_flexgridsizerSplitterTop
    //p_boxsizerSplitterTop
    ) ;
  mp_wxsplitterwindow->SplitHorizontally(
    //m_panelSplitterTop
    mp_textctrlEnglishText
    , m_panelParseTree
    ) ;

  p_wxbutton = new wxButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    //this
    , //wxID_ANY
    ID_Translate
    , wxT("translate")
    ) ;
  p_boxsizerOuter->Add(
    p_wxbutton
    //m_textCtrl7
    //p_wxbutton
    //strech factor. 0=do not stretch
    , 0
      //0
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
	//gSizer3->Add( m_panel1, 1, wxEXPAND | wxALL, 5 );
  //gSizer3->Add( p_wxbutton, 0 , wxALL, 5 );
	//p_gridsizerOuter->Add(
  p_boxsizerOuter->Add(
    mp_wxsplitterwindow
    //m_textCtrl7
    //p_wxbutton
    , 1
      //0
    , wxEXPAND |
      wxBOTTOM
    , 2 );
//  p_boxsizerOuter->Add(
//    m_panel1
//    //m_textCtrl7
//    //p_wxbutton
//    , 1
//      //0
//    , wxEXPAND |
//      wxBOTTOM
//    , 2 );
	
  //m_panelSplitterTop->Layout() ;
	this->SetSizer( //p_gridsizerOuter
    p_boxsizerOuter
    );
	this->SetAutoLayout(true);

	this->Layout();
}

wxTextInputDlg::~wxTextInputDlg()
{
}

void wxTextInputDlg::OnClose( wxCloseEvent & wxcmd )
{
  //Call delete to exit this application. (without it is not finished)
  delete(this) ;
  //::wxGetApp().ExitMainLoop() ;
}

void wxTextInputDlg::OnTranslateButton( wxCommandEvent & wxcmd )
{
  std::string stdstrText ;
  wxString wxstringWholeText ;
  wxTextPos wxtextposNumberOfCharacters  ;
  wxtextposNumberOfCharacters = mp_textctrlEnglishText->GetLastPosition() ;
  wxstringWholeText = mp_textctrlEnglishText->GetRange(0 ,
    wxtextposNumberOfCharacters) ;
  stdstrText = std::string( wxstringWholeText.c_str() ) ;
  m_parsebyrise.ClearParseTree() ;
  m_parsebyrise.CreateInitialGrammarParts ( stdstrText ) ;
  m_parsebyrise.ResolveGrammarRulesForAllParseLevels() ;
  m_panelParseTree->DrawParseTree(m_parsebyrise) ;
  //You can also trigger this call by calling Refresh()/Update()
  //m_panel1->Refresh() ;
}
