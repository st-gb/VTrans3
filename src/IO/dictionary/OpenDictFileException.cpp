/*
 * OpenDictFileException.cpp
 *
 *  Created on: 06.02.2014
 *      Author: mr.sys
 */

#include <IO/dictionary/OpenDictFileException.hpp>

namespace VTrans3
{
//  namespace DictionaryReader
//  {

    OpenDictFileException::OpenDictFileException(enum I_File::OpenError openError)
    {
      m_openError = openError;
    }

    OpenDictFileException::~OpenDictFileException()
    {
      // TODO Auto-generated destructor stub
    }

    std::string OpenDictFileException::GetErrorMessageA()
    {
      switch(m_openError)
      {
        case I_File::fileNotFound :
          return "file not found";
        default:
          return "";
      }
      return "";
    }
//  } /* namespace DictionaryReader */
} /* namespace VTrans3 */
