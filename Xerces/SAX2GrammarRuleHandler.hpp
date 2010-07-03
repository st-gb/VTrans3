#ifndef _SAX2_GRAMMAR_RULE_HANDLER_HPP
#define _SAX2_GRAMMAR_RULE_HANDLER_HPP

//#include "XercesAttributesHelper.hpp"

//#ifdef COMPILE_WITH_XERCES
#include <xercesc/sax2/Attributes.hpp> //for "xercesc_2_8::Attributes"
#include <xercesc/sax2/DefaultHandler.hpp>
//#include "../global.h" //for DEBUG(...) etc.
#include <string> //for std::string
//#include <string> //std::string

//Needed for verzichten auf the exact namespace.
XERCES_CPP_NAMESPACE_USE

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
  : public DefaultHandler
  //ContentHandler
{
private:
  ParseByRise & mr_parsebyrise ;
  std::string m_strElementName ;

public:
  //MySAX2Handler(PStates & pstates);
  SAX2GrammarRuleHandler(
    ParseByRise & r_parsebyrise
    );

  void startElement(
      const   XMLCh* const    uri,
      const   XMLCh* const    localname,
      const   XMLCh* const    qname,
      const   Attributes&     attrs
//        XercesAttributesHelper &
  );
  void fatalError(const SAXParseException&);
};

#endif //_SAX2_GRAMMAR_RULE_HANDLER_HPP
