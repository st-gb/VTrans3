#ifndef _SAX2_GRAMMAR_RULE_HANDLER_HPP
#define _SAX2_GRAMMAR_RULE_HANDLER_HPP

//#ifdef COMPILE_WITH_XERCES
//class "XERCES_CPP_NAMESPACE::Attributes"
#include <xercesc/sax2/Attributes.hpp>
//class "XERCES_CPP_NAMESPACE::DefaultHandler
#include <xercesc/sax2/DefaultHandler.hpp>
//#include "../global.h" //for DEBUG(...) etc.
#include <string> //for std::string
//#include <string> //std::string

//http://xerces.apache.org/xerces-c/build-3.html:
//If you are linking your application to the static Xerces-C++ library,
//then you will need to compile your application with the
//XERCES_STATIC_LIBRARY
//preprocessor macro defined in order to turn off the DLL
//import/export mechanism.
//#define XERCES_STATIC_LIBRARY

//#define MB_CUR_MAX 1

//Forward declaration (because _this_ header file may be included very often /
//more than once) is faster than to #include the while declaration file.
class ParseByRise ;

class SAX2GrammarRuleHandler
  :
  public XERCES_CPP_NAMESPACE::DefaultHandler
  //ContentHandler
{
private:
  XERCES_CPP_NAMESPACE::Locator const * m_pc_locator ;
  ParseByRise & mr_parsebyrise ;
  std::string m_strElementName ;

public:
  SAX2GrammarRuleHandler(
    ParseByRise & r_parsebyrise
    );

  void InsertGrammarRule(
    const std::string & stdstrLeftChild,
    const std::string & stdstrRightChild,
    const std::string & stdstrSuperordinate);
  void startElement(
    const XMLCh * const cpc_xmlchURI,
    const XMLCh * const cpc_xmlchLocalName,
    const XMLCh * const cpc_xmlchQualifiedName,
    const XERCES_CPP_NAMESPACE::Attributes & cr_xercesc_attributes
//        XercesAttributesHelper &
  );
  void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException & );
  void setDocumentLocator( const XERCES_CPP_NAMESPACE::Locator *
    const cpc_locator ) ;
};

#endif //_SAX2_GRAMMAR_RULE_HANDLER_HPP
