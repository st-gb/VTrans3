//Ohne das folgende include/wenn Pfad in den Eigenschaften dieser Datei 
//unter C/C++->vorkompilierte Header->"PHC durch..." falsch ist: 
//"fatal error C1010: Unerwartetes Dateiende während der Suche nach dem vorkompilierten Header.[...]"
#include "../../StdAfx.h"
#include "LetterTree.hpp"

#ifdef _DEBUG
#define _DEBUG_FREEING_MEM
#endif

  //The sense of mapping is to allow the array of the direct child of a
  //node to be have less than 255 elements.
  void LetterTree::createMapping()
  {
    //Initially set all array values to "255".
    ::memset(m_arbyCharValueToArrayIndex,255,MAPPING_ARRAY_SIZE) ;
    //m_arbyCharValueToArrayIndex = new BYTE[255 ] ;
    addToCharValueToArrayIndexMapping(' ') ;
    addToCharValueToArrayIndexMapping('.') ; //e.g. "Allrad..."
    addToCharValueToArrayIndexMapping(':') ; //e.g. "Edition: Unpublished"
    //e.g. investor's confidence (Anlegervertrauen)
    addToCharValueToArrayIndexMapping('\'') ; // '
    addToCharValueToArrayIndexMapping('/') ; // / "water/steam"
    addToCharValueToArrayIndexMapping('\"') ; // "
    //addToCharValueToArrayIndexMapping('(') ; 
    //addToCharValueToArrayIndexMapping(')') ; 
    addToCharValueToArrayIndexMapping('0','9') ; // (e.g. "4x4 drive")
    addToCharValueToArrayIndexMapping('A','Z') ;
    addToCharValueToArrayIndexMapping('a','z') ;
    addToCharValueToArrayIndexMapping('é') ; //Aprés Ski
    addToCharValueToArrayIndexMapping(228) ; //"ae" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(246) ; //"oe" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(252) ; //"ue" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(196) ; //"Ae" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(214) ; //"Oe" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(220) ; //"Ue" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(223) ; //"sz" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping('-') ; //für "passers-by" etc.
    //mappingToArray();
  }

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
	  cout<<"NODE ALREADY THERE ppletternodeToCreateIfNonExistant:"<<
		  ppletternodeToCreateIfNonExistant;
	  cout<<" *pletternodeToCreateIfNonExistant:"<<
		  *ppletternodeToCreateIfNonExistant<<"\n";
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


//Returns NULL if no item was added to "m_psetvocabularyandtranslation".
//LetterNode * 

