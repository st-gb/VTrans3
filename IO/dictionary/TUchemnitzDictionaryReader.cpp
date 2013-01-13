/*
 * TUchemnitzDictionaryReader.cpp
 *
 *  Created on: 05.01.2013
 *      Author: Stefan
 */
#include "TUchemnitzDictionaryReader.hpp"
#include <fstream> //class std::ofstream
#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
#include <stdint.h> //

#define INDEX_OF_LAST_SMALL_LETTER_IN_ASCII 128
/** Static variables need also to be declared in 1 source file. */
LetterTree * TUchemnitzDictionaryReader::s_p_lettertree ;
I_UserInterface * TUchemnitzDictionaryReader::s_p_i_userinterface ;
NodeTrie<TUchemnitzDictionaryReader::insertVocable>
  TUchemnitzDictionaryReader::s_nodetrieWordKind(
    INDEX_OF_LAST_SMALL_LETTER_IN_ASCII, NULL);

TUchemnitzDictionaryReader::TUchemnitzDictionaryReader(
  I_UserInterface & i_userinterface)
{
  s_p_i_userinterface = & i_userinterface;
  if( s_nodetrieWordKind.size() == 0 )
  {
//    Build();
    //Wortart: adj, adv, vi=verb intrans. vr=verb reflexiv.
    std::string wordKind = "adj";
    s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
      InsertAdjective);
    wordKind = "adv";
    s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
      InsertAdverb);
  }
}

void TUchemnitzDictionaryReader::Insert1stEnglishWord(
//  const std::string & strCurrentWordData,
  const char * ar_ch,
  unsigned nCharIndexOf1stWordChar,
  unsigned nIndexOfCurrentChar,
  enum EnglishWord::English_word_class word_class)
{
  bool bInsertNewVocabularyAndTranslation = true;
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
  int stringLen = nIndexOfCurrentChar - nCharIndexOf1stWordChar;
  if( ! s_p_lettertree->InsertIntoTrieAndHandleVocabularyAndTranslation(
      stdsetpletternodeLastStringChar
      , bInsertNewVocabularyAndTranslation//
      , //byVocabularyType
      word_class //EnglishWord::adjective_positiveForm
      , //strCurrentWordData
        ar_ch
      , stringLen
      , //1
      nCharIndexOf1stWordChar
      )
    )
  {
//    std::string str(ar_ch + nCharIndexOf1stWordChar, stringLen);
//    s_p_i_userinterface->Message( "\"" + str + "\" contains invalid characters");
  }
}

void TUchemnitzDictionaryReader::Insert1stGermanWord(
  //const std::string & strCurrentWordData,
  const char * ar_ch,
  unsigned nCharIndexOf1stWordChar,
  unsigned nCharIndexOfLastWordChar)
{
//  bool bInsertNewVocabularyAndTranslation = false;
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
//  s_p_lettertree->//InsertIntoTrieAndHandleVocabularyAndTranslation(
//    HandleVocabularyAndTranslationPointerInsertion(
//    stdsetpletternodeLastStringChar
//    , bInsertNewVocabularyAndTranslation //
//    , //byVocabularyType
//    EnglishWord::adjective_positiveForm
//    , strCurrentWordData
//    , nIndexOfCurrentChar - 1 //nIndexOf1stChar
//    , //1
//    nCharIndexOf1stWordChar
//    ) ;
  const VocabularyAndTranslation * const
    p_vocabularyandtranslation =
    s_p_lettertree->s_pvocabularyandtranslation;
  std::string * p_std_str = & p_vocabularyandtranslation->
    m_arstrGermanWord[0 ];
  //Add to the structure that also contains the English strings.
  //So the German equivalent can be retrieved when the last
  //character (LetterNode) of the English string inside the Trie
  //is got.
//        g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord[
  * p_std_str = //std_strCurrentWord;
//    strCurrentWordData.substr(
//      nCharIndexOf1stWordChar
//      , nCharIndexOfLastWordChar - nCharIndexOf1stWordChar
//      ) ;
      std::string( ar_ch + nCharIndexOf1stWordChar,
        nCharIndexOfLastWordChar - nCharIndexOf1stWordChar);
}

