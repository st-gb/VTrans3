/*
 * TranslationControllerBase.cpp
 *
 *  Created on: Dec 6, 2010
 *      Author: Stefan
 */
#include <Controller/character_string/stdtstr.hpp> //GetStdString_Inline(...)
#include <Controller/TranslationControllerBase.hpp>
#include <Controller/time/GetTickCount.hpp>
//GenerateXMLtreeFromParseTree(...)
#include <IO/GenerateXMLtreeFromParseTree.hpp>
#include <IO/dictionary/VTransDictFormatReader.hpp> //class OneLinePerWordPair
////class TUchemnitzDictionaryReader
//#include <IO/dictionary/TUchemnitzDictionaryReader.hpp>
//class TUchemnitzDictEngWord2LineNumberReader
#include <IO/dictionary/TUchemnitzDictEngWord2LineNumberReader.hpp>
#include <InputOutput/XML/OutputXMLindented.hpp> //OutputXMLindented(...)
//class ParseTreeTraverser::TransformTreeTraverser
#include <Translate/TransformTreeTraverser.hpp>
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
#include <Xerces/ReadXMLfile.hpp> //ReadXMLfile_Inline(...)
//Class SAX2GrammarRuleHandler
#include <Xerces/SAX2GrammarRuleHandler.hpp>
#include <Xerces/SAX2MainConfigHandler.hpp> //class SAX2MainConfigHandler
//Class SAX2TranslationRuleHandler
#include <Xerces/SAX2TranslationRuleHandler.hpp>
//Class SAX2TranslationRuleHandler
#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>

//Static variables need also to be defined in 1 source file.
I_UserInterface * SyntaxTreePath::sp_userinterface ;
I_UserInterface * VocabularyAndTranslation::s_p_userinterface;
//LetterTree VTransApp::s_dictionary ;
/*LetterTree*/dictionary_type TranslationControllerBase::s_dictionary
  //(::wxGetApp(), NULL)
  ;

//#if defined __cplusplus
//extern "C" {
//#endif

void MakeFemale(std::string & r_stdstr )
{
  //e.g. "klein" E
  r_stdstr += "e";
}

void MakeMale(std::string & r_stdstr )
{
  //e.g. "klein" "ER"
  r_stdstr += "er";
}

/** Make German genus? from positive e.g. "klein" "ES"
 * @param r_stdstr: word stem/ positive form of an adverb */
void MakeNeutral(std::string & r_stdstr )
{
  //e.g. "klein" "ES"
  r_stdstr += "es";
}

/** Make 1st person singular */
void Make1stPersonSing(std::string & //r_std_strWordStem
  r_std_strInfinitive)
{
  std::string std_strWordStem;
  GermanVerb::GetWordStem( r_std_strInfinitive.c_str(),
    r_std_strInfinitive.length(), std_strWordStem );
  //e.g. "arbeit" "E"
  r_std_strInfinitive = GermanVerb::GetPresentFiniteForm(std_strWordStem,
    (enum GermanVerb::person_indices) (GermanVerb::firstPersonSing ) );
}

/** Make German past participle from infinitive.
 *   sehen->GEsehen  ansehen->anGEsehen
 *   speichern->GEspeicherT schiCKen->GEschickT tauSCHen -> GEtauschT
 *   klauen->GEklauT
 *   reiten –> geritten
 *   */
void MakePastParticiple(std::string & //r_std_strWordStem
  r_std_strInfinitive)
{
  //ending consonant + "en" -> "ge" + infinitive
  //if begin = preposition ("an", "zu" etc.): preposition + "ge" + inf.
  std::string std_strWordStem;
  GermanVerb::GetWordStem( r_std_strInfinitive.c_str(),
    r_std_strInfinitive.length(), std_strWordStem );
  //e.g. "arbeit" "E"
  r_std_strInfinitive = GermanVerb::GetPastFiniteForm(
    std_strWordStem.c_str(), std_strWordStem.length(),
    (enum GermanVerb::person_indices) (GermanVerb::firstPersonSing ) );
}

//#if defined __cplusplus
//}
//#endif

TranslationControllerBase::TranslationControllerBase()
  :
