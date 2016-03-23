/** OperatingSystem::GetTimeCountInNanoSeconds(...) */
#include <Controller/time/GetTickCount.hpp>

#include <wx/event.h> //BEGIN_EVENT_TABLE
#include <wx/textdlg.h> //::wxGetTextFromUser(...)
#include <wx/msgdlg.h> //::wxMessageBox(...)

#include "MainFrame.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h>
//getwxString(...)
#include <wxWidgets/Controller/character_string/wxStringHelper.hpp>
#include <Attributes/TranslationAndConsecutiveID.hpp>
#include <Attributes/TranslationResult.hpp> //class WordCompound
#include <Controller/thread_type.hpp> //typedef VTrans::thread_type
#include <Translate/TranslationRule.hpp> //class TranslationRule

//#include <Xerces/SAX2GrammarRuleHandler.hpp>
//#include <Xerces/SAX2TransformationRuleHandler.hpp>
//#include <Xerces/SAX2TranslationRuleHandler.hpp>
//#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>
#include <Controller/ConfigurationHandler_type.hpp>
#include <Controller/GetErrorMessageFromLastErrorCode.hpp>
#include <Controller/Logger/LogFileAccessException.hpp>

/** For multi-threaded translation (e.g. translation in a non-GUI thread). */
#include <multi_threaded_translation/TranslateThreadProc.hpp>
#include <multi_threaded_translation/TranslateParameters.hpp>

#include <wxWidgets/UserInterface/TranslationRules/ShowTranslationRulesDialog.hpp>
#include <wxWidgets/UserInterface/UserInterface.hpp>
#include <wxWidgets/UserInterface/DictionaryStatisticsWindow.hpp>
#include <wxWidgets/UserInterface/wxParseTreePanel.hpp>
#include <wxWidgets/UserInterface/wxGermanTranslationPanel.hpp>
#include <wxWidgets/VTransApp.hpp> //::wxGetApp()
#include <wxWidgets/UserInterface/wxTextControlDialog.hpp>
#include <wxWidgets/UserInterface/ParseRules/ShowParseRulesDialog.hpp>
#include <Controller/thread_type.hpp> //typedef VTrans::thread_tyoe
#include "VocabularyInMainMem/VocablesForWord.hpp"
#include <Translate/TranslateParseByRiseTree.hpp>

#define EVENT_HANDLER_BASE_CLASS_NAME wxFrame /*MainWindowBase*/ /*wxTopLevelWindowBase wxDialog*/
#define EVENT_HANDLER_CLASS_NAME_NON_SCOPED MainWindowBase /*MainFrame*/ /*wxTextInputDlg*/
#define EVENT_HANDLER_CLASS_NAMESPACE "wxWidgets::"
//#define EVENT_HANDLER_CLASS_NAME wxCONCAT(EVENT_HANDLER_CLASS_NAMESPACE,EVENT_HANDLER_CLASS_NAME_NON_SCOPED) /*wxTextInputDlg*/
#define EVENT_HANDLER_CLASS_NAME wxWidgets::MainFrame /*MainWindowBase*/
  //wxWidgets::MainFrame /*wxTextInputDlg*/
#define EVT_HANDLER_CASTED (EVENT_HANDLER_BASE_CLASS_NAME) EVENT_HANDLER_CLASS_NAME