void TUchemnitzDictionaryReader::InsertAdjective(
//  const std::string & strCurrentWordData,
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
  Insert1stEnglishWord(//strCurrentWordData,
    ar_ch, englishWords[0].m_charIndexOfBegin,
    englishWords[0].m_charIndexOfEnd,
    EnglishWord::adjective_positiveForm);
//  convertUTF8toASCII();
  Insert1stGermanWord(/*strCurrentWordData,*/ ar_ch,
    germanWords[0].m_charIndexOfBegin,
    germanWords[0].m_charIndexOfEnd);
  ++ s_p_i_userinterface->s_numberOfVocabularyPairs;
}

// deutsches Wort[; Synonym] '{' Wortart '}' [| deutsches Beispiel] ::
//  englisches Wort [; Synonym] [| englisches Beispiel]

//void TUchemnitzDictionaryReader::InsertAdjective(//const char * array
////  const std::string & strCurrentWordData,
//  const char * ar_ch, unsigned numChars,
//  unsigned charIndex, const unsigned germanPipeCount)
//{
//  //This set is to ensure that if strings for the SAME vocabulary
//  // not 2 or more VocAndTransl object should be inserted.
//  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
////  bool bInsertNewVocabularyAndTranslation = true;
//
//  unsigned indexOf1stEnglishPositiveChar = 0;
////  const char * ar_ch = strCurrentWordData.c_str();
//  unsigned charIndexOfPreviousColon = //INT_M;
//    0xFFFFFFFE;
//  bool bEnglish = false;
//  unsigned delemiterCount = 0;
//  unsigned indexOfSpaceChar = 0xFFFFFFFE;
////  unsigned firstWordChar = 0;
////  unsigned //indexOf1stGermanPositiveChar,
////    indexOflastGermanPositiveChar;
////  bool previousCharIsSpace = false;
//  WordData englishpositive, germanpositive;
//  unsigned pipeCount = 0;
//  for( unsigned nIndexOfCurrentChar = 0 ; nIndexOfCurrentChar <
//    /*strCurrentWordData.length()*/ numChars; nIndexOfCurrentChar ++ )
//  {
//    switch(ar_ch[nIndexOfCurrentChar])
//    {
//      case ' ' :
//        if( bEnglish && pipeCount == germanPipeCount )
//        {
//          if( englishpositive.m_charIndexOfBegin != WordData::isNotSet )
//          {
//            englishpositive.m_charIndexOfEnd = nIndexOfCurrentChar;
//            InsertAdjective(//strCurrentWordData
//              ar_ch, englishpositive,
//              germanpositive);
//          }
//        }
//        else
//          if( germanpositive.m_charIndexOfEnd == WordData::isNotSet &&
//              germanpositive.m_charIndexOfBegin != WordData::isNotSet)
//          {
//            germanpositive.m_charIndexOfEnd = nIndexOfCurrentChar;
//          }
////        previousCharIsSpace = true;
//        indexOfSpaceChar = nIndexOfCurrentChar;
//        break;
//      case '|' :
//        if( bEnglish )
//        {
//        }
//        ++ pipeCount;
////        ++ delemiterCount;
//        break;
//      case ':' :
//        if( charIndexOfPreviousColon == nIndexOfCurrentChar - 1)
//        {
//          bEnglish = true;
////          delemiterCount = 0;
//          pipeCount = 0;
//        }
//        charIndexOfPreviousColon = nIndexOfCurrentChar;
//        break;
//      default: //letter
//        if( bEnglish )
//        {
//          if( englishpositive.m_charIndexOfBegin == WordData::isNotSet )
//          {
//            indexOf1stEnglishPositiveChar = nIndexOfCurrentChar;
//            englishpositive.m_charIndexOfBegin = nIndexOfCurrentChar;
//          }
//        }
//        else
//          if(//previousCharIsSpace
//  //            indexOfSpaceChar == nIndexOfCurrentChar - 1
//              germanpositive.m_charIndexOfBegin == WordData::isNotSet
//              && pipeCount == germanPipeCount )
//          {
//  //          firstWordChar = nIndexOfCurrentChar;
//  //          previousCharIsSpace = false;
//            germanpositive.m_charIndexOfBegin = nIndexOfCurrentChar;
//          }
//        break;
//    }
//  }
//  if( englishpositive.m_charIndexOfEnd == -1)
//  {
//    englishpositive.m_charIndexOfEnd = //strCurrentWordData.length();
//      numChars;
//    InsertAdjective(//strCurrentWordData
//      ar_ch, englishpositive,
//      germanpositive);
//  }
//}

