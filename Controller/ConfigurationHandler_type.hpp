/*
 * ConfigurationHandler_type.hpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#ifndef CONFIGURATIONHANDLER_TYPE_HPP_
#define CONFIGURATIONHANDLER_TYPE_HPP_

//#ifdef __ANDROID__
  //#include <IO/TinyXMLconfigReader.hpp>
//  typedef TinyXML::TinyXMLconfigReader ConfigurationHandler_type;
  #include <IO/MiniXMLconfigReader.hpp>
  typedef VTrans3::MiniXML::MiniXMLconfigReader ConfigurationHandler_type;
//#else
//  typedef Apache::Xerces::SAX2configReader ConfigurationHandler_type;
//#endif

#endif /* CONFIGURATIONHANDLER_TYPE_HPP_ */
