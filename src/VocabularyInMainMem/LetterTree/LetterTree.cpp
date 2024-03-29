//Ohne das folgende include/wenn Pfad in den Eigenschaften dieser Datei 
//unter C/C++->vorkompilierte Header->"PHC durch..." falsch ist: 
//"fatal error C1010: Unerwartetes Dateiende w�hrend der Suche nach dem vorkompilierten Header.[...]"
#include "../../StdAfx.h"
#include <Attributes/EnglishWord.hpp> //class EnglishWord's enum
#include <Attributes/GermanWord.hpp> //class GermanWord
#include "LetterTree.hpp" //header file of this LetterTree class
#include <preprocessor_macros/logging_preprocessor_macros.h>
#include <preprocessor_macros/string_typedefs.h> //for "LPCSTR"

#ifdef _DEBUG
  #define _DEBUG_FREEING_MEM
#endif

//Static variables also need to defined. Else g++ error:
//"undefined reference to `OneLinePerWordPair::s_pvocabularyandtranslation'"
VocabularyAndTranslation * LetterTree::s_pvocabularyandtranslation ;
LetterNode * LetterTree::sp_letternodeLastForInsertedWord ;

  //pletternode1LevelAboveNodeToCreateIfNonExistantAndGoneInto:
  //the entry at byIndex by 1 level below is checked:
  //
  //           node above
  //              |
  // +---+--------+
  // |   |
  //[0] [1] [2] ...       <-element 1 level below
LetterNode * LetterTree::CreateNodeIfNonExistant(
	LetterNode * 
	pletternode1LevelAboveNodeToCreateIfNonExistantAndGoneInto,
	BYTE byIndex)
{
  //Point to the address of the desired array element.
  LetterNode ** ppletternodeToCreateIfNonExistant =
	  &pletternode1LevelAboveNodeToCreateIfNonExistantAndGoneInto->
	  m_arpletternode1LevelLower[byIndex];
  //#ifdef _DEBUG_DETLEVEL_STEFAN
	 // //cout<<"CPersonEntryNodeTree::CreateNodeIfNonExistantAndGoIntoIt() begin\n";
	 // TRACE("pletternode1LevelAboveNodeToCreateIfNonExistantAndGoneInto:%u",
		//  pletternode1LevelAboveNodeToCreateIfNonExistantAndGoneInto);
	 // TRACE(" byIndex:%u\n",(short)byIndex);
  //#endif
  //TRACE("*ppletternodeToCreateIfNonExistant: \"%x\"\n",
  //  *ppletternodeToCreateIfNonExistant ) ;

  //If the node has NOT been created yet (=NULL).
  //if(!pletternode1LevelAboveNodeToCreateIfNonExistantAndGoneInto->
	 // m_arpletternode1LevelLower[byIndex])
  if(! *ppletternodeToCreateIfNonExistant)
  {
	  *ppletternodeToCreateIfNonExistant = new LetterNode(
      m_byArrayIndexRespSize, this);
//#ifdef _DEBUG_DETLEVEL_STEFAN
//	  TRACE("ppletternodeToCreateIfNonExistant:%u",
//		  ppletternodeToCreateIfNonExistant);
//	  TRACE(" *ppletternodeToCreateIfNonExistant:%u\n",
//		  *ppletternodeToCreateIfNonExistant);
//#endif//_DEBUG
  }
#ifdef _DEBUG//_DETLEVEL_STEFAN
  else
  {
//	  cout<<"NODE ALREADY THERE ppletternodeToCreateIfNonExistant:"<<
//		  ppletternodeToCreateIfNonExistant;
//	  cout<<" *pletternodeToCreateIfNonExistant:"<<
//		  *ppletternodeToCreateIfNonExistant<<"\n";
  }
#endif//_DEBUG
  return *ppletternodeToCreateIfNonExistant;
}

//Word * LetterTree::GetFirstOccurance(
//  const std::string & cr_stdstr
//  , Word ** p_wordEnglish
//  , Word ** p_wordGerman
//  )
//{
//}

//void 
//0=failure (no vocable with the string found)
BYTE LetterTree::GetFirstOccurance(
  const std::string & cr_stdstr
  //, Word & r_wordEnglish
  //, Word & r_wordGerman
  //, Word * & p_wordEnglish
  //, Word * & p_wordGerman
  , AutomDelWord & r_automdelwordEng
  , AutomDelWord & r_automdelwordGer
  )
{
  //std::string::_Elem 
  //TCHAR * p_tch = cr_stdstr.c_str() ;
  const char * p_ch = cr_stdstr.c_str() ;
  std::set<VocabularyAndTranslation *> * p_stdsetp_vocabularyandtranslation 
    = search( //p_tch 
    p_ch , 0, cr_stdstr.length()
    //,LetterNode * & pletternode,
    ) ;
  //is NULL if no vocable with the string was found.
  if( p_stdsetp_vocabularyandtranslation )
  {
    p_stdsetp_vocabularyandtranslation_const_iter = 
      p_stdsetp_vocabularyandtranslation->begin() ;
    (*p_stdsetp_vocabularyandtranslation_const_iter)->
      GetWord(//r_wordEnglish
        r_automdelwordEng ) ;
    return 1 ;
  }
  //return NULL ;
  return 0 ;
}

Word * LetterTree::GetNextOccurance(
  const std::string & cr_stdstr
  , Word ** p_wordEnglish
  , Word ** p_wordGerman
  )
{
  //p_stdsetp_vocabularyandtranslation_const_iter->
  return NULL ;
}