void TUchemnitzDictionaryReader::InsertAdverb(
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
  Insert1stEnglishWord(//strCurrentWordData,
    ar_ch, englishWords[0].m_charIndexOfBegin,
    englishWords[0].m_charIndexOfEnd,
    EnglishWord::adverb);
//  convertUTF8toASCII();
  Insert1stGermanWord(/*strCurrentWordData,*/ ar_ch,
    germanWords[0].m_charIndexOfBegin,
    germanWords[0].m_charIndexOfEnd);
  ++ s_p_i_userinterface->s_numberOfVocabularyPairs;
}

void Reset(WordData words[10] )
{
  for( unsigned index = 0; index < 10; ++ index)
  {
    words[index].Reset();
  }
}

inline void PossiblySetStringBegin(
  WordData englishWords[10],
  WordData germanWords[10],
  unsigned wordIndex,
  bool insideBracket,
  unsigned pipeCount,
  unsigned germanPipeCount,
  bool bEnglish,
  unsigned nIndexOfCurrentChar)
{
  if( ! insideBracket && pipeCount == germanPipeCount)
  {
    if( bEnglish )
    {
      if( englishWords[wordIndex].BeginIsNotSet() )
      {
  //            indexOf1stEnglishPositiveChar = nIndexOfCurrentChar;
        englishWords[wordIndex].m_charIndexOfBegin = nIndexOfCurrentChar;
      }
    }
    else
    {
      if(//previousCharIsSpace
  //            indexOfSpaceChar == nIndexOfCurrentChar - 1
          germanWords[wordIndex].BeginIsNotSet() )
      {
  //          firstWordChar = nIndexOfCurrentChar;
  //          previousCharIsSpace = false;
        germanWords[wordIndex].m_charIndexOfBegin = nIndexOfCurrentChar;
      }
    }
  }
}