//inline 
VocabularyAndTranslation * LetterTree::insert(
  const char * pch,
  int start,
  int length,
  bool bInsertNewVocabularyAndTranslation, 
  LetterNode * & pletternode,
  BYTE byVocabularyType)
{
  bool bDoNotAddToLetterTree = false ;
  //const char * pch = strVocabularyEntry.c_str() ;
  VocabularyAndTranslation * pvocabularyandtranslation = NULL ;
  LetterNode * pletternodeCurrent = //NULL ;
    m_pletternodeRoot ;
  pch += start ;
  const char * const pchFirstLetter = pch ;
#ifdef _DEBUG
  //Returns a pointer to the first occurrence of strSearch in str, or NULL if strSearch does not appear in str.
  if( strstr(pch,"bertr") )
    start = start ;
#endif
  //In this loop check that for every character of the string a valid index exists.
  //(the maximum index entspricht the amount of added characters to the map)
  //Else we would address an unallocated  LetterNode (=NULL) and so the program crashes.
  for(WORD wIndex=0; wIndex < length; ++wIndex)
  {
    if( MapInputCharacterToLetterNodePointerArrayIndex(
		  //byCurrentCharOfName
      *pch) //> m_byArrayIndexRespSize 
      == 255 )
    {
      //bDoNotAddToLetterTree = true ;
      std::string strMsg = std::string("ungueltiges Zeichen im Wort: \"") 
        + std::string(pchFirstLetter) + std::string("\"");
      //mp_userinterface->Message(//"anderer Buchstabe als Sonderzeichen im Wort"
      //  strMsg ) ;
      return NULL ;
      break ;
    }
    pch ++ ;
  }
  if( ! bDoNotAddToLetterTree )
  {
    pch = pchFirstLetter ;
    //BYTE byRes = 0 ;
    for(WORD wIndex=0; wIndex < length; ++wIndex)
    {
      pletternodeCurrent = CreateNodeIfNonExistant(pletternodeCurrent,
		    MapInputCharacterToLetterNodePointerArrayIndex(
		    //byCurrentCharOfName
        *pch)
        );
		  //byRes = MapInputCharacterToLetterNodePointerArrayIndex(
		  //  //byCurrentCharOfName
    //    *pch) ;
    //  if( //If no mapping available
    //    byRes == 255 
    //    )
    //    return ;
    //  pletternodeCurrent = CreateNodeIfNonExistant( pletternodeCurrent,
    //    byRes );
      pch ++ ;
    }
    //E.g. ist the loop above is terated "0" times the pointer may be equal
    //to the root node pointer. But the root node pointer must not
    //contain "Vocabularyandtranslation" elments!
    if( pletternodeCurrent == m_pletternodeRoot )
      //pletternodeCurrent = NULL ;
      pletternode = NULL ;
    else
    {
    //if(! pletternodeCurrent->m_psetvocabularyandtranslation)
    if(! pletternodeCurrent->m_psetpvocabularyandtranslation)
    //{
      //pletternodeCurrent->m_psetvocabularyandtranslation = new 
        //std::set<VocabularyAndTranslation>(//0,new Word(),new Word() 
      pletternodeCurrent->m_psetpvocabularyandtranslation = new 
        std::set<VocabularyAndTranslation *>(//0,new Word(),new Word() 
        ) ;
      //If allocating was successfull / if exits yet.
      //if( pletternodeCurrent->m_psetvocabularyandtranslation )
      if( pletternodeCurrent->m_psetpvocabularyandtranslation )
      {        
        if(bInsertNewVocabularyAndTranslation)
        {
          //TRACE("LetterTree::insert: inserting into voc. set at \"%x\"\n", 
            //pletternodeCurrent->m_psetpvocabularyandtranslation ) ;
          //std::pair <std::set<VocabularyAndTranslation>::iterator, bool> 
          std::pair <std::set<VocabularyAndTranslation *>::iterator, bool> 
            pairisetandbool =
            //pletternodeCurrent->m_psetvocabularyandtranslation->insert(
            pletternodeCurrent->m_psetpvocabularyandtranslation->insert(
              //VocabularyAndTranslation(byVocabularyType //+
              ////bGermanVocabulary * NUMBER_OF_WORD_TYPES 
              //) 
              new VocabularyAndTranslation(byVocabularyType ) 
              ) ;
          pvocabularyandtranslation = 
            ////Pointer to VocabularyAndTranslation object/reference.
            //& 
            //VocabularyAndTranslation object/reference.
            *(pairisetandbool.first) ;
        }
      }
      //}
      //return pletternodeCurrent ;
      //return pletternodeCurrent->m_psetvocabularyandtranslation ;
      pletternode = pletternodeCurrent ;
    }
    //TRACE("LetterTree::insert(...) end--pletternode: \"%x\"\n", 
    //  pletternode ) ;
  }
  return pvocabularyandtranslation ;
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
  #ifdef _DEBUG
	  cout << "DeleteCompleteList() begin\n";
  #endif
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
  m_pletternodeRoot = new LetterNode(0, this);
}

LetterTree::~LetterTree()
{
  DeleteCompleteList() ;
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
  //const PositionCStringVector & psv, 
  const PositionstdstringVector & psv, 
  DWORD & r_dwTokenIndex
  //,LetterNode * & pletternode,
  )
{
  const char * pchCurrentChar ;
  //const char * pch = strVocabularyEntry.c_str() ;
  //std::set<VocabularyAndTranslation> * psetvocabularyandtranslation = NULL ;
  std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation = NULL ;
  LetterNode * pletternodeCurrent = //NULL ;
    m_pletternodeRoot;
  LetterNode * pletternodeCurrentWithMostTokens = NULL ;
  WORD wTokenLength ;
  //pchCurrentChar += start ;
  for(WORD wTokenIndex = r_dwTokenIndex; wTokenIndex < psv.size(); 
    ++ wTokenIndex )
  {
    //pchCurrentChar = psv.at(wTokenIndex).m_Str ;
    pchCurrentChar = psv.at(wTokenIndex).m_Str.c_str() ;
    wTokenLength = strlen(pchCurrentChar) ;
    for(WORD wCurrentTokenCharIndex=0; wCurrentTokenCharIndex< wTokenLength ; 
      ++wCurrentTokenCharIndex)
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
    //= If the token(s) is at least 1 vocabulary
    if(pletternodeCurrent )
    {
      pletternodeCurrentWithMostTokens = pletternodeCurrent ;
      //If no space in lettertree after token.
      if( ! (pletternodeCurrent = pletternodeCurrent->m_arpletternode1LevelLower[
        MapInputCharacterToLetterNodePointerArrayIndex(
          //seperates 2 tokens within lettertree.
          ' ') ] )
        )
        break ;
    }
    else
      break ;
  }
  //=If the name exists.
  if(pletternodeCurrentWithMostTokens )
  {
    //psetvocabularyandtranslation = pletternodeCurrent->m_psetvocabularyandtranslation ;
    psetpvocabularyandtranslation = pletternodeCurrentWithMostTokens->
      m_psetpvocabularyandtranslation ;
    r_dwTokenIndex = wTokenIndex ;
  }
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
  //const PositionCStringVector & psv, 
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
  DWORD dwTokenIndex = r_dwTokenIndex ;
  //const char * pch = strVocabularyEntry.c_str() ;
  //std::set<VocabularyAndTranslation> * psetvocabularyandtranslation = NULL ;
  std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation = NULL ;
  LetterNode * pletternodeCurrent = //NULL ;
    m_pletternodeRoot;
  LetterNode * pletternodeCurrentWithMostTokens = NULL ;
  WORD //wTokenLength, 
    wArrayIndex ;
  //pchCurrentChar += start ;
  while (!istreamEnglish.eof() )  
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
        !bSeperatorOccured 
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
      !bSearchedForSpaceChar || ! bSeperatorOccured )
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

