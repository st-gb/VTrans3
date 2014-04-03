/*
 * EachAttributeInSingleLine.cpp
 *
 *  Created on: 03.01.2014
 *      Author: mr.sys
 */

#include "EachAttributeInSingleLine.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)
#include <compiler/GCC/enable_disable_write_strings_warning.h> //GCC_DIAG_OFF
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>//class IVocabularyInMainMem
#include <IO/dictionary/TUchemnitz/TUchemnitzDictionary.hpp>
#include <ctype.h> //isalpha(int)
/** SAME_VOCABLE_SEPERATOR_CHAR etc. */
#include <IO/dictionary/TUchemnitz/TUchemnitzDictSeparatorChars.h>

//namespace VTrans3
//{
  namespace DictionaryReader
  {
    namespace TUchemnitz {
    namespace EngWordSorted1st {
    namespace EachAttributeInSingleLine
    {
      BinarySearchInDictFile::BinarySearchInDictFile(IVocabularyInMainMem & vocaccess)
        : DictionaryReader::TUchemnitz::EngWordSorted1st::BinarySearchInDictFile(vocaccess)
      {
        /** "Intransitive Verb 3rd person singular present" */
        std::string wordKind = "vi3Pres";
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::vi_3singPres);

        wordKind = "vi_3singPast"; //="Verb Intransitive"
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::vi_3singPast);

        /** "Transitive Verb 3rd person singular present" */
        wordKind = "vt3Pres";
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::vt_3singPres);

        /** "Reflexive Verb 3rd person singular present" */
        wordKind = "vr3Pres";
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::vr_3singPres);

        wordKind = "positive"; //=adjective in positive form
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::adjPositive);

        wordKind = "viI"; /** "Intransitive Verb infinitive" */
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::intransitiveVerb);
        wordKind = "vrI"; /** "Reflexive Verb infinitive" */
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::reflexiveVerb);
        wordKind = "vtI"; /** "Transitive Verb infinitive" */
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::transitiveVerb);
        //TODO  insert more nodes: for "vt", "vr", ...
      }

      BinarySearchInDictFile::~BinarySearchInDictFile()
      {
        // TODO Auto-generated destructor stub
      }

      IVocabularyInMainMem::voc_container_type * BinarySearchInDictFile::
        AddVocable(
        const std::vector<std::string> & englishVocableWords,
        enum TUchemnitzDictionary::wordKinds wordKind,
  //      enum EnglishWord::English_word_class word_class
  //      IVocabularyInMainMem::voc_container_type & voc_container
        VocabularyAndTranslation *& p_vocabularyandtranslation
        )
      {
        LOGN_DEBUG("begin wordkind:" << wordKind << " voc&transl:" << p_vocabularyandtranslation)
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
        case TUchemnitzDictionary::vi_3singPres:
          /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
          word_class = EnglishWord::mainVerbAllows0object3rdPersonSingularPresent;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
          p_vocabularyandtranslation = NULL;
          break;
        case TUchemnitzDictionary::vt_3singPres:
          /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
          word_class = EnglishWord::mainVerbAllows1object3rdPersonSingularPresent;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
          p_vocabularyandtranslation = NULL;
          break;
        case TUchemnitzDictionary::vi_3singPast:
          /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
          word_class = EnglishWord::mainVerbAllows0object3rdPersonSingularPast;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
          p_vocabularyandtranslation = NULL;
          break;
        case TUchemnitzDictionary::vt_3singPast:
          /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
          word_class = EnglishWord::mainVerbAllows1object3rdPersonSingularPast;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
          p_vocabularyandtranslation = NULL;
          break;
          //TODO handle more wordkinds
        case TUchemnitzDictionary::transitiveVerb:
          /** http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29:
          * " sowohl Subjekt als auch ein Objekt benötigen, damit ein Satz, der
          * mit diesem Verb gebildet wird, grammatisch ist." */
          word_class = EnglishWord::main_verb_allows_1object_infinitive;
          break;
        case TUchemnitzDictionary::adj:
          word_class = EnglishWord::adjective;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
          p_vocabularyandtranslation = NULL;
          break;
        case TUchemnitzDictionary::adjPositive:
          word_class = EnglishWord::adjective_positiveForm;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
          p_vocabularyandtranslation = NULL;
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
          word_class = EnglishWord::plural_noun;
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
          VocabularyAndTranslation * p_vocAndTransl =
            c_iterPipeIndex2VocAndTranslPtr->second;
          const int index = c_iterPipeIndex2VocAndTranslPtr->first;

          EnglishWord::English_word_class engWordClass = EnglishWord::
            MapGrammarPartIDtoWordClass(p_vocAndTransl->m_englishWordClass);
          if( p_vocAndTransl && /*p_vocAndTransl->m_englishWordClass*/
              engWordClass <= EnglishWord::adjective )
          {
            const VocabularyAndTranslation::ArraySizes & arraySizes =
              VocabularyAndTranslation::s_arraysizes[engWordClass];
            const fastestUnsignedDataType germanWordIndex = EnglishWord::
              GetGermanWordIndexFromGramarPartID(p_vocAndTransl->m_englishWordClass);
//            while(c_iterGermanWord != r_.end() )
//            {
              if( germanWordIndex < arraySizes.m_byArraySizeForGermanWord )
              {
                const std::string & germanWord = * c_iterGermanWord;
                LOGN_DEBUG("setting German word \"" << germanWord
                  << "\" for index " << germanWordIndex
                  << " for pipe index " << index
                  << " ,vocl&transl ptr " << p_vocAndTransl)
                p_vocAndTransl->SetGermanWord(
                    germanWord.c_str(),
                    germanWord.length(),
                    germanWordIndex);
//                ++ germanWordIndex;
                p_vocAndTransl->PossiblyGenerateAndAddGermanAttributes(
//                  engWordClass,
                  p_vocAndTransl->m_englishWordClass,
                  germanWord
                  );
              }
//              ++ c_iterGermanWord;
//            }
          }
          ++ c_iterPipeIndex2VocAndTranslPtr;
          ++ c_iterGerman;
        }
        LOGN_DEBUG("end")
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
        bool afterRoundOpeningBracket = false;
        bool afterSquaredOpeningBracket = false;
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
          if( ! afterRoundOpeningBracket && ! afterSquaredOpeningBracket &&
              ( ::isalpha(i) ||
              /** e.g. "fork-lift" */
              i == '-' ) )
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

              case SYNONYM_SEPERATOR_CHAR :
    //                  break;
    //                  psvDictFile.clear();
              case SAME_VOCABLE_SEPERATOR_CHAR :
              case '/' : // e.g. "engineer(with university degree) /eng./"
                /** e.g. for "work{work,wrought; worked, wrought}|"
                 to use word "work" before '{' */
              case '{' :
              case ENGLISH_GERMAN_SEPERATOR_CHAR :
                endSearchForCompareStringInCurrentVocData = true;
    //                  compareVectors = true;
                HandleEndOfWord(word, charIndex, psvDictFile, tokenIndex, compareVectors);
                break;
              /** e.g. "captive bolt pistol(slaughter house)" : do not consider
               *   words inside brackets. */
              case '(' : // e.g. "bank of(gas) cylinders"
                afterRoundOpeningBracket = true;
                HandleEndOfWord(word, charIndex, psvDictFile, tokenIndex, compareVectors);
                break;
              case '[' : // e.g. "losings [Am.]"
    //                  compareVectors = true;
                afterSquaredOpeningBracket = true;
                endSearchForCompareStringInCurrentVocData = true;
                HandleEndOfWord(word, charIndex, psvDictFile, tokenIndex, compareVectors);
                break;
              case ')' : // e.g. "bank of(gas) cylinders"
                afterRoundOpeningBracket = false;
                break;
              case ']' :
                afterSquaredOpeningBracket = false;
                break;
    //              else
    //                byteOffset
    //              word[charIndex ++] = ch;
            } //switch
            if( /*compareVectors ||*/  endSearchForCompareStringInCurrentVocData )
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
        LOGN_DEBUG("return " << comp)
        return comp;
      }
    }
    }
    }
  } /* namespace DictionaryReader */
//} /* namespace VTrans3 */
