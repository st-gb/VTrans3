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

//  } /* namespace DictionaryReader */
} /* namespace VTrans3 */
