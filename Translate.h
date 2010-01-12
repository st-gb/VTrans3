#if !defined(TRANSLATE_H__FB6F08E6_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_)
     #define TRANSLATE_H__FB6F08E6_1B96_11D5_A7D8_0000B45A1FE5__INCLUDED_

#ifdef _USRDLL//for the case that no MFC
#define TRACE(x) 
//#define TRACE(x,y) 
#define ASSERT(x) 
#endif


// Translate.h
// const DATENTYP & :
// diese Variable soll vor dem versehentlichen Verändern geschützt werden (const)
// und zusätzlich als Referenz (&) dienen, um speichersparend zu sein, und
// nicht als Kopie den doppelten Speicherplatz zu belegen
#include "Resource.h" // für die Ressourcen-Makros (z.B. IDD_VTRANS_DIALOG), 
// damit das Projekt kompiliert werden kann
#include "StdAfx.h"
//#include "VTransDlg.h"
#include <vector> // für ADVERBVECTOR
#include "Word.hpp"
#include "Token.h" //for PositionCStringVector
//#ifndef TEXT_DEFINED
#include "Text.h" // für MarkSentence()
#include "rest.h" //für typedef	std::vector<CString> CStringVector;
//#include "EnglishView.h" //für TranslateText
#include <string>
#include "xmlwriter.h"
#include "SentenceElement.h" //for EnumerationElement etc.
#include "./VocabularyInMainMem/LetterTree/LetterTree.hpp"

//#endif
//#include"EnglishView.h"
//#include"EnglishView.h"
//VocNode * m_first;
//VocNode * m_first_match;
//#ifndef TRANSLATE_H_INCLUDED
//#define TRANSLATE_H_INCLUDED

//WordNode * m_first=NULL;


//englische Personalpronomen, Anfang
#define I 1
#define YOU 2
#define HE_SHE_IT 3
#define WE 4
#define THEY 6
//englische Personalpronomen, Ende
//Indizes für gebeugte Verformen, Anfang
#define INDEX_FOR_GERMAN_INFINITIVE 0
#define INDEX_FOR_ICH_PRAESENS 1
#define INDEX_FOR_DU_PRAESENS 2
#define INDEX_FOR_ER_SIE_ES_PRAESENS 3
#define INDEX_FOR_WIR_PRAESENS 4
#define INDEX_FOR_IHR_PRAESENS 5
#define INDEX_FOR_PLURAL_SIE_PRAESENS 6
#define INDEX_FOR_ICH_PRAETERITUM 7
#define INDEX_FOR_DU_PRAETERITUM 8
#define INDEX_FOR_ER_SIE_ES_PRAETERITUM 9
#define INDEX_FOR_WIR_PRAETERITUM 10
#define INDEX_FOR_IHR_PRAETERITUM 11
#define INDEX_FOR_PLURAL_SIE_PRAETERITUM 12
//Indizes für gebeugte Verformen, Ende
//Artikel-Arten für ein bestimmtes englisches Substantiv:
#define A 1
#define AN 2
#define THE 3

//extern WordNode * m_first;
//extern volatile BYTE g_bContinue;
//extern volatile DWORD g_dwStatus;
//extern volatile DWORD g_dwSentenceStatus;
//extern volatile DWORD g_dwTotal;
//extern volatile DWORD g_dwTranslated;
//extern volatile DWORD g_dwTokensInSentence;
//volatile BYTE g_bContinueLoading=TRUE;
//WordNode * m_first=NULL;
//volatile BYTE g_bContinue=TRUE;
//volatile DWORD g_dwStatus=0;
//volatile DWORD g_dwSentenceStatus=0;
//volatile DWORD g_dwTotal=0;
//volatile DWORD g_dwTranslated=0;
//volatile DWORD g_dwTokensInSentence=0;

//typedef std::vector<CObject> CObjectVector;
typedef std::vector<CString> ADVERBVECTOR;

//typedef	std::vector<PositionCString> PositionCStringVector;

