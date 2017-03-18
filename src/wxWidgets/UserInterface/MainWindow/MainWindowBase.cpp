/* 
 * File:   MainWindowBase.cpp
 * Author: stefan
 * 
 * Created on May 8, 2013, 10:07 AM
 */

#include "MainWindowBase.hpp"

#include <Attributes/TranslationAndConsecutiveID.hpp>

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
#include <bitmaps/show_parse_rules24x24.xpm>
#include <bitmaps/translate_bitmap.xpm> //for array translate_bitmap_xpm
#include <bitmaps/log_entries.xpm> //for array log_entries_xpm
#include <bitmaps/VT_icon.xpm> // array "VT_icon_xpm"
////see //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//#pragma GCC diagnostic pop
//ENable warning
#pragma GCC diagnostic warning "-Wwrite-strings"

/** For multi-threaded translation (e.g. translation in a non-GUI thread). */
#include <multi_threaded_translation/TranslateThreadProc.hpp>
#include <multi_threaded_translation/TranslateParameters.hpp>

//GetStdString(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <wxWidgets/UserInterface/wxGermanTranslationPanel.hpp>
#include <wxWidgets/UserInterface/wxParseTreePanel.hpp>//class wxParseTreePanel

#include <wx/icon.h> //class wxIcon
#include <wx/sizer.h> //class wxBoxSizer
#include <wx/splitter.h> //class wxSplitterWindow
#include <wx/textctrl.h> //class wxTextCtrl

#include <Translate/TranslationRule.hpp> //class TranslationRule
#include <Controller/thread_type.hpp> //typedef VTrans::thread_tyoe
#include <wxWidgets/UserInterface/TranslationRules/ShowTranslationRulesDialog.hpp>

////class SAX2GrammarRuleHandler
//#include <Xerces/SAX2GrammarRuleHandler.hpp>
////class SAX2TransformationRuleHandler
//#include <Xerces/SAX2TransformationRuleHandler.hpp>
////class SAX2TranslationRuleHandler
//#include <Xerces/SAX2TranslationRuleHandler.hpp>
////class SAX2VocAttributeDefintionHandler
//#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>
#include <Controller/ConfigurationHandler_type.hpp>

#include <wxWidgets/VTransApp.hpp> //::wxGetApp()
//UnLoadAndLoadDictionary(wxWindow *);
#include <wxWidgets/UserInterface/UserInterface.hpp>
#include <wxWidgets/UserInterface/wxTextControlDialog.hpp> //class wxTextControlDialog

//#define EVENT_HANDLER_BASE_CLASS_NAME wxTopLevelWindow /*wxTopLevelWindowBase wxDialog*/
//#define EVENT_HANDLER_CLASS_NAME_NON_SCOPED MainWindowBase /*wxTextInputDlg*/
//#define EVENT_HANDLER_CLASS_NAMESPACE "wxWidgets::"
////#define EVENT_HANDLER_CLASS_NAME wxCONCAT(EVENT_HANDLER_CLASS_NAMESPACE,EVENT_HANDLER_CLASS_NAME_NON_SCOPED) /*wxTextInputDlg*/
//#define EVENT_HANDLER_CLASS_NAME wxWidgets::MainWindowBase /*wxTextInputDlg*/

//BEGIN_EVENT_TABLE( wxWidgets::MainWindowBase, wxFrame)
//  EVT_TEXT(inputText, MainWindowBase::OnTextChanges)
//END_EVENT_TABLE

unsigned wxWidgets::MainWindowBase::s_windowID = 3;

