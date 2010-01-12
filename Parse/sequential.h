
#ifndef SEQUENTIAL_H
  #define SEQUENTIAL_H
  #include "../SentenceElement.h"

  #define INVALID_PREDICATE 1
  class LetterNode ;
  class LetterTree ;
  //for PositionstdstringVector in "Parse( const PositionstdstringVector & psv) ;"
  //#include "../Token.h"

  extern LetterTree g_lettertree ;

  class SequentialParse
  {
  public:
    bool m_bSubOrObjOrPlaceEnumEleOccured ;
    bool m_bPredicateStarted ;
    std::set<VocabularyAndTranslation *> * mp_setpvocabularyandtranslationCurrent ;
    //Should point to the current main or subordinate clause.
    Clause * m_pclause ;
    Predicate * m_ppredicate ;
    //std::vector<Predicate *> m_vecppredicate ;
    EnumerationElement * m_pee ;
    SequentialParse()
      //C++ style initialization.
      : mp_setpvocabularyandtranslationCurrent(NULL)
    {
      m_pclause = NULL ;
      m_bPredicateStarted = false ;
      m_ppredicate = NULL ;
      m_pee = NULL ;
    }
    EnumerationElement * AddEnumEleToSubjOrObj() ;
    bool AddToCurrentPredicate(
      VocabularyAndTranslation * pvocabularyandtranslation) ;
    Sentence Parse(//const PositionCStringVector & psv
      const PositionstdstringVector & psv) ;
    BYTE HandleEnumeration() ;
    BYTE HandleNoun(VocabularyAndTranslation *) ;
    BYTE HandleVerb(VocabularyAndTranslation *) ;
  };

#endif//#ifndef SEQUENTIAL_H
