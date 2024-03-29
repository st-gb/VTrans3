/** File:   BinarySearchInDictData.cpp
 * Author: sg
 * Created on 6. August 2017, 13:18  */

#include "BinarySearchInDictData.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_ERROR(...)
#include "UserInterface/I_UserInterface.hpp"
#include <FileSystem/File/FileReadException.hpp>
#include <Controller/character_string/ISO_8859_1.hpp>
#include <Controller/character_string/stdtstr.hpp>
#include <InputOutput/GetCharInACSIIcodePage850.hpp>
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>//class IVocabularyInMainMem
/** SAME_VOCABLE_SEPERATOR_CHAR etc. */
#include <IO/dictionary/TUchemnitz/TUchemnitzDictSeparatorChars.h>
#include <limits.h> //UINT_MAX
#include <ctype.h> //::isdigit(...), isalpha(int)
#include <stdexcept>      // std::out_of_range
#include <vector> //class std::vector
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)
#include <compiler/GCC/enable_disable_write_strings_warning.h> //GCC_DIAG_OFF
#include <hardware/CPU/atomic/atomicIncrement.h>
#include <FileSystem/File/File.hpp> //class I_File
#include <FileSystem/File/EndOfFileException.hpp> //class EndOfFileException
#include <IO/dictionary/DictionaryFileAccessException.hpp>
#include <IO/dictionary/OpenDictFileException.hpp>//class OpenDictFileException
#include <OperatingSystem/GetLastErrorCode.hpp>

#define INDEX_OF_LAST_SMALL_LETTER_IN_ASCII 128

//NodeTrie<enum TUchemnitzDictionary::wordKinds>
//  DictionaryReader::TUchemnitz::EngWordSorted1st::BinarySearchInDictData::s_wordKindContainer(
//    INDEX_OF_LAST_SMALL_LETTER_IN_ASCII,
//    TUchemnitzDictionary::not_set);

namespace DictionaryReader
{
  namespace TUchemnitz
  {
    namespace EngWordSorted1st
    {
      BinarySearchInDictData::wordKindContainerType BinarySearchInDictData::s_wordKindContainer;

//BinarySearchInDictData::BinarySearchInDictData()
//{
//    addTrieNodes();
//}

//BinarySearchInDictData::BinarySearchInDictData(const BinarySearchInDictData& orig) {
//}

BinarySearchInDictData::~BinarySearchInDictData() {
//  s_nodetrieWordKind.FreeMemory();
}

    void BinarySearchInDictData::InsertIntoWordKindContainer(
      const char * const wordKindString,
      TUchemnitzDictionary::wordKinds wordKind)
    {
//      s_wordKindContainer.insert_inline
      s_wordKindContainer.insert(std::make_pair(wordKindString, wordKind) );
    }
    
    //TODO move to a general TU Chemnitz file?! because it may be used for any
    //  TU Chemnitz reader
    void BinarySearchInDictData::addTrieNodes()
    {
//      s_p_i_userinterface = & i_userinterface;
//      if( s_nodetrieWordKind.size() == 0 )
      {
    //    Build();
        try
        {
        //Wortart: adj, adv, vi=verb intrans. vr=verb reflexiv.
        InsertIntoWordKindContainer("adj", TUchemnitzDictionary::adj);
        InsertIntoWordKindContainer("adv", TUchemnitzDictionary::adv);
        InsertIntoWordKindContainer("m", TUchemnitzDictionary::mascNoun);
        InsertIntoWordKindContainer("f", TUchemnitzDictionary::femNoun);
        InsertIntoWordKindContainer("n", TUchemnitzDictionary::neutralNoun);
        InsertIntoWordKindContainer("pl", TUchemnitzDictionary::pluralNoun);
        InsertIntoWordKindContainer("vi", //="Verb Intransitive",
          TUchemnitzDictionary::intransitiveVerb);
        InsertIntoWordKindContainer("vt", //="Verb Transitive"
          TUchemnitzDictionary::transitiveVerb);
        }catch( const NS_NodeTrie::RootNodeNotInitalizedException & e)
        {
          LOGN_ERROR("NS_NodeTrie::RootNodeNotInitalizedException")
        }
      }
    }

    //TODO move to a general TU Chemnitz file?! because it may be used for any
    //  TU Chemnitz reader
   inline enum EnglishWord::English_word_class 
      getEnglishWordClassFromTUchemnitzDictWordClass(
      const enum TUchemnitzDictionary::wordKinds wordKind, 
      VocabularyAndTranslation *& p_vocabularyandtranslation)
    {
      enum EnglishWord::English_word_class word_class = EnglishWord::beyond_last_entry;
      GCC_DIAG_OFF(switch)
      switch(wordKind)
      {
//      case TUchemnitzDictionary::reflexiveVerb:
//        word_class = EnglishWord::ref
      case TUchemnitzDictionary::intransitiveVerb:
        /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
        word_class = EnglishWord::main_verb_allows_0object_infinitive;
        break;
      case TUchemnitzDictionary::transitiveVerb:
        /** http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29:
        * " sowohl Subjekt als auch ein Objekt benötigen, damit ein Satz, der
        * mit diesem Verb gebildet wird, grammatisch ist." */
        word_class = EnglishWord::main_verb_allows_1object_infinitive;
        break;
      case TUchemnitzDictionary::adv:
        word_class = EnglishWord::adverb;
        break;
      case TUchemnitzDictionary::adj:
        word_class = EnglishWord::adjective;
        /** =NULL means: The next time a VocabularyAndTranslation should be
         *   created. (multiple singular nouns may appear in a row->create
         *   a VocabularyAndTranslation for each noun. */
        p_vocabularyandtranslation = NULL;
        break;
      case TUchemnitzDictionary::femNoun:
      case TUchemnitzDictionary::mascNoun:
      case TUchemnitzDictionary::neutralNoun:
        /** Must add as singular, else applying grammar rule fails. */
        word_class = EnglishWord::singular;
        /** =NULL means: The next time a VocabularyAndTranslation should be
         *   created. (multiple singular nouns may appear in a row->create
         *   a VocabularyAndTranslation for each noun. */
        p_vocabularyandtranslation = NULL;
        break;
      case TUchemnitzDictionary::pluralNoun:
//        word_class = EnglishWord::noun;
        /** =NULL means: The next time a VocabularyAndTranslation should be
         *   created.*/
        p_vocabularyandtranslation = NULL;
        word_class = EnglishWord::plural_noun;
        break;
      }
      GCC_DIAG_ON(switch)
      return word_class;
    }
//    VocabularyAndTranslation *
    IVocabularyInMainMem::voc_container_type * BinarySearchInDictData::AddVocable(
      const std::vector<std::string> & englishVocableWords,
      enum TUchemnitzDictionary::wordKinds wordKind,
//      enum EnglishWord::English_word_class word_class
//      IVocabularyInMainMem::voc_container_type & voc_container
      VocabularyAndTranslation *& p_vocabularyandtranslation
      )
    {
      LOGN_DEBUG("begin wordkind:" << wordKind << " " << p_vocabularyandtranslation)
//      VocabularyAndTranslation * p_vocabularyandtranslation = NULL;
      IVocabularyInMainMem::voc_container_type * p_voc_container = NULL;
#ifdef _DEBUG
      const int sz = englishVocableWords.size();
#endif
      enum EnglishWord::English_word_class word_class = EnglishWord::beyond_last_entry;
      //TODO replace the switch/case with getEnglishWordClassFromTUchemnitzDictWordClass ?!
//      word_class = getEnglishWordClassFromTUchemnitzDictWordClass(wordKind);
      GCC_DIAG_OFF(switch)
      switch(wordKind)
      {
//      case TUchemnitzDictionary::reflexiveVerb:
//        word_class = EnglishWord::ref
      case TUchemnitzDictionary::intransitiveVerb:
        /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
        word_class = EnglishWord::main_verb_allows_0object_infinitive;
        break;
      case TUchemnitzDictionary::transitiveVerb:
        /** http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29:
        * " sowohl Subjekt als auch ein Objekt benötigen, damit ein Satz, der
        * mit diesem Verb gebildet wird, grammatisch ist." */
        word_class = EnglishWord::main_verb_allows_1object_infinitive;
        break;
      case TUchemnitzDictionary::adv:
        word_class = EnglishWord::adverb;
        break;
      case TUchemnitzDictionary::adj:
        word_class = EnglishWord::adjective;
        /** =NULL means: The next time a VocabularyAndTranslation should be
         *   created. (multiple singular nouns may appear in a row->create
         *   a VocabularyAndTranslation for each noun. */
        p_vocabularyandtranslation = NULL;
        break;
      case TUchemnitzDictionary::femNoun:
      case TUchemnitzDictionary::mascNoun:
      case TUchemnitzDictionary::neutralNoun:
        /** Must add as singular, else applying grammar rule fails. */
        word_class = EnglishWord::singular;
        /** =NULL means: The next time a VocabularyAndTranslation should be
         *   created. (multiple singular nouns may appear in a row->create
         *   a VocabularyAndTranslation for each noun. */
        p_vocabularyandtranslation = NULL;
        break;
      case TUchemnitzDictionary::pluralNoun:
//        word_class = EnglishWord::noun;
        /** =NULL means: The next time a VocabularyAndTranslation should be
         *   created.*/
        p_vocabularyandtranslation = NULL;
        word_class = EnglishWord::beyond_last_entry;
        break;
      }
      GCC_DIAG_ON(switch)
      LOGN_DEBUG("EnglishWord::class: " << word_class)
      if( word_class != EnglishWord::beyond_last_entry &&
          englishVocableWords.size() > 0 )
      {
        const std::string str = englishVocableWords.at(0);
        const char * ar_chWordBegin = str.c_str();
        const int stringLen = str.length();
        p_voc_container =
          //s_p_vocinmainmem
            m_p_vocaccess->InsertAsKeyAndAddVocabularyAttributes(
          ar_chWordBegin
          , (int &) stringLen
          , word_class,
          p_vocabularyandtranslation);
        LOGN_DEBUG("p_vocabularyandtranslation:" << p_vocabularyandtranslation)
        if( p_vocabularyandtranslation)
        {
          GCC_DIAG_OFF(switch)
          switch(wordKind)
          {
          case TUchemnitzDictionary::mascNoun :
            p_vocabularyandtranslation->SetAttributeValue(0, GermanNoun::der);
            break;
          case TUchemnitzDictionary::femNoun :
            p_vocabularyandtranslation->SetAttributeValue(0, GermanNoun::die);
            break;
          case TUchemnitzDictionary::neutralNoun :
            p_vocabularyandtranslation->SetAttributeValue(0, GermanNoun::das);
            break;
          }
          GCC_DIAG_ON(switch)
        }
//        p_vocandtransl->SetGermanWord();
      }
      LOGN_DEBUG("return p_voc_container:" << p_voc_container)
      return /*p_vocabularyandtranslation*/ p_voc_container;
    }

