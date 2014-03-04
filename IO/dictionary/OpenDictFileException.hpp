/*
 * OpenDictFileException.hpp
 *
 *  Created on: 06.02.2014
 *      Author: mr.sys
 */

#ifndef OPENDICTFILEEXCEPTION_HPP_
#define OPENDICTFILEEXCEPTION_HPP_

#include <FileSystem/File/File.hpp>

namespace VTrans3
{
//  namespace DictionaryReader
//  {

    class OpenDictFileException
    {
    public:
      enum I_File::OpenError m_openError;
      OpenDictFileException(enum I_File::OpenError);
      virtual
      ~OpenDictFileException();
    };

//  } /* namespace DictionaryReader */
} /* namespace VTrans3 */

#endif /* OPENDICTFILEEXCEPTION_HPP_ */