//"Lexikon {n}; Verzeichnis {n}; Wörterbuch {n} | Lexika {pl}; Verzeichnisse {pl};"
void TUchemnitzDictionaryReader::ExtractVocables(//const char * array
//  const std::string & strCurrentWordData,
  const char * ar_ch, unsigned numChars,
  unsigned charIndex, const unsigned germanPipeCount,
  insertVocable pfn)
{
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
//  bool bInsertNewVocabularyAndTranslation = true;

//  unsigned indexOf1stEnglishPositiveChar = 0;
//  const char * ar_ch = strCurrentWordData.c_str();
  unsigned charIndexOfPreviousColon = //INT_M;
    0xFFFFFFFE;
  bool bEnglish = false;
//  unsigned delemiterCount = 0;
  unsigned indexOfSpaceChar = 0xFFFFFFFE;
//  unsigned firstWordChar = 0;
//  unsigned //indexOf1stGermanPositiveChar,
//    indexOflastGermanPositiveChar;
//  bool previousCharIsSpace = false;
//  std::vector<WordData[10]> englishVocables, germanVocables;
  WordData englishWords[10], germanWords[10];
  unsigned pipeCount = 0;
  unsigned wordIndex = 0;
  bool insideBracket = false;
  for( unsigned nIndexOfCurrentChar = 0 ; nIndexOfCurrentChar <
    /*strCurrentWordData.length()*/ numChars; nIndexOfCurrentChar ++ )
  {
    switch(ar_ch[nIndexOfCurrentChar])
    {
      case ' ' :
//        previousCharIsSpace = true;
        indexOfSpaceChar = nIndexOfCurrentChar;
        break;
      case ';' : //e.g. "| in b\366ser Absicht; boswillig {adv} ::" :
        // insert "in b\366ser Absicht" and "boswillig" for "maliciously"
        if( pipeCount == germanPipeCount )
        {
          if( bEnglish)
          {
            if( englishWords[wordIndex].EndIsNotSet() )
            {
              englishWords[wordIndex].m_charIndexOfEnd = nIndexOfCurrentChar;
  //            englishVocables.push_back(englishWords);
  //            Reset(englishWords);
//              wordIndex = 0;
            }
          }
          else
          {
            if( germanWords[wordIndex].EndIsNotSet() )
              germanWords[wordIndex].m_charIndexOfEnd = nIndexOfCurrentChar;
//            germanVocables.push_back(germanWords);
//            Reset(germanWords);
//            wordIndex = 0;
          }
        }
        break;
      case '}' :
      case ')' : //e.g. "pleasantly stimulating (cosmetics)"
      case '>' : // "reversionary <reversional>"
      case ']' : // e.g. "moribund [fig.]"
        insideBracket = false;
        break;
      case '{' :
      case '(' : //e.g. "pleasantly stimulating (cosmetics)"
      case '<' : // "reversionary <reversional>"
      case '[' : // e.g. "moribund [fig.]"
        if( pipeCount == germanPipeCount )
        {
          //kind of word can also stand in English part: "interdepartmental {adj}"
          if( bEnglish )
          {
            if( //bracket may be the first character as in ":: (from) out of town;"
                englishWords[wordIndex].BeginIsSet() &&
                ! englishWords[wordIndex].EndIsSet() )
            {
              englishWords[wordIndex ++].m_charIndexOfEnd = nIndexOfCurrentChar - 1;
            }
          }
          else
            if( ! germanWords[wordIndex].EndIsSet() &&
                germanWords[wordIndex].BeginIsSet() )
            {
              germanWords[wordIndex ++].m_charIndexOfEnd = nIndexOfCurrentChar
                //e.g. "nicht {adj}"
                - 1;
            }
        }
        insideBracket = true;
        break;
      case '|' :
//        if( bEnglish )
//        {
//        }
        if( bEnglish && pipeCount == germanPipeCount )
        {
          if( englishWords[wordIndex].//m_charIndexOfBegin != WordData::isNotSet
              EndIsNotSet() )
          {
            englishWords[wordIndex].m_charIndexOfEnd = nIndexOfCurrentChar
                //e.g. "| not yet |
                - 1;
//            (*pfn)(//strCurrentWordData
//              ar_ch, englishWords,
//              germanWords);
          }
        }
        else
          if( germanWords[wordIndex].m_charIndexOfEnd == WordData::isNotSet &&
              germanWords[wordIndex].m_charIndexOfBegin != WordData::isNotSet)
          {
            germanWords[wordIndex ++].m_charIndexOfEnd = nIndexOfCurrentChar
              //e.g. "| noch nicht |"
              - 1;
          }
        ++ pipeCount;
//        ++ delemiterCount;
        break;
      case ':' :
        if( charIndexOfPreviousColon == nIndexOfCurrentChar - 1)
        {
          if( germanWords[wordIndex].m_charIndexOfEnd == WordData::isNotSet &&
              germanWords[wordIndex].m_charIndexOfBegin != WordData::isNotSet)
          {
            germanWords[wordIndex ++].m_charIndexOfEnd = nIndexOfCurrentChar
              //e.g. "| noch nicht ::"
              - 1;
          }
          wordIndex = 0;
          bEnglish = true;
//          delemiterCount = 0;
          pipeCount = 0;
        }
        charIndexOfPreviousColon = nIndexOfCurrentChar;
        break;
      default: //letter
        PossiblySetStringBegin(englishWords, germanWords, wordIndex,
          insideBracket, pipeCount, germanPipeCount, bEnglish, nIndexOfCurrentChar);
        break;
    }
  }
  if( englishWords[wordIndex].m_charIndexOfEnd == -1)
  {
    englishWords[wordIndex].m_charIndexOfEnd = //strCurrentWordData.length();
      numChars;
    //InsertAdverb(//strCurrentWordData
//    (*pfn)(
//      ar_ch, englishWords,
//      germanWords);
  }
  //There may be no translation:
  //"angehalten {adj} | [...] ::  |"
  if( englishWords[0].m_charIndexOfBegin != WordData::isNotSet &&
      germanWords[0].m_charIndexOfBegin != WordData::isNotSet )
    (*pfn)(
      ar_ch, englishWords,
      germanWords);
//  std::vector<WordData[]>::const_iterator germanVocIter = germanVocables.begin();
//  std::vector<WordData[]>::const_iterator englishVocIter = englishVocables.begin();
//  while( germanVocIter != germanVocables.end() && englishVocIter !=
//      englishVocables.end() )
//  {
//    ( * pfn)(
//      ar_ch, //englishWords,
//      * englishVocIter
//      //germanWords
//      * germanVocIter);
//    ++ englishVocIter;
//    ++ germanVocIter;
//  }
}

