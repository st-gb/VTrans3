/*
 * LetterTree_unused_code.cpp
 *
 *  Created on: 31.03.2012
 *      Author: Stefan
 */



  //Called by a GLR (created by bison parser generator; similar to yacc)
  //lexer.
  //std::set<VocabularyAndTranslation *> *
  bool LetterTree::IsPlural(
    const PositionStringVector & psv ,
//    const PositionstdstringVector & psv ,
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

  //Called by a GLR (created by bison parser generator; similar to yacc)
  //lexer.
  //std::set<VocabularyAndTranslation *> *
  bool LetterTree::IsSingular(
    const PositionStringVector & psv ,
//    const PositionstdstringVector & psv ,
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
    //If the word was found.
    if( p_letternode )
    {
      psetpvocabularyandtranslation = p_letternode->
        m_psetpvocabularyandtranslation ;
      if( psetpvocabularyandtranslation )
      {
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
      }
    }
    return bIsSingular ;
  }//IsSingular(...)