    void AddWord(
      const bool english,
      const unsigned pipeCount,
      char word[BinarySearchInDictData::numberOfWordCharacters],
      const fastestUnsignedDataType charIndex,
//      std::vector<std::vector<std::string> > & englishVocables,
      std::vector<std::string> & englishVocableWords,
      /** e.g. synonyms */
      std::vector< std::vector <std::string> > & germanVocables,
      unsigned & semicolCountInsideCurrentPipeCharRange,
      fastestUnsignedDataType & synonymIndex//,
      )
    {
      LOGN_DEBUG("begin--English word?:" << (english ? "yes" : "no") << "# '|':"
        << pipeCount << "# ';':" << semicolCountInsideCurrentPipeCharRange)
      if( word[0] != '\0' )
      {
//        word[charIndex] = '\0';
        //            HandleSynonymSeparatorChar();
        if (english)
        {
          /** A pipe character ('|') separates different words belonging to
           *  the same vocable (e.g. singular and plural) */
          //              if(englishVocables.size() > semicolCountInsideCurrentPipeCharRange )
          if (pipeCount == 0)
          {
  //          std::vector<std::string> vec;
  //          vec.push_back(word);
  //          englishVocables.push_back(vec);
              /**crashed when using "push_back(word)" when calling vector dtor.*/
//            englishVocableWords.push_back(word);
            englishVocableWords.push_back(std::string(word) );
          }
          else
          {
  //          if (semicolCountInsideCurrentPipeCharRange + 1
  //              < englishVocables.size())
            {
  //            std::vector<std::string> & vec =
  //              englishVocables[semicolCountInsideCurrentPipeCharRange];
  //            vec.push_back(word);
            }
            //                  englishVocableWords.push_back(word);
          }
        }
        else
        {
  #ifdef _DEBUG
          int sz = germanVocables.size();
  #endif
          if( semicolCountInsideCurrentPipeCharRange /*pipeCount*/ <
              germanVocables.size() )
          {
            const fastestUnsignedDataType vocableIndex =
              semicolCountInsideCurrentPipeCharRange;
            LOGN_DEBUG("using German vocable \"" << word << "\" at index "
              << /*semicolCountInsideCurrentPipeCharRange*/ vocableIndex )
            std::vector<std::string> & vec = germanVocables[
              /*semicolCountInsideCurrentPipeCharRange*/ vocableIndex];
            vec.push_back(std::string(word) );
          }
          else
          {
            LOGN_DEBUG("appending German vocable \"" << word << "\" at index "
              << germanVocables.size() )
            std::vector<std::string> vec;
            vec.push_back(std::string(word) );
            germanVocables.push_back( vec);
          }
  //        germanVocableWords.push_back(word);
        }
        word[0] = '\0';
      }
      LOGN_DEBUG("end")
    }

    void BinarySearchInDictData::HandleSynonymSeparatorChar(
      const bool english,
      const unsigned pipeCount,
      const char word[100],
//      std::vector<std::vector<std::string> > & englishVocables,
      std::vector<std::string> & englishVocableWords,
      /** e.g. synonyms */
      std::vector< std::vector <std::string> > & germanVocables,
      unsigned & semicolCountInsideCurrentPipeCharRange,
      fastestUnsignedDataType & synonymIndex//,
//      unsigned wordStart
      )
    {
      LOGN_DEBUG("begin")
      ++ semicolCountInsideCurrentPipeCharRange;
      ++ synonymIndex;
      //for "Auto {n}; Wagen {m};"
//      wordStart = charIndex + 2;
    }

    enum TUchemnitzDictionary::wordKinds
      BinarySearchInDictData::HandleClosingBrace(//int i,
      const fastestUnsignedDataType charIndex,
      fastestUnsignedDataType & kindOfWordStart,
      char wordKind[5]
      )
    {
      LOGN_DEBUG("begin")
      enum TUchemnitzDictionary::wordKinds e_wordKind =
        TUchemnitzDictionary::not_set;
      {
        //        std::string kindOfWord = strCurrentWordData.substr(kindOfWordStart,
        //          charIndex - kindOfWordStart);
        //            int i = s_p_i_userinterface->m_dictionaryFileLineNumber;
//          int i = 55;
        //        NodeTrieNode<TUchemnitzDictionaryReader::extractVocable> * p_ntn =
        //          s_nodetrieWordKind.contains(
        //          (BYTE *) (array + kindOfWordStart), charIndex - kindOfWordStart,
        //          true);
        //#else
        const int numWordKindChars = charIndex - kindOfWordStart
//              //NULL terminating char
//              + 1
            ;
        //            char * wordKind = new char[numWordKindChars];
        wordKind[numWordKindChars /*- 1*/ ] = '\0';
        
        wordKindContainerType::const_iterator c_iterWordKind =
            s_wordKindContainer.find(wordKind);
        //            delete [] wordKind;
        kindOfWordStart = 0;
        //#endif
        if (c_iterWordKind != s_wordKindContainer.end() )
          {
            bool bExtractVocables = false;
            //          ( * (extractVocable) p_ntn->m_member )(array, numChars
            //              /*strCurrentWordData*/, charIndex, pipeCount);
            //              if( p_ntn->m_member == & TUchemnitzDictionaryReader::InsertIntransitiveVerb )
            //              {
            //                //Else: "tries to add other data like finite verbs for
            //                //"[...]; anlaufen (Vorgang) {vi}" although there is no such data.
            //                if( semicolCountInsideCurrentPipeCharRange == 0 && pipeCount == 0 )
            //                  bExtractVocables = true;
            //              }
            //              else
            //                bExtractVocables = true;
            if (bExtractVocables)            //            return
              //                ExtractVocables(
              //                  array + wordStart,
              //                  numChars
              //                  /*strCurrentWordData*/, charIndex,
              //                  pipeCount,
              //                  semicolCountInsideCurrentPipeCharRange,
              //                  p_ntn->m_member,
              //                  * this);
              //                AddVocable(p_ntn->m_member);
              ;

            //          InsertAdjective(array);
//              return p_ntn->m_member;
            e_wordKind = c_iterWordKind->second;
          }
      }
//      return i;
      LOGN_DEBUG("return " << e_wordKind << wordKind )
      return /*BinarySearchInDictFile::not_set*/ e_wordKind;
    }

