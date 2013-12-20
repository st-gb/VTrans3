/*
 * TUchemnitzDictEngWord2LineNumberReader.cpp
 *
 *  Created on: Sep 7, 2013
 *      Author: Stefan
 */

#include "IO/dictionary/TUchemnitz/TUchemnitzDictEngWord2LineNumberReader.hpp"
#include <IO/dictionary/TUchemnitz/TUchemnitzDictSeparatorChars.h>
#include <UserInterface/I_UserInterface.hpp>
//class IVocabularyInMainMem
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>
#include <VocabularyInMainMem/ReadFileLneAndExtract.hpp>

NodeTrie<EnglishWord::English_word_class>
  DictionaryReader::TUchemnitzDictEngWord2LineNumberReader::s_nodetrieWordKind2EngWordClassEnum(
    INDEX_OF_LAST_SMALL_LETTER_IN_ASCII, EnglishWord::beyond_last_entry);

namespace DictionaryReader
{

  TUchemnitzDictEngWord2LineNumberReader::TUchemnitzDictEngWord2LineNumberReader(
    I_UserInterface & r_userInterface,
//    IVocabularyInMainMem * p_ivocabularyinmainmem
    VocabularyAccess::ReadFileLineAndExtract * p_ivocabularyinmainmem
    )
    : TUchemnitzDictionaryReader(r_userInterface, p_ivocabularyinmainmem)
      , m_r_ivocabularyinmainmem(* p_ivocabularyinmainmem)
      //, m_p_vocaccess(& r_ivocabularyinmainmem
  {
//    m_r_ivocabularyinmainmem.SetDictionaryReader(this);
//    s_nodetrieWordKind2EngWordClassEnum.insert_inline("")
    if( s_nodetrieWordKind2EngWordClassEnum.size() == 0 )
      CreateWordKindString2wordKindEnumMapping();
  }

  TUchemnitzDictEngWord2LineNumberReader::~TUchemnitzDictEngWord2LineNumberReader()
  {
    // TODO Auto-generated destructor stub
  }

  void TUchemnitzDictEngWord2LineNumberReader::
    CreateWordKindString2wordKindEnumMapping()
  {
    if( s_nodetrieWordKind2EngWordClassEnum.size() == 0 )
    {
  //    Build();
      //Wortart: adj, adv, vi=verb intrans. vr=verb reflexiv.
      std::string wordKind = "adj";
      s_nodetrieWordKind2EngWordClassEnum.insert_inline(
        (BYTE *) wordKind.c_str(), wordKind.size(), EnglishWord::adjective);
      wordKind = "adv";
      s_nodetrieWordKind2EngWordClassEnum.insert_inline(
        (BYTE *) wordKind.c_str(), wordKind.size(), EnglishWord::adverb);
      wordKind = "m"; /** _m_asculine noun */
      s_nodetrieWordKind2EngWordClassEnum.insert_inline(
        (BYTE *) wordKind.c_str(), wordKind.size(), EnglishWord::noun);
      wordKind = "f"; /** _f_eminine noun */
      s_nodetrieWordKind2EngWordClassEnum.insert_inline(
        (BYTE *) wordKind.c_str(), wordKind.size(), EnglishWord::noun);
      wordKind = "n"; /** _n_eutral noun */
      s_nodetrieWordKind2EngWordClassEnum.insert_inline(
        (BYTE *) wordKind.c_str(), wordKind.size(), EnglishWord::noun);
      wordKind = "vi"; //="_v_erb _i_ntransitive"
      s_nodetrieWordKind2EngWordClassEnum.insert_inline(
        (BYTE *) wordKind.c_str(), wordKind.size(), EnglishWord::main_verb);
      wordKind = "vt"; //="_v_erb _t_ransitive"
      s_nodetrieWordKind2EngWordClassEnum.insert_inline(
        (BYTE *) wordKind.c_str(), wordKind.size(), EnglishWord::main_verb);
      wordKind = "vr"; /** ="_v_erb _r_eflexive" (e.g. "he turns himself") */
      s_nodetrieWordKind2EngWordClassEnum.insert_inline(
        (BYTE *) wordKind.c_str(), wordKind.size(), EnglishWord::main_verb);
    }
  }

  bool TUchemnitzDictEngWord2LineNumberReader::read(const char * filePath)
  {
//    TUchemnitzDictionaryReader::UTF8toGermanASCII(
//      );
    m_std_strFilePath = std::string(filePath);
    extractVocables(filePath);
    if( ! m_r_ivocabularyinmainmem.OpenDictionaryFile(filePath) )
    {
      s_p_i_userinterface->Message("failed to open dict file");
    }
  }

