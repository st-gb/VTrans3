//This file contains file format specific code.

//Ohne das folgende include: fatal error C1010: Unerwartetes Dateiende 
//waehrend der Suche nach dem vorkompilierten Header.
#include "StdAfx.h"
#include "IO.hpp"
#include "rest.h" //for LOGN() etc.
#include "Word.hpp" //for "class Word" etc.
#include <fstream> //for std::ofstream
#include <ios>//fo ios_base
#include "charsetconv.h"
#include "VocabularyInMainMem/LetterTree/LetterTree.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordNode.hpp" //for "class WordNode" etc.

#include <sstream> //for ostringstream
#ifdef __CYGWIN__
//cygwin: from dir "cygwin/usr/include/mingw"
#include <mingw/tchar.h> //for _T()
#else //MinGW, MS VC++
#include <tchar.h> //for _T()
#endif

//class WordList ;

extern std::ofstream ofstreamLogFile; //for LOGN(...)
extern WordList wordList;

LetterTree g_lettertree ;
//VocabularyAndTranslation * g_pvocabularyandtranslation ;

//CString EncodeToUTF8(LPCTSTR szSource)
//{
//  WORD ch;
//
//  BYTE bt1, bt2, bt3, bt4, bt5, bt6;
//
//  int n, nMax = _tcslen(szSource);
//
//  CString sFinal, sTemp;
//
//  for (n = 0; n < nMax; ++n)
//  {
//    ch = (WORD)szSource[n];
//
//    if (ch == _T('='))
//    {
//      sTemp.Format(_T("=%02X"), ch);
//
//      sFinal += sTemp;
//    }
//    else if (ch < 128)
//    {
//      sFinal += szSource[n];
//    }
//    else if (ch <= 2047)
//    {
//       bt1 = (BYTE)(192 + (ch / 64));
//       bt2 = (BYTE)(128 + (ch % 64));
//
//      sTemp.Format(_T("=%02X=%02X"), bt1, bt2);
//
//      sFinal += sTemp;
//    }
//    else if (ch <= 65535)
//    {
//       bt1 = (BYTE)(224 + (ch / 4096));
//       bt2 = (BYTE)(128 + ((ch / 64) % 64));
//       bt3 = (BYTE)(128 + (ch % 64));
//
//      sTemp.Format(_T("=%02X=%02X=%02X"), bt1, bt2, bt3);
//
//      sFinal += sTemp;
//    }
//    else if (ch <= 2097151)
//    {
//       bt1 = (BYTE)(240 + (ch / 262144));
//       bt2 = (BYTE)(128 + ((ch / 4096) % 64));
//       bt3 = (BYTE)(128 + ((ch / 64) % 64));
//       bt4 = (BYTE)(128 + (ch % 64));
//
//      sTemp.Format(_T("=%02X=%02X=%02X=%02X"), bt1, bt2, bt3, bt4);
//      sFinal += sTemp;
//    }
//    else if (ch <=67108863)
//    {
//      bt1 = (BYTE)(248 + (ch / 16777216));
//      bt2 = (BYTE)(128 + ((ch / 262144) % 64));
//      bt3 = (BYTE)(128 + ((ch / 4096) % 64));
//      bt4 = (BYTE)(128 + ((ch / 64) % 64));
//      bt5 = (BYTE)(128 + (ch % 64));
//
//      sTemp.Format(_T("=%02X=%02X=%02X=%02X=%02X"), bt1, bt2, bt3, bt4, bt5);
//      sFinal += sTemp;
//    }
//    else if (ch <=2147483647)
//    {
//       bt1 = (BYTE)(252 + (ch / 1073741824));
//       bt2 = (BYTE)(128 + ((ch / 16777216) % 64));
//       bt3 = (BYTE)(128 + ((ch / 262144) % 64));
//       bt4 = (BYTE)(128 + ((ch / 4096) % 64));
//       bt5 = (BYTE)(128 + ((ch / 64) % 64));
//       bt6 = (BYTE)(128 + (ch % 64));
//
//      sTemp.Format(_T("=%02X=%02X=%02X=%02X=%02X=%02X"),
//                bt1, bt2, bt3, bt4, bt5, bt6);
//      sFinal += sTemp;
//    }
//  }
//  return sFinal;
//}

#define _INSERT_INTO_HASH_TREE

//void OneLinePerWordPair::InsertSingularNounReferringNounAttributes()
//{
//  VocabularyAndTranslation * p_vocabularyandtranslationEnglishNoun =
//    g_lettertree.s_pvocabularyandtranslation ;
//
//  //        g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
//  //          stdsetpletternodeLastStringChar
//  //          //, LetterNode * pletternodeCurrent
//  //          //, VocabularyAndTranslation * pvocabularyandtranslation
//  //          , bInsertNewVocabularyAndTranslation
//  //          , EnglishWord::singular
//  //          , strCurrentWordData
//  //          , //nIndexOfCurrentChar - nIndexOf1stChar
//  //            nLength
//  //          , nIndexOf1stChar
//  //          ) ;
//  //Force insertion of new VocAndTransl(singular) object.
//  bool bDoInsertNewVocabularyAndTranslation = true ;
//  //For parsing (constructing a grammar rule ) "a >>singular<<"
//  //(if the rule was just "a >>noun<<", then
//  // "a cars" would also be possible.
//  g_lettertree.HandleVocabularyAndTranslationPointerInsertion(
//    stdsetpletternodeLastStringChar
//  //          , p_letternodeLastForInsertedWord
//    //, pvocabularyandtranslation
//    , bDoInsertNewVocabularyAndTranslation
//    , EnglishWord::singular
//    ) ;
//  //object for singular
//  //refer/ point to the attributes of the "main verb" (saves storage instead
//  // of allocating array for the singular, too)
//  g_lettertree.s_pvocabularyandtranslation->m_arstrEnglishWord =
//      p_vocabularyandtranslationEnglishNoun->m_arstrEnglishWord ;
//  g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord=
//      p_vocabularyandtranslationEnglishNoun->m_arstrGermanWord ;
//  g_lettertree.s_pvocabularyandtranslation->m_arbyAttribute=
//      p_vocabularyandtranslationEnglishNoun->m_arbyAttribute ;
//
//  //The pointer points to the singular VocAndTransl object. Set it back
//  //to the address of the noun object in order to assign the plural etc.
//  //correctly.
//  g_lettertree.s_pvocabularyandtranslation =
//    p_vocabularyandtranslationEnglishNoun ;
//  //              en->m_strSingular = //str.Mid(start,i-start);
//  //                str.substr(start,i-start);
//
//  //If the singular and the plural are identical: add only once to
//  //the "trie" structure/ add only 1 VocabularyAndTranslation to the
//  //last LetterNode.
////  bInsertNewVocabularyAndTranslation = false ;
//  }