Word * LetterTree::GetPreviousOccurance(
  const std::string & str
  , Word ** p_wordEnglish
  , Word ** p_wordGerman
  ) 
{
  return NULL ;
}

  //"inline makes it faster (->no function call)
  //The word whose last char sp_letternodeLastForInsertedWord points to
  //must have been inserted into the tree previously.

  /** Insert either:
   * -a new VocabularyAndTranslation to the last char of a word or
   * -add a pointer to a VocabularyAndTranslation obj for a word of an
   *  existing */
 inline //void
   VocabularyAndTranslation *
   LetterTree::HandleVocabularyAndTranslationPointerInsertion(
   //This set is to ensure that for identical strings for the SAME vocabulary
   //not 2 or more VocAndTransl object should be inserted into the same
   //LetterNode of the last character.
  std::set<LetterNode *> & stdsetpletternodeLastStringChar
//  , LetterNode * p_letternodeLastForInsertedWord
  //, VocabularyAndTranslation * pvocabularyandtranslation
  , bool bInsertNewVocabularyAndTranslation
  , BYTE byVocabularyType
  )
{
   //TODO do not use a copy here (->faster)
   LetterNode * p_letternodeLastForInsertedWord =
       sp_letternodeLastForInsertedWord ;
  //If a string for the current English-German was inserted before
  //the pointer to the VocabularyAndTranslation should not be again.
  if( //pletternode may be NULL if the character was not inside the map that
    // translates a character to an LetterNode array index.
    p_letternodeLastForInsertedWord //&&
    //pvocabularyandtranslation
    //s_pvocabularyandtranslation
    )
  {
    //If already a word with the same string was inserted into the Trie
    //(e.g. "to love" before and now the noun "the love")
    //m_psetpvocabularyandtranslation is not NULL. Else it must be created.
    if( ! p_letternodeLastForInsertedWord->m_psetpvocabularyandtranslation )
      //{
      //pletternodeCurrent->m_psetvocabularyandtranslation = new
        //std::set<VocabularyAndTranslation>(//0,new Word(),new Word()
      p_letternodeLastForInsertedWord->m_psetpvocabularyandtranslation = new
        std::set<VocabularyAndTranslation *>(//0,new Word(),new Word()
        ) ;
    //if( pletternodeCurrent->m_psetvocabularyandtranslation )

    //If allocating was successful / if exits yet.
    if( p_letternodeLastForInsertedWord->m_psetpvocabularyandtranslation )
    {
      if(bInsertNewVocabularyAndTranslation)
      {
        //TRACE("LetterTree::insert: inserting into voc. set at \"%x\"\n",
          //pletternodeCurrent->m_psetpvocabularyandtranslation ) ;
        //std::pair <std::set<VocabularyAndTranslation>::iterator, bool>
        std::pair <std::set<VocabularyAndTranslation *>::iterator, bool>
          pairisetandbool =
          //pletternodeCurrent->m_psetvocabularyandtranslation->insert(
          p_letternodeLastForInsertedWord->m_psetpvocabularyandtranslation->insert(
            //VocabularyAndTranslation(byVocabularyType //+
            ////bGermanVocabulary * NUMBER_OF_WORD_TYPES
            //)
            new VocabularyAndTranslation(byVocabularyType )
            ) ;
        s_pvocabularyandtranslation =
          ////Pointer to VocabularyAndTranslation object/reference.
          //&
          //VocabularyAndTranslation object/reference.
          *(pairisetandbool.first) ;
        stdsetpletternodeLastStringChar.insert( p_letternodeLastForInsertedWord ) ;
      }
    //}
    else
      if(
        //If the LetterNode-pointer does NOT exist in the std::set yet:
        //If for instance for the verb "love" the simple past
        //"loved" was inserted
        //then for the past participle
        //"loved" that has the same LetterNode pointer
        //there should not be inserted a VocabularyAndTranslation pointer again.
        stdsetpletternodeLastStringChar.find( p_letternodeLastForInsertedWord ) ==
        stdsetpletternodeLastStringChar.end()
      )
      {
        p_letternodeLastForInsertedWord->insert( //pvocabularyandtranslation
          s_pvocabularyandtranslation ) ;
        stdsetpletternodeLastStringChar.insert( p_letternodeLastForInsertedWord ) ;
      }
    }
//    stdsetpletternodeLastStringChar.insert( pletternodeCurrent ) ;
  }
//  if( bInsertNewVocabularyAndTranslation )
//  {
//    pletternodeCurrent->insert(pvocabularyandtranslation) ;
//    stdsetpletternodeLastStringChar.insert( pletternodeCurrent ) ;
//  }
  return s_pvocabularyandtranslation;
}

 //Adds words/ strings into a Trie data structure.
 //The last character / Trie node contains a pointer to a
 //VocabularyAndTranslation object where the grammatical attributes like article,
 //3rd person plural present for a German verb etc. are stored.
 //"inline makes it faster (->no function call)
 inline LetterNode * /*unsigned*/ LetterTree::insert(
   const char * pchWordBegin,
   int & lengthOrIndexOf1stInvalidChar //,
 //  bool bInsertNewVocabularyAndTranslation,
 //  LetterNode * & rp_letternodeLastForInsertedWord
 // , BYTE byVocabularyType
   )
 {
   bool bDoNotAddToLetterTree = false ;
   //const char * pchWordBegin = strVocabularyEntry.c_str() ;
//   VocabularyAndTranslation * pvocabularyandtranslation = NULL ;
   LetterNode * pletternodeCurrent = //NULL ;
     m_pletternodeRoot ;
   const char * const pchFirstLetter = pchWordBegin ;
 #ifdef _DEBUG
//   //Returns a pointer to the first occurrence of strSearch in str, or NULL if strSearch does not appear in str.
//   if( strstr(pchWordBegin,"buggery") )
//     start = start ;
 #endif
   //In this loop check that for every character of the string a valid index
   //exists.
   //(the maximum index entspricht the amount of added characters to the map)
   //Else we would address an unallocated  LetterNode (=NULL) and so the program
   //crashes.
   for(unsigned wIndex = 0; wIndex < lengthOrIndexOf1stInvalidChar; ++ wIndex)
   {
     if( MapInputCharacterToLetterNodePointerArrayIndex(
       //byCurrentCharOfName
       * pchWordBegin) //> m_byArrayIndexRespSize
       == 255 )
     {
       //bDoNotAddToLetterTree = true ;
//       std::ostringstream oss;
//       std::string invalidChar;
//       if( * pchWordBegin )
//         invalidChar = * pchWordBegin;
//       else
//         invalidChar = "string end";
//       oss << "ungueltiges Zeichen \"" << invalidChar << "\" im Wort: \""
//         << std::string(pchFirstLetter, lengthOrIndexOf1stInvalidChar) << "\"";
////       std::string strMsg = oss.str();
//       mp_userinterface->Message(//"anderer Buchstabe als Sonderzeichen im Wort"
//         //  strMsg
//         oss.str() ) ;
//       LOGN_ERROR( oss.str() )
       lengthOrIndexOf1stInvalidChar = wIndex;
       sp_letternodeLastForInsertedWord = NULL ;
       return NULL;
         //wIndex;
//       break ;
     }
     pchWordBegin ++ ;
   }
   if( ! bDoNotAddToLetterTree )
   {
     pchWordBegin = pchFirstLetter ;
     //BYTE byRes = 0 ;
     for(WORD wIndex=0; wIndex < lengthOrIndexOf1stInvalidChar; ++wIndex)
     {
       pletternodeCurrent = CreateNodeIfNonExistant(pletternodeCurrent,
         MapInputCharacterToLetterNodePointerArrayIndex(
         //byCurrentCharOfName
         *pchWordBegin)
         );
       //byRes = MapInputCharacterToLetterNodePointerArrayIndex(
       //  //byCurrentCharOfName
     //    *pchWordBegin) ;
     //  if( //If no mapping available
     //    byRes == 255
     //    )
     //    return ;
     //  pletternodeCurrent = CreateNodeIfNonExistant( pletternodeCurrent,
     //    byRes );
       pchWordBegin ++ ;
     }
     //E.g. ist the loop above is iterated "0" times the pointer may be equal
     //to the root node pointer. But the root node pointer must not
     //contain "Vocabularyandtranslation" elements!
     if( pletternodeCurrent == m_pletternodeRoot )
       //pletternodeCurrent = NULL ;
 //      rp_letternodeLastForInsertedWord = NULL ;
       sp_letternodeLastForInsertedWord = NULL ;
     else
     {
       //if(! pletternodeCurrent->m_psetvocabularyandtranslation)

       //}
       //return pletternodeCurrent ;
       //return pletternodeCurrent->m_psetvocabularyandtranslation ;
 //      rp_letternodeLastForInsertedWord = pletternodeCurrent ;
       sp_letternodeLastForInsertedWord = pletternodeCurrent ;
     }
     //TRACE("LetterTree::insert(...) end--pletternode: \"%x\"\n",
     //  pletternode ) ;
   }
   return sp_letternodeLastForInsertedWord ;
 }

 //Returns NULL if no item was added to "m_psetvocabularyandtranslation".
 //LetterNode *

 /** Adds words/ strings into a Trie data structure.
 * The last character / Trie node contains a pointer to a
 * VocabularyAndTranslation object where the grammatical attributes like article,
 * 3rd person plural present for a German verb etc. are stored.
 * "inline makes it faster (->no function call) */
 inline LetterNode * /*unsigned*/ LetterTree::insert(
   const char * pchVocData,
   int start,
   int length //,
 //  bool bInsertNewVocabularyAndTranslation,
 //  LetterNode * & rp_letternodeLastForInsertedWord
 // , BYTE byVocabularyType
   )
 {
   pchVocData += start ;
   return insert(pchVocData, length);
 }

   //void
