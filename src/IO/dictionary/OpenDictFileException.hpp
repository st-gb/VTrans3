/*
 * OpenDictFileException.hpp
 *
 *  Created on: 06.02.2014
 *      Author: mr.sys
 */

#ifndef OPENDICTFILEEXCEPTION_HPP_
#define OPENDICTFILEEXCEPTION_HPP_

#include <FileSystem/File/File.hpp> //I_File::OpenError
#include <string> //class std::string

namespace VTrans3
{
//  namespace DictionaryReader
//  {

    class OpenDictFileException
    {
      std::string m_absoluteFilePath;
    public:
      enum I_File::OpenResult m_openResult;
      OpenDictFileException(enum I_File::OpenResult);
      OpenDictFileException(enum I_File::OpenResult, const std::string &);
      virtual
      ~OpenDictFileException();
      std::string GetErrorMessageA() const;
    };

//  } /* namespace DictionaryReader */
} /* namespace VTrans3 */

#endif /* OPENDICTFILEEXCEPTION_HPP_ */
