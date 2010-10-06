
//class XERCES_CPP_NAMESPACE::LocalFileInputSource
#include <xercesc/framework/LocalFileInputSource.hpp>
//from http://xerces.apache.org/xerces-c/program-sax2-3.html
//class XERCES_CPP_NAMESPACE::SAX2XMLReader
#include <xercesc/sax2/SAX2XMLReader.hpp>
//class XERCES_CPP_NAMESPACE::XMLReaderFactory
#include <xercesc/sax2/XMLReaderFactory.hpp>
//#include <xercesc/sax2/ContentHandler.hpp>
//class XERCES_CPP_NAMESPACE::DefaultHandler
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>//class XERCES_CPP_NAMESPACE::XMLString

#include <Xerces/ReadXMLdocument.hpp> //ReadXMLdocument(...)

#include <iostream> //std::cout

using namespace std;
//using namespace xercesc;

unsigned char ReadViaSAX2(
  const char * const cpc_chXMLfilePath ,
  XERCES_CPP_NAMESPACE::ContentHandler * const pc_contenthandler
  )
{
  try {
    XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize();
  }
  catch (const XERCES_CPP_NAMESPACE::XMLException & toCatch) {
      char * message = XERCES_CPP_NAMESPACE::XMLString::transcode(
        toCatch.getMessage() );
      cout << "Error during initialization! :\n";
      cout << "Exception message is: \n"
           << message << "\n";
      XERCES_CPP_NAMESPACE::XMLString::release( & message);
      return 1;
  }
  XERCES_CPP_NAMESPACE::SAX2XMLReader * parser =
    XERCES_CPP_NAMESPACE::XMLReaderFactory::createXMLReader();
  parser->setFeature(XERCES_CPP_NAMESPACE::XMLUni::fgSAX2CoreValidation, true);
  // optional
  parser->setFeature(XERCES_CPP_NAMESPACE::XMLUni::fgSAX2CoreNameSpaces, true);

  XERCES_CPP_NAMESPACE::DefaultHandler * defaultHandler = new
    XERCES_CPP_NAMESPACE::DefaultHandler();
  parser->setContentHandler(//defaultHandler
    pc_contenthandler );
  parser->setErrorHandler(defaultHandler);

  try {
      parser->parse(cpc_chXMLfilePath);
  }
  catch (const XERCES_CPP_NAMESPACE::XMLException & toCatch) {
      char* message = XERCES_CPP_NAMESPACE::XMLString::transcode(
        toCatch.getMessage() );
      cout << "Exception message is: \n"
           << message << "\n";
      XERCES_CPP_NAMESPACE::XMLString::release(&message);
      return 1;
  }
  catch (const XERCES_CPP_NAMESPACE::SAXParseException & toCatch) {
      char* message = XERCES_CPP_NAMESPACE::XMLString::transcode(
        toCatch.getMessage() );
      cout << "Exception message is: \n"
           << message << "\n";
      XERCES_CPP_NAMESPACE::XMLString::release(&message);
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
  XERCES_CPP_NAMESPACE::XMLPlatformUtils::Terminate();
  return 0;
}

unsigned char ReadViaSAX2InitAndTermXerces(
  const char * const cpc_chXMLfilePath ,
//  XERCES_CPP_NAMESPACE::ContentHandler * const pc_contenthandler ,
  XERCES_CPP_NAMESPACE::DefaultHandler * const pc_contenthandler ,
  std::wstring & r_stdwstrErrorMessage
  )
{
  unsigned char by = 1 ;
//  LOG( "read XML configuration--filename: \"" << cp_chXMLfilePath << "\"" );
  bool bXercesSuccessfullyInitialized = false ;
  //from http://xerces.apache.org/xerces-c/program-sax2-3.html:
  try
  {
    //http://xerces.apache.org/xerces-c/program-3.html:
    //"Independent of the API you want to use, DOM, SAX, or SAX2, your
    //application must initialize the Xerces system before using the API[...]"
    //Initialize() must be called _before_ any Xerces function call, else SIGSEV
    // /program crash.
    XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize();
//    LOG( "Xerces access successfully initialized"//"\n"
//      )
    bXercesSuccessfullyInitialized = true ;
  }
  catch (const XERCES_CPP_NAMESPACE::XMLException & toCatch )
  {
    r_stdwstrErrorMessage = L"Xerces error during initialization! :\n"
      "Exception message is: \n"
       + std::wstring( toCatch.getMessage() ) + L"\n" ;
      //DEBUG("XML error:%s\n",message);
//      LOG( "XML error:" << message << "\n" );
//    return 0 ;
  }
  if( bXercesSuccessfullyInitialized )
  {
    //  XERCES_CPP_NAMESPACE::LocalFileInputSource xerces_localfileinputsource(
    //    X(cp_chXMLfilePath) ) ;
    XMLCh * p_xmlchXMLfilePath = XERCES_CPP_NAMESPACE::XMLString::transcode(
      cpc_chXMLfilePath);
    if( p_xmlchXMLfilePath )
    {
      XERCES_CPP_NAMESPACE::LocalFileInputSource xerces_localfileinputsource(
        p_xmlchXMLfilePath ) ;
      by = //ReadViaSAX2( cpc_chXMLfilePath, pc_contenthandler ) ;
        ReadXMLdocument(
          xerces_localfileinputsource ,
          pc_contenthandler ,
          r_stdwstrErrorMessage
          ) ;
//      delete pc_contenthandler ;
      XERCES_CPP_NAMESPACE::XMLString::release( & p_xmlchXMLfilePath );
    }
    //http://xerces.apache.org/xerces-c/program-3.html:
    //"Independent of the API you want to use, DOM, SAX, or SAX2, your
    //application must [...] and terminate it after you are done.
    //When Terminate() was called in another block (even if in a function that
    //is called in the same block) than program crash.
    XERCES_CPP_NAMESPACE::XMLPlatformUtils::Terminate();
//    LOG( "Xerces access terminated"//"\n"
//      )
  }
  return by ;
}