//  m_parsebyrise( * this ) ,
  m_nodetrie_ui32GrammarPartName2colour(256, 0),
  m_translateparsebyrisetree(
    m_parsebyrise
    , * this
    )
{
  VocabularyAndTranslation::s_p_translationControllerBase = this;
  /** Can be used for executing GUI operations: if a GUI control action (e.g.
   *  showing a dialog) should be performed then the caller's thread can be
   *  compared to this thread ID and only if it is the same the GUI action is
   *  executed.*/
  m_GUIthreadID = OperatingSystem::GetCurrentThreadNumber();
//  m_nodetrie_ui32GrammarPartName2colour.Create(256);
//  OneLinePerWordPair::s_p_userinterface = this;

  std::string std_str = "makeFemale";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    std::make_pair(std_str, //(pfnTransformString)
    & MakeFemale) );
  std_str = "makeMale";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    //"makeFemale"
    std::make_pair(std_str, & MakeMale) );
  std_str = "makeNeutral";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    //"makeFemale",
    std::make_pair(std_str, & MakeNeutral) );
  std_str = "make1stPersonSing";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    std::make_pair(std_str, & Make1stPersonSing) );
  std_str = "makePastParticiple";
  ConditionsAndTranslation::s_std_mapFunctionName2Function.insert(
    std::make_pair(std_str, & Make1stPersonSing) );
}

TranslationControllerBase::~TranslationControllerBase()
{
  // TODO Auto-generated destructor stub
  s_dictionary.clear(); //DeleteCompleteList();
}

BYTE TranslationControllerBase::Init(const std::string & cr_stdstrFilePath)
{
//  TransformationRule transformationrule ;
//  transformationrule.m_stdstrParseTreePathWhereToInsert = "clauseWith1Obj.obj" ;
//  transformationrule.m_bInsertLeftChild = false ;
////  m_stdmap_stdstrTransformationRule2transformationrule.insert(
////    std::make_pair( "clauseWith1Obj.*.adverb" , transformationrule
////      )
////    ) ;
//  std::string stdstr = "clauseWith1Obj.*.adverb" ;
//  m_stdmap_syntaxtreepath2transformationrule.insert(
//    std::make_pair(
//      SyntaxTreePath( stdstr , & m_parsebyrise ) ,
//      transformationrule
//      )
//    ) ;
  VocabularyAndTranslation::s_p_userinterface = this;
  SyntaxTreePath::sp_userinterface = this ;
  s_dictionary.SetUserInterface(this);
  s_dictionary.InsertFundamentalWords() ;
  if( ReadMainConfigFile(cr_stdstrFilePath) )
  {
    m_std_strMainConfigFilePath = cr_stdstrFilePath;
    if( m_stdstrVocabularyFilePath.empty() )
    {
      Message( "error: The vocabulary file path is empty") ;
      return TranslationControllerBaseClass::InitFunction::
        vocabularyFilePathIsEmpty;
    }
    else
    {
  //    OneLinePerWordPair::s_p_lettertree = & s_dictionary ;
//      TUchemnitzDictionaryReader::s_p_vocinmainmem = & s_dictionary ;

//      DictionaryReader dictReader;
  //    if( OneLinePerWordPair::LoadWords( //pWordNodeCurrent
  //          m_stdstrVocabularyFilePath
  //        )
  //      )
  //    {
  //      //Return true to continue to run the (main loop of ) this program.
  ////        return true ;
  //    }
  //    else
  //    {
  //      LoadingVocabularyFileFailed(m_stdstrVocabularyFilePath);
  //      return TranslationControllerBaseClass::InitFunction::
  //        loadingVocabularyFileFailed;
  //    }
      CreateAndShowMainWindow() ;
  #ifndef COMPILE_AS_EXECUTABLE
      //      TUchemnitzDictionaryReader tcdr(* this, s_dictionary );
      DictionaryReader::TUchemnitzDictEngWord2LineNumberReader dictReader(
        * this, & s_dictionary);
//      TUchemnitzDictEngWord1stReader & dictReader = (* this, s_dictionary);
  //    StartTimer();
      bool b = /*TUchemnitzDictionaryReader::*/ //tcdr.extractVocables(
        dictReader.read(
//      bool b = dictReader::read(
        m_stdstrVocabularyFilePath.c_str() );
      LOGN_INFO( "# of vocable pairs:" << s_numberOfVocabularyPairs )
      if( ! b )
        return TranslationControllerBaseClass::InitFunction::loadingVocabularyFileFailed;
  #endif //#ifndef COMPILE_AS_EXECUTABLE
  ////    StartTimer();
    }
  }
  else
    return TranslationControllerBaseClass::InitFunction::loadingMainConfigFileFailed;
  return TranslationControllerBaseClass::InitFunction::success;
}

