/*
 * TUchemnitzDictionaryReader.cpp
 *
 *  Created on: 05.01.2013
 *      Author: Stefan
 */
#include "TUchemnitzDictionaryReader.hpp"
#include "TUchemnitzDictSeparatorChars.h" //SYNONYM_SEPERATOR_CHAR etc.
#include <fstream> //class std::ofstream
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN()
//#include <VocabularyInMainMem/LetterTree/LetterTree.hpp>
//class IVocabularyInMainMem
#include <VocabularyInMainMem/IVocabularyInMainMem.hpp>
//enum ASCIIcodepage850::ASCIIcodepage850_chars
#include <InputOutput/ASCIIcodepage850.hpp>
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
#include <stdint.h> //

/** Static variables need also to be declared in 1 source file. */
//LetterTree * TUchemnitzDictionaryReader::s_p_lettertree ;
IVocabularyInMainMem * TUchemnitzDictionaryReader::s_p_vocinmainmem;
I_UserInterface * TUchemnitzDictionaryReader::s_p_i_userinterface ;
NodeTrie<TUchemnitzDictionaryReader::insertVocable>
  TUchemnitzDictionaryReader::s_nodetrieWordKind(
    INDEX_OF_LAST_SMALL_LETTER_IN_ASCII, NULL);

#define WORD_DATA_ARRAY_SIZE 10
/** Number of characters for a single dictionary line
 * "statistischer Test" entry in TU Chemnitz dictionary line goes until colum
 * /character position "9227" -> use at least 9227 bytes/ characters for buffer
 * size. */
#define BUFSIZE 10000

//fastestUnsignedDataType TUchemnitzDictionaryReader::m_numBytesRead;

std::ostream & operator << (std::ostream & std_os, const WordData & wd)
{
  if( /*wd.EndIsNotSet() || wd.BeginIsNotSet()*/ wd.GetStringLength() > 0 &&
      wd.begin )
  {
    //end & begin char index may be set but length may <= 0 (e.g. if begin & end
    //index are the same)
    std::string std_str(wd.begin, wd.m_charIndexOfBegin, wd.m_charIndexOfEnd -
      wd.m_charIndexOfBegin + 1);
    return std_os << std_str;
  }
  else
  {
    std_os << "(" << wd.m_charIndexOfEnd << "," << wd.m_charIndexOfBegin << ")";
    return std_os;// << "(" << m_charIndexOfEnd << "," << m_charIndexOfBegin << ")";
  }
}

  std::string WordData::getStdString()
  {
	  int strLen = GetStringLength();
	  if( strLen > 0 )
	  {
		  return std::string(begin, strLen);
	  }
	  return "";
  }


TUchemnitzDictionaryReader::TUchemnitzDictionaryReader(
  I_UserInterface & i_userinterface
  , IVocabularyInMainMem * p_vocaccess)
  : DictionaryReader::DictionaryReaderBase(p_vocaccess)
{
  s_p_i_userinterface = & i_userinterface;
  if( s_nodetrieWordKind.size() == 0 )
  {
//    Build();
    //Wortart: adj, adv, vi=verb intrans. vr=verb reflexiv.
    std::string wordKind = "adj";
    s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
      & TUchemnitzDictionaryReader::InsertAdjective);
    wordKind = "adv";
    s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
      & TUchemnitzDictionaryReader::InsertAdverb);
    wordKind = "m";
    s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
      & TUchemnitzDictionaryReader::InsertMasculineNoun);
    wordKind = "f";
    s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
      & TUchemnitzDictionaryReader::InsertFeminineNoun);
    wordKind = "n";
    s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
      & TUchemnitzDictionaryReader::InsertNeutralNoun);
    wordKind = "vi"; //="Verb Intransitive"
    s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
      & TUchemnitzDictionaryReader::InsertIntransitiveVerb);
    wordKind = "vt"; //="Verb Transitive"
    s_nodetrieWordKind.insert_inline( (BYTE *) wordKind.c_str(), wordKind.size(),
      & TUchemnitzDictionaryReader::InsertTransitiveVerb);
  }
}

/** @return NULL if VocAndTrans object not created */
/*LetterNode * */ VocabularyAndTranslation * TUchemnitzDictionaryReader::
  Insert1stEnglishWord(
//  std::set<LetterNode *> & std_setpletternodeLastStringChar,
//  const std::string & strCurrentWordData,
  const char * ar_chWordBegin,
//  unsigned nCharIndexOf1stWordChar,
//  unsigned nIndexOfCurrentChar,
  unsigned stringLen,
  enum EnglishWord::English_word_class word_class)
{
//  bool bInsertNewVocabularyAndTranslation = true;
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
//  int stringLen = nIndexOfCurrentChar - nCharIndexOf1stWordChar;
//  LetterNode * p_ln = s_p_lettertree->
//    InsertIntoTrieAndHandleVocabularyAndTranslation(
  //    std_setpletternodeLastStringChar,
//    bInsertNewVocabularyAndTranslation//
//    , //byVocabularyType
//    word_class //EnglishWord::adjective_positiveForm
//    , //strCurrentWordData
//      ar_chWordBegin
//    , (int &) stringLen
////      , nCharIndexOf1stWordChar
//    );
  VocabularyAndTranslation * p_vocandtransl =
    //s_p_vocinmainmem
      m_p_vocaccess->InsertAsKeyAndAddVocabularyAttributes(
    ar_chWordBegin
    , (int &) stringLen
    , word_class);
//  VocabularyAndTranslation * p_vocandtransl = s_p_vocinmainmem->
//    InsertAsKeyAndHandleVocabularyAttributes(
//    ar_chWordBegin,
//    stringLen,
//    word_class
//    );
//  if( ! p_ln )
  {
//    std::string str(ar_ch + nCharIndexOf1stWordChar, stringLen);
//    s_p_i_userinterface->Message( "\"" + str + "\" contains invalid characters");
  }
//  return p_ln;
  return p_vocandtransl;
}