    void BinarySearchInDictData::AddGermanAttributes(
        std::map<unsigned, VocabularyAndTranslation *> & voc_containerVocsCreated,
        std::vector< std::vector <std::string> > & germanVocables)
    {
      LOGN_DEBUG("begin--# vocs created:" << voc_containerVocsCreated.size()
          << "# german vocs:" << germanVocables.size() )
#ifdef _DEBUG
      const int numVocsCreated = voc_containerVocsCreated.size();
      const int numGermanVocables = germanVocables.size();
#endif
      std::map<unsigned, VocabularyAndTranslation *>::const_iterator
        c_iterPipeIndex2VocAndTranslPtr = voc_containerVocsCreated.begin();
      std::vector< std::vector <std::string> >::const_iterator 
        c_iterGermanVocable = germanVocables.begin();
      std::vector<std::string>::const_iterator c_iterGermanWord;
      while( c_iterGermanVocable != germanVocables.end() && 
        c_iterPipeIndex2VocAndTranslPtr != voc_containerVocsCreated.end() )
      {
        const std::vector<std::string> & r_GermanWords = ( * c_iterGermanVocable);
        c_iterGermanWord = r_GermanWords.begin();
        VocabularyAndTranslation * p_vocAndTransl = 
          c_iterPipeIndex2VocAndTranslPtr->second;
        const int pipeIndex = c_iterPipeIndex2VocAndTranslPtr->first;

        EnglishWord::English_word_class engWordClass = EnglishWord::
            MapGrammarPartIDtoWordClass(p_vocAndTransl->m_englishWordClass);
        if( p_vocAndTransl && /*p_vocAndTransl->m_englishWordClass*/ 
            engWordClass <= EnglishWord::adjective )
        {
          const VocabularyAndTranslation::ArraySizes & arraySizes =
            VocabularyAndTranslation::s_arraysizes[engWordClass];
          fastestUnsignedDataType germanWordIndex = 0;
          while(c_iterGermanWord != r_GermanWords.end() )
          {
            if( germanWordIndex < arraySizes.m_byArraySizeForGermanWord )
            {
              const std::string & germanWord = * c_iterGermanWord;
              LOGN_DEBUG("setting German word \"" << germanWord
                << "\" for index " << germanWordIndex
                << " for pipe index " << pipeIndex
                << " ,vocl&transl ptr " << p_vocAndTransl)
              p_vocAndTransl->SetGermanWord(germanWord.c_str(),
                  germanWord.length(), germanWordIndex);
              ++ germanWordIndex;
            }
            ++ c_iterGermanWord;
          }
        }
        ++ c_iterPipeIndex2VocAndTranslPtr;
        ++ c_iterGermanVocable;
      }
      LOGN_DEBUG("end")
    }

    inline void output(
        const fastestUnsignedDataType offset,
//        const fastestUnsignedDataType charIndex,
        const int i,
        const char * const word
        )
    {
//      const fastestUnsignedDataType offset = offsetOfVocBegin + charIndex;
      LOGN_DEBUG( "offset:" << offset << " char:\'"
        << (char) i << "\' word: \"" << word << "\"")
    }

    inline void SetWordCharacter(char word[BinarySearchInDictData::numberOfWordCharacters],
        const fastestUnsignedDataType charIndexInsideWord,
        const char ch)
    {
      if(charIndexInsideWord < BinarySearchInDictData::numberOfWordCharacters)
        word[charIndexInsideWord] = ch;
#ifdef _DEBUG
      else
      {
        int i = ch;
      }
#endif
    }

    /** @param comp: lower: word to search is before current word in dictionary*/
    bool BinarySearchInDictData::CompareVectors(
      PositionStringVector::cmp & comp,
      const PositionStringVector& psvStringToSearch,
      fastestUnsignedDataType & hi,
      fastestUnsignedDataType & lo,
      bool & endSearchForCompareStringInCurrentVocData,
      const fastestUnsignedDataType numTokenForStringToSearch,
      PositionStringVector & psvDictFile,
      const DWORD & r_dwTokenIndex,
      fastestUnsignedDataType & byteOffset,
      fastestUnsignedDataType & closestBeforeNonMatchOffset,
      bool & breakWhile)
    {
      //                  CompareVectors();
//      comp = psvStringToSearch.Compare(psvDictFile, r_dwTokenIndex,
//          numTokenForStringToSearch);
      LOGN_DEBUG("return value of comparison: " << PositionStringVector:://GetResultAsString(comp)
          s_comparisonResultString[comp] )
      GCC_DIAG_OFF(switch)
      switch (comp)
      {
      //                  case PositionStringVector::match :
      //                    /*p_vocandtransl*/
      //                    p_voc_container = extractVocable(byteOffsetOfVocable, p_vocandtransl);
      ////                    if( p_vocandtransl != NULL)
      ////                      p_voc_container->
      //                    break;
      /** String to search is lower than string in dictionary. */
//      case PositionStringVector::tooFewTokens:
//        endSearchForCompareStringInCurrentVocData;
//        break;
      case PositionStringVector::notSet: //e.g. searching for word "Ö"
        breakWhile = true;
        endSearchForCompareStringInCurrentVocData = true;
        break;
      case PositionStringVector::lower:
        psvDictFile.clear();
        hi = byteOffset;
        byteOffset = /*lo + (hi - lo)*/(lo + hi) / 2;
        LOGN_DEBUG(
            "string to search is < string in dict -> new offset:("
            << lo << "+" << hi << ")/2=" << byteOffset)
        //                    breakWhile = true;
        endSearchForCompareStringInCurrentVocData = true;
        break;
        /** String to search is greater than string in dictionary. */
      case PositionStringVector::greater:
      case PositionStringVector::tooFewTokensInComparisonVector:
        /** string to search is greater than string in dictionary-> dictionary
         *  string is before string to search->memorize the offset*/
//        closestBeforeNonMatchOffset = byteOffset;
        LOGN_DEBUG("closest offset smaller offset of string match is now:"
			<< closestBeforeNonMatchOffset )
        psvDictFile.clear();
        lo = byteOffset;
        byteOffset = /*lo + (hi - lo )*/(lo + hi) / 2;
        LOGN_DEBUG(
          "string to search is > string in dict -> new offset:("
          << lo << "+" << hi << ")/2=" << byteOffset << "=0x" << std::ios::hex
          << byteOffset)
        endSearchForCompareStringInCurrentVocData = true;
        /** e.g. "vacuum" (string to search) is < "vacuum cleaner"
         *   */
        const fastestUnsignedDataType numTokenStringFromDictFile =
            psvDictFile.size();
        if (numTokenForStringToSearch <= numTokenStringFromDictFile)
          breakWhile = true;
        break;
      }
      GCC_DIAG_ON(switch)
      if (hi < lo + 2)
        {
          LOGN_DEBUG("hi <= lo")
          //                    return NULL;
          breakWhile = true;
          //                    break;
        }
      return endSearchForCompareStringInCurrentVocData;
    }

