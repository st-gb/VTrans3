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
//#include <wx/event.h> //for EVT_CLOSE
//#include <wx/html/htmlwin.h>
#include <wx/arrstr.h> //wxArrayString
#include <wx/bmpbuttn.h> //class wxBitmapButton
#include <wx/sizer.h> //class wxBoxSizer
#include <wx/splitter.h> //class wxSplitterWindow
#include <wx/textctrl.h> //class wxTextCtrl
#include <wx/timer.h> //for EVT_TIMER (,...?)

//#define COMPILE_WITH_TEXT_TO_SPEECH
#ifdef COMPILE_WITH_TEXT_TO_SPEECH
  #include <ax/ax_speech.h>
#endif

#include "wxTextInputDlg.hpp"
#include "wxTextControlDialog.hpp" //class wxTextControlDialog
#include "ShowTranslationRulesDialog.hpp" //class ShowTranslationRulesDialog
#include <IO/dictionary/VTransDictFormatReader.hpp> //class OneLinePerWordPair

////see http://stackoverflow.com/questions/59670/how-to-get-rid-of-deprecated-conversion-from-string-constant-to-char-warning
////to ignore "warning: deprecated conversion from string constant to 'char*'"
//#pragma GCC diagnostic ignored "-Wno-write-strings"
//DISable warning, from
//http://stackoverflow.com/questions/59670/how-to-get-rid-of-deprecated-conversion-from-string-constant-to-char-warning
// : "I believe passing -Wno-write-strings to gcc will suppress this warning."
#pragma GCC diagnostic ignored "-Wwrite-strings"

//For array "add_grammar_rules_xpm" .
#include <bitmaps/add_grammar_rules24x24.xpm>
//For array "add_vocable_attribute_definitions_xpm" .
#include <bitmaps/add_vocable_attribute_definitions24x24.xpm>
//For array "remove_vocable_attribute_definitions_xpm" .
#include <bitmaps/remove_vocable_attribute_definitions24x24.xpm>
//For array "add_translation_rules_xpm" .
#include <bitmaps/add_translation_rules24x24.xpm>
//For array "add_transformation_rules_xpm" .
#include <bitmaps/add_transformation_rules24x24.xpm>

//static char * grammar_part_colours_xpm
#include <bitmaps/grammar_part_colours.xpm>
#include <bitmaps/info.xpm>

//For array "re_initialize_grammar_xpm" .
#include <bitmaps/re-initialize_grammar.xpm>
//For array "re_load_dictionary_xpm" .
#include <bitmaps/re-load_dictionary.xpm>

//For array "remove_grammar_rules_xpm" .
#include <bitmaps/remove_grammar_rules24x24.xpm>
//For array "remove_translation_rules_xpm" .
#include <bitmaps/remove_translation_rules24x24.xpm>
//For array "remove_transformation_rules_xpm" .
#include <bitmaps/remove_transformation_rules24x24.xpm>
#include <bitmaps/resolve_superclasses24x24.xpm> //for array resolve_superclasses_xpm
#include <bitmaps/resolve_1parse_level24x24.xpm> //for array resolve_1parse_level_xpm
#include <bitmaps/show_translation_rules24x24.xpm>
#include <bitmaps/translate_bitmap.xpm> //for array translate_bitmap_xpm
#include <bitmaps/truncate_log_file.xpm> //for array truncate_log_file_xpm
#include <bitmaps/VT_icon.xpm> // array "VT_icon_xpm"
////see //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//#pragma GCC diagnostic pop
//ENable warning
#pragma GCC diagnostic warning "-Wwrite-strings"

#include <Controller/character_string/convertFromAndToStdString.hpp>
#include <Translate/TranslateTreeTraverser.hpp> //TranslationAndGrammarPart
//class TranslateParseByRiseTree
#include <Translate/TranslateParseByRiseTree.hpp>
#include <Translate/TranslationRule.hpp> //class TranslationRule