static std::string arstrInflectionSuffix[]={_T("em"),_T("er"),_T("e"),_T("en")};
//Ohne "static": Linker-Fehler "LNK2005"
static std::string g_arstrInflectionSuffixForNominative[]={
  _T(""),//Er ist ein("") Mann.
  _T("e"),//Sie ist einE Frau.
  _T(""),//Es ist ein("") Kind.
  _T("e")//Sie sind meinE Kinder.
};

static std::string g_arstrInflectionSuffixForGenitive[]={_T("es"),_T("er"),_T("es"),
  //Ich bin meinER Kinder Vater.
  _T("er")};

//Also for the translation as object: 
//  "Your man is given things."->DeinEM Mann werden Dinge gegeben.
//  "Your woman is given things."->DeinER Frau werden Dinge gegeben.
//  "Your child is given things."->DeinEM Kind werden Dinge gegeben.
//  "Your children are given things."->DeinEN Kindern werden Dinge gegeben.
static std::string g_arstrInflectionSuffixForDative[]={
  _T("em"),
  _T("er"),
  _T("em"),
  //"Ich vertraue meinEN Kindern."
  _T("en")};
static std::string g_arstrInflectionSuffixForAccusative[]={_T("en"),_T("e"),_T(""),
  //"Ich mag meinE Kinder."
  _T("e")};

static std::string g_arstrAdjInflectionSuffixForIndefArticleAccusative[]={
  _T("en"),//"Ich mag einen altEN Mann."
  _T("e"),//"Ich mag eine altE Frau."
  _T("es"),//"Ich mag ein altES Kind."
  _T("e") //"Ich mag meinE Kinder."
  };

static std::string g_arstrAdjInflectionSuffixForNominative[]={
  _T("er"),//Er ist ein altER Mann.
  _T("e"),//Sie ist ein altE Frau.
  _T("es"),//Es ist ein altES Kind.
  //"Ich mag meinE Kinder."
  _T("e")};

 static std::string g_arstrAdjInflectionSuffixForDefArticleNominative[]={
  _T("e"),//Er ist der altE Mann.
  _T("e"),//Sie ist die altE Frau.
  _T("e"),//Es ist das altE Kind.
  _T("en")//"Das sind die altEN Kinder."
  };

static std::string g_arstrAdjInflectionSuffixForDefArticleAccusative[]=
  {
  _T("en"),//"Ich mag den altEN Mann."
  _T("e"),//"Ich mag die altE Frau."
  _T("e"),//"Ich mag das altE Kind."
  _T("en")//"Ich mag die altEN Kinder."
  };

static std::string g_arstrInflectedDefiniteArticleForNominative[]={
  _T("der"),//DER Mann ist groß.
  _T("die"),//DIE Frau ist groß.
  _T("das"),//DAS Kind ist groß.
  _T("die")//DIE Kinder sind groß.
};

static std::string g_arstrInflectedDefiniteArticleForGenitive[]={
  _T("des"),//Das Kind DES Mannes
  _T("der"),//Das Kind DER Frau
  _T("des"),//Das Kind DES Kindes
  _T("der")//Das Kind DER Kinder
};

static std::string g_arstrInflectedDefiniteArticleForDative[]={
  _T("dem"),//Ich vertraue DEM Mann.
  _T("der"),//Ich vertraue DER Frau
  _T("dem"),//Ich vertraue DEM Kind
  _T("den")//Ich vertraue DEN Kindern
};

static std::string g_arstrInflectedDefiniteArticleForAccusative[]={
  _T("den"),//Ich mag DEN Mann.
  _T("die"),//Ich mag DIE Frau.
  _T("das"),//Ich mag DAS Kind.
  _T("die")//Ich mag DIE Kinder.
};

//Wenn für jeden Genus(Geschlecht) definiert, dann müssen die jeweiligen 
//Werte keine Systematik aufweisen(also z.B, hintereinander liegen).
#define VALUE_FOR_MASCULINE_TO_ARRAY_INDEX(value) value-49
#define VALUE_FOR_MASCULINE_TO_ARRAY_INDEX(value) value-49
#define VALUE_FOR_NEUTER_TO_ARRAY_INDEX(value) value-49

#define GERMAN_INFLECTION_ARRAY_INDEX_FOR_PLURAL 3