    std::set<VocabularyAndTranslation *> * BinarySearchInDictData::findEnglishWord(
      const PositionStringVector & psvStringToSearch,
      DWORD & r_dwTokenIndex
      )
    {
      IVocabularyInMainMem::voc_container_type * p_valid_voc_container = NULL, 
        * p_voc_container = NULL;
      typedef std::vector< std::set<fastestUnsignedDataType> > offsetVec;

//      fastestUnsignedDataType byteOffsetOfVocable =
      VocabularyAndTranslation * p_vocandtransl = NULL;
      std::set<fastestUnsignedDataType> byteOffsetsOfVocData;
      /** vector containing sets of byte offsets for vocabulary with same 
       *    # of tokens.
       *    1st element: byte offsets of voc data with 1 token */
      std::vector< std::set<fastestUnsignedDataType> > byteOffsetsOfVocDataVec;
      PositionStringVector::cmp comp;// = PositionStringVector;
      fastestUnsignedDataType numToken = 1;
      do
      {
        byteOffsetsOfVocData.clear();
        findEnglishWord(
          psvStringToSearch,
          r_dwTokenIndex,
          numToken,
          byteOffsetsOfVocData
          );
        if( byteOffsetsOfVocData.size() > 0 ) {
          byteOffsetsOfVocDataVec.push_back(byteOffsetsOfVocData);
      //TODO if only largest words found in dictionary are used then 
      // possibilities are missing?:"Linux is software developer Stefan" ->
      // "Linux ist ein Softwareentwickler" & "Stefan" statt 
      // "Linux ist eine Software" & "Entwickler Stefan".
      // "he likes the vacuum cleaner sucks." -> 
      // "er mag den Staubsauger" & "saugt"
      // "er mag das Vakuum" & "Reiniger nervt"
      // (this also holds for/affects other implementations of 
      //std::set<VocabularyAndTranslation *> * IVocabularyInMainMem::find() )

      ///Set to the max. tokens covered
      r_dwTokenIndex = r_dwTokenIndex + numToken - 1;
        }
        ++ numToken;
        if( r_dwTokenIndex + numToken > psvStringToSearch.size() )
          break;
      }while(byteOffsetsOfVocData.size() > 0 );
#ifdef _DEBUG
      const int vecsz = byteOffsetsOfVocDataVec.size();
      LOGN_DEBUG("# vectors:" << vecsz)
#endif
      /** Use vector containing the most tokens.
       *  Must use "reverse_iterator" rather than "const_reverse_iterator"
       *  for Android NDK. */
      offsetVec::reverse_iterator
        /** Start with the word containing the most tokens. */
        c_rev_iter = byteOffsetsOfVocDataVec.rbegin();
      if( c_rev_iter != byteOffsetsOfVocDataVec.rend() )
      {
        const std::set<fastestUnsignedDataType> & r_byteOffsetsOfVocData = * c_rev_iter;
#ifdef _DEBUG
        const int vecsz = r_byteOffsetsOfVocData.size();
        LOGN_DEBUG("# offset data for same # token:" << vecsz)
#endif
        std::set<fastestUnsignedDataType>::const_iterator c_iter =
          r_byteOffsetsOfVocData.begin();
        fastestUnsignedDataType byteOffsetOfVocable;
        while(c_iter != r_byteOffsetsOfVocData.end() )
        {
          byteOffsetOfVocable = * c_iter;
          //TODO it seems that more than 1 container is created for the same word "car"
          // for the dict.cc dictionary
          p_voc_container = extractVocable(
            byteOffsetOfVocable,
            //p_voc_container,
            p_vocandtransl);
          /** If word kind can't be determined the p_voc_container is NULL */
          if( p_voc_container )
            p_valid_voc_container = p_voc_container;
          ++ c_iter;
        }
        //++ c_rev_iter;
      }
//      if()
      return /*NULL*/ p_valid_voc_container;
    }

    inline void HandleEndOfToken(
      char word [],
      fastestUnsignedDataType & charIndex,
      PositionStringVector & psvDictFile,
      fastestUnsignedDataType & tokenIndex
      )
    {
      word[charIndex] = '\0';
      LOGN_DEBUG("word: \"" << word << "\"")
      /** For "issuance; issue(of sth.)" : between ";" and " " is an empty
       * string token -> do not add to vector. */
      if( charIndex > 0 )
      {
        charIndex = 0;
        VTrans::string_type str(word);
        PositionString pos_str(str, tokenIndex, tokenIndex);
        psvDictFile.push_back(pos_str);
        LOGN_DEBUG("size of position string vector created from dict file: " << psvDictFile.size() )
      }
    }

    void HandleEndOfWord(
      char word [],
      fastestUnsignedDataType & charIndex,
      PositionStringVector & psvDictFile,
      fastestUnsignedDataType & tokenIndex,
      bool & compareVectors
//      ,const char currentChar
      )
    {
//      word[charIndex] = '\0';
//      charIndex = 0;
//      VTrans::string_type str(word);
//      PositionString pos_str(str, tokenIndex, tokenIndex);
//      psvDictFile.push_back(pos_str);
//      ++ tokenIndex;
      LOGN_DEBUG("begin current char: \'" << word[charIndex] << "\'")
      /** For "issue(of sth.)::Ausgabe{f}" : between ")" and "::" is an empty
       * string token -> do not compare. */
      if( charIndex == 0 )
        compareVectors = false;
      else
      {
        compareVectors = true;
        HandleEndOfToken(word, charIndex, psvDictFile, tokenIndex);
      }
    }

    void BinarySearchInDictData::GetStatistics(
//        fastestUnsignedDataType * representations,
//        const fastestUnsignedDataType numArrayEles
      std::map<enum EnglishWord::English_word_class, unsigned> &
        englishWordClass2CounterMap
      )
    {
     m_p_englishWordClass2CounterMap = & englishWordClass2CounterMap;
      SetDictDataOffset(0);
      char wordType[100];
      fastestUnsignedDataType numWordKindChars = 0;

      I_File::file_pointer_type fileOffset;
      int currentFileByte;
      bool afterOpeningCurlyBrace = false;
      VocabularyAndTranslation * pVocabularyAndTranslation = NULL;
      do
      {
        currentFileByte = GetNextByte();
        switch(currentFileByte)
        {
         case '{':
          afterOpeningCurlyBrace = true;
          numWordKindChars = 0;
          break;
         case '}':
          if( afterOpeningCurlyBrace )
          {
            wordType[numWordKindChars] = '\0';
            wordKindContainerType::const_iterator wordKindIterator = 
              s_wordKindContainer.find(wordType);
            if( wordKindIterator != s_wordKindContainer.end() )
            {
              const enum EnglishWord::English_word_class word_class = 
                getEnglishWordClassFromTUchemnitzDictWordClass(wordKindIterator->second, 
                pVocabularyAndTranslation);
//              std::map<enum EnglishWord::English_word_class, unsigned>::const_iterator 
//                c_iter = englishWordClass2CounterMap.find(word_class);
//              if( c_iter != englishWordClass2CounterMap.end() )
//              {
//               }
              try
              {
                unsigned & counter =
                  /** "[]" Because Android NDK compiler claims: no
                   * std::map::at()" */
                  englishWordClass2CounterMap[word_class];
                atomicIncrement( (long *) & counter);
#ifdef _DEBUG
                unsigned & counter2 = englishWordClass2CounterMap.at(word_class);
                int i = counter;
                i += 0;
#endif
              }catch(const std::out_of_range & e) //thrown by std::map::at(...)
              {
              }
            }
            afterOpeningCurlyBrace = false;
          }
          break;
         default:
           if( afterOpeningCurlyBrace )
           {
             if(numWordKindChars > 99 )
              {
                const char * const filePath = m_dictFile.GetFilePathA().c_str();
                fileOffset = m_dictFile.GetCurrentFilePointerPosition();
                std::ostringstream oss;
                oss << "in file " << filePath << ": for \'{\' char at offset "
                  << (fileOffset - numWordKindChars) << 
                  " there is no closing \'}\' after " << numWordKindChars << 
                  " chars";
                m_p_vocaccess->GetUserInterface()->Message(oss.str() );
//               throw DictionaryFileAccessException( 
//                  DictionaryFileAccessException::extractWordType, 0, filePath, 
//                  fileOffset );
                afterOpeningCurlyBrace = false;
              }
             else
               wordType[numWordKindChars ++] = currentFileByte;
           }
        }
        fileOffset = m_dictFile.GetCurrentFilePointerPosition();
      }while( fileOffset < m_fileSizeInBytes );
      m_p_vocaccess->GetUserInterface()->EndedGetDictStats();
    }
    