//Searches for the vocabularies with most identical tokens 
//in psv beginning at index.
//Advantage: if at first token "vacuum" and second "cleaner",
//vocabulary entry for "vacuum cleaner" is returned and index
//is set to the last token of the vocabulary within psv.
//Returns a pointer to all vocabulary with the word, 
//otherwise NULL if the name doesn't exist.
//@pchCurrentChar points to the first string char at the begin.
//std::set<VocabularyAndTranslation> * LetterTree::search(
std::set<VocabularyAndTranslation *> * LetterTree::search(
  //const PositionCStringVector & psv, 
  const PositionstdstringVector & psv, 
  DWORD & r_dwTokenIndex
  //,LetterNode * & pletternode,
  )
{
  //const char * pchCurrentChar ;
  ////const char * pch = strVocabularyEntry.c_str() ;
  ////std::set<VocabularyAndTranslation> * psetvocabularyandtranslation = NULL ;
  //std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation = NULL ;
  //LetterNode * pletternodeCurrent = //NULL ;
  //  m_pletternodeRoot;
  //LetterNode * pletternodeCurrentWithMostTokens = NULL ;
  //WORD wTokenLength ;
  ////pchCurrentChar += start ;
  //for(WORD wTokenIndex = r_dwTokenIndex; wTokenIndex < psv.size(); 
  //  ++ wTokenIndex )
  //{
  //  pchCurrentChar = psv.at(wTokenIndex).m_Str ;
  //  wTokenLength = strlen(pchCurrentChar) ;
  //  for(WORD wCurrentTokenCharIndex=0; wCurrentTokenCharIndex< wTokenLength ; 
  //    ++wCurrentTokenCharIndex)
  //  {
  //    if(//If the current letter node has children / pointer is NOT NULL.
  //      (pletternodeCurrent = pletternodeCurrent->m_arpletternode1LevelLower[
  //      MapInputCharacterToLetterNodePointerArrayIndex(
  //        *pchCurrentChar) ] )
  //      )
  //      //Forward/go to the string's next char.
  //      pchCurrentChar ++ ;
  //    else
  //      break ;
  //  }
  //  //= If the token(s) is at least 1 vocabulary
  //  if(pletternodeCurrent )
  //  {
  //    pletternodeCurrentWithMostTokens = pletternodeCurrent ;
  //    //If no space in lettertree after token.
  //    if( ! (pletternodeCurrent = pletternodeCurrent->m_arpletternode1LevelLower[
  //      MapInputCharacterToLetterNodePointerArrayIndex(
  //        //seperates 2 tokens within lettertree.
  //        ' ') ] )
  //      )
  //      break ;
  //  }
  //  else
  //    break ;
  //}
  ////=If the name exists.
  //if(pletternodeCurrentWithMostTokens )
  //{
  //  //psetvocabularyandtranslation = pletternodeCurrent->m_psetvocabularyandtranslation ;
  //  psetpvocabularyandtranslation = pletternodeCurrentWithMostTokens->
  //    m_psetpvocabularyandtranslation ;
  //  r_dwTokenIndex = wTokenIndex ;
  //}
  ////return psetvocabularyandtranslation ;
  //return psetpvocabularyandtranslation ;
  return searchAndReturnLetterNode( psv, r_dwTokenIndex )->
    m_psetpvocabularyandtranslation ;
}