void OneLinePerWordPair::InsertEnglishNoun(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  bool bInsertNewVocabularyAndTranslation = true ;
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
  EnglishNoun * en = new EnglishNoun;
  BYTE delemiterCount = 0 ;
  BYTE otherCount = 0 ;
  int nIndexOf1stChar = 1;
  int nIndexOfCurrentChar ;
  //TRACE("englisches Substantiv: %s\n", str);
#ifdef _INSERT_INTO_HASH_TREE
  LetterNode * pletternode = NULL ;
//        VocabularyAndTranslation * pvocabularyandtranslation = NULL ;
//        VocabularyAndTranslation * pvocabularyandtranslationReturn = NULL ;
#endif //#ifdef _INSERT_INTO_HASH_TREE
  int nStringLength = strCurrentWordData.length() ;
  for( nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar < nStringLength ;
    nIndexOfCurrentChar++ )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == STRING_DELIMITER )
    {
#ifdef _INSERT_INTO_HASH_TREE
      //if(bEnglishWord)
      //{
        //pletternode = g_lettertree.insert(//strVocabularyEntry

        //pvocabularyandtranslationReturn->m_byType = ENGLISH_NOUN ;
//              //If not assigned yet within THIS function.
//              //If nothing was added to "pletternode->m_psetvocabularyandtranslation".
//              if( pvocabularyandtranslationReturn )
//              {
//                //pvocabularyandtranslationReturn->m_arpletternodeBeginOfWord = new
//                //  LetterNode * [2] ;
//                //pvocabularyandtranslationReturn->m_word
//                pvocabularyandtranslation = pvocabularyandtranslationReturn ;
//                //For assigning VocabularyAndTranslation part of LetterNode
//                //when THIS function is called with a German vocabulary.
//                g_pvocabularyandtranslation = pvocabularyandtranslation ;
//                //;
////                pvocabularyandtranslationReturn->m_byType = WORD_TYPE_NOUN ;
//              }
//              else
//              {
//                if( pletternode &&
//                  //pvocabularyandtranslation was NULL sometimes.
//                  pvocabularyandtranslation )//If the pointer is assigned yet.
//                  pletternode->insert(pvocabularyandtranslation) ;

//              }
      int nLength = nIndexOfCurrentChar - nIndexOf1stChar ;
          #ifdef _DEBUG
          std::string stdstrCurrrentWord = strCurrentWordData.substr(
            nIndexOf1stChar , nIndexOfCurrentChar - nIndexOf1stChar ) ;
          if( stdstrCurrrentWord == "love" )
            nLength = nLength ;
          #endif
        g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
          stdsetpletternodeLastStringChar
          //, LetterNode * pletternodeCurrent
          //, VocabularyAndTranslation * pvocabularyandtranslation
          , bInsertNewVocabularyAndTranslation
          , byVocabularyType
          , strCurrentWordData
          , //nIndexOfCurrentChar - nIndexOf1stChar
            nLength
          , nIndexOf1stChar
          ) ;

        if( delemiterCount < NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN )
        {
          //Is NULL if e.g. an English noun that only has a plural
          //and thus the singular string is empty.
          if( g_lettertree.s_pvocabularyandtranslation )
#ifdef COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
          {
#endif
            g_lettertree.s_pvocabularyandtranslation->m_arstrEnglishWord[
              delemiterCount] = //str.Mid(start,i-start);
              strCurrentWordData.substr( nIndexOf1stChar
              , nIndexOfCurrentChar - nIndexOf1stChar );
#ifdef _DEBUG
            std::cout << "eng noun read:" <<
                g_lettertree.s_pvocabularyandtranslation->
                m_arstrEnglishWord[delemiterCount] << "\n" ;
#endif
#ifdef COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
            //Reason: in the lettertrie the reference to ...
            // is for every string of a
            // word
            // e.g. in lettertrie: "car"
            //   the last letternode has a reference to
            //   a list where all words that contain "car" are stored.
            //   If we parse the grammar of "the car drives", we find
            //   "car" within the lettertree.
            //  But at least when we want to translate we need to know
            //  if "car" is singular or plural.
            //  So if we store the letternode within the grammar parse
            //  tree we can see if it was a singular or plural in the
            //  cases where both differ (=mostly).
            //  Therefore we store the pointer to the letternode in an
            //  array index that is the same index as in the words array:
            //  example:
            //  letternode of ('r') from "car":
            //   +--vocabularyandtranslation list
            //     +--vocabularyandtranslation "the car"
            //        "car" (singular, index 0)
            //        "cars" (plural, index 1)
            //       +--letternode pointer array
            //         +--pointer to last letternode of "car" (index 0)
            //         +--pointer to last letternode of "cars" (index 1)
            //
            // So by comparing the pointer addresses stored for the tokens
            //one can see if it was singular or plural.
            g_lettertree.s_pvocabularyandtranslation->
              m_arpletternodeLastEngChar[
              delemiterCount] = pletternode ;
          }
#endif
        }
      //}//if(bEnglishWord)
      //else
      //{
      //}
#endif //#ifdef _INSERT_INTO_HASH_TREE
      if(delemiterCount == 0) //singular noun
      {
        g_lettertree.InsertSingularNounReferringNounAttributes(
          stdsetpletternodeLastStringChar ) ;
      }
      if(delemiterCount == 1 )
      {
//						  en->m_strPlural = //str.Mid(start,i-start);
//                str.substr(start,i-start);
        g_lettertree.InsertPluralNounReferringNounAttributes(
          stdsetpletternodeLastStringChar ) ;
      }
      nIndexOf1stChar = nIndexOfCurrentChar + 1 ;
      ++ delemiterCount ;
      if( delemiterCount == 2 )
        break ;
    }
  }//for-loop
  if( delemiterCount == 2 &&
    ( nIndexOfCurrentChar + NUMBER_OF_CHARS_FOR_ENG_NOUN_ATTS ) <
    nStringLength )
  {
    //else
    //{
      //if(delemiterCount==2 )
      {
        bool bCorrect = false ;
        //Wert gibt an: Wann muss ein Artikel da sein?
        switch( strCurrentWordData[ ++nIndexOfCurrentChar ] )
        {
          case '1':
          case '2':
          case '3':
            en->m_bType = strCurrentWordData[ nIndexOfCurrentChar ] -
              ASCII_CODE_FOR_DIGIT_1 ;
            bCorrect = true ;
            break;
        }
        if(bCorrect)
        {
          bCorrect = false ;
          switch( strCurrentWordData[ ++nIndexOfCurrentChar ] )
          {
            case '1':
            case '2':
            case '3':
              en->m_bTranslationType =
                strCurrentWordData[ nIndexOfCurrentChar ] -
                ASCII_CODE_FOR_DIGIT_1 ;
#ifdef _INSERT_INTO_HASH_TREE
              //May be NULL if (the first) string (= singular) is
              //empty.
              if(g_lettertree.s_pvocabularyandtranslation )
                g_lettertree.s_pvocabularyandtranslation->SetNounTranslationType(
                  en->m_bTranslationType) ;
#endif //#ifdef _INSERT_INTO_HASH_TREE
              bCorrect = true ;
              break;
          }
          //if(str[i]>48)
     //     {
           // en->m_bTranslationType = str[i] - ASCII_CODE_FOR_DIGIT_1;
     //     }
        //}
          if(bCorrect)
          {
            //wenn ein Substantiv z�hlbar ist, steht in der Vokabeldatei eine
            //"1", wenn nicht, steht da eine "2"
            switch( strCurrentWordData[ ++nIndexOfCurrentChar ] )
            {
              case '1':
              case '2':
                //in der Datenstruktur im Arbeitsspeicher soll aber eine 1 stehen,
                //wenn ein Substantiv z�hlbar ist, deswegen invertieren mit "!"
                en->m_bCountable =
                  ! (strCurrentWordData[nIndexOfCurrentChar] -
                    ASCII_CODE_FOR_DIGIT_1 );
//                return en;
                break;
            }
          }//if(bCorrect)
        }//if(bCorrect)
      }
    //}//char != '9'
  }
  //ASSERT(g_lettertree.m_pletternodeRoot->m_psetpvocabularyandtranslation==NULL) ;
}

