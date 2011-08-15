/*
 * TranslationControllerBase.cpp
 *
 *  Created on: Dec 6, 2010
 *      Author: Stefan
 */
#include <Controller/character_string/stdtstr.hpp> //GetStdString_Inline(...)
#include <Controller/TranslationControllerBase.hpp>
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
//LetterTree VTransApp::s_lettertree ;
LetterTree TranslationControllerBase::s_lettertree ;

TranslationControllerBase::TranslationControllerBase()
  :
//  m_parsebyrise( * this ) ,
  m_translateparsebyrisetree(
    m_parsebyrise
    , * this
    )
{
}

TranslationControllerBase::~TranslationControllerBase()
{
  // TODO Auto-generated destructor stub
  s_lettertree.DeleteCompleteList();
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
  SyntaxTreePath::sp_userinterface = this ;
  s_lettertree.InsertFundamentalWords() ;
  ReadMainConfigFile(cr_stdstrFilePath) ;
  if( m_stdstrVocabularyFilePath.empty() )
  {
    Message( "error: The vocabulary file path is empty") ;
    return TranslationControllerBaseClass::InitFunction::
      vocabularyFilePathIsEmpty;
  }
  else
  {
    OneLinePerWordPair::s_p_lettertree = & s_lettertree ;
    if( OneLinePerWordPair::LoadWords( //pWordNodeCurrent
         //stdstrFilePath
          m_stdstrVocabularyFilePath
        )
      )
    {
//        CreateAndShowMainWindow() ;
      //Return true to continue to run the (main loop of ) this program.
//        return true ;
    }
    else
    {
      LoadingVocabularyFileFailed(m_stdstrVocabularyFilePath);
      return TranslationControllerBaseClass::InitFunction::
        loadingVocabularyFileFailed;
    }
    CreateAndShowMainWindow() ;
  }
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
    LOGN("Reading xml file " << cr_stdstrFilePath << "failed:"
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

void TranslationControllerBase::ReadMainConfigFile(
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
  }
  else
  {
    LOGN("successfully read main config file " << cr_stdstrFilePath )
    m_stdstrVocabularyFilePath = sax2mainconfighandler.
      m_stdstrVocabularyFilePath ;
  }
}

void TranslationControllerBase::ReadTranslationRuleFile(
  SAX2TranslationRuleHandler & r_sax2translationrulehandler ,
  const std::string & cr_stdstrFilePath
  )
{
  LOGN("TranslationControllerBase::ReadTranslationRuleFile( \"" <<
    cr_stdstrFilePath << "\")" )
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
    LOGN("Failed to read translation rule file \"" << cr_stdstrFilePath
      << "\"" )
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
  LOGN("TranslationControllerBase::ReadTranslationRuleFile( \"" <<
    cr_stdstrFilePath << "\")" )
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
    LOGN("Successfully read translation rule file \"" << cr_stdstrFilePath
      << "\"" )
  //          mr_i_userinterface.Message( wstr ) ;
  }
  else
  {
    LOGN("Failed to read translation rule file \"" << cr_stdstrFilePath
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
  LOGN("TranslationControllerBase::ReadTranslationRuleFile( \"" <<
    cr_stdstrFilePath << "\")" )
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
    LOGN("Successfully read translation rule file \"" << cr_stdstrFilePath
      << "\"" )
  //          mr_i_userinterface.Message( wstr ) ;
  }
  else
  {
    LOGN("Failed to read translation rule file \"" << cr_stdstrFilePath
      << "\"" )
    m_translateparsebyrisetree.mr_i_userinterface.Message(
      stdwstrErrorMessage ) ;
  }
}

void TranslationControllerBase::Transform()
{
  LOGN("TranslationControllerBase::Transform() begin")
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
  LOGN("TranslationControllerBase::Transform() end")
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
  LOGN("TranslationControllerBase::Translate(...) begin")
  m_parsebyrise.ClearParseTree() ;
  m_parsebyrise.CreateInitialGrammarParts ( cr_stdstrWholeInputText ) ;

  DEBUG_COUT("before resolving GrammarRulesForAllParseLevels \n")
  m_parsebyrise.ResolveGrammarRulesForAllParseLevels() ;
  //TODO idea: before further processing: delete all parse tree that
  //are identical after removing the superordinate grammar rules:

  //the fan               the fan
  //  \ /               =  \  /
  // def_article_noun     def_article_noun
  //                        |
  //                      3rdPersSingEnumEle

  //So the translation would be faster, no double translations.
  //But on the other hand: if there are translation rules that include at least 1
  //of these superordinate rules, they wouldn't apply.
  m_parsebyrise.RemoveSuperordinateRulesFromRootNodes();
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
  LOGN("TranslationControllerBase::Translate(...) end")
}
