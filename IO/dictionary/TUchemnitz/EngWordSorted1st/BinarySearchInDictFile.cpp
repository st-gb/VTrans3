/*
 * TUchemnitzDictEngWord1stReader.cpp
 *
 *  Created on: Dec, 2013
 *      Author: Stefan
 */

#include "BinarySearchInDictFile.hpp"
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
#include <Controller/GetLastErrorCode.hpp>

#define INDEX_OF_LAST_SMALL_LETTER_IN_ASCII 128

NodeTrie<enum TUchemnitzDictionary::wordKinds>
  DictionaryReader::TUchemnitz::EngWordSorted1st::BinarySearchInDictFile::s_nodetrieWordKind(
    INDEX_OF_LAST_SMALL_LETTER_IN_ASCII,
    TUchemnitzDictionary::not_set);

namespace DictionaryReader
{
  namespace TUchemnitz {
  namespace EngWordSorted1st
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
        try
        {
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
        }catch( const NS_NodeTrie::RootNodeNotInitalizedException & e)
        {
          LOGN_ERROR("NS_NodeTrie::RootNodeNotInitalizedException")
        }
      }
    }

    /** Do not load anything into memory. */
    bool BinarySearchInDictFile::open(const std::string & std_strDictFilePath )
    {
      LOGN_DEBUG("Opening file " << std_strDictFilePath)
//      m_englishDictionary.open(std_strDictFilePath.c_str(),
//        std::ios_base::in | std::ios_base::binary );
      enum I_File::OpenError openError = m_dictFile.OpenA(
        std_strDictFilePath.c_str(),
        I_File::readOnly
        );
//      bool dictFileIsOpen = m_englishDictionary.is_open();
      bool dictFileIsOpen = openError == I_File::success;
      if( /*dictFileIsOpen*/ openError == I_File::success )
      {
        LOGN_DEBUG("Dict file is open")
//        m_englishDictionary.seekg(0, std::ios_base::end);
//        m_fileSizeInBytes = m_englishDictionary.tellg();
        m_fileSizeInBytes = m_dictFile.GetFileSizeInBytes();
        if( m_fileSizeInBytes == -1 )
        {
          const DWORD lastError = OperatingSystem::GetLastErrorCode();
          DictionaryFileAccessException dictionaryFileAccessException(
            DictionaryFileAccessException::getFileSize, lastError,
            std_strDictFilePath.c_str() );
  	      LOGN_ERROR("error getting dictionary file size->throwing an exception")
          throw dictionaryFileAccessException;
        }
//        m_englishDictionary.seekg(0, std::ios_base::beg);
//        m_dictFile.Seek(0);
      }
      else //Or throw enum I_File::OpenError openError
      {
	    LOGN_ERROR("error loading dictionary->throwing an exception")
        throw VTrans3::OpenDictFileException(openError);
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

    int BinarySearchInDictFile::ReadByte()
    {
     int readByteReturnValue;
      try
      {
        readByteReturnValue = m_dictFile.ReadByte();
      }
      catch( const FileReadException & fileReadException ) {
        /** Re-throw a DictionaryFileAccessException to indicate that the 
         *  exception was caused while accessing a _dictionary_ file. */
        DictionaryFileAccessException dictionaryFileAccessException(
          fileReadException,
          DictionaryFileAccessException::read
//          lastOSerrorCode,
//            m_dictFile.
//          filePath
          );
        throw dictionaryFileAccessException;
      }
      return readByteReturnValue;
    }

    int BinarySearchInDictFile::GetCurrentFilePointerPosition()
    {
      LOGN_DEBUG("begin")
      I_File::file_pointer_type currOffset = m_dictFile.GetCurrentFilePointerPosition();
      if( currOffset == -1 )
      {
        const DWORD lastOSerrorCode = OperatingSystem::GetLastErrorCode();
        const char * const filePath = m_dictFile.GetFilePathA().c_str();
        DictionaryFileAccessException dictionaryFileAccessException(
          DictionaryFileAccessException::getCurrentFilePointerPosition,
          lastOSerrorCode,
//            m_dictFile.
          filePath
          );
        const std::string errorMessage = dictionaryFileAccessException.GetErrorMessageA();
        LOGN_ERROR(//"error seeking file pointer to offset " << byteOffset
          errorMessage
          );
        throw dictionaryFileAccessException;
      }
      LOGN_DEBUG("return " << currOffset)
      return currOffset;
    }

    void BinarySearchInDictFile::GetStatistics(
//        fastestUnsignedDataType * representations,
//        const fastestUnsignedDataType numArrayEles
      std::map<enum EnglishWord::English_word_class, unsigned> &
        englishWordClass2CounterMap
      )
    {
     m_p_englishWordClass2CounterMap = & englishWordClass2CounterMap;
      SeekFilePointerPosition(0);
      char wordType[100];
      fastestUnsignedDataType numWordKindChars = 0;

      I_File::file_pointer_type fileOffset;
      int currentFileByte;
      bool afterOpeningCurlyBrace = false;
      VocabularyAndTranslation * pVocabularyAndTranslation = NULL;
      do
      {
        currentFileByte = ReadByte();
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
            NodeTrieNode<enum TUchemnitzDictionary::wordKinds> * p_ntn =
            s_nodetrieWordKind.contains_inline( (BYTE *) (wordType),
                numWordKindChars /*- 1*/, true);
            if( p_ntn )
            {
              const enum EnglishWord::English_word_class word_class = 
                getEnglishWordClassFromTUchemnitzDictWordClass(p_ntn->m_member, 
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
             wordType[numWordKindChars ++] = currentFileByte;
           }
        }
        fileOffset = m_dictFile.GetCurrentFilePointerPosition();
      }while( fileOffset < m_fileSizeInBytes );
      m_p_vocaccess->GetUserInterface()->EndedGetDictStats();
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
      int currOffset = //m_englishDictionary.tellg();
        GetCurrentFilePointerPosition();
#endif
//      m_englishDictionary.seekg(offset, std::ios_base::beg);
      /** SeekFilePointerPosition Should throw an error on failure. */
      //TODO handle exceptions?
      bool streamIsGood = SeekFilePointerPosition(offset);
      LOGN_DEBUG("after seeking to offset " << offset)
      int i;
//      i = m_englishDictionary.get();
      /** ReadByte Should throw an error on failure. */
      i = ReadByte();
//      bool streamIsGood = m_englishDictionary.good();
//      streamIsGood = i > -1;
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
        i = ReadByte();
//        streamIsGood = m_englishDictionary.good();
        streamIsGood = i > -1;
#ifdef _DEBUG
//        currOffset = m_englishDictionary.tellg();
        currOffset = GetCurrentFilePointerPosition();
#endif
      } //while
      AddGermanAttributes(voc_containerVocsCreated, germanVocables);
      englishVocables.clear();
      IVocabularyInMainMem::OutputVocs(p_voc_container);
      LOGN_DEBUG("return " << p_voc_container)
      return p_voc_container;
    }

    /** @param comp: lower: word to search is before current word in dictionary*/
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
      typedef std::vector< std::set<fastestUnsignedDataType> > offsetVec;

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
      /** Use vector containing the most tokens.
       *  Must use "reverse_iterator" rather than "const_reverse_iterator"
       *  for Android NDK. */
      offsetVec::reverse_iterator
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

    void BinarySearchInDictFile::AddAllOffsetsOfMatchingWords(
      const fastestUnsignedDataType /*c_closestBeforeNonMatchOffset*/ byteOffsetOfFirstMatchingVocable,
      const PositionStringVector & psvStringToSearch,
//      const PositionStringVector & psvDictFile,
      DWORD & r_dwTokenIndex,
      const fastestUnsignedDataType numToken,
      std::set<fastestUnsignedDataType> & byteOffsetsOfVocData
      )
    {
      LOGN_DEBUG("begin")
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
      fastestUnsignedDataType currOffset = GetCurrentFilePointerPosition();
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
          byteOffsetOfVocable = GetByteOffsetOfNextVocDataBegin(
            endSearchForCompareStringInCurrentVocData, byteOffsetOfVocable);
//        else
        {
          LOGN_DEBUG("seeking to byte offset " << byteOffsetOfVocable)
//          m_englishDictionary.seekg(byteOffsetOfVocable, std::ios_base::beg);
          //TODO error handling on seek failure.
          streamIsGood = SeekFilePointerPosition(byteOffsetOfVocable);
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

    fastestUnsignedDataType BinarySearchInDictFile::GetByteOffsetOfNextVocDataBegin(
        bool & endSearchForCompareStringInCurrentVocData,
		fastestUnsignedDataType currentFileOffsetOrig )
    {
      LOGN_DEBUG("begin")
#ifdef DEBUG
      fastestUnsignedDataType byteOffsetOfVocable = UINT_MAX;
#endif
      fastestUnsignedDataType currentFileOffset = currentFileOffsetOrig;
//      int i = m_englishDictionary.get();
//      fastestUnsignedDataType numBytesRead = 0;
      int i = ReadByte();
      ++ currentFileOffset;
//      char ch;
//      bool streamIsGood = m_englishDictionary.good();
//      bool streamIsGood = i == I_File::successfullyRead;
      while(//i != std::ios_base::eof
//          streamIsGood 
        //true
        i != 0xA /* <> '\n'*/
        )
      {
//        i = m_englishDictionary.get();
        i = ReadByte();
//        ++ numBytesRead;
        ++ currentFileOffset;
//        streamIsGood = m_englishDictionary.good();
//        streamIsGood = i == I_File::successfullyRead;
      }
      if( i == 0xA /*'\n'*/ )
      {
#ifdef DEBUG
  		byteOffsetOfVocable = GetCurrentFilePointerPosition();
  		if( currentFileOffsetOrig == UINT_MAX )
  			currentFileOffset = byteOffsetOfVocable;
#else
  		if( currentFileOffsetOrig == UINT_MAX )
  			currentFileOffset = GetCurrentFilePointerPosition();
#endif
  		LOGN_DEBUG("offset of voc data begin:" << byteOffsetOfVocable)
  		endSearchForCompareStringInCurrentVocData = false;
      }
      LOGN_DEBUG("return " << byteOffsetOfVocable)
      return /*byteOffsetOfVocable*/ currentFileOffset;
    }

    /** @brief called to determine whether a specific word exists in the
     *  dictionary file. */
    PositionStringVector::cmp BinarySearchInDictFile::ContainsEnglishWord(
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
      fastestUnsignedDataType currOffset = GetCurrentFilePointerPosition();
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
      int i = ReadByte();
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
              currOffset = GetCurrentFilePointerPosition();
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
                GetCurrentFilePointerPosition();
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
        i = ReadByte();
//        streamIsGood = m_englishDictionary.good();
        streamIsGood = i > -1;
//        psvDictFile.clear();
//        m_englishDictionary.seekg(byteOffset, std::ios_base::beg);
      } //while loop for current voc data
      LOGN_DEBUG("return " << comp << " (" << 
        PositionStringVector::s_comparisonResultString[comp] << ")")
      return comp;
    }

    bool BinarySearchInDictFile::SeekFilePointerPosition(
      const fastestUnsignedDataType byteOffset)
    {
      LOGN_DEBUG("begin seek to offset " << byteOffset )
		const enum I_File::SeekResult seekResult = m_dictFile.SeekFilePointerPosition(byteOffset);
      const bool streamIsGood = seekResult == I_File::successfullySeeked;
      if( seekResult != I_File::successfullySeeked )
      {
        const DWORD lastOSerrorCode = OperatingSystem::GetLastErrorCode();
        const char * const filePath = m_dictFile.GetFilePathA().c_str();
        DictionaryFileAccessException dictionaryFileAccessException(
          DictionaryFileAccessException::seek,
          lastOSerrorCode,
//            m_dictFile.
          filePath,
          byteOffset
          );
        const std::string errorMessage = dictionaryFileAccessException.GetErrorMessageA();
        LOGN_ERROR(//"error seeking file pointer to offset " << byteOffset
          errorMessage
          );
        throw dictionaryFileAccessException;
      }
      LOGN_DEBUG("return " << streamIsGood << " (=stream is " <<
        (streamIsGood ? "good" : "bad") )
      return streamIsGood;
    }

    bool BinarySearchInDictFile::GetByteOffsetOfFirstVocable(
      fastestUnsignedDataType & byteOffsetOfVocable,
      bool & endSearchForCompareStringInCurrentVocData
      )
    {
      try
      {
        byteOffsetOfVocable = GetByteOffsetOfNextVocDataBegin(
          endSearchForCompareStringInCurrentVocData);
      }catch( const EndOfFileException & e )
      {
//        //EOF or read error.
//        if( byteOffsetOfVocable == UINT_MAX )
        {
          SeekFilePointerPosition(0);
          byteOffsetOfVocable = 0;
        }
      }
    }
    
    //TODO find 1st matching vocable in binary search
    //e.g.:2 dictionary entries: "car ............... car ......."
    // found: 2nd entry with "car"
    // no search in itnerval "[0...begin of 2nd car["
    fastestUnsignedDataType BinarySearchInDictFile::
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
      fastestUnsignedDataType byteOffsetOfFirstMatchingWord = byteOffsetOfVocable;
      
      fastestUnsignedDataType lowerBound = closestBeforeNonMatchOffset,
        higherBound = byteOffsetOfVocable;

      bool endSearchForCompareStringInCurrentVocData;
      fastestUnsignedDataType byteOffsetOfCurrentVocable;
      fastestUnsignedDataType byteOffsetOfNextVocDataBegin = 0;
      do
      {      
        byteOffsetOfCurrentVocable = lowerBound + (higherBound - lowerBound) / 2;
        SeekFilePointerPosition(byteOffsetOfCurrentVocable);

        byteOffsetOfNextVocDataBegin = GetByteOffsetOfNextVocDataBegin(
          endSearchForCompareStringInCurrentVocData, byteOffsetOfCurrentVocable);
        if( ! endSearchForCompareStringInCurrentVocData)
          SeekFilePointerPosition(byteOffsetOfNextVocDataBegin);

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
		  byteOffsetOfFirstMatchingWord = byteOffsetOfNextVocDataBegin;
          LOGN_DEBUG("setting higher bound to " << byteOffsetOfCurrentVocable)
          //byteOffsetOfFirstMatchingWord = byteOffsetOfCurrentVocable;
          higherBound = byteOffsetOfCurrentVocable /*byteOffsetOfVocable*/;
//          byteOffsetOfCurrentVocable = lowerBound + (higherBound - lowerBound) / 2;
        }
        /** String to search is lexicgraphically after CURRENT vocable in dict file. */
        else if( comp == PositionStringVector::greater )
        {
          LOGN_DEBUG("setting lower bound to " << byteOffsetOfCurrentVocable)
          lowerBound = //GetByteOffsetOfNextVocDataBegin(
            //endSearchForCompareStringInCurrentVocData);
            byteOffsetOfCurrentVocable;
//          byteOffsetOfCurrentVocable = lowerBound + (higherBound - lowerBound) / 2;
        }
        else if( comp == PositionStringVector::lower )
        {
          higherBound = byteOffsetOfCurrentVocable;
        }
      }while(higherBound > lowerBound + 6 /** Minimal 6 characters are needed for an entry: English_word::German:word */);
      LOGN_DEBUG("end")
      return /*byteOffsetOfNextVocDataBegin*/ byteOffsetOfFirstMatchingWord;
    }
    
    /** @brief indirectly called when */
    void BinarySearchInDictFile::findEnglishWord(
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
        fastestUnsignedDataType currOffset = GetCurrentFilePointerPosition();
#endif
        bool breakWhile = false;
        bool endSearchForCompareStringInCurrentVocData = false;

        fastestUnsignedDataType byteOffset = m_fileSizeInBytes / 2;
        fastestUnsignedDataType lo = 0, hi = m_fileSizeInBytes;
//        m_englishDictionary.seekg(byteOffset, std::ios_base::beg);
//        bool streamIsGood = m_englishDictionary.good();
        //TODO error handling on seek failure.
        bool streamIsGood = SeekFilePointerPosition(byteOffset);
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
              streamIsGood = SeekFilePointerPosition(byteOffset);
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
  }
} /* namespace DictionaryReader */
