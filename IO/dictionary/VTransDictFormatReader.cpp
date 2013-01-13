//This file contains file format specific code.

//Ohne das folgende include: fatal error C1010: Unerwartetes Dateiende 
//waehrend der Suche nach dem vorkompilierten Header.
#include "StdAfx.h"
#include "VTransDictFormatReader.hpp"
//#include "rest.h"
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN(...)
#include <Attributes/EnglishWord.hpp> //class EnglishAuxiliaryVerb etc.
#include <Attributes/Word.hpp> //for "class Word" etc.
#include <fstream> //for std::ofstream
#include <ios>//fo ios_base
#include "Controller/charsetconv.h"
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

//extern std::ofstream ofstreamLogFile; //for LOGN(...)
//extern WordList wordList;

//LetterTree g_lettertree ;
//Static variables need also to be declared in the source file.
LetterTree * OneLinePerWordPair::s_p_lettertree ;
I_UserInterface * OneLinePerWordPair::s_p_userinterface;

//extern LetterTree & g_lettertree ;

DWORD OneLinePerWordPair::s_dwNumberOfVocabularyPairs = 0 ;

//VocabularyAndTranslation * g_pvocabularyandtranslation ;

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

BYTE OneLinePerWordPair::InsertEnglishNoun(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  BYTE byRet = 1;
  bool bInsertNewVocabularyAndTranslation = true ;
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
//  EnglishNoun * p_english_noun = new EnglishNoun;
  BYTE delemiterCount = 0 ;
//  BYTE otherCount = 0 ;
  int nIndexOf1stChar = 1;
  int nIndexOfCurrentChar ;
  //TRACE("englisches Substantiv: %s\n", str);
#ifdef _INSERT_INTO_HASH_TREE
//  LetterNode * pletternode = NULL ;
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
//        g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
        s_p_lettertree->InsertIntoTrieAndHandleVocabularyAndTranslation(
          stdsetpletternodeLastStringChar
          //, LetterNode * pletternodeCurrent
          //, VocabularyAndTranslation * pvocabularyandtranslation
          , bInsertNewVocabularyAndTranslation
          , //byVocabularyType
            EnglishWord::noun
          , strCurrentWordData
          , //nIndexOfCurrentChar - nIndexOf1stChar
            nLength
          , nIndexOf1stChar
          ) ;

        if( delemiterCount < NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN )
        {
          //Is NULL if e.g. an English noun that only has a plural
          //and thus the singular string is empty.
//          if( g_lettertree.s_pvocabularyandtranslation )
          if( s_p_lettertree->s_pvocabularyandtranslation )
#ifdef COMPILE_WITH_REFERENCE_TO_LAST_LETTER_NODE
          {
#endif
//            g_lettertree.s_pvocabularyandtranslation->m_arstrEnglishWord[
            s_p_lettertree->s_pvocabularyandtranslation->m_arstrEnglishWord[
              delemiterCount] = //str.Mid(start,i-start);
              strCurrentWordData.substr( nIndexOf1stChar
              , nIndexOfCurrentChar - nIndexOf1stChar );
#ifdef _DEBUG
            std::cout << "eng noun read:" <<
//              g_lettertree.s_pvocabularyandtranslation->
              s_p_lettertree->s_pvocabularyandtranslation->
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
//            g_lettertree.s_pvocabularyandtranslation->
            s_p_lettertree->s_pvocabularyandtranslation->
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
//        g_lettertree.InsertSingularNounReferringNounAttributes(
        s_p_lettertree->InsertSingularNounReferringNounAttributes(
          stdsetpletternodeLastStringChar ) ;
      }
      if(delemiterCount == 1 )
      {
//        p_english_noun->m_strPlural = //str.Mid(start,i-start);
//        str.substr(start,i-start);
//        g_lettertree.InsertPluralNounReferringNounAttributes(
        s_p_lettertree->InsertPluralNounReferringNounAttributes(
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
      nStringLength
    )
  {
    //else
    //{
      //if(delemiterCount==2 )
      {
        bool bCorrect = false ;
        //Wert gibt an: Wann muss ein Artikel da sein?
        switch( strCurrentWordData[ ++ nIndexOfCurrentChar ] )
        {
          case '1':
          case '2':
          case '3':
//            p_english_noun->m_bType = strCurrentWordData[
//              nIndexOfCurrentChar ] - ASCII_CODE_FOR_DIGIT_1 ;
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
//              p_english_noun->m_bTranslationType =
//                strCurrentWordData[ nIndexOfCurrentChar ] -
//                ASCII_CODE_FOR_DIGIT_1 ;
#ifdef _INSERT_INTO_HASH_TREE
              //May be NULL if (the first) string (= singular) is
              //empty.
//              if( g_lettertree.s_pvocabularyandtranslation )
              if( s_p_lettertree->s_pvocabularyandtranslation )
//                g_lettertree.s_pvocabularyandtranslation->
                s_p_lettertree->s_pvocabularyandtranslation->
                  SetNounTranslationType( //p_english_noun->m_bTranslationType
                    strCurrentWordData[ nIndexOfCurrentChar ] -
                    ASCII_CODE_FOR_DIGIT_1 ) ;
#endif //#ifdef _INSERT_INTO_HASH_TREE
              bCorrect = true ;
              byRet = 0;
              break;
          }
          //if(str[i]>48)
     //     {
           // p_english_noun->m_bTranslationType = str[i] -
          //    ASCII_CODE_FOR_DIGIT_1;
     //     }
        //}
          if(bCorrect)
          {
            //wenn ein Substantiv zählbar ist, steht in der Vokabeldatei eine
            //"1", wenn nicht, steht da eine "2"
            switch( strCurrentWordData[ ++nIndexOfCurrentChar ] )
            {
              case '1':
              case '2':
//                //In der Datenstruktur im Arbeitsspeicher soll aber eine 1 stehen,
//                //wenn ein Substantiv zählbar ist, deswegen invertieren mit "!"
//                p_english_noun->m_bCountable =
//                  ! (strCurrentWordData[nIndexOfCurrentChar] -
//                    ASCII_CODE_FOR_DIGIT_1 );
//                return p_english_noun ;
                break;
            }
          }//if(bCorrect)
        }//if(bCorrect)
      }
    //}//char != '9'
  }
  //ASSERT(g_lettertree.m_pletternodeRoot->m_psetpvocabularyandtranslation==NULL) ;
  return byRet;
}

void OneLinePerWordPair::InsertEnglishMainVerb(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  //This set keeps track of whether for the _same_ vocabulary a
  //"voc and transl" object was added to a trie node, e.g.
  // to not insert a "voc and transl" for the same vocabulary more than once.
  , std::set<LetterNode *> stdsetpletternodeLastStringChar
  )
{
  bool bInsertNewVocabularyAndTranslation = true ;
  LOGN("OneLinePerWordPair::InsertEnglishMainVerb(...)");
  EnglishVerb * p_englishverb = new EnglishVerb;
  BYTE delemiterCount = 0 ;
  BYTE byNumberOfObjectsAllowed = 0 ;
  BYTE byAllowsProgressiveForm = 0 ;
  char chCurrent ;
  int nIndexOf1stWordChar = 1 ;
  std::string stdstrInfinitive ;
#ifdef _INSERT_INTO_HASH_TREE
//        LetterNode * pletternodeLastForInsertedWord = NULL ;
  //VocabularyAndTranslation * pvocabularyandtranslation = NULL ;
  //VocabularyAndTranslation * pvocabularyandtranslationReturn = NULL ;
#endif //#ifdef _INSERT_INTO_HASH_TREE
  for( WORD nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar <
    strCurrentWordData.length() ; nIndexOfCurrentChar ++ )
  {
    chCurrent = strCurrentWordData[nIndexOfCurrentChar] ;
    if( nIndexOfCurrentChar == 1 && chCurrent != STRING_DELIMITER )
    {
      nIndexOf1stWordChar = 2 ;
      byNumberOfObjectsAllowed = chCurrent - '1' ;
    }
    if( nIndexOfCurrentChar == 2 && chCurrent != STRING_DELIMITER )
    {
      nIndexOf1stWordChar = 3 ;
      byAllowsProgressiveForm = chCurrent - '1' ;
    }
    if( chCurrent == STRING_DELIMITER)
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

          BYTE byGrammarPartID = 0;
          switch( byNumberOfObjectsAllowed )
          {
          case 0 :
            byGrammarPartID = EnglishVerb::main_verb_allows_0object_infinitive;
            break;
          case 1:
            byGrammarPartID = EnglishWord::main_verb_allows_1object_infinitive;
            break;
          case 2:
            byGrammarPartID = EnglishWord::main_verb_allows_2objects_infinitive;
            break;
          }

          s_p_lettertree->InsertIntoTrieAndHandleVocabularyAndTranslation(
            stdsetpletternodeLastStringChar
            , bInsertNewVocabularyAndTranslation
            , byGrammarPartID
            , strCurrentWordData
            , nIndexOfCurrentChar - nIndexOf1stWordChar
            , nIndexOf1stWordChar
            ) ;
  //        ev->m_strInfinitive =
  //          strCurrentWordData.substr(
  //            nIndexOf1stChar
  //            , nIndexOfCurrentChar - nIndexOf1stChar );
  #ifdef _INSERT_INTO_HASH_TREE
          if( byAllowsProgressiveForm )
            s_p_lettertree->InsertPresentParticipleReferringVerbAttributes(
              //This set is to ensure that if strings for the SAME vocabulary
              // not 2 or more VocAndTransl object should be inserted.
              stdsetpletternodeLastStringChar
              , stdstrInfinitive
              , byNumberOfObjectsAllowed
              ) ;
          s_p_lettertree->Insert3rdPersonSingularPresentReferringVerbAttributes(
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
          stdstrInfinitive = strCurrentWordData.substr(
            nIndexOf1stWordChar , nIndexOfCurrentChar - nIndexOf1stWordChar ) ;
//        //Ensure insertion into the trie because the grammar part should
          // not be as for the infinitive.
//          bInsertNewVocabularyAndTranslation = true;

//        p_englishverb->m_strPastTense = strCurrentWordData.substr(
//          nIndexOf1stWordChar, nIndexOfCurrentChar - nIndexOf1stWordChar );

          s_p_lettertree->InsertPastTenseReferringVerbAttributes(
              stdsetpletternodeLastStringChar
              , stdstrInfinitive
              , byNumberOfObjectsAllowed
            );
        break ;
      //if(delemiterCount==2)
        case string_index_for_past_participle :
//        p_englishverb->m_strPastParticiple = strCurrentWordData.substr(
//          nIndexOf1stWordChar, nIndexOfCurrentChar - nIndexOf1stWordChar );
          stdstrInfinitive = strCurrentWordData.substr(
            nIndexOf1stWordChar , nIndexOfCurrentChar - nIndexOf1stWordChar ) ;
          s_p_lettertree->InsertPastParticipleReferringVerbAttributes(
              stdsetpletternodeLastStringChar
              , stdstrInfinitive
              , byNumberOfObjectsAllowed
            );
        break ;
//      if(delemiterCount == 3)
//        p_englishverb->m_strPreposition = strCurrentWordData.substr(
//          nIndexOf1stChar, nIndexOfCurrentChar - nIndexOf1stChar );
#ifdef _INSERT_INTO_HASH_TREE
#ifdef _DEBUG
//        if( p_englishverb->m_strInfinitive == _T("talk") )
//          //Senseless line just for ability to set breakpoints.
//          p_englishverb->m_strInfinitive += "" ;
#endif
      } //switch
        if(delemiterCount < NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB )
        {
//          if( g_lettertree.s_pvocabularyandtranslation )
          if( s_p_lettertree->s_pvocabularyandtranslation )
//            g_lettertree.s_pvocabularyandtranslation->m_arstrEnglishWord[
            s_p_lettertree->s_pvocabularyandtranslation->m_arstrEnglishWord[
              delemiterCount] = //str.Mid(start,i-start);
              strCurrentWordData.substr( nIndexOf1stWordChar
                , nIndexOfCurrentChar - nIndexOf1stWordChar ) ;
        }
#endif //#ifdef _INSERT_INTO_HASH_TREE
      delemiterCount ++ ;
      nIndexOf1stWordChar = nIndexOfCurrentChar + 1 ;
    } //if(chCurrent == STRING_DELIMITER)
    if( delemiterCount == 5
        &&
        ( chCurrent == '1' || chCurrent == '2' || chCurrent == '3' ||
          chCurrent == '4'
        )
      )
    {
//      p_englishverb->m_bAllowsIngForm = 0 ;
//      p_englishverb->m_bAllowsToPlusInfinitive = 0 ;
      if( chCurrent == '1' || chCurrent == '2' )
        p_englishverb->m_bAllowsToPlusInfinitive = 1 ;
      if( chCurrent == '1' || chCurrent == '3' )
      {
//        p_englishverb->m_bAllowsIngForm = 1;
//        g_lettertree.InsertPresentParticipleReferringVerbAttributes(
//          //This set is to ensure that if strings for the SAME vocabulary
//          // not 2 or more VocAndTransl object should be inserted.
//          stdsetpletternodeLastStringChar
//          ) ;
      }
#ifdef _INSERT_INTO_HASH_TREE
//      g_lettertree.s_pvocabularyandtranslation->m_arbyAttribute[0] = 0 ;
      s_p_lettertree->s_pvocabularyandtranslation->m_arbyAttribute[0] = 0 ;
#endif
//            return ev;
      return ;
    }
    if( delemiterCount == 4 && ( chCurrent == '1' || chCurrent == '2' )
      )
      ++ delemiterCount;
  }//"for" loop
}