void TUchemnitzDictionaryReader::InsertEnglishWord(
  //const std::string & strCurrentWordData,
  const char * const ar_chWordBegin,
//  unsigned nCharIndexOf1stWordChar,
  unsigned stringLen,
//  unsigned nCharIndexOfLastWordChar
  unsigned vocAndTranslArrayIndex,
  const VocabularyAndTranslation * const
    p_vocabularyandtranslation
  )
{
//  const VocabularyAndTranslation * const
//    p_vocabularyandtranslation =
//    s_p_lettertree->s_pvocabularyandtranslation;
//  std::string * p_std_str = & p_vocabularyandtranslation->
//    m_arstrEnglishWord[vocAndTranslArrayIndex ];
//  * p_std_str = std::string( ar_chWordBegin, stringLen);
//  p_vocabularyandtranslation->SetEnglishWord(vocAndTranslArrayIndex,
//    ar_chWordBegin, stringLen);
}

void TUchemnitzDictionaryReader::InsertGermanWord(
  //const std::string & strCurrentWordData,
  const char * const ar_chWordBegin,
//  unsigned nCharIndexOf1stWordChar,
  unsigned stringLen,
//  unsigned nCharIndexOfLastWordChar
  unsigned vocAndTranslArrayIndex,
  VocabularyAndTranslation * p_vocabularyandtranslation
  )
{
//  const VocabularyAndTranslation * const
//    p_vocabularyandtranslation =
//    s_p_lettertree->s_pvocabularyandtranslation;

//  const char * const stringBegin = ar_ch + nCharIndexOf1stWordChar;
//  unsigned stringLen = nCharIndexOfLastWordChar - nCharIndexOf1stWordChar;
//  std::string * p_std_str = & p_vocabularyandtranslation->
//    m_arstrGermanWord[vocAndTranslArrayIndex ];
//  //Add to the structure that also contains the English strings.
//  //So the German equivalent can be retrieved when the last
//  //character (LetterNode) of the English string inside the Trie
//  //is got.
////        g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord[
//  * p_std_str = //std_strCurrentWord;
////    strCurrentWordData.substr(
////      nCharIndexOf1stWordChar
////      , nCharIndexOfLastWordChar - nCharIndexOf1stWordChar
////      ) ;
//      std::string( ar_chWordBegin, stringLen);
  p_vocabularyandtranslation->SetGermanWord(
    ar_chWordBegin, stringLen, vocAndTranslArrayIndex);
}

void TUchemnitzDictionaryReader::InsertEnglishWord(
  const char * ar_ch,
  const WordData & englishWord,
  unsigned vocAndTranslArrayIndex,
  const VocabularyAndTranslation * const
    p_vocabularyandtranslation
  )
{
  //Must be a signed data type, else e.g. "-1" gets the highest positive value.
  int stringLength = englishWord.GetStringLength();
  if( englishWord.BeginIsSet() && stringLength > 0 )
  {
    const char * wordBegin = ar_ch + englishWord.m_charIndexOfBegin;
    InsertEnglishWord(
      wordBegin,
      stringLength,
      vocAndTranslArrayIndex,
      p_vocabularyandtranslation
      );
  }
  else
  {
    std::ostringstream oss;
    oss << "begin index not set for line #" <<
      s_p_i_userinterface->m_dictionaryFileLineNumber << "(content:\n"
      << ar_ch << "\n)";
//    s_p_i_userinterface->Message("begin index not set");
    LOGN_INFO( oss.str() )
  }
}

void TUchemnitzDictionaryReader::InsertGermanWord(
  const char * ar_ch,
  const WordData & germanWord,
  unsigned vocAndTranslArrayIndex,
  VocabularyAndTranslation * p_vocandtransl
  )
{
  //Must be a signed data type, else e.g. "-1" gets the highest positive value.
  int stringLength = germanWord.GetStringLength();
  if( germanWord.BeginIsSet() && stringLength > 0 )
  {
    const char * wordBegin = ar_ch + germanWord.m_charIndexOfBegin;
    //TODO idea: when inserting synonyms for the same English word then
    // only German words need to be allocated?:
    // "car" -> Auto, Kraftwagen
    InsertGermanWord(
      wordBegin,
      stringLength,
      vocAndTranslArrayIndex,
      p_vocandtransl);
  }
  else
  {
//    std::ostringstream oss;
//    oss << "begin index not set for line #" <<
//      s_p_i_userinterface->m_dictionaryFileLineNumber << "(content:\n"
//      << ar_ch << "\n)";
////    s_p_i_userinterface->Message("begin index not set");
//    LOGN_DEBUG( oss.str() )
  }
}

void TUchemnitzDictionaryReader::InsertAdjective(
//  const std::string & strCurrentWordData,
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
//  std::set<LetterNode *> std_setpletternodeLastStringChar;
  VocabularyAndTranslation * p_vocandtransl = Insert1stEnglishWord(
    //strCurrentWordData,
//    std_setpletternodeLastStringChar,
    ar_ch + englishWords[0].m_charIndexOfBegin,
    englishWords[0].GetStringLength(),
    EnglishWord::adjective_positiveForm
    );
  if( p_vocandtransl )
  {
    InsertGermanWord(/*strCurrentWordData,*/
//      ar_ch + germanWords[0].m_charIndexOfBegin,
//      germanWords[0].GetStringLength() ,
      ar_ch,
      germanWords[0],
      0,
      p_vocandtransl
      );
    ++ s_p_i_userinterface->s_numberOfVocabularyPairs;
//    s_p_vocinmainmem->
  }
}

