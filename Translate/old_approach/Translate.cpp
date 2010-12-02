//Use "-fno-for-scope" for the compiler because of code like this:  
	//"for(i=0;i<dwBeginOfEnglishNoun && g_bContinue;i++)". Else error message
	//like "error: name lookup of `dwRun' changed for new ISO `for' scoping".
/*const DATENTYP & BEZEICHNER:
(Bsp.: const int & i)
diese Variable soll vor dem versehentlichen Verändern geschützt werden (const)
und zusätzlich als Referenz (&) dienen, um speichersparend zu sein, und
nicht als Kopie den doppelten Speicherplatz zu belegen
const DATENTYP & BEZEICHNER wird in meinen Funktionen oft als Parameter verwendet*/

//#include <atlstr.h>//This include is needed for "CString" when compiling as DLL.
// Translate.cpp
#include "StdAfx.h" // für vorkompilierte Header
#include "GetAndTestWord.h" //for class PointerToWordNodeWhereAnEnglishNounIsEncapsulated etc.
#include "ParseEnglish.h"
#include "Token.h" //for Append(...) etc.
#include "Translate.h"
#include "VocabularyInMainMem/DoublyLinkedList/WordNode.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"
//#include "VTrans.h"
//#include "MainFrm.h"
//#ifdef _WINDOWS
#ifdef _MSC_VER //MS Compiler
	#include <typeinfo.h> // für RTTI (RunTime Type Information)
	#include "./Resource.h" //Use "./" because else e.g. cygwin's Resource.h is included.
	//#include "Windows/ErrorCodeFromGetLastErrorToString.h"
	#include "MFC/ErrorCodeFromGetLastErrorToString.h"
#endif
//#include <sstream> //for ostringstream
#include <math.h> // für pow() in der Funktion WordNode * 
	//GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(const CString & str,
	//BYTE & bIndexes,BYTE & bTense)

#ifndef ID_USER//if not included/MFC not used(=compile as DLL).
#define ID_USER 0
#endif

//"ofstreamLogFile" should be declared in the module that starts the translation:
//Either the TransApp constructor or a DLL module.
extern std::ofstream ofstreamLogFile;

//#define _DEBUG
//#include "EnglishView.h"
//#include "GermanView.h"
//extern VocNode * m_first;
	//extern VocNode * m_first;
//extern VocNode * m_first_match;
//using namespace CDialog;
//extern * theApp;
//CVTransDlg * pDlg=(CVTransDlg*)theApp->m_pMainWnd

//VocNode * m_first=m_first;

volatile BYTE g_bContinueLoading=TRUE;
//WordNode * m_first=NULL;
volatile BYTE g_bContinue=TRUE;
volatile DWORD g_dwStatus=0;
volatile DWORD g_dwSentenceStatus=0;
volatile DWORD g_dwTotal=0;
volatile DWORD g_dwTranslated=0;
volatile DWORD g_dwTokensInSentence=0;
WordNode * m_first = NULL;
WordList wordList;

//SubSent


BYTE ObjectPersonalPonounToIndex(CString str)
{
	if(str=="me")
		return 1;
	if(str=="you")
		return 2;
	if(str=="him")
		return 3;
	if(str=="her")
		return 4;
	if(str=="it")
		return 5;
	if(str=="us")
		return 6;
	if(str=="them")
		return 7;
	return 0;
}

int GetPradicatePosition(CString str)
{
	int i=0;
	for(;i<str.GetLength();i++)
	{

	}
	return 0;
}

std::vector<CStringVector> TranslateByAgent(const ByAgent & byAgent,int & nStartID)
{
	std::vector<CStringVector> vv;
	CStringVector sv;
	sv.push_back(CString("von"));
	vv.push_back(sv);
	for(DWORD dwRun=0;dwRun<byAgent.m_enumerationElements.size();dwRun++)
	{
		//Append(vv,TranslateObjectEnumerationElement2(place.m_enumerationElements.at(dwRun),1));
		Append(vv,TranslateByAgentEnumerationElement(byAgent.
			m_enumerationElements.at(dwRun),nStartID));
	}
	return vv;
}

std::vector<CStringVector> TranslateByAgentEnumerationElement(const 
	ByAgentEnumerationElement & ee,int & nStartID)
{
	TRACE("std::vector<CStringVector> TranslatePlaceEnumerationElement(const EnumerationElement & ee) ANFANG\n");
	//CStringVector sv;
	std::vector<CStringVector> vv;
	if(ee.m_personalPronoun)
	{
		#ifdef _DEBUG
		printf("ee.m_personalPronoun: %u\n",ee.m_personalPronoun);
		#endif
		CStringVector sv;
		switch(ee.m_personalPronoun)
		{
			case 1:
				sv.push_back("mir");
				break;
			case 2:
				sv.push_back("dir");
				sv.push_back("euch");
				sv.push_back("Ihnen");
				break;
			case 3:
				sv.push_back("ihm");
				break;
			case 4:
				sv.push_back("ihr");
				break;
			case 5:
				sv.push_back("es");
				break;
			case 6:
				sv.push_back("uns");
				break;
			case 7:
				sv.push_back("ihnen");
				break;
		}
		vv.push_back(sv);
		//sv.push_back(GetGermanPersonalPronoun(CString & str));
	}
	if(ee.m_pDan)
	{
		#ifdef _DEBUG
		printf("ee.m_pDan\n");
		{
			for(DWORD dwRun=0;dwRun<ee.m_pDan->m_Noun.size();dwRun++)
			{
				printf("ee.m_pDan->m_Noun.at(%u): %s\n",dwRun,ee.m_pDan->m_Noun.at(dwRun));
			}
		}
		#endif
		CStringVector sv,sv2;
		std::vector <GermanNoun> vecGermanNoun=GetGermanWordOfEnglishNoun2(
			ee.m_pDan->m_Noun);
		for(DWORD dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
		{
			TRACE("vecGermanNoun\n");
			#ifdef _DEBUG
			printf("vecGermanNoun\n");
			#endif
			GermanNoun gn=vecGermanNoun.at(dwRun);
			sv.push_back(gn.m_strSingular);
			if(gn.m_bArticle=='1')
				sv2.push_back(_T("der"));
			else if(gn.m_bArticle=='2')
				sv2.push_back(_T("die"));
			else if(gn.m_bArticle=='3')
				sv2.push_back(_T("das"));
			else 
				sv2.push_back(_T("das"));
			//sv.push_back(gn.plural);
		}
		vv.push_back(sv2);
		vv.push_back(sv);
		//vv.push_back(
		//Append(vv,
	}
	if(ee.m_pEnr)
	{
		BYTE bGermanNounGender;
    BYTE bPersonIndex=0;
		BYTE bFirstTokenInSentence=0;
		#ifdef _DEBUG
		printf("ee.m_pEnr\n");
		#endif
		Append(vv,TranslateENR(ee.m_pEnr,1,//FALSE,FALSE,
      false,false,bPersonIndex,
			bFirstTokenInSentence,bGermanNounGender,nStartID)); // übersetzen im 3. Fall 
		// (I am in the kitchen.-> Ich bin in deR Küche.)
	}
	if(ee.m_pOfPhrase)
	{
		#ifdef _DEBUG
		printf("ee.m_pOfPhrase\n");
		#endif
		Append(vv,TranslateOfPhrase(ee.m_pOfPhrase->m_Enr,nStartID));
	}
	TRACE("CStringVector TranslateObjectEnumerationElement(EnumerationElement ee) ENDE\n");
	return vv;
}

/*std::vector<CStringVector> TranslateClauseWithoutSubject(ClauseWithoutSubject cws)
{
	TRACE("CStringVector TranslateClause ANFANG\n");
	if(cws.m_bNegation)
		TRACE("cws.m_bNegation\n");
	else
		TRACE("!cws.m_bNegation\n");
	std::vector<CStringVector> vv;
	//BYTE satzbau=SPO;
	if(cws.m_bWordOrder==SPO) // Subjekt Prädikat Objekt
	{
		//TRACE("satzbau==SPO\n");
		if(cws.m_bTense==FUTURE)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
			sv.push_back(gv->m_strWords[cws.m_personIndex]);
			vv.push_back(sv);
		}
		if(cws.m_bTense==PRESENT_PERFECT)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("have"));
			sv.push_back(gv->m_strWords[cws.m_personIndex]);
			vv.push_back(sv);
		}
		if(cws.m_bTense==PRESENT_PROGRESSIVE || cws.m_bTense==SIMPLE_PRESENT
			|| cws.m_bTense==SIMPLE_PAST)
		{
			GermanVerb * gv=GetGermanWordOfEnglishVerb(cws.m_strPredicate);
			if(gv!=NULL)
			{
//				int index=0;
	//			if((index=subSentence.m_Subject.m_personIndex*subSentence.tense+1)<14)
		//		{
					CStringVector svVerb;
					if(cws.m_bTense==SIMPLE_PRESENT || cws.m_bTense==PRESENT_PROGRESSIVE)
						svVerb.push_back(gv->m_strWords[cws.m_personIndex]); // Prädikat
					if(cws.m_bTense==SIMPLE_PAST)
						svVerb.push_back(gv->m_strWords[cws.m_personIndex+6]); // Prädikat
					vv.push_back(svVerb);
			//	}
			}
			else
				TRACE("gv==NULL\n");
		}
		if(cws.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(cws.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanSubSentence,germanObject); // Subjekt
		if(!cws.m_bBe)
			if(cws.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		if(cws.m_Object.m_vecEnumerationElement.size()>0) // Objekt
		{
			std::vector<CStringVector> vvObject=TranslateObject2(cws.m_Object);
			Append(vv,vvObject); // Objekt
		}
		if(cws.m_bTense==PRESENT_PERFECT)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(cws.m_strPredicate);
			sv.push_back(gv->m_strWords[13]);
			vv.push_back(sv);
		}
		for(int i=0;i<cws.m_vecEnglishAdverb.size();i++)
		{
			if(cws.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(cws.m_vecEnglishAdverb.
					at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		for(i=0;i<cws.m_vecMiddleEnumerationElements.size();i++)
		{
			if(i>0)
				if((i+1)==cws.m_vecMiddleEnumerationElements.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(cws.m_vecMiddleEnumerationElements.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				//sv.push_back(subSentence.m_MiddleEnumerationElements.at(i).m_strAdverbRefersToAdjectiveOrAdverb);
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					cws.m_vecMiddleEnumerationElements.at(i).m_strAdverbRefersToAdjectiveOrAdverb);
				if(typeid(*pWord)==typeid(GermanAdverb)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdverb * ga=dynamic_cast<GermanAdverb*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strWord);
				}
				vv.push_back(sv);
			}
			if(cws.m_vecMiddleEnumerationElements.at(i).m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					cws.m_vecMiddleEnumerationElements.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
		if(cws.m_bTense==FUTURE) // wenn Zeitform Zukunft
		{
			GermanVerb * gv=GetGermanWordOfEnglishVerb(cws.m_strPredicate);
			if(gv!=NULL)
			{
				CStringVector sv;
				sv.push_back(gv->m_strWords[0]); // Infinitiv
				vv.push_back(sv);
			}
		}
	}
	if(cws.m_bWordOrder==PSO)
	{

		GermanVerb * gv=GetGermanWordOfEnglishVerb(cws.m_strPredicate);
		CStringVector sv;
		sv.push_back(gv->m_strWords[cws.m_personIndex]); // Prädikat
		vv.push_back(sv);
		if(cws.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(cws.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanSubSentence,germanObject); // Subjekt
		if(cws.m_Object.m_vecEnumerationElement.size()>0)
		{
			std::vector<CStringVector> vvObject=TranslateObject2(cws.m_Object);
			Append(vv,vvObject); // Subjekt
		}
		if(!cws.m_bBe)
			if(cws.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		for(int i=0;i<cws.m_vecEnglishAdverb.size();i++)
		{
			if(cws.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(cws.m_vecEnglishAdverb.
					at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		for(i=0;i<cws.m_vecMiddleEnumerationElements.size();i++)
		{
			if(i>0)
				if((i+1)==cws.m_vecMiddleEnumerationElements.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(cws.m_vecMiddleEnumerationElements.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				sv.push_back(cws.m_vecMiddleEnumerationElements.at(i).
					m_strAdverbRefersToAdjectiveOrAdverb);
				vv.push_back(sv);
			}
			if(cws.m_vecMiddleEnumerationElements.at(i).m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					cws.m_vecMiddleEnumerationElements.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
	}
	if(cws.m_bWordOrder==SOP)
	{
		//TRACE("satzbau==SPO\n");
		if(cws.m_Object.m_vecEnumerationElement.size()>0)
		{
			std::vector<CStringVector> vvObject=TranslateObject2(cws.m_Object);
			Append(vv,vvObject); // Subjekt
		}
		GermanVerb * gv=GetGermanWordOfEnglishVerb(cws.m_strPredicate);
		if(gv!=NULL)
		{
			int index=0;
			if((index=cws.m_personIndex*cws.m_bTense+1)<14)
			{
				CStringVector sv;
				if(cws.m_bTense==SIMPLE_PRESENT)
					sv.push_back(gv->m_strWords[cws.m_personIndex]); // Prädikat
				if(cws.m_bTense==SIMPLE_PAST)
					sv.push_back(gv->m_strWords[cws.m_personIndex+6]); // Prädikat
				vv.push_back(sv);
			}
		}
		else
			TRACE("gv==NULL\n");
		if(cws.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(cws.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanSubSentence,germanObject); // Subjekt
		if(!cws.m_bBe)
			if(cws.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		for(int i=0;i<cws.m_vecEnglishAdverb.size();i++)
		{
			if(cws.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(cws.m_vecEnglishAdverb.
					at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
	}
	for(int i=0;i<cws.m_vecEnglishAdverb.size();i++)
	{
		if(cws.m_vecEnglishAdverb.at(i).m_bType=='2')
		{
			GermanAdverb * ga=GetGermanWordOfEnglishAdverb(cws.m_vecEnglishAdverb.
				at(i).m_strWord);
			CStringVector sv;
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	for(i=0;i<cws.m_vecEnglishAdverb.size();i++)
	{
		if(cws.m_vecEnglishAdverb.at(i).m_bType=='3')
		{
			GermanAdverb * ga=GetGermanWordOfEnglishAdverb(cws.m_vecEnglishAdverb.
				at(i).m_strWord);
			CStringVector sv;
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	//germanSubSentence.push_back(TranslatePredicate(subSentence.germanSubject.at(i));
	//CStringVector germanSubject=TranslateSubject(subSentence.m_Subject);
	TRACE("CStringVector TranslateSubSentence ENDE\n");
	return vv;
}*/

//wird von TranslateMannerEnumerationElement aufgerufen
std::vector<CStringVector> TranslateComparison(const Comparison & comparison,
	int & nStartID)
{
	std::vector<CStringVector> vecvecStr;
	CStringVector vecStr;
	GermanAdjective *ga=dynamic_cast<GermanAdjective *>(comparison.
		m_pWordNodeWhereAnEnglishComperativeIsEncapsulated->m_pWordNodeNext->m_pWord);
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateSimilarity(const Similarity & \
similarity) ANFANG\n");
#endif
	if(ga)
	{
		BYTE bTmp=0;
		std::vector<CStringVector> vecvecStrObject;
		vecStr.push_back(ga->m_strComperativ);
#ifdef _DEBUG
		printf("ga->m_strComperativ: %s\n",ga->m_strComperativ);
#endif
		vecvecStr.push_back(vecStr);
		vecStr.clear();
		vecStr.push_back(_T("als"));
		vecvecStr.push_back(vecStr);
    std::vector<IntPair> vecintpair;
		vecvecStrObject=TranslateObject2(comparison.m_Object,0,0,bTmp,vecintpair,
			nStartID);
		Append(vecvecStr,vecvecStrObject);
	}
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateSimilarity(const Similarity & \
similarity) return vecvecStr\n");
#endif
	return vecvecStr;
}

std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr)
{
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr)\n");
	#endif
	std::vector <EnglishNoun> vecEN=GetEnglishNoun(enr->m_Noun);
	//if(Is
	#ifdef _DEBUG
	printf("enr->m_Noun.size(): %u\n",enr->m_Noun.size());
	{
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			printf("enr->m_Noun.at(i): %s\n",enr->m_Noun.at(i));
	}
	#endif
	std::vector<CStringVector> vv;
	//TRACE("ee.words.size(): %d\n",ee.words.size());
	BOOL bIndefiniteArticle=FALSE;
	BOOL bDefiniteArticle=FALSE;
	/*if(enr->bArticle) // Artikel
	{
		TRACE("enr->bArticle\n");
		GermanNoun * gn=GetGermanVocableOfEnglishNoun(enr->m_Noun);
		for(int i=0;i<enr->m_Noun.size();i++)
			TRACE("enr->m_Noun.at(i): %s",enr->m_Noun.at(i));
		TRACE("\ngn->singular: %s gn->plural: %s gn->article: %c %d\n",gn->singular,gn->plural,gn->article,gn->article);
		if(IsEnglishSingular(enr->m_Noun))
		{
			CStringVector sv;
			if(gn->article=='1')
				sv.push_back("der");
			if(gn->article=='2')
				sv.push_back("die");
			if(gn->article=='3')
				sv.push_back("das");
			vv.push_back(sv);
		}
		else // Mehrzahl
		{
			CStringVector sv;
			sv.push_back("die");
			vv.push_back(sv);
		}
	}*/
	if(enr->bDefiniteArticle) // bestimmter Artikel
	{
		//TRACE("ee.bArticle\n");
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		#ifdef _DEBUG
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			TRACE("enr->m_Noun.at(i): %s",enr->m_Noun.at(i));
		#endif
		TRACE("\ngn->m_strSingular: %s gn->m_strPlural: %s gn->m_bArticle: %c %d\n",
			gn->m_strSingular,gn->m_strPlural,gn->m_bArticle,gn->m_bArticle);
		if(IsEnglishSingular(enr->m_Noun))
		{
			CStringVector sv;
			if(gn->m_bArticle=='1')
				sv.push_back("der");
			if(gn->m_bArticle=='2')
				sv.push_back("die");
			if(gn->m_bArticle=='3')
				sv.push_back("das");
			vv.push_back(sv);
		}
		else // Mehrzahl
		{
			CStringVector sv;
			sv.push_back("die");
			vv.push_back(sv);
		}
	}
	// m_possessivePronoun;
	if(enr->m_possessivePronoun>0) // besitzanzeigendes Fürwort
	{
		#ifdef _DEBUG
		printf("enr->m_possessivePronoun: %u\n",enr->m_possessivePronoun);
		for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
		{
			TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
		}
		#endif
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		CStringVector strVec=TranslatePossessivePronoun2(enr->m_possessivePronoun);
		for(DWORD i=0;i<strVec.size();i++)
		{
			if(IsEnglishSingular(enr->m_Noun))
			{
				/*if(vecEN.at(0).m_TranslationType==0)
				{
					sv.push_back(gn.singular);
				}*/
				if(vecEN.at(0).m_bTranslationType==0)
				{
					if(gn->m_bArticle=='2') // Artikel: die, also weiblich
						strVec.at(i)+="e"; // ein 'e' an den String anhängen (deinE Mutter)
				}
				if(vecEN.at(0).m_bTranslationType==1) // m_TranslationType==1 bedeutet, dass
				// ein Wort, das im Englischen im Singular steht, im Deutschen im Plural ist
				// (Beispiel: the information -> die Informationen)
				{
					strVec.at(i)+="e"; // also Plural (z.B. deinE Informationen)
					//sv.push_back(gn.plural);
				}
			}
			if(IsEnglishPlural(enr->m_Noun))
			{
				if(vecEN.at(0).m_bTranslationType==2) // m_TranslationType==1 bedeutet, dass
				// ein Wort, das im Englischen im Plural steht, im Deutschen im Singular ist
				// (Beispiel: the scissors -> die Schere)
				{
					if(gn->m_bArticle=='2') // Artikel: die, also weiblich
						strVec.at(i)+="e"; // ein 'e' an den String anhängen (deinE Schere)
					//sv.push_back(gn.singular);
				}
			}
			//else // Substantiv ist Plural
			//	strVec.at(i)+="e";
		}
		vv.push_back(strVec);
	}
	for(DWORD i=0;i<enr->m_Adjectives.size();i++) // Adjektive
	{
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		/*if(!ee.bArticle)
		{
			CStringVector sv;
			if(gn->article=='1')
				sv.push_back("der");
			if(gn->article=='2')
				sv.push_back("die");
			if(gn->article=='3')
				sv.push_back("das");
			vv.push_back(sv);
		}*/
		GermanAdjective * ga=GetGermanWordOfEnglishAdjective(enr->m_Adjectives.at(i));
		CStringVector sv;
		if(IsEnglishSingular(enr->m_Noun))
			sv.push_back(ga->m_strWortstamm+"e");
		else
			sv.push_back(ga->m_strWortstamm+"en");
		vv.push_back(sv);
	}
	if(enr->m_personalPronoun)
	{
		CStringVector sv;
		if(enr->m_Noun.at(0)=="I")
			sv.push_back("ich");
		if(enr->m_Noun.at(0)=="you")
		{
			//Option o;
			sv.push_back("du");
			sv.push_back("ihr");
			sv.push_back("sie");
			//sv.index=0;
			//vv.push_back(sv);
		}
		if(enr->m_Noun.at(0)=="he")
			sv.push_back("er");
		if(enr->m_Noun.at(0)=="she")
			sv.push_back("sie");
		if(enr->m_Noun.at(0)=="it")
			sv.push_back("es");
		if(enr->m_Noun.at(0)=="we")
			sv.push_back("wir");
		if(enr->m_Noun.at(0)=="they")
			sv.push_back("sie");
		vv.push_back(sv);
	}
	else // sonst: Substantiv
	{
		#ifdef _DEBUG
		printf("Substantiv!\n");
		//CStringVector sv;
		{
			for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
			{
				TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
			}
		}
		#endif
		if(vecEN.size()>0) // wenn der Vektor mind. 1 Element enthält
		{
			CStringVector sv,sv2;
			std::vector <GermanNoun> vecGermanNoun=GetGermanWordOfEnglishNoun2(
				enr->m_Noun);
			for(DWORD dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
			{
				TRACE("vecGermanNoun\n");
				GermanNoun gn=vecGermanNoun.at(dwRun);
				if(IsEnglishSingular(enr->m_Noun)) // wenn das englische Nomen Singular ist
				{
					if(vecEN.at(0).m_bTranslationType==0)
					{
						/*if(gn.article=='1')
							sv2.push_back(_T("der"));
						else if(gn.article=='2')
							sv2.push_back(_T("die"));
						else if(gn.article=='3')
							sv2.push_back(_T("das"));
						else 
							sv2.push_back(_T("das"));*/
						sv.push_back(gn.m_strSingular);
					}
					if(vecEN.at(0).m_bTranslationType==1) // m_TranslationType==1 bedeutet, dass
					// ein Wort, das im Englischen im Singular steht, im Deutschen im Plural ist
					// (Beispiel: the information -> die Informationen)
					{
						//sv2.push_back(_T("die"));
						sv.push_back(gn.m_strPlural);
					}
				}
				else
				{
					if(vecEN.at(0).m_bTranslationType==0)
					{
						//sv2.push_back(_T("die"));
						sv.push_back(gn.m_strPlural);
					}
					if(vecEN.at(0).m_bTranslationType==2) // m_TranslationType==1 bedeutet, dass
					// ein Wort, das im Englischen im Plural steht, im Deutschen im Singular ist
					// (Beispiel: the scissors -> die Schere)
					{
						/*if(gn.article=='1')
							sv2.push_back(_T("der"));
						else if(gn.article=='2')
							sv2.push_back(_T("die"));
						else if(gn.article=='3')
							sv2.push_back(_T("das"));
						else 
							sv2.push_back(_T("das"));*/
						sv.push_back(gn.m_strSingular);
					}
				}
			}
			//vv.push_back(sv2);
			vv.push_back(sv);
		}
/*		std::vector <GermanNoun> vecGermanNoun=GetGermanVocableOfEnglishNoun2(enr->m_Noun);
		for(dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
		{
			TRACE("vecGermanNoun\n");
			GermanNoun gn=vecGermanNoun.at(dwRun);
			if(IsEnglishSingular(enr->m_Noun))
				sv.push_back(gn.singular);
			else
				sv.push_back(gn.plural);
		}*/
			/*if(IsEnglishSingular(ee.m_Noun))
				sv.push_back(gn->singular);
			else
				sv.push_back(gn->plural);*/
		//vv.push_back(sv);
	}
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr) ENDE\n");
	#endif
	return vv;
}

std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,BYTE Case)
{ // Case: der Fall: kann 0=kein 1=3.  oder 2=4. Fall sein
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,\
BYTE Case) ANFANG\n");
	printf("Case: %u\n",Case);
	#endif
	std::vector <EnglishNoun> vecEN=GetEnglishNoun(enr->m_Noun);
	#ifdef _DEBUG
	printf("enr->m_Noun.size(): %u\n",enr->m_Noun.size());
	{
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			printf("enr->m_Noun.at(i): %s\n",enr->m_Noun.at(i));
	}
	#endif
	std::vector<CStringVector> vv;
	//TRACE("ee.words.size(): %d\n",ee.words.size());
	BOOL bIndefiniteArticle=FALSE;
	BOOL bDefiniteArticle=FALSE;
	if(enr->bDefiniteArticle) // bestimmter Artikel
	{
		//TRACE("ee.bArticle\n");
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		#ifdef _DEBUG
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			TRACE("enr->m_Noun.at(i): %s",enr->m_Noun.at(i));
		#endif
		if(IsEnglishSingular(enr->m_Noun))
		{
			CStringVector sv;
			if(gn->m_bArticle=='1')
				sv.push_back("der");
			if(gn->m_bArticle=='2')
				sv.push_back("die");
			if(gn->m_bArticle=='3')
				sv.push_back("das");
			vv.push_back(sv);
		}
		else // Mehrzahl
		{
			CStringVector sv;
			sv.push_back("die");
			vv.push_back(sv);
		}
	}
	if(enr->bIndefiniteArticle) // englischer unbestimmter Artikel ('a' oder 'an')
	{
		//TRACE("ee.bArticle\n");
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			TRACE("enr->m_Noun.at(i): %s",enr->m_Noun.at(i));
		if(IsEnglishSingular(enr->m_Noun))
		{
			#ifdef _DEBUG
			printf("Case: %u\n",Case);
			#endif
			if(Case==0) // kein Fall
			{
				CStringVector sv;
				if(gn->m_bArticle=='1')
					sv.push_back("ein");
				if(gn->m_bArticle=='2')
					sv.push_back("eine");
				if(gn->m_bArticle=='3')
					sv.push_back("ein");
				vv.push_back(sv);
			}
			if(Case==4) // 3. Fall
			{
				CStringVector sv;
				if(gn->m_bArticle=='1')
					sv.push_back("einem");
				if(gn->m_bArticle=='2')
					sv.push_back("einem");
				if(gn->m_bArticle=='3')
					sv.push_back("einer");
				vv.push_back(sv);
			}
			if(Case==3) // 4. Fall
			{
				CStringVector sv;
				if(gn->m_bArticle=='1')
					sv.push_back("einen");
				if(gn->m_bArticle=='2')
					sv.push_back("eine");
				if(gn->m_bArticle=='3')
					sv.push_back("ein");
				vv.push_back(sv);
			}
		}
		if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
		{
			GermanNoun * gn=
				GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
				enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				m_pWordNode);
			EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
				m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				m_pWordNode->m_pWord);
			if(en && gn)
			{
				#ifdef _DEBUG
				printf("en && gn\n");
				#endif
				CStringVector sv;
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
					m_bSingular)
				{
					if(en->m_bTranslationType==0)
					{
						if(Case==0) // kein Fall
						{
							CStringVector sv;
							if(gn->m_bArticle=='1') // männlich (Artikel=='der')
								sv.push_back("ein"); // I am a man. -> Ich bin ein Mann.
							if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
								sv.push_back("eine"); // I am a woman. -> Ich bin eine Frau.
							if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
								sv.push_back("ein"); // I am a child. -> Ich bin ein Kind.
							vv.push_back(sv);
						}
						if(Case==1) // 3. Fall
						{
							CStringVector sv;
							if(gn->m_bArticle=='1') // männlich (Artikel=='der')
								sv.push_back("einem"); // I trust a man. -> Ich vertraue einem Mann.
							if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
								sv.push_back("einer"); // I trust a woman. -> Ich vertraue einer Frau.
							if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
								sv.push_back("einem"); // I trust a child. -> Ich vertraue einem Kind.
							vv.push_back(sv);
						}
						if(Case==2) // 4. Fall
						{
							CStringVector sv;
							if(gn->m_bArticle=='1') // männlich (Artikel=='der')
								sv.push_back("einen"); // I like a man. -> Ich mag einen Mann.
							if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
								sv.push_back("eine"); // I like a woman. -> Ich mag eine Mann.
							if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
								sv.push_back("ein"); // I like a man. -> Ich mag ein Kind.
							vv.push_back(sv);
						}
					}
				}
			}
		}
	}
	// m_possessivePronoun;
	if(enr->m_possessivePronoun>0) // besitzanzeigendes Fürwort
	{
		#ifdef _DEBUG
		printf("enr->m_possessivePronoun: %u\n",enr->m_possessivePronoun);
		for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
		{
			TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
		}
		#endif
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		CStringVector strVec=TranslatePossessivePronoun2(enr->m_possessivePronoun);
		for(DWORD i=0;i<strVec.size();i++)
		{
			if(IsEnglishSingular(enr->m_Noun))
			{
				if(vecEN.at(0).m_bTranslationType==0)
				{
					if(gn->m_bArticle=='2') // Artikel: die, also weiblich
						strVec.at(i)+="e"; // ein 'e' an den String anhängen (deinE Mutter)
				}
				if(vecEN.at(0).m_bTranslationType==1) // m_TranslationType==1 bedeutet, dass
				// ein Wort, das im Englischen im Singular steht, im Deutschen im Plural ist
				// (Beispiel: the information -> die Informationen)
				{
					strVec.at(i)+="e"; // also Plural (z.B. deinE Informationen)
				}
			}
			if(IsEnglishPlural(enr->m_Noun))
			{
				if(vecEN.at(0).m_bTranslationType==0) // im Deutschen: Plural
				{ //
					strVec.at(i)+="e"; // ein 'e' an den String anhängen (deinE Schulen)
				}
				if(vecEN.at(0).m_bTranslationType==2) // m_TranslationType==1 bedeutet, dass
				// ein Wort, das im Englischen im Plural steht, im Deutschen im Singular ist
				// (Beispiel: the scissors -> die Schere)
				{ // nur bei weiblichen Nomen wird im Deutschen ein 'e' angehängt
					if(gn->m_bArticle=='2') // Artikel: die, also weiblich
						strVec.at(i)+="e"; // ein 'e' an den String anhängen (deinE Schere)
				}
			}
		}
		vv.push_back(strVec);
	}
	if(enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated)
	{
		#ifdef _DEBUG
		printf("enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated\n");
		#endif
		if(enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated->
			m_pWordNodeNext)
		{
			GermanPronoun * gp=dynamic_cast<GermanPronoun*>(enr->
				m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			if(gp)
			{
				#ifdef _DEBUG
				printf("gp\n");
				#endif
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
				{
					GermanNoun * gn=
						GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
						enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
						m_pWordNode);
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
						m_pWordNode->m_pWord);
					if(en && gn)
					{
						#ifdef _DEBUG
						printf("en && gn\n");
						#endif
						CStringVector sv;
						if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).m_bSingular)
						{
							if(en->m_bTranslationType==0)
							{
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back(gp->m_strWord+_T("en"));
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back(gp->m_strWord+_T("e"));
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back(gp->m_strWord+_T("es"));
							}
							if(en->m_bTranslationType==1) // im Deutschen: Plural
							{
								sv.push_back(gp->m_strWord+_T("e"));
							}
						}
						else // Plural
						{
							if(en->m_bTranslationType==0)
							{
								if(Case==1) // wenn 3. Fall I trust these animals. -> Ich vertraue jenEN Tieren.
									sv.push_back(gp->m_strWord+_T("en"));
								else
									sv.push_back(gp->m_strWord+_T("e"));
							}
							if(en->m_bTranslationType==2) // im Deutschen: Singular
							{
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back(gp->m_strWord+_T("en"));
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back(gp->m_strWord+_T("e"));
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back(gp->m_strWord+_T("es"));
							}
						}
						vv.push_back(sv);
					}
				}
			}
		}
	}
	for(DWORD i=0;i<enr->
		m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size();i++) // Adjektive
	{ // Adjektiv(e)
		//GermanNoun * gn=GetGermanVocableOfEnglishNoun(enr->m_Noun);
		/*if(!ee.bArticle)
		{
			CStringVector sv;
			if(gn->article=='1')
				sv.push_back("der");
			if(gn->article=='2')
				sv.push_back("die");
			if(gn->article=='3')
				sv.push_back("das");
			vv.push_back(sv);
		}*/
		if(enr->m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.
			at(i)->m_pWordNodeNext)
		{
			GermanAdjective * ga=dynamic_cast<GermanAdjective*>(enr->
				m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.at(i)->
				m_pWordNodeNext->m_pWord);
			if(ga)
			{
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
				{
					GermanNoun * gn=
						GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
						enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
						at(0).m_pWordNode);
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
						m_pWordNode->m_pWord);
					if(en && gn)
					{
						CStringVector sv;
						if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
							at(0).m_bSingular)
						{ // wenn im Englischen Singular
							if(en->m_bTranslationType==0) // wenn im Deutschen Singular
							{
								if(enr->bIndefiniteArticle)
								{
									if(Case==0) // kein Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("er")); // Er ist ein altER Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("e")); // Sie ist ein altE Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("es")); // Es ist ein altES Kind
									}
									if(Case==1) // 3. Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
									}
									if(Case==2) // 4. Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er mag einen altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("e")); // Er mag eine altE Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("es")); // Er mag ein altES Kind
									}
								}
								if(enr->bDefiniteArticle)
									sv.push_back(ga->m_strWortstamm+_T("e"));
							}
							if(en->m_bTranslationType==1) // im Deutschen: Plural
							{
								if(enr->bDefiniteArticle)
									sv.push_back(ga->m_strWortstamm+_T("en"));
								else if(enr->bIndefiniteArticle)
									sv.push_back(ga->m_strWortstamm+_T("e"));
								else
									sv.push_back(ga->m_strWortstamm+_T("e"));
							}
						}
						else // im Englischen: Plural
						{
							if(en->m_bTranslationType==0) // wenn im Deutschen Plural
							{
								if(enr->bDefiniteArticle) // bestimmter Artikel
									sv.push_back(ga->m_strWortstamm+_T("en")); // Beispiel: I like the old animals. -> Ich mag die altEN Tiere.
								else if(enr->bIndefiniteArticle) // unbestimmter Artikel
									sv.push_back(ga->m_strWortstamm+_T("e"));
								else // gar kein Artikel
								{
									if(Case==1) // 3. Fall
										sv.push_back(ga->m_strWortstamm+_T("en")); // Beispiel: I trust old animals. -> Ich vertraue altEN Tieren.
									else
										sv.push_back(ga->m_strWortstamm+_T("e")); // Beispiel: I like old animals. -> Ich mag altE Tiere.
										// 2. Beispiel: They are old animals. -> Sie sind altE Tiere.
								}
							}
							if(en->m_bTranslationType==2) // im Deutschen: Singular
							{
								if(enr->bIndefiniteArticle) // unbestimmter Artikel
								{
									if(Case==0) // kein Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("er")); // Er ist ein altER Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("e")); // Sie ist ein altE Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("es")); // Es ist ein altES Kind
									}
									if(Case==1) // 3. Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
									}
									if(Case==2) // 4. Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er mag einen altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("e")); // Er mag eine altE Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("es")); // Er mag ein altES Kind
									}
								}
								if(enr->bDefiniteArticle)
									sv.push_back(ga->m_strWortstamm+_T("e"));
							}
						}
						vv.push_back(sv);
					}
				}
			}
		}
		/*GermanAdjective * ga=GetGermanVocableOfEnglishAdjective(enr->m_Adjectives.at(i));
		CStringVector sv;
		if(IsEnglishSingular(enr->m_Noun))
			sv.push_back(ga->wortstamm+"e");
		else
			sv.push_back(ga->wortstamm+"en");*/
	}
	if(enr->m_personalPronoun)
	{
		CStringVector sv;
		if(enr->m_Noun.at(0)==_T("I"))
			sv.push_back("ich");
		if(enr->m_Noun.at(0)==_T("you"))
		{
			//Option o;
			sv.push_back("du");
			sv.push_back("ihr");
			sv.push_back("sie");
			//sv.index=0;
			//vv.push_back(sv);
		}
		if(enr->m_Noun.at(0)==_T("he"))
			sv.push_back("er");
		if(enr->m_Noun.at(0)==_T("she"))
			sv.push_back("sie");
		if(enr->m_Noun.at(0)==_T("it"))
			sv.push_back("es");
		if(enr->m_Noun.at(0)==_T("we"))
			sv.push_back("wir");
		if(enr->m_Noun.at(0)==_T("they"))
			sv.push_back("sie");
		vv.push_back(sv);
	}
	else // sonst: Substantiv
	{
		#ifdef _DEBUG
		printf("Substantiv!\n");
		#endif
		CStringVector sv;
		#ifdef _DEBUG
		{
			for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
			{
				TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
			}
		}
		#endif
		std::vector <EnglishNoun> vecEN=GetEnglishNoun(enr->m_Noun);
		if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0) // wenn der Vektor mind. 1 Element enthält
		{
			#ifdef _DEBUG
			printf("enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
			#endif
			//std::vector <GermanNoun> vecGermanNoun=GetGermanVocableOfEnglishNoun2(enr->m_Noun);
			for(DWORD dwRun=0;dwRun<enr->
				m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size();dwRun++)
			{
				#ifdef _DEBUG
				printf("Schleifenbeginn\n");
				#endif
				GermanNoun * gn=
					GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
					enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
					at(dwRun).m_pWordNode);
				TRACE("vecGermanNoun\n");
				//GermanNoun gn=vecGermanNoun.at(dwRun);
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).m_bSingular)
				{
					TRACE("Singular\n");
					#ifdef _DEBUG
					printf("Singular\n");
					#endif
					EnglishNoun * en=dynamic_cast<EnglishNoun*>
						(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
						at(dwRun).m_pWordNode->m_pWord);
					if(en!=NULL)
					{
						TRACE("en\n");
						if(en->m_bTranslationType==0)
							sv.push_back(gn->m_strSingular);
						if(en->m_bTranslationType==1)
							sv.push_back(gn->m_strPlural);
					}
				}
				else // im Englischen: Plural
				{
					TRACE("Plural\n");
					#ifdef _DEBUG
					printf("Plural\n");
					#endif
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
						m_pWordNode->m_pWord);
					if(en!=NULL)
					{
						#ifdef _DEBUG
						printf("en\n");
						printf("en->m_bTranslationType: %u\n",en->m_bTranslationType);
						TRACE("en\n");
						if(gn)
						{
							printf("gn\n");
							printf("gn->m_strPlural: %s\n",gn->m_strPlural);
						}
						#endif
						if(en->m_bTranslationType==0)
						{
							if(Case==1 && gn->m_strPlural.GetLength()>0) // 3. Fall
								if(gn->m_strPlural.Right(1)=="e") // wenn letztes Zeichen gleich 'e'
								{
									sv.push_back(gn->m_strPlural+_T("n")); // Beispiel: I trust old animals. -> Ich vertraue altEN Tieren.
									goto printf;
								}
							sv.push_back(gn->m_strPlural);
printf:
							#ifdef _DEBUG
							printf("nach: sv.push_back(gn->m_strPlural);\n");
							#endif
							;
						}
						if(en->m_bTranslationType==2)
							sv.push_back(gn->m_strSingular);
					}
				}
			}
			vv.push_back(sv);
			#ifdef _DEBUG
			printf("nach: vv.push_back(sv);\n");
			#endif
		}
		#ifdef _DEBUG
		else
		{
			printf("!enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
		}
		#endif
			/*if(IsEnglishSingular(ee.m_Noun))
				sv.push_back(gn->singular);
			else
				sv.push_back(gn->plural);*/
		//vv.push_back(sv);
	}
	/*for(DWORD dwRun=0;dwRun<vv.size();dwRun++)
	{
		//CStringVector sv=
		TRACE("vv.at(dwRun).at(0): %s\n",vv.at(dwRun).at(0));
	}*/
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,\
BYTE Case) ENDE\n");
	#endif
	return vv;
}

std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,BYTE Case,
	BOOL bNegation)
{ // Case: der Fall: kann 0=kein 1=3.  oder 2=4. Fall sein
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,BYTE Case,BOOL bNegation) ANFANG\n");
	TRACE("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,BYTE Case,BOOL bNegation) ANFANG\n");
	printf("Case: %u\n",Case);
	#endif
	std::vector <EnglishNoun> vecEN=GetEnglishNoun(enr->m_Noun);
	#ifdef _DEBUG
	printf("enr->m_Noun.size(): %u\n",enr->m_Noun.size());
	{
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			printf("enr->m_Noun.at(i): %s\n",enr->m_Noun.at(i));
	}
	#endif
	std::vector<CStringVector> vv;
	//TRACE("ee.words.size(): %d\n",ee.words.size());
	BOOL bIndefiniteArticle=FALSE;
	BOOL bDefiniteArticle=FALSE;
	if(enr->bDefiniteArticle) // englischer bestimmter Artikel ('the')
	{
		//TRACE("ee.bArticle\n");
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		#ifdef _DEBUG
		{
			for(DWORD i=0;i<enr->m_Noun.size();i++)
				TRACE("enr->m_Noun.at(i): %s",enr->m_Noun.at(i));
		}
		#endif
		if(IsEnglishSingular(enr->m_Noun))
		{
			CStringVector sv;
			if(gn->m_bArticle=='1')
				sv.push_back("der");
			if(gn->m_bArticle=='2')
				sv.push_back("die");
			if(gn->m_bArticle=='3')
				sv.push_back("das");
			vv.push_back(sv);
		}
		else // Mehrzahl
		{
			CStringVector sv;
			sv.push_back("die");
			vv.push_back(sv);
		}
	}
	if(enr->bIndefiniteArticle) // englischer unbestimmter Artikel ('a' oder 'an')
	{
		if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
		{
			GermanNoun * gn=
				GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
				enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				m_pWordNode);
			EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
				m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				m_pWordNode->m_pWord);
			if(en && gn)
			{
				#ifdef _DEBUG
				printf("en && gn\n");
				#endif
				CStringVector sv;
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
					m_bSingular)
				{
					if(en->m_bTranslationType==0)
					{
						if(bNegation) // Negation/Verneinung (Satz enthielt 'not')
						{
							if(Case==0) // kein Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("kein"); // I am a man. -> Ich bin ein Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("keine"); // I am a woman. -> Ich bin eine Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("kein"); // I am a child. -> Ich bin ein Kind.
								vv.push_back(sv);
							}
							if(Case==1) // 3. Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("keinem"); // I trust a man. -> Ich vertraue einem Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("keiner"); // I trust a woman. -> Ich vertraue einer Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("keinem"); // I trust a child. -> Ich vertraue einem Kind.
								vv.push_back(sv);
							}
							if(Case==2) // 4. Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("keinen"); // I like a man. -> Ich mag einen Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("keine"); // I like a woman. -> Ich mag eine Mann.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("kein"); // I like a man. -> Ich mag ein Kind.
								vv.push_back(sv);
							}
						}
						else // keine Negation/Verneinung (kein 'not')
						{
							if(Case==0) // kein Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("ein"); // I am a man. -> Ich bin ein Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("eine"); // I am a woman. -> Ich bin eine Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("ein"); // I am a child. -> Ich bin ein Kind.
								vv.push_back(sv);
							}
							if(Case==1) // 3. Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("einem"); // I trust a man. -> Ich vertraue einem Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("einer"); // I trust a woman. -> Ich vertraue einer Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("einem"); // I trust a child. -> Ich vertraue einem Kind.
								vv.push_back(sv);
							}
							if(Case==2) // 4. Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("einen"); // I like a man. -> Ich mag einen Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("eine"); // I like a woman. -> Ich mag eine Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("ein"); // I like a man. -> Ich mag ein Kind.
								vv.push_back(sv);
							}
						}
					}
				}
			}
		}
	}
	// m_possessivePronoun;
	if(enr->m_possessivePronoun>0) // besitzanzeigendes Fürwort
	{
		#ifdef _DEBUG
		printf("enr->m_possessivePronoun: %u\n",enr->m_possessivePronoun);
		for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
		{
			TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
		}
		#endif
		CStringVector vecStr=TranslatePossessivePronoun2(enr->m_possessivePronoun);
		if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
		{
			for(DWORD dwRun=0;dwRun<enr->
				m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size();dwRun++)
			{
				GermanNoun * gn=
					GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
					enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
					m_pWordNode);
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
					m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
					m_pWordNode->m_pWord);
				if(en && gn)
				{
					if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
						m_bSingular)
					{
						if(en->m_bTranslationType==0)
						{
							if(Case==0) // kein Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("kein"); // I am a man. -> Ich bin ein Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("keine"); // I am a woman. -> Ich bin eine Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("kein"); // I am a child. -> Ich bin ein Kind.
								vecStr.at(dwRun)+=_T("e");
							}
							if(Case==1) // 3. Fall
							{
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									vecStr.at(dwRun)+=_T("em"); // I trust my man. -> Ich vertraue meinEM Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									vecStr.at(dwRun)+=_T("er"); // I trust my woman. -> Ich vertraue meinER Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									vecStr.at(dwRun)+=_T("em"); // I trust my child. -> Ich vertraue meinEM Kind.
							}
							if(Case==2) // 4. Fall
							{
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									vecStr.at(dwRun)+=_T("en"); // I like my man. -> Ich mag meinEN Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									vecStr.at(dwRun)+=_T("e"); // I like my woman. -> Ich mag meinE Frau.
							}
						}
						if(en->m_bTranslationType==1) // m_TranslationType==1 bedeutet, dass
						// ein Wort, das im Englischen im Singular steht, im Deutschen im Plural ist
						// (Beispiel: the information -> die Informationen)
						{
							vecStr.at(dwRun)+="e"; // also Plural (z.B. deinE Informationen)
						}
					}
					else // im Englischen: Plural
					{
						if(vecEN.at(0).m_bTranslationType==0) // im Deutschen: Plural
						{ //
							vecStr.at(dwRun)+="e"; // ein 'e' an den String anhängen (deinE Schulen)
						}
						if(vecEN.at(0).m_bTranslationType==2) // m_TranslationType==1 bedeutet, dass
						// ein Wort, das im Englischen im Plural steht, im Deutschen im Singular ist
						// (Beispiel: the scissors -> die Schere)
						{ // nur bei weiblichen Nomen wird im Deutschen ein 'e' angehängt
							if(Case==0) // kein Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("kein"); // I am a man. -> Ich bin ein Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("keine"); // I am a woman. -> Ich bin eine Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("kein"); // I am a child. -> Ich bin ein Kind.
								vecStr.at(dwRun)+=_T("e");
							}
							if(Case==1) // 3. Fall
							{
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									vecStr.at(dwRun)+=_T("em"); // I trust my man. -> Ich vertraue meinEM Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									vecStr.at(dwRun)+=_T("er"); // I trust my woman. -> Ich vertraue meinER Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									vecStr.at(dwRun)+=_T("em"); // I trust my child. -> Ich vertraue meinEM Kind.
							}
							if(Case==2) // 4. Fall
							{
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									vecStr.at(dwRun)+=_T("en"); // I like my man. -> Ich mag meinEN Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									vecStr.at(dwRun)+=_T("e"); // I like my woman. -> Ich mag meinE Frau.
							}
						}
					}
				}
			}
		}
		vv.push_back(vecStr);
	}
	if(enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated)
	{
		#ifdef _DEBUG
		printf("enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated\n");
		#endif
		if(enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated->
			m_pWordNodeNext)
		{
			GermanPronoun * gp=dynamic_cast<GermanPronoun*>(enr->
				m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			if(gp)
			{
				#ifdef _DEBUG
				printf("gp\n");
				#endif
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
				{
					GermanNoun * gn=
						GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
						enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
						m_pWordNode);
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
						m_pWordNode->m_pWord);
					if(en && gn)
					{
						#ifdef _DEBUG
						printf("en && gn\n");
						#endif
						CStringVector sv;
						if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
							m_bSingular)
						{
							if(en->m_bTranslationType==0)
							{
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back(gp->m_strWord+_T("en"));
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back(gp->m_strWord+_T("e"));
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back(gp->m_strWord+_T("es"));
							}
							if(en->m_bTranslationType==1) // im Deutschen: Plural
							{
								sv.push_back(gp->m_strWord+_T("e"));
							}
						}
						else // Plural
						{
							if(en->m_bTranslationType==0)
							{
								if(Case==1) // wenn 3. Fall I trust these animals. -> Ich vertraue jenEN Tieren.
									sv.push_back(gp->m_strWord+_T("en"));
								else
									sv.push_back(gp->m_strWord+_T("e"));
							}
							if(en->m_bTranslationType==2) // im Deutschen: Singular
							{
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back(gp->m_strWord+_T("en"));
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back(gp->m_strWord+_T("e"));
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back(gp->m_strWord+_T("es"));
							}
						}
						vv.push_back(sv);
					}
				}
			}
		}
	}
	for(DWORD i=0;i<enr->
		m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size();i++) // Adjektive
	{ // Adjektiv(e)
		//GermanNoun * gn=GetGermanVocableOfEnglishNoun(enr->m_Noun);
		/*if(!ee.bArticle)
		{
			CStringVector sv;
			if(gn->article=='1')
				sv.push_back("der");
			if(gn->article=='2')
				sv.push_back("die");
			if(gn->article=='3')
				sv.push_back("das");
			vv.push_back(sv);
		}*/
		if(enr->m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.at(i)->
			m_pWordNodeNext)
		{
			GermanAdjective * ga=dynamic_cast<GermanAdjective*>(enr->
				m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.at(i)->
				m_pWordNodeNext->m_pWord);
			if(ga)
			{
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
				{
					GermanNoun * gn=
						GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
						enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
						m_pWordNode);
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
						m_pWordNode->m_pWord);
					if(en && gn)
					{
						CStringVector sv;
						if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
							m_bSingular)
						{ // wenn im Englischen Singular
#ifdef _DEBUG
							printf("im Englischen: Singular\n");
#endif
							if(en->m_bTranslationType==0) // wenn im Deutschen Singular
							{
#ifdef _DEBUG
								printf("im Deutschen: Singular\n");
#endif

								if(enr->bIndefiniteArticle)
								{
									if(Case==0) // kein Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("er")); // Er ist ein altER Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("e")); // Sie ist ein altE Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("es")); // Es ist ein altES Kind
									}
									if(Case==1) // 3. Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
									}
									if(Case==2) // 4. Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er mag einen altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("e")); // Er mag eine altE Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("es")); // Er mag ein altES Kind
									}
								}
								if(enr->bDefiniteArticle)
									sv.push_back(ga->m_strWortstamm+_T("e"));
							}
							if(en->m_bTranslationType==1) // im Deutschen: Plural
							{
#ifdef _DEBUG
								printf("im Deutschen: Plural\n");
#endif
								if(enr->bDefiniteArticle)
								{
#ifdef _DEBUG
									printf("bestimmter Artikel\n");
#endif
									sv.push_back(ga->m_strWortstamm+_T("en"));
								}
								else if(enr->bIndefiniteArticle)
									sv.push_back(ga->m_strWortstamm+_T("e"));
								else
									sv.push_back(ga->m_strWortstamm+_T("e"));
							}
						}
						else // im Englischen: Plural
						{
							if(en->m_bTranslationType==0) // wenn im Deutschen Plural
							{
								if(enr->bDefiniteArticle) // bestimmter Artikel
									sv.push_back(ga->m_strWortstamm+_T("en")); // Beispiel: I like the old animals. -> Ich mag die altEN Tiere.
								else if(enr->bIndefiniteArticle) // unbestimmter Artikel
									sv.push_back(ga->m_strWortstamm+_T("e"));
								else // gar kein Artikel
								{
									if(Case==1) // 3. Fall
										sv.push_back(ga->m_strWortstamm+_T("en")); // Beispiel: I trust old animals. -> Ich vertraue altEN Tieren.
									else
										sv.push_back(ga->m_strWortstamm+_T("e")); // Beispiel: I like old animals. -> Ich mag altE Tiere.
										// 2. Beispiel: They are old animals. -> Sie sind altE Tiere.
								}
							}
							if(en->m_bTranslationType==2) // im Deutschen: Singular
							{
								if(enr->bIndefiniteArticle) // unbestimmter Artikel
								{
									if(Case==0) // kein Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("er")); // Er ist ein altER Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("e")); // Sie ist ein altE Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("es")); // Es ist ein altES Kind
									}
									if(Case==1) // 3. Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
									}
									if(Case==2) // 4. Fall
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er mag einen altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("e")); // Er mag eine altE Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("es")); // Er mag ein altES Kind
									}
								}
								if(enr->bDefiniteArticle)
									sv.push_back(ga->m_strWortstamm+_T("e"));
							}
						}
						vv.push_back(sv);
					}
				}
			}
		}
		/*GermanAdjective * ga=GetGermanVocableOfEnglishAdjective(enr->m_Adjectives.at(i));
		CStringVector sv;
		if(IsEnglishSingular(enr->m_Noun))
			sv.push_back(ga->wortstamm+"e");
		else
			sv.push_back(ga->wortstamm+"en");*/
	}
	if(enr->m_personalPronoun)
	{
		CStringVector sv;
		if(enr->m_Noun.at(0)=="I")
			sv.push_back("ich");
		if(enr->m_Noun.at(0)=="you")
		{
			//Option o;
			sv.push_back("du");
			sv.push_back("ihr");
			sv.push_back("sie");
			//sv.index=0;
			//vv.push_back(sv);
		}
		if(enr->m_Noun.at(0)=="he")
			sv.push_back("er");
		if(enr->m_Noun.at(0)=="she")
			sv.push_back("sie");
		if(enr->m_Noun.at(0)=="it")
			sv.push_back("es");
		if(enr->m_Noun.at(0)=="we")
			sv.push_back("wir");
		if(enr->m_Noun.at(0)=="they")
			sv.push_back("sie");
		vv.push_back(sv);
	}
	else // sonst: Substantiv
	{
		#ifdef _DEBUG
		printf("Substantiv!\n");
		#endif
		CStringVector sv;
		#ifdef _DEBUG
		{
			for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
			{
				TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
			}
		}
		#endif
		std::vector <EnglishNoun> vecEN=GetEnglishNoun(enr->m_Noun);
		if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0) // wenn der Vektor mind. 1 Element enthält
		{
			TRACE("enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
			#ifdef _DEBUG
			printf("enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
			#endif
			//std::vector <GermanNoun> vecGermanNoun=GetGermanVocableOfEnglishNoun2(enr->m_Noun);
			for(DWORD dwRun=0;dwRun<enr->
				m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size();dwRun++)
			{
				#ifdef _DEBUG
				printf("Schleifenbeginn\n");
				#endif
				GermanNoun * gn=
					GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
					enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
					m_pWordNode);
				TRACE("vecGermanNoun\n");
				//GermanNoun gn=vecGermanNoun.at(dwRun);
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
					m_bSingular)
				{
					TRACE("Singular\n");
					#ifdef _DEBUG
					printf("Singular\n");
					#endif
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
						m_pWordNode->m_pWord);
					if(en!=NULL)
					{
						TRACE("en\n");
						if(en->m_bTranslationType==0)
							sv.push_back(gn->m_strSingular);
						if(en->m_bTranslationType==1)
							sv.push_back(gn->m_strPlural);
					}
				}
				else // im Englischen: Plural
				{
					TRACE("Plural\n");
					#ifdef _DEBUG
					printf("Plural\n");
					#endif
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
						m_pWordNode->m_pWord);
					if(en!=NULL)
					{
						#ifdef _DEBUG
						printf("en\n");
						printf("en->m_bTranslationType: %u\n",en->m_bTranslationType);
						TRACE("en\n");
						if(gn)
						{
							printf("gn\n");
							printf("gn->m_strPlural: %s\n",gn->m_strPlural);
						}
						#endif
						if(en->m_bTranslationType==0)
						{
							if(Case==1 && gn->m_strPlural.GetLength()>0) // 3. Fall
								if(gn->m_strPlural.Right(1)=="e") // wenn letztes Zeichen gleich 'e'
								{
									sv.push_back(gn->m_strPlural+_T("n")); // Beispiel: I trust old animals. -> Ich vertraue altEN Tieren.
									goto printf;
								}
							sv.push_back(gn->m_strPlural);
printf:
							#ifdef _DEBUG
							printf("nach: sv.push_back(gn->plural);\n");
							#endif
							;
						}
						if(en->m_bTranslationType==2)
							sv.push_back(gn->m_strSingular);
					}
				}
			}
			vv.push_back(sv);
			#ifdef _DEBUG
			printf("nach: vv.push_back(sv);\n");
			#endif
		}
		#ifdef _DEBUG
		else
		{
			printf("!enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
		}
		#endif
			/*if(IsEnglishSingular(ee.m_Noun))
				sv.push_back(gn->singular);
			else
				sv.push_back(gn->plural);*/
		//vv.push_back(sv);
	}
	/*for(DWORD dwRun=0;dwRun<vv.size();dwRun++)
	{
		//CStringVector sv=
		TRACE("vv.at(dwRun).at(0): %s\n",vv.at(dwRun).at(0));
	}*/
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,\
BYTE Case,BOOL bNegation) ENDE\n");
	#endif
	return vv;
}

//wird von std::vector<CStringVector> TranslatePlaceEnumerationElement(
//const EnumerationElement & ee) aufgerufen
std::vector<CStringVector> TranslateENR(
  EnglishNounRecord * enr,
	BYTE Case,
  bool bNegation,
  bool bTranslateAsObject,
  BYTE & personIndex,
	BYTE & bFirstTokenInSentence,
  BYTE & bGermanNounGender,
  int & nStartID,
  IVocabularyInMainMem & ivocabularyinmainmem)
{ // Case: der Fall: kann 0=kein   1=3.  oder 2=4. Fall sein
  //std::string strUnflected;//nur Wortstamm (eines Artikels oder Pronomens)
  std::vector<std::string> vecstrUnflected;//nur Wortstamm (eines Artikels oder Pronomens)
	std::vector<CStringVector> vv;
	BOOL bIndefiniteArticle=FALSE;
	BOOL bDefiniteArticle=FALSE;
  //Notlösung for design problem(Case should not be "0"): 
  //Avoid missing assignment of pointer to array.
  if(Case==0)
    Case=NOMINATIVE;
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,\
BYTE Case,BOOL bNegation,BOOL bTranslateAsObject,BYTE & bFirstTokenInSente\
nce,int & nStartID) ANFANG\n");
	TRACE("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,BYTE Case,BOOL bNegation,BOOL bTranslateAsObject) ANFANG\n");
	printf("Case: %u bFirstTokenInSentence: %u\n",Case,bFirstTokenInSentence);
#endif
	std::vector <EnglishNoun> vecEN=GetEnglishNoun(enr->m_Noun);
#ifdef _DEBUG
	printf("enr->m_Noun.size(): %u\n",enr->m_Noun.size());
	{
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			printf("enr->m_Noun.at(i): %s\n",enr->m_Noun.at(i));
	}
#endif
	if(enr->bDefiniteArticle) // englischer bestimmter Artikel ('the')
	{
    bool bSingularInGerman;
		CStringVector sv;
#ifdef _DEBUG
		printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
#endif
    std::string * parstrInflectionSuffix;

    if(bTranslateAsObject)
      parstrInflectionSuffix=g_arstrInflectedDefiniteArticleForDative;
    else
      switch(Case)
      {
      case NOMINATIVE:
        parstrInflectionSuffix=g_arstrInflectedDefiniteArticleForNominative;
        break;
      case DATIVE:
        parstrInflectionSuffix=g_arstrInflectedDefiniteArticleForDative;
        break;
      case ACCUSATIVE:
        parstrInflectionSuffix=g_arstrInflectedDefiniteArticleForAccusative;
        break;
      case GENITIVE:
        parstrInflectionSuffix=g_arstrInflectedDefiniteArticleForGenitive;
        break;
      }//switch
		//if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
		//{
		//	GermanNoun * gn=
		//		GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
		//		enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
		//		m_pWordNode);
		//	EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
		//		m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
		//		m_pWordNode->m_pWord);

    //PointerToWordNodeWhereAnEnglishNounIsEncapsulated
    //  pointertowordnodewhereanenglishnounisencapsulated;

    //Jede(s) Wort(art) muss leider in einer eigenen for-Schleife durchlaufen
    //werden, da: wenn ein englisches Nomen im, dass mehrere Übersetzungen liefert
    //z.B. (fan->Lüfter, Anhänger),  dann sollen die unterschiedlichen Flexions-Formen
    //für Adjekte usw. UNTEREINANDER in einer Liste angezeigt werden.
    //for(DWORD dwEnglishNounIndex=0;dwEnglishNounIndex<enr->
	   // m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size();
    //  dwEnglishNounIndex++)
    WORD wNumberOfEnglishNouns = ivocabularyinmainmem.GetNumberOfNouns() ;
    for(WORD wEnglishNounIndex ;wEnglishNounIndex<wNumberOfEnglishNouns ;
      wEnglishNounIndex++)
    {
      bSingularInGerman=true;
   //   pointertowordnodewhereanenglishnounisencapsulated=
   //     enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
   //       at(dwEnglishNounIndex);
	  //  GermanNoun * gn=
		 //   GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
		 //     pointertowordnodewhereanenglishnounisencapsulated.m_pWordNode);
	  //  EnglishNoun * en=dynamic_cast<EnglishNoun*>
   //     (pointertowordnodewhereanenglishnounisencapsulated.m_pWordNode->m_pWord);
			//if(en && gn)
			{
				#ifdef _DEBUG
				printf("en && gn\n");
				printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
				#endif
				//wenn das Nomen im Englischen im Singular ist
				//if(pointertowordnodewhereanenglishnounisencapsulated.
				//	m_bSingular)
        if(ivocabularyinmainmem.IsSingular() )
				{
				#ifdef _DEBUG
					printf("enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsul\
ated.at(0).m_bSingular\n");
					printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
				#endif
          BYTE byTranslationType = ivocabularyinmainmem.GetTranslationType() ;
					//if(en->m_bTranslationType==
          if( byTranslationType ==
            SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN)
					{
						personIndex=2;
					}
					// wenn das Substantiv im Deutschen im Plural steht
					//if(en->m_bTranslationType==SINGULAR_IN_ENGLISH_PLURAL_IN_GERMAN)
          if(byTranslationType == SINGULAR_IN_ENGLISH_PLURAL_IN_GERMAN)
					{
						personIndex=5;

            bSingularInGerman=false;
					}
				}
				else// sonst: im Englischen: Plural
				{
				#ifdef _DEBUG
					printf("!enr->m_vecPointerToWordNodeWhereAnEnglishNounIs\
Encapsulated.at(0).m_bSingular\n");
				#endif
          BYTE byTranslationType = ivocabularyinmainmem.GetTranslationType() ;
					//if(en->m_bTranslationType==
          if( byTranslationType == 
            SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN)
					{ // im Deutschen auch Plural
						personIndex=5;

            bSingularInGerman=false;
					}
					// im Deutschen: Singular
					//if(en->m_bTranslationType==PLURAL_IN_ENGLISH_SINGULAR_IN_GERMAN)
          if(byTranslationType==PLURAL_IN_ENGLISH_SINGULAR_IN_GERMAN )
					{
						personIndex=2;
					}// im Deutschen: Singular
				}//sonst: im Englischen: Plural
      if(bSingularInGerman)
      {
        if(bFirstTokenInSentence)
        {
          CString str=parstrInflectionSuffix[
            VALUE_FOR_GENDER_TO_ARRAY_INDEX(//gn->m_bArticle)
              ivocabularyinmainmem.GetGermanArticle() )
                                            ].c_str();
          MakeFirstLetterUpper(str);
          sv.push_back(str);
        }
        else
          sv.push_back(CString(parstrInflectionSuffix[
            VALUE_FOR_GENDER_TO_ARRAY_INDEX(//gn->m_bArticle)
              ivocabularyinmainmem.GetGermanArticle() )
                                                     ].c_str()
                              )
            );
      }//if(bSingularInGerman)
      else
        if(bFirstTokenInSentence)
        {
          CString str = parstrInflectionSuffix[
            GERMAN_INFLECTION_ARRAY_INDEX_FOR_PLURAL].c_str();
          MakeFirstLetterUpper(str);
          sv.push_back(str);
        }
        else
          //vecstrUnflected.at(byIndexOfUnreflectedStringVector)+
          //  parstrInflectionSuffix[3];
          sv.push_back(CString(
            (parstrInflectionSuffix[
              GERMAN_INFLECTION_ARRAY_INDEX_FOR_PLURAL]).c_str()
                              )
            );
			}//if(en && gn)
		}//for-loop for English nouns with identical spelling.
		vv.push_back(sv);

    if(bFirstTokenInSentence)
      //Important: Negate AFTER the loop iterating through nouns
			bFirstTokenInSentence=FALSE;
	}//if(enr->bDefiniteArticle) // englischer bestimmter Artikel ('the')

  //Die folgenden 3 Wortarten können vor einem Substantiv stehen, werden in
  //gleicher Weise flektiert und schließen sich gegenseitig aus(es
  //kann also immer nur 1 dieser Wortarten da stehen).
  if(enr->bIndefiniteArticle) // englischer unbestimmter Artikel ('a' oder 'an')
	{
    if(bNegation)
      //strUnflected=_T("ein");
      vecstrUnflected.push_back(_T("kein"));
    else
      //strUnflected=_T("kein");
      vecstrUnflected.push_back(_T("ein"));
  }
	if(enr->m_possessivePronoun>0) // besitzanzeigendes Fürwort
	{
    //CStringVector vecstrGermanPossessivePronoun;
	#ifdef _DEBUG
		printf("enr->m_possessivePronoun: %u\n",enr->m_possessivePronoun);
		for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
		{
			TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
		}
	#endif
    //CStringVector vecstrGermanPossessivePronoun;
	  vecstrUnflected=TranslatePossessivePronoun3(
      enr->m_possessivePronoun);
  }//if(enr->m_possessivePronoun>0) // besitzanzeigendes Fürwort

  if(enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated)
	{
		#ifdef _DEBUG
		printf("enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated\n");
		#endif
		if(enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated->
			m_pWordNodeNext)
		{
			GermanPronoun * gp=dynamic_cast<GermanPronoun*>(enr->
				m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			if(gp)
			{
        //strUnflected=gp->m_strWord;
        vecstrUnflected.push_back(gp->m_strWord.GetBuffer());
      }
    }
  }//if(enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated)

  //if(!strUnflected.empty())
  if(!vecstrUnflected.empty())
  {
    bool bSingularInGerman=true;
		CStringVector sv;
    std::string * parstrInflectionSuffix;
	  if(bFirstTokenInSentence)
      for(BYTE byIndexOfUnreflectedStringVector=0;
        byIndexOfUnreflectedStringVector<vecstrUnflected.size();
        ++byIndexOfUnreflectedStringVector)
        //Make upper here because if more translations for a noun,
        //this must be just once.
		    MakeFirstLetterUpper(vecstrUnflected.at(
          byIndexOfUnreflectedStringVector));

    if(bTranslateAsObject)
      parstrInflectionSuffix=g_arstrInflectionSuffixForDative;
    else
      switch(Case)
      {
      case NOMINATIVE:
        parstrInflectionSuffix=g_arstrInflectionSuffixForNominative;
        break;
      case DATIVE:
        parstrInflectionSuffix=g_arstrInflectionSuffixForDative;
        break;
      case ACCUSATIVE:
        parstrInflectionSuffix=g_arstrInflectionSuffixForAccusative;
        break;
      case GENITIVE:
        parstrInflectionSuffix=g_arstrInflectionSuffixForGenitive;
        break;
      }//switch

		//if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
		//{
		//	GermanNoun * gn=
		//		GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
		//		enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
		//		m_pWordNode);
		//	EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
		//		m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
		//		m_pWordNode->m_pWord);
    PointerToWordNodeWhereAnEnglishNounIsEncapsulated
      pointertowordnodewhereanenglishnounisencapsulated;

    //Jede(s) Wort(art) muss leider in einer eigenen for-Schleife durchlaufen
    //werden, da: wenn ein englisches Nomen im, dass mehrere Übersetzungen liefert
    //z.B. (fan->Lüfter, Anhänger),  dann sollen die unterschiedlichen Flexions-Formen
    //für Adjekte usw. UNTEREINANDER in einer Liste angezeigt werden.
    for(DWORD dwEnglishNounIndex=0;dwEnglishNounIndex<enr->
	    m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size();
      dwEnglishNounIndex++)
    {
      pointertowordnodewhereanenglishnounisencapsulated=
        enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
          at(dwEnglishNounIndex);
	    GermanNoun * gn=
		    GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
          pointertowordnodewhereanenglishnounisencapsulated.m_pWordNode);
	    EnglishNoun * en=dynamic_cast<EnglishNoun*>
        (pointertowordnodewhereanenglishnounisencapsulated.m_pWordNode->m_pWord);
			if(en && gn)
			{
				#ifdef _DEBUG
				printf("en && gn\n");
				#endif
				if(pointertowordnodewhereanenglishnounisencapsulated.m_bSingular)
				{
					if(en->m_bTranslationType==
            SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN)
					{
						personIndex=2;

          }//if(en->m_bTranslationType==SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN)
          else
            bSingularInGerman=false;
				}//if(pointertowordnodewhereanenglishnounisencapsulated.m_bSingular)
        else
          if(en->m_bTranslationType==
            SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN)
              bSingularInGerman=false;
			}//if(en && gn)
      if(bSingularInGerman)
        for(BYTE byIndexOfUnreflectedStringVector=0;
          byIndexOfUnreflectedStringVector<vecstrUnflected.size();
          ++byIndexOfUnreflectedStringVector)
        {
          
          sv.push_back(CString(
            //Hier erfolgt die Zusammensatzung des Worten plus des
            //deklinierten Endes.
            (vecstrUnflected.at(byIndexOfUnreflectedStringVector)+
            parstrInflectionSuffix[VALUE_FOR_GENDER_TO_ARRAY_INDEX(
            gn->m_bArticle)]).c_str()
                              )
            );
        }
      else
        for(BYTE byIndexOfUnreflectedStringVector=0;
          byIndexOfUnreflectedStringVector<vecstrUnflected.size();
          ++byIndexOfUnreflectedStringVector)
        {
          //vecstrUnflected.at(byIndexOfUnreflectedStringVector)+
          //  parstrInflectionSuffix[3];
          sv.push_back(CString(
            //Hier erfolgt die Zusammensatzung des Worten plus des
            //deklinierten Endes.
            (vecstrUnflected.at(byIndexOfUnreflectedStringVector)+
            parstrInflectionSuffix[3]).c_str()
                              )
            );
        }
			//if(bFirstTokenInSentence)
			//	MakeFirstLetterUpper(vecstrUnflected.at();
      //sv.push_back(CString(vecstrUnflected));
		}//for-loop
		vv.push_back(sv);

		if(bFirstTokenInSentence)
      //Important: Negate AFTER the loop iterating through nouns
			bFirstTokenInSentence=FALSE;
	//}//if(enr->bIndefiniteArticle) // englischer unbestimmter Artikel ('a' oder 'an')
  }//if(!strUnflected.empty())

	// m_possessivePronoun;
	
  //Wenn das Aufzählungselement (ein) Adverb(ien) enthält, die sich auf 
	//ein Adjektiv oder Adverb beziehen.
	if(enr->
		m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
	{
		#ifdef _DEBUG
		printf("enr->m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdj\
ectiveOrAdverbIsEncapsulated\n");
		#endif
		CStringVector sv;
		GermanAdverb * ga=dynamic_cast<GermanAdverb*>(enr->
			m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated->
			m_pWordNodeNext->m_pWord);
		if(ga)
		{
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	//Adjektive
	for(DWORD i=0;i<enr->
		m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size();i++) // Adjektive
	{ // Adjektiv(e)
		#ifdef _DEBUG
		printf("Adjektive(e)\n");
		#endif
		if(enr->m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.at(i)->
			m_pWordNodeNext)
		{
			GermanAdjective * ga=dynamic_cast<GermanAdjective*>(enr->
				m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.at(i)->
				m_pWordNodeNext->m_pWord);
			if(ga)
			{
				CStringVector sv;
				//if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
				//{
				//	GermanNoun * gn=
				//		GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
				//		enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				//		m_pWordNode);
				//	EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
				//		m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				//		m_pWordNode->m_pWord);
        PointerToWordNodeWhereAnEnglishNounIsEncapsulated
          pointertowordnodewhereanenglishnounisencapsulated;
        //Jede(s) Wort(art) muss leider in einer eigenen for-Schleife durchlaufen
        //werden, da: wenn ein englisches Nomen im, dass mehrere Übersetzungen liefert
        //z.B. (fan->Lüfter, Anhänger),  dann sollen die unterschiedlichen Flexions-Formen
        //für Adjekte usw. UNTEREINANDER in einer Liste angezeigt werden.
			  for(DWORD dwEnglishNounIndex=0;dwEnglishNounIndex<enr->
				  m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size();
          dwEnglishNounIndex++)
			  {
          pointertowordnodewhereanenglishnounisencapsulated=
            enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
              at(dwEnglishNounIndex);
				  GermanNoun * gn=
					  GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
              pointertowordnodewhereanenglishnounisencapsulated.m_pWordNode);
				  EnglishNoun * en=dynamic_cast<EnglishNoun*>
            (pointertowordnodewhereanenglishnounisencapsulated.m_pWordNode->m_pWord);
					if(en && gn)
					{
						 // wenn das Substantiv im Englischen im Singular ist
						if(pointertowordnodewhereanenglishnounisencapsulated.m_bSingular)
						{
#ifdef _DEBUG
							printf("im Englischen: Singular\n");
#endif
							if(en->m_bTranslationType==0) // wenn im Deutschen auch Singular
							{
								personIndex=2;
#ifdef _DEBUG
								printf("im Deutschen: Singular\n");
#endif
                //If an article exits THIS one is (entscheidend) 
                //dekliniert ("inflected"?)
                //by the case.
								if(enr->bIndefiniteArticle)//If an indefinite article("a"/"an") 
                  //exists the noun can only be singular.
								{
									if(bTranslateAsObject)
									{
	// An old man is told a story. -> Einem altEN Mann wird eine Geschichte erzählt.
	// An old woman is told a story. -> Einer altEN Frau wird eine Geschichte erzählt.
	// An old child is told a story. -> Einem altEN Kind wird eine Geschichte erzählt.
										sv.push_back(ga->m_strWortstamm + _T("en"));
									}
									else
									{
										if(Case==NOMINATIVE) // kein Fall
										{
                      //Beispiele:
                      // Er ist ein altER Mann.
                      // Sie ist ein altE Frau.
                      // Es ist ein altES Kind.
                      sv.push_back(g_arstrAdjInflectionSuffixForNominative
                        [VALUE_FOR_GENDER_TO_ARRAY_INDEX(
                        gn->m_bArticle)].c_str() ) ;
										}
										if(Case==GENITIVE) // 2ter Fall
                    {
                      //Beispiele.
                      // "die Art eines altEN Mannes"
                      // "die Art einer altEN Frau"
                      // "die Art eines altEN Kindes"
											sv.push_back(ga->m_strWortstamm+_T("en"));
                    }
										if(Case==DATIVE) // 3. Fall
										{
                      //Beispiele.
                      // Er vertraut einem altEN Mann.
                      // Er vertraut einer altEN Frau.
                      // Er vertraut einem altEN Kind.
											sv.push_back(ga->m_strWortstamm+_T("en")); 
										}
										if(Case==ACCUSATIVE) // 4. Fall
										{
                      //Beispiele:
                      // Er mag einen altEN Mann.
                      // Er mag eine altE Frau.
                      // Er mag ein altES Kind.
                      sv.push_back(
                        g_arstrAdjInflectionSuffixForIndefArticleAccusative
                        [VALUE_FOR_GENDER_TO_ARRAY_INDEX(gn->m_bArticle)]
                        .c_str() ) ;
										}
									}
								}
								else if(enr->bDefiniteArticle) // wenn englischer bestimmter Artikel ('the')
								{
                  //ex.: "The old man is given a thing."
									if(bTranslateAsObject)
                    //ex.: "The old man is given a thing."->
                    //  "Dem altEN Mann wird ein Ding gegeben."
                    //  "Der altEN Frau wird ein Ding gegeben."
                    //  "Dem altEN Kind wird ein Ding gegeben."
										sv.push_back(ga->m_strWortstamm+_T("en"));
									else
                  {
                    switch(Case)
                    {
                      case ACCUSATIVE: // 4. Fall
										  {
                        //Beispiele:
                        // Er mag einen altEN Mann.
                        // Er mag eine altE Frau.
                        // Er mag ein altES Kind.
                        sv.push_back(
                          g_arstrAdjInflectionSuffixForDefArticleAccusative
                          [VALUE_FOR_GENDER_TO_ARRAY_INDEX(
                          gn->m_bArticle)].c_str() ) ;
										  }
                      break;
                      case GENITIVE: // 2ter Fall
                      {
                        //Beispiele.
                        // "die Art eines altEN Mannes"
                        // "die Art einer altEN Frau"
                        // "die Art eines altEN Kindes"
											  sv.push_back(ga->m_strWortstamm+_T("en"));
                      }
                      break;
                      case DATIVE: // 3. Fall
										  {
                        //Beispiele.
                        // Er vertraut einem altEN Mann.
                        // Er vertraut einer altEN Frau.
                        // Er vertraut einem altEN Kind.
											  sv.push_back(ga->m_strWortstamm+_T("en")); 
										  }
                      break ;
                      case NOMINATIVE:
                      {
                        sv.push_back(
                          g_arstrAdjInflectionSuffixForDefArticleNominative
                          [VALUE_FOR_GENDER_TO_ARRAY_INDEX(
                          gn->m_bArticle)].c_str() ) ;
                      }
                      break;
                    }//switch(Case)
          //          else
          //          //Das ist der altE Mann.
          //          //Das ist die altE Mann.
										//sv.push_back(ga->m_strWortstamm+_T("e"));
                  }
								}
								else // kein Artikel (weder bestimmt noch unbestimmt)
								{
                  //TODO implement like above
									if(bTranslateAsObject)
									{
										if(gn->m_bArticle=='1') // wenn männliches Substantiv
	// My old man is told a story. -> Meinem altEN Mann wird eine Geschichte erzählt.
											sv.push_back(ga->m_strWortstamm+_T("en"));
										if(gn->m_bArticle=='2')
	// My old woman is told a story. -> Meiner altEN Frau wird eine Geschichte erzählt.
											sv.push_back(ga->m_strWortstamm+_T("en"));
										if(gn->m_bArticle=='3')
	// My old child is told a story. -> Meinem altEN Kind wird eine Geschichte erzählt.
											sv.push_back(ga->m_strWortstamm+_T("en"));
									}
									else
									{
										if(Case==0) // kein Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("er")); // Er ist ein altER Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); // Sie ist ein altE Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); // Es ist ein altES Kind
										}
										if(Case==1) // 3. Fall
										{
                      //Beispiele:
                      // Er vertraut diesem altEN Mann.
                      // Er vertraut dieser altEN Frau.
                      // Er vertraut diesem altEN Kind.
                      // Er vertraut diesen altEN Kindern.
											sv.push_back(ga->m_strWortstamm+_T("en")); 
										}
										if(Case==2) // 4. Fall
										{
                      //Beispiele:
                      // Er mag diesen altEN Mann.
                      // Er mag diese altE Frau.
                      // Er mag dieses altE Kind.
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("en")); 
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); 
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); 
										}
									}
								}
							}
							//wenn das Substantiv im Deutschen im Plural steht
							//Beispiel: "Long hair needs washing." -> 
							//"Lange Haare müssen gewaschen werden."
							if(en->m_bTranslationType==1) // im Deutschen: Plural
							{
								personIndex=5;
#ifdef _DEBUG
								printf("im Deutschen: Plural\n");
#endif
								if(enr->bDefiniteArticle)
								{
#ifdef _DEBUG
									printf("bestimmter Artikel\n");
#endif
									sv.push_back(ga->m_strWortstamm+_T("en"));
								}
								else if(enr->bIndefiniteArticle)
									sv.push_back(ga->m_strWortstamm+_T("e"));
								else
									sv.push_back(ga->m_strWortstamm+_T("e"));
								//Wenn das Token das erste im Satz ist, dann muss der 
								//erste Buchstabe durch MakeFirstLetterUpper() groß gemacht
								//werden.
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
							}
						}
						//wenn das Substantiv im Englischen im Plural steht
						else // im Englischen: Plural
						{
							//wenn das Substantiv im Deutschen auch im Plural steht
							//Beispiel: "Big cars are ugly." -> "Große Autos sind hässlich."
							if(en->m_bTranslationType==0) // wenn im Deutschen Plural
							{
								personIndex=5;
                //Wenn bestimmter Artikel oder bes.anz.Fürwort.
								if(enr->bDefiniteArticle || enr->m_possessivePronoun>0 || 
                  enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated) 
                  //Beispiel: I like (the|my|these) old animals. -> 
                  //Ich mag (die|meine|diese) altEN Tiere.
									sv.push_back(ga->m_strWortstamm+_T("en")); 
								//else if(enr->bIndefiniteArticle) // unbestimmter Artikel
								//	sv.push_back(ga->m_strWortstamm+_T("e"));
								else // gar kein Artikel
								{
									if(Case==1) // 3. Fall
										sv.push_back(ga->m_strWortstamm+_T("en"));
									// Beispiel: I trust old animals. -> Ich vertraue altEN Tieren.
									else
										sv.push_back(ga->m_strWortstamm+_T("e"));
									// Beispiel: I like old animals. -> Ich mag altE Tiere.
									// 2. Beispiel: They are old animals. -> Sie sind altE Tiere.
								}
								//Wenn das Token das erste im Satz ist, dann muss der 
								//erste Buchstabe durch MakeFirstLetterUpper() groß gemacht
								//werden.
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
							}
							//wenn das Substantiv im Deutschen im Singular steht
							if(en->m_bTranslationType==2) // im Deutschen: Singular
							{
								personIndex=2;
								if(enr->bIndefiniteArticle) // unbestimmter Artikel
								{
									if(bTranslateAsObject)
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
									}
									else
									{
										if(Case==0) // kein Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("er")); // Er ist ein altER Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); // Sie ist ein altE Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); // Es ist ein altES Kind
										}
										if(Case==1) // 3. Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
										}
										if(Case==2) // 4. Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er mag einen altEN Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); // Er mag eine altE Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); // Er mag ein altES Kind
										}
									}
								}
								//wenn das Aufzählungselement des Subjekts einen bestimmten
								//Artikel enthält (es kam "the" vor).
								if(enr->bDefiniteArticle)
								{
									//wenn das Subjekt als Objekt übersetzt werden soll
									if(bTranslateAsObject)
										sv.push_back(ga->m_strWortstamm+_T("en"));
									//wenn das Subjekt als Subjekt übersetzt werden soll
									//Beispiel: "I like the old scissors." -> 
									//"Ich mag die altE Schere."
									else
										sv.push_back(ga->m_strWortstamm+_T("e"));
								}
							}//im Deutschen: Singular
						}//im Englischen: Plural
					}
				}//for-loop iterating all nouns with idental Rechtschreibung that
        //are different words in German: fan:Lüfter, fan:Anhänger.
			  vv.push_back(sv);
			}
		}
	}//for-loop iterating all adjectives referring to the noun.
	//wenn das Aufzählungselement des Subjekts ein Personalpronom enthält
	if(enr->m_personalPronoun)
	{
		CStringVector sv;
		if(enr->m_Noun.at(0)==_T("I"))
		{
			personIndex=0;
			sv.push_back(_T("ich"));
		}
		if(enr->m_Noun.at(0)==_T("you"))
		{
			personIndex=1;
			//Option o;
			sv.push_back(_T("du"));
			sv.push_back(_T("ihr"));
			sv.push_back(_T("sie"));
			//sv.index=0;
			//vv.push_back(sv);
		}
		if(enr->m_Noun.at(0)==_T("he"))
		{
			personIndex=2;
			sv.push_back(_T("er"));
		}
		if(enr->m_Noun.at(0)==_T("she"))
		{
			personIndex=2;
			sv.push_back(_T("sie"));
		}
		if(enr->m_Noun.at(0)==_T("it"))
		{
			personIndex=2;
			sv.push_back(_T("es"));
		}
		if(enr->m_Noun.at(0)==_T("we"))
		{
			personIndex=3;
			sv.push_back(_T("wir"));
		}
		if(enr->m_Noun.at(0)==_T("they"))
		{
			personIndex=5;
			sv.push_back(_T("sie"));
		}
		vv.push_back(sv);
	}
	else // sonst: Substantiv
	{
		#ifdef _DEBUG
		printf("Substantiv!\n");
		#endif
		CStringVector sv;
		for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
		{
			TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
		}
		std::vector <EnglishNoun> vecEN=GetEnglishNoun(enr->m_Noun);
    WORD wNumberOfEnglishNouns = ivocabularyinmainmem.GetNumberOfNouns() ;
		//Wenn der Substantiv-Vektor mind. 1 Element enthält.
		//if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
    if(wNumberOfEnglishNouns > 0 )
		{
			#ifdef _DEBUG
			printf("enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
			#endif
			//std::vector <GermanNoun> vecGermanNoun=GetGermanVocableOfEnglishNoun2(enr->m_Noun);
        //Jede(s) Wort(art) muss leider in einer eigenen for-Schleife durchlaufen
        //werden, da: wenn ein englisches Nomen im, dass mehrere Übersetzungen liefert
        //z.B. (fan->Lüfter, Anhänger),  dann sollen die unterschiedlichen Flexions-Formen
        //für Adjekte usw. UNTEREINANDER in einer Liste angezeigt werden.
      //Alle englischen Nomen mit der identischen Rechtschreibung durchgehen.
      //(I like fans.->Ich mag (Fans|Lüfter).
			//for(dwRun=0;dwRun<enr->
			//	m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size();dwRun++)
      for(WORD wEnglishNounIndex = 0 ;wEnglishNounIndex< wNumberOfEnglishNouns ;
        wEnglishNounIndex++)
			{
				#ifdef _DEBUG
				printf("Schleifenbeginn\n");
				#endif
				//GermanNoun * gn=
				//	GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
				//	enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
				//	at(dwRun).m_pWordNode);
				TRACE("vecGermanNoun\n");
				//GermanNoun gn=vecGermanNoun.at(dwRun);
				//wenn das Substantiv im Englischen im Singular steht
				//if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
				//	at(dwRun).m_bSingular)
        if( ivocabularyinmainmem.IsSingular() )
				{ // wenn im Englischen Singular
					TRACE("Singular\n");
					#ifdef _DEBUG
					printf("Singular\n");
					#endif
					//EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
					//	m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
					//	m_pWordNode->m_pWord);
					//if(en!=NULL)
					{
						TRACE("en\n");
            BYTE byTranslationType = ivocabularyinmainmem.GetTranslationType() ;
						//#ifdef _DEBUG
						//printf("en->m_strSingular: %s  en->m_strPlural: %s\n",
						//	en->m_strSingular,en->m_strPlural);
						//#endif
						//wenn das Substantiv im Deutschen auch im Singular steht
						//if(en->m_bTranslationType==
            if(//ivocabularyinmainmem.GetTranslationType() ==
              byTranslationType ==
              SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN)
						{
							personIndex=2;
              BYTE byArticle = ivocabularyinmainmem.GetGermanArticle() ;
              std::string strGermanSingular = ivocabularyinmainmem.
                GetGermanSingular() ;
              //Z.B.: "trotz meines Wissens."
              //Z.B.: "des Vaters Sohn."
              if( Case == GENITIVE && 
                (//gn->m_bArticle==DER || gn->m_bArticle==DAS) &&
                byArticle == DER || byArticle == DAS 
                ) 
                &&
                //TODO richtiges Ende für verschiedene Endungen.
                //TODO wie lautet Regel für "Herr"->"dieses Herrn" ?
                //letzte 2 Buchstaben: Vokal->Konsonant: z.B. "VatER"
                ( 
                    //Important for "str[...]".
                    strGermanSingular.length() > 1 
                    && 
                    IsVowel(//gn->m_strSingular.Right(2).Left(1)
                    strGermanSingular[strGermanSingular.size()-2] )
                    && 
                    IsConsonant(//gn->m_strSingular.Right(1)
                    strGermanSingular[strGermanSingular.size()-1]
                    )
                  //letzter Buchstabe: Vokal: z.B. "AutO".
                  || 
                    //Important for "str[...]".
                    strGermanSingular.length() > 0 
                    &&
                    IsVowel(//gn->m_strSingular.Right(1) 
                    strGermanSingular[strGermanSingular.size()-1]
                    )
                )
                )//if
  							sv.push_back(//gn->m_strSingular
                  (strGermanSingular + _T("s")).c_str() );
              else
                //Bei Genitiv und weiblich wird auch kein "s" angehängt:               
                //Z.B.: "trotz meiner Frau."
	  						sv.push_back(//gn->m_strSingular
                  strGermanSingular.c_str() );
              bGermanNounGender=VALUE_FOR_GENDER_TO_ARRAY_INDEX(
                //gn->m_bArticle
                byArticle );
						}
						//wenn das Substantiv im Deutschen im Plural steht
						else if(//en->m_bTranslationType
              byTranslationType == SINGULAR_IN_ENGLISH_PLURAL_IN_GERMAN )
						{
              std::string strGermanPlural = ivocabularyinmainmem.
                GetGermanPlural() ;
							personIndex=2;
							//wenn das Subjekt als Objekt übersetzt werden soll
							if(bTranslateAsObject)//Z.B. "Furniture are given things."
							{
								//if(gn->m_strPlural.GetLength()>0)
                if( strGermanPlural.length() > 0 )
								{
                  // Beispiel: Möbel -> den MöbelN
									if(//gn->m_strPlural.Right(1) != _T("n") )
                    strGermanPlural.length() > 0 && strGermanPlural[ 
                    strGermanPlural.length() - 1] != 'n'
                    )
										sv.push_back(//gn->m_strPlural
                      ( strGermanPlural + _T("n") ).c_str() );
									else
                    // Beispiel: Informationen -> den Informationen
										sv.push_back(//gn->m_strPlural
                      strGermanPlural.c_str() ); 
								}
								else
                  // Beispiel: Informationen -> den Informationen
									sv.push_back(//gn->m_strPlural
                    strGermanPlural.c_str() );
							}
							//wenn das Subjekt als Subjekt übersetzt werden soll
							else
								sv.push_back(//gn->m_strPlural
                  strGermanPlural.c_str() );
              bGermanNounGender = PLURAL;
						}// im Deutschen: Plural
					}
				}
				else // im Englischen: Plural
				{
					TRACE("Plural\n");
					#ifdef _DEBUG
					printf("Plural\n");
					#endif
					//EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
					//	m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
					//	at(dwRun).m_pWordNode->m_pWord);
					//if(en!=NULL)
					{
            BYTE byTranslationType = ivocabularyinmainmem.GetTranslationType() ;
						#ifdef _DEBUG
						printf("en\n");
						//printf("en->m_strSingular: %s  en->m_strPlural: %s\n",
						//	en->m_strSingular,en->m_strPlural);
						//printf("en->m_bTranslationType: %u\n",en->m_bTranslationType);
						//TRACE("en\n");
						//if(gn)
						//{
						//	printf("gn\n");
						//	printf("gn->m_strPlural: %s\n",gn->m_strPlural);
						//}
						#endif
						if(//en->m_bTranslationType==
              byTranslationType ==
              SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN) // im Deutschen: Plural
						{
              std::string strGermanPlural = ivocabularyinmainmem.
                GetGermanPlural() ;
							personIndex=5;
							if(bTranslateAsObject)//e.g "Kids are given things."
							{
								//if(gn->m_strPlural.GetLength()>0)
                if( strGermanPlural.length() > 0 )
								{
                  // Beispiel: Kinder -> den KinderN
									//if(gn->m_strPlural.Right(1)!=_T("n")) 
                  if( strGermanPlural.length() > 0 && strGermanPlural[ 
                    strGermanPlural.length() - 1] != 'n' 
                    )
                    sv.push_back(//gn->m_strPlural
                      ( strGermanPlural + _T("n") ).c_str() );
									else
                    // Beispiel: Frauen -> den Frauen
										sv.push_back(//gn->m_strPlural
                      strGermanPlural.c_str() );
								}
								else
                  // Beispiel: Frauen -> den Frauen
									sv.push_back(//gn->m_strPlural
                    strGermanPlural.c_str() );
							}//if(bTranslateAsObject)
							else
							{
								if(Case==1 && //gn->m_strPlural.GetLength() > 0 
                  strGermanPlural.length() > 0 ) // 3. Fall
                  // wenn letztes Zeichen gleich 'e'
									if(//gn->m_strPlural.Right(1)
                    strGermanPlural[strGermanPlural.length() - 1 ] == 'e' ) 
									{
                    // Beispiel: I trust old animals. -> Ich vertraue altEN Tieren.
										sv.push_back(//gn->m_strPlural
                      ( strGermanPlural + _T("n") ).c_str() ); 
										goto printf;
									}
								sv.push_back(//gn->m_strPlural
                  strGermanPlural.c_str() );
printf:
							#ifdef _DEBUG
							printf("nach: sv.push_back(gn->m_strPlural);\n");
							#endif
							;
							}
              bGermanNounGender=PLURAL;
						}
						else if(//en->m_bTranslationType
              byTranslationType == PLURAL_IN_ENGLISH_SINGULAR_IN_GERMAN )
						{
              std::string strGermanSingular = ivocabularyinmainmem.
                GetGermanSingular() ;
							personIndex=2;
							sv.push_back(//gn->m_strSingular
                strGermanSingular.c_str() );
              bGermanNounGender=VALUE_FOR_GENDER_TO_ARRAY_INDEX(
                //gn->m_bArticle
                ivocabularyinmainmem.GetGermanArticle() );
						}
					}
				}
			}//for-loop for nouns that are written identically.
			//Wenn das Token das erste im Satz ist, dann muss der 
			//erste Buchstabe durch MakeFirstLetterUpper() groß gemacht
			//werden.
			if(bFirstTokenInSentence)
			{
				MakeFirstLetterUpper(sv.at(0));
				bFirstTokenInSentence=FALSE;
			}
			vv.push_back(sv);
			#ifdef _DEBUG
			printf("nach: vv.push_back(sv);\n");
			#endif
		}//Wenn der Substantiv-Vektor mind. 1 Element enthält.
		#ifdef _DEBUG
		else
		{
			printf("!enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
		}
		#endif
	}//ENR ist Substantiv
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,\
BYTE Case,BOOL bNegation,BOOL bTranslateAsObject,BYTE & personIndex,\
BYTE & bFirstTokenInSentence) ENDE\n");
	#endif
	return vv;
}

//wird von std::vector<CStringVector> TranslateObjectEnumerationElement2(
//const EnumerationElement & ee,BYTE Case,BOOL bNegation,BYTE & 
//bFirstTokenInSentence) und von TranslateEnumerationElement() aufgerufen

//@param bArticle: welches Geschlecht das Substantiv hat, wird für Relativsätze
//benötigt: "The number you called is incomplete."
//   ->      Die Nummer, die du anriefst, ist unvollständig."
//            |           |
//            +-----------+
//                  |
//                  +--Artikel sind gleich
//@param nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
//die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
//bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
//Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
//sich das "folgst" automatisch in "folgt" ändern
std::vector<CStringVector> TranslateENR(
  EnglishNounRecord * enr,
	BYTE Case,
  BOOL bNegation,
  BOOL bTranslateAsObject,
  BYTE & personIndex,
	BYTE & bFirstTokenInSentence,
  BYTE & bArticle,
  int & nStartID)
{ // Case: der Fall: kann "0"="kein" "1"="3."  oder "2"="4. Fall" sein
	std::vector<CStringVector> vv;
	BOOL bIndefiniteArticle=FALSE;
	BOOL bDefiniteArticle=FALSE;
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,\
BYTE Case,BOOL bNegation,BOOL bTranslateAsObject,BYTE & bFirstTokenInSente\
nce,BYTE & bArticle) ANFANG\n");
	TRACE("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,BYTE Case,BOOL bNegation,BOOL bTranslateAsObject) ANFANG\n");
	printf("Case: %u bFirstTokenInSentence: %u\n",Case,bFirstTokenInSentence);
	#endif
	std::vector <EnglishNoun> vecEN=GetEnglishNoun(enr->m_Noun);
	#ifdef _DEBUG
	printf("enr->m_Noun.size(): %u\n",enr->m_Noun.size());
	{
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			printf("enr->m_Noun.at(i): %s\n",enr->m_Noun.at(i));
	}
	#endif
	if(enr->bDefiniteArticle) // englischer bestimmter Artikel ('the')
	{
#ifdef _DEBUG
		printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
#endif
		if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
		{
			GermanNoun * gn=
				GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
				enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				m_pWordNode);
			EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
				m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				m_pWordNode->m_pWord);
			if(en && gn)
			{
				#ifdef _DEBUG
				printf("en && gn\n");
				printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
				#endif
				CStringVector sv;
				//wenn das Nomen im Englischen im Singular ist
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
					m_bSingular)
				{
				#ifdef _DEBUG
					printf("enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEn\
capsulated.at(0).m_bSingular\n");
					printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
				#endif
					if(en->m_bTranslationType==0)
					{
#ifdef _DEBUG
						printf("en->m_bTranslationType==0\n");
#endif
						personIndex=2;
						// wenn das Subjekt als Objekt übersetzt werden soll
						if(bTranslateAsObject)
						{
							CStringVector sv;
#ifdef _DEBUG
							printf("bTranslateAsObject\n");
#endif
							if(gn->m_bArticle=='1') // männlich (Artikel=='der')
								sv.push_back("dem"); // The man is given things. -> DEM Mann werden 
								// Dinge gegeben.
							if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
								sv.push_back("der"); // The woman is given things. -> DER Frau werden 
								// Dinge gegeben.
							if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
								sv.push_back("dem"); // The child is given things. -> DEM Kind werden 
								// Dinge gegeben.
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv.at(0));
								bFirstTokenInSentence=FALSE;
							}
							vv.push_back(sv);
						}
						// wenn das Subjekt als Subjekt übersetzt werden soll
						else
						{
#ifdef _DEBUG
							printf("!bTranslateAsObject\n");
#endif
							if(Case==0) // kein Fall
							{
#ifdef _DEBUG
								printf("Case==0\n");
#endif
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("der"); // I am the man. -> Ich bin ein Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("die"); // I am the woman. -> Ich bin eine Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("das"); // I am the child. -> Ich bin ein Kind.
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
								vv.push_back(sv);
							}
							if(Case==1) // 3. Fall
							{
				#ifdef _DEBUG
								printf("Case==1\n");
				#endif
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("dem"); // I trust the man. -> Ich vertraue dem Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("der"); // I trust the woman. -> Ich vertraue der Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("dem"); // I trust the child. -> Ich vertraue dem Kind.
				#ifdef _DEBUG
								printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
				#endif
								if(bFirstTokenInSentence)
								{
				#ifdef _DEBUG
									printf("bFirstTokenInSentence\n");
									printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
				#endif
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
								vv.push_back(sv);
							}
							if(Case==2) // 4. Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("den"); // I like a man. -> Ich mag den Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("die"); // I like a woman. -> Ich mag die Mann.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("das"); // I like a man. -> Ich mag das Kind.
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
								vv.push_back(sv);
							}
						}
					}
					// wenn das Substantiv im Deutschen im Plural steht
					if(en->m_bTranslationType==1)
					{
#ifdef _DEBUG
						printf("en->m_bTranslationType==1\n");
#endif
						personIndex=5;
						if(bTranslateAsObject)
						{
							CStringVector sv;
							sv.push_back("den"); // The hair is given things. -> DEN Haaren werden Dinge gegeben.
								// Dinge gegeben.
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv.at(0));
								bFirstTokenInSentence=FALSE;
							}
							vv.push_back(sv);
						}
						else
						{
							CStringVector sv;
							sv.push_back("die"); // The hair is good. -> DIE Haare sind gut. 
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv.at(0));
								bFirstTokenInSentence=FALSE;
							}
							vv.push_back(sv);
						}
					}
				}
				else // sonst: im Englischen: Plural
				{
				#ifdef _DEBUG
					printf("!enr->m_vecPointerToWordNodeWhereAnEnglishNounIs\
Encapsulated.at(0).m_bSingular\n");
				#endif
					if(en->m_bTranslationType==0)
					{ // im Deutschen auch Plural
						personIndex=5;
						if(bTranslateAsObject)
						{
							CStringVector sv;
							sv.push_back("den"); // The hair is given things. -> DEN Haaren werden 
								// Dinge gegeben.
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv.at(0));
								bFirstTokenInSentence=FALSE;
							}
							vv.push_back(sv);
						}
						else
						{
							CStringVector sv;
							sv.push_back("die"); // The hair is good. -> DIE Haare sind gut. 
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv.at(0));
								bFirstTokenInSentence=FALSE;
							}
							vv.push_back(sv);
						}
					}
					// im Deutschen: Singular
					if(en->m_bTranslationType==2)
					{
						personIndex=2;
						//wenn das Subjekt im Deutschen als Objekt übersettzt werden soll
						if(bTranslateAsObject)
						{
							CStringVector sv;
							if(gn->m_bArticle=='1') // männlich (Artikel=='der')
								sv.push_back("dem"); // leider kein Beispiel
							if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
								sv.push_back("der"); // The scissors is given things. -> DER Schere 
								// werden Dinge gegeben.
							if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
								sv.push_back("dem"); // leider kein Beispiel
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv.at(0));
								bFirstTokenInSentence=FALSE;
							}
							vv.push_back(sv);
						}
						else // wenn das Subjekt als Subjekt übersetzt werden soll
						{
							if(Case==0) // kein Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("der"); // leider kein Beispiel
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("die"); // The scissors cut well. -> DIE Schere
									// schneidet gut.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("das"); // leider kein Beispiel
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
								vv.push_back(sv);
							}
							if(Case==1) // 3. Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("dem"); // leider kein Beispiel
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("der"); // I trust the scissors. -> Ich vertraue DER
									// Schere.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("dem"); // leider kein Beispiel
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
								vv.push_back(sv);
							}
							if(Case==2) // 4. Fall
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("den"); // leider kein Beispiel
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("die"); // I like the scissors. -> Ich mag DIE
									// Schere.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("das"); // leider kein Beispiel
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
								vv.push_back(sv);
							}
						}
					}
				}
			}
			bArticle=gn->m_bArticle-49;
		}
	}
	if(enr->bIndefiniteArticle) // englischer unbestimmter Artikel ('a' oder 'an')
	{
		if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
		{
			GermanNoun * gn=
				GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
				enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
				at(0).m_pWordNode);
			EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
				m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				m_pWordNode->m_pWord);
			if(en && gn)
			{
				#ifdef _DEBUG
				printf("en && gn\n");
				#endif
				CStringVector sv;
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
					m_bSingular)
				{
					if(en->m_bTranslationType==0)
					{
						personIndex=2;
						if(bNegation) // Negation/Verneinung (Satz enthielt 'not')
						{
							//das Subjekt soll im Deutschen als Objekt übersetzt werden
							if(bTranslateAsObject)
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("keinem"); // I do not trust a man. -> 
									//Ich vertraue KEINEM Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("keiner"); // I do not trust a woman. -> 
									//Ich vertraue KEINER Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("kein"); // I do not trust a child. -> 
									//Ich vetraue KEINEM Kind.
								vv.push_back(sv);
							}
							else // wenn das Subjekt als Subjekt übersetzt werden soll
							{
								if(Case==0) // kein Fall
								{
									CStringVector sv;
									if(gn->m_bArticle=='1') // männlich (Artikel=='der')
										sv.push_back("kein"); // I am not a man. -> Ich bin kein Mann.
									if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
										sv.push_back("keine"); // I am not a woman. -> Ich bin keine Frau.
									if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
										sv.push_back("kein"); // I am not a child. -> Ich bin kein Kind.
									vv.push_back(sv);
								}
								if(Case==1) // 3. Fall
								{
									CStringVector sv;
									if(gn->m_bArticle=='1') // männlich (Artikel=='der')
										sv.push_back("keinem"); // I trust a man. -> Ich vertraue einem Mann.
									if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
										sv.push_back("keiner"); // I trust a woman. -> Ich vertraue einer Frau.
									if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
										sv.push_back("keinem"); // I trust a child. -> Ich vertraue einem Kind.
									vv.push_back(sv);
								}
								if(Case==2) // 4. Fall
								{
									CStringVector sv;
									if(gn->m_bArticle=='1') // männlich (Artikel=='der')
										sv.push_back("keinen"); // I like a man. -> Ich mag einen Mann.
									if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
										sv.push_back("keine"); // I like a woman. -> Ich mag eine Mann.
									if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
										sv.push_back("kein"); // I like a man. -> Ich mag ein Kind.
									vv.push_back(sv);
								}
							}
						}
						else // keine Negation/Verneinung (kein 'not')
						{
							if(bTranslateAsObject)
							{
								CStringVector sv;
								if(gn->m_bArticle=='1') // männlich (Artikel=='der')
									sv.push_back("ein"); // I am a man. -> Ich bin ein Mann.
								if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
									sv.push_back("eine"); // I am a woman. -> Ich bin eine Frau.
								if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
									sv.push_back("ein"); // I am a child. -> Ich bin ein Kind.
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
								vv.push_back(sv);
							}
							else // wenn das Subjekt nicht als Objekt übersetzt werden soll
							{
								if(Case==0) // kein Fall
								{
									CStringVector sv;
									if(gn->m_bArticle=='1') // männlich (Artikel=='der')
										sv.push_back("ein"); // I am a man. -> Ich bin ein Mann.
									if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
										sv.push_back("eine"); // I am a woman. -> Ich bin eine Frau.
									if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
										sv.push_back("ein"); // I am a child. -> Ich bin ein Kind.
									if(bFirstTokenInSentence)
									{
										MakeFirstLetterUpper(sv.at(0));
										bFirstTokenInSentence=FALSE;
									}
									vv.push_back(sv);
								}
								if(Case==1) // 3. Fall
								{
									CStringVector sv;
									if(gn->m_bArticle=='1') // männlich (Artikel=='der')
										sv.push_back("einem"); // I trust a man. -> Ich vertraue einem Mann.
									if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
										sv.push_back("einer"); // I trust a woman. -> Ich vertraue einer Frau.
									if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
										sv.push_back("einem"); // I trust a child. -> Ich vertraue einem Kind.
									if(bFirstTokenInSentence)
									{
										MakeFirstLetterUpper(sv.at(0));
										bFirstTokenInSentence=FALSE;
									}
									vv.push_back(sv);
								}
								if(Case==2) // 4. Fall
								{
									CStringVector sv;
									if(gn->m_bArticle=='1') // männlich (Artikel=='der')
										sv.push_back("einen"); // I like a man. -> Ich mag einen Mann.
									if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
										sv.push_back("eine"); // I like a woman. -> Ich mag eine Frau.
									if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
										sv.push_back("ein"); // I like a man. -> Ich mag ein Kind.
									if(bFirstTokenInSentence)
									{
										MakeFirstLetterUpper(sv.at(0));
										bFirstTokenInSentence=FALSE;
									}
									vv.push_back(sv);
								}
							}
						}
					}
				}
			}
		}
	}
	// m_possessivePronoun;
	if(enr->m_possessivePronoun>0) // besitzanzeigendes Fürwort
	{
		#ifdef _DEBUG
		printf("enr->m_possessivePronoun: %u\n",enr->m_possessivePronoun);
		for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
		{
			TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
		}
		#endif
		CStringVector vecStr=TranslatePossessivePronoun2(enr->m_possessivePronoun);
		if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
		{
      BYTE byArrayIndex=0;
			for(DWORD dwRun=0;dwRun<enr->
				m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size();dwRun++)
			{
				GermanNoun * pGN=
					GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
					enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
					m_pWordNode);
				EnglishNoun * pEN=dynamic_cast<EnglishNoun*>(enr->
					m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
					m_pWordNode->m_pWord);
				if(pEN && pGN)
				{
					//wenn das Substantiv im Englischen im Singular ist
					if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
						at(dwRun).m_bSingular)
					{
						if(pEN->m_bTranslationType==
              SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN)
						{
							personIndex=2;
							//wenn das Subjekt als im Deutschen als Objekt übersetzt werden soll
							if(bTranslateAsObject)
							{
								if(pGN->m_bArticle=='1') // männlich (Artikel=='der')
                  // I trust my man. -> Ich vertraue meinEM Mann.
									//vecStr.at(dwRun)+=_T("em"); 
                  byArrayIndex=INDEX_FOR_EM;
								if(pGN->m_bArticle=='2') // weiblich (Artikel=='die')
                  // I trust my woman. -> Ich vertraue meinER Frau.
									//vecStr.at(dwRun)+=_T("er"); 
                  byArrayIndex=INDEX_FOR_ER;
								if(pGN->m_bArticle=='3') // sächlich (Artikel=='das')
                  // I trust my child. -> Ich vertraue meinEM Kind.
									//vecStr.at(dwRun)+=_T("em"); 
                  byArrayIndex=INDEX_FOR_EM;
							}
							//wenn das Subjekt als im Deutschen auch als Subjekt übersetzt werden soll
							else
							{
								if(Case==0) // kein Fall
								{
									if(pGN->m_bArticle=='2') // weiblich (Artikel=='die')
                    // I am my woman. -> Ich bin meinE Frau.
										//vecStr.at(dwRun)+=_T("e"); 
                    byArrayIndex=INDEX_FOR_E;
									// bei männlichen Substantiven (Artikel=='der'), Beispiel:
									// I am my man. -> Ich bin mein Mann    oder
									// bei sächlichen Substantiven (Artikel=='das'), Beispiel: 
									// I am my child. -> Ich bin mein Kind. 	wird nichts angehängt
								}
								if(Case==DATIVE) // 3. Fall
								{
									if(pGN->m_bArticle=='1') // männlich (Artikel=='der')
                    // I trust my man. -> Ich vertraue meinEM Mann.
										//vecStr.at(dwRun)+=_T("em"); 
                    byArrayIndex=INDEX_FOR_EM;
									if(pGN->m_bArticle=='2') // weiblich (Artikel=='die')
                    // I trust my woman. -> Ich vertraue meinER Frau.
										//vecStr.at(dwRun)+=_T("er"); 
                    byArrayIndex=INDEX_FOR_ER;
									if(pGN->m_bArticle=='3') // sächlich (Artikel=='das')
                    // I trust my child. -> Ich vertraue meinEM Kind.
										//vecStr.at(dwRun)+=_T("em"); 
                    byArrayIndex=INDEX_FOR_EM;
								}
								if(Case==ACCUSATIVE) // 4. Fall
								{
									if(pGN->m_bArticle=='1') // männlich (Artikel=='der')
										// I like my man. -> Ich mag meinEN Mann.
                    //vecStr.at(dwRun)+=_T("en"); 
                    byArrayIndex=INDEX_FOR_EN;
									if(pGN->m_bArticle=='2') // weiblich (Artikel=='die')
										// I like my woman. -> Ich mag meinE Frau.
                    //vecStr.at(dwRun)+=_T("e"); 
                    byArrayIndex=INDEX_FOR_E;
								}
							}
						}
						if(pEN->m_bTranslationType==
              SINGULAR_IN_ENGLISH_PLURAL_IN_GERMAN) // m_TranslationType==1 bedeutet, dass
						// ein Wort, das im Englischen im Singular steht, im Deutschen im Plural ist
						// (Beispiel: the information -> die Informationen)
						{
							personIndex=5;
							if(bTranslateAsObject)
                // also Plural (z.B. deinEN Informationen)
								//vecStr.at(dwRun)+="en"; 
                byArrayIndex=INDEX_FOR_EN;
							else
                // also Plural (z.B. deinE Informationen)
								//vecStr.at(dwRun)+="e"; 
                byArrayIndex=INDEX_FOR_E;
						}
					}
					//wenn das Substantiv im Englischen im Plural ist
					else // im Englischen: Plural
					{
#ifdef _DEBUG
						printf("im Englischen: Plural\n");
#endif
						if(pEN->m_bTranslationType==
              SAME_GRAMMATICAL_NUMBER_BETWEEN_ENGLISH_AND_GERMAN) // im Deutschen: Plural
						{ //
#ifdef _DEBUG
							printf("im Deutschen: Plural\n");
#endif
							personIndex=5;
							if(bTranslateAsObject)
                // ein 'en' an den String anhängen (deinEN Schulen)
								//vecStr.at(dwRun)+="en"; 
                byArrayIndex=INDEX_FOR_EN;
							else
							{
								if(Case==0 || Case==2) //wenn kein Fall oder 4. Fall
									// ein 'e' an den String anhängen (deinE Schulen)
                  //vecStr.at(dwRun)+="e"; 
                  byArrayIndex=INDEX_FOR_E;
								if(Case==1) //wenn 3. Fall
									// ein 'e' an den String anhängen (deinE Schulen)
                  //vecStr.at(dwRun)+="en"; 
                  byArrayIndex=INDEX_FOR_EN;
							}
						}
						if(pEN->m_bTranslationType==
              PLURAL_IN_ENGLISH_SINGULAR_IN_GERMAN) // m_TranslationType==1 bedeutet, dass
						// ein Wort, das im Englischen im Plural steht, im Deutschen im Singular ist
						// (Beispiel: the scissors -> die Schere)
						{ // nur bei weiblichen Nomen wird im Deutschen ein 'e' angehängt
							personIndex=2;
							if(bTranslateAsObject)
							{
								if(pGN->m_bArticle=='1') // männlich (Artikel=='der')
									// I trust my man. -> Ich vertraue meinEM Mann.
                  //vecStr.at(dwRun)+=_T("em"); 
                  byArrayIndex=INDEX_FOR_EM;
								if(pGN->m_bArticle=='2') // weiblich (Artikel=='die')
									// I trust my woman. -> Ich vertraue meinER Frau.
                  //vecStr.at(dwRun)+=_T("er"); 
                  byArrayIndex=INDEX_FOR_ER;
								if(pGN->m_bArticle=='3') // sächlich (Artikel=='das')
									// I trust my child. -> Ich vertraue meinEM Kind.
                  //vecStr.at(dwRun)+=_T("em"); 
                  byArrayIndex=INDEX_FOR_EM;
							}
							else
							{
								if(Case==0) // kein Fall
								{
									CStringVector sv;
									//if(pGN->m_bArticle=='1') // männlich (Artikel=='der')
									//	// I am a man. -> Ich bin ein Mann.
         //           sv.push_back("kein"); 
									if(pGN->m_bArticle='2') // weiblich (Artikel=='die')
										// I am a woman. -> Ich bin eine Frau.
                    //sv.push_back("keine"); 
                    byArrayIndex=INDEX_FOR_E;
									//if(pGN->m_bArticle=='3') // sächlich (Artikel=='das')
									//	// I am a child. -> Ich bin ein Kind.
         //           sv.push_back("kein"); 
									//vecStr.at(dwRun)+=_T("e");
								}
								if(Case==DATIVE) // 3. Fall
								{
									if(pGN->m_bArticle=='1') // männlich (Artikel=='der')
										// I trust my man. -> Ich vertraue meinEM Mann.
                    //vecStr.at(dwRun)+=_T("em"); 
                    byArrayIndex=INDEX_FOR_EM;
									if(pGN->m_bArticle=='2') // weiblich (Artikel=='die')
										// I trust my woman. -> Ich vertraue meinER Frau.
                    //vecStr.at(dwRun)+=_T("er"); 
                    byArrayIndex=INDEX_FOR_ER;
									if(pGN->m_bArticle=='3') // sächlich (Artikel=='das')
										// I trust my child. -> Ich vertraue meinEM Kind.
                    //vecStr.at(dwRun)+=_T("em"); 
                    byArrayIndex=INDEX_FOR_EM;
								}
								if(Case==ACCUSATIVE) // 4. Fall
								{
									if(pGN->m_bArticle=='1') // männlich (Artikel=='der')
                    // I like my man. -> Ich mag meinEN Mann.
										//vecStr.at(dwRun)+=_T("en"); 
                    byArrayIndex=INDEX_FOR_EN;
									if(pGN->m_bArticle=='2') // weiblich (Artikel=='die')
                    // I like my woman. -> Ich mag meinE Frau.
										//vecStr.at(dwRun)+=_T("e"); 
                    byArrayIndex=INDEX_FOR_E;
									// be einem sächlichen Substantiv (Artikel=='das') wrid nichts
									// mehr angehängt: I like my child. -> Ich mag mein Kind.
								}
							}
						}
					}//im Englischen: Plural
				}//if(pEN && pGN)
        //Loop because for the personal pronoun "you" 3 translations 
        // ("du","Sie","ihr") are possible and all should possibly get a suffix.
        for(std::vector<CString>::iterator ivecstr=vecStr.begin();
          ivecstr!=vecStr.end();ivecstr++)
          *ivecstr+=//EnglishNounRecord::
            arstrInflectionSuffix[byArrayIndex].c_str();
			}//for-loop iterating through nouns.
		}
		if(bFirstTokenInSentence)
		{
			MakeFirstLetterUpper(vecStr.at(0));
			bFirstTokenInSentence=FALSE;
		}
		vv.push_back(vecStr);
	}
	//wenn das Aufzählungselement des Subjekt ein Pronomen enthält wie z.B. 'this'
	if(enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated)
	{
		#ifdef _DEBUG
		printf("enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated\n");
		#endif
		if(enr->m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated->
			m_pWordNodeNext)
		{
			GermanPronoun * gp=dynamic_cast<GermanPronoun*>(enr->
				m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			if(gp)
			{
				#ifdef _DEBUG
				printf("gp\n");
				#endif
				//wenn das Aufzählungselement des Subjekts (ein) Substantiv(e) enthält
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
				{
					GermanNoun * gn=
						GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
						enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
						at(0).m_pWordNode);
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(
						enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
						at(0).m_pWordNode->m_pWord);
					if(en && gn)
					{
						#ifdef _DEBUG
						printf("en && gn\n");
						#endif
						CStringVector sv;
						//wenn das Substantiv im Englischen im Singular ist
						if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
							at(0).m_bSingular)
						{
							if(en->m_bTranslationType==0)
							{
								personIndex=2;
								//wenn das Subjekt als Objekt übersetzt werden soll
								if(bTranslateAsObject)
								{
									if(gn->m_bArticle=='1') // männlich (Artikel=='der')
										sv.push_back(gp->m_strWord+_T("em"));
									if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
										sv.push_back(gp->m_strWord+_T("er"));
									if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
										sv.push_back(gp->m_strWord+_T("em"));
								}
								//wenn das Subjekt als Subjekt übersetzt werden soll
								else
								{
									if(Case==0) // kein Fall
									{
										CStringVector sv;
										if(gn->m_bArticle=='1') // männlich (Artikel=='der')
											sv.push_back(gp->m_strWord+_T("er")); // I am this man. -> Ich bin diesER Mann.
										if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
											sv.push_back(gp->m_strWord+_T("e")); // I am this woman. -> Ich bin diesE Frau.
										if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
											sv.push_back(gp->m_strWord+_T("es")); // I am this child. -> Ich bin diesES Kind.
										//vecStr.at(dwRun)+=_T("e");
									}
									if(Case==1) // 3. Fall
									{
										if(gn->m_bArticle=='1') // männlich (Artikel=='der')
											sv.push_back(gp->m_strWord+_T("em")); // I trust this man. -> Ich vertraue diesEM Mann.
										if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
											sv.push_back(gp->m_strWord+_T("er")); // I trust this woman. -> Ich vertraue diesER Frau.
										if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
											sv.push_back(gp->m_strWord+_T("em")); // I trust this child. -> Ich vertraue diesEM Kind.
									}
									if(Case==2) // 4. Fall
									{
										if(gn->m_bArticle=='1') // männlich (Artikel=='der')
											sv.push_back(gp->m_strWord+_T("en")); // I like this man. -> Ich mag diesEN Mann.
										if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
											sv.push_back(gp->m_strWord+_T("e")); // I like this woman. -> Ich mag diesE Frau.
										if(gn->m_bArticle=='3') // weiblich (Artikel=='das')
											sv.push_back(gp->m_strWord+_T("es")); // I like this child. -> Ich mag diesES Kind.
									}
								}
							}
							if(en->m_bTranslationType==1) // im Deutschen: Plural
							{
								personIndex=5;
								if(bTranslateAsObject)
									sv.push_back(gp->m_strWord+_T("en"));
								else
									sv.push_back(gp->m_strWord+_T("e"));
							}
						}
						//wenn das Substantiv im Englischen im Plural ist
						else
						{
							if(en->m_bTranslationType==0)
							{
								personIndex=5;
								if(bTranslateAsObject)
								{
									sv.push_back(gp->m_strWord+_T("en"));
								}
								else
								{
									if(Case==1) // wenn 3. Fall
										//I trust these animals. -> Ich vertraue jenEN Tieren.
										sv.push_back(gp->m_strWord+_T("en"));
									else
										sv.push_back(gp->m_strWord+_T("e"));
								}
							}
							if(en->m_bTranslationType==2) // im Deutschen: Singular
							{
								personIndex=2;
								if(bTranslateAsObject)
								{
									if(gn->m_bArticle=='1') // männlich (Artikel=='der')
										sv.push_back(gp->m_strWord+_T("em"));
									if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
										sv.push_back(gp->m_strWord+_T("er"));
									if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
										sv.push_back(gp->m_strWord+_T("em"));
								}
								else
								{
									if(gn->m_bArticle=='1') // männlich (Artikel=='der')
										sv.push_back(gp->m_strWord+_T("en"));
									if(gn->m_bArticle=='2') // weiblich (Artikel=='die')
										sv.push_back(gp->m_strWord+_T("e"));
									if(gn->m_bArticle=='3') // sächlich (Artikel=='das')
										sv.push_back(gp->m_strWord+_T("es"));
								}
							}
						}
						if(bFirstTokenInSentence)
						{
							MakeFirstLetterUpper(sv.at(0));
							bFirstTokenInSentence=FALSE;
						}
						vv.push_back(sv);
					}
				}
				//wenn das Aufzählungselement des Subjekts kein(e) Substantiv(e) enthält
				//Beispiel: "This is a car." -> "Dies ist ein Auto."
				else
				{
					CStringVector sv;
					sv.push_back(gp->m_strWord);
					personIndex=2;
					if(bFirstTokenInSentence)
					{
						MakeFirstLetterUpper(sv.at(0));
						bFirstTokenInSentence=FALSE;
					}
					vv.push_back(sv);
				}
			}
		}
	}
	//wenn das Aufzählungselement (ein) Adverb(ien) enthält, die sich auf 
	//ein Adjektiv oder Adverb beziehen
	if(enr->
		m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
	{
		#ifdef _DEBUG
		printf("enr->m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdj\
ectiveOrAdverbIsEncapsulated\n");
		#endif
		CStringVector sv;
		GermanAdverb * ga=dynamic_cast<GermanAdverb*>(enr->
			m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated->
			m_pWordNodeNext->m_pWord);
		if(ga)
		{
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	//Adjektive
	for(DWORD i=0;i<enr->
		m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size();i++) // Adjektive
	{ // Adjektiv(e)
		#ifdef _DEBUG
		printf("Adjektive(e)\n");
		#endif
		if(enr->m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.
			at(i)->m_pWordNodeNext)
		{
			GermanAdjective * ga=dynamic_cast<GermanAdjective*>(enr->
				m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.at(i)->
				m_pWordNodeNext->m_pWord);
			if(ga)
			{
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
				{
					GermanNoun * gn=
						GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
						enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
						m_pWordNode);
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
						m_pWordNode->m_pWord);
					if(en && gn)
					{
						CStringVector sv;
						 // wenn das Substantiv im Englischen im Singular ist
						if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
							at(0).m_bSingular)
						{
#ifdef _DEBUG
							printf("im Englischen: Singular\n");
#endif
							if(en->m_bTranslationType==0) // wenn im Deutschen auch Singular
							{
								personIndex=2;
#ifdef _DEBUG
								printf("im Deutschen: Singular\n");
#endif
								if(enr->bIndefiniteArticle)
								{
									if(bTranslateAsObject)
									{
										if(gn->m_bArticle=='1') // wenn männliches Substantiv
	// An old man is told a story. -> Einem altEN Mann wird eine Geschichte erzählt.
											sv.push_back(ga->m_strWortstamm+_T("en"));
										if(gn->m_bArticle=='2')
	// An old woman is told a story. -> Einer altEN Frau wird eine Geschichte erzählt.
											sv.push_back(ga->m_strWortstamm+_T("en"));
										if(gn->m_bArticle=='3')
	// An old child is told a story. -> Einem altEN Kind wird eine Geschichte erzählt.
											sv.push_back(ga->m_strWortstamm+_T("en"));
									}
									else
									{
										if(Case==0) // kein Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("er")); // Er ist ein altER Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); // Sie ist ein altE Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); // Es ist ein altES Kind
										}
										if(Case==1) // 3. Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
										}
										if(Case==2) // 4. Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er mag einen altEN Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); // Er mag eine altE Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); // Er mag ein altES Kind
										}
									}
								}
								else if(enr->bDefiniteArticle) // wenn englischer bestimmter Artikel ('the')
								{
		#ifdef _DEBUG
									printf("enr->bDefiniteArticle\n");
		#endif
									if(bTranslateAsObject)
									{
										if(enr->m_vecAdjectiveType.at(i)==0)
											sv.push_back(ga->m_strWortstamm+_T("en"));
										if(enr->m_vecAdjectiveType.at(i)==1)
											sv.push_back(ga->m_strComperativ+_T("en"));
										if(enr->m_vecAdjectiveType.at(i)==2)
											sv.push_back(ga->m_strSuperlativ);
									}
									else
									{
		#ifdef _DEBUG
										printf("!bTranslateAsObject\n");
										printf("enr->m_vecAdjectiveType.at(i): %d\n",
											enr->m_vecAdjectiveType.at(i));
		#endif
										if(enr->m_vecAdjectiveType.at(i)==0)
											sv.push_back(ga->m_strWortstamm+_T("e"));
										if(enr->m_vecAdjectiveType.at(i)==1)
											sv.push_back(ga->m_strComperativ+_T("e"));
										if(enr->m_vecAdjectiveType.at(i)==2)
										{
											if(ga->m_strSuperlativ.GetLength()>0)
												sv.push_back(ga->m_strSuperlativ.Left(ga->
												m_strSuperlativ.GetLength()-1));
										}
									}
								}
								else // kein Artikel (weder bestimmt noch unbestimmt)
								{
									if(bTranslateAsObject)
									{
										if(gn->m_bArticle=='1') // wenn männliches Substantiv
										{
	// My old man is told a story. -> Dem altEN Mann wird eine Geschichte erzählt.
											if(enr->m_vecAdjectiveType.at(i)==0)
												sv.push_back(ga->m_strWortstamm+_T("en"));
											if(enr->m_vecAdjectiveType.at(i)==1)
												sv.push_back(ga->m_strComperativ+_T("en"));
											if(enr->m_vecAdjectiveType.at(i)==2)
												sv.push_back(ga->m_strSuperlativ);
										}
										if(gn->m_bArticle=='2')
										{
	// My old woman is told a story. -> Der altEN Frau wird eine Geschichte erzählt.
											if(enr->m_vecAdjectiveType.at(i)==0)
												sv.push_back(ga->m_strWortstamm+_T("en"));
											if(enr->m_vecAdjectiveType.at(i)==1)
												sv.push_back(ga->m_strComperativ+_T("en"));
											if(enr->m_vecAdjectiveType.at(i)==2)
												sv.push_back(ga->m_strSuperlativ);
										}
										if(gn->m_bArticle=='3')
										{
	// My old child is told a story. -> Dem altEN Kind wird eine Geschichte erzählt.
											if(enr->m_vecAdjectiveType.at(i)==0)
												sv.push_back(ga->m_strWortstamm+_T("en"));
											if(enr->m_vecAdjectiveType.at(i)==1)
												sv.push_back(ga->m_strComperativ+_T("en"));
											if(enr->m_vecAdjectiveType.at(i)==2)
												sv.push_back(ga->m_strSuperlativ);
										}
									}
									else
									{
										if(Case==0) // kein Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("er")); // Er ist ein altER Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); // Sie ist ein altE Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); // Es ist ein altES Kind
										}
										if(Case==1) // 3. Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
										}
										if(Case==2) // 4. Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er mag einen altEN Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); // Er mag eine altE Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); // Er mag ein altES Kind
										}
									}
								}
							}
							//wenn das Substantiv im Deutschen im Plural steht
							//Beispiel: "Long hair needs washing." -> 
							//"Lange Haare müssen gewaschen werden."
							if(en->m_bTranslationType==1) // im Deutschen: Plural
							{
								personIndex=5;
#ifdef _DEBUG
								printf("im Deutschen: Plural\n");
#endif
								if(enr->bDefiniteArticle)
								{
#ifdef _DEBUG
									printf("bestimmter Artikel\n");
#endif
									if(enr->m_vecAdjectiveType.at(i)==0)
										sv.push_back(ga->m_strWortstamm+_T("en"));
									if(enr->m_vecAdjectiveType.at(i)==1)
										sv.push_back(ga->m_strComperativ+_T("en"));
									if(enr->m_vecAdjectiveType.at(i)==2)
									{
										if(ga->m_strSuperlativ.GetLength()>0)
											sv.push_back(ga->m_strSuperlativ.Left(ga->
											m_strSuperlativ.GetLength()-1));
									}
								}
								else if(enr->bIndefiniteArticle)
									sv.push_back(ga->m_strWortstamm+_T("e"));
								else
									sv.push_back(ga->m_strWortstamm+_T("e"));
								//Wenn das Token das erste im Satz ist, dann muss der 
								//erste Buchstabe durch MakeFirstLetterUpper() groß gemacht
								//werden.
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
							}
						}
						//wenn das Substantiv im Englischen im Plural steht
						else // im Englischen: Plural
						{
							//wenn das Substantiv im Deutschen auch im Plural steht
							//Beispiel: "Big cars are ugly." -> "Große Autos sind hässlich."
							if(en->m_bTranslationType==0) // wenn im Deutschen Plural
							{
								personIndex=5;
								if(enr->bDefiniteArticle) // bestimmter Artikel
								{
									// Beispiel: I like the old animals. -> Ich mag die altEN Tiere.
										if(enr->m_vecAdjectiveType.at(i)==0)
											sv.push_back(ga->m_strWortstamm+_T("en"));
										if(enr->m_vecAdjectiveType.at(i)==1)
											sv.push_back(ga->m_strComperativ+_T("en"));
									// Beispiel: I like the oldest animals. -> Ich mag die ältestEN Tiere.
										if(enr->m_vecAdjectiveType.at(i)==2)
										{
											if(ga->m_strSuperlativ.GetLength()>0)
												sv.push_back(ga->m_strSuperlativ.Left(ga->
												m_strSuperlativ.GetLength()-1));
										}
								}
								else if(enr->bIndefiniteArticle) // unbestimmter Artikel
									sv.push_back(ga->m_strWortstamm+_T("e"));
								else // gar kein Artikel
								{
									if(Case==1) // 3. Fall
										sv.push_back(ga->m_strWortstamm+_T("en"));
									// Beispiel: I trust old animals. -> Ich vertraue altEN Tieren.
									else
										sv.push_back(ga->m_strWortstamm+_T("e"));
									// Beispiel: I like old animals. -> Ich mag altE Tiere.
									// 2. Beispiel: They are old animals. -> Sie sind altE Tiere.
								}
								//Wenn das Token das erste im Satz ist, dann muss der 
								//erste Buchstabe durch MakeFirstLetterUpper() groß gemacht
								//werden.
								if(bFirstTokenInSentence)
								{
									MakeFirstLetterUpper(sv.at(0));
									bFirstTokenInSentence=FALSE;
								}
							}
							//wenn das Substantiv im Deutschen im Singular steht
							if(en->m_bTranslationType==2) // im Deutschen: Singular
							{
								personIndex=2;
								if(enr->bIndefiniteArticle) // unbestimmter Artikel
								{
									if(bTranslateAsObject)
									{
										if(gn->m_bArticle=='1')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
										if(gn->m_bArticle=='2')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
										if(gn->m_bArticle=='3')
											sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
									}
									else
									{
										if(Case==0) // kein Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("er")); // Er ist ein altER Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); // Sie ist ein altE Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); // Es ist ein altES Kind
										}
										if(Case==1) // 3. Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einer altEN Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er vertraut einem altEN Kind
										}
										if(Case==2) // 4. Fall
										{
											if(gn->m_bArticle=='1')
												sv.push_back(ga->m_strWortstamm+_T("en")); // Er mag einen altEN Mann
											if(gn->m_bArticle=='2')
												sv.push_back(ga->m_strWortstamm+_T("e")); // Er mag eine altE Frau
											if(gn->m_bArticle=='3')
												sv.push_back(ga->m_strWortstamm+_T("es")); // Er mag ein altES Kind
										}
									}
								}
								//wenn das Aufzählungselement des Subjekts einen bestimmten
								//Artikel enthält (es kam "the" vor).
								if(enr->bDefiniteArticle)
								{
									//wenn das Subjekt als Objekt übersetzt werden soll
									if(bTranslateAsObject)
									{
										if(enr->m_vecAdjectiveType.at(i)==0)
											sv.push_back(ga->m_strWortstamm+_T("en"));
										if(enr->m_vecAdjectiveType.at(i)==1)
											sv.push_back(ga->m_strComperativ+_T("en"));
										if(enr->m_vecAdjectiveType.at(i)==2)
										{
											if(ga->m_strSuperlativ.GetLength()>0)
												sv.push_back(ga->m_strSuperlativ.Left(ga->
												m_strSuperlativ.GetLength()-1));
										}
									}
									//wenn das Subjekt als Subjekt übersetzt werden soll
									//Beispiel: "I like the old scissors." -> 
									//"Ich mag die altE Schere."
									else
										sv.push_back(ga->m_strWortstamm+_T("e"));
								}
							}
						}
						vv.push_back(sv);
					}
				}
			}
		}
	}
	//wenn das Aufzählungselement des Subjekts ein Personalpronom enthält
	if(enr->m_personalPronoun)
	{
		CStringVector sv;
		if(enr->m_Noun.at(0)==_T("I"))
		{
			personIndex=0;
			sv.push_back(_T("ich"));
		}
		if(enr->m_Noun.at(0)==_T("you"))
		{
			personIndex=1;
			//Option o;
			sv.push_back(_T("du"));
			sv.push_back(_T("ihr"));
			sv.push_back(_T("sie"));
			//sv.index=0;
			//vv.push_back(sv);
		}
		if(enr->m_Noun.at(0)==_T("he"))
		{
			personIndex=2;
			sv.push_back(_T("er"));
		}
		if(enr->m_Noun.at(0)==_T("she"))
		{
			personIndex=2;
			sv.push_back(_T("sie"));
		}
		if(enr->m_Noun.at(0)==_T("it"))
		{
			personIndex=2;
			sv.push_back(_T("es"));
		}
		if(enr->m_Noun.at(0)==_T("we"))
		{
			personIndex=3;
			sv.push_back(_T("wir"));
		}
		if(enr->m_Noun.at(0)==_T("they"))
		{
			personIndex=5;
			sv.push_back(_T("sie"));
		}
		vv.push_back(sv);
	}
	else // sonst: Substantiv
	{
		#ifdef _DEBUG
		printf("Substantiv!\n");
		#endif
		CStringVector sv;
		for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
		{
			TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
		}
		std::vector <EnglishNoun> vecEN=GetEnglishNoun(enr->m_Noun);
		 // wenn der Substantiv-Vektor mind. 1 Element enthält
		if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
		{
			TRACE("enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
			#ifdef _DEBUG
			printf("enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
			#endif
			//std::vector <GermanNoun> vecGermanNoun=GetGermanVocableOfEnglishNoun2(enr->m_Noun);
			for(dwRun=0;dwRun<enr->
				m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size();dwRun++)
			{
				#ifdef _DEBUG
				printf("Schleifenbeginn\n");
				#endif
				GermanNoun * gn=
					GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
					enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
					at(dwRun).m_pWordNode);
				TRACE("vecGermanNoun\n");
				//GermanNoun gn=vecGermanNoun.at(dwRun);
				//wenn das Substantiv im Englischen im Singular steht
				if(enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
					at(dwRun).m_bSingular)
				{ // wenn im Englischen Singular
					TRACE("Singular\n");
					#ifdef _DEBUG
					printf("Substantiv ist Singular\n");
					#endif
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(dwRun).
						m_pWordNode->m_pWord);
					if(en!=NULL)
					{
						TRACE("en\n");
						#ifdef _DEBUG
						printf("en->m_strSingular: %s  en->m_strPlural: %s\n",
							en->m_strSingular,en->m_strPlural);
						#endif
						//wenn das Substantiv im Deutschen auch im Singular steht
						if(en->m_bTranslationType==0)
						{
							personIndex=2;
							sv.push_back(gn->m_strSingular);
						}
						//wenn das Substantiv im Deutschen im Plural steht
						if(en->m_bTranslationType==1) // im Deutschen: Plural
						{
							personIndex=2;
							//wenn das Subjekt als Objekt übersetzt werden soll
							if(bTranslateAsObject)
							{
								if(gn->m_strPlural.GetLength()>0)
								{
									if(gn->m_strPlural.Right(1)!=_T("n")) // Beispiel: Möbel -> den MöbelN
										sv.push_back(gn->m_strPlural+"n");
									else
										sv.push_back(gn->m_strPlural); // Beispiel: Informationen -> 
									//den Informationen
								}
								else
									sv.push_back(gn->m_strPlural); // Beispiel: Informationen -> 
								//den Informationen
							}
							//wenn das Subjekt als Subjekt übersetzt werden soll
							else
								sv.push_back(gn->m_strPlural);
						}
					}
				}
				else // im Englischen: Plural
				{
					TRACE("Plural\n");
					#ifdef _DEBUG
					printf("Substantiv ist Plural\n");
					#endif
					EnglishNoun * en=dynamic_cast<EnglishNoun*>(enr->
						m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
						at(dwRun).m_pWordNode->m_pWord);
					if(en!=NULL)
					{
						#ifdef _DEBUG
						printf("en\n");
						printf("en->m_strSingular: %s  en->m_strPlural: %s\n",
							en->m_strSingular,en->m_strPlural);
						printf("en->m_bTranslationType: %u\n",en->m_bTranslationType);
						TRACE("en\n");
						if(gn)
						{
							printf("gn\n");
							printf("gn->m_strPlural: %s\n",gn->m_strPlural);
						}
						#endif
						if(en->m_bTranslationType==0) // im Deutschen: Plural
						{
					#ifdef _DEBUG
							printf("im Deutschen: Plural\n");
					#endif
							personIndex=5;
							if(bTranslateAsObject)
							{
								if(gn->m_strPlural.GetLength()>0)
								{
									if(gn->m_strPlural.Right(1)!=_T("n")) // Beispiel: Kinder -> den KinderN
										sv.push_back(gn->m_strPlural+"n");
									else
										sv.push_back(gn->m_strPlural); // Beispiel: Frauen -> den Frauen
								}
								else
									sv.push_back(gn->m_strPlural); // Beispiel: Frauen -> den Frauen
							}
							else
							{
								if(Case==1 && gn->m_strPlural.GetLength()>0) // 3. Fall
									if(gn->m_strPlural.Right(1)=="e") // wenn letztes Zeichen gleich 'e'
									{
										// Beispiel: I trust old animals. -> Ich vertraue alten TiereN.
										sv.push_back(gn->m_strPlural+_T("n"));
										goto printf;
									}
								sv.push_back(gn->m_strPlural);
printf:
							#ifdef _DEBUG
							printf("nach: sv.push_back(gn->m_strPlural);\n");
							#endif
							;
							}
						}
						if(en->m_bTranslationType==2) // im Deutschen: Singular
						{
							personIndex=2;
							sv.push_back(gn->m_strSingular);
						}
					}
				}
			}
			//Wenn das Token das erste im Satz ist, dann muss der 
			//erste Buchstabe durch MakeFirstLetterUpper() groß gemacht
			//werden.
			if(bFirstTokenInSentence)
			{
				MakeFirstLetterUpper(sv.at(0));
				bFirstTokenInSentence=FALSE;
			}
			vv.push_back(sv);
			#ifdef _DEBUG
			printf("nach: vv.push_back(sv);\n");
			#endif
		}
		#ifdef _DEBUG
		else
		{
			printf("!enr->m_vecPointerToVocNodeWhereAnEnglishNounIsEncapsulated.size()>0\n");
		}
		#endif
	}
	#ifdef _DEBUG
	for(i=0;i<vv.size();i++)
		printf("vv.at(%d).at(0).m_Str: %s ",i,vv.at(i).at(0));
	printf("\nstd::vector<CStringVector> TranslateENR(EnglishNounRecord * enr,\
BYTE Case,BOOL bNegation,BOOL bTranslateAsObject,BYTE & personIndex,\
BYTE & bFirstTokenInSentence,BYTE & bArticle) ENDE\n");
	#endif
	return vv;
}

CStringVector	TranslateEnumerationElement(EnumerationElement ee)
{
	CStringVector sv;
	BOOL bIndefiniteArticle=FALSE;
	BOOL bDefiniteArticle=FALSE;
	if(ee.m_bArticle)
	{
		GermanNoun * gn=GetGermanWordOfEnglishNoun(ee.m_Noun);
		for(DWORD i=0;i<ee.m_Noun.size();i++)
			TRACE("ee.m_Noun.at(i): %s",ee.m_Noun.at(i));
		if(IsEnglishSingular(ee.m_Noun))
		{
			if(gn->m_bArticle=='1')
				sv.push_back("der");
			if(gn->m_bArticle=='2')
				sv.push_back("die");
			if(gn->m_bArticle=='3')
				sv.push_back("das");
		}
		else
			sv.push_back("die");
	}
	for(DWORD i=0;i<ee.m_Adjectives.size();i++)
	{
		GermanNoun * gn=GetGermanWordOfEnglishNoun(ee.m_Noun);
		if(!ee.m_bArticle)
		{
			if(gn->m_bArticle=='1')
				sv.push_back("der");
			if(gn->m_bArticle=='2')
				sv.push_back("die");
			if(gn->m_bArticle=='3')
				sv.push_back("das");
		}
		GermanAdjective * ga=GetGermanWordOfEnglishAdjective(
			ee.m_Adjectives.at(i));
		if(IsEnglishSingular(ee.m_Noun))
			sv.push_back(ga->m_strWortstamm+"e");
		else
			sv.push_back(ga->m_strWortstamm+"en");
	}
	if(ee.m_personalPronoun)
	{
		if(ee.m_Noun.at(0)=="I")
			sv.push_back("ich");
		if(ee.m_Noun.at(0)=="you")
			sv.push_back("du");
		if(ee.m_Noun.at(0)=="he")
			sv.push_back("er");
		if(ee.m_Noun.at(0)=="she")
			sv.push_back("sie");
		if(ee.m_Noun.at(0)=="it")
			sv.push_back("es");
		if(ee.m_Noun.at(0)=="we")
			sv.push_back("wir");
		if(ee.m_Noun.at(0)=="they")
			sv.push_back("sie");
	}
	else
	{
		GermanNoun * gn=GetGermanWordOfEnglishNoun(ee.m_Noun);
		if(IsEnglishSingular(ee.m_Noun))
			sv.push_back(gn->m_strSingular);
		else
			sv.push_back(gn->m_strPlural);
	}
	TRACE("CStringVector TranslateEnumerationElement(EnumerationElement ee) ENDE\n");
	return sv;
}

//wird von TranslateSubject() aufgerufen
//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
// m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
// zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
// (Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
// ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
// Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
// auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
// (Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
// pEV wird aber nichts in TranslateEnumerationElement2(const EnumerationElement 
// & ee,EnglishView * pEV,int & nStartID); hinzugefügt, sondern dient zur 
// Weiterleitung an die Funktion TranslateRelativeClause, die eventuell 
// innerhalb TranslateEnumerationElement(const EnumerationElement & ee,
// EnglishView * pEV,int & nStartID); aufgerufen wird
//nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
// die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
// bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
// Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
// sich das "folgst" automatisch in "folgt" ändern
std::vector<CStringVector> TranslateEnumerationElement(
  const EnumerationElement & ee,
  BOOL bTranslateAsObject,
  BYTE & personIndex,
	BYTE & bFirstTokenInSentence,//EnglishView * pEV
  std::vector<IntPair> & rvecintpair,
  int & nStartID
  )
{
	BYTE bArticle=0;
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateEnumerationElement(const \
EnumerationElement & ee,BOOL bTranslateAsObject,BYTE & personIndex,\
BYTE & bFirstTokenInSentence) ANFANG\n");
	printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
#endif
	std::vector<CStringVector> vv;
	//wenn das Aufzählungselement des Subjekts ein Eigenname ist
	if(ee.m_vecStrPlainText.size()>0)
	{
	#ifdef _DEBUG
		printf("ee.m_vecStrPlainText.size()>0\n");
	#endif
		personIndex=2;
		for(DWORD dwRun=0;dwRun<ee.m_vecStrPlainText.size();dwRun++)
		{
			CStringVector sv;
#ifdef _DEBUG
			printf("ee.m_vecStrPlainText.at(%u): %s\n",dwRun,ee.m_vecStrPlainText.
				at(dwRun));
#endif
			sv.push_back(ee.m_vecStrPlainText.at(dwRun));
			vv.push_back(sv);
		}
	}
	//wenn das Aufzählungselement des Subjekts ein Personalpronomen ist
	if(ee.m_personalPronoun)
	{
		#ifdef _DEBUG
		printf("ee.m_personalPronoun: %u\n",ee.m_personalPronoun);
		#endif
		CStringVector sv;
		if(bTranslateAsObject)
		{
			switch(ee.m_personalPronoun)
			{
				case 1:
					personIndex=0;
					sv.push_back(_T("mir"));
					break;
				case 2:
					personIndex=1;
					sv.push_back(_T("dir"));
					sv.push_back(_T("euch"));
					sv.push_back(_T("Ihnen"));
					//++nStartID;
					break;
				case 3:
					personIndex=2;
					sv.push_back(_T("ihm"));
					break;
				case 4:
					personIndex=2;
					sv.push_back(_T("ihr"));
					break;
				case 5:
					personIndex=2;
					sv.push_back(_T("es"));
					break;
				case 6:
					personIndex=3;
					sv.push_back(_T("uns"));
					break;
				case 7:
					personIndex=5;
					sv.push_back(_T("ihnen"));
					break;
			}
		}
		else
		{
			switch(ee.m_personalPronoun)
			{
				case 1:
					personIndex=0;
					sv.push_back(_T("ich"));
					break;
				case 2:
					personIndex=1;
					sv.push_back(_T("du"));
					sv.push_back(_T("ihr"));
					sv.push_back(_T("Sie"));
					sv.push_back(_T("man"));
					//++nStartID;
					break;
				case 3:
					personIndex=2;
					sv.push_back(_T("er"));
					break;
				case 4:
					personIndex=2;
					sv.push_back(_T("sie"));
					break;
				case 5:
					personIndex=2;
					sv.push_back(_T("es"));
					break;
				case 6:
					personIndex=3;
					sv.push_back(_T("wir"));
					break;
				case 7:
					personIndex=5;
					sv.push_back(_T("sie"));
					break;
			}
		}
		if(bFirstTokenInSentence)
		{
			for(DWORD dwRun=0;dwRun<sv.size();dwRun++)
				MakeFirstLetterUpper(sv.at(dwRun));
			bFirstTokenInSentence=FALSE;
		}
		vv.push_back(sv);
		//sv.push_back(GetGermanPersonalPronoun(CString & str));
	}
	//wenn das Aufzählungselement des Subjekts ein Substantiv mit bestimmtem
	//Artikel ist
	if(ee.m_pDan)
	{
		#ifdef _DEBUG
		printf("ee.dan\n");
		{
			for(DWORD dwRun=0;dwRun<ee.m_pDan->m_Noun.size();dwRun++)
			{
				printf("ee.m_pDan->m_Noun.at(%u): %s\n",dwRun,ee.m_pDan->m_Noun.at(dwRun));
			}
		}
		#endif
		std::vector <EnglishNoun> vecEN=GetEnglishNoun(ee.m_pDan->m_Noun);
		if(vecEN.size()>0) // wenn der Vektor mind. 1 Element enthält
		{
			CStringVector sv,sv2;
			std::vector <GermanNoun> vecGermanNoun=GetGermanWordOfEnglishNoun2(
				ee.m_pDan->m_Noun);
			for(DWORD dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
			{
				TRACE("vecGermanNoun\n");
				GermanNoun gn=vecGermanNoun.at(dwRun);
				if(IsEnglishSingular(ee.m_pDan->m_Noun)) // wenn das englische Nomen im Singular ist
				{
					if(vecEN.at(0).m_bTranslationType==0)
					{
						//wenn das Aufzählungselement im Englishen ein Subjekt ist
						//und im Deutschen als Objekt übersetzt werden soll
						personIndex=2;
						if(bTranslateAsObject)
						{
							if(gn.m_bArticle=='1') // wenn männliches Substantiv
																					// The man has been told a story. 
								sv2.push_back(_T("dem")); // -> DEM Mann ist eine Geschichte erzählt worden.
							else if(gn.m_bArticle=='2') // weibliches Substantiv
								// The woman has been told a story.
								sv2.push_back(_T("der")); // -> DER Frau ist eine Geschichte erzählt worden.
							else if(gn.m_bArticle=='3') // sächliches Substantiv
								// The child has been told a story. 
								sv2.push_back(_T("dem")); // -> DEM Kind ist eine Geschichte erzählt worden.
							else 
								sv2.push_back(_T("das"));
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv2.at(0));
								bFirstTokenInSentence=FALSE;
							}
							sv.push_back(gn.m_strSingular);
						}
						//das Aufzählungselement ist im Englishen ein Subjekt und soll
						//auch im Deutschen als Subjekt übersetzt werden
						else
						{
							if(gn.m_bArticle=='1') // wenn männliches Substantiv 
																					// The man has been told a story. 
								sv2.push_back(_T("der")); // -> DEM Mann ist eine Geschichte erzählt worden.
							else if(gn.m_bArticle=='2') // weibliches Substantiv
								// The woman has been told a story.
								sv2.push_back(_T("die")); // -> DER Frau ist eine Geschichte erzählt worden.
							else if(gn.m_bArticle=='3') // sächliches Substantiv
								// The child has been told a story. 
								sv2.push_back(_T("das")); // -> DEM Kind ist eine Geschichte erzählt worden.
							else 
								sv2.push_back(_T("das"));
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv2.at(0));
								bFirstTokenInSentence=FALSE;
							}
							bArticle=gn.m_bArticle-49;
							sv.push_back(gn.m_strSingular);
						}
					}
					if(vecEN.at(0).m_bTranslationType==1) // m_TranslationType==1 bedeutet, dass
					// ein Wort, das im Englischen im Singular steht, im Deutschen im Plural ist
					// (Beispiel: the hair -> die Haare)
					{
						personIndex=5;
						if(bTranslateAsObject)
							sv2.push_back(_T("den"));
						else
							sv2.push_back(_T("die"));
						if(bFirstTokenInSentence)
						{
							MakeFirstLetterUpper(sv2.at(0));
							bFirstTokenInSentence=FALSE;
						}
						bArticle=3;
						sv.push_back(gn.m_strPlural);
					}
				}
				else // im Englischen: Plural
				{
					if(vecEN.at(0).m_bTranslationType==0) // im Deutschen Plural
					{ // The children have been told a story.
						personIndex=5;
						if(bTranslateAsObject)
							sv2.push_back(_T("den"));
						else
							sv2.push_back(_T("die"));
						if(bFirstTokenInSentence)
						{
							MakeFirstLetterUpper(sv2.at(0));
							bFirstTokenInSentence=FALSE;
						}
						bArticle=3;
						sv.push_back(gn.m_strPlural);
					}
					if(vecEN.at(0).m_bTranslationType==2) // m_TranslationType==1 bedeutet, dass
					// ein Wort, das im Englischen im Plural steht, im Deutschen im Singular ist
					// (Beispiel: the scissors -> die Schere)
					{
						personIndex=2;
						if(bTranslateAsObject)
						{
							if(gn.m_bArticle=='1') // wenn männliches Substantiv 
																					// The man has been told a story. 
								sv2.push_back(_T("dem")); // -> DEM Mann ist eine Geschichte erzählt worden.
							else if(gn.m_bArticle=='2') // weibliches Substantiv
								// The woman has been told a story.
								sv2.push_back(_T("der")); // -> DER Frau ist eine Geschichte erzählt worden.
							else if(gn.m_bArticle=='3') // sächliches Substantiv
								// The child has been told a story. 
								sv2.push_back(_T("dem")); // -> DEM Kind ist eine Geschichte erzählt worden.
							else 
								sv2.push_back(_T("das"));
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv2.at(0));
								bFirstTokenInSentence=FALSE;
							}
							sv.push_back(gn.m_strSingular);
						}
						else
						{
							if(gn.m_bArticle=='1') // wenn männliches Substantiv 
																					// The man has been told a story. 
								sv2.push_back(_T("der")); // -> DEM Mann ist eine Geschichte erzählt worden.
							else if(gn.m_bArticle=='2') // weibliches Substantiv
								// The woman has been told a story.
								sv2.push_back(_T("die")); // -> DER Frau ist eine Geschichte erzählt worden.
							else if(gn.m_bArticle=='3') // sächliches Substantiv
								// The child has been told a story. 
								sv2.push_back(_T("das")); // -> DEM Kind ist eine Geschichte erzählt worden.
							else 
								sv2.push_back(_T("das"));
							if(bFirstTokenInSentence)
							{
								MakeFirstLetterUpper(sv2.at(0));
								bFirstTokenInSentence=FALSE;
							}
							bArticle=gn.m_bArticle-49;
							sv.push_back(gn.m_strSingular);
						}
					}
				}
			}
			vv.push_back(sv2);
			vv.push_back(sv);
		}
	}
	if(ee.m_pEnr)
	{
    EngNounsLinkedList engnounll(
      ee.m_pEnr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated
      ) ;
    #ifdef _DEBUG
		printf("ee.m_pEnr\n");
		#endif
		Append(vv,TranslateENR(ee.m_pEnr,0,//FALSE,
      false,(bool)bTranslateAsObject,personIndex,
		bFirstTokenInSentence,bArticle,nStartID,engnounll));
	}
	if(ee.m_pOfPhrase)
	{
		#ifdef _DEBUG
		printf("ee.m_OfPhrase\n");
		#endif
		Append(vv,TranslateOfPhrase(ee.m_pOfPhrase->m_Enr,nStartID));
	}
	if(ee.m_pGerund)
	{
		personIndex=2;
		Append(vv,TranslateGerund(*ee.m_pGerund,bFirstTokenInSentence,//pEV,
      rvecintpair,
			nStartID));
	}
	if(ee.m_pRelativeClause)
	{
		CStringVector sv;
		sv.push_back(_T(","));
		vv.push_back(sv);
		//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
		//m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
		//zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
		//(Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
		//ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen
		//steht. Wenn man bei dem einen CComboBox-(Kombinationsfeld-)
		//Steuerelement auswählt, soll dann nämlich auch in dem zugehörigen 
		//CComboBox-(Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt 
		//werden.
		Append(vv,TranslateRelativeClause(*ee.m_pRelativeClause,bArticle,
			//pEV
      rvecintpair,nStartID));
		vv.push_back(sv);
	}
	TRACE("CStringVector TranslateEnumerationElement2(EnumerationElement ee) ENDE\n");
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateEnumerationElement(const EnumerationElement & ee,BOOL bTranslateAsObject) ENDE\n");
#endif
	return vv;
}

/*std::vector<CStringVector> TranslateEnumerationElement2(const EnumerationElement & ee)
{
	TRACE("CStringVector TranslateEnumerationElement2(EnumerationElement ee) ANFANG\n");
	#ifdef _DEBUG
	printf("CStringVector TranslateEnumerationElement2(EnumerationElement ee) ANFANG\n");
	#endif
	std::vector<CStringVector> vv;
	if(ee.m_vecStrPlainText.size()>0)
	{
		for(DWORD dwRun=0;dwRun<ee.m_vecStrPlainText.size();dwRun++)
		{
			CStringVector sv;
			sv.push_back(ee.m_vecStrPlainText.at(dwRun));
			vv.push_back(sv);
		}
	}
	if(ee.m_personalPronoun)
	{
		#ifdef _DEBUG
		printf("ee.m_personalPronoun: %u\n",ee.m_personalPronoun);
		#endif
		CStringVector sv;
		switch(ee.m_personalPronoun)
		{
			case 1:
				sv.push_back("ich");
				break;
			case 2:
				sv.push_back("du");
				sv.push_back("ihr");
				sv.push_back("Sie");
				break;
			case 3:
				sv.push_back("er");
				break;
			case 4:
				sv.push_back("sie");
				break;
			case 5:
				sv.push_back("es");
				break;
			case 6:
				sv.push_back("wir");
				break;
			case 7:
				sv.push_back("sie");
				break;
		}
		vv.push_back(sv);
		//sv.push_back(GetGermanPersonalPronoun(CString & str));
	}
	if(ee.m_pDan)
	{
		#ifdef _DEBUG
		printf("ee.m_pDan\n");
		{
			for(DWORD dwRun=0;dwRun<ee.m_pDan->m_Noun.size();dwRun++)
			{
				printf("ee.m_pDan->m_Noun.at(%u): %s\n",dwRun,ee.m_pDan->m_Noun.at(dwRun));
			}
		}
		#endif
		std::vector <EnglishNoun> vecEN=GetEnglishNoun(ee.m_pDan->m_Noun);
		if(vecEN.size()>0) // wenn der Vektor mind. 1 Element enthält
		{
			CStringVector sv,sv2;
			std::vector <GermanNoun> vecGermanNoun=
				GetGermanWordOfEnglishNoun2(ee.m_pDan->m_Noun);
			for(DWORD dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
			{
				TRACE("vecGermanNoun\n");
				GermanNoun gn=vecGermanNoun.at(dwRun);
				if(IsEnglishSingular(ee.m_pDan->m_Noun)) // wenn das englische Nomen Singular ist
				{
					if(vecEN.at(0).m_bTranslationType==0)
					{
						if(gn.m_bArticle=='1')
							sv2.push_back(_T("der"));
						else if(gn.m_bArticle=='2')
							sv2.push_back(_T("die"));
						else if(gn.m_bArticle=='3')
							sv2.push_back(_T("das"));
						else 
							sv2.push_back(_T("das"));
						sv.push_back(gn.m_strSingular);
					}
					if(vecEN.at(0).m_bTranslationType==1) // m_TranslationType==1 bedeutet, dass
					// ein Wort, das im Englischen im Singular steht, im Deutschen im Plural ist
					// (Beispiel: the scissors -> die Schere)
					{
						sv2.push_back(_T("die"));
						sv.push_back(gn.m_strPlural);
					}
				}
				else
				{
					if(vecEN.at(0).m_bTranslationType==0)
					{
						sv2.push_back(_T("die"));
						sv.push_back(gn.m_strPlural);
					}
					if(vecEN.at(0).m_bTranslationType==2) // m_TranslationType==1 bedeutet, dass
					// ein Wort, das im Englischen im Plural steht, im Deutschen im Singular ist
					// (Beispiel: the scissors -> die Schere)
					{
						if(gn.m_bArticle=='1')
							sv2.push_back(_T("der"));
						else if(gn.m_bArticle=='2')
							sv2.push_back(_T("die"));
						else if(gn.m_bArticle=='3')
							sv2.push_back(_T("das"));
						else 
							sv2.push_back(_T("das"));
						sv.push_back(gn.m_strSingular);
					}
				}
			}
			vv.push_back(sv2);
			vv.push_back(sv);
		}

		//vv.push_back(
		//Append(vv,
	}
	if(ee.m_pEnr)
	{
		BYTE bPersonIndex=0;
		BYTE bFirstTokenInSentence=0;
		#ifdef _DEBUG
		printf("ee.m_pEnr\n");
		#endif
		Append(vv,TranslateENR(ee.m_pEnr,0,FALSE,FALSE,bPersonIndex,
			bFirstTokenInSentence));
	}
	if(ee.m_pOfPhrase)
	{
		#ifdef _DEBUG
		printf("ee.m_pOfPhrase\n");
		#endif
		//Append(vv,TranslateOfPhrase(ee.m_pOfPhrase->m_Enr,nStartID));
	}
	if(ee.m_pGerund)
	{
		BYTE b=0;
		//Append(vv,TranslateGerund(*ee.m_pGerund,b,nStartID));
	}
	if(ee.m_pToPlusInfinitive)
		Append(vv,TranslateToPlusInfinitive(*ee.m_pToPlusInfinitive,nStartID));
	TRACE("CStringVector TranslateEnumerationElement2(EnumerationElement ee) ENDE\n");
#ifdef _DEBUG
	printf("CStringVector TranslateEnumerationElement2(EnumerationElement ee) ENDE\n");
#endif
	return vv;
}*/

//wird von TranslateEnumerationElement() aufgerufen
//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
// m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
// zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
// (Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
// ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
// Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
// auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
// (Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
// Ein Gerundium kann Subjekt und Prädikat haben, wenn es ein Objekt eines 
// Haupt- oder Nebensatzes ist: "I like you hurting me." -> "Ich mag, wenn du|
// ihr|Sie|man mich verletzst|verletzt|verletzen|verletzt."
//nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
// die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
// bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
// Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
// sich das "folgst" automatisch in "folgt" ändern
// Es kann folgenden Satz geben: "I like hurting you." "hurting you": 
// Gerundium -> "Ich mag |dich| zu verletzen."
//                      |Sie |
//                      |euch|
std::vector<CStringVector> TranslateGerund(const Gerund & gerund,BYTE & 
	bFirstTokenInSentence,//EnglishView * pEV,
  std::vector<IntPair> & rvecintpair,int & nStartID)
{
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateGerund(const Gerund & gerund) ANFANG\n");
	#endif
	std::vector<CStringVector> vv;
	//wenn das Gerundium ein perönliches Subjekt beinhaltet
	//Beispiel: "I like him being happy." -> "Ich mag, dass er glücklich ist."
	//           S --P- ---Gerundium---
	//                  -PS
	//PS (persönliches Subjekt): "him"
	if(gerund.m_pPersonalSubject)
	{
		CStringVector sv;
		sv.push_back(_T(","));
		vv.push_back(sv);
		sv.clear();
		sv.push_back(_T("dass"));
		vv.push_back(sv);
		BYTE b=0;
		Append(vv,TranslateSubject(*gerund.m_pPersonalSubject,FALSE,b,
			bFirstTokenInSentence,//pEV,
      rvecintpair,nStartID));
		if(gerund.m_pPlace)
			Append(vv,TranslatePlace(*gerund.m_pPlace,b,nStartID));
	}
	//wenn das Gerundium eine Präposition beinhaltet
	if(gerund.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated)
	{
		if(gerund.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated->
			m_pWordNodeNext)
		{
			GermanPreposition * gp=dynamic_cast<GermanPreposition *>(gerund.
				m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			CStringVector sv;
			sv.push_back(gp->m_strWord);
			#ifdef _DEBUG
			printf("Präposition: %s\n",gp->m_strWord);
			#endif
			vv.push_back(sv);
		}
	}
	//wenn das Gerundium ein Objekt beinhaltet
	//Beispiel: "Liking you is great." -> "Dich zu mögen ist grossartig."
	if(gerund.m_pObject)
	{
		// der Wert von 2 für den 2. Parameter für den 4. Fall (Akkusativ)
		Append(vv,TranslateObject2(*gerund.m_pObject,2,FALSE,
			bFirstTokenInSentence,//pEV,
      rvecintpair,nStartID));
	}
	//wenn das Gerundium ein perönliches Subjekt beinhaltet
	//Beispiel: "I like him being happy." -> "Ich mag, dass er glücklich ist."
	//           S --P- ---Gerundium---
	//                  -PS
	//PS (persönliches Subjekt): "him"
	if(gerund.m_pPersonalSubject)
	{
		//wenn das Gerundium eine Umstandsbestimmung der Art und Weise enthält
		//Beispiel: "I like him working hard". Gerundium: "him working hard" 
		//Umstandsbestimmung der Art und Weise: "hard".
		//if(gerund.m_pManner)
		//	vv.push_back(TranslateManner(*gerund.m_pManner));
		if(gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated)
			if(gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext)
			{
				GermanVerb * gv=dynamic_cast<GermanVerb*>(gerund.
					m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext->m_pWord);
				if(gv)
				{
					CStringVector sv;
					CStringVector svVerb=GetCStringVector2(gv->m_strWords[gerund.
						m_pPersonalSubject->m_personIndex]);
					// aus dem String einen Vektor aus Strings generieren
					if(svVerb.size()>1) // Bespiel: 'aufhören' - ('er') 'hört auf'
					{
						sv.push_back(svVerb.at(1)+svVerb.at(0)); // Beispiel: 'auf'+'hört'
						// zusammen mit mit ', dass' und mit dem übersetzten Subjekt könnte
						// ein Beispielsatz so aussehen: I like him stopping. ->
						// Ich mag, dass er aufhört.
						vv.push_back(sv);
					}
					if(svVerb.size()==1)
					{
						vv.push_back(svVerb);
					}
				}
			}
	}
	//wenn das Gerundium eine Verneinung (Negation) enthält
	//Beispiel: "Not hating you is great." -> "Dich nicht zu hassen ist grossartig."
	if(gerund.m_bNegation)
	{
		CStringVector sv;
		sv.push_back(_T("nicht"));
		if(bFirstTokenInSentence)
		{
			MakeFirstLetterUpper(sv.at(0));
			bFirstTokenInSentence=FALSE;
		}
		vv.push_back(sv);
	}
	//wenn das Gerundium KEIN persönliches Subjekt und eine Umstandsbestimmung
	//des Ortes beinhaltet
	//Beispiel: "Living in Germany is great." -> "In Deutschland zu leben ist
	//grossartig."      Gerundium: "Living in Germany"  
	//Umstandsbestimmung des Ortes: "in Germany"
	if(!gerund.m_pPersonalSubject && gerund.m_pPlace)
		Append(vv,TranslatePlace(*gerund.m_pPlace,bFirstTokenInSentence,nStartID));
	//wenn das Gerundium KEIN persönliches Subjekt enthält
	//Bespiele: "I like working." "Working is great."
	if(!gerund.m_pPersonalSubject)
	{
		GermanVerb * gv=NULL;
		if(gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated)
			if(gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext)
				gv=dynamic_cast<GermanVerb*>(gerund.
				m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			//GetGermanWordOfEnglishVerb(gerund.m_predicate);
		//wenn das Gerundium im aktiv ist
		if(gerund.m_bActiveForm) // Aktivform
		{ // Beispiel: I like playing. -> Ich mag zu spielen.
			CStringVector sv;
			//wenn die Zeitform Perfekt ist
			// Beispiel: I like having played. -> Ich mag gespielt zu haben.
			// "Having played is great". -> "Gespielt zu haben ist grossartig."
			if(gerund.m_bTense==PRESENT_PERFECT)
			{
				sv.push_back(gv->m_strWords[13]); // Partizip Perfekt (z.B. "gespielt")
				if(bFirstTokenInSentence)
				{
					MakeFirstLetterUpper(sv.at(0));
					bFirstTokenInSentence=FALSE;
				}
				vv.push_back(sv);
				sv.clear();
			}
			CStringVector svVerb=GetCStringVector2(gv->m_strWords[4]);
			//wenn die Zeitform Perfekt ist
			if(svVerb.size()==1 || gerund.m_bTense==PRESENT_PERFECT)
			{
				sv.push_back(_T("zu"));
				vv.push_back(sv);
				sv.clear();
			}
			//wenn die Zeitform Perfekt ist
			if(gerund.m_bTense==PRESENT_PERFECT)
			{
				sv.push_back(_T("haben"));
			}
			//wenn die Zeitform NICHT Perfekt ist
			else
			{
				//wenn das Verb ein teilbares Verb ist
				//Beispiel: "aussehen"- "ich sehe aus"
				//im Gegensatz zu unteilbaren Verben wie: "sehen"-"ich sehe"
				if(svVerb.size()>1)
					sv.push_back(svVerb.at(1)+_T("zu")+svVerb.at(0));
					// Bespiel: Looking well is important.
					// Deutsche Übersetzung: Gut auszusehen ist wichtig.
					// wir nehmen die gebeugte Form des Verbes in der 1.
					// Person Plural
					// aussehen - wir sehen aus
					//									1    2
					// -> auszusehen
					//     2     1
				//wenn das Verb ein unteilbares Verb ist
				//Beispiel: "sehen"- "ich sehe"
				//im Gegensatz zu teilbaren Verben wie: "aussehen"-"ich sehe aus"
				//Beispielsatz: "Working is great."->"Arbeiten ist grossartig."
				else
					sv.push_back(gv->m_strWords[0]); // Infinitiv
				if(bFirstTokenInSentence)
				{
					MakeFirstLetterUpper(sv.at(0));
					bFirstTokenInSentence=FALSE;
				}
				#ifdef _DEBUG
				printf("Infinitiv: %s\n",gv->m_strWords[0]);
				#endif
			}
			vv.push_back(sv);
		}
		if(!gerund.m_bActiveForm)
		{ // wenn Passiv (Beispiel: "I do not like being observed." ->
			// "Ich mag nicht beobachtet zu werden.")
			// "Being observed is great." -> "Beobachtet zu werden is grossartig."
			CStringVector sv;
			sv.push_back(gv->m_strWords[13]); // Partizip Perfekt
			if(bFirstTokenInSentence)
			{
				MakeFirstLetterUpper(sv.at(0));
				bFirstTokenInSentence=FALSE;
			}
			vv.push_back(sv);
			sv.clear();
			//wenn die Zeitform Perfekt ist
			//Beispielsatz: "Having been observed is great." ->
			//"Beobachtet worden zu sein ist grossartig."
			if(gerund.m_bTense==PRESENT_PERFECT)
			{
				sv.push_back(_T("worden"));
				vv.push_back(sv);
				sv.clear();
			}
			sv.push_back(_T("zu"));
			vv.push_back(sv);
			sv.clear();
			if(gerund.m_bTense==PRESENT_PERFECT)
			{
				sv.push_back(_T("sein"));
				vv.push_back(sv);
			}
			else
			{
				sv.push_back(_T("werden"));
				vv.push_back(sv);
			}
		}
	}
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateGerund(const Gerund & gerund) \
ENDE return vv\n");
	#endif
	return vv;
}

std::vector<CStringVector> TranslateManner(const Manner & manner,int & nStartID)
{
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateManner(const Manner & manner) ANFANG\n");
#endif
	std::vector<CStringVector> vv;
	BOOL bTranslated=FALSE;
	for(DWORD dwRun=0;dwRun<manner.m_vecMannerEnumerationElement.size();dwRun++)
	{
		if(dwRun+1==manner.m_vecMannerEnumerationElement.size() && bTranslated)
		{
			CStringVector sv;
			sv.push_back(_T("und"));
			vv.push_back(sv);
		}
		Append(vv,TranslateMannerEnumerationElement(manner.
			m_vecMannerEnumerationElement.at(dwRun),nStartID));
		bTranslated=TRUE;
	}
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateManner(const Manner & manner) return vv\n");
#endif
	return vv;
}

CString TranslateMannerAsCString(const Manner & manner)
{
	CString str;
	BOOL bTranslated=FALSE;
	for(DWORD dwRun=0;dwRun<manner.m_vecMannerEnumerationElement.size();dwRun++)
	{
		if(dwRun+1==manner.m_vecMannerEnumerationElement.size() && bTranslated)
		{
			str+=_T(" und ");
		}
		//Append(vv,TranslateMannerEnumerationElement(manner.m_vecMannerEnumerationElements.at(dwRun)));
		str+=TranslateMannerEnumerationElementAsCString(manner.
			m_vecMannerEnumerationElement.at(dwRun));
		bTranslated=TRUE;
	}
	return str;
}

std::vector<CStringVector> TranslateMannerEnumerationElement(const 
	MannerEnumerationElement & mee,int & nStartID)
{
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateMannerEnumerationElement(const \
MannerEnumerationElement & mee) ANFANG\n");
#endif
	std::vector<CStringVector> vv;
	if(mee.m_pWordNodeAdverbRefersToAdjectiveOrAdverb)
	{
		if(mee.m_pWordNodeAdverbRefersToAdjectiveOrAdverb->m_pWordNodeNext)
		{
			GermanAdverb * ga=dynamic_cast<GermanAdverb*> 
				(mee.m_pWordNodeAdverbRefersToAdjectiveOrAdverb->
				m_pWordNodeNext->m_pWord);
			CStringVector sv;
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	if(mee.m_pWordNodeAdverbOfManner)
	{
#ifdef _DEBUG
		printf("mee.m_pWordNodeAdverbOfManner\n");
#endif
		if(mee.m_pWordNodeAdverbOfManner->m_pWordNodeNext)
		{
			GermanAdjective * ga=dynamic_cast<GermanAdjective*> 
				(mee.m_pWordNodeAdverbOfManner->m_pWordNodeNext->m_pWord);
			CStringVector sv;
			sv.push_back(ga->m_strPositiv);
			vv.push_back(sv);
		}
	}
	if(mee.m_pSimilarity)
		Append(vv,TranslateSimilarity(*mee.m_pSimilarity,nStartID));
	if(mee.m_pComparison)
		Append(vv,TranslateComparison(*mee.m_pComparison,nStartID));
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateMannerEnumerationElement(const \
MannerEnumerationElement & mee) return vv\n");
#endif
	return vv;
}

CString TranslateMannerEnumerationElementAsCString(const MannerEnumerationElement & mee)
{
	CString str;
	if(mee.m_pWordNodeAdverbRefersToAdjectiveOrAdverb)
	{
		if(mee.m_pWordNodeAdverbRefersToAdjectiveOrAdverb->m_pWordNodeNext)
		{
			GermanAdverb * ga=dynamic_cast<GermanAdverb*> (mee.
				m_pWordNodeAdverbRefersToAdjectiveOrAdverb->m_pWordNodeNext->m_pWord);
			str+=ga->m_strWord;
		}
	}
	if(mee.m_pWordNodeAdverbOfManner)
	{
		if(mee.m_pWordNodeAdverbOfManner->m_pWordNodeNext)
		{
			GermanAdjective * ga=dynamic_cast<GermanAdjective*> (mee.
				m_pWordNodeAdverbOfManner->m_pWordNodeNext->m_pWord);
			if(str.GetLength()>0)
				str+=" ";
			str+=ga->m_strPositiv;
		}
	}
	return str;
}

CStringVector TranslateObject(Object object)
{
	TRACE("CStringVector TranslateObject(Object object) ANFANG\n");
	CStringVector retVec;
	BOOL bFirst=TRUE;
	for(DWORD i=0;i<object.m_vecEnumerationElement.size();i++)
	{
		if(!bFirst)
			if(i!=object.m_vecEnumerationElement.size()-1) // wenn nicht das letzte Element
				retVec.push_back(",");
			else
				retVec.push_back("und");

		CStringVector cstrvec=TranslateObjectEnumerationElement(object.
			m_vecEnumerationElement.at(i),0);
		Append(retVec,cstrvec);

		bFirst=FALSE;
	}
	TRACE("CStringVector TranslateObject(Object object) ENDE\n");
	return retVec;

}

/*std::vector<CStringVector> TranslateObject2(Object object)
{
	TRACE("CStringVector TranslateObject(Object object) ANFANG\n");
	std::vector<CStringVector> vv;
	BOOL bFirst=TRUE;
	for(int i=0;i<object.m_vecEnumerationElement.size();i++)
	{
		if(!bFirst)
		{
			CStringVector sv;
			if(i!=object.m_vecEnumerationElement.size()-1) // wenn nicht das letzte Element
				sv.push_back(",");
			else
				sv.push_back("und");
			vv.push_back(sv);
		}
		std::vector<CStringVector> vvObject=TranslateObjectEnumerationElement2(
			object.m_vecEnumerationElement.at(i),0,nStartID);
		Append(vv,vvObject);
		
		bFirst=FALSE;
	}
	TRACE("CStringVector TranslateObject(Object object) ENDE\n");
	return vv;
}*/

/*std::vector<CStringVector> TranslateObject2(Object object,BYTE Case)
{
	TRACE("CStringVector TranslateObject(Object object,BYTE Case)) ANFANG\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateObject2(Object object,BYTE Case) ANFANG\n");
	#endif
	std::vector<CStringVector> vv;
	BOOL bFirst=TRUE;
	for(int i=0;i<object.m_vecEnumerationElement.size();i++)
	{
		if(!bFirst)
		{
			CStringVector sv;
			if(i!=object.m_vecEnumerationElement.size()-1) // wenn nicht das letzte Element
				sv.push_back(",");
			else
				sv.push_back("und");
			vv.push_back(sv);
		}
		std::vector<CStringVector> vvObject=TranslateObjectEnumerationElement2(
			object.m_vecEnumerationElement.at(i),Case,nStartID);
		Append(vv,vvObject);
		
		bFirst=FALSE;
	}
	TRACE("CStringVector TranslateObject(Object object) ENDE\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateObject2(Object object,BYTE Case) ENDE\n");
	#endif
	return vv;
}*/

//wird von TranslateClause aufgerufen
//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
// m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
// zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
// (Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
// ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
// Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
// auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
// (Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
// Ein Objekt kann Subjekt und Prädikat haben, wenn es ein Gerundium 
// beinhaltet: "I like you hurting me." -> "Ich mag, wenn du|
// ihr|Sie|man mich verletzst|verletzt|verletzen|verletzt."
//nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
// die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
// bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
// Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
// sich das "folgst" automatisch in "folgt" ändern
// Es kann folgenden Satz geben: "I like hurting you." "hurting you": 
// Gerundium -> "Ich mag |dich| zu verletzen."
//                      |Sie |
//                      |euch|
std::vector<CStringVector> TranslateObject2(Object object,BYTE Case,
	BOOL bNegation,BYTE & bFirstTokenInSentence,//EnglishView * pEV,
  std::vector<IntPair> & rvecintpair,int & nStartID)
{
	TRACE("CStringVector TranslateObject(Object object,BYTE Case,BOOL bNegation)) ANFANG\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateObject2(Object object,BYTE \
Case,BOOL bNegation,BYTE & bFirstTokenInSentence) ANFANG\n");
	printf("Case: %u\n",Case);
	fflush(stdout);
	#endif
	std::vector<CStringVector> vv;
	BOOL bFirst=TRUE;
	for(DWORD i=0;i<object.m_vecEnumerationElement.size();i++)
	{
		if(!bFirst)
		{
			CStringVector sv;
			if(i!=object.m_vecEnumerationElement.size()-1) // wenn nicht das letzte Element
				sv.push_back(",");
			else
				sv.push_back("und");
			vv.push_back(sv);
		}
		std::vector<CStringVector> vvObject=TranslateObjectEnumerationElement2(
			object.m_vecEnumerationElement.at(i),Case,bNegation,
			bFirstTokenInSentence,//pEV,
      rvecintpair,nStartID);
		Append(vv,vvObject);
		
		bFirst=FALSE;
	}
	TRACE("CStringVector TranslateObject(Object object) ENDE\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateObject2(Object object,BYTE Case,BOOL bNegation) ENDE\n");
	#endif
	return vv;
}

CStringVector	TranslateObjectEnumerationElement(EnumerationElement ee,BYTE bCase)
{
	CStringVector sv;
	BOOL bIndefiniteArticle=FALSE;
	BOOL bDefiniteArticle=FALSE;
	if(ee.m_bArticle)
	{
		TRACE("ee.m_bArticle\n");
		GermanNoun * gn=GetGermanWordOfEnglishNoun(ee.m_Noun);
		for(DWORD i=0;i<ee.m_Noun.size();i++)
			TRACE("ee.m_Noun.at(i): %s",ee.m_Noun.at(i));
		if(IsEnglishSingular(ee.m_Noun))
		{
			if(gn->m_bArticle=='1') // der
				sv.push_back("ein");
			if(gn->m_bArticle=='2') // die
				sv.push_back("eine");
			if(gn->m_bArticle=='3') // das
				sv.push_back("ein");
		}
	}
	if(ee.m_possessivePronoun!=0)
	{
		GermanNoun * gn=GetGermanWordOfEnglishNoun(ee.m_Noun);
		CString str=TranslatePossessivePronoun(ee.m_possessivePronoun);
		if(IsEnglishSingular(ee.m_Noun))
		{
			if(gn->m_bArticle=='1') // der
				str+="en";
			if(gn->m_bArticle=='2') // die
				str+="e";
		}
		else
				str+="e";
		sv.push_back(str);
	}
	for(DWORD i=0;i<ee.m_Adjectives.size();i++)
	{
		GermanAdjective * ga=GetGermanWordOfEnglishAdjective(ee.m_Adjectives.at(i));
		if(ee.m_Noun.size()>0)
		{
			TRACE("if(ee.m_Noun.size()>0)\n");
			TRACE("%s\n",ee.m_Noun.at(0));
			GermanNoun * gn=GetGermanWordOfEnglishNoun(ee.m_Noun);
/*		if(gn->article=='1')
			sv.push_back("der");
		if(gn->article=='2')
			sv.push_back("die");
		if(gn->article=='3')
			sv.push_back("das");*/
			if(IsEnglishSingular(ee.m_Noun))
			{
				if(gn->m_bArticle=='1') // der
					sv.push_back(ga->m_strWortstamm+"er");
				if(gn->m_bArticle=='2') // die
					sv.push_back(ga->m_strWortstamm+"e");
				if(gn->m_bArticle=='3') // das
					sv.push_back(ga->m_strWortstamm+"es");
			}
			else
				sv.push_back(ga->m_strWortstamm+"e");
		}
		else
			sv.push_back(ga->m_strPositiv);
	}
	if(ee.m_personalPronoun)
	{
		if(ee.m_Noun.at(0)=="I")
			sv.push_back("ich");
		if(ee.m_Noun.at(0)=="you")
		{
			if(bCase==0)
				sv.push_back("du");
			if(bCase==1)
				sv.push_back("dich");
			if(bCase==2)
				sv.push_back("dir");
		}
		if(ee.m_Noun.at(0)=="he")
			sv.push_back("er");
		if(ee.m_Noun.at(0)=="she")
			sv.push_back("sie");
		if(ee.m_Noun.at(0)=="it")
			sv.push_back("es");
		if(ee.m_Noun.at(0)=="we")
			sv.push_back("wir");
		if(ee.m_Noun.at(0)=="they")
			sv.push_back("sie");
	}
	else
	{
		GermanNoun * gn=GetGermanWordOfEnglishNoun(ee.m_Noun);
		if(IsEnglishSingular(ee.m_Noun))
			sv.push_back(gn->m_strSingular);
		else
			sv.push_back(gn->m_strPlural);
	}
	TRACE("CStringVector TranslateObjectEnumerationElement(EnumerationElement ee) ENDE\n");
	return sv;
}

/*std::vector<CStringVector> TranslateObjectEnumerationElement2(
	const EnumerationElement & ee,BYTE Case,int & nStartID)
{
	TRACE("std::vector<CStringVector> TranslateObjectEnumerationElement2(EnumerationElement ee,BYTE bCase) ANFANG\n");
	//CStringVector sv;
	std::vector<CStringVector> vv;
	if(ee.m_personalPronoun)
	{
		#ifdef _DEBUG
		printf("ee.m_personalPronoun: %u\n",ee.m_personalPronoun);
		#endif
		if(Case==3)
		{
			CStringVector sv;
			switch(ee.m_personalPronoun)
			{
				case 1:
					sv.push_back("mir");
					break;
				case 2:
					sv.push_back("dir");
					sv.push_back("euch");
					sv.push_back("Ihnen");
					break;
				case 3:
					sv.push_back("ihm");
					break;
				case 4:
					sv.push_back("ihr");
					break;
				case 5:
					sv.push_back("es");
					break;
				case 6:
					sv.push_back("uns");
					break;
				case 7:
					sv.push_back("ihnen");
					break;
			}
			vv.push_back(sv);
		}
		if(Case==4)
		{
			CStringVector sv;
			switch(ee.m_personalPronoun)
			{
				case 1:
					sv.push_back("mich");
					break;
				case 2:
					sv.push_back("dich");
					sv.push_back("euch");
					sv.push_back("Sie");
					break;
				case 3:
					sv.push_back("ihn");
					break;
				case 4:
					sv.push_back("sie");
					break;
				case 5:
					sv.push_back("es");
					break;
				case 6:
					sv.push_back("uns");
					break;
				case 7:
					sv.push_back("euch");
					break;
			}
			vv.push_back(sv);
		}
		//sv.push_back(GetGermanPersonalPronoun(CString & str));
	}
	if(ee.m_pDan)
	{
		#ifdef _DEBUG
		printf("ee.m_pDan\n");
		{
			for(DWORD dwRun=0;dwRun<ee.m_pDan->m_Noun.size();dwRun++)
			{
				printf("ee.m_pDan->m_Noun.at(%u): %s\n",dwRun,ee.m_pDan->
					m_Noun.at(dwRun));
			}
		}
		#endif
		CStringVector sv,sv2;
		std::vector <GermanNoun> vecGermanNoun=GetGermanWordOfEnglishNoun2(
			ee.m_pDan->m_Noun);
		for(DWORD dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
		{
			TRACE("vecGermanNoun\n");
			#ifdef _DEBUG
			printf("vecGermanNoun\n");
			#endif
			GermanNoun gn=vecGermanNoun.at(dwRun);
			sv.push_back(gn.m_strSingular);
			if(gn.m_bArticle=='1')
				sv2.push_back(_T("der"));
			else if(gn.m_bArticle=='2')
				sv2.push_back(_T("die"));
			else if(gn.m_bArticle=='3')
				sv2.push_back(_T("das"));
			else 
				sv2.push_back(_T("das"));
			//sv.push_back(gn.plural);
		}
		vv.push_back(sv2);
		vv.push_back(sv);
		//vv.push_back(
		//Append(vv,
	}
	if(ee.m_pEnr)
	{
		#ifdef _DEBUG
		printf("ee.enr\n");
		#endif
		TRACE("ee.enr\n");
		Append(vv,TranslateENR(ee.m_pEnr,Case));
	}
	if(ee.m_pOfPhrase)
	{
		#ifdef _DEBUG
		printf("ee.m_pOfPhrase\n");
		#endif
		Append(vv,TranslateOfPhrase(ee.m_pOfPhrase->m_Enr,nStartID));
	}
	if(ee.m_pGerund)
	{
		#ifdef _DEBUG
		printf("ee.m_pGerund\n");
		#endif
		BYTE b=0;
		Append(vv,TranslateGerund(*ee.m_pGerund,b,pEV,nStartID));
	}
	TRACE("CStringVector TranslateObjectEnumerationElement(EnumerationElement ee) ENDE\n");
	return vv;
}*/

//wird von std::vector<CStringVector> TranslateObject2(Object object,BYTE 
//Case,BOOL bNegation,BYTE & bFirstTokenInSentence) aufgerufen
std::vector<CStringVector> TranslateObjectEnumerationElement2(const 
	EnumerationElement & ee,BYTE Case,BOOL bNegation,BYTE & 
	bFirstTokenInSentence,//EnglishView * pEV,
  std::vector<IntPair> & rvecintpair,int & nStartID)
{
	//CStringVector sv;
	BYTE bArticle=0;
	std::vector<CStringVector> vv;
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateObjectEnumerationElement2(const \
EnumerationElement & ee,BYTE Case,BOOL bNegation,BYTE & \
bFirstTokenInSentence\n");
#endif
	if(ee.m_vecStrPlainText.size()>0)
	{
		for(DWORD dwRun=0;dwRun<ee.m_vecStrPlainText.size();dwRun++)
		{
			CStringVector sv;
			sv.push_back(ee.m_vecStrPlainText.at(dwRun));
			vv.push_back(sv);
		}
	}

	if(ee.m_personalPronoun)
	{
#ifdef _DEBUG
		printf("ee.m_personalPronoun: %u\n",ee.m_personalPronoun);
#endif
		if(Case==0) // kein Fall Beispiel: I am you -> Ich bin du.
		{
			CStringVector sv;
			switch(ee.m_personalPronoun)
			{
				case 1:
					sv.push_back(_T("ich"));
					break;
				case 2:
					sv.push_back(_T("du"));
					sv.push_back(_T("ihr"));
					sv.push_back(_T("Sie"));
					break;
				case 3:
					sv.push_back(_T("er"));
					break;
				case 4:
					sv.push_back(_T("sie"));
					break;
				case 5:
					sv.push_back(_T("es"));
					break;
				case 6:
					sv.push_back(_T("wir"));
					break;
				case 7:
					sv.push_back(_T("sie"));
					break;
			}
			if(bFirstTokenInSentence)
			{
				for(BYTE bRun=0;bRun<sv.size();bRun++)
					MakeFirstLetterUpper(sv.at(bRun));
				bFirstTokenInSentence=FALSE;
			}
			vv.push_back(sv);
		}
		//wenn das Verb den 3. Fall verlangt
		//Beispiel: "I trust you." -> "Ich vertraue dir."
		if(Case==1) // 3. Fall
		{
			CStringVector sv;
			switch(ee.m_personalPronoun)
			{
				case 1:
					sv.push_back(_T("mir"));
					break;
				case 2:
					sv.push_back(_T("dir"));
					sv.push_back(_T("euch"));
					sv.push_back(_T("Ihnen"));
					break;
				case 3:
					sv.push_back(_T("ihm"));
					break;
				case 4:
					sv.push_back(_T("ihr"));
					break;
				case 5:
					sv.push_back(_T("es"));
					break;
				case 6:
					sv.push_back(_T("uns"));
					break;
				case 7:
					sv.push_back(_T("ihnen"));
					break;
			}
			if(bFirstTokenInSentence)
			{
				for(BYTE bRun=0;bRun<sv.size();bRun++)
					MakeFirstLetterUpper(sv.at(bRun));
				bFirstTokenInSentence=FALSE;
			}
			vv.push_back(sv);
		}
		if(Case==2) // 4. Fall
		{
			CStringVector sv;
			switch(ee.m_personalPronoun)
			{
				case 1:
					sv.push_back(_T("mich"));
					break;
				case 2:
					sv.push_back(_T("dich"));
					sv.push_back(_T("euch"));
					sv.push_back(_T("Sie"));
					break;
				case 3:
					sv.push_back(_T("ihn"));
					break;
				case 4:
					sv.push_back(_T("sie"));
					break;
				case 5:
					sv.push_back(_T("es"));
					break;
				case 6:
					sv.push_back(_T("uns"));
					break;
				case 7:
					sv.push_back(_T("sie"));
					break;
			}
			if(bFirstTokenInSentence)
			{
				for(BYTE bRun=0;bRun<sv.size();bRun++)
					MakeFirstLetterUpper(sv.at(bRun));
				bFirstTokenInSentence=FALSE;
			}
			vv.push_back(sv);
		}
		//sv.push_back(GetGermanPersonalPronoun(CString & str));
	}
	//wenn das Aufzählungselement des Objekts ein Nomen mit bestimmtem Artikel ist
	//Beispiele: "Liking th car is great." -> "Das Auto zu mögen ist grossartig."
	// "I like the car." -> "Ich mag das Auto."
	if(ee.m_pDan)
	{
		#ifdef _DEBUG
		printf("ee.dan\n");
		TRACE("ee.dan\n");
		{
			for(DWORD dwRun=0;dwRun<ee.m_pDan->m_Noun.size();dwRun++)
			{
				printf("ee.m_pDan->m_Noun.at(%u): %s\n",dwRun,ee.m_pDan->m_Noun.at(dwRun));
			}
		}
		#endif
		CStringVector sv,sv2;
		std::vector <GermanNoun> vecGermanNoun=GetGermanWordOfEnglishNoun2(
			ee.m_pDan->m_Noun);
		for(DWORD dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
		{
			TRACE("vecGermanNoun\n");
			#ifdef _DEBUG
			printf("vecGermanNoun\n");
			#endif
			GermanNoun gn=vecGermanNoun.at(dwRun);
			sv.push_back(gn.m_strSingular);
			if(gn.m_bArticle=='1')
				sv2.push_back(_T("der"));
			else if(gn.m_bArticle=='2')
				sv2.push_back(_T("die"));
			else if(gn.m_bArticle=='3')
				sv2.push_back(_T("das"));
			else 
				sv2.push_back(_T("das"));
			if(bFirstTokenInSentence)
			{
				MakeFirstLetterUpper(sv2.at(0));
				bFirstTokenInSentence=FALSE;
			}
			//sv.push_back(gn.plural);
		}
		vv.push_back(sv2);
		vv.push_back(sv);
		//vv.push_back(
		//Append(vv,
	}
	if(ee.m_pEnr)
	{
		BYTE bPersonIndex=0;
		#ifdef _DEBUG
		printf("ee.enr\n");
		#endif
		TRACE("ee.enr\n");
		//Append(vv,TranslateENR(ee.enr,Case,bNegation,FALSE,bPersonIndex,
		//	bFirstTokenInSentence));
		Append(vv,TranslateENR(ee.m_pEnr,Case,//bNegation,FALSE,
      (bool)bNegation,false,bPersonIndex,
			bFirstTokenInSentence,bArticle,nStartID));
	}
	if(ee.m_pOfPhrase)
	{
		#ifdef _DEBUG
		printf("ee.m_OfPhrase\n");
		#endif
		Append(vv,TranslateOfPhrase(ee.m_pOfPhrase->m_Enr,nStartID));
	}
	if(ee.m_pGerund)
	{
		#ifdef _DEBUG
		printf("ee.m_pGerund\n");
		#endif
		BYTE b=0;
		Append(vv,TranslateGerund(*ee.m_pGerund,b,//pEV,
      rvecintpair,nStartID));
	}
	if(ee.m_pRelativeClause)
	{
#ifdef _DEBUG
		printf("ee.m_pRelativeClause\n");
#endif
		CStringVector sv;
		sv.push_back(_T(","));
		vv.push_back(sv);
		Append(vv,TranslateRelativeClause(*ee.m_pRelativeClause,bArticle,//pEV,
      rvecintpair,
			nStartID));
		//vv.push_back(sv);
	}
	TRACE("CStringVector TranslateObjectEnumerationElement2(EnumerationElement ee,BYTE Case,BOOL bNegation) ENDE\n");
	return vv;
}

//wird von TranslateEnumerationElement() aufgerufen
//nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
//die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
//bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
//Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
//sich das "folgst" automatisch in "folgt" ändern
std::vector<CStringVector> TranslateOfPhrase(EnglishNounRecord enr,int & 
	nStartID)//OfPhrase * op)
{
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateOfPhrase(EnglishNounRecord enr)\n");
	#endif
	std::vector<CStringVector> vv;
	CStringVector sv;
	sv.push_back(_T("von"));
	vv.push_back(sv);
	GermanNoun * gn=NULL;
	if(enr.m_possessivePronoun)
	{
		#ifdef _DEBUG
		printf("enr.m_possessivePronoun: %u\n",enr.m_possessivePronoun);
		#endif
		gn=GetGermanWordOfEnglishNoun(enr.m_Noun);
		CStringVector strVec=TranslatePossessivePronoun2(enr.m_possessivePronoun);
		for(DWORD i=0;i<strVec.size();i++)
		{
			if(IsEnglishSingular(enr.m_Noun))
			{
				if(gn->m_bArticle=='1' || gn->m_bArticle=='3') // Artikel: 'der' oder 'das'
					strVec.at(i)+="em";
				if(gn->m_bArticle=='2') // die
					strVec.at(i)+="er";
			}
			else // Substantiv ist Plural
				strVec.at(i)+="en";
		}
		vv.push_back(strVec);
	}
	if(enr.bDefiniteArticle)
	{
		#ifdef _DEBUG
		printf("enr.bDefiniteArticle\n");
		#endif
		CStringVector sv;
		gn=GetGermanWordOfEnglishNoun(enr.m_Noun);
		if(IsEnglishSingular(enr.m_Noun))
		{
			if(gn->m_bArticle=='0' || gn->m_bArticle=='1') // die
				sv.push_back("dem");
			if(gn->m_bArticle=='2') // die
				sv.push_back("der");
		}
		else // Substantiv ist Plural
			sv.push_back("den");
		vv.push_back(sv);
	}
	if(enr.bIndefiniteArticle)
	{
		#ifdef _DEBUG
		printf("enr.bIndefiniteArticle\n");
		#endif
		CStringVector sv;
		gn=GetGermanWordOfEnglishNoun(enr.m_Noun);
		if(IsEnglishSingular(enr.m_Noun))
		{
			if(gn->m_bArticle=='0' || gn->m_bArticle=='1') // die
				sv.push_back("einem");
			if(gn->m_bArticle=='2') // die
				sv.push_back("einer");
		}
		else // Substantiv ist Plural
			sv.push_back("den");
		vv.push_back(sv);
	}
	if(gn)
	{
		#ifdef _DEBUG
		printf("gn\n");
		#endif
		CStringVector sv;
		std::vector <GermanNoun> vecGermanNoun=GetGermanWordOfEnglishNoun2(
			enr.m_Noun);
		for(DWORD dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
		{
			TRACE("vecGermanNoun\n");
			GermanNoun gn=vecGermanNoun.at(dwRun);
			if(IsEnglishPlural(enr.m_Noun))
			{
				sv.push_back(gn.m_strSingular+"n");
				#ifdef _DEBUG
				printf("gn.m_strSingular+\"n\": %s\n",gn.m_strSingular+"n");
				#endif
			}
			else
			{
				sv.push_back(gn.m_strSingular);
				#ifdef _DEBUG
				printf("gn.m_strSingular: %s\n",gn.m_strSingular);
				#endif
			}
			//sv.push_back(gn.plural);
		}
		vv.push_back(sv);
	}
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateOfPhrase(EnglishNounRecord enr) ENDE\n");
	#endif
	return vv;
}

//bFirstTokenInSentence muss eine Referenz (&) enthalten, damit sich
//eine Änderung der Variablen in dieser Funktion auf den Aufrufer überträgt
std::vector<CStringVector> TranslatePlace(Place & place,BYTE & 
	bFirstTokenInSentence,int & nStartID)
{
  //BYTE byCase;
	std::vector<CStringVector> vv;
	CStringVector sv;
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslatePlace(Place & place) ANFANG\n");
	#endif
	//sv.push_back(CString("in"));
	//wenn die Umstandsbestimmung des Ortes eine Präposition enthält
	if(place.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated)
		if(place.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated->
			m_pWordNodeNext)
		{
			GermanPreposition * gp=dynamic_cast<GermanPreposition *>(place.
				m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated->
				m_pWordNodeNext->m_pWord);
      if(gp)
      {
			  sv.push_back(gp->m_strWord);
			  if(bFirstTokenInSentence)
			  {
				  MakeFirstLetterUpper(sv.at(0));
				  bFirstTokenInSentence=FALSE;
			  }
			  vv.push_back(sv);
	      for(DWORD dwRun=0;dwRun<place.m_vecEnumerationElement.size();dwRun++)
	      {
		      //Append(vv,TranslateObjectEnumerationElement2(place.m_enumerationElements.at(dwRun),1));
		      if(dwRun>0)
		      {
			      //CStringVector sv;
            sv.clear();
			      if(dwRun+1==place.m_vecEnumerationElement.size())
				      sv.push_back(_T("und"));
			      else
				      sv.push_back(_T(","));
				      vv.push_back(sv);
		      }
		      Append(vv,TranslatePlaceEnumerationElement(place.
			      m_vecEnumerationElement.at(dwRun),nStartID,//byCase
            gp->m_byCase));
	      }//for
      }//if(gp)
		}
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslatePlace(Place & place) ENDE\n");
	#endif
	return vv;
}

std::vector<CStringVector> TranslatePlaceENR(EnglishNounRecord * enr)
{
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslatePlaceENR(EnglishNounRecord * enr)\n");
	printf("enr->m_Noun.size(): %u\n",enr->m_Noun.size());
	{
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			printf("enr->m_Noun.at(i): %s\n",enr->m_Noun.at(i));
	}
	#endif
	std::vector<CStringVector> vv;
	//TRACE("ee.words.size(): %d\n",ee.words.size());
	BOOL bIndefiniteArticle=FALSE;
	BOOL bDefiniteArticle=FALSE;
	if(enr->bDefiniteArticle) // bestimmter Artikel
	{
		//TRACE("ee.bArticle\n");
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		for(DWORD i=0;i<enr->m_Noun.size();i++)
			TRACE("enr->m_Noun.at(i): %s",enr->m_Noun.at(i));
		if(IsEnglishSingular(enr->m_Noun))
		{
			CStringVector sv;
			if(gn->m_bArticle=='1')
				sv.push_back("der");
			if(gn->m_bArticle=='2')
				sv.push_back("die");
			if(gn->m_bArticle=='3')
				sv.push_back("das");
			vv.push_back(sv);
		}
		else // Mehrzahl
		{
			CStringVector sv;
			sv.push_back("die");
			vv.push_back(sv);
		}
	}
	// m_possessivePronoun;
	if(enr->m_possessivePronoun>0) // besitzanzeigendes Fürwort
	{
		#ifdef _DEBUG
		printf("enr->m_possessivePronoun: %u\n",enr->m_possessivePronoun);
		for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
		{
			TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
		}
		#endif
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		CStringVector strVec=TranslatePossessivePronoun2(enr->
			m_possessivePronoun);
		for(DWORD i=0;i<strVec.size();i++)
		{
			if(IsEnglishSingular(enr->m_Noun))
			{
				if(gn->m_bArticle=='2') // die
					strVec.at(i)+=_T("er");
				else
					strVec.at(i)+=_T("em");
			}
			else // Substantiv ist Plural
				strVec.at(i)+=_T("en");
		}
		vv.push_back(strVec);
	}
	for(DWORD i=0;i<enr->m_Adjectives.size();i++) // Adjektive
	{
		GermanNoun * gn=GetGermanWordOfEnglishNoun(enr->m_Noun);
		/*if(!ee.bArticle)
		{
			CStringVector sv;
			if(gn->article=='1')
				sv.push_back("der");
			if(gn->article=='2')
				sv.push_back("die");
			if(gn->article=='3')
				sv.push_back("das");
			vv.push_back(sv);
		}*/
		GermanAdjective * ga=GetGermanWordOfEnglishAdjective(enr->m_Adjectives.at(i));
		CStringVector sv;
		if(IsEnglishSingular(enr->m_Noun))
			sv.push_back(ga->m_strWortstamm+"e");
		else
			sv.push_back(ga->m_strWortstamm+"en");
		vv.push_back(sv);
	}
	if(enr->m_personalPronoun)
	{
		CStringVector sv;
		if(enr->m_Noun.at(0)=="I")
			sv.push_back("ich");
		if(enr->m_Noun.at(0)=="you")
		{
			//Option o;
			sv.push_back("du");
			sv.push_back("ihr");
			sv.push_back("sie");
			//sv.index=0;
			//vv.push_back(sv);
		}
		if(enr->m_Noun.at(0)=="he")
			sv.push_back("er");
		if(enr->m_Noun.at(0)=="she")
			sv.push_back("sie");
		if(enr->m_Noun.at(0)=="it")
			sv.push_back("es");
		if(enr->m_Noun.at(0)=="we")
			sv.push_back("wir");
		if(enr->m_Noun.at(0)=="they")
			sv.push_back("sie");
		vv.push_back(sv);
	}
	else // sonst: Substantiv
	{
		CStringVector sv;
		for(DWORD dwRun=0;dwRun<enr->m_Noun.size();dwRun++)
		{
			TRACE("enr->m_Noun.at(dwRun): %s\n",enr->m_Noun.at(dwRun));
		}
		std::vector <GermanNoun> vecGermanNoun=GetGermanWordOfEnglishNoun2(
			enr->m_Noun);
		for(dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
		{
			TRACE("vecGermanNoun\n");
			GermanNoun gn=vecGermanNoun.at(dwRun);
			sv.push_back(gn.m_strSingular);
			//sv.push_back(gn.plural);
		}
			/*if(IsEnglishSingular(ee.m_Noun))
				sv.push_back(gn->singular);
			else
				sv.push_back(gn->plural);*/
		vv.push_back(sv);
	}
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateENR(EnglishNounRecord * enr) ENDE\n");
	#endif
	return vv;
}

std::vector<CStringVector> TranslatePlaceEnumerationElement(const 
	EnumerationElement & ee,int & nStartID,BYTE byCase)
{
	TRACE("std::vector<CStringVector> TranslatePlaceEnumerationElement(const EnumerationElement & ee) ANFANG\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslatePlaceEnumerationElement(const EnumerationElement & ee) ANFANG\n");
	#endif
	//CStringVector sv;
	std::vector<CStringVector> vv;
	if(ee.m_vecStrPlainText.size()>0)
	{
		for(DWORD dwRun=0;dwRun<ee.m_vecStrPlainText.size();dwRun++)
		{
			CStringVector sv;
			sv.push_back(ee.m_vecStrPlainText.at(dwRun));
			vv.push_back(sv);
		}
	}
	if(ee.m_personalPronoun)
	{
		#ifdef _DEBUG
		printf("ee.m_personalPronoun: %u\n",ee.m_personalPronoun);
		#endif
		CStringVector sv;
    //wenn der Fall für eine Präpos. der 4. Fall ist, müsste es 
    //z.B. "ohne dich" heißen.
    //"I stand below you."->"Ich stehe unter dir."
		switch(ee.m_personalPronoun)
		{
			case 1:
				sv.push_back("mir");
				break;
			case 2:
				sv.push_back("dir");
				sv.push_back("euch");
				sv.push_back("Ihnen");
				break;
			case 3:
				sv.push_back("ihm");
				break;
			case 4:
				sv.push_back("ihr");
				break;
			case 5:
				sv.push_back("es");
				break;
			case 6:
				sv.push_back("uns");
				break;
			case 7:
				sv.push_back("ihnen");
				break;
		}
		vv.push_back(sv);
		//sv.push_back(GetGermanPersonalPronoun(CString & str));
	}
	if(ee.m_pDan)
	{
		#ifdef _DEBUG
		printf("ee.dan\n");
		{
			for(DWORD dwRun=0;dwRun<ee.m_pDan->m_Noun.size();dwRun++)
			{
				printf("ee.m_pDan->m_Noun.at(%u): %s\n",dwRun,ee.m_pDan->m_Noun.at(dwRun));
			}
		}
		#endif
		CStringVector sv,sv2;
		std::vector <GermanNoun> vecGermanNoun=GetGermanWordOfEnglishNoun2(
			ee.m_pDan->m_Noun);
		for(DWORD dwRun=0;dwRun<vecGermanNoun.size();dwRun++)
		{
			TRACE("vecGermanNoun\n");
			#ifdef _DEBUG
			printf("vecGermanNoun\n");
			#endif
			GermanNoun gn=vecGermanNoun.at(dwRun);
			sv.push_back(gn.m_strSingular);
			if(gn.m_bArticle=='1')
				sv2.push_back(_T("der"));
			else if(gn.m_bArticle=='2')
				sv2.push_back(_T("die"));
			else if(gn.m_bArticle=='3')
				sv2.push_back(_T("das"));
			else 
				sv2.push_back(_T("das"));
			//sv.push_back(gn.plural);
		}
		vv.push_back(sv2);
		vv.push_back(sv);
		//vv.push_back(
		//Append(vv,
	}
	if(ee.m_pEnr)
	{
    BYTE bGermanNounGender;
    BYTE bFirstTokenInSentence=0;
		BYTE bPersonIndex=0;
		#ifdef _DEBUG
		printf("ee.m_pEnr\n");
		#endif
    //Dieselbe Präposition kann in verschiedenen Fällen übersetzt werden:
    //"vor {prp; wo? +Dativ; wohin? +Akkusativ}  in front of; ahead of   
    //vor dEM Haus stehen <> to stand in front of the house 
    //vor dAS Haus gehen  <> to go in front of the house.
    //Wobei nur die Übersetzung mit "dem" eine Umstandsbestimmung des Ortes ist.

		Append(vv,TranslateENR(ee.m_pEnr,byCase,//FALSE,FALSE,
      false,false,bPersonIndex,
			bFirstTokenInSentence,bGermanNounGender,nStartID)); // übersetzen im 3. Fall 
		// (I am in the kitchen.-> Ich bin in deR Küche.)
		#ifdef _DEBUG
		for(DWORD dwRun=0;dwRun<vv.size();dwRun++)
		{
			printf("vv.at(%u).at(0): %s\n",dwRun,vv.at(dwRun).at(0));
		}
		printf("ee.m_pEnr\n");
		#endif
	}
	if(ee.m_pOfPhrase)
	{
		#ifdef _DEBUG
		printf("ee.m_pOfPhrase\n");
		#endif
		Append(vv,TranslateOfPhrase(ee.m_pOfPhrase->m_Enr,nStartID));
	}
	TRACE("CStringVector TranslateObjectEnumerationElement(EnumerationElement ee) ENDE\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslatePlaceEnumerationElement(const EnumerationElement & ee) ENDE\n");
	#endif
	return vv;
}

std::vector<CStringVector> TranslateQuestion(Question & question,//EnglishView 
	//* pEV,
  std::vector<IntPair> & rvecintpair,int & nStartID)
{
	BYTE bFirstTokenInSentence=true;
	std::vector<CStringVector> vv;
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateQuestion(const Question & question) ANFANG\n");
	#endif
  //TODO This sanity check is unnecessary if the size can be ensured at another place.
  if(question.m_vecClause.size()>0)
  {
    Append(vv,TranslateQuestionClause(question.m_vecClause.at(0),//pEV,
      rvecintpair,nStartID));
    ////Der Satzbau eines deutschen Fragesatzes ist PSO: Wo schlägt(P) er(S) ihn(O)?
    ////Bis auf das Fragewort kann man also die Funktion zum Übersetzen eines 
    ////Haupt- oder Nebensatzes nehmen.
    //question.m_vecClause.at(0).m_bWordOrder=PSO;
		//Append(vv,TranslateClause2(question.m_vecClause.at(0),bFirstTokenInSentence,pEV,nStartID));
    //Append(vv,TranslatePSO(question.m_vecClause.at(0),bFirstTokenInSentence,pEV,nStartID));
  }
	typedef std::vector<Clause>::const_iterator It;
	It end = question.m_vecClause.end();
	for(It run = 
    //Start with the 2nd clause because the 1st one has been translated above.
    question.m_vecClause.begin()+1 ; run != end ; ++run)
	{
		Append(vv,TranslateClause2(*run,bFirstTokenInSentence,//pEV,
      rvecintpair,nStartID));
		if(bFirstTokenInSentence)
			bFirstTokenInSentence=false;
	}
	//for(question.m_vecClause()
	//if(question.m_vecClause.size()>0)
	//{
	//}
	CStringVector sv;
	sv.push_back("?");
	vv.push_back(sv);
	for(DWORD dwRun=0;dwRun<vv.at(0).size();dwRun++)
	{ // die ersten Buchstaben von allen CStrings, die der CStringVector 
		// an Position dwRun des Vektors vv hat großmachen
		MakeFirstLetterUpper(vv.at(0).at(dwRun));
	}
	#ifdef _DEBUG
	dwRun=0;
	for(;dwRun<vv.size();dwRun++)
	{
		//die einzelnen Token der deutschen Übersetzung ausgeben
		printf("vv.at(%u).at(0): %s\n",dwRun,vv.at(dwRun).at(0));
	}
	printf("std::vector<CStringVector> TranslateQuestion(const Question & \
question) ENDE - return vv\n");
	#endif
	return vv;
}

//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
// m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
// zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
// (Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
// ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
// Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
// auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
// (Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
// pEV wird zum Beispiel an TranslateSubject weitergeleitet
//nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
// die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
// bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
// Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
// sich das "folgst" automatisch in "folgt" ändern
// Es kann folgenden Satz geben: "I like hurting you." "hurting you": 
// Gerundium -> "Ich mag |dich| zu verletzen."
//                      |Sie |
//                      |euch|
std::vector<CStringVector> TranslateQuestionClause(Clause & subSentence,
	//EnglishView * pEV,
  std::vector<IntPair> & rvecintpair,int & nStartID)
{
	TRACE("CStringVector TranslateClause ANFANG\n");
	BYTE bFirstTokenInSentence=0;
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateQuestionClause(Clause & subSentence) - ANFANG\n");
	if(subSentence.m_bNegation)
		TRACE("subSentence.m_bNegation\n");
	else
		TRACE("!subSentence.m_bNegation\n");
	#endif
	std::vector<CStringVector> vv;
	#ifdef _DEBUG
	printf("subSentence.m_bWordOrder: %u\n",subSentence.m_bWordOrder);
	printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
	if(subSentence.m_bActiveClause)
		printf("subSentence.m_bActiveClause\n");
	else
		printf("!subSentence.m_bActiveClause\n");
	#endif
	//BYTE satzbau=SPO;
	GermanVerb * gv=NULL;
	GermanVerb * gvInfinitive=NULL;
	if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated)
	{
		#ifdef _DEBUG
		printf("subSentence.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated\n");
		#endif
		if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
			m_pWordNodeNext)
		{
			gv=dynamic_cast<GermanVerb *>(subSentence.
				m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
				m_pWordNodeNext->m_pWord);
		}
		if(subSentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated)
			if(subSentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
				m_pWordNodeNext)
			{
				gvInfinitive=dynamic_cast<GermanVerb *>(subSentence.
					m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
					m_pWordNodeNext->m_pWord);
			}
	}
	else
	{
		#ifdef _DEBUG
		printf("!subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
		#endif
		if(subSentence.m_bTense==FUTURE || !subSentence.m_bActiveClause)
		{
			if(subSentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated)
				if(subSentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
					m_pWordNodeNext)
				{
					gvInfinitive=dynamic_cast<GermanVerb *>(subSentence.
						m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
						m_pWordNodeNext->m_pWord);
				}
		}
		else
		{
			if(subSentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated)
				if(subSentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
					m_pWordNodeNext)
				{
					#ifdef _DEBUG
					printf("subSentence.m_pWordNodeWhereAnEnglishVerbIsEncapsula\
ted->m_pWordNodeNext\n");
					#endif
					if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.
						m_bTense==SIMPLE_PAST)
						gv=dynamic_cast<GermanVerb *>(subSentence.
						m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
						m_pWordNodeNext->m_pWord);
					else
						gvInfinitive=dynamic_cast<GermanVerb *>(subSentence.
						m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
						m_pWordNodeNext->m_pWord);
					#ifdef _DEBUG
					printf("gv=dynamic_cast<GermanVerb *>(subSentence.m_pWordNodeWh\
ereAnEnglishVerbIsEncapsulated->m_pWordNodeNext->m_pWord);\n");
					#endif
				}
			if(subSentence.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated)
				if(subSentence.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated->
					m_pWordNodeNext)
					gvInfinitive=dynamic_cast<GermanVerb *>(subSentence.
					m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated->
					m_pWordNodeNext->m_pWord);
		}
	}
	#ifdef _DEBUG
	if(gv)
	{
		printf("gv->m_strWords[0]: %s\n",gv->m_strWords[0]);
	}
	if(gvInfinitive)
	{
		printf("gvInfinitive->m_strWords[0]: %s\n",gvInfinitive->m_strWords[0]);
	}
	#endif
	if(subSentence.m_bWordOrder==SPO) // Subjekt Prädikat Objekt
	{
		#ifdef _DEBUG
		printf("m_bWordOrder==SPO\n");
		printf("subSentence.m_strPredicate: %s\n",subSentence.m_strPredicate);
		printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
		#endif
		/*if(subSentence.tense==FUTURE) // Zukunft / will-future (z.B. will go)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
			sv.push_back(gv->words[subSentence.m_Subject.m_personIndex]);
			vv.push_back(sv);
		}*/
		if(subSentence.m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated)
			if(subSentence.m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated->
				m_pWordNodeNext)
			{
				#ifdef _DEBUG
				printf("subSentence.m_pWordNodeWhereAnEnglishQuestionWordIsEn\
capsulated->m_pWordNodeNext\n");
				#endif
				GermanAdverb * ga=dynamic_cast<GermanAdverb *>(subSentence.
					m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated->
					m_pWordNodeNext->m_pWord);
				if(ga)
				{
					CStringVector sv;
					if(ga->m_strWord==_T("wo"))
					{
						if(gvInfinitive)
						{
							if(gvInfinitive->m_bMove)
								sv.push_back(_T("wohin"));
							else
								sv.push_back(ga->m_strWord);
						}
						else
						{
							if(gv)
							{
								if(gv->m_bMove)
									sv.push_back(_T("wohin"));
								else
									sv.push_back(ga->m_strWord);
							}
						}
					}
					else
						sv.push_back(ga->m_strWord);
					vv.push_back(sv);
					bFirstTokenInSentence=0;
				}
			}
		if(subSentence.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
			sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
			vv.push_back(sv);
		}
		CStringVector svGermanVerb;
		if(subSentence.m_bTense==SIMPLE_PRESENT || // Do I begin?
			subSentence.m_bTense==PRESENT_PROGRESSIVE || // Am I beginning?
			subSentence.m_bTense==SIMPLE_PAST || // Did I begin?
			subSentence.m_bTense==PAST_PROGRESSIVE || // Was I beginning?
			subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
			subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
			subSentence.m_bTense==PRESENT_PERFECT) // Have I begun?
		{
			#ifdef _DEBUG
			printf("subSentence.m_bTense==SIMPLE_PRESENT || \
subSentence.m_bTense==PRESENT_PROGRESSIVE || \
subSentence.m_bTense==SIMPLE_PAST || \
subSentence.m_bTense==PAST_PROGRESSIVE \
subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE \
subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
subSentence.m_bTense==PRESENT_PERFECT\n");
			printf("gv: %u gvInfinitive: %u\n",gv,gvInfinitive);
			#endif
			//GermanVerb * gv=GetGermanWordOfEnglishVerb(subSentence.predicate);
			if(gv!=NULL || gvInfinitive!=NULL)
			{
				#ifdef _DEBUG
				printf("gv!=NULL || gvInfinitive!=NULL\n");
				//TRACE("gv->words[0]: %s\n",gv->words[0]);
//				int index=0;
	//			if((index=subSentence.m_Subject.m_personIndex*subSentence.tense+1)<14)
		//		{
				if(subSentence.m_pSubject)
				{
					printf("subSentence.m_pSubject\n");
					printf("subSentence.m_pSubject->m_personIndex: %u\n",subSentence.m_pSubject->m_personIndex);
				}
				else
					printf("!subSentence.m_pSubject\n");
				#endif
				if(subSentence.m_pPersonalSubject)
				{
#ifdef _DEBUG
					printf("subSentence.m_pPersonalSubject\n");
#endif
					if(subSentence.m_pPersonalSubject->m_vecEnumerationElement.size()==1)
					{
						CStringVector svVerb;
						if(subSentence.m_pPersonalSubject->m_vecEnumerationElement.at(0).
							m_vecStrPlainText.size()>0)
						{
#ifdef _DEBUG
							printf("subSentence.m_pPersonalSubject->m_vecEnumerationElement.\
at(0).m_vecStrPlainText.size()>0\n");
#endif
							if(subSentence.m_bTense==SIMPLE_PRESENT || 
								subSentence.m_bTense==PRESENT_PROGRESSIVE ||
								subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
								subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								if(subSentence.m_bActiveClause)
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präsens)
									if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
								else
								{
									if(gvInfinitive) // Beispiel: 'can be used'
									{ 
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präsens)
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präteritum)
										}
									}
									else // Beispiel: 'is used'
									{
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präsens)
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präteritum)
										}
									}
								}
							}
							if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
								PAST_PROGRESSIVE)
							{
								if(subSentence.m_bActiveClause) // Beispiel: I used water.
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
								else
								{
									if(gvInfinitive) // Beispiel: I could be used. / 
									{ // I could be being used.
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
									}
									else // Beispiel: I was used. / I was being used.
									{
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
									}
								}
							}
							if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.m_bActiveClause)
							{ // Beispiel: I have been used. -> Ich bin benutzt worden.
								GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
								svVerb.push_back(svGermanVerb.at(0));
								// finite Form von 'sein' anhängen
							}
							// Prädikat (Präteritum)
							vv.push_back(svVerb);
						}
						if(subSentence.m_pPersonalSubject->m_vecEnumerationElement.
							at(0).m_pDan)
						{
#ifdef _DEBUG
							printf("subSentence.m_pPersonalSubject->m_vecEnumerationElement.\
at(0).dan\n");
#endif
							std::vector <EnglishNoun> vecEN=GetEnglishNoun(subSentence.
							m_pPersonalSubject->m_vecEnumerationElement.at(0).m_pDan->m_Noun);
							if(vecEN.size()>0)
							{
								if(IsEnglishSingular(subSentence.m_pPersonalSubject->
									m_vecEnumerationElement.at(0).m_pDan->m_Noun))
								{
									if(vecEN.at(0).m_bTranslationType==0)
									{
										CStringVector svVerb;
										if(subSentence.m_bTense==SIMPLE_PRESENT || 
											subSentence.m_bTense==PRESENT_PROGRESSIVE ||
											subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
											subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pPersonalSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pPersonalSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
										if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
											PAST_PROGRESSIVE)
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==PRESENT_PERFECT)
										{
											CStringVector svVerb;
											 // wenn der Satz ein 
											 // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
										}
										// Prädikat (Präteritum)
										vv.push_back(svVerb);
									}
									if(vecEN.at(0).m_bTranslationType==1)
									{ // im Deutschen: Plural
										CStringVector svVerb;
										if(subSentence.m_bTense==SIMPLE_PRESENT || 
											subSentence.m_bTense==PRESENT_PROGRESSIVE ||
											subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
											subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PAST || subSentence.
											m_bTense==PAST_PROGRESSIVE)
										{
											CStringVector svVerb;
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==PRESENT_PERFECT)
										{ // Beispiel: I have been used. -> Ich bin benutzt worden.
											CStringVector svVerb;
											GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0));
											// finite Form von 'sein' anhängen
										}
										vv.push_back(svVerb);
									}
								}
								if(IsEnglishPlural(subSentence.m_pSubject->
									m_vecEnumerationElement.at(0).m_pDan->m_Noun))
								{ // im Englischen: Plural
									if(vecEN.at(0).m_bTranslationType==0)
									{
										//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
										CStringVector svVerb;
										if(subSentence.m_bTense==SIMPLE_PRESENT || 
											subSentence.m_bTense==PRESENT_PROGRESSIVE ||
											subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
											subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // Beispiel: I am used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PAST || subSentence.
											m_bTense==PAST_PROGRESSIVE)
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										if(subSentence.m_bTense==PRESENT_PERFECT)
										{
											// wenn der Satz ein 
											// persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
										}
										vv.push_back(svVerb);
									}
									if(vecEN.at(0).m_bTranslationType==2)
									{
										CStringVector svVerb;
										if(subSentence.m_bTense==SIMPLE_PRESENT || 
											subSentence.m_bTense==PRESENT_PROGRESSIVE ||
											subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
											subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pPersonalSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pPersonalSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
										if(subSentence.m_bTense==SIMPLE_PAST || subSentence.
											m_bTense==PAST_PROGRESSIVE)
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==PRESENT_PERFECT)
										{
											// wenn der Satz ein 
											 // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
										}
										vv.push_back(svVerb);
									}
								}
							}
						}
						if(subSentence.m_pPersonalSubject->m_vecEnumerationElement.
							at(0).m_pEnr)
						{
							#ifdef _DEBUG
							printf("subSentence.m_pPersonalSubject->m_vecEnumeration\
Element.at(0).m_pEnr\n");
							#endif
							if(subSentence.m_pPersonalSubject->m_vecEnumerationElement.
								at(0).m_pEnr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
								size()>0)
								if(subSentence.m_pPersonalSubject->m_vecEnumerationElement.
									at(0).m_pEnr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
									at(0).m_bSingular)
								{
									#ifdef _DEBUG
									printf("subSentence.m_pPersonalSubject->m_enumerationElements.at(0).enr->m_vecPointerToVocNodeWhereAnEnglishNounIsEncapsulated.at(0).m_bSingular\n");
									#endif
									// im Englischen: Plural
									#ifdef _DEBUG
									printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
									#endif
									EnglishNoun * en=dynamic_cast<EnglishNoun *>(subSentence.
										m_pPersonalSubject->m_vecEnumerationElement.at(0).m_pEnr->
										m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
										at(0).m_pWordNode->m_pWord);
									if(en->m_bTranslationType==0)
									{ // im Deutschen: Singular
									//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
										#ifdef _DEBUG
										printf("en->m_TranslationType==0\n");
										#endif
										CStringVector svVerb;
										if(subSentence.m_bTense==SIMPLE_PRESENT || 
											subSentence.m_bTense==PRESENT_PROGRESSIVE ||
											subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
											subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											if(gv) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pPersonalSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pPersonalSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
										if(subSentence.m_bTense==SIMPLE_PAST || subSentence.
											m_bTense==PAST_PROGRESSIVE)
										{
											if(gv) // Beispiel: I could be used. / 
											{ // I could be being used.
												 svGermanVerb=GetCStringVector2(gv->m_strWords[
													 subSentence.m_pPersonalSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==PRESENT_PERFECT)
										{
												// wenn der Satz ein 
												// persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
										}
										vv.push_back(svVerb);
									}
									if(en->m_bTranslationType==1)
									{ // im Deutschen: Plural
										#ifdef _DEBUG
										printf("en->m_bTranslationType==1\n");
										#endif
										CStringVector svVerb;
										if(subSentence.m_bTense==SIMPLE_PRESENT || 
											subSentence.m_bTense==PRESENT_PROGRESSIVE ||
											subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
											subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											if(gv)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PAST || subSentence.
											m_bTense==PAST_PROGRESSIVE)
											if(gv) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										if(subSentence.m_bTense==PRESENT_PERFECT)
										{
											// wenn der Satz ein 
											 // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
										}
										vv.push_back(svVerb);
									}
								}
								else
								{ // Im Englischen: Plural
									#ifdef _DEBUG
									printf("!subSentence.m_pPersonalSubject->m_enumerationElements.\
at(0).enr->m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).m_bSingular\n");
									#endif
									EnglishNoun * en=dynamic_cast<EnglishNoun *>(subSentence.
									m_pPersonalSubject->m_vecEnumerationElement.at(0).m_pEnr->
										m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
									at(0).m_pWordNode->m_pWord);
									if(en->m_bTranslationType==0)
									{ // im Deutschen: Plural
									//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
										#ifdef _DEBUG
										printf("en->m_bTranslationType==0\n");
										#endif
										CStringVector svVerb;
										if(subSentence.m_bTense==SIMPLE_PRESENT || 
											subSentence.m_bTense==PRESENT_PROGRESSIVE ||
											subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
											subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											if(gv)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PAST || subSentence.
											m_bTense==PAST_PROGRESSIVE)
											if(gv) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										if(subSentence.m_bTense==PRESENT_PERFECT)
										{
												// wenn der Satz ein 
											// persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
										}
										vv.push_back(svVerb);
									}
									if(en->m_bTranslationType==2)
									{ // im Deutschen: Singular
										#ifdef _DEBUG
										printf("en->m_bTranslationType==2\n");
										#endif
										CStringVector svVerb;
										if(subSentence.m_bTense==SIMPLE_PRESENT || 
											subSentence.m_bTense==PRESENT_PROGRESSIVE ||
											subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
											subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											if(gv) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[3]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[9]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[3]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[9]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
										if(subSentence.m_bTense==SIMPLE_PAST || subSentence.
											m_bTense==PAST_PROGRESSIVE)
										{
											if(gv) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[9]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[9]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==PRESENT_PERFECT)
										{
											// wenn der Satz ein 
											 // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[3]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
										}
										vv.push_back(svVerb);
									}
								}
						}
						if(subSentence.m_pPersonalSubject->m_vecEnumerationElement.
							at(0).m_pGerund)
						{
							#ifdef _DEBUG
							printf("subSentence.m_pPersonalSubject->m_vecEnumerationElements.\
at(0).m_pGerund\n");
							#endif
							CStringVector svVerb;
							if(subSentence.m_bTense==SIMPLE_PRESENT || 
								subSentence.m_bTense==PRESENT_PROGRESSIVE ||
								subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
								subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pPersonalSubject->m_personIndex]);
								svVerb.push_back(svGermanVerb.at(0)); // Prädikat
								if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pPersonalSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
								// Prädikat (Präteritum)
							}
							if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
								PAST_PROGRESSIVE)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pPersonalSubject->m_personIndex+6]);
								svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							}
							vv.push_back(svVerb);
						}
						if(subSentence.m_pPersonalSubject->m_vecEnumerationElement.at(0).
							m_personalPronoun)
						{
							#ifdef _DEBUG
							printf("subSentence.m_pPersonalSubject->m_vecEnumerationElement.\
at(0).m_personalPronoun\n");
							#endif
							CStringVector svVerb;
							if(subSentence.m_bTense==SIMPLE_PRESENT || 
								subSentence.m_bTense==PRESENT_PROGRESSIVE ||
								subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
								subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								#ifdef _DEBUG
								printf("subSentence.tense==SIMPLE_PRESENT ||\
subSentence.tense==PRESENT_PROGRESSIVE || \
subSentence.tense==PRESENT_PERFECT_PROGRESSIVE || \
subSentence.tense==SIMPLE_PRESENT_OR_SIMPLE_PAST\n");
								#endif
								//printf("gv->words[0]: %s\n",gv->words[0]);
								BOOL bAuxiliaryVerb=FALSE;
								if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated)
									if(typeid(*subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)
										==typeid(EnglishAuxiliaryVerb) && 
										subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
										m_pWordNodeNext)
									{
										bAuxiliaryVerb=TRUE;
										GermanVerb * gv=dynamic_cast<GermanVerb*>(subSentence.
											m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										// wenn der Satz ein 
										 // persönliches Subjekt beinhaltet Beispiel: I can be given
										// CHANCES. -> Mir KÖNNEN Chancen gegeben werden. anstatt:
										// Ich KANN Chancen gegeben werden.
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pPersonalSubject->m_personIndex]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										// gebeugte From vom modalen Hilfsverb anhängen
									}
								if(!bAuxiliaryVerb) // kein modales Hilfsverb
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									// wenn der Satz ein 
									 // persönliches Subjekt beinhaltet Beispiel: I am given
									// CHANCES. -> Mir WERDEN Chancen gegeben. anstatt:
									// Ich WERDE Chancen gegeben.
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pPersonalSubject->m_personIndex]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									// gebeugte From von 'werden' anhängen
								}
								if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									// wenn der Satz ein 
									 // persönliches Subjekt beinhaltet Beispiel: I was given
									// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
										// Ich WURDE eine Chance gegeben.
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pPersonalSubject->m_personIndex+6]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									// gebeugte From von 'sein' anhängen
								}
								// Prädikat (Präteritum)
								vv.push_back(svVerb);
							}
							if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
								PAST_PROGRESSIVE)
							{ // Vergangenheit oder fortschreitende Vergangenheit
								//if(!gv)
								CStringVector svVerb;
								BOOL bAuxiliaryVerb=FALSE;
								if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated)
									if(typeid(*subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)
										==typeid(EnglishAuxiliaryVerb) && 
										subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
										m_pWordNodeNext)
									{
										bAuxiliaryVerb=TRUE;
										GermanVerb * gv=dynamic_cast<GermanVerb*>(subSentence.
											m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										// wenn der Satz ein 
										// persönliches Subjekt beinhaltet Beispiel: I could be given
										// CHANCES. -> Mir KONNTEN Chancen gegeben werden. anstatt:
										// Ich KONNTE Chancen gegeben werden.
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pPersonalSubject->m_personIndex+6]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										// gebeugte From vom modalen Hilfsverb anhängen
									}
								if(!bAuxiliaryVerb) // kein modales Hilfsverb
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									// wenn der Satz ein 
									 // persönliches Subjekt beinhaltet Beispiel: I was given
									// CHANCES. -> Mir WURDEN Chancen gegeben. anstatt:
									// Ich WURDE Chancen gegeben.
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pPersonalSubject->m_personIndex+6]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									// gebeugte From von 'werden' anhängen
								}
								vv.push_back(svVerb);
							}
							if(subSentence.m_bTense==PRESENT_PERFECT)
							{
								CStringVector svVerb;
								// wenn der Satz ein 
								// persönliches Subjekt beinhaltet Beispiel: I have been given
								// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
								// Ich BIN eine Chance gegeben worden.
								GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
								svGermanVerb=GetCStringVector2(gv->m_strWords[
									subSentence.m_pPersonalSubject->m_personIndex]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // Prädikat
								// gebeugte From von 'sein' anhängen
								vv.push_back(svVerb);
							}
						} // Ende personalPronoun
					}
				}
				//wenn genau ein Aufzählungselement
				else if(subSentence.m_pSubject->m_vecEnumerationElement.size()==1)
				{
					#ifdef _DEBUG
					printf("subSentence.m_pSubject->m_vecEnumerationElement.size()==1\n");
					#endif
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).
						m_vecStrPlainText.size()>0)
					{
#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).\
m_vecStrPlainText.size()>0\n");
#endif
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || 
							subSentence.m_bTense==PRESENT_PROGRESSIVE ||
							subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							if(subSentence.m_bActiveClause)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
								svVerb.push_back(svGermanVerb.at(0));
								// Prädikat (Präsens)
								if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präteritum)
								}
							}
							else
							{
								if(gvInfinitive) // Beispiel: 'can be used'
								{ 
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präsens)
									if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
								else // Beispiel: 'is used'
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präsens)
									if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
							}
						}
						if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
							PAST_PROGRESSIVE)
						{
							if(subSentence.m_bActiveClause) // Beispiel: I used water.
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex+6]);
								svVerb.push_back(svGermanVerb.at(0));
							}
							else
							{
								if(gvInfinitive) // Beispiel: I could be used. / 
								{ // I could be being used.
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
								else // Beispiel: I was used. / I was being used.
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
							}
						}
						if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
							m_bActiveClause)
						{ // Beispiel: I have been used. -> Ich bin benutzt worden.
							GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
							svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex]);
							svVerb.push_back(svGermanVerb.at(0));
							// finite Form von 'sein' anhängen
						}
						if(subSentence.m_bTense==PRESENT_PERFECT && subSentence.
							m_bActiveClause)
						{
							//Hilfsverb: sein
							// Beispiel: Where have you been? -> Wo bist du gewesen?
							if(gvInfinitive->m_bBe)
							{
								GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
								svVerb.push_back(svGermanVerb.at(0));
							}
							//Hilfsverb: haben
							// Beispiel: Why have I liked you? -> Warum habe ich dich gemocht?
							else
							{
								GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("have"));
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
								svVerb.push_back(svGermanVerb.at(0));
							}
							// finite Form von 'sein' anhängen
						}
						// Prädikat (Präteritum)
						vv.push_back(svVerb);
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pDan)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pDan\n");
						#endif
						std::vector <EnglishNoun> vecEN=GetEnglishNoun(subSentence.
							m_pSubject->m_vecEnumerationElement.at(0).m_pDan->m_Noun);
						if(vecEN.size()>0)
						{
							if(IsEnglishSingular(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pDan->m_Noun))
							{
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
													m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
													m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT)
									{
										if(subSentence.m_bActiveClause) // Aktivsatz
										{
											GermanVerb * gv=NULL;
											if(gvInfinitive)
												if(gvInfinitive->m_bBe)
													gv=GetGermanWordOfEnglishVerb(_T("be"));
												else
													gv=GetGermanWordOfEnglishVerb(_T("be"));
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
											{ // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
										}
									}
									// Prädikat (Präteritum)
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==1)
								{ // im Deutschen: Plural
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
							}
							if(IsEnglishPlural(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pDan->m_Noun))
							{ // im Englischen: Plural
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // Beispiel: I am used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT)
									{
										if(subSentence.m_bActiveClause) // Aktivsatz
										{
											GermanVerb * gv=NULL;
											if(gvInfinitive)
												if(gvInfinitive->m_bBe)
													gv=GetGermanWordOfEnglishVerb(_T("be"));
												else
													gv=GetGermanWordOfEnglishVerb(_T("be"));
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
											{ // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
										}
									}
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==2)
								{
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT)
									{
										if(subSentence.m_bActiveClause) // Aktivsatz
										{
											GermanVerb * gv=NULL;
											if(gvInfinitive)
												if(gvInfinitive->m_bBe)
													gv=GetGermanWordOfEnglishVerb(_T("be"));
												else
													gv=GetGermanWordOfEnglishVerb(_T("be"));
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
											{ // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
										}
									}
									vv.push_back(svVerb);
								}
							}
						}
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pEnr)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pEnr\n");
						#endif
						std::vector <EnglishNoun> vecEN=GetEnglishNoun(subSentence.
							m_pSubject->m_vecEnumerationElement.at(0).m_pEnr->m_Noun);
						if(vecEN.size()>0)
						{
							#ifdef _DEBUG
							printf("vecEN.size()>0\n");
							printf("vecEN.at(0).m_bTranslationType: %u\n",vecEN.at(0).
								m_bTranslationType);
							#endif
							if(IsEnglishSingular(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pEnr->m_Noun))
							{ // im Englischen: Plural
								#ifdef _DEBUG
								printf("IsEnglishSingular(subSentence.m_pSubject->\
									m_enumerationElements.at(0).m_pEnr->m_Noun)\n");
								printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
								#endif
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									#ifdef _DEBUG
									printf("vecEN.at(0).m_bTranslationType==0\n");
									#endif
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												 svGermanVerb=GetCStringVector2(gv->m_strWords[
													 subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT)
									{
										if(subSentence.m_bActiveClause) // Aktivsatz
										{
											GermanVerb * gv=NULL;
											if(gvInfinitive)
												if(gvInfinitive->m_bBe)
													gv=GetGermanWordOfEnglishVerb(_T("be"));
												else
													gv=GetGermanWordOfEnglishVerb(_T("be"));
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
											{ // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
										}
									}
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==1)
								{ // im Deutschen: Singular
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT)
									{
										if(subSentence.m_bActiveClause) // Aktivsatz
										{
											GermanVerb * gv=NULL;
											if(gvInfinitive)
												if(gvInfinitive->m_bBe)
													gv=GetGermanWordOfEnglishVerb(_T("be"));
												else
													gv=GetGermanWordOfEnglishVerb(_T("be"));
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
											{ // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
										}
									}
									vv.push_back(svVerb);
								}
							}
							if(IsEnglishPlural(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pEnr->m_Noun))
							{ // Im Englischen: Plural
								#ifdef _DEBUG
								printf("IsEnglishPlural(subSentence.m_pSubject->\
m_enumerationElements.at(0).enr->m_Noun)\n");
								#endif
								if(vecEN.at(0).m_bTranslationType==0)
								{ // im Deutschen: Plural
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT)
									{
										if(subSentence.m_bActiveClause) // Aktivsatz
										{
											GermanVerb * gv=NULL;
											if(gvInfinitive)
												if(gvInfinitive->m_bBe)
													gv=GetGermanWordOfEnglishVerb(_T("be"));
												else
													gv=GetGermanWordOfEnglishVerb(_T("be"));
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
											{ // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
										}
									}
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==2)
								{ // im Deutschen: Singular
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT)
									{
										if(subSentence.m_bActiveClause) // Aktivsatz
										{
											GermanVerb * gv=NULL;
											if(gvInfinitive)
												if(gvInfinitive->m_bBe)
													gv=GetGermanWordOfEnglishVerb(_T("be"));
												else
													gv=GetGermanWordOfEnglishVerb(_T("be"));
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
											{ // persönliches Subjekt beinhaltet Beispiel: I have been given
												// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
												// Ich BIN eine Chance gegeben worden.
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pPersonalSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
												// gebeugte From von 'sein' anhängen
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												// GetCStringVector deshalb, weil ein Verb in der gebeugten
												// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
										}
									}
									vv.push_back(svVerb);
								}
							}
						}
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pGerund)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pGerund\n");
						#endif
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || 
							subSentence.m_bTense==PRESENT_PROGRESSIVE ||
							subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex]);
							svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex+6]);
								svVerb.push_back(svGermanVerb.at(0));
							}
								// Prädikat (Präteritum)
						}
						if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
							PAST_PROGRESSIVE)
						{
							svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex+6]);
							svVerb.push_back(svGermanVerb.at(0)); // Prädikat
						}
						vv.push_back(svVerb);
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).
						m_personalPronoun)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).\
m_personalPronoun\n");
						#endif
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || 
							subSentence.m_bTense==PRESENT_PROGRESSIVE ||
							subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							#ifdef _DEBUG
							printf("subSentence.m_bTense==SIMPLE_PRESENT ||\
subSentence.m_bTense==PRESENT_PROGRESSIVE || \
subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE || \
subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST\n");
							#endif
							//printf("gv->words[0]: %s\n",gv->words[0]);
							if(subSentence.m_bActiveClause) // Aktivsatz
							{
								if(gv)
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex]);
								if(gvInfinitive)
									svGermanVerb=GetCStringVector2(gvInfinitive->m_strWords[
										subSentence.m_pSubject->m_personIndex]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // 1. Wort des Vektors anhängen
							}
							else // Passivsatz
							{
								BOOL bAuxiliaryVerb=FALSE;
								if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated)
									if(typeid(*subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)
										==typeid(EnglishAuxiliaryVerb) && 
										subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
										m_pWordNodeNext)
									{
										bAuxiliaryVerb=TRUE;
										GermanVerb * gv=dynamic_cast<GermanVerb*>(subSentence.
											m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
										{ // persönliches Subjekt beinhaltet Beispiel: I can be given
											// CHANCES. -> Mir KÖNNEN Chancen gegeben werden. anstatt:
											// Ich KANN Chancen gegeben werden.
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pPersonalSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											// gebeugte From vom modalen Hilfsverb anhängen
										}
										else // zum Beispiel: I can be used. 
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
									}
								if(!bAuxiliaryVerb) // kein modales Hilfsverb
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
									{ // persönliches Subjekt beinhaltet Beispiel: I am given
										// CHANCES. -> Mir WERDEN Chancen gegeben. anstatt:
										// Ich WERDE Chancen gegeben.
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pPersonalSubject->m_personIndex]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										// gebeugte From von 'werden' anhängen
									}
									else // zum Beispiel: I am be used. 
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									}
								}
							}
							if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								if(subSentence.m_bActiveClause) // Aktivsatz
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0));
								}
								else // Passivsatz
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
									{ // persönliches Subjekt beinhaltet Beispiel: I was given
										// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
										// Ich WURDE eine Chance gegeben.
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pPersonalSubject->m_personIndex+6]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										// gebeugte From von 'sein' anhängen
									}
									else
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									}
								}
							}
								// Prädikat (Präteritum)
							vv.push_back(svVerb);
						}
						if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
							PAST_PROGRESSIVE)
						{ // Vergangenheit oder fortschreitende Vergangenheit
							if(subSentence.m_bActiveClause)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex+6]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							}
							else // Passivsatz
							{
								//if(!gv)
								BOOL bAuxiliaryVerb=FALSE;
								if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated)
									if(typeid(*subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)
										==typeid(EnglishAuxiliaryVerb) && 
										subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
										m_pWordNodeNext)
									{
										bAuxiliaryVerb=TRUE;
										GermanVerb * gv=dynamic_cast<GermanVerb*>(subSentence.
											m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
										{ // persönliches Subjekt beinhaltet Beispiel: I could be given
											// CHANCES. -> Mir KONNTEN Chancen gegeben werden. anstatt:
											// Ich KONNTE Chancen gegeben werden.
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pPersonalSubject->m_personIndex+6]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											// gebeugte From vom modalen Hilfsverb anhängen
										}
										else // zum Beispiel: I could be used. 
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex+6]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
									}
								if(!bAuxiliaryVerb) // kein modales Hilfsverb
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
									{ // persönliches Subjekt beinhaltet Beispiel: I was given
										// CHANCES. -> Mir WURDEN Chancen gegeben. anstatt:
										// Ich WURDE Chancen gegeben.
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pPersonalSubject->m_personIndex+6]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										// gebeugte From von 'werden' anhängen
									}
									else // zum Beispiel: I was used. 
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									}
								}
							}
							vv.push_back(svVerb);
						}
						if(subSentence.m_bTense==PRESENT_PERFECT)
						{
							if(subSentence.m_bActiveClause)
							{
								GermanVerb * gv=NULL;
								if(gvInfinitive)
									if(gvInfinitive->m_bBe)
										gv=GetGermanWordOfEnglishVerb(_T("be"));
									else
										gv=GetGermanWordOfEnglishVerb(_T("be"));
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							}
							else // Passivsatz
							{
								if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
								{ // persönliches Subjekt beinhaltet Beispiel: I have been given
									// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
									// Ich BIN eine Chance gegeben worden.
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pPersonalSubject->m_personIndex]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									// gebeugte From von 'sein' anhängen
								}
								else
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
								}
							}
							vv.push_back(svVerb);
						}
					}
				}
				else
				{
					CStringVector svVerb;
					if(subSentence.m_bTense==SIMPLE_PRESENT || 
						subSentence.m_bTense==PRESENT_PROGRESSIVE ||
						subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
						subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
					{
						svVerb.push_back(gv->m_strWords[subSentence.m_pSubject->
							m_personIndex]); // Prädikat
						if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							svVerb.push_back(gv->m_strWords[subSentence.m_pSubject->
							m_personIndex+6]);
							// Prädikat (Präteritum)
					}
					if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
						PAST_PROGRESSIVE)
						svVerb.push_back(gv->m_strWords[subSentence.m_pSubject->
						m_personIndex+6]); // Prädikat
					vv.push_back(svVerb);
				}
			//	}
			}
			#ifdef _DEBUG
			else
			{
				TRACE("gv==NULL\n");
				printf("gv==NULL\n");
			}
			#endif
		}
		BOOL bNotInserted=FALSE;
		#ifdef _DEBUG
		printf("subSentence.m_pSubject->m_personIndex : %u\n",subSentence.
			m_pSubject->m_personIndex);
		#endif
		//wenn Passivsatz und der Satz ein persönliches Subjekt enthält 
		// Beispiel: Am I being given a real chance?
		//           -P S -----P----- ------PS-----  // PS=persönliches Subjekt
		//           Wird mir eine wirkliche Chance gegeben?
		if(!subSentence.m_bActiveClause && subSentence.m_pPersonalSubject)
		{
			BYTE b=0;
			BYTE b2=0;
			#ifdef _DEBUG
			printf("!subSentence.m_bActiveClause && subSentence.m_pPersonalSubject\n");
			#endif
			std::vector<CStringVector> vvObject=TranslateSubject(
				*subSentence.m_pSubject,TRUE,b,b2,//pEV,
        rvecintpair,nStartID);
			Append(vv,vvObject); // Subjekt
			if(subSentence.m_bNegation) // bei einer Verneinung steht das "nicht"
			{ // vor dem Objekt
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
				bNotInserted=TRUE;
			}
		}
		//wenn Aktivsatz und/oder kein persönliches Subjekt vorhanden ist
		else
		{
			BYTE b=0;
			BYTE b2=0;
#ifdef _DEBUG
			printf("!(!subSentence.m_bActiveClause && subSentence.m_pPerso\
nalSubject)\n");
#endif
			//Das Subjekt kann in einem Fragesatz nie das erste Token sein,
			//deswegen muss der letzte Parameter 0 sein
			std::vector<CStringVector> vvSubject=TranslateSubject(
				*subSentence.m_pSubject,FALSE,b,b2,//pEV,
        rvecintpair,nStartID);
			Append(vv,vvSubject); // Subjekt
		}
		BYTE b=0;
		BYTE b2=0;
		if(subSentence.m_pPersonalSubject)
			Append(vv,TranslateSubject(*subSentence.m_pPersonalSubject,
			FALSE,b,b2,//pEV,
      rvecintpair,nStartID));
		//std::vector<CStringVector> vvSubject=TranslateSubject2(subSentence.m_Subject);
		//Append(vv,vvSubject); // Subjekt
		if(subSentence.m_pRelativeClause)
		{
			if(subSentence.m_pRelativeClause->m_bRefersToSubject)
			{
				#ifdef _DEBUG
				printf("subSentence.m_pRelativeClause->m_bRefersToSubject\n");
				#endif
				if(subSentence.m_pSubject->m_vecEnumerationElement.size()>0)
				{
					std::vector <GermanNoun> vecGermanNoun=
						GetGermanWordOfEnglishNoun2(subSentence.m_pSubject->
						m_vecEnumerationElement.at(0).m_pDan->m_Noun);
					if(vecGermanNoun.size()>0)
					{
						CStringVector sv;
						sv.push_back(_T(","));
						vv.push_back(sv);
						sv.clear();
						GermanNoun gn=vecGermanNoun.at(0);
						if(gn.m_bArticle=='1')
							sv.push_back(_T("den"));
						else if(gn.m_bArticle=='2')
							sv.push_back(_T("die"));
						else if(gn.m_bArticle=='3')
							sv.push_back(_T("das"));
						vv.push_back(sv);
					}
				}
				Append(vv,TranslateRelativeClause(*subSentence.m_pRelativeClause,0,
					//pEV,
          rvecintpair,nStartID));
				CStringVector sv;
				sv.push_back(_T(","));
				vv.push_back(sv);
			}
		}
		if(gv)
			if(gv->m_strPreposition!="" && subSentence.m_pObjectFirst)
			{
				CStringVector sv;
				if(subSentence.m_bNegation && !bNotInserted) // wenn ein Verb mit einer 
				{ // Präposition verbunden ist ("MIT jemandem reden"),
					// steht die Verneinung ('nicht') vor dieser
					sv.push_back("nicht");
					vv.push_back(sv);
					sv.clear();
					bNotInserted=TRUE;
				}
				sv.push_back(gv->m_strPreposition);
				vv.push_back(sv);
			}
		if(gvInfinitive)
			if(gvInfinitive->m_strPreposition!="" && subSentence.m_pObjectFirst)
			{
				CStringVector sv;
				if(subSentence.m_bNegation) // wenn ein Verb mit einer Präposition
				{ // verbunden ist, steht die Verneinung ('nicht') vor dieser
					sv.push_back("nicht");
					vv.push_back(sv);
					sv.clear();
					bNotInserted=TRUE;
				}
				sv.push_back(gvInfinitive->m_strPreposition);
				vv.push_back(sv);
			}
		if(subSentence.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
		{
			GermanAdverb * ga=dynamic_cast<GermanAdverb *>(subSentence.
				m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->m_pWordNodeNext->
				m_pWord);
			if(ga)
			{
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		if(subSentence.m_pObjectFirst) // Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectFirst\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(subSentence.m_bNegation && subSentence.m_pObjectFirst->
				m_vecEnumerationElement.at(0).m_pGerund)
			{
				bNotInserted=TRUE;
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
			BYTE Case=0;
			//if(gvInfinitive)
			//	Case=gvInfinitive->Case;
			//else if(gv)
			if(gv)
				Case=gv->m_bCase;
			else
			{
				if(gvInfinitive)
					Case=gvInfinitive->m_bCase;
			}
#ifdef _DEBUG
			printf("Case: %u\n",Case);
#endif
			if(Case==1 || Case==3) // wenn das Verb 1 Objekt im vierten Fall oder 
				// 2 Objekt verlangt
			{ // im dritten Fall übersetzen
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectFirst,1,subSentence.m_bNegation,b,//pEV,
          rvecintpair,nStartID);
				Append(vv,vvObject); // Objekt
			}
			else if(Case==2)
			{ // im vierten Fall übersetzen
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectFirst,2,subSentence.m_bNegation,b,//pEV,
          rvecintpair,nStartID);
				Append(vv,vvObject); // Objekt
			}
			else
			{ // mit keinem Fall übersetzen
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectFirst,0,subSentence.m_bNegation,b,//pEV,
          rvecintpair,nStartID);
				Append(vv,vvObject); // Objekt
			}
		}
		if(subSentence.m_pObjectSecond) // 2. Objekt
		{
			BYTE b=0;
			std::vector<CStringVector> vvObject=TranslateObject2(
				*subSentence.m_pObjectSecond,0,subSentence.m_bNegation,b,//pEV,
        rvecintpair,nStartID);
			Append(vv,vvObject); // Objekt
		}
		#ifdef _DEBUG
		else
			printf("subSentence.m_Object.m_enumerationElements.size()<=0\n");
		#endif
		if(subSentence.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(subSentence.m_bNegation && !bNotInserted)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanClause,germanObject); // Subjekt
		if(!subSentence.m_bBe)
			if(subSentence.m_bNegation && !bNotInserted)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		for(DWORD i=0;i<subSentence.m_vecEnglishAdverb.size();i++) // Adverbien
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		/*if(subSentence.m_Object.m_enumerationElements.size()>0) // Objekt
		{
			std::vector<CStringVector> vvObject=TranslateObject2(subSentence.m_Object);
			Append(vv,vvObject); // Objekt
		}*/
		if(subSentence.m_pManner) // Umstandsbestimmung der Art und Weise
		{
			if(subSentence.m_pManner->m_vecMannerEnumerationElement.size()>0) // Objekt
			{
				Append(vv,TranslateManner(*subSentence.m_pManner,nStartID));
			}
		}
		if(subSentence.m_bTense==PRESENT_PERFECT)
		{
			//CStringVector sv;
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			//sv.push_back(gv->words[13]);
			//vv.push_back(sv);
		}
		for(i=0;i<subSentence.m_vecMiddleEnumerationElement.size();i++)
		{
			if(i>0)
				if((i+1)==subSentence.m_vecMiddleEnumerationElement.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				//sv.push_back(subSentence.m_MiddleEnumerationElements.at(i).m_strAdverbRefersToAdjectiveOrAdverb);
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).
					m_strAdverbRefersToAdjectiveOrAdverb);
				if(typeid(*pWord)==typeid(GermanAdverb)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdverb * ga=dynamic_cast<GermanAdverb*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strWord);
				}
				vv.push_back(sv);
			}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
		if(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated)
			if(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated->
				m_pWordNodeNext)
			{
				#ifdef _DEBUG
				printf("subSentence.m_pVocNodeWhereAnEnglishAdverbOfTimeIs\
Encapsulated->next\n");
				#endif
				GermanAdverb * ga=dynamic_cast<GermanAdverb *>(subSentence.
					m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated->
					m_pWordNodeNext->m_pWord);
				if(ga)
				{
					CStringVector sv;
					sv.push_back(ga->m_strWord);
					vv.push_back(sv);
				}
			}
		if(gvInfinitive) // 
		{
			#ifdef _DEBUG
			printf("gvInfinitive!=NULL\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(subSentence.m_bActiveClause) // wenn Aktivsatz
			{
				//wenn die Zeitform "Zukunft", "Präsens" oder "Präteritum" ist
				if(subSentence.m_bTense==FUTURE || subSentence.m_bTense==SIMPLE_PRESENT ||
					subSentence.m_bTense==SIMPLE_PAST)
				{
					CStringVector sv;
					//Infinitiv anhängen
					sv.push_back(gvInfinitive->m_strWords[0]);
					vv.push_back(sv);
				}
				//wenn die Zeitform "fortschreitendes Präsens" oder "fortschreitendes 
				//Präteritum" oder "Perfekt" oder "Plusquamperfekt" ist
				else
				{
					//wenn eine gebeugte Verbform existiert. Beispiel: "I can be 
					//hurting you".
					if(gv)
					{
						CStringVector sv;
						sv.push_back(gvInfinitive->m_strWords[13]); // Partizip Perfekt
						vv.push_back(sv);
					}
					else
					{
						if(gvInfinitive)
						{
							CStringVector sv;
							sv.push_back(gvInfinitive->m_strWords[13]); // Partizip Perfekt
							vv.push_back(sv);
						}
					}
				}
			}
			else // Passivsatz
			{
				//printf("gvInfinitive==NULL\n");
				CStringVector sv;
				sv.push_back(gvInfinitive->m_strWords[13]); // Partizip Perfekt ('geschlagen')
				vv.push_back(sv);
				if(subSentence.m_bTense==PRESENT_PERFECT)
				{
					sv.clear();
					sv.push_back("worden"); // Infinitiv
					vv.push_back(sv);
				}
				if(subSentence.m_bTense==FUTURE || gv)
				{
					sv.clear();
					sv.push_back("werden"); // Infinitiv
					vv.push_back(sv);
				}
			}
		}
		//Append(vv,vvSubject=TranslateObject2(subSentence.m_Object)); // Objekt
		if(subSentence.m_pRelativeClause)
			if(!subSentence.m_pRelativeClause->m_bRefersToSubject) // Relativsatz bezieht sich aufs Objekt
				if(subSentence.m_pObjectFirst)
				{
					std::vector <GermanNoun> vecGermanNoun=
						GetGermanWordOfEnglishNoun2(subSentence.m_pObjectFirst->
						m_vecEnumerationElement.at(0).m_pDan->m_Noun);
					if(vecGermanNoun.size()>0)
					{
						CStringVector sv;
						sv.push_back(_T(","));
						vv.push_back(sv);
						sv.clear();
						GermanNoun gn=vecGermanNoun.at(0);
						if(gn.m_bArticle=='1')
							sv.push_back(_T("den"));
						else if(gn.m_bArticle=='2')
							sv.push_back(_T("die"));
						else if(gn.m_bArticle=='3')
							sv.push_back(_T("das"));
						vv.push_back(sv);
					}
					Append(vv,TranslateRelativeClause(*subSentence.
						m_pRelativeClause,0,//pEV,
            rvecintpair,nStartID));
				}
		//}
		if(subSentence.m_pPlace)
		{
		BYTE b=0;
			std::vector<CStringVector> vvPlace=TranslatePlace(*subSentence.
				m_pPlace,b,nStartID);
			Append(vv,vvPlace);
		}
		if(svGermanVerb.size()>1)
		{
			CStringVector sv;
			sv.push_back(svGermanVerb.at(1));
			vv.push_back(sv);
		}
	}//german worder order is SPO
	if(subSentence.m_bWordOrder==PSO)
	{
		GermanVerb * gv=GetGermanWordOfEnglishVerb(subSentence.m_strPredicate);
		CStringVector sv;
		BYTE b=0;
		sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]); // Prädikat
		vv.push_back(sv);
		std::vector<CStringVector> vvSubject=TranslateSubject(*subSentence.
			m_pSubject,FALSE,subSentence.m_pSubject->m_personIndex,
			bFirstTokenInSentence,//pEV,
      rvecintpair,nStartID);
		Append(vv,vvSubject); // Subjekt
		if(subSentence.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanClause,germanObject); // Subjekt
		//wenn es mindestens ein Objekt gibt
		if(subSentence.m_pObjectFirst)
		{
			BYTE b=0;
			std::vector<CStringVector> vvObject=TranslateObject2(
				*subSentence.m_pObjectFirst,0,0,b,//pEV,
        rvecintpair,nStartID);
			Append(vv,vvObject); // Subjekt
		}
		if(!subSentence.m_bBe)
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		for(DWORD i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		for(i=0;i<subSentence.m_vecMiddleEnumerationElement.size();i++)
		{
			if(i>0)
				if((i+1)==subSentence.m_vecMiddleEnumerationElement.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				sv.push_back(subSentence.m_vecMiddleEnumerationElement.at(i).
					m_strAdverbRefersToAdjectiveOrAdverb);
				vv.push_back(sv);
			}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
	}//if(subSentence.m_bWordOrder==PSO)
	if(subSentence.m_bWordOrder==SOP)
	{
		TRACE("satzbau==SOP\n");
		BYTE b=0;
		std::vector<CStringVector> vvSubject=TranslateSubject(*subSentence.
			m_pSubject,FALSE,subSentence.m_pSubject->m_personIndex,
			bFirstTokenInSentence,//pEV,
      rvecintpair,nStartID);
		Append(vv,vvSubject); // Subjekt
		//TRACE("satzbau==SPO\n");
		if(subSentence.m_pObjectFirst)
		{
			BYTE b=0;
			std::vector<CStringVector> vvObject=TranslateObject2(
				*subSentence.m_pObjectFirst,0,0,b,//pEV,
        rvecintpair,nStartID);
			Append(vv,vvObject); // Subjekt
		}
		GermanVerb * gv=GetGermanWordOfEnglishVerb(subSentence.m_strPredicate);
		if(gv!=NULL)
		{
			int index=0;
			if((index=subSentence.m_pSubject->m_personIndex*subSentence.m_bTense+1)<14)
			{
				CStringVector sv;
				if(subSentence.m_bTense==SIMPLE_PRESENT)
					sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]); // Prädikat
				if(subSentence.m_bTense==SIMPLE_PAST)
					sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex+6]); // Prädikat
				vv.push_back(sv);
			}
		}
		else
			TRACE("gv==NULL\n");
		if(subSentence.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanClause,germanObject); // Subjekt
		if(!subSentence.m_bBe)
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		for(DWORD i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
	}//if(subSentence.m_bWordOrder==SOP)
	for(DWORD i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
	{
		if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='2')
		{
			GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
				m_vecEnglishAdverb.at(i).m_strWord);
			CStringVector sv;
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	for(i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
	{
		if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='3')
		{
			GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
				m_vecEnglishAdverb.at(i).m_strWord);
			CStringVector sv;
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	//germanClause.push_back(TranslatePredicate(subSentence.germanSubject.at(i));
	//CStringVector germanSubject=TranslateSubject(subSentence.m_Subject);
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateQuestionClause(Clause \
& subSentence) - ENDE\n");
	#endif
	return vv;
}

/*std::vector<CStringVector> TranslateRelativeClause(RelativeClause subSentence)
{
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateRelativeClause(RelativeClause \
subSentence) - ANFANG\n");
	if(subSentence.m_bNegation)
		TRACE("subSentence.m_bNegation\n");
	else
		TRACE("!subSentence.m_bNegation\n");
	#endif
	std::vector<CStringVector> vv;
	//BYTE satzbau=SPO;
	if(subSentence.m_bWordOrder==SPO) // Subjekt Prädikat Objekt
	{
		BYTE b=0;
		std::vector<CStringVector> vvSubject=TranslateSubject(*subSentence.
			m_pSubject,FALSE,subSentence.m_Subject.m_personIndex,
			bFirstTokenInSentence,nStartID);
		Append(vv,vvSubject); // Subjekt
		if(subSentence.m_Object.m_vecEnumerationElement.size()>0) // Objekt
		{
			std::vector<CStringVector> vvObject=TranslateObject2(subSentence.m_Object,3);
			Append(vv,vvObject); // Objekt
		}
		if(subSentence.m_Subject.m_vecEnumerationElement.size()>0)
		{
			#ifdef _DEBUG
			printf("subSentence.m_pSubject->m_vecEnumerationElement.size(): %u\n",
				subSentence.m_Subject.m_vecEnumerationElement.size());
			printf("subSentence.m_Subject.m_vecEnumerationElement.size()>0\n");
			if(subSentence.m_Subject.m_vecEnumerationElement.at(0).m_pEnr)
				printf("subSentence.m_Subject.m_vecEnumerationElement.at(0).m_pEnr==TRUE\n");
			if(subSentence.m_Subject.m_vecEnumerationElement.at(0).m_pDan)
				printf("subSentence.m_Subject->m_vecEnumerationElement.at(0).m_pEnr==TRUE\n");
			printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).\
m_personalPronoun: %u\n",subSentence.m_Subject.m_vecEnumerationElement.at(0).
			m_personalPronoun);
			#endif
		}
		//TRACE("satzbau==SPO\n");
		TRACE("subSentence.m_bTense: %u\n",subSentence.m_bTense);
		if(subSentence.m_bTense==FUTURE)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
			sv.push_back(gv->m_strWords[subSentence.m_Subject.m_personIndex]);
			vv.push_back(sv);
		}
		if(subSentence.m_bTense==PRESENT_PERFECT)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("have"));
			sv.push_back(gv->m_strWords[subSentence.m_Subject.m_personIndex]);
			vv.push_back(sv);
		}
		if(subSentence.m_bTense==PRESENT_PROGRESSIVE || subSentence.m_bTense==
			SIMPLE_PRESENT || subSentence.m_bTense==SIMPLE_PAST)
		{
			GermanVerb * gv=GetGermanWordOfEnglishVerb(subSentence.m_strPredicate);
			if(gv!=NULL)
			{
//				int index=0;
	//			if((index=subSentence.m_Subject.m_personIndex*subSentence.tense+1)<14)
		//		{
					CStringVector svVerb;
					if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==
						PRESENT_PROGRESSIVE)
						svVerb.push_back(gv->m_strWords[subSentence.m_Subject.m_personIndex]); // Prädikat
					if(subSentence.m_bTense==SIMPLE_PAST)
						svVerb.push_back(gv->m_strWords[subSentence.m_Subject.m_personIndex+6]); // Prädikat
					vv.push_back(svVerb);
			//	}
			}
			else
				TRACE("gv==NULL\n");
		}
		if(subSentence.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanSubSentence,germanObject); // Subjekt
		if(!subSentence.m_bBe)
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		if(subSentence.m_bTense==PRESENT_PERFECT)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(subSentence.m_strPredicate);
			sv.push_back(gv->m_strWords[13]);
			vv.push_back(sv);
		}
		for(int i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		for(i=0;i<subSentence.m_vecMiddleEnumerationElement.size();i++)
		{
			if(i>0)
				if((i+1)==subSentence.m_vecMiddleEnumerationElement.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				//sv.push_back(subSentence.m_MiddleEnumerationElements.at(i).m_strAdverbRefersToAdjectiveOrAdverb);
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).
					m_strAdverbRefersToAdjectiveOrAdverb);
				if(typeid(*pWord)==typeid(GermanAdverb)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdverb * ga=dynamic_cast<GermanAdverb*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strWord);
				}
				vv.push_back(sv);
			}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
		if(subSentence.m_bTense==FUTURE) // wenn Zeitform Zukunft
		{
			GermanVerb * gv=GetGermanWordOfEnglishVerb(subSentence.m_strPredicate);
			if(gv!=NULL)
			{
				CStringVector sv;
				sv.push_back(gv->m_strWords[0]); // Infinitiv
				vv.push_back(sv);
			}
		}
	}
	if(subSentence.m_bWordOrder==PSO)
	{

		GermanVerb * gv=GetGermanWordOfEnglishVerb(subSentence.m_strPredicate);
		CStringVector sv;
		sv.push_back(gv->m_strWords[subSentence.m_Subject.m_personIndex]); // Prädikat
		vv.push_back(sv);
		BYTE b=0;
		std::vector<CStringVector> vvSubject=TranslateSubject(*subSentence.
			m_pSubject,FALSE,subSentence.m_pSubject->m_personIndex,
			bFirstTokenInSentence,nStartID);
		Append(vv,vvSubject); // Subjekt
		if(subSentence.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanSubSentence,germanObject); // Subjekt
		if(subSentence.m_Object.m_vecEnumerationElement.size()>0)
		{
			std::vector<CStringVector> vvObject=TranslateObject2(subSentence.m_Object);
			Append(vv,vvObject); // Subjekt
		}
		if(!subSentence.m_bBe)
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		for(int i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		for(i=0;i<subSentence.m_vecMiddleEnumerationElement.size();i++)
		{
			if(i>0)
				if((i+1)==subSentence.m_vecMiddleEnumerationElement.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				sv.push_back(subSentence.m_vecMiddleEnumerationElement.at(i).
					m_strAdverbRefersToAdjectiveOrAdverb);
				vv.push_back(sv);
			}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
	}
	if(subSentence.m_bWordOrder==SOP)
	{
		BYTE b=0;
		std::vector<CStringVector> vvSubject=TranslateSubject(*subSentence.
			m_pSubject,FALSE,subSentence.m_pSubject->m_personIndex,
			bFirstTokenInSentence,nStartID);
		Append(vv,vvSubject); // Subjekt
		//TRACE("satzbau==SPO\n");
		if(subSentence.m_Object.m_vecEnumerationElement.size()>0)
		{
			std::vector<CStringVector> vvObject=TranslateObject2(subSentence.m_Object);
			Append(vv,vvObject); // Subjekt
		}
		GermanVerb * gv=GetGermanWordOfEnglishVerb(subSentence.m_strPredicate);
		if(gv!=NULL)
		{
			int index=0;
			if((index=subSentence.m_Subject.m_personIndex*subSentence.m_bTense+1)<14)
			{
				CStringVector sv;
				if(subSentence.m_bTense==SIMPLE_PRESENT)
					sv.push_back(gv->m_strWords[subSentence.m_Subject.m_personIndex]); // Prädikat
				if(subSentence.m_bTense==SIMPLE_PAST)
					sv.push_back(gv->m_strWords[subSentence.m_Subject.m_personIndex+6]); // Prädikat
				vv.push_back(sv);
			}
		}
		else
			TRACE("gv==NULL\n");
		if(subSentence.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanSubSentence,germanObject); // Subjekt
		if(!subSentence.m_bBe)
			if(subSentence.m_bNegation)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		for(int i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
	}
	for(int i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
	{
		if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='2')
		{
			GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
				m_vecEnglishAdverb.at(i).m_strWord);
			CStringVector sv;
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	for(i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
	{
		if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='3')
		{
			GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
				m_vecEnglishAdverb.at(i).m_strWord);
			CStringVector sv;
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	//germanClause.push_back(TranslatePredicate(subSentence.germanSubject.at(i));
	//CStringVector germanSubject=TranslateSubject(subSentence.m_Subject);
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateRelativeClause(\
RelativeClause subSentence) - ENDE\n");
	#endif
	return vv;
}*/

//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
// m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
// zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
// (Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
// ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
// Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
// auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
// (Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
//nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
// die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
// bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
// Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
// sich das "folgst" automatisch in "folgt" ändern
// Es kann folgenden Satz geben: "I like hurting you." "hurting you": 
// Gerundium -> "Ich mag |dich| zu verletzen."
//                      |Sie |
//                      |euch|
std::vector<CStringVector> TranslateRelativeClause(RelativeClause & 
	relativeClause,BYTE bArticle,//EnglishView * pEV,
  std::vector<IntPair> & rvecintpair,int & nStartID)
{
	BYTE personIndex=0,bTemp=0;
	GermanVerb * gv=NULL,* gvInfinitive=NULL;
	std::vector<CStringVector> vv;
	Predicate predicate;
	predicate=relativeClause.m_Predicate;
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateRelativeClause(RelativeClause & \
relativeClause,BYTE bArticle) - ANFANG\n");
	/*if(relativeClause.bNegation)
		TRACE("relativeClause.bNegation\n");
	else
		TRACE("!relativeClause.bNegation\n");*/
	if(predicate.m_bActiveClause)
		printf("predicate.m_bActiveClause\n");
	else
		printf("!predicate.m_bActiveClause\n");
	printf("predicate.m_bTense:%u personIndex: %u\n",predicate.m_bTense,personIndex);
	#endif
	//BYTE satzbau=SPO;
	/*for(int i=0;i<subSentence.m_adverbs.size();i++)
	{
		if(subSentence.m_adverbs.at(i).type=='2')
			satzbau=PSO;
	}*/
	if(bArticle==0)
	{
		CStringVector sv;
		sv.push_back(_T("den"));
		vv.push_back(sv);
	}
	if(bArticle==1 || bArticle==3)
	{
		CStringVector sv;
		sv.push_back(_T("die"));
		vv.push_back(sv);
	}
	if(bArticle==2)
	{
		CStringVector sv;
		sv.push_back(_T("das"));
		vv.push_back(sv);
	}
	Append(vv,TranslateSubject(relativeClause.m_Subject,FALSE,personIndex,
		bTemp,//pEV,
    rvecintpair,nStartID));
	if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
	{
		#ifdef _DEBUG
			printf("predicate.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated\n");
		#endif
		if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
			m_pWordNodeNext)
		{
			gv=dynamic_cast<GermanVerb*>(predicate.
			m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
			m_pWordNodeNext->m_pWord);
		#ifdef _DEBUG
			printf("gv->m_strWords[0]: %s\n",gv->m_strWords[0]);
		#endif
		}
	}
	if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
	{
#ifdef _DEBUG
		printf("subSentence.m_pWordNodeWhereTheGermanTranslationIsABendVerb\n");
#endif
		if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
			m_pWordNodeNext)
			if(!gv)
			{
				gv=dynamic_cast<GermanVerb*>(predicate.
					m_pWordNodeWhereTheGermanTranslationIsABendVerb->
					m_pWordNodeNext->m_pWord);
		#ifdef _DEBUG
				printf("gv->m_strWords[0]: %s\n",gv->m_strWords[0]);
		#endif
			}
			else
			{
				gvInfinitive=dynamic_cast<GermanVerb*>(predicate.
					m_pWordNodeWhereTheGermanTranslationIsABendVerb->
					m_pWordNodeNext->m_pWord);
		#ifdef _DEBUG
				printf("gvInfinitive->m_strWords[0]: %s\n",gvInfinitive->m_strWords[0]);
		#endif
			}
	}
	//wenn nur ein Aufzählungselement im Subjekt vorhanden ist und in 
	//diesem Element ein Personalpronom der 2. Person Singular enthalten
	//ist
	if(relativeClause.m_Subject.m_vecEnumerationElement.size()==1 &&
		relativeClause.m_Subject.m_vecEnumerationElement.at(0).
			m_personalPronoun==2)
	{
		BYTE bReflexive=0;
		CString str;
		CStringVector svGermanVerb;
		IntPair intPair;
		intPair.m_n1=nStartID;
		intPair.m_n2=nStartID+1;
		//pEV->m_vecIntPair.push_back(intPair);
    rvecintpair.push_back(intPair);
		nStartID+=2;
		if(gv)
		{
			//wenn KEIN reflexives deutsches Verb
			//zum Beispiel: "I can be used."
			if(gv->m_bReflexive)
				bReflexive=1;
		}
		if(gvInfinitive)
			//wenn KEIN reflexives deutsches Verb
			//zum Beispiel: "I still need a friend."
			//zum Beispiel: "I am still used."
			if(gvInfinitive->m_bReflexive)
				bReflexive=1;
		if(predicate.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
		{
			CStringVector sv;
			GermanVerb * gvWill=GetGermanWordOfEnglishVerb(_T("will"));
			str+=gvWill->m_strWords[2];
			if(bReflexive)
				str=_T("dich ");
			sv.push_back(str);
			str=gvWill->m_strWords[5];
			if(bReflexive)
				str=_T("euch ");
			sv.push_back(str);
			str=gvWill->m_strWords[6];
			if(bReflexive)
				str=_T("sich ");
			sv.push_back(str);
			str=gvWill->m_strWords[3];
			if(bReflexive)
				str=_T("sich ");
			sv.push_back(str);
			vv.push_back(sv);
		}
		if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
			predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
			predicate.m_bTense==SIMPLE_PAST || // I began.
			predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
			predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
			predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
			predicate.m_bTense==PRESENT_PERFECT) // I have begun.
		{
#ifdef _DEBUG
			printf("subSentence.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PAST || \
predicate.m_bTense==PAST_PROGRESSIVE \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
predicate.m_bTense==PRESENT_PERFECT\n");
#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(gv!=NULL)
			{
				CStringVector svSubjectAndVerb;
				//Wenn die Zeitform "Präsens", "fortschreitendes Präsens", 
				//"fortschreitender Perfekt" oder "Präsens oder Präteritum" ist.
				if(predicate.m_bTense==SIMPLE_PRESENT || 
					predicate.m_bTense==PRESENT_PROGRESSIVE ||
					predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
					predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
				{
					GermanVerb * gvBend;
					//wenn Aktivsatz
					if(predicate.m_bActiveClause)
					{
#ifdef _DEBUG
						printf("predicate.m_bActiveClause\n");
#endif
						gvBend=gv;
					}
					else
					{
						if(gvInfinitive) // Beispiel: 'can be used'
							gvBend=gv;
						else // Beispiel: 'is used'
							gvBend=dynamic_cast<GermanVerb*>(
								GetGermanWordOfEnglishVerb(_T("will")));
					}
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					//String-Vektor aus der 2. Person Singular ("du") Präsens bilden
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[2]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn du dich wäschst."
					if(bReflexive)
						str=_T("dich ");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
					//interpretiert werden kann
					// Prädikat (Präsens)
					if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
					{
						//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element.
						svGermanVerb=GetCStringVector2(gvBend->m_strWords[8]);
						//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst 
						//das Reflexiv-Pronom, dann die gebeugte Verbform. 
						//Beispiel: "Wenn du dich wäschst."
						if(bReflexive)
							str=_T("dich ");
						str+=svGermanVerb.at(0);
						svSubjectAndVerb.push_back(str);
						//Zeichenkette leeren
						str.Empty();
						// Prädikat (Präteritum)
					}
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[5]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn du dich wäschst."
					if(bReflexive)
						str=_T("euch ");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
					//interpretiert werden kann
					// Prädikat (Präsens)
					if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
					{
						//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element.
						svGermanVerb=GetCStringVector2(gvBend->m_strWords[11]);
						//bei dem Satzbau "SOP" steht zuerst das Reflexiv-Pronom, dann
						//die gebeugte Verbform. Beispiel: "Wenn du dich wäschst."
						if(bReflexive)
							str=_T("euch ");
						str+=svGermanVerb.at(0);
						svSubjectAndVerb.push_back(str);
						//Zeichenkette leeren
						str.Empty();
						// Prädikat (Präteritum)
					}
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[6]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn du dich wäschst."
					if(bReflexive)
						str=_T("sich ");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
					//interpretiert werden kann
					// Prädikat (Präsens)
					if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
					{
						//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element.
						svGermanVerb=GetCStringVector2(gvBend->m_strWords[12]);
						//bei dem Satzbau "SOP" steht zuerst das Reflexiv-Pronom, dann
						//die gebeugte Verbform. Beispiel: "Wenn du dich wäschst."
						if(bReflexive)
							str=_T("sich ");
						str+=svGermanVerb.at(0);
						svSubjectAndVerb.push_back(str);
						//Zeichenkette leeren
						str.Empty();
						// Prädikat (Präteritum)
					}
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[3]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn du dich wäschst."
					if(bReflexive)
						str=_T("sich ");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
					//interpretiert werden kann
					// Prädikat (Präsens)
					if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
					{
						//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element.
						svGermanVerb=GetCStringVector2(gvBend->m_strWords[9]);
						//bei dem Satzbau "SOP" steht zuerst das Reflexiv-Pronom, dann
						//die gebeugte Verbform. Beispiel: "Wenn du dich wäschst."
						if(bReflexive)
							str=_T("sich ");
						str+=svGermanVerb.at(0);
						svSubjectAndVerb.push_back(str);
						//Zeichenkette leeren
						str.Empty();
						// Prädikat (Präteritum)
					}
						/*//wenn ein Adverb im Prädikat enthalten ist
						//zum Beispiel: "I still need a friend." Adverb: "still"
						if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
						{
							GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
								m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
								m_pWordNodeNext->m_pWord);
							vv.push_back(svVerb);
							svVerb.clear();
							svVerb.push_back(ga->m_strWord);
						}*/
				//Wenn die Zeitform "Präsens", "fortschreitendes Präsens", 
				//"fortschreitender Perfekt" oder "Präsens oder Präteritum" ist.
				}
				//wenn die Zeitform "Präteritum" oder "fortschreitendes Präteritum"
				//ist
				//Präteritum: "I used water."
				//fortschreitendes Präteritum: "I was using water."
				if(predicate.m_bTense==SIMPLE_PAST || predicate.m_bTense==
					PAST_PROGRESSIVE)
				{
					GermanVerb * gvBend;
					//wenn Aktivsatz Beispiel: "I used water."
					if(predicate.m_bActiveClause)
					{
#ifdef _DEBUG
						printf("predicate.m_bActiveClause\n");
#endif
						gvBend=gv;
					}
					else
					{
						if(gvInfinitive) // Beispiel: 'can be used'
							gvBend=gv;
						else // Beispiel: 'is used'
							gvBend=dynamic_cast<GermanVerb*>(
								GetGermanWordOfEnglishVerb(_T("will")));
					}
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[8]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn du dich wuschst."
					if(bReflexive)
						str=_T("dich ");
					str=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[11]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn ihr euch wuscht."
					if(bReflexive)
						str+=_T("euch ");
					str=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[12]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn Sie sich wuschen."
					if(bReflexive)
						str=_T("sich ");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[9]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn man sich wusch."
					if(bReflexive)
						str=_T("sich ");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
				//if(predicate.m_bTense==SIMPLE_PAST || predicate.m_bTense==
				//	PAST_PROGRESSIVE)
				}
				//wenn die Zeitform "Perfekt" ist und der Relativsatz ein
				//Aktivsatz ist
				//Beispiel: you have used. -> den/die/das du benutzt hast.
				if(predicate.m_bTense==PRESENT_PERFECT)
				{
					GermanVerb * gvBend;
					//wenn Aktivsatz. Beispiel: "you have used."
					if(predicate.m_bActiveClause)
					{
#ifdef _DEBUG
						printf("predicate.m_bActiveClause\n");
#endif
						gvBend=gv;
					}
					//wenn Passivsatz. Beispiel: "You have been used."
					else
					{
						if(gvInfinitive) // Beispiel: 'You can have been used'
							gvBend=gv;
						else // Beispiel: 'is used'
							gvBend=dynamic_cast<GermanVerb*>(
								GetGermanWordOfEnglishVerb(_T("be")));
					}
					//Partizip Perfekt, zum Beispiel: "benutzt"
					svSubjectAndVerb.push_back(gvBend->m_strWords[13]);
					vv.push_back(svSubjectAndVerb);
					svSubjectAndVerb.clear();
					gvBend=dynamic_cast<GermanVerb*>(
						GetGermanWordOfEnglishVerb(_T("have")));
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[2]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn du dich gewaschen hast."
					if(bReflexive)
						str=_T("dich ");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[5]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn ihr euch gewaschen habt."
					if(bReflexive)
						str=_T("euch ");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[6]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn Sie sich gewaschen haben."
					if(bReflexive)
						str=_T("sich ");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
					//Zeichenkette leeren
					str.Empty();
					//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
					//2 Elementen. Sonst hat der Vektor nur 1 Element.
					svGermanVerb=GetCStringVector2(gvBend->m_strWords[3]);
					//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
					//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
					//"Wenn man sich gewaschen hat."
					if(bReflexive)
						str=_T(" sich");
					str+=svGermanVerb.at(0);
					svSubjectAndVerb.push_back(str);
				}//if(predicate.m_bTense==PRESENT_PERFECT)
				// Prädikat (Präteritum)
				vv.push_back(svSubjectAndVerb);
				// gv ist NULL, wenn der 'UnterSatz' ein Gerundium ist
				// deshalb vorher gv prüfen, bevor gv->m_bReflexive
				// abgefragt wird, sonst gibt es einen Absturz
				if(gv)
				{
					//wenn KEIN reflexives deutsches Verb
					//zum Beispiel: "I can be used."
					if(!gv->m_bReflexive)
					{
						//wenn ein Adverb im Prädikat enthalten ist
						//zum Beispiel: "I can still be used." Adverb: "still"
						if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
						{
							GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
								m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
								m_pWordNodeNext->m_pWord);
							svSubjectAndVerb.clear();
							svSubjectAndVerb.push_back(ga->m_strWord);
							vv.push_back(svSubjectAndVerb);
						}
					}
				}
				if(gvInfinitive)
					//wenn KEIN reflexives deutsches Verb
					//zum Beispiel: "I still need a friend."
					//zum Beispiel: "I am still used."
					if(!gvInfinitive->m_bReflexive)
					{
						//wenn ein Adverb im Prädikat enthalten ist
						//zum Beispiel: "I still need a friend." Adverb: "still"
						//zum Beispiel: "I am still used." Adverb: "still"
						//bei "The world still turns." muss das Adverb allerdings
						//erst hinter dem "sich" stehen. Also: "Die Welt dreht sich
						//immer noch."
						if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
						{
							GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
								m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
								m_pWordNodeNext->m_pWord);
							svSubjectAndVerb.clear();
							svSubjectAndVerb.push_back(ga->m_strWord);
							vv.push_back(svSubjectAndVerb);
						}
					}
			}//if(gv!=NULL)
		}
	}
	//wenn nicht: wenn nur ein Aufzählungselement im Subjekt vorhanden ist 
	//und in diesem Element ein Personalpronom der 2. Person Singular 
	//enthalten ist
	else
	{
	if(predicate.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
	{
		CStringVector sv;
		GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
		sv.push_back(gv->m_strWords[relativeClause.m_Subject.m_personIndex]);
		vv.push_back(sv);
	}
	personIndex++;
	if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
		predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
		predicate.m_bTense==SIMPLE_PAST || // I began.
		predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
		predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
		predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
		predicate.m_bTense==PRESENT_PERFECT) // I have begun.
	{
		CStringVector svVerb,svGermanVerb;
		#ifdef _DEBUG
		printf("predicate.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PAST || \
predicate.m_bTense==PAST_PROGRESSIVE || \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST || \
predicate.m_bTense==PRESENT_PERFECT\n");
	#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
		if(gv!=NULL)
		{
			CStringVector svVerb;
			if(predicate.m_bTense==SIMPLE_PRESENT || 
				predicate.m_bTense==PRESENT_PROGRESSIVE ||
				predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
				predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
			{
		#ifdef _DEBUG
				printf("predicate.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST\n");
	#endif
				if(predicate.m_bActiveClause)
				{
	#ifdef _DEBUG
					printf("predicate.m_bActiveClause\n");
	#endif
					svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex]);
	#ifdef _DEBUG
					printf("svGermanVerb.at(0): %s\n",svGermanVerb.at(0));
	#endif
					svVerb.push_back(svGermanVerb.at(0));
					// Prädikat (Präsens)
					if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
					{
						svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex+6]);
						svVerb.push_back(svGermanVerb.at(0));
						// Prädikat (Präteritum)
					}
				}
				else
				{
	#ifdef _DEBUG
					printf("!predicate.m_bActiveClause\n");
	#endif
					if(gvInfinitive) // Beispiel: 'can be used'
					{ 
						svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex]);
						svVerb.push_back(svGermanVerb.at(0));
						// Prädikat (Präsens)
						if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex+6]);
							svVerb.push_back(svGermanVerb.at(0));
							// Prädikat (Präteritum)
						}
					}
					else // Beispiel: 'is used'
					{
						GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
						svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex]);
						svVerb.push_back(svGermanVerb.at(0));
						// Prädikat (Präsens)
						if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex+6]);
							svVerb.push_back(svGermanVerb.at(0));
							// Prädikat (Präteritum)
						}
					}
				}
				vv.push_back(svVerb);
			}
			if(predicate.m_bTense==SIMPLE_PAST || predicate.m_bTense==PAST_PROGRESSIVE)
			{
	#ifdef _DEBUG
				printf("predicate.m_bTense==SIMPLE_PAST || \
predicate.m_bTense==PAST_PROGRESSIVE\n");
	#endif
				if(predicate.m_bActiveClause) // Beispiel: I used water.
				{
					svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex+6]);
					svVerb.push_back(svGermanVerb.at(0));
				}
				else
				{
					if(gvInfinitive) // Beispiel: I could be used. / 
					{ // I could be being used.
						svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex+6]);
						svVerb.push_back(svGermanVerb.at(0));
					}
					else // Beispiel: I was used. / I was being used.
					{
						GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
						svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex+6]);
						svVerb.push_back(svGermanVerb.at(0));
					}
				}
				vv.push_back(svVerb);
			}
			if(predicate.m_bTense==PRESENT_PERFECT && !predicate.m_bActiveClause)
			{ // Beispiel: I have been used. -> Ich bin benutzt worden.
	#ifdef _DEBUG
				printf("predicate.m_bTense==PRESENT_PERFECT && \
!predicate.m_bActiveClause\n");
	#endif
				GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
				svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex]);
				svVerb.push_back(svGermanVerb.at(0));
				vv.push_back(svVerb);
				// finite Form von 'sein' anhängen
			}
			if(predicate.m_bTense==PRESENT_PERFECT && predicate.m_bActiveClause)
			{ // Beispiel: I have been used. -> Ich bin benutzt worden.
				GermanVerb * gvTmp=NULL;
	#ifdef _DEBUG
				printf("predicate.m_bTense==PRESENT_PERFECT && \
predicate.m_bActiveClause\n");
	#endif
				svVerb.push_back(gv->m_strWords[13]);
				vv.push_back(svVerb);
				svVerb.clear();
				if(gv->m_bBe)
					gvTmp=GetGermanWordOfEnglishVerb(_T("be"));
				else
					gvTmp=GetGermanWordOfEnglishVerb(_T("have"));
				svVerb.push_back(gvTmp->m_strWords[personIndex]);
				vv.push_back(svVerb);
				// finite Form von 'sein' anhängen
			}
			// Prädikat (Präteritum)
			//vv.push_back(svVerb);
		}
	}
	}
	//vv.push_back(sv);
	if(predicate.m_bNegation)
	{
		CStringVector sv;
		sv.push_back(_T("nicht"));
		vv.push_back(sv);
	}
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateRelativeClause(RelativeClause & \
relativeClause,BYTE bArticle) - return vv\n");
	#endif
	return vv;
}

std::vector<CStringVector> TranslateSentence2(const Sentence & sentence,
	//EnglishView * pEV,
  std::vector<IntPair> & rvecintpair,int & nStartID)
{ // liefert einen Vector, der aus mehreren Vekoren, die CStrings beinhalten, besteht
	//erstes Token im Satz, wird an angerufene Funktionen, die den Text 
	//übersetzen, als Parameter übergeben. Wenn das erste Token im Satz 
	//KEIN Eigenname ist, dann wird dessen erstes Zeichen in einen Gross-
	//buchstaben umgewandelt.
	BYTE bFirstToken=FALSE;
	DWORD dwRun=0;
	TRACE("CStringVector TranslateSentence2 ANFANG\n");
  LOG("23.05.2008 11.16.38\n");//TC=TranslateClause Begin
	#ifdef _DEBUG
	printf("CStringVector TranslateSentence2 ANFANG\n");
	printf("sentence.m_subSentences.size(): %d\n",sentence.m_vecClause.size());
	#endif
	std::vector<CStringVector> vv;
	//sentence.m_satzbau=SPO;
	for(DWORD i=0;i<sentence.m_vecEnglishAdverb.size();i++)
	{
		if(sentence.m_vecEnglishAdverb.at(i).m_bType==2)
		{
			CStringVector sv;
			GermanAdverb * ga=GetGermanWordOfEnglishAdverb(sentence.
				m_vecEnglishAdverb.at(i).m_strWord);
			sv.push_back(ga->m_strWord);
			MakeFirstLetterUpper(sv.at(0));
			vv.push_back(sv);
			//sentence.m_bWordOrder=PSO;
			bFirstToken=FALSE;
		}
	}

	//if(sentence.m_sentenceElements.size()>0)
	//	goto sentenceelements;
	for(i=0;i<sentence.m_vecClause.size();i++)
	{
		TRACE("TranslateSentence ANFANG size\n");
    LOG("23.05.2008 11.16.46 "<<i<<"\n");//TC=TranslateClause
		//sentence.m_subSentences.at(i).satzbau=sentence.m_satzbau;
		std::vector<CStringVector> vvClause=TranslateClause2(
			sentence.m_vecClause.at(i),bFirstToken,//pEV,
      rvecintpair,nStartID);
		Append(vv,vvClause);
    LOG("23.05.2008 11.16.56 "<<i<<"\n");//ATC=After TranslateClause
		if(sentence.m_vecConjunction.size()>0)
		{
			if(i<sentence.m_vecConjunction.size())
			{
				if(sentence.m_vecConjunction.at(i)==_T("that"))
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
					sv.clear();
					sv.push_back(_T("dass"));
					vv.push_back(sv);
				}
			}
		}
		else
		{
			if(i<sentence.m_vecClause.size()-1)
			{
				CStringVector sv;
				sv.push_back(",");
				vv.push_back(sv);
				//sv.clear();
				//sv.push_back("dass");
				//vv.push_back(CObjectDerivedWithCStringMember(","));
				//vv.push_back(sv);//CObjectDerivedWithCStringMember("dass"));
			}
		}
	}
  LOGN("26.05.2008 8.58.15");
	#ifdef _DEBUG
	printf("nach TranslateClause2()\n");
	#endif
	CStringVector sv;
	sv.push_back(".");
	vv.push_back(sv);//CObjectDerivedWithCStringMember("."));
	//MakeFirstLetterUpper(germanSentence.at(0));
	//sv

	//MakeFirstLetterUpper(vv.at(0).at(0));
	//TRACE("((CStringVector)vv.at(0)).at(0): %s\n",((CStringVector)vv.at(0)).at(0));
	#ifdef _DEBUG
	printf("nach TRACE\n");
	#endif
	//sv.clear();
	//sv.push_back("ich");
	//MakeFirstLetterUpper(sv.at(0));
	TRACE("sv.at(0): %s\n",sv.at(0));
	/*for(DWORD dwRun=0;dwRun<vv.at(0).size();dwRun++)
	{ // die ersten Buchstaben von allen CStrings, die der ERSTE CStringVector 
		// an Position dwRun des Vektors vv hat großmachen (wegen des Satzanfanges)
		MakeFirstLetterUpper(vv.at(0).at(dwRun));
	}*/
	for(;dwRun<vv.at(0).size();++dwRun)
		MakeFirstLetterUpper(vv.at(0).at(dwRun));
	#ifdef _DEBUG
	for(dwRun=0;dwRun<vv.size();dwRun++)
	{
		//die einzelnen Token der deutschen Übersetzung ausgeben
		printf("vv.at(%u).at(0): %s\n",dwRun,vv.at(dwRun).at(0));
	}
	TRACE("CStringVector TranslateSentence2 ENDE\n");
	printf("CStringVector TranslateSentence2 ENDE\n");
	#endif
  LOGN("26.05.2008 8.59.41");
	return vv;
}

std::vector<CStringVector> TranslateSimilarity(const Similarity & similarity,
	int & nStartID)
{
	std::vector<CStringVector> vecvecStr;
	CStringVector vecStr;
	GermanAdjective *ga=dynamic_cast<GermanAdjective *>(similarity.
		m_pWordNodeWhereAnEnglishPositiveIsEncapsulated->m_pWordNodeNext->
		m_pWord);
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateSimilarity(const Similarity & \
similarity) ANFANG\n");
#endif
	if(ga)
	{
		BYTE bTmp=0;
		std::vector<CStringVector> vecvecStrObject;
		vecStr.push_back(_T("genauso"));
		vecvecStr.push_back(vecStr);
		vecStr.clear();
		vecStr.push_back(ga->m_strPositiv);
#ifdef _DEBUG
		printf("ga->m_strPositiv: %s\n",ga->m_strPositiv);
#endif
		vecvecStr.push_back(vecStr);
		vecStr.clear();
		vecStr.push_back(_T("wie"));
		vecvecStr.push_back(vecStr);
    std::vector<IntPair> vecintpair;
		vecvecStrObject=TranslateObject2(*similarity.m_pObject,0,0,bTmp,//NULL,
      vecintpair,
			nStartID);
		Append(vecvecStr,vecvecStrObject);
	}
#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateSimilarity(const Similarity & \
similarity) return vecvecStr\n");
#endif
	return vecvecStr;
}

CStringVector TranslateSubject(Subject subject)
{
	TRACE("CStringVector TranslateSubject(Subject subject) ANFANG\n");
	CStringVector retVec;
	BOOL bFirst=TRUE;
	for(DWORD i=0;i<subject.m_vecEnumerationElement.size();i++)
	{
//		if(sv.at(i)=="," || sv.at(i)=="and")
//		{
		if(!bFirst)
			if(i!=subject.m_vecEnumerationElement.size()-1)
				retVec.push_back(",");
				//Append(retVec,",");
			else
				retVec.push_back("und");
//				Append(retVec,"and");

		CStringVector cstrvec=TranslateEnumerationElement(subject.
			m_vecEnumerationElement.at(i));
		Append(retVec,cstrvec);

		bFirst=FALSE;
//		}
	}
	TRACE("CStringVector TranslateSubject(Subject subject) ENDE\n");
	return retVec;
}

//TranslateSubject wird von TranslateClause2 aufgerufen
//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
// m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
// zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
// (Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
// ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
// Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
// auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
// (Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
// pEV wird aber nichts in std::vector<CStringVector> TranslateSubject(Subject 
// & subject,BOOL bTranslateAsObject,BYTE & personIndex,BYTE & 
// bFirstTokenInSentence,EnglishView * pEV,int & nStartID); hinzugefügt, 
// sondern dient zur Weiterleitung an die Funktion TranslateEnumerationElement.
//nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
// die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
// bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
// Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
// sich das "folgst" automatisch in "folgt" ändern
std::vector<CStringVector> TranslateSubject(
  Subject & subject,
	BOOL bTranslateAsObject,
  BYTE & personIndex,
  BYTE & bFirstTokenInSentence,
	//EnglishView * pEV,
  std::vector<IntPair> & rvecintpair,
  int & nStartID
  )
{
	BYTE personIndexEE=0;
	personIndex=255;
	std::vector<CStringVector> vv;
	BOOL bFirst=TRUE;
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateSubject(Subject & subject,\
BOOL bTranslateAsObject,BYTE & personIndex,BYTE & bFirstTokenInSentence) ANFANG\n");
	printf("subject.m_enumerationElements.size(): %d\n",subject.
		m_vecEnumerationElement.size());
	printf("bFirstTokenInSentence: %u\n",bFirstTokenInSentence);
	#endif
	for(DWORD i=0; i < subject.m_vecEnumerationElement.size(); i++)
	{
		if(!bFirst)
			if(i!=subject.m_vecEnumerationElement.size()-1)
			{
				CStringVector sv;
				sv.push_back(_T(","));
				vv.push_back(sv);
			}
			else
			{
				CStringVector sv;
				sv.push_back(_T("und"));
				vv.push_back(sv);
			}
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
		std::vector<CStringVector> vvEE=TranslateEnumerationElement(
			subject.m_vecEnumerationElement.at(i),bTranslateAsObject,personIndexEE,
			bFirstTokenInSentence,//pEV,
      rvecintpair,nStartID);
		//wenn personIndex im Anfangszustand ist
		if(personIndex==255)
			personIndex=personIndexEE;
		//personIndex wurde bereits mindestens ein Wert zugewiesen
		else
		{
			//wenn "ich" , dann "wir"
			if(personIndex == 0)
				personIndex=3;
      //TODO if -> switch()
			//wenn "du", 
			if(personIndex==1)
			{
				//und wenn "ich", dann "wir"
				if(personIndexEE==0)
					personIndex=3;
				//und wenn "er/sie/es", dann "ihr"
				if(personIndexEE==2)
					personIndex=4;
				//und wenn "wir", dann "wir"
				if(personIndexEE==3)
					personIndex=3;
				//und wenn "ihr" oder "sie", dann "ihr"
				if(personIndexEE==4 || personIndexEE==5)
					personIndex=4;
			}
			//wenn "er/sie/es", 
			if(personIndex==2)
			{
				//und wenn "ich", dann "wir"
				if(personIndexEE==0)
					personIndex=3;
				//und wenn "du", dann "ihr"
				if(personIndexEE==1)
					personIndex=4;
				//und wenn "er/sie/es", dann "sie"
				if(personIndexEE==2)
					personIndex=5;
				//und wenn "wir", dann "wir"
				if(personIndexEE==3)
					personIndex=3;
				//und wenn "ihr", dann "ihr"
				if(personIndexEE==4)
					personIndex=4;
				//und wenn sie", dann "sie"
				if(personIndexEE==5)
					personIndex=5;
			}
			//wenn "wir",
			if(personIndex==3)
				//dann immer "wir"
				personIndex=3;
			//wenn "ihr", 
			if(personIndex==4)
			{
				//und wenn "ich" oder "wir", dann "wir"
				if(personIndexEE==0 || personIndexEE==3)
					personIndex=3;
				//und wenn "du" oder "er/sie/es" oder "ihr" oder  "sie", dann "ihr"
				if(personIndexEE==1 || personIndexEE==2)
					personIndex=4;
			}
			//wenn "sie" (Plural), 
			if(personIndex==5)
			{
				//und wenn "ich" oder "wir", dann "wir"
				if(personIndexEE==0 || personIndexEE==3)
					personIndex=3;
				//und wenn "du" oder "ihr", dann "ihr"
				if(personIndexEE==1)
					personIndex=4;
				//und wenn "er/sie/es", dann "sie"
				if(personIndexEE==2)
					personIndex=5;
			}
		}
		Append(vv,vvEE);
		bFirst=FALSE;
	}
	TRACE("CObjectVector TranslateSubject(Subject subject) ENDE\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateSubject(Subject & subject\
,BOOL bTranslateAsObject,bFirstTokenInSentence) ENDE\n");
	#endif
	return vv;
}

//TranslateSubject2 wird von TranslateClause2 wird von TranslateSentence2 
//aufgerufen
/*std::vector<CStringVector> TranslateSubject2(const Subject & subject,
	BYTE & bFirstTokenInSentence)
{
	BYTE personIndexEE=0;
	TRACE("std::vector<CStringVector> TranslateSubject2(Subject subject) ANFANG\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateSubject2(Subject subject) ANFANG\n");
	#endif
	std::vector<CStringVector> vv;
	BOOL bFirst=TRUE;
	for(int i=0;i<subject.m_vecEnumerationElement.size();i++)
	{
		if(!bFirst)
			if(i!=subject.m_vecEnumerationElement.size()-1)
			{
				CStringVector sv;
				sv.push_back(_T(","));
				vv.push_back(sv);
			}
			else
			{
				CStringVector sv;
				sv.push_back(_T("und"));
				vv.push_back(sv);
			}
		std::vector<CStringVector> vvEE=TranslateEnumerationElement(
			subject.m_vecEnumerationElement.at(i),FALSE,personIndexEE,
			bFirstTokenInSentence,nStartID);
		Append(vv,vvEE);
		bFirst=FALSE;
	}
	TRACE("CObjectVector TranslateSubject(Subject subject) ENDE\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateSubject2(Subject subject) ENDE\n");
	#endif
	return vv;
}*/

/*std::vector<CStringVector> TranslateClause(Clause & subSentence)
{
	TRACE("CStringVector TranslateClause ANFANG\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateClause2(Clause & \
subSentence) - ANFANG\n");
	if(subSentence.m_bNegation)
	{
		TRACE("subSentence.m_bNegation\n");
		printf("subSentence.m_bNegation\n");
	}
	else
	{
		TRACE("!subSentence.m_bNegation\n");
		printf("!subSentence.m_bNegation\n");
	}
	#endif
	std::vector<CStringVector> vv;
	//BYTE satzbau=SPO;
	#ifdef _DEBUG
	if(subSentence.m_bActiveClause)
		printf("subSentence.m_bActiveClause\n");
	else
		printf("!subSentence.m_bActiveClause\n");
	printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
	printf("subSentence.m_pSubject->m_personIndex: %u\n",subSentence.
		m_pSubject->m_personIndex); 
	#endif
	GermanVerb * gv=NULL;
	GermanVerb * gvInfinitive=NULL;
	if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated)
	{
		if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
			m_pWordNodeNext)
		{
			gv=dynamic_cast<GermanVerb*>(subSentence.
				m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWordNodeNext->
				m_pWord);
			#ifdef _DEBUG
			printf("gv->m_strWords[0]: %s\n",gv->m_strWords[0]);
			#endif
		}
	}
	if(subSentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated)
		if(subSentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext)
			if(!gv)
				gv=dynamic_cast<GermanVerb*>(subSentence.
				m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext->
				m_pWord);
			else
				gvInfinitive=dynamic_cast<GermanVerb*>(subSentence.
				m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext->
				m_pWord);
	#ifdef _DEBUG
	if(gv)
		printf("gv\n");
	else
		printf("!gv\n");
	#endif
	if(subSentence.m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
	{
		for(DWORD dwRun=0;dwRun<subSentence.
			m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun++)
		{
			if(subSentence.m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.
				at(dwRun)->m_pWordNodeNext)
			{
				GermanConjunction * gc=dynamic_cast<GermanConjunction*>(subSentence.
					m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.at(dwRun)->
					m_pWordNodeNext->m_pWord);
				if(gc)
				{
					CStringVector sv;
					sv.push_back(gc->m_strWord);
					vv.push_back(sv);
				}
			}
		}
	}
	#ifdef _DEBUG
	else
		printf("!subSentence.m_pVocNodeWhereAnEnglishConjunctionIsEncapsulated\n");
	printf("subSentence.m_bWordOrder: %u\n",subSentence.m_bWordOrder);
	printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
	#endif
	if(subSentence.m_bWordOrder==SPO) // Subjekt Prädikat Objekt
	{
		TRACE("satzbau==SPO\n");
		#ifdef _DEBUG
		printf("satzbau==SPO\n");
		#endif
		if(subSentence.m_pGerund)
		{
			#ifdef _DEBUG
			printf("subSentence.m_pGerund\n");
			#endif
			BYTE b=0;
			Append(vv,TranslateGerund(*subSentence.m_pGerund,b,nStartID));
		}
		if(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated &&
			subSentence.m_bAdverbOfTimeAtTheBeginning)
		{ // Umstandsbestimmung der Zeit
			GermanAdverb * ga=dynamic_cast<GermanAdverb *>(subSentence.
				m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			if(ga)
			{
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		else // wenn eine Umstandsbestimmung der Zeit am Anfang eines deutschen Satzes
		{ // steht, steht das gebeugte Verb direkt dahinter, und nicht zuerst
			// ein Subjekt Beispiel: "Today I am intelligent." ->
			// "Heute bin ich intelligent." gebeugtes Verb: "bin"
			if(!subSentence.m_bActiveClause && subSentence.m_pPersonalSubject) // wenn Passivsatz
			{ // und der Satz ein persönliches Subjekt enthält 
				// zum Bespiel: "I am given a car." -> Mir wird ein Auto gegeben
				#ifdef _DEBUG
				printf("!subSentence.m_bActiveClause && subSentence.m_pPersonalSubject\n");
				#endif
				// Beispiel: I am being given a real chance.
				//           S ------P------- ------PS-----  // PS=persönliches Subjekt
				//           Mir wird eine richtige Chance gegeben.
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateSubject(
					*subSentence.m_pSubject,TRUE,b,b,nStartID);
				Append(vv,vvObject); // Subjekt
			}
			else
			{
				BYTE b=0;
				std::vector<CStringVector> vvSubject=TranslateSubject2(
					*subSentence.m_pSubject,b);
				Append(vv,vvSubject); // Subjekt
			}
		}
		if(subSentence.m_pRelativeClause) // Relativsatz
		{
			if(subSentence.m_pRelativeClause->m_bRefersToSubject)
			{
				#ifdef _DEBUG
				printf("subSentence.m_rc->m_bRefersToSubject\n");
				#endif
				if(subSentence.m_pSubject->m_vecEnumerationElement.size()>0)
				{
					std::vector <GermanNoun> vecGermanNoun=
						GetGermanWordOfEnglishNoun2(subSentence.m_pSubject->
						m_vecEnumerationElement.at(0).m_pDan->m_Noun);
					if(vecGermanNoun.size()>0)
					{
						CStringVector sv;
						sv.push_back(_T(","));
						vv.push_back(sv);
						sv.clear();
						GermanNoun gn=vecGermanNoun.at(0);
						if(gn.m_bArticle=='1')
							sv.push_back(_T("den"));
						else if(gn.m_bArticle=='2')
							sv.push_back(_T("die"));
						else if(gn.m_bArticle=='3')
							sv.push_back(_T("das"));
						vv.push_back(sv);
					}
				}
				Append(vv,TranslateRelativeClause(*subSentence.m_pRelativeClause));
				CStringVector sv;
				sv.push_back(_T(","));
				vv.push_back(sv);
			}
		}
		#ifdef _DEBUG
		printf("subSentence.m_strPredicate: %s\n",subSentence.m_strPredicate);
		//TRACE("satzbau==SPO\n");
		printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
		#endif
		if(subSentence.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
			sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
			vv.push_back(sv);
		}
		CStringVector svGermanVerb;
		if(subSentence.m_bTense==SIMPLE_PRESENT || // I begin.
			subSentence.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
			subSentence.m_bTense==SIMPLE_PAST || // I began.
			subSentence.m_bTense==PAST_PROGRESSIVE || // I was beginning.
			subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
			subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
			subSentence.m_bTense==PRESENT_PERFECT) // I have begun.
		{
			#ifdef _DEBUG
			printf("subSentence.tense==SIMPLE_PRESENT || \
			subSentence.tense==PRESENT_PROGRESSIVE || \
			subSentence.tense==SIMPLE_PAST || \
			subSentence.tense==PAST_PROGRESSIVE \
			subSentence.tense==PRESENT_PERFECT_PROGRESSIVE \
			subSentence.tense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
			subSentence.tense==PRESENT_PERFECT\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(gv!=NULL)
			{
//				int index=0;
	//			if((index=subSentence.m_Subject.m_personIndex*subSentence.tense+1)<14)
		//		{
				#ifdef _DEBUG
				printf("subSentence.m_pSubject->m_personIndex: %u\n",subSentence.
					m_pSubject->m_personIndex);
				#endif
				if(subSentence.m_pSubject->m_vecEnumerationElement.size()==1)
				{
					//if(subSentence.m_Subject.m_enumerationElements.at(0).m_plainText!="");
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).
						m_vecStrPlainText.size()>0)
					{
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || 
							subSentence.m_bTense==PRESENT_PROGRESSIVE ||
							subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							if(subSentence.m_bActiveClause)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
								svVerb.push_back(svGermanVerb.at(0));
								// Prädikat (Präsens)
								if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präteritum)
								}
							}
							else
							{
								if(gvInfinitive) // Beispiel: 'can be used'
								{ 
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präsens)
									if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
								else // Beispiel: 'is used'
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präsens)
									if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
							}
						}
						if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
							PAST_PROGRESSIVE)
						{
							if(subSentence.m_bActiveClause) // Beispiel: I used water.
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex+6]);
								svVerb.push_back(svGermanVerb.at(0));
							}
							else
							{
								if(gvInfinitive) // Beispiel: I could be used. / 
								{ // I could be being used.
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
								else // Beispiel: I was used. / I was being used.
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
							}
						}
						if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
							m_bActiveClause)
						{ // Beispiel: I have been used. -> Ich bin benutzt worden.
							GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
							svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex]);
							svVerb.push_back(svGermanVerb.at(0));
							// finite Form von 'sein' anhängen
						}
						// Prädikat (Präteritum)
						vv.push_back(svVerb);
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pDan)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pDan\n");
						#endif
						std::vector <EnglishNoun> vecEN=GetEnglishNoun(subSentence.
							m_pSubject->m_vecEnumerationElement.at(0).m_pDan->m_Noun);
						if(vecEN.size()>0)
						{
							if(IsEnglishSingular(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pDan->m_Noun))
							{
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
													m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
													m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									// Prädikat (Präteritum)
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==1)
								{ // im Deutschen: Plural
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
							}
							if(IsEnglishPlural(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pDan->m_Noun))
							{ // im Englischen: Plural
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // Beispiel: I am used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==2)
								{
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
													m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
							}
						}
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pEnr)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pEnr\n");
						#endif
						std::vector <EnglishNoun> vecEN=GetEnglishNoun(subSentence.
							m_pSubject->m_vecEnumerationElement.at(0).m_pEnr->m_Noun);
						if(vecEN.size()>0)
						{
							#ifdef _DEBUG
							printf("vecEN.size()>0\n");
							printf("vecEN.at(0).m_bTranslationType: %u\n",vecEN.at(0).
								m_bTranslationType);
							#endif
							if(IsEnglishSingular(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pEnr->m_Noun))
							{
								#ifdef _DEBUG
								printf("IsEnglishSingular(subSentence.m_pSubject->\
m_vecEnumerationElement.at(0).m_pEnr->m_Noun)\n");
								printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
								#endif
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									#ifdef _DEBUG
									printf("vecEN.at(0).m_bTranslationType==0\n");
									#endif
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
													m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												 svGermanVerb=GetCStringVector2(gv->m_strWords[
													 subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==1)
								{
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
										svVerb.push_back(svGermanVerb.at(0)); // 'bin' anhängen
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
							}
							if(IsEnglishPlural(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pEnr->m_Noun))
							{ // Im Englischen: Plural
								#ifdef _DEBUG
								printf("IsEnglishPlural(subSentence.m_pSubject->\
m_vecEnumerationElement.at(0).enr->m_Noun)\n");
								#endif
								if(vecEN.at(0).m_bTranslationType==0)
								{ // im Deutschen: Plural
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==2)
								{ // im Deutschen: Singular
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
							}
						}
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pGerund)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.\
at(0).m_pGerund\n");
						#endif
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || 
							subSentence.m_bTense==PRESENT_PROGRESSIVE ||
							subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex]);
							svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex+6]);
								svVerb.push_back(svGermanVerb.at(0));
							}
								// Prädikat (Präteritum)
						}
						if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
							PAST_PROGRESSIVE)
						{
							svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex+6]);
							svVerb.push_back(svGermanVerb.at(0)); // Prädikat
						}
						vv.push_back(svVerb);
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_personalPronoun)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.\
at(0).m_personalPronoun\n");
						#endif
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || 
							subSentence.m_bTense==PRESENT_PROGRESSIVE ||
							subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							#ifdef _DEBUG
							printf("subSentence.m_bTense==SIMPLE_PRESENT || subSentence.\
m_bTense==PRESENT_PROGRESSIVE || subSentence.m_bTense==PRESENT_PERFECT_\
PROGRESSIVE || subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST\n");
							printf("gv->m_strWords[0]: %s\n",gv->m_strWords[0]);
							#endif
							if(subSentence.m_bActiveClause) // Aktivsatz
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // 1. Wort des Vektors anhängen
							}
							else // Passivsatz
							{
								BOOL bAuxiliaryVerb=FALSE;
								if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated)
									if(typeid(*subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)
										==typeid(EnglishAuxiliaryVerb) && subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
										m_pWordNodeNext)
									{
										bAuxiliaryVerb=TRUE;
										GermanVerb * gv=dynamic_cast<GermanVerb*>(subSentence.
											m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
										{ // persönliches Subjekt beinhaltet Beispiel: I can be given
											// CHANCES. -> Mir KÖNNEN Chancen gegeben werden. anstatt:
											// Ich KANN Chancen gegeben werden.
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pPersonalSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											// gebeugte From vom modalen Hilfsverb anhängen
										}
										else // zum Beispiel: I can be used. 
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
									}
								if(!bAuxiliaryVerb) // kein modales Hilfsverb
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
									{ // persönliches Subjekt beinhaltet Beispiel: I am given
										// CHANCES. -> Mir WERDEN Chancen gegeben. anstatt:
										// Ich WERDE Chancen gegeben.
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pPersonalSubject->m_personIndex]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										// gebeugte From von 'werden' anhängen
									}
									else // zum Beispiel: I can be used. 
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									}
								}
							}
							if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								if(subSentence.m_bActiveClause) // Aktivsatz
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pSubject->m_personIndex+6]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0));
								}
								else // Passivsatz
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
									{ // persönliches Subjekt beinhaltet Beispiel: I was given
										// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
										// Ich WURDE eine Chance gegeben.
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pPersonalSubject->m_personIndex+6]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										// gebeugte From von 'sein' anhängen
									}
									else
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex+6]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									}
								}
							}
								// Prädikat (Präteritum)
							vv.push_back(svVerb);
						}
						if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
							PAST_PROGRESSIVE)
						{ // Vergangenheit oder fortschreitende Vergangenheit
							if(subSentence.m_bActiveClause)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex+6]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							}
							else // Passivsatz
							{
								GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
								if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
								{ // persönliches Subjekt beinhaltet Beispiel: I was given
									// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
									// Ich WURDE eine Chance gegeben.
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pPersonalSubject->m_personIndex+6]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									// gebeugte From von 'sein' anhängen
								}
								else
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
								}
							}
							vv.push_back(svVerb);
						}
						if(subSentence.m_bTense==PRESENT_PERFECT)
						{
							if(subSentence.m_bActiveClause)
							{
								GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("have"));
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							}
							else // Passivsatz
							{
								if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
								{ // persönliches Subjekt beinhaltet Beispiel: I have been given
									// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
									// Ich BIN eine Chance gegeben worden.
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pPersonalSubject->m_personIndex]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									// gebeugte From von 'sein' anhängen
								}
								else
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
								}
							}
							vv.push_back(svVerb);
						}
					}
				}
				else
				{
					CStringVector svVerb;
					if(subSentence.m_bTense==SIMPLE_PRESENT || 
						subSentence.m_bTense==PRESENT_PROGRESSIVE ||
						subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
						subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
					{
						svVerb.push_back(gv->m_strWords[subSentence.m_pSubject->
							m_personIndex]); // Prädikat
						if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							svVerb.push_back(gv->m_strWords[subSentence.m_pSubject->
							m_personIndex+6]);
							// Prädikat (Präteritum)
					}
					if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
						PAST_PROGRESSIVE)
						svVerb.push_back(gv->m_strWords[subSentence.m_pSubject->
						m_personIndex+6]); // Prädikat
					vv.push_back(svVerb);
				}
			//	}
			}
			#ifdef _DEBUG
			else
			{
				TRACE("gv==NULL\n");
				printf("gv==NULL\n");
			}
			#endif
		}
		if(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated &&
			!subSentence.m_bAdverbOfTimeAtTheBeginning)
		{ // Umstandsbestimmung der Zeit
			GermanAdverb * ga=dynamic_cast<GermanAdverb *>(subSentence.
				m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			if(ga)
			{
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		if(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated &&
			subSentence.m_bAdverbOfTimeAtTheBeginning)
		{
			if(!subSentence.m_bActiveClause && subSentence.m_pPersonalSubject) // wenn Passivsatz
			{ // und der Satz ein persönliches Subjekt enthält 
				// zum Bespiel: "I am given a car." -> Mir wird ein Auto gegeben
				BYTE b=0;
				#ifdef _DEBUG
				printf("!subSentence.m_bActiveClause && subSentence.m_pPersonalSubject\n");
				#endif
				// Beispiel: I am being given a real chance.
				//           S ------P------- ------PS-----  // PS=persönliches Subjekt
				//           Mir wird eine richtige Chance gegeben.
				std::vector<CStringVector> vvObject=TranslateSubject(
					*subSentence.m_pSubject,TRUE,b,b,nStartID);
				Append(vv,vvObject); // Subjekt
			}
			else
			{
				BYTE b=0;
				std::vector<CStringVector> vvSubject=TranslateSubject2(
					*subSentence.m_pSubject,b);
				Append(vv,vvSubject); // Subjekt
			}
		}
		#ifdef _DEBUG
		printf("vor: subSentence.m_pByAgent\n");
		#endif
		if(subSentence.m_pByAgent)
		{
			Append(vv,TranslateByAgent(*subSentence.m_pByAgent,nStartID));
			//CStringVector sv;
			//sv.push_back("von");
			//vv.push_back(sv);
			
		}
		#ifdef _DEBUG
		printf("vor: subSentence.m_pPersonalSubject\n");
		#endif
		if(subSentence.m_pPersonalSubject)
		{
			BYTE b=0;
			Append(vv,TranslateSubject2(*subSentence.m_pPersonalSubject,b));
		}
		#ifdef _DEBUG
		printf("vor: gv->m_bReflexive\n");
		#endif
		if(gv) // gv ist NULL, wenn der 'UnterSatz' ein Gerundium ist
		{ // deshalb vorher gv prüfen, bevor gv->m_bReflexive
			// abgefragt wird, sonst gibt es einen Absturz
			if(gv->m_bReflexive)
			{ // wenn nur 1 Verb im Satz, dann ist gv das Prädikat
				// wenn 2 Verben im Satz (zum Beispiel: 'can' und 'hide',
				// dann ist auch gvInfinitive initialisiert
				CStringVector sv;
				if(subSentence.m_pSubject->m_personIndex==1) // I
					sv.push_back("mich");
				if(subSentence.m_pSubject->m_personIndex==2) // you
				{
					sv.push_back("dich");
					sv.push_back("euch");
					sv.push_back("sich");
				}
				if(subSentence.m_pSubject->m_personIndex==3) // he/she/it
					sv.push_back("sich");
				if(subSentence.m_pSubject->m_personIndex==4) // we
					sv.push_back("uns");
				if(subSentence.m_pSubject->m_personIndex==6) // they
					sv.push_back("sich");
				vv.push_back(sv);
			}
		}
		#ifdef _DEBUG
		printf("vor: gvInfinitive\n");
		#endif
		if(gvInfinitive)
			if(gvInfinitive->m_bReflexive)
			{
				CStringVector sv;
				if(subSentence.m_pSubject->m_personIndex==1) // I
					sv.push_back("mich");
				if(subSentence.m_pSubject->m_personIndex==2) // you
				{
					sv.push_back("dich");
					sv.push_back("euch");
					sv.push_back("sich");
				}
				if(subSentence.m_pSubject->m_personIndex==3) // he/she/it
					sv.push_back("sich");
				if(subSentence.m_pSubject->m_personIndex==4) // we
					sv.push_back("uns");
				if(subSentence.m_pSubject->m_personIndex==6) // they
					sv.push_back("sich");
				vv.push_back(sv);
			}
		if(gv)
		{
			if(gv->m_strPreposition!="")
			{
				CStringVector sv;
				sv.push_back(gv->m_strPreposition);
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanClause,germanObject); // Subjekt
		BOOL bNotInserted=FALSE;
		if(subSentence.m_pObjectFirst) // Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectFirst\n");
			printf("subSentence.m_pObjectFirst->m_enumerationElements.\
size(): %u\n",subSentence.m_pObjectFirst->m_vecEnumerationElement.size());
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(subSentence.m_bNegation && subSentence.m_pObjectFirst->
				m_vecEnumerationElement.at(0).m_pGerund)
			{
				bNotInserted=TRUE;
				CStringVector sv;
				sv.push_back(_T("nicht"));
				vv.push_back(sv);
			}
			BYTE Case=0;
			if(gvInfinitive)
				Case=gvInfinitive->m_bCase;
			else if(gv)
				Case=gv->m_bCase;
			if(Case==1 || Case==3) // wenn das Verb 1 Objekt im vierten Fall oder 
				// 2 Objekt verlangt
			{ // im dritten Fall übersetzen
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectFirst,1,subSentence.m_bNegation,b,nStartID);
				Append(vv,vvObject); // Objekt
			}
			else if(Case==2)
			{ // im vierten Fall übersetzen
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectFirst,2,subSentence.m_bNegation,b,nStartID);
				Append(vv,vvObject); // Objekt
			}
			else
			{ // mit keinem Fall übersetzen
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectFirst,0,subSentence.m_bNegation,b,nStartID);
				Append(vv,vvObject); // Objekt
			}
			bNotInserted=subSentence.m_bNegation;
		}
		#ifdef _DEBUG
		else
			printf("subSentence.m_Object.m_enumerationElements.size()<=0\n");
		printf("vor 2. Objekt\n");
		#endif
		if(subSentence.m_pObjectSecond) // 2. Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectSecond\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
				//printf("gv
			BYTE b=0;
			std::vector<CStringVector> vvObject=TranslateObject2(
				*subSentence.m_pObjectSecond,2,subSentence.m_bNegation,b,nStartID);
			Append(vv,vvObject); // Objekt
		}
		if(subSentence.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(subSentence.m_bNegation)
			{
				if(subSentence.m_pObjectFirst)
					if(!subSentence.m_pObjectFirst->m_vecEnumerationElement.at(0).m_pGerund 
						&& !bNotInserted)
					{ // wenn kein Gerundium
						CStringVector sv;
						sv.push_back("nicht");
						vv.push_back(sv);
						bNotInserted=TRUE;
					}
				if(!bNotInserted)
				{
					CStringVector sv;
					sv.push_back("nicht");
					vv.push_back(sv);
				}
			}
		}
		if(!subSentence.m_bBe)
			if(subSentence.m_bNegation && !bNotInserted)
			{
				CStringVector sv;
				sv.push_back("nicht");
				vv.push_back(sv);
			}
		for(int i=0;i<subSentence.m_vecEnglishAdverb.size();i++) // Adverbien
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		#ifdef _DEBUG
		printf("vor Art und Weise\n");
		#endif
		if(subSentence.m_pManner) // Umstandsbestimmung der Art und Weise
		{
			if(subSentence.m_pManner->m_vecMannerEnumerationElement.size()>0) // Objekt
			{
				Append(vv,TranslateManner(*subSentence.m_pManner));
			}
		}
		#ifdef _DEBUG
		else
			printf("!subSentence.m_pManner\n");
		#endif
		if(subSentence.m_bTense==PRESENT_PERFECT)
		{
			//CStringVector sv;
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			//sv.push_back(gv->words[13]);
			//vv.push_back(sv);
		}
		#ifdef _DEBUG
		printf("vor: for(i=0;i<subSentence.m_vecMiddleEnumerationElement.size();i++)\n");
		#endif
		for(i=0;i<subSentence.m_vecMiddleEnumerationElement.size();i++)
		{
			if(i>0)
				if((i+1)==subSentence.m_vecMiddleEnumerationElement.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				//sv.push_back(subSentence.m_MiddleEnumerationElements.at(i).m_strAdverbRefersToAdjectiveOrAdverb);
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).
					m_strAdverbRefersToAdjectiveOrAdverb);
				if(typeid(*pWord)==typeid(GermanAdverb)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdverb * ga=dynamic_cast<GermanAdverb*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strWord);
				}
				vv.push_back(sv);
			}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
		if(svGermanVerb.size()>1)
		{
			#ifdef _DEBUG
			printf("svPredicate.size()>1\n");
			#endif
			CStringVector sv;
			sv.push_back(svGermanVerb.at(1)); // 2. Wort des Vektors anhängen
			vv.push_back(sv);
		}
		//Append(vv,vvSubject=TranslateObject2(subSentence.m_Object)); // Objekt
		#ifdef _DEBUG
		printf("subSentence.m_pRelativeClause\n");
		#endif
		if(subSentence.m_pRelativeClause) // enthält der Satz einen Relativsatz?
			if(!subSentence.m_pRelativeClause->m_bRefersToSubject) // Relativsatz bezieht sich aufs Objekt
				if(subSentence.m_pObjectFirst)
				{
					std::vector <GermanNoun> vecGermanNoun=
						GetGermanWordOfEnglishNoun2(subSentence.m_pObjectFirst->
						m_vecEnumerationElement.at(0).m_pDan->m_Noun);
					if(vecGermanNoun.size()>0)
					{
						CStringVector sv;
						sv.push_back(_T(","));
						vv.push_back(sv);
						sv.clear();
						GermanNoun gn=vecGermanNoun.at(0);
						if(gn.m_bArticle=='1')
							sv.push_back(_T("den"));
						else if(gn.m_bArticle=='2')
							sv.push_back(_T("die"));
						else if(gn.m_bArticle=='3')
							sv.push_back(_T("das"));
						vv.push_back(sv);
					}
					Append(vv,TranslateRelativeClause(*subSentence.m_pRelativeClause));
				}
		//}
		if(subSentence.m_pPlace) // Umstandsbestimmung des Ortes
		{
			BYTE b=0;
			std::vector<CStringVector> vvPlace=TranslatePlace(
				*subSentence.m_pPlace,b,nStartID);
			Append(vv,vvPlace);
		}
		if(subSentence.m_bTense==FUTURE) // wenn Zeitform Zukunft
		{
			
			//if(subSentence.m_pVocNodeWhere
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			//if(subSentence.m_pVocNodeWhereAnEnglishVerbIsEncapsulated)
			//	if(subSentence.m_pVocNodeWhereAnEnglishVerbIsEncapsulated->next)
			//		GermanVerb * gv=dynamic_cast<GermanVerb *>(subSentence.m_pVocNodeWhereAnEnglishVerbIsEncapsulated->next->voc);
			if(subSentence.m_bActiveClause)
				if(gv!=NULL)
				{
					CStringVector sv;
					sv.push_back(gv->m_strWords[0]); // Infinitiv
					vv.push_back(sv);
				}
		}
		if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==SIMPLE_PAST ||
			subSentence.m_bTense==PRESENT_PERFECT || subSentence.m_bTense==
			PRESENT_PROGRESSIVE || subSentence.m_bTense==PAST_PROGRESSIVE || 
			subSentence.m_bTense==FUTURE)
		{ // wenn die Zeitform Präsens ist
			#ifdef _DEBUG
			printf("subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==\
SIMPLE_PAST || subSentence.m_bTense==PRESENT_PERFECT || subSentence.m_bTense==\
PRESENT_PROGRESSIVE || subSentence.m_bTense==PAST_PROGRESSIVE || \
subSentence.m_bTense==FUTURE\n");
			#endif
			if(!subSentence.m_bActiveClause) // wenn Passivsatz
			{

				CStringVector sv;
				if(gvInfinitive) // 
					sv.push_back(gvInfinitive->m_strWords[13]); // Partizip Perfekt anhängen (z.B. 'gemocht')
				else
					sv.push_back(gv->m_strWords[13]); // Partizip Perfekt anhängen (z.B. 'gemocht')
				vv.push_back(sv);
			}
		}
		if(subSentence.m_bTense==PRESENT_PERFECT && subSentence.m_bActiveClause)
		{ // wenn Perfekt und Aktivsatz
			CStringVector sv;
			sv.push_back(gv->m_strWords[13]); // Partizip Perfekt anhängen (z.B. 'gemocht')
			vv.push_back(sv);
		}
		if(!subSentence.m_bActiveClause) // wenn Passivsatz
		{
			if(subSentence.m_bTense==PRESENT_PERFECT) // wenn die Zeitform Perfekt ist
			{
				#ifdef _DEBUG
				printf("subSentence.m_bTense==PRESENT_PERFECT\n");
				#endif
				CStringVector sv;
				sv.push_back("worden");
				vv.push_back(sv);
			}
			if(subSentence.m_bTense==FUTURE || gvInfinitive) // wenn die Zeitform Zukunft ist
			{
				#ifdef _DEBUG
				printf("subSentence.m_bTense==PRESENT_PERFECT\n");
				#endif
				CStringVector sv;
				sv.push_back("werden");
				vv.push_back(sv);
			}
		}
		//if((subSentence.tense==FUTURE || gvInfinitive) && subSentence.m_bActiveClause)
		if(gvInfinitive && subSentence.m_bActiveClause)
		{ // wenn Aktivsatz 
			CStringVector sv;
			sv.push_back(gvInfinitive->m_strWords[0]); // Infinitiv anhängen
			vv.push_back(sv);
		}
	}
	if(subSentence.m_bWordOrder==SOP)
	{
		#ifdef _DEBUG
		printf("subSentence.m_bWordOrder==SOP\n");
		#endif
		CStringVector svGermanBendVerb;
		BYTE b=0;
		std::vector<CStringVector> vvSubject=TranslateSubject2(
			*subSentence.m_pSubject,b);
		Append(vv,vvSubject); // Subjekt
		#ifdef _DEBUG
		printf("vor Art und Weise\n");
		#endif
		if(subSentence.m_pManner) // Umstandsbestimmung der Art und Weise
		{
			if(subSentence.m_pManner->m_vecMannerEnumerationElement.size()>0) // Objekt
			{
				Append(vv,TranslateManner(*subSentence.m_pManner));
			}
		}
		#ifdef _DEBUG
		else
			printf("!subSentence.m_pManner\n");
		#endif
		if(subSentence.m_pObjectFirst) // Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectFirst\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(gv)
			{
				//printf("gv
				if(gv->m_bCase==1)
				{
					BYTE b=0;
					std::vector<CStringVector> vvObject=TranslateObject2(
						*subSentence.m_pObjectFirst,1,subSentence.m_bNegation,b,nStartID);
					Append(vv,vvObject); // Objekt
				}
				else if(gv->m_bCase==2)
				{
					BYTE b=0;
					std::vector<CStringVector> vvObject=TranslateObject2(
						*subSentence.m_pObjectFirst,2,subSentence.m_bNegation,b,nStartID);
					Append(vv,vvObject); // Objekt
				}
				else
				{
					BYTE b=0;
					std::vector<CStringVector> vvObject=TranslateObject2(
						*subSentence.m_pObjectFirst,0,subSentence.m_bNegation,b,nStartID);
					Append(vv,vvObject); // Objekt
				}
			}
		}
		#ifdef _DEBUG
		else
			printf("!subSentence.m_pObjectFirst\n");
		#endif
		if(subSentence.m_pObjectSecond) // 2. Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectSecond\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(gv)
			{
				//printf("gv
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectSecond,2,subSentence.m_bNegation,b,nStartID);
				Append(vv,vvObject); // Objekt
			}
		}
		if(subSentence.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
			sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
			vv.push_back(sv);
		}
		CStringVector svGermanVerb;
		if(subSentence.m_bTense==SIMPLE_PRESENT || // I begin.
			subSentence.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
			subSentence.m_bTense==SIMPLE_PAST || // I began.
			subSentence.m_bTense==PAST_PROGRESSIVE || // I was beginning.
			subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
			subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
			subSentence.m_bTense==PRESENT_PERFECT) // I have begun.
		{
			#ifdef _DEBUG
			printf("subSentence.m_bTense==SIMPLE_PRESENT || \
			subSentence.m_bTense==PRESENT_PROGRESSIVE || \
			subSentence.m_bTense==SIMPLE_PAST || \
			subSentence.m_bTense==PAST_PROGRESSIVE \
			subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE \
			subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
			subSentence.m_bTense==PRESENT_PERFECT\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(gv!=NULL)
			{
//				int index=0;
	//			if((index=subSentence.m_Subject.m_personIndex*subSentence.tense+1)<14)
		//		{
				#ifdef _DEBUG
				printf("subSentence.m_pSubject->m_personIndex: %u\n",subSentence.
					m_pSubject->m_personIndex);
				#endif
				if(subSentence.m_pSubject->m_vecEnumerationElement.size()==1)
				{
					//if(subSentence.m_Subject.m_enumerationElements.at(0).m_plainText!="");
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).
						m_vecStrPlainText.size()>0)
					{
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || 
							subSentence.m_bTense==PRESENT_PROGRESSIVE ||
							subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							if(subSentence.m_bActiveClause)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex]);
								svVerb.push_back(svGermanVerb.at(0));
								// Prädikat (Präsens)
								if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präteritum)
								}
							}
							else
							{
								if(gvInfinitive) // Beispiel: 'can be used'
								{ 
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präsens)
									if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
								else // Beispiel: 'is used'
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präsens)
									if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
							}
						}
						if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
							PAST_PROGRESSIVE)
						{
							if(subSentence.m_bActiveClause) // Beispiel: I used water.
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
									m_pSubject->m_personIndex+6]);
								svVerb.push_back(svGermanVerb.at(0));
							}
							else
							{
								if(gvInfinitive) // Beispiel: I could be used. / 
								{ // I could be being used.
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
								else // Beispiel: I was used. / I was being used.
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
							}
						}
						if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
							m_bActiveClause)
						{ // Beispiel: I have been used. -> Ich bin benutzt worden.
							GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
							svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex]);
							svVerb.push_back(svGermanVerb.at(0));
							// finite Form von 'sein' anhängen
						}
						// Prädikat (Präteritum)
						vv.push_back(svVerb);
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pDan)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pDan\n");
						#endif
						std::vector <EnglishNoun> vecEN=GetEnglishNoun(subSentence.
							m_pSubject->m_vecEnumerationElement.at(0).m_pDan->m_Noun);
						if(vecEN.size()>0)
						{
							if(IsEnglishSingular(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pDan->m_Noun))
							{
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
													m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
														m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
												m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									// Prädikat (Präteritum)
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==1)
								{ // im Deutschen: Plural
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
							}
							if(IsEnglishPlural(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pDan->m_Noun))
							{ // im Englischen: Plural
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else // Passivsatz
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // Beispiel: I am used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==2)
								{
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
							}
						}
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pEnr)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement\
.at(0).m_pEnr\n");
						#endif
						std::vector <EnglishNoun> vecEN=GetEnglishNoun(subSentence.
							m_pSubject->m_vecEnumerationElement.at(0).m_pEnr->m_Noun);
						if(vecEN.size()>0)
						{
							#ifdef _DEBUG
							printf("vecEN.size()>0\n");
							printf("vecEN.at(0).m_bTranslationType: %u\n",vecEN.at(0).
								m_bTranslationType);
							#endif
							if(IsEnglishSingular(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pEnr->m_Noun))
							{
								#ifdef _DEBUG
								printf("IsEnglishSingular(subSentence.m_pSubject->\
m_vecEnumerationElement.at(0).m_pEnr->m_Noun)\n");
								printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
								#endif
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									#ifdef _DEBUG
									printf("vecEN.at(0).m_bTranslationType==0\n");
									#endif
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												 svGermanVerb=GetCStringVector2(gv->m_strWords[
													 subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==1)
								{
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
										svVerb.push_back(svGermanVerb.at(0)); // 'bin' anhängen
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
							}
							if(IsEnglishPlural(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pEnr->m_Noun))
							{ // Im Englischen: Plural
								#ifdef _DEBUG
								printf("IsEnglishPlural(subSentence.m_pSubject->\
m_vecEnumerationElement.at(0).m_pEnr->m_Noun)\n");
								#endif
								if(vecEN.at(0).m_bTranslationType==0)
								{ // im Deutschen: Plural
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
										if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											if(subSentence.m_bActiveClause)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
										if(subSentence.m_bActiveClause) // wenn Aktivsatz
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
										else
										{
											if(gvInfinitive) // I could be used. / I could be being used.
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											}
											else // I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[12]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[6]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==2)
								{ // im Deutschen: Singular
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || 
										subSentence.m_bTense==PRESENT_PROGRESSIVE ||
										subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
										subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										if(subSentence.m_bActiveClause)
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präsens)
											if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
											{
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präteritum)
											}
										}
										else
										{
											if(gvInfinitive) // Beispiel: 'can be used'
											{ 
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
											else // Beispiel: 'is used'
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex]);
												svVerb.push_back(svGermanVerb.at(0));
												// Prädikat (Präsens)
												if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
												{
													svGermanVerb=GetCStringVector2(gv->m_strWords[
														subSentence.m_pSubject->m_personIndex+6]);
													svVerb.push_back(svGermanVerb.at(0));
													// Prädikat (Präteritum)
												}
											}
										}
									}
									if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
										PAST_PROGRESSIVE)
									{
										if(subSentence.m_bActiveClause) // Beispiel: I used water.
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
										}
										else
										{
											if(gvInfinitive) // Beispiel: I could be used. / 
											{ // I could be being used.
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
											else // Beispiel: I was used. / I was being used.
											{
												GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
												svGermanVerb=GetCStringVector2(gv->m_strWords[
													subSentence.m_pSubject->m_personIndex+6]);
												svVerb.push_back(svGermanVerb.at(0));
											}
										}
									}
									if(subSentence.m_bTense==PRESENT_PERFECT && !subSentence.
										m_bActiveClause)
									{ // Beispiel: I have been used. -> Ich bin benutzt worden.
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// finite Form von 'sein' anhängen
									}
									vv.push_back(svVerb);
								}
							}
						}
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pGerund)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.\
at(0).m_pGerund\n");
						#endif
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || 
							subSentence.m_bTense==PRESENT_PROGRESSIVE ||
							subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							svGermanVerb=GetCStringVector2(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex]);
							svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[
									subSentence.m_pSubject->m_personIndex+6]);
								svVerb.push_back(svGermanVerb.at(0));
							}
								// Prädikat (Präteritum)
						}
						if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
							PAST_PROGRESSIVE)
						{
							svGermanVerb=GetCStringVector2(gv->m_strWords[
								subSentence.m_pSubject->m_personIndex+6]);
							svVerb.push_back(svGermanVerb.at(0)); // Prädikat
						}
						vv.push_back(svVerb);
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).
						m_personalPronoun)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.\
at(0).m_personalPronoun\n");
						#endif
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || 
							subSentence.m_bTense==PRESENT_PROGRESSIVE ||
							subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							#ifdef _DEBUG
							printf("subSentence.m_bTense==SIMPLE_PRESENT || subSentence.\
m_bTense==PRESENT_PROGRESSIVE || subSentence.m_bTense==PRESENT_PERFECT_\
PROGRESSIVE || subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST\n");
							printf("gv->m_strWords[0]: %s\n",gv->m_strWords[0]);
							#endif
							if(subSentence.m_bActiveClause) // Aktivsatz
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[
									subSentence.m_pSubject->m_personIndex]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // 1. Wort des Vektors anhängen
							}
							else // Passivsatz
							{
								BOOL bAuxiliaryVerb=FALSE;
								if(subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated)
									if(typeid(*subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)
										==typeid(EnglishAuxiliaryVerb) && 
										subSentence.
										m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
										m_pWordNodeNext)
									{
										bAuxiliaryVerb=TRUE;
										GermanVerb * gv=dynamic_cast<GermanVerb*>(subSentence.
											m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
										{ // persönliches Subjekt beinhaltet Beispiel: I can be given
											// CHANCES. -> Mir KÖNNEN Chancen gegeben werden. anstatt:
											// Ich KANN Chancen gegeben werden.
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pPersonalSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
											// gebeugte From vom modalen Hilfsverb anhängen
										}
										else // zum Beispiel: I can be used. 
										{
											svGermanVerb=GetCStringVector2(gv->m_strWords[
												subSentence.m_pSubject->m_personIndex]);
											// GetCStringVector deshalb, weil ein Verb in der gebeugten
											// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
											svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										}
									}
								if(!bAuxiliaryVerb) // kein modales Hilfsverb
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
									{ // persönliches Subjekt beinhaltet Beispiel: I am given
										// CHANCES. -> Mir WERDEN Chancen gegeben. anstatt:
										// Ich WERDE Chancen gegeben.
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pPersonalSubject->m_personIndex]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										// gebeugte From von 'werden' anhängen
									}
									else // zum Beispiel: I can be used. 
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									}
								}
							}
							if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								if(subSentence.m_bActiveClause) // Aktivsatz
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pSubject->m_personIndex+6]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0));
								}
								else // Passivsatz
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
									{ // persönliches Subjekt beinhaltet Beispiel: I was given
										// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
										// Ich WURDE eine Chance gegeben.
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pPersonalSubject->m_personIndex+6]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
										// gebeugte From von 'sein' anhängen
									}
									else
									{
										svGermanVerb=GetCStringVector2(gv->m_strWords[
											subSentence.m_pSubject->m_personIndex+6]);
										// GetCStringVector deshalb, weil ein Verb in der gebeugten
										// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
										svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									}
								}
							}
								// Prädikat (Präteritum)
							vv.push_back(svVerb);
						}
						if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
							PAST_PROGRESSIVE)
						{ // Vergangenheit oder fortschreitende Vergangenheit
							if(subSentence.m_bActiveClause)
							{
								svGermanVerb=GetCStringVector2(gv->m_strWords[
									subSentence.m_pSubject->m_personIndex+6]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							}
							else // Passivsatz
							{
								GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
								if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
								{ // persönliches Subjekt beinhaltet Beispiel: I was given
									// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
									// Ich WURDE eine Chance gegeben.
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pPersonalSubject->m_personIndex+6]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									// gebeugte From von 'sein' anhängen
								}
								else
								{
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pSubject->m_personIndex+6]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
								}
							}
							vv.push_back(svVerb);
						}
						if(subSentence.m_bTense==PRESENT_PERFECT)
						{
							if(subSentence.m_bActiveClause)
							{
								GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("have"));
								svGermanVerb=GetCStringVector2(gv->m_strWords[
									subSentence.m_pSubject->m_personIndex]);
								// GetCStringVector deshalb, weil ein Verb in der gebeugten
								// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
								svVerb.push_back(svGermanVerb.at(0)); // Prädikat
							}
							else // Passivsatz
							{
								if(subSentence.m_pPersonalSubject) // wenn der Satz ein 
								{ // persönliches Subjekt beinhaltet Beispiel: I have been given
									// A CHANCE. -> Mir IST eine Chance gegeben worden. anstatt:
									// Ich BIN eine Chance gegeben worden.
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pPersonalSubject->m_personIndex]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
									// gebeugte From von 'sein' anhängen
								}
								else
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[
										subSentence.m_pSubject->m_personIndex]);
									// GetCStringVector deshalb, weil ein Verb in der gebeugten
									// Form aus 2 Wörtern bestehen kann: 'aussehen' - 'ich sehe aus'
									svVerb.push_back(svGermanVerb.at(0)); // Prädikat
								}
							}
							vv.push_back(svVerb);
						}
					}
				}
				else
				{
					CStringVector svVerb;
					if(subSentence.m_bTense==SIMPLE_PRESENT || 
						subSentence.m_bTense==PRESENT_PROGRESSIVE ||
						subSentence.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
						subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
					{
						svVerb.push_back(gv->m_strWords[subSentence.m_pSubject->
							m_personIndex]); // Prädikat
						if(subSentence.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							svVerb.push_back(gv->m_strWords[subSentence.m_pSubject->
							m_personIndex+6]);
							// Prädikat (Präteritum)
					}
					if(subSentence.m_bTense==SIMPLE_PAST || subSentence.m_bTense==
						PAST_PROGRESSIVE)
						svVerb.push_back(gv->m_strWords[subSentence.m_pSubject->
						m_personIndex+6]); // Prädikat
					vv.push_back(svVerb);
				}
			//	}
			}
			#ifdef _DEBUG
			else
			{
				TRACE("gv==NULL\n");
				printf("gv==NULL\n");
			}
			#endif
		}
	}
	if(subSentence.m_bWordOrder==PSO)
	{
		if(subSentence.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
		{
			CStringVector sv;
			GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
			sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
			vv.push_back(sv);
		}
		if(subSentence.m_bTense==SIMPLE_PRESENT) // Präsens
		{
			if(!subSentence.m_bActiveClause) // wenn Passivsatz
			{
				CStringVector sv;
				GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
				sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
				vv.push_back(sv);
			}
		}
		if(subSentence.m_bTense==PRESENT_PERFECT) // Perfekt
		{
			if(subSentence.m_bActiveClause)
			{
				CStringVector sv;
				GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("have"));
				sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
				vv.push_back(sv);
			}
			else
			{
				CStringVector sv;
				GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("sein"));
				sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
				vv.push_back(sv);
			}
		}
		if(subSentence.m_bActiveClause && (subSentence.m_bTense==
			PRESENT_PROGRESSIVE || subSentence.m_bTense==SIMPLE_PRESENT
			|| subSentence.m_bTense==SIMPLE_PAST))
		{
			#ifdef _DEBUG
			printf("PRESENT_PROGRESSIVE || SIMPLE_PRESENT || SIMPLE_PAST\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(gv!=NULL)
			{
//				int index=0;
	//			if((index=subSentence.m_Subject.m_personIndex*subSentence.tense+1)<14)
		//		{
				#ifdef _DEBUG
				printf("subSentence.m_pSubject->m_personIndex: %u\n",
					subSentence.m_pSubject->m_personIndex);
				#endif
				if(subSentence.m_pSubject->m_vecEnumerationElement.size()==1)
				{
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pDan)
					{
						std::vector <EnglishNoun> vecEN=GetEnglishNoun(subSentence.
							m_pSubject->m_vecEnumerationElement.at(0).m_pDan->m_Noun);
						if(vecEN.size()>0)
						{
							if(IsEnglishSingular(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pDan->m_Noun))
							{
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.
										m_bTense==PRESENT_PROGRESSIVE)
									{
										svVerb.push_back(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex]); // Prädikat
									}
									if(subSentence.m_bTense==SIMPLE_PAST)
										svVerb.push_back(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]); // Prädikat
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==1)
								{
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.
										m_bTense==PRESENT_PROGRESSIVE)
									{
										svVerb.push_back(gv->m_strWords[6]); // Prädikat
									}
									if(subSentence.m_bTense==SIMPLE_PAST)
										svVerb.push_back(gv->m_strWords[12]); // Prädikat
									vv.push_back(svVerb);
								}
							}
							if(IsEnglishPlural(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pDan->m_Noun))
							{
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.
										m_bTense==PRESENT_PROGRESSIVE)
									{
										svVerb.push_back(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex]); // Prädikat
									}
									if(subSentence.m_bTense==SIMPLE_PAST)
										svVerb.push_back(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]); // Prädikat
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==2)
								{
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.
										m_bTense==PRESENT_PROGRESSIVE)
									{
										svVerb.push_back(gv->m_strWords[4]); // Prädikat
									}
									if(subSentence.m_bTense==SIMPLE_PAST)
										svVerb.push_back(gv->m_strWords[10]); // Prädikat
									vv.push_back(svVerb);
								}
							}
						}
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pEnr)
					{
						#ifdef _DEBUG
						printf("subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pEnr\n");
						#endif
						std::vector <EnglishNoun> vecEN=GetEnglishNoun(subSentence.
							m_pSubject->m_vecEnumerationElement.at(0).m_pEnr->m_Noun);
						if(vecEN.size()>0)
						{
							#ifdef _DEBUG
							printf("vecEN.size()>0\n");
							printf("vecEN.at(0).m_bTranslationType: %u\n",
								vecEN.at(0).m_bTranslationType);
							#endif
							if(IsEnglishSingular(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pEnr->m_Noun))
							{
								#ifdef _DEBUG
								printf("IsEnglishSingular(subSentence.m_pSubject->\
m_vecEnumerationElement.at(0).m_pEnr->m_Noun)\n");
								printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
								#endif
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									#ifdef _DEBUG
									printf("vecEN.at(0).m_bTranslationType==0\n");
									#endif
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==
										PRESENT_PROGRESSIVE)
									{
										#ifdef _DEBUG
										printf("subSentence.m_bTense==SIMPLE_PRESENT || subSentence.\
tense==PRESENT_PROGRESSIVE\n");
										printf("gv->m_strWords[subSentence.m_pSubject->\
m_personIndex]: %s\n",gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
										#endif
										svVerb.push_back(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex]); // Prädikat
									}
									if(subSentence.m_bTense==SIMPLE_PAST)
										svVerb.push_back(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]); // Prädikat
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==1)
								{
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==
										PRESENT_PROGRESSIVE)
									{
										svVerb.push_back(gv->m_strWords[6]); // Prädikat
									}
									if(subSentence.m_bTense==SIMPLE_PAST)
										svVerb.push_back(gv->m_strWords[12]); // Prädikat
									vv.push_back(svVerb);
								}
							}
							if(IsEnglishPlural(subSentence.m_pSubject->
								m_vecEnumerationElement.at(0).m_pEnr->m_Noun))
							{
								#ifdef _DEBUG
								printf("IsEnglishPlural(subSentence.m_pSubject->\
m_vecEnumerationElement.at(0).enr->m_Noun)\n");
								#endif
								if(vecEN.at(0).m_bTranslationType==0)
								{
							//if(subSentence.m_Subject.m_enumerationElements.at(0).dan->m_Noun)
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==
										PRESENT_PROGRESSIVE)
									{
										svVerb.push_back(gv->m_strWords[subSentence.
											m_pSubject->m_personIndex]); // Prädikat
									}
									if(subSentence.m_bTense==SIMPLE_PAST)
										svVerb.push_back(gv->m_strWords[subSentence.
										m_pSubject->m_personIndex+6]); // Prädikat
									vv.push_back(svVerb);
								}
								if(vecEN.at(0).m_bTranslationType==2)
								{
									CStringVector svVerb;
									if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.
										m_bTense==PRESENT_PROGRESSIVE)
									{
										#ifdef _DEBUG
										printf("gv->m_strWords[4]: %s\n",gv->m_strWords[4]);
										#endif
										svVerb.push_back(gv->m_strWords[3]); // Prädikat
									}
									if(subSentence.m_bTense==SIMPLE_PAST)
										svVerb.push_back(gv->m_strWords[10]); // Prädikat
									vv.push_back(svVerb);
								}
							}
						}
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).m_pGerund)
					{
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==
							PRESENT_PROGRESSIVE)
						{
							svVerb.push_back(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex]); // Prädikat
						}
						if(subSentence.m_bTense==SIMPLE_PAST)
							svVerb.push_back(gv->m_strWords[subSentence.
							m_pSubject->m_personIndex+6]); // Prädikat
						vv.push_back(svVerb);
					}
					if(subSentence.m_pSubject->m_vecEnumerationElement.at(0).
						m_personalPronoun)
					{
						CStringVector svVerb;
						if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==
							PRESENT_PROGRESSIVE)
						{
							svVerb.push_back(gv->m_strWords[subSentence.
								m_pSubject->m_personIndex]); // Prädikat
						}
						if(subSentence.m_bTense==SIMPLE_PAST)
							svVerb.push_back(gv->m_strWords[subSentence.
							m_pSubject->m_personIndex+6]); // Prädikat
						vv.push_back(svVerb);
					}
				}
				else
				{
					CStringVector svVerb;
					if(subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==
						PRESENT_PROGRESSIVE)
					{
						svVerb.push_back(gv->m_strWords[subSentence.
							m_pSubject->m_personIndex]); // Prädikat
					}
					if(subSentence.m_bTense==SIMPLE_PAST)
						svVerb.push_back(gv->m_strWords[subSentence.
						m_pSubject->m_personIndex+6]); // Prädikat
					vv.push_back(svVerb);
				}
			//	}
			}
			else
				TRACE("gv==NULL\n");
		}
		BYTE b=0;
		std::vector<CStringVector> vvSubject=TranslateSubject2(
			*subSentence.m_pSubject,b);
		Append(vv,vvSubject); // Subjekt
		if(subSentence.m_pRelativeClause) // Relativsatz
		{
			if(subSentence.m_pRelativeClause->m_bRefersToSubject)
			{
				#ifdef _DEBUG
				printf("subSentence.m_rc->m_bRefersToSubject\n");
				#endif
				if(subSentence.m_pSubject->m_vecEnumerationElement.size()>0)
				{
					std::vector <GermanNoun> vecGermanNoun=
						GetGermanWordOfEnglishNoun2(subSentence.m_pSubject->
						m_vecEnumerationElement.at(0).m_pDan->m_Noun);
					if(vecGermanNoun.size()>0)
					{
						CStringVector sv;
						sv.push_back(_T(","));
						vv.push_back(sv);
						sv.clear();
						GermanNoun gn=vecGermanNoun.at(0);
						if(gn.m_bArticle=='1')
							sv.push_back(_T("den"));
						else if(gn.m_bArticle=='2')
							sv.push_back(_T("die"));
						else if(gn.m_bArticle=='3')
							sv.push_back(_T("das"));
						vv.push_back(sv);
					}
				}
				Append(vv,TranslateRelativeClause(*subSentence.m_pRelativeClause));
				CStringVector sv;
				sv.push_back(_T(","));
				vv.push_back(sv);
			}
		}
		if(subSentence.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(subSentence.m_bNegation)
			{
				BOOL bNot=TRUE;
				if(subSentence.m_pObjectFirst)
				{
					for(DWORD dwRun=0;dwRun<subSentence.m_pObjectFirst->
						m_vecEnumerationElement.size();dwRun++)
					{
						if(subSentence.m_pObjectFirst->m_vecEnumerationElement.at(dwRun).m_pEnr)
							if(subSentence.m_pObjectFirst->m_vecEnumerationElement.at(dwRun).
								m_pEnr->bIndefiniteArticle)
								bNot=FALSE;
					}
				}
				if(bNot)
				{
					CStringVector sv;
					sv.push_back("nicht");
					vv.push_back(sv);
				}
			}
		}
		if(subSentence.m_pObjectFirst) // Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectFirst\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(gv)
			{
				//printf("gv
				if(gv->m_bCase==1)
				{
					BYTE b=0;
					std::vector<CStringVector> vvObject=TranslateObject2(
						*subSentence.m_pObjectFirst,1,subSentence.m_bNegation,b,nStartID);
					Append(vv,vvObject); // Objekt
				}
				else if(gv->m_bCase==2)
				{
					BYTE b=0;
					std::vector<CStringVector> vvObject=TranslateObject2(
						*subSentence.m_pObjectFirst,2,subSentence.m_bNegation,b,nStartID);
					Append(vv,vvObject); // Objekt
				}
				else
				{
					BYTE b=0;
					std::vector<CStringVector> vvObject=TranslateObject2(
						*subSentence.m_pObjectFirst,0,subSentence.m_bNegation,b,nStartID);
					Append(vv,vvObject); // Objekt
				}
			}
		}
		#ifdef _DEBUG
		else
			printf("subSentence.m_pObjectFirst->m_vecEnumerationElement.size()<=0\n");
		#endif
		if(subSentence.m_pObjectSecond) // 2. Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectSecond\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(gv)
			{
				//printf("gv
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectSecond,2,subSentence.m_bNegation,b,nStartID);
				Append(vv,vvObject); // Objekt
			}
		}
		if(subSentence.m_bTense==FUTURE) // wenn Zeitform Zukunft
		{
			if(gv!=NULL)
			{
				CStringVector sv;
				sv.push_back(gv->m_strWords[0]); // Infinitiv
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanClause,germanObject); // Subjekt
		for(int i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		for(i=0;i<subSentence.m_vecMiddleEnumerationElement.size();i++)
		{
			if(i>0)
				if((i+1)==subSentence.m_vecMiddleEnumerationElement.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				sv.push_back(subSentence.m_vecMiddleEnumerationElement.at(i).
					m_strAdverbRefersToAdjectiveOrAdverb);
				vv.push_back(sv);
			}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
	}
	if(subSentence.m_bWordOrder==SOP)
	{
	}
	for(int i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
	{
		if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='3')
		{
			GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
				m_vecEnglishAdverb.at(i).m_strWord);
			CStringVector sv;
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	TRACE("CStringVector TranslateClause ENDE\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateClause2(Clause \
& subSentence) - ENDE\n");
	#endif
	return vv;

}*/

//liefert die gebeugte Verbform für die Satzbauten SPO,SOP,POS zurück; 
//wird von
//std::vector<CStringVector> TranslateClause2(const Clause & 
//subSentence,BYTE & bFirstTokenInSentence,EnglishView * pEV,int & 
//nStartID)
//aufgerufen
CStringVector GetGermanBendVerb(GermanVerb * pgvBend, const Predicate 
	& predicate,BYTE personIndex)
{
	CStringVector svVerb;
	CStringVector svGermanVerb;
	personIndex++;
	//wenn es keine gebeugte deutsche Verbform gibt (kann auch bei
	//der Zeitform "Gegenwart" sein, nämlich wenn es ein passives
	//Prädikat ist, denn es sollen nur die für eine korrekte 
	//Übersetzung benötigten Daten in der Klasse "Predicate" 
	//gespeichert und nicht z.B. das Hilfsverb "haben" für die 
	//Zeitform "Perfekt" gespeichert, denn das Hilfsverb lässt sich
	//aus der Zeitform "Perfekt" herleiten)
	if(!pgvBend)
	{
		//wenn es ein passives Prädikat ist
		if(!predicate.m_bActiveClause)
		{
			//wenn die Zeitform "Perfekt" ("ich bin gemocht worden") 
			//oder "Plusquamperfekt" ("ich war gemocht worden") oder
			//"fortschreitendes Perfekt" ("ich bin gerade gemocht 
			//worden") oder "fortschreitendes Plusquamperfekt" ("ich 
			//war gerade gemocht worden") ist, ...
			if(predicate.m_bTense==PRESENT_PERFECT || 
				predicate.m_bTense==PAST_PERFECT ||
				predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
				predicate.m_bTense==PAST_PERFECT_PROGRESSIVE)
			{
				//...dann als gebeugte Verbform "sein" nehmen
				pgvBend=GetGermanWordOfEnglishVerb(_T("be"));
			}
			//sonst...
			else
				//als gebeugte Verbform "werden" nehmen
				pgvBend=GetGermanWordOfEnglishVerb(_T("will"));
		}
		else
		{
			//wenn die Zeitform "Zukunft" ("ich werde gehen") oder 
			//"fortschreitende Zukunft" ("ich werde gerade gehen") 
			//oder "Zukunft Perfekt" ("ich werde gegangen sein") 
			//ist, ...
			if(predicate.m_bTense==FUTURE || predicate.m_bTense==
				FUTURE_CONTINOUS || predicate.m_bTense==FUTURE_PERFECT)
			{
				//...dann als gebeugte Verbform "werden" nehmen
				pgvBend=GetGermanWordOfEnglishVerb(_T("will"));
			}
			//wenn die Zeitform "Perfekt" ("ich bin gemocht worden") 
			//oder "Plusquamperfekt" ("ich war gemocht worden") oder
			//"fortschreitendes Perfekt" ("ich bin gerade gemocht 
			//worden") oder "fortschreitendes Plusquamperfekt" ("ich 
			//war gerade gemocht worden") ist, ...
			if(predicate.m_bTense==PRESENT_PERFECT || 
				predicate.m_bTense==PAST_PERFECT || 
				predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE || 
				predicate.m_bTense==PAST_PERFECT_PROGRESSIVE)
			{
				//...dann als gebeugte Verbform "haben" nehmen
				pgvBend=GetGermanWordOfEnglishVerb(_T("have"));
			}
		}
	}
	//wenn die Zeitform "Präsens", "fortschreitendes Präsens", 
	//"fortschreitender Perfekt" oder "Präsens oder Präteritum"
	//,... ist
	if(predicate.m_bTense==SIMPLE_PRESENT || // ("I begin.")
		predicate.m_bTense==PRESENT_PROGRESSIVE || // ("I am beginning.")
		predicate.m_bTense==PRESENT_PERFECT || // ("I have begun.")
		predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE || // ("I have been beginning.")
		predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST || // ("I hit.")
		predicate.m_bTense==FUTURE || // ("I will begin.")
		predicate.m_bTense==FUTURE_CONTINOUS || // ("I will be beginning.")
		predicate.m_bTense==FUTURE_PERFECT) // ("I will have begun.")
	{
		//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
		//"ich sehe an"), dann entsteht ein Vektor aus
		//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
		//Beispiel: "sehen", "ich sehe")
		svGermanVerb=GetCStringVector2(pgvBend->m_strWords[personIndex]);
		svVerb.push_back(svGermanVerb.at(0));
		//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
		//interpretiert werden kann (zum Beispiel beim Verb 
		//"to hit": Präteritum ist "hit")
		if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
		{
			//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
			//"ich sehe an"), dann entsteht ein Vektor aus
			//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
			//Beispiel: "sehen", "ich sehe")
			svGermanVerb=GetCStringVector2(pgvBend->
				m_strWords[personIndex+6]);
			svVerb.push_back(svGermanVerb.at(0));
			// Prädikat (Präteritum)
		}
	}
	//wenn die Zeitform "Präteritum" oder "fortschreitendes 
	//Präteritum" ist
	//Präteritum: "I used water."
	//fortschreitendes Präteritum: "I was using water."
	if(predicate.m_bTense==SIMPLE_PAST || // ("I began.")
		predicate.m_bTense==PAST_PROGRESSIVE || // ("I was beginning.")
		predicate.m_bTense==PAST_PERFECT || // ("I had begun.")
		predicate.m_bTense==PAST_PERFECT_PROGRESSIVE) // ("I had been beginning.")
	{
		//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
		//"ich sehe an"), dann entsteht ein Vektor aus
		//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
		//Beispiel: "sehen", "ich sehe")
		svGermanVerb=GetCStringVector2(pgvBend->
			m_strWords[personIndex+6]);
		svVerb.push_back(svGermanVerb.at(0));
	}
	return svVerb;
}

//liefert die Übersetzung des Rests des Prädikats (ohne die gebeugte 
//Verbform) für die Satzbauten SPO,SOP,POS zurück; 
//wird von
//std::vector<CStringVector> TranslateClause2(const Clause & 
//subSentence,BYTE & bFirstTokenInSentence,EnglishView * pEV,int & 
//nStartID)
//aufgerufen
std::vector<CStringVector> TranslateRestOfGermanPredicate(const Predicate & predicate)
{
	std::vector<CStringVector> vvRestOfPredicate;
  LOG("23.05.2008 11.17.06\n");//=TranslateRestOfGermanPredicate
	//wenn das Prädikat ein aktives Prädikat ist
	if(predicate.m_bActiveClause)
	{
    LOG("23.05.2008 11.17.24\n");//=TranslateRestOfGermanPredicate ActiveClause
		if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
		{
      LOG("23.05.2008 11.17.36\n");//=TranslateRestOfGermanPredicate NBV
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
				m_pWordNodeNext)
			{
				bool bAddhaben=false;
				CStringVector sv;
				GermanVerb * pgvNonBend=dynamic_cast<GermanVerb*>(predicate.
					m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
					m_pWordNodeNext->m_pWord);
#ifdef _DEBUG
				printf("pgvNonBend->m_strWords[0]: %s\n",pgvNonBend->
					m_strWords[0]);
#endif
				//wenn die Zeitform "Präsens/Gegenwart" ("I can like you."
				//->"Ich kann dich mögen.") oder "Zukunft" 
				//("I will like you."->"Ich werde dich mögen.") ist, ...
				if(predicate.m_bTense==SIMPLE_PRESENT || 
					predicate.m_bTense==FUTURE
					//TELEKOLLEG I ENGLISCH Band 3 Seite 38: Future Continuous
					//I'll be working late.->Ich werde spät arbeiten.
					|| predicate.m_bTense==FUTURE_CONTINOUS)
				{
          LOG("23.05.2008 11.17.44\n");//=TranslateRestOfGermanPredicate Present Or Future
					//...dann den Infinitiv des nicht-gebeugten Verbes anhängen
					sv.push_back(pgvNonBend->m_strWords[INDEX_FOR_GERMAN_INFINITIVE]); // Infinitiv
				}
				//wenn die Zeitform "fortschreitende Zukunft" 
				//("I will be walking."->"Ich werde gelaufen sein.") oder 
        //, ...
				if(// || 
				  //"Zukunft Perfekt" ("I will have liked you."->
				  //"Ich werde dich gemocht haben.")  oder
					predicate.m_bTense==FUTURE_PERFECT ||
          //oder "Perfekt" (z.B. "I have liked you."->"Ich habe dich gemocht.")
					predicate.m_bTense==PRESENT_PERFECT ||
          //"Plusquamperfekt" (z.B. "I had liked you."->"Ich hatte dich gemocht.") oder 
					predicate.m_bTense==PAST_PERFECT ||
          //"fortschreitendes Perfekt" (z.B. "I have been liking you."->
				  //"Ich habe dich (gerade) gemocht.") oder 
					predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
          //"fortschreitendes Plusquamperfekt" (z.B. "I had been liking you."->
				  //"Ich hatte dich (gerade) gemocht.") ist
					predicate.m_bTense==PAST_PERFECT_PROGRESSIVE)
				{
					//...dann das Partizip Perfekt des nicht-gebeugten Verbes 
					//anhängen
					sv.push_back(pgvNonBend->m_strWords[EAV_ARRAY_INDEX_FOR_PAST_PARTICIPLE]);
				}
				vvRestOfPredicate.push_back(sv);
				////wenn die Zeitform "fortschreitende Zukunft" 
				////("I will be walking."->"Ich werde gelaufen sein.") ist, ...
				//if(predicate.m_bTense==FUTURE_CONTINOUS)
				//{
				//	sv.clear();
				//	//...dann den Infinitiv des Verbes "sein" anhängen
				//	sv.push_back(_T("sein")); // Infinitiv
				//	vvRestOfPredicate.push_back(sv);
				//}
				//wenn die Zeitform "Zukunft Perfekt" 
				//("I will have liked you."->
				//"Ich werde dich gemocht haben.") ist
				if(predicate.m_bTense==FUTURE_PERFECT)
					bAddhaben=true;
				if(predicate.
					m_pWordNodeWhereTheGermanTranslationIsABendVerb &&
					typeid(*predicate.
						m_pWordNodeWhereTheGermanTranslationIsABendVerb->
						m_pWord)==typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast
						<EnglishAuxiliaryVerb*>(predicate.
						m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
					//wenn zu wenig Speicherplatz zur Verfügung stand, kann es sein,
					//dass kein Speicher für eav alloziert wurde und eav "NULL" ist
					if(eav &&
						//wenn modales Hilfsverb (zum beispiel "can", "may"):
						//modale Hilfsverbeb können nur im Präsens vorkommen
						!eav->m_bIntegral)
					{
					//oder das Prädikat 
				//ein modales Hilfsverb beinhaltet UND die Zeitform "Perfekt" 
				//("I can have seen you."->"Ich kann dich gesehen haben.") 
				//oder "Plusquamperfekt" ("I could have seen you."->
				//"Ich konnte dich gesehen haben.") oder 
				//"fortschreitendes Perfekt"
				//("I can have been liking you."->
				//"Ich kann dich (gerade) gemocht haben.") oder 
				//"fortschreitendes Plusquamperfekt" 
				//("I could have been liking you."->
				//"Ich konnte dich (gerade) gemocht haben.") ist, ...
						if(predicate.m_bTense==PRESENT_PERFECT ||
							predicate.m_bTense==PAST_PERFECT ||
							predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							predicate.m_bTense==PAST_PERFECT_PROGRESSIVE)
						{
							bAddhaben=true;
						}
					}
				}//if bend aux. verb
				if(bAddhaben)
				{
					sv.clear();
					//...dann den Infinitiv des Verbes "haben" anhängen
					sv.push_back(_T("haben")); // Infinitiv
					vvRestOfPredicate.push_back(sv);
				}
			}
		}
	}
	//wenn das Prädikat ein passives Prädikat ist, ...
	else
	{
		CStringVector sv;
    LOG("23.05.2008 11.17.56\n");//=TranslateRestOfGermanPredicate Passive Clause
		GermanVerb * pgvNonBend=dynamic_cast<GermanVerb*>(predicate.
			m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
			m_pWordNodeNext->m_pWord);
    LOG("23.05.2008 11.18.03\n");//=TranslateRestOfGermanPredicate Passive Clause
		//...dann das Partizip Perfekt des nicht-gebeugten Verbes 
		//anhängen
		sv.push_back(pgvNonBend->m_strWords[13]); // Infinitiv
		vvRestOfPredicate.push_back(sv);
		//wenn das Prädikat ein modales Hilfsverb beinhaltet UND
		//die Zeitform "Präsens/Gegenwart" ("I can be liked."
		//->"Ich kann gemocht werden.") ist oder die Zeitform 
		//"Zukunft" ("I will be liked."->"Ich werde gemocht werden.") 
		//ist, ...
		if((predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb && 
			predicate.m_bTense==SIMPLE_PRESENT) ||
			predicate.m_bTense==FUTURE)
		{
			sv.clear();
			//...dann den Infinitiv des Verbes "werden" anhängen
			sv.push_back(_T("werden")); // Infinitiv
			vvRestOfPredicate.push_back(sv);
		}
		//wenn die Zeitform "Zukunft Perfekt" 
		//("I will have been liked."->
		//"Ich werde gemocht worden sein.") oder die Zeitform "Perfekt" 
		//("I have been liked."->"Ich bin gemocht worden.") 
		//oder "Plusquamperfekt" ("I had been liked."->
		//"Ich war gemocht worden.") oder 
		//"fortschreitendes Perfekt"
		//("I was been liking."->
		//"Ich bin (gerade) gemocht worden.") oder 
		//"fortschreitendes Plusquamperfekt" 
		//("I had been liking."->
		//"Ich war (gerade) gemocht worden.") ist, ...
		if(predicate.m_bTense==FUTURE_PERFECT ||
			predicate.m_bTense==PRESENT_PERFECT ||
			predicate.m_bTense==PAST_PERFECT ||
			predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
			predicate.m_bTense==PAST_PERFECT_PROGRESSIVE)
		{
			sv.clear();
			//...dann das Partizip Perfekt des Verbes "werden" anhängen
			sv.push_back(_T("worden"));
			vvRestOfPredicate.push_back(sv);
			//wenn das Prädikat ein modales Hilfsverb beinhaltet oder
			//die Zeitform "Zukunft Perfekt" ist, ...
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb ||
				predicate.m_bTense==FUTURE_PERFECT)
			{
				sv.clear();
				//...dann den Infinitiv des Verbes "sein" anhängen
				sv.push_back(_T("sein"));
				vvRestOfPredicate.push_back(sv);
			}
		}
	}
	return vvRestOfPredicate;
}

//TranslateClause2 wird von TranslateSentence2 aufgerufen
//wenn bFirstTokenInSentence verändert wird, sollen sich die Änderungen auf 
//den Aufrufer übertragen, deswegen eine Referenz (&)
//ein "clause" ist ein Teilsatz wie Haupt- oder Nebensatz 
//(Nebensatz OHNE Relativsatz)

/*In anderen Sprachen, wie zum Beispiel im Deutschen, gilt 
die grundsätzliche Struktur (hier: SPO) nur in Aussagesätzen, 
verändert sich aber in anderen Satztypen.
Beispiel:
    * "Er isst den Apfel" (SPO) (Aussagesatz, neutral)
    * "Wenn er den Apfel isst, ..." (SOP) (Nebensatz)
    * "Isst er den Apfel?" (PSO) (Frage)
    * "Äpfel isst er nicht." (OPS) (Aussagesatz, Objekt wird betont)
		Quelle:http://de.wikipedia.org/wiki/Satzstellung
*/
		//Falls ich dich sehe, isst er den Apfel.(?)
std::vector<CStringVector> TranslateClause2(const Clause & subSentence,
	BYTE & bFirstTokenInSentence,
//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
// m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
// zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
// (Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
// ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
// Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
// auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
// (Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
// pEV wird zum Beispiel an TranslateSubject weitergeleitet
	//EnglishView * pEV,
  std::vector<IntPair> & rvecintpair,
	//IDentifikationsnummern, über die es möglich ist, dass 
	//bei 2 oder mehr Kombinationsfeldern gleichzeitig 
	//ausgewählt wird (für mögliche Übersetzungen von "you"
	//ins Deutsche [du|ma|ihr|Sie]), damit korrespondierende 
	//gebeugte Verform gleichzeitig ausgewählt wird
//nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
// die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
// bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
// Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
// sich das "folgst" automatisch in "folgt" ändern
// Es kann folgenden Satz geben: "I like hurting you." "hurting you": 
// Gerundium -> "Ich mag |dich| zu verletzen."
//                      |Sie |
//                      |euch|
	int & nStartID)
{
	BYTE personIndex=0;
	//CString strBendVerb;
	//CString strTime;
	//CString 
	GermanVerb * pgvBend=NULL;
	GermanVerb * pgvNonBend=NULL;
	std::vector<CStringVector> vv;
	
	TRACE("CStringVector TranslateClause ANFANG\n");
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateClause2(Clause & \
subSentence,BYTE & bFirstTokenInSentence) - ANFANG\n");
	if(subSentence.m_bNegation)
	{
		printf("subSentence.m_bNegation\n");
	}
	else
	{
		printf("!subSentence.m_bNegation\n");
	}
	if(subSentence.m_pManner)
		printf("subSentence.m_pManner\n");
	else
		printf("!subSentence.m_pManner\n");
	#endif
	#ifdef _DEBUG
	if(subSentence.m_bActiveClause)
		printf("subSentence.m_bActiveClause\n");
	else
		printf("!subSentence.m_bActiveClause\n");
	printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
	if(subSentence.m_pSubject)
		printf("subSentence.m_pSubject->m_personIndex: %u\n",subSentence.
		m_pSubject->m_personIndex); 
	#endif
#ifdef _DEBUG
	else
		printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
	#ifdef _DEBUG
	if(pgvBend)
		printf("pgvBend\n");
	else
		printf("!pgvBend\n");
	#endif
	if(subSentence.m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
	{
	#ifdef _DEBUG
		printf("subSentence.m_vecpWordNodeWhereAnEnglishConjunctionIs\
Encapsulated.size()>0\n");
	#endif
		for(DWORD dwRun=0;dwRun<subSentence.
		m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun++)
		{
			if(subSentence.m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.
			at(dwRun)->m_pWordNodeNext)
			{
				GermanConjunction * gc=dynamic_cast<GermanConjunction*>(
				subSentence.m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.
				at(dwRun)->m_pWordNodeNext->m_pWord);
				if(gc)
				{
					CStringVector sv;
					sv.push_back(gc->m_strWord);
					if(bFirstTokenInSentence)
					{
						MakeFirstLetterUpper(sv.at(0));
						bFirstTokenInSentence=FALSE;
					}
					vv.push_back(sv);
				}
			}
		}
	}
	#ifdef _DEBUG
	else
		printf("!subSentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated\n");
	printf("subSentence.m_bWordOrder: %u\n",subSentence.m_bWordOrder);
	printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
	#endif
	//Satzbau ist Subjekt Prädikat Objekt
	if(subSentence.m_bWordOrder==SPO)
	{
    bool bNoGermanReflexivePronounInsertedInToTranslation=true;
    LOG("23.05.2008 11.18.09\n");//SPO=Subjekt Prädikat Objekt
		TRACE("subSentence.m_bWordOrder==SPO\n");
		#ifdef _DEBUG
		printf("m_bWordOrder==SPO\n");
		#endif
		if(subSentence.m_pGerund)
		{
			std::vector<CStringVector> vvGerund;
			#ifdef _DEBUG
			printf("subSentence.m_pGerund\n");
			#endif
			//ein Gerundium kann auch Kombinationsfelder (CComboBox) enthalten:
			//Damaging a fan is great.
			//Einen |Anhänger| zu beschädigen ist großartig.
			//      |Lüfter  |
			//zwischen den Querstrichen: Kombinationsfeld (CComboBox)
			vvGerund=TranslateGerund(*subSentence.m_pGerund,
				bFirstTokenInSentence,//pEV,
        rvecintpair,nStartID);
			//nStartID+=vvGerund.size();
			Append(vv,vvGerund);
		}//if(subSentence.m_pGerund)
		//wenn der Untersatz kein Subjekt beinhaltet, ist er ein 
		//Aufforderungssatz
		if(!subSentence.m_pSubject)
		{
		#ifdef _DEBUG
			printf("!subSentence.m_pSubject\n");
		#endif
      LOGN("19.05.2008 17.37");
      //Notlösung für das Problem ein paar Zeilen tiefer.
      if(!subSentence.m_vecPredicate.at(0).
				m_pWordNodeWhereTheGermanTranslationIsABendVerb)
      {
        vv.clear();
        return vv;
      }
			CStringVector sv;
      //TODO Fehler an dieser Stelle beim Satz "You will make no mistake.":
      // subSentence.m_vecPredicate.at(0).
		  //		m_pWordNodeWhereTheGermanTranslationIsABendVerb ist NULL
			pgvBend=dynamic_cast<GermanVerb*>(subSentence.m_vecPredicate.at(0).
				m_pWordNodeWhereTheGermanTranslationIsABendVerb->
				m_pWordNodeNext->m_pWord);
			if(pgvBend)
			{
				sv.push_back(pgvBend->m_strWords[1]);
				vv.push_back(sv);
			}
		}//if(!subSentence.m_pSubject)
		//wenn eine Umstandsbestimmung der Zeit am Anfang eines deutschen Satzes
		// steht, steht das gebeugte Verb direkt dahinter, und nicht zuerst
		// ein Subjekt Beispiel: "Today I am intelligent." ->
		// "Heute bin ich intelligent." gebeugtes Verb: "bin"
		if(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated &&
			subSentence.m_bAdverbOfTimeAtTheBeginning)
		{
			GermanAdverb * ga=dynamic_cast<GermanAdverb *>
				(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated->
				m_pWordNodeNext->m_pWord);
		#ifdef _DEBUG
			printf("subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIs\
Encapsulated &&	subSentence.m_bAdverbOfTimeAtTheBeginning\n");
		#endif
			if(ga)
			{
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				if(bFirstTokenInSentence)
				{
					MakeFirstLetterUpper(sv.at(0));
					bFirstTokenInSentence=FALSE;
				}
				vv.push_back(sv);
			}
		}
		//wenn keine Umstandsbestimmung der Zeit am Anfang eines deutschen Satzes
		//steht
		{
			#ifdef _DEBUG
			printf("nicht: Umstandsbestimmung der Zeit am Anfang des Haupt- oder Nebensatzes\n");
			#endif
			//wenn Passivsatz und der Satz ein persönliches Subjekt enthält
			if(!subSentence.m_bActiveClause && subSentence.m_pPersonalSubject)
			{
				// zum Bespiel: "I am given a car." -> Mir wird ein Auto gegeben
				BYTE b=0;
				#ifdef _DEBUG
				printf("!subSentence.m_bActiveClause && subSentence.m_pPersonalSubject\n");
				#endif
				// Beispiel: I am being given a real chance.
				//           S ------P------- ------PS-----  // PS=persönliches Subjekt
				//           Mir wird eine richtige Chance gegeben.
				std::vector<CStringVector> vvObject=TranslateSubject(
					*subSentence.m_pSubject,TRUE,b,bFirstTokenInSentence,//pEV,
          rvecintpair,nStartID);
				//nStartID+=dwNumberOfCComboBoxes;
				Append(vv,vvObject); // Subjekt
			}
			//wenn Aktivsatz
			else
			{
				TRACE("Aktivsatz\n");
        LOGN("19.05.2008 17.38");
				#ifdef _DEBUG
				printf("!(!subSentence.m_bActiveClause && subSentence.\
m_pPersonalSubject)\n");
				#endif
				//wenn nicht: "nur ein Aufzählungselement und in diesem Element ein Personal-
				//pronom der 2. Person Singular enthalten ist"
				if(subSentence.m_pSubject)
					if(!(subSentence.m_pSubject->m_vecEnumerationElement.size()==1 &&
					subSentence.m_pSubject->m_vecEnumerationElement.at(0).
						m_personalPronoun==2))
					{
						//personIndex: wird für die Beugung des deutschen Verbes benötigt
						//0=1. Person Singular ... 5=3. Person Plural
						std::vector<CStringVector> vvSubject=TranslateSubject(
							*subSentence.m_pSubject,FALSE,personIndex,
							bFirstTokenInSentence,//pEV,
              rvecintpair,nStartID);
						//nStartID+=vvSubject.size();
						Append(vv,vvSubject); // Subjekt
					}
				//else
				//	personIndex=
			}
		}//Block ohne Bedingung.
		#ifdef _DEBUG
		printf("subSentence.m_strPredicate: %s\n",subSentence.m_strPredicate);
		//TRACE("satzbau==SPO\n");
		printf("subSentence.m_bTense: %u\n",subSentence.m_bTense);
		printf("personIndex: %u\n",personIndex);
		printf("subSentence.m_vecPredicate.size(): %u\n",subSentence.
			m_vecPredicate.size());
		#endif
		CStringVector svGermanVerb;
		//wenn der "Untersatz" ein Subjekt beinhaltet (Aufforderungssätze
		//beinhalten kein Subjekt)
		if(subSentence.m_pSubject)
		{
			TRACE("enthält Subjekt\n");
      LOGN("19.05.2008 17.39");
			//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
			//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
			//           -----S---- -----P---- 
			//          "My glasses are broken." -> "Meine Brille ist kaputt."
			//           -----S---- -P- ---A--
			//S: Subjekt P: Prädikat
			//A: Umstandsbestimmung der Art und Weise
			//("broken" als Adjektiv und Partizip)
			if(subSentence.m_vecPredicate.size()==2)
			{
				CString strBendVerb;
				CString strManner;
				CStringVector sv;
				personIndex++;
				for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
				{
					Predicate predicate=subSentence.m_vecPredicate.at(bRun);
#ifdef _DEBUG
					printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
					{
						if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
							m_pWordNodeNext)
						{
							pgvNonBend=dynamic_cast<GermanVerb*>(predicate.
								m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
								m_pWordNodeNext->m_pWord);
			#ifdef _DEBUG
							printf("pgvNonBend->m_strWords[0]: %s\n",pgvNonBend->
								m_strWords[0]);
			#endif
						}
					}
#ifdef _DEBUG
					else
						printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
						if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
							m_pWordNodeNext)
							pgvBend=dynamic_cast<GermanVerb*>(predicate.
								m_pWordNodeWhereTheGermanTranslationIsABendVerb->
								m_pWordNodeNext->m_pWord);
	//#ifdef _DEBUG
					if(predicate.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
					{
						CStringVector sv;
						GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
						sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
						//vv.push_back(sv);
					}
					if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
						predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
						predicate.m_bTense==SIMPLE_PAST || // I began.
						predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
						predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
						predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
						predicate.m_bTense==PRESENT_PERFECT) // I have begun.
					{
					#ifdef _DEBUG
						printf("subSentence.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PAST || \
predicate.m_bTense==PAST_PROGRESSIVE \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
predicate.m_bTense==PRESENT_PERFECT\n");
					#endif
					//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
						if(pgvBend)
						{
							CStringVector svVerb;
							//CStringVector sv;
							if(predicate.m_bTense==SIMPLE_PRESENT || 
								predicate.m_bTense==PRESENT_PROGRESSIVE ||
								predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
								predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
#ifdef _DEBUG
								printf("predicate.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST\n");
#endif
								//wenn Aktivsatz ("I hit.") -> "Ich schlage."
								if(predicate.m_bActiveClause)
								{
#ifdef _DEBUG
									printf("predicate.m_bActiveClause\n");
#endif
									svGermanVerb=GetCStringVector2(pgvBend->
										m_strWords[personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									strManner=svGermanVerb.at(0);
									// Prädikat (Präsens)
									if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(pgvBend->
											m_strWords[personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
								//wenn Passivsatz ("I am hit.") -> "Ich werde geschlagen."
								else
								{
#ifdef _DEBUG
									printf("!predicate.m_bActiveClause\n");
#endif
									if(pgvNonBend) // Beispiel: 'can be used'
									{ 
										svGermanVerb=GetCStringVector2(pgvBend->
											m_strWords[personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präsens)
										if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											svGermanVerb=GetCStringVector2(pgvBend->
												m_strWords[personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präteritum)
										}
									}
									else // Beispiel: 'is used'
									{
#ifdef _DEBUG
										printf("!gvInfinitive\n");
#endif
										GermanVerb * pgvBend=GetGermanWordOfEnglishVerb(_T("will"));
										//svGermanVerb=GetCStringVector2(gv->words[personIndex]);
										strBendVerb=pgvBend->m_strWords[personIndex];
										//svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präsens)
										if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											//svGermanVerb=GetCStringVector2(gv->words[personIndex+6]);
											//svVerb.push_back(svGermanVerb.at(0));
											//strBendVerb=svGermanVerb.at(0);
											// Prädikat (Präteritum)
										}
									}
								}
							}
							if(predicate.m_bTense==SIMPLE_PAST || predicate.m_bTense==
								PAST_PROGRESSIVE)
							{
								if(predicate.m_bActiveClause) // Beispiel: I used water.
								{
									svGermanVerb=GetCStringVector2(pgvBend->
										m_strWords[personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
								else
								{
									if(pgvNonBend) // Beispiel: I could be used. / 
									{ // I could be being used.
										svGermanVerb=GetCStringVector2(pgvBend->
											m_strWords[personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
									}
									else // Beispiel: I was used. / I was being used.
									{
										GermanVerb * pgvBend=GetGermanWordOfEnglishVerb(
											_T("will"));
										svGermanVerb=GetCStringVector2(pgvBend->
											m_strWords[personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
									}
								}
							}
							if(predicate.m_bTense==PRESENT_PERFECT && !predicate.
								m_bActiveClause)
							{ // Beispiel: I have been used. -> Ich bin benutzt worden.
								GermanVerb * pgvBend=GetGermanWordOfEnglishVerb(_T("be"));
								svGermanVerb=GetCStringVector2(pgvBend->
									m_strWords[personIndex]);
								svVerb.push_back(svGermanVerb.at(0));
								// finite Form von 'sein' anhängen
							}
							if(predicate.m_bActiveClause && subSentence.m_pManner)
							{
#ifdef _DEBUG
								printf("predicate.m_bActiveClause && subSentence.m_pManner\n");
#endif
								/*if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
								{
									if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
										m_pWordNodeNext)
									{
										GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
											m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										if(ga)
											strManner+=_T(" ")+ga->m_strWord;
									}
								}*/
								if(predicate.m_bNegation)
									strManner+=_T(" nicht");
//								strManner+=_T(" ")+TranslateMannerAsCString(*subSentence.
								//Use "CString" / "CStdString" as left operand in order be 
								//compatible with the g++ compiler.
								strManner+=CString(" ")+TranslateMannerAsCString(*subSentence.
									m_pManner);
								//wenn ein Infinitiv oder Partizip Perfekt im Prädikat ist
								//Beispiel: "I will be broken." -> "Ich werde kaputt sein."
								//"I have been broken." -> "Ich bin kaputt gewesen."
								if(pgvNonBend)
									sv.push_back(strManner+" "+pgvNonBend->m_strWords[0]);
								//wenn kein Infinitiv im Prädikat ist
								//Beispiel: "I am broken." -> "Ich bin kaputt."
								//else
								//	sv.push_back(strManner+" "+gvInfinitive->words[0]);
							}
							//wenn passives Prädikat
							if(!predicate.m_bActiveClause)
							{
								/*if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
								{
									if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
										m_pWordNodeNext)
									{
										GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
											m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										if(ga)
											strManner+=_T(" ")+ga->m_strWord;
									}
								}*/
								//wenn das Prädikat eine Verneinung enthält ("not")
								if(predicate.m_bNegation)
									strBendVerb+=_T(" nicht");
//								strBendVerb+=_T(" ")+pgvBend->m_strWords[13];
								//Use "CString" / "CStdString" as left operand in order be 
								//compatible with the g++ compiler.
								strBendVerb+=CString(" ")+pgvBend->m_strWords[13];
							}
							// Prädikat (Präteritum)
							//vv.push_back(sv);
						}
						#ifdef _DEBUG
						else
						{
							TRACE("gv==NULL\n");
							printf("gv==NULL\n");
						}
						#endif
					}
					//if(subSentence.m_pManner)
					//	strBendVerb+=TranslateMannerAsCString(*subSentence.m_pManner);
				}
				if(strBendVerb.GetLength()>0)
					sv.push_back(strBendVerb);
				if(strManner.GetLength()>0)
					sv.push_back(strManner);
				vv.push_back(sv);
			}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind
			// der "Untersatz" enthält mehr oder weniger als 2 Prädikate
			else
			{
        LOGN("19.05.2008 17.38.30");
        //TODO schnelle Notlösung, um Absturz beim Satz "Where do you go?" zu vermeiden:
        if(subSentence.m_vecPredicate.size()==0)
        {
          vv.clear();
          return vv;
        }
				Predicate predicate=subSentence.m_vecPredicate.at(0);
				TRACE("der \"Untersatz\" enthält mehr oder weniger als 2 Prädikate\n");
#ifdef _DEBUG
				printf("predicate.size()!=2\n");
				printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
				{
#ifdef _DEBUG
					printf("predicate.m_pWordNodeWhereTheGermanTranslationIsA\
NonBendVerb\n");
#endif
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
						m_pWordNodeNext)
					{
						pgvNonBend=dynamic_cast<GermanVerb*>(predicate.
							m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
							m_pWordNodeNext->m_pWord);
#ifdef _DEBUG
						printf("pgvNonBend->m_strWords[0]: %s\n",pgvNonBend->
							m_strWords[0]);
#endif
					}
				}
#ifdef _DEBUG
				else
					printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
#ifdef _DEBUG
				{
					printf("predicate.m_pWordNodeWhereTheGermanTranslationIsA\
BendVerb\n");
#endif
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
						m_pWordNodeNext)
						pgvBend=dynamic_cast<GermanVerb*>(predicate.
							m_pWordNodeWhereTheGermanTranslationIsABendVerb->
							m_pWordNodeNext->m_pWord);
#ifdef _DEBUG
				}
#endif
				//wenn nur ein Aufzählungselement im Subjekt vorhanden ist und in 
				//diesem Element ein Personalpronom der 2. Person Singular enthalten
				//ist
				if(subSentence.m_pSubject->m_vecEnumerationElement.size()==1 &&
					subSentence.m_pSubject->m_vecEnumerationElement.at(0).
						m_personalPronoun==YOU)
				{
					BYTE bReflexive=0;
					CStringVector svSubjectAndVerb;
					CString str=_T("du ");
					TRACE("1 Aufzählungselement & 2. Person Singular\n");
          LOGN("19.05.2008 17.39");
					//IntPair intPair;
					//intPair.m_n1=nStartID;
					//intPair.m_n1=nStartID+1;
					//pEV->m_vecIntPair.push_back(intPair);
					//nStartID+=2;
					nStartID++;
					if(pgvBend)
					{
						//wenn KEIN reflexives deutsches Verb
						//zum Beispiel: "I can be used."
						if(pgvBend->m_bReflexive)
							bReflexive=1;
					}
					if(pgvNonBend)
						//wenn KEIN reflexives deutsches Verb
						//zum Beispiel: "I still need a friend."
						//zum Beispiel: "I am still used."
						if(pgvNonBend->m_bReflexive)
							bReflexive=1;
					//wenn es keine gebeugte deutsche Verbform gibt (kann auch bei
					//der Zeitform "Gegenwart" sein, nämlich wenn es ein passives
					//Prädikat ist, denn es sollen nur die für eine korrekte 
					//Übersetzung benötigten Daten in der Klasse "Predicate" 
					//gespeichert und nicht z.B. das Hilfsverb "haben" für die 
					//Zeitform "Perfekt" gespeichert, denn das Hilfsverb lässt sich
					//aus der Zeitform "Perfekt" herleiten)
					if(!pgvBend)
					{
						//wenn es ein passives Prädikat ist
						if(!predicate.m_bActiveClause)
						{
							//wenn die Zeitform "Perfekt" ("ich bin gemocht worden") 
							//oder "Plusquamperfekt" ("ich war gemocht worden") oder
							//"fortschreitendes Perfekt" ("ich bin gerade gemocht 
							//worden") oder "fortschreitendes Plusquamperfekt" ("ich 
							//war gerade gemocht worden") ist, ...
							if(predicate.m_bTense==PRESENT_PERFECT || 
								predicate.m_bTense==PAST_PERFECT ||
								predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
								predicate.m_bTense==PAST_PERFECT_PROGRESSIVE)
							{
								//...dann als gebeugte Verbform "sein" nehmen
								pgvBend=GetGermanWordOfEnglishVerb(_T("be"));
							}
							//sonst...
							else
								//als gebeugte Verbform "werden" nehmen
								pgvBend=GetGermanWordOfEnglishVerb(_T("will"));
						}
						else
						{
							//wenn die Zeitform "Zukunft" ("ich werde gehen") oder 
							//"fortschreitende Zukunft" ("ich werde gerade gehen") 
							//oder "Zukunft Perfekt" ("ich werde gegangen sein") 
							//ist, ...
							if(predicate.m_bTense==FUTURE || predicate.m_bTense==
								FUTURE_CONTINOUS || predicate.m_bTense==FUTURE_PERFECT)
							{
								//...dann als gebeugte Verbform "werden" nehmen
								pgvBend=GetGermanWordOfEnglishVerb(_T("will"));
							}
							//wenn die Zeitform "Perfekt" ("ich bin gemocht worden") 
							//oder "Plusquamperfekt" ("ich war gemocht worden") oder
							//"fortschreitendes Perfekt" ("ich bin gerade gemocht 
							//worden") oder "fortschreitendes Plusquamperfekt" ("ich 
							//war gerade gemocht worden") ist, ...
							if(predicate.m_bTense==PRESENT_PERFECT || 
								predicate.m_bTense==PAST_PERFECT || 
								predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE || 
								predicate.m_bTense==PAST_PERFECT_PROGRESSIVE)
							{
								//...dann als gebeugte Verbform "haben" nehmen
								pgvBend=GetGermanWordOfEnglishVerb(_T("have"));
							}
						}
					}
					//wenn die Zeitform "Präsens", "fortschreitendes Präsens", 
					//"fortschreitender Perfekt" oder "Präsens oder Präteritum"
					//,... ist
					if(predicate.m_bTense==SIMPLE_PRESENT || // ("I begin.")
						predicate.m_bTense==PRESENT_PROGRESSIVE || // ("I am beginning.")
						predicate.m_bTense==PRESENT_PERFECT || // ("I have begun.")
						predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE || // ("I have been beginning.")
						predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST || // ("I hit.")
						predicate.m_bTense==FUTURE || // ("I will begin.")
						predicate.m_bTense==FUTURE_CONTINOUS || // ("I will be beginning.")
						predicate.m_bTense==FUTURE_PERFECT) // ("I will have begun.")
					{
            ASSERT(pgvBend);//"pgvBend" should have been assigned above or earlier.
						//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
						//"ich sehe an"), dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
						//Beispiel: "sehen", "ich sehe")
						//CStringVector der 2. Person Singular Präsens holen
						svGermanVerb=GetCStringVector2(pgvBend->m_strWords[2]);
						str+=svGermanVerb.at(0);
						if(bReflexive)
            {
							str+=_T(" dich");
              bNoGermanReflexivePronounInsertedInToTranslation=false;
            }
						svSubjectAndVerb.push_back(str);
						//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
						//interpretiert werden kann (zum Beispiel beim Verb 
						//"to hit": Präteritum ist "hit")
						if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
							//"ich sehe an"), dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
							//Beispiel: "sehen", "ich sehe")
							//CStringVector der 2. Person Singular Präteritum holen
							svGermanVerb=GetCStringVector2(pgvBend->m_strWords[8]);
//							str=_T("du ")+svGermanVerb.at(0);
							//Use "CString" / "CStdString" as left operand in order be 
							//compatible with the g++ compiler.
							str=CString("du ")+svGermanVerb.at(0);
							if(bReflexive)
								str+=_T(" dich");
							svSubjectAndVerb.push_back(str);
							// Prädikat (Präteritum)
						}
						//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
						//"ich sehe an"), dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
						//Beispiel: "sehen", "ich sehe")
						//CStringVector der 2. Person Plural Präsens holen
						svGermanVerb=GetCStringVector2(pgvBend->m_strWords[5]);
//						str=_T("ihr ")+svGermanVerb.at(0);
						//Use "CString" / "CStdString" as left operand in order be 
						//compatible with the g++ compiler.
						str=CString("ihr ")+svGermanVerb.at(0);
						if(bReflexive)
							str+=_T(" euch");
						svSubjectAndVerb.push_back(str);
						//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
						//interpretiert werden kann (zum Beispiel beim Verb 
						//"to hit": Präteritum ist "hit")
						if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
							//"ich sehe an"), dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
							//Beispiel: "sehen", "ich sehe")
							//CStringVector der 2. Person Plural Präteritum holen
							svGermanVerb=GetCStringVector2(pgvBend->m_strWords[11]);
//							str=_T("ihr ")+svGermanVerb.at(0);
							//Use "CString" / "CStdString" as left operand in order be 
							//compatible with the g++ compiler.
							str=CString("ihr ")+svGermanVerb.at(0);
							if(bReflexive)
								str+=_T(" euch");
							svSubjectAndVerb.push_back(str);
							// Prädikat (Präteritum)
						}
						//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
						//"ich sehe an"), dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
						//Beispiel: "sehen", "ich sehe")
						//CStringVector der 3. Person Plural Präsens holen
						//für das siezen ("Sie")
						svGermanVerb=GetCStringVector2(pgvBend->m_strWords[6]);
						str=_T("Sie ")+svGermanVerb.at(0);
						//Use "CString" / "CStdString" as left operand in order be 
						//compatible with the g++ compiler.
//						str=CString("Sie ")+svGermanVerb.at(0);
						if(bReflexive)
							str+=_T(" sich");
						svSubjectAndVerb.push_back(str);
						//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
						//interpretiert werden kann (zum Beispiel beim Verb 
						//"to hit": Präteritum ist "hit")
						if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
							//"ich sehe an"), dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
							//Beispiel: "sehen", "ich sehe")
							//CStringVector der 3. Person Plural Präteritum holen
							//für das siezen ("Sie")
							svGermanVerb=GetCStringVector2(pgvBend->m_strWords[12]);
							str=_T("Sie ")+svGermanVerb.at(0);
							if(bReflexive)
								str+=_T(" sich");
							svSubjectAndVerb.push_back(str);
							// Prädikat (Präteritum)
						}
						//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
						//"ich sehe an"), dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
						//Beispiel: "sehen", "ich sehe")
						//CStringVector der 3. Person Singular Präsens holen
						//für "man" (zum Beispiel: "man sieht")
						svGermanVerb=GetCStringVector2(pgvBend->m_strWords[3]);
						str=_T("man ")+svGermanVerb.at(0);
						if(bReflexive)
							str+=_T(" sich");
						svSubjectAndVerb.push_back(str);
						//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
						//interpretiert werden kann (zum Beispiel beim Verb 
						//"to hit": Präteritum ist "hit")
						if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
							//"ich sehe an"), dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
							//Beispiel: "sehen", "ich sehe")
							//CStringVector der 3. Person Singular Präteritum holen
							//für "man" (zum Beispiel: "man sieht")
							svGermanVerb=GetCStringVector2(pgvBend->m_strWords[9]);
							str=_T("man ")+svGermanVerb.at(0);
							if(bReflexive)
								str+=_T(" sich");
							svSubjectAndVerb.push_back(str);
							// Prädikat (Präteritum)
						}
					}
					//wenn die Zeitform "Präteritum" oder "fortschreitendes 
					//Präteritum" ist
					//Präteritum: "I used water."
					//fortschreitendes Präteritum: "I was using water."
					if(predicate.m_bTense==SIMPLE_PAST || // ("I began.")
						predicate.m_bTense==PAST_PROGRESSIVE || // ("I was beginning.")
						predicate.m_bTense==PAST_PERFECT || // ("I had begun.")
						predicate.m_bTense==PAST_PERFECT_PROGRESSIVE) // ("I had been beginning.")
					{
						//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
						//"ich sehe an"), dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
						//Beispiel: "sehen", "ich sehe")
						//CStringVector der 2. Person Singular Präteritum holen
						svGermanVerb=GetCStringVector2(pgvBend->m_strWords[8]);
						str+=svGermanVerb.at(0);
						if(bReflexive)
            {
							str+=_T(" dich");
              bNoGermanReflexivePronounInsertedInToTranslation=false;
            }
						svSubjectAndVerb.push_back(str);
						//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
						//"ich sehe an"), dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
						//Beispiel: "sehen", "ich sehe")
						//CStringVector der 2. Person Plural Präteritum holen
						svGermanVerb=GetCStringVector2(pgvBend->m_strWords[11]);
						str=_T("ihr ")+svGermanVerb.at(0);
						if(bReflexive)
							str+=_T(" euch");
						svSubjectAndVerb.push_back(str);
						//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
						//"ich sehe an"), dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
						//Beispiel: "sehen", "ich sehe")
						//CStringVector der 3. Person Plural Präteritum holen
						//für das siezen ("Sie")
						svGermanVerb=GetCStringVector2(pgvBend->m_strWords[12]);
						str=_T("Sie ")+svGermanVerb.at(0);
						if(bReflexive)
							str+=_T(" sich");
						svSubjectAndVerb.push_back(str);
						//wenn zusammengesetztes Verb (zum Beispiel: "ansehen",
						//"ich sehe an"), dann entsteht ein Vektor aus
						//2 Elementen. Sonst hat der Vektor nur 1 Element (zum 
						//Beispiel: "sehen", "ich sehe")
						//CStringVector der 3. Person Singular Präteritum holen
						//für "man" (zum Beispiel: "man sieht")
						svGermanVerb=GetCStringVector2(pgvBend->m_strWords[9]);
						str=_T("man ")+svGermanVerb.at(0);
						if(bReflexive)
							str+=_T(" sich");
						svSubjectAndVerb.push_back(str);
					}
					vv.push_back(svSubjectAndVerb);
					/*
					// gv ist NULL, wenn der 'UnterSatz' ein Gerundium ist
					// deshalb vorher gv prüfen, bevor gv->m_bReflexive
					// abgefragt wird, sonst gibt es einen Absturz
					if(gv)
					{
						//wenn KEIN reflexives deutsches Verb
						//zum Beispiel: "I can be used."
						if(!gv->m_bReflexive)
						{
							//wenn ein Adverb im Prädikat enthalten ist
							//zum Beispiel: "I can still be used." Adverb: "still"
							if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
							{
								GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
									m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
									m_pWordNodeNext->m_pWord);
								svSubjectAndVerb.clear();
								svSubjectAndVerb.push_back(ga->m_strWord);
								vv.push_back(svSubjectAndVerb);
							}
						}
					}
					if(gvInfinitive)
						//wenn KEIN reflexives deutsches Verb
						//zum Beispiel: "I still need a friend."
						//zum Beispiel: "I am still used."
						if(!gvInfinitive->m_bReflexive)
						{
							//wenn ein Adverb im Prädikat enthalten ist
							//zum Beispiel: "I still need a friend." Adverb: "still"
							//zum Beispiel: "I am still used." Adverb: "still"
							//bei "The world still turns." muss das Adverb allerdings
							//erst hinter dem "sich" stehen. Also: "Die Welt dreht sich
							//immer noch."
							if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
							{
								GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
									m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
									m_pWordNodeNext->m_pWord);
								svSubjectAndVerb.clear();
								svSubjectAndVerb.push_back(ga->m_strWord);
								vv.push_back(svSubjectAndVerb);
							}
						}*/
				}
				//wenn nicht: "wenn nur ein Aufzählungselement im Subjekt vorhanden ist 
				//und in diesem Element ein Personalpronom der 2. Person Singular 
				//enthalten ist"
				else
				{
					vv.push_back(GetGermanBendVerb(pgvBend,predicate,personIndex));
				}
			}
		}//if(subSentence.m_pSubject)
		#ifdef _DEBUG
		printf("vor: gv->m_bReflexive\n");
		#endif
    if(bNoGermanReflexivePronounInsertedInToTranslation)
    {
		  // gv ist NULL, wenn der 'UnterSatz' ein Gerundium ist
		  // deshalb vorher gv prüfen, bevor gv->m_bReflexive
		  // abgefragt wird, sonst gibt es einen Absturz
		  if(pgvBend)
		  { 
			  //wenn ein reflexives deutsches Verb
			  //zum Beispiel: "I can be used."
			  if(pgvBend->m_bReflexive)
			  { // wenn nur 1 Verb im Satz, dann ist gv das Prädikat
				  // wenn 2 Verben im Satz (zum Beispiel: 'can' und 'hide',
				  // dann ist auch gvInfinitive initialisiert
				  CStringVector sv;
          LOGN("23.05.2008 11.18.27");//=German Bend Verb
				  if(subSentence.m_pSubject->m_personIndex==1) // I
					  sv.push_back(_T("mich"));
				  if(subSentence.m_pSubject->m_personIndex==2) // you
				  {
					  sv.push_back(_T("dich"));
					  sv.push_back(_T("euch"));
					  sv.push_back(_T("sich"));
				  }
				  if(subSentence.m_pSubject->m_personIndex==3) // he/she/it
					  sv.push_back(_T("sich"));
				  if(subSentence.m_pSubject->m_personIndex==4) // we
					  sv.push_back(_T("uns"));
				  if(subSentence.m_pSubject->m_personIndex==6) // they
					  sv.push_back(_T("sich"));
				  vv.push_back(sv);
				  //wenn ein Adverb im Prädikat enthalten ist
				  //zum Beispiel: "I can still be used." Adverb: "still"
				  /*if(subSentence.m_vecPredicate.at(0).
					  m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
				  {
					  GermanAdverb * ga=dynamic_cast<GermanAdverb*>(subSentence.
						  m_vecPredicate.at(0).
						  m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
						  m_pWordNodeNext->m_pWord);
					  sv.clear();
					  sv.push_back(ga->m_strWord);
					  vv.push_back(sv);
				  }*/
			  }
		  }//if(pgvBend)
		  #ifdef _DEBUG
		  printf("vor: if(gvInfinitive)\n");
		  #endif
		  if(pgvNonBend)
			  //wenn ein reflexives deutsches Verb
			  //zum Beispiel: "I still need a friend."
			  //zum Beispiel: "I am still used."
			  if(pgvNonBend->m_bReflexive)
			  {
				  CStringVector sv;
          LOGN("13.06.2008 17.15.53");
          if(subSentence.m_pSubject->m_personIndex==1) // I
					  sv.push_back(_T("mich"));
				  if(subSentence.m_pSubject->m_personIndex==2) // you
				  {
					  sv.push_back(_T("dich"));
					  sv.push_back(_T("euch"));
					  sv.push_back(_T("sich"));
				  }
				  if(subSentence.m_pSubject->m_personIndex==3) // he/she/it
					  sv.push_back(_T("sich"));
				  if(subSentence.m_pSubject->m_personIndex==4) // we
					  sv.push_back(_T("uns"));
				  if(subSentence.m_pSubject->m_personIndex==6) // they
					  sv.push_back(_T("sich"));
				  vv.push_back(sv);
				  //wenn ein Adverb im Prädikat enthalten ist
				  //zum Beispiel: "I still need a friend." Adverb: "still"
				  //zum Beispiel: "I am still used." Adverb: "still"
				  //bei "The world still turns." muss das Adverb allerdings
				  //erst hinter dem "sich" stehen. Also: "Die Welt dreht sich
				  //immer noch."
				  /*if(subSentence.m_vecPredicate.at(0).
					  m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
				  {
					  GermanAdverb * ga=dynamic_cast<GermanAdverb*>(subSentence.
						  m_vecPredicate.at(0).
						  m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
						  m_pWordNodeNext->m_pWord);
					  sv.clear();
					  sv.push_back(ga->m_strWord);
					  vv.push_back(sv);
				  }*/
			  }//if(pgvNonBend->m_bReflexive)
    }//if(bNoGermanReflexivePronounInsertedInToTranslation)
    LOG("23.05.2008 11.18.22");//=After Subject
		//wenn Umstandsbestimmung der Zeit am Ende des Satzes
		//Beispiel: "I went to school yesterday."
		//Umstandsbestimmung der Zeit: "yesterday"
		if(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated &&
			!subSentence.m_bAdverbOfTimeAtTheBeginning)
		{ // Umstandsbestimmung der Zeit
			GermanAdverb * ga=dynamic_cast<GermanAdverb *>
				(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			if(ga)
			{
				CStringVector sv;
        //Wird nach einem Reflexivpronomen eingefügt: "Er wäscht sich heute."
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		//wenn der Satz eine Umstandsbestimmung der Zeit enthält, die sich 
		//am Satzanfang befindet
		//Beispiel: "Yesterday I went to school."
		//Umstandsbestimmung der Zeit: "Yesterday"
		if(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated &&
			subSentence.m_bAdverbOfTimeAtTheBeginning)
		{
			// wenn Passivsatz und der Satz ein persönliches Subjekt enthält 
			// zum Bespiel: "Tomorrow I am given a car." -> "Morgen wird mir ein 
			//Auto gegeben."
			if(!subSentence.m_bActiveClause && subSentence.m_pPersonalSubject)
			{ 
				BYTE b=0;
				#ifdef _DEBUG
				printf("!subSentence.m_bActiveClause && subSentence.\
m_pPersonalSubject\n");
				#endif
				// Beispiel: Tomorrow I am being given a real chance.
				//           S ------P------- ------PS-----  // PS=persönliches Subjekt
				//           Morgen wird mir eine richtige Chance gegeben.
				std::vector<CStringVector> vvObject=TranslateSubject(
					*subSentence.m_pSubject,TRUE,b,b,//pEV,
          rvecintpair,nStartID);
				//nStartID+=vvObject.size();
				Append(vv,vvObject); // Subjekt
			}
			else
			{
				BYTE b=0;
				std::vector<CStringVector> vvSubject=TranslateSubject(
					*subSentence.m_pSubject,TRUE,b,b,//pEV,
          rvecintpair,nStartID);
				//nStartID+=vvSubject.size();
				Append(vv,vvSubject); // Subjekt
			}
		}//Ende von: wenn der Satz eine Umstandsbestimmung der Zeit am Satzanfang enthält

		#ifdef _DEBUG
		printf("vor: subSentence.m_pByAgent\n");
		#endif
		//wenn der "Untersatz" einen By-Agent beinhaltet. Beispiel: "I am hit by 
		//my mother." By-Agent: "by my mother"
		//by-Agent sind nur bei Passivsätzen möglich, glaube ich
		if(subSentence.m_pByAgent)
		{
			std::vector<CStringVector> vvByAgent;
			vvByAgent=TranslateByAgent(*subSentence.m_pByAgent,nStartID);
			//nStartID+=vvByAgent.size();
			Append(vv,vvByAgent);
			//CStringVector sv;
			//sv.push_back("von");
			//vv.push_back(sv);
			
		}
		#ifdef _DEBUG
		printf("vor: subSentence.m_pPersonalSubject\n");
		#endif
		//wenn der "Untersatz" ein persönliches Subjekt beinhaltet
		//zum Bespiel: "I am given a car." -> "Mir wird ein Auto gegeben."
		if(subSentence.m_pPersonalSubject)
		{
			BYTE b=0;
			std::vector<CStringVector> vvPersonalSubject=TranslateSubject(
				*subSentence.m_pPersonalSubject,TRUE,b,b,//pEV,
        rvecintpair,nStartID);
			//nStartID+=vvPersonalSubject.size();
			Append(vv,vvPersonalSubject);
		}

#ifdef _DEBUG
		printf("vor: if(gv)\n");
#endif
		//gv ist NULL, wenn der 'UnterSatz' ein Gerundium ist deshalb vorher gv
		//prüfen, bevor gv->m_strPreposition verglichen wird, sonst gibt es einen 
		//Absturz
		if(pgvBend)
		{
			if(pgvBend->m_strPreposition!="")
			{
				CStringVector sv;
				sv.push_back(pgvBend->m_strPreposition);
				vv.push_back(sv);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanClause,germanObject); // Subjekt
		#ifdef _DEBUG
		printf("vor Objekt\n");
		#endif
		BOOL bNotInserted=FALSE;
		// wenn der "UnterSatz" ein erstes Objekt enthält (kann maximal 2 
		//Objekte enthalten)
		if(subSentence.m_pObjectFirst)
		{
      LOG("23.05.2008 11.18.34\n");//=First Object
			#ifdef _DEBUG
			printf("subSentence.m_pObjectFirst->m_vecEnumerationElement.size()>0\n");
			#endif
			/*if(subSentence.m_vecPredicate.at(0).m_bNegation && subSentence.
				m_pObjectFirst->m_vecEnumerationElement.at(0).m_pGerund)
			{
				bNotInserted=TRUE;
				CStringVector sv;
				sv.push_back(_T("nicht"));
				vv.push_back(sv);
			}*/
			BYTE Case=0;
			if(pgvNonBend)
				Case=pgvNonBend->m_bCase;
			else if(pgvBend)
				Case=pgvBend->m_bCase;
			// wenn das Verb 1 Objekt im vierten Fall oder 2 Objekte verlangt
			if(Case==1 || Case==3)
			{ // im dritten Fall übersetzen
				BYTE b=0;
				//3. Parameter: nur wenn kein 2. Objekt existiert, das Attribut 
				//"subSentence.m_vecPredicate.at(0).m_bNegation" einsetzen, sonst
				//bNegation (Verneinung) auf "0"
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectFirst,1,subSentence.m_pObjectSecond==NULL?
					subSentence.m_vecPredicate.at(0).m_bNegation:0,b,//pEV,
          rvecintpair,nStartID);
				//nStartID+=vvObject.size();
				Append(vv,vvObject); // Objekt
			}
			else if(Case==2)
			{ // im vierten Fall übersetzen
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectFirst,2,subSentence.m_vecPredicate.at(0).
					m_bNegation,b,//pEV,
          rvecintpair,nStartID);
				//nStartID+=vvObject.size();
				Append(vv,vvObject); // Objekt
			}
			else
			{ // mit keinem Fall übersetzen
				BYTE b=0;
				//3. Parameter: bNegation (Verneinung) auf "0"
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectFirst,0,subSentence.m_vecPredicate.at(0).
					m_bNegation,b,//pEV,
          rvecintpair,nStartID);
				//nStartID+=vvObject.size();
				Append(vv,vvObject); // Objekt
			}
			/*if(subSentence.m_vecPredicate.at(0).m_bNegation && !subSentence.
				m_pObjectFirst->m_vecEnumerationElement.at(0).m_pGerund)
			{
				bNotInserted=TRUE;
				CStringVector sv;
				sv.push_back(_T("nicht"));
				vv.push_back(sv);
			}*/
		}// Ende von: wenn der Satz ein erstes Objekt enthält (kann maximal 2 Objekte 
		//enthalten)
		#ifdef _DEBUG
		else
			printf("!subSentence.m_pObjectFirst\n");
		printf("vor 2. Objekt\n");
		#endif
		//Adverb der Häufigkeit einfügen:
		//wenn ein Verb 2 Objekte verlangt, steht das Adverb der Häufigkeit zwischen
		//dem ersten und dem zweiten Objekt: "I sometimes gave him a book." ->
		//"Ich gab ihm manchmal ein Buch."
		//wenn der "Untersatz" ein Subjekt beinhaltet (wenn der "Untersatz" ein
		//Aufforderungssatz ist, besitzt er kein Subjekt)
		if(subSentence.m_pSubject)
		{
      LOG("23.05.2008 11.18.45\n");//S=Subject
			//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
			//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
			//           -----S---- -----P----
			//          "My glasses are broken." -> "Meine Brille ist kaputt."
			//           -----S---- -P- ---A--
			//S: Subjekt P: Prädikat
			//A: Umstandsbestimmung der Art und Weise
			if(subSentence.m_vecPredicate.size()==2)
			{
				for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
				{
					Predicate predicate=subSentence.m_vecPredicate.at(bRun);
					if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
					{
						if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
							m_pWordNodeNext)
						{
							GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
								m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
								m_pWordNodeNext->m_pWord);
							if(ga)
							{
								CStringVector sv;
								sv.push_back(ga->m_strWord);
								vv.push_back(sv);
							}
						}
					}
				}
			}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind
			// der "Untersatz" enthält mehr oder weniger als 2 Prädikate
			else
			{
				Predicate predicate=subSentence.m_vecPredicate.at(0);
				//wenn das Prädikat ein Adverb beinhaltet
				if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
				{
					CStringVector sv;
					GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
						m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
						m_pWordNodeNext->m_pWord);
					//Das Adverb steht direkt hinter dem 1. Objekt, oder, wenn kein 
					//Objekt existiert, direkt hinter der gebeugten Verbform:
					//"I sometimes go." -> "Ich gehe manchmal."
					//   -Adverb--              -GV- -Adverb-
					//"I sometimes find you." -> "Ich finde dich manchmal."
					//                                -GV-- -O--
					//"I sometimes give you a book." -> "Ich gebe dir manchmal ein Buch."
					//             -GV- -O- --O---           -GV- -O-          ---O----
					//GV: gebeugte Verbform      O:Objekt
					sv.push_back(ga->m_strWord);
					vv.push_back(sv);
				}
				//Verneinung "nicht" einfügen: nur wenn alle Aufzaählungselemente
				//des betreffenden Objekte einen unbestimmten Artikel haben
				if(subSentence.m_pObjectSecond)
				{
					for(DWORD dwRun=0;dwRun<subSentence.m_pObjectSecond->
						m_vecEnumerationElement.size();++dwRun)
					{
						if(!subSentence.m_pObjectSecond->m_vecEnumerationElement.
							at(dwRun).m_pEnr)
						{
							//wenn das Prädikat eine Negation enthält, steht die Verneinung vor 
							//dem Adverb der Häufigkeit zwischen
							//dem ersten und dem zweiten Objekt: "I did not sometimes give him 
							//a book." -> "Ich gab ihm nicht manchmal ein Buch."
							/*if(predicate.m_bNegation)
							{
								CStringVector sv;
								sv.push_back(_T("nicht"));
								vv.push_back(sv);
							}*/
						}
						else
						{
							/*if(!subSentence.m_pObjectSecond->m_vecEnumerationElement.
								at(dwRun).m_pEnr->bIndefiniteArticle)
								//wenn das Prädikat eine Negation enthält, steht die Verneinung vor 
								//dem Adverb der Häufigkeit zwischen
								//dem ersten und dem zweiten Objekt: "I did not sometimes give him 
								//a book." -> "Ich gab ihm nicht manchmal ein Buch."
								if(predicate.m_bNegation)
								{
									CStringVector sv;
									sv.push_back(_T("nicht"));
									vv.push_back(sv);
								}*/
						}
					}
				}
				else
				{
					if(subSentence.m_pObjectFirst)
					{
						for(DWORD dwRun=0;dwRun<subSentence.m_pObjectFirst->
							m_vecEnumerationElement.size();++dwRun)
						{
							if(!subSentence.m_pObjectFirst->m_vecEnumerationElement.
								at(dwRun).m_pEnr)
							{
								//wenn das Prädikat eine Negation enthält, steht die Verneinung vor 
								//dem Adverb der Häufigkeit zwischen
								//dem ersten und dem zweiten Objekt: "I did not sometimes give him 
								//a book." -> "Ich gab ihm nicht manchmal ein Buch."
								/*if(predicate.m_bNegation)
								{
									CStringVector sv;
									sv.push_back(_T("nicht"));
									vv.push_back(sv);
								}*/
							}
							else
							{
								/*if(!subSentence.m_pObjectFirst->m_vecEnumerationElement.
									at(dwRun).m_pEnr->bIndefiniteArticle)
									//wenn das Prädikat eine Negation enthält, steht die Verneinung vor 
									//dem Adverb der Häufigkeit zwischen
									//dem ersten und dem zweiten Objekt: "I did not sometimes give him 
									//a book." -> "Ich gab ihm nicht manchmal ein Buch."
									if(predicate.m_bNegation)
									{
										CStringVector sv;
										sv.push_back(_T("nicht"));
										vv.push_back(sv);
									}*/
							}
						}
					}
				}
				/*if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
				{
					if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
						m_pWordNodeNext)
					{
						CStringVector sv;
						GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
							m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
							m_pWordNodeNext->m_pWord);
						if(ga)
						{
							sv.push_back(ga->m_strWord);
							vv.push_back(sv);
						}
					}
				}*/
			}
		}
		// wenn der Satz ein zweites Objekt enthält (dann enthält er 2 Objekte)
		if(subSentence.m_pObjectSecond) // 2. Objekt
      //Z.B. "I give you a book."
      //             1.O --2.O-  O=Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectSecond\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
				//printf("gv
			BYTE b=0;
			//3. Parameter
			std::vector<CStringVector> vvObject=TranslateObject2(
				*subSentence.m_pObjectSecond,2,subSentence.m_vecPredicate.at(0).
				m_bNegation,b,//pEV,
        rvecintpair,nStartID);
			//nStartID+=vvObject.size();
			Append(vv,vvObject); // Objekt
		}// Ende von: wenn der Satz ein zweites Objekt enthält (dann enthält er 2 Objekte)
		// wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		/*if(subSentence.m_bBe)
		{
			if(subSentence.m_bNegation)
			{
				if(subSentence.m_pObjectFirst)
					if(!subSentence.m_pObjectFirst->m_vecEnumerationElement.at(0).
						m_pGerund && !bNotInserted)
					{ // wenn kein Gerundium
						CStringVector sv;
						sv.push_back("nicht");
						vv.push_back(sv);
						bNotInserted=TRUE;
					}
				if(!bNotInserted)
				{
					CStringVector sv;
					sv.push_back(_T("nicht"));
					vv.push_back(sv);
				}
			}
		}
		if(!subSentence.m_bBe)
			if(subSentence.m_bNegation && !bNotInserted)
			{
				CStringVector sv;
				sv.push_back(_T("nicht"));
				vv.push_back(sv);
			}*/
		for(DWORD i=0;i<subSentence.m_vecEnglishAdverb.size();i++) // Adverbien
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		#ifdef _DEBUG
		printf("vor Art und Weise\n");
		#endif
		//wenn der "Untersatz" eine Umstandsbestimmung der Art und Weise beinhaltet
		//Beispiel: "He looks good." Umstandsbestimmung der Art und Weise: "good"
		if(subSentence.m_pManner)
		{
			if(subSentence.m_pManner->m_vecMannerEnumerationElement.size()>0 
				 && subSentence.m_vecPredicate.size()!=2)
			{
				std::vector<CStringVector> vvManner=TranslateManner(*subSentence.
					m_pManner,nStartID);
				//nStartID+=vvManner.size();
				Append(vv,vvManner);
			}
		}
		#ifdef _DEBUG
		else
			printf("!subSentence.m_pManner\n");
		#endif
		#ifdef _DEBUG
		printf("vor: for(i=0;i<subSentence.m_vecMiddleEnumerationElement.\
size();i++)\n");
		#endif
		for(i=0;i<subSentence.m_vecMiddleEnumerationElement.size();i++)
		{
			if(i>0)
				if((i+1)==subSentence.m_vecMiddleEnumerationElement.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				//sv.push_back(subSentence.m_MiddleEnumerationElements.at(i).m_strAdverbRefersToAdjectiveOrAdverb);
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).
					m_strAdverbRefersToAdjectiveOrAdverb);
				if(typeid(*pWord)==typeid(GermanAdverb)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdverb * ga=dynamic_cast<GermanAdverb*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strWord);
				}
				vv.push_back(sv);
			}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
		if(svGermanVerb.size()>1)
		{
			#ifdef _DEBUG
			printf("svPredicate.size()>1\n");
			#endif
			CStringVector sv;
			sv.push_back(svGermanVerb.at(1)); // 2. Wort des Vektors anhängen
			vv.push_back(sv);
		}
		//Append(vv,vvSubject=TranslateObject2(subSentence.m_Object)); // Objekt
#ifdef _DEBUG
		printf("vor der Prüfung auf subSentence.m_pRelativeClause\n");
#endif
		//wenn der "UnterSatz" einen Relativsatz enthält
		/*if(subSentence.m_pRelativeClause)
		{
#ifdef _DEBUG
			printf("subSentence.m_pRelativeClause\n");
#endif
			//wenn sich der Relativsatz aufs Objekt bezieht
			if(!subSentence.m_pRelativeClause->m_bRefersToSubject)
				if(subSentence.m_pObjectFirst)
				{
					std::vector <GermanNoun> vecGermanNoun=GetGermanWordOfEnglishNoun2(
						subSentence.m_pObjectFirst->m_vecEnumerationElement.at(0).
						m_pDan->m_Noun);
					std::vector<CStringVector> vvRelativeClause=
						TranslateRelativeClause(*subSentence.m_pRelativeClause,nStartID);
					if(vecGermanNoun.size()>0)
					{
						CStringVector sv;
						sv.push_back(_T(","));
						vv.push_back(sv);
						sv.clear();
						GermanNoun gn=vecGermanNoun.at(0);
						if(gn.m_bArticle=='1')
							sv.push_back(_T("den"));
						else if(gn.m_bArticle=='2')
							sv.push_back(_T("die"));
						else if(gn.m_bArticle=='3')
							sv.push_back(_T("das"));
						vv.push_back(sv);
					}
					//nStartID+=vvRelativeClause.size();
					Append(vv,vvRelativeClause);
				}
		}*/
#ifdef _DEBUG
		printf("vor der Prüfung auf subSentence.m_pPlace\n");
#endif
		if(subSentence.m_pPlace) // Umstandsbestimmung des Ortes
		{
			BYTE b=0;
			std::vector<CStringVector> vvPlace=TranslatePlace(
				*subSentence.m_pPlace,b,nStartID);
#ifdef _DEBUG
			printf("subSentence.m_pPlace\n");
#endif
			//nStartID+=vvPlace.size();
			Append(vv,vvPlace);
		}
		//Den Rest des Prädikates (ohne die gebeugte Verbform) anhängen.
		//Wenn die Zeitform "Zukunft" ist, muss der Infinitiv am Ende des Satzes
		//stehen: "I will go very fast to school tomorrow." -> "Ich werde morgen
		//sehr schnell zur Schule gehen." (in diesem Fall steht der Infinitiv 
		//"gehen" am Ende des Satzes)
		if(subSentence.m_pSubject)
		{
      LOG("23.05.2008 11.19.13\n");//SBP=Subject Before Predicate
			//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
			//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
			//           -----S---- -----P----
			//          "My glasses are broken." -> "Meine Brille ist kaputt."
			//           -----S---- -P- ---A--
			//S: Subjekt P: Prädikat
			//A: Umstandsbestimmung der Art und Weise
			if(subSentence.m_vecPredicate.size()==2)
			{
				CString strBendVerb;
				CString strManner;
				CStringVector sv;
#ifdef _DEBUG
				printf("predicate.size()==2\n");
#endif
				personIndex++;
				for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
				{
					Predicate predicate=subSentence.m_vecPredicate.at(bRun);
					if(predicate.m_bTense=FUTURE && predicate.m_bActiveClause)
					{
#ifdef _DEBUG
					printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
						if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
						{
							if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
								m_pWordNodeNext)
							{
								CStringVector sv;
								pgvBend=dynamic_cast<GermanVerb*>(predicate.
									m_pWordNodeWhereTheGermanTranslationIsABendVerb->
									m_pWordNodeNext->m_pWord);
			#ifdef _DEBUG
								printf("pgvBend->m_strWords[0]: %s\n",pgvBend->m_strWords[0]);
			#endif
								sv.push_back(pgvBend->m_strWords[INDEX_FOR_GERMAN_INFINITIVE]); // Infinitiv
								vv.push_back(sv); // anhängen
							}
						}
#ifdef _DEBUG
						else
							printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
					}
				}
			}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind
			//der "Untersatz" enthält mehr oder weniger als 2 Prädikate
			else
			{
				std::vector<CStringVector> vvRestOfPredicate=
					TranslateRestOfGermanPredicate(subSentence.m_vecPredicate.at(0));
				//die Verneinung ("nein") erscheint im Deutschen am Ende eines
				//Haupt- oder Nebensatzes:
				//"I do not like you."->"Ich mag dich nicht."
				//"I can not like you."->"Ich kann dich nicht mögen."
				//"I am not liked."->"Ich werde nicht gemocht."
				//"I can not be liked."->"Ich kann nicht gemocht werden."
				//"I did not like you."->"Ich mochte dich nicht."
				//"I was not liked."->"Ich wurde nicht gemocht."
				//"I will not like you."->"Ich werde dich nicht mögen."
				//"I will not be liked."->"Ich werde nicht gemocht werden."
				//"I will not be walking."->"Ich werde nicht gelaufen sein."
				//"I will not be being liked."->"Ich werde nicht gemocht worden sein."
				//"I will not have liked you."->"Ich werde dich nicht gemocht haben."
				//"I will not have been liked."->"Ich werde nicht gemocht worden sein."
				//"I am not walking."->"Ich laufe (gerade) nicht."
				//"I am not being liked."->"Ich werde (gerade) nicht gemocht."
				//"I was not walking."->"Ich lief (gerade) nicht."
				//"I was not being liked."->"Ich wurde (gerade) nicht gemocht."
				//"I have not liked you."->"Ich habe dich nicht gemocht."
				//"I can not have seen you."->"Ich kann dich nicht gesehen haben."
				//"I have not been liked."->"Ich bin nicht gemocht worden."
				//"I can not have been liked."->"Ich kann nicht gemocht worden sein."
				//"I had not liked you."->"Ich hatte dich nicht gemocht."
				//"I could not have seen you."->"Ich konnte dich nicht gesehen haben."
				//"I had not been liked."->"Ich war nicht gemocht worden."
				//"I could not have been liked."->"Ich konnte nicht gemocht worden sein."
				//"I have not been liking you."->"Ich habe dich (gerade) nicht gemocht."
				//"I can not have been liking you."->"Ich kann dich (gerade) nicht gemocht haben."
				//"I was not been liking."->"Ich bin (gerade) nicht gemocht worden."
				//"I can not have been liking."->"Ich kann (gerade) nicht gemocht worden sein."
				//"I had not been liking you."->"Ich hatte dich (gerade) nicht gemocht."
				//"I could not have been liking you."->"Ich konnte dich (gerade) nicht gemocht haben.")
				//"I had not been liking."->"Ich war (gerade) nicht gemocht worden."
				//"I could not have been liking."->"Ich konnte (gerade) nicht gemocht worden sein."
				if(subSentence.m_vecPredicate.at(0).m_bNegation)
				{
					CStringVector sv;
					sv.push_back(_T("nicht")); // Infinitiv
					vv.push_back(sv); // anhängen
				}
#ifdef _DEBUG
				printf("subSentence.m_vecPredicate.size()!=2\n");
				printf("subSentence.m_vecPredicate.at(0).m_bTense:%u \n",
					subSentence.m_vecPredicate.at(0).m_bTense);
#endif
				Append(vv,vvRestOfPredicate);
			}
		}
		/*if((subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==SIMPLE_PAST 
			|| subSentence.m_bTense==PRESENT_PERFECT || subSentence.m_bTense==
			PRESENT_PROGRESSIVE || subSentence.m_bTense==PAST_PROGRESSIVE || 
			subSentence.m_bTense==FUTURE) && subSentence.m_vecPredicate.size()!=2)
		{ // wenn die Zeitform Präsens ist
			#ifdef _DEBUG
			printf("subSentence.m_bTense==SIMPLE_PRESENT || subSentence.m_bTense==\
SIMPLE_PAST || subSentence.m_bTense==PRESENT_PERFECT || subSentence.m_bTense==\
PRESENT_PROGRESSIVE || subSentence.m_bTense==PAST_PROGRESSIVE || subSentence\
.m_bTense==FUTURE\n");
			#endif
			if(!subSentence.m_bActiveClause) // wenn Passivsatz
			{

				CStringVector sv;
				if(gvInfinitive) // 
					sv.push_back(gvInfinitive->m_strWords[13]); // Partizip Perfekt anhängen (z.B. 'gemocht')
				else
					sv.push_back(gv->m_strWords[13]); // Partizip Perfekt anhängen (z.B. 'gemocht')
				vv.push_back(sv);
			}
		}
		if(subSentence.m_bTense==PRESENT_PERFECT && subSentence.m_bActiveClause)
		{ // wenn Perfekt und Aktivsatz
			CStringVector sv;
			sv.push_back(gv->m_strWords[13]); // Partizip Perfekt anhängen (z.B. 'gemocht')
			vv.push_back(sv);
		}
		if(!subSentence.m_bActiveClause) // wenn Passivsatz
		{
			if(subSentence.m_bTense==PRESENT_PERFECT) // wenn die Zeitform Perfekt ist
			{
				#ifdef _DEBUG
				printf("subSentence.m_bTense==PRESENT_PERFECT\n");
				#endif
				CStringVector sv;
				sv.push_back("worden");
				vv.push_back(sv);
			}
			if(subSentence.m_bTense==FUTURE || gvInfinitive) // wenn die Zeitform Zukunft ist
			{
				#ifdef _DEBUG
				printf("subSentence.m_bTense==PRESENT_PERFECT\n");
				#endif
				CStringVector sv;
				sv.push_back("werden");
				vv.push_back(sv);
			}
		}
		//if((subSentence.tense==FUTURE || gvInfinitive) && subSentence.m_bActiveClause)
		if(gvInfinitive && subSentence.m_bActiveClause)
		{ // wenn Aktivsatz 
			CStringVector sv;
			sv.push_back(gvInfinitive->m_strWords[0]); // Infinitiv anhängen
			vv.push_back(sv);
		}*/
	}//Satzbau: SPO
	
  //wenn der Satzbau "Subjekt Objekt Prädikat" ist
	//zum Beispiel: "Weil ich dich liebe."
	//                    -S- -O-- --P--
	//S=Subjekt O=Objekt P=Prädikat
	if(subSentence.m_bWordOrder==SOP)
	{
    LOG("23.05.2008 11.19.19\n");//SOP=Subjekt Objekt Prädikat
		TRACE("subSentence.m_bWordOrder==SOP\n");
		#ifdef _DEBUG
		printf("subSentence.m_bWordOrder==SOP\n");
		#endif
		CStringVector svGermanBendVerb;
		BYTE b=0;
    std::vector<IntPair> vecintpair;
		//personIndex: wird für die Beugung des deutschen Verbes benötigt
		//0=1. Person Singular ... 5=3. Person Plural
		std::vector<CStringVector> vvSubject=TranslateSubject(
			*subSentence.m_pSubject,FALSE,personIndex,bFirstTokenInSentence,//NULL,
      vecintpair,
			nStartID);
		//nStartID+=vvSubject.size();
		Append(vv,vvSubject); // Subjekt
		#ifdef _DEBUG
		printf("vor Art und Weise\n");
		#endif
		if(subSentence.m_pManner) // Umstandsbestimmung der Art und Weise
		{
			if(subSentence.m_pManner->m_vecMannerEnumerationElement.size()>0) // Objekt
			{
				std::vector<CStringVector> vvManner=TranslateManner(*subSentence.
					m_pManner,nStartID);
				//nStartID+=vvManner.size();
				Append(vv,vvManner);
			}
		}
		#ifdef _DEBUG
		else
			printf("!subSentence.m_pManner\n");
		#endif
		if(subSentence.m_vecPredicate.size()==1)
		{
			Predicate predicate=subSentence.m_vecPredicate.at(0);
#ifdef _DEBUG
			printf("predicate.size()!=2\n");
			printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
			{
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
					m_pWordNodeNext)
				{
					pgvBend=dynamic_cast<GermanVerb*>(predicate.
						m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
						m_pWordNodeNext->m_pWord);
			#ifdef _DEBUG
					printf("pgvBend->m_strWords[0]: %s\n",pgvBend->m_strWords[0]);
		#endif
				}
			}
#ifdef _DEBUG
				else
				printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
					m_pWordNodeNext)
					if(!pgvBend)
						pgvBend=dynamic_cast<GermanVerb*>(predicate.
							m_pWordNodeWhereTheGermanTranslationIsABendVerb->
							m_pWordNodeNext->m_pWord);
					else
						pgvNonBend=dynamic_cast<GermanVerb*>(predicate.
							m_pWordNodeWhereTheGermanTranslationIsABendVerb->
							m_pWordNodeNext->m_pWord);
			if(predicate.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
			{
				CStringVector sv;
				GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
			}
			if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
				predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
				predicate.m_bTense==SIMPLE_PAST || // I began.
				predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
				predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
				predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
				predicate.m_bTense==PRESENT_PERFECT) // I have begun.
			{
				#ifdef _DEBUG
				printf("subSentence.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PAST || \
predicate.m_bTense==PAST_PROGRESSIVE \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
predicate.m_bTense==PRESENT_PERFECT\n");
				#endif
					//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
				if(pgvBend!=NULL)
				{
					CStringVector svVerb;
					if(predicate.m_bTense==SIMPLE_PRESENT || 
						predicate.m_bTense==PRESENT_PROGRESSIVE ||
						predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
						predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
					{
						if(predicate.m_bActiveClause)
						{
#ifdef _DEBUG
							printf("predicate.m_bActiveClause\n");
#endif
						}
						//wenn Passivsatz
						else
						{
							if(pgvNonBend) // Beispiel: 'can be used'
							{ 
							}
							else // Beispiel: 'is used'
							{
								GermanVerb * pgvBend=GetGermanWordOfEnglishVerb(_T("will"));
							}
						}
					}
					if(predicate.m_bTense==SIMPLE_PAST || predicate.m_bTense==
						PAST_PROGRESSIVE)
					{
						if(predicate.m_bActiveClause) // Beispiel: I used water.
						{
						}
						else
						{
							if(pgvNonBend) // Beispiel: I could be used. / 
							{ // I could be being used.
							}
							else // Beispiel: I was used. / I was being used.
							{
								GermanVerb * pgvBend=GetGermanWordOfEnglishVerb(_T("will"));
							}
						}
					}
					if(predicate.m_bTense==PRESENT_PERFECT && !predicate.m_bActiveClause)
					{ // Beispiel: I have been used. -> Ich bin benutzt worden.
						GermanVerb * pgvBend=GetGermanWordOfEnglishVerb(_T("be"));
						// finite Form von 'sein' anhängen
					}
					// Prädikat (Präteritum)
				}
			}
		}//if(subSentence.m_vecPredicate.size()==1)
		if(subSentence.m_pObjectFirst) // Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectFirst\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(pgvBend)
			{
				//printf("gv
				if(pgvBend->m_bCase==1)
				{
					BYTE b=0;
					std::vector<CStringVector> vvObject=TranslateObject2(
						*subSentence.m_pObjectFirst,1,subSentence.m_bNegation,b,//pEV,
            rvecintpair,nStartID);
					//nStartID+=vvObject.size();
					Append(vv,vvObject); // Objekt
				}
				else if(pgvBend->m_bCase==2)
				{
					BYTE b=0;
					std::vector<CStringVector> vvObject=TranslateObject2(
						*subSentence.m_pObjectFirst,2,subSentence.m_bNegation,b,//pEV,
            rvecintpair,nStartID);
					//nStartID+=vvObject.size();
					Append(vv,vvObject); // Objekt
				}
				else
				{
					BYTE b=0;
					std::vector<CStringVector> vvObject=TranslateObject2(
						*subSentence.m_pObjectFirst,0,subSentence.m_bNegation,b,//pEV,
            rvecintpair,nStartID);
					//nStartID+=vvObject.size();
					Append(vv,vvObject); // Objekt
				}
			}
		}
		#ifdef _DEBUG
		else
			printf("!subSentence.m_pObjectFirstm_enumerationElements.size()<=0\n");
		#endif
		//Adverb der Häufigkeit einfügen:
		//wenn ein Verb 2 Objekte verlangt, steht das Adverb der Häufigkeit zwischen
		//dem ersten und dem zweiten Objekt: "If I sometimes gave him a book." ->
		//"Wenn ich ihm manchmal ein Buch gab."
		if(subSentence.m_pSubject)
		{
			//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
			//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
			//           -----S---- -----P----
			//          "My glasses are broken." -> "Meine Brille ist kaputt."
			//           -----S---- -P- ---A--
			//S: Subjekt P: Prädikat
			//A: Umstandsbestimmung der Art und Weise
			if(subSentence.m_vecPredicate.size()==2)
			{
				for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
				{
					Predicate predicate=subSentence.m_vecPredicate.at(bRun);
					if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
					{
						if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
							m_pWordNodeNext)
						{
							GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
								m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
								m_pWordNodeNext->m_pWord);
							if(ga)
							{
								CStringVector sv;
								sv.push_back(ga->m_strWord);
								vv.push_back(sv);
							}
						}
					}
				}
			}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind
			// der "Untersatz" enthält mehr oder weniger als 2 Prädikate
			else
			{
				Predicate predicate=subSentence.m_vecPredicate.at(0);
				if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
				{
					if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
						m_pWordNodeNext)
					{
						CStringVector sv;
						GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
							m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
							m_pWordNodeNext->m_pWord);
						if(ga)
						{
							sv.push_back(ga->m_strWord);
							vv.push_back(sv);
						}
					}
				}
			}
		}
		if(subSentence.m_pObjectSecond) // 2. Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectSecond\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(pgvBend)
			{
				//printf("gv
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectSecond,2,subSentence.m_bNegation,b,//pEV,
          rvecintpair,nStartID);
				//nStartID+=vvObject.size();
				Append(vv,vvObject); // Objekt
			}
		}
		//Den Rest des Prädikates (ohne die gebeugte Verbform) anhängen.
		//Wenn die Zeitform "Zukunft" ist, muss der Infinitiv am Ende des 
		//Satzes stehen: "Because I will go very fast to school tomorrow." 
		//-> "Weil ich morgen sehr schnell zur Schule gehen werde." 
		//(in diesem Fall steht der Infinitiv "gehen" am Ende des Satzes)
		if(subSentence.m_pSubject)
		{
			//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
			//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
			//           -----S---- -----P----
			//          "My glasses are broken." -> "Meine Brille ist kaputt."
			//           -----S---- -P- ---A--
			//S: Subjekt P: Prädikat
			//A: Umstandsbestimmung der Art und Weise
			if(subSentence.m_vecPredicate.size()==2)
			{
				CString strBendVerb;
				CString strManner;
				CStringVector sv;
#ifdef _DEBUG
				printf("predicate.size()==2\n");
#endif
				personIndex++;
				for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
				{
					Predicate predicate=subSentence.m_vecPredicate.at(bRun);
					if(predicate.m_bTense=FUTURE && predicate.m_bActiveClause)
					{
#ifdef _DEBUG
					printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
						if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
						{
							if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
								m_pWordNodeNext)
							{
								CStringVector sv;
								pgvBend=dynamic_cast<GermanVerb*>(predicate.
									m_pWordNodeWhereTheGermanTranslationIsABendVerb->
									m_pWordNodeNext->m_pWord);
			#ifdef _DEBUG
								printf("pgvBend->m_strWords[0]: %s\n",pgvBend->m_strWords[0]);
			#endif
								sv.push_back(pgvBend->m_strWords[0]); // Infinitiv
								vv.push_back(sv); // anhängen
							}
						}
#ifdef _DEBUG
						else
							printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
					}
				}
			}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind
			//der "Untersatz" enthält mehr oder weniger als 2 Prädikate
			else
			{
				std::vector<CStringVector> vvRestOfPredicate=
					TranslateRestOfGermanPredicate(subSentence.m_vecPredicate.at(0));
				//die Verneinung ("nein") erscheint im Deutschen am Ende eines
				//Haupt- oder Nebensatzes:
				//"I do not like you."->"Ich mag dich nicht."
				//"I can not like you."->"Ich kann dich nicht mögen."
				//"I am not liked."->"Ich werde nicht gemocht."
				//"I can not be liked."->"Ich kann nicht gemocht werden."
				//"I did not like you."->"Ich mochte dich nicht."
				//"I was not liked."->"Ich wurde nicht gemocht."
				//"I will not like you."->"Ich werde dich nicht mögen."
				//"I will not be liked."->"Ich werde nicht gemocht werden."
				//"I will not be walking."->"Ich werde nicht gelaufen sein."
				//"I will not be being liked."->"Ich werde nicht gemocht worden sein."
				//"I will not have liked you."->"Ich werde dich nicht gemocht haben."
				//"I will not have been liked."->"Ich werde nicht gemocht worden sein."
				//"I am not walking."->"Ich laufe (gerade) nicht."
				//"I am not being liked."->"Ich werde (gerade) nicht gemocht."
				//"I was not walking."->"Ich lief (gerade) nicht."
				//"I was not being liked."->"Ich wurde (gerade) nicht gemocht."
				//"I have not liked you."->"Ich habe dich nicht gemocht."
				//"I can not have seen you."->"Ich kann dich nicht gesehen haben."
				//"I have not been liked."->"Ich bin nicht gemocht worden."
				//"I can not have been liked."->"Ich kann nicht gemocht worden sein."
				//"I had not liked you."->"Ich hatte dich nicht gemocht."
				//"I could not have seen you."->"Ich konnte dich nicht gesehen haben."
				//"I had not been liked."->"Ich war nicht gemocht worden."
				//"I could not have been liked."->"Ich konnte nicht gemocht worden sein."
				//"I have not been liking you."->"Ich habe dich (gerade) nicht gemocht."
				//"I can not have been liking you."->"Ich kann dich (gerade) nicht gemocht haben."
				//"I was not been liking."->"Ich bin (gerade) nicht gemocht worden."
				//"I can not have been liking."->"Ich kann (gerade) nicht gemocht worden sein."
				//"I had not been liking you."->"Ich hatte dich (gerade) nicht gemocht."
				//"I could not have been liking you."->"Ich konnte dich (gerade) nicht gemocht haben.")
				//"I had not been liking."->"Ich war (gerade) nicht gemocht worden."
				//"I could not have been liking."->"Ich konnte (gerade) nicht gemocht worden sein."
				if(subSentence.m_vecPredicate.at(0).m_bNegation)
				{
					CStringVector sv;
					sv.push_back(_T("nicht")); // Infinitiv
					vv.push_back(sv); // anhängen
				}
#ifdef _DEBUG
				printf("subSentence.m_vecPredicate.size()!=2\n");
				printf("subSentence.m_vecPredicate.at(0).m_bTense:%u \n",
					subSentence.m_vecPredicate.at(0).m_bTense);
#endif
				Append(vv,vvRestOfPredicate);
			}
		}//if(subSentence.m_pSubject)
		//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
		//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
		//           -----S---- -----P----
		//          "My glasses are broken." -> "Meine Brille ist kaputt."
		//           -----S---- -P- ---A--
		//S: Subjekt P: Prädikat
		//A: Umstandsbestimmung der Art und Weise
		if(subSentence.m_vecPredicate.size()==2)
		{
			CString strBendVerb;
			CString strManner;
			CStringVector sv;
			CStringVector svGermanVerb;
			personIndex++;
			for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
			{
				Predicate predicate=subSentence.m_vecPredicate.at(bRun);
#ifdef _DEBUG
				printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
				{
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
						m_pWordNodeNext)
					{
						pgvBend=dynamic_cast<GermanVerb*>(predicate.
							m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
							m_pWordNodeNext->m_pWord);
		#ifdef _DEBUG
						printf("pgvBend->m_strWords[0]: %s\n",pgvBend->m_strWords[0]);
		#endif
					}
				}
#ifdef _DEBUG
				else
					printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
						m_pWordNodeNext)
						if(!pgvBend)
							pgvBend=dynamic_cast<GermanVerb*>(predicate.
								m_pWordNodeWhereTheGermanTranslationIsABendVerb->
								m_pWordNodeNext->m_pWord);
						else
							pgvNonBend=dynamic_cast<GermanVerb*>(predicate.
								m_pWordNodeWhereTheGermanTranslationIsABendVerb->
								m_pWordNodeNext->m_pWord);
//#ifdef _DEBUG
				if(predicate.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
				{
					CStringVector sv;
					GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
					sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
					//vv.push_back(sv);
				}
				if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
					predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
					predicate.m_bTense==SIMPLE_PAST || // I began.
					predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
					predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
					predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
					predicate.m_bTense==PRESENT_PERFECT) // I have begun.
				{
				#ifdef _DEBUG
					printf("subSentence.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PAST || \
predicate.m_bTense==PAST_PROGRESSIVE \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
predicate.m_bTense==PRESENT_PERFECT\n");
				#endif
				//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
					if(pgvBend!=NULL)
					{
						CStringVector svVerb;
						//CStringVector sv;
						if(predicate.m_bTense==SIMPLE_PRESENT || 
							predicate.m_bTense==PRESENT_PROGRESSIVE ||
							predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
#ifdef _DEBUG
							printf("predicate.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST\n");
#endif
							//wenn Aktivsatz ("I hit.") -> "Ich schlage."
							if(predicate.m_bActiveClause)
							{
#ifdef _DEBUG
								printf("predicate.m_bActiveClause\n");
#endif
								svGermanVerb=GetCStringVector2(pgvBend->
									m_strWords[personIndex]);
								svVerb.push_back(svGermanVerb.at(0));
								strManner=svGermanVerb.at(0);
								// Prädikat (Präsens)
								if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									svGermanVerb=GetCStringVector2(pgvBend->
										m_strWords[personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präteritum)
								}
							}
							//wenn Passivsatz ("I am hit.") -> "Ich werde geschlagen."
							else
							{
#ifdef _DEBUG
								printf("!predicate.m_bActiveClause\n");
#endif
								if(pgvNonBend) // Beispiel: 'can be used'
								{ 
									svGermanVerb=GetCStringVector2(pgvBend->
										m_strWords[personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präsens)
									if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(pgvBend->
											m_strWords[personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
								else // Beispiel: 'is used'
								{
#ifdef _DEBUG
									printf("!gvInfinitive\n");
#endif
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									//svGermanVerb=GetCStringVector2(gv->words[personIndex]);
									strBendVerb=gv->m_strWords[personIndex];
									//svVerb.push_back(svGermanVerb.at(0));
									// Prädikat (Präsens)
									if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										//svGermanVerb=GetCStringVector2(gv->words[personIndex+6]);
										//svVerb.push_back(svGermanVerb.at(0));
										//strBendVerb=svGermanVerb.at(0);
										// Prädikat (Präteritum)
									}
								}
							}
						}
						if(predicate.m_bTense==SIMPLE_PAST || predicate.m_bTense==
							PAST_PROGRESSIVE)
						{
							if(predicate.m_bActiveClause) // Beispiel: I used water.
							{
								svGermanVerb=GetCStringVector2(pgvBend->
									m_strWords[personIndex+6]);
								svVerb.push_back(svGermanVerb.at(0));
							}
							else
							{
								if(pgvNonBend) // Beispiel: I could be used. / 
								{ // I could be being used.
									svGermanVerb=GetCStringVector2(pgvBend->
										m_strWords[personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
								else // Beispiel: I was used. / I was being used.
								{
									GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
									svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
							}
						}
						if(predicate.m_bTense==PRESENT_PERFECT && !predicate.
							m_bActiveClause)
						{ // Beispiel: I have been used. -> Ich bin benutzt worden.
							GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
							svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex]);
							svVerb.push_back(svGermanVerb.at(0));
							// finite Form von 'sein' anhängen
						}
						if(predicate.m_bActiveClause && subSentence.m_pManner)
						{
#ifdef _DEBUG
							printf("predicate.m_bActiveClause && subSentence.m_pManner\n");
#endif
							if(predicate.m_bNegation)
								strManner+=_T(" nicht");
							strManner+=_T(" ")+TranslateMannerAsCString(*subSentence.
								m_pManner);
							//wenn ein Infinitiv oder Partizip Perfekt im Prädikat ist
							//Beispiel: "I will be broken." -> "Ich werde kaputt sein."
							//"I have been broken." -> "Ich bin kaputt gewesen."
							if(pgvNonBend)
								sv.push_back(strManner+" "+pgvNonBend->m_strWords[0]);
							//wenn kein Infinitiv im Prädikat ist
							//Beispiel: "I am broken." -> "Ich bin kaputt."
							//else
							//	sv.push_back(strManner+" "+gvInfinitive->words[0]);
						}
						//wenn passives Prädikat
						if(!predicate.m_bActiveClause)
						{
							//wenn das Prädikat eine Verneinung enthält ("not")
							if(predicate.m_bNegation)
								strBendVerb+=_T(" nicht");
							strBendVerb+=_T(" ")+pgvBend->m_strWords[13];
						}
						// Prädikat (Präteritum)
						//vv.push_back(sv);
					}
					#ifdef _DEBUG
					else
					{
						TRACE("gv==NULL\n");
						printf("gv==NULL\n");
					}
					#endif
				}
				//if(subSentence.m_pManner)
				//	strBendVerb+=TranslateMannerAsCString(*subSentence.m_pManner);
			}
			if(strBendVerb.GetLength()>0)
				sv.push_back(strBendVerb);
			if(strManner.GetLength()>0)
				sv.push_back(strManner);
			vv.push_back(sv);
		}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind.
		//Der "Untersatz" enthält mehr oder weniger als 2 Prädikate
		else
		{
			CStringVector svGermanVerb;
			Predicate predicate=subSentence.m_vecPredicate.at(0);
#ifdef _DEBUG
			printf("predicate.size()!=2\n");
			printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
			{
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
					m_pWordNodeNext)
				{
					pgvBend=dynamic_cast<GermanVerb*>(predicate.
						m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
						m_pWordNodeNext->m_pWord);
		#ifdef _DEBUG
					printf("pgvBend->m_strWords[0]: %s\n",pgvBend->m_strWords[0]);
	#endif
				}
			}
#ifdef _DEBUG
			else
				printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
					m_pWordNodeNext)
					if(!pgvBend)
						pgvBend=dynamic_cast<GermanVerb*>(predicate.
							m_pWordNodeWhereTheGermanTranslationIsABendVerb->
							m_pWordNodeNext->m_pWord);
					else
						pgvNonBend=dynamic_cast<GermanVerb*>(predicate.
							m_pWordNodeWhereTheGermanTranslationIsABendVerb->
							m_pWordNodeNext->m_pWord);
			//wenn nur ein Aufzählungselement im Subjekt vorhanden ist und in 
			//diesem Element ein Personalpronom der 2. Person Singular enthalten
			//ist
			if(subSentence.m_pSubject->m_vecEnumerationElement.size()==1 &&
				subSentence.m_pSubject->m_vecEnumerationElement.at(0).
					m_personalPronoun==YOU)
			{
				BYTE bReflexive=0;
				CString str;
				IntPair intPair;
				intPair.m_n1=nStartID;
				intPair.m_n2=nStartID+1;
				//pEV->m_vecIntPair.push_back(intPair);
        rvecintpair.push_back(intPair);
        //The current 2 pull-down lists are connected. So the next pull-down list
        //ID starts 2 indices later.
				nStartID+=2;
				if(pgvBend)
				{
					//wenn KEIN reflexives deutsches Verb
					//zum Beispiel: "I can be used."
					if(pgvBend->m_bReflexive)
						bReflexive=1;
				}
				if(pgvNonBend)
					//wenn KEIN reflexives deutsches Verb
					//zum Beispiel: "I still need a friend."
					//zum Beispiel: "I am still used."
					if(pgvNonBend->m_bReflexive)
						bReflexive=1;
				if(predicate.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
				{
					CStringVector sv;
					GermanVerb * gvWill=GetGermanWordOfEnglishVerb(_T("will"));
					str+=gvWill->m_strWords[INDEX_FOR_DU_PRAESENS];
					if(bReflexive)
						str=_T("dich ");
					sv.push_back(str);
					str=gvWill->m_strWords[INDEX_FOR_IHR_PRAESENS];
					if(bReflexive)
						str=_T("euch ");
					sv.push_back(str);
					str=gvWill->m_strWords[INDEX_FOR_PLURAL_SIE_PRAESENS];
					if(bReflexive)
						str=_T("sich ");
					sv.push_back(str);
					str=gvWill->m_strWords[INDEX_FOR_ER_SIE_ES_PRAESENS];
					if(bReflexive)
						str=_T("sich ");
					sv.push_back(str);
					vv.push_back(sv);
				}
				if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
					predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
					predicate.m_bTense==SIMPLE_PAST || // I began.
					predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
					predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
					predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
					predicate.m_bTense==PRESENT_PERFECT) // I have begun.
				{
#ifdef _DEBUG
					printf("subSentence.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PAST || \
predicate.m_bTense==PAST_PROGRESSIVE \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
predicate.m_bTense==PRESENT_PERFECT\n");
#endif
					//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
					if(pgvBend!=NULL)
					{
						CStringVector svSubjectAndVerb;
						//wenn die Zeitform "Präsens", "fortschreitendes Präsens", 
						//"fortschreitender Perfekt" oder "Präsens oder Präteritum" ist
						if(predicate.m_bTense==SIMPLE_PRESENT || 
							predicate.m_bTense==PRESENT_PROGRESSIVE ||
							predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
							predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
						{
							GermanVerb * gvBend;
							//wenn Aktivsatz
							if(predicate.m_bActiveClause)
							{
#ifdef _DEBUG
								printf("predicate.m_bActiveClause\n");
#endif
								gvBend=pgvBend;
							}
							else
							{
								if(pgvNonBend) // Beispiel: 'can be used'
									gvBend=pgvBend;
								else // Beispiel: 'is used'
									gvBend=dynamic_cast<GermanVerb*>(
										GetGermanWordOfEnglishVerb(_T("will")));
							}
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							//String-Vektor aus der 2. Person Singular ("du") Präsens bilden
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[2]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn du dich wäschst."
							if(bReflexive)
								str=_T("dich ");
							str+=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
							//interpretiert werden kann
							// Prädikat (Präsens)
							if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                  INDEX_FOR_DU_PRAETERITUM]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst 
								//das Reflexiv-Pronom, dann die gebeugte Verbform. 
								//Beispiel: "Wenn du dich wäschst."
								if(bReflexive)
									str=_T("dich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								// Prädikat (Präteritum)
							}
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                INDEX_FOR_IHR_PRAESENS]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn du dich wäschst."
							if(bReflexive)
								str=_T("euch ");
							str+=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
							//interpretiert werden kann
							// Prädikat (Präsens)
							if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                  INDEX_FOR_IHR_PRAETERITUM]);
								//bei dem Satzbau "SOP" steht zuerst das Reflexiv-Pronom, dann
								//die gebeugte Verbform. Beispiel: "Wenn du dich wäschst."
								if(bReflexive)
									str=_T("euch ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								// Prädikat (Präteritum)
							}
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                INDEX_FOR_PLURAL_SIE_PRAESENS]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn du dich wäschst."
							if(bReflexive)
								str=_T("sich ");
							str+=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
							//interpretiert werden kann
							// Prädikat (Präsens)
							if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                  INDEX_FOR_PLURAL_SIE_PRAETERITUM]);
								//bei dem Satzbau "SOP" steht zuerst das Reflexiv-Pronom, dann
								//die gebeugte Verbform. Beispiel: "Wenn du dich wäschst."
								if(bReflexive)
									str=_T("sich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								// Prädikat (Präteritum)
							}
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                INDEX_FOR_ER_SIE_ES_PRAESENS]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform.
							if(bReflexive)
                //Für die mögliche Übersetzung "man" für "you": z.B. "Wenn man sich sieht."
								str=_T("sich ");
							str+=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
							//interpretiert werden kann
							// Prädikat (Präsens)
							if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                  INDEX_FOR_ER_SIE_ES_PRAETERITUM]);
								//bei dem Satzbau "SOP" steht zuerst das Reflexiv-Pronom, dann
								//die gebeugte Verbform. Beispiel: "Wenn du dich wäschst."
								if(bReflexive)
									str=_T("sich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								// Prädikat (Präteritum)
							}
								/*//wenn ein Adverb im Prädikat enthalten ist
								//zum Beispiel: "I still need a friend." Adverb: "still"
								if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
								{
									GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
										m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
										m_pWordNodeNext->m_pWord);
									vv.push_back(svVerb);
									svVerb.clear();
									svVerb.push_back(ga->m_strWord);
								}*/
						//Wenn die Zeitform "Präsens", "fortschreitendes Präsens", 
						//"fortschreitender Perfekt" oder "Präsens oder Präteritum" ist.
						}
						//Wenn die Zeitform "Präteritum" oder "fortschreitendes Präteritum"
						//ist.
						//Präteritum: "I used water."
						//fortschreitendes Präteritum: "I was using water."
						if(predicate.m_bTense==SIMPLE_PAST || predicate.m_bTense==
							PAST_PROGRESSIVE)
						{
							GermanVerb * gvBend;
							//wenn Aktivsatz Beispiel: "I used water."
							if(predicate.m_bActiveClause)
							{
#ifdef _DEBUG
								printf("predicate.m_bActiveClause\n");
#endif
								gvBend=pgvBend;
							}
							else
							{
								if(pgvNonBend) // Beispiel: 'can be used'
									gvBend=pgvBend;
								else // Beispiel: 'is used'
									gvBend=dynamic_cast<GermanVerb*>(
										GetGermanWordOfEnglishVerb(_T("will")));
							}
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                INDEX_FOR_DU_PRAETERITUM]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn du dich wuschst."
							if(bReflexive)
								str=_T("dich ");
							str=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[INDEX_FOR_IHR_PRAETERITUM]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn ihr euch wuscht."
							if(bReflexive)
								str+=_T("euch ");
							str=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                INDEX_FOR_PLURAL_SIE_PRAETERITUM]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn Sie sich wuschen."
							if(bReflexive)
								str=_T("sich ");
							str+=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                INDEX_FOR_ER_SIE_ES_PRAETERITUM]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn man sich wusch."
							if(bReflexive)
								str=_T("sich ");
							str+=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
						//Wenn die Zeitform "Präteritum" oder "fortschreitendes Präteritum"
						//ist.
						}
						//Wenn die Zeitform "Perfekt" ist und der Haupt-/Nebensatz ein
						//Aktivsatz ist.
						// Beispiel: I have used you. -> Ich habe dich benutzt.
						if(predicate.m_bTense==PRESENT_PERFECT)
						{
							GermanVerb * gvBend;
							//wenn Aktivsatz Beispiel: "I used water."
							if(predicate.m_bActiveClause)
							{
#ifdef _DEBUG
								printf("predicate.m_bActiveClause\n");
#endif
								gvBend=pgvBend;
							}
							else
							{
								if(pgvNonBend) // Beispiel: 'can be used'
									gvBend=pgvBend;
								else // Beispiel: 'is used'
									gvBend=dynamic_cast<GermanVerb*>(
										GetGermanWordOfEnglishVerb(_T("be")));
							}
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[INDEX_FOR_DU_PRAESENS]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn du dich gewaschen hast."
							if(bReflexive)
								str=_T("dich ");
							str+=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[INDEX_FOR_IHR_PRAESENS]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn ihr euch gewaschen habt."
							if(bReflexive)
								str=_T("euch ");
							str+=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[
                INDEX_FOR_PLURAL_SIE_PRAESENS]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn Sie sich gewaschen haben."
							if(bReflexive)
								str=_T("sich ");
							str+=svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
							//Zeichenkette leeren
							str.Empty();
							//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
							//2 Elementen. Sonst hat der Vektor nur 1 Element.
							svGermanVerb=GetCStringVector2(gvBend->m_strWords[INDEX_FOR_ER_SIE_ES_PRAESENS]);
							//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
							//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
							//"Wenn man sich gewaschen hat."
							if(bReflexive)
								str=_T(" sich");
							str=_T("man ")+svGermanVerb.at(0);
							svSubjectAndVerb.push_back(str);
						//Wenn die Zeitform "Perfekt" ist.
            }
						// Prädikat (Präteritum)
						vv.push_back(svSubjectAndVerb);
						//// gv ist NULL, wenn der 'TeilSatz' ein Gerundium ist
						//// deshalb vorher gv prüfen, bevor gv->m_bReflexive
						//// abgefragt wird, sonst gibt es einen Absturz
						//if(pgvBend)
						//{
						//	//wenn KEIN reflexives deutsches Verb
						//	//zum Beispiel: "I can be used."
						//	if(!pgvBend->m_bReflexive)
						//	{
						//		//wenn ein Adverb im Prädikat enthalten ist
						//		//zum Beispiel: "I can still be used." Adverb: "still"
						//		if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
						//		{
						//			GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
						//				m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
						//				m_pWordNodeNext->m_pWord);
						//			svSubjectAndVerb.clear();
						//			svSubjectAndVerb.push_back(ga->m_strWord);
						//			vv.push_back(svSubjectAndVerb);
						//		}
						//	}
						//}
						//if(pgvNonBend)
						//	//wenn KEIN reflexives deutsches Verb
						//	//zum Beispiel: "I still need a friend."
						//	//zum Beispiel: "I am still used."
						//	if(!pgvNonBend->m_bReflexive)
						//	{
						//		//wenn ein Adverb im Prädikat enthalten ist
						//		//zum Beispiel: "I still need a friend." Adverb: "still"
						//		//zum Beispiel: "I am still used." Adverb: "still"
						//		//bei "The world still turns." muss das Adverb allerdings
						//		//erst hinter dem "sich" stehen. Also: "Die Welt dreht sich
						//		//immer noch."
						//		if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
						//		{
						//			GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
						//				m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
						//				m_pWordNodeNext->m_pWord);
						//			svSubjectAndVerb.clear();
						//			svSubjectAndVerb.push_back(ga->m_strWord);
						//			vv.push_back(svSubjectAndVerb);
						//		}
						//	}
					}
				}
			//wenn nur ein Aufzählungselement im Subjekt vorhanden ist und in 
			//diesem Element ein Personalpronom der 2. Person Singular enthalten
			//ist
			//if(subSentence.m_pSubject->m_vecEnumerationElement.size()==1 &&
			//	subSentence.m_pSubject->m_vecEnumerationElement.at(0).
			//		m_personalPronoun==2)
			}
			//wenn nicht: wenn nur ein Aufzählungselement im Subjekt vorhanden ist 
			//und in diesem Element ein Personalpronom der 2. Person Singular 
			//enthalten ist
			else
			{
				std::vector<CStringVector> vvRestOfPredicate=
					TranslateRestOfGermanPredicate(subSentence.m_vecPredicate.at(0));
#ifdef _DEBUG
				printf("predicate.size()!=2\n");
				printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
				Append(vv,vvRestOfPredicate);
				vv.push_back(GetGermanBendVerb(pgvBend,predicate,personIndex));
			}
			//vv.push_back(sv);
			if(predicate.m_bNegation)
			{
				CStringVector sv;
				sv.push_back(_T("nicht"));
				vv.push_back(sv);
			}
		}
	}//Satzbau: SOP
	
  //wenn der Satzbau "Prädikat Subjekt Objekt" ist
	//zum Beispiel: "Darum liebe ich dich."
	//                     --P-- -S- -O--
	//S=Subjekt O=Objekt P=Prädikat
	if(subSentence.m_bWordOrder==PSO)
	{
    LOG("23.05.2008 11.19.25\n");//SOP=Prädikat Subjekt Objekt
		Append(vv,TranslatePSO(subSentence,bFirstTokenInSentence,//pEV,
      rvecintpair,nStartID));
		TRACE("satzbau==PSO\n");

	}
	if(subSentence.m_bWordOrder==SOP)
	{
	}
	for(DWORD i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
	{
		if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='3')
		{
			GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
				m_vecEnglishAdverb.at(i).m_strWord);
			CStringVector sv;
			sv.push_back(ga->m_strWord);
			vv.push_back(sv);
		}
	}
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateClause2(Clause \
& subSentence) - ENDE\n");
	#endif
	return vv;
}

//wird von TranslateAutomatically aufgerufen
std::vector<CStringVector> TranslateText(Text & text,
	std::vector<Range> & rVecRange,const DWORD & dwFlags,//EnglishView * pEV
  std::vector<IntPair> & rvecintpair)
{
	CString strText;
	int nStartID=ID_USER+100;
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateText(Text & text,\
std::vector<Range> & rVecRange,const DWORD & dwFlags) - ANFANG\n");
	printf("nStartID: %u\n",nStartID);
	#endif
	g_dwTranslated=0;
	text.GetLength();
	//if(pEV)
	//	//damit sich eine Veränderung der Auswahl in einem Kombinationsfeld
	//	//auf ein anderes Kombinationsfeld auswirkt, wenn ein Subjekt 
	//	//"du|ihr|Sie|man" ist, muss pEV->m_dwHighestID
	//	//wieder auf den Anfangswert für die IDs der Steuerelemente in einem
	//	//EnglishView gesetzt werden. Alle nachher erstellten Kombinationsfelder
	//	//kriegen eine um 1 erhöhte ID.
	//	pEV->m_dwHighestID=ID_USER+100;
	if(text.GetCString(strText,0,text.GetLength()-1))
	{
		#ifdef _DEBUG
		printf("text.GetCString(strText,0,text.GetLength()-1)\n");
		printf("strText: %s\n",strText);
		#endif
		strText.Replace(13,' '); // Carriage Returns durch Leerzeichen ersetzen
		strText.Replace(10,' '); // Newlines durch Leerzeichen ersetzen
		PositionCStringVector psvText=GetPositionCStringVector(strText,0);
		g_dwTotal=psvText.size();
		#ifdef _DEBUG
		printf("psvText.size(): %u g_dwTotal: %u\n",psvText.size(),g_dwTotal);
		#endif
	}
	std::vector<CStringVector> vv;
	TextNode * current=text.m_first;
	DWORD dwStart=0;
	DWORD dwSize=text.GetLength();
	TRACE("dwSize: %u\n",dwSize);
	//g_dwTotal=text.GetLength();

	for(int i=0;i<dwSize;i++)
	{
    LOG("23.05.2008 11.19.33 "<<i<<"\n");
		//wenn Punkt (mögliches Satzendezeichen für einen Aussagesatz)
		if(current->ch=='.')
		{
			TRACE("if(current->ch=='.')\n");
      LOG("23.05.2008 11.19.43\n");
			CString strSentence;
			if(text.GetCString(strSentence,dwStart,i))
			{
				TRACE("strSentence: %s\n",strSentence);
				strSentence.Replace(13,' '); // Carriage Returns durch Leerzeichen ersetzen
				strSentence.Replace(10,' '); // Newlines durch Leerzeichen ersetzen
				CString strTransformed=Transform(strSentence);
				CStringVector svSentence=GetCStringVector(strTransformed);
				PositionCStringVector psvOriginalSentence=GetPositionCStringVector(
					strSentence,dwStart);
				PositionCStringVector psvSentence=psvOriginalSentence;
				//letztes Vektor-Element (den Punkt) löschen
				psvSentence.erase(psvSentence.end()-1);
				g_dwTokensInSentence=psvSentence.size();
				Sentence sentence;
				TRACE("strTransformed: %s\n",strTransformed);
				std::vector<Range> vecRange;
        LOG("23.05.2008 11.19.59\n");
				if(ParseSentence(sentence,psvSentence,vecRange,dwFlags))//bCaseSensitive))
				{
          LOG("23.05.2008 11.20.06\n");//VPS=Valid ParseSentence
					Append(vv,TranslateSentence2(sentence,//pEV,
            rvecintpair,nStartID));
					svSentence.clear();
					dwStart=i+1;
          LOG("23.05.2008 11.20.13\n");//BGPCSV=Before GetPositionCStringVector
					PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
					for(DWORD j=0;j<pcstrv.size();j++)
					{
						PositionCString pcstr=pcstrv.at(j);
						TRACE("pcstr.m_Str: %s  pcstr.m_dwStart: %u,pcstr.m_dwLenght: \
%u\n",pcstr.m_Str,pcstr.m_dwStart,pcstr.m_dwLength);
					}
          LOG("23.05.2008 11.20.20\n");//AL=After Loop
					//if(vecRange.size())
					//	rVecRange.insert(rVecRange.end()-1,vecRange.begin(),vecRange.end()-1);
					for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
						rVecRange.push_back(vecRange.at(dwRun));
          LOG("23.05.2008 11.20.26\n");//EOVPS=End Of Valid ParseSentence
				}
				//der überprüfte Satz ist grammatikalisch inkorrekt
				else
				{
					if(g_bContinue)
					{
						for(DWORD dwRun=dwStart;dwRun<i;dwRun++)
						{
							//die Zeichen, die der grammatikalisch inkorrekte Satz beinhaltet,
							//rot (die "1" wird in EnglishView::OnDraw als rot interpretiert)
							//markieren
							text.SetColor(dwRun,1);
						}
#ifdef _DEBUG
						printf("dwFlags: %u dwFlags&2: %u\n",dwFlags,dwFlags&2);
#endif
						//der Ausdruck "dwFlags&2" muss geklammert werden!
						//wenn grammatikalisch inkorrekte Sätze in den Zieltext übernommen
						//werden sollen
						//if((dwFlags&2)==2)
						{
							CStringVector sv;
							std::vector<CStringVector> vecvecCString=
								GetCStringVectorVector(strTransformed);
#ifdef _DEBUG
							printf("(dwFlags&2)==2\n");
#endif
							sv.push_back(_T("."));					
							vecvecCString.push_back(sv);
							Append(vv,vecvecCString);
						}
#ifdef _DEBUG
						TRACE("!ParseSentence(sentence,svSentence)\n");
						PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
						for(DWORD j=0;j<pcstrv.size();j++)
						{
							PositionCString pcstr=pcstrv.at(j);
						}
#endif
						dwStart=i+1;
					}
				}//else-branch of "if(ParseSentence...)"
        LOG("23.05.2008 11.20.35\n");
				g_dwTranslated+=psvSentence.size();
			}
		}
		//wenn Fragezeichen (mögliches Satzendezeichen für einen Fragesatz)
		if(current->ch=='?')
		{
			CString strSentence;
			if(text.GetCString(strSentence,dwStart,i))
			{
				TRACE("strSentence: %s\n",strSentence);
				strSentence.Replace(13,' '); // Carriage Returns durch Leerzeichen ersetzen
				strSentence.Replace(10,' '); // Newlines durch Leerzeichen ersetzen
				CString strTransformed=Transform(strSentence);
				PositionCStringVector psvSentence=GetPositionCStringVector(strSentence,dwStart);
				psvSentence.erase(psvSentence.end()-1);
				CStringVector svSentence=GetCStringVector(strTransformed);
				Question question;
				TRACE("strTransformed: %s\n",strTransformed);
				std::vector<Range> vecRange;
				if(ParseQuestion(question,psvSentence,vecRange,dwFlags))//TRUE))
				{
					Append(vv,TranslateQuestion(question,//pEV,
            rvecintpair,nStartID));
					svSentence.clear();
					for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
						rVecRange.push_back(vecRange.at(dwRun));
					/*for(DWORD dwRun=dwStart;dwRun<i;dwRun++)
					{
						text.SetColor(dwRun,0);
					}*/
					dwStart=i+1;
					PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
					for(DWORD j=0;j<pcstrv.size();j++)
					{
						PositionCString pcstr=pcstrv.at(j);
					}
				}
				else
				{
					/*if(g_bContinue)
					{
						for(DWORD dwRun=dwStart;dwRun<i;dwRun++)
						{
							//die Zeichen, die der grammatikalisch inkorrekte Satz beinhaltet,
							//rot (die "1" wird in EnglishView::OnDraw als rot interpretiert)
							//markieren
							text.SetColor(dwRun,1);
						}
#ifdef _DEBUG
						printf("dwFlags: %u dwFlags&2: %u\n",dwFlags,dwFlags&2);
#endif
						//der Ausdruck "dwFlags&2" muss geklammert werden!
						//wenn grammatikalisch inkorrekte Sätze in den Zieltext übernommen
						//werden sollen
						if((dwFlags&2)==2)
						{
							CStringVector sv;
							std::vector<CStringVector> vecvecCString=
								GetCStringVectorVector(strTransformed);
							sv.push_back(_T("."));					
							vecvecCString.push_back(sv);
							Append(vv,vecvecCString);
						}
#ifdef _DEBUG
						TRACE("!ParseSentence(sentence,svSentence)\n");
						PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
						for(int j=0;j<pcstrv.size();j++)
						{
							PositionCString pcstr=pcstrv.at(j);
							TRACE("pcstr.m_Str: %s  pcstr.m_dwStart: %u,pcstr.m_dwLenght: %u\n",pcstr.m_Str,pcstr.m_dwStart,pcstr.m_dwLength);
						}
#endif
						dwStart=i+1;
					}*/
					if(g_bContinue)
					{
						for(DWORD dwRun=dwStart;dwRun<i;dwRun++)
						{
							//die Zeichen, die der grammatikalisch inkorrekte Satz beinhaltet,
							//rot (die "1" wird in EnglishView::OnDraw als rot interpretiert)
							//markieren
							text.SetColor(dwRun,1);
						}
#ifdef _DEBUG
						printf("dwFlags: %u dwFlags&2: %u\n",dwFlags,dwFlags&2);
#endif
						PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
						for(DWORD j=0;j<pcstrv.size();j++)
						{
							PositionCString pcstr=pcstrv.at(j);
						}
						//der Ausdruck "dwFlags&2" muss geklammert werden!
						//wenn grammatikalisch inkorrekte Sätze in den Zieltext übernommen
						//werden sollen
						/*if((dwFlags&2)==2)
						{
							std::vector<CStringVector> vecvecCString=
								GetCStringVectorVector(strTransformed);
							Append(vv,vecvecCString);
						}*/
						if((dwFlags&ADOPT_GRAMMATICALLY_INCORRECT_SENTENCES)==
							ADOPT_GRAMMATICALLY_INCORRECT_SENTENCES)
						{
							CStringVector sv;
							std::vector<CStringVector> vecvecCString=
								GetCStringVectorVector(strTransformed);
							sv.push_back(_T("?"));					
							vecvecCString.push_back(sv);
							Append(vv,vecvecCString);
						}
						dwStart=i+1;
					}
				}
			}
		}
		if(current->m_next)
			current=current->m_next;
		else
			break;
	}
	if(vv.size()>0)
	{
		CStringVector cstrv=vv.at(0);
	}
	TRACE("std::vector<CStringVector> TranslateText(Text & text) - ENDEkkk\n");
	#ifdef _DEBUG
	printf("vv.size(): %u\n",vv.size());
	//printf("pEV->m_vecIntPair.size(): %u\n",pEV->m_vecIntPair.size());
	//for(DWORD dwRun=0;dwRun<pEV->m_vecIntPair.size();++dwRun)
	//	printf("pEV->m_vecIntPair.at(dwRun).m_n1: %d\n",pEV->m_vecIntPair.
	//		at(dwRun).m_n1);
	//	printf("pEV->m_vecIntPair.at(dwRun).m_n2: %d\n",pEV->m_vecIntPair.
	//		at(dwRun).m_n2);
	printf("std::vector<CStringVector> TranslateText(Text & text,\
std::vector<Range> & rVecRange,const DWORD & dwFlags) - ENDE\n");
	#endif
	return vv;
}

//Wird aufgerufen von CVTransApp
std::vector<CStringVector> TranslateText(
  Text & text,
  //std::vector<Range> & rVecRange,
  std::vector<SentenceAndValidityAndProperName> & rvecsentenceandvalidityandpropername,
  const DWORD & dwFlags//,
  //std::vector<BYTE> & rvecbyReturnOfSentenceParsing
  )
{
#if !defined(_USRDLL__) && defined(_WINDOWS)
  BOOL bFileTransInterruptionSuccessfullyOpened=FALSE;
  CFile fileTransInterruption;
#endif
  CString strText;
  //EnglishView * pEV=NULL;
	int nStartID=ID_USER+100;

  //the current sentence is written into this file: if an interruption(mainly:
  //crash of the translator itself; power loss; user killed trans.exe) 
  //of the translator occurs one can see what the last sentence was.
  std::ofstream ofstreamTransInterruption;
  LOGN("05.06.2008 21.40.22");
#if !defined(_USRDLL__) && defined(_WINDOWS)
  CFileException fileexception;
  bFileTransInterruptionSuccessfullyOpened=fileTransInterruption.Open(FILE_PATH_FOR_INTERRUPTED_TRANSLATION,
    //Create file if it does not exist. Otherwise: do not truncate to lenght "0"(byte).
    CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate|
    CFile::shareDenyWrite,&fileexception);
  if(!bFileTransInterruptionSuccessfullyOpened)
    LOGN("05.06.2008 21.52.35 "<<::GetLastErrorMessageString(fileexception.m_lOsError));
#endif

	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateText(Text & text,\
std::vector<Range> & rVecRange,const DWORD & dwFlags) - ANFANG\n");
	printf("nStartID: %u\n",nStartID);
	#endif
	g_dwTranslated=0;
	text.GetLength();
	//if(pEV)
	//	//damit sich eine Veränderung der Auswahl in einem Kombinationsfeld
	//	//auf ein anderes Kombinationsfeld auswirkt, wenn ein Subjekt 
	//	//"du|ihr|Sie|man" ist, muss pEV->m_dwHighestID
	//	//wieder auf den Anfangswert für die IDs der Steuerelemente in einem
	//	//EnglishView gesetzt werden. Alle nachher erstellten Kombinationsfelder
	//	//kriegen eine um 1 erhöhte ID.
	//	pEV->m_dwHighestID=ID_USER+100;
	if(text.GetCString(strText,0,text.GetLength()-1))
	{
		#ifdef _DEBUG
		printf("text.GetCString(strText,0,text.GetLength()-1)\n");
		printf("strText: %s\n",strText);
		#endif
		strText.Replace(13,' '); // Carriage Returns durch Leerzeichen ersetzen
		strText.Replace(10,' '); // Newlines durch Leerzeichen ersetzen
		PositionCStringVector psvText=GetPositionCStringVector(strText,0);
		g_dwTotal=psvText.size();
		#ifdef _DEBUG
		printf("psvText.size(): %u g_dwTotal: %u\n",psvText.size(),g_dwTotal);
		#endif
	}
	std::vector<CStringVector> vv;
	TextNode * current=text.m_first;
	DWORD dwStart=0;
	DWORD dwSize=text.GetLength();
	TRACE("dwSize: %u\n",dwSize);
	//g_dwTotal=text.GetLength();
#if !defined(_USRDLL__) && defined(_WINDOWS)
	LOGN("05.06.2008 21.41.06 "<<dwSize<<" "<<bFileTransInterruptionSuccessfullyOpened);
#endif //#if !defined(_USRDLL__) && defined(_WINDOWS)
	for(int i=0;i<dwSize;i++)
	{
    LOGN("12.06.2008 21.52.32 "<<current->ch);
		//wenn Punkt (mögliches Satzendezeichen für einen Aussagesatz)
		if(current->ch=='.')
		{
			TRACE("if(current->ch=='.')\n");
			CString strSentence;
			if(text.GetCString(strSentence,dwStart,i))
			{
				TRACE("strSentence: %s\n",strSentence);
        LOGN("12.06.2008 21.43.58");
				strSentence.Replace(13,' '); // Carriage Returns durch Leerzeichen ersetzen
				strSentence.Replace(10,' '); // Newlines durch Leerzeichen ersetzen

        //ofstreamTransInterruption.open(FILE_PATH_FOR_INTERRUPTED_TRANSLATION,
        //  ////Append writes to the end of the file
        //  std::ios_base::ate|
        //  std::ios_base::out);
        ////At first seek to the end of file in order to append data.
        //ofstreamTransInterruption.seekp(0,ios_base::end);
        //LOGN("24.05.2008 21.07.52 "<<ofstreamTransInterruption.tellp());
        ////if(ofstreamTransInterruption.is_open()
        //DWORD dwFileOffset=ofstreamTransInterruption.tellp();
        //ofstreamTransInterruption<<strSentence.GetBuffer()<<"\n";
        //ofstreamTransInterruption.flush();
	      //ofstreamError.close();
        LOGN("05.06.2008 21.45.02");
#if !defined(_USRDLL__) && defined(_WINDOWS)
        DWORD dwFileOffset;
        if(bFileTransInterruptionSuccessfullyOpened)
        {
          fileTransInterruption.SeekToEnd();
          LOGN("05.06.2008 21.57.51");
          dwFileOffset=fileTransInterruption.GetPosition();
          LOGN("05.06.2008 21.58.00");
          fileTransInterruption.Write(strSentence.GetBuffer(),strSentence.GetLength());
          LOGN("05.06.2008 21.58.06");
          fileTransInterruption.Write((void *)"\r\n",2);
          LOGN("05.06.2008 21.58.13");
          fileTransInterruption.Flush();
        }
#endif //#ifndef _USRDLL
        LOGN("05.06.2008 21.45.39");
        CString strTransformed=Transform(strSentence);
				CStringVector svSentence=GetCStringVector(strTransformed);
				PositionCStringVector psvOriginalSentence=GetPositionCStringVector(
					strSentence,dwStart);
				PositionCStringVector psvSentence=psvOriginalSentence;
				//letztes Vektor-Element (den Punkt) löschen
				psvSentence.erase(psvSentence.end()-1);
				g_dwTokensInSentence=psvSentence.size();
				Sentence sentence;
				TRACE("strTransformed: %s\n",strTransformed);
				std::vector<Range> vecRange;
				if(ParseSentence(sentence,psvSentence,vecRange,dwFlags))//bCaseSensitive))
				{
          //EnglishView englishview;
          std::vector<IntPair> vecintpair;
          std::vector<CStringVector> vecvecstr=TranslateSentence2(sentence,
            //&englishview,
            vecintpair,nStartID);
          LOGN("05.06.2008 22.42.31 "<<vecvecstr.size());
          ////For the test if the file contains the last sentence where the interruption occured:
          //int * pi=(int*)0xFF;
          //*pi=66;
          //LOGN("24.05.2008 21.05.20 "<<ofstreamTransInterruption.tellp());
          //Overwrite the last written sentence with e.g. spaces or cut the file size 
          //by the lenght of the last sentence.
          //ofstreamTransInterruption.seekp(strSentence.GetLength()*-1, ios_base::end);
          //ofstreamTransInterruption.seekp(dwFileOffset);
          //DWORD dwNumWriteSpaces=strSentence.GetLength();
          //std::cerr<<ofstreamTransInterruption.tellp()<<" ";
          //while(dwNumWriteSpaces--)
          //  ofstreamTransInterruption<<'\b';
          //ofstreamTransInterruption.close();
          //file.Open(FILE_PATH_FOR_INTERRUPTED_TRANSLATION,CFile::modeWrite|
          //  CFile::shareDenyWrite|CFile::modeNoTruncate );
          //ofstreamTransInterruption.flush();

          //rvecbyReturnOfSentenceParsing.push_back(1);
					//Append(vv,TranslateSentence2(sentence,pEV,nStartID));
          rvecsentenceandvalidityandpropername.push_back(
            SentenceAndValidityAndProperName((BYTE)1,vecvecstr
            ,vecRange,//englishview.m_vecIntPair
            vecintpair)
            );
					svSentence.clear();
					dwStart=i+1;
					PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
					for(DWORD j=0;j<pcstrv.size();j++)
					{
						PositionCString pcstr=pcstrv.at(j);
						TRACE("pcstr.m_Str: %s  pcstr.m_dwStart: %u,pcstr.m_dwLenght: \
%u\n",pcstr.m_Str,pcstr.m_dwStart,pcstr.m_dwLength);
					}
					//if(vecRange.size())
					//	rVecRange.insert(rVecRange.end()-1,vecRange.begin(),vecRange.end()-1);
					//for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
					//	rVecRange.push_back(vecRange.at(dwRun));
				}
				//der überprüfte Satz ist grammatikalisch inkorrekt
				else
				{
          //rvecbyReturnOfSentenceParsing.push_back(0);
          if(g_bContinue)
					{
						for(DWORD dwRun=dwStart;dwRun<i;dwRun++)
						{
							//die Zeichen, die der grammatikalisch inkorrekte Satz beinhaltet,
							//rot (die "1" wird in EnglishView::OnDraw als rot interpretiert)
							//markieren
							text.SetColor(dwRun,1);
						}
#ifdef _DEBUG
						printf("dwFlags: %u dwFlags&2: %u\n",dwFlags,dwFlags&2);
#endif
						//der Ausdruck "dwFlags&2" muss geklammert werden!
						//wenn grammatikalisch inkorrekte Sätze in den Zieltext übernommen
						//werden sollen
						//if((dwFlags&2)==2)
						{
							CStringVector sv;
      				std::vector<Range> vecRange;
							std::vector<CStringVector> vecvecCString=
								GetCStringVectorVector(strTransformed);
#ifdef _DEBUG
							printf("(dwFlags&2)==2\n");
#endif
							sv.push_back(_T("."));					
							vecvecCString.push_back(sv);
							//Append(vv,vecvecCString);
              rvecsentenceandvalidityandpropername.push_back(
                SentenceAndValidityAndProperName(//0,
                  //vecvecCString,vecRange
                  )
                );
						}
#ifdef _DEBUG
						TRACE("!ParseSentence(sentence,svSentence)\n");
						PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
						for(DWORD j=0;j<pcstrv.size();j++)
						{
							PositionCString pcstr=pcstrv.at(j);
						}
#endif
						dwStart=i+1;
					}
				}
				g_dwTranslated+=psvSentence.size();
#if !defined(_USRDLL__) && defined(_WINDOWS)
        if(bFileTransInterruptionSuccessfullyOpened)
          fileTransInterruption.SetLength(dwFileOffset);
#endif
			}
		}//wenn Punkt (mögliches Satzendezeichen für einen Aussagesatz)
		//wenn Fragezeichen (mögliches Satzendezeichen für einen Fragesatz)
		if(current->ch=='?')
		{
			CString strSentence;
			if(text.GetCString(strSentence,dwStart,i))
			{
				TRACE("strSentence: %s\n",strSentence);
				strSentence.Replace(13,' '); // Carriage Returns durch Leerzeichen ersetzen
				strSentence.Replace(10,' '); // Newlines durch Leerzeichen ersetzen

       // ofstreamTransInterruption.open(FILE_PATH_FOR_INTERRUPTED_TRANSLATION,
       //   ////Append writes to the end of the file
       //   std::ios_base::ate|
       //   std::ios_base::out);
       // //At first seek to the end of file in order to append data.
       // ofstreamTransInterruption.seekp(0,ios_base::end);
       // LOGN("24.05.2008 21.08.23 "<<ofstreamTransInterruption.tellp());
       // DWORD dwFileOffset=ofstreamTransInterruption.tellp();
       // //if(ofstreamTransInterruption.is_open()
       // ofstreamTransInterruption<<strSentence.GetBuffer()<<"\n";
       // ofstreamTransInterruption.flush();
	      ////ofstreamError.close();
#if !defined(_USRDLL__) && defined(_WINDOWS)
        DWORD dwFileOffset;
        if(bFileTransInterruptionSuccessfullyOpened)
        {
          fileTransInterruption.SeekToEnd();
          dwFileOffset=fileTransInterruption.GetPosition();
          fileTransInterruption.Write(strSentence.GetBuffer(),strSentence.GetLength());
          fileTransInterruption.Write((void *)"\r\n",2);
          fileTransInterruption.Flush();
        }
#endif//#ifndef _USRDLL
        CString strTransformed=Transform(strSentence);
				PositionCStringVector psvSentence=GetPositionCStringVector(strSentence,dwStart);
				psvSentence.erase(psvSentence.end()-1);
				CStringVector svSentence=GetCStringVector(strTransformed);
				Question question;
				TRACE("strTransformed: %s\n",strTransformed);
				std::vector<Range> vecRange;
				if(ParseQuestion(question,psvSentence,vecRange,dwFlags))//TRUE))
				{
          //EnglishView englishview;
          std::vector<IntPair> vecintpair;
          std::vector<CStringVector> vecvecstr=TranslateQuestion(
            question,//&englishview,
            vecintpair,nStartID);

          //Overwrite the last written sentence with e.g. spaces or cut the file size 
          //by the lenght of the last sentence.
          //ofstreamTransInterruption.seekp(strSentence.GetLength()*-1, ios_base::end);
          //DWORD dwNumWriteSpaces=strSentence.GetLength();
          //while(dwNumWriteSpaces--)
          //  ofstreamTransInterruption<<' ';
          //ofstreamTransInterruption.flush();
          //ofstreamTransInterruption.close();
          //CFile fileTransInterruption(FILE_PATH_FOR_INTERRUPTED_TRANSLATION,CFile::modeWrite|
          //  CFile::shareDenyWrite|CFile::modeNoTruncate );
          ////file.
     //     rvecbyReturnOfSentenceParsing.push_back(1);
					//Append(vv,TranslateQuestion(question,pEV,nStartID));
          rvecsentenceandvalidityandpropername.push_back(
            SentenceAndValidityAndProperName(SENTENCE_IS_GRAMMATICALLY_CORRECT,
            vecvecstr,vecRange,//englishview.m_vecIntPair
            vecintpair)
            );
					svSentence.clear();
					//for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
					//	rVecRange.push_back(vecRange.at(dwRun));
					//for(DWORD dwRun=dwStart;dwRun<i;dwRun++)
					//{
					//	text.SetColor(dwRun,0);
					//}
					dwStart=i+1;
					PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
					for(DWORD j=0;j<pcstrv.size();j++)
					{
						PositionCString pcstr=pcstrv.at(j);
					}
				}
        //VTrans regards the grammar as invalid(maybe because of missing or wrong words
        //in its dictionary).
				else
				{
          //rvecbyReturnOfSentenceParsing.push_back(0);
//					if(g_bContinue)
//					{
//						for(DWORD dwRun=dwStart;dwRun<i;dwRun++)
//						{
//							//die Zeichen, die der grammatikalisch inkorrekte Satz beinhaltet,
//							//rot (die "1" wird in EnglishView::OnDraw als rot interpretiert)
//							//markieren
//							text.SetColor(dwRun,1);
//						}
//#ifdef _DEBUG
//						printf("dwFlags: %u dwFlags&2: %u\n",dwFlags,dwFlags&2);
//#endif
//						//der Ausdruck "dwFlags&2" muss geklammert werden!
//						//wenn grammatikalisch inkorrekte Sätze in den Zieltext übernommen
//						//werden sollen
//						if((dwFlags&2)==2)
//						{
//							CStringVector sv;
//							std::vector<CStringVector> vecvecCString=
//								GetCStringVectorVector(strTransformed);
//							sv.push_back(_T("."));					
//							vecvecCString.push_back(sv);
//							Append(vv,vecvecCString);
//						}
//#ifdef _DEBUG
//						TRACE("!ParseSentence(sentence,svSentence)\n");
//						PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
//						for(int j=0;j<pcstrv.size();j++)
//						{
//							PositionCString pcstr=pcstrv.at(j);
//							TRACE("pcstr.m_Str: %s  pcstr.m_dwStart: %u,pcstr.m_dwLenght: %u\n",pcstr.m_Str,pcstr.m_dwStart,pcstr.m_dwLength);
//						}
//#endif
//						dwStart=i+1;
//					}
					if(g_bContinue)
					{
						for(DWORD dwRun=dwStart;dwRun<i;dwRun++)
						{
							//die Zeichen, die der grammatikalisch inkorrekte Satz beinhaltet,
							//rot (die "1" wird in EnglishView::OnDraw als rot interpretiert)
							//markieren
							text.SetColor(dwRun,1);
						}
#ifdef _DEBUG
						printf("dwFlags: %u dwFlags&2: %u\n",dwFlags,dwFlags&2);
#endif
						PositionCStringVector pcstrv=GetPositionCStringVector(strSentence);
						for(DWORD j=0;j<pcstrv.size();j++)
						{
							PositionCString pcstr=pcstrv.at(j);
						}
						//der Ausdruck "dwFlags&2" muss geklammert werden!
						//wenn grammatikalisch inkorrekte Sätze in den Zieltext übernommen
						//werden sollen
//						if((dwFlags&2)==2)
//						{
//							std::vector<CStringVector> vecvecCString=
//								GetCStringVectorVector(strTransformed);
//							Append(vv,vecvecCString);
//						}
						if((dwFlags&ADOPT_GRAMMATICALLY_INCORRECT_SENTENCES)==
							ADOPT_GRAMMATICALLY_INCORRECT_SENTENCES)
						{
							CStringVector sv;
							std::vector<CStringVector> vecvecCString=
								GetCStringVectorVector(strTransformed);
							sv.push_back(_T("?"));					
							vecvecCString.push_back(sv);
							//Append(vv,vecvecCString);
              rvecsentenceandvalidityandpropername.push_back(
                SentenceAndValidityAndProperName(//0,
                  //vecvecCString,vecRange
                  )
                );
						}
						dwStart=i+1;
					}
				}
#if !defined(_USRDLL__) && defined(_WINDOWS)
        if(bFileTransInterruptionSuccessfullyOpened)
          fileTransInterruption.SetLength(dwFileOffset);
#endif//#ifndef _USRDLL
			}//if(text.GetCString(strSentence,dwStart,i))
		}
		if(current->m_next)
			current=current->m_next;
		else
			break;
	}
	if(vv.size()>0)
	{
		CStringVector cstrv=vv.at(0);
	}
	TRACE("std::vector<CStringVector> TranslateText(Text & text) - ENDEkkk\n");
	#ifdef _DEBUG
	printf("vv.size(): %u\n",vv.size());
  //if(pEV)
	 // printf("pEV->m_vecIntPair.size(): %u\n",pEV->m_vecIntPair.size());
	//for(DWORD dwRun=0;dwRun<pEV->m_vecIntPair.size();++dwRun)
	//	printf("pEV->m_vecIntPair.at(dwRun).m_n1: %d\n",pEV->m_vecIntPair.
	//		at(dwRun).m_n1);
	//	printf("pEV->m_vecIntPair.at(dwRun).m_n2: %d\n",pEV->m_vecIntPair.
	//		at(dwRun).m_n2);
	printf("std::vector<CStringVector> TranslateText(Text & text,\
std::vector<Range> & rVecRange,const DWORD & dwFlags) - ENDE\n");
	#endif
	return vv;
}

std::vector<CStringVector> TranslateToPlusInfinitive(const ToPlusInfinitive 
& toPlusInfinitive,int & nStartID)
{
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateToPlusInfinitive(const \
ToPlusInfinitive & toPlusInfinitive) ANFANG\n");
	#endif
	std::vector<CStringVector> vv;
	if(toPlusInfinitive.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated)
	{
		if(toPlusInfinitive.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated->
			m_pWordNodeNext)
		{
			GermanPreposition * gp=dynamic_cast<GermanPreposition *>(
				toPlusInfinitive.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			CStringVector sv;
			sv.push_back(gp->m_strWord);
			#ifdef _DEBUG
			printf("Präposition: %s\n",gp->m_strWord);
			#endif
			vv.push_back(sv);
		}
	}
	if(toPlusInfinitive.m_pObject)
	{
		BYTE b=0;
    std::vector<IntPair> vecintpair;
		Append(vv,TranslateObject2(*toPlusInfinitive.m_pObject,4,FALSE,b,//NULL,
      vecintpair,
			nStartID));
	}
	if(toPlusInfinitive.m_bNegation)
	{
		CStringVector sv;
		sv.push_back("nicht");
		vv.push_back(sv);
	}
	if(toPlusInfinitive.m_pPlace)
	{
		BYTE b=0;
		Append(vv,TranslatePlace(*toPlusInfinitive.m_pPlace,b,nStartID));
	}
	GermanVerb * gv=NULL;
	if(toPlusInfinitive.m_pWordNodeWhereAnEnglishVerbIsEncapsulated)
		if(toPlusInfinitive.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
			m_pWordNodeNext)
		{
			gv=dynamic_cast<GermanVerb*>(toPlusInfinitive.
				m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext->m_pWord);
			//GetGermanVocableOfEnglishVerb(gerund.m_predicate);
			CStringVector sv;
			CStringVector svVerb=GetCStringVector2(gv->m_strWords[4]);
			if(svVerb.size()==1)
			{
				sv.push_back("zu");
				vv.push_back(sv);
				sv.clear();
			}
			if(svVerb.size()>1)
				sv.push_back(svVerb.at(1)+_T("zu")+svVerb.at(0));
				// Bespiel: Looking well is important.
				// Deutsche Übersetzung: Gut auszusehen ist wichtig.
				// wir nehmen die gebeugte Form des Verbes in der 1.
				// Person Plural
				// aussehen - wir sehen aus
				//									1    2
				// -> auszusehen
				//     2     1
			else
				sv.push_back(gv->m_strWords[0]); // Infinitiv
			#ifdef _DEBUG
			printf("Infinitiv: %s\n",gv->m_strWords[0]);
			#endif
			vv.push_back(sv);
		}
	#ifdef _DEBUG
	printf("std::vector<CStringVector> TranslateToPlusInfinitive(const \
ToPlusInfinitive & toPlusInfinitive) ENDE return vv\n");
	#endif
	return vv;
}

CString TranslatePossessivePronoun(BYTE byte)
{
	if(byte==1) // my
		return CString("mein");
	if(byte==2) //"your")
	{
		return CString("dein");
	}
	if(byte==3) // his
		return CString("sein");
	if(byte==4) // her
		return CString("ihr");
	if(byte==5) // its
		return CString("sein");
	if(byte==6) // our
		return CString("unser");
	if(byte==7) // their
		return CString("ihr");
	return CString("");
}

CStringVector TranslatePossessivePronoun2(BYTE byte)
{
	CStringVector cstrvec;
	if(byte==1) // my
		cstrvec.push_back("mein");
	if(byte==2) //"your")
	{
		cstrvec.push_back("dein");
		cstrvec.push_back("euer");
		cstrvec.push_back("Ihr");
	}
	if(byte==3) // his
		cstrvec.push_back("sein");
	if(byte==4) // her
		cstrvec.push_back("ihr");
	if(byte==5) // its
		cstrvec.push_back("sein");
	if(byte==6) // our
		cstrvec.push_back("unser");
	if(byte==7) // their
		cstrvec.push_back("ihr");
	return cstrvec;
}

std::vector<std::string> TranslatePossessivePronoun3(BYTE byte)
{
	std::vector<std::string> vecstr;
	if(byte==1) // my
		vecstr.push_back("mein");
	if(byte==2) //"your")
	{
		vecstr.push_back("dein");
		vecstr.push_back("euer");
		vecstr.push_back("Ihr");
	}
	if(byte==3) // his
		vecstr.push_back("sein");
	if(byte==4) // her
		vecstr.push_back("ihr");
	if(byte==5) // its
		vecstr.push_back("sein");
	if(byte==6) // our
		vecstr.push_back("unser");
	if(byte==7) // their
		vecstr.push_back("ihr");
	return vecstr;
}

/*In anderen Sprachen, wie zum Beispiel im Deutschen, gilt 
die grundsätzliche Struktur (hier: SPO) nur in Aussagesätzen, 
verändert sich aber in anderen Satztypen.
Beispiel:
    * "Er isst den Apfel" (SPO) (Aussagesatz, neutral)
    * "Wenn er den Apfel isst, ..." (SOP) (Nebensatz)
    * "Isst er den Apfel?" (PSO) (Frage)
    * "Äpfel isst er nicht." (OPS) (Aussagesatz, Objekt wird betont)
		Quelle:http://de.wikipedia.org/wiki/Satzstellung
*/
//Falls ich dich sehe, isst er den Apfel.(?)
//wird für Fragesätze("Isst er den Apfel?") und ZUSAMMENGESETZTE Aussage-/Aufforderungssätze
//verwendet.
//Z.B. für "Darum liebe ich dich."
std::vector<CStringVector> TranslatePSO(const Clause & subSentence,
	BYTE & bFirstTokenInSentence,
//pEV: ein EnglishView-Objekt wird benötigt, um dessen Member-Variable
// m_vecIntPair (ein Vektor) gegebenenfalls Elemente hinzuzufügen, die
// zwei integer als Membervariablen haben, die jeweils IDs für CComboBox-
// (Kombinationsfeld-)Steuerelemente sind und eine ID ein Subjekt, in dem
// ein "du|ihr|Sie|man" vorkommt und eine ID für die gebeugten Verbformen steht.
// Wenn man bei dem einen CComboBox-(Kombinationsfeld-)Steuerelement 
// auswählt, soll dann nämlich auch in dem zugehörigen CComboBox-
// (Kombinationsfeld-)Steuerelement der gleiche Index ausgewählt werden.
// pEV wird zum Beispiel an TranslateSubject weitergeleitet
	//EnglishView * pEV,
  std::vector<IntPair> & rvecintpair,
//nStartID: wird benötigt, um beim Anhängen eines Elementes an einen Vektor
// die richtige ID für das CComboBox (Kombinationsfeld) zu haben (wenn man
// bei einem Kombinationsfeld, wo gerade "du" ausgewählt ist und im anderen
// Kombinationsfeld "folgst" ausgewählt ist, soll bei einer Auswahl von "ihr"
// sich das "folgst" automatisch in "folgt" ändern
// Es kann folgenden Satz geben: "I like hurting you." "hurting you": 
// Gerundium -> "Ich mag |dich| zu verletzen."
//                      |Sie |
//                      |euch|
	int & rnStartID)
{
	BYTE personIndex=0;
	GermanVerb * pgvBend=NULL;
	GermanVerb * pgvNonBend=NULL;
	std::vector<CStringVector> vv;
		//wenn eine Umstandsbestimmung der Zeit am Anfang eines deutschen Satzes
		//steht. Beispiel: "Now I close my eyes."
		//Umstandsbestimmung der Zeit: "Now" ->
		//"Jetzt schliesse ich meine Augen."
		if(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated &&
			subSentence.m_bAdverbOfTimeAtTheBeginning)
		{
			GermanAdverb * ga=dynamic_cast<GermanAdverb *>
				(subSentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated->
				m_pWordNodeNext->m_pWord);
			if(ga)
			{
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				if(bFirstTokenInSentence)
				{
					MakeFirstLetterUpper(sv.at(0));
					bFirstTokenInSentence=FALSE;
				}
				vv.push_back(sv);
			}
		}
		if(subSentence.m_pSubject)
		{
			//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
			//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
			//           -----S---- -----P----
			//          "My glasses are broken." -> "Meine Brille ist kaputt."
			//           -----S---- -P- ---A--
			//S: Subjekt P: Prädikat
			//A: Umstandsbestimmung der Art und Weise
			if(subSentence.m_vecPredicate.size()==2)
			{
				CString strBendVerb;
				CString strManner;
				CStringVector sv;
				CStringVector svGermanVerb;
				personIndex++;
				for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
				{
					Predicate predicate=subSentence.m_vecPredicate.at(bRun);
#ifdef _DEBUG
					printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
					{
						if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
							m_pWordNodeNext)
						{
							pgvBend=dynamic_cast<GermanVerb*>(predicate.
								m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
								m_pWordNodeNext->m_pWord);
			#ifdef _DEBUG
							printf("pgvBend->m_strWords[0]: %s\n",pgvBend->m_strWords[0]);
			#endif
						}
					}
#ifdef _DEBUG
					else
						printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
						if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
							m_pWordNodeNext)
							if(!pgvBend)
								pgvBend=dynamic_cast<GermanVerb*>(predicate.
									m_pWordNodeWhereTheGermanTranslationIsABendVerb->
									m_pWordNodeNext->m_pWord);
							else
								pgvNonBend=dynamic_cast<GermanVerb*>(predicate.
									m_pWordNodeWhereTheGermanTranslationIsABendVerb->
									m_pWordNodeNext->m_pWord);
	//#ifdef _DEBUG
					if(predicate.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
					{
						CStringVector sv;
						GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
						sv.push_back(gv->m_strWords[subSentence.m_pSubject->m_personIndex]);
						//vv.push_back(sv);
					}
					if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
						predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
						predicate.m_bTense==SIMPLE_PAST || // I began.
						predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
						predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
						predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
						predicate.m_bTense==PRESENT_PERFECT) // I have begun.
					{
					#ifdef _DEBUG
						printf("subSentence.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PAST || \
predicate.m_bTense==PAST_PROGRESSIVE \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
predicate.m_bTense==PRESENT_PERFECT\n");
					#endif
					//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
						if(pgvBend!=NULL)
						{
							CStringVector svVerb;
							//CStringVector sv;
							if(predicate.m_bTense==SIMPLE_PRESENT || 
								predicate.m_bTense==PRESENT_PROGRESSIVE ||
								predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
								predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
#ifdef _DEBUG
								printf("predicate.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST\n");
#endif
								//wenn Aktivsatz ("I hit.") -> "Ich schlage."
								if(predicate.m_bActiveClause)
								{
#ifdef _DEBUG
									printf("predicate.m_bActiveClause\n");
#endif
									svGermanVerb=GetCStringVector2(pgvBend->
										m_strWords[personIndex]);
									svVerb.push_back(svGermanVerb.at(0));
									strManner=svGermanVerb.at(0);
									// Prädikat (Präsens)
									if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
									{
										svGermanVerb=GetCStringVector2(pgvBend->
											m_strWords[personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präteritum)
									}
								}
								//wenn Passivsatz ("I am hit.") -> "Ich werde geschlagen."
								else
								{
#ifdef _DEBUG
									printf("!predicate.m_bActiveClause\n");
#endif
									if(pgvNonBend) // Beispiel: 'can be used'
									{ 
										svGermanVerb=GetCStringVector2(pgvBend->
											m_strWords[personIndex]);
										svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präsens)
										if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											svGermanVerb=GetCStringVector2(pgvBend->
												m_strWords[personIndex+6]);
											svVerb.push_back(svGermanVerb.at(0));
											// Prädikat (Präteritum)
										}
									}
									else // Beispiel: 'is used'
									{
#ifdef _DEBUG
										printf("!gvInfinitive\n");
#endif
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
										//svGermanVerb=GetCStringVector2(gv->words[personIndex]);
										strBendVerb=gv->m_strWords[personIndex];
										//svVerb.push_back(svGermanVerb.at(0));
										// Prädikat (Präsens)
										if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
										{
											//svGermanVerb=GetCStringVector2(gv->words[personIndex+6]);
											//svVerb.push_back(svGermanVerb.at(0));
											//strBendVerb=svGermanVerb.at(0);
											// Prädikat (Präteritum)
										}
									}
								}
							}
							if(predicate.m_bTense==SIMPLE_PAST || predicate.m_bTense==
								PAST_PROGRESSIVE)
							{
								if(predicate.m_bActiveClause) // Beispiel: I used water.
								{
									svGermanVerb=GetCStringVector2(pgvBend->
										m_strWords[personIndex+6]);
									svVerb.push_back(svGermanVerb.at(0));
								}
								else
								{
									if(pgvNonBend) // Beispiel: I could be used. / 
									{ // I could be being used.
										svGermanVerb=GetCStringVector2(pgvBend->
											m_strWords[personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
									}
									else // Beispiel: I was used. / I was being used.
									{
										GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("will"));
										svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex+6]);
										svVerb.push_back(svGermanVerb.at(0));
									}
								}
							}
							if(predicate.m_bTense==PRESENT_PERFECT && !predicate.
								m_bActiveClause)
							{ // Beispiel: I have been used. -> Ich bin benutzt worden.
								GermanVerb * gv=GetGermanWordOfEnglishVerb(_T("be"));
								svGermanVerb=GetCStringVector2(gv->m_strWords[personIndex]);
								svVerb.push_back(svGermanVerb.at(0));
								// finite Form von 'sein' anhängen
							}
							if(predicate.m_bActiveClause && subSentence.m_pManner)
							{
#ifdef _DEBUG
								printf("predicate.m_bActiveClause && subSentence.m_pManner\n");
#endif
								/*if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
								{
									if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
										m_pWordNodeNext)
									{
										GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
											m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										if(ga)
											strManner+=_T(" ")+ga->m_strWord;
									}
								}*/
								if(predicate.m_bNegation)
									strManner+=_T(" nicht");
								strManner+=_T(" ")+TranslateMannerAsCString(*subSentence.
									m_pManner);
								//wenn ein Infinitiv oder Partizip Perfekt im Prädikat ist
								//Beispiel: "I will be broken." -> "Ich werde kaputt sein."
								//"I have been broken." -> "Ich bin kaputt gewesen."
								if(pgvNonBend)
									sv.push_back(strManner+" "+pgvNonBend->m_strWords[0]);
								//wenn kein Infinitiv im Prädikat ist
								//Beispiel: "I am broken." -> "Ich bin kaputt."
								//else
								//	sv.push_back(strManner+" "+gvInfinitive->words[0]);
							}
							//wenn passives Prädikat
							if(!predicate.m_bActiveClause)
							{
								/*if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
								{
									if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
										m_pWordNodeNext)
									{
										GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
											m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										if(ga)
											strManner+=_T(" ")+ga->m_strWord;
									}
								}*/
								//wenn das Prädikat eine Verneinung enthält ("not")
								if(predicate.m_bNegation)
									strBendVerb+=_T(" nicht");
								strBendVerb+=_T(" ")+pgvBend->m_strWords[13];
							}
							// Prädikat (Präteritum)
							//vv.push_back(sv);
						}
						#ifdef _DEBUG
						else
						{
							TRACE("gv==NULL\n");
							printf("gv==NULL\n");
						}
						#endif
					//if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
					//	predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
					//	predicate.m_bTense==SIMPLE_PAST || // I began.
					//	predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
					//	predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
					//	predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
					//	predicate.m_bTense==PRESENT_PERFECT) // I have begun.
					}
					//if(subSentence.m_pManner)
					//	strBendVerb+=TranslateMannerAsCString(*subSentence.m_pManner);
				}
				if(strBendVerb.GetLength()>0)
					sv.push_back(strBendVerb);
				if(strManner.GetLength()>0)
					sv.push_back(strManner);
				vv.push_back(sv);
			}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind
			// der "Untersatz" enthält mehr oder weniger als 2 Prädikate
			else
			{
				CStringVector svGermanVerb;
				Predicate predicate=subSentence.m_vecPredicate.at(0);
#ifdef _DEBUG
				printf("predicate.size()!=2\n");
				printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
				{
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
						m_pWordNodeNext)
					{
						pgvBend=dynamic_cast<GermanVerb*>(predicate.
							m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
							m_pWordNodeNext->m_pWord);
			#ifdef _DEBUG
						printf("pgvBend->m_strWords[0]: %s\n",pgvBend->m_strWords[0]);
		#endif
					}
				}
#ifdef _DEBUG
				else
					printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
				if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
					if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
						m_pWordNodeNext)
						if(!pgvBend)
							pgvBend=dynamic_cast<GermanVerb*>(predicate.
								m_pWordNodeWhereTheGermanTranslationIsABendVerb->
								m_pWordNodeNext->m_pWord);
						else
							pgvNonBend=dynamic_cast<GermanVerb*>(predicate.
								m_pWordNodeWhereTheGermanTranslationIsABendVerb->
								m_pWordNodeNext->m_pWord);
				//wenn nur ein Aufzählungselement im Subjekt vorhanden ist und in 
				//diesem Element ein Personalpronom der 2. Person Singular enthalten
				//ist
				if(subSentence.m_pSubject->m_vecEnumerationElement.size()==1 &&
					subSentence.m_pSubject->m_vecEnumerationElement.at(0).
						m_personalPronoun==2)
				{
					BYTE bReflexive=0;
					CString str;
					IntPair intPair;
					intPair.m_n1=rnStartID;
					intPair.m_n2=rnStartID+1;
					//pEV->m_vecIntPair.push_back(intPair);
          rvecintpair.push_back(intPair);
					rnStartID+=2;
					if(pgvBend)
					{
						//wenn KEIN reflexives deutsches Verb
						//zum Beispiel: "I can be used."
						if(pgvBend->m_bReflexive)
							bReflexive=1;
					}
					if(pgvNonBend)
						//wenn KEIN reflexives deutsches Verb
						//zum Beispiel: "I still need a friend."
						//zum Beispiel: "I am still used."
						if(pgvNonBend->m_bReflexive)
							bReflexive=1;
					if(predicate.m_bTense==FUTURE) // Zukunft / will-future (z.B. will go)
					{
						CStringVector sv;
						GermanVerb * gvWill=GetGermanWordOfEnglishVerb(_T("will"));
						str+=gvWill->m_strWords[2];
						if(bReflexive)
							str=_T("dich ");
						sv.push_back(str);
						str=gvWill->m_strWords[5];
						if(bReflexive)
							str=_T("euch ");
						sv.push_back(str);
						str=gvWill->m_strWords[6];
						if(bReflexive)
							str=_T("sich ");
						sv.push_back(str);
						str=gvWill->m_strWords[3];
						if(bReflexive)
							str=_T("sich ");
						sv.push_back(str);
						vv.push_back(sv);
					}
					if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
						predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
						predicate.m_bTense==SIMPLE_PAST || // I began.
						predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
						predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
						predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
						predicate.m_bTense==PRESENT_PERFECT) // I have begun.
					{
#ifdef _DEBUG
						printf("subSentence.m_bTense==SIMPLE_PRESENT || \
predicate.m_bTense==PRESENT_PROGRESSIVE || \
predicate.m_bTense==SIMPLE_PAST || \
predicate.m_bTense==PAST_PROGRESSIVE \
predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE \
predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST \
predicate.m_bTense==PRESENT_PERFECT\n");
#endif
						//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
						if(pgvBend!=NULL)
						{
							CStringVector svSubjectAndVerb;
							//wenn die Zeitform "Präsens", "fortschreitendes Präsens", 
							//"fortschreitender Perfekt" oder "Präsens oder Präteritum" ist
							if(predicate.m_bTense==SIMPLE_PRESENT || 
								predicate.m_bTense==PRESENT_PROGRESSIVE ||
								predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
								predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
							{
								GermanVerb * gvBend;
								//wenn Aktivsatz
								if(predicate.m_bActiveClause)
								{
#ifdef _DEBUG
									printf("predicate.m_bActiveClause\n");
#endif
									gvBend=pgvBend;
								}
								else
								{
									if(pgvNonBend) // Beispiel: 'can be used'
										gvBend=pgvBend;
									else // Beispiel: 'is used'
										gvBend=dynamic_cast<GermanVerb*>(
											GetGermanWordOfEnglishVerb(_T("will")));
								}
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								//String-Vektor aus der 2. Person Singular ("du") Präsens bilden
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[2]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn du dich wäschst."
								if(bReflexive)
									str=_T("dich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
								//interpretiert werden kann
								// Prädikat (Präsens)
								if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
									//2 Elementen. Sonst hat der Vektor nur 1 Element.
									svGermanVerb=GetCStringVector2(gvBend->m_strWords[8]);
									//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst 
									//das Reflexiv-Pronom, dann die gebeugte Verbform. 
									//Beispiel: "Wenn du dich wäschst."
									if(bReflexive)
										str=_T("dich ");
									str+=svGermanVerb.at(0);
									svSubjectAndVerb.push_back(str);
									//Zeichenkette leeren
									str.Empty();
									// Prädikat (Präteritum)
								}
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[5]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn du dich wäschst."
								if(bReflexive)
									str=_T("euch ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
								//interpretiert werden kann
								// Prädikat (Präsens)
								if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
									//2 Elementen. Sonst hat der Vektor nur 1 Element.
									svGermanVerb=GetCStringVector2(gvBend->m_strWords[11]);
									//bei dem Satzbau "SOP" steht zuerst das Reflexiv-Pronom, dann
									//die gebeugte Verbform. Beispiel: "Wenn du dich wäschst."
									if(bReflexive)
										str=_T("euch ");
									str+=svGermanVerb.at(0);
									svSubjectAndVerb.push_back(str);
									//Zeichenkette leeren
									str.Empty();
									// Prädikat (Präteritum)
								}
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[6]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn du dich wäschst."
								if(bReflexive)
									str=_T("sich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
								//interpretiert werden kann
								// Prädikat (Präsens)
								if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
									//2 Elementen. Sonst hat der Vektor nur 1 Element.
									svGermanVerb=GetCStringVector2(gvBend->m_strWords[12]);
									//bei dem Satzbau "SOP" steht zuerst das Reflexiv-Pronom, dann
									//die gebeugte Verbform. Beispiel: "Wenn du dich wäschst."
									if(bReflexive)
										str=_T("sich ");
									str+=svGermanVerb.at(0);
									svSubjectAndVerb.push_back(str);
									//Zeichenkette leeren
									str.Empty();
									// Prädikat (Präteritum)
								}
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[3]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn du dich wäschst."
								if(bReflexive)
									str=_T("sich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn ein gebeugtes Verb als Präsens ODER Präteritum 
								//interpretiert werden kann
								// Prädikat (Präsens)
								if(predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST)
								{
									//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
									//2 Elementen. Sonst hat der Vektor nur 1 Element.
									svGermanVerb=GetCStringVector2(gvBend->m_strWords[9]);
									//bei dem Satzbau "SOP" steht zuerst das Reflexiv-Pronom, dann
									//die gebeugte Verbform. Beispiel: "Wenn du dich wäschst."
									if(bReflexive)
										str=_T("sich ");
									str+=svGermanVerb.at(0);
									svSubjectAndVerb.push_back(str);
									//Zeichenkette leeren
									str.Empty();
									// Prädikat (Präteritum)
								}
									/*//wenn ein Adverb im Prädikat enthalten ist
									//zum Beispiel: "I still need a friend." Adverb: "still"
									if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
									{
										GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
											m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										vv.push_back(svVerb);
										svVerb.clear();
										svVerb.push_back(ga->m_strWord);
									}*/
							}
							//wenn die Zeitform "Präteritum" oder "fortschreitendes Präteritum"
							//ist
							//Präteritum: "I used water."
							//fortschreitendes Präteritum: "I was using water."
							if(predicate.m_bTense==SIMPLE_PAST || predicate.m_bTense==
								PAST_PROGRESSIVE)
							{
								GermanVerb * gvBend;
								//wenn Aktivsatz Beispiel: "I used water."
								if(predicate.m_bActiveClause)
								{
#ifdef _DEBUG
									printf("predicate.m_bActiveClause\n");
#endif
									gvBend=pgvBend;
								}
								else
								{
									if(pgvNonBend) // Beispiel: 'can be used'
										gvBend=pgvBend;
									else // Beispiel: 'is used'
										gvBend=dynamic_cast<GermanVerb*>(
											GetGermanWordOfEnglishVerb(_T("will")));
								}
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[8]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn du dich wuschst."
								if(bReflexive)
									str=_T("dich ");
								str=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[11]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn ihr euch wuscht."
								if(bReflexive)
									str+=_T("euch ");
								str=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[12]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn Sie sich wuschen."
								if(bReflexive)
									str=_T("sich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[9]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn man sich wusch."
								if(bReflexive)
									str=_T("sich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
							}
							//wenn die Zeitform "Perfekt" ist und der Haupt-/Nebensatz ein
							//Aktivsatz ist
							// Beispiel: I have used you. -> Ich habe dich benutzt.
							if(predicate.m_bTense==PRESENT_PERFECT)
							{
								GermanVerb * gvBend;
								//wenn Aktivsatz Beispiel: "I used water."
								if(predicate.m_bActiveClause)
								{
#ifdef _DEBUG
									printf("predicate.m_bActiveClause\n");
#endif
									gvBend=pgvBend;
								}
								else
								{
									if(pgvNonBend) // Beispiel: 'can be used'
										gvBend=pgvBend;
									else // Beispiel: 'is used'
										gvBend=dynamic_cast<GermanVerb*>(
											GetGermanWordOfEnglishVerb(_T("be")));
								}
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[2]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn du dich gewaschen hast."
								if(bReflexive)
									str=_T("dich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[5]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn ihr euch gewaschen habt."
								if(bReflexive)
									str=_T("euch ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[6]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn Sie sich gewaschen haben."
								if(bReflexive)
									str=_T("sich ");
								str+=svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
								//Zeichenkette leeren
								str.Empty();
								//wenn zusammengesetztes Verb, dann entsteht ein Vektor aus
								//2 Elementen. Sonst hat der Vektor nur 1 Element.
								svGermanVerb=GetCStringVector2(gvBend->m_strWords[3]);
								//bei dem Satzbau "Subjekt Objekt Prädikat" steht zuerst das 
								//Reflexiv-Pronom, dann die gebeugte Verbform. Beispiel: 
								//"Wenn man sich gewaschen hat."
								if(bReflexive)
									str=_T(" sich");
								str=_T("man ")+svGermanVerb.at(0);
								svSubjectAndVerb.push_back(str);
							}
							// Prädikat (Präteritum)
							vv.push_back(svSubjectAndVerb);
							// gv ist NULL, wenn der 'UnterSatz' ein Gerundium ist
							// deshalb vorher gv prüfen, bevor gv->m_bReflexive
							// abgefragt wird, sonst gibt es einen Absturz
							if(pgvBend)
							{
								//wenn KEIN reflexives deutsches Verb
								//zum Beispiel: "I can be used."
								if(!pgvBend->m_bReflexive)
								{
									//wenn ein Adverb im Prädikat enthalten ist
									//zum Beispiel: "I can still be used." Adverb: "still"
									if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
									{
										GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
											m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										svSubjectAndVerb.clear();
										svSubjectAndVerb.push_back(ga->m_strWord);
										vv.push_back(svSubjectAndVerb);
									}
								}
							}
							if(pgvNonBend)
								//wenn KEIN reflexives deutsches Verb
								//zum Beispiel: "I still need a friend."
								//zum Beispiel: "I am still used."
								if(!pgvNonBend->m_bReflexive)
								{
									//wenn ein Adverb im Prädikat enthalten ist
									//zum Beispiel: "I still need a friend." Adverb: "still"
									//zum Beispiel: "I am still used." Adverb: "still"
									//bei "The world still turns." muss das Adverb allerdings
									//erst hinter dem "sich" stehen. Also: "Die Welt dreht sich
									//immer noch."
									if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
									{
										GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
											m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
											m_pWordNodeNext->m_pWord);
										svSubjectAndVerb.clear();
										svSubjectAndVerb.push_back(ga->m_strWord);
										vv.push_back(svSubjectAndVerb);
									}
								}
						}
					//if(predicate.m_bTense==SIMPLE_PRESENT || // I begin.
					//	predicate.m_bTense==PRESENT_PROGRESSIVE || // I am beginning.
					//	predicate.m_bTense==SIMPLE_PAST || // I began.
					//	predicate.m_bTense==PAST_PROGRESSIVE || // I was beginning.
					//	predicate.m_bTense==PRESENT_PERFECT_PROGRESSIVE ||
					//	predicate.m_bTense==SIMPLE_PRESENT_OR_SIMPLE_PAST ||
					//	predicate.m_bTense==PRESENT_PERFECT) // I have begun.
          }
				}
				//wenn nicht: wenn nur ein Aufzählungselement im Subjekt vorhanden ist 
				//und in diesem Element ein Personalpronom der 2. Person Singular 
				//enthalten ist
				else
				{
					vv.push_back(GetGermanBendVerb(pgvBend,predicate,personIndex));
				}
			}
		}//if(subSentence.m_pSubject)
		BYTE b=0;
		if(subSentence.m_pSubject)
		{
			//personIndex: wird für die Beugung des deutschen Verbes benötigt
			//0=1. Person Singular ... 5=3. Person Plural
			std::vector<CStringVector> vvSubject=TranslateSubject(
				*subSentence.m_pSubject,FALSE,personIndex,bFirstTokenInSentence,
				//pEV,
        rvecintpair,rnStartID);
			Append(vv,vvSubject); // Subjekt
		}
		/*if(subSentence.m_pRelativeClause) // Relativsatz
		{
			if(subSentence.m_pRelativeClause->m_bRefersToSubject)
			{
				#ifdef _DEBUG
				printf("subSentence.m_pRelativeClause->m_bRefersToSubject\n");
				#endif
				if(subSentence.m_pSubject->m_vecEnumerationElement.size()>0)
				{
					std::vector <GermanNoun> vecGermanNoun=
						GetGermanWordOfEnglishNoun2(subSentence.m_pSubject->
						m_vecEnumerationElement.at(0).m_pDan->m_Noun);
					std::vector<CStringVector> vvRelativeClause=
						TranslateRelativeClause(*subSentence.m_pRelativeClause,rnStartID);
					if(vecGermanNoun.size()>0)
					{
						CStringVector sv;
						sv.push_back(_T(","));
						vv.push_back(sv);
						sv.clear();
						GermanNoun gn=vecGermanNoun.at(0);
						if(gn.m_bArticle=='1')
							sv.push_back(_T("den"));
						else if(gn.m_bArticle=='2')
							sv.push_back(_T("die"));
						else if(gn.m_bArticle=='3')
							sv.push_back(_T("das"));
						vv.push_back(sv);
					}
					//rnStartID+=vvRelativeClause.size();
					Append(vv,vvRelativeClause);
				}
				CStringVector sv;
				sv.push_back(_T(","));
				vv.push_back(sv);
			}
		}*/
		if(subSentence.m_bBe) // wenn das Prädikat vom Verb 'be' (english für: 'sein') kommt
		{
			if(subSentence.m_bNegation)
			{
				BOOL bNot=TRUE;
				if(subSentence.m_pObjectFirst)
				{
					for(DWORD dwRun=0;dwRun<subSentence.m_pObjectFirst->
						m_vecEnumerationElement.size();dwRun++)
					{
						if(subSentence.m_pObjectFirst->m_vecEnumerationElement.
							at(dwRun).m_pEnr)
							if(subSentence.m_pObjectFirst->m_vecEnumerationElement.
								at(dwRun).m_pEnr->bIndefiniteArticle)
								bNot=FALSE;
					}
				}
				if(bNot)
				{
					CStringVector sv;
					sv.push_back("nicht");
					vv.push_back(sv);
				}
			}
		}
		if(subSentence.m_pObjectFirst) // Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectFirst\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(pgvBend)
			{
				//printf("gv
				if(pgvBend->m_bCase==1)
				{
					BYTE b=0;
					//wenn der Satz ein zweites Objekt enthält
					if(subSentence.m_pObjectSecond)
					{
					//dann beim vorletzten Parameter (Verneinung) FALSE angeben,
					//da nur in das zweite Objekt die Verneinung mit einbezogen wird:
					//"I do not give a boy a book." -> "Ich gebe einem Jungen kein Buch."
						std::vector<CStringVector> vvObject=TranslateObject2(
							*subSentence.m_pObjectFirst,1,FALSE,b,//pEV,
              rvecintpair,rnStartID);
						//rnStartID+=vvObject.size();
						Append(vv,vvObject); // Objekt
					}
					else
					{
						std::vector<CStringVector> vvObject=TranslateObject2(
							*subSentence.m_pObjectFirst,1,subSentence.m_vecPredicate.at(0).
							m_bNegation,b,//pEV,
              rvecintpair,rnStartID);
						//rnStartID+=vvObject.size();
						Append(vv,vvObject); // Objekt
					}
				}
				else if(pgvBend->m_bCase==2)
				{
					BYTE b=0;
					//wenn der Satz ein zweites Objekt enthält
					if(subSentence.m_pObjectSecond)
					{
					//dann beim vorletzten Parameter (Verneinung) FALSE angeben,
					//da nur in das zweite Objekt die Verneinung mit einbezogen wird:
					//"I do not give a boy a book." -> "Ich gebe einem Jungen kein Buch."
						std::vector<CStringVector> vvObject=TranslateObject2(
							*subSentence.m_pObjectFirst,2,FALSE,b,//pEV,
              rvecintpair,rnStartID);
						//rnStartID+=vvObject.size();
						Append(vv,vvObject); // Objekt
					}
					else
					{
						std::vector<CStringVector> vvObject=TranslateObject2(
							*subSentence.m_pObjectFirst,2,subSentence.m_vecPredicate.at(0).
							m_bNegation,b,//pEV,
              rvecintpair,rnStartID);
						//rnStartID+=vvObject.size();
						Append(vv,vvObject); // Objekt
					}
				}
				else
				{
					BYTE b=0;
					//wenn der Satz ein zweites Objekt enthält
					if(subSentence.m_pObjectSecond)
					{
					//dann beim vorletzten Parameter (Verneinung) FALSE angeben,
					//da nur in das zweite Objekt die Verneinung mit einbezogen wird:
					//"I do not give a boy a book." -> "Ich gebe einem Jungen kein Buch."
						std::vector<CStringVector> vvObject=TranslateObject2(
							*subSentence.m_pObjectFirst,0,FALSE,b,//pEV,
              rvecintpair,rnStartID);
						//rnStartID+=vvObject.size();
						Append(vv,vvObject); // Objekt
					}
					else
					{
						std::vector<CStringVector> vvObject=TranslateObject2(
							*subSentence.m_pObjectFirst,0,subSentence.m_vecPredicate.at(0).
							m_bNegation,b,//pEV,
              rvecintpair,rnStartID);
						//rnStartID+=vvObject.size();
						Append(vv,vvObject); // Objekt
					}
				}
			}
		}//if(subSentence.m_pObjectFirst) 
		#ifdef _DEBUG
		else
			printf("!subSentence.m_pObjectFirst\n");
		#endif
		//Adverb der Häufigkeit einfügen:
		//wenn ein Verb 2 Objekte verlangt, steht das Adverb der Häufigkeit zwischen
		//dem ersten und dem zweiten Objekt: "Then I sometimes gave him a book." ->
		//"Dann gab ich ihm manchmal ein Buch."
		if(subSentence.m_pSubject)
		{
			//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
			//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
			//           -----S---- -----P----
			//          "My glasses are broken." -> "Meine Brille ist kaputt."
			//           -----S---- -P- ---A--
			//S: Subjekt P: Prädikat
			//A: Umstandsbestimmung der Art und Weise
			if(subSentence.m_vecPredicate.size()==2)
			{
				for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
				{
					Predicate predicate=subSentence.m_vecPredicate.at(bRun);
					if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
					{
						if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
							m_pWordNodeNext)
						{
							GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
								m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
								m_pWordNodeNext->m_pWord);
							if(ga)
							{
								CStringVector sv;
								sv.push_back(ga->m_strWord);
								vv.push_back(sv);
							}
						}
					}
				}
			}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind
			// der "Untersatz" enthält mehr oder weniger als 2 Prädikate
			else
			{
				Predicate predicate=subSentence.m_vecPredicate.at(0);
				if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated)
				{
					if(predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
						m_pWordNodeNext)
					{
						CStringVector sv;
						GermanAdverb * ga=dynamic_cast<GermanAdverb*>(predicate.
							m_pWordNodeWhereAnEnglishAdverbIsEncapsulated->
							m_pWordNodeNext->m_pWord);
						if(ga)
						{
							sv.push_back(ga->m_strWord);
							vv.push_back(sv);
						}
					}
				}
			}
		}
		if(subSentence.m_pObjectSecond) // 2. Objekt
		{
			#ifdef _DEBUG
			printf("subSentence.m_pObjectSecond\n");
			#endif
			//GermanVerb * gv=GetGermanVocableOfEnglishVerb(subSentence.predicate);
			if(pgvBend)
			{
				//printf("gv
				BYTE b=0;
				std::vector<CStringVector> vvObject=TranslateObject2(
					*subSentence.m_pObjectSecond,2,subSentence.m_vecPredicate.at(0).
					m_bNegation,b,//pEV,
          rvecintpair,rnStartID);
				//rnStartID+=vvObject.size();
				Append(vv,vvObject); // Objekt
			}
		}
    //Wenn Umstandsbestimmung der Art und Weise.
    //Z.B.:Gabst du ihm den Apfel sorgfältig?
    //           S   O1  ---O2--- --A und W-
		if(subSentence.m_pManner)
		{
			std::vector<CStringVector> vvManner=TranslateManner(*subSentence.
				m_pManner,rnStartID);
			rnStartID+=vvManner.size();
			Append(vv,vvManner);
		}
		//wenn die Zeitform "Zukunft" ist, muss der Infinitiv am Ende des Satzes
		//stehen: "I will go very fast to school tomorrow." -> "Ich werde morgen
		//sehr schnell zur Schule gehen." (in diesem Fall steht der Infinitiv 
		//"gehen" am Ende des Satzes)
		if(subSentence.m_pSubject)
		{
			//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
			//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
			//           -----S---- -----P----
			//          "My glasses are broken." -> "Meine Brille ist kaputt."
			//           -----S---- -P- ---A--
			//S: Subjekt P: Prädikat
			//A: Umstandsbestimmung der Art und Weise
			if(subSentence.m_vecPredicate.size()==2)
			{
				CString strBendVerb;
				CString strManner;
				CStringVector sv;
				personIndex++;
				for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
				{
					Predicate predicate=subSentence.m_vecPredicate.at(bRun);
					if(predicate.m_bTense=FUTURE && predicate.m_bActiveClause)
					{
#ifdef _DEBUG
					printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
						if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
						{
							if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
								m_pWordNodeNext)
							{
								CStringVector sv;
								pgvBend=dynamic_cast<GermanVerb*>(predicate.
									m_pWordNodeWhereTheGermanTranslationIsABendVerb->
									m_pWordNodeNext->m_pWord);
			#ifdef _DEBUG
								printf("pgvBend->m_strWords[0]: %s\n",pgvBend->m_strWords[0]);
			#endif
								sv.push_back(pgvBend->m_strWords[0]); // Infinitiv
								vv.push_back(sv); // anhängen
							}
						}
#ifdef _DEBUG
						else
							printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
					}
				}
			}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind
			// der "Untersatz" enthält mehr oder weniger als 2 Prädikate
			else
			{
				std::vector<CStringVector> vvRestOfPredicate=
					TranslateRestOfGermanPredicate(subSentence.m_vecPredicate.at(0));
#ifdef _DEBUG
				printf("subSentence.m_vecPredicate.size()!=2\n");
				printf("subSentence.m_vecPredicate.at(0).m_bTense:%u \n",
					subSentence.m_vecPredicate.at(0).m_bTense);
#endif
				Append(vv,vvRestOfPredicate);
			}
		}
		//CStringVector germanObject=TranslateObject(subSentence.m_Subject);
		//Append(germanClause,germanObject); // Subjekt
		for(DWORD i=0;i<subSentence.m_vecEnglishAdverb.size();i++)
		{
			if(subSentence.m_vecEnglishAdverb.at(i).m_bType=='1') // Adverb der Häufigkeit
			{
				GermanAdverb * ga=GetGermanWordOfEnglishAdverb(subSentence.
					m_vecEnglishAdverb.at(i).m_strWord);
				CStringVector sv;
				sv.push_back(ga->m_strWord);
				vv.push_back(sv);
			}
		}
		for(i=0;i<subSentence.m_vecMiddleEnumerationElement.size();i++)
		{
			if(i>0)
				if((i+1)==subSentence.m_vecMiddleEnumerationElement.size())
				{
					CStringVector sv;
					sv.push_back(_T("und"));
					vv.push_back(sv);
				}
				else
				{
					CStringVector sv;
					sv.push_back(_T(","));
					vv.push_back(sv);
				}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbRefersToAdjectiveOrAdverb!="") // Adverb der Häufigkeit
			{
				CStringVector sv;
				sv.push_back(subSentence.m_vecMiddleEnumerationElement.at(i).
					m_strAdverbRefersToAdjectiveOrAdverb);
				vv.push_back(sv);
			}
			if(subSentence.m_vecMiddleEnumerationElement.at(i).
				m_strAdverbOfManner!="") // Adverb der Art und Weise
			{
				CStringVector sv;
				Word * pWord=GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(
					subSentence.m_vecMiddleEnumerationElement.at(i).m_strAdverbOfManner);
				if(typeid(*pWord)==typeid(GermanAdjective)) // das Adverb der
				{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
					GermanAdjective * ga=dynamic_cast<GermanAdjective*>(pWord);
					if(ga!=NULL)
						sv.push_back(ga->m_strPositiv);
				}
				vv.push_back(sv);
			}
		}
		//Den Rest des Prädikates (ohne die gebeugte Verbform) anhängen.
		//Wenn die Zeitform "Zukunft" ist, muss der Infinitiv am Ende 
		//des Satzes stehen: "That is why I will go very fast to school 
		//tomorrow." -> "Darum werde ich morgen sehr schnell zur Schule 
		//gehen." (in diesem Fall steht der Infinitiv "gehen" am Ende des 
		//Satzes)
		if(subSentence.m_pSubject)
		{
			//wenn bei dem "Untersatz" zwei Prädikate möglich sind.
			//Beispiel: "My glasses are broken." -> "Meine Brille wird gebrochen."
			//           -----S---- -----P----
			//          "My glasses are broken." -> "Meine Brille ist kaputt."
			//           -----S---- -P- ---A--
			//S: Subjekt P: Prädikat
			//A: Umstandsbestimmung der Art und Weise
			if(subSentence.m_vecPredicate.size()==2)
			{
				CString strBendVerb;
				CString strManner;
				CStringVector sv;
#ifdef _DEBUG
				printf("predicate.size()==2\n");
#endif
				personIndex++;
				for(BYTE bRun=0;bRun<subSentence.m_vecPredicate.size();bRun++)
				{
					Predicate predicate=subSentence.m_vecPredicate.at(bRun);
					if(predicate.m_bTense=FUTURE && predicate.m_bActiveClause)
					{
#ifdef _DEBUG
					printf("predicate.m_bTense:%u \n",predicate.m_bTense);
#endif
						if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
						{
							if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
								m_pWordNodeNext)
							{
								CStringVector sv;
								pgvBend=dynamic_cast<GermanVerb*>(predicate.
									m_pWordNodeWhereTheGermanTranslationIsABendVerb->
									m_pWordNodeNext->m_pWord);
			#ifdef _DEBUG
								printf("pgvBend->m_strWords[0]: %s\n",pgvBend->m_strWords[0]);
			#endif
								sv.push_back(pgvBend->m_strWords[0]); // Infinitiv
								vv.push_back(sv); // anhängen
							}
						}
#ifdef _DEBUG
						else
							printf("subSentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
					}
				}
			}//Ende von: wenn beim Haupt- oder Nebensatz 2 Prädikate möglich sind
			//der "Untersatz" enthält mehr oder weniger als 2 Prädikate
			else
			{
				std::vector<CStringVector> vvRestOfPredicate=
					TranslateRestOfGermanPredicate(subSentence.m_vecPredicate.at(0));
				//die Verneinung ("nein") erscheint im Deutschen am Ende eines
				//Haupt- oder Nebensatzes:
				//"I do not like you."->"Ich mag dich nicht."
				//"I can not like you."->"Ich kann dich nicht mögen."
				//"I am not liked."->"Ich werde nicht gemocht."
				//"I can not be liked."->"Ich kann nicht gemocht werden."
				//"I did not like you."->"Ich mochte dich nicht."
				//"I was not liked."->"Ich wurde nicht gemocht."
				//"I will not like you."->"Ich werde dich nicht mögen."
				//"I will not be liked."->"Ich werde nicht gemocht werden."
				//"I will not be walking."->"Ich werde nicht gelaufen sein."
				//"I will not be being liked."->"Ich werde nicht gemocht worden sein."
				//"I will not have liked you."->"Ich werde dich nicht gemocht haben."
				//"I will not have been liked."->"Ich werde nicht gemocht worden sein."
				//"I am not walking."->"Ich laufe (gerade) nicht."
				//"I am not being liked."->"Ich werde (gerade) nicht gemocht."
				//"I was not walking."->"Ich lief (gerade) nicht."
				//"I was not being liked."->"Ich wurde (gerade) nicht gemocht."
				//"I have not liked you."->"Ich habe dich nicht gemocht."
				//"I can not have seen you."->"Ich kann dich nicht gesehen haben."
				//"I have not been liked."->"Ich bin nicht gemocht worden."
				//"I can not have been liked."->"Ich kann nicht gemocht worden sein."
				//"I had not liked you."->"Ich hatte dich nicht gemocht."
				//"I could not have seen you."->"Ich konnte dich nicht gesehen haben."
				//"I had not been liked."->"Ich war nicht gemocht worden."
				//"I could not have been liked."->"Ich konnte nicht gemocht worden sein."
				//"I have not been liking you."->"Ich habe dich (gerade) nicht gemocht."
				//"I can not have been liking you."->"Ich kann dich (gerade) nicht gemocht haben."
				//"I was not been liking."->"Ich bin (gerade) nicht gemocht worden."
				//"I can not have been liking."->"Ich kann (gerade) nicht gemocht worden sein."
				//"I had not been liking you."->"Ich hatte dich (gerade) nicht gemocht."
				//"I could not have been liking you."->"Ich konnte dich (gerade) nicht gemocht haben.")
				//"I had not been liking."->"Ich war (gerade) nicht gemocht worden."
				//"I could not have been liking."->"Ich konnte (gerade) nicht gemocht worden sein."
				if(subSentence.m_vecPredicate.at(0).m_bNegation)
				{
					CStringVector sv;
					sv.push_back(_T("nicht")); // Infinitiv
					vv.push_back(sv); // anhängen
				}
#ifdef _DEBUG
				printf("subSentence.m_vecPredicate.size()!=2\n");
				printf("subSentence.m_vecPredicate.at(0).m_bTense:%u \n",
					subSentence.m_vecPredicate.at(0).m_bTense);
#endif
				Append(vv,vvRestOfPredicate);
			}
		}
	return vv;
}

CString GetGermanPersonalPronoun(CString & str)
{
	if(str=="I")
		return "ich";
	if(str=="you")
		return "du";
	if(str=="he")
		return "er";
	if(str=="she")
		return "sie";
	if(str=="it")
		return "es";
	if(str=="we")
		return "wir";
	if(str=="they")
		return "sie";
	return "";
}