//   void *
   VocabularyAndTranslation * LetterTree::Insert(const std::string & stdstr,
       BYTE byWordClass )
   {
     bool bInsertNewVocabularyAndTranslation = true ;
//     LetterNode * pletternode ;
     std::set<LetterNode *> stdsetpletternodeLastStringChar ;
 //    insert(
 //      stdstr.c_str() //const char * pch,
 //      , 0 ,
 //      stdstr.length() ,
 //      false ,
 //      pletternode,
 //      byWordClass
 //      ) ;
     InsertIntoTrieAndHandleVocabularyAndTranslation(
       stdsetpletternodeLastStringChar
       //, LetterNode * pletternodeCurrent
       //, VocabularyAndTranslation * pvocabularyandtranslation
       , bInsertNewVocabularyAndTranslation
       , byWordClass //BYTE byVocabularyType
       , stdstr //const std::string & str
       , stdstr.length() // int nLength
       , 0 //int nIndexOf1stChar
       ) ;
     return //(void *)
       s_pvocabularyandtranslation;
   }

   void
//   VocabularyAndTranslation *
   //void *
   LetterTree::Insert(
       EnglishWord & ew , GermanWord & gw, //void * p_v
       VocabularyAndTranslation * p_vocabularyandtranslation
       )
   {
//     bool bInsertNewVocabularyAndTranslation = true ;
     std::string stdstr ;
     std::set<LetterNode *> stdsetpletternodeLastStringChar ;
     ew.InitGetNextString() ;
//     VocabularyAndTranslation * p_vocabularyandtranslation = //new
////         VocabularyAndTranslation(ew.GetWordClass() );
//       (VocabularyAndTranslation *) p_v;
     unsigned uiIndex = 0;
     while( ew.GetNextString(stdstr) )
     {
//       InsertIntoTrieAndHandleVocabularyAndTranslation(
//         stdsetpletternodeLastStringChar
//         //std::set<LetterNode *> & stdsetpletternodeLastStringChar
//         , bInsertNewVocabularyAndTranslation //bool & bInsertNewVocabularyAndTranslation
//         , ew.GetWordClass()
//         , stdstr
//         , stdstr.length()
//         , 0
//         ) ;
         p_vocabularyandtranslation->m_arstrEnglishWord[uiIndex] = stdstr;
         ++ uiIndex;
     }
     uiIndex = 0;
     while( gw.GetNextString(stdstr) )
     {
#ifdef _DEBUG
         const char * p_ch = stdstr.c_str();
#endif
         p_vocabularyandtranslation->m_arstrGermanWord[uiIndex] = stdstr;
         ++ uiIndex;
     }
//     return (void *) //s_pvocabularyandtranslation;
//         p_vocabularyandtranslation;
   }

   void LetterTree::Insert(const char * c_p_ch, unsigned uiGrammarPartID,
     //void * p_v
     VocabularyAndTranslation * p_vocabularyandtranslationPointTo
     )
   {
//     if( p_v)
     {
//       VocabularyAndTranslation * p_vocabularyandtranslationPointTo =
//           (VocabularyAndTranslation *) p_v;
       LetterNode * p_letternode = insert(
         //std::string(
           c_p_ch //)
         , 0
         , strlen(c_p_ch)
     //    , bInsertNewVocabularyAndTranslation
         ////If not assigned yet within THIS function.
         //! pvocabularyandtranslation
     //    , p_letternodeLastForInsertedWord
     //    , byVocabularyType
         ) ;
       if( p_letternode)
       {
         //Force insertion.
         bool bInsertNewVocabularyAndTranslation = true;
         std::set<LetterNode *> stdsetpletternodeLastStringChar;
         VocabularyAndTranslation * p_vocabularyandtranslationCreated =
           HandleVocabularyAndTranslationPointerInsertion(
           //This set is to ensure that for identical strings for the SAME vocabulary
           //not 2 or more VocAndTransl object should be inserted into the same
           //LetterNode of the last character.
          stdsetpletternodeLastStringChar
        //  , LetterNode * p_letternodeLastForInsertedWord
          //, VocabularyAndTranslation * pvocabularyandtranslation
          , bInsertNewVocabularyAndTranslation
          , (BYTE) uiGrammarPartID
          );
         p_vocabularyandtranslationCreated->PointToAttributeData(
             p_vocabularyandtranslationPointTo);
       }
     }
   }

   void LetterTree::Insert3rdPersonSingularPresentReferringVerbAttributes(
     //This set is to ensure that if strings for the SAME vocabulary
     // not 2 or more VocAndTransl object should be inserted.
     std::set<LetterNode *> & stdsetpletternodeLastStringChar
     //"const" because: The infinitive should not be modified because it may
     //be needed afterwards.
     , const char * const strInfinitive
     , int stringLength
     , BYTE byNumberOfObjectsAllowed
     )
   {
     BYTE byVocType = EnglishVerb::Get3rdPersonVerbWordClass(
       byNumberOfObjectsAllowed);
     if( byVocType )
     {
       bool bDoInsertNewVocabularyAndTranslation = true ;
       VTrans::string_type vtransstr3rdPersSingularPresent =
         //r_stdstrInfinitive ;
         VTrans::string_type(strInfinitive, stringLength);
       EnglishVerb::Get3rdPersonForm( vtransstr3rdPersSingularPresent ) ;

       //Store the current pointer
       VocabularyAndTranslation * p_vocabularyandtranslationEnglishVerb =
         //g_lettertree.
           s_pvocabularyandtranslation ;

       //For parsing (constructing a grammar rule ) "the sheep >>3rd_pers_sing_pres"
       //(if the rule was just "the >>noun<< >>main verb<<", then the
       //  "person index" -> "finite verb form" match could
       // be done after the parse tree was created at first.
       // If using a rule ">>mainVerbAllows0object3rdPersonSingularPresent<<
       //And so "The cars sits" could not be a parse tree by using the rule
       //  "the >>singular<< >>3rd_pers_sing_pres<<"
       // would also be possible parse tree at first.
       //g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
       InsertIntoTrieAndHandleVocabularyAndTranslation(
         stdsetpletternodeLastStringChar
         , bDoInsertNewVocabularyAndTranslation
//         , s_bDoInsertNewVocabularyAndTranslation
         , byVocType
         , vtransstr3rdPersSingularPresent.c_str()
         , //nIndexOfCurrentChar - nIndexOf1stChar
           vtransstr3rdPersSingularPresent.length()
         , 0 //nIndexOf1stChar
         ) ;
       //Set to NULL if "insert()" failed.
       if( //g_lettertree.
           sp_letternodeLastForInsertedWord )
       {
         //refer/ point to the attributes of the "main verb" (saves storage)
         s_pvocabularyandtranslation->PointToAttributeData(
             p_vocabularyandtranslationEnglishVerb);
         //Set back the current pointer to the vocandtransl for the main verb.
         //g_lettertree.
         s_pvocabularyandtranslation =
           p_vocabularyandtranslationEnglishVerb ;
       }
     }
   }

   void LetterTree::Insert3rdPersonSingularPresentReferringVerbAttributes(
     //This set is to ensure that if strings for the SAME vocabulary
     // not 2 or more VocAndTransl object should be inserted.
     std::set<LetterNode *> & std_setpletternodeLastStringChar
     //"const" because: The infinitive should not be modified because it may
     //be needed afterwards.
     , const std::string & r_std_strInfinitive
     , BYTE byNumberOfObjectsAllowed
     )
   {
     Insert3rdPersonSingularPresentReferringVerbAttributes(
      //This set is to ensure that if strings for the SAME vocabulary
      // not 2 or more VocAndTransl object should be inserted.
      std_setpletternodeLastStringChar
      //"const" because: The infinitive should not be modified because it may
      //be needed afterwards.
      , r_std_strInfinitive.c_str()
      , r_std_strInfinitive.length()
      , byNumberOfObjectsAllowed
      );
   }
