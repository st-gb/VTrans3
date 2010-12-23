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
//Class SAX2TranslationRuleHandler
#include <Xerces/SAX2TranslationRuleHandler.hpp>
//Class SAX2TranslationRuleHandler
#include <Xerces/SAX2VocAttributeDefintionHandler.hpp>

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
}

void TranslationControllerBase::Init()
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

void TranslationControllerBase::Transform()
{
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
}

void TranslationControllerBase::Translate(
//  ParseByRise & r_parsebyrise ,
  const std::string & cr_stdstrWholeInputText ,
  std::vector<std::string> & r_stdvec_stdstrWholeTransl ,
  std::vector<std::vector<TranslationAndGrammarPart> > &
    r_stdvec_stdvecTranslationAndGrammarPart
//  , std::vector<std::vector<TranslationAndConsecutiveID> > &
//    r_stdvec_stdvecTranslationAndConsecutiveID
  )
{
  m_parsebyrise.ClearParseTree() ;
  m_parsebyrise.CreateInitialGrammarParts ( cr_stdstrWholeInputText ) ;
  DEBUG_COUT("before resolving GrammarRulesForAllParseLevels \n")
  m_parsebyrise.ResolveGrammarRulesForAllParseLevels() ;

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
    r_stdvec_stdvecTranslationAndGrammarPart
//    stdvec_stdvecTranslationAndConsecutiveID
    ) ;
}