namespace wxWidgets
{
MainWindowBase::MainWindowBase(wxTopLevelWindow * p_mainwindow)
  : m_p_mainwindow(p_mainwindow),
    m_parsebyrise(::wxGetApp().m_parsebyrise),
    m_translationcontrollerbase(::wxGetApp())
{
//	wxIcon wxiconThisDialog( VT_icon_xpm ) ;
//	SetIcon( wxiconThisDialog ) ;
}

//MainWindowBase::MainWindowBase(const MainWindowBase& orig) {
//}

MainWindowBase::~MainWindowBase() {
}

#define EVENT_HANDLER_CLASS_NAME wxWidgets::MainWindowBase

void EVENT_HANDLER_CLASS_NAME::AddInputAndOutputPanels()
{
  LOGN_DEBUG("begin")
//  mp_wxpanelTop = new wxPanel( mp_wxsplitterwindow ) ;

  mp_wxparsetreepanel = new /*wxPanel(*/ wxParseTreePanel(
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
    /*this*/ mp_wxsplitterwindow
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
    , /*wxID_ANY*/ ID_InputText, 
    wxEmptyString,
    wxDefaultPosition,
    wxDefaultSize,
    wxHSCROLL | wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB
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
//	wxGridSizer * p_gridsizerOuter;
//	p_gridsizerOuter = new wxGridSizer( 
//    1, //rows
//    1, //cols
//    0,
//    0 );
  p_boxsizerOuter = new wxBoxSizer( wxVERTICAL ) ;
	
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
  m_p_mainwindow->SetSizer( //p_gridsizerOuter
    p_boxsizerOuter
    );
  m_p_mainwindow->SetAutoLayout(true);
//	Layout();  
  LOGN_DEBUG("end")
}

void EVENT_HANDLER_CLASS_NAME::AddToolButtons()
{
  wxButton * p_wxbutton ;
  wxBoxSizer * p_boxsizerButtons = NULL;
  AddTranslateButton( p_boxsizerButtons ) ;
//  AddSettingsButton( p_boxsizerButtons ) ;

  AddAddTranslationRulesButton( p_boxsizerButtons ) ;
  AddRemoveTranslationRulesButton( p_boxsizerButtons ) ;
  AddShowTranslationRulesButton( p_boxsizerButtons ) ;
  AddShowParseRulesButton( p_boxsizerButtons ) ;
  AddAddTransformationRulesButton( p_boxsizerButtons ) ;
  AddRemoveTransformationRulesButton( p_boxsizerButtons ) ;
  AddAddGrammarRulesButton( p_boxsizerButtons ) ;
  AddRemoveGrammarRulesButton( p_boxsizerButtons ) ;
  AddAddVocAttrDefsButton( p_boxsizerButtons ) ;
  AddRemoveVocAttrDefsButton( p_boxsizerButtons ) ;

  AddResolve1ParseLevelButton( p_boxsizerButtons ) ;
  AddResolveSuperClassesButton( p_boxsizerButtons ) ;

  AddReInitializeGrammarButton(p_boxsizerButtons);

//  p_wxbutton = new wxButton(
//    //mp_wxsplitterwindow
//    //m_panelSplitterTop
//    //p_boxsizerOuter
//    (wxDialog *) this
//    , //wxID_ANY
//    ID_ShowTokenIndex2GrammarPart
//    , wxT("i2g")
//    ) ;
//  p_wxbutton->SetToolTip( wxT("show token index 2 grammar part map content")) ;
//  p_boxsizerButtons->Add(p_wxbutton, 0,
//    wxBOTTOM
//  , 2 );

  AddLoadDictionaryButton( p_boxsizerButtons);
  AddShowInformationButton( p_boxsizerButtons ) ;
  AddDrawParseTreeButton( p_boxsizerButtons);
  AddTruncateLogFileButton( p_boxsizerButtons);
//  p_boxsizerOuter->Add( p_boxsizerButtons ) ;
}

void EVENT_HANDLER_CLASS_NAME::AddReInitializeGrammarButton(wxSizer * p_wxsizer)
{
  AddBitmapButtonAndTooltip(ID_ReInitGrammarRules, re_initialize_grammar_xpm, 
    wxT("re-init grammar")/*, p_wxsizer*/ );
}

void EVENT_HANDLER_CLASS_NAME::AddAddTranslationRulesButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip( 
    ID_AddTranslationRules, add_translation_rules24x24_xpm, 
    wxT("add translation rules")/*, p_sizer*/ );
}

void EVENT_HANDLER_CLASS_NAME::AddShowTranslationRulesButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip(ID_ShowTranslationRules
    , show_translation_rules24x24_xpm, wxT("show translation rules")//, p_sizer
    );
}

void EVENT_HANDLER_CLASS_NAME::AddShowParseRulesButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip(ID_ShowParseRules
    , show_parse_rules24x24_xpm, wxT("show parse rules")//, p_sizer
    );
}

void EVENT_HANDLER_CLASS_NAME::AddAddTransformationRulesButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip(ID_AddTransformationRules, 
    add_transformation_rules24x24_xpm, wxT("add transformation rules")//, p_sizer
    );
}

void EVENT_HANDLER_CLASS_NAME::AddAddVocAttrDefsButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip(
    ID_AddVocAttrDefs, add_vocable_attribute_definitions24x24_xpm, 
    wxT("add vocable attribute definitions")/*, p_sizer*/);
}

void EVENT_HANDLER_CLASS_NAME::AddDrawParseTreeButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip(ID_DrawParseTree, info_xpm, 
    wxT("(re-)draw parse tree")/*, p_sizer*/);
}

