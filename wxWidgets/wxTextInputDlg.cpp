///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//#include "wx/html/webkit.h"
//#include <wx/filedlg.h>
//#include <wx/defs.h>
//#include <wx/frame.h>
//#include <wx/textctrl.h>
//#include <wx/event.h> //for EVT_CLOSE
#include <wx/html/htmlwin.h>

#include "wxTextInputDlg.hpp"
#include <Translate/TranslateTreeTraverser.hpp> //TranslationAndGrammarPart
#include "wxParseTreePanel.hpp"
#include "wxHTMLfileOutput.hpp"

#include <sstream>

#include <Translate/TranslateParseByRiseTree.h>

///////////////////////////////////////////////////////////////////////////

//An enum guarantees a unique number for each element.
enum
{
  ID_Close = 1 ,
  ID_LoadDictionary ,
  ID_ReInitGrammarRules ,
  ID_ShowTokenIndex2GrammarPart ,
  ID_ResolveSuperclass ,
  ID_Resolve1ParseLevel ,
  ID_Translate ,
};

BEGIN_EVENT_TABLE(wxTextInputDlg, wxDialog)
  EVT_BUTTON( ID_Translate , wxTextInputDlg::OnTranslateButton)
  EVT_BUTTON( ID_LoadDictionary , wxTextInputDlg::OnLoadDictionaryButton)
  EVT_BUTTON( ID_ShowTokenIndex2GrammarPart, wxTextInputDlg::
    OnShowTokenIndex2GrammarPartButton)
  EVT_BUTTON( ID_ReInitGrammarRules , wxTextInputDlg::OnReInitGrammarRulesButton)
  EVT_BUTTON( ID_ResolveSuperclass,
    wxTextInputDlg::OnResolveSuperclassGrammarParts )
  EVT_BUTTON( ID_Resolve1ParseLevel , wxTextInputDlg::OnResolve1ParseLevelButton)
  EVT_CLOSE( wxTextInputDlg::OnClose)
END_EVENT_TABLE()

wxTextInputDlg::wxTextInputDlg( wxWindow* parent, wxWindowID id,
  const wxString& title, const wxPoint& pos, const wxSize& size, long style )
: wxDialog( parent, id, title, pos, size, style )
 , mp_wxhtmlwindow( NULL)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

//  if( ! mp_wxhtmlwindow )
//  {
//    wxFrame * p_frame = new wxFrame(NULL,wxID_ANY,wxT("dd")) ;
//    p_frame->Show(true) ;
////    mp_wxwebkitctrl = new wxWebKitCtrl( p_frame , wxID_ANY ,
////      _T("http://www.wxwidgets.org"), wxDefaultPosition, wxSize(200, 200));
//    mp_wxhtmlwindow = new wxHtmlWindow( //p_frame
//      ) ;
//    mp_wxhtmlwindow->SetRelatedFrame( p_frame
//      //this
//      , wxT("HtmlHelp : %s") ) ;
//    wxString wxstrHTML = "<html><body>\n"
//                 "<h1>Error</h1>\n"
//             "Some error occurred :-H)\n" ;
//    wxstrHTML += "<form action=\"select.htm\">\n"
//        "<p>\n"
//          "<select name=\"top5\" size=\"3\">\n"
//            "<option>Heino</option>\n"
//            "<option>Michael Jackson</option>\n"
//            "<option>Tom Waits</option>\n"
//            "<option>Nina Hagen</option>\n"
//            "<option>Marianne Rosenberg</option>\n"
//          "</select>\n"
//        "</p>\n"
//      "</form>\n"
//      "</body></hmtl>" ;
//    mp_wxhtmlwindow->SetPage(wxstrHTML);
//    mp_wxhtmlwindow->LoadPage(
////    mp_wxhtmlwindow->LoadFile(
//      wxT("http://de.selfhtml.org/html/formulare/anzeige/select_multiple.htm")
//      ) ;
//    mp_wxhtmlwindow->SetRelatedStatusBar(0);
//    mp_wxhtmlwindow->Show(true) ;
//  }

  wxButton * p_wxbutton ;

  mp_wxsplitterwindow = new wxSplitterWindow( this, wxID_ANY ) ;
//  mp_wxpanelTop = new wxPanel( mp_wxsplitterwindow ) ;

  mp_wxparsetreepanel = new //wxPanel(
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
    , mp_wxparsetreepanel
    ) ;