//GetStdString(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <wxWidgets/VTransApp.hpp> //::wxGetApp()
//UnLoadAndLoadDictionary(wxWindow *);
#include <wxWidgets/UserInterface/UserInterface.hpp>
#include <wxWidgets/UserInterface/wxParseTreePanel.hpp>//class wxParseTreePanel
#include <wxWidgets/wxHTMLfileOutput.hpp> //class wxHTMLfileOutput
//class SAX2GrammarRuleHandler
#include <Xerces/SAX2GrammarRuleHandler.hpp>
//class SAX2TransformationRuleHandler
#include <Xerces/SAX2TransformationRuleHandler.hpp>
//class SAX2TranslationRuleHandler
#include <Xerces/SAX2TranslationRuleHandler.hpp>
//class SAX2VocAttributeDefintionHandler
#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>

#include <sstream>//class std::stringstream

using namespace wxWidgets ;

///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxTextInputDlg, wxDialog)
  EVT_TIMER( Timer, wxTextInputDlg::OnTimerEvent)
  EVT_BUTTON( ID_AddGrammarRules , wxTextInputDlg::OnAddGrammarRules )
  EVT_BUTTON( ID_AddTransformationRules ,
    wxTextInputDlg::OnAddTransformationRules )
  EVT_BUTTON( ID_AddTranslationRules , wxTextInputDlg::OnAddTranslationRules )
  EVT_BUTTON( ID_AddVocAttrDefs, wxTextInputDlg::OnAddVocAttrDefs )
  EVT_BUTTON( ID_Translate , wxTextInputDlg::OnTranslateButton)
  EVT_BUTTON( ID_TruncateLogFile, wxTextInputDlg::OnTruncateLogFileButton)
  EVT_BUTTON( ID_LoadDictionary , wxTextInputDlg::OnLoadDictionaryButton)
  EVT_BUTTON( ID_ShowTokenIndex2GrammarPart, wxTextInputDlg::
    OnShowTokenIndex2GrammarPartButton)
  EVT_BUTTON( ID_ShowTranslationRules,
    wxTextInputDlg::OnShowTranslationRulesButton )
  EVT_BUTTON( ID_ReInitGrammarRules ,
    wxTextInputDlg::OnReInitGrammarRulesButton)
  EVT_BUTTON( ID_RemoveGrammarRules ,
    wxTextInputDlg::OnRemoveGrammarRules )
  EVT_BUTTON( ID_RemoveTransformationRules ,
    wxTextInputDlg::OnRemoveTransformationRules )
  EVT_BUTTON( ID_RemoveTranslationRules ,
    wxTextInputDlg::OnRemoveTranslationRules )
  EVT_BUTTON( ID_RemoveVocAttrDefs ,
    wxTextInputDlg::OnRemoveVocAttrDefs )
  EVT_BUTTON( ID_ResolveSuperclass,
    wxTextInputDlg::OnResolveSuperclassGrammarParts )
  EVT_BUTTON( ID_Resolve1ParseLevel ,
    wxTextInputDlg::OnResolve1ParseLevelButton)
  EVT_BUTTON( ID_DrawParseTree ,
    wxTextInputDlg::OnDrawParseTreeButton)
  EVT_BUTTON( ID_Info, wxTextInputDlg::OnInfoButton)
  EVT_CLOSE( wxTextInputDlg::OnClose)
END_EVENT_TABLE()

void wxTextInputDlg::AddReInitializeGrammarButton(wxSizer * p_wxsizer)
{
  mp_wxbutton = new wxBitmapButton(
    this,
    ID_ReInitGrammarRules,
    wxBitmap(re_initialize_grammar_xpm)
    );
  mp_wxbutton->SetToolTip( wxT("re-init grammar") ) ;
  p_wxsizer->Add(mp_wxbutton, 0, wxBOTTOM, 2);
}

