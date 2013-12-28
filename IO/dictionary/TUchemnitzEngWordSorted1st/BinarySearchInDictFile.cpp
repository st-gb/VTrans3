/*
 * TUchemnitzDictEngWord1stReader.cpp
 *
 *  Created on: Dec, 2013
 *      Author: Stefan
 */

#include "BinarySearchInDictFile.hpp"
#include <InputOutput/GetCharInACSIIcodePage850.hpp>
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>//class IVocabularyInMainMem
/** SAME_VOCABLE_SEPERATOR_CHAR etc. */
#include <IO/dictionary/TUchemnitz/TUchemnitzDictSeparatorChars.h>
#include <limits.h> //UINT_MAX
#include <vector> //class std::vector
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)
#include <compiler/GCC/enable_disable_write_strings_warning.h> //GCC_DIAG_OFF

#define INDEX_OF_LAST_SMALL_LETTER_IN_ASCII 128

NodeTrie<enum TUchemnitzDictionary::wordKinds>
  DictionaryReader::TUchemnitzEngWordSorted1st::BinarySearchInDictFile::s_nodetrieWordKind(
    INDEX_OF_LAST_SMALL_LETTER_IN_ASCII,
    TUchemnitzDictionary::not_set);

namespace DictionaryReader
{
  namespace TUchemnitzEngWordSorted1st
  {
    BinarySearchInDictFile::BinarySearchInDictFile(
        IVocabularyInMainMem & vocaccess)
      : DictionaryReaderBase(& vocaccess)
    {
      addTrieNodes();
    }

    BinarySearchInDictFile::~BinarySearchInDictFile()
    {
      LOGN_DEBUG("end")
    }

    void BinarySearchInDictFile::addTrieNodes()
    {
//      s_p_i_userinterface = & i_userinterface;
      if( s_nodetrieWordKind.size() == 0 )
      {
    //    Build();
        //Wortart: adj, adv, vi=verb intrans. vr=verb reflexiv.
        std::string wordKind = "adj";
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::adj);
        wordKind = "adv";
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::adv);
        wordKind = "m";
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::mascNoun);
        wordKind = "f";
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::femNoun);
        wordKind = "n";
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::neutralNoun);
        wordKind = "pl";
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::pluralNoun);
        wordKind = "vi"; //="Verb Intransitive"
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::intransitiveVerb);
        wordKind = "vt"; //="Verb Transitive"
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::transitiveVerb);
      }
    }

    /** Do not load anything into memory. */
    bool BinarySearchInDictFile::open(const std::string & std_str )
    {
      LOGN_DEBUG("Opening file " << std_str)
      m_englishDictionary.open(std_str.c_str() );
      bool dictFileIsOpen = m_englishDictionary.is_open();
      if( dictFileIsOpen )
      {
        LOGN_DEBUG("Dict file is open")
        m_englishDictionary.seekg(0, std::ios_base::end);
        m_fileSizeInBytes = m_englishDictionary.tellg();
        m_englishDictionary.seekg(0, std::ios_base::beg);
      }
      return dictFileIsOpen;
    }

    //          switch(ch)
    //          {
    //            case ENGLISH_GERMAN_SEPERATOR_CHAR :
    //            case '('
    //            case SYNONYM_SEPERATOR_CHAR :
    //            case SAME_VOCABLE_SEPERATOR_CHAR :
    //              englishGermanSeperatorCharOccurred = true;
    //              word[charIndex ++] = '\0';
    //              break;
    //            case '\r':
    //  //          case '\n':
    //              newLine = true;
    //              break;
    //              if(englishGermanSeperatorCharOccurred)
    //              char * pch = new char[charIndex];
    //              memcpy(pch, word, charIndex + 1);
    //              m_p_vocabularyAccess->InsertAsKeyAndAddVocabularyAttributes(
    //                word,
    //                );
    //              charIndex = 0;
    //              break;
    //            default:
    //              word[charIndex ++] = ch;
    //              break;
    //          }
    //          if(englishGermanSeperatorCharOccurred)
    //          {
    //            psv.at(0);
    //          }
    //          i = m_arbyCharValueToArrayIndex[ch];
    //          p_BytePosAndNextChar = m_BytePosAndNextChar[i];
    //          p_BytePosAndNextChar->m_bytePos = filePosInByte;
    //          ++ filePosInByte;
    //          i = m_englishDictionary.get();
    //        }

