//#pragma once 
//Ohne das folgende include/wenn Pfad in den Eigenschaften dieser Datei 
//unter C/C++->vorkompilierte Header->"PHC durch..." falsch ist: 
//"fatal error C1010: Unerwartetes Dateiende während der Suche nach dem vorkompilierten Header.[...]"
#include "../StdAfx.h"
#include "sequential.h"

//Adds word (verb, "not" etc.) to predicate if the predicate is valid afterwards.
//Note: a predicate may be valid before adding (e.g. "have") and may be valid
//("have been") or invalid ("have go") afterwards.
//returns: true: success, else false
bool SequentialParse::AddToCurrentPredicate(
  VocabularyAndTranslation * pvocabularyandtranslation)
{
  if( m_ppredicate->m_pvocabularyandtranslationInflectedVerb )
    return false;
  m_ppredicate->m_pvocabularyandtranslationInflectedVerb =
    pvocabularyandtranslation ;
  return true;
}

EnumerationElement * SequentialParse::AddEnumEleToSubjOrObj()
{
  if( ! m_pclause )
    m_pclause = new Clause() ;
  return m_pclause->AddEnumEleToSubjOrObj() ;
}

BYTE SequentialParse::HandleNoun(//EnumerationElement & renumele
  VocabularyAndTranslation * pvocabularyandtranslation
                )
{
  //if( m_bPredicateStarted)
  if( m_ppredicate )
    //A predicate existed before->this noun belongs to an object or place 
    //enum elem. etc.
  {
    //If a predicate is e.g. incomplete (e.g. "I will you") it is
    //invalid.
    if(m_ppredicate->IsValid() )
    {
      m_pclause->m_vecppredicate.push_back(m_ppredicate) ;
      //TODO handle: if 1 or more tokens before the first
      //predicate token were unknown insert them as proper name.
      //if(!m_pclause->m_pSubject()
    }
    //Signal that the next verb belongs to a further possible predicate.
    //E.g. "Some girls like her like you.": first "like": possible predicate.
    //Second "like": real predicate.
    m_ppredicate = NULL ;
    m_bPredicateStarted = false ;
  }
  //if(! m_bSubOrObjOrPlaceEnumEleOccured )
  {
    //clause.m_pSubject = new Subject() ;
    //If this noun does NOT belong to a NEW enumeration element.
    if( ! m_pee )
      //if( m_pvocabularyandtranslationPredicate )
      //Append a new enum ele.
      //m_pee = m_pclause->AddEnumEleToSubjOrObj() ;
      m_pee = AddEnumEleToSubjOrObj() ;
    //EnumerationElement * pee ;
    //if(!m_bPredicateOccured)
    //  pee = & clause.m_pSubject->m_vecEnumerationElement.at(
    //    clause.m_pSubject->m_vecEnumerationElement.size() - 1 ) ;
    //else
    //  pee = & 
    //  //clause.m_pObject->m_vecEnumerationElement.at(
    //  //  clause.m_pObject->m_vecEnumerationElement.size() - 1 ) ;
    //  clause.getPointerToFirstObjectEnumEle() ;
    //EnumerationElement ee ;
    //ee.m_pEnr = new EnglishNounRecord() ;
    //ee.m_pEnr

    //ee.m_pengnounslettertree = new EngNounsLetterTree() ;
    m_pee->AddVocabularyAndTranslation( //*iter 
      pvocabularyandtranslation);
    /*clause.m_pSubject->m_vecEnumerationElement.push_back( ee ) ;*/
  }
  //if(m_bSubOrObjOrPlaceEnumEleOccured && m_bEnumerationTerminated )
  {
    //clause.m_pSubject = new Subject() ;
    //EnumerationElement ee ;
    //ee = clause.
      //Returns the current enum ele for a subject or an object,
      //whatever is current.
      //getCurrentSubjEnumEle() ;
    //ee.m_pEnr = * iter ;
    //clause.m_pSubject->m_vecEnumerationElement.push_back(
  }
  //bPossibleSubjectWithinCurrentClause = true ;
  //pvecpvocabularyandtranslationPossSubjOrObj.push_back(*iter) ;
  //bSubOrObjOrPlaceEnumEleOccured = true ;

  //if(m_pvocabularyandtranslationPredicate && 
  //  m_pvocabularyandtranslationPredicate->m_arbyAttribute[0] == 
  //  //ALLOWS_1_OBJECT
  //  1 )
  //  clause.m_pObjectFirst = new Object() ;

  //TODO implement this method:
  //AddPossibleAdjectives() ;
  return 0;
}

