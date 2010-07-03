//from http://xerces.apache.org/xerces-c/program-sax2-3.html
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

#include <iostream>

using namespace std;
using namespace xercesc;

unsigned char ReadViaSAX2(
  const char * const cpc_chXMLfilePath ,
  XERCES_CPP_NAMESPACE::ContentHandler * const pc_contenthandler
  )
{
  try {
      XMLPlatformUtils::Initialize();
  }
  catch (const XMLException & toCatch) {
      char * message = XMLString::transcode( toCatch.getMessage());
      cout << "Error during initialization! :\n";
      cout << "Exception message is: \n"
           << message << "\n";
      XMLString::release(&message);
      return 1;
  }
  SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
  parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
  parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);   // optional

  XERCES_CPP_NAMESPACE::DefaultHandler * defaultHandler = new DefaultHandler();
  parser->setContentHandler(//defaultHandler
    pc_contenthandler );
  parser->setErrorHandler(defaultHandler);

  try {
      parser->parse(cpc_chXMLfilePath);
  }
  catch (const XMLException& toCatch) {
      char* message = XMLString::transcode(toCatch.getMessage());
      cout << "Exception message is: \n"
           << message << "\n";
      XMLString::release(&message);
      return 1;
  }
  catch (const SAXParseException& toCatch) {
      char* message = XMLString::transcode(toCatch.getMessage());
      cout << "Exception message is: \n"
           << message << "\n";
      XMLString::release(&message);
      return 2;
  }
  catch (...) {
      cout << "Unexpected Exception \n" ;
      return 3;
  }

  delete parser;
  delete defaultHandler;
  //http://xerces.apache.org/xerces-c/program-3.html:
  //XMLPlatformUtils::Terminate  must be called at least once in each process.
  XMLPlatformUtils::Terminate();
  return 0;
}

unsigned char ReadViaSAX2AndDeleteContentHandler(
  const char * const cpc_chXMLfilePath ,
  XERCES_CPP_NAMESPACE::ContentHandler * const pc_contenthandler
  )
{
  ReadViaSAX2( cpc_chXMLfilePath, pc_contenthandler ) ;
  delete pc_contenthandler ;
  return 0 ;
}
