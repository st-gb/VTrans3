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
#include <Translate/TranslationRule.hpp> //class TranslationRule

//#include <Xerces/SAX2GrammarRuleHandler.hpp>
//#include <Xerces/SAX2TransformationRuleHandler.hpp>
//#include <Xerces/SAX2TranslationRuleHandler.hpp>
//#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>
#include <Controller/ConfigurationHandler_type.hpp>

#include <wxWidgets/UserInterface/TranslationRules/ShowTranslationRulesDialog.hpp>
#include <wxWidgets/UserInterface/UserInterface.hpp>
#include <wxWidgets/UserInterface/wxParseTreePanel.hpp>
#include <wxWidgets/UserInterface/wxGermanTranslationPanel.hpp>
#include <wxWidgets/VTransApp.hpp> //::wxGetApp()
#include <wxWidgets/UserInterface/wxTextControlDialog.hpp>
#include <Controller/thread_type.hpp> //typedef VTrans::thread_tyoe
#include "VocabularyInMainMem/VocablesForWord.hpp"

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

//  EVT_BUTTON( ID_AddGrammarRules , (EVENT_HANDLER_BASE_CLASS_NAME) ( EVENT_HANDLER_CLASS_NAME::)OnAddGrammarRules )

/** see http://docs.wxwidgets.org/trunk/classwx_tool_bar.html:
 *   if the control is a tool button, it should be "EVT_MENU" should be used,
 *   else the event handler function is not called.*/
#define BUTTON_EVENT_TYPE EVT_MENU /*EVT_BUTTON*/

BEGIN_EVENT_TABLE( EVENT_HANDLER_CLASS_NAME, EVENT_HANDLER_BASE_CLASS_NAME)
  EVT_TIMER( ID_Timer, EVENT_HANDLER_CLASS_NAME::OnTimerEvent)
  BUTTON_EVENT_TYPE( ID_AddGrammarRules , EVENT_HANDLER_CLASS_NAME::OnAddGrammarRules )
  BUTTON_EVENT_TYPE( ID_ShowGrammarPartMemoryAddress,
    EVENT_HANDLER_CLASS_NAME::OnShowGrammarPartMemoryAddress)
  BUTTON_EVENT_TYPE( ID_ShowGrammarTranslatedWord,
    EVENT_HANDLER_CLASS_NAME::OnShowTranslatedWord)
  EVT_BUTTON( ID_AddTransformationRules ,
    EVENT_HANDLER_CLASS_NAME::OnAddTransformationRules )
  EVT_BUTTON( ID_AddTranslationRules , EVENT_HANDLER_CLASS_NAME::OnAddTranslationRules )
  EVT_BUTTON( ID_AddVocAttrDefs, EVENT_HANDLER_CLASS_NAME::OnAddVocAttrDefs )
  BUTTON_EVENT_TYPE( ID_Settings , EVENT_HANDLER_CLASS_NAME::OnSettings)
  BUTTON_EVENT_TYPE( ID_Translate , EVENT_HANDLER_CLASS_NAME::OnTranslateButton)
  BUTTON_EVENT_TYPE( ID_LookupWord , EVENT_HANDLER_CLASS_NAME::OnLookupWord)
  BUTTON_EVENT_TYPE( ID_ShowDictionaryStatistics , EVENT_HANDLER_CLASS_NAME::OnShowDictionaryStatistics)
  BUTTON_EVENT_TYPE( ID_UnloadDictionary , EVENT_HANDLER_CLASS_NAME::OnUnloadDictionary)
  BUTTON_EVENT_TYPE( ID_TruncateLogFile, EVENT_HANDLER_CLASS_NAME::OnTruncateLogFileButton)
  BUTTON_EVENT_TYPE( ID_LoadDictionary , EVENT_HANDLER_CLASS_NAME::OnLoadDictionaryButton)
  EVT_BUTTON( ID_ShowTokenIndex2GrammarPart, EVENT_HANDLER_CLASS_NAME::
    OnShowTokenIndex2GrammarPartButton)
  BUTTON_EVENT_TYPE( ID_ShowTranslationRules,
    EVENT_HANDLER_CLASS_NAME::OnShowTranslationRulesButton )
  EVT_BUTTON( ID_ReInitGrammarRules ,
    EVENT_HANDLER_CLASS_NAME::OnReInitGrammarRulesButton)
  EVT_BUTTON( ID_RemoveGrammarRules ,
    EVENT_HANDLER_CLASS_NAME::OnRemoveGrammarRules )
  EVT_BUTTON( ID_RemoveTransformationRules ,
    EVENT_HANDLER_CLASS_NAME::OnRemoveTransformationRules )
  EVT_BUTTON( ID_RemoveTranslationRules ,
    EVENT_HANDLER_CLASS_NAME::OnRemoveTranslationRules )
  EVT_BUTTON( ID_RemoveVocAttrDefs ,
    EVENT_HANDLER_CLASS_NAME::OnRemoveVocAttrDefs )
  EVT_BUTTON( ID_ResolveSuperclass,
    EVENT_HANDLER_CLASS_NAME::OnResolveSuperclassGrammarParts )
  EVT_BUTTON( ID_Resolve1ParseLevel ,
    EVENT_HANDLER_CLASS_NAME::OnResolve1ParseLevelButton)
  EVT_BUTTON( ID_DrawParseTree ,
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
}