//  mp_wxsplitterwindow->SplitHorizontally(
//    //m_panelSplitterTop
//    mp_textctrlGermanText
//    , mp_wxparsetreepanel
//    ) ;

  p_wxbutton = new wxButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_Translate
    , wxT("t")
    ) ;
  p_wxbutton->SetToolTip( wxT("translate")) ;
  p_boxsizerButtons->Add(
    p_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
  p_wxbutton = new wxButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_Resolve1ParseLevel
    , wxT("1 L")
    ) ;
  p_wxbutton->SetToolTip( wxT("resolve 1 parse level")) ;
  p_boxsizerButtons->Add(
    p_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
  p_wxbutton = new wxButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_ResolveSuperclass
    , wxT("res SC")
    ) ;
  p_wxbutton->SetToolTip( wxT("resolve superclasses")) ;
  p_boxsizerButtons->Add(
    p_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
  p_wxbutton = new wxButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_ReInitGrammarRules
    , wxT("re G")
    ) ;
  p_wxbutton->SetToolTip( wxT("re-init grammar")) ;
  p_boxsizerButtons->Add(
    p_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
  p_wxbutton = new wxButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_ShowTokenIndex2GrammarPart
    , wxT("i2g")
    ) ;
  p_wxbutton->SetToolTip( wxT("show token index 2 grammar part map content")) ;
  p_boxsizerButtons->Add(
    p_wxbutton
    , 0 //strech factor. 0=do not stretch
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
    , wxT("dict")
    ) ;
  p_wxbutton->SetToolTip(wxT("re-load dictionary...")) ;
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