//    VocabularyAndTranslation *
    IVocabularyInMainMem::voc_container_type * BinarySearchInDictFile::AddVocable(
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
      char word[100],
      const fastestUnsignedDataType charIndex,
//      std::vector<std::vector<std::string> > & englishVocables,
      std::vector<std::string> & englishVocableWords,
      /** e.g. synonyms */
      std::vector< std::vector <std::string> > & germanVocables,
      unsigned & semicolCountInsideCurrentPipeCharRange,
      fastestUnsignedDataType & synonymIndex//,
      )
    {
      LOGN_DEBUG("begin--add English word?:" << (english ? "yes" : "no") )
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
          if( semicolCountInsideCurrentPipeCharRange < germanVocables.size() )
          {
            LOGN_DEBUG("using German vocable at index "
              << semicolCountInsideCurrentPipeCharRange )
            std::vector<std::string> & vec = germanVocables[
              semicolCountInsideCurrentPipeCharRange];
            vec.push_back(std::string(word) );
          }
          else
          {
            LOGN_DEBUG("appending German vocable at index "
              << germanVocables.size() )
            std::vector<std::string> vec;
            vec.push_back(std::string(word) );
            germanVocables.push_back( vec);
          }
  //        germanVocableWords.push_back(word);
        }
        word[0] = '\0';
      }
//      LOGN_DEBUG("end")
    }

    void BinarySearchInDictFile::HandleSynonymSeparatorChar(
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
      BinarySearchInDictFile::HandleClosingBrace(//int i,
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
        NodeTrieNode<
            enum TUchemnitzDictionary::wordKinds>* p_ntn =
            s_nodetrieWordKind.contains_inline((BYTE*) ((wordKind)),
                numWordKindChars /*- 1*/, true);
        //            delete [] wordKind;
        kindOfWordStart = 0;
        //#endif
        if (p_ntn)
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
            e_wordKind = p_ntn->m_member;
          }
      }
