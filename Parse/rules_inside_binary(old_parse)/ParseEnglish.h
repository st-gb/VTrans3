#if !defined (PARSEENGLISH_H_INCLUDED)
	    #define PARSEENGLISH_H_INCLUDED

#include "Token.h" //for PositionCStringVector etc.
#include "Word.hpp" //for "class Object" etc.
#include "rest.h" //for CStringVector etc.
#include "SentenceElement.h" //for EnumerationElement etc.

//Satzbau-Arten(in der deutschen Sprache):
#define SPO 0
#define PSO 1
#define SOP 2

enum ClauseErrorCode
{
  success=0,
  NoGrammaticalPersonAgreement
};

BYTE												GetIndexFromPossessivePronoun(const CString &);
BYTE												GetPersonIndex(CString str);
BYTE  GetPersonIndex(
                     BYTE indexBefore,
                     BYTE newIndex) ;
BOOL												ParseArticleObject(Object & object,const 
															CStringVector & sv);
BOOL												ParseArticleObjectEnumerationElement(
															EnumerationElement & ee,CStringVector sv);
BOOL ParseByAgent(
      ByAgent & byAgent,
      const PositionCStringVector & psv,
      BOOL bCaseSensitive);
BOOL ParseByAgent(
      ByAgent & byAgent,
      const PositionCStringVector & psv,
      const DWORD & dwFlags) ;
BYTE												ParseByAgentEnumerationElement(
															ByAgentEnumerationElement & baee,const 
															PositionCStringVector & psv,const DWORD & dwFlags);
BOOL												ParseClauseWithArticleObject(CStringVector sv);
BYTE												ParseEnumerationElement(EnumerationElement & ee,
															const PositionCStringVector & psv,BOOL 
															bExcludeGerund,std::vector<Range> & rVecRange,
															BOOL bCaseSensitive);
//BYTE ParseDefiniteArticleNoun(
//      EnglishNounRecord & enr,
//			CStringVector & sv,
//      BOOL bCaseSensitive,
//      BYTE bFirstTokenOfSentence);
BYTE ParseDefiniteArticleNoun(
      EnglishNounRecord & enr,
      CStringVector & sv,
	    const DWORD & dwFlags,
      BYTE bContainsFirstTokenOfSentence) ;
BYTE ParseFullPredicate(
      Predicate & predicate,
      BOOL bBe,
      const CStringVector & sv,
	    BYTE personIndex,
      const DWORD & dwFlags) ;
BYTE                        ParseGerund(Gerund & gerund,BOOL bClause,BOOL 
															bObject,const PositionCStringVector & psv,
															std::vector<Range> &,const DWORD & dwFlags,BYTE 
															bFirstTokenOfSentence);
BYTE												ParsePlace(Place &,const PositionCStringVector & 
															sv,std::vector<Range> &,const DWORD & dwFlags);
BYTE												ParsePlaceEnumerationElement(EnumerationElement & 
															ee,const PositionCStringVector & psv,
															std::vector<Range> &,const DWORD & dwFlags);
BOOL												ParseManner(Manner & manner,const 
															PositionCStringVector & sv,BOOL bBe,const 
															DWORD & dwFlags);
BOOL												ParseMannerEnumerationElement(
															MannerEnumerationElement & mee,const 
															PositionCStringVector & psv,BOOL bBe,const 
															DWORD & dwFlags);
BOOL												ParseMiddle(ClauseWithoutSubject & ss,BOOL bBe,
															const CStringVector & sv);
BOOL												ParseMiddle(RelativeClause & ss,BOOL bBe,const 
															CStringVector & sv);
BOOL												ParseMiddle(Clause & ss,BOOL bBe,const 
															CStringVector & sv,BYTE personIndex,const DWORD & dwFlags);
BOOL												ParseMiddleEnumerationElement(
															MiddleEnumerationElement & mee,const CStringVector 
															& sv,const DWORD & dwFlags);
BYTE ParseNonBendPartOfPredicate(
  Predicate & predicate,
  BOOL bBe,
	const CStringVector & sv,	
  const DWORD & dwFlags) ;
BYTE												ParseObject(Object & object,const 
															PositionCStringVector & psv,BOOL bBe,
															std::vector<Range> &,const DWORD & dwFlags);
BOOL												ParseObjectEnumerationElement(EnumerationElement 
															& ee,const CStringVector & sv);
BOOL												ParseObjectEnumerationElement(EnumerationElement 
															& ee,const PositionCStringVector & psv,
															std::vector<Range> & rVecRange,const DWORD & dwFlags);
BOOL												ParseOfPhrase(OfPhrase &,const 
															PositionCStringVector & sv,const DWORD & dwFlags);
BYTE												ParsePersonalPronounOrNounStruct(EnglishNounRecord 
															& enr,const PositionCStringVector & psv,const 
															DWORD & dwFlags,BYTE bFirstTokenOfSentence);
BOOL												ParsePlaceEnumerationElement(EnumerationElement & 
															ee,const PositionCStringVector & psv);
BOOL ParseQuestion(
  Question & question,
  PositionCStringVector & psv,
	std::vector<Range> & rVecRange,
  const DWORD & dwFlags) ;
BOOL												ParseQuestionClause(Clause & ss,const 
															PositionCStringVector & psv,std::vector<Range> 
															&,const DWORD & dwFlags);
BOOL												ParseQuestionMiddle(Clause & ss,const 
															CStringVector & sv);
BOOL												ParseRelativeClause(Sentence & sentence,
															CStringVector sv);
BOOL												ParseRelativeClause(RelativeClause &,CStringVector sv);
BYTE												ParseRelativeClause(RelativeClause & relativeClause,
															const PositionCStringVector & psv,std::vector<Range> 
															& vecRange,DWORD dwFlags);
BOOL										    ParseSentence(Sentence &,CStringVector &);
//mit "Sentence" ist ein einfacher und/oder zusammengetzter Satz gemeint
BOOL ParseSentence(
  Sentence & sentence,
// psv soll eine Kopie sein (deswegen steht auch kein "&"
	// davor), damit die Veränderung durch die Funktion "MakeFirstLetterUpper"
	// sich nicht auf das Original auswirkt
	PositionCStringVector & psv,
	std::vector<Range> & rVecRange,
  const DWORD & dwFlags) ;
BYTE												ParseSimilarity(Similarity & similarity,const 
															PositionCStringVector & psv,BOOL bBe,const 
															DWORD & dwFlags);
BOOL												ParseSubject(Subject &,const PositionCStringVector &,
															BOOL bExcludeGerund,std::vector<Range> &,const 
															DWORD & dwFlags,BYTE bFirstTokenOfSentence);
BOOL												ParseSubjectWithoutGerund(Subject &,const 
															CStringVector &);
BOOL												ParseSubjectWithoutGerund(Subject & subject,const 
															PositionCStringVector & psv);
BYTE											  ParseClause(Clause &,const 
															PositionCStringVector &,std::vector<Range> &,
															const DWORD & dwFlags,BYTE bFirstTokenOfSentence);
BYTE												ParseToPlusInfinitive(ToPlusInfinitive & 
															toPlusInfinitive,const PositionCStringVector & 
															psv,std::vector<Range> & rVecRange,const DWORD & 
															dwFlags,BYTE bFirstTokenOfSentence);
BYTE												PredicateAllowsGerund(const Predicate & predicate);

#endif //!defined (PARSEENGLISH_H_INCLUDED)