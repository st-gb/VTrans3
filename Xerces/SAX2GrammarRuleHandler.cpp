////#ifdef WIN32 //with Linux I can't compile with xerces yet.
//#ifdef _WINDOWS //with Linux I can't compile with xerces yet.
//  #define COMPILE_WITH_XERCES
//#endif

//#ifdef COMPILE_WITH_XERCES
  #include "../stdafx.h"
//  #include "../global.h" //for if "COMPILE_WITH_XERCES" is defined or not

  //If not included: compiler error "C1010".
  #include "SAX2GrammarRuleHandler.hpp"
  #include <Xerces/XercesAttributesHelper.hpp>
#include <Parse/ParseByRise.hpp>
//  #include "XercesHelper.hpp" //for GetAttributeValue(...)

  #include <xercesc/sax2/Attributes.hpp>
  #include <xercesc/util/xmlstring.hpp> //for XMLString::transcode(...)
  //#include "global.h" //for DEBUG(...) etc.
//  #include <ModelData/ServiceAttributes.hpp>
//  #include <ModelData/ModelData.hpp>
//  #include <UserInterface/UserInterface.hpp>
  #include <string>
  #include <iostream>
//  #include "../global.h"
	using namespace std;
	
	//#define MB_CUR_MAX 1 
	
	//With Xerces < Version 2.8:
	//  Add "XML_LIBRARY" to "Preprocessor Definitions" to compile with Xerces statically (else many "LNK2001" and "LNK2019" and linker errors).
	//with Xerces 3.0: "XERCES_STATIC_LIBRARY"
	//And: Add "Advapi32.lib" as additional dependancy for the linker.
	//zu Linker->Bibliothek ignorieren hinzufgen:"LIBCMT.lib", sonst: "LIBCMT.lib(_ctype.obj) : error LNK2005: _isspace ist bereits in MSVCRT.lib(MSVCR80.dll) definiert."
	
	XERCES_CPP_NAMESPACE_USE //to NOT need to prefix the xerces classes with the "xerces::"
	
	// need to properly scope any forward declarations
	XERCES_CPP_NAMESPACE_BEGIN
	  class Attributes;
	XERCES_CPP_NAMESPACE_END
	
	SAX2GrammarRuleHandler::SAX2GrammarRuleHandler(//PStates & pstates
//	  Model & model ,
//	  UserInterface * p_userinterface //,
    //I_CPUcontroller * p_cpucontroller
	  ParseByRise & r_parsebyrise
	  )
    : mr_parsebyrise( r_parsebyrise )
	{
	  //m_p_pstates = & pstates ;
//	  m_p_model = & model ;
//	  m_p_userinterface = p_userinterface ;
    //p_cpucontroller = p_cpucontroller ;
	}

  void SAX2GrammarRuleHandler::startElement
    (
    const   XMLCh * const    uri,
    const   XMLCh * const    cpc_xmlchLocalName,
    const   XMLCh * const    qname,
    const   XERCES_CPP_NAMESPACE::Attributes & attrs
//      XercesAttributesHelper & attrs
	  )
	{
	  char * pchXMLelementName = XMLString::transcode(cpc_xmlchLocalName);
	  if( pchXMLelementName )
	  {
      std::wstring stdwstrValue ;
//      LOG( "XML element: " << pchXMLelementName << endl );
      m_strElementName = std::string(pchXMLelementName) ;
      //LOG( "uri:" << uri << " localname:" << localname << " qname:" << qname << endl );
      if( m_strElementName == "grammar_rule" )
      {
  //	    xercesc_3_1:: Get
        std::string stdstrLeftChild, stdstrRightChild , stdstrSuperordinate;
//        int i ;
        if(
//            XercesHelper::GetAttributeValue
//            (
//              attrs,//"processor_name"
//  //            archAttributeName ,
//              stdstrAttributeName.c_str() ,
//              stdwstrValue
//            )
            //attrs.getValue<int>( //stdstr
//            XercesAttributesHelper::getValue<int>(
//            ConvertXercesAttributesValue<int>(
            XercesAttributesHelper::getValue(
              attrs ,
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
              attrs ,
              stdstrSuperordinate ,
              "superordinate"
              )
          )
        {
          if(
            XercesAttributesHelper::getValue(
              attrs ,
              stdstrRightChild ,
              "right_child"
              )
            && stdstrRightChild != ""
            )
//          m_p_model->m_serviceattributes.m_stdwstrGUICurrentDirFullPathTo =
//            stdwstrValue ;
//          LOGWN_WSPRINTF(L"set current dir for GUI to %ls" ,
//            //stdwstrValue.c_str()
//            m_p_model->m_serviceattributes.m_stdwstrGUICurrentDirFullPathTo.
//            c_str()
//            )
            mr_parsebyrise.InsertGrammarRule(
              stdstrLeftChild.c_str() //const char * cp_chLeftGrammarRuleName
              , stdstrRightChild.c_str() //const char * cp_chRightGrammarRuleName
              , //std::string
              stdstrSuperordinate.c_str() //const char * cp_chSuperordinateGrammarRuleName
              ) ;
          else
            mr_parsebyrise.InsertSuperClassGrammarRule(
              stdstrLeftChild.c_str() , //cp_chSubclassGrammarRuleName
              stdstrSuperordinate.c_str() //cp_chSuperclassGrammarRuleName
              ) ;
        }
//        if(
////            XercesHelper::GetAttributeValue
////            (
////                attrs,//"processor_name"
//////                archAttributeName ,
////                stdstrAttributeName.c_str() ,
////                stdwstrValue
////            )
////            attrs.getValue<std::string>( stdstr ,"right child")
//            ConvertXercesAttributesValue<std::string>(
//                attrs ,
//                stdstr ,
//                "right child"
//                )
//          )
//        {
////          m_p_model->m_serviceattributes.m_stdwstrPathToGUIexe =
////            stdwstrValue ;
////          LOGWN_WSPRINTF(L"set path to GUI exe to %ls" , //stdwstrValue.c_str()
////            m_p_model->m_serviceattributes.m_stdwstrPathToGUIexe.c_str() )
//        }
      }
  //    else if( m_strElementName == "path_to_GUI_exe" )
  //    {
  //
  //    }
      //Release memory AFTER comparing.
      XMLString::release(&pchXMLelementName);
	  }
	}
	
	void SAX2GrammarRuleHandler::fatalError(const SAXParseException& exception)
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