void TUchemnitzDictionaryReader::InsertAdverb(
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
//  std::set<std::string> std_set_vocableWords;
//  std::set<LetterNode *> std_setpletternodeLastStringChar;
  const char * const wordBegin = ar_ch + englishWords[0].m_charIndexOfBegin;
  int numChars = englishWords[0].GetStringLength();
  VocabularyAndTranslation * p_vocandtransl = Insert1stEnglishWord(
    //strCurrentWordData,
//    std_setpletternodeLastStringChar,
    wordBegin,
    numChars,
    EnglishWord::adverb);
//  std_set_vocableWords.insert(std::string(wordBegin, numChars) );
  if( p_vocandtransl )
  {
  //  convertUTF8toASCII();
    InsertGermanWord(/*strCurrentWordData,*/
//      ar_ch + germanWords[0].m_charIndexOfBegin,
//      germanWords[0].GetStringLength(),
      ar_ch,
      germanWords[0],
      0,
      p_vocandtransl
      );
    ++ s_p_i_userinterface->s_numberOfVocabularyPairs;
  }
}

void TUchemnitzDictionaryReader::InsertAndReferToExistingVocData(
//  std::set<LetterNode *> & std_setpletternodeLastStringChar,
//  std::set<std::string> & std_setVocableWords,
  const char * ar_ch,
  enum EnglishWord::English_word_class word_class,
  const WordData & englishWord,
//  unsigned vocAndTranslArrayIndex
  const VocabularyAndTranslation * const p_vocandtranslAllocated
  )
{
  //Must be a signed data type, else e.g. "-1" gets the highest positive value.
  const int stringLength = englishWord.GetStringLength();
  if( englishWord.BeginIsSet() && stringLength > 0 )
  {
    const char * wordBegin = ar_ch + englishWord.m_charIndexOfBegin;
    int stringLengthOrIndexOf1stInvalidChar = stringLength;
//    if( std_setVocableWords.find( std::string(wordBegin, stringLength) ) !=
//        std_setVocableWords.end()
//      )
    {
//    s_p_lettertree->InsertIntoTrieAndReferToExistingVocData(
//      std_setpletternodeLastStringChar,
//      word_class,
//      wordBegin,
//      stringLengthOrIndexOf1stInvalidChar
//      );
      //Also insert if identical words for a vocable, e.g. sing and plural
      // (sheep {sing}, sheep {plural})
      const VocabularyAndTranslation * const p_vocandtransl =
        /*s_p_vocinmainmem*/m_p_vocaccess->InsertAsKeyAndReferToExistingVocData(
        word_class,
        wordBegin,
        stringLengthOrIndexOf1stInvalidChar,
        p_vocandtranslAllocated
        );
      if( /*s_p_lettertree->sp_letternodeLastForInsertedWord*/
          p_vocandtransl == NULL )
      {
        std::ostringstream oss;
        std::string strInvalidChar;
        char chInvalidChar = * (wordBegin + stringLengthOrIndexOf1stInvalidChar);
        if( chInvalidChar ) //<> string-terminating '\0'
          strInvalidChar = chInvalidChar;
        else
          strInvalidChar = "string end";
        oss << "ungueltiges Zeichen " << "\"" << strInvalidChar << "\""
            << "im Wort: \""
          << std::string(wordBegin, stringLength) << "\" in Zeile "
          << s_p_i_userinterface->m_dictionaryFileLineNumber;
  //       std::string strMsg = oss.str();
        s_p_i_userinterface->Message(//"anderer Buchstabe als Sonderzeichen im Wort"
          //  strMsg
          oss.str() ) ;
        LOGN_ERROR( oss.str() )
      }
    }
  }
  else
  {
//    std::ostringstream oss;
//    oss << "begin index not set for line #" <<
//      s_p_i_userinterface->m_dictionaryFileLineNumber << "(content:\n"
//      << ar_ch << "\n)";
////    s_p_i_userinterface->Message("begin index not set");
//    LOGN_DEBUG( oss.str() )
  }
}

/*void*/ VocabularyAndTranslation * TUchemnitzDictionaryReader::InsertNoun(
  const char * const ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
#ifdef _DEBUG
//  const char * const stringBegin = ar_ch + englishWords[0].m_charIndexOfBegin;
#endif
  const int stringLength = englishWords[Singular].GetStringLength();
  const char * wordBegin = ar_ch + englishWords[Singular].m_charIndexOfBegin;

//  std::set<LetterNode *> std_setpletternodeLastStringChar;
  std::set<std::string> std_set_vocableWords;
  /*LetterNode * p_lnAllocatedVocAndTranslObj = */
  VocabularyAndTranslation * p_vocandtransl = TUchemnitzDictionaryReader::
    Insert1stEnglishWord(//strCurrentWordData,
//    std_setpletternodeLastStringChar,
    wordBegin,
    stringLength,
    EnglishWord::/*noun*/ singular);
  std_set_vocableWords.insert( std::string(wordBegin, stringLength) );
  if( /*p_lnAllocatedVocAndTranslObj*/ p_vocandtransl )
  {
//    s_p_lettertree->InsertSingularNounReferringNounAttributes(
//      std_setpletternodeLastStringChar ) ;
      TUchemnitzDictionaryReader::InsertGermanWord(
        ar_ch,
        germanWords[Singular],
        VocabularyAndTranslation::SingularArrayIndex,
        p_vocandtransl
        );
      TUchemnitzDictionaryReader::InsertGermanWord(
        ar_ch,
        germanWords[Plural],
        VocabularyAndTranslation::PluralArrayIndex,
        p_vocandtransl
        );
//    TUchemnitzDictionaryReader::InsertEnglishWord(
//      ar_ch,
//      germanWords[TUchemnitzDictionaryReader::Plural],
//      VocabularyAndTranslation::PluralArrayIndex
//      );
    //not every noun has plural data: "Aalbutt {m} :: plaice"
    const WordData & englishPlural = englishWords[VocabularyAndTranslation::
      PluralArrayIndex];
//    stringLength = englishPlural.GetStringLength();
//    wordBegin = ar_ch + englishPlural.m_charIndexOfBegin;
    /*s_p_lettertree->*/InsertAndReferToExistingVocData(
//      std_setpletternodeLastStringChar,
      ar_ch,
      EnglishWord::/*noun*/ plural_noun, //BYTE byVocType,
//      wordBegin, //const char * const c_strInsertVocAndTranslObjAt,
//      stringLength //int nLength
      englishPlural,
      p_vocandtransl
      );
  //  convertUTF8toASCII();
  //  TUchemnitzDictionaryReader::InsertGermanWord(
  //    ar_ch,
  //    1,
  //    germanWords[1].m_charIndexOfBegin,
  //    germanWords[1].m_charIndexOfEnd);
    ++ s_p_i_userinterface->s_numberOfVocabularyPairs;
  }
  return p_vocandtransl;
}