void OneLinePerWordPair::InsertEnglishMainVerb(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  , std::set<LetterNode *> stdsetpletternodeLastStringChar
  )
{
  bool bInsertNewVocabularyAndTranslation = true ;
  TRACE("englisches Verb\n");
  EnglishVerb * ev = new EnglishVerb;
  BYTE delemiterCount = 0 ;
  BYTE byNumberOfObjectsAllowed = 0 ;
  BYTE byAllowsProgressiveForm = 0 ;
  int nIndexOf1stWordChar = 1 ;
  std::string stdstrInfinitive ;
#ifdef _INSERT_INTO_HASH_TREE
//        LetterNode * pletternodeLastForInsertedWord = NULL ;
  //VocabularyAndTranslation * pvocabularyandtranslation = NULL ;
  //VocabularyAndTranslation * pvocabularyandtranslationReturn = NULL ;
#endif //#ifdef _INSERT_INTO_HASH_TREE
  for( int nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar <
    strCurrentWordData.length() ; nIndexOfCurrentChar++ )
  {
    if( nIndexOfCurrentChar == 1 &&
        strCurrentWordData[nIndexOfCurrentChar] != '9' )
    {
      nIndexOf1stWordChar = 2 ;
      byNumberOfObjectsAllowed = strCurrentWordData[nIndexOfCurrentChar] - '1' ;
    }
    if( nIndexOfCurrentChar == 2 &&
        strCurrentWordData[nIndexOfCurrentChar] != '9' )
    {
      nIndexOf1stWordChar = 3 ;
      byAllowsProgressiveForm = strCurrentWordData[nIndexOfCurrentChar] - '1' ;
    }
    if( strCurrentWordData[nIndexOfCurrentChar] == STRING_DELIMITER)
    {
      switch( delemiterCount )
      {
        case STRING_INDEX_FOR_INFINITIVE :
        {
          #ifdef _DEBUG
          std::string stdstrCurrrentWord = strCurrentWordData.substr(
            nIndexOf1stWordChar , nIndexOfCurrentChar - nIndexOf1stWordChar ) ;
          #endif
          stdstrInfinitive = strCurrentWordData.substr(
            nIndexOf1stWordChar , nIndexOfCurrentChar - nIndexOf1stWordChar ) ;
          switch( byNumberOfObjectsAllowed )
          {
          case 0 :
          g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
            stdsetpletternodeLastStringChar
            //, LetterNode * pletternodeCurrent
            //, VocabularyAndTranslation * pvocabularyandtranslation
            , bInsertNewVocabularyAndTranslation
//            , byVocabularyType
            , EnglishVerb::main_verb_allows_0object_infinitive
            , strCurrentWordData
            , nIndexOfCurrentChar - nIndexOf1stWordChar
            , nIndexOf1stWordChar
            ) ;
          case 1:
            g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
              stdsetpletternodeLastStringChar
              //, LetterNode * pletternodeCurrent
              //, VocabularyAndTranslation * pvocabularyandtranslation
              , bInsertNewVocabularyAndTranslation
              , EnglishWord::main_verb_allows_1object_infinitive
              , strCurrentWordData
              , nIndexOfCurrentChar - nIndexOf1stWordChar
              , nIndexOf1stWordChar
              ) ;
          case 2:
            g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
              stdsetpletternodeLastStringChar
              //, LetterNode * pletternodeCurrent
              //, VocabularyAndTranslation * pvocabularyandtranslation
              , bInsertNewVocabularyAndTranslation
              , EnglishWord::main_verb_allows_2objects_infinitive
              , strCurrentWordData
              , nIndexOfCurrentChar - nIndexOf1stWordChar
              , nIndexOf1stWordChar
              ) ;
          }
  //        ev->m_strInfinitive = //str.Mid(start,i-start);
  //          strCurrentWordData.substr(
  //            nIndexOf1stChar
  //            , nIndexOfCurrentChar - nIndexOf1stChar );
  #ifdef _INSERT_INTO_HASH_TREE
          if( byAllowsProgressiveForm )
            g_lettertree.InsertProgressiveReferringVerbAttributes(
              //This set is to ensure that if strings for the SAME vocabulary
              // not 2 or more VocAndTransl object should be inserted.
              stdsetpletternodeLastStringChar
              , stdstrInfinitive
              , byNumberOfObjectsAllowed
              ) ;
          g_lettertree.Insert3rdPersonSingularPresentReferringNounAttributes(
               //This set is to ensure that if strings for the SAME vocabulary
               // not 2 or more VocAndTransl object should be inserted.
               stdsetpletternodeLastStringChar
               , stdstrInfinitive
               , byNumberOfObjectsAllowed
               ) ;

  #endif //#ifdef _INSERT_INTO_HASH_TREE
        }
        break ;
        case string_index_for_past_tense :
        ev->m_strPastTense = //str.Mid(start,i-start);
          strCurrentWordData.substr( nIndexOf1stWordChar
          , nIndexOfCurrentChar - nIndexOf1stWordChar );
        break ;
      //if(delemiterCount==2)
        case string_index_for_past_participle :
        ev->m_strPastParticiple = //str.Mid(start,i-start);
          strCurrentWordData.substr(nIndexOf1stWordChar
          , nIndexOfCurrentChar - nIndexOf1stWordChar );
        break ;
//      if(delemiterCount == 3)
//        ev->m_strPreposition = //str.Mid(start,i-start);
//          strCurrentWordData.substr( nIndexOf1stChar
//          , nIndexOfCurrentChar - nIndexOf1stChar );
#ifdef _INSERT_INTO_HASH_TREE
#ifdef _DEBUG
        if( ev->m_strInfinitive == _T("talk") )
          //Senseless line just for ability to set breakpoints.
          ev->m_strInfinitive += "" ;
#endif
      } //switch
        if(delemiterCount < NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB )
        {
          if( g_lettertree.s_pvocabularyandtranslation )
            g_lettertree.s_pvocabularyandtranslation->m_arstrEnglishWord[
              delemiterCount] = //str.Mid(start,i-start);
              strCurrentWordData.substr( nIndexOf1stWordChar
                , nIndexOfCurrentChar - nIndexOf1stWordChar ) ;
        }
#endif //#ifdef _INSERT_INTO_HASH_TREE
      delemiterCount++;
      nIndexOf1stWordChar = nIndexOfCurrentChar + 1 ;
    } //if(strCurrentWordData[nIndexOfCurrentChar] == STRING_DELIMITER)
    if( delemiterCount == 5
      &&
      ( strCurrentWordData[nIndexOfCurrentChar] == '1' ||
      strCurrentWordData[nIndexOfCurrentChar] == '2' ||
      strCurrentWordData[nIndexOfCurrentChar] == '3' ||
      strCurrentWordData[nIndexOfCurrentChar] == '4'
      )
      )
    {
      ev->m_bAllowsIngForm = 0 ;
      ev->m_bAllowsToPlusInfinitive = 0 ;
      if( strCurrentWordData[nIndexOfCurrentChar] == '1' ||
        strCurrentWordData[nIndexOfCurrentChar] == '2' )
        ev->m_bAllowsToPlusInfinitive = 1 ;
      if( strCurrentWordData[nIndexOfCurrentChar] == '1' ||
        strCurrentWordData[nIndexOfCurrentChar] == '3' )
      {
        ev->m_bAllowsIngForm=1;
//        g_lettertree.InsertProgressiveReferringVerbAttributes(
//          //This set is to ensure that if strings for the SAME vocabulary
//          // not 2 or more VocAndTransl object should be inserted.
//          stdsetpletternodeLastStringChar
//          ) ;
      }
#ifdef _INSERT_INTO_HASH_TREE
      g_lettertree.s_pvocabularyandtranslation->m_arbyAttribute[0] = 0 ;
#endif
//            return ev;
      return ;
    }
    if( delemiterCount == 4 &&
      ( strCurrentWordData[nIndexOfCurrentChar] == '1' ||
        strCurrentWordData[nIndexOfCurrentChar] == '2' )
      )
      ++ delemiterCount;
  }
}

void OneLinePerWordPair::InsertEnglishAdjective(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  TRACE("englisches Adj");
  EnglishAdjective * ea=new EnglishAdjective;
  if(ea)
  {
    BYTE delemiterCount=0;
    BYTE bMoreMost=2;
    int start=3;
    if(strCurrentWordData[1]=='1' || strCurrentWordData[1]=='2')
    {
      ea->m_bMoreMost=strCurrentWordData[1]-ASCII_CODE_FOR_DIGIT_1;
    }
    if(strCurrentWordData[2]=='1' || strCurrentWordData[2]=='2' || strCurrentWordData[2]=='3')
    {
      ea->m_bAllowedPlace=strCurrentWordData[2]-ASCII_CODE_FOR_DIGIT_1;
    }
    for(int i=3; i < strCurrentWordData.length(); i++)
    {
      if(strCurrentWordData[i]=='9')
      {
        if(delemiterCount==0)
        {
          ea->m_strPositiv = //str.Mid(start,i-start);
            strCurrentWordData.substr(start,i-start);
        }
        if(delemiterCount==1)
        {
          if(ea->m_bMoreMost==1)
          {
            ea->m_strAdverb = //str.Mid(start,i-start);
              strCurrentWordData.substr(start,i-start);
//                  return ea;
            return ;
          }
          ea->m_strComperativ = //str.Mid(start,i-start);
            strCurrentWordData.substr(start,i-start);
        }
        if( delemiterCount == 2 )
          ea->m_strSuperlativ = //str.Mid(start,i-start);
            strCurrentWordData.substr(start,i-start);
        if( delemiterCount == 3 )
        {
          ea->m_strAdverb = //str.Mid(start,i-start);
            strCurrentWordData.substr(start,i-start);
        }
        delemiterCount ++;
        start = i + 1 ;
      }
    }
    if(delemiterCount == 4 )
    {
      //return TRUE;
      //voc=dynamic_cast<Vocable*>(ea);
      //return 0;
//            return ea;
      return ;
    }
  }
}

void OneLinePerWordPair::InsertEnglishAdverb(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  EnglishAdverb * ea = new EnglishAdverb;
  BYTE delemiterCount=0;
  for(int i = 1 ; i < strCurrentWordData.length(); ++i )
  {
    if( strCurrentWordData[i] == '9' )
    {
      if(delemiterCount == 0 )
        ea->m_strWord = //str.Mid(1,i-1);
          strCurrentWordData.substr(1,i-1);
      ++delemiterCount;
    }
    if(strCurrentWordData[i]=='1' || strCurrentWordData[i]=='2' ||
        strCurrentWordData[i]=='3' || strCurrentWordData[i]=='4' ||
      strCurrentWordData[i]=='5' || strCurrentWordData[i]=='6')
      if(delemiterCount == 1)
      {
        //return TRUE;
        //voc=dynamic_cast<Vocable*>(ea);
        //return 0;
        ea->m_bType = strCurrentWordData[i] - ASCII_CODE_FOR_DIGIT_1;
//              return ea;
        return ;
      }
  }
}

