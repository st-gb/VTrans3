/*
 * ConfigurationReader.hpp
 *
 *  Created on: 09.12.2013
 *      Author: mr.sys
 */

#ifndef CONFIGURATIONREADER_HPP_
#define CONFIGURATIONREADER_HPP_

#include <string> //class std::string

namespace VTrans3
{
  /** Base class for reading (translation rule, ...) configuration files.
   *  May be derived e.g. by SAX2 reader (Apache Xerces or MiniXML) or by a
   *  reader for another file format or by completely another implementation.*/
  class ConfigurationReader
  {
  protected:
  public:
    std::string m_stdstrVocabularyFilePath ;
    ConfigurationReader();
    virtual
    ~ConfigurationReader();
  };

} /* namespace VTrans3 */

#endif /* CONFIGURATIONREADER_HPP_ */
