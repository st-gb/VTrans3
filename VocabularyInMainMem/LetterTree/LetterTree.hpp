#ifndef LETTERTREE_H
  #define LETTERTREE_H
  //I checked it too late: the appropriate name for the following data structure 
  //is not a letter tree but "trie", see "http://en.wikipedia.org/wiki/Trie".
  //But I want to lassen the file name so that nobody sees what data structure
  //  this is.
  //from "http://en.wikipedia.org/wiki/Trie#As_replacement_of_other_data_structures":
  //A trie can also be used to replace a hash table, over which it has the
  // following advantages:
  //  * Looking up data in a trie is faster in the worst case, O(m) time, 
  //    compared to an imperfect hash table.
  //    An imperfect hash table can have key collisions. A key collision is
  //    the hash function mapping of 
  //    different keys to the same position in a hash table.
  //    The worst-case lookup speed in an imperfect hash table is O(N) time,
  //    but far more typically is O(1), with O(m) time spent evaluating the hash.
  //  * There are no collisions of different keys in a trie.
  //  * Buckets in a trie which are analogous to hash table buckets that 
  //    store key collisions are only necessary if a single key is associated
  //    with more than one value.
  //  * There is no need to provide a hash function or to change
  //    hash functions as more keys are added to a trie.
  //  * A trie can provide an alphabetical ordering of the entries by key.

  
  #include "../../IO.h" //for NUMBER_OF_WORD_TYPES
  #include <string>
  //#include <map>
  #include <set>
  //for GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated,
  // PointerToWordNodeWhereAnEnglishNounIsEncapsulated
  #include "../../GetAndTestWord.h"
  #include "../../Token.h" //for PositionCStringVector
  #include "../../I_UserInterface.hpp"
  #include "VocabularyInMainMem/DoublyLinkedList/WordNode.hpp"
  #include "VocabularyAndTranslation.hpp"
  #include "LetterNode.hpp"
  #include "../IVocabularyInMainMem.hpp"
  #include "../I_WordSearch.hpp"
#ifndef _MSC_VER
  #include <TRACE.h>
