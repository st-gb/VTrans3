/*
 * TranslationControllerBase.cpp
 *
 *  Created on: Dec 6, 2010
 *      Author: Stefan
 */
#include <Controller/character_string/stdtstr.hpp> //GetStdString_Inline(...)
#include <Controller/TranslationControllerBase.hpp>
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
  // TODO Auto-generated constructor stub

}

TranslationControllerBase::~TranslationControllerBase()
{
  // TODO Auto-generated destructor stub
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