    /** @return container of different vocables.
     * e.g. line "schoolbook; textbook; educational book | schoolbooks; textbooks; educational books :: Lehrbuch {n} | Lehrbücher {pl} "
     * */
    IVocabularyInMainMem::voc_container_type *
      BinarySearchInDictData::extractVocable(
        const fastestUnsignedDataType offset//,
//        IVocabularyInMainMem::voc_container_type & voc_type
        , VocabularyAndTranslation * p_vocabularyandtranslation
        )
    {
      LOGN_DEBUG("begin offset:" << offset)
      bool firstEngGerSepChar = true;
      std::vector< std::vector <std::string> > germanVocables;

      IVocabularyInMainMem::voc_container_type * p_voc_container = NULL;
      std::map<unsigned, VocabularyAndTranslation *> voc_containerVocsCreated;
    //  BYTE * p_kindOfWordStart;
      unsigned pipeCount = 0;
      unsigned wordStart = 0;
      unsigned prevPipeCharIndex = 0;
      /** Separates synonyms */
      unsigned semicolCountInsideCurrentPipeCharRange = 0;
#ifdef _DEBUG
      int currOffset = //m_englishDictionary.tellg();
        GetCurrentDictDataOffset();
#endif
//      m_englishDictionary.seekg(offset, std::ios_base::beg);
      /** SeekFilePointerPosition Should throw an error on failure. */
      //TODO handle exceptions?
      bool streamIsGood = SetDictDataOffset(offset);
      LOGN_DEBUG("after seeking to offset " << offset)
      int i;
      /** ReadByte Should throw an error on failure. */
      i = GetNextByte();
//      streamIsGood = i > -1;
      bool breakWhile = false;
      char word[numberOfWordCharacters];
      int prevChar = 0;
      std::vector<std::string> englishVocableWords;
      std::vector<std::vector<std::string> > englishVocables;
      fastestUnsignedDataType synonymIndex = 0;
      bool english = true;
      fastestUnsignedDataType kindOfWordStart = 0;
      fastestUnsignedDataType charIndex = 0, charIndexInsideWord = 0;
      char wordKind[/*5*/ numberOfWordCharacters];
//      unsigned char UTF8sequence[8];
      bool insideBracket = false;
      char ASCIIcodePage850Char;
      char ch;
      while( streamIsGood /*&& ! breakWhile*/ )
      {
        if(kindOfWordStart)
          if( ::isdigit(i) || i == '_' )
          {
            SetWordCharacter(wordKind,/*charIndex - kindOfWordStart*/
              charIndexInsideWord, i);
            ++ charIndexInsideWord;
          }
//        enum UTF8byteSequence::bytePos bp = UTF8byteSequence::IsByteOfUTF8sequence(prevChar);
        const bool isByteOfMultiByteUTF8sequence =
          UTF8byteSequence::IsByteOfMultiByteUTF8sequence(i);
        if(//prevChar > 127
//            bp == UTF8byteSequence::insideUTF8sequence
            isByteOfMultiByteUTF8sequence
           )
        {
          if( ! insideBracket && charIndexInsideWord < 100)
          {
            ASCIIcodePage850Char = //GetCharInASCIIcodePage850(i);
              ISO_8859_1::GetFromUTF8(i);
            if( ASCIIcodePage850Char != 0 )
            {
              SetWordCharacter(word,/*charIndex*/ charIndexInsideWord,
                ASCIIcodePage850Char);
              ++ charIndexInsideWord;
            }
//            else
//              SetWordCharacter(word,/*charIndex*/ charIndexInsideWord, i);
//            ++ charIndexInsideWord;
          }
        }
        else
        if( ( ::isalpha(i) || i == ' ' ) )
        {
          if(kindOfWordStart)
            SetWordCharacter(wordKind,/*charIndex - kindOfWordStart*/
              charIndexInsideWord, i);
          else if( ! insideBracket && charIndexInsideWord < 100)
          {
//            if( i == ' ')
//
//            else
//              wordHasStarted = true;
//            else
//            {
              ch = i;
              SetWordCharacter(word,/*charIndex*/ charIndexInsideWord, ch);
//            }
          }
          ++ charIndexInsideWord;
//          prevChar = i;
        }
        else
        {
          switch(i)
          {
          case SAME_VOCABLE_SEPERATOR_CHAR: // "car ride; drive"
            SetWordCharacter(word, charIndexInsideWord, '\0');
            output(offset + charIndex, i, word);
            charIndexInsideWord = 0;
            break;
          case SYNONYM_SEPERATOR_CHAR: //"car thief |"
          case ENGLISH_GERMAN_SEPERATOR_CHAR: //" carwash ::"
//            endWord = true;
            SetWordCharacter(word,charIndexInsideWord,'\0');
            output(offset + charIndex, i, word);
            charIndexInsideWord = 0;
            break;
          case '{': //"Autorennen {"
          case '(':
          case '[':
            insideBracket = true;
            SetWordCharacter(word,charIndexInsideWord,'\0');
            output(offset + charIndex, i, word);
            charIndexInsideWord = 0;
            break;
          case '}':
          case ')':
          case ']':
            insideBracket = false;
            break;
          }
//          if( ! insideBracket )
//          {
//          word[charIndexInsideWord] = '\0';
//          charIndexInsideWord = 0;
        }
        switch(i)
        {
          case 0xA :
            breakWhile = true;
           break;
          case SAME_VOCABLE_SEPERATOR_CHAR :
            AddWord(
              english,
              pipeCount,
              word,
              charIndex,
              englishVocableWords,
              germanVocables,
              semicolCountInsideCurrentPipeCharRange,
              synonymIndex
              );
            semicolCountInsideCurrentPipeCharRange = 0;
            ++ pipeCount; //next word
//            semicolCountInsideCurrentPipeCharRange = 0;
            synonymIndex = 0;
            break;
          case SYNONYM_SEPERATOR_CHAR: /** "schoolbook; textbook; educational book | "*/
//            HandleSynonymSeparatorChar();
//            HandleSynonymSeparatorChar(
//              english, pipeCount, word,
////              englishVocables,
//              englishVocableWords,
//              germanVocables,
//              semicolCountInsideCurrentPipeCharRange,
//              synonymIndex//, wordStart
//              );
            AddWord(
              english,
              pipeCount,
              word,
              charIndex,
              englishVocableWords,
              germanVocables,
              semicolCountInsideCurrentPipeCharRange,
              synonymIndex
              );
            ++ semicolCountInsideCurrentPipeCharRange;
            ++ synonymIndex;
            break;
          case ENGLISH_GERMAN_SEPERATOR_CHAR :
            if( firstEngGerSepChar)
            {
            AddWord(
              english,
              pipeCount,
              word,
              charIndex,
              englishVocableWords,
              germanVocables,
              semicolCountInsideCurrentPipeCharRange,
              synonymIndex
              );
              firstEngGerSepChar = false;
              semicolCountInsideCurrentPipeCharRange = 0;
              pipeCount = 0;
              synonymIndex = 0;
            }
//            if ( prevPipeCharIndex == charIndex - 1 ) //-> "::"
//              //The kind of word may only be specified in English:
//            //"Hintergrund {m} (Theater) | im Hintergrund (der Bühne) :: back | upstage {adv}"
//              pipeCount = 0;
//            else
//              prevPipeCharIndex = charIndex;
            english = false;
            break;
          case '(':
          case '[':
            AddWord(
              english,
              pipeCount,
              word,
              charIndex,
              englishVocableWords,
              germanVocables,
              semicolCountInsideCurrentPipeCharRange,
              synonymIndex
              );
            break;
          case '{':
            AddWord(
              english,
              pipeCount,
              word,
              charIndex,
              englishVocableWords,
              germanVocables,
              semicolCountInsideCurrentPipeCharRange,
              synonymIndex
              );
            kindOfWordStart = charIndex + 1;
            break;
          case '}':
          {
            /** e.g. to not trying to get word kind of "{work,wrought; worked, wrought}"
             *   from line "work{work,wrought; worked, wrought}|"
               to use word "work" before '{' */
            if( ! english )
            {
            enum TUchemnitzDictionary::wordKinds e_wordKind =
              HandleClosingBrace(charIndex, kindOfWordStart, wordKind);
#ifdef _DEBUG
            const int sz = voc_containerVocsCreated.size();
#endif
//            switch( e_wordKind ) //for "plural noun" etc. do nothing
//              //because it shpuld have been created before
//            {
//            case BinarySearchInDictFile::adv:
//            case BinarySearchInDictFile::femNoun:
//            case BinarySearchInDictFile::mascNoun:
//            case BinarySearchInDictFile::neutralNoun:
              std::map<unsigned, VocabularyAndTranslation *>::iterator
                c_iter = voc_containerVocsCreated.find(semicolCountInsideCurrentPipeCharRange);
              const bool isNotInMap = c_iter == voc_containerVocsCreated.end();
              LOGN_DEBUG("pipe count" << semicolCountInsideCurrentPipeCharRange
                << " is not in map:" << isNotInMap)
              if( isNotInMap )
              {
                p_voc_container = AddVocable(
                  englishVocableWords,
                  e_wordKind,
  //                p_voc_container,
                  p_vocabularyandtranslation);
                /** Prevent from adding a NULL pointer. */
                if( p_vocabularyandtranslation)
                {
                  LOGN_DEBUG("adding " << semicolCountInsideCurrentPipeCharRange
                      << "->" << p_vocabularyandtranslation << " to the container")
    //              bool b;
                  std::pair<std::map<unsigned, VocabularyAndTranslation *>::iterator, bool> p =
                      voc_containerVocsCreated.insert(std::make_pair(
                    semicolCountInsideCurrentPipeCharRange, p_vocabularyandtranslation) );
                }
  //              if(p.second )
  //              {
  //                c_iter = p.first;
  //                AddGermanAttributes();
  //              }
              }
              break;
            }
//            else
              kindOfWordStart = 0;
          }
          break;
//          default :
        }
        if(breakWhile)
          break;
//        } //switch
        ++ charIndex;
        prevChar = i;
//        i = m_englishDictionary.get();
        i = GetNextByte();
//        streamIsGood = m_englishDictionary.good();
        streamIsGood = i > -1;
#ifdef _DEBUG
//        currOffset = m_englishDictionary.tellg();
        currOffset = GetCurrentDictDataOffset();
#endif
      } //while
      AddGermanAttributes(voc_containerVocsCreated, germanVocables);
      englishVocables.clear();
      IVocabularyInMainMem::OutputVocs(p_voc_container);
      LOGN_DEBUG("return " << p_voc_container)
      return p_voc_container;
    }

