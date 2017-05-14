/*
 * ConfigurationHandler_type.hpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#ifndef CONFIGURATIONHANDLER_TYPE_HPP_
#define CONFIGURATIONHANDLER_TYPE_HPP_

//#define XERCES
#ifdef USE_APACHE_XERCES
  #include <Xerces/SAX2configReader.hpp>
  typedef Apache::Xerces::SAX2configReader ConfigurationHandler_type;
  typedef XERCES_CPP_NAMESPACE::Attributes attributeType;
#else
  //#include <IO/TinyXMLconfigReader.hpp>
//  typedef TinyXML::TinyXMLconfigReader ConfigurationHandler_type;
  #include <IO/MiniXML/MiniXMLconfigReader.hpp>
  typedef mxml_node_t * attributeType;
  typedef VTrans3::MiniXML::MiniXMLconfigReader ConfigurationHandler_type;
#endif

#endif /* CONFIGURATIONHANDLER_TYPE_HPP_ */