/** @param array: end of string is determined by NULL char
 *
 */
void TUchemnitzDictionaryReader::extractSingleEntry(
  const char * array,
  unsigned numChars
  //const std::string & strCurrentWordData
  )
{
  char ch;
  unsigned kindOfWordStart;
//  BYTE * p_kindOfWordStart;
  unsigned pipeCount = 0;
  unsigned prevPipeCharIndex = 0;
//  const char * array = strCurrentWordData.c_str();
  for( unsigned charIndex = 0; charIndex < //strCurrentWordData.size();
      numChars;
      ++ charIndex)
  {
    ch = array[charIndex];
    switch(ch)
    {
      case '|':
        ++ pipeCount; //next word
        break;
      case ':' :
        if ( prevPipeCharIndex == charIndex - 1 ) //-> "::"
          //The kind of word may only be specified in English:
        //"Hintergrund {m} (Theater) | im Hintergrund (der Bühne) :: back | upstage {adv}"
          pipeCount = 0;
        else
          prevPipeCharIndex = charIndex;
        break;
      case '{':
        kindOfWordStart = charIndex + 1;
        break;
      case '}':
//        p_kindOfWordStart =
#ifdef _DEBUG
//        std::string kindOfWord = strCurrentWordData.substr(kindOfWordStart,
//          charIndex - kindOfWordStart);
        int i = s_p_i_userinterface->m_dictionaryFileLineNumber;
        i = 55;
#endif
//        NodeTrieNode<TUchemnitzDictionaryReader::extractVocable> * p_ntn =
//          s_nodetrieWordKind.contains(
//          (BYTE *) (array + kindOfWordStart), charIndex - kindOfWordStart,
//          true);
//#else
        NodeTrieNode<TUchemnitzDictionaryReader::insertVocable> * p_ntn =
          s_nodetrieWordKind.contains_inline(
          (BYTE *) (array + kindOfWordStart), charIndex - kindOfWordStart,
          true);
//#endif
        if( p_ntn//->
          )
        {
//          ( * (extractVocable) p_ntn->m_member )(array, numChars
//              /*strCurrentWordData*/, charIndex, pipeCount);
          ExtractVocables(array, numChars
            /*strCurrentWordData*/, charIndex, pipeCount, p_ntn->m_member);
//          InsertAdjective(array);
        }
        break;
    }
  }
}

