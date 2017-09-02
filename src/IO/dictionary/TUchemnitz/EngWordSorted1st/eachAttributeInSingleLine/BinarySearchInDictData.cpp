/*
 * EachAttributeInSingleLine.cpp
 *
 *  Created on: 03.01.2014
 *      Author: mr.sys
 */

#include "BinarySearchInDictData.hpp"
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
      BinarySearchInDictData::BinarySearchInDictData(IVocabularyInMainMem & vocaccess)
        : DictionaryReader::TUchemnitz::EngWordSorted1st::BinarySearchInDictData(vocaccess)
      {
        try
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
        wordKind = "vdI"; /** "Transitive dative Verb infinitive" */
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::dativeVerb);
        wordKind = "vaI"; /** "Transitive accusative Verb infinitive" */
        s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
          TUchemnitzDictionary::accusativeVerb);
        //TODO  insert more nodes: for "vt", "vr", ...
        }catch( const NS_NodeTrie::RootNodeNotInitalizedException & e)
        {
          LOGN_ERROR("NS_NodeTrie::RootNodeNotInitalizedException")
        }
      }

      BinarySearchInDictData::~BinarySearchInDictData()
      {
        // TODO Auto-generated destructor stub
      }

      enum EnglishWord::English_word_class GetEnglishWordClass(
        const enum TUchemnitzDictionary::wordKinds wordKind)
      {
        enum EnglishWord::English_word_class english_word_class = EnglishWord::beyond_last_entry;
        GCC_DIAG_OFF(switch)
        switch(wordKind)
        {
  //      case TUchemnitzDictionary::reflexiveVerb:
  //        word_class = EnglishWord::ref
        case TUchemnitzDictionary::intransitiveVerb:
          /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
          english_word_class = EnglishWord::main_verb_allows_0object_infinitive;
          break;
        case TUchemnitzDictionary::vi_3singPres:
          /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
          english_word_class = EnglishWord::mainVerbAllows0object3rdPersonSingularPresent;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
//          p_vocabularyandtranslation = NULL;
          break;
        case TUchemnitzDictionary::vt_3singPres:
          /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
          english_word_class = EnglishWord::mainVerbAllows1object3rdPersonSingularPresent;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
//          p_vocabularyandtranslation = NULL;
          break;
        case TUchemnitzDictionary::vi_3singPast:
          /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
          english_word_class = EnglishWord::mainVerbAllows0object3rdPersonSingularPast;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
//          p_vocabularyandtranslation = NULL;
          break;
        case TUchemnitzDictionary::vt_3singPast:
          /** see http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29 */
          english_word_class = EnglishWord::mainVerbAllows1object3rdPersonSingularPast;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
//          p_vocabularyandtranslation = NULL;
          break;
          //TODO handle more wordkinds
        case TUchemnitzDictionary::transitiveVerb:
        case TUchemnitzDictionary::dativeVerb:
        case TUchemnitzDictionary::accusativeVerb:
          /** http://de.wikipedia.org/wiki/Transitivit%C3%A4t_%28Grammatik%29:
          * " sowohl Subjekt als auch ein Objekt benötigen, damit ein Satz, der
          * mit diesem Verb gebildet wird, grammatisch ist." */
          english_word_class = EnglishWord::main_verb_allows_1object_infinitive;
          break;
        case TUchemnitzDictionary::adj:
          english_word_class = EnglishWord::adjective;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
//          p_vocabularyandtranslation = NULL;
          break;
        case TUchemnitzDictionary::adjPositive:
          english_word_class = EnglishWord::adjective_positiveForm;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created. (multiple singular nouns may appear in a row->create
           *   a VocabularyAndTranslation for each noun. */
//          p_vocabularyandtranslation = NULL;
          break;
        case TUchemnitzDictionary::adv:
          english_word_class = EnglishWord::adverb;
          break;
        case TUchemnitzDictionary::femNoun:
        case TUchemnitzDictionary::mascNoun:
        case TUchemnitzDictionary::neutralNoun:
          /** Must add as singular, else applying grammar rule fails. */
          english_word_class = EnglishWord::singular;
          break;
        case TUchemnitzDictionary::pluralNoun:
  //        word_class = EnglishWord::noun;
          /** =NULL means: The next time a VocabularyAndTranslation should be
           *   created.*/
//          p_vocabularyandtranslation = NULL;
          english_word_class = EnglishWord::plural_noun;
          break;
        }
        GCC_DIAG_ON(switch)
        return english_word_class;
      }
      
      IVocabularyInMainMem::voc_container_type * BinarySearchInDictData::
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
        enum EnglishWord::English_word_class word_class = 
          GetEnglishWordClass(wordKind);
        
        /** For the file format "each attribute in single line" each attribute
         *  (singular, plural, ...) of a vocable is stored in a single line and
         *  so for each line a VocabularyAndTranslation object should be created.
         *  NULL means: The next time a VocabularyAndTranslation should be
         *   created. (multiple singular nouns may appear in a row->create
         *   a VocabularyAndTranslation for each noun. */
        p_vocabularyandtranslation = NULL;
        
        LOGN_DEBUG("EnglishWord::class: " << word_class)
        if( word_class != EnglishWord::beyond_last_entry &&
            englishVocableWords.size() > 0 )
        {
          const std::string std_strFirstVocableWord = englishVocableWords.at(0);
          const char * ar_chFirstVocableWordBegin = std_strFirstVocableWord.c_str();
          const int stringLen = std_strFirstVocableWord.length();
          p_voc_container =
            //s_p_vocinmainmem
              m_p_vocaccess->InsertAsKeyAndAddVocabularyAttributes(
            ar_chFirstVocableWordBegin
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
            case TUchemnitzDictionary::transitiveVerb:
            case TUchemnitzDictionary::vt_3singPres:
            case TUchemnitzDictionary::vt_3singPast:
              /** If a grammatical case is not given in the dictionary, it
               *  typically may be either dative or accusative. */
              p_vocabularyandtranslation->SetAttributeValue(0,
                GermanVerb::dative_or_accusative);
            case TUchemnitzDictionary::dativeVerb:
              p_vocabularyandtranslation->SetAttributeValue(0,
                GermanVerb::dative);
            case TUchemnitzDictionary::accusativeVerb:
              p_vocabularyandtranslation->SetAttributeValue(0,
                GermanVerb::accusative);
            }
            GCC_DIAG_ON(switch)
          }
  //        p_vocandtransl->SetGermanWord();
        }
        LOGN_DEBUG("return p_voc_container:" << p_voc_container)
        return /*p_vocabularyandtranslation*/ p_voc_container;
      }

      /** @param germanVocables: vector of vocables.
       *     each vocable has vector of words */
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
        std::vector< std::vector <std::string> >::const_iterator c_iterGermanVocable =
          germanVocables.begin();
        std::vector<std::string>::const_iterator c_iterGermanWord;
        while( c_iterGermanVocable != germanVocables.end() && 
          c_iterPipeIndex2VocAndTranslPtr != voc_containerVocsCreated.end() )
        {
          const std::vector<std::string> & r_currentVocableWords = 
            ( * c_iterGermanVocable);
//#ifdef _DEBUG
          const int numGermanWords = r_currentVocableWords.size();
//#endif
          c_iterGermanWord = r_currentVocableWords.begin();
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
            const fastestUnsignedDataType germanWordIndex = EnglishWord::
              GetGermanWordIndexFromGramarPartID(p_vocAndTransl->m_englishWordClass);
            fastestUnsignedDataType attributeIndexInsideDictFile = 0;
            while(c_iterGermanWord != r_currentVocableWords.end() )
            {
              if( germanWordIndex < arraySizes.m_byArraySizeForGermanWord )
              {
                const std::string & germanWord = * c_iterGermanWord;
                LOGN_DEBUG("setting German word \"" << germanWord
                  << "\" for index " << germanWordIndex
                  << " for pipe index " << pipeIndex
                  << " ,vocl&transl ptr " << p_vocAndTransl)
      //fixed? if e.g. verb has words "moegen" and "mag": then "mag" overwrites "moegen" here.
                p_vocAndTransl->SetGermanWord(
                    germanWord.c_str(),
                    germanWord.length(),
                    //germanWordIndex
                    attributeIndexInsideDictFile
                    );
//                ++ germanWordIndex;
//                if(
//                    ( p_vocAndTransl->m_englishWordClass ==
//                    EnglishWord::main_verb_allows_0object_infinitive ||
//                    p_vocAndTransl->m_englishWordClass ==
//                    EnglishWord::main_verb_allows_1object_infinitive ||
//                    p_vocAndTransl->m_englishWordClass ==
//                    EnglishWord::main_verb_allows_2objects_infinitive )
//                    && (attributeIndexInsideDictFile == 1 ||
//                    numGermanWords == 1
//                    )
//                  )
                {
                  p_vocAndTransl->PossiblyGenerateAndAddGermanAttributes(
  //                  engWordClass,
                    p_vocAndTransl->m_englishWordClass,
                    germanWord,
                    attributeIndexInsideDictFile
                    );
                }
              }
              ++ attributeIndexInsideDictFile;
              ++ c_iterGermanWord;
            }
          }
          ++ c_iterPipeIndex2VocAndTranslPtr;
          ++ c_iterGermanVocable;
        }
        LOGN_DEBUG("end")
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
        //bool streamIsGood;
        PositionStringVector psvDictFile;
        bool breakWhile = false;
        bool afterRoundOpeningBracket = false;
        bool afterSquaredOpeningBracket = false;
        char englishWord[100] = ""; /** Initialize, else valgrind error. */
        fastestUnsignedDataType charIndex = 0;
        bool endSearchForCompareStringInCurrentVocData = false;
        fastestUnsignedDataType tokenIndex = 0;
        bool compareVectors = false;
        /** Set to a default value to avoid using an array out of bounds error
         *  if "cmp" was too large and it was used as a array index for
         *  PositionStringVector::s_comparisonResultString */
        PositionStringVector::cmp comp = PositionStringVector::notSet;

        //int i = ReadByte();
        fastestUnsignedDataType numBytes = 200;
        uint8_t buffer[numBytes];
        ReadByteBuffer(buffer /*(uint8_t *) englishWord*/, numBytes);
        fastestUnsignedDataType bufferIndex = 0;
        
        char byte;
        //TODO check is only necessary if NOT using exceptions.
        //streamIsGood = i < 256 /** return codes start*/;
        while( bufferIndex < numBytes /*streamIsGood*/ /*&& ! breakWhile*/ )
        {
          byte = buffer[bufferIndex];
          if( ! afterRoundOpeningBracket && ! afterSquaredOpeningBracket &&
              isValidEnglishWordCharacter(byte )
            )
          {
            englishWord[charIndex ++] = byte;
            //charIndex++;
          }
          else
          {
            /** Only for outputting the character that caused the call to
             * HandleEndOfWord(...) */
            //word[charIndex] = i;
            englishWord[charIndex] = byte;
            switch( /*i*/ byte /*buffer[bufferIndex]*/ )
            {
              case ' ' :
              {
    #ifdef _DEBUG
  //              currOffset = m_englishDictionary.tellg();
                currOffset = GetCurrentDictDataOffset();
    #endif
                //englishWord[charIndex] = 0;
                LOGN_DEBUG("space reached--word is: \"" << englishWord << "\"")
                HandleEndOfToken(englishWord, charIndex, psvDictFile, tokenIndex);
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
                HandleEndOfWord(englishWord, charIndex, psvDictFile, tokenIndex, compareVectors);
                break;
              /** e.g. "captive bolt pistol(slaughter house)" : do not consider
               *   words inside brackets. */
              case '(' : // e.g. "bank of(gas) cylinders"
                afterRoundOpeningBracket = true;
                HandleEndOfWord(englishWord, charIndex, psvDictFile, tokenIndex, compareVectors);
                break;
              case '[' : // e.g. "losings [Am.]"
    //                  compareVectors = true;
                afterSquaredOpeningBracket = true;
                endSearchForCompareStringInCurrentVocData = true;
                HandleEndOfWord(englishWord, charIndex, psvDictFile, tokenIndex, compareVectors);
                break;
              case ')' : // e.g. "bank of(gas) cylinders"
                afterRoundOpeningBracket = false;
                break;
              case ']' : /** e.g. "losings [Am.]" */
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
                LOGN_DEBUG("result of comparison: " << PositionStringVector:://GetResultAsString(comp)
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
          //i = ReadByte();
          //streamIsGood = i > 0;
  //        psvDictFile.clear();
          bufferIndex ++;
        } //while loop for current voc data
        LOGN_DEBUG("return " << comp << "(\"" << PositionStringVector::
		  s_comparisonResultString[comp] << "\")" )
        return comp;
      }
    }
    }
    }
  } /* namespace DictionaryReader */
//} /* namespace VTrans3 */