void OneLinePerWordPair::InsertEnglishAuxiliaryVerb(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  TRACE("englisches Verb\n");;
  EnglishAuxiliaryVerb * eav=new EnglishAuxiliaryVerb;
  BYTE delemiterCount=0;
  BYTE otherCount=0;
  int start=1;
  for(int i=1;i < strCurrentWordData.length();i++)
  {
    if(strCurrentWordData[i]=='9') // finite (gebeugte) Verbformen
    {
      if(delemiterCount<14)
        eav->m_strWords[delemiterCount] = //str.Mid(start,i-start);
          strCurrentWordData.substr(start,i-start);
//            if(delemiterCount==13)
//              return eav;
      return ;
      delemiterCount++;
      start=i+1;
    }
  }
}

void OneLinePerWordPair::InsertEnglishConjunction(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  bool bInsertNewVocabularyAndTranslation = true ;
  int nIndexOf1stChar = 1 ;
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
//  EnglishConjunction * ec=new EnglishConjunction;
  for( int nIndexOfCurrentChar = 1; nIndexOfCurrentChar <
    strCurrentWordData.length() ; nIndexOfCurrentChar ++ )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == '9' ) // finite (gebeugte) Verbformen
    {
//      ec->m_strWord = //str.Mid(1,i-1);
//        strCurrentWordData.substr(1,i-1);
//            if( i + 1 == strCurrentWordData.length() )
//              return ec;
      g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
          stdsetpletternodeLastStringChar
          //, LetterNode * pletternodeCurrent
          //, VocabularyAndTranslation * pvocabularyandtranslation
          , //true
            bInsertNewVocabularyAndTranslation
          , byVocabularyType
          , strCurrentWordData
          , nIndexOfCurrentChar - 1 //nIndexOf1stChar
          , //1
          nIndexOf1stChar
          ) ;
        if( g_lettertree.s_pvocabularyandtranslation )
          g_lettertree.s_pvocabularyandtranslation->m_arstrEnglishWord[
            //delemiterCount
          0 ] = //str.Mid(start,i-start);
            strCurrentWordData.substr( 1 //nIndexOf1stChar
              , nIndexOfCurrentChar - 1 //nIndexOf1stChar
              ) ;
      return ;
    }
  }
}

void OneLinePerWordPair::InsertEnglishPreposition(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  EnglishPreposition * ep = new EnglishPreposition;
  for(int i=1 ; i < strCurrentWordData.length(); ++i)
  {
    if( strCurrentWordData[i] == STRING_DELIMITER )
    {
      ep->m_strWord = //str.Mid(1,i-1);
        strCurrentWordData.substr(1,i-1);
//            if( i + 1 == strCurrentWordData.length() )
//              return ep;
//            else
//              return NULL;
      return ;
    }
  }
}

void OneLinePerWordPair::InsertEnglishPonoun(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  EnglishPronoun * ep = new EnglishPronoun;
  BYTE delemiterCount=0;
  DWORD dwStart=0;
  for(int i=1; i < strCurrentWordData.length(); i++)
  {
    if(strCurrentWordData[i]=='9')
    {
      if(delemiterCount==0)
      {
        ep->m_strSingular = //str.Mid(1,i-1);
          strCurrentWordData.substr(1,i-1);
      }
      if(delemiterCount==1)
      {
        ep->m_strPlural = //str.Mid(dwStart,i-dwStart);
          strCurrentWordData.substr(dwStart,i-dwStart);
//              if( i + 1 == strCurrentWordData.length() )
//                return ep;
        return ;
      }
      delemiterCount++;
      dwStart=i+1;
    }
  }
}

void OneLinePerWordPair::InsertGermanAdjective(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  TRACE("englisches Adj\n");;
  GermanAdjective * ga=new GermanAdjective;
  BYTE delemiterCount=0;
  int start=1;
  for(int i=1;i < strCurrentWordData.length();++i)
  {
    if(strCurrentWordData[i]=='9')
    {
      if(delemiterCount == 0 )
        ga->m_strPositiv = //str.Mid(start,i-start);
          strCurrentWordData.substr(start,i-start);
      if(delemiterCount == 1 )
        ga->m_strComperativ = //str.Mid(start,i-start);
          strCurrentWordData.substr(start,i-start);
      if(delemiterCount == 2 )
        ga->m_strSuperlativ = //str.Mid(start,i-start);
          strCurrentWordData.substr(start,i-start);
      if(delemiterCount == 3 )
        ga->m_strWortstamm = //str.Mid(start,i-start);
          strCurrentWordData.substr(start,i-start);
      delemiterCount++;
      start = i+1;
    }
  }
  if(delemiterCount==4)
  {
    //return TRUE;
    //voc=dynamic_cast<Vocable*>(ga);
    //return 0;
//            return ga;
    return ;
  }
}

void OneLinePerWordPair::InsertGermanAdverb(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  GermanAdverb * ga=new GermanAdverb;
  BYTE delemiterCount=0;
  for(int i=1; i < strCurrentWordData.length(); ++i)
  {
    if(strCurrentWordData[i]=='9')
    {
      if(delemiterCount==0)
        ga->m_strWord = //str.Mid(1,i-1);
          strCurrentWordData.substr(1,i-1);
      ++delemiterCount;
    }
  }
  if(delemiterCount==1)
  {
//            return ga;
    return ;
  }
}

void OneLinePerWordPair::InsertGermanAuxiliaryVerb(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  GermanVerb * gv=new GermanVerb;
  gv->m_bAuxiliaryVerb=TRUE;
  BYTE delemiterCount=0;
  BYTE otherCount=0;
  int start=1;
  for(int i=1;i < strCurrentWordData.length(); ++i)
  {
    if(strCurrentWordData[i] == '9') // finite (gebeugte) Verbformen
    {
      if(delemiterCount<16)
        gv->m_strWords[delemiterCount] = //str.Mid(start,i-start);
          strCurrentWordData.substr(start,i-start);
      delemiterCount++;
      start=i+1;
    }
    if(strCurrentWordData[i]=='1' || strCurrentWordData[i]=='2')
    {
      if(otherCount==0)
      {
        gv->m_bBe=strCurrentWordData[i];
//                if(delemiterCount==16)
//                  return gv;
        return ;
      }
      otherCount++;
    }
  }
}

void OneLinePerWordPair::InsertGermanConjunction(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  bool bInsertNewVocabularyAndTranslation = true ;
//  GermanConjunction * gc = new GermanConjunction;
  BYTE bDelemiterOccured=FALSE;
  BYTE otherCount=0;
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
  for(int nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar <
    strCurrentWordData.length(); ++ nIndexOfCurrentChar )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == '9' ) // finite (gebeugte) Verbformen
    {
//      gc->m_strWord = //str.Mid(1,i-1);
//        strCurrentWordData.substr(1,nIndexOfCurrentChar-1);
      bDelemiterOccured = TRUE ;
      g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
        stdsetpletternodeLastStringChar
        //, LetterNode * pletternodeCurrent
        //, VocabularyAndTranslation * pvocabularyandtranslation
        , //true
          bInsertNewVocabularyAndTranslation
        , byVocabularyType
        , strCurrentWordData
        , nIndexOfCurrentChar - 1 //nIndexOf1stChar
        , 1
        //nIndexOf1stChar
        ) ;
        if( g_lettertree.s_pvocabularyandtranslation )
          g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord[
            //delemiterCount
          0 ] = //str.Mid(start,i-start);
            strCurrentWordData.substr( 1 //nIndexOf1stChar
              , nIndexOfCurrentChar - 1 //nIndexOf1stChar
              ) ;
    }
    if( strCurrentWordData[nIndexOfCurrentChar] == '1' ||
        strCurrentWordData[nIndexOfCurrentChar] == '2' )
    {
//      gc->m_bWordOrder = strCurrentWordData[nIndexOfCurrentChar] - 49 ;
//              if( bDelemiterOccured && i+1 == strCurrentWordData.length() )
//                return gc;
      return ;
    }
  }
}

