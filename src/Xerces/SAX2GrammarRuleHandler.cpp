//#ifdef COMPILE_WITH_XERCES
//  #include "../stdafx.h"
//  #include "../global.h" //for if "COMPILE_WITH_XERCES" is defined or not

  //If not included: compiler error "C1010".
  #include "SAX2GrammarRuleHandler.hpp"
  #include <Controller/TranslationControllerBase.hpp>
  //for GetStdWstring(const std::string & )
  #include <Controller/character_string/stdtstr.hpp>
  //for convertToStdString(...)
  #include <Controller/character_string/stdstring_format.hpp>
  //for I_UserInterface::Message(const std::string &)
  #include <UserInterface/I_UserInterface.hpp>
  #include <Xerces/XercesAttributesHelper.hpp>
  //for Xerces::ConvertXercesStringToStdWstring(const XMLCh *)
  #include <Xerces/XercesString.hpp>
  #include <Parse/ParseByRise.hpp>
//  #include "XercesHelper.hpp" //for GetAttributeValue(...)

  #include <xercesc/sax/Locator.hpp> //class XERCES_CPP_NAMESPACE::Locator
  #include <xercesc/sax2/Attributes.hpp>
  #include <xercesc/util/XMLString.hpp> //for XMLString::transcode(...)
  #include <string>
  #include <iostream>