#define LOWEST_VALUE_FOR_GENDER 49
#define VALUE_FOR_GENDER_TO_ARRAY_INDEX(value) value-LOWEST_VALUE_FOR_GENDER

#define INDEX_FOR_EM 0
#define INDEX_FOR_ER 1
#define INDEX_FOR_E 2
#define INDEX_FOR_EN 3

CString											GetArticle(GermanNoun * gn);
CString											GetBefore();
int													GetCurrentIndex();
CString											GetNextWord();
CString											GetNextToken();
int													GetNumToken(CString);
BYTE												GetPersonIndex(BYTE indexBefore,BYTE newIndex);
int													GetPradicatePosition(CString str);
CString											GetTokenAt(CString,int);
BYTE												ObjectPersonalPonounToIndex(CString);
CString											Transform(CString);
std::vector<CStringVector>	TranslateByAgentEnumerationElement(const 
															ByAgentEnumerationElement & ee,int & nStartID);
std::vector<CStringVector>	TranslateENR(EnglishNounRecord * enr,BYTE Case);
std::vector<CStringVector>	TranslateENR(
                              EnglishNounRecord * enr,
                              BYTE Case,
	                            bool,
                              bool bTranslateAsObject,
                              BYTE & personIndex,
	                            BYTE & bFirstTokenInSentence,
                              BYTE & bGermanNounGender,
                              int & nStartID,
                              IVocabularyInMainMem & ivocabularyinmainmem
                              );
std::vector<CStringVector>	TranslateENR(EnglishNounRecord * enr,
															BYTE Case,BOOL bNegation,BOOL 
															bTranslateAsObject,BYTE & personIndex,BYTE & 
															bFirstTokenInSentence,BYTE & bArticle,int & 
															nStartID);
CStringVector								TranslateEnumerationElement(EnumerationElement ee);
//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
//m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
//zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
//(Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
//ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
//Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
//auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
//(Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
//pEV wird aber nichts in TranslateEnumerationElement2(const EnumerationElement 
//& ee,EnglishView * pEV,int & nStartID); hinzugefügt, sondern dient zur 
//Weiterleitung an die Funktion TranslateRelativeClause, die eventuell 
//innerhalb TranslateEnumerationElement2(const EnumerationElement & ee,
//EnglishView * pEV,int & nStartID); aufgerufen wird
std::vector<CStringVector>	TranslateEnumerationElement(const 
															EnumerationElement & ee,BOOL bTranslateAsObject,
															BYTE & personIndex,BYTE & bFirstTokenInSentence,
															//EnglishView * pEV,
                              std::vector<IntPair> & rvecintpair,int & nStartID);
//std::vector<CStringVector>	TranslateEnumerationElement2(const 
//															EnumerationElement & ee,EnglishView * pEV,
//															int & nStartID);
std::vector<CStringVector>	TranslateGerund(const Gerund & gerund,BYTE & 
															bFirstTokenInSentence,//EnglishView * pEV,
                              std::vector<IntPair> & rvecintpair,int & 
															nStartID);
std::vector<CStringVector>	TranslateManner(const Manner & manner,int & nStartID);
CString											TranslateMannerAsCString(const Manner & manner);
std::vector<CStringVector>	TranslateMannerEnumerationElement(const 
															MannerEnumerationElement & mee,int & nStartID);
CString											TranslateMannerEnumerationElementAsCString(const 
															MannerEnumerationElement & mee);
//CString											TranslateMiddle(CString str,MIDDLESTRUCT);
CString											TranslateObject(CString before);
CStringVector								TranslateObject(Object object);
//std::vector<CStringVector>	TranslateObject2(Object object);
std::vector<CStringVector>	TranslateObject2(Object object,BYTE Case);
//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
// m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
// zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
// (Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
// ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
// Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
// auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
// (Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
// pEV wird aber nichts in std::vector<CStringVector>	TranslateObject2(Object 
// object,BYTE Case,BOOL bNegation,BYTE & bFirstTokenInSentence,EnglishView * 
// pEV,int & nStartID); hinzugefügt, sondern dient zur eventuellen
// Weiterleitung an die Funktion TranslateObjectEnumerationElement.
std::vector<CStringVector>	TranslateObject2(Object object,BYTE Case,BOOL 
															bNegation,BYTE & bFirstTokenInSentence,
															//EnglishView * pEV,
                              std::vector<IntPair> & rvecintpair,int & nStartID);