    fastestUnsignedDataType BinarySearchInDictData::GetByteOffsetOfNextVocDataBegin(
      bool & endSearchForCompareStringInCurrentVocData,
      fastestUnsignedDataType currentFileOffsetOrig )
    {
      /** Save to get the current file pointer: expensive but safe to to do
       *  because it's unknown if currentFileOffsetOrig really represents the
       *  current file pos. */
      currentFileOffsetOrig = GetCurrentDictDataOffset();
      LOGN_DEBUG("begin--start from byte offset " << currentFileOffsetOrig)
//#ifdef DEBUG
//      fastestUnsignedDataType byteOffsetOfVocable = UINT_MAX;
//#endif
      fastestUnsignedDataType currentFileOffset = currentFileOffsetOrig;
//      int i = m_englishDictionary.get();
//      fastestUnsignedDataType numBytesRead = 0;
      /** http://stackoverflow.com/questions/13225014/why-fgetc-too-slow
       *  -> do not use ReadByte() but read multiple bytes at once. */
//      int i = ReadByte();

      /** Because reading a buffer is only done once this value also indicates
       *  the maximum line length in bytes. */
      const fastestUnsignedDataType numBytesToRead =
        512;
      uint8_t buffer[numBytesToRead];
      fastestUnsignedDataType numBytesRead = numBytesToRead;
      /** Read more bytes at once to speed up translation. */
      enum I_File::ReadResult fileReadResult = ReadByteBuffer(buffer, numBytesToRead, numBytesRead);
      if( fileReadResult == I_File::successfullyRead ||
          /** End of file reached? */
          fileReadResult == I_File::readLessThanIntended )
      {
        bool endOfLineCharFound = false;
        for( fastestUnsignedDataType bufferOffset = 0 ;
            bufferOffset < numBytesRead ; ++ bufferOffset )
        {
          if( buffer[bufferOffset] == 0xA /* <> '\n'*/ )
          {
            currentFileOffset += bufferOffset + 1;
//            m_dictFile.SeekFilePointerPosition(currentFileOffset);
            endOfLineCharFound = true;
            break;
          }
        }
        if( endOfLineCharFound )
        {
//  #ifdef DEBUG
//        byteOffsetOfVocable = GetCurrentFilePointerPosition();
//        if( currentFileOffsetOrig == UINT_MAX )
//          currentFileOffset = byteOffsetOfVocable;
//  #else
        if( currentFileOffsetOrig == UINT_MAX )
          currentFileOffset = GetCurrentDictDataOffset();
//  #endif
        LOGN_DEBUG("offset of voc data begin:" << currentFileOffset)
        endSearchForCompareStringInCurrentVocData = false;
        }
        else //if( ! endOfLineCharFound )
          currentFileOffset += numBytesRead;
      }
      LOGN_DEBUG("return " << currentFileOffset)
      return /*byteOffsetOfVocable*/ currentFileOffset;
    }

    /** @brief called to determine whether a specific word exists in the
     *  dictionary file. */
    PositionStringVector::cmp BinarySearchInDictData::ContainsEnglishWord(
      const PositionStringVector & psvStringToSearch,
      DWORD & r_dwTokenIndex,
      const fastestUnsignedDataType numTokenForStringToSearch,
//      std::set<fastestUnsignedDataType> & byteOffsetsOfVocData,
      fastestUnsignedDataType & closestBeforeNonMatchOffset
      )
    {
      LOGN_DEBUG("begin--\"" << psvStringToSearch << "\" "
          << "token index:" << r_dwTokenIndex
          << " # token:" << numTokenForStringToSearch
          //<< " closest before non-match offset:" << closestBeforeNonMatchOffset
        )
  #ifdef _DEBUG
//      fastestUnsignedDataType currOffset = m_englishDictionary.tellg();
      fastestUnsignedDataType currOffset = GetCurrentDictDataOffset();
  #endif
      bool streamIsGood;
      PositionStringVector psvDictFile;
      bool breakWhile = false;
      char word[100];
      fastestUnsignedDataType charIndex = 0;
      bool endSearchForCompareStringInCurrentVocData = false;
      fastestUnsignedDataType tokenIndex = 0;
      bool compareVectors = false;
      /** Set to a default value to avoid using an array out of bounds error
       *  if "cmp" was too large and it was used as a array index for
       *  PositionStringVector::s_comparisonResultString */
      PositionStringVector::cmp comp = PositionStringVector::notSet;

//      int i = m_englishDictionary.get();
      int i = GetNextByte();
//      streamIsGood = m_englishDictionary.good();
      streamIsGood = i > -1;
      while( streamIsGood /*&& ! breakWhile*/ )
      {
        if( ::isalpha(i) ||
            /** e.g. "fork-lift" */
            i == '-' || i == '\'' /** e.g. dog's breakfast */ )
        {
          word[charIndex ++] = i;
        }
        else
        {
          /** Only for outputting the character that caused the call to
           * HandleEndOfWord(...) */
          word[charIndex] = i;
          switch( i )
          {
            case ' ' :
            {
  #ifdef _DEBUG
//              currOffset = m_englishDictionary.tellg();
              currOffset = GetCurrentDictDataOffset();
  #endif
              LOGN_DEBUG("space reached--word is: \"" << word << "\"")
              HandleEndOfToken(word, charIndex, psvDictFile, tokenIndex);
              ++ tokenIndex;
  //                  endSearchForCompareStringInCurrentVocData =
  //                    CompareVectors(
  //                      comp,
  //                      psvStringToSearch,
  //                      hi, lo,
  //                      endSearchForCompareStringInCurrentVocData,
  //                      numTokenForStringToSearch,
  //                      psvDictFile,
  //                      r_dwTokenIndex,
  //                      byteOffset,
  //                      breakWhile);
            }
            break;

            case ENGLISH_GERMAN_SEPERATOR_CHAR :
              endSearchForCompareStringInCurrentVocData = true;
  //                  compareVectors = true;
              HandleEndOfWord(word, charIndex, psvDictFile, tokenIndex, compareVectors);
              break;
            case SAME_VOCABLE_SEPERATOR_CHAR :
              /** e.g. for "work{work,wrought; worked, wrought}|"
               to use word "work" before '{' */
            case '{' :
            case '(' : // e.g. "bank of(gas) cylinders"
//            case ')' : // e.g. "bank of(gas) cylinders"
  //                  compareVectors = true;
              HandleEndOfWord(word, charIndex, psvDictFile, tokenIndex, compareVectors);
              break;
            case SYNONYM_SEPERATOR_CHAR :
              HandleEndOfWord(word, charIndex, psvDictFile, tokenIndex,
                  compareVectors);
  //                  breakWhile = true;
  //                  break;
  //                  psvDictFile.clear();
  //                  breakWhile = true;
              break;
  //              else
  //                byteOffset
  //              word[charIndex ++] = ch;
          } //switch
          if(compareVectors)
          {
            LOGN_DEBUG("compare string vectors==true")
//            endSearchForCompareStringInCurrentVocData =
//              CompareVectors(
//                comp,
//                psvStringToSearch,
//                hi, lo,
//                endSearchForCompareStringInCurrentVocData,
//  //                    numTokenForStringToSearch,
//                numToken,
//                psvDictFile,
//                r_dwTokenIndex,
//                byteOffset,
//                closestBeforeNonMatchOffset,
//                breakWhile);
              comp = psvStringToSearch.Compare(psvDictFile, r_dwTokenIndex,
                numTokenForStringToSearch);
            if( comp == PositionStringVector::lower || PositionStringVector::greater)
              LOGN_DEBUG("token(s) from input text is " << PositionStringVector:://GetResultAsString(comp)
                s_comparisonResultString[comp] << " than token in dictionary")
			else if( comp == PositionStringVector::match )
			  LOGN_DEBUG("token(s) from input text fully matches token in dictionary")
		    else
			  LOGN_DEBUG("result of string comparison:"
			    << PositionStringVector::s_comparisonResultString[comp] )
  //                comp = psvStringToSearch.Compare(/*word*/ psvDictFile, r_dwTokenIndex);
            if( comp == PositionStringVector::match)
            { /** Word is found. Now add all words with the same name. */
  //                  p_voc_container = extractVocable(byteOffsetOfVocable, p_vocandtransl);
//              AddAllOffsetOfMatchingWords(
//                closestBeforeNonMatchOffset,
//                psvStringToSearch,
//                psvDictFile,
//                r_dwTokenIndex,
//                numToken,
//                byteOffsetsOfVocData
//                );
  //                  byteOffsetsOfVocData.insert(byteOffsetOfVocable);
              breakWhile = true;
            }
            /** String to search is greater than string in dict file. */
            else if( comp == PositionStringVector::greater )
            {
              const unsigned currentFilePointerPosition =
                GetCurrentDictDataOffset();
              if( currentFilePointerPosition > closestBeforeNonMatchOffset )
                closestBeforeNonMatchOffset = currentFilePointerPosition;
            }
            psvDictFile.clear();
            compareVectors = false;
          }
        }
  //            } //while()
        if( breakWhile || endSearchForCompareStringInCurrentVocData)
          break;
//        i = m_englishDictionary.get();
        i = GetNextByte();
//        streamIsGood = m_englishDictionary.good();
        streamIsGood = i > -1;
//        psvDictFile.clear();
//        m_englishDictionary.seekg(byteOffset, std::ios_base::beg);
      } //while loop for current voc data
      LOGN_DEBUG("return " << comp << " (" << 
        PositionStringVector::s_comparisonResultString[comp] << ")")
      return comp;
    }