void OneLinePerWordPair::InsertGermanMainVerb(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  TRACE("deutsches Verb\n");;
  GermanVerb * gv = new GermanVerb;
  gv->m_bAuxiliaryVerb = FALSE;
  BYTE byDelemiterCount = 0 ;
  BYTE otherCount = 0 ;
  int nIndexOf1stChar = 1 ;
  for( int nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar <
    strCurrentWordData.length() ; nIndexOfCurrentChar ++ )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == STRING_DELIMITER ) // finite (gebeugte) Verbformen,
      //Praeposition
    {
      if( byDelemiterCount < 16 )
      {
        gv->m_strWords[byDelemiterCount] = //str.Mid(start,i-start);
          strCurrentWordData.substr( nIndexOf1stChar
          , nIndexOfCurrentChar - nIndexOf1stChar );
        //Add to the stucture that also contains the English strings.
        //So the German equivalent can be retrieved when the last
        //character (LetterNode) of the English string inside the Trie
        //is got.
        g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord[
          byDelemiterCount ] = //str.Mid(start,i-start);
          strCurrentWordData.substr( nIndexOf1stChar ,
          nIndexOfCurrentChar - nIndexOf1stChar );
      }
      if( byDelemiterCount == 16 )
        gv->m_strPreposition = //str.Mid(start,i-start);
          strCurrentWordData.substr( nIndexOf1stChar
          , nIndexOfCurrentChar - nIndexOf1stChar );
        byDelemiterCount ++ ;
      nIndexOf1stChar = nIndexOfCurrentChar + 1 ;
    }
    // in welchem/welchen Fall/F�llen steht/stehen das/die Objekt(e)
    if( strCurrentWordData[nIndexOfCurrentChar] == '1' ||
      strCurrentWordData[nIndexOfCurrentChar] == '2' ||
      strCurrentWordData[nIndexOfCurrentChar] == '3' ||
      strCurrentWordData[nIndexOfCurrentChar] == '4'
      )
    {
      if( otherCount == 0 )
        gv->m_bMove = strCurrentWordData[nIndexOfCurrentChar] - 49 ;
      if( otherCount == 1 )
        gv->m_bBe = strCurrentWordData[nIndexOfCurrentChar] - 49 ;
      if( otherCount == 2 )
      {
        if( strCurrentWordData[nIndexOfCurrentChar] == '1' ) // Verb verlangt kein Objekt
          gv->m_bCase = 0 ;
        if(strCurrentWordData[nIndexOfCurrentChar] == '2' ) // Verb verlangt Objekt im 3. Fall
          gv->m_bCase = 1 ;
        if(strCurrentWordData[nIndexOfCurrentChar]=='3') // Verb verlangt Objekt im 4. Fall
        {
          gv->m_bCase = 2 ;
          g_lettertree.s_pvocabularyandtranslation->m_arbyAttribute[
              VocabularyAndTranslation::array_index_for_case ] =
              VocabularyAndTranslation::dative ;
        }
        if(strCurrentWordData[nIndexOfCurrentChar]=='4') // Verb verlangt 2 Objekte im 3. und 4. Fall
          gv->m_bCase = 3 ;
      }
      if( otherCount == 3 )
      {
        if(strCurrentWordData[nIndexOfCurrentChar]=='1')
          gv->m_bReflexive=FALSE;
        if(strCurrentWordData[nIndexOfCurrentChar]=='2')
          gv->m_bReflexive=TRUE;
      }
      if( byDelemiterCount == 17 && otherCount==3)
      {
//                return gv;
        return ;
      }
      otherCount++;
    }
  }
}

void OneLinePerWordPair::InsertGermanNoun(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  //TRACE("englisches Subst\n");;
  TRACE("deutsches Substantiv: %s\n", strCurrentWordData);
  GermanNoun * gn = new GermanNoun;
  BYTE delemiterCount=0;
  int nIndexOf1stChar = 1 ;
  LetterNode * pletternode = NULL ;
  for( int nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar <
    strCurrentWordData.length() ; nIndexOfCurrentChar ++ )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == '9' )
    {
      if( delemiterCount == 0 )
        gn->m_strSingular = //str.Mid(start,i-start);
          strCurrentWordData.substr( nIndexOf1stChar ,
          nIndexOfCurrentChar - nIndexOf1stChar );
      if( delemiterCount == 1 )
        gn->m_strPlural = //str.Mid(start,i-start);
          strCurrentWordData.substr( nIndexOf1stChar ,
          nIndexOfCurrentChar - nIndexOf1stChar );
#ifdef _INSERT_INTO_HASH_TREE
      //pvocabularyandtranslationReturn =
//              g_lettertree.insert(
//                //std::string(
//                (LPCSTR)strCurrentWordData.c_str() //)
//                ,start,i-start,
//                ////If not assigned yet within THIS function.
//                //!pvocabularyandtranslation,
//                false, //"insert new vocabularyandtranslation?" == false.
//                pletternode,
//                byVocabularyType //+ NUMBER_OF_WORD_TYPES
//                ) ;
//              //g_pvocabularyandtranslation->m_byType = GERMAN_NOUN ;
//              if(pletternode)//If the pointer is assigned yet.
//                pletternode->insert(g_pvocabularyandtranslation) ;
//              InsertIntoTrieAndHandleVocabularyAndTranslation(
//                stdsetpletternodeLastStringChar
//                //, LetterNode * pletternodeCurrent
//                //, VocabularyAndTranslation * pvocabularyandtranslation
//                , bInsertNewVocabularyAndTranslation
//                , byVocabularyType
//                , strCurrentWordData
//                , nIndexOfCurrentChar - nIndexOf1stChar
//                , nIndexOf1stChar
//                ) ;
      if( delemiterCount < NUMBER_OF_STRINGS_FOR_GERMAN_NOUN )
        //pvocabularyandtranslation->m_arstrGermanWord[

        //Add to the stucture that also contains the English strings.
        //So the German equivalent can be retrieved when the last
        //character (LetterNode) of the English string inside the Trie
        //is got.
        g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord[
          delemiterCount ] = //str.Mid(start,i-start);
          strCurrentWordData.substr( nIndexOf1stChar
          , nIndexOfCurrentChar - nIndexOf1stChar );
#ifdef _DEBUG
        std::cout << "German noun read:" <<
            g_lettertree.s_pvocabularyandtranslation->
            m_arstrGermanWord[delemiterCount] << "\n" ;
#endif
#endif //#ifdef _INSERT_INTO_HASH_TREE
      delemiterCount ++ ;
      nIndexOf1stChar = nIndexOfCurrentChar + 1 ;
    }
    if( strCurrentWordData[nIndexOfCurrentChar] == '1' ||
      strCurrentWordData[nIndexOfCurrentChar] == '2' ||
      strCurrentWordData[nIndexOfCurrentChar] == '3'
      )
      if( delemiterCount == 2 )
      {
        gn->m_bArticle = strCurrentWordData[nIndexOfCurrentChar];
        //return TRUE;
        //voc=dynamic_cast<Vocable*>(gn);
        //return 0;
//                return gn;
        g_lettertree.s_pvocabularyandtranslation->m_arbyAttribute[
          0 ] = ( strCurrentWordData[nIndexOfCurrentChar] - '1' ) ;
        return ;
      }
  }
}

void OneLinePerWordPair::InsertGermanPreposition(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  GermanPreposition * gp = new GermanPreposition;
  BYTE delemiterCount=0;
  int i = 1 ;
  for(;i < strCurrentWordData.length();i++)
  {
    if(strCurrentWordData[i]==STRING_DELIMITER)
    {
      gp->m_strWord = //str.Mid(1,i-1);
        strCurrentWordData.substr(1,i-1);
      //if(i+1==str.GetLength())
       // return gp;
      //else
       // return NULL;
      return ;
      if(delemiterCount == 0 )
        ++ delemiterCount ;
      break ;
    }
  }
  if( delemiterCount == 1 )
  {
    if( ++i < strCurrentWordData.length() )
      gp->m_byCase = strCurrentWordData[i] - ASCII_CODE_FOR_DIGIT_0 ;
//            return gp;
    return ;
  }
}

void OneLinePerWordPair::InsertGermanPronoun(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  GermanPronoun * gp = new GermanPronoun;
  for(int i=1;i < strCurrentWordData.length();++i)
  {
    if(strCurrentWordData[i]=='9')
    {
      gp->m_strWord = //str.Mid(1,i-1);
        strCurrentWordData.substr(1,i-1);
//              if( i + 1 == strCurrentWordData.length() )
//                return gp;
//              else
//                return NULL;
      return ;
    }
  }
}

