/*
 * ReadFileLneAndExtract.hpp
 *
 *  Created on: Sep 7, 2013
 *      Author: Stefan
 */

#ifndef READFILELNEANDEXTRACT_HPP_
#define READFILELNEANDEXTRACT_HPP_

#include "IVocabularyInMainMem.hpp"
#include <map> //class std::map
#include <IO/dictionary/TUchemnitzDictionaryReader.hpp>
#include "CharStringStdMap/CharStringStdMap.hpp"

namespace VocabularyAccess
{
  class DictLineNumbers
  {
  public:
    typedef fastestUnsignedDataType dictLineNumberDataType;
    typedef std::vector<dictLineNumberDataType> stl_container_type;
    stl_container_type m_dictLineNumbers;
//    VocablesForWord vocfw;
//    fastestUnsignedDataType dictLineNo;
  };

  /** Voc access class that */
  class ReadFileLineAndExtract
    : public IVocabularyInMainMem
  {
    /** Following 2 members for caching VocAndTranslObjects */
    TUchemnitzDictionaryReader m_TUchemnitzDictionaryReader;
    CharStringStdMap m_CharStringStdMap;

    typedef std::map<fastestUnsignedDataType, const char * const> map_type;
    typedef std::map<//const char * const,
      std::string,
      DictLineNumbers> stl_container_type;
//    map_type m_std_mapDictLineNumber2EnglishWord;
    stl_container_type m_englishWord2DictLineNumber;
    stl_container_type m_englishWord2DictFilOffset;
  public:
    ReadFileLineAndExtract() {
      Init(); }
    ReadFileLineAndExtract(I_UserInterface & ui, IVocabularyInMainMem * p_vocaccess);
//    ReadFileLineAndExtract(I_UserInterface & ui)
//      : m_TUchemnitzDictionaryReader(ui) { }
    virtual
    ~ReadFileLineAndExtract();

    void AddVocabularyAttributes(
      enum EnglishWord::English_word_class word_class,
      void * p_v) { }

    /*void * */ std::set<VocabularyAndTranslation *> * find(
      const PositionStringVector & psv,
  //      const PositionstdstringVector & psv,
      DWORD & r_dwTokenIndex
      );// { return NULL; }
    /*void*/ VocabularyAndTranslation * Insert(
      const char * wordBegin, int numChars, //void * p_v
      enum EnglishWord::English_word_class, void *);
    fastestUnsignedDataType GetNumberOfAllocatedBytes();
    fastestUnsignedDataType GetNumberOfNouns();
    fastestUnsignedDataType GetNumberOfVerbs();
    void GetStatistics(fastestUnsignedDataType [] );
    void clear();
    void //void *
      Insert(EnglishWord & ew , GermanWord & gw, //void * p_v
        VocabularyAndTranslation * p_vocabularyandtranslation
        ) { }
    void Init()
    {
      m_TUchemnitzDictionaryReader.SetVocabularyAccess(& m_CharStringStdMap);
    }
    bool OpenDictionaryFile(const char * const filePath)
    {
      return m_TUchemnitzDictionaryReader.OpenDictFile(filePath);
    }
    /** called when con-/ destructing DictionaryReader*/
    void SetDictionaryReader(
      DictionaryReader::DictionaryReaderBase * p_dictionaryReader)
//        : IVocabularyInMainMem::SetDictionaryReader(p_dictionaryReader)
    {
      IVocabularyInMainMem::SetDictionaryReader(p_dictionaryReader);
//        m_p_dictionaryReader = p_dictionaryReader;
//      if( p_dictionaryReader)
//        m_TUchemnitzDictionaryReader.OpenDictFile(p_dictionaryReader->GetFilePath() );
    }
    void SetUserInterface(I_UserInterface * ui)
    {
      mp_userinterface = ui;
      m_TUchemnitzDictionaryReader.SetUserInterface(ui);
    }
  };

} /* namespace VocabularyAccess */
#endif /* READFILELNEANDEXTRACT_HPP_ */