void TranslationControllerBase::ReadGrammarRuleFile(
  SAX2GrammarRuleHandler & r_sax2grammarrulehandler ,
  const std::string & cr_stdstrFilePath
  )
{
  //  SAX2GrammarRuleHandler sax2grammarrulehandler(*this) ;
  //  ReadViaSAX2("grammar_rules.xml", & sax2grammarrulehandler ) ;
  //Must create on heap, else the callback functions like "startElement" aren't
  //called?!
  //  SAX2GrammarRuleHandler * p_sax2grammarrulehandler = new
  //    SAX2GrammarRuleHandler( * this ) ;
//  SAX2GrammarRuleHandler sax2grammarrulehandler( * this ) ;

  std::wstring stdwstrErrorMessage ;
  // <> 0 = error
  if( //ReadViaSAX2InitAndTermXerces(
      ! ReadXMLfileInitAndTermXerces_Inline(
  //    "grammar_rules.xml",
      cr_stdstrFilePath.c_str() ,
    //    p_sax2grammarrulehandler ,
//      & sax2grammarrulehandler ,
      & r_sax2grammarrulehandler ,
      stdwstrErrorMessage
      )
    )
    LOGN("Reading xml file " << cr_stdstrFilePath << "succeeded.")
  else
  {
    LOGN_ERROR("Reading xml file " << cr_stdstrFilePath << "failed:"
      << GetStdString_Inline(stdwstrErrorMessage) << "." )
  //    LOGWN_WSPRINTF( "%ls", stdwstrErrorMessage.c_str() )
    //::wxGetApp().Message( stdwstrErrorMessage ) ;
  //    mr_userinterface.Message( stdwstrErrorMessage ) ;
  //      Message( stdwstrErrorMessage ) ;
//    mp_translateparsebyrisetree->mr_i_userinterface.Message(
    m_translateparsebyrisetree.mr_i_userinterface.Message(
      stdwstrErrorMessage ) ;
  }
}

bool TranslationControllerBase::ReadMainConfigFile(
  const std::string & cr_stdstrFilePath )
{
  std::wstring stdwstrErrorMessage ;
  Xerces::SAX2MainConfigHandler sax2mainconfighandler(
    //m_translateparsebyrisetree
    * this
    ) ;
  if( //ReadViaSAX2InitAndTermXerces(
      ReadXMLfile_Inline(
      cr_stdstrFilePath.c_str() ,
      & sax2mainconfighandler ,
      stdwstrErrorMessage
      )
    )
  {
    Message("Failed to read main config file" + cr_stdstrFilePath ) ;
    return false;
  }
  else
  {
    LOGN("successfully read main config file " << cr_stdstrFilePath )
    m_stdstrVocabularyFilePath = sax2mainconfighandler.
      m_stdstrVocabularyFilePath ;
  }
  return true;
}

void TranslationControllerBase::ReadTranslationRuleFile(
  SAX2TranslationRuleHandler & r_sax2translationrulehandler ,
  const std::string & cr_stdstrFilePath
  )
{
  LOGN(//"TranslationControllerBase::ReadTranslationRuleFile( "
    "\"" << cr_stdstrFilePath << "\")" )
  std::wstring stdwstrErrorMessage ;
  // <> 0 = error
  if( //ReadViaSAX2InitAndTermXerces(
      ! ReadXMLfileInitAndTermXerces_Inline(
      //"translation_rules.xml",
      cr_stdstrFilePath.c_str() ,
  //    p_sax2grammarrulehandler ,
      & r_sax2translationrulehandler ,
      stdwstrErrorMessage
      )
    )
  {
    LOGN("Successfully read translation rule file \"" << cr_stdstrFilePath
      << "\"" )
  //          mr_i_userinterface.Message( wstr ) ;
  }
  else
  {
    LOGN_TYPE("Failed to read translation rule file \"" << cr_stdstrFilePath
      << "\"", LogLevel::error )
    m_translateparsebyrisetree.mr_i_userinterface.Message(
      stdwstrErrorMessage ) ;
  }
}

void TranslationControllerBase::ReadVocAttributeDefinitionFile(
  Xerces::SAX2VocAttributeDefinitionHandler &
    r_sax2vocattributedefinitionhandler ,
  const std::string & cr_stdstrFilePath
  )
{
  LOGN( "\"" << cr_stdstrFilePath << "\")" )
  std::wstring stdwstrErrorMessage ;
  // <> 0 = error
  if( //ReadViaSAX2InitAndTermXerces(
      ! ReadXMLfileInitAndTermXerces_Inline(
      //"translation_rules.xml",
      cr_stdstrFilePath.c_str() ,
  //    p_sax2grammarrulehandler ,
      & r_sax2vocattributedefinitionhandler ,
      stdwstrErrorMessage
      )
    )
  {
    LOGN("Successfully read VocAttributeDefinition XML file \""
      << cr_stdstrFilePath << "\"" )
  //          mr_i_userinterface.Message( wstr ) ;
  }
  else
  {
    LOGN_ERROR("Failed to read translation rule file \"" << cr_stdstrFilePath
      << "\"" )
    m_translateparsebyrisetree.mr_i_userinterface.Message(
      stdwstrErrorMessage ) ;
  }
}