void wxTextInputDlg::GetEntireInputText(std::string & r_stdstrInputText)
{
//  std::string stdstrInputText ;
  wxString wxstringWholeInputText ;
  wxTextPos wxtextposNumberOfCharacters  ;
  wxtextposNumberOfCharacters = mp_textctrlEnglishText->GetLastPosition() ;
  wxstringWholeInputText = mp_textctrlEnglishText->GetRange(0 ,
    wxtextposNumberOfCharacters) ;
  r_stdstrInputText = std::string( wxstringWholeInputText.c_str() ) ;
//  return stdstrInputText ;
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


void wxTextInputDlg::OnResolveSuperclassGrammarParts(wxCommandEvent & wxcmd )
{
  m_parsebyrise.ReplaceGrammarPartIDsBySuperordinate() ;
  OnShowTokenIndex2GrammarPartButton(wxcmd) ;
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
}

void wxTextInputDlg::OnResolve1ParseLevelButton( wxCommandEvent & wxcmd )
{
  std::string stdstrWholeInputText ;
  GetEntireInputText(stdstrWholeInputText) ;
  if( m_parsebyrise.m_wParseLevel ==
    //fully resolved or not begun yet.
    0 )
  {
    m_parsebyrise.ClearParseTree() ;
    m_parsebyrise.CreateInitialGrammarParts ( stdstrWholeInputText ) ;
  }
  else
  {
    m_parsebyrise.ResolveGrammarRules() ;
  }
  OnShowTokenIndex2GrammarPartButton(wxcmd) ;
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
}

void wxTextInputDlg::OnShowTokenIndex2GrammarPartButton( wxCommandEvent & wxcmd )
{
  std::string stdstr ;
  std::multimap<DWORD, GrammarPart *>::iterator
    iter_mm_rightmostidx2p_grammarptLeftGrammarPart =
    m_parsebyrise.m_stdmultimap_dwRightmostIndex2p_grammarpart.begin() ;

  for( ; iter_mm_rightmostidx2p_grammarptLeftGrammarPart !=
    m_parsebyrise.m_stdmultimap_dwRightmostIndex2p_grammarpart.end() ;
    ++ iter_mm_rightmostidx2p_grammarptLeftGrammarPart
    )
  {
    GrammarPart & r_gp = * iter_mm_rightmostidx2p_grammarptLeftGrammarPart->
        second ;
    std::stringstream strstream ;
    strstream << m_parsebyrise.GetGrammarPartName( r_gp.m_wGrammarPartID )
      << " " << & r_gp
      << " " << r_gp.m_dwLeftmostIndex
      << " " << r_gp.m_dwRightmostIndex
      ;
    if( r_gp.mp_grammarpartLeftChild )
        strstream << " left child:" << r_gp.mp_grammarpartLeftChild ;
    if( r_gp.mp_grammarpartRightChild )
        strstream << " right child:" << r_gp.mp_grammarpartRightChild ;
    stdstr += strstream.str() + "\n" ;
//    m_parsebyrise.m_stdmultimap_dwLeftmostIndex2p_grammarpartSuperordinate
  }
  mp_textctrlGermanText->SetValue( stdstr ) ;
}

void wxTextInputDlg::OnTranslateButton( wxCommandEvent & wxcmd )
{
  std::string stdstrWholeInputText ;
  GetEntireInputText(stdstrWholeInputText) ;

  m_parsebyrise.ClearParseTree() ;
  m_parsebyrise.CreateInitialGrammarParts ( stdstrWholeInputText ) ;
  DEBUG_COUT("before resolving GrammarRulesForAllParseLevels \n")
  m_parsebyrise.ResolveGrammarRulesForAllParseLevels() ;

  TranslateParseByRiseTree translateParseByRiseTree( m_parsebyrise ) ;
  DEBUG_COUT("before translation\n")
//  std::string stdstrWholeTransl ;
  std::vector<std::string> stdvec_stdstrWholeTransl ;
  std::vector<std::vector<TranslationAndGrammarPart> >
    stdvec_stdvecTranslationAndGrammarPart ;
  std::vector<std::vector<TranslationAndConsecutiveID> >
    stdvec_stdvecTranslationAndConsecutiveID ;
  translateParseByRiseTree.Translate( m_parsebyrise,
    stdvec_stdstrWholeTransl ,
    stdvec_stdvecTranslationAndGrammarPart
//    stdvec_stdvecTranslationAndConsecutiveID
    ) ;

  std::string stdstrAllPoss ;
  for( std::vector<std::string>::const_iterator c_iter_stdvec_stdstr =
      stdvec_stdstrWholeTransl.begin() ;
      c_iter_stdvec_stdstr != stdvec_stdstrWholeTransl.end()
      ; ++ c_iter_stdvec_stdstr
      )
  {
    stdstrAllPoss += *c_iter_stdvec_stdstr + "\n" ;
  }
  for( std::vector< std::vector<TranslationAndGrammarPart
//      TranslationAndConsecutiveID
      > >::const_iterator
      c_iter_stdvec_stdvec =
      stdvec_stdvecTranslationAndGrammarPart.begin() ;
//      stdvec_stdvecTranslationAndConsecutiveID.begin() ;
      c_iter_stdvec_stdvec != stdvec_stdvecTranslationAndGrammarPart.end() ;
//          stdvec_stdvecTranslationAndConsecutiveID.end() ;
      ++ c_iter_stdvec_stdvec
      )
  {
    std::stringstream stdstrstream ;
    //translation token for the same poss.
    for( std::vector<TranslationAndGrammarPart
        //TranslationAndConsecutiveID
        >::const_iterator
        c_iter_stdvec =
            c_iter_stdvec_stdvec->begin() ;
        c_iter_stdvec != c_iter_stdvec_stdvec->end() ;
        ++ c_iter_stdvec
        )
    {
      stdstrstream << c_iter_stdvec->m_stdstrTranslation ;
      if( c_iter_stdvec->mp_grammarpart )
      {
        stdstrstream << " ID: "
            << c_iter_stdvec->mp_grammarpart->m_wConsecutiveID
          << " addr. with ID:"
          << std::ios::hex << c_iter_stdvec->mp_grammarpart << " " ;
      }
//          << c_iter_stdvec->m_wConsecutiveID  ;
    }
    stdstrAllPoss += stdstrstream.str() + "\n";
  }
  mp_textctrlGermanText->SetValue(stdstrAllPoss ) ;
  wxHTMLfileOutput wxhtml_file_output(
//    stdvec_stdvecTranslationAndConsecutiveID
    stdvec_stdvecTranslationAndGrammarPart
    ) ;
  wxhtml_file_output.writeFile( //stdvec_stdstrWholeTransl ,
//    stdvec_stdvecTranslationAndConsecutiveID ,
    stdvec_stdvecTranslationAndGrammarPart ,
    wxT("trans.html") ) ;

//  mp_textctrlGermanText->SetValue( stdstrWholeTransl ) ;
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
  DEBUG_COUT("end of OnTranslateButton\n")
  //You can also trigger this call by calling Refresh()/Update()
  //m_panel1->Refresh() ;
}