//      return i;
      LOGN_DEBUG("return " << e_wordKind << wordKind )
      return /*BinarySearchInDictFile::not_set*/ e_wordKind;
    }

    void BinarySearchInDictFile::AddGermanAttributes(
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
      std::vector< std::vector <std::string> >::const_iterator c_iterGerman =
        germanVocables.begin();
      std::vector<std::string>::const_iterator c_iterGermanWord;
      while( c_iterGerman != germanVocables.end() && c_iterPipeIndex2VocAndTranslPtr !=
          voc_containerVocsCreated.end() )
      {
        const std::vector<std::string> & r_ = ( * c_iterGerman);
        c_iterGermanWord = r_.begin();
        VocabularyAndTranslation * p_vocAndTransl = c_iterPipeIndex2VocAndTranslPtr->second;
        const int index = c_iterPipeIndex2VocAndTranslPtr->first;

        EnglishWord::English_word_class engWordClass = EnglishWord::
            MapGrammarPartIDtoWordClass(p_vocAndTransl->m_englishWordClass);
        if( p_vocAndTransl && /*p_vocAndTransl->m_englishWordClass*/ engWordClass <= EnglishWord::adjective )
        {
          const VocabularyAndTranslation::ArraySizes & arraySizes =
            VocabularyAndTranslation::s_arraysizes[engWordClass];
          fastestUnsignedDataType germanWordIndex = 0;
          while(c_iterGermanWord != r_.end() )
          {
            if( germanWordIndex < arraySizes.m_byArraySizeForGermanWord )
            {
              const std::string & germanWord = * c_iterGermanWord;
              LOGN_DEBUG("setting German word \"" << germanWord
                << "\" for index " << germanWordIndex
                << " for pipe index " << index
                << " ,vocl&transl ptr " << p_vocAndTransl)
              p_vocAndTransl->SetGermanWord(germanWord.c_str(),
                  germanWord.length(), germanWordIndex);
              ++ germanWordIndex;
            }
            ++ c_iterGermanWord;
          }
        }
        ++ c_iterPipeIndex2VocAndTranslPtr;
        ++ c_iterGerman;
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

    inline void SetWordCharacter(char word[100],
        const fastestUnsignedDataType charIndexInsideWord,
        const char ch)
    {
      if(charIndexInsideWord < 100)
        word[charIndexInsideWord] = ch;
#ifdef _DEBUG
      else
      {
        int i = ch;
      }
#endif
    }

    /** e.g. "schoolbook; textbook; educational book | schoolbooks; textbooks; educational books :: Lehrbuch {n} | Lehrbücher {pl} "
     * */
    IVocabularyInMainMem::voc_container_type *
      BinarySearchInDictFile::extractVocable(
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
      int currOffset = m_englishDictionary.tellg();
#endif
      m_englishDictionary.seekg(offset, std::ios_base::beg);
      LOGN_DEBUG("after seeking to offset " << offset)
      int i;
      i = m_englishDictionary.get();
      bool streamIsGood = m_englishDictionary.good();
      bool breakWhile = false;
      char word[100];
      int prevChar = 0;
      std::vector<std::string> englishVocableWords;
      std::vector<std::vector<std::string> > englishVocables;
      fastestUnsignedDataType synonymIndex = 0;
      bool english = true;
      fastestUnsignedDataType kindOfWordStart = 0;
      fastestUnsignedDataType charIndex = 0, charIndexInsideWord = 0;
      char wordKind[/*5*/ 100];
      bool insideBracket = false;
      char ASCIIcodePage850Char;
      char ch;
      while( streamIsGood /*&& ! breakWhile*/ )
      {
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
            if(prevChar > 127 )
            {
              ASCIIcodePage850Char = GetCharInASCIIcodePage850(i);
              if( ASCIIcodePage850Char != 0 )
                SetWordCharacter(word,/*charIndex*/ charIndexInsideWord,
                  ASCIIcodePage850Char);
              else
                SetWordCharacter(word,/*charIndex*/ charIndexInsideWord, i);
            }
            else
            {
              ch = i;
              SetWordCharacter(word,/*charIndex*/ charIndexInsideWord, ch);
            }
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
        i = m_englishDictionary.get();
        streamIsGood = m_englishDictionary.good();
#ifdef _DEBUG
        currOffset = m_englishDictionary.tellg();
#endif
      } //while
      AddGermanAttributes(voc_containerVocsCreated, germanVocables);
      englishVocables.clear();
      IVocabularyInMainMem::OutputVocs(p_voc_container);
      LOGN_DEBUG("return " << p_voc_container)
      return p_voc_container;
    }

    bool BinarySearchInDictFile::CompareVectors(
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
      switch (comp)
        {
      //                  case PositionStringVector::match :
      //                    /*p_vocandtransl*/
      //                    p_voc_container = extractVocable(byteOffsetOfVocable, p_vocandtransl);
      ////                    if( p_vocandtransl != NULL)
      ////                      p_voc_container->
      //                    break;
      /** String to search is lower than string in dictionary. */
      case PositionStringVector::tooFewTokens:
        endSearchForCompareStringInCurrentVocData;
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
        /** string to search is greater than string in dictionary-> dictionary
         *  string is before string to search->memorize the offset*/
        closestBeforeNonMatchOffset = byteOffset;
        psvDictFile.clear();
        lo = byteOffset;
        byteOffset = /*lo + (hi - lo )*/(lo + hi) / 2;
        LOGN_DEBUG(
            "string to search is > string in dict -> new offset:("
            << lo << "+" << hi << ")/2=" << byteOffset)
        endSearchForCompareStringInCurrentVocData = true;
        /** e.g. "vacuum" (string to search) is < "vacuum cleaner"
         *   */
        const fastestUnsignedDataType numTokenStringFromDictFile =
            psvDictFile.size();
        if (numTokenForStringToSearch <= numTokenStringFromDictFile)
          breakWhile = true;
        break;
        }
      if (hi < lo + 2)
        {
          LOGN_DEBUG("hi <= lo")
          //                    return NULL;
          breakWhile = true;
          //                    break;
        }
      return endSearchForCompareStringInCurrentVocData;
    }

    std::set<VocabularyAndTranslation *> * BinarySearchInDictFile::findEnglishWord(
      const PositionStringVector & psvStringToSearch,
      DWORD & r_dwTokenIndex
      )
    {
      IVocabularyInMainMem::voc_container_type * p_voc_container = NULL;
//      fastestUnsignedDataType byteOffsetOfVocable =
      VocabularyAndTranslation * p_vocandtransl = NULL;
      std::set<fastestUnsignedDataType> byteOffsetsOfVocData;
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
        if( byteOffsetsOfVocData.size() > 0 )
          byteOffsetsOfVocDataVec.push_back(byteOffsetsOfVocData);
        ++ numToken;
        if( r_dwTokenIndex + numToken > psvStringToSearch.size() )
          break;
      }while(byteOffsetsOfVocData.size() > 0 );
#ifdef _DEBUG
      const int vecsz = byteOffsetsOfVocDataVec.size();
      LOGN_DEBUG("# vectors:" << vecsz)
#endif
      /** Use vector containing the most tokens. */
      std::vector< std::set<fastestUnsignedDataType> >::const_reverse_iterator
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
          p_voc_container = extractVocable(
            byteOffsetOfVocable,
            //p_voc_container,
            p_vocandtransl);
          ++ c_iter;
        }
        //++ c_rev_iter;
      }