void wxTextInputDlg::AddButtons()
{
  wxButton * p_wxbutton ;
  wxBoxSizer * p_boxsizerButtons = new wxBoxSizer( wxHORIZONTAL ) ;
  AddTranslateButton( p_boxsizerButtons ) ;

  AddAddTranslationRulesButton( p_boxsizerButtons ) ;
  AddRemoveTranslationRulesButton( p_boxsizerButtons ) ;
  AddShowTranslationRulesButton( p_boxsizerButtons ) ;
  AddAddTransformationRulesButton( p_boxsizerButtons ) ;
  AddRemoveTransformationRulesButton( p_boxsizerButtons ) ;
  AddAddGrammarRulesButton( p_boxsizerButtons ) ;
  AddRemoveGrammarRulesButton( p_boxsizerButtons ) ;
  AddAddVocAttrDefsButton( p_boxsizerButtons ) ;
  AddRemoveVocAttrDefsButton( p_boxsizerButtons ) ;

  AddResolve1ParseLevelButton( p_boxsizerButtons ) ;
  AddResolveSuperClassesButton( p_boxsizerButtons ) ;

  AddReInitializeGrammarButton(p_boxsizerButtons);

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
  p_boxsizerButtons->Add(p_wxbutton, 0,
    wxBOTTOM
  , 2 );

  AddLoadDictionaryButton( p_boxsizerButtons);
  AddShowInformationButton( p_boxsizerButtons ) ;
  AddDrawParseTreeButton( p_boxsizerButtons);
  AddTruncateLogFileButton( p_boxsizerButtons);
  p_boxsizerOuter->Add( p_boxsizerButtons ) ;
}

