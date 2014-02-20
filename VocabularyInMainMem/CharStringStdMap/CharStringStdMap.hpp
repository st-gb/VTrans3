/*
 * CharStringMap.hpp
 *
 *  Created on: 19.01.2013
 *      Author: Stefan
 */

#ifndef CHARSTRINGMAP_HPP_
#define CHARSTRINGMAP_HPP_

#include "../IVocabularyInMainMem.hpp" //base class
#include "../I_WordSearch.hpp" //base class
#include <map> //class std::map
#include <set> //class std::set

#include "VocabularyInMainMem/VocablesForWord.hpp"

class CharStringStdMap
  : //public I_WordSearch,
   public IVocabularyInMainMem
{
private:
  typedef std::map<std::string, VocablesForWord> map_type;
  map_type m_charStringMap;
  static VocablesForWord * s_p_vocablesforwordLastForInsertedWord;
  static VocabularyAndTranslation * s_p_vocabularyandtranslation;
public:
  CharStringStdMap();
  virtual
  ~CharStringStdMap();

  void AddVocabularyAttributes(
    enum EnglishWord::English_word_class word_class,
    void * p_v);
  void clear();
//  unsigned GetNumberOfVocPairs();
  /*void * */std::set<VocabularyAndTranslation *> * find(
    const PositionStringVector & psv,
//      const PositionstdstringVector & psv,
    DWORD & r_dwTokenIndex
    );
//  /** For all (sub-)classes that use class "VocabularyAndTranslation". */
//  void Insert(
//    EnglishWord & ew , GermanWord & gw, //void * p_v
//    VocabularyAndTranslation * p_vocabularyandtranslation
//    );
//  inline VocabularyAndTranslation *
//    HandleVocabularyAndTranslationPointerInsertion(
//    //This set is to ensure that for identical strings for the SAME vocabulary
//    //not 2 or more VocAndTransl object should be inserted into the same
//    //LetterNode of the last character.
//    std::set<VocablesForWord *> & stdsetpletternodeLastStringChar
//   //, VocabularyAndTranslation * pvocabularyandtranslation
//   , bool bInsertNewVocabularyAndTranslation
//   , BYTE byVocabularyType
//   );
  fastestUnsignedDataType GetNumberOfAllocatedBytes();
  //TODO implement
  void GetStatistics(fastestUnsignedDataType [] ) { }
  unsigned GetNumberOfEnglishWords() { return m_charStringMap.size(); }
  void Insert(EnglishWord&, GermanWord&, VocabularyAndTranslation*) { };
  //VocabularyAndTranslation * /*void * */
  IVocabularyInMainMem::voc_container_type * Insert(//const std::string & stdstr,
    const char * const wordBegin,
    int numChars,
    /*BYTE byWordClass*/ enum EnglishWord::English_word_class,
    /*bool insertNewVocAtts*/ //void *&
    VocabularyAndTranslation *& p_vocabularyandtranslation);
//  void Insert(const char * wordBegin, int numChars, //void * p_v
//    enum EnglishWord::English_word_class);
};

#endif /* CHARSTRINGMAP_HPP_ */