void OneLinePerWordPair::InsertEnglishAdjective(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  LOGN("OneLinePerWordPair::InsertEnglishAdjective(...)");

  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;

  //Set to "true" for the first string/ word belonging to this vocabulary.
  bool bInsertNewVocabularyAndTranslation = true ;

//  EnglishAdjective * p_englishadjective = new EnglishAdjective;
//  if( p_englishadjective )
  {
    BYTE delemiterCount = 0 ;
//    BYTE bMoreMost=2;
    WORD wIndexOf1stChar = 3;
    if( strCurrentWordData.size() > 2 )
    {
      char ch = strCurrentWordData[1] ;
      if( ch == '1' || ch == '2' )
      {
//        p_englishadjective->m_bMoreMost = strCurrentWordData[1] -
//          ASCII_CODE_FOR_DIGIT_1;
      }
      ch = strCurrentWordData[2] ;
      if( //ch == '1' || ch == '2' || ch == '3'
          ch >= '1' && ch <= '3'
        )
      {
//        p_englishadjective->m_bAllowedPlace = strCurrentWordData[2] -
//          ASCII_CODE_FOR_DIGIT_1;
      }
    }
    int nCharIndexOf1stWordChar = 1;
    for( int nIndexOfCurrentChar = 3 ; nIndexOfCurrentChar <
      strCurrentWordData.length(); nIndexOfCurrentChar ++ )
    {
      if( strCurrentWordData[nIndexOfCurrentChar] == STRING_DELIMITER )
      {
        if( delemiterCount == 0 )
        {
//          p_englishadjective->m_strPositiv = //str.Mid(start,i-start);
//            strCurrentWordData.substr(wIndexOf1stChar,nIndexOfCurrentChar -
//              wIndexOf1stChar);

          s_p_lettertree->InsertIntoTrieAndHandleVocabularyAndTranslation(
            stdsetpletternodeLastStringChar
            , //true
              bInsertNewVocabularyAndTranslation
            , //byVocabularyType
            EnglishWord::adjective_positiveForm
            , strCurrentWordData
            , nIndexOfCurrentChar - 1 //nIndexOf1stChar
            , //1
            nCharIndexOf1stWordChar
            ) ;
        }
        else if( delemiterCount < NUMBER_OF_STRINGS_FOR_ENGLISH_ADJECTIVE )
  //        if( g_lettertree.s_pvocabularyandtranslation )
          if( s_p_lettertree->s_pvocabularyandtranslation )
  //          g_lettertree.s_pvocabularyandtranslation->m_arstrEnglishWord[
            s_p_lettertree->s_pvocabularyandtranslation->m_arstrEnglishWord[
              delemiterCount
              ] =
              strCurrentWordData.substr( 1 //nIndexOf1stChar
                , nIndexOfCurrentChar - 1 //nIndexOf1stChar
                ) ;
        if( delemiterCount == 1 )
        {
//          if(p_englishadjective->m_bMoreMost == 1 )
//          {
//            p_englishadjective->m_strAdverb = strCurrentWordData.substr(
//              wIndexOf1stChar,nIndexOfCurrentChar - wIndexOf1stChar);
//            return ;
//          }
//          p_englishadjective->m_strComperativ = strCurrentWordData.substr(
//            wIndexOf1stChar,nIndexOfCurrentChar - wIndexOf1stChar);
        }
//        if( delemiterCount == 2 )
//          p_englishadjective->m_strSuperlativ = strCurrentWordData.substr(
//            wIndexOf1stChar,nIndexOfCurrentChar - wIndexOf1stChar);
        if( delemiterCount == 3 )
        {
//          p_englishadjective->m_strAdverb = strCurrentWordData.substr(
//            wIndexOf1stChar,nIndexOfCurrentChar - wIndexOf1stChar);
        }
        delemiterCount ++;
        wIndexOf1stChar = nIndexOfCurrentChar + 1 ;
      }
    }
    if(delemiterCount == 4 )
    {
      return ;
    }
  }
}

