#pragma once //incl. guard
#ifndef IVOCABULARYINMAINMEM_HPP
#define IVOCABULARYINMAINMEM_HPP

#include <string>
#include <windef.h> //for BYTE etc.
#include <map> //class std::map
#include <set> //class std::set
#include <Attributes/EnglishWord.hpp> //class EnglishAuxiliaryVerb etc.
#include <Attributes/GermanWord.hpp> //class GermanAuxiliaryVerb etc.
#include <Attributes/PositionString.hpp> //class PositionString
#include <Attributes/Word.hpp>
#include <Attributes/CollectDictStatsThreadFuncParams.h>
#include "Controller/thread_type.hpp"

#include "VocabularyAndTranslation.hpp" //class VocabularyAndTranslation
#include <IO/dictionary/DictionaryReaderBase.hpp> //class DictionaryReaderBase

#define NUMBER_OF_PERSONAL_PRONOUNS 7

//Forward declarations:
class EnglishWord;
class GermanWord;

  class IVocabularyInMainMem
  {
  protected:
    I_UserInterface * mp_userinterface;
    DictionaryReader::DictionaryReaderBase * m_p_dictionaryReader;
    fastestUnsignedDataType m_numberOfVocPairs;
    /** number of all English words (added inside binary and added from
     *  dictionary */
    fastestSignedDataType m_numberOfEnglishWords;
  public:
    typedef std::set<VocabularyAndTranslation *> voc_container_type;
    static std::string s_arstdstrPersonalPronoun [] ; //= {"hh", "j"} ;
    //Use the English_word_class enum in class EnglishWord instead
//  //Because this is a translation for English->German the dictionary resp.
//  //the order of the words is primarily important for English words / word
//  //attributes.
//  //For the
//  enum English_word_class
//  {
//    noun = //0
//      ENGLISH_NOUN
//    , main_verb //Vollverb
//    , adjective
//    , adverb
//    , preposition
//    , pronoun
//    , auxiliary_verb
//    , conjunction
//    , English_definite_article
//    , English_indefinite_article
//    //, personal_pronoun
//    , comma
//  } ;
  enum personal_pronoun
  {
    _1_st_person_singular = 0
    , _2nd_person_singular //Vollverb
    , _3rd_person_singular
    , _1st_person_plural
    , _2nd_person_plural
    , _3rd_person_plural
  } ;

  fastestUnsignedDataType m_maxTokenToConsider;
  IVocabularyInMainMem() ;
  //Define virtual constructor to avoid g++ warning in subclasses like
  //"`class EngNounsLetterTree' has virtual functions but non-virtual
  //destructor"
  virtual ~IVocabularyInMainMem() {}

  virtual void AddVocabularyAttributes(
    enum EnglishWord::English_word_class word_class,
    void * p_v) = 0;
  /** Delete all dictionary entries. */
  virtual void clear() = 0;
  virtual unsigned GetNumberOfVocPairs() { return m_numberOfVocPairs; }
  void DecreaseNumberOfVocPairs() { -- m_numberOfVocPairs; }
  virtual fastestUnsignedDataType GetNumberOfAllocatedBytes() = 0;
//  virtual fastestUnsignedDataType GetNumberOfNouns() = 0;
//  virtual fastestUnsignedDataType GetNumberOfVerbs() = 0;
  virtual void GetCollectDictionaryStatisticsStatus(fastestUnsignedDataType & currentItemNo) { };
  virtual void GetStatistics(
//    fastestUnsignedDataType wordClass[], 
//    const fastestUnsignedDataType numArrayEles
    std::map<enum EnglishWord::English_word_class, unsigned> &
      englishWordClass2CounterMap
    ) = 0;
  unsigned GetNumberOfEnglishWords() { return m_numberOfEnglishWords; }
//  void createMapping();
    //Make methods pure virtual ("virtual [...] = 0" ) to avoid
    //linker error "LNK 2001" in MSVC++.
//    virtual BYTE GetGermanArticle() = 0 ;
  /** @return a container because 1 word may have multiple translations:
   *  e.g. "love": the "love", to "love" */
  virtual /*void * */ std::set<VocabularyAndTranslation *> * find(
    const PositionStringVector & psv,
//      const PositionstdstringVector & psv,
    DWORD & r_dwTokenIndex
    ) = 0;

  I_UserInterface * GetUserInterface() { return mp_userinterface; }
//    virtual std::string GetGermanPlural() = 0 ;
//    virtual WORD GetNumberOfNouns() = 0 ;
//    virtual BYTE IsSingular() = 0 ;
//    virtual BYTE GetTranslationType() = 0 ;
//    virtual std::string GetGermanSingular() = 0 ;
    virtual void InsertPersonalPronouns() {} ;
    virtual void InsertPersonalPronounsObjectiveForm() {}
//    virtual void NextNoun() = 0 ;
    void InsertAuxiliaryVerbBe();
    void InsertAuxiliaryVerbHave();
    void InsertAuxiliaryVerbWill();
    void InsertFundamentalWords() ;
    virtual /*void*/ //VocabularyAndTranslation *
      IVocabularyInMainMem::voc_container_type * Insert(
      const char * wordBegin, int numChars, //void * p_v
      enum EnglishWord::English_word_class, /*void *&*/
      VocabularyAndTranslation *& p_vocabularyandtranslation) = 0;// { };
    /** For inserting fundamental words etc. */
    virtual void /* * */ /*VocabularyAndTranslation * */
      Insert(const std::string & stdstrWord,
        enum EnglishWord::English_word_class word_class, /*void * p_v */
        VocabularyAndTranslation *& p_vocabularyandtranslation)
    {
      ++ m_numberOfEnglishWords;
//      ++ m_numberOfVocPairs;
      //If not "= 0 ":
      //"undefined reference to `vtable for IVocabularyInMainMem'" when linking
      //with g++ .
      Insert(stdstrWord.c_str(), stdstrWord.length(), word_class, /*p_v*/
        p_vocabularyandtranslation);
    }
    /** For inserting fundamental words easily: the pointer is set to NULL
     *  inside this function. */
    virtual void /* * */ /*VocabularyAndTranslation * */
      Insert(
        const std::string & stdstrWord,
        enum EnglishWord::English_word_class word_class /*void * p_v */
        )
    {
      VocabularyAndTranslation * p_vocabularyandtranslation = NULL;
      Insert(
        stdstrWord,
        word_class, /*void * p_v */
        p_vocabularyandtranslation);
    }
    /**For inserting objects of subclasses of "EnglishWord" and "GermanWord".*/
    virtual void /* void * */
      Insert(EnglishWord & ew , GermanWord & gw, //void * p_v
        VocabularyAndTranslation * p_vocabularyandtranslation
        ) = 0 ;
    /** Inserts the string as a key value to the container and adds the string
     *  as an attribute value.*/
    virtual void InsertAsKeyAndHandleVocabularyAttributes(
      const char * ar_chWordBegin,
      unsigned stringLen,
      enum EnglishWord::English_word_class word_class
      ) {};
    /** Inserts the string as a key value to the container and adds the string
     *  as an attribute value.*/
    virtual /*void*/ //VocabularyAndTranslation *
    IVocabularyInMainMem::voc_container_type *
    InsertAsKeyAndAddVocabularyAttributes(
      const char * ar_chWordBegin,
      unsigned stringLen,
      enum EnglishWord::English_word_class word_class,
      VocabularyAndTranslation *& p_vocabularyandtranslation
      )
    {
      ++ m_numberOfVocPairs;
      /*void * p_v =*/ return Insert(ar_chWordBegin, stringLen, word_class,
        /*NULL*/ p_vocabularyandtranslation);
//      AddVocabularyAttributes( word_class, p_v);
    }
    /** Save memory by referring to existing voc attributes. */
    virtual /*void*/ //VocabularyAndTranslation *
    IVocabularyInMainMem::voc_container_type *
    InsertAsKeyAndReferToExistingVocData(
      enum EnglishWord::English_word_class word_class,
      const char * ar_chWordBegin,
      unsigned stringLen,
      const VocabularyAndTranslation * const p_vocandtranslAllocated
      )
    {
      VocabularyAndTranslation * p_vocandtransl = NULL;
      IVocabularyInMainMem::voc_container_type * p_voc_container =
        Insert(ar_chWordBegin, stringLen, word_class, /*NULL*/
            p_vocandtransl);
      if( p_vocandtransl )
      {
        p_vocandtransl->PointToAttributeData(p_vocandtranslAllocated);
        p_vocandtransl->m_englishWordClass = word_class;
      }
      return p_voc_container;
    }
    static void OutputVocs(const voc_container_type &);
    static void OutputVocs(const voc_container_type *);
    void SetUserInterface( I_UserInterface * p_userinterface )
    {
      mp_userinterface = p_userinterface ;
    }
    virtual void SetDictionaryReader(
      DictionaryReader::DictionaryReaderBase * p_dictionaryReader)
    {
      m_p_dictionaryReader = p_dictionaryReader;
    }
    
    void GetStatisticsInSeparateThread(
      /*const*/ CollectDictStatsThreadFuncParams & collectDictStatsThreadFuncParams,
      VTrans::thread_type & thread);
  };
#endif