#define BEGIN_EVENT_TABLE_(theClass, baseClass) \
    const wxEventTable (EVENT_HANDLER_BASE_CLASS_NAME) (theClass::sm_eventTable) = \
        { &baseClass::sm_eventTable, &theClass::sm_eventTableEntries[0] }; \
    const wxEventTable *theClass::GetEventTable() const \
        { return &theClass::sm_eventTable; } \
    wxEventHashTable theClass::sm_eventHashTable(theClass::sm_eventTable); \
    wxEventHashTable &theClass::GetEventHashTable() const \
        { return theClass::sm_eventHashTable; } \
    const wxEventTableEntry theClass::sm_eventTableEntries[] = { \

//#define wxStaticCastEvent(wxTimerEventFunction, &func)
//#define wxStaticCastEvent(type, val) wx_static_cast(type, (& EVENT_HANDLER_BASE_CLASS_NAME) val)
typedef void (wxEvtHandler::*wxEventFunction)(wxEvent&);

/** This event is intended to be posted after translation in a non-GUI thread
 *  for updating the view in GUI thread. */
DEFINE_LOCAL_EVENT_TYPE(UpdateAfterTranslationEvent)

//  EVT_BUTTON( ID_AddGrammarRules , (EVENT_HANDLER_BASE_CLASS_NAME) ( EVENT_HANDLER_CLASS_NAME::)OnAddGrammarRules )

/** see http://docs.wxwidgets.org/trunk/classwx_tool_bar.html:
 *   if the control is a tool button, it should be "EVT_MENU" should be used,
 *   else the event handler function is not called.*/
#define BUTTON_EVENT_TYPE EVT_MENU /*EVT_BUTTON*/

BEGIN_EVENT_TABLE( EVENT_HANDLER_CLASS_NAME, EVENT_HANDLER_BASE_CLASS_NAME)
  EVT_COMMAND(wxID_ANY, UpdateAfterTranslationEvent, 
    EVENT_HANDLER_CLASS_NAME::OnUpdateAfterTranslation)
  //EVT_TEXT(ID_InputText, EVENT_HANDLER_CLASS_NAME::OnInputTextChanges)
  //TODO http://www.wxwidgets.org/docs/faqgtk.htm#charinframe
  // "Why does my simple program using EVT_CHAR not work?"
  EVT_CHAR( EVENT_HANDLER_CLASS_NAME::OnChar)
  EVT_TIMER( ID_Timer, EVENT_HANDLER_CLASS_NAME::OnTimerEvent)
  BUTTON_EVENT_TYPE( ID_AddGrammarRules , EVENT_HANDLER_CLASS_NAME::OnAddGrammarRules )
  BUTTON_EVENT_TYPE( ID_ShowGrammarPartMemoryAddress,
    EVENT_HANDLER_CLASS_NAME::OnShowGrammarPartMemoryAddress)
  BUTTON_EVENT_TYPE( ID_Translate_On_Text_Changes,
    EVENT_HANDLER_CLASS_NAME::OnTranslateOnTextChanges)
  /** see http://stackoverflow.com/questions/27911915/updating-wxwidgets-radio-item-dynamically
   * for radio items several IDs are mapped to an event function. */
  BUTTON_EVENT_TYPE( ID_LogLevel_Warning, EVENT_HANDLER_CLASS_NAME::OnLogLevel)
  BUTTON_EVENT_TYPE( ID_LogLevel_Debug, EVENT_HANDLER_CLASS_NAME::OnLogLevel)
  BUTTON_EVENT_TYPE( ID_ShowTranslatedWord,
    EVENT_HANDLER_CLASS_NAME::OnShowTranslatedWord)
  BUTTON_EVENT_TYPE( ID_DecreaseFontSize, EVENT_HANDLER_CLASS_NAME::OnDecreaseParseTreePanelFontSize )
  BUTTON_EVENT_TYPE( ID_IncreaseFontSize, EVENT_HANDLER_CLASS_NAME::OnIncreaseParseTreePanelFontSize )
  BUTTON_EVENT_TYPE( ID_AddTransformationRules ,
    EVENT_HANDLER_CLASS_NAME::OnAddTransformationRules )
  BUTTON_EVENT_TYPE( ID_AddTranslationRules , EVENT_HANDLER_CLASS_NAME::OnAddTranslationRules )
  BUTTON_EVENT_TYPE( ID_AddVocAttrDefs, EVENT_HANDLER_CLASS_NAME::OnAddVocAttrDefs )
  BUTTON_EVENT_TYPE( ID_Settings , EVENT_HANDLER_CLASS_NAME::OnSettings)
  BUTTON_EVENT_TYPE( ID_Translate , EVENT_HANDLER_CLASS_NAME::OnTranslateButton)
  BUTTON_EVENT_TYPE( ID_LookupWord , EVENT_HANDLER_CLASS_NAME::OnLookupWord)
  BUTTON_EVENT_TYPE( ID_ShowDictionaryStatistics , EVENT_HANDLER_CLASS_NAME::OnShowDictionaryStatistics)
  BUTTON_EVENT_TYPE( ID_UnloadDictionary , EVENT_HANDLER_CLASS_NAME::OnUnloadDictionary)
  BUTTON_EVENT_TYPE( ID_TruncateLogFile, EVENT_HANDLER_CLASS_NAME::OnTruncateLogFileButton)
  BUTTON_EVENT_TYPE( ID_LoadDictionary , EVENT_HANDLER_CLASS_NAME::OnLoadDictionaryButton)
  BUTTON_EVENT_TYPE( ID_ShowTokenIndex2GrammarPart, EVENT_HANDLER_CLASS_NAME::
    OnShowTokenIndex2GrammarPartButton)
  BUTTON_EVENT_TYPE( ID_ShowTranslationRules,
    EVENT_HANDLER_CLASS_NAME::OnShowTranslationRulesButton )
  BUTTON_EVENT_TYPE( ID_ShowParseRules,
    EVENT_HANDLER_CLASS_NAME::OnShowParseRulesButton )
  BUTTON_EVENT_TYPE( ID_ReInitGrammarRules ,
    EVENT_HANDLER_CLASS_NAME::OnReInitGrammarRulesButton)
  BUTTON_EVENT_TYPE( ID_RemoveGrammarRules ,
    EVENT_HANDLER_CLASS_NAME::OnRemoveGrammarRules )
  BUTTON_EVENT_TYPE( ID_RemoveTransformationRules ,
    EVENT_HANDLER_CLASS_NAME::OnRemoveTransformationRules )
  BUTTON_EVENT_TYPE( ID_RemoveTranslationRules ,
    EVENT_HANDLER_CLASS_NAME::OnRemoveTranslationRules )
  BUTTON_EVENT_TYPE( ID_RemoveVocAttrDefs ,
    EVENT_HANDLER_CLASS_NAME::OnRemoveVocAttrDefs )
  BUTTON_EVENT_TYPE( ID_ResolveSuperclass,
    EVENT_HANDLER_CLASS_NAME::OnResolveSuperclassGrammarParts )
  BUTTON_EVENT_TYPE( ID_Resolve1ParseLevel ,
    EVENT_HANDLER_CLASS_NAME::OnResolve1ParseLevelButton)
  BUTTON_EVENT_TYPE( ID_DrawParseTree ,
    EVENT_HANDLER_CLASS_NAME::OnDrawParseTreeButton)
  BUTTON_EVENT_TYPE( ID_Info, EVENT_HANDLER_CLASS_NAME::OnInfoButton)
  EVT_CLOSE( EVENT_HANDLER_CLASS_NAME::OnClose)
END_EVENT_TABLE()

void EVENT_HANDLER_CLASS_NAME::OnAddGrammarRules( wxCommandEvent & wxcmd )
{
//  AddTranslationRules() ;
//  SAX2GrammarRuleHandler sax2grammarrulehandler(
//    wxGetApp().m_parsebyrise
//    ) ;
  wxString wxstrTitle( wxT("Choose at least 1 grammar rule file") ) ;
  //TODO make usable for class ConfigReaderBase
//  wxGetApp().ProcessSelectedXMLfiles(
//    sax2grammarrulehandler ,
//    wxstrTitle,
//    ::wxGetCwd()
//    );
}

void EVENT_HANDLER_CLASS_NAME::OnChar( wxKeyEvent & e)
{
    if( e.GetKeyCode() == WXK_ESCAPE )
    {
        m_translationcontrollerbase.Stop();
    }
}

void EVENT_HANDLER_CLASS_NAME::OnDecreaseParseTreePanelFontSize( wxCommandEvent & wxcmd )
{
  mp_wxparsetreepanel->DecreaseFontSizeBy1Point();
//  p_wxmenu->Append( ID_DecreaseFontSize, wxT("&decrease parse tree panel's font size\tCTRL+-") );
  //TODO
//  SetFontSize(mp_wxparsetreepanel->GetFontSizeInPoint() );
}

void EVENT_HANDLER_CLASS_NAME::OnLogLevel( wxCommandEvent & wxcmd )
{
  switch( wxcmd.GetId() )
  {
   case ID_LogLevel_Debug :
     g_logger.SetLogLevel("debug");
     break;
   case ID_LogLevel_Warning :
     g_logger.SetLogLevel("warning");
     break;
  }
}

void EVENT_HANDLER_CLASS_NAME::OnIncreaseParseTreePanelFontSize( wxCommandEvent & wxcmd )
{
  mp_wxparsetreepanel->IncreaseFontSizeBy1Point();
  //mp_wxparsetreepanel->IncreaseFontHeightBy1Pixel();
}

void EVENT_HANDLER_CLASS_NAME::OnShowGrammarPartMemoryAddress( wxCommandEvent & wxcmd )
{
  ::wxGetApp().m_GUIattributes.m_bShowGrammarPartAddress = wxcmd.IsChecked();
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
}

void EVENT_HANDLER_CLASS_NAME::OnShowTranslatedWord( wxCommandEvent & wxcmd )
{
  ::wxGetApp().m_GUIattributes.m_bShowTranslation = wxcmd.IsChecked();
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
}

void EVENT_HANDLER_CLASS_NAME::OnAddTransformationRules( wxCommandEvent & wxcmd )
{
//  AddTranslationRules() ;
//  Xerces::SAX2TransformationRuleHandler sax2transformationrulehandler(
//    wxGetApp()
//    ) ;
  //TODO make usable for class ConfigReaderBase
//  wxGetApp().ProcessSelectedXMLfiles(
//    sax2transformationrulehandler,
//    wxString( wxT("Choose at least 1 transFORMation rule file") ),
//    ::wxGetCwd()
//    ) ;
}

void EVENT_HANDLER_CLASS_NAME::OnAddTranslationRules( wxCommandEvent & wxcmd )
{
//  AddTranslationRules() ;
//  SAX2TranslationRuleHandler sax2translationrulehandler(
//    wxGetApp().m_translateparsebyrisetree ,
//    wxGetApp().m_parsebyrise ,
//    wxGetApp()
//    ) ;
  //TODO make usable for class ConfigReaderBase
//  wxGetApp().ProcessSelectedXMLfiles(
//    sax2translationrulehandler ,
//    wxT("Choose at least 1 translation rule file"),
//    ::wxGetCwd()
//    ) ;
}

void EVENT_HANDLER_CLASS_NAME::OnAddVocAttrDefs( wxCommandEvent & wxcmd )
{
//  AddTranslationRules() ;
  //TODO make usable for class ConfigReaderBase
//  Xerces::SAX2VocAttributeDefinitionHandler sax2vocattributedefintionhandler(
//    wxGetApp().m_translateparsebyrisetree ,
//    wxGetApp()
//    ) ;
//  wxGetApp().ProcessSelectedXMLfiles(
//    sax2vocattributedefintionhandler ,
//    wxT("Choose at least 1 vocabulary attribute definitions file"),
//    ::wxGetCwd()
//    );
}

DWORD THREAD_FUNCTION_CALLING_CONVENTION UnloadDictionary(void * p_v)
{
//  wxCondition & wxCond = *(wxCondition *)p_v;
  EVENT_HANDLER_CLASS_NAME & event_handler = *(EVENT_HANDLER_CLASS_NAME *)p_v;
//  EVENT_HANDLER_CLASS_NAME * p_event_handler = (EVENT_HANDLER_CLASS_NAME *) p_v;
  ::wxGetApp().s_dictReaderAndVocAccess.m_vocAccess.clear();
//  wxCond.Signal();
  ::wxGetApp().EndTimer();
  LOGN("after clearing the dictionary.")
//  wxCloseEvent wxcloseEvent;
//  //Add the close event for destroying the window
//  /*::wxGetApp().*/event_handler.GetEventHandler()->AddPendingEvent(wxcloseEvent);
  return 0;
}

/** The sequence:
 *  -unloading the dictionary and
 *  -sending a main window close event afterwards
 *  must be done in this thread function. */
DWORD THREAD_FUNCTION_CALLING_CONVENTION UnloadDictionaryAndSendCloseEvent(void * p_v)
{
  UnloadDictionary(p_v);
  EVENT_HANDLER_CLASS_NAME & event_handler = *(EVENT_HANDLER_CLASS_NAME *) p_v;
//  EVENT_HANDLER_CLASS_NAME * p_event_handler = (EVENT_HANDLER_CLASS_NAME *) p_v;
  wxCloseEvent wxcloseEvent(wxEVT_CLOSE_WINDOW, wxWidgets::MainWindowBase::s_windowID);
  //Add the close event for destroying the window
//  /*::wxGetApp().*/event_handler.GetEventHandler()->AddPendingEvent(wxcloseEvent);
  //::wxPostEvent(event_handler.GetEventHandler(), wxcloseEvent);
  event_handler.AddPendingEvent(wxcloseEvent);
  return 0;
}

void EVENT_HANDLER_CLASS_NAME::UnloadDictionaryShowingStatus()
{
  //TODO SIGSEGV here
  ::wxGetApp().StartTimer();
  VTrans::thread_type thread;
//    wxMutex mutex;
//    wxCondition wxCond(mutex);
//    //"The mutex object MUST be locked before calling Wait()"
//    mutex.Lock();
  thread.start(UnloadDictionary, /*& wxCond NULL*/ this );
//    wxCond.Wait();
//  ::wxGetApp().s_dictionary.clear();
//    ::wxGetApp().EndTimer();
//    LOGN("after clearing the dictionary.")

//  thread.WaitForTermination();
//  wxCloseEvent wxcloseEvent;
//  //Add the close event for destroying the window
//  /*::wxGetApp().*/AddPendingEvent(wxcloseEvent);
}

/** The close event must be sent after unloading the dictionary.
 *  While unloading the dictionary a status should be shown. */
void EVENT_HANDLER_CLASS_NAME::UnloadDictionaryShowingStatusAndSendCloseEvent()
{
  //TODO SIGSEGV here
  ::wxGetApp().StartTimer();
  /** Must use a thread object that persists longer than the thread function,
   *  else this program may crash? (if thread.start(...) or functions called
   *  from it accesses member variables of the thread object) */
//  VTrans::thread_type thread;
  /*thread*/ m_unloadDictAndCloseWindowThread.start(
    UnloadDictionaryAndSendCloseEvent, /*& wxCond NULL*/ this );

//  wxCloseEvent wxcloseEvent;
//  /** Add the close event for destroying the window */
//  /*::wxGetApp().*/AddPendingEvent(wxcloseEvent);
}

void EVENT_HANDLER_CLASS_NAME::OnClose( wxCloseEvent & wxcmd )
{
  LOGN("begin before clearing the dictionary")
  const unsigned numberOfVocPairs = ::wxGetApp().s_dictReaderAndVocAccess.
    m_vocAccess.GetNumberOfVocPairs();
  const unsigned numberOfEnglishWords = ::wxGetApp().s_dictReaderAndVocAccess.
    m_vocAccess.GetNumberOfEnglishWords();
  if( /*numberOfVocPairs*/ numberOfEnglishWords > 0 )
  {
    UnloadDictionaryShowingStatusAndSendCloseEvent();
  }
  else
  {
//  //Call delete to exit this application. (without it is not finished)
//  delete(this) ;
//  this->Destroy() ;
    Destroy() ;
//  Close() ;
  }
  //::wxGetApp().ExitMainLoop() ;
  LOGN("end")
}

void EVENT_HANDLER_CLASS_NAME::OnDrawParseTreeButton(wxCommandEvent & wxcmd )
{
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
}

void EVENT_HANDLER_CLASS_NAME::OnSettings(wxCommandEvent & wxcmd )
{
  wxString wxstrName = ::wxGetTextFromUser(
    wxT("enter a settings name:") );
  wxString wxstrValue = ::wxGetTextFromUser(
    wxT("enter a settings value:") );
  ::wxGetApp().Settings(
    wxWidgets::GetStdString(wxstrName).c_str(),
    wxWidgets::GetStdString(wxstrValue).c_str() );
}

void EVENT_HANDLER_CLASS_NAME::OnShowDictionaryStatistics(wxCommandEvent & wxcmd )
{
//  const fastestUnsignedDataType numArrayEles = EnglishWord::beyond_last_entry;
//  fastestUnsignedDataType numberOfWordClassRepresentations[numArrayEles];
//  VTransApp & vt = ::wxGetApp();
//  vt.s_dictReaderAndVocAccess.m_vocAccess.GetStatistics(
//    numberOfWordClassRepresentations, numArrayEles);

//  wxString wxstr = wxString::Format(
//    wxT("# English words:%lu\n"
//        "# nouns:%lu\n")
//    , numberOfWordClassRepresentations[EnglishWord::noun]
//   );
//  wxTextControlDialog wxd(wxstr);
//  wxd.ShowModal();
  if( m_translateThread.IsRunning() )
  {
    wxMessageBox(wxT("can't collect dict stats because currently using "
      "dictionary while translating") );
  }
  else
  {
    DictionaryStatisticsWindow * p_dictionaryStatisticsWindow = new DictionaryStatisticsWindow();
    p_dictionaryStatisticsWindow->Show();
    p_dictionaryStatisticsWindow->GetStatistics();
    wxGetApp().m_p_dictionaryStatisticsWindow = p_dictionaryStatisticsWindow;
  //  ShowStatistics();
    wxGetApp().DisableDictAccessingActions();
  }
}

void EVENT_HANDLER_CLASS_NAME::OnInfoButton( wxCommandEvent & wxcmd )
{
  VTransApp & vt = ::wxGetApp();
  wxString wxstr = wxString::Format(
    wxT("# English words:%lu\n"
        "# voc pairs:%lu\n"
        "# bytes for dict:%lu\n"
        "# grammar rules:%u\n"
        "# translation rules:%u\n"
        "# transFORMation rules:%u\n"
        "# vocabulary attribute definitions:%u"
      ) ,
//    OneLinePerWordPair::s_dwNumberOfVocabularyPairs
//    ::wxGetApp().s_numberOfVocabularyPairs
    vt.s_dictReaderAndVocAccess.m_vocAccess.GetNumberOfEnglishWords()
    , ::wxGetApp().s_dictReaderAndVocAccess.m_vocAccess.GetNumberOfVocPairs()
    , ::wxGetApp().s_dictReaderAndVocAccess.m_vocAccess.GetNumberOfAllocatedBytes()
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

void EVENT_HANDLER_CLASS_NAME::OnInputTextChanges(wxCommandEvent & event)
{
  if( /*::wxGetApp::InstantTranslation*/ 1 )
  {
//    PossiblyStopTranslation();
    if( m_translateThread.IsRunning() )
    {
      m_translationcontrollerbase.Stop();
      m_translateThread.WaitForTermination();
//      wxMessageBox( wxT("already translating") );
    }
    Translate();
  }
}

void EVENT_HANDLER_CLASS_NAME::OnLoadDictionaryButton( wxCommandEvent & wxcmd )
{
  const std::string & std_strVocabularyFilePath = ::wxGetApp().
    m_stdstrVocabularyFilePath;
  std::string::size_type lastPathSep = std_strVocabularyFilePath.rfind("/\\");
  wxString wxstrVocabularyDirPath, wxstrVocabularyFileName;
  if( lastPathSep == std::string::npos )
  {
    wxstrVocabularyDirPath = wxT("");
    wxstrVocabularyFileName = wxWidgets::getwxString(std_strVocabularyFilePath);
  }
  else
  {
    wxstrVocabularyDirPath = wxWidgets::getwxString(
      std_strVocabularyFilePath.substr(0, lastPathSep) );
    wxstrVocabularyFileName = wxWidgets::getwxString(
      std_strVocabularyFilePath.substr(lastPathSep + 1) );
  }
//  wxString wxstrVocabularyFilePath = wxWidgets::getwxString();
  LoadOrReloadDictionary( this, wxstrVocabularyDirPath, wxstrVocabularyFileName);
}

void EVENT_HANDLER_CLASS_NAME::OnReInitGrammarRulesButton( wxCommandEvent & wxcmd )
{

  m_parsebyrise.ClearAllGrammarStuff() ;
  wxGetApp().m_translateparsebyrisetree.FreeMemoryForTranslationRules();
  wxGetApp().m_stdmap_syntaxtreepath2transformationrule.clear();
  wxGetApp().m_translateparsebyrisetree.FreeMemoryForVocAttrDefs() ;

  m_parsebyrise.InitGrammarRules() ;
  m_translationcontrollerbase.ReadMainConfigFile(
    m_translationcontrollerbase.m_std_strMainConfigFilePath);
}

void EVENT_HANDLER_CLASS_NAME::OnRemoveGrammarRules(wxCommandEvent & wxcmd )
{
  LOGN("begin")
//  wxGetApp().m_parsebyrise.m_stdmap_RuleName2RuleID.clear() ;
//  wxGetApp().m_parsebyrise.m_stdmap_wRuleID2RuleName.clear() ;
//  wxGetApp().m_parsebyrise.m_stdmap_wGrammarPartID2SuperordinateID.clear() ;
//  //Absolutely necessary?!
//  wxGetApp().m_parsebyrise.InsertFundamentalRuleIDs() ;

  m_parsebyrise.ClearAllGrammarStuff() ;
  m_parsebyrise.InitGrammarRules() ;
  LOGN("end")
}

void EVENT_HANDLER_CLASS_NAME::OnRemoveTransformationRules(wxCommandEvent & wxcmd )
{
  LOGN("begin")
  wxGetApp().m_stdmap_syntaxtreepath2transformationrule.clear() ;
  LOGN("end")
}

void EVENT_HANDLER_CLASS_NAME::OnRemoveTranslationRules(wxCommandEvent & wxcmd )
{
  LOGN("begin")
  wxGetApp().m_translateparsebyrisetree.FreeMemoryForTranslationRules() ;
//  wxGetApp().m_translateparsebyrisetree.
//    m_stdmap_translationrule2ConditionsAndTranslation.clear() ;
//  wxGetApp().m_translateparsebyrisetree.
//    m_stdmap_p_translationrule2ConditionsAndTranslation.clear() ;
  LOGN("end")
}

void EVENT_HANDLER_CLASS_NAME::OnRemoveVocAttrDefs(wxCommandEvent & wxcmd )
{
  LOGN("begin")
  wxGetApp().m_translateparsebyrisetree.FreeMemoryForVocAttrDefs() ;
//  wxGetApp().m_translateparsebyrisetree.
//    m_stdmap_translationrule2ConditionsAndTranslation.clear() ;
//  wxGetApp().m_translateparsebyrisetree.
//    m_stdmap_p_translationrule2ConditionsAndTranslation.clear() ;
  LOGN("end")
}

void EVENT_HANDLER_CLASS_NAME::OnResolveSuperclassGrammarParts(wxCommandEvent & wxcmd )
{
  m_parsebyrise.ReplaceGrammarPartIDsBySuperordinate() ;
  OnShowTokenIndex2GrammarPartButton(wxcmd) ;
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
}

void EVENT_HANDLER_CLASS_NAME::OnResolve1ParseLevelButton( wxCommandEvent & wxcmd )
{
  std::string stdstrWholeInputText ;
  GetEntireInputText(stdstrWholeInputText) ;
  if( m_std_strWholeInputText == "" || 
     m_std_strWholeInputText != stdstrWholeInputText )
  {
    m_parsebyrise.m_wParseLevel = 0;
    m_std_strWholeInputText = stdstrWholeInputText;
  }
  if( m_parsebyrise.m_wParseLevel ==
    //fully resolved or not begun yet.
    0 )
  {
    m_translationcontrollerbase.ResetVocabularyInMainMemToFundamentalWordsOnly();
    m_parsebyrise.ClearParseTree() ;
    m_parsebyrise.CreateInitialGrammarParts ( stdstrWholeInputText ) ;
  }
  else
  {
    ++ m_parsebyrise.m_dwMapIndex ;
    bool grammarRuleApplied = m_parsebyrise.ResolveGrammarRules();
    
    bool bReplacedGrammarPartIDsBySuperordinate = m_parsebyrise.
        ReplaceGrammarPartIDsBySuperordinate();
    
    if( ! (grammarRuleApplied || bReplacedGrammarPartIDsBySuperordinate) )
    {
      //m_parsebyrise.m_wParseLevel = 0;
      wxMessageBox(wxT("no futher resolution of parse levels possible.\n"
        "Change the input text in order to start from the beginning again") );
    }
  }
  OnShowTokenIndex2GrammarPartButton(wxcmd) ;
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
}

void EVENT_HANDLER_CLASS_NAME::OnShowTokenIndex2GrammarPartButton( wxCommandEvent & wxcmd )
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

void EVENT_HANDLER_CLASS_NAME::OnShowParseRulesButton( wxCommandEvent & wxcmd )
{

  std::map<WORD,WORD>::const_iterator c_iter =
    m_parsebyrise.m_stdmap_wGrammarPartID2SuperordinateID.begin();
  int arraySize = m_parsebyrise.m_stdmap_wGrammarPartID2SuperordinateID.size();
  wxString choices[arraySize];
  fastestUnsignedDataType arrayIndex = 0;
  while( c_iter != m_parsebyrise.m_stdmap_wGrammarPartID2SuperordinateID.end() )
  {
    choices[arrayIndex ++] = wxWidgets::getwxString(
      m_parsebyrise.GetGrammarPartName(c_iter->second) );
    ++ c_iter;
  }
  wxWidgets::ShowParseRulesDialog showParseRulesDialog(this, choices,
    arraySize);
  showParseRulesDialog.ShowModal();
}

void EVENT_HANDLER_CLASS_NAME::OnShowTranslationRulesButton( wxCommandEvent & wxcmd )
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
//    wxstr += wxT("\n") + wxstrSyntaxTreePath;
    choices[ arrayIndex] = wxstrSyntaxTreePath;
    const ConditionsAndTranslation conditionsandtranslation = c_iter->second;
    std::vector<Condition>::const_iterator conditions_iter =
      conditionsandtranslation.m_conditions.begin();
//    while( conditions_iter != conditionsandtranslation.m_conditions.end() )
//    {
//      wxstr += conditions_iter->m_syntaxtreepath.GetAs_std_string() + wxT(" WHERE ")
//        + conditions_iter->m_stdstrAttributeName + "=" +
//        convertToStdString<WORD>(conditions_iter->m_byAttributeValue);
//      ++ conditions_iter;
//    }
    ++ c_iter;
    ++ arrayIndex;
  }
//  wxTextControlDialog wxd(wxstr);
//  wxd.ShowModal();
  VTrans::ShowTranslationRulesDialog showtranslationrulesdialog(this, choices,
    arraySize);
  showtranslationrulesdialog.ShowModal();
}

void EVENT_HANDLER_CLASS_NAME::OnLoadDictionaryTimerEvent(wxTimerEvent &event)
{
  //TODO show status as "bytes read"/"total bytes" :
  //  wxGetApp().m_dictionaryReader.GetCurrentPosInByte();
//  unsigned numberOfBytesRead = ::wxGetApp().m_dictionaryReader.
//    GetNumberOfBytesRead();
  SetTitle( wxString::Format( wxT("%u"), //wxGetApp().m_dictionaryFileLineNumber
//    ::wxGetApp().s_numberOfVocabularyPairs
    ::wxGetApp().s_dictReaderAndVocAccess.m_vocAccess.GetNumberOfVocPairs()

     ) );
//  SetTitle( wxString::Format(wxT("%f"), TUchemnitzDictionaryReader::m_numBytesRead ) );
}

void EVENT_HANDLER_CLASS_NAME::OnLookupWord(wxCommandEvent & wxcmd)
{
  const wxString wxstrEnglishWord = ::wxGetTextFromUser(
    wxT("search for an English word") );
  PositionStringVector psv;
  std::string std_strEnglishWord = wxWidgets::GetStdString(wxstrEnglishWord);
  psv.push_back(PositionString(std_strEnglishWord, 0,
    std_strEnglishWord.length() )
    );
  DWORD dw = 0;
  TimeCountInNanosec_type beginTimeCountInNanoSeconds, endTimeCountInNanoSeconds;
//#ifdef WIN32
//  Windows_API::GetTimeCountInNanoSeconds(beginTimeCountInNanoSeconds);
//#else
  OperatingSystem::GetTimeCountInNanoSeconds(beginTimeCountInNanoSeconds);
//#endif
  VocablesForWord::voc_container_type * p_voc_container = ::wxGetApp().
    s_dictReaderAndVocAccess.m_vocAccess.find(psv, dw);
//#ifdef WIN32
//  Windows_API::GetTimeCountInNanoSeconds(endTimeCountInNanoSeconds);
//#else
  OperatingSystem::GetTimeCountInNanoSeconds(endTimeCountInNanoSeconds);
//#endif
  const uint64_t ns = endTimeCountInNanoSeconds - beginTimeCountInNanoSeconds;
  long double seconds = ( (long double) ns)/1000000000.0;
  /** http://forums.wxwidgets.org/viewtopic.php?f=23&t=35759 :
   *  "Print long double" */
  wxString wxstrLookUpTime = wxString::Format(
    wxT(" lookup took %lu ns=%Lfus=%Lfms=%Lfs"),
    ns,
    ( (long double) ns)/1000.0,
    ( (long double) ns)/1000000.0,
    seconds );
  if( p_voc_container )
  {
    ::wxMessageBox(
      wxString::Format( wxT("# English words with name %s: %u"),
        /*std_strEnglishWord.c_str()*/ wxstrEnglishWord.c_str(),
        p_voc_container->size()) + wxstrLookUpTime
      );
  }
  else
    ::wxMessageBox(
      wxString::Format( wxT("# English words with name %s: 0"),
        wxstrEnglishWord.c_str()) + wxstrLookUpTime
      );
}

void EVENT_HANDLER_CLASS_NAME::OnTimerEvent(wxTimerEvent &event)
{
  SetTitle( wxString::Format(wxT("%u"), //wxGetApp().m_dictionaryFileLineNumber
//    ::wxGetApp().s_numberOfVocabularyPairs
    ::wxGetApp().s_dictReaderAndVocAccess.m_vocAccess.GetNumberOfVocPairs()
     ) );
  //TODO show status as "bytes read"/"total bytes" :
//  wxGetApp().m_dictionaryReader.GetCurrentPosInByte();
//  SetTitle( wxString::Format(wxT("%f"), TUchemnitzDictionaryReader::m_numBytesRead ) );
}

void EVENT_HANDLER_CLASS_NAME::OnTruncateLogFileButton( wxCommandEvent & wxcmd )
{
  g_logger.TruncateOutputSizeToZero();
//  LogEntriesDialog dlg;
  m_p_logEntriesDialog = new LogEntriesDialog(g_logger, wxGetApp().m_GUIthreadID);
//  m_logEntriesDialog.Show();
  m_p_logEntriesDialog->Show();
}

void EVENT_HANDLER_CLASS_NAME::OnUpdateAfterTranslation(wxCommandEvent &)
{
  UpdateAfterTranslation();
}

/** This method should only be called from within the GUI thread because GUI 
 *  controls are accessed. */
void EVENT_HANDLER_CLASS_NAME::UpdateAfterTranslation()
{
  if( ::wxGetApp().CurrentThreadIsGUIthread()/*GetGUIthreadNumber()*/ )
  {
    const fastestUnsignedDataType numParseTrees = ::wxGetApp().
      GetNumberOfParseTrees(//stdvec_stdvec_stdvecTranslationAndGrammarPart
      m_translationResult);
    SetTitle( wxString::Format( wxT("%u parse trees;translation in %f s"), numParseTrees, 
      (double) m_translationcontrollerbase.m_translationDurationInSeconds) );

  //  std::string stdstrAllPossibilities = GetAllTranslationPossibilites(
  //    stdvec_stdstrWholeTransl,
  //    stdvec_stdvecTranslationAndGrammarPart ) ;

  //  mp_textctrlGermanText->SetValue(stdstrAllPossibilities ) ;
    m_p_wxgermantranslationpanel->Set(//stdvec_stdvecTranslationAndGrammarPart
      /* stdvec_stdvec_stdvecTranslationAndGrammarPart */ m_translationResult);
    m_p_wxgermantranslationpanel->Create();
    /** Force redraw : sends EVT_PAINT */
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
    //You can also trigger this call by calling Refresh()/Update()
    //m_panel1->Refresh() ;
  //  }
  //  catch(/*const*/ LogFileAccessException & lfae)
  //  {
  //    const std::string std_strErrorMessage = lfae.GetErrorMessage();
  //    const wxString wxstrMessage = wxWidgets::getwxString(std_strErrorMessage);
  //    wxGetApp().ShowMessage(wxstrMessage);
  //    throw lfae;
  //  }
  }
  else
  {
    wxCommandEvent wxcommand_event(UpdateAfterTranslationEvent);

    //wxcommand_event.SetString(GetwxString_Inline(cr_stdstr));
  //    AsyncMessage(cr_stdstr);
  //    QueueEvent  (       wxEvent *       event   );
    //FIXME: http://docs.wxwidgets.org/trunk/classwx_evt_handler.html#a0737c6d2cbcd5ded4b1ecdd53ed0def3
    //"[...] can't be used to post events from worker threads for the event
    //objects with wxString fields (i.e. in practice most of them) [...]"
    AddPendingEvent(wxcommand_event);
  }
}

void EVENT_HANDLER_CLASS_NAME::OnTranslateButton( wxCommandEvent & wxcmd )
{
  LOGN_DEBUG("begin")
  if( m_translateThread.IsRunning() )
  {
    m_translationcontrollerbase.Stop();
    wxMessageBox( wxT("already translating") );
  }
  else
  {
    Translate();
  }
  LOGN_DEBUG("end")
}

void EVENT_HANDLER_CLASS_NAME::OnTranslateOnTextChanges( wxCommandEvent & wxcmd )
{
  ::wxGetApp().m_GUIattributes.m_translateOnTextChanges = wxcmd.IsChecked();
  if( ::wxGetApp().m_GUIattributes.m_translateOnTextChanges )
    Connect(ID_InputText, wxEVT_COMMAND_TEXT_UPDATED //wxEventType eventType, 
    , wxTextEventHandler(EVENT_HANDLER_CLASS_NAME::OnInputTextChanges) );
  else
    Disconnect(ID_InputText, wxEVT_COMMAND_TEXT_UPDATED //wxEventType eventType, 
      , wxTextEventHandler(EVENT_HANDLER_CLASS_NAME::OnInputTextChanges) );
}

void EVENT_HANDLER_CLASS_NAME::OnUnloadDictionary( wxCommandEvent & wxcmd )
{
  UnloadDictionaryShowingStatus();
}
