/*
 * TinyXMLconfigReader.hpp
 *
 *  Created on: 08.12.2013
 *      Author: mr.sys
 */

#ifndef TINYXMLCONFIGREADER_HPP_
#define TINYXMLCONFIGREADER_HPP_

namespace TinyXML
{
  /** Reads XML configuration files via the TinyXML library/ framework (DOM). */
  class TinyXMLconfigReader
  {
  public:
    TinyXMLconfigReader();
    virtual
    ~TinyXMLconfigReader();
    bool ReadMainConfigFile(const char * const);
  };

} /* namespace TinyXML */

#endif /* TINYXMLCONFIGREADER_HPP_ */