//str: a string containing the specific words file format entry.
//Inserts into LetterTree.
//Word *
void OneLinePerWordPair::extract(
  const VTrans::string_type & strCurrentWordData
  , BYTE bEnglishWord
  , int & ret )
{
	LOGN("extract--new word from file") ;
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
  if( strCurrentWordData.length() > 0 )
  {
    bool bInsertNewVocabularyAndTranslation = true ;
#ifdef _DEBUG
	//printf("Vocable * extract(CString s,BOOL english,int & ret) ANFANG\n");
#endif
	  //TRACE("Vocable * extract(CString s,BOOL english,int & ret) ANFANG\n");
	  if(bEnglishWord)
	  {
      //The VocabularyAndTranslation object contains all grammatical data like
      //plural, article and so on for English and German. It should only be
      //created ONCE for a English/German word pair.
      //The other last LetterNodes should point to its address.
      bInsertNewVocabularyAndTranslation = true ;
      BYTE byVocabularyType = strCurrentWordData[ WORD_TYPE_CHAR_INDEX ] ;

//      byVocabularyType = WordFileWordClassValueToZeroBasedIndex(
//        byVocabularyType) ;

      //Make empty, else VocabularyAndTranslation kann not be appended to the
      //last LetterNode if e.g. noun "love" followed by verb "love".
      stdsetpletternodeLastStringChar.clear() ;
      //switch ( str[ WORD_TYPE_CHAR_INDEX ] )
      switch( byVocabularyType )
      {
      case WORD_TYPE_NOUN : // Substantiv
        InsertEnglishNoun(strCurrentWordData,byVocabularyType) ;
        return ;
        break;
      case WORD_TYPE_MAIN_VERB : // englisches Vollverb
        InsertEnglishMainVerb(strCurrentWordData,byVocabularyType,
            stdsetpletternodeLastStringChar) ;
        return ;
        break;
      case WORD_TYPE_ADJECTIVE : // englisches Adjektiv
        InsertEnglishAdjective(strCurrentWordData,byVocabularyType) ;
        return ;
        break ;
      case WORD_TYPE_ADVERB : // Adverb
        InsertEnglishAdverb(strCurrentWordData,byVocabularyType) ;
        return ;
        break ;
      case WORD_TYPE_PREPOSITION : // Pr�position
        InsertEnglishPreposition(strCurrentWordData,byVocabularyType) ;
        return ;
        break ;
      case WORD_TYPE_PRONOUN : // Pronomen
        InsertEnglishPonoun(strCurrentWordData,byVocabularyType) ;
        return ;
        break ;
      case WORD_TYPE_AUX_VERB : // Hilfsverb
        InsertEnglishAuxiliaryVerb(strCurrentWordData,byVocabularyType) ;
        return ;
        break ;
      case WORD_TYPE_CONJUNCTION : // englische Konjunktion
        InsertEnglishConjunction(strCurrentWordData,byVocabularyType) ;
        return ;
        break ;
      }//switch ( str[ WORD_TYPE_CHAR_INDEX ] )
	  }
	  else // deutsch
	  {
      BYTE byVocabularyType = strCurrentWordData[ WORD_TYPE_CHAR_INDEX ] ;
      //switch ( str[ WORD_TYPE_CHAR_INDEX ] )
      switch ( byVocabularyType )
      {
        case WORD_TYPE_NOUN : // deutsches Substantiv
          InsertGermanNoun( strCurrentWordData, byVocabularyType) ;
          return ;
          break ;
        case WORD_TYPE_MAIN_VERB : // deutsches Verb
          InsertGermanMainVerb( strCurrentWordData, byVocabularyType) ;
          return ;
          break ;
        case WORD_TYPE_ADJECTIVE : // deutsches Adjektiv
          InsertGermanAdjective( strCurrentWordData, byVocabularyType) ;
          return ;
          break ;
        case WORD_TYPE_ADVERB : // deutsches Adverb
          InsertGermanAdverb( strCurrentWordData, byVocabularyType) ;
          return ;
          break ;
        case WORD_TYPE_PREPOSITION : // deutsche Pr�position
          InsertGermanPreposition( strCurrentWordData, byVocabularyType) ;
          return ;
          break ;
        case WORD_TYPE_PRONOUN : // deutsches Pronomen
          InsertGermanPronoun( strCurrentWordData, byVocabularyType) ;
          return ;
          break ;
        case WORD_TYPE_AUX_VERB : // deutsches Hilfsverb
          InsertGermanAuxiliaryVerb( strCurrentWordData, byVocabularyType) ;
          return ;
          break ;
        case WORD_TYPE_CONJUNCTION : // deutsche Konjunktion
          InsertGermanConjunction( strCurrentWordData, byVocabularyType) ;
          return ;
          break ;
      }//switch
	  }
  } //if( str.GetLength()>0 )
//	return NULL;
}

BYTE OneLinePerWordPair::LoadWords(//WordNode * pWordNodeCurrent
  std::string & r_strWordsFilePath )
{
  BYTE byRet = 0 ;
	FILE * p_fileWords ;
	int i;
	BOOL break_while = FALSE ;
	BOOL flag = FALSE ;
	VTrans::string_type concatenate;
  VTrans::string_type strWordFile = //_T(
    //"http://www.f4.fhtw-berlin.de/~s0518039/"
    "words.txt" //);
    ;
  WordNode * pWordNodePrevious=NULL;

	//zuerst die integralen Vokabeln der verketteten Liste hinzuf�gen, Ende
  LOGN("05.06.2008 22.22.17");
	if( ( p_fileWords = fopen( //strWordFile.c_str()
    r_strWordsFilePath.c_str() ,"rb") ) != NULL )
	{
	  byRet = 1 ;
    LOGN("05.06.2008 22.22.26");
		concatenate = "" ;
		BYTE bEnglishWord = TRUE;
		VTrans::string_type str;
		DWORD dwOffset = 0;
		DWORD dwOffsetOfBeginOfEntry = 0;//Offset (Position) des Anfanges eines
		//Vokabel-Eintrages
		while( ( i = getc(p_fileWords) ) != -1 )
		{
//			if( i == (char) '0'
//        //Decimal 10 = newline
//        || i == 10
//        )
//			{
      switch( i )
      {
        //An ASCII zero ('0') marks the end of the attributes of an English word.
        case '0':
//				if(bEnglishWord)
				{
					int ret=0;
//					pWordNodePrevious=pWordNodeCurrent;
//					pWordNodeCurrent->m_pWordNodeNext=new WordNode();
//					pWordNodeCurrent->m_pWordNodeNext->m_pWord=extract(str,TRUE,ret);
          OneLinePerWordPair::extract(str,TRUE,ret);
          #ifdef _DEBUG
          const char * pch = str.c_str() ;
          pch = 0 ;
          #endif
					//TODO also provide an error message for non-Windows binaries. 
#ifdef _WINDOWS
					if(!pWordNodeCurrent->m_pWordNodeNext->m_pWord)
					{
						char * pbCurrentDirectory=new char[10001];
						CString strMessage;
						GetCurrentDirectory(10001,pbCurrentDirectory);
						strcat(pbCurrentDirectory,"\\");
						strcat(pbCurrentDirectory,strWordFile.GetBuffer(strWordFile.
							GetLength()));
						strMessage.Format(_T("Die Datei %s \nenth�lt kein g�ltiges \
Format oder sonstiger Fehler zwischen\nOffset (=Position in Byte ab \
Dateibeginn) %u (dezimal)\nund Offset (=Position in Byte ab Dateibeginn) \
%u (dezimal) .\nDas Laden der Vokabeln wird beendet. Versuchen Sie, den \
Fehler in der Dateistruktur zu beheben."),
						pbCurrentDirectory,dwOffsetOfBeginOfEntry,dwOffset);
						AfxMessageBox(strMessage,MB_OK,0);
						delete pbCurrentDirectory;//Speicher an das Betriebssystem freigeben
						break;
					}
#endif //#ifdef _WINDOWS
//					pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
//					pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;
//					if(ret==-1)
//					{
//						break;
//					}
  				str = "" ;
          break ;
				}
//				else // deutsch
          case '\n' :
				{
					int ret=0;
//					pWordNodePrevious=pWordNodeCurrent;
//					pWordNodeCurrent->m_pWordNodeNext = new WordNode();
//					pWordNodeCurrent->m_pWordNodeNext->m_pWord=extract(str,FALSE,ret);
          OneLinePerWordPair::extract(str,FALSE,ret);
					//TODO also provide an error message for non-Windows binaries. 
#ifdef _WINDOWS
					if(!pWordNodeCurrent->m_pWordNodeNext->m_pWord)
					{
						char * pbCurrentDirectory=new char[10001];
						CString strMessage;
						GetCurrentDirectory(10001,pbCurrentDirectory);
						strMessage.Format(_T("Die Datei %s \nenth�lt kein g�ltiges \
Format oder sonstiger Fehler zwischen Offset %u und Offset %u.\n Das Laden \
der Vokabeln wird beendet. Versuchen Sie den Fehler in der Dateistruktur zu beheben"),
						pbCurrentDirectory,dwOffsetOfBeginOfEntry,dwOffset);
						AfxMessageBox(strMessage,MB_OK,0);
						delete pbCurrentDirectory;//Speicher an das Betriebssystem freigeben
						break;
					}
#endif //#ifdef _WINDOWS
//					pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
//					pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;
//					if(ret!=0)
//						//MessageBox("Datei %s enth�lt kein g�ltiges Format oder sonstiger Fehler","Fehler beim Lesen der Datei vocs.txt",MB_OK);
//						break;
  				str = "" ;
          break ;
				}
				//bEnglishWord=!bEnglishWord;
//				str = "" ;
				dwOffsetOfBeginOfEntry = dwOffset + 1 ;
//			}
//			else
//			{
        default:
          str += (char) i ;
      }
//			}
			++dwOffset;
		} //while
		fclose(p_fileWords);
//		if(pWordNodeCurrent!=NULL)
//		{
//			pWordNodeCurrent->m_pWordNodeNext=NULL;
//		}
	}
	else
	{
#ifdef _WINDOWS
		char * pbCurrentDirectory=new char[10001];
		GetCurrentDirectory(10001,pbCurrentDirectory);
		CString strMessage=_T("Die Datei ")+CString(pbCurrentDirectory)+
      strWordFile+
		_T(//"\\words.txt
      "\nkonnte nicht ge�ffnet werden.\nDiese Datei enth�lt \
das Englisch-Deutsch-W�rterbuch.\nWenn diese Datei nicht geladen wird, \
gibt es anf�nglich keine Vokabeln.\nM�gliche Ursachen:\
 \n 1. sie existiert nicht\n 2. Fehler beim �ffnen der Datei");
		AfxMessageBox(strMessage,MB_OK,0);
#endif
    std::cout << "file \"" << r_strWordsFilePath << "\" could not be opened\n" ;
	}
//	wordList.m_pWordNodeLast=pWordNodeCurrent;
//	wordList.m_pWordNodeLast->m_pWordNodeNext=NULL;
#ifdef _DEBUG
	printf("IO.cpp: void OneLinePerWordPair::LoadWords(WordNode * pWordNode) ENDE\n");
#endif
	return byRet ;
}