    //TODO find 1st matching vocable in binary search
    //e.g.:2 dictionary entries: "car ............... car ......."
    // found: 2nd entry with "car"
    // no search in itnerval "[0...begin of 2nd car["
    /** @brief if a matching word was found:
     * Multiple vocabulary entries for the same word may exist: "
        "e.g. 1 for verb \"to work\" and 1 for noun \"the work\". "
        "We may not be at the 1st one. So get this now. */
    fastestUnsignedDataType BinarySearchInDictData::
      GetByteOffsetOfFirstMatchingWord(
        const PositionStringVector & psvStringToSearch,
        DWORD & r_dwTokenIndex,
        const fastestUnsignedDataType numToken,
        fastestUnsignedDataType closestBeforeNonMatchOffset,
        fastestUnsignedDataType byteOffsetOfVocable
      )
    {
      LOGN_DEBUG("begin--closest offset smaller than offset of string match:" <<
        closestBeforeNonMatchOffset)
      LOGN_DEBUG("Multiple vocabulary entries for the same word may exist: "
        "e.g. 1 for verb \"to work\" and 1 for noun \"the work\". "
        "We may not be at the 1st one. So get this now.")
      fastestUnsignedDataType byteOffsetOfFirstMatchingWord = byteOffsetOfVocable;
      
      fastestUnsignedDataType lowerBound = closestBeforeNonMatchOffset,
        higherBound = byteOffsetOfVocable;

      bool endSearchForCompareStringInCurrentVocData;
      fastestUnsignedDataType byteOffsetOfCurrentVocable;
      fastestUnsignedDataType byteOffsetOfNextVocDataBegin = 0;
      do
      {      
        byteOffsetOfCurrentVocable = lowerBound + (higherBound - lowerBound) / 2;
        LOGN_DEBUG("lower bound:" << lowerBound << " higher bound:"  
          << higherBound << "->seeking to " << byteOffsetOfCurrentVocable)
        SetDictDataOffset(byteOffsetOfCurrentVocable);

        byteOffsetOfNextVocDataBegin = GetByteOffsetOfNextVocDataBegin(
          endSearchForCompareStringInCurrentVocData, byteOffsetOfCurrentVocable);
        if( ! endSearchForCompareStringInCurrentVocData)
          SetDictDataOffset(byteOffsetOfNextVocDataBegin);
        //TODO gets "notSet" -> endless loop
        PositionStringVector::cmp comp = ContainsEnglishWord(
          psvStringToSearch,
  //            psvDictFile,
          r_dwTokenIndex,
          numToken,
  //                byteOffsetsOfVocData,
          closestBeforeNonMatchOffset
          );
        switch( comp )
        {
        case PositionStringVector::match :
          byteOffsetOfFirstMatchingWord = byteOffsetOfNextVocDataBegin;
          LOGN_DEBUG("setting higher bound to " << byteOffsetOfCurrentVocable)
          //byteOffsetOfFirstMatchingWord = byteOffsetOfCurrentVocable;
          higherBound = byteOffsetOfCurrentVocable /*byteOffsetOfVocable*/;
//          byteOffsetOfCurrentVocable = lowerBound + (higherBound - lowerBound) / 2;
          break;
        /** String to search is lexicgraphically after CURRENT vocable in dict file. */
        case PositionStringVector::greater :
          LOGN_DEBUG("setting lower bound to " << byteOffsetOfCurrentVocable)
          lowerBound = //GetByteOffsetOfNextVocDataBegin(
            //endSearchForCompareStringInCurrentVocData);
            byteOffsetOfCurrentVocable;
//          byteOffsetOfCurrentVocable = lowerBound + (higherBound - lowerBound) / 2;
          break;
        case PositionStringVector::lower :
          higherBound = byteOffsetOfCurrentVocable;
          break;
        default: /** notSet etc. */
            /** Force end of "while" loop */
            higherBound = lowerBound + NUM_MIN_CHARS_PER_VOC_ENTRY + 1;
            break;
        }
      }while(higherBound > lowerBound + NUM_MIN_CHARS_PER_VOC_ENTRY );
      LOGN_DEBUG("end--higherBound <= lowerBound + 6--return " 
        << byteOffsetOfFirstMatchingWord)
      return /*byteOffsetOfNextVocDataBegin*/ byteOffsetOfFirstMatchingWord;
    }
    
