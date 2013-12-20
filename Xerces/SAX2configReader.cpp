/*
 * SAX2configReader.cpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#include "SAX2configReader.hpp"
#include <UserInterface/I_UserInterface.hpp>

#include <Xerces/ReadXMLfile.hpp> //ReadXMLfile_Inline(...)

namespace Apache
{
  namespace Xerces
  {

    SAX2configReader::SAX2configReader(
//      I_UserInterface & ui
        TranslationControllerBase & translationController
      )
      : //m_userInterface(ui)
      m_translationController(translationController),
    m_sax2translationrulehandler(
      translationController.m_translateparsebyrisetree,//  TranslateParseByRiseTree & ,
      translationController.m_parsebyrise,// ParseByRise & ,
      translationController //I_UserInterface & r_userinterface
      )
    {

    }

    SAX2configReader::~SAX2configReader()
    {
      // TODO Auto-generated destructor stub
    }

    void SAX2configReader::ReadGrammarRuleFile(
        const std::string & cr_stdstrFilePath )
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
          ! Apache::Xerces::ReadXMLfileInitAndTermXerces_Inline(
      //    "grammar_rules.xml",
          cr_stdstrFilePath.c_str() ,
        //    p_sax2grammarrulehandler ,
    //      & sax2grammarrulehandler ,
          & m_sax2grammarrulehandler ,
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
        m_userInterface.Message(stdwstrErrorMessage ) ;
      }
    }

    bool SAX2configReader::ReadMainConfigFile(
      const std::string & cr_stdstrFilePath )
    {
      std::wstring stdwstrErrorMessage ;
      Xerces::SAX2MainConfigHandler sax2mainconfighandler(
        //m_translateparsebyrisetree
        //* this
        m_translationController
        ) ;
      if( //ReadViaSAX2InitAndTermXerces(
          Apache::Xerces::ReadXMLfile_Inline(
          cr_stdstrFilePath.c_str() ,
          & sax2mainconfighandler ,
          stdwstrErrorMessage
          )
        )
      {
        m_translationController.Message("Failed to read main config file" + cr_stdstrFilePath ) ;
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

    void SAX2configReader::ReadTranslationRuleFile(const char * const cr_stdstrFilePath )
    {
      LOGN(//"TranslationControllerBase::ReadTranslationRuleFile( "
        "\"" << cr_stdstrFilePath << "\")" )
      std::wstring stdwstrErrorMessage ;
      // <> 0 = error
      if( //ReadViaSAX2InitAndTermXerces(
          ! Apache::Xerces::ReadXMLfileInitAndTermXerces_Inline(
          //"translation_rules.xml",
          cr_stdstrFilePath.c_str() ,
      //    p_sax2grammarrulehandler ,
          & m_sax2translationrulehandler ,
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
        LOGN_ERROR("Failed to read translation rule file \"" << cr_stdstrFilePath
          << "\"" /*, LogLevel::error*/ )
        m_translationController.Message(stdwstrErrorMessage ) ;
      }
    }

    void SAX2configReader::ReadVocAttributeDefinitionFile(
        const std::string & cr_stdstrFilePath)
    {
      LOGN( "\"" << cr_stdstrFilePath << "\")" )
      std::wstring stdwstrErrorMessage ;
      // <> 0 = error
      if( //ReadViaSAX2InitAndTermXerces(
          ! Apache::Xerces::ReadXMLfileInitAndTermXerces_Inline(
          //"translation_rules.xml",
          cr_stdstrFilePath.c_str() ,
      //    p_sax2grammarrulehandler ,
          & m_sax2vocattributedefinitionhandler ,
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
        m_userInterface.Message(stdwstrErrorMessage ) ;
      }
    }
  } /* namespace Xerces */
} /* namespace Apache */