#ifdef _WINDOWS
LPCSTR UTF8toASCII(const char * str)
{
  LPWSTR pwstrTranslation=NULL;

  int nNumberOfRequiredWideCharacters=MultiByteToWideChar(
    CP_UTF8,         // code page
    0,         // character-type options
    //str.c_str(), // string to map
    str,
    -1,       // number of bytes in string
    NULL,  // wide-character buffer
    0        // size of buffer
    );
  LOGN("28.05.2008 21.50.08 "<<nNumberOfRequiredWideCharacters);
  if(nNumberOfRequiredWideCharacters)
  {
    pwstrTranslation=new WCHAR[nNumberOfRequiredWideCharacters];
    if(pwstrTranslation)
      MultiByteToWideChar(
        CP_UTF8,         // code page
        0,         // character-type options
        //str.c_str(), // string to map
        str,
        -1,       // number of bytes in string
        pwstrTranslation,  // wide-character buffer
        nNumberOfRequiredWideCharacters        // size of buffer
        );
  }
  //Error.
  else
    return NULL;
#ifdef _LOG
  BYTE byWideCharByte;
  for(DWORD i=0;i<wcslen(pwstrTranslation);++i)
  {
    byWideCharByte=*(pwstrTranslation+i);
    LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
    byWideCharByte=*((BYTE *)(pwstrTranslation+i)+1);
    LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
  }
#endif
  //Newline erzwingen.
  LOGN("");
  LOGN("28.05.2008 22.03.41");
  //When using an ATL string conversion macro, specify the USES_CONVERSION macro at the beginning of your function in order to avoid compiler errors.
  USES_CONVERSION;
  //LPCSTR pszA =W2A(pwstrTranslation);
  //Use an object that is created on the heao rather than a stack object. So
  //its contents aren't freed after leaving the scope where it was declared.
  CW2A * pw2a=new CW2A(pwstrTranslation);
  LPCSTR pszA=pw2a->m_psz;
  LOGN("28.05.2008 22.03.52 "<<strlen(pszA));
  LOGN(pszA);
  for(DWORD i=0;i<strlen(pszA);++i)
  {
    LOG(*(pszA+i)<<"("<<(int)*(pszA+i)<<")");
  }
  //Newline erzwingen.
  //LOGN((unsigned long)pszA<<pszA<<"");
  return pszA;
}
#endif //#ifdef _WINDOWS