void TranslationControllerBase::ReadXMLfile(
  XERCES_CPP_NAMESPACE::DefaultHandler & r_xercesc_defaulthandler ,
  const std::string & cr_stdstrFilePath
  )
{
  LOGN("\"" << cr_stdstrFilePath << "\")" )
  std::wstring stdwstrErrorMessage ;
  // <> 0 = error
  if( //ReadViaSAX2InitAndTermXerces(
      ! ReadXMLfileInitAndTermXerces_Inline(
      //"translation_rules.xml",
      cr_stdstrFilePath.c_str() ,
  //    p_sax2grammarrulehandler ,
      & r_xercesc_defaulthandler ,
      stdwstrErrorMessage
      )
    )
  {
    LOGN("Successfully read XML file \"" << cr_stdstrFilePath
      << "\"" )
  //          mr_i_userinterface.Message( wstr ) ;
  }
  else
  {
    LOGN_ERROR("Failed to read XML file \"" << cr_stdstrFilePath << "\"" )
    m_translateparsebyrisetree.mr_i_userinterface.Message(
      stdwstrErrorMessage ) ;
  }
}

void TranslationControllerBase::Transform()
{
  LOGN(//"TranslationControllerBase::Transform() "
      "begin")
  DWORD dwLeftMostTokenIndex = 0 ;
  std::vector<GrammarPart *> stdvec_p_grammarpartRootNode ;
  m_parsebyrise.GetGrammarPartCoveringMostTokens(
    dwLeftMostTokenIndex ,
    stdvec_p_grammarpartRootNode
    ) ;
  for( std::vector<GrammarPart *>::const_iterator c_iter_p_grammarpartRootNode
    = stdvec_p_grammarpartRootNode.begin() ; c_iter_p_grammarpartRootNode <
      stdvec_p_grammarpartRootNode.end() ; ++ c_iter_p_grammarpartRootNode
     )
  {
    ParseTreeTraverser::TransformTreeTraverser transformtreetransverser(
//      m_stdmap_stdstrTransformationRule2transformationrule ,
      m_stdmap_syntaxtreepath2transformationrule ,
      * c_iter_p_grammarpartRootNode ,
      m_parsebyrise
      ) ;
    transformtreetransverser.Traverse() ;
  }
  LOGN(//"TranslationControllerBase::Transform() "
    "end")
}

void TranslationControllerBase::Translate(
//  ParseByRise & r_parsebyrise ,
  const std::string & cr_stdstrWholeInputText ,
  std::vector<std::string> & r_stdvec_stdstrWholeTransl ,

//  std::vector<std::vector<TranslationAndGrammarPart> > &
//    r_stdvec_stdvecTranslationAndGrammarPart
  //A vector of sentences that begin at the same token index
  // (sentences that begin at the same token index:
  // vector of sentences that each contains a vector of words).
  std::vector <std::vector <std::vector <TranslationAndGrammarPart> > > &
    r_stdvec_stdvec_stdvecTranslationAndGrammarPart
//  , std::vector<std::vector<TranslationAndConsecutiveID> > &
//    r_stdvec_stdvecTranslationAndConsecutiveID
  )
{
  LOGN(//"TranslationControllerBase::Translate(...) "
      "begin")
//  OperatingSystem::GetCurrentTimeInNanoSeconds();
//    OperatingSystem::GetTimeCountInNanoSeconds();
  uint64_t timeCountInNanoSeconds;
  OperatingSystem::GetTimeCountInNanoSeconds(timeCountInNanoSeconds);
  m_parsebyrise.CreateParseTree(cr_stdstrWholeInputText);
//  RemoveDuplicateParseTrees();

  Transform() ;
//  TranslateParseByRiseTree translateParseByRiseTree(
//    m_parsebyrise
//    , ::wxGetApp()
//    ) ;
  TranslateParseByRiseTree & translateParseByRiseTree =
    //::wxGetApp().
    m_translateparsebyrisetree ;
  DEBUG_COUT("before translation\n")

  translateParseByRiseTree.Translate(
    m_parsebyrise,
    r_stdvec_stdstrWholeTransl ,
//    r_stdvec_stdvecTranslationAndGrammarPart
    r_stdvec_stdvec_stdvecTranslationAndGrammarPart
//    stdvec_stdvecTranslationAndConsecutiveID
    ) ;

  std::string std_strXML;
  GenerateXMLtreeFromParseTree( & m_parsebyrise, std_strXML);
  std::ostringstream std_ostringstream;
  OutputXMLindented(std_strXML.c_str(), std_ostringstream);
  LOGN("translation as indented XML:" << std_ostringstream.str())

  LOGN( /*FULL_FUNC_NAME <<*/ "generated XML data:" << std_strXML)
  LOGN(//"TranslationControllerBase::Translate(...) "
      "end")
}