/** Inserting into the Trie and handling the insertion of a pointer to
* VocabularyAndTranslation often needs to be done in conjunction. So implement
* this conjunction here. */
void LetterTree::InsertIntoTrieAndHandleVocabularyAndTranslation(
  //This set is to ensure that for identical strings for the SAME vocabulary
  //not 2 or more VocAndTransl object should be inserted into the same
  //LetterNode of the last character.
  std::set<LetterNode *> & stdsetpletternodeLastStringChar
  //, LetterNode * pletternodeCurrent
  //, VocabularyAndTranslation * pvocabularyandtranslation
  , bool & bInsertNewVocabularyAndTranslation
  , BYTE byVocabularyType
  , const std::string & str
  , int nLength
  , int nIndexOf1stChar
  )
{
#ifdef _DEBUG
  std::string std_strWord = str.substr(nIndexOf1stChar, nLength);
  LOGN_TYPE( /*FULL_FUNC_NAME <<*/ "token to insert into trie/ letter tree: " <<
    std_strWord, LogLevel::debug)
#endif //#ifdef _DEBUG
//  LetterNode * p_letternodeLastForInsertedWord ;
  //If the singular and the plural are identical: add only once to
  //the "trie" structure/ add only 1 VocabularyAndTranslation to the
  //last LetterNode.
  //pvocabularyandtranslationReturn =
    insert(
    //std::string(
    (LPCSTR) str.c_str() //)
    , nIndexOf1stChar
    , nLength
//    , bInsertNewVocabularyAndTranslation
    ////If not assigned yet within THIS function.
    //! pvocabularyandtranslation
//    , p_letternodeLastForInsertedWord
//    , byVocabularyType
    ) ;
    //Set to NULL if "insert()" failed.
  if( sp_letternodeLastForInsertedWord )
    HandleVocabularyAndTranslationPointerInsertion(
      stdsetpletternodeLastStringChar
  //    , p_letternodeLastForInsertedWord
      //, pvocabularyandtranslation
      , bInsertNewVocabularyAndTranslation
      , byVocabularyType
      ) ;
  //Insert an allocated VocabularyAndTranslation object only ONCE for a
  //vocabulary pair.
  if( bInsertNewVocabularyAndTranslation )
    bInsertNewVocabularyAndTranslation = false ;
}

/** Insert the 1st English word of a vocable.
 * Inserting into the Trie and handling the insertion of a pointer to
* VocabularyAndTranslation often needs to be done in conjunction. So implement
* this conjunction here.
* @return NULL : vocable has not been inserted.*/
/*LetterNode * */
VocabularyAndTranslation *
 LetterTree::InsertIntoTrieAndHandleVocabularyAndTranslation(
  //This set is to ensure that for identical strings for the SAME vocabulary
  //not 2 or more VocAndTransl object should be inserted into the same
  //LetterNode of the last character.
  std::set<LetterNode *> & stdsetpletternodeLastStringChar
  //, LetterNode * pletternodeCurrent
  //, VocabularyAndTranslation * pvocabularyandtranslation
  , bool & bInsertNewVocabularyAndTranslation
  , BYTE byVocabularyType
  , const char * ar_chWordStart
  , int & nLengthOr1stError
  )
{
  m_std_set_p_letternodeLastStringChar.clear();
  VocabularyAndTranslation * p_vocandtranls = NULL;
  LetterNode * p_letternode = insert(
    //std::string(
    ar_chWordStart //)
    , nLengthOr1stError
  //    , bInsertNewVocabularyAndTranslation
    ////If not assigned yet within THIS function.
    //! pvocabularyandtranslation
  //    , p_letternodeLastForInsertedWord
  //    , byVocabularyType
    ) ;
    //Set to NULL if "insert()" failed.
  if( /*sp_letternodeLastForInsertedWord*/ p_letternode )
    p_vocandtranls = HandleVocabularyAndTranslationPointerInsertion(
      stdsetpletternodeLastStringChar
  //    , p_letternodeLastForInsertedWord
      //, pvocabularyandtranslation
      , bInsertNewVocabularyAndTranslation
      , byVocabularyType
      ) ;
  //Insert an allocated VocabularyAndTranslation object only ONCE for a
  //vocabulary pair.
  if( bInsertNewVocabularyAndTranslation )
    bInsertNewVocabularyAndTranslation = false ;
  return /*p_letternode*/ p_vocandtranls;
}

/** Inserting into the Trie and handling the insertion of a pointer to
* VocabularyAndTranslation often needs to be done in conjunction. So implement
* this conjunction here. */
LetterNode * LetterTree::InsertIntoTrieAndHandleVocabularyAndTranslation(
  //This set is to ensure that for identical strings for the SAME vocabulary
  //not 2 or more VocAndTransl object should be inserted into the same
  //LetterNode of the last character.
  std::set<LetterNode *> & stdsetpletternodeLastStringChar
  //, LetterNode * pletternodeCurrent
  //, VocabularyAndTranslation * pvocabularyandtranslation
  , bool & bInsertNewVocabularyAndTranslation
  , BYTE byVocabularyType
  , const char * ar_ch
  , int nLength
  , int nIndexOf1stChar
  )
{
  LetterNode * p_ln = insert(
    //std::string(
    ar_ch //)
    , nIndexOf1stChar
    , nLength
  //    , bInsertNewVocabularyAndTranslation
    ////If not assigned yet within THIS function.
    //! pvocabularyandtranslation
  //    , p_letternodeLastForInsertedWord
  //    , byVocabularyType
    ) ;
    //Set to NULL if "insert()" failed.
  if( sp_letternodeLastForInsertedWord )
    HandleVocabularyAndTranslationPointerInsertion(
      stdsetpletternodeLastStringChar
  //    , p_letternodeLastForInsertedWord
      //, pvocabularyandtranslation
      , bInsertNewVocabularyAndTranslation
      , byVocabularyType
      ) ;
  //Insert an allocated VocabularyAndTranslation object only ONCE for a
  //vocabulary pair.
  if( bInsertNewVocabularyAndTranslation )
    bInsertNewVocabularyAndTranslation = false ;
  return p_ln;
}

