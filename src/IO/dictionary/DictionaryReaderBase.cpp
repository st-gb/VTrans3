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
  DictionaryReaderBase::nounGenderString2genderEnumContainerType 
    DictionaryReaderBase::s_nounGenderString2genderEnum;

  DictionaryReaderBase::DictionaryReaderBase(
    IVocabularyInMainMem * p_vocaccess)
    : m_p_vocaccess(p_vocaccess),
      m_currentLineNumber(0)
    , m_numBytesRead(0)
  {
    if( s_nounGenderString2genderEnum.empty() )
      Build_NounGenderString2genderEnum();

    if(m_p_vocaccess)
      m_p_vocaccess->SetDictionaryReader(this);
  }

  DictionaryReaderBase::~DictionaryReaderBase()
  {
    if(m_p_vocaccess)
      m_p_vocaccess->SetDictionaryReader(NULL);
  }
  
  /** These are common notations in many dictionaries. */
  void DictionaryReaderBase::Build_NounGenderString2genderEnum()
  {
    s_nounGenderString2genderEnum.insert(std::make_pair("{m}", GermanNoun::masculine));
    s_nounGenderString2genderEnum.insert(std::make_pair("{f}", GermanNoun::feminine));
    s_nounGenderString2genderEnum.insert(std::make_pair("{n}", GermanNoun::neuter));
    s_nounGenderString2genderEnum.insert(std::make_pair("{pl}", GermanNoun::plural));
  }

} /* namespace DictionaryReader */