#endif

  ////Forward declarations are more performant than "#include"s.
  //class Word ;

  //!If you set this value to maybe 60 instead of 255,
  //!you can save a lot of space needed for the array within
  //!a Node element.
  #define VALUES_RANGE_FOR_SINGLE_CHARACTER_OF_NAME 60

  //The first letter ('A') in the ASCII char set has the decimal
  //value 65.
  #define NUMBER_TO_SUBTRACT_FROM_VALUE_OF_CHARACTER_OF_NAME 64

  //#define ENGLISH_NOUN WORD_TYPE_NOUN
  #define GERMAN_NOUN WORD_TYPE_NOUN + NUMBER_OF_WORD_TYPES
  //#define ENGLISH_MAIN_VERB WORD_TYPE_MAIN_VERB
  #define GERMAN_MAIN_VERB WORD_TYPE_VERB + NUMBER_OF_WORD_TYPES

  //#define NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN 2 //2 strings: singular and plural
  //#define NUMBER_OF_STRINGS_FOR_GERMAN_NOUN 2 //2 strings: singular and plural
  //Vollverben:
  //#define NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB 3//inf., simple past, past part.
  //#define NUMBER_OF_STRINGS_FOR_GERMAN_MAIN_VERB 14//inf.+ 6*present+ 6*past+past part.

  #define ARRAY_INDEX_FOR_SINGULAR 0
  #define ARRAY_INDEX_FOR_PLURAL 1

  //#define BIT_POSITION_FOR_TRANSLATION_TYPE 2
  //Declare forward for member of class VocabularyAndTranslation.
  class LetterNode ;
  class Word ;

  static BYTE g_arbyNumberOfEnglishWords [] = {
    NUMBER_OF_STRINGS_FOR_ENGLISH_NOUN ,
    NUMBER_OF_STRINGS_FOR_ENGLISH_MAIN_VERB } ;

  #define MAPPING_ARRAY_SIZE 255

  class LetterTree
    : public I_WordSearch
  {
  private:
    //Needed only for the creation of the char to array index mapping.
    std::vector<CHAR> m_vecch ;
    BYTE m_byArrayIndexRespSize ;
    I_UserInterface * mp_userinterface ;
    std::set<VocabularyAndTranslation *>::const_iterator 
      p_stdsetp_vocabularyandtranslation_const_iter ;
  public:
    LetterNode * //pnodeCurrent ; //= m_pletternodeRoot;
      m_pletternodeRoot ;
    BYTE m_pbyMappingTableFromCharacterToLetterNodePointerArrayIndex[
      VALUES_RANGE_FOR_SINGLE_CHARACTER_OF_NAME ] ;

    //BYTE * m_arbyCharValueToArrayIndex ;
    BYTE m_arbyCharValueToArrayIndex[MAPPING_ARRAY_SIZE] ;

    std::set<VocabularyAndTranslation *>
      m_setpvocabularyandtranslationDeletedYet ;
    //void mappingToArray()
    //{
    //  m_arbyCharValueToArrayIndex = new BYTE[m_vecch.size() ] ;
    //  int nSize = m_vecch.size() ;
    //  if( nSize < 256 )
    //    for(BYTE byIndex = 0 ; byIndex < nSize ; ++ byIndex ) ;
    //      m_arbyCharValueToArrayIndex[ byIndex ] = m_vecch[ byIndex ] ;
    //}

    void addToCharValueToArrayIndexMapping(//CHAR
      BYTE ch)
    {
      //m_vecch.push_back(ch);
      //Sanity check.
      if(ch < 255 )
        m_arbyCharValueToArrayIndex[ch] = m_byArrayIndexRespSize ++ ;
    }

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
    ) ;

  //BYTE 
    Word * GetNextOccurance(
      const std::string & str
      , Word ** p_wordEnglish
      , Word ** p_wordGerman
      ) ;

    Word * GetPreviousOccurance(
      const std::string & str
      , Word ** p_wordEnglish
      , Word ** p_wordGerman
      ) ;

    void addToCharValueToArrayIndexMapping(//CHAR
      BYTE chFrom, //CHAR
      BYTE chTo )
    {
      for(CHAR chIndex = chFrom ; chIndex <= chTo ; ++ chIndex )
        //m_vecch.push_back(ch);
        //Sanity check.
        if(chIndex < 255 )
          m_arbyCharValueToArrayIndex[chIndex] = m_byArrayIndexRespSize ++ ;
    }

    //The sense of mapping is to allow the array of the direct child of a
    //node to be have less than 255 elements.
    void LetterTree::createMapping() ;

    LetterTree(//I_UserInterface * p_userinterface
      )
      //Initializations .
      : m_byArrayIndexRespSize(0)
    {
      //createMapping() must be called first because it sets / determines the
      //the mapping array size.
      createMapping() ;
      //mp_userinterface = p_userinterface ;
      m_pletternodeRoot = new LetterNode(m_byArrayIndexRespSize, this) ;
    }

    void SetUserInterface( I_UserInterface * p_userinterface )
    {
      mp_userinterface = p_userinterface ;
    }

    void DeleteCompleteList() ;

    ~LetterTree() ;

    //Make faster by inline
    inline LetterNode * CreateNodeIfNonExistant(
		  LetterNode *
		  pletternode1LevelAboveNodeToCreateIfNonExistantAndGoneInto,
		  BYTE byIndex) ;

    //Make faster by inline
    //inline 
    VocabularyAndTranslation * insert(
      const char * pch,
      int start,
      int length,
      bool bInsertNewVocabularyAndTranslation,
      LetterNode * & pletternode,
      BYTE byVocabularyType) ;

    //std::set<VocabularyAndTranslation> * search(
    std::set<VocabularyAndTranslation *> * search(
      const char * pchCurrentChar,
      int start,
      int length) ;

    //Make faster by inline
    inline BYTE MapInputCharacterToLetterNodePointerArrayIndex(
	    unsigned char byCharValue)
    {
	    //BYTE byTranslated;
	    //return m_pbyMappingTableFromCharacterToLetterNodePointerArrayIndex[
		   // //Indices must be (of) unsigned (data type).
		   // byOriginal];//byTranslated
      //Sanity checks.
      //if( byOriginal >= NUMBER_TO_SUBTRACT_FROM_VALUE_OF_CHARACTER_OF_NAME &&
      //  byOriginal < NUMBER_TO_SUBTRACT_FROM_VALUE_OF_CHARACTER_OF_NAME +
      //  VALUES_RANGE_FOR_SINGLE_CHARACTER_OF_NAME )
      //  return byOriginal - NUMBER_TO_SUBTRACT_FROM_VALUE_OF_CHARACTER_OF_NAME ;
      //return 255 ;
  #ifdef _DEBUG
      //TRACE("Array index for character \"%c\" is: %u\n",
      //  byCharValue, m_arbyCharValueToArrayIndex[byCharValue] ) ;
      //TRACE("Letter: \"%c\"", byCharValue ) ;
  #endif
      return m_arbyCharValueToArrayIndex[byCharValue] ;
    }

    #define _DEBUG_DETLEVEL_STEFAN

    //std::set<VocabularyAndTranslation> * search(const char * pchCurrentChar)
    std::set<VocabularyAndTranslation *> * search(const char * pchCurrentChar)
    {
      return search(pchCurrentChar,0,strlen(pchCurrentChar) ) ;
    }

    ////Searches for the vocabularies with most identical tokens 
    ////in psv beginning at index.
    ////Advantage: if at first token "vacuum" and second "cleaner",
    ////vocabulary entry for "vacuum cleaner" is returned and index
    ////is set to the last token of the vocabulary within psv.
    //std::set<VocabularyAndTranslation *> * search(
    //  const PositionCStringVector & psv, 
    //  DWORD & r_dwIndex) ;
    //{
    //  return search(pchCurrentChar,0,strlen(pchCurrentChar) ) ;
    //}

    std::set<VocabularyAndTranslation *> * search(
      const PositionstdstringVector & psv, 
      DWORD & r_dwTokenIndex);

    LetterNode * searchAndReturnLetterNode(
      //const 
        std::istream & istreamEnglish, 
      DWORD & r_dwTokenIndex);

    //Should search including variations: if e.g. verb "walking" is searched:
    //if a full verb is found: add "ing" to the infinitive->if found....
    LetterNode * searchIncludingInflections(const char * pchCurrentChar)
    {
      LetterNode * pletternodeCurrent = //NULL ;
        m_pletternodeRoot ;
      WORD wStringLength = strlen(pchCurrentChar) ;
      for(WORD wIndex=0; wIndex < wStringLength; ++wIndex)
      {
        if(//If the current letter node has children / pointer is NOT NULL.
          (pletternodeCurrent = pletternodeCurrent->m_arpletternode1LevelLower[
          MapInputCharacterToLetterNodePointerArrayIndex(
            *pchCurrentChar) ] )
          )
          //Forward/go to the string's next char.
          pchCurrentChar ++ ;
      }
      return pletternodeCurrent ;
    }

    //@returns: NULL: does not contain word class specified by parameter
    std::set<VocabularyAndTranslation *> * ContainsWordClass(
      BYTE byWordClass,
      std::set<VocabularyAndTranslation *> * 
        psetpvocabularyandtranslation
        )
    {
      std::set<VocabularyAndTranslation *> * 
        psetpvocabularyandtranslationReturn = NULL ;

      //Loop for all vocabularies having at least 1 word that 
      //equals the current token.
      for(std::set<VocabularyAndTranslation *>::iterator iter =
        psetpvocabularyandtranslation->begin() ; iter != 
        psetpvocabularyandtranslation->end() ; iter ++ )
      {
        //TODO there may be more possibilities (e.g. if spelling for a noun
        //and verb are identical, e.g. the >>heating<<, I am >>heating<<.
        if((*iter)->m_byType == byWordClass )
        {
          psetpvocabularyandtranslationReturn = 
            psetpvocabularyandtranslation ;
        }
      }
      return psetpvocabularyandtranslationReturn ;
    }

    void GetVocabulariesContainingWordClass(
      BYTE byWordClass,
      std::set<VocabularyAndTranslation *> * 
        psetpvocabularyandtranslation,
      std::set<VocabularyAndTranslation *> & 
        r_setpvocabularyandtranslation
        )
    {
      //Loop for all vocabularies having at least 1 word that 
      //equals the current token.
      for(std::set<VocabularyAndTranslation *>::iterator iter =
        psetpvocabularyandtranslation->begin() ; iter != 
        psetpvocabularyandtranslation->end() ; iter ++ )
      {
        //TODO there may be more possibilities (e.g. if spelling for a noun
        //and verb are identical, e.g. the >>heating<<, I am >>heating<<.
        if((*iter)->m_byType == byWordClass )
        {
          r_setpvocabularyandtranslation.insert(*iter);
        }
      }
    }

    LetterNode * searchAndReturnLetterNode(
      //const PositionCStringVector & psv, 
      const PositionstdstringVector & psv, 
      DWORD & r_dwTokenIndex
      );

    //Called by a GLR (created by bison parser generator; similar to yacc) 
    //lexer.
    //std::set<VocabularyAndTranslation *> * 
    bool IsSingular(
      //const PositionCStringVector & psv
      const PositionstdstringVector & psv ,
      DWORD & r_dwTokenIndex,
      std::set<VocabularyAndTranslation *> & r_setpvocabularyandtranslation
      )
    {
      bool bIsSingular = false ;
      r_setpvocabularyandtranslation.empty() ;
      std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation = 
        NULL ;
      //std::set<VocabularyAndTranslation *> setpvocabularyandtranslation ;
      //psetpvocabularyandtranslation = search( psv, i2 );
      //return ContainsWordClass(WORD_TYPE_NOUN,psetpvocabularyandtranslation) 
        //TODO at this point I merke: It would be intelligent to save schon
        //within a VocabularyAndTranslation for which string of it the 
        //letternode has a reference to this VocabularyAndTranslation.
        //Else I would have to compare the string with the part of the psv
        //(I highly accumulated this would be a waste of computing time).
        //This would mean to store the address of the last letter node 
        //in a member of a VocabularyAndTranslation element?
        //&& issingular(psetpvocabularyandtranslation);

      LetterNode * p_letternode = searchAndReturnLetterNode( psv, 
        r_dwTokenIndex );
      psetpvocabularyandtranslation = p_letternode->
        m_psetpvocabularyandtranslation ;

      //GetVocabulariesContainingWordClass(psetpvocabularyandtranslation,
      //  setpvocabularyandtranslation);
      //Loop for all vocabularies having at least 1 word that 
      //equals the current token.
      for(std::set<VocabularyAndTranslation *>::iterator iter =
        psetpvocabularyandtranslation->begin() ; iter != 
        psetpvocabularyandtranslation->end() ; iter ++ )
      {
        //TODO there may be more possibilities (e.g. if spelling for a noun
        //and verb are identical, e.g. the >>heating<<, I am >>heating<<.
        if((*iter)->m_byType == WORD_TYPE_NOUN && 
          //(*iter)->m_arstrEnglishWord[0] ==  
          (*iter)->m_arpletternodeLastEngChar[ ARRAY_INDEX_FOR_SINGULAR ] 
          == p_letternode 
          )
        {
          r_setpvocabularyandtranslation.insert(*iter);
          bIsSingular = true ;
        }
      }
      return bIsSingular ;
    }//IsSingular(...)

    //Called by a GLR (created by bison parser generator; similar to yacc) 
    //lexer.
    //std::set<VocabularyAndTranslation *> * 
    bool IsPlural(
      //const PositionCStringVector & psv
      const PositionstdstringVector & psv ,
      DWORD & r_dwTokenIndex,
      std::set<VocabularyAndTranslation *> & r_setpvocabularyandtranslation
      )
    {
      bool bIsPlural = false ;
      r_setpvocabularyandtranslation.empty() ;
      std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation = 
        NULL ;
      //std::set<VocabularyAndTranslation *> setpvocabularyandtranslation ;
      //psetpvocabularyandtranslation = search( psv, i2 );
      //return ContainsWordClass(WORD_TYPE_NOUN,psetpvocabularyandtranslation) 
        //TODO at this point I merke: It would be intelligent to save schon
        //within a VocabularyAndTranslation for which string of it the 
        //letternode has a reference to this VocabularyAndTranslation.
        //Else I would have to compare the string with the part of the psv
        //(I highly accumulated this would be a waste of computing time).
        //This would mean to store the address of the last letter node 
        //in a member of a VocabularyAndTranslation element?
        //&& issingular(psetpvocabularyandtranslation);

      LetterNode * p_letternode = searchAndReturnLetterNode( psv, 
        r_dwTokenIndex );
      psetpvocabularyandtranslation = p_letternode->
        m_psetpvocabularyandtranslation ;

      //GetVocabulariesContainingWordClass(psetpvocabularyandtranslation,
      //  setpvocabularyandtranslation);
      //Loop for all vocabularies having at least 1 word that 
      //equals the current token.
      for(std::set<VocabularyAndTranslation *>::iterator iter =
        psetpvocabularyandtranslation->begin() ; iter != 
        psetpvocabularyandtranslation->end() ; iter ++ )
      {
        //TODO there may be more possibilities (e.g. if spelling for a noun
        //and verb are identical, e.g. the >>heating<<, I am >>heating<<.
        if((*iter)->m_byType == WORD_TYPE_NOUN && 
          //(*iter)->m_arstrEnglishWord[0] ==  
          (*iter)->m_arpletternodeLastEngChar[ ARRAY_INDEX_FOR_PLURAL ] 
          == p_letternode 
          )
        {
          r_setpvocabularyandtranslation.insert(*iter);
          bIsPlural = true ;
        }
      }
      return bIsPlural ;
    }//IsPlural(...)

    ////Should search including variations: if e.g. verb "walking" is searched:
    ////if a full verb is found: add "ing" to the infinitive->if found....
    //LetterNode * searchIncludingInflections(const std::string & strWord)
    //{
    //  bool  bAlsoLookForInfinitive = false ;
    //  LetterNode * pletternodeCurrent = //NULL ;
    //    m_pletternodeRoot ;
    //  WORD wStringLength = strWord.size() ;
    //  if(strWord.rfind("ing") )
    //    bAlsoLookForInfinitive = true ;
    //  for(WORD wIndex=0; wIndex < wStringLength; ++wIndex)
    //  {
    //    if(//If the current letter node has children / pointer is NOT NULL.
    //      (pletternodeCurrent = pletternodeCurrent->m_arpletternode1LevelLower[
    //      MapInputCharacterToLetterNodePointerArrayIndex(
    //        *pchCurrentChar) ] )
    //      )
    //    {
    //      if( bAlsoLookForInfinitive )
    //        ;
    //      //Forward/go to the string's next char.
    //      pchCurrentChar ++ ;
    //    }
    //  }
    //  return pletternodeCurrent ;
    //}
  };//class LetterTree

  class EngNounsLetterTree 
    : public IVocabularyInMainMem
  {
  public:
    std::set<VocabularyAndTranslation *> * m_psetpvocandtransl ;
    std::set<VocabularyAndTranslation *>::iterator m_isetpvocandtransl ;

    EngNounsLetterTree()
    {
      m_psetpvocandtransl = new std::set<VocabularyAndTranslation *> ;
      TRACE("EngNounsLetterTree constructor--m_psetpvocandtransl:%x \n",
        m_psetpvocandtransl);
      m_isetpvocandtransl = m_psetpvocandtransl->begin() ;
    }
    ////Copy c'tor.
    //EngNounsLetterTree(const EngNounsLetterTree & t2)
    //{
    //
    //}
    EngNounsLetterTree(//const
      std::set<VocabularyAndTranslation *> &
      rcsetpvocandtransl)
    {
      m_psetpvocandtransl = & rcsetpvocandtransl ;
      m_isetpvocandtransl = rcsetpvocandtransl.begin() ;
    }
    ~ EngNounsLetterTree()
    {
      TRACE("EngNounsLetterTree d'tor--address: %x m_psetpvocandtransl:%x \n",
        this, m_psetpvocandtransl);
      //If a EnumartionElement object is added to a vector, another
      //EnumartionElement object is disposed (and created first--a copy
      //of the object to insert?). So when the destructor of the second
      //object is called it releases the memory. But because the address
      //of the m_psetpvocandtransl was just copied, it shell be released
      //for a second time->error. So prevent this.
      if(! m_psetpvocandtransl )
      {
        delete m_psetpvocandtransl ;
        m_psetpvocandtransl = NULL ;
      }
    }
    void Insert(VocabularyAndTranslation * pvoc)
    {
      if( ! m_psetpvocandtransl )
        m_psetpvocandtransl = new std::set<VocabularyAndTranslation *> ;
      m_psetpvocandtransl->insert(pvoc) ;
      if(m_psetpvocandtransl->size() == 1 )
        m_isetpvocandtransl = m_psetpvocandtransl->begin() ;
    }
    BYTE GetGermanArticle()
    {
      //return (*m_isetpvocandtransl)->m_arbyAttribute[1] ;
      //return VOC_AND_TRANSL_GET_GERMAN_ARTICLE() ;
      return (*m_isetpvocandtransl)->GetGermanArticle() ;
    }
    std::string GetGermanPlural()
    {
      return (*m_isetpvocandtransl)->m_arstrGermanWord[1] ;
    }
    std::string GetGermanSingular()
    {
      return (*m_isetpvocandtransl)->m_arstrGermanWord[0] ;
    }
    WORD GetNumberOfNouns()
    {
      //WORD wNumberOfNouns = 0 ;
      ////m_psetpvocandtransl->
      //for(isetpvocandtransl )
      //  if( isetpvocandtransl->m_byWType==WORD_TYPE_NOUN)
      //    ++ wNumberOfNouns ;
      //return wNumberOfNouns ;
      return m_psetpvocandtransl->size() ;
    }
    BYTE GetTranslationType()
    {
      return (*m_isetpvocandtransl)->m_arbyAttribute[0] <<
        BIT_POSITION_FOR_TRANSLATION_TYPE ;
    }
    BYTE IsSingular()
    {
      return (*m_isetpvocandtransl)->m_arbyAttribute[0] ;
    }
    void NextNoun()
    {
      m_isetpvocandtransl ++ ;
    }
  };

  class PointerToWordNodeWhereAnEnglishNounIsEncapsulated ;

  class EngNounsLinkedList 
    : public IVocabularyInMainMem
  {
  public:
    const std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated>
      * m_pvecpwordnodeEnglish ;
    std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated>::iterator
      m_ivecpwordnodeEnglish ;

    EngNounsLinkedList(
      //const
      std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> &
      rcvecpwordnode)
    {
      m_pvecpwordnodeEnglish = & rcvecpwordnode ;
      m_ivecpwordnodeEnglish = rcvecpwordnode.begin() ;
    }
    BYTE GetGermanArticle()
    {
      GermanNoun * pgn=
        GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
          m_ivecpwordnodeEnglish->m_pWordNode);
      if(pgn)
        return pgn->m_bArticle ;
      return -1 ;
    }
    std::string GetGermanPlural()
    {
      GermanNoun * pgn=
        GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
          m_ivecpwordnodeEnglish->m_pWordNode);
      if(pgn)
        return pgn->m_strPlural.GetBuffer() ;
      return "" ;
    }
    std::string GetGermanSingular()
    {
      GermanNoun * pgn=
        GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
          m_ivecpwordnodeEnglish->m_pWordNode);
      if(pgn)
        return pgn->m_strSingular.GetBuffer() ;
      return "" ;
    }
    WORD GetNumberOfNouns()
    {
      return m_pvecpwordnodeEnglish->size() ;
    }
    BYTE GetTranslationType()
    {
      EnglishNoun * pen=dynamic_cast<EnglishNoun*>
        (m_ivecpwordnodeEnglish->m_pWordNode->m_pWord);
      if(pen)
      {
        return pen->m_bTranslationType ;
      }
      return -1 ;
    }
    BYTE IsSingular()
    {
      return m_ivecpwordnodeEnglish->m_bSingular ;
    }
    void NextNoun()
    {
      m_ivecpwordnodeEnglish ++ ;
    }
  };

  //Declare forward(faster than "#include" for the following methods)
  class Sentence ;
  class Subject ;
  class Clause ;

//  namespace NSLetterTree
//  {
//    void Translate(const Sentence & sentence,
//      std::vector<std::vector<std::string>> & vecvecstr) ;
//    void Translate(
//      //const
//      Subject & subject,
//      std::vector<std::vector<std::string>> & vecvecstr
//      );
//    void Translate(
//      const Clause & clause,
//      std::vector<std::vector<std::string> > & vecvecstr
//      );
//  };

#endif //#ifndef LETTERTREE_H
