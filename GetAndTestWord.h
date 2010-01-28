#if !defined (GETANDTESTWORD_H_INCLUDED)
	    #define GETANDTESTWORD_H_INCLUDED

#include "rest.h" //for CStringVector etc.
#include "Word.hpp" //for WordNode, EnglishVerb etc.
//#ifndef _WINDOWS
//	#include <typedefs.h> //for DWORD etc.
//	#include <StdString.h> //for class compatible to MFC's "CString"  
//#endif//#ifndef _WINDOWS

//Substitutionenn f�r GetPointerToWordnodeWhereANonBendEnglishVerbIsIncluded
#define INFINITIVE                    1//to GO
#define INFINITIVE_OR_PAST_PARTICIPLE 2//z.B. to HIT,I have HIT
#define PAST_PARTICIPLE               3//z.B. you were GONE //WENT
#define PARTICIPLE_PROGRESSIVE        4//I am WALKING
//Substitutionenn f�r GetPointerToWordnodeWhereANonEnglishVerbIsIncluded ENDE

//Zeitformen, Anfang
#define SIMPLE_PRESENT 1
#define SIMPLE_PAST 2
#define FUTURE 3
#define PRESENT_PROGRESSIVE 4
#define PAST_PROGRESSIVE 5
#define PRESENT_PERFECT 6
#define PRESENT_PERFECT_PROGRESSIVE 7
#define SIMPLE_PRESENT_OR_SIMPLE_PAST 8
#define PAST_PERFECT 9
#define FUTURE_CONTINOUS 10
#define FUTURE_PERFECT 11
#define PAST_PERFECT_PROGRESSIVE 12
//Zeitformen, Ende

class PointerToEnglishNoun
{
public:
	BOOL m_bSingular;
	EnglishNoun * m_pEnglishNoun;
	PointerToEnglishNoun()
	{
		m_bSingular=TRUE;
		m_pEnglishNoun=NULL;
	};
};

class PointerToWordNodeWhereAnEnglishNounIsEncapsulated
{
public:
	BOOL m_bSingular;
	WordNode * m_pWordNode;
	PointerToWordNodeWhereAnEnglishNounIsEncapsulated()
	{
		m_bSingular=TRUE;
		m_pWordNode=NULL;
	};
};


EnglishAdverb								GetEnglishAdverb(CString str);
EnglishAuxiliaryVerb *			GetEnglishAuxiliaryVerb(CString & str);
EnglishNoun *								GetEnglishNoun(CString & str);
std::vector <EnglishNoun>		GetEnglishNoun(const VTrans_string_typeVector & svNoun);
EnglishVerb *								GetEnglishVerb(CString &);
Word *											GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
															const CString & str);
CString											GetGermanAdverb(CString str);
CString											GetGermanNoun(CString);
CString											GetGermanPersonalPronoun(CString & str);
GermanNoun *								GetGermanWordOfEnglishNoun(CString str);
GermanNoun *								GetGermanWordOfEnglishNoun(const VTrans_string_typeVector &);
std::vector <GermanNoun >		GetGermanWordOfEnglishNoun2(const VTrans_string_typeVector
															& cstrvec);
GermanVerb *								GetGermanWordOfEnglishVerb(CString str);
GermanAdjective *						GetGermanWordOfEnglishAdjective(CString);
GermanAdverb *							GetGermanWordOfEnglishAdverb(CString str);
WordNode * 
														GetPointerToWordNodeWhereABendEnglishVerbIsEncapsulated(
														const CString & str,WORD & rwFiniteVerbFormsAffectedbySearchString
														//die Menge der Formen eines Wortes, die das 
														//Flexionsparadigma von Verben oder Substantiven bilden; 
														//Konjugation beziehungsweise Deklination.
														);
WordNode *									GetPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated(
															const CString &str,const DWORD & dwFlags);
WordNode *									GetPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated(
															const CString & str,const DWORD & dwFlags,BYTE & bType);
WordNode *									GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
															const CString & str);
WordNode * GetPointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated(
															const CString & str);
WordNode * GetPointerToWordNodeWhereAnEnglishComperativeIsEncapsulated(
															const VTrans_string_typeVector & sv,const DWORD & dwFlags);
WordNode * GetPointerToWordNodeWhereAnEnglishConjunctionIsEncapsulated(
															const VTrans_string_typeVector & sv);