void EVENT_HANDLER_CLASS_NAME::UnloadDictionaryShowingStatus()
{
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
}

void EVENT_HANDLER_CLASS_NAME::OnClose( wxCloseEvent & wxcmd )
{
  LOGN("begin before clearing the dictionary")
  unsigned numberOfVocPairs = ::wxGetApp().s_dictReaderAndVocAccess.m_vocAccess.GetNumberOfVocPairs();
  unsigned numberOfEnglishWords = ::wxGetApp().s_dictReaderAndVocAccess.m_vocAccess.GetNumberOfEnglishWords();
  if( numberOfVocPairs /*numberOfEnglishWords*/  > 0 )
  {
    UnloadDictionaryShowingStatus();
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
  fastestUnsignedDataType numWordClassReps[EnglishWord::beyond_last_entry];
  VTransApp & vt = ::wxGetApp();
  vt.s_dictReaderAndVocAccess.m_vocAccess.GetStatistics(numWordClassReps);

  wxString wxstr = wxString::Format(
    wxT("# English words:%lu\n"
        "# nouns:%lu\n")
    , numWordClassReps[EnglishWord::noun]
   );
  wxTextControlDialog wxd(wxstr);
  wxd.ShowModal();
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
  wxString wxstrEnglishWord = ::wxGetTextFromUser(
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
  wxString wxstrLookUpTime = wxString::Format(
    wxT(" lookup took %lu ns=%fus=%fms=%fs"),
    ns,
    ( (long double) ns)/1000.0,
    ( (long double) ns)/1000000.0,
    ( (long double) ns)/1000000000.0 );
  if( p_voc_container )
  {
    ::wxMessageBox(
      wxString::Format( wxT("# English words with name %s: %u"),
        std_strEnglishWord.c_str(),
        p_voc_container->size()) + wxstrLookUpTime
      );
  }
  else
    ::wxMessageBox(
      wxString::Format( wxT("# English words with name %s: 0"),
      std_strEnglishWord.c_str()) + wxstrLookUpTime
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
}

void EVENT_HANDLER_CLASS_NAME::OnTranslateButton( wxCommandEvent & wxcmd )
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
  DEBUGN("end of OnTranslateButton\n")
  //You can also trigger this call by calling Refresh()/Update()
  //m_panel1->Refresh() ;
}

void EVENT_HANDLER_CLASS_NAME::OnUnloadDictionary( wxCommandEvent & wxcmd )
{
  UnloadDictionaryShowingStatus();
}
