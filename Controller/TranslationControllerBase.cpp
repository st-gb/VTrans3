/*
 * TranslationControllerBase.cpp
 *
 *  Created on: Dec 6, 2010
 *      Author: Stefan
 */

#include <Controller/TranslationControllerBase.hpp>
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
#include <Xerces/ReadXMLfile.hpp> //ReadXMLfile_Inline(...)
//Class SAX2TranslationRuleHandler
#include <Xerces/SAX2TranslationRuleHandler.hpp>

TranslationControllerBase::TranslationControllerBase()
  :
  m_parsebyrise( * this ) ,
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