void EVENT_HANDLER_CLASS_NAME::AddRemoveTranslationRulesButton( wxSizer * p_sizer )
{
//  mp_wxbutton = new //wxButton(
//    wxBitmapButton(
//    //mp_wxsplitterwindow
//    //m_panelSplitterTop
//    //p_boxsizerOuter
//    this
//    , //wxID_ANY
//    ID_RemoveTranslationRules
//  //    , wxT("1 L")
//    , wxBitmap( remove_translation_rules24x24_xpm )
//    ) ;
//  mp_wxbutton->SetToolTip( wxT("remove translations rules")) ;
//  p_sizer->Add(
//    mp_wxbutton
//    , 0 //strech factor. 0=do not stretch
//    , //wxEXPAND |
//      wxBOTTOM
//    , 2 );
}

void EVENT_HANDLER_CLASS_NAME::AddRemoveTransformationRulesButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip( ID_RemoveTransformationRules, 
    remove_transformation_rules24x24_xpm, wxT("remove transformation rules")/*, 
    p_sizer*/);
}

void EVENT_HANDLER_CLASS_NAME::AddAddGrammarRulesButton( wxSizer * p_sizer )
{
//  mp_wxbutton = new //wxButton(
//    wxBitmapButton(
//    //mp_wxsplitterwindow
//    //m_panelSplitterTop
//    //p_boxsizerOuter
//    this
//    , //wxID_ANY
//    ID_AddGrammarRules
//  //    , wxT("1 L")
//    , wxBitmap( add_grammar_rules24x24_xpm )
//    ) ;
//  mp_wxbutton->SetToolTip( wxT("add grammar rules")) ;
//  p_sizer->Add(
//    mp_wxbutton
//    , 0 //strech factor. 0=do not stretch
//    , //wxEXPAND |
//      wxBOTTOM
//    , 2 );
}

void EVENT_HANDLER_CLASS_NAME::AddLoadDictionaryButton( wxSizer * p_sizer)
{
  m_p_wxToolBarToolBaseLoadDictionary = AddBitmapButtonAndTooltip(
    ID_LoadDictionary
    , re_load_dictionary_xpm
    , wxT("re-load dictionary...")
    ) ;
}

void EVENT_HANDLER_CLASS_NAME::AddRemoveGrammarRulesButton( wxSizer * p_sizer )
{
//  mp_wxbutton = new //wxButton(
//    wxBitmapButton(
//    //mp_wxsplitterwindow
//    //m_panelSplitterTop
//    //p_boxsizerOuter
//    this
//    , //wxID_ANY
//    ID_RemoveGrammarRules
//  //    , wxT("1 L")
//    , wxBitmap( remove_grammar_rules24x24_xpm )
//    ) ;
//  mp_wxbutton->SetToolTip( wxT("remove grammar rules")) ;
//  p_sizer->Add(
//    mp_wxbutton
//    , 0 //strech factor. 0=do not stretch
//    , //wxEXPAND |
//      wxBOTTOM
//    , 2 );
}

void EVENT_HANDLER_CLASS_NAME::AddRemoveVocAttrDefsButton( wxSizer * p_sizer )
{
//  mp_wxbutton = new //wxButton(
//    wxBitmapButton(
//    //mp_wxsplitterwindow
//    //m_panelSplitterTop
//    //p_boxsizerOuter
//    this
//    , //wxID_ANY
//    ID_RemoveVocAttrDefs
//  //    , wxT("1 L")
//    , wxBitmap( remove_vocable_attribute_definitions24x24_xpm )
//    ) ;
//  mp_wxbutton->SetToolTip( wxT("remove vocable attribute definitions")) ;
//  p_sizer->Add(
//    mp_wxbutton
//    , 0 //strech factor. 0=do not stretch
//    , //wxEXPAND |
//      wxBOTTOM
//    , 2 );
}

void EVENT_HANDLER_CLASS_NAME::AddResolve1ParseLevelButton( wxSizer * p_sizer )
{
//  mp_wxbutton = new //wxButton(
//    wxBitmapButton(
//    //mp_wxsplitterwindow
//    //m_panelSplitterTop
//    //p_boxsizerOuter
//    this
//    , //wxID_ANY
//    ID_Resolve1ParseLevel
//  //    , wxT("1 L")
//    , wxBitmap( resolve_1parse_level24x24_xpm )
//    ) ;
//  mp_wxbutton->SetToolTip( wxT("resolve 1 parse level")) ;
//  p_sizer->Add(
//    mp_wxbutton
//    , 0 //strech factor. 0=do not stretch
//    , //wxEXPAND |
//      wxBOTTOM
//    , 2 );
  AddBitmapButtonAndTooltip(
    ID_Resolve1ParseLevel, resolve_1parse_level24x24_xpm, 
    wxT("resolve 1 parse level")/*, p_sizer*/);
}