inline std::istream::int_type TUchemnitzDictionaryReader::UTF8toGermanASCII(
  std::ifstream & dictFile,
  char ar_ch[200] )
{
  bool b1stByteOf2ByteUT8char = false;
  std::istream::int_type _char;
//  _char = dictFile.get();
  unsigned charIndex = 0;
  try
  {
    do
    {
      _char = dictFile.get();
      if( _char > 127 )
      {
        switch (_char)
        {
        case 195: //11000011bin
          b1stByteOf2ByteUT8char = true;
          break;
        //see http://www.utf8-zeichentabelle.de/unicode-utf8-table.pl?number=1024&htmlent=1
        case 164: //10100100bin
          if( b1stByteOf2ByteUT8char )
  //          ch = 'ä';
            ar_ch[charIndex ++] = ASCIIcodepage850::auml /*'ä'*/;
          b1stByteOf2ByteUT8char = false;
          break;
        case 0xb6:
          if( b1stByteOf2ByteUT8char )
            ar_ch[charIndex ++] = ASCIIcodepage850::ouml ; //'ö';
          b1stByteOf2ByteUT8char = false;
          break;
        case 0xbc:
          if( b1stByteOf2ByteUT8char )
            ar_ch[charIndex ++] = ASCIIcodepage850::uuml ;//'ü';
          b1stByteOf2ByteUT8char = false;
          break;
        case 0x9f:
          if( b1stByteOf2ByteUT8char )
            ar_ch[charIndex ++] = ASCIIcodepage850::szlig ;//'ß';
          b1stByteOf2ByteUT8char = false;
          break;
        case 0x84:
          if( b1stByteOf2ByteUT8char )
            ar_ch[charIndex ++] = ASCIIcodepage850::Auml; //'Ä';
          b1stByteOf2ByteUT8char = false;
          break;
        case 0x96:
          if( b1stByteOf2ByteUT8char )
            ar_ch[charIndex ++] = ASCIIcodepage850::Ouml; //'Ö';
          b1stByteOf2ByteUT8char = false;
          break;
        case 0x9c:
          if( b1stByteOf2ByteUT8char )
            ar_ch[charIndex ++] = ASCIIcodepage850::Uuml; //'Ü';
          b1stByteOf2ByteUT8char = false;
          break;
        }
      }
      else
      {
        if( _char == '\n' ) //carriage >R<eturn->line end
          break;
  //      ch = _char;
        ar_ch[charIndex ++] = _char;
        b1stByteOf2ByteUT8char = false;
      }
    }while( //_char != dictFile.eof()
      //see http://www.cplusplus.com/reference/istream/istream/get/
      dictFile.good() );
    ar_ch[charIndex] = '\0';
  }
  catch( ... )
  {
    s_p_i_userinterface->Message("IO exc");
  }
//  if( _char != )
  return //_char;
    charIndex;
}

void TUchemnitzDictionaryReader::extractVocables(const char * filePath)
{
  std::ifstream dictFile;
  dictFile.open(filePath);
//  char * array;
  std::string line;
//  unsigned ui = 1;
  s_p_i_userinterface->m_dictionaryFileLineNumber = 0;
  if( dictFile.is_open() )
  {
    char ar_ch[10000]; //"statistischer Test" entry line goes until colum 9227
    unsigned numChars;
//    std::istream::int_type _char;
    do
    {
//    dictFile.
      //see http://www.cplusplus.com/reference/string/getline/
//      std::getline(dictFile, line);
      /*_char*/ numChars = UTF8toGermanASCII(dictFile, ar_ch );
      extractSingleEntry(//line.c_str(), numChars
        //line
        ar_ch, numChars
        );
//      ++ OneLinePerWordPair::s_dwNumberOfVocabularyPairs ;
  //    s_p_i_userinterface->DictionaryFileLine(ui);
      ++ s_p_i_userinterface->m_dictionaryFileLineNumber;
    }while( //! line.empty()
        //see http://www.cplusplus.com/reference/ios/ios/rdstate/
        ! (dictFile.rdstate() & std::ifstream::eofbit) );
  }
}
