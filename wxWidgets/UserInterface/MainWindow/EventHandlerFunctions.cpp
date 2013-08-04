#include <wx/event.h> //BEGIN_EVENT_TABLE
#include "MainFrame.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h>

#define EVENT_HANDLER_BASE_CLASS_NAME wxFrame /*wxTopLevelWindowBase wxDialog*/
#define EVENT_HANDLER_CLASS_NAME_NON_SCOPED MainFrame /*wxTextInputDlg*/
#define EVENT_HANDLER_CLASS_NAMESPACE "wxWidgets::"
//#define EVENT_HANDLER_CLASS_NAME wxCONCAT(EVENT_HANDLER_CLASS_NAMESPACE,EVENT_HANDLER_CLASS_NAME_NON_SCOPED) /*wxTextInputDlg*/
#define EVENT_HANDLER_CLASS_NAME wxWidgets::MainFrame /*wxTextInputDlg*/
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


BEGIN_EVENT_TABLE( EVENT_HANDLER_CLASS_NAME, EVENT_HANDLER_BASE_CLASS_NAME)
  EVT_TIMER( ID_Timer, EVENT_HANDLER_CLASS_NAME::OnTimerEvent)
  EVT_BUTTON( ID_AddGrammarRules , (EVENT_HANDLER_BASE_CLASS_NAME) ( EVENT_HANDLER_CLASS_NAME::)OnAddGrammarRules )
  EVT_BUTTON( ID_AddTransformationRules ,
    EVENT_HANDLER_CLASS_NAME::OnAddTransformationRules )
  EVT_BUTTON( ID_AddTranslationRules , EVENT_HANDLER_CLASS_NAME::OnAddTranslationRules )
  EVT_BUTTON( ID_AddVocAttrDefs, EVENT_HANDLER_CLASS_NAME::OnAddVocAttrDefs )
  EVT_BUTTON( ID_Translate , EVENT_HANDLER_CLASS_NAME::OnTranslateButton)
  EVT_BUTTON( ID_TruncateLogFile, EVENT_HANDLER_CLASS_NAME::OnTruncateLogFileButton)
  EVT_BUTTON( ID_LoadDictionary , EVENT_HANDLER_CLASS_NAME::OnLoadDictionaryButton)
  EVT_BUTTON( ID_ShowTokenIndex2GrammarPart, EVENT_HANDLER_CLASS_NAME::
    OnShowTokenIndex2GrammarPartButton)
  EVT_BUTTON( ID_ShowTranslationRules,
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
  EVT_BUTTON( ID_Info, EVENT_HANDLER_CLASS_NAME::OnInfoButton)
  EVT_CLOSE( EVENT_HANDLER_CLASS_NAME::OnClose)
END_EVENT_TABLE()

void EVENT_HANDLER_CLASS_NAME::OnAddGrammarRules( wxCommandEvent & wxcmd )
{
//  AddTranslationRules() ;
  SAX2GrammarRuleHandler sax2grammarrulehandler(
    wxGetApp().m_parsebyrise
    ) ;
  wxString wxstrTitle( wxT("Choose at least 1 grammar rule file") ) ;
  wxGetApp().ProcessSelectedXMLfiles(
    sax2grammarrulehandler ,
    wxstrTitle,
    ::wxGetCwd()
    );
}

void EVENT_HANDLER_CLASS_NAME::OnAddTransformationRules( wxCommandEvent & wxcmd )
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

void EVENT_HANDLER_CLASS_NAME::OnAddTranslationRules( wxCommandEvent & wxcmd )
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

void EVENT_HANDLER_CLASS_NAME::OnAddVocAttrDefs( wxCommandEvent & wxcmd )
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

void EVENT_HANDLER_CLASS_NAME::OnClose( wxCloseEvent & wxcmd )
{
  LOGN("begin")
//  //Call delete to exit this application. (without it is not finished)
//  delete(this) ;
//  this->Destroy() ;
  Destroy() ;
//  Close() ;
  //::wxGetApp().ExitMainLoop() ;
  LOGN("end")
}

void EVENT_HANDLER_CLASS_NAME::OnDrawParseTreeButton(wxCommandEvent & wxcmd )
{
  mp_wxparsetreepanel->DrawParseTree(m_parsebyrise) ;
}

void EVENT_HANDLER_CLASS_NAME::OnInfoButton( wxCommandEvent & wxcmd )
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

void EVENT_HANDLER_CLASS_NAME::OnLoadDictionaryButton( wxCommandEvent & wxcmd )
{
  UnLoadAndLoadDictionary( this ) ;
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

void EVENT_HANDLER_CLASS_NAME::OnTimerEvent(wxTimerEvent &event)
{
  SetTitle( wxString::Format(wxT("%u"), wxGetApp().m_dictionaryFileLineNumber) );
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

