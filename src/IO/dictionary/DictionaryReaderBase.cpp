/*
 * DictionaryReaderBase.cpp
 *
 *  Created on: Sep 4, 2013
 *      Author: Stefan
 */

#include "IO/dictionary/DictionaryReaderBase.hpp"
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>

namespace DictionaryReader
{

  DictionaryReaderBase::DictionaryReaderBase(
    IVocabularyInMainMem * p_vocaccess)
    : m_p_vocaccess(p_vocaccess),
      m_currentLineNumber(0)
    , m_numBytesRead(0)
  {
    if(m_p_vocaccess)
      m_p_vocaccess->SetDictionaryReader(this);
  }

  DictionaryReaderBase::~DictionaryReaderBase()
  {
    if(m_p_vocaccess)
      m_p_vocaccess->SetDictionaryReader(NULL);
  }

} /* namespace DictionaryReader */
