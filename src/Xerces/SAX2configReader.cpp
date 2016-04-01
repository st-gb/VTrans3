/*
 * SAX2configReader.cpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#include "SAX2configReader.hpp"
#include <UserInterface/I_UserInterface.hpp>
#include <Controller/TranslationControllerBase.hpp>
#include "SAX2MainConfigHandler.hpp"
#include "SAX2GrammarRuleHandler.hpp"
#include "SAX2TranslationRuleHandler.hpp"
#include "SAX2VocAttributeDefintionHandler.hpp"
#include <Xerces/XercesAttributesHelper.hpp>//ConvertXercesAttributesValue(...)

#include <Xerces/ReadXMLfile.hpp> //ReadXMLfile_Inline(...)

extern TranslationControllerBase * g_p_translationcontrollerbase;

namespace Apache
{
  namespace Xerces
  {
    SAX2configReader::SAX2configReader(
//      I_UserInterface & ui
        TranslationControllerBase & translationController
      )
      : //m_userInterface(ui)
        VTrans3::ConfigurationReader<XERCES_CPP_NAMESPACE::Attributes>( & translationController),
      m_translationController(translationController)//,
    /*m_sax2translationrulehandler(
      translationController.m_translateparsebyrisetree,//  TranslateParseByRiseTree & ,
      translationController.m_parsebyrise,// ParseByRise & ,
      translationController //I_UserInterface & r_userinterface
      ),
    m_sax2grammarrulehandler(translationController.m_parsebyrise),
    m_sax2vocattributedefinitionhandler(translationController.m_translateparsebyrisetree, 
      translationController)*/
    {

    }

    SAX2configReader::~SAX2configReader()
    {
      // TODO Auto-generated destructor stub
    }
    
    bool SAX2configReader::GetAttributeValue(
      /*XMLelementType & xmlElement,*/
      XERCES_CPP_NAMESPACE::Attributes & attribute,
      std::string & std_strXMLattributeValue, 
      const char * const attributeName)
    {
      return XercesAttributesHelper::getValue(
        attribute,
        std_strXMLattributeValue,
        attributeName
        );
    }

    void SAX2configReader::ReadGrammarRuleFile(
        const std::string & cr_stdstrFilePath )
    {
      SAX2GrammarRuleHandler sax2grammarrulehandler(m_translationController.m_parsebyrise) ;
      //  ReadViaSAX2("grammar_rules.xml", & sax2grammarrulehandler ) ;
      //Must create on heap, else the callback functions like "startElement" aren't
      //called?!
      //  SAX2GrammarRuleHandler * p_sax2grammarrulehandler = new
      //    SAX2GrammarRuleHandler( * this ) ;
    //  SAX2GrammarRuleHandler sax2grammarrulehandler( * this ) ;

      g_p_translationcontrollerbase->m_std_strCurrentConfigfile = cr_stdstrFilePath;

      std::wstring stdwstrErrorMessage ;
      // <> 0 = error
      if( //ReadViaSAX2InitAndTermXerces(
          ! Apache::Xerces::ReadXMLfileInitAndTermXerces_Inline(
      //    "grammar_rules.xml",
          cr_stdstrFilePath.c_str() ,
        //    p_sax2grammarrulehandler ,
    //      & sax2grammarrulehandler ,
          & sax2grammarrulehandler ,
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
        m_translationController.Message(stdwstrErrorMessage ) ;
      }
    }

    bool SAX2configReader::ReadMainConfigFile(
      const std::string & cr_stdstrFilePath )
    {
      std::wstring stdwstrErrorMessage ;
      /*Xerces::*/SAX2MainConfigHandler sax2mainconfighandler(
        //m_translateparsebyrisetree
        //* this
        m_translationController, *this
        ) ;
      if( //ReadViaSAX2InitAndTermXerces(
          Apache::Xerces::ReadXMLfile_Inline(
          cr_stdstrFilePath.c_str() ,
          & sax2mainconfighandler ,
          stdwstrErrorMessage
          )
        )
      {
        m_translationController.Message(L"Failed to read main config file\n:" + //cr_stdstrFilePath 
          stdwstrErrorMessage) ;
        return false;
      }
      else
      {
        LOGN("successfully read main config file " << cr_stdstrFilePath )
//        m_stdstrVocabularyFilePath = sax2mainconfighandler.
//          m_stdstrVocabularyFilePath ;
      }
      return true;
    }

    void SAX2configReader::ReadTranslationRuleFile(const char * const cr_stdstrFilePath )
    {
      LOGN(//"TranslationControllerBase::ReadTranslationRuleFile( "
        "\"" << cr_stdstrFilePath << "\")" )
      std::wstring stdwstrErrorMessage ;
      SAX2TranslationRuleHandler sax2translationRuleHandler(
        m_translationController.m_translateparsebyrisetree, //TranslateParseByRiseTree & ,
        m_translationController.m_parsebyrise, //ParseByRise & ,
        m_translationController //I_UserInterface & r_userinterface);
        , *this
        );
      sax2translationRuleHandler.m_std_strFilePath = cr_stdstrFilePath;
      // <> 0 = error
      if( //ReadViaSAX2InitAndTermXerces(
          ! Apache::Xerces::ReadXMLfileInitAndTermXerces_Inline(
          //"translation_rules.xml",
          cr_stdstrFilePath/*.c_str()*/ ,
      //    p_sax2grammarrulehandler ,
          & sax2translationRuleHandler ,
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
      SAX2VocAttributeDefinitionHandler sax2VocAttributeDefintionHandler(
        m_translationController.m_translateparsebyrisetree,
        m_translationController);
      // <> 0 = error
      if( //ReadViaSAX2InitAndTermXerces(
          ! Apache::Xerces::ReadXMLfileInitAndTermXerces_Inline(
          //"translation_rules.xml",
          cr_stdstrFilePath.c_str() ,
      //    p_sax2grammarrulehandler ,
          & sax2VocAttributeDefintionHandler ,
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
        m_translationController.Message(stdwstrErrorMessage ) ;
      }
    }
  } /* namespace Xerces */
} /* namespace Apache */