void EVENT_HANDLER_CLASS_NAME::AddResolveSuperClassesButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip(
    ID_ResolveSuperclass, resolve_superclasses24x24_xpm, 
    wxT("resolve superclasses")/*, p_sizer*/);
}

void EVENT_HANDLER_CLASS_NAME::AddShowInformationButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip(ID_Info, info_xpm, wxT("show information")/*, 
    p_sizer*/);
}

//void EVENT_HANDLER_CLASS_NAME::AddSettingsButton( wxSizer * p_sizer )
//{
//  AddBitmapButtonAndTooltip(ID_Settings, translate_bitmap_xpm,
//    wxT("settings")/*, p_sizer*/);
//}

void EVENT_HANDLER_CLASS_NAME::AddTranslateButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip(ID_Translate, translate_bitmap_xpm,
    wxT("translate")/*, p_sizer*/);
}

void EVENT_HANDLER_CLASS_NAME::AddTruncateLogFileButton( wxSizer * p_sizer )
{
  AddBitmapButtonAndTooltip(ID_ShowLogEntriesDialog, log_entries_xpm, 
    wxT("show log entries dialog")/*, p_sizer*/);
}

void EVENT_HANDLER_CLASS_NAME::GetEntireInputText(std::string & r_stdstrInputText)
{
//  std::string stdstrInputText ;
  wxString wxstringWholeInputText ;
//  wxTextPos wxtextposNumberOfCharacters  ;
//  wxtextposNumberOfCharacters = mp_textctrlEnglishText->GetLastPosition() ;
//  wxstringWholeInputText = mp_textctrlEnglishText->GetRange(0 ,
//    wxtextposNumberOfCharacters) ;
  wxstringWholeInputText = mp_textctrlEnglishText->GetValue() ;
  r_stdstrInputText = //std::string( wxstringWholeInputText.c_str() ) ;
    GetStdString(wxstringWholeInputText);
//  return stdstrInputText ;
}

void EVENT_HANDLER_CLASS_NAME::Translate()
{
  //  try
  //  {
    std::string stdstrWholeInputText ;
    GetEntireInputText(stdstrWholeInputText) ;
  //  AxSpeech axspeech ;
  //  axspeech.Say( stdstrWholeInputText ) ;

  //  std::string stdstrWholeTransl ;

  //  std::vector<std::vector<TranslationAndConsecutiveID> >
  //    stdvec_stdvecTranslationAndConsecutiveID ;

  /** Avoid translating if there is no input text. */
  if( ! stdstrWholeInputText.empty() )
  {
    bool translateAsync = true;
    /** Prevent multiple translation threads. */
    DisableDoTranslateControls();

    VTrans::multiThreadedTranslation::TranslateParameters * p_tranlParams = new 
      VTrans::multiThreadedTranslation::TranslateParameters(
      stdstrWholeInputText,
      //g_p_translationcontrollerbase,
      & m_translationcontrollerbase,
      & m_translationResult,
      m_translationcontrollerbase
      );
    if( translateAsync )
    {
      m_translateThread.start(
        VTrans::TranslateThreadProc, p_tranlParams, "Translate");
    }
    else
      VTrans::TranslateThreadProc(p_tranlParams);
  }
}

//DWORD THREAD_FUNCTION_CALLING_CONVENTION UnloadDictionary(void * p_v)
//{
////  wxCondition & wxCond = *(wxCondition *)p_v;
//  EVENT_HANDLER_CLASS_NAME & event_handler = *(EVENT_HANDLER_CLASS_NAME *)p_v;
////  EVENT_HANDLER_CLASS_NAME * p_event_handler = (EVENT_HANDLER_CLASS_NAME *) p_v;
//  ::wxGetApp().s_dictionary.clear();
////  wxCond.Signal();
//  ::wxGetApp().EndTimer();
//  LOGN("after clearing the dictionary.")
//  wxCloseEvent wxcloseEvent;
//  //Add the close event for destroying the window
//  /*::wxGetApp().*/event_handler.GetEventHandler()->AddPendingEvent(wxcloseEvent);
//}
//
//void EVENT_HANDLER_CLASS_NAME::UnloadDictionaryShowingStatus()
//{
//  ::wxGetApp().StartTimer();
//  VTrans::thread_type thread;
////    wxMutex mutex;
////    wxCondition wxCond(mutex);
////    //"The mutex object MUST be locked before calling Wait()"
////    mutex.Lock();
//  thread.start(UnloadDictionary, /*& wxCond NULL*/ this );
////    wxCond.Wait();
////  ::wxGetApp().s_dictionary.clear();
////    ::wxGetApp().EndTimer();
////    LOGN("after clearing the dictionary.")
//}

//};// class MainWindowBase

}