    /** @brief indirectly called when */
    void BinarySearchInDictData::findEnglishWord(
      const PositionStringVector & psvStringToSearch,
      DWORD & r_dwTokenIndex,
      const fastestUnsignedDataType numToken,
      std::set<fastestUnsignedDataType> & byteOffsetsOfVocData
      )
    {
      LOGN_DEBUG("psvStringToSearch:" << psvStringToSearch << "# token: " 
        << numToken << "r_dwTokenIndex:" << r_dwTokenIndex)
//      IVocabularyInMainMem::voc_container_type * p_voc_container = NULL;
      if( psvStringToSearch.size() > 0 )
      {
        PositionStringVector psvDictFile;
//        VocabularyAndTranslation * p_vocandtransl = NULL;
        const fastestUnsignedDataType numTokenForStringToSearch =
          psvStringToSearch.size();

        fastestUnsignedDataType byteOffsetOfVocable = UINT_MAX;
        fastestUnsignedDataType closestBeforeNonMatchOffset = /*UINT_MAX*/ 0;
//        int i;
//        char ch;
#ifdef _DEBUG
//        fastestUnsignedDataType currOffset = m_englishDictionary.tellg();
        fastestUnsignedDataType currOffset = GetCurrentDictDataOffset();
#endif
        bool breakWhile = false;
        bool endSearchForCompareStringInCurrentVocData = false;
        //TODO change: for binary search succeeding tokens are located (byte offset) 
        // after the previous word (e.g. vacuum cleaner is behind "vacuum".
        //  So byte offset should be the location of the previous found.
        fastestUnsignedDataType byteOffset = m_fileSizeInBytes / 2;
        fastestUnsignedDataType lo = 0, hi = m_fileSizeInBytes;
//        m_englishDictionary.seekg(byteOffset, std::ios_base::beg);
//        bool streamIsGood = m_englishDictionary.good();
        //TODO error handling on seek failure.
        bool streamIsGood = SetDictDataOffset(byteOffset);
        bool atLeast1VocDataBeginFound = false;
        
//        streamIsGood = GetByteOffsetOfFirstVocable(
//          byteOffsetOfVocable,
//          endSearchForCompareStringInCurrentVocData);
//        SeekFilePointerPosition(0);
//        byteOffsetOfVocable = 0;

//        if( ! streamIsGood )
//          return;
        LOGN_DEBUG("streamIsGood:" << streamIsGood)
//        fastestUnsignedDataType closestBeforeNonMatchOffset = 0;
        while( streamIsGood)
        {
  //        BytePosAndNextChar * p_BytePosAndNextChar;
          bool newLine = false;
          bool englishGermanSeperatorCharOccurred = false;
//          //EOF or read error.
//          if( byteOffsetOfVocable == UINT_MAX )
//          {
//            if( atLeast1VocDataBeginFound )
//             break;
//            else //just 1 entry in dict file -> pos in middle
//            {
//              SeekFilePointerPosition(0);
//              byteOffsetOfVocable = 0;
//            }
//          }
//          else
//           atLeast1VocDataBeginFound = true;
          try
          {
            byteOffsetOfVocable = GetByteOffsetOfNextVocDataBegin(
              endSearchForCompareStringInCurrentVocData, byteOffset);
            SetDictDataOffset(byteOffsetOfVocable);
            PositionStringVector::cmp comp = ContainsEnglishWord(
              psvStringToSearch,
  //            psvDictFile,
              r_dwTokenIndex,
              numToken,
  //                byteOffsetsOfVocData,
              closestBeforeNonMatchOffset
              );
            if( comp == PositionStringVector::match )
            {
//              LOGN_DEBUG("We have a match->but multiple vocabulary entries for "
//                "the same word may exist: e.g. 1 for verb \"to work\" and 1 for noun "
//                "\"the work\". We may not be at the 1st one. So get this now.")
              /** Either found matching at first attempt or binary search
               *  was always backwards (directing file begin).
               *  case 1: in current search no word in dictionary < than word to search read.
               *    -e.g. search "car" and immediately read word "car".)
               *    -e.g. looked up the first word in the dictionary, so no word with
               *      offset smaller than searched word is possible */
//              if( closestBeforeNonMatchOffset != /*0*/ UINT_MAX)
              {
                //1st matching vocable in range:[0...byteOffsetOfVocable]
                //byteOffsetOfVocable;
			  byteOffsetOfVocable = GetByteOffsetOfFirstMatchingWord(
                  psvStringToSearch,
                  r_dwTokenIndex,
                  numToken,
                  /*0*/ closestBeforeNonMatchOffset,
                  byteOffsetOfVocable);
              }
              AddAllOffsetsOfMatchingWords(
                /*closestBeforeNonMatchOffset*/ byteOffsetOfVocable,
                psvStringToSearch,
  //              psvDictFile,
                r_dwTokenIndex,
                numToken,
                byteOffsetsOfVocData
                );
  //            return;
              breakWhile = true;
            }
            else
            {
              if( comp == PositionStringVector::greater ||
			    comp == PositionStringVector::tooFewTokensInComparisonVector)
              {
			    closestBeforeNonMatchOffset= byteOffsetOfVocable;
			    lo = byteOffsetOfVocable;
              }
              CompareVectors(
                comp,
                psvStringToSearch,
                hi, lo,
                endSearchForCompareStringInCurrentVocData,
  //                    numTokenForStringToSearch,
                numToken,
                psvDictFile,
                r_dwTokenIndex,
                byteOffset,
                closestBeforeNonMatchOffset,
                breakWhile);
              LOGN_DEBUG("seeking to dict file offset " << byteOffset)
              //TODO error handling on seek failure.
              /** Throws DictionaryFileAccessException on seek failure. */
              streamIsGood = SetDictDataOffset(byteOffset);
           }
            if( breakWhile )
              break;
//            byteOffsetOfVocable = GetByteOffsetOfNextVocDataBegin(
//              endSearchForCompareStringInCurrentVocData);
            streamIsGood = byteOffsetOfVocable != UINT_MAX;
          }
          catch(const EndOfFileException & e)
          {
            break;
          }
          catch(FileReadException & e)
          {
            if( e.GetResult() == I_File::endOfFileReached )
              break;
            else
              throw DictionaryFileAccessException();
          }
//          psvDictFile.clear();
//          m_englishDictionary.seekg(byteOffset, std::ios_base::beg);
        }//while(1);
      }

//      if(/*m_p_vocabularyAccess*/ m_p_vocaccess)
//      {
//        std::set<VocabularyAndTranslation *> * p =
//          /*m_p_vocabularyAccess*/ m_p_vocaccess->find(psvStringToSearch,
//            r_dwTokenIndex);
//        return p;
////        VocablesForWord::voc_container_type * p_voc_container =
////            m_charStringStdMapTemp.find(psv, r_dwTokenIndex);
////        if( p_voc_container )
////        {
////          return
////        }
//      }
      LOGN_DEBUG("return")
//      return p_voc_container;
    }

    bool BinarySearchInDictData::GetByteOffsetOfFirstVocable(
      fastestUnsignedDataType & byteOffsetOfVocable,
      bool & endSearchForCompareStringInCurrentVocData
      )
    {
      bool successfullyGotByteOffsetOfFirstVocable = false;
      try
      {
        byteOffsetOfVocable = GetByteOffsetOfNextVocDataBegin(
          endSearchForCompareStringInCurrentVocData);
        successfullyGotByteOffsetOfFirstVocable = true;
      }catch( const EndOfFileException & e )
      {
//        //EOF or read error.
//        if( byteOffsetOfVocable == UINT_MAX )
        {
          SetDictDataOffset(0);
          byteOffsetOfVocable = 0;
        }
      }
      return successfullyGotByteOffsetOfFirstVocable;
    }
    
    void BinarySearchInDictData::AddAllOffsetsOfMatchingWords(
      const fastestUnsignedDataType /*c_closestBeforeNonMatchOffset*/ byteOffsetOfFirstMatchingVocable,
      const PositionStringVector & psvStringToSearch,
//      const PositionStringVector & psvDictFile,
      DWORD & r_dwTokenIndex,
      const fastestUnsignedDataType numToken,
      std::set<fastestUnsignedDataType> & byteOffsetsOfVocData
      )
    {
      LOGN_DEBUG("begin--byte offs. of 1st match voc:" 
        << byteOffsetOfFirstMatchingVocable)
      fastestUnsignedDataType byteOffsetOfVocable = 0;
      bool endSearchForCompareStringInCurrentVocData;
//      if( c_closestBeforeNonMatchOffset == 0)
//      {
//        //TODO
//        LOGN_WARNING("c_closestBeforeNonMatchOffset is 0->may read up to <size of dict> bytes")
//      }
//      else
//        byteOffsetOfVocable = GetByteOffsetOfNextVocDataBegin(
//          endSearchForCompareStringInCurrentVocData);

//      p_voc_container = extractVocable(byteOffsetOfVocable, p_vocandtransl);
//      m_englishDictionary.seekg(c_closestBeforeNonMatchOffset, std::ios_base::beg);
      //TODO error handling on seek failure.
      bool streamIsGood;// = SeekFilePointerPosition(c_closestBeforeNonMatchOffset);
#ifdef _DEBUG
//      fastestUnsignedDataType currOffset = m_englishDictionary.tellg();
      fastestUnsignedDataType currOffset = GetCurrentDictDataOffset();
#endif
      fastestUnsignedDataType closestBeforeNonMatchOffset;
      PositionStringVector::cmp comp = PositionStringVector::lower;
      byteOffsetOfVocable = byteOffsetOfFirstMatchingVocable;
      LOGN_DEBUG("adding " << byteOffsetOfVocable
        << " to voc data byte offset vector")
      byteOffsetsOfVocData.insert(byteOffsetOfFirstMatchingVocable);

      /** Force seek to next vocable by adding a positive value.*/
      byteOffsetOfVocable = byteOffsetOfFirstMatchingVocable + 1;
//      fastestUnsignedDataType closestBeforeNonMatchOffset;
      do
      {
        streamIsGood = SetDictDataOffset(byteOffsetOfVocable);
          byteOffsetOfVocable = GetByteOffsetOfNextVocDataBegin(
            endSearchForCompareStringInCurrentVocData, byteOffsetOfVocable);
//        else
        {
          /** The file pointer may be beyond the next voc data offset->seek */
          LOGN_DEBUG("seeking to byte offset " << byteOffsetOfVocable)
//          m_englishDictionary.seekg(byteOffsetOfVocable, std::ios_base::beg);
          //TODO error handling on seek failure.
          streamIsGood = SetDictDataOffset(byteOffsetOfVocable);
          if( ! streamIsGood )
          {
            break;
          }
          comp = ContainsEnglishWord(
            psvStringToSearch,
            r_dwTokenIndex,
            numToken,
//            byteOffsetsOfVocData,
            /*(fastestUnsignedDataType) */ closestBeforeNonMatchOffset);
          if( comp == PositionStringVector::match )
          {
            LOGN_DEBUG("adding " << byteOffsetOfVocable
              << " to voc data byte offset vector")
            byteOffsetsOfVocData.insert(byteOffsetOfVocable);
            /** Start from next. */
            byteOffsetOfVocable += 1;
          }
        }
        if( byteOffsetOfVocable == UINT_MAX )
        {
          LOGN_ERROR("byte offset of voc data begin not found")
          break;
        }
      }while(comp == PositionStringVector::match); // || comp == PositionStringVector::greater );
      LOGN_DEBUG("end")
    }


    }
  }
}
