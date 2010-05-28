///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxTextInputDlg.h"
#include "wxParseTreePanel.h"
#include <wx/filedlg.h>
#include <wx/textctrl.h>
#include <wx/event.h> //for EVT_CLOSE

#include <Translate/TranslateParseByRiseTree.h>

///////////////////////////////////////////////////////////////////////////

//An enum guarantees a unique number for each element.
enum
{
  ID_Close = 1 ,
  ID_LoadDictionary ,
  ID_ReInitGrammarRules ,
  ID_Translate ,
};

BEGIN_EVENT_TABLE(wxTextInputDlg, wxDialog)
  EVT_BUTTON( ID_Translate , wxTextInputDlg::OnTranslateButton)
  EVT_BUTTON( ID_LoadDictionary , wxTextInputDlg::OnLoadDictionaryButton)
  EVT_BUTTON( ID_ReInitGrammarRules , wxTextInputDlg::OnReInitGrammarRulesButton)
  EVT_CLOSE( wxTextInputDlg::OnClose)
END_EVENT_TABLE()

wxTextInputDlg::wxTextInputDlg( wxWindow* parent, wxWindowID id,
  const wxString& title, const wxPoint& pos, const wxSize& size, long style )
: wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  wxButton * p_wxbutton ;

  mp_wxsplitterwindow = new wxSplitterWindow( this, wxID_ANY ) ;
//  mp_wxpanelTop = new wxPanel( mp_wxsplitterwindow ) ;

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

	 //wxGridBagSizer * p_gridbagsizerSplitterTop = new wxGridBagSizer( ) ;
	  wxFlexGridSizer * p_flexgridsizerSplitterTop = new wxFlexGridSizer( 0 ) ;
	  p_flexgridsizerSplitterTop->SetFlexibleDirection( wxVERTICAL ) ;
	  wxBoxSizer * p_boxsizerSplitterTop = new wxBoxSizer( //wxHORIZONTAL
	    wxVERTICAL ) ;

  m_panelSplitterTop->SetSizer(
    //p_gridbagsizerSplitterTop
//    p_flexgridsizerSplitterTop
    p_boxsizerSplitterTop
    ) ;

	mp_textctrlEnglishText = new wxTextCtrl(
    //this
//    mp_wxsplitterwindow
    m_panelSplitterTop
    , wxID_ANY, 
    wxEmptyString,
    wxDefaultPosition,
    wxDefaultSize,
    wxHSCROLL|wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB
    );
  mp_textctrlGermanText = new wxTextCtrl(
    //this
//    mp_wxsplitterwindow
    m_panelSplitterTop
    , wxID_ANY,
    wxEmptyString,
    wxDefaultPosition,
    wxDefaultSize,
    wxHSCROLL|wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB
    );

  //p_gridbagsizerSplitterTop->Add( m_textCtrl7, 1, wxEXPAND | wxALL, 5 );
//  p_flexgridsizerSplitterTop->Add(
  p_boxsizerSplitterTop->Add(
    mp_textctrlEnglishText,
    1,
    wxEXPAND |
    wxALL
    , 5
    );
//  p_flexgridsizerSplitterTop->Add(
  p_boxsizerSplitterTop->Add(
    mp_textctrlGermanText,
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
  wxBoxSizer * p_boxsizerButtons = new wxBoxSizer( wxHORIZONTAL ) ;
	
//	gSizer3->Add( m_textCtrl7, 1, wxALL|wxEXPAND, 5 );

  mp_wxsplitterwindow->SplitHorizontally(
    m_panelSplitterTop
//    mp_textctrlEnglishText
    , m_panelParseTree
    ) ;
//  mp_wxsplitterwindow->SplitHorizontally(
//    //m_panelSplitterTop
//    mp_textctrlGermanText
//    , m_panelParseTree
//    ) ;

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
  p_boxsizerButtons->Add(
    p_wxbutton
    //m_textCtrl7
    //p_wxbutton
    //strech factor. 0=do not stretch
    , 0
      //0
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
  p_wxbutton = new wxButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    //this
    , //wxID_ANY
    ID_ReInitGrammarRules
    , wxT("re-init grammar")
    ) ;
  p_boxsizerButtons->Add(
    p_wxbutton
    //m_textCtrl7
    //p_wxbutton
    //strech factor. 0=do not stretch
    , 0
      //0
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
  p_wxbutton = new wxButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    //this
    , //wxID_ANY
    ID_LoadDictionary
    , wxT("re-load dict...")
    ) ;
  p_boxsizerButtons->Add(
    p_wxbutton
    //m_textCtrl7
    //p_wxbutton
    //strech factor. 0=do not stretch
    , 0
      //0
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
  p_boxsizerOuter->Add( p_boxsizerButtons ) ;
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

void wxTextInputDlg::OnLoadDictionaryButton( wxCommandEvent & wxcmd )
{
  wxFileDialog wxfiled(
    this ,
    "Choose a dict file" ,
    "" , //defaultDir
    "" //const wxString&  defaultFile = ""
    , "*.txt" //const wxString&  wildcard = "*.*"
    , wxOPEN | wxFILE_MUST_EXIST //long style = 0,
    ) ;
  wxfiled.ShowModal() ;
}

void wxTextInputDlg::OnReInitGrammarRulesButton( wxCommandEvent & wxcmd )
{
  m_parsebyrise.ClearAllGrammarStuff() ;
  m_parsebyrise.InitGrammarRules() ;
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
  DEBUG_COUT("before resolving GrammarRulesForAllParseLevels \n")
  m_parsebyrise.ResolveGrammarRulesForAllParseLevels() ;

  TranslateParseByRiseTree translateParseByRiseTree( m_parsebyrise ) ;
  DEBUG_COUT("before translation\n")
  std::string stdstrWholeTransl ;
  translateParseByRiseTree.Translate( m_parsebyrise, stdstrWholeTransl ) ;

  mp_textctrlGermanText->SetValue( stdstrWholeTransl ) ;
  m_panelParseTree->DrawParseTree(m_parsebyrise) ;
  DEBUG_COUT("end of OnTranslateButton\n")
  //You can also trigger this call by calling Refresh()/Update()
  //m_panel1->Refresh() ;
}