//  #include "../global.h"
	using namespace std;
  
	extern TranslationControllerBase * g_p_translationcontrollerbase;

	//#define MB_CUR_MAX 1 
	
	/** With Xerces < Version 2.8:
	*  Add "XML_LIBRARY" to "Preprocessor Definitions" to compile with Xerces
	*  statically (else in MSVC many "LNK2001" and "LNK2019" and linker errors).
	* with Xerces 3.0: "XERCES_STATIC_LIBRARY"
	* And: Add "Advapi32.lib" as additional dependancy for the linker.
	* zu Linker->Bibliothek ignorieren hinzufgen:"LIBCMT.lib", sonst:
	* "LIBCMT.lib(_ctype.obj) : error LNK2005: _isspace ist bereits in
	* MSVCRT.lib(MSVCR80.dll) definiert." */
	
	//XERCES_CPP_NAMESPACE_USE //to NOT need to prefix the xerces classes with the "xerces::"
	
	//need to properly scope any forward declarations
	XERCES_CPP_NAMESPACE_BEGIN
	  class Attributes;
	XERCES_CPP_NAMESPACE_END
	
	SAX2GrammarRuleHandler::SAX2GrammarRuleHandler(
//	  Model & model ,
//	  UserInterface * p_userinterface //,
	  ParseByRise & r_parsebyrise
	  )
    : mr_parsebyrise( r_parsebyrise )
	{
//	  m_p_model = & model ;
//	  m_p_userinterface = p_userinterface ;
	}

  void SAX2GrammarRuleHandler::InsertGrammarRule(
    const std::string & stdstrLeftChild,
    const std::string & stdstrRightChild,
    const std::string & stdstrSuperordinate
    )
  {
    enum ParseByRise::InsertGrammarRuleReturnCodes insertGrammarRuleReturnCode
      = mr_parsebyrise.InsertGrammarRule(
      stdstrLeftChild.c_str() //const char * cp_chLeftGrammarRuleName
      , stdstrRightChild.c_str() //const char * cp_chRightGrammarRuleName
      , //std::string
      stdstrSuperordinate.c_str() //const char * cp_chSuperordinateGrammarRuleName
      ) ;
    std::wstring std_wstrMessage;
    if( insertGrammarRuleReturnCode != ParseByRise::AllGrammarPartsAreKnown )
    {
      std_wstrMessage = L"In document \n\"" +
        Xerces::ConvertXercesStringToStdWstring(m_pc_locator->
          getSystemId() ) +
        L"\"\n"
        L" line:"
        + GetStdWstring( convertToStdString(m_pc_locator->
            getLineNumber() ) )
        + L" column:"
        + GetStdWstring( convertToStdString(m_pc_locator->
            getColumnNumber() ) ) +
        L" : grammar rule \"" + GetStdWstring(stdstrSuperordinate)
        + L"\" was not added because: ";
      std::string insertGrammarRuleErrMsg = mr_parsebyrise.GetErrorMessage(
        insertGrammarRuleReturnCode);
      mr_parsebyrise.m_p_userinterface->Message( std_wstrMessage +
        GetStdWstring(insertGrammarRuleErrMsg) );
    }
  }

  void SAX2GrammarRuleHandler::startElement
    (
    const XMLCh * const cpc_xmlchURI,
    const XMLCh * const cpc_xmlchLocalName,
    const XMLCh * const cpc_xmlchQualifiedName,
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
//      XercesAttributesHelper & attrs
	  )
	{
	  char * pchXMLelementName = XERCES_CPP_NAMESPACE::XMLString::transcode(
	    cpc_xmlchLocalName);
	  if( pchXMLelementName )
	  {
      std::wstring stdwstrValue ;
//      LOG( "XML element: " << pchXMLelementName << endl );
      m_strElementName = std::string(pchXMLelementName) ;
      //LOG( "uri:" << uri << " localname:" << localname << " qname:" <<
      // qname << endl );
      if( m_strElementName == "grammar_rule" )
      {
  //	    xercesc_3_1:: Get
        std::string stdstrLeftChild, stdstrRightChild , stdstrSuperordinate;
//        int i ;
        if(
            //attrs.getValue<int>( //stdstr
//            XercesAttributesHelper::getValue<int>(
//            ConvertXercesAttributesValue<int>(
            XercesAttributesHelper::getValue(
              cr_xercesc_attributes ,
              stdstrLeftChild ,
//              std::string(
              "left_child"
//               )
//              cpc_xmlchLocalName
              )
//            &&
//            XercesAttributesHelper::getValue(
//              attrs ,
//              stdstrRightChild ,
//              "right_child"
//              )
            &&
            XercesAttributesHelper::getValue(
              cr_xercesc_attributes ,
              stdstrSuperordinate ,
              "superordinate"
              )
          )
        {
#ifdef COMPILE_AS_EXECUTABLE
          g_p_translationcontrollerbase->m_std_map_grammarRuleName2filepath.
            insert(std::make_pair(std::string(stdstrSuperordinate),
            g_p_translationcontrollerbase->m_std_strCurrentConfigfile) );
#endif
          if(
              XercesAttributesHelper::getValue(
                cr_xercesc_attributes ,
                stdstrRightChild ,
                "right_child"
                )
              && stdstrRightChild != ""
            )
          {
            InsertGrammarRule(stdstrLeftChild, stdstrRightChild,
              stdstrSuperordinate);
          }
          else
          {
            enum ParseByRise::InsertGrammarRuleReturnCodes
              insertGrammarRuleReturnCode = mr_parsebyrise.
              InsertSuperClassGrammarRule(
                stdstrLeftChild.c_str() , //cp_chSubclassGrammarRuleName
                stdstrSuperordinate.c_str() //cp_chSuperclassGrammarRuleName
              ) ;
            if( insertGrammarRuleReturnCode != ParseByRise::AllGrammarPartsAreKnown )
            {
              std::wstring std_wstrMessage = L"In document \n\"" +
                Xerces::ConvertXercesStringToStdWstring(m_pc_locator->
                  getSystemId() ) +
                L"\"\n"
                L" line:"
                + GetStdWstring( convertToStdString(m_pc_locator->
                    getLineNumber() ) )
                + L" column:"
                + GetStdWstring( convertToStdString(m_pc_locator->
                    getColumnNumber() ) ) +
                L" : superclass grammar rule \"" +
                  GetStdWstring(stdstrSuperordinate)
              + L"\"was not added because: unknown grammar part \n"
                + GetStdWstring(stdstrLeftChild) + L"\""
                ;
              //TODO should use mr_parsebyrise.GetErrorMessage(
              //  insertGrammarRuleReturnCode);??
              mr_parsebyrise.m_p_userinterface->Message(std_wstrMessage);
            }
          }
        }
//        if(
////            attrs.getValue<std::string>( stdstr ,"right child")
//            ConvertXercesAttributesValue<std::string>(
//                attrs ,
//                stdstr ,
//                "right child"
//                )
//          )
//        {
//        }
      }
      //Release memory AFTER comparing.
      XERCES_CPP_NAMESPACE::XMLString::release( & pchXMLelementName );
	  }
	}
	
  void SAX2GrammarRuleHandler::setDocumentLocator(
    const XERCES_CPP_NAMESPACE::Locator * const cpc_locator )
  {
    m_pc_locator = cpc_locator ;
  }

	void SAX2GrammarRuleHandler::fatalError(
	  const XERCES_CPP_NAMESPACE::SAXParseException & exception )
	{
//	    char* message = XMLString::transcode(exception.getMessage());
      //DEBUG_COUT( << "SAX2 handler: Fatal Error: " << message
	     //    << " at line: " << exception.getLineNumber()
	     //    << endl ) ;
//      LOG( "SAX2 handler: Fatal Error: " << message
//         << " at line: " << exception.getLineNumber()
//         << endl ) ;
	}
//#endif //#ifdef COMPILE_WITH_XERCES