//TODO return VocabularyAndTranslation pointer
void //VocabularyAndTranslation *
TUchemnitzDictionaryReader::
  InsertFeminineNoun(
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
  VocabularyAndTranslation * p_vocandtranl =
      InsertNoun(ar_ch, englishWords, germanWords);
  if( p_vocandtranl)
    p_vocandtranl->m_arbyAttribute[0] = //GermanNoun::die;
      VocabularyAndTranslation::noun_gender_female;
//  return p_vocandtranl;
}

void TUchemnitzDictionaryReader::InsertMasculineNoun(
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
  VocabularyAndTranslation * p_vocandtranl =
    InsertNoun(ar_ch, englishWords, germanWords);
  if( p_vocandtranl )
    p_vocandtranl->m_arbyAttribute[0] = GermanNoun::der;
}

void TUchemnitzDictionaryReader::InsertNeutralNoun(
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
  VocabularyAndTranslation * p_vocandtranl =
      InsertNoun(ar_ch, englishWords, germanWords);
  if( p_vocandtranl )
    p_vocandtranl->m_arbyAttribute[0] = GermanNoun::das;
}

void GetFiniteWord(const char * ar_ch, WordData & finiteGermanWord)
{
  if( finiteGermanWord.BeginIsSet() )
  {
    int index = finiteGermanWord.m_charIndexOfBegin;
  //      while( * (ar_ch + index) )
    for( ; index < finiteGermanWord.m_charIndexOfEnd ; ++ index )
    {
      //e.g. "er/sie arbeitet"
      if( *(ar_ch + index) == ' ' ) //first space after personal pronouns
      {
        finiteGermanWord.m_charIndexOfBegin = index + 1;
        break;
      }
    }
  }
}

void TUchemnitzDictionaryReader::InsertFiniteGermanVerbForms(
  const char * ar_ch, const WordData germanWords[],
  VocabularyAndTranslation * p_vocandtransl)
{
  std::string std_strWortstamm;
  const WordData & germanInfinitive = germanWords[Infinitive];
//    GermanVerb::GetWordStem(ar_ch, germanWords[Infinitive], std_strWortstamm);
//    GermanVerb::Get1stPersSing(ar_ch, germanWords[Infinitive] );
  GermanVerb::GetWordStem( ar_ch + germanInfinitive.m_charIndexOfBegin,
    germanInfinitive.GetStringLength(), std_strWortstamm );

  std::string std_strFiniteForm;
  for( unsigned index = GermanVerb::firstPersonSing;
      index < GermanVerb::beyondLastPerson; ++ index)
  {
    std_strFiniteForm = GermanVerb::GetPresentFiniteForm(std_strWortstamm,
      (enum GermanVerb::person_indices) (GermanVerb::firstPersonSing + index) );
    InsertGermanWord(std_strFiniteForm.c_str(), std_strFiniteForm.length(),
      GermanVerb::arrayIndexFor1stPersSingPres + index,
      p_vocandtransl);

    if( index == GermanVerb::secondPersonSing ) //Skip 3rd person.
      ++ index;
  }

  WordData & germanThirdPersPres = (WordData &) germanWords[ThirdPersSingPres];
  GetFiniteWord(ar_ch, germanThirdPersPres);
  InsertGermanWord(ar_ch, germanThirdPersPres,
    GermanVerb::arrayIndexFor3rdPersSingPres,
    p_vocandtransl);

  WordData & germanThirdPersPast = (WordData &) germanWords[ThirdPersSingPast];
  GetFiniteWord(ar_ch, germanThirdPersPast);
  InsertGermanWord(ar_ch, germanThirdPersPast,
    GermanVerb::arrayIndexFor3rdPersSingPast,
    p_vocandtransl);

  std::string thirdPersPast = std::string(ar_ch + germanThirdPersPast.
    m_charIndexOfBegin, germanThirdPersPast.GetStringLength() );
  for( unsigned index = GermanVerb::firstPersonSing;
      index < GermanVerb::beyondLastPerson; ++ index)
  {
    std_strFiniteForm = GermanVerb::GetPastFiniteForm(//std_strWortstamm
      ar_ch + germanThirdPersPast.m_charIndexOfBegin,
      germanThirdPersPast.GetStringLength()
      , (enum GermanVerb::person_indices) (GermanVerb::firstPersonSing + index)
      );
    InsertGermanWord(std_strFiniteForm.c_str(), std_strFiniteForm.length(),
      GermanVerb::arrayIndexFor1stPersSingPast + index,
      p_vocandtransl);
    if( index == GermanVerb::secondPersonSing ) //Skip 3rd person.
      ++ index;
  }
}

void TUchemnitzDictionaryReader::InsertGermanVerbWords(
  const char * const ar_ch, const WordData germanWords[10],
  VocabularyAndTranslation * p_vocandtransl)
{
  /** German TU Chemnitz dictionary entry:
  * "arbeiten {vi} (an) | arbeitend | gearbeitet | arbeitet | arbeitete ::"
  * Infinitiv             Partizip    Partizip     Präsens    Präteritum
  *                       Präsens     Perfekt      gebeugt    gebeugt */
  TUchemnitzDictionaryReader::InsertGermanWord(
    ar_ch,
    germanWords[Infinitive],
    GermanVerb::arrayIndexForInfinitive,
    p_vocandtransl
    );
  TUchemnitzDictionaryReader::InsertGermanWord(
    ar_ch,
    germanWords[PastParticiple],
    GermanVerb::arrayIndexForPastParticiple,
    p_vocandtransl
    );

  InsertFiniteGermanVerbForms(ar_ch, germanWords, p_vocandtransl);

  TUchemnitzDictionaryReader::InsertGermanWord(
    ar_ch,
    germanWords[ThirdPersSingPres],
    GermanVerb::arrayIndexFor3rdPersSingPres,
    p_vocandtransl
    );
  TUchemnitzDictionaryReader::InsertGermanWord(
    ar_ch,
    germanWords[ThirdPersSingPast],
    GermanVerb::arrayIndexFor3rdPersSingPast,
    p_vocandtransl
    );
}