  void TUchemnitzDictEngWord2LineNumberReader::InsertEnglishWord(
    const char * const dictionaryLine,
    unsigned & englishVocableBeginCharIndex,
    unsigned wordLength,
    unsigned & charIndex,
    const NodeTrieNode</*GermanWord::German_word_class*/
      EnglishWord::English_word_class> * const p_ntnValidWordKind,
    const bool englishPart,
//    const unsigned //numChars
    unsigned & englishWordIndex)
  {
    if( englishPart &&
        //for "aasen {vi} [ugs.] | mit etwas aasen ::  | to be wasteful with sth."
        englishVocableBeginCharIndex < charIndex
        )
    {
    //#ifdef __WXDEBUG__
    //          if(englishPart)
    //            if(p_ntnValidWordKind && p_ntnValidWordKind->m_member !=
    //              EnglishWord::beyond_last_entry)
    //#else
      if(englishVocableBeginCharIndex && p_ntnValidWordKind &&
          p_ntnValidWordKind->m_member
          != EnglishWord::beyond_last_entry)
      {
  //#endif
        /*s_p_vocinmainmem*/m_p_vocaccess->
        InsertAsKeyAndAddVocabularyAttributes(
          dictionaryLine + englishVocableBeginCharIndex,
          wordLength,
          p_ntnValidWordKind->m_member);
        if( englishWordIndex == 0 && p_ntnValidWordKind->m_member ==
            EnglishWord::main_verb)
        {
          const VTrans::string_type vt_strInf(
            dictionaryLine + englishVocableBeginCharIndex, wordLength);
          VTrans::string_type vt_str3rdPersonSingPres = EnglishVerb::
            Get3rdPersonSingularPresent(vt_strInf);

          m_p_vocaccess->InsertAsKeyAndAddVocabularyAttributes(
            vt_str3rdPersonSingPres.c_str(),
            vt_str3rdPersonSingPres.length(),
            p_ntnValidWordKind->m_member);
        }
//        //end loop
//        charIndex = numChars;
  //      break;
      }
  //          vocableBegin = array + charIndex + 1;
  //          vocableBeginCharIndex = charIndex + 1;
      if(englishPart)
        englishVocableBeginCharIndex = charIndex + 1;
      ++ englishWordIndex;
    }
  }

  void TUchemnitzDictEngWord2LineNumberReader::extractSingleEntry(
    const char * dictionaryLine, const unsigned numCharsForCurrentDictLine)
  {
    char ch;
    unsigned kindOfWordStart;
    unsigned englishWordIndex = 0;
  //  BYTE * p_kindOfWordStart;
    unsigned prevPipeCharIndex = 0;
    bool englishPart = false;
    NodeTrieNode</*GermanWord::German_word_class*/
      EnglishWord::English_word_class> * p_ntnWordKind = NULL,
      * p_ntnValidWordKind = NULL;
    m_currentLineNumber ++;
//    const char * vocableBegin = NULL;
    unsigned vocableBeginCharIndex = 0, englishVocableBeginCharIndex = 0;
  //  const char * dictionaryLine = strCurrentWordData.c_str();
    unsigned currentCharIndex = 0;
    for( ; currentCharIndex < //strCurrentWordData.size();
        numCharsForCurrentDictLine; ++ currentCharIndex)
    {
      ch = dictionaryLine[currentCharIndex];
      switch(ch)
      {
        case SAME_VOCABLE_SEPERATOR_CHAR :
          InsertEnglishWord(
            dictionaryLine,
            englishVocableBeginCharIndex,
            currentCharIndex - englishVocableBeginCharIndex - 1,
            currentCharIndex,
            p_ntnValidWordKind,
            englishPart,
//            numCharsForCurrentDictLine,
            englishWordIndex
            );
          break;
        case SYNONYM_SEPERATOR_CHAR :
          InsertEnglishWord(
            dictionaryLine,
            englishVocableBeginCharIndex,
            currentCharIndex - englishVocableBeginCharIndex,
            currentCharIndex,
            p_ntnValidWordKind,
            englishPart,
//            numCharsForCurrentDictLine
            englishWordIndex
            );
          break;
        case ':' :
          if ( prevPipeCharIndex == currentCharIndex - 1 ) //-> "::"
          {
            //The kind of word may only be specified in English:
          //"Hintergrund {m} (Theater) | im Hintergrund (der Bühne) :: back | upstage {adv}"
            englishPart = true;
            if( p_ntnValidWordKind && p_ntnValidWordKind->m_member ==
                EnglishWord::main_verb )
              //skip "to " before an infinitive
              englishVocableBeginCharIndex = currentCharIndex + 5;
            else
              englishVocableBeginCharIndex = currentCharIndex + 2;
          }
          else
            prevPipeCharIndex = currentCharIndex;
          break;
        case '{':
          kindOfWordStart = currentCharIndex + 1;
          InsertEnglishWord(
            dictionaryLine,
            englishVocableBeginCharIndex,
            currentCharIndex - englishVocableBeginCharIndex - 1,
            currentCharIndex,
            p_ntnValidWordKind,
            englishPart,
//            numCharsForCurrentDictLine
            englishWordIndex
            );
          break;
        case '}':
  //        p_kindOfWordStart =
  #ifdef _DEBUG
  //        std::string kindOfWord = strCurrentWordData.substr(kindOfWordStart,
  //          currentCharIndex - kindOfWordStart);
//          int i = s_p_i_userinterface->m_dictionaryFileLineNumber;
          int i = 55;
  #endif
  //#else
          {
          const char * p_chStartOfWordKind = dictionaryLine + kindOfWordStart;
          const unsigned numWordKindChars = currentCharIndex - kindOfWordStart;
          p_ntnWordKind =
            s_nodetrieWordKind2EngWordClassEnum.contains_inline(
            (BYTE *) p_chStartOfWordKind, numWordKindChars,
            true);
          if( p_ntnWordKind)
            p_ntnValidWordKind = p_ntnWordKind;
          }
          break;
  //#endif
//          if( p_ntnWordKind//->
//            )
//          {
//          }
//        default:
//          if( ch >= 'a' && ch <= 'z')
//          {
//            if( currentCharIndex > 0 && ! ::isalpha(dictionaryLine[currentCharIndex]) )
//              wordBegin = currentCharIndex;
//          }
      }
    }
    //e.g. for "scavenge" in line "Aas fressen {vi} [zool.] :: to scavenge"
//    InsertEnglishWord(
//      dictionaryLine,
//      englishVocableBeginCharIndex,
//      currentCharIndex - englishVocableBeginCharIndex - 1,
//      currentCharIndex,
//      p_ntnValidWordKind,
//      englishPart,
//      numCharsForCurrentDictLine
//      );
  }
} /* namespace DictionaryReader */