void wxTextInputDlg::AddAddTranslationRulesButton( wxSizer * p_sizer )
{
  mp_wxbutton = new wxBitmapButton(
    this
    , ID_AddTranslationRules
    , wxBitmap( add_translation_rules24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("add translation rules")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddShowTranslationRulesButton( wxSizer * p_sizer )
{
  mp_wxbutton = new wxBitmapButton(
    this
    , ID_ShowTranslationRules
    , wxBitmap( show_translation_rules24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("show translation rules")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddAddTransformationRulesButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_AddTransformationRules
  //    , wxT("1 L")
    , wxBitmap( add_transformation_rules24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("add transformation rules")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddAddVocAttrDefsButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_AddVocAttrDefs
  //    , wxT("1 L")
    , wxBitmap( add_vocable_attribute_definitions24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("add vocable attribute definitions")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddDrawParseTreeButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_DrawParseTree
  //    , wxT("t")
    , wxBitmap( info_xpm)
    ) ;
  mp_wxbutton->SetToolTip( wxT("(re-)draw parse tree")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddRemoveTranslationRulesButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_RemoveTranslationRules
  //    , wxT("1 L")
    , wxBitmap( remove_translation_rules24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("remove translations rules")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddRemoveTransformationRulesButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_RemoveTransformationRules
  //    , wxT("1 L")
    , wxBitmap( remove_transformation_rules24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("remove transformation rules")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddAddGrammarRulesButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_AddGrammarRules
  //    , wxT("1 L")
    , wxBitmap( add_grammar_rules24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("add grammar rules")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddLoadDictionaryButton( wxSizer * p_sizer)
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_LoadDictionary
    //, wxT("dict")
    , wxBitmap( re_load_dictionary_xpm)
    ) ;
  mp_wxbutton->SetToolTip( wxT("re-load dictionary...") );
  p_sizer->Add(
    mp_wxbutton
    //stretch factor. 0=do not stretch
    , 0
      //0
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddRemoveGrammarRulesButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_RemoveGrammarRules
  //    , wxT("1 L")
    , wxBitmap( remove_grammar_rules24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("remove grammar rules")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddRemoveVocAttrDefsButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_RemoveVocAttrDefs
  //    , wxT("1 L")
    , wxBitmap( remove_vocable_attribute_definitions24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("remove vocable attribute definitions")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddResolve1ParseLevelButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_Resolve1ParseLevel
  //    , wxT("1 L")
    , wxBitmap( resolve_1parse_level24x24_xpm )
    ) ;
  mp_wxbutton->SetToolTip( wxT("resolve 1 parse level")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddResolveSuperClassesButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_ResolveSuperclass
  //    , wxT("res SC")
    , wxBitmap( resolve_superclasses24x24_xpm)
    ) ;
  mp_wxbutton->SetToolTip( wxT("resolve superclasses")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddShowInformationButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_Info
//    , wxT("info")
    , wxBitmap( info_xpm)
    ) ;
  mp_wxbutton->SetToolTip( wxT("show information")) ;
  p_sizer->Add(
    mp_wxbutton
    //stretch factor. 0=do not stretch
    , 0
      //0
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddTranslateButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_Translate
  //    , wxT("t")
    , wxBitmap( translate_bitmap_xpm)
    ) ;
  mp_wxbutton->SetToolTip( wxT("translate")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

void wxTextInputDlg::AddTruncateLogFileButton( wxSizer * p_sizer )
{
  mp_wxbutton = new //wxButton(
    wxBitmapButton(
    //mp_wxsplitterwindow
    //m_panelSplitterTop
    //p_boxsizerOuter
    this
    , //wxID_ANY
    ID_TruncateLogFile
  //    , wxT("t")
    , wxBitmap( truncate_log_file_xpm)
    ) ;
  mp_wxbutton->SetToolTip( wxT("translate")) ;
  p_sizer->Add(
    mp_wxbutton
    , 0 //strech factor. 0=do not stretch
    , //wxEXPAND |
      wxBOTTOM
    , 2 );
}

wxTextInputDlg::wxTextInputDlg(
  wxWindow * p_wxwindowParent,
  TranslationControllerBase & r_translationcontrollerbase,
  wxWindowID wxwindow_id,
  const wxString & cr_wxstrTitle,
  const wxPoint & cr_wxpointWindowPosition,
  const wxSize & cr_wxsizeWindow,
  long style
  )
  : wxDialog( p_wxwindowParent, wxwindow_id, cr_wxstrTitle,
      cr_wxpointWindowPosition, cr_wxsizeWindow, style )
//  , mp_wxhtmlwindow( NULL)
//  , m_parsebyrise( ::wxGetApp() )
  , m_translationcontrollerbase(r_translationcontrollerbase)
  , m_parsebyrise ( ::wxGetApp().m_parsebyrise )
{
	SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxIcon wxiconThisDialog( VT_icon_xpm ) ;
	SetIcon( wxiconThisDialog ) ;

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
//            "<option>Tom Waits</option>\n"
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
//  mp_textctrlGermanText = new wxTextCtrl(
//    //this
////    mp_wxsplitterwindow
//    m_panelSplitterTop
//    , wxID_ANY,
//    wxEmptyString,
//    wxDefaultPosition,
//    wxDefaultSize,
//    wxHSCROLL | wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB
//    );
  m_p_wxgermantranslationpanel = new wxGermanTranslationPanel(
      m_panelSplitterTop);

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
//    mp_textctrlGermanText,
    m_p_wxgermantranslationpanel,
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
  p_boxsizerOuter = new wxBoxSizer( wxVERTICAL ) ;
  AddButtons() ;
	
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
	SetSizer( //p_gridsizerOuter
    p_boxsizerOuter
    );
	SetAutoLayout(true);
	Layout();
}

wxTextInputDlg::~wxTextInputDlg()
{
  LOGN("wxTextInputDlg::~wxTextInputDlg")
}

std::string wxTextInputDlg::GetAllTranslationPossibilites(
  const std::vector<std::string> & stdvec_stdstrWholeTransl ,
  const std::vector<std::vector<TranslationAndGrammarPart> > &
    stdvec_stdvecTranslationAndGrammarPart
  )
{
  std::string stdstrAllPossibilities ;
  for( std::vector<std::string>::const_iterator c_iter_stdvec_stdstr =
      stdvec_stdstrWholeTransl.begin() ;
      c_iter_stdvec_stdstr != stdvec_stdstrWholeTransl.end()
      ; ++ c_iter_stdvec_stdstr
      )
  {
    stdstrAllPossibilities += *c_iter_stdvec_stdstr + "\n" ;
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
    stdstrAllPossibilities += stdstrstream.str() + "\n";
  }
  return stdstrAllPossibilities ;
}

void wxTextInputDlg::GetEntireInputText(std::string & r_stdstrInputText)
{
//  std::string stdstrInputText ;
  wxString wxstringWholeInputText ;
//  wxTextPos wxtextposNumberOfCharacters  ;
//  wxtextposNumberOfCharacters = mp_textctrlEnglishText->GetLastPosition() ;
//  wxstringWholeInputText = mp_textctrlEnglishText->GetRange(0 ,
//    wxtextposNumberOfCharacters) ;
  wxstringWholeInputText = mp_textctrlEnglishText->GetValue() ;
  r_stdstrInputText = std::string( wxstringWholeInputText.c_str() ) ;
//  return stdstrInputText ;
}

void wxTextInputDlg::OnAddGrammarRules( wxCommandEvent & wxcmd )
{
//  AddTranslationRules() ;
  SAX2GrammarRuleHandler sax2grammarrulehandler(
    wxGetApp().m_parsebyrise
    ) ;
  wxString wxstrTitle("Choose at least 1 grammar rule file") ;
  wxGetApp().ProcessSelectedXMLfiles(
    sax2grammarrulehandler ,
    wxstrTitle,
    ::wxGetCwd()
    );
}

void wxTextInputDlg::OnAddTransformationRules( wxCommandEvent & wxcmd )
{
//  AddTranslationRules() ;
  Xerces::SAX2TransformationRuleHandler sax2transformationrulehandler(
    wxGetApp()
    ) ;
  wxGetApp().ProcessSelectedXMLfiles(
    sax2transformationrulehandler,
    wxString( wxT("Choose at least 1 transFORMation rule file") ),
    ::wxGetCwd()
    ) ;
}

void wxTextInputDlg::OnAddTranslationRules( wxCommandEvent & wxcmd )
{
//  AddTranslationRules() ;
  SAX2TranslationRuleHandler sax2translationrulehandler(
    wxGetApp().m_translateparsebyrisetree ,
    wxGetApp().m_parsebyrise ,
    wxGetApp()
    ) ;
  wxGetApp().ProcessSelectedXMLfiles(
    sax2translationrulehandler ,
    wxT("Choose at least 1 translation rule file"),
    ::wxGetCwd()
    ) ;
}

void wxTextInputDlg::OnAddVocAttrDefs( wxCommandEvent & wxcmd )
{
//  AddTranslationRules() ;
  Xerces::SAX2VocAttributeDefinitionHandler sax2vocattributedefintionhandler(
    wxGetApp().m_translateparsebyrisetree ,
    wxGetApp()
    ) ;
  wxGetApp().ProcessSelectedXMLfiles(
    sax2vocattributedefintionhandler ,
    wxT("Choose at least 1 vocabulary attribute definitions file"),
    ::wxGetCwd()
    );
}

void wxTextInputDlg::OnClose( wxCloseEvent & wxcmd )
{
  LOGN("wxTextInputDlg::OnClose begin")
//  //Call delete to exit this application. (without it is not finished)
//  delete(this) ;
//  this->Destroy() ;
  Destroy() ;
//  Close() ;
  //::wxGetApp().ExitMainLoop() ;
  LOGN("wxTextInputDlg::OnClose end")
}

void wxTextInputDlg::OnDrawParseTreeButton(wxCommandEvent & wxcmd )
{
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
}

void wxTextInputDlg::OnInfoButton( wxCommandEvent & wxcmd )
{
  wxString wxstr = wxString::Format(
    wxT("# voc pairs:%lu\n"
        "# grammar rules:%u\n"
        "# translation rules:%u\n"
        "# transFORMation rules:%u\n"
        "# vocabulary attribute definitions:%u"
      ) ,
//    OneLinePerWordPair::s_dwNumberOfVocabularyPairs
    ::wxGetApp().s_numberOfVocabularyPairs
    , wxGetApp().m_parsebyrise.m_stdmap_RuleName2RuleID.size()
    , wxGetApp().m_translateparsebyrisetree.
      m_stdmap_p_translationrule2ConditionsAndTranslation.size()
    , wxGetApp().m_stdmap_syntaxtreepath2transformationrule.size()
    , wxGetApp().m_translateparsebyrisetree.
      m_stdmap_AttrName2VocAndTranslAttrDef.size()
    );
//  ::wxMessageBox(
//    wxstr
//    , wxT("info") ) ;

  wxTextControlDialog wxd(wxstr);
  wxd.ShowModal();
}

void wxTextInputDlg::OnLoadDictionaryButton( wxCommandEvent & wxcmd )
{
  UnLoadAndLoadDictionary( this ) ;
}

void wxTextInputDlg::OnReInitGrammarRulesButton( wxCommandEvent & wxcmd )
{

  m_parsebyrise.ClearAllGrammarStuff() ;
  wxGetApp().m_translateparsebyrisetree.FreeMemoryForTranslationRules();
  wxGetApp().m_stdmap_syntaxtreepath2transformationrule.clear();
  wxGetApp().m_translateparsebyrisetree.FreeMemoryForVocAttrDefs() ;

  m_parsebyrise.InitGrammarRules() ;
  m_translationcontrollerbase.ReadMainConfigFile(
    m_translationcontrollerbase.m_std_strMainConfigFilePath);
}

void wxTextInputDlg::OnRemoveGrammarRules(wxCommandEvent & wxcmd )
{
  LOGN("wxTextInputDlg::OnRemoveGrammarRules begin")
//  wxGetApp().m_parsebyrise.m_stdmap_RuleName2RuleID.clear() ;
//  wxGetApp().m_parsebyrise.m_stdmap_wRuleID2RuleName.clear() ;
//  wxGetApp().m_parsebyrise.m_stdmap_wGrammarPartID2SuperordinateID.clear() ;
//  //Absolutely necessary?!
//  wxGetApp().m_parsebyrise.InsertFundamentalRuleIDs() ;

  m_parsebyrise.ClearAllGrammarStuff() ;
  m_parsebyrise.InitGrammarRules() ;
  LOGN("wxTextInputDlg::OnRemoveGrammarRules end")
}

void wxTextInputDlg::OnRemoveTransformationRules(wxCommandEvent & wxcmd )
{
  LOGN("wxTextInputDlg::OnRemoveTransformationRules begin")
  wxGetApp().m_stdmap_syntaxtreepath2transformationrule.clear() ;
  LOGN("wxTextInputDlg::OnRemoveTransformationRules end")
}

void wxTextInputDlg::OnRemoveTranslationRules(wxCommandEvent & wxcmd )
{
  LOGN("wxTextInputDlg::OnRemoveTranslationRules begin")
  wxGetApp().m_translateparsebyrisetree.FreeMemoryForTranslationRules() ;
//  wxGetApp().m_translateparsebyrisetree.
//    m_stdmap_translationrule2ConditionsAndTranslation.clear() ;
//  wxGetApp().m_translateparsebyrisetree.
//    m_stdmap_p_translationrule2ConditionsAndTranslation.clear() ;
  LOGN("wxTextInputDlg::OnRemoveTranslationRules end")
}

void wxTextInputDlg::OnRemoveVocAttrDefs(wxCommandEvent & wxcmd )
{
  LOGN("wxTextInputDlg::OnRemoveTranslationRules begin")
  wxGetApp().m_translateparsebyrisetree.FreeMemoryForVocAttrDefs() ;
//  wxGetApp().m_translateparsebyrisetree.
//    m_stdmap_translationrule2ConditionsAndTranslation.clear() ;
//  wxGetApp().m_translateparsebyrisetree.
//    m_stdmap_p_translationrule2ConditionsAndTranslation.clear() ;
  LOGN("wxTextInputDlg::OnRemoveTranslationRules end")
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
//  mp_textctrlGermanText->SetValue( stdstr ) ;
}

void wxTextInputDlg::OnShowTranslationRulesButton( wxCommandEvent & wxcmd )
{
  wxString wxstr, wxstrSyntaxTreePath;
  std::map<TranslationRule *,ConditionsAndTranslation> &
    r_stdmap_p_translationrule2ConditionsAndTranslation = wxGetApp().
    m_translateparsebyrisetree.
    m_stdmap_p_translationrule2ConditionsAndTranslation;
  int arraySize = r_stdmap_p_translationrule2ConditionsAndTranslation.size();
  wxString choices[arraySize];
  std::map<TranslationRule *,ConditionsAndTranslation>::const_iterator c_iter
    = r_stdmap_p_translationrule2ConditionsAndTranslation.begin();
  unsigned arrayIndex = 0;
  while( c_iter != r_stdmap_p_translationrule2ConditionsAndTranslation.end() )
  {
    const TranslationRule * c_p_translationrule =
      c_iter->first;
//      r_translationrule.mp_parsebyrise->Get
    wxstrSyntaxTreePath = getwxString( c_p_translationrule->
      m_syntaxtreepathCompareWithCurrentPath.GetAs_std_string() );
    wxstr += wxT("\n") + wxstrSyntaxTreePath;
    choices[ arrayIndex] = wxstrSyntaxTreePath;
    const ConditionsAndTranslation conditionsandtranslation = c_iter->second;
    std::vector<Condition>::const_iterator conditions_iter =
      conditionsandtranslation.m_conditions.begin();
    while( conditions_iter != conditionsandtranslation.m_conditions.end() )
    {
      wxstr += conditions_iter->m_syntaxtreepath.GetAs_std_string() + " WHERE "
        + conditions_iter->m_stdstrAttributeName + "=" +
        convertToStdString<WORD>(conditions_iter->m_byAttributeValue);
      ++ conditions_iter;
    }
    ++ c_iter;
    ++ arrayIndex;
  }
//  wxTextControlDialog wxd(wxstr);
//  wxd.ShowModal();
  VTrans::ShowTranslationRulesDialog showtranslationrulesdialog(this, choices,
    arraySize);
  showtranslationrulesdialog.ShowModal();
}

void wxTextInputDlg::OnTimerEvent(wxTimerEvent &event)
{
  SetTitle( wxString::Format(wxT("%u"), wxGetApp().m_dictionaryFileLineNumber) );
}

void wxTextInputDlg::OnTruncateLogFileButton( wxCommandEvent & wxcmd )
{
  g_logger.TruncateFileSizeToZero();
}

void wxTextInputDlg::OnTranslateButton( wxCommandEvent & wxcmd )
{
  std::string stdstrWholeInputText ;
  GetEntireInputText(stdstrWholeInputText) ;
//  AxSpeech axspeech ;
//  axspeech.Say( stdstrWholeInputText ) ;

//  std::string stdstrWholeTransl ;
  std::vector<std::string> stdvec_stdstrWholeTransl ;
//  std::vector<std::vector<TranslationAndGrammarPart> >
//    stdvec_stdvecTranslationAndGrammarPart ;
  //A vector of sentences that begin at the same token index
  // (sentences that begin at the same token index:
  // vector of sentences that each contains a vector of words).
  std::vector <std::vector <std::vector <TranslationAndGrammarPart> > >
    stdvec_stdvec_stdvecTranslationAndGrammarPart;

  std::vector<std::vector<TranslationAndConsecutiveID> >
    stdvec_stdvecTranslationAndConsecutiveID ;

  ::wxGetApp().Translate(
    stdstrWholeInputText ,
    stdvec_stdstrWholeTransl ,
//    stdvec_stdvecTranslationAndGrammarPart
    stdvec_stdvec_stdvecTranslationAndGrammarPart
    ) ;

//  std::string stdstrAllPossibilities = GetAllTranslationPossibilites(
//    stdvec_stdstrWholeTransl,
//    stdvec_stdvecTranslationAndGrammarPart ) ;

//  mp_textctrlGermanText->SetValue(stdstrAllPossibilities ) ;
  m_p_wxgermantranslationpanel->Set(//stdvec_stdvecTranslationAndGrammarPart
      stdvec_stdvec_stdvecTranslationAndGrammarPart);
  m_p_wxgermantranslationpanel->Create();
  //force redraw
  m_p_wxgermantranslationpanel->Refresh();

//  wxHTMLfileOutput wxhtml_file_output(
////    stdvec_stdvecTranslationAndConsecutiveID
//    stdvec_stdvecTranslationAndGrammarPart
//    ) ;
//  wxhtml_file_output.writeFile( //stdvec_stdstrWholeTransl ,
////    stdvec_stdvecTranslationAndConsecutiveID ,
//    stdvec_stdvecTranslationAndGrammarPart ,
//    wxT("trans.html") ) ;

//  mp_textctrlGermanText->SetValue( stdstrWholeTransl ) ;
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
  DEBUG_COUT("end of OnTranslateButton\n")
  //You can also trigger this call by calling Refresh()/Update()
  //m_panel1->Refresh() ;
}