//":: to work {work, wrought; worked, wrought} | working | worked | he/she works | I/he/she worked |
void TUchemnitzDictionaryReader::InsertEnglishVerbWords(
//  std::set<LetterNode *> & std_setpletternodeLastStringChar,
//  std::set<std::string> & std_setVocableWords,
  const char * ar_ch,
  const WordData englishWords[10],
  enum EnglishWord::English_word_class english_word_class//,
//  VocabularyAndTranslation * p_vocandtransl
  , const VocabularyAndTranslation * const p_vocandtranslAllocated
  )
{
//    InsertEnglishWord(
//      ar_ch,
//      englishWords[EnglishArrayIndices::Progressive],
//      VocabularyAndTranslation::arrayIndexForInfinitive
//      );
//  int stringLength = englishWords[Infinitive].GetStringLength();
//  const char * wordBegin = ar_ch + englishWords[Infinitive].m_charIndexOfBegin;

  BYTE byNumberOfObjectsAllowed = english_word_class ==
    EnglishWord::main_verb_allows_0object_infinitive ? 0 : 1;

//  s_p_lettertree->Insert3rdPersonSingularPresentReferringVerbAttributes(
//    std_setpletternodeLastStringChar, wordBegin, stringLength,
//    byNumberOfObjectsAllowed
//    );
//  enum EnglishWord::English_word_class thirdPerson_word_class = EnglishVerb::
//    Get3rdPersonVerbWordClass(byNumberOfObjectsAllowed);
  WordData & englishThirdPersPres = (WordData &) englishWords[ThirdPersSingPres];
  GetFiniteWord(ar_ch, englishThirdPersPres);
//  InsertEnglishWord(ar_ch, englishThirdPersPres, EnglishVerb::third_person_present);


  InsertAndReferToExistingVocData(
//    std_setpletternodeLastStringChar,
    ar_ch,
    byNumberOfObjectsAllowed == 0 ? EnglishWord:://mainVerbAllows0object3rdPersonSingularPresent, //byVocType,
      mainVerbAllows0object3rdPersonSingularPresent :
      EnglishWord::mainVerbAllows1object3rdPersonSingularPresent,
      englishThirdPersPres //stdstrProgressiveForm
      , p_vocandtranslAllocated
    );

  InsertAndReferToExistingVocData(
//    std_setpletternodeLastStringChar,
    ar_ch,
    byNumberOfObjectsAllowed == 0 ? EnglishWord:://mainVerbAllows0object3rdPersonSingularPresent, //byVocType,
      main_verb_allows_0object_progressive_form :
      EnglishWord::main_verb_allows_1object_progressive_form,
    englishWords[Progressive] //stdstrProgressiveForm
     , p_vocandtranslAllocated
    );
  InsertAndReferToExistingVocData(
//    std_setpletternodeLastStringChar,
    ar_ch,
    byNumberOfObjectsAllowed == 0 ? EnglishWord:://mainVerbAllows0object3rdPersonSingularPresent, //byVocType,
        mainVerbPastParticiple0Obj :
      EnglishWord::mainVerbPastParticiple1Obj,
    englishWords[PastParticiple] //stdstrProgressiveForm
    , p_vocandtranslAllocated
    );

  WordData & englishThirdPersPast = (WordData &) englishWords[ThirdPersSingPast];
  GetFiniteWord(ar_ch, englishThirdPersPast);
  InsertAndReferToExistingVocData(
//    std_setpletternodeLastStringChar,
    ar_ch,
    byNumberOfObjectsAllowed == 0 ? EnglishWord:://mainVerbAllows0object3rdPersonSingularPresent, //byVocType,
        main_verb_allows_0object_past_form :
      EnglishWord::main_verb_allows_1object_past_form,
      englishThirdPersPast //stdstrProgressiveForm
      , p_vocandtranslAllocated
    );
}

/** @param english_word_class: verb with 0 or 1 object */
void TUchemnitzDictionaryReader::InsertVerb(
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10],
  enum EnglishWord::English_word_class english_word_class)
{
  int stringLength = englishWords[Infinitive].GetStringLength();
  const char * wordBegin = ar_ch + englishWords[Infinitive].m_charIndexOfBegin;
//  std::set<LetterNode *> std_setpletternodeLastStringChar;
  std::set<std::string> std_setVocableWords;
  /*LetterNode * p_lnAllocatedVocAndTranslObj = */
  VocabularyAndTranslation * p_vocandtransl = TUchemnitzDictionaryReader::
    Insert1stEnglishWord(
//    std_setpletternodeLastStringChar,
    wordBegin,
    stringLength,
    english_word_class
    );
  if( /*p_lnAllocatedVocAndTranslObj*/ p_vocandtransl != NULL )
  {
    std_setVocableWords.insert( std::string(wordBegin, stringLength) );
    InsertGermanVerbWords(ar_ch, germanWords, p_vocandtransl);

    InsertEnglishVerbWords(
//      std_setpletternodeLastStringChar,
//      std_setVocableWords,
      ar_ch, englishWords,
      english_word_class//, p_vocandtransl
      , p_vocandtransl
      );
    ++ s_p_i_userinterface->s_numberOfVocabularyPairs;
  }
}

void TUchemnitzDictionaryReader::InsertTransitiveVerb(
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
  InsertVerb(ar_ch, englishWords, germanWords,
    EnglishWord::/*main_verb*/ main_verb_allows_1object_infinitive);
}

