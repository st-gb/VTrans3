/*
 * ReadViaSAX2.hpp
 *
 *  Created on: Jun 11, 2010
 *      Author: Stefan
 */

#ifndef READVIASAX2_HPP_
#define READVIASAX2_HPP_

#include <xercesc/util/XercesDefs.hpp> //XERCES_CPP_NAMESPACE_BEGIN
#include <xercesc/util/XercesVersion.hpp> //XERCES_CPP_NAMESPACE

//class XERCES_CPP_NAMESPACE::ContentHandler ;
XERCES_CPP_NAMESPACE_BEGIN
  class ContentHandler ;
XERCES_CPP_NAMESPACE_END

unsigned char ReadViaSAX2(
  const char * const cpc_chXMLfilePath ,
  XERCES_CPP_NAMESPACE::ContentHandler * p_contenthandler
  ) ;
unsigned char ReadViaSAX2AndDeleteContentHandler(
  const char * const cpc_chXMLfilePath ,
  XERCES_CPP_NAMESPACE::ContentHandler * const pc_contenthandler
  ) ;

#endif /* READVIASAX2_HPP_ */
