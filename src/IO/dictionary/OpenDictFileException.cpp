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

    OpenDictFileException::OpenDictFileException(enum I_File::OpenResult openResult)
    {
      m_openResult = openResult;
    }
    
    OpenDictFileException::OpenDictFileException(
      enum I_File::OpenResult openResult, 
      const std::string & absoluteFilePath)
      : m_absoluteFilePath(absoluteFilePath), m_openResult(openResult)
    {
    }

    OpenDictFileException::~OpenDictFileException()
    {
      // TODO Auto-generated destructor stub
    }

    std::string OpenDictFileException::GetErrorMessageA() const
    {
      std::string result = "file ";
      if( ! m_absoluteFilePath.empty() )
        result += "\"" + m_absoluteFilePath + "\"";
      switch(m_openResult)
      {
        case I_File::fileNotFound :
          result += "not found"; 
          return result;
          break;
        default:
          return "";
      }
      return "";
    }
//  } /* namespace DictionaryReader */
} /* namespace VTrans3 */