void TUchemnitzDictionaryReader::InsertIntransitiveVerb(
  const char * ar_ch,
  const WordData englishWords[10], const WordData germanWords[10])
{
  InsertVerb(ar_ch, englishWords, germanWords,
    EnglishWord::/*main_verb*/ main_verb_allows_0object_infinitive);
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
  const unsigned englishPipeCount,
  const unsigned germanPipeCount,
  const unsigned semicolCountInsideCurrentPipeCharRange,
  const fastestUnsignedDataType semicolCountInsidePipeCharRangeFor1stWord,
  bool bEnglish,
  unsigned nIndexOfCurrentChar,
  TUchemnitzDictionaryReader::insertVocable pfn)
{
  bool bSetBegin = false;
  if( ! insideBracket && wordIndex < WORD_DATA_ARRAY_SIZE
      //&& germanPipeCount == englishPipeCount
      )
  {
    //e.g. "to fuse; to burn out; to blow"
    //e.g. "Aalleiter {f}; Aaltreppe {f}; Aalpass {m} (Wasserbau) |
    // Aalleitern {pl}; Aaltreppen {pl}; Aalpässe {pl} :: eel ladder
    // (water engineering) | eel ladders"
    if( ( pfn == & TUchemnitzDictionaryReader::InsertIntransitiveVerb ||
        pfn == & TUchemnitzDictionaryReader::InsertTransitiveVerb //||
//        pfn == & TUchemnitzDictionaryReader::InsertMasculineNoun ||
//        pfn == & TUchemnitzDictionaryReader::InsertFeminineNoun ||
//        pfn == & TUchemnitzDictionaryReader::InsertNeutralNoun
        ) &&
        semicolCountInsideCurrentPipeCharRange == 0)
    {
//      if( bEnglish )
      bSetBegin = true;
    }
    else
      //The corresponding plural for a noun is: the same semicolon index after the
      // next pipe ('|') character.
      if( semicolCountInsidePipeCharRangeFor1stWord ==
          semicolCountInsideCurrentPipeCharRange &&
//          && germanPipeCount == englishPipeCount
          ( pfn == & TUchemnitzDictionaryReader::InsertMasculineNoun ||
        pfn == & TUchemnitzDictionaryReader::InsertFeminineNoun ||
        pfn == & TUchemnitzDictionaryReader::InsertNeutralNoun )
        )
        bSetBegin = true;
      else if( englishPipeCount == germanPipeCount)
        bSetBegin = true;
    if( bSetBegin )
    {
      if( bEnglish
		  && semicolCountInsidePipeCharRangeFor1stWord ==
			semicolCountInsideCurrentPipeCharRange
    		  )
      {
        if( englishWords[wordIndex].BeginIsNotSet() )
        {
          if( pfn == & TUchemnitzDictionaryReader::InsertIntransitiveVerb )
            nIndexOfCurrentChar +=
              //for "to "
              + 3;
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
}

inline void PossiblySetVocableEnd(
//  WordData englishWords[10],
//  WordData germanWords[10],
  WordData words[10],
  unsigned & wordIndex,
  bool insideBracket,
  unsigned pipeCount,
  unsigned germanPipeCount,
//  bool bEnglish,
  const unsigned semicolCountInsideCurrrentPipeCharRange,
  const unsigned semicolCountInsidePipeCharRangeFor1stWord,
  unsigned nIndexOfVocEndChar,
  TUchemnitzDictionaryReader::insertVocable pfn
  )
{
  if( //Skip ";" in "(vergossenes; geronnenes)"
      ! insideBracket && wordIndex < WORD_DATA_ARRAY_SIZE
      )
  {
    bool bSetEnd = false;
    //e.g. "to fuse; to burn out; to blow"
    //e.g. "Aalleiter {f}; Aaltreppe {f}; Aalpass {m} (Wasserbau) |
    // Aalleitern {pl}; Aaltreppen {pl}; Aalpässe {pl} :: eel ladder
    // (water engineering) | eel ladders"
    if( ( pfn == & TUchemnitzDictionaryReader::InsertIntransitiveVerb ||
        pfn == & TUchemnitzDictionaryReader::InsertTransitiveVerb //||
//        pfn == & TUchemnitzDictionaryReader::InsertMasculineNoun ||
//        pfn == & TUchemnitzDictionaryReader::InsertFeminineNoun ||
//        pfn == & TUchemnitzDictionaryReader::InsertNeutralNoun
        )
        &&
        semicolCountInsideCurrrentPipeCharRange == 0)
      bSetEnd = true;
    else
      if( semicolCountInsidePipeCharRangeFor1stWord ==
          semicolCountInsideCurrrentPipeCharRange && (
          pfn == & TUchemnitzDictionaryReader::InsertMasculineNoun ||
          pfn == & TUchemnitzDictionaryReader::InsertFeminineNoun ||
          pfn == & TUchemnitzDictionaryReader::InsertNeutralNoun )
        )
        bSetEnd = true;
      else if( pipeCount == germanPipeCount )
        bSetEnd = true;
    if( bSetEnd )
    {
      WordData & word = //bEnglish ? englishWords[wordIndex] : germanWords[wordIndex];
        words[wordIndex];
//      if( bEnglish)
      {
        if( word.BeginIsSet() //to not insert " " for  "{m} |"
            && word.EndIsNotSet() )
        {
          word.m_charIndexOfEnd = nIndexOfVocEndChar;
//          ++ wordIndex;
//            englishVocables.push_back(englishWords);
//            Reset(englishWords);
//              wordIndex = 0;
        }
      }
//      else
//      {
//        if( .EndIsNotSet() )
//          germanWords[wordIndex ++].m_charIndexOfEnd = nIndexOfVocEndChar;
////            germanVocables.push_back(germanWords);
////            Reset(germanWords);
////            wordIndex = 0;
//      }
    }
  }
}

/** "Lexikon {n}; Verzeichnis {n}; Wörterbuch {n} | Lexika {pl}; Verzeichnisse {pl};"
 *  Auto {n}; Wagen {m}; Kraftwagen {m} | Autos {pl}; Wagen {pl}; Kraftwagen {pl}
 *  :: car | cars |
 *  -> "Auto, Autos"
 * */
void TUchemnitzDictionaryReader::ExtractVocables(//const char * array
//  const std::string & strCurrentWordData,
  const char * ar_ch,
  unsigned numChars,
  unsigned charIndex,
  const unsigned germanPipeCount,
  const unsigned semicolCountInsidePipeCharRangeFor1stWord,
  insertVocable pfn,
  TUchemnitzDictionaryReader & tuchemnitzdictionaryreader
  )
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
//  std::map<unsigned,WordData[10]> englishVocables, germanRelatedVocables;
  WordData englishWords[WORD_DATA_ARRAY_SIZE], germanWords[WORD_DATA_ARRAY_SIZE];
  unsigned pipeCount = 0;
  unsigned wordIndex = 0;
  unsigned semicolCountInsideCurrentPipeCharRange = 0;
  bool insideBracket = false;
  for( unsigned nIndexOfCurrentChar = 0 ; nIndexOfCurrentChar <
    /*strCurrentWordData.length()*/ numChars
    /*&& wordIndex < WORD_DATA_ARRAY_SIZE*/; nIndexOfCurrentChar ++ )
  {
    switch(ar_ch[nIndexOfCurrentChar])
    {
      case ' ' :
//        previousCharIsSpace = true;
        indexOfSpaceChar = nIndexOfCurrentChar;
        break;
      case ';' : //e.g. "| in b\366ser Absicht; boswillig {adv} ::" :
        // insert "in b\366ser Absicht" and "boswillig" for "maliciously"
        PossiblySetVocableEnd(bEnglish ? englishWords : germanWords, wordIndex,
          insideBracket,
          pipeCount, germanPipeCount, //bEnglish,
          semicolCountInsideCurrentPipeCharRange,
          semicolCountInsidePipeCharRangeFor1stWord,
          nIndexOfCurrentChar, pfn);
        ++ semicolCountInsideCurrentPipeCharRange;
        break;
      case '}' :
      case ')' : //e.g. "pleasantly stimulating (cosmetics)"
      case '>' : // "reversionary <reversional>"
      case ']' : // e.g. "moribund [fig.]"
        insideBracket = false;
        break;
      case '{' : //e.g. "nicht {adj}"
      case '(' : //e.g. "pleasantly stimulating (cosmetics)"
      case '<' : // "reversionary <reversional>"
      case '[' : // e.g. "moribund [fig.]"
          //kind of word can also stand in English part: "interdepartmental {adj}"
          //bracket may be the first character as in ":: (from) out of town;"
        PossiblySetVocableEnd(bEnglish ? englishWords : germanWords, wordIndex,
          insideBracket, pipeCount, germanPipeCount, //bEnglish,
          semicolCountInsideCurrentPipeCharRange,
          semicolCountInsidePipeCharRangeFor1stWord,
          nIndexOfCurrentChar - 1, pfn);
        insideBracket = true;
        break;
      case '|' :
//        if( bEnglish )
//        {
//        }
        PossiblySetVocableEnd(bEnglish ? englishWords : germanWords, wordIndex,
          insideBracket, pipeCount, germanPipeCount, //bEnglish,
          semicolCountInsideCurrentPipeCharRange,
          semicolCountInsidePipeCharRangeFor1stWord,
          nIndexOfCurrentChar - 1, pfn);
        ++ pipeCount;
        ++ wordIndex;
        semicolCountInsideCurrentPipeCharRange = 0;
//        ++ delemiterCount;
//        PossiblySetStringBegin(englishWords, germanWords, wordIndex,
//          insideBracket, pipeCount, germanPipeCount, semicolCountInsideCurrentPipeCharRange, bEnglish,
//          nIndexOfCurrentChar + 2, pfn);
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
//          PossiblySetStringBegin(englishWords, germanWords, wordIndex,
//            insideBracket, pipeCount, germanPipeCount, semicolCountInsideCurrentPipeCharRange, bEnglish,
//            nIndexOfCurrentChar + 2, pfn);
        }
        charIndexOfPreviousColon = nIndexOfCurrentChar;
        break;
      default: //letter
        PossiblySetStringBegin(englishWords, germanWords, wordIndex,
          insideBracket,
          pipeCount,
          germanPipeCount,
          semicolCountInsideCurrentPipeCharRange,
          semicolCountInsidePipeCharRangeFor1stWord,
          bEnglish,
          nIndexOfCurrentChar, pfn);
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
  if( englishWords[0].BeginIsNotSet() ||
      germanWords[0].BeginIsNotSet() )
  {
    std::ostringstream oss;
    oss << "begin index not set for line #" <<
      s_p_i_userinterface->m_dictionaryFileLineNumber << "(content:\n"
      << ar_ch << "\n), pipe index:"
      << germanPipeCount;
//    s_p_i_userinterface->Message("begin index not set");
    LOGN_WARNING( oss.str() )
  }
  else
    if( englishWords[0].GetStringLength() > 0 &&
      germanWords[0].GetStringLength() > 0 )
      /** Calls the member function via function pointer "pfn" on the
       *  object "tuchemnitzdictionaryreader" */
//      return
        (tuchemnitzdictionaryreader.*pfn) //(*pfn)
        (
        ar_ch,
        englishWords,
        germanWords);
    else
    {
      std::ostringstream oss;

      oss << "array index out of bounds in line #" <<
        s_p_i_userinterface->m_dictionaryFileLineNumber << ":"
//        << englishWords[0] << " " << germanWords[0]
        ;
      LOGN_WARNING( oss.str() )
      s_p_i_userinterface->Message(oss.str());
    }
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
// deutsches Wort[; Synonym] '{' Wortart '}' [| deutsches Beispiel] ::
//  englisches Wort [; Synonym] [| englisches Beispiel]
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
  unsigned wordStart = 0;
  unsigned prevPipeCharIndex = 0;
  /** Separates synonyms */
  unsigned semicolCountInsideCurrentPipeCharRange = 0;
//  const char * array = strCurrentWordData.c_str();
  for( unsigned charIndex = 0; charIndex < //strCurrentWordData.size();
      numChars;
      ++ charIndex)
  {
    ch = array[charIndex];
    switch(ch)
    {
      case SAME_VOCABLE_SEPERATOR_CHAR :
        ++ pipeCount; //next word
        semicolCountInsideCurrentPipeCharRange = 0;
        break;
      case SYNONYM_SEPERATOR_CHAR:
        ++ semicolCountInsideCurrentPipeCharRange;
        //for "Auto {n}; Wagen {m};"
        wordStart = charIndex + 2;
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
          bool bExtractVocables = false;
//          ( * (extractVocable) p_ntn->m_member )(array, numChars
//              /*strCurrentWordData*/, charIndex, pipeCount);
          if( p_ntn->m_member == & TUchemnitzDictionaryReader::InsertIntransitiveVerb )
          {
            //Else: "tries to add other data like finite verbs for
            //"[...]; anlaufen (Vorgang) {vi}" although there is no such data.
            if( semicolCountInsideCurrentPipeCharRange == 0 && pipeCount == 0 )
              bExtractVocables = true;
          }
          else
            bExtractVocables = true;
          if( bExtractVocables )
//            return
            ExtractVocables(
              array + wordStart,
              numChars
              /*strCurrentWordData*/, charIndex,
              pipeCount,
              semicolCountInsideCurrentPipeCharRange,
              p_ntn->m_member,
              * this);
//          InsertAdjective(array);
        }
        break;
    }
  }
}

inline void AssignNonASCIIchars( std::istream::int_type _char,
    char ar_ch[], unsigned & charIndex, bool & b1stByteOf2ByteUT8char
    //,unsigned & numBytesRead
    )
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

/** @return number of characters in array excluding newline (\r,\n) chars */
/*inline*/ std::istream::int_type TUchemnitzDictionaryReader::UTF8toGermanASCII(
  std::ifstream & dictFile,
  char ar_ch[]//,
  //fastestUnsignedDataType & numNewlineChars
  )
{
  bool b1stByteOf2ByteUT8char = false;
  /** Make static so the variable is not created for every call of this
   * function.*/
  //TODO check via performance tests/ in assembler code whether a static
  //variable is really faster.
  static std::istream::int_type _char;
//  _char = dictFile.get();
  unsigned charIndex = 0;
  try
  {
    bool continueLoop = true;
    do
    {
      _char = dictFile.get();
      //if(_char != std::traits::eof )
      if( dictFile.good() )
        ++ m_numBytesRead;
      if( _char > 127 )
      {
        AssignNonASCIIchars(_char, ar_ch, charIndex, b1stByteOf2ByteUT8char
          //,m_numBytesRead
          );
      }
      else
      {
        switch(_char)
        {
          //"\r\n": for Linux "\r\n" is read, for Windows just "\n" ?
          case '\r': //carriage >R<eturn->line end
            ar_ch[charIndex] = '\0';
  //          continueLoop = false;
//            charIndex = BUFSIZE;
//            continue; // go to loop begin / directly read next char
//            ++ numNewlineChars;
            break;
          case '\n': //Newline->line end
//            ar_ch[charIndex] = '\0';
            continueLoop = false;
//            charIndex = BUFSIZE;
//            ++ numNewlineChars
            break;
          default:
            //      ch = _char;
            ar_ch[charIndex ++] = _char;
            b1stByteOf2ByteUT8char = false;
            break;
        }
      }
      if( continueLoop )
        //see http://www.cplusplus.com/reference/istream/istream/get/
        continueLoop = dictFile.good();
//      if( ! continueLoop )
//        charIndex = BUFSIZE;
    }while( //_char != dictFile.eof()
        continueLoop &&
        charIndex < BUFSIZE );
  }
  catch( ... )
  {
    s_p_i_userinterface->Message("IO exc");
  }
//  if( _char != )
  return //_char;
    charIndex;
}

bool TUchemnitzDictionaryReader::extractVocables(const char * filePath)
{
  std::ifstream dictFile;
  //TODO Deny write access to other processes when opening the file. This cannot
  // be done with the C++ std::ifstream? but must be implemented with OS
  // specific functions? Possibly use STLsoft for that.
  // Else errors when reading from the file may occur/ the progress status
  // may vary when the file size can be changed.
  dictFile.open(filePath);
  bool dictFileIsOpen = dictFile.is_open();
  if( dictFileIsOpen )
  {
    dictFile.seekg(0, std::ios_base::end);
    m_fileSizeInBytes = dictFile.tellg();
    dictFile.seekg(0, std::ios_base::beg);
  //  m_p_vocaccess->
  //  dictFile.rdbuf()->open(filePath, std::ios_base::app, _SH_DENYWR);
  //  char * array;
    std::string line;
  //  unsigned ui = 1;
    s_p_i_userinterface->m_dictionaryFileLineNumber = 1;
    m_numBytesRead = 0;
    char ar_chDictionaryLine[BUFSIZE];
    unsigned numChars = BUFSIZE;
//    std::istream::int_type _char;
    do
    {
      m_currentLineBeginFileOffset = m_numBytesRead;
//    dictFile.
      //see http://www.cplusplus.com/reference/string/getline/
//      std::getline(dictFile, line);
      /*_char*/ numChars = UTF8toGermanASCII(dictFile, ar_chDictionaryLine );
      extractSingleEntry(//line.c_str(), numChars
        //line
        ar_chDictionaryLine, numChars
        );
//      ++ OneLinePerWordPair::s_dwNumberOfVocabularyPairs ;
  //    s_p_i_userinterface->DictionaryFileLine(ui);
      ++ s_p_i_userinterface->m_dictionaryFileLineNumber;
      //TODO show this value in user interface.
//      m_numBytesRead += numChars + 2;
    }while( //! line.empty()
      //see http://www.cplusplus.com/reference/ios/ios/rdstate/
//        ! (dictFile.rdstate() & std::ifstream::eofbit)
      dictFile.good()
      );
    dictFile.close();
  }
  return dictFileIsOpen;
}