CStringVector								TranslateObjectEnumerationElement(EnumerationElement 
															ee,BYTE bCase);
std::vector<CStringVector>	TranslateObjectEnumerationElement2(const 
															EnumerationElement & ee,BYTE bCase,int & nStartID);
std::vector<CStringVector>	TranslateObjectEnumerationElement2(const 
															EnumerationElement & ee,BYTE bCase,BOOL,BYTE & 
															bFirstTokenInSentence,//EnglishView * pEV,
                              std::vector<IntPair> & rvecintpair,int & 
															nStartID);
std::vector<CStringVector>	TranslateOfPhrase(EnglishNounRecord enr,int & 
															nStartID);
std::vector<CStringVector>	TranslatePlace(Place &,BYTE & 
															bFirstTokenInSentence,int & nStartID);
std::vector<CStringVector>	TranslatePlaceEnumerationElement(const 
															EnumerationElement & ee,int & nStartID,BYTE byCase);
CString											TranslatePossessivePronoun(BYTE);
std::vector<std::string>    TranslatePossessivePronoun3(BYTE byte);
CStringVector								TranslatePossessivePronoun2(BYTE);
CString											TranslatePSO(CString before);
std::vector<CStringVector>	TranslatePSO(const Clause & subSentence,
															BYTE & bFirstTokenInSentence,
															//EnglishView * pEV
                              std::vector<IntPair> & rvecintpair,int & rnStartID);
std::vector<CStringVector>	TranslateQuestion(Question & question,//EnglishView 
															//* pEV,
                              std::vector<IntPair> & rvecintpair,int & nStartID);
std::vector<CStringVector>	TranslateQuestionClause(Clause & ss,
															//EnglishView * pEV,
                              std::vector<IntPair> & rvecintpair,int & nStartID);
//std::vector<CStringVector>	TranslateRelativeClause(RelativeClause Clause);
std::vector<CStringVector>	TranslateRelativeClause(RelativeClause & 
															relativeClause,BYTE bArticle,//EnglishView * pEV,
                              std::vector<IntPair> & rvecintpair,
															int & nStartID);
CString											TranslateSentence(CString str);
std::vector<CStringVector>	TranslateSentence2(const Sentence &,//EnglishView * pEV
                              std::vector<IntPair> & rvecintpair);
std::vector<CStringVector>	TranslateSimilarity(const Similarity & similarity,
															int & nStartID);
CStringVector								TranslateSubject(Subject);
std::vector<CStringVector>	TranslateSubject(Subject & subject,BOOL 
															bTranslateAsObject,BYTE & personIndex,BYTE & 
															bFirstTokenInSentence,//EnglishView * pEV,
                              std::vector<IntPair> & rvecintpair,int & 
															nStartID);
//std::vector<CStringVector>	TranslateSubject2(const Subject &,BYTE &);
std::vector<CStringVector>	TranslateClause(Clause &);
std::vector<CStringVector>	TranslateClause2(const Clause &,BYTE & 
															bFirstTokenInSentence,//EnglishView * pEV
                              std::vector<IntPair> & rvecintpair, int 
															& nStartID);
CString											TranslateSPO(CString before);
std::vector<CStringVector>	TranslateText(Text & text);
std::vector<CStringVector>	TranslateText(Text & text,std::vector<Range> &,
															const DWORD & dwFlags,//EnglishView * pEV
                              std::vector<IntPair> & rvecintpair);
std::vector<CStringVector> TranslateText(
  Text & text,
  std::vector<SentenceAndValidityAndProperName> & rvecsentenceandvalidityandpropername,
  const DWORD & dwFlags  );

std::vector<CStringVector>	TranslateToPlusInfinitive(const ToPlusInfinitive & 
															toPlusInfinitive,int & nStartID);
#endif //!defined (TRANSLATE_H_INCLUDED)