void OneLinePerWordPair::InsertEnglishAdverb(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  bool bInsertNewVocabularyAndTranslation = true ;
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
//  EnglishAdverb * p_english_adverb = new EnglishAdverb;
  BYTE delemiterCount = 0 ;
  int nIndexOf1stChar = 1;
  char chCurrent ;
  for( WORD nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar <
    strCurrentWordData.length(); ++ nIndexOfCurrentChar )
  {
    chCurrent = strCurrentWordData[nIndexOfCurrentChar] ;
    if( chCurrent == STRING_DELIMITER )
    {
//      if( delemiterCount == 0 )
//        p_english_adverb->m_strWord = //str.Mid(1,i-1);
//          strCurrentWordData.substr(1, nIndexOfCurrentChar - 1 );
      ++ delemiterCount ;
      int nLength = nIndexOfCurrentChar - nIndexOf1stChar ;
  //        g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
        s_p_lettertree->InsertIntoTrieAndHandleVocabularyAndTranslation(
          stdsetpletternodeLastStringChar
          //, LetterNode * pletternodeCurrent
          //, VocabularyAndTranslation * pvocabularyandtranslation
          , bInsertNewVocabularyAndTranslation
          , //byVocabularyType
            EnglishWord::adverb
          , strCurrentWordData
          , //nIndexOfCurrentChar - nIndexOf1stChar
            nLength
          , nIndexOf1stChar
          ) ;
    }
    if( //chCurrent == '1' || chCurrent == '2' || chCurrent == '3' ||
        //chCurrent == '4' || chCurrent == '5' || chCurrent == '6'
        chCurrent >= '1' && chCurrent <= '6'
      )
      if( delemiterCount == 1)
      {
//        p_english_adverb->m_bType = strCurrentWordData[nIndexOfCurrentChar] -
//          ASCII_CODE_FOR_DIGIT_1 ;
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
//  BYTE otherCount=0;
  int start=1;
  for( WORD i=1;i < strCurrentWordData.length();i++)
  {
    if( strCurrentWordData[i] == STRING_DELIMITER )
    {
      if( delemiterCount < 14 )
        eav->m_strWords[delemiterCount] = //str.Mid(start,i-start);
          strCurrentWordData.substr(start,i-start);
//            if(delemiterCount==13)
//              return eav;
      else
        return ;
      delemiterCount ++;
      start = i + 1 ;
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
  for( WORD nIndexOfCurrentChar = 1; nIndexOfCurrentChar <
    strCurrentWordData.length() ; nIndexOfCurrentChar ++ )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == STRING_DELIMITER )
    {
//      ec->m_strWord = //str.Mid(1,i-1);
//        strCurrentWordData.substr(1,i-1);
//            if( i + 1 == strCurrentWordData.length() )
//              return ec;
//      g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
      s_p_lettertree->InsertIntoTrieAndHandleVocabularyAndTranslation(
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
//        if( g_lettertree.s_pvocabularyandtranslation )
        if( s_p_lettertree->s_pvocabularyandtranslation )
//          g_lettertree.s_pvocabularyandtranslation->m_arstrEnglishWord[
          s_p_lettertree->s_pvocabularyandtranslation->m_arstrEnglishWord[
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
  for( WORD i=1 ; i < strCurrentWordData.length(); ++i)
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

void OneLinePerWordPair::InsertEnglishPronoun(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  EnglishPronoun * p_englishpronoun = new EnglishPronoun;
  BYTE delemiterCount = 0 ;
  DWORD dwStart = 0 ;
  for( WORD i = 1 ; i < strCurrentWordData.length(); i++)
  {
    if( strCurrentWordData[i] == STRING_DELIMITER )
    {
      if(delemiterCount == 0 )
      {
        p_englishpronoun->m_strSingular = //str.Mid(1,i-1);
          strCurrentWordData.substr(1, i - 1 );
      }
      if( delemiterCount == 1 )
      {
        p_englishpronoun->m_strPlural = //str.Mid(dwStart,i-dwStart);
          strCurrentWordData.substr( dwStart,i - dwStart );
//              if( i + 1 == strCurrentWordData.length() )
//                return ep;
        return ;
      }
      delemiterCount++;
      dwStart = i + 1 ;
    }
  }
}

void OneLinePerWordPair::InsertGermanAdjective(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
  LOGN( //"OneLinePerWordPair::InsertGermanAdjective(...)"
    /*FULL_FUNC_NAME <<*/ "begin" )
//  GermanAdjective * p_germanadjective = new GermanAdjective;
  BYTE delemiterCount = 0 ;
  int nIndexOf1stChar = 1 ;
  for( WORD nCurrentCharIndexWithinWholeVocabularyString = 1;
    nCurrentCharIndexWithinWholeVocabularyString <
    strCurrentWordData.length(); ++ nCurrentCharIndexWithinWholeVocabularyString )
  {
    if( strCurrentWordData[nCurrentCharIndexWithinWholeVocabularyString] ==
        STRING_DELIMITER )
    {
//      VTrans::string_type * p_strCurrentWord;
//      switch( delemiterCount )
//      {
//      case 0 :
////        p_strCurrentWord = & p_germanadjective->m_strPositiv;
//        break ;
//      case 1 :
////        p_strCurrentWord = & p_germanadjective->m_strComperativ;
//        break ;
//      case 2 :
////        p_strCurrentWord = & p_germanadjective->m_strSuperlativ;
//        break ;
//      case 3 :
////        p_strCurrentWord = & p_germanadjective->m_strWortstamm;
//        break ;
//      case 4 :
//          return ;
//      }
//      * p_strCurrentWord = strCurrentWordData.substr(
//        nIndexOf1stChar,nCurrentCharIndexWithinWholeVocabularyString -
//        nIndexOf1stChar);
      if( delemiterCount < NUMBER_OF_STRINGS_FOR_GERMAN_ADJECTIVE )
      {
        const VocabularyAndTranslation * const
          p_vocabularyandtranslation =
          s_p_lettertree->s_pvocabularyandtranslation;
        LOGN( /*FULL_FUNC_NAME <<*/ "\"voc and translation pointer\" is: \""
          << p_vocabularyandtranslation << "\"")

        std::string * p_std_str = & p_vocabularyandtranslation->
          m_arstrGermanWord[delemiterCount ];
        //Add to the structure that also contains the English strings.
        //So the German equivalent can be retrieved when the last
        //character (LetterNode) of the English string inside the Trie
        //is got.
//        g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord[
        * p_std_str = //std_strCurrentWord;
          strCurrentWordData.substr(
            nIndexOf1stChar
            , nCurrentCharIndexWithinWholeVocabularyString - nIndexOf1stChar
            ) ;
        LOGN( /*FULL_FUNC_NAME <<*/ "inserted string \"" << * p_std_str
          << "\" at array index \"" << delemiterCount << "\"")
      }
      delemiterCount ++;
      nIndexOf1stChar = nCurrentCharIndexWithinWholeVocabularyString + 1 ;
    }
  }
}

void OneLinePerWordPair::InsertGermanAdverb(
  const std::string & strCurrentWordData
  , BYTE byVocabularyType
  )
{
//  GermanAdverb * p_german_adverb = new GermanAdverb;
  BYTE delemiterCount = 0;
  int nIndexOf1stChar = 1 ;
  for( WORD nIndexOfCurrentChar = nIndexOf1stChar ;
      nIndexOfCurrentChar < strCurrentWordData.length();
      ++ nIndexOfCurrentChar
    )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == STRING_DELIMITER )
    {
      if( delemiterCount == 0 )
      {
//        p_german_adverb->m_strWord = //str.Mid(1,i-1);
//          strCurrentWordData.substr(1,nIndexOfCurrentChar - 1);
#ifdef COMPILE_WITH_LOG
        std::string stdstr = strCurrentWordData.substr(
          nIndexOf1stChar
          , nIndexOfCurrentChar - nIndexOf1stChar
          ) ;
        LOGN("OneLinePerWordPair::InsertGermanAdverb(...)--Inserting \""
          << stdstr << "\" into data structure.")
        s_p_lettertree->s_pvocabularyandtranslation->m_arstrGermanWord[
          delemiterCount ] = stdstr ;
#else
        s_p_lettertree->s_pvocabularyandtranslation->m_arstrGermanWord[
          delemiterCount ] = //str.Mid(start,i-start);
          strCurrentWordData.substr(
            nIndexOf1stChar
            , nIndexOfCurrentChar - nIndexOf1stChar
            );
#endif //#ifdef COMPILE_WITH_LOG
      }
      ++ delemiterCount;
    }
  }
  if( delemiterCount == 1 )
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
  for( WORD i=1;i < strCurrentWordData.length(); ++i)
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
//  bool bInsertNewVocabularyAndTranslation = true ;
//  GermanConjunction * gc = new GermanConjunction;
  BYTE bDelemiterOccured=FALSE;
//  BYTE otherCount=0;
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
  for( WORD nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar <
    strCurrentWordData.length(); ++ nIndexOfCurrentChar )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == '9' ) // finite (gebeugte) Verbformen
    {
//      gc->m_strWord = //str.Mid(1,i-1);
//        strCurrentWordData.substr(1,nIndexOfCurrentChar-1);
      bDelemiterOccured = TRUE ;
//      g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(

//        if( g_lettertree.s_pvocabularyandtranslation )
        if( s_p_lettertree->s_pvocabularyandtranslation )
//          g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord[
          s_p_lettertree->s_pvocabularyandtranslation->m_arstrGermanWord[
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
  TRACE("deutsches Verb\n");
  char chCurrentChar ;
//  GermanVerb * gv = new GermanVerb;
//  gv->m_bAuxiliaryVerb = FALSE;
  BYTE byDelemiterCount = 0 ;
  BYTE otherCount = 0 ;
  int nIndexOf1stChar = 1 ;
  for( WORD nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar <
    strCurrentWordData.length() ; nIndexOfCurrentChar ++ )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == STRING_DELIMITER ) // finite (gebeugte) Verbformen,
      //Praeposition
    {
      if( byDelemiterCount < 16 )
      {
//        gv->m_strWords[byDelemiterCount] = //str.Mid(start,i-start);
//          strCurrentWordData.substr( nIndexOf1stChar
//          , nIndexOfCurrentChar - nIndexOf1stChar );
        //Add to the stucture that also contains the English strings.
        //So the German equivalent can be retrieved when the last
        //character (LetterNode) of the English string inside the Trie
        //is got.
//        g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord[
        s_p_lettertree->s_pvocabularyandtranslation->m_arstrGermanWord[
          byDelemiterCount ] = //str.Mid(start,i-start);
          strCurrentWordData.substr( nIndexOf1stChar ,
          nIndexOfCurrentChar - nIndexOf1stChar );
      }
//      if( byDelemiterCount == 16 )
//        gv->m_strPreposition = //str.Mid(start,i-start);
//          strCurrentWordData.substr( nIndexOf1stChar
//          , nIndexOfCurrentChar - nIndexOf1stChar );
      byDelemiterCount ++ ;
      nIndexOf1stChar = nIndexOfCurrentChar + 1 ;
    }
    chCurrentChar = strCurrentWordData[nIndexOfCurrentChar] ;
    // in welchem/welchen Fall/Fällen steht/stehen das/die Objekt(e)
    if( chCurrentChar == '1' ||
      strCurrentWordData[nIndexOfCurrentChar] == '2' ||
      strCurrentWordData[nIndexOfCurrentChar] == '3' ||
      strCurrentWordData[nIndexOfCurrentChar] == '4'
      )
    {
//      if( otherCount == OneLinePerWordPair::movement_verb )
//        gv->m_bMove = chCurrentChar - 49 ;
//      if( otherCount == OneLinePerWordPair::auxiliary_verb_type )
//        gv->m_bBe = chCurrentChar - 49 ;
      if( otherCount == OneLinePerWordPair::grammatical_case )
      {
//        //Verb verlangt kein Objekt.
//        if( chCurrentChar == '1' )
//          gv->m_bCase = 0 ;
//        //Verb verlangt Objekt im 3. Fall.
//        if( chCurrentChar == '2' )
//          gv->m_bCase = 1 ;
//        //Verb verlangt Objekt im 4. Fall.
//        if( chCurrentChar == '3' )
//        {
//          gv->m_bCase = 2 ;
//        }
//        if( chCurrentChar == '4') // Verb verlangt 2 Objekte im 3. und 4. Fall
//          gv->m_bCase = 3 ;
//        g_lettertree.s_pvocabularyandtranslation->m_arbyAttribute[
        s_p_lettertree->s_pvocabularyandtranslation->m_arbyAttribute[
          VocabularyAndTranslation::array_index_for_case ] =
          chCurrentChar - //49
          '0';
      }
      if( otherCount == OneLinePerWordPair::reflexive_or_not )
      {
//        if( chCurrentChar == '1' )
//          gv->m_bReflexive = FALSE ;
//        if( chCurrentChar == '2' )
//          gv->m_bReflexive = TRUE ;
          s_p_lettertree->s_pvocabularyandtranslation->m_arbyAttribute[
            VocabularyAndTranslation::array_index_for_reflexive ] =
            chCurrentChar - //49
            '1';
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
  TRACE("deutsches Substantiv: %s\n", strCurrentWordData);
//  GermanNoun * p_german_noun = new GermanNoun;
  BYTE delemiterCount=0;
  int nIndexOf1stChar = 1 ;
//  LetterNode * pletternode = NULL ;
  for( WORD nIndexOfCurrentChar = 1 ; nIndexOfCurrentChar <
      strCurrentWordData.length() ; nIndexOfCurrentChar ++
    )
  {
    if( strCurrentWordData[nIndexOfCurrentChar] == STRING_DELIMITER )
    {
//      if( delemiterCount == 0 )
//        p_german_noun->m_strSingular = //str.Mid(start,i-start);
//          strCurrentWordData.substr(
//            nIndexOf1stChar ,
//            nIndexOfCurrentChar - nIndexOf1stChar
//            );
//      if( delemiterCount == 1 )
//        p_german_noun->m_strPlural = //str.Mid(start,i-start);
//          strCurrentWordData.substr(
//            nIndexOf1stChar ,
//            nIndexOfCurrentChar - nIndexOf1stChar
//            );
#ifdef _INSERT_INTO_HASH_TREE
//              //g_pvocabularyandtranslation->m_byType = GERMAN_NOUN ;
//              if(pletternode)//If the pointer is assigned yet.
//                pletternode->insert(g_pvocabularyandtranslation) ;
      if( delemiterCount < NUMBER_OF_STRINGS_FOR_GERMAN_NOUN )
      {
        //pvocabularyandtranslation->m_arstrGermanWord[
        std::string std_strCurrentWord = strCurrentWordData.substr(
          nIndexOf1stChar
          , nIndexOfCurrentChar - nIndexOf1stChar
          );

        //Add to the structure that also contains the English strings.
        //So the German equivalent can be retrieved when the last
        //character (LetterNode) of the English string inside the Trie
        //is got.
//        g_lettertree.s_pvocabularyandtranslation->m_arstrGermanWord[
        s_p_lettertree->s_pvocabularyandtranslation->m_arstrGermanWord[
          delemiterCount ] = //str.Mid(start,i-start);
          std_strCurrentWord;
#ifdef _DEBUG
        std::cout << "German noun read:" <<
//            g_lettertree.s_pvocabularyandtranslation->
          s_p_lettertree->s_pvocabularyandtranslation->
            m_arstrGermanWord[delemiterCount] << "\n" ;
#endif //#ifdef _DEBUG
      }
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
//        p_german_noun->m_bArticle = strCurrentWordData[nIndexOfCurrentChar];
        //return TRUE;
        //voc=dynamic_cast<Vocable*>(gn);
        //return 0;
//                return gn;
//        g_lettertree.s_pvocabularyandtranslation->m_arbyAttribute[
        s_p_lettertree->s_pvocabularyandtranslation->m_arbyAttribute[
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
  WORD i = 1 ;
  for( ;i < strCurrentWordData.length();i++)
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
  for( WORD i=1;i < strCurrentWordData.length();++i)
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
BYTE OneLinePerWordPair::InsertEnglishVocable(
  //bool bInsertNewVocabularyAndTranslation,
  const VTrans::string_type & strCurrentWordData,
  std::set<LetterNode*> & stdsetpletternodeLastStringChar
  )
{
//    bInsertNewVocabularyAndTranslation = true;
  BYTE byVocabularyType = strCurrentWordData[ WORD_TYPE_CHAR_INDEX ] ;
  LOGN("vocabulary type: " << (WORD) byVocabularyType )
  stdsetpletternodeLastStringChar.clear();
  switch( byVocabularyType )
  {
  case WORD_TYPE_NOUN : // Substantiv
    LOGN("inserting English noun")
    return InsertEnglishNoun(strCurrentWordData, byVocabularyType) ;
  case WORD_TYPE_MAIN_VERB : // englisches Vollverb
    LOGN("inserting English main verb")
    InsertEnglishMainVerb(strCurrentWordData,byVocabularyType,
        stdsetpletternodeLastStringChar) ;
//        return ;
    break;
  case WORD_TYPE_ADJECTIVE : // englisches Adjektiv
    LOGN("inserting English adjective")
    InsertEnglishAdjective(strCurrentWordData,byVocabularyType) ;
//        return ;
    break ;
  case WORD_TYPE_ADVERB : // Adverb
    LOGN("inserting English adverb")
    InsertEnglishAdverb(strCurrentWordData,byVocabularyType) ;
//        return ;
    break ;
  case WORD_TYPE_PREPOSITION : // Präposition
    LOGN("inserting English preposition")
    InsertEnglishPreposition(strCurrentWordData,byVocabularyType) ;
//        return ;
    break ;
  case WORD_TYPE_PRONOUN : // Pronomen
    LOGN("inserting English pronoun")
    InsertEnglishPronoun(strCurrentWordData,byVocabularyType) ;
//        return ;
    break ;
  case WORD_TYPE_AUX_VERB : // Hilfsverb
    LOGN("inserting English auxiliary verb")
    InsertEnglishAuxiliaryVerb(strCurrentWordData,byVocabularyType) ;
//        return ;
    break ;
  case WORD_TYPE_CONJUNCTION : // englische Konjunktion
    LOGN("inserting English conjunction")
    InsertEnglishConjunction(strCurrentWordData,byVocabularyType) ;
//        return ;
    break ;
  }
  return 0;
}

BYTE OneLinePerWordPair::InsertGermanVocable(
  const VTrans::string_type & strCurrentWordData)
{
  LOGN("inserting German word")
  BYTE byVocabularyType = strCurrentWordData[ WORD_TYPE_CHAR_INDEX ] ;
  switch ( byVocabularyType )
  {
    case WORD_TYPE_NOUN : // deutsches Substantiv
      InsertGermanNoun( strCurrentWordData, byVocabularyType) ;
//          return ;
      break ;
    case WORD_TYPE_MAIN_VERB : // deutsches Verb
      InsertGermanMainVerb( strCurrentWordData, byVocabularyType) ;
//          return ;
      break ;
    case WORD_TYPE_ADJECTIVE : // deutsches Adjektiv
      InsertGermanAdjective( strCurrentWordData, byVocabularyType) ;
//          return ;
      break ;
    case WORD_TYPE_ADVERB : // deutsches Adverb
      InsertGermanAdverb( strCurrentWordData, byVocabularyType) ;
//          return ;
      break ;
    case WORD_TYPE_PREPOSITION : // deutsche Präposition
      InsertGermanPreposition( strCurrentWordData, byVocabularyType) ;
//          return ;
      break ;
    case WORD_TYPE_PRONOUN : // deutsches Pronomen
      InsertGermanPronoun( strCurrentWordData, byVocabularyType) ;
//          return ;
      break ;
    case WORD_TYPE_AUX_VERB : // deutsches Hilfsverb
      InsertGermanAuxiliaryVerb( strCurrentWordData, byVocabularyType) ;
//          return ;
      break ;
    case WORD_TYPE_CONJUNCTION : // deutsche Konjunktion
      InsertGermanConjunction( strCurrentWordData, byVocabularyType) ;
//          return ;
      break ;
  }
  return 0;
}

void OneLinePerWordPair::extract(
  const VTrans::string_type & strCurrentWordData
  , BYTE bEnglishWord
  , int & ret
  )
{
	LOGN(//"OneLinePerWordPair::extract--"
    "word data: " << strCurrentWordData
	  << "English word:" << (bEnglishWord ? "yes" : "no" ) )
  std::set<LetterNode *> stdsetpletternodeLastStringChar ;
  if( strCurrentWordData.length() > 0 )
  {
//    bool bInsertNewVocabularyAndTranslation = true ;
#ifdef _DEBUG
	//printf("Vocable * extract(CString s,BOOL english,int & ret) ANFANG\n");
#endif
	  //TRACE("Vocable * extract(CString s,BOOL english,int & ret) ANFANG\n");
	  if(bEnglishWord)
	  {
      ret = InsertEnglishVocable( //bInsertNewVocabularyAndTranslation,
        strCurrentWordData, stdsetpletternodeLastStringChar);
	  }
	  else // deutsch
	  {
      ret = InsertGermanVocable(strCurrentWordData);
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
//	BOOL break_while = FALSE ;
//	BOOL flag = FALSE ;
	VTrans::string_type concatenate;
  VTrans::string_type strWordFile = //_T(
    //"http://www.f4.fhtw-berlin.de/~s0518039/"
    "words.txt" //);
    ;
//  WordNode * pWordNodePrevious=NULL;

	//zuerst die integralen Vokabeln der verketteten Liste hinzufügen, Ende
  LOGN("05.06.2008 22.22.17");
	if( ( p_fileWords = fopen( //strWordFile.c_str()
    r_strWordsFilePath.c_str() ,"rb") ) != NULL )
	{
	  byRet = 1 ;
    LOGN("05.06.2008 22.22.26");
		concatenate = "" ;
//		BYTE bEnglishWord = TRUE;
		VTrans::string_type str;
		DWORD dwOffset = 0;
		DWORD dwOffsetOfBeginOfEntry = 0;//Offset (Position) des Anfanges eines
		BYTE byLanguage = English;
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
					int ret = 0;
          if( byLanguage == English )
          {
            byLanguage = ! byLanguage;

            OneLinePerWordPair::extract(str,TRUE,ret);
//            ++ OneLinePerWordPair::s_dwNumberOfVocabularyPairs ;
            ++ s_p_userinterface->s_numberOfVocabularyPairs;
            #ifdef _DEBUG
            const char * pch = str.c_str() ;
            pch = 0 ;
            #endif

          }
          else
            ret = 1;

          if( ret != 0 )
          {
            s_p_userinterface->ShowInvalidVocabularyFileFormatMessage(
//              strWordFile,
              r_strWordsFilePath,
              dwOffsetOfBeginOfEntry,
              dwOffset
              );
            return ret;
          }
  				str = "" ;
          dwOffsetOfBeginOfEntry = dwOffset + 1 ;
          break ;
				}
//				else // deutsch
          case '\n' :
				{
					int ret = 0;
          if( byLanguage == German )
          {
            byLanguage = ! byLanguage;

            OneLinePerWordPair::extract(str,FALSE,ret);
          }
          else
            ret = 1;

					if( ret != 0)
          {
            //MessageBox("Datei %s enthält kein gültiges Format oder sonstiger
					  // Fehler","Fehler beim Lesen der Datei vocs.txt",MB_OK);
            s_p_userinterface->ShowInvalidVocabularyFileFormatMessage(
//              strWordFile,
              r_strWordsFilePath,
              dwOffsetOfBeginOfEntry,
              dwOffset
              );
            return ret;
          }
  				str = "" ;
          dwOffsetOfBeginOfEntry = dwOffset + 1 ;
          break ;
				}
				//bEnglishWord=!bEnglishWord;
//				str = "" ;
//			}
//			else
//			{
        default:
          str += (char) i ;
      }
//			}
			++ dwOffset;
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
    LOGN( "file \"" << r_strWordsFilePath << "\" could not be opened")
	}
//	wordList.m_pWordNodeLast=pWordNodeCurrent;
//	wordList.m_pWordNodeLast->m_pWordNodeNext=NULL;
#ifdef _DEBUG
	printf("IO.cpp: void OneLinePerWordPair::LoadWords(WordNode * pWordNode) ENDE\n");
#endif
	return byRet ;
}

//void ::TranslateAndWriteOutputFile()
//{
//}

//const char * 
BYTE readInputText(const std::string & strFilePath, std::string & str)
{
    const char * pch = NULL ;
    //std::string str;
	std::ifstream ifstreamGermanText(
	  //Output filename.
	  //"english.txt"
	  strFilePath.c_str(), std::ios_base::in);
	//wenn erfolgreich geöffnet.
	if(ifstreamGermanText)
	{
	  char ch;
	  //LPWSTR pwstrTranslation=NULL;
	  while(true)
	  {
	    ch = ifstreamGermanText.get();
	    if( ifstreamGermanText.eof() )
	      break;
	    str += ch;
	  }
	  LOGN("28.05.2008 20.59.56");
	  //LPWSTR wstrTranslation((LPWSTR)str.c_str());
	  LOGN("28.05.2008 20.58.44");
	
	  //pch=pszA;
	  pch = //::UTF8toASCII(
      str.c_str() //)
      ;
	  LOGN("12.06.2008 22.24.35 " << (unsigned long) pch << pch);
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