void LetterTree::InsertPersonalPronouns()
{
  Insert("I", EnglishWord::personal_pronoun ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _1_st_person_singular ;
  s_pvocabularyandtranslation->m_arstrGermanWord[0] = "ich" ;
  Insert("you", EnglishWord::personal_pronoun ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _2nd_person_singular ;
  s_pvocabularyandtranslation->m_arstrGermanWord[0] = "du" ;
  Insert("he", EnglishWord::personal_pronoun ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _3rd_person_singular ;
  s_pvocabularyandtranslation->m_arstrGermanWord[0] = "er" ;
  Insert("she", EnglishWord::personal_pronoun ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _3rd_person_singular ;
  s_pvocabularyandtranslation->m_arstrGermanWord[0] = "sie" ;
  Insert("it", EnglishWord::personal_pronoun ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _3rd_person_singular ;
  s_pvocabularyandtranslation->m_arstrGermanWord[0] = "es" ;
  Insert("we", EnglishWord::personal_pronoun ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _1st_person_plural ;
  s_pvocabularyandtranslation->m_arstrGermanWord[0] = "wir" ;
  Insert("you", EnglishWord::personal_pronoun ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _2nd_person_plural ;
  s_pvocabularyandtranslation->m_arstrGermanWord[0] = "ihr" ;
  Insert("they", EnglishWord::personal_pronoun ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _3rd_person_plural ;
  s_pvocabularyandtranslation->m_arstrGermanWord[0] = "sie" ;
}

/** Insert into main memory in order to resolve grammar rules. */
void LetterTree::InsertPersonalPronounsObjectiveForm()
{
  Insert("me", EnglishWord::personal_pronoun_objective_form
    //personal_pronoun_me
      ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _1_st_person_singular ;
  Insert("you", EnglishWord::personal_pronoun_objective_form
      //personal_pronoun_objective_you
      ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _2nd_person_singular ;
  Insert("him", EnglishWord::personal_pronoun_objective_form
    //personal_pronoun_him
    ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _3rd_person_singular ;
  Insert("her", EnglishWord::personal_pronoun_objective_form
      //personal_pronoun_her
      ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _3rd_person_singular ;
  Insert("it", EnglishWord::personal_pronoun_objective_form
      //personal_pronoun_objective_it
      ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _3rd_person_singular ;
  Insert("us", EnglishWord::personal_pronoun_objective_form
      //personal_pronoun_us
      ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _1st_person_plural ;
//  Insert("you", EnglishWord::personal_pronoun_objective_form ) ;
//  s_pvocabularyandtranslation->m_arbyAttribute[0] = _2nd_person_plural ;
  Insert("them", EnglishWord::personal_pronoun_objective_form
    //personal_pronoun_them
      ) ;
  s_pvocabularyandtranslation->m_arbyAttribute[0] = _3rd_person_plural ;
}

//Add a new VocabularyAndTranslation object (because of the grammar part ID for
//plural) to the trie node but refer to the
//attribute data of an existing noun to not waste main memory.
void LetterTree::InsertPluralNounReferringNounAttributes(
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> & stdsetpletternodeLastStringChar
  )
{
  VocabularyAndTranslation * p_vocabularyandtranslationEnglishNoun =
    s_pvocabularyandtranslation ;

  //        g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
  //          stdsetpletternodeLastStringChar
  //          //, LetterNode * pletternodeCurrent
  //          //, VocabularyAndTranslation * pvocabularyandtranslation
  //          , bInsertNewVocabularyAndTranslation
  //          , EnglishWord::singular
  //          , strCurrentWordData
  //          , //nIndexOfCurrentChar - nIndexOf1stChar
  //            nLength
  //          , nIndexOf1stChar
  //          ) ;
  //Force insertion of new VocAndTransl(singular) object.
  //For parsing (constructing a grammar rule ) "a >>singular<<"
  //(if the rule was just "a >>noun<<", then
  // "a cars" would also be possible.
  HandleVocabularyAndTranslationPointerInsertion(
    stdsetpletternodeLastStringChar
  //          , p_letternodeLastForInsertedWord
    //, pvocabularyandtranslation
    , s_bDoInsertNewVocabularyAndTranslation
    , EnglishWord::plural_noun
    ) ;
  //object for singular
  //refer/ point to the attributes of the "main verb" (saves storage instead
  // of allocating array for the singular, too)
  s_pvocabularyandtranslation->PointToAttributeData(
    p_vocabularyandtranslationEnglishNoun);

    //This should not be necessary as long as the pointers to the attribute data
    // refer the noun object's arrays.
  //The pointer points to the singular VocAndTransl object. Set it back
  //to the address of the noun object in order to assign the plural etc.
  //correctly.
  s_pvocabularyandtranslation =
    p_vocabularyandtranslationEnglishNoun ;

  //              en->m_strSingular = //str.Mid(start,i-start);
  //                str.substr(start,i-start);
  //If the singular and the plural are identical: add only once to
  //the "trie" structure/ add only 1 VocabularyAndTranslation to the
  //last LetterNode.
//  bInsertNewVocabularyAndTranslation = false ;
}

void LetterTree::InsertIntoTrieAndReferToExistingVocData(
  std::set<LetterNode *> & stdsetpletternodeLastStringChar,
  BYTE byVocType,
  const char * const c_strInsertVocAndTranslObjAt,
  int & nLengthOr1stError
  )
{
  bool bDoInsertNewVocabularyAndTranslation = true ;
  //Store the address of the infinitive object.
  VocabularyAndTranslation * p_vocabularyandtranslationPrevious =
    s_pvocabularyandtranslation ;
  //Also save the verb forms that do not stand within the
  //vocabulary file.
  VocabularyAndTranslation * p_vocandtransl =
  InsertIntoTrieAndHandleVocabularyAndTranslation(
    stdsetpletternodeLastStringChar
    //, LetterNode * pletternodeCurrent
    //, VocabularyAndTranslation * pvocabularyandtranslation
  //      , bInsertNewVocabularyAndTranslation

    //Ensure that a VocAndTransl object is created.
  //      , s_bDoInsertNewVocabularyAndTranslation
    , bDoInsertNewVocabularyAndTranslation
    , //byVocabularyType
    byVocType
    , //strIng
    c_strInsertVocAndTranslObjAt
    , nLengthOr1stError
    ) ;
  if( p_vocandtransl )
  {
    //refer/ point to the attributes of the "main verb" (saves storage instead
    // of allocating array for the singular, too)
    s_pvocabularyandtranslation->PointToAttributeData(
      p_vocabularyandtranslationPrevious);
  }
  //Without this assignment for a plural noun string a singular VocAndTrnsl
  // object existed.
  s_pvocabularyandtranslation =
    p_vocabularyandtranslationPrevious ;
}

void LetterTree::InsertIntoTrieAndReferToExistingVocData(
    std::set<LetterNode *> & stdsetpletternodeLastStringChar,
    BYTE byVocType,
    const std::string & c_r_std_strInsertVocAndTranslObjAt
    )
{
  int strLen = c_r_std_strInsertVocAndTranslObjAt.length();
  InsertIntoTrieAndReferToExistingVocData(
    stdsetpletternodeLastStringChar,
    byVocType,
    c_r_std_strInsertVocAndTranslObjAt.c_str(),
    strLen
    );
}

//Add a new VocabularyAndTranslation object (because of the grammar part ID for
//participle progressive) to the trie node but refer to the
//attribute data of an existing noun to not waste main memory.
void LetterTree::InsertPresentParticipleReferringVerbAttributes(
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> & stdsetpletternodeLastStringChar
  //The infinitive should not be modified because it may be needed afterwards.
  , const std::string & r_stdstrInfinitive
  , BYTE byNumberOfObjectsAllowedByVerb
  )
{
  BYTE byVocType = 0 ;
  switch(byNumberOfObjectsAllowedByVerb)
  {
  case 0 :
    byVocType = EnglishWord::main_verb_allows_0object_progressive_form ;
    break ;
  case 1 :
    byVocType = EnglishWord::main_verb_allows_1object_progressive_form ;
    break ;
  case 2 :
    byVocType = EnglishWord::main_verb_allows_2objects_progressive_form ;
    break ;
  }
  if( byVocType )
  {
    std::string stdstrProgressiveForm = r_stdstrInfinitive ;
    //TODO if last letter = consonant, e.g. "refer": "refeRRing"
  //  std::string strIng = ev->m_strInfinitive + "ing" ;
    EnglishVerb::GetProgressiveForm( stdstrProgressiveForm ) ;

    InsertIntoTrieAndReferToExistingVocData(
      stdsetpletternodeLastStringChar,
      byVocType,
      stdstrProgressiveForm
      );
  }
}

//Add a new VocabularyAndTranslation object (because of the grammar part ID for
//participle progressive) to the trie node but refer to the
//attribute data of an existing noun to not waste main memory.
void LetterTree::InsertPastTenseReferringVerbAttributes(
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> & stdsetpletternodeLastStringChar
  //The infinitive should not be modified because it may be needed afterwards.
  , const std::string & r_std_strPastTense
  , BYTE byNumberOfObjectsAllowedByVerb
  )
{
  BYTE byVocType = 0 ;
  switch(byNumberOfObjectsAllowedByVerb)
  {
  case 0 :
    byVocType = EnglishWord::main_verb_allows_0object_past_form ;
    break ;
  case 1 :
    byVocType = EnglishWord::main_verb_allows_1object_past_form ;
    break ;
  case 2 :
    byVocType = EnglishWord::main_verb_allows_2objects_past_form ;
    break ;
  }
  if( byVocType )
  {
    InsertIntoTrieAndReferToExistingVocData(stdsetpletternodeLastStringChar,
        byVocType, r_std_strPastTense);
  }
}

//Add a new VocabularyAndTranslation object (because of the grammar part ID for
//participle progressive) to the trie node but refer to the
//attribute data of an existing noun to not waste main memory.
void LetterTree::InsertPastParticipleReferringVerbAttributes(
  //This set is to ensure that if strings for the SAME vocabulary
  // not 2 or more VocAndTransl object should be inserted.
  std::set<LetterNode *> & stdsetpletternodeLastStringChar
  //The infinitive should not be modified because it may be needed afterwards.
  , const std::string & r_std_strPastTense
  , BYTE byNumberOfObjectsAllowedByVerb
  )
{
  BYTE byVocType = 0 ;
  switch(byNumberOfObjectsAllowedByVerb)
  {
  case 0 :
    byVocType = EnglishWord::mainVerbPastParticiple0Obj ;
    break ;
  case 1 :
    byVocType = EnglishWord::mainVerbPastParticiple1Obj ;
    break ;
  case 2 :
    byVocType = EnglishWord::mainVerbPastParticiple2Obj ;
    break ;
  }
  if( byVocType )
  {
    InsertIntoTrieAndReferToExistingVocData(stdsetpletternodeLastStringChar,
        byVocType, r_std_strPastTense);
  }
}

void LetterTree::InsertSingularNounReferringNounAttributes(
  //This set is to ensure that for identical strings for the SAME vocabulary
  //not 2 or more VocAndTransl object should be inserted into the same
  //LetterNode of the last character.
  std::set<LetterNode *> & stdsetpletternodeLastStringChar
  )
{
  VocabularyAndTranslation * p_vocabularyandtranslationEnglishNoun =
    s_pvocabularyandtranslation ;

  //        g_lettertree.InsertIntoTrieAndHandleVocabularyAndTranslation(
  //          stdsetpletternodeLastStringChar
  //          //, LetterNode * pletternodeCurrent
  //          //, VocabularyAndTranslation * pvocabularyandtranslation
  //          , bInsertNewVocabularyAndTranslation
  //          , EnglishWord::singular
  //          , strCurrentWordData
  //          , //nIndexOfCurrentChar - nIndexOf1stChar
  //            nLength
  //          , nIndexOf1stChar
  //          ) ;
  //Force insertion of new VocAndTransl(singular) object.
  //For parsing (constructing a grammar rule ) "a >>singular<<"
  //(if the rule was just "a >>noun<<", then
  // "a cars" would also be possible.
  HandleVocabularyAndTranslationPointerInsertion(
    stdsetpletternodeLastStringChar
  //          , p_letternodeLastForInsertedWord
    //, pvocabularyandtranslation
    //Ensure that a VocAndTransl object is created.
    , s_bDoInsertNewVocabularyAndTranslation
    , EnglishWord::singular
    ) ;
  //object for singular
  //refer/ point to the attributes of the "main verb" (saves storage instead
  // of allocating array for the singular, too)
  s_pvocabularyandtranslation->PointToAttributeData(
      p_vocabularyandtranslationEnglishNoun);

  ////This should not be necessary as long as the pointers to the attribute data
  // // refer the noun object's arrays.
  //The pointer points to the singular VocAndTransl object. Set it back
  //to the address of the noun object in order to assign the plural etc.
  //correctly.
  //Without this assignment for a plural noun string a singular VocAndTrnsl
  // object existed.
  s_pvocabularyandtranslation =
    p_vocabularyandtranslationEnglishNoun ;

  //              en->m_strSingular = //str.Mid(start,i-start);
  //                str.substr(start,i-start);
  //If the singular and the plural are identical: add only once to
  //the "trie" structure/ add only 1 VocabularyAndTranslation to the
  //last LetterNode.
//  bInsertNewVocabularyAndTranslation = false ;
}

//!How it works:
//! use an "auxiliary dynamic data structure specifically for iterative 
//! deletion" abbreviated with "ADDSSFID"
//! Begin with the root, add all followers
//! (successors) of the root node to the ADDSSFID (a std::vector, so we do not 
//! have to implement it ourselves). Then delete the predecessor 
//! (also from the ADDSSFID) because all 
//! (pointers to the) successors have benn saved.
//! Continue with the earliest added element that is in the ADDSSFID.
//! So this element then again may (and for example if it's the root node and
//! the name is longer than 1 character, then it has) have 
//! followers(successors).
//! With other words we emulate recursive calls where every possible
//! branch of the tree is traversed beginning with the "leftmost" 
//! branch. 
//Ex.: tree containing "car" and "cars"
//  root
//   |
// AB--Zabc
//        |
//    A..Za..z
//        |
// A..Za..r..z
//        |
// A..Za..s..z
//1st iteration: add all children (1 level lower) of root to vector V-> V[c]
//2nd iteration: 1. begin with the first element of V->with "c".
//               2. add all children (1 level lower) of root to vector V-> V[c,a]
//               3. delete "c"->V[a]
//3rd iteration: 1. begin with the first element of V->with "a".
//               2. add all children (1 level lower) of root to vector V-> V[a,r]
//               3. delete "a"->V[r]
//4th iteration: 1. begin with the first element of V->with "r".
//               2. add all children (1 level lower) of root to vector V-> V[r,s]
//               3. delete "r"->V[s]
//4th iteration: 1. begin with the first element of V->with "s".
//               2. add all children (1 level lower) of root to vector V-> V[s]
//               3. delete "s"
//"Disadvantage": if a node contains very much subnodes (children) 
//that also contain much subnodes (children) the vector holding the nodes to
//check for children gets much elements.
//Advantage: fast, little program code. 
void LetterTree::DeleteCompleteList()
{
  LetterNode * pletternodeCurrent =
	  //The root node is always allocated due to simplification
	  //of operations like inserting a node.
	  m_pletternodeRoot;
//  LOGN_DEBUG( "DeleteCompleteList() begin" )
  //At the beginning of this vector the entries of the lowest 
  //levels are stored.
  //A vector is not the best data structure: it operates best
  //at its end. Use deque instead?
  std::vector<LetterNode *> vecpletternode;
  //Just for making it the processing of the ROOT node 
  //uniform for/to "vecNode.delete(0);"
  vecpletternode.push_back(pletternodeCurrent);
  while(pletternodeCurrent)
  {
  #ifdef _DEBUG
	  //cout<<"pletternodeCurrent:"<<pletternodeCurrent<<"\n";
  #endif
	  //Loop/iterate for every possible slot/character of a (single) level.
	  //Use a data type able to handle values larger than 255 because
	  //the whole values range of a char may be used (if it's a BYTE
	  //and the value is 255 and is added by 1 it gets 0 again).
	  for(unsigned short wIndex=0; wIndex<
		  VALUES_RANGE_FOR_SINGLE_CHARACTER_OF_NAME; ++wIndex)
	  {
  #ifdef _DEBUG
//			cout<<"pletternodeCurrent->m_apletternode1LevelLower["<<
//			wIndex<<"]:"<<
//			pletternodeCurrent->m_apletternode1LevelLower[wIndex]<<endl;
  #endif
		  //If allocated
		  if(pletternodeCurrent->m_arpletternode1LevelLower[wIndex])
		  {
			  vecpletternode.push_back(pletternodeCurrent->
				  m_arpletternode1LevelLower[wIndex]);
  #ifdef _DEBUG
//				cout<<(char)this->MapPersonEntryNodePointerArrayIndexToOutputCharacter(wIndex);
  #endif
		  }
	  }
#ifdef _DEBUG_FREEING_MEM
    //std::string * pstr = pletternodeCurrent->
    //  m_psetpvocabularyandtranslation 
    //  ? (*pletternodeCurrent->m_psetpvocabularyandtranslation->begin() 
    //  )->m_arstrEnglishWord 
    //  : NULL ;
    //if( pstr )
    //  TRACE("Will delete node \"%x\"; set: addr.: %x immediately: %s\n", 
    //    pletternodeCurrent, m_psetpvocabularyandtranslation, pstr->c_str() );
    //else
    //  TRACE("Will delete node \"%x\" with no voc data attached immediately\n", 
    //    pletternodeCurrent);
#endif //#ifdef _DEBUG
	  //All addresses of the elements that (are allocated and)
	  //lay by 1 level deeper
	  //have been stored into the vector, so we can delete their
	  //predecessor.
	  delete pletternodeCurrent;
		
	  //Delete the first element so that we do not use it again
	  //(we could also use an index that describes the first
	  //element to use but we also want to release memory, 
	  //don't we)?
	  vecpletternode.erase(vecpletternode.begin());//  delete(0);
	  if(vecpletternode.size()==0)
		  break;
	  //Iterate through all elements of the level that we just had 
	  pletternodeCurrent = vecpletternode.at(0);
  }//while(pletternodeCurrent)
  //The root node is deleted as a result of the loop
  //The root node SHOULD ALWAYS be allocated due to simplification
  //of operations like inserting a node.
//  m_pletternodeRoot = new LetterNode(0, this);
  m_pletternodeRoot = new LetterNode(m_byArrayIndexRespSize, this) ;
  //DEBUGN("DeleteCompleteList end")
}

LetterTree::~LetterTree()
{
//  LOGN("~LetterTree() begin")
  DeleteCompleteList() ;
//  LOGN("~LetterTree() end")
}


//LetterNode * insert(std::string & strVocabularyEntry,int start,int length) 
//{
//  return insert(strVocabularyEntry.c_str(),start,length) ;
//}

//Returns a pointer to all vocabulary with the word, 
//otherwise NULL if the name doesn't exist.
//@pchCurrentChar points to the first string char at the begin.
//std::set<VocabularyAndTranslation> * LetterTree::search(
std::set<VocabularyAndTranslation *> * LetterTree::search(
  const char * pchCurrentChar,
  int start,
  int length
  //,LetterNode * & pletternode,
  )
{
  //const char * pch = strVocabularyEntry.c_str() ;
  //std::set<VocabularyAndTranslation> * psetvocabularyandtranslation = NULL ;
  std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation = NULL ;
  LetterNode * pletternodeCurrent = //NULL ;
    m_pletternodeRoot ;
  pchCurrentChar += start ;
  for(WORD wIndex=0; wIndex < length; ++wIndex)
  {
    if(//If the current letter node has children / pointer is NOT NULL.
      (pletternodeCurrent = pletternodeCurrent->m_arpletternode1LevelLower[
      MapInputCharacterToLetterNodePointerArrayIndex(
        *pchCurrentChar) ] )
      )
      //Forward/go to the string's next char.
      pchCurrentChar ++ ;
    else
      break ;
  }
  //=If the name exists.
  if(pletternodeCurrent )
    //psetvocabularyandtranslation = pletternodeCurrent->m_psetvocabularyandtranslation ;
    psetpvocabularyandtranslation = pletternodeCurrent->
      m_psetpvocabularyandtranslation ;
  //return psetvocabularyandtranslation ;
  return psetpvocabularyandtranslation ;
}

//Searches for the vocabularies with most identical tokens 
//in psv beginning at index.
//Advantage: if at first token "vacuum" and second "cleaner",
//vocabulary entry for "vacuum cleaner" is returned and index
//is set to the last token of the vocabulary within psv.
//Returns a pointer to all vocabulary with the word, 
//otherwise NULL if the name doesn't exist.
//@return LetterNode of last char of string within LetterTree identical to psv 
//in most tokens.
LetterNode * LetterTree::searchAndReturnLetterNode(
  //Vector that contains the tokens of a text / a sentence.
  const PositionStringVector & psv,
//  const PositionstdstringVector & psv,
  //The index of the first token to include in the trie search.
  DWORD & r_dwTokenIndex
  //,LetterNode * & pletternode,
  )
{
//  #ifdef _DEBUG
  BYTE byNodePointerArrayIndex ;
//  #endif
  const char * pchCurrentChar ;
  //const char * pch = strVocabularyEntry.c_str() ;
  //std::set<VocabularyAndTranslation> * psetvocabularyandtranslation = NULL ;
//  std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation = NULL ;
  LetterNode * pletternodeCurrent = //NULL ;
    m_pletternodeRoot;
  LetterNode * pletternodeCurrentWithMostTokens = NULL ;
  WORD wVectorSize = psv.size() ;
  WORD wTokenLength ;
  //WORD wTokenIndex = 0 ;
  WORD wTokenIndex = r_dwTokenIndex ;
  //pchCurrentChar += start ;
  for( //r_dwTokenIndex
    ; wTokenIndex < //psv.size() ;
    wVectorSize ;
    ++ wTokenIndex )
  {
    //pchCurrentChar = psv.at(wTokenIndex).m_Str ;
    pchCurrentChar = psv.at(wTokenIndex).m_Str.c_str() ;
    wTokenLength = strlen(pchCurrentChar) ;
    for(WORD wCurrentTokenCharIndex = 0; wCurrentTokenCharIndex < wTokenLength;
      ++ wCurrentTokenCharIndex)
    {
      //#ifdef _DEBUG
      byNodePointerArrayIndex = MapInputCharacterToLetterNodePointerArrayIndex(
        * pchCurrentChar) ;
      LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "mapped index for character \""
        << * pchCurrentChar << "\" is :" << (WORD) byNodePointerArrayIndex)
      //#endif
      if( //If array index for a contained character (not contained == 255).
        byNodePointerArrayIndex != 255 &&
          //If the current letter node has children / pointer is NOT NULL.
        (pletternodeCurrent = pletternodeCurrent->m_arpletternode1LevelLower[
        byNodePointerArrayIndex ] )
        )
        //Forward/go to the string's next char.
        pchCurrentChar ++ ;
      else
      {
        pletternodeCurrent = NULL;
        break ;
      }
    }
    LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "pletternodeCurrent: " << pletternodeCurrent)
    //= If the token(s) are inside the dictionary.
    if(pletternodeCurrent
      //else: returns last letter of "liable" from voc. "not liable for tax"
      //for input text "Tril is not liable".
      && pletternodeCurrent->m_psetpvocabularyandtranslation )
    {
      pletternodeCurrentWithMostTokens = pletternodeCurrent ;
      //All chars of the prev token match -> update rightmost token index.
      r_dwTokenIndex = wTokenIndex ;
      //If no space in lettertree after token.
      if( ! (pletternodeCurrent = pletternodeCurrent->m_arpletternode1LevelLower[
        MapInputCharacterToLetterNodePointerArrayIndex(
          //Separates 2 tokens within lettertree.
          ' ') ] )
        )
        break ;
    }
    else
      break ;
  }
//  //=If the name exists.
//  if(pletternodeCurrentWithMostTokens )
//  {
//    //psetvocabularyandtranslation = pletternodeCurrent->m_psetvocabularyandtranslation ;
////    psetpvocabularyandtranslation = pletternodeCurrentWithMostTokens->
////      m_psetpvocabularyandtranslation ;
//  }
  //return psetvocabularyandtranslation ;
  return pletternodeCurrentWithMostTokens ;
}

//Searches for the vocabularies with most identical tokens 
//in psv beginning at index.
//Advantage: if at first token "vacuum" and second "cleaner",
//vocabulary entry for "vacuum cleaner" is returned and index
//is set to the last token of the vocabulary within psv.
//Returns a pointer to all vocabulary with the word, 
//otherwise NULL if the name doesn't exist.
//@return LetterNode of last char of string within LetterTree identical to psv 
//in most tokens.
LetterNode * LetterTree::searchAndReturnLetterNode(
  //const PositionStringVector & psv, 
  //const 
  std::istream & istreamEnglish,
  DWORD & r_dwTokenIndex
  //,LetterNode * & pletternode,
  )
{
  bool bSearchedForSpaceChar = false ;
  bool bSeperatorOccured = false ;
  //const char chCurrentChar ;
  char chCurrentChar ;
//  DWORD dwTokenIndex = r_dwTokenIndex ;
  //const char * pch = strVocabularyEntry.c_str() ;
  //std::set<VocabularyAndTranslation> * psetvocabularyandtranslation = NULL ;
//  std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation = NULL ;
  LetterNode * pletternodeCurrent = //NULL ;
    m_pletternodeRoot;
  LetterNode * pletternodeCurrentWithMostTokens = NULL ;
  WORD //wTokenLength, 
    wArrayIndex ;
  //pchCurrentChar += start ;
  while ( ! istreamEnglish.eof() )
  {
    //"istreamEnglish >> chCurrentChar ;" did not work.
    //cin >> chCurrentChar ;
    //istreamEnglish.get(chCurrentChar);
    chCurrentChar = (char) istreamEnglish.get() ;
    switch(chCurrentChar)
    {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      if( 
        //If previous char was not a separator (-> last letter of previous token)
        ! bSeperatorOccured
        && pletternodeCurrent && pletternodeCurrent->
        m_psetpvocabularyandtranslation )
      {
        pletternodeCurrentWithMostTokens = pletternodeCurrent ;
        ++ r_dwTokenIndex ;
      }

      bSeperatorOccured = true ;
      if( !bSearchedForSpaceChar)
        //If another separator char then space char 
        //(e.g. "vacuum\ncleaner" also search for space char.
        chCurrentChar = ' ' ;
      break ;
    default:
      if( bSeperatorOccured )
      {
        bSeperatorOccured = false ;
        //++ dwTokenIndex ;
      }
      bSearchedForSpaceChar = false ;
    }

    if( //bSeperatorOccured && chCurrentChar == ' ' || ! bSeperatorOccured
      ! bSearchedForSpaceChar || ! bSeperatorOccured )
    {
      wArrayIndex = MapInputCharacterToLetterNodePointerArrayIndex(
        chCurrentChar) ;
      if( 
        //If array index for a contained character (not contained == 255).
        wArrayIndex != 255 &&
        //If the current letter node has children / pointer is NOT NULL.
        (pletternodeCurrent = pletternodeCurrent->m_arpletternode1LevelLower[
        wArrayIndex ] )
        )
        //if(pletternodeCurrent->m_psetpvocabularyandtranslation && 
        //  bSeperatorCharOccured)
        //  ++ //TODOr_dwTokenIndex ;
        ;
      else
        break ;
      bSearchedForSpaceChar = true ;
    }
  }
  ////=If the name exists.
  //if(pletternodeCurrentWithMostTokens )
  //{
  //  //psetvocabularyandtranslation = pletternodeCurrent->m_psetvocabularyandtranslation ;
  //  psetpvocabularyandtranslation = pletternodeCurrentWithMostTokens->
  //    m_psetpvocabularyandtranslation ;
  //  r_dwTokenIndex = dwTokenIndex ;
  //}
  //return psetvocabularyandtranslation ;
  return pletternodeCurrentWithMostTokens ;
}

/** Searches for the vocabularies with most identical tokens
* in psv beginning at index.
* Advantage: if at first token "vacuum" and second "cleaner", vocabulary entry
* for "vacuum cleaner" is returned and index is set to the last token of the
* vocabulary within psv.
* @return a pointer to all vocabulary with the word,
* otherwise NULL if the name doesn't exist.
* @param pchCurrentChar points to the first string char at the begin. */
//std::set<VocabularyAndTranslation> * LetterTree::search(
std::set<VocabularyAndTranslation *> * LetterTree::search(
  const PositionStringVector & psv,
//  const PositionstdstringVector & psv,
  DWORD & r_dwTokenIndex
  //,LetterNode * & pletternode,
  )
{
  return searchAndReturnLetterNode( psv, r_dwTokenIndex )->
    m_psetpvocabularyandtranslation ;
}