BYTE SequentialParse::HandleVerb(
  VocabularyAndTranslation * pvocabularyandtranslation)
{
  m_bPredicateStarted = true ;
  //if( bPossibleSubjectWithinCurrentClause )
  //  pvecpvocabularyandtranslationSubject = pvecpvocabularyandtranslation ;
  //pvecpvocabularyandtranslation = new std::vector<VocabularyAndTranslation *> ;
  //bPossiblePredicateFound = true ;
  //pvocabularyandtranslationPredicate = *iter ;

  if( m_ppredicate)
  {
    //if(clause.AddToCurrentPredicate(*iter) )
    if(! AddToCurrentPredicate(//*iter
      pvocabularyandtranslation ) )
      return 0 ;
  }
  //This is the first or next predicate.
  //if(! m_ppredicate)
  else
  {
    m_ppredicate = new Predicate() ;
    this->m_bPredicateStarted = true ;
    if(! AddToCurrentPredicate(//*iter
      pvocabularyandtranslation ) )
      return 0 ;
  }
  //If imperative, there is no subject/noun before.
  //if( pvocabularyandtranslationNoun )
    //if( pvocabularyandtranslationNoun->GetPersonIndex()

  //if(clause.m_pSubject)
  if( m_pclause->m_pSubject )
  //if(! bSubOrObjOrPlaceEnumEleOccured )
  //  //if(::PersonIndicesMatch(clause.m_pSubject->m_personIndex,
  //  //  (*iter)->m_byTypeGetPersonIndex() )
  //    ;

    ////An infinitive at the beginning can only be followed by an 
    ////imperative sentence.
    //ParsePossibleImperative(//Pass the current letter node because
    //  //its attributes contain infos about possible prepositions
    //  //(e.g."go": "Go TO the cinema!"and so on.
    //  (*iter)) ;
    //or: clause.ParseParsePossibleImperative(...): ends at clause's 
    //(if more than 1 clause follows: BEFORE the end of sentence.
    //remember: a imperative clause may also contain (imperative, 
    //noun,... )enumerations-> recursive call top THIS function?
  //else
  //  if(maybeProperNameOccured)
  //    clause.AddToSubject(psv.at(i2)) ;
  //Tell to add a new Enum ele.
  m_pee = NULL ;
  return 1 ;
}

  //TODO implement this method:
//BYTE SequentialParse::AddPossibleAdjectives(noun)
//{
//  if( adjectiveEnumeration )
//  {
//    noun->assign ( adjectiveEnumeration ) ;
//    pAdjectiveEnumeration = NULL ;
//  }
//}

  //TODO implement this method:
////Adjectives: e.g. with "He is intelligent and sexy."
//BYTE SequentialParse::AddPossibleAdjectives(Clause & clause)
//{
//  if( adjectiveEnumeration )
//  {
//    clause->assign ( adjectiveEnumeration ) ;
//    pAdjectiveEnumeration = NULL ;
//  }
//}

BYTE SequentialParse::HandleEnumeration()
{
  //m_byBitmaskLastTypeOfEnumeration = 0 ;
  //if( m_bPredicateStarted ) //or: if(m_ppredicate)
  if( m_ppredicate )
  {
    if(! m_ppredicate->IsValid() )
      return INVALID_PREDICATE ;
  //  //TODO (also) add predicate to list of enumeration elements because
  //  //there may be sentences like "I like, watch and get you."
  //  //clause.m_vecPredicate.push_back(*m_ppredicate);
  //  clause.m_ppredicate->push_back(*m_ppredicate);
    m_pclause->m_vecPredicate.push_back(*m_ppredicate);
  //  //Set back to false
  //  m_bPredicateStarted = false ;
  //  //Use a bitmask because a word that is an enumeration element may be 
  //  //spelled the same if e.g. noun and verb (e.g. "heating").
  //  m_byBitmaskLastTypeOfEnumeration |= VERB ;

    //Set to NULL so the next time a part of a predicate appears 
    //a new object is dynamically created.
    m_ppredicate = NULL ;
  }
  //TODO implement this method:
  //if( HasAdjectiveInIt(*mp_setpvocabularyandtranslationCurrent) )
  //{
  //  if( ! adjectiveEnumeration )
  //    adjectiveEnumeration = new adjectiveEnumeration ();
  //  adjectiveEnumeration->push_back(pvocabularyandtranslation);
  //  if( m_pAdverbEnumeration )
  //  {
  //    adjective->assign ( pAdverbEnumeration ) ;
  //    m_pAdverbEnumeration = NULL ;
  //  }
  //}
  //"else if" because: An adjective and a noun may be spelled identically, so avoid assigning an adjective token to
  //the same token (as noun) .
  //TODO implement this method:
  //else if( HasNounInIt(*mp_setpvocabularyandtranslationCurrent) )
  //{
  //  if( !pNounEnumeration )
  //    m_pNounEnumeration = new nounEnumeration ();
  //  
  //  //AddPossibleAdjectives(noun)
  //  if( m_pAdjectiveEnumeration )
  //  {
  //    noun->assign ( adjectiveEnumeration ) ;
  //    m_pAdjectiveEnumeration = NULL ;
  //  }
  //  //nounEnumeration->push_back(pvocabularyandtranslation);
  //  nounEnumeration->push_back(noun);
  //}

  //reset.
  mp_setpvocabularyandtranslationCurrent = NULL ;
  return 0; //SUCCESS ;
}

  //TODO implement this method:
////void SequentialParse::HandleEndOfClause()
//void SequentialParse::HandleBeginOfClause()
//{
//  //if( m_pclause->IsValid() )
//  //E.g. if "He is intelligent WHEN I saw him." : add "intelligent" to the clause 
//  //"He is intelligent".
//  AddPossibleAdjectives(m_pclause) ;
//}
//TODO new parse strategy (should be faster): don't test for single 
//sentence elements but check the word type. And then based on the 
//previous sentence info check which further combinations are grammatically
//possible.
//Ex.: The car is red. current word: car previous info: The -> valid subject->
//further possible: verb, enumertion of subjects (,  ...).
Sentence SequentialParse::Parse(//const PositionCStringVector & psv
  const PositionstdstringVector & psv )
{
  //"clause" may be every possible clause: relative clause, main clause,...
  bool bPossibleSubjectWithinCurrentClause = false ;
  bool bPossiblePredicateFound = false ;
  bool bSubOrObjOrPlaceEnumEleOccured = false ;
  bool bEnumerationTerminated = true ;
  //std::set<VocabularyAndTranslation> * psetvocabularyandtranslation ;
  std::set<VocabularyAndTranslation *> * psetpvocabularyandtranslation ;
  std::vector<Sentence> vecPossibleSentences ;
  VocabularyAndTranslation * pvocabularyandtranslationPredicate = NULL ;
  LetterNode * pletternode =  NULL ;
  Clause clause ;
  Sentence sentence ;
  //g_lettertree.search(str.GetBuffer() ) ;
  for(DWORD i2=0;i2<psv.size();i2++)
  {
    //if( IsSatzzeichen( psv.at(i2) )
    if( psv.at(i2).m_Str == "." )
    {
      //TODO: falls man die Arbeit SO aufteilt(bei verteilter Übersetzung), 
      //dass man nur bis zu einem
      //Satzendezeichen parsen (und übersetzen) lassen will, kann das 
      //passieren: I like mp3.com. -> "I like mp3." ist zwar ein Satz, aber
      //nicht der vollstädnige Satz. Wenn man dann "com." übersetzen soll,
      //würde ein Prädikat fehlen. -> diesen Zeil müsste man als Wortgruppe
      //übersetzen (kann ja auch "I like mp3.com, web.de and google" heissen
      //-> "com, web" übersetzen, "de and google" übersetzen.->an den 
      //Verteiler zurückschicken und DORT zusammensetzen.
      //if( psv.at(i2).m_Str == "," )
      //  addToEnumeration( 
      //    //need top know the wordkind (may be verb, noun,...)
      //    //because the list of vocs may 
      //    //contains different wordkinds .
      //    byLastWordKind, pletternode->pset...) ;

      //sentence.m_vecClause.push_back() ;
      //vecPossibleSentences.push_back() ;

      //If the last token belonged to a (possible) predicate.
      if(m_ppredicate && m_ppredicate->IsValid() )
        if(m_ppredicate->GetNumberOfNeededObjects() == 0 )
          clause.m_vecppredicate.push_back(m_ppredicate) ;
        //else
        //  return error
        //clause.isvalid = false
      
      ////If a proper name contains a point / Satzendezeichen it may not be a valid 
      ////(Subject, predicate )clause: "mp3.com is a website."
      ////TODO implement this method:
      //if( m_pclause->IsValid() )
      //  sentence.m_vecClause.push_back(//clause
      //    *m_pclause) ;
    }
    else if( psv.at(i2).m_Str == "," )
    {
      bEnumerationTerminated = false ;
      HandleEnumeration();
    }
    else if( psv.at(i2).m_Str == _T("and") )
    {  
      //;
      HandleEnumeration();
    }
    else
    {
      //psetvocabularyandtranslation = g_lettertree.searchIncludingInflections();
      //Inklusive Verb-Beugungen und (Nomen-)Deklinationen suchen.
      //pletternode = g_lettertree.searchIncludingInflections();

      //TODO a word may consist of 2 or more tokens (ex.: "vacuum cleaner").
      //Then "vacuum" could hide "vacuum cleaner".
      //psetvocabularyandtranslation = g_lettertree.search( psv.at(i2).m_Str );
      //TODO handle vocabularies with more than 1 word. Maybe do this like that:
      //if searching a vocabulary for 1 word shows that more words could follow
      //(the array 1 level lower contains a space) continue searching for further
      //words.
      //psetpvocabularyandtranslation = g_lettertree.search( psv.at(i2).m_Str );

      //Geht mehrere Token durch: besonders für Nomen ("vacuum cleaner") sinnvoll?
      //auch für Verben sinnvoll? ("to shirt circuit" = kurzschließen?)
      psetpvocabularyandtranslation = g_lettertree.search( psv, i2 );
      //if( psetvocabularyandtranslation )
      //if( pletternode->ContainsAtLeast1Vocable() )
      //if( psetvocabularyandtranslation && psetvocabularyandtranslation->size() > 0 )
      if( psetpvocabularyandtranslation && psetpvocabularyandtranslation->size() > 0 )
      {
        mp_setpvocabularyandtranslationCurrent = psetpvocabularyandtranslation ;
        //std::set<VocabularyAndTranslation> * 
        //  psetvocabularyandtranslation = pletternode->
        //  m_psetvocabularyandtranslation ;
        //std::set<VocabularyAndTranslation *> * 
        //  psetpvocabularyandtranslation = pletternode->
        //  m_psetpvocabularyandtranslation ;
        //for(WORD wIndex = 0 ; wIndex < psetvocabularyandtranslation->size() ; 
        //  ++ wIndex )
        //for(std::set<VocabularyAndTranslation>::iterator iter =
        //  psetvocabularyandtranslation->begin() ; iter != 
        //  psetvocabularyandtranslation->end() ; iter ++ )
        EnumerationElement ee ;
        //if(!m_bPredicateOccured)
        //  m_pee = & clause.m_pSubject->m_vecEnumerationElement.at(
        //    clause.m_pSubject->m_vecEnumerationElement.size() - 1 ) ;
        //else
        //  m_pee = & 
        //  //clause.m_pObject->m_vecEnumerationElement.at(
        //  //  clause.m_pObject->m_vecEnumerationElement.size() - 1 ) ;
        //  clause.getPointerToFirstObjectEnumEle() ;

        //Loop for all vocabularies having at least 1 word that 
        //equals the current token.
        for(std::set<VocabularyAndTranslation *>::iterator iter =
          psetpvocabularyandtranslation->begin() ; iter != 
          psetpvocabularyandtranslation->end() ; iter ++ )
        {
          //TODO there may be more possibilities (e.g. if spelling for a noun
          //and verb are identical, e.g. the >>heating<<, I am >>heating<<.
          if((*iter)->m_byType == WORD_TYPE_MAIN_VERB )
          {
            HandleVerb(*iter);
          }
          ////An adverb and a verb may be identical: "I LIKE you.",
          ////"People LIKE you suck."
          //if((*iter)->m_byType == WORD_TYPE_ADVERB )
          //  HandleAdverb(*iter);
          
          //I like heating. I like heating up.
          //I like vacuum. I like vacuum cleaners.
          //Adjective enumeration inside noun enumeration:
          //I like women, long, fat and black dogs, and money.
          if((*iter)->m_byType == WORD_TYPE_NOUN )
          {
            //HandleNoun(//ee
            //  );
            //if(m_pee)
              HandleNoun(*iter) ;
          }
          if((*iter)->m_byType == WORD_TYPE_CONJUNCTION ) //e.g. "He is intelligent WHEN he goes."
          {
            //HandleNoun(//ee
            //  );
            //if(m_pee)
            //HandleEndOfClause(*iter) ;
            //TODO implement this method:
            //HandleBeginOfClause(*iter) ;
          }
          //possibilitytree.add
        }
        //  if(
        //if( pletternode->
        //There may be even 2 or more noun the the same spelling. So
        //add the ee containing VocabularyAndTranslation after
        //iterating through all possible nouns with the same spelling.
        //clause.m_pSubject->m_vecEnumerationElement.push_back( ee ) ;
      }
    }
  }
  return sentence ;
}