//void ::TranslateAndWriteOutputFile()
//{
//}
#ifdef COMPILE_WITH_XML_OUTPUT
#ifndef __MINGW32__ //MinGW has no iconv
void writeToOutputStream(std::ostream & rofstreamTranslToGerman,
  //std::vector<Range> & vecRange,std::vector<CStringVector> & vecstrvec
  std::vector<SentenceAndValidityAndProperName> & vecsentenceandvalidityandpropername,
  xmlwriter & MyXml
  )
{
  if(rofstreamTranslToGerman)
  {
    VTrans_string_typeVector strvec;
    SentenceAndValidityAndProperName sentenceandvalidityandpropername;
    for(WORD wSentenceIndex=0;wSentenceIndex<
      vecsentenceandvalidityandpropername.size();++wSentenceIndex)
    {
      IntPair intpairConntectedWords;
      Range range;
      sentenceandvalidityandpropername=vecsentenceandvalidityandpropername.
        at(wSentenceIndex);
      rofstreamTranslToGerman<<sentenceandvalidityandpropername.
        byReturnOfSentenceParsing;
      //Only if the sentence is grammatically correct a translation exists
      //and thus it makes sense to write it and the proper names.
      if(sentenceandvalidityandpropername.byReturnOfSentenceParsing==1)
      {
        MyXml.Createtag("sentence");
        for(WORD wWordIndex=0;wWordIndex<//vecstrvec
          sentenceandvalidityandpropername.vecvecstrSentenceTokens.size();++wWordIndex)
        {
          LOGN("31.05.2008 22.30.20");
          strvec=sentenceandvalidityandpropername.vecvecstrSentenceTokens.
            at(wWordIndex);
          LOGN("31.05.2008 22.30.44");
          //rofstreamTranslToGerman<<//vecstrvec
          //  sentenceandvalidityandpropername.vecvecstrSentenceTokens.at(wWordIndex).
          //  at(0).GetBuffer()<<" ";
          LOGN("31.05.2008 22.30.55");
          //MyXml.CreateChild("word",sentenceandvalidityandpropername.
          //  vecvecstrSentenceTokens.at(wWordIndex).at(0).GetBuffer());
          MyXml.Createtag("word");
          //LPWSTR wstrTranslation;
          //int nReturnOfWideCharToMultiByte;
          LPSTR lpTranslationInUTF8=NULL;
          for(WORD wTranslationForEnglishWordIndex=0;
            wTranslationForEnglishWordIndex<strvec.size();
            ++wTranslationForEnglishWordIndex)
          {
#ifdef _WINDOWS
//            //Without this errors while building:
//             USES_CONVERSION;
////#endif
//            wstrTranslation=A2W(strvec.at(wTranslationForEnglishWordIndex).
//              GetBuffer()
//              //"�������"
//              );
//            nReturnOfWideCharToMultiByte = ::WideCharToMultiByte(CP_UTF8, 0, //lpwStr
//              wstrTranslation,
//              //Specifies the number of wide characters in the string pointed to by the lpWideCharStr parameter. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically. The length will include the null-terminator. 
//              -1,
//              lpTranslationInUTF8,//Points to the buffer to receive the translated string.
//                //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string. 
//                0,//cbMultiByte
//                NULL,//If this parameter is NULL, a system default value is used. 
//                NULL//This parameter may be NULL. The function is faster when both lpDefaultChar and lpUsedDefaultChar are NULL. 
//              );
//            ////If a German umlaut, at least 1 more byte is needed.
//            //nReturnOfWideCharToMultiByte*=200;
//            LOG("23.05.2008 11.23.17 "<<nReturnOfWideCharToMultiByte<<"\n");
//#ifdef _LOG
//            BYTE byWideCharByte;
//            for(DWORD i=0;i<wcslen(wstrTranslation);++i)
//            {
//              byWideCharByte=*(wstrTranslation+i);
//              LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
//              byWideCharByte=*((BYTE *)(wstrTranslation+i)+1);
//              LOG(byWideCharByte<<"("<<(int)byWideCharByte<<")");
//            }
//#endif
//            if(nReturnOfWideCharToMultiByte)
//            {
////#ifdef _WINDOWS
//              //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string
//              lpTranslationInUTF8=new CHAR[nReturnOfWideCharToMultiByte];
//              if(WideCharToMultiByte(
//                //When this is set, dwFlags must be zero and both lpDefaultChar and lpUsedDefaultChar must be NULL.
//                CP_UTF8,//CodePage 
//                0,//dwFlags  
//                //lpwStr
//                wstrTranslation,//Points to the wide-character string to be converted.
//                -1,//cchWideChar: Specifies the number of wide characters in the string pointed to by the lpWideCharStr parameter. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically. The length will include the null-terminator.
//                lpTranslationInUTF8,//Points to the buffer to receive the translated string. 
//                  //If the function succeeds, and cbMultiByte is zero, the return value is the required size, in bytes, for a buffer that can receive the translated string. 
//                  nReturnOfWideCharToMultiByte,//cbMultiByte
//                  NULL,//LPCSTR lpDefaultChar, If this parameter is NULL, a system default value is used. 
//                  NULL//LPBOOL lpUsedDefaultChar:This parameter may be NULL. The function is faster when both lpDefaultChar and lpUsedDefaultChar are NULL. 
//                  )
//                )
//              {
//#else
//              	CString strUTF8 = EncodeToUTF8(strvec.at(
//              		wTranslationForEnglishWordIndex)) ;
//              	lpTranslationInUTF8 = strUTF8.Buffer() ;  
//
//              if(lpTranslationInUTF8 = 
//                  ( CharSetConv::EncodeASCIIToUTF8(strvec.at(
//                wTranslationForEnglishWordIndex).GetBuffer() ) 
//                  )
//                )
//              {
#endif //#ifdef _WINDOWS
              if(lpTranslationInUTF8 = 
                ( CharSetConv::EncodeASCIIToUTF8( (char *) strvec.at(
                  //wTranslationForEnglishWordIndex).GetBuffer()
                  wTranslationForEnglishWordIndex).c_str()
                  )
                  )
                )
              {
                LOG("23.05.2008 11.24.05 "<<lpTranslationInUTF8<<"\n");
                MyXml.CreateChild("translation",//EncodeToUTF8(
                  lpTranslationInUTF8);
                delete[] lpTranslationInUTF8 ;
                LOGN("31.05.2008 22.27.11");
//#ifdef _WINDOWS
              }
#ifdef _WINDOWS
              else
                LOG("23.05.2008 11.24.24 "<<::GetLastError()<<ERROR_INSUFFICIENT_BUFFER<<" "<<ERROR_INVALID_FLAGS<<" "<<ERROR_INVALID_PARAMETER<<"\n");
            //}//if(nReturnOfWideCharToMultiByte)
#endif //#ifdef _WINDOWS
          }//loop through possible German translations for an English word.
          MyXml.CloseLasttag();//"word" tag
        }//Loop through tokens.
        rofstreamTranslToGerman<<
          //carriage Return +Newline is used to indicate the end of the sentence
          "\r\nproper name Size:"<<
          //HTONL: Convert Host(Intel) TO Network(Big Endian) byte order Long value.
          //htonl(
          (unsigned long)//vecRange
          sentenceandvalidityandpropername.vecrangeProperName.size()//)
          <<" ";

        if( sentenceandvalidityandpropername.vecrangeProperName.size() > 0 )
          MyXml.AddComment("The character indices of the proper name "
            "ranges refer to the beginning of WHOLE source text");
        //The character indices of the proper name ranges refer to the 
        //beginning of WHOLE source text (and not) to the beginning of 
        //a sentence -> put them at the end after all sentences.
	      for(DWORD dwRangeIndex=0;dwRangeIndex<//vecRange
          sentenceandvalidityandpropername.vecrangeProperName.size();dwRangeIndex++)
	      {
		      range=//vecRange
            sentenceandvalidityandpropername.vecrangeProperName.at(dwRangeIndex);
		      //for(DWORD dwCharacterIndexWithinEnglishText=range.m_dwStart;
        //    dwCharacterIndexWithinEnglishText<range.m_dwEnd;
            //dwCharacterIndexWithinEnglishText++)

          std::ostringstream ostrstream;
          //The start index ist wrongly stored into the field "m_dwEnd".
          //ostrstream<<vecsentenceandvalidityandpropername.at(0).vecrangeProperName.at(wIndex).m_dwEnd;
          //ostrstream<<range.at(wIndex).m_dwEnd;
          ostrstream<<range.m_dwStart;
          MyXml.AddAtributes("start",ostrstream.str());
          std::ostringstream ostrstream2;
          //The end index ist wrongly stored into the field "m_dwStart".
          //ostrstream2<<vecsentenceandvalidityandpropername.at(0).vecrangeProperName.at(wIndex).m_dwStart;
          //ostrstream2<<range.at(wIndex).m_dwStart;
          ostrstream2<<range.m_dwEnd;
          MyXml.AddAtributes("end",ostrstream2.str());
          MyXml.AddComment("The character indices of a range of a proper name correspond to the SOURCE text, Huey! Believe me.");
          //Man kann diese Indizes nicht einfach auch f�r jede deutsche �bersetzung nehmen:
          //I do not like Uwe. : Uwe ist 5tes Wort. Im deutschen: Ich mag Uwe nicht. 
          //Das 5te "Wort" ist hier ein PUNKT ! Soll der etwa zum Eigennamen gemacht werden?
          MyXml.Createtag("range-of-proper-name");
          //Do NOT write this closing tag, else Absturz with "I like Uwe.I like Uwe."
          //MyXml.CloseLasttag();//"range-of-proper-name" tag
		      rofstreamTranslToGerman<<
            //HTONL: Convert Host(Intel) TO Network(Big Endian) byte order Long value.
            //htonl(
            (unsigned long)range.m_dwStart//)
            <<//htonl(
            (unsigned long)range.m_dwEnd//)
            ;
	      }//Proper name range loop
        if( sentenceandvalidityandpropername.m_vecintpairConntectedWords.size() > 0 )
          MyXml.Createtag("connected-words");
        {
          for(DWORD dwRangeIndex=0;dwRangeIndex<//vecRange
            sentenceandvalidityandpropername.m_vecintpairConntectedWords.size();dwRangeIndex++)
	        {
		        intpairConntectedWords=
              sentenceandvalidityandpropername.m_vecintpairConntectedWords.at(dwRangeIndex);
            std::ostringstream ostrstream;
            ostrstream<<intpairConntectedWords.m_n1-(ID_USER+100);
            //MyXml.AddAtributes("number1",ostrstream.str());
            std::ostringstream ostrstream2;
            ostrstream2<<intpairConntectedWords.m_n2-(ID_USER+100);
            //MyXml.AddAtributes("number2",ostrstream2.str());
            MyXml.Createtag("connection");
            //MyXml.Createtag("link");
            MyXml.CreateChild("link",ostrstream.str() );
            MyXml.CreateChild("link",ostrstream2.str() );
            MyXml.CloseLasttag();//"connection" tag
          }
          MyXml.CloseLasttag();//"connected-words" tag
        }//if connected words for this sentence exist.
        MyXml.CloseLasttag();//"sentence" tag
        
      }//if(sentenceandvalidityandpropername.byReturnOfSentenceParsing==1)
      //MyXml.CloseAlltags();
      //MyXml.CloseLasttag();//"sentence" tag
    }//sentences loop
    //rofstreamTranslToGerman.close();
  }//if(rofstreamTranslToGerman)
}
#endif //#ifndef __MINGW32__ //MinGW has no iconv
#endif//#ifdef COMPILE_WITH_XML_OUTPUT

//const char * 
BYTE readInputText(const std::string & strFilePath, std::string & str)
{
    const char * pch = NULL ;
    //std::string str;
	std::ifstream ifstreamGermanText(
	  //Output filename.
	  //"english.txt"
	  strFilePath.c_str(),ios_base::in);
	//wenn erfolgreich ge�ffnet.
	if(ifstreamGermanText)
	{
	  char ch;
	  //LPWSTR pwstrTranslation=NULL;
	  while(true)
	  {
	    ch=ifstreamGermanText.get();
	    if(ifstreamGermanText.eof())
	      break;
	    str+=ch;
	  }
	  LOGN("28.05.2008 20.59.56");
	  //LPWSTR wstrTranslation((LPWSTR)str.c_str());
	  LOGN("28.05.2008 20.58.44");
	
	  //pch=pszA;
	  pch = //::UTF8toASCII(
		str.c_str() //)
		;
	  LOGN("12.06.2008 22.24.35 "<<(unsigned long)pch<<pch);
	}
	//return pch ;
	//return str ;
	return 0 ;
}

BYTE OneLinePerWordPair::WordFileWordClassValueToZeroBasedIndex(
    BYTE byWordFileWordClassValue )
{
  BYTE byRet = 255 ;
  switch( byWordFileWordClassValue )
  {
  case WORD_TYPE_NOUN:
    byRet = EnglishWord::noun ;
    break ;
  case WORD_TYPE_MAIN_VERB:
    byRet = EnglishWord::main_verb ;
    break ;
  }
  return byRet ;
}