//      if()
      return /*NULL*/ p_voc_container;
    }

    inline void HandleEndOfToken(
      char word [],
      fastestUnsignedDataType & charIndex,
      PositionStringVector & psvDictFile,
      fastestUnsignedDataType & tokenIndex
      )
    {
      word[charIndex] = '\0';
      LOGN_DEBUG("word:" << word)
      charIndex = 0;
      VTrans::string_type str(word);
      PositionString pos_str(str, tokenIndex, tokenIndex);
      psvDictFile.push_back(pos_str);
    }

    void HandleEndOfWord(
      char word [],
      fastestUnsignedDataType & charIndex,
      PositionStringVector & psvDictFile,
      fastestUnsignedDataType & tokenIndex,
      bool & compareVectors
      )
    {
//      word[charIndex] = '\0';
//      charIndex = 0;
//      VTrans::string_type str(word);
//      PositionString pos_str(str, tokenIndex, tokenIndex);
//      psvDictFile.push_back(pos_str);
//      ++ tokenIndex;
      LOGN_DEBUG("begin")
      HandleEndOfToken(word, charIndex, psvDictFile, tokenIndex);
      compareVectors = true;
    }

    void BinarySearchInDictFile::AddAllOffsetsOfMatchingWords(
      const fastestUnsignedDataType c_closestBeforeNonMatchOffset,
      const PositionStringVector & psvStringToSearch,
//      const PositionStringVector & psvDictFile,
      DWORD & r_dwTokenIndex,
      const fastestUnsignedDataType numToken,
      std::set<fastestUnsignedDataType> & byteOffsetsOfVocData
      )
    {
//      p_voc_container = extractVocable(byteOffsetOfVocable, p_vocandtransl);
      m_englishDictionary.seekg(c_closestBeforeNonMatchOffset, std::ios_base::beg);
#ifdef _DEBUG
      fastestUnsignedDataType currOffset = m_englishDictionary.tellg();
#endif
      fastestUnsignedDataType closestBeforeNonMatchOffset;
      fastestUnsignedDataType byteOffsetOfVocable;
      bool endSearchForCompareStringInCurrentVocData;
      PositionStringVector::cmp comp = PositionStringVector::lower;
//      fastestUnsignedDataType closestBeforeNonMatchOffset;
      do
      {
        byteOffsetOfVocable = GetByteOffsetOfVocDataBegin(
          endSearchForCompareStringInCurrentVocData);
        if( byteOffsetOfVocable != UINT_MAX )
        {
          m_englishDictionary.seekg(byteOffsetOfVocable, std::ios_base::beg);

          comp = ContainsEnglishWord(
            psvStringToSearch,
            r_dwTokenIndex,
            numToken,
//            byteOffsetsOfVocData,
            /*(fastestUnsignedDataType) */ closestBeforeNonMatchOffset);
          if( comp == PositionStringVector::match )
          {
            byteOffsetsOfVocData.insert(byteOffsetOfVocable);
          }
        }
      }while(comp == PositionStringVector::match || comp == PositionStringVector::greater );
    }

    fastestUnsignedDataType BinarySearchInDictFile::GetByteOffsetOfVocDataBegin(
        bool & endSearchForCompareStringInCurrentVocData)
    {
      fastestUnsignedDataType byteOffsetOfVocable = UINT_MAX;
      int i = m_englishDictionary.get();
      char ch;
      bool streamIsGood = m_englishDictionary.good();
      while(//i != std::ios_base::eof
          streamIsGood )
      {
        ch = i;
        if(ch == 0xA /*'\n'*/ )
        {
          byteOffsetOfVocable = m_englishDictionary.tellg();
          LOGN_DEBUG("offset of voc data begin:" << byteOffsetOfVocable)
          endSearchForCompareStringInCurrentVocData = false;
          break;
        }
        i = m_englishDictionary.get();
        streamIsGood = m_englishDictionary.good();
      }
      return byteOffsetOfVocable;
    }

    /** @brief called to determine whether a specfic word exists in the
     *  dictionary file. */
    PositionStringVector::cmp BinarySearchInDictFile::ContainsEnglishWord(
      const PositionStringVector & psvStringToSearch,
      DWORD & r_dwTokenIndex,
      const fastestUnsignedDataType numTokenForStringToSearch,
//      std::set<fastestUnsignedDataType> & byteOffsetsOfVocData,
      fastestUnsignedDataType & closestBeforeNonMatchOffset
      )
    {
  #ifdef _DEBUG
      fastestUnsignedDataType currOffset = m_englishDictionary.tellg();
  #endif
      bool streamIsGood;
      PositionStringVector psvDictFile;
      bool breakWhile = false;
      char word[100];
      fastestUnsignedDataType charIndex = 0;
      bool endSearchForCompareStringInCurrentVocData = false;
      fastestUnsignedDataType tokenIndex = 0;
      bool compareVectors = false;
      PositionStringVector::cmp comp;

      int i = m_englishDictionary.get();
      streamIsGood = m_englishDictionary.good();
      while( streamIsGood /*&& ! breakWhile*/ )
      {
        if( ::isalpha(i) )
        {
          word[charIndex ++] = i;
        }
        else
          switch( i )
          {
            case ' ' :
            {
  #ifdef _DEBUG
              currOffset = m_englishDictionary.tellg();
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
            endSearchForCompareStringInCurrentVocData =
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
              LOGN_DEBUG("return value of comparison: " << PositionStringVector:://GetResultAsString(comp)
                s_comparisonResultString[comp] )
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
            psvDictFile.clear();
            compareVectors = false;
          }
  //            } //while()
        if( breakWhile || endSearchForCompareStringInCurrentVocData)
          break;
        i = m_englishDictionary.get();
        streamIsGood = m_englishDictionary.good();
//        psvDictFile.clear();
//        m_englishDictionary.seekg(byteOffset, std::ios_base::beg);
      } //while loop for current voc data
      return comp;
    }

    /** @brief indirectly called when */
    void BinarySearchInDictFile::findEnglishWord(
      const PositionStringVector & psvStringToSearch,
      DWORD & r_dwTokenIndex,
      const fastestUnsignedDataType numToken,
      std::set<fastestUnsignedDataType> & byteOffsetsOfVocData
      )
    {
      LOGN_DEBUG("# token: " << numToken)
//      IVocabularyInMainMem::voc_container_type * p_voc_container = NULL;
      if( psvStringToSearch.size() > 0 )
      {
        PositionStringVector psvDictFile;
//        VocabularyAndTranslation * p_vocandtransl = NULL;
        const fastestUnsignedDataType numTokenForStringToSearch =
          psvStringToSearch.size();

        fastestUnsignedDataType byteOffsetOfVocable = UINT_MAX;
        fastestUnsignedDataType closestBeforeNonMatchOffset = UINT_MAX;
//        int i;
//        char ch;
#ifdef _DEBUG
        fastestUnsignedDataType currOffset = m_englishDictionary.tellg();
#endif
        bool breakWhile = false;
        bool endSearchForCompareStringInCurrentVocData = false;

        fastestUnsignedDataType byteOffset = m_fileSizeInBytes / 2;
        fastestUnsignedDataType lo = 0, hi = m_fileSizeInBytes;
        m_englishDictionary.seekg(byteOffset, std::ios_base::beg);
        bool streamIsGood = m_englishDictionary.good();
        while( streamIsGood)
        {
  //        BytePosAndNextChar * p_BytePosAndNextChar;
          bool newLine = false;
          bool englishGermanSeperatorCharOccurred = false;

          byteOffsetOfVocable = GetByteOffsetOfVocDataBegin(
            endSearchForCompareStringInCurrentVocData);

          fastestUnsignedDataType closestBeforeNonMatchOffset;
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
            AddAllOffsetsOfMatchingWords(
              closestBeforeNonMatchOffset,
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
            m_englishDictionary.seekg(byteOffset, std::ios_base::beg);
          }
          if( breakWhile )
            break;
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

    //TODO idea: because words are sorted lexicographically:
    // store english words: at 1st store number of 1st identical
    // characters:
    //  antimatter
    //  antimephitic
    // ->if antimatter is the 1st word inside the dictionary, store:
    // "antimatter", "5"(# 1st identical characters),"ephitic"
    void BinarySearchInDictFile::read()
    {
//  //    m_bytePos[letter] = bytePos;
//      unsigned filePosInByte = 0;
//      char ch;
//      int i;
//      i = m_englishDictionary.get();
//      char word[100];
////      BytePosAndNextChar * p_BytePosAndNextChar;
//      fastestUnsignedDataType charIndex = 0;
//      bool newLine = false;
//      bool englishGermanSeperatorCharOccurred = false;
//      while(i != eof)
//      {
//        ch = i;
//        switch(ch)
//        {
//          case ENGLISH_GERMAN_SEPERATOR_CHAR :
//            englishGermanSeperatorCharOccurred = true;
//          case '\r':
//          case '\n':
//            newLine = true;
//          case SYNONYM_SEPERATOR_CHAR :
//          case SAME_VOCABLE_SEPERATOR_CHAR :
//            if(englishGermanSeperatorCharOccurred)
//            word[charIndex] = '\0';
////            char * pch = new char[charIndex];
//            memcpy(pch, word, charIndex + 1);
////            m_p_vocabularyAccess->InsertAsKeyAndAddVocabularyAttributes(
////              word,
////              );
//            charIndex = 0;
//            break;
//          default:
//            word[charIndex ++] = ch;
//            break;
//        }
//        i = m_arbyCharValueToArrayIndex[ch];
//        p_BytePosAndNextChar = m_BytePosAndNextChar[i];
//        p_BytePosAndNextChar->m_bytePos = filePosInByte;
//        ++ filePosInByte;
//        i = m_englishDictionary.get();
//      }
    }

//    /** Inserts the string as a key value to the container and adds the string
//     *  as an attribute value.*/
//    VocabularyAndTranslation * InsertAsKeyAndAddVocabularyAttributes(
//      const char * ar_chWordBegin,
//      unsigned stringLen,
//      enum EnglishWord::English_word_class word_class
//      )
//    {
//      /*void * p_v =*/ return Insert(ar_chWordBegin, stringLen, word_class, NULL);
//  //      AddVocabularyAttributes( word_class, p_v);
//    }
//
//    /** Save memory by referring to existing voc attributes. */
//    VocabularyAndTranslation * InsertAsKeyAndReferToExistingVocData(
//      enum EnglishWord::English_word_class word_class,
//      const char * ar_chWordBegin,
//      unsigned stringLen,
//      const VocabularyAndTranslation * const p_vocandtranslAllocated
//      )
//    {
//      VocabularyAndTranslation * p_vocandtransl =
//        Insert(ar_chWordBegin, stringLen, word_class, NULL);
//      if( p_vocandtransl )
//      {
//        p_vocandtransl->PointToAttributeData(p_vocandtranslAllocated);
//        p_vocandtransl->m_englishWordClass = word_class;
//      }
//      return p_vocandtransl;
//    };
  }
} /* namespace DictionaryReader */