WordNode * GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(
															const CString &);
WordNode *									GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(
															const CString & str);
WordNode *									GetPointerToWordNodeWhereAnEnglishPositiveIsEncapsulated(
															const CString & str,const DWORD & dwFlags);
WordNode *									GetPointerToWordNodeWhereAnEnglishPrepositionIsEncapsulated(
															const VTrans_string_typeVector & sv);
WordNode *									GetPointerToWordNodeWhereAnEnglishProgressiveIsEncapsulated(
															const CString & str);
WordNode *									GetPointerToWordNodeWhereAnEnglishPronounIsEncapsulated(
															const CString & str);
WordNode *									GetPointerToWordNodeWhereAnEnglishPronounIsEncapsulated(
															const VTrans_string_typeVector & sv);
WordNode *									GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
															const CString &);
WordNode *									GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
															const CString & str,BYTE & bIndexes,BYTE & bTense);
WordNode * 
														GetPointerToWordNodeWhereANonBendEnglishVerbIsEncapsulated(
															const CString & str,BYTE & rbyType
															);

GermanNoun *								GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
															WordNode *);
std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> 
															GetVecPointerToWordNodeWhereAnEnglishNounIsEncapsulated(
															const VTrans_string_typeVector & cstrvec,const DWORD & dwFlags);
WordNode *									GetWordNodeWhereAnEnglishPositiveIsEncapsulated(
															const CString & str);
WordNode *									GetWordNodeWhereAnEnglishAdverbOfMannerIsEncapsulated(
															const CString &);
WordNode *									GetWordNodeWhereAnEnglishAdverbRefersToAdjectiveOrAdverbIsEncapsulated(
															const CString &);
std::vector<PointerToEnglishNoun> IfIsEnglishNounThenGetVectorOfPointerToEnglishNoun(
															const VTrans_string_typeVector & cstrvev);
std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> 
														IfIsEnglishNounThenReturnEnglishNounEncapsulatedInWord(
															const VTrans_string_typeVector & cstrvec);
BYTE												IsConsonant(CString str);
BOOL												IsEnglishAdjective(CString str); // englisches Adjektiv
WordNode *									IsEnglishAdjective2(CString & str); // englisches Adjektiv
BOOL												IsEnglishAdverb(CString str); // englisches Adverb
BOOL												IsEnglishAdverb(CString str,BYTE); // englisches Adverb
BOOL												IsEnglishAdverbOfFrequency(CString);
BOOL												IsEnglishAdverbOfManner(CString);
BOOL												IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString &);
BOOL												IsEnglishAdverbRefersToWholeSentence(CString);
BOOL										    IsEnglishAuxiliaryVerb(CString & str);
BOOL												IsEnglishInfinitive(CString str); // englisches Infinitiv
BOOL												IsEnglishNoun(CString str); // englisches Substanmtiv
BOOL												IsEnglishObjectVerb(CString str);
BOOL										    IsEnglishPersonalPronoun(CString &);
BOOL												IsEnglishPlural(CString str); // englischer Plural
BOOL												IsEnglishPlural(const VTrans_string_typeVector & cstrvec);
BOOL												IsEnglishPossessivePronoun(CString str); // englisches Possessivpronom
BOOL												IsPresentProgressiveCopyVocable(EnglishVerb & 
															rEV,const CString & token);
BOOL												IsEnglishPronoun(CString str);
BOOL												IsEnglishPronoun3rdPerson(CString str);
BOOL												IsEnglishSingular(CString str);
BOOL												IsEnglishSingular(VTrans_string_typeVector cstrvec);
BOOL												IsEnglishVerb(CString str); // englisches Verb
BYTE												IsEnglishVerb3rdPerson(const CString & str);
BYTE												IsEnglishVerb3rdPersonSingular(const CString 
															& rstrToken,const EnglishVerb & rEnglishVerb);
BYTE												IsEnglishWord(const CString & strWord);
BOOL												IsObjectPersonalpronoun(CString token);
BOOL												IsPastParticiple(CString str);
BOOL										    IsPersonalPronoun(CString token);
BYTE												IsPresentProgressive(const CString & token);
BOOL												IsPresentTense(CString &);
BOOL												IsSimplePast(CString str);
BOOL												IsVerb(CString str);
BYTE												IsVowel(CString str);

#endif //!defined (GETANDTESTWORD)
