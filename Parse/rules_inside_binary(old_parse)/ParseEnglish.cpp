//Compile with compiler option/cmd line arg "-std=c99" for "g++" compiler 
//because of lines like this:
//"for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)"
//Use "-fno-for-scope" for the compiler because of code like this:  
	//"for(i=0;i<dwBeginOfEnglishNoun && g_bContinue;i++)". Else error message
	//like "error: name lookup of `dwRun' changed for new ISO `for' scoping".

//Ohne das folgende include: fatal error C1010: Unerwartetes Dateiende während der Suche nach dem vorkompilierten Header.
#include "StdAfx.h"
#include "GetAndTestWord.h" //for GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated etc.
#include "ParseEnglish.h"
#include "SentenceElement.h" //for "class Clause" etc.
#include "rest.h" //for "class Range" etc.
#include "Token.h" //for "PositionCStringVector" etc.
////Declare forward (faster than "#include "Word.h") for use in
////"BOOL ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv)"
//class EnglishAuxiliaryVerb;
#include "Word.hpp" //for "EnglishAuxiliaryVerb" etc.
//for std::set<VocabularyAndTranslation> * psetvocabularyandtranslation ; in "void Parse(const PositionCStringVector & psv)"
#include <set> 
#include "VocabularyInMainMem\LetterTree\LetterTree.hpp" //for Parse(...)
#include "VocabularyInMainMem/DoublyLinkedList/WordNode.hpp"
//#include "

extern std::ofstream ofstreamLogFile;
extern volatile BYTE g_bContinue ;
extern volatile DWORD g_dwStatus ;
extern volatile DWORD g_dwSentenceStatus ;

class LetterTree ;
class LetterNode ;
class VocabularyAndTranslation ;
extern LetterTree g_lettertree ;

BYTE GetIndexFromPossessivePronoun(const CString & str)
{
	if(str=="my")
		return 1;
	if(str=="your")
		return 2;
	if(str=="his")
		return 3;
	if(str=="her")
		return 4;
	if(str=="its")
		return 5;
	if(str=="our")
		return 6;
	if(str=="their")
		return 7;
	return 0; // Fehler
}

BYTE GetPersonIndex(CString str)
{
	/*BOOL b1=FALSE; // I
	BOOL b2=FALSE; // you
	BOOL b3=FALSE; // he
	BOOL b4=FALSE; // we
	BOOL b5=FALSE; // you Mehrzahl (ihr)
	BOOL b6=FALSE; // they
	//BOOL bPlural=FALSE;
	CString token;
	for(int i=0;(token=GetTokenAt(str,i))!="";i++)
	{
		if(token=="I")
		{
			if(b2 || b3 || b4 || b5 || b6)
				b4=TRUE;
			b1=TRUE;
		}
		if(token=="you")
		{
			if(b1) // 'I' und 'you' == we
				b4=TRUE;
			if(b3 || b6) // 'you' und 'he' oder 'you' und 'they' == you (ihr)
				b5=TRUE; // ihr
			b2=TRUE;		
		}
		if(token=="he" || token=="she" || token=="it")
			b3=TRUE;
		if(token=="we")
			b4=TRUE;
		if(token=="they")
			b6=TRUE;
		if(IsEnglishNoun(token))
			b6=TRUE;
	}
	if(b4)
		return 3;
	if(b5)
		return 4;
	if(b6)
		return 5;
	if(b2)
		return 1;
	if(b1)
		return 0;*/
	if(str=="I")
		return 1;
	if(str=="you")
		return 2;
	if(str=="he" || str=="she" || str=="it")
		return 3;
	if(str=="we")
		return 4;
	if(str=="they")
		return 6;
	return 0;
}

BYTE GetPersonIndex(BYTE indexBefore,BYTE newIndex)
{
	BYTE personIndex=indexBefore;
	BYTE ret=newIndex;
	if(personIndex==0)
		personIndex=newIndex;
			if(personIndex==1) // I
			{
				if(ret==2 || ret==3 || ret==4 || ret==6)
					personIndex=4; // wir
			}
			if(personIndex==2) // you (2. Person  Singular)
			{
				if(ret==1)
					personIndex=4; // we
				if(ret==3 || ret==5 || ret==6)
					personIndex=5; // ihr
				if(ret==4) // we
					personIndex=4; // wir
			}
			if(personIndex==3) // he/she/it (3. Person  Singular)
			{
				if(ret==1)
					personIndex=4; // we
				if(ret==2 || ret==5)
					personIndex=5; // ihr
				if(ret==4) // we
					personIndex=4; // wir
				if(ret==3 || ret==6) // they
					personIndex=4; // sie
			}
			if(personIndex==4) // we (1. Person Plural)
			{
				if(ret==1 || ret==2 || ret==3 || ret==5 || ret==6)
					personIndex=4; // we
			}
			if(personIndex==5) // you (2. Person Plural) (deutsch: ihr)
			{
				if(ret==1 || ret==4)
					personIndex=4; // we
			}
			if(personIndex==6) // they (3. Person Plural) (deutsch: sie)
			{
				if(ret==1 || ret==4)
					personIndex=4; // we
				if(ret==2 || ret==5)
					personIndex=5; // ihr
			}
	return personIndex;
}

//@return 
// 2, wenn sich mindestens ein vermeintlicher Eigenname 
//  im Haupt- oder Nebensatz befindet, 
// 1 wenn sich keine Eigenname im Haupt- 
//  oder Nebensatz befindet, 0 wenn der Haupt- oder Nebensatz grammatikalisch
//inkorrekt ist
//mit "clause" ist ein Haupt- oder Nebensatz (mit Nebensatz ist 
//NICHT Relativsatz gemeint) /nicht-zusammengesetzer Satz gemeint,
//also NICHT mehrere Haupt- oder Nebensätze hintereinander
BYTE ParseClause(Clause & subsentence,
	const PositionCStringVector & psv,std::vector<Range> & rVecRange,
	const DWORD & dwFlags,BYTE bContainsFirstTokenOfSentence)
{
	Subject subject;
	BOOL bConjunction=FALSE;
	BOOL bMiddle=FALSE;
	BOOL bObject=FALSE;
	BOOL bObjectVerb=FALSE;
	BOOL bParsedSubject=FALSE;
	BOOL bParsedMiddle=FALSE;
	BOOL bPredicateWithActiveToBe=FALSE;
	BOOL bPredicateRest=FALSE;
	BOOL bVerb=FALSE;
	BOOL bValidByAgent=FALSE;
	BOOL bValidManner=FALSE;
	BOOL bValidMiddle=FALSE;
	BOOL bValidPlace=FALSE;
	BOOL bValidTime=FALSE;
	BOOL bValidSubject=FALSE;
	BOOL bValidObject=FALSE;
	BYTE returnValue=//2
    CLAUSE_EXCLUSIVELY_CONTAINS_KNOWN_WORDS;
	BYTE personIndex=0;
	BYTE bValidImperativ=FALSE;
	BYTE bObjectVerbPossible=TRUE;
	DWORD dwEndOfConjunction=0;
	DWORD dwEndOfObject=0;
	DWORD dwEndOfValidSentenceElement=0; // Ende eines gültigen Satzgliedes
	DWORD dwStartOfNextSentenceElement=0;
	DWORD dwEndOfValidManner=0;
	DWORD dwEndOfValidMiddle=0;
	DWORD dwEndOfValidMiddleWithActiveToBe=0;
	DWORD dwStartOfFirstSentenceElementAfterMiddle=0;
	DWORD dwStartOfValidMiddle=0;
	DWORD dwEndOfElementBeforeMiddle=dwStartOfNextSentenceElement;
	DWORD dwStart=0;
	DWORD dwEnd=0;
	DWORD dwEndOfValidSubject=0;
	DWORD dwEndOfValidObject=0;
	Gerund gerund;
	int start=0;
	int end=0;
	std::vector<BYTE> vecByte;
	std::vector<Range> vecRange;
	WordNode * pWordNode=NULL;
	//SubSentence * subsentence=new SubSentence();
#ifdef _DEBUG
	printf("BYTE ParseClause(Clause & subsentence,\
const PositionCStringVector & psv,std::vector<Range> & rVecRange,\
const DWORD & dwFlags,BYTE bContainsFirstTokenOfSentence(%u)) ANFANG\n",
	bContainsFirstTokenOfSentence);
	printf("g_bContinue: %u\n",g_bContinue);
	printf("subsentence.m_vecPredicate.size(): %u\n",
		subsentence.m_vecPredicate.size());
#endif
	subsentence.m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.clear();
	#ifdef _DEBUG
	for(DWORD i2=0;i2<psv.size();i2++)
	{
		printf("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
	}
	#endif
	subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated=NULL;
	//es können auch mehrere Konjunktionen hintereinander stehen Beispiel: 
	//"But if you are intelligent." (="Aber falls du intelligent bist)
	//Konjunktionen: "But", "if"
	for(DWORD dwRun=0;dwRun<psv.size() && g_bContinue;dwRun++)
	{
    LOG("23.05.2008 11.14.41 "<<dwRun<<"\n");
		BOOL bValidConjunction=FALSE;
		for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
		{
#ifdef _DEBUG
			printf("dwRun2: %u\n",dwRun2);
#endif
			CStringVector sv=GetBetweenAsCStringVector(psv,dwRun,dwRun2);
			if(!(dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)//wenn die Groß- und Kleinschreibung nicht relevant ist
				for(DWORD dwRun3=0;dwRun3<sv.size();dwRun3++)
					sv.at(dwRun3).MakeLower();
			else if(bContainsFirstTokenOfSentence)
				MakeFirstLetterLower(sv.at(0));
			if((pWordNode=GetPointerToWordNodeWhereAnEnglishConjunctionIsEncapsulated(sv)))
			{
				subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated=
					pWordNode;
				subsentence.m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.
					push_back(pWordNode);
				dwEndOfConjunction=dwRun2;
				bConjunction=TRUE;
				#ifdef _DEBUG
				printf("GetPointerToWordNodeWhereAnEnglishConjunctionIsEncapsula\
ted(GetBetween(psv,0,dwRun))==TRUE\n");
				#endif
				bValidConjunction=TRUE;
			}
		#ifdef _DEBUG
			else
			{
				printf("GetPointerToWordNodeWhereAnEnglishConjunctionIsEncapsula\
ted(GetBetween(psv,0,dwRun))==FALSE\n");
				if(pWordNode==NULL)
					printf("pWordNode==NULL\n");
			}
		#endif
		}
		if(bValidConjunction)
		{
			dwRun=dwEndOfConjunction;//dwRun2+1;
			dwStartOfNextSentenceElement=dwEndOfConjunction+1;
			dwEndOfValidSentenceElement=dwEndOfConjunction;
			bContainsFirstTokenOfSentence=FALSE;
		}
		else // die Konjunktionen dürfen nur am Satzbeginn stehen
			break;
	}
	#ifdef _DEBUG
	if(subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated)
		printf("subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated\n");
	else
		printf("!subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated\n");
	#endif
  LOG("19.05.2008 20.44\n");
	//ein Gerundium kann das Subjekt eines Satzes sein.
	//Beispiel: "Leaving your home country is a big step." 
	//           --------Gerundium-------- -P -----O----  P:Prädikat O:Objekt
	//-> "Deine Heimat zu verlassen ist ein großer Schritt."
	if(ParseGerund(gerund,TRUE,FALSE,psv,vecRange,dwFlags,
		bContainsFirstTokenOfSentence))
	{
		if(subsentence.m_pGerund)
			delete subsentence.m_pGerund;
		subsentence.m_pGerund=new Gerund(gerund);
		#ifdef _DEBUG
		printf("rVecRange.size(): %u\n",rVecRange.size());
		printf("BOOL ParseClause(Clause & subsentence,const \
PositionCStringVector & psv) - ENDE - return TRUE\n");
		#endif
		return TRUE;
	}
	if(subsentence.m_pGerund)
		bContainsFirstTokenOfSentence=FALSE;
	//ein "Untersatz" kann am Anfang ein Adverb (Umstandsbestimmung) der 
	//Zeit beinhalten.
	//Beispiel: "Now you are great." -> "Jetzt bist du grossartig."
	// Adverb der Zeit: "Now"
	if(psv.size()>dwStartOfNextSentenceElement)
	{
		CString str=psv.at(dwStartOfNextSentenceElement).m_Str;
		if(bContainsFirstTokenOfSentence)
			MakeFirstLetterLower(str);
		if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
			str)) )
		{
			#ifdef _DEBUG
			printf("Adverb\n");
			#endif
			EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
			if(ea)
				if(ea->m_bType==5) // Adverb der Zeit ('today')
				{
					#ifdef _DEBUG
					printf("Adverb der Zeit\n");
					#endif
					subsentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=
						pWordNode;
					//subsentence.m_bWordOrder=PSO;
					dwStartOfNextSentenceElement++;
					bContainsFirstTokenOfSentence=FALSE;
				}
		}
	}
	if(psv.size()>dwStartOfNextSentenceElement)
	{
		//if(!bValidMiddle)
		CString str=psv.at(dwStartOfNextSentenceElement).m_Str;
		if(bContainsFirstTokenOfSentence)
			MakeFirstLetterLower(str);
			//wenn Imperativ
			if((pWordNode=GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(
				str)))
			{
				Predicate predicate;
				subsentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
				subsentence.m_bTense=1;
				bValidImperativ=TRUE;
				//predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb=
				//	pWordNode;
        //Im Englischen ist der Imperativ zwar identisch mit dem Infinitiv(
        //sieht man sehr gut beim Verb "to be", bei dem es viele verschiede 
        //gebeugte Verbformen gibt: "Be careful", im Deutschen: "Sei|seid vorsichtig".
				predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb=
					pWordNode;
				predicate.m_bTense=1;
				predicate.m_bActiveClause=1;
				predicate.m_bNegation=0;
				subsentence.m_vecPredicate.push_back(predicate);
				dwStartOfNextSentenceElement++;
				bContainsFirstTokenOfSentence=FALSE;
			}
	}
	//Prüfung auf gültiges Mittelteil erst ab dem Token hinter 
	//dwStartOfNextSentenceElement, denn wenn das kein Aufforderungssatz 
	//(bValidImperativ==FALSE) ist, muss sich ein Subjekt direkt vor dem 
	//Mittelteil befinden. Ein Subjekt besteht aus mindestens einem Token.
	if(!bValidImperativ && psv.size()>dwStartOfNextSentenceElement+1)
	{
		BOOL bThisPredicateIsWithActiveToBe=FALSE;
		BYTE bValidPredicate=FALSE;
		BYTE bPersonIndex=0;
		DWORD dwEndOfSubjectWithPersonalNames=0;
		DWORD dwEndOfSubjectWithoutPersonalNames=0;
		DWORD dwEndOfSubjectWithRelativeClause=0;
		Predicate predicateToBe;
		Predicate predicateRest;
		Subject subjectWithPersonalNames;
		Subject subjectWithRelativeClause;
		Subject subjectWithoutPersonalNames;
		std::vector <DWORD> vectordwStartOfPredicate;
		std::vector <DWORD> vectordwEndOfPredicate;
		std::vector <Predicate> vectorPredicate;
		Predicate predicate,predicateValid;
#ifdef _DEBUG
		printf("!bValidImperativ && psv.size()>dwStartOfNextSentenceElement+1\n");
          if(psv.size()==7)
            bPersonIndex=bPersonIndex;
#endif
		//es kann passieren, dass mehrere Prädikate in einem "Untersatz" gefunden
		//werden. Dann kann es sein, dass ein oder mehrere Prädikate zu 
		//einem Relativsatz oder mehreren Relativsätzen gehört/gehören.
		//Wenn zum Beispiel 2 Prädikate existieren, kann das letzte zum 
		//Relativsatz gehören: "You are the man I hate." Relativsatz: "I hate"
		//Damit bei der Überprüfung, ob die gebeugte Verbform des Prädikats mit
		//dem Subjekt übereinstimmt, nur noch ein Prädikat übrigbleibt,
		//existiert eine verschachtelte Schleife, in der geprüft wird, ob ein
		//Prädikat zu einem Relativsatz gehört und - falls nicht - an einen neuen 
		//Prädikatvektor angehängt
		for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size();++dwRun)
		{
			BYTE bReturnOfParseMiddle;
			DWORD	dwEndOfPredicate=0;
		  bValidPredicate=FALSE;
			BYTE bPersonIndexValid;
			for(DWORD dwRun2=dwRun;dwRun2<psv.size();++dwRun2)
			{
#ifdef _LOG
				if(dwRun==1 && dwRun2==3)
					LOG("");
#endif
        //Reset the pointers(they may have been set from before 
        //and would have been overtaken->possible side effects).
        predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb=NULL;
        predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb=NULL;
        predicate.m_bActiveClause=true;
        predicate.m_bNegation=false;
				//das ParseMiddle kann TRUE zurückliefern, obwohl das Prädikat
				//nicht vollständig ist: "I am walking.": wenn ParseMiddle 'am'
				//als Parameter übermittelt bekommt, ist 'am' als Prädikat zwar
				//grundsätzlich richtig, doch das Prädikat in diesem Haupt- oder
				//Nebensatz schliesst noch 'walking' ein.
				//Deswegen kann man nicht gleich beim ersten Mal, wenn ParseMiddle
				//TRUE zurückliefert, aus der inneren Schleife ausbrechen
				//wenn das Prädikat grammatikalisch korrekt ist
				if((bReturnOfParseMiddle=ParseFullPredicate(predicate,TRUE,
					GetBetweenAsCStringVector(psv,dwRun,dwRun2),0,dwFlags)))
				{
#ifdef _DEBUG
					printf("(bReturnOfParseMiddle=ParseFullPredicate(predicate,TRUE,\
GetBetweenAsCStringVector(psv,dwRun,dwRun2),0,dwFlags))\n");
#endif
					if(vectordwEndOfPredicate.size()>0)
					{
#ifdef _DEBUG
						printf("vectordwEndOfPredicate.size()>0\n");
#endif
					//wenn der Anfang dieses Prädikats vor oder beim letzten Token des
					//letzten Prädikats beginnt, überschneiden sich die Prädikate und
					//dieses Prädikat darf dem Vektor nicht angehängt werden
					//Beispiel: "The number you have dialled is incomplete."
					//                          +----------+
					//													 |	 +-----+
					//													 |			 |
					//							letztes Prädikat		dieses Prädikat
						if(dwRun>vectordwEndOfPredicate.at(vectordwEndOfPredicate.size()-1))
						{
							bValidPredicate=TRUE;
							bValidMiddle=TRUE;
							predicateValid=predicate;
							//die folgende Zeile auskommentiert und ausgetauscht gegen
							//die Zeile unterhalb, da bei Relativsätzen falsche
							//zulässive Flexionsparadigmen waren (vom Prädikat vom
							//Relativsatz anstatt vom Prädikat des umgebenden Satzes.
							//bPersonIndexValid=bPersonIndex;
							bPersonIndexValid=bReturnOfParseMiddle;
							//dwStartOfPredicate=dwRun;
							dwEndOfPredicate=dwRun2;
	#ifdef _DEBUG
							printf("dwRun2: %u\n",dwRun2);
	#endif
						}
					}
					//wenn vectordwEndOfPredicate.size() gleich 0 ist
					else
					{
#ifdef _DEBUG
						printf("!vectordwEndOfPredicate.size()>0\n");
#endif
						//erst hier zuweisen, da sonst zum Beispiel bei 
						//"has worked" das "worked" für gültige 
						//Personalpronomen genommen 
						//die gebeugte Verbform befindet sich ja am Anfang
						bPersonIndex=bReturnOfParseMiddle;
						bValidPredicate=TRUE;
						bValidMiddle=TRUE;
						predicateValid=predicate;
						bPersonIndexValid=bPersonIndex;
						dwEndOfPredicate=dwRun2;
	#ifdef _DEBUG
						printf("dwRun2: %u\n",dwRun2);
	#endif
						//vectordwEndOfPredicate.push_back(dwRun2);
					}
				}//if(ParseFullPredicate(...)
        LOG("23.05.2008 11.15.08 "<<(short)predicate.m_bTense<<" "<<(short)predicate.m_bActiveClause<<"\n");
        //if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
          //LOG("PC-W:"<<predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWordNodeNext->m_pWord
			}//for(DWORD dwRun2=dwRun;dwRun2<psv.size();++dwRun2)
#ifdef _DEBUG
			printf("nach dem Ende der for-Schleife, in der das \"Mittelteil\" \
geparst wird\n");
#endif
		  if(bValidPredicate)
			{
				vectorPredicate.push_back(predicateValid);
				vecByte.push_back(bPersonIndexValid);
				vectordwEndOfPredicate.push_back(dwEndOfPredicate);
				vectordwStartOfPredicate.push_back(dwRun);
			}
#ifdef _DEBUG
			printf("nach push_back\n");
#endif
		}//for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size();++dwRun)
#ifdef _DEBUG
		printf("nach der äußeren Schleife, in der das Mittelteil geparst wird\n");
		printf("vectorPredicate.size(): %d\n",vectorPredicate.size());
		printf("vectordwEndOfPredicate.size(): %d\n",vectordwEndOfPredicate.
size());
		printf("vectordwStartOfPredicate.size(): %d\n",vectordwStartOfPredicate.
size());
#endif
		BYTE bValidSubjectOrObject=FALSE;
		BYTE bHasObject=FALSE;
		if(vectorPredicate.size()==0)
		{
#ifdef _DEBUG
			printf("BYTE ParseClause(Clause & subsentence,\
const PositionCStringVector & psv,std::vector<Range> & rVecRange,\
const DWORD & dwFlags,BYTE bContainsFirstTokenOfSentence return 0-\
vectorPredicate.size()==0\n");
#endif
			return 0;
		}
#ifdef _DEBUG
		for(dwRun=0;dwRun<vectordwEndOfPredicate.size();++dwRun)
			printf("vectordwEndOfPredicate.at(%u): %u\n",dwRun,
				vectordwEndOfPredicate.at(dwRun));
		printf("vectorPredicate.size(): %u\n",vectorPredicate.size());
		for(dwRun=0;dwRun<vectorPredicate.size();dwRun++)
		{
			printf("vectorPredicate.at(dwRun).m_bNegation: %u \
vectorPredicate.at(dwRun).m_bActiveClause: %u \
vectorPredicate.at(dwRun).m_bTense: %u\n",
				vectorPredicate.at(dwRun).m_bNegation,
				vectorPredicate.at(dwRun).m_bActiveClause,
				vectorPredicate.at(dwRun).m_bTense);
			if(vectorPredicate.at(dwRun).
				m_pWordNodeWhereTheGermanTranslationIsABendVerb)
			{
				printf("vectorPredicate.at(dwRun).\
m_pWordNodeWhereAnEnglishVerbIsEncapsulated\n");
				if(typeid(vectorPredicate.at(dwRun).
					m_pWordNodeWhereTheGermanTranslationIsABendVerb)==
					typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
						(vectorPredicate.at(dwRun).
						m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
					if(eav)
						printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]);
				}
				if(typeid(vectorPredicate.at(dwRun).
					m_pWordNodeWhereTheGermanTranslationIsABendVerb)==
					typeid(EnglishVerb))
				{
					EnglishVerb * ev=dynamic_cast<EnglishVerb *>
						(vectorPredicate.at(dwRun).
						m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
					if(ev)
						printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
				}
			}
			if(vectorPredicate.at(dwRun).
				m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
			{
				printf("vectorPredicate.at(dwRun).\
m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated\n");
				if(typeid(*vectorPredicate.at(dwRun).
					m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord)==
					typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
						(vectorPredicate.at(dwRun).
						m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord);
					if(eav)
						printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]);
				}
				if(typeid(*vectorPredicate.at(dwRun).
					m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord)==
					typeid(EnglishVerb))
				{
					EnglishVerb * ev=dynamic_cast<EnglishVerb *>
						(vectorPredicate.at(dwRun).
						m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord);
					if(ev)
						printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
				}
			}
		}
#endif//_DEBUG
		//wenn der Haupt- oder Nebensatz mehr als 1 Prädikat enthält
		if(vectorPredicate.size()>1)
		{
		  BYTE bBreak2ndLoopFromInside=FALSE;
			std::vector<BYTE> vecBYTESubSentence;
			//jetzt wird das Prädikat des Haupt oder Nebensatzes gesucht:
			//wenn vor sich vor dem Prädikat ein Artikel und ein Nomen befinden und
			//zwischen dem Artikel plus Nomen und dem Prädikat ein Subjekt ist, das
			//keine Eigennamen enthält, ist dieses Prädikat NICHT das Prädikat des 
			//Haupt- oder Nebensatzes, das gesucht wird, sondern das Prädikat eines
			//Relativsatzes
		  //maximal alle Prädikate durchlaufen
			//The number you have dialled is incomplete.
			//Art Nomen  SOE --Prädikat--
			//You are the air I am breathing.
			//        Art Nom E --Prädikat--
			//Art:Artikel  Nom:Nomen  SOE/E:Subjekt ohne Eigennamen
			//es wird mit dem letzten Prädikat begonnen
		  for(DWORD dwRun=vectorPredicate.size()-1;dwRun>=0 && g_bContinue;--dwRun)
		  {
				DWORD dwStartOfPredicate=vectordwStartOfPredicate.at(dwRun);
		    bBreak2ndLoopFromInside=FALSE;
#ifdef _DEBUG
				printf("Schleife 1 dwStartOfPredicate: %u\n",dwStartOfPredicate);
#endif
				//dwStartOfNextSentenceElement entspricht dem Index des Tokens hinter
				//einem Imperativ, dem Index eines Tokens hinter dem Adverb der Zeit
				//am Anfang des Satzes, dem Index eines Tokens hinter der letzten 
				//Konjunktion oder 0
		    if(dwStartOfPredicate>dwStartOfNextSentenceElement+2)
				{
			    for(DWORD dwRun2=dwStartOfPredicate-3;dwRun2>=
						dwStartOfNextSentenceElement && dwRun2!=4294967295 && g_bContinue;
						--dwRun2)
			    {
#ifdef _DEBUG
						printf("Schleife 2 dwRun2: %u dwStartOfNextSentenceElement: %u\n"
						,dwRun2,dwStartOfNextSentenceElement);
#endif
				    for(DWORD dwRun3=dwRun2+1;dwRun3<dwStartOfPredicate && g_bContinue;
							++dwRun3)
			      {
#ifdef _DEBUG
							printf("Schleife 3\n");
#endif
							EnglishNounRecord enr;
							CStringVector sv = GetBetweenAsCStringVector(psv,dwRun2,dwRun3) ;
					    //eine Struktur aus einem bestimmten englischen Artikel und einem Nomen
			        //besteht immer mindestens aus 2 Token
					    //Beispiel: "the number"
							//ein Subjekt besteht immer mindestens aus einem 1 Token
					    //Deswegen soll nach einem Artikel plus Nomen erst 3 Token vor
						  //dem Anfang des Prädikats geparst werden
							//Beispiel: I like the number you dialled.
			        //                                +-----+
					    //                                   ^
							//                                   |
				      //                                Prädikat
					    //erste Prüfung auf ParseDefiniteArticleNoun: "you"
						  //zweite Prüfung auf ParseDefiniteArticleNoun: "number"
							//dritte Prüfung auf ParseDefiniteArticleNoun: "number you"
							//vierte Prüfung auf ParseDefiniteArticleNoun: "the"
			        //fünfte Prüfung auf ParseDefiniteArticleNoun: "the number"
					    //ParseDefiniteArticleNoun gibt TRUE zurück
				      //Prüfung auf ParseSubject: "you"
					    //ParseSubject gibt 1 zurück
						  if(ParseDefiniteArticleNoun(enr,sv,dwFlags,FALSE))
							{
								std::vector<Range> vecRange;
					      //wenn das Subjekt keine Eigennamen enthält
							  if(ParseSubject(subject,GetBetween(psv,dwRun3+1, 
					        dwStartOfPredicate-1),FALSE,vecRange,dwFlags,
									bContainsFirstTokenOfSentence)==1)
									//das Prädikat gehört zu einem Relativsatz, also Flag setzen, 
									//um schnellstmöglich zum nächsten Prädikat zu kommen
					        bBreak2ndLoopFromInside=TRUE;
			        }
					    if(bBreak2ndLoopFromInside)
							  break;
			      }
					  if(bBreak2ndLoopFromInside)
							break;
					}
				}
				//!dwStartOfPredicate>dwStartOfNextSentenceElement+2
				else
				{
#ifdef _DEBUG
					printf("!(dwStartOfPredicate>dwStartOfNextSentenceElement+2)\n");
#endif
					std::vector<Range> vecRange;
				  subsentence.m_pSubject=new Subject();
		      returnValue=ParseSubject(*subsentence.m_pSubject,GetBetween(psv,
						dwStartOfNextSentenceElement,dwStartOfPredicate-1),FALSE,vecRange,
						dwFlags,bContainsFirstTokenOfSentence);
					//if(returnValue==2)
					//	returnValue=1;
					//else if(returnValue==1)
					//	returnValue=2;
					for(DWORD dwRun2=0;dwRun2<vecRange.size();dwRun2++)
						rVecRange.push_back(vecRange.at(dwRun2));
#ifdef _DEBUG
					printf("vectordwEndOfPredicate.at(%u): %u\n",dwRun,
						vectordwEndOfPredicate.at(dwRun));
					printf("subsentence.m_pSubject->m_vecEnumerationElement.size(): %d\n",
						subsentence.m_pSubject->m_vecEnumerationElement.size());
#endif
					dwEndOfValidMiddle=vectordwEndOfPredicate.at(dwRun);
				  dwStartOfNextSentenceElement=vectordwEndOfPredicate.at(dwRun)+1;
					predicateRest=vectorPredicate.at(dwRun);
					//subsentence.m_vecPredicate.push_back(predicateRest);
					vecBYTESubSentence.push_back(vecByte.at(dwRun));
					bPredicateRest=TRUE;
					break;
				}
		    //wenn nicht: bestimmter Artikel plus Nomen und zwischen 
				//dem bestimmten Artikel
				//plus Nomen und einem Prädikat ein Subjekt ohne Eigennamen 
				//vorkam, dann ist
		    //das Prädikat das gesuchte Prädikat
				if(!bBreak2ndLoopFromInside)
		    {
#ifdef _DEBUG
					printf("!bBreak2ndLoopFromInside\n");
#endif
					std::vector<Range> vecRange;
				  subsentence.m_pSubject=new Subject();
					returnValue=ParseSubject(*subsentence.m_pSubject,GetBetween(psv,
						dwStartOfNextSentenceElement,dwStartOfPredicate-1),FALSE,vecRange,
						dwFlags,bContainsFirstTokenOfSentence);
					//if(returnValue==2)
					//	returnValue=1;
					//else if(returnValue==1)
					//	returnValue=2;
					for(DWORD dwRun2=0;dwRun2<vecRange.size();dwRun2++)
						rVecRange.push_back(vecRange.at(dwRun2));
#ifdef _DEBUG
					printf("vectordwEndOfPredicate.at(%u): %u\n",dwRun,
						vectordwEndOfPredicate.at(dwRun));
					printf("subsentence.m_pSubject->m_enumerationElements.size(): %d\n",
						subsentence.m_pSubject->m_vecEnumerationElement.size());
					printf("subsentence.m_vecPredicate.size(): %u\n",subsentence.
						m_vecPredicate.size());
#endif
					dwEndOfValidMiddle=vectordwEndOfPredicate.at(dwRun);
		      dwStartOfNextSentenceElement=vectordwEndOfPredicate.at(dwRun)+1;
					predicateRest=vectorPredicate.at(dwRun);
					//subsentence.m_vecPredicate.push_back(predicateRest);
					vecBYTESubSentence.push_back(vecByte.at(dwRun));
					bPredicateRest=TRUE;
				  break;
		    }
		  }
		  //if(!
			//die folgende Zeile auskommentiert und ausgetauscht gegen
			//die Zeile unterhalb, da bei Relativsätzen falsche
			//zulässive Flexionsparadigmen waren (vom Prädikat vom
			//Relativsatz anstatt vom Prädikat des umgebenden Satzes.
			//subsentence.m_vecPredicate.size() war nämlich 1 bei einem
			//Satz, der auf Übereinstimmung von Prädikat und Subjekt im
			//Flexionsparadigma geprüft hätte werden sollen
			//if(subsentence.m_vecPredicate.size()==1)
			if(vecBYTESubSentence.size()==1)
			{
				if(!((vecBYTESubSentence.at(0)>>subsentence.m_pSubject->m_personIndex)&1))
				{
          //See http://en.wikipedia.org/wiki/Agreement_%28linguistics%29
          subsentence.m_dwErrorCode=NoGrammaticalPersonAgreement;
	#ifdef _DEBUG
					printf("BYTE ParseClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
dwFlags,BYTE bContainsFirstTokenOfSentence - return FALSE - falscher Personenindex\n");
	#endif
					return FALSE;
				}
			}
		}
		//wenn nur ein Prädikat vorhanden ist
		else
		{
			std::vector<Range> vecRange;
			subsentence.m_pSubject=new Subject();
		  returnValue=ParseSubject(*subsentence.m_pSubject,GetBetween(psv,
				dwStartOfNextSentenceElement,vectordwStartOfPredicate.at(0)-1),
				FALSE,vecRange,dwFlags,bContainsFirstTokenOfSentence);
#ifdef _DEBUG
			printf("bPersonIndex: %u subsentence.m_pSubject->m_personIndex: %u\n",
				bPersonIndex,subsentence.m_pSubject->m_personIndex);
			printf("(bPersonIndex>>subsentence.m_pSubject->m_personIndex)&1: %u\n",
				(bPersonIndex>>subsentence.m_pSubject->m_personIndex)&1);
#endif
			//wenn der Personenindex des Prädikats nicht mit dem Personenindex des
			//Subjekts übereinstimmt
			//Beispiel, wenn er nicht übereinstimmt: "I likes you."
			//8 Bit von bPersonIndex geben an, welcher Personenindex bei einem
			//gebeugtem Verb möglich ist
			//Beispiel: burns:
			//Infinitiv: "burn" : stimmt nicht überein mit "burns"            0
			//Personenindex 1: "I burn" : stimmt nicht überein mit "burns"    0
			//Personenindex 2: "you burn" : stimmt nicht überein mit "burns"  0
			//Personenindex 3: "he/she/it burns" : stimmt überein mit "burns" 1
			//Personenindex 4: "we burn" : stimmt nicht überein mit "burns"   0
			//Personenindex 5: "you burn" : stimmt nicht überein mit "burns"  0
			//Personenindex 6: "they burn" : stimmt nicht überein mit "burns" 0
			//                                   Bit 8 bleibt ungenutzt, also 0
			//das resultierende Byte sieht so aus: 00010000 (dezimal: 16)
			//wenn die Bits des Byte nun nach rechts verschoben werden mit der
			//Anzahl der Stellen des Personenindex des Subjekts und das Ergebnis 
			//bitweise-Und mit 1 verknüpft wird und ungleich
			//"0" ist, stimmen der Personenindex des Subjekts und des Prädikats
			//überein
			//wenn Personenindex 3 (zum Beispiel "he"): 00001000 >> 3= 00000001
			//00000001 & 00000001=00000001 (also ungleich "0")
			if(!((bPersonIndex>>subsentence.m_pSubject->m_personIndex)&1))
			{
	#ifdef _DEBUG
				printf("BYTE ParseClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
dwFlags,BYTE bContainsFirstTokenOfSentence - return FALSE - falscher Personenindex\n");
	#endif
				return FALSE;
			}
			//if(returnValue==2)
			//	returnValue=1;
			//else if(returnValue==1)
			//	returnValue=2;
			for(DWORD dwRun=0;dwRun<vecRange.size();++dwRun)
				rVecRange.push_back(vecRange.at(dwRun));
		  dwStartOfNextSentenceElement=vectordwEndOfPredicate.at(0)+1;
			dwEndOfValidMiddle=vectordwEndOfPredicate.at(0);
			predicateRest=vectorPredicate.at(0);
			bPredicateRest=TRUE;
#ifdef _DEBUG
			printf("am Ende von else-Zweig der Bedingung if(vectorPredicate.size()>1)\n");
#endif
		}
		/*for(DWORD dwRun=dwStartOfNextSentenceElement+1;dwRun<psv.size() && 
			g_bContinue;dwRun++)
		{
			for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
			{
				Predicate predicate;
				bThisPredicateIsWithActiveToBe=FALSE;
				if(ParseMiddle(predicate,TRUE,GetBetweenAsCStringVector(psv,dwRun,
					dwRun2),0,dwFlags))
				{
					BYTE bReturnValueOfParseSubject=0;
#ifdef _DEBUG
					if(predicate.m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated)
					{
						printf("predicate.m_pVocNodeWhereAnEnglishInfinitiveIsEnca\
psulated\n");
						printf("typeid(predicate.m_pVocNodeWhereAnEnglishInfinitive\
IsEncapsulated %s\n",typeid(*predicate.
							m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated->voc));
					}
#endif
					vecRange.clear();
					bReturnValueOfParseSubject=ParseSubject(subject,GetBetween(psv,
						0,dwRun-1),TRUE,vecRange,dwFlags,bContainsFirstTokenOfSentence);
					if(bReturnValueOfParseSubject==0)
					{
						dwEndOfSubjectWithPersonalNames=dwRun-1;
						subjectWithPersonalNames=subject;
					}
					if(bReturnValueOfParseSubject==1)
					{
						dwEndOfSubjectWithoutPersonalNames=dwRun-1;
						subjectWithoutPersonalNames=subject;
					}
					if(bReturnValueOfParseSubject==2)
					{
						dwEndOfSubjectWithRelativeClause=dwRun-1;
						subjectWithRelativeClause=subject;
					}
					if(!bValidMiddle)
						dwStartOfValidMiddle=dwRun;
					dwStartOfValidMiddle=dwRun;
					dwEndOfValidSentenceElement=dwRun;
					bValidMiddle=TRUE;
					if(predicate.m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated)
						if(typeid(*predicate.
							m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated->voc)==
							typeid(EnglishAuxiliaryVerb))
						{
								EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
									(predicate.
									m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated->voc);
#ifdef _DEBUG
								printf("typeid(*predicate.m_pVocNodeWhereAnEnglishInfiniti\
veIsEncapsulated->voc)==typeid(EnglishAuxiliaryVerb)\n");
#endif
								if(eav->words[0]==_T("be") && predicate.m_bActiveClause)
								{
#ifdef _DEBUG
									printf("eav->words[0]==_T(\"be\") && predicate.m_bActiveClause\n");
#endif
									predicateToBe=Predicate(predicate);
									bThisPredicateIsWithActiveToBe=TRUE;
									bPredicateWithActiveToBe=TRUE;
									dwEndOfValidMiddleWithActiveToBe=dwRun2;
								}
						}
					if(!bThisPredicateIsWithActiveToBe)
					{
#ifdef _DEBUG
						printf("!bPredicateWithActiveToBe\n");
#endif
						bPredicateRest=TRUE;
						dwEndOfValidMiddle=dwRun2;
						predicateRest=predicate;
					}
				}
				//if(predicate.m_ActiveClause)
				//if(ParseMiddle(subsentence,TRUE,GetBetweenAsCStringVector(psv,dwRun,
				//	dwRun2),0,dwFlags))
				//{
				//	if(!bValidMiddle)
				//		dwStartOfValidMiddle=dwRun;
				//	dwEndOfValidMiddle=dwRun2;
				//	dwStartOfValidMiddle=dwRun;
				//	dwEndOfValidSentenceElement=dwRun;
				//	bValidMiddle=TRUE;
				//}
			}
/*			if(bValidMiddle)
			{
#ifdef _DEBUG
				printf("bValidMiddle\n");
#endif
				if(bPredicateRest)
				{
#ifdef _DEBUG
					printf("bPredicateRest\n");
#endif
					subsentence.m_vecPredicate.push_back(predicateRest);
				}
				if(bPredicateWithActiveToBe)
				{
#ifdef _DEBUG
					printf("bPredicateWithActiveToBe\n");
#endif
					subsentence.m_vecPredicate.push_back(predicateToBe);
				}
				dwStartOfNextSentenceElement=dwEndOfValidMiddle+1;
				//break;
			}
		}*/
		if(bValidMiddle)
		{
#ifdef _DEBUG
			printf("bValidMiddle\n");
			fflush(stdout);
#endif
			if(bPredicateRest)
			{
#ifdef _DEBUG
				printf("bPredicateRest\n");
				printf("dwEndOfValidMiddle: %u\n",dwEndOfValidMiddle);
#endif
				if(dwEndOfValidMiddle==dwEndOfValidMiddleWithActiveToBe)
					subsentence.m_vecPredicate.push_back(predicateRest);
				else
				{
					if(dwEndOfValidMiddle>dwEndOfValidMiddleWithActiveToBe)
						subsentence.m_vecPredicate.push_back(predicateRest);
				}
			}
			if(bPredicateWithActiveToBe)
			{
#ifdef _DEBUG
				printf("bPredicateWithActiveToBe\n");
				printf("dwEndOfValidMiddleWithActiveToBe: %u\n",
					dwEndOfValidMiddleWithActiveToBe);
#endif
				if(dwEndOfValidMiddle==dwEndOfValidMiddleWithActiveToBe)
					subsentence.m_vecPredicate.push_back(predicateToBe);
				else
				{
					if(dwEndOfValidMiddleWithActiveToBe>dwEndOfValidMiddle)
						subsentence.m_vecPredicate.push_back(predicateToBe);
				}
			}
			//dwStartOfNextSentenceElement=dwEndOfValidMiddle+1;
			/*if(dwEndOfSubjectWithRelativeClause>0 && 
				dwEndOfSubjectWithRelativeClause>
				dwEndOfSubjectWithoutPersonalNames)
			{
				subsentence.m_pSubject=new Subject(subjectWithRelativeClause);
			}
			else
				subsentence.m_pSubject=new Subject(subjectWithoutPersonalNames);
			*/
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
			{
				rVecRange.push_back(vecRange.at(dwRun));
#ifdef _DEBUG
				printf("vecRange.at(%lu).m_dwStart: %lu vecRange.at(%lu).\
m_dwEnd: %lu\n",dwRun,vecRange.at(dwRun).m_dwStart,dwRun,vecRange.at(dwRun).m_dwEnd);
#endif
			}
#ifdef _DEBUG
			printf("subsentence.m_vecPredicate.size(): %u\n",subsentence.
				m_vecPredicate.size());
#endif
		}
	}
#ifdef _DEBUG
	printf("subsentence.m_vecPredicate.size(): %u\n",
		subsentence.m_vecPredicate.size());
#endif
	if(bValidMiddle) // wegen: dwStartOfValidMiddle-1
	{
		/*Subject subject;
		std::vector<Range> vecRange;
		if(!ParseSubject(subject,GetBetween(psv,dwEndOfElementBeforeMiddle,
			dwStartOfValidMiddle-1),FALSE,vecRange,dwFlags,bContainsFirstTokenOfSentence))
		{
#ifdef _DEBUG
			printf("BOOL ParseClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange) - return FALSE \
- ungültiges Subjekt\n");
#endif
			return FALSE;
		}
		subsentence.m_pSubject=new Subject(subject);
		bContainsFirstTokenOfSentence=FALSE;
		//if(vecRange.size())
		//	rVecRange.insert(rVecRange.end()-1,vecRange.begin(),vecRange.end()-1);
		for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
			rVecRange.push_back(vecRange.at(dwRun));
		//subsentence.m_Subject=subject;*/
	}
//	else
//	{
		//#ifdef _DEBUG
		//printf("BOOL ParseClause(Clause & subsentence,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange) - return FALSE\
// - !bValidMiddle\n");
//		#endif
//	}
	if(bValidMiddle || bValidImperativ) // gültiges 'Mittelteil' (Prädikat, usw)
	{
		BOOL bBe=FALSE;
		DWORD dwStartOfManner=0;
		GermanVerb * gv=NULL;
		Predicate predicate;
		#ifdef _DEBUG
		printf("bValidMiddle || bValidImperativ\n");
		if(bValidMiddle)
			printf("bValidMiddle\n");
		else
			printf("bValidImperativ\n");
		#endif
#ifdef _DEBUG
		printf("psv.size: %u dwStartOfNextSentenceElement: %u vor ParseManner\n",
			psv.size(),dwStartOfNextSentenceElement);
#endif
		predicate=subsentence.m_vecPredicate.at(0);
#ifdef _DEBUG
		printf("nach: predicate=subsentence.m_vecPredicate.at(0)\n");
#endif
		if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
		{
#ifdef _DEBUG
			printf("predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb\n");
#endif
			if(typeid(*predicate.
				m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord)==
				typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
					(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
					m_pWord);
				if(eav)
				{
#ifdef _DEBUG
					printf("eav->m_strWords[0]:%s\n",eav->m_strWords[0]);
#endif
					if(eav->m_strWords[0]==_T("be"))
						bBe=TRUE;
				}
			}
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
				m_pWordNodeNext)
				gv=dynamic_cast<GermanVerb*>(predicate.
					m_pWordNodeWhereTheGermanTranslationIsABendVerb->
					m_pWordNodeNext->m_pWord);
		}
		if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
		{
#ifdef _DEBUG
			printf("predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb\n");
#endif
			if(typeid(*predicate.
				m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord)==
				typeid(EnglishAuxiliaryVerb))
			{
#ifdef _DEBUG
			printf("predicate.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated->\
m_pWord==typeid(EnglishAuxiliaryVerb)\n");
#endif
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
					(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
					m_pWord);
				if(eav)
				{
#ifdef _DEBUG
					printf("eav->m_strWords[0]:%s\n",eav->m_strWords[0]);
#endif
					if(eav->m_strWords[0]==_T("be"))
					{
						bBe=TRUE;
					}
				}
			}
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
				m_pWordNodeNext)
				gv=dynamic_cast<GermanVerb*>(predicate.
				m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
				m_pWordNodeNext->m_pWord);
		}
		if(gv)
			if(gv->m_bCase==1 || gv->m_bCase==2 || gv->m_bCase==3 || gv->m_bCase==4)
				bObjectVerb=TRUE;
		if(dwStartOfNextSentenceElement<psv.size() || (bPredicateWithActiveToBe 
			&& bPredicateRest))
		{
			//if(
			//BOOL bBe=FALSE;
			DWORD dwEndOfValidManner=0;
			CString strBe("be") ;
			EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strBe);
		#ifdef _DEBUG
			printf("dwStartOfNextSentenceElement<psv.size() || \
(bPredicateWithActiveToBe && bPredicateRest)\n");
		#endif
			/*if(subsentence.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated)
				if(typeid(*subsentence.
					m_pVocNodeWhereAnEnglishPredicateIsEncapsulated->voc)==
					typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
						(subsentence.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated->voc);
					if(eav)
						if(eav->words[0]==_T("be"))
							bBe=TRUE;
				}*/
			//wenn das Verb kein Objekt verlangt
			if(!bObjectVerb)// || bPredicateWithActiveToBe)  
			{
		#ifdef _DEBUG
				printf("!bObjectVerb || bPredicateWithActiveToBe\n");
		#endif
				//DWORD dwRun=0;
				if(bPredicateWithActiveToBe)
					dwStartOfNextSentenceElement=dwEndOfValidMiddleWithActiveToBe+1;
				else
					dwStartOfNextSentenceElement=dwEndOfValidMiddle+1;
				for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && 
				//for(;dwRun<psv.size() && 
					g_bContinue;dwRun++)
				{
					Manner manner;
					if(ParseManner(manner,GetBetween(psv,
						dwStartOfNextSentenceElement,dwRun),bPredicateWithActiveToBe,dwFlags))
					{
						TRACE("nach: if(ParseManner(manner,GetBetween(psv,dwStartOfNextSentenceElement,dwRun),TRUE))\n");
						dwEndOfValidManner=dwRun;
						bValidManner=TRUE;
						//bValidObject=TRUE;
						if(subsentence.m_pManner)
						{
							TRACE("subsentence.m_pManner\n");
							delete subsentence.m_pManner;
						}
						subsentence.m_pManner=new Manner;
						*subsentence.m_pManner=manner;
						dwStartOfNextSentenceElement=dwRun+1;
						dwStartOfManner=dwRun;
						TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
						dwStartOfFirstSentenceElementAfterMiddle=dwStartOfNextSentenceElement;
					}
				}
				if(bValidManner)
					dwStartOfNextSentenceElement=dwEndOfValidManner+1;
			}
			//wenn das Verb ein Objekt verlangt
			else
			{
        //Strategie: zuerst alle Satz-Elemente, die zwischen einem Prädikat 
        //und einem Objekt befinden können, parsen. Denn falls das Objekt ein
        //Eigenname ist, weiß man dann den Beginn des Eigennamens.
#ifdef _DEBUG
				printf("bObjectVerb\n");
#endif
				BOOL bBreak=FALSE;
				//if(bPredicateWithActiveToBe)
				//	dwStartOfNextSentenceElement=dwEndOfValidMiddleWithActiveToBe+1;
				//else
				//	dwStartOfNextSentenceElement=dwEndOfValidMiddle+1;
        
        //Auch Verben, die ein Objekt verlangen, können eine Art und Weise beinhalten:
        //He hit her completely wrong." Art und Weise: completely wrong
				for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && 
					g_bContinue;dwRun++)
				{
					for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
					{
						//if(subsentence.predicate==eav->words[1] || subsentence.predicate==eav->words[2] ||  // am, are, is
						//	subsentence.predicate==eav->words[3] || subsentence.predicate==eav->words[7] || subsentence.predicate==eav->words[8]) // was, were
						//	bBe=TRUE;
						Manner manner;
						if(ParseManner(manner,GetBetween(psv,dwRun,dwRun2),
							bBe,dwFlags))
						{
							TRACE("nach: if(ParseManner(manner,GetBetween(psv,dwStartOfNextSentenceElement,dwRun),TRUE))\n");
							dwEndOfValidManner=dwRun2;
							bValidManner=TRUE;
							//bValidObject=TRUE;
							if(subsentence.m_pManner)
							{
								TRACE("subsentence.m_pManner\n");
								delete subsentence.m_pManner;
							}
							subsentence.m_pManner=new Manner;
							*subsentence.m_pManner=manner;
							//dwStartOfNextSentenceElement=dwRun2+1;
							dwEndOfObject=dwRun-1;
							TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
							dwStartOfNextSentenceElement=dwRun2+1;
							dwStartOfFirstSentenceElementAfterMiddle=dwRun;
						}
					}
					if(bValidManner) //if(bBreak)
						if(gv->m_bCase==4 && dwEndOfValidMiddle==dwEndOfObject && 
							dwStartOfNextSentenceElement==psv.size())
						{
#ifdef _DEBUG
							printf("ParseClause - return returnValue (%u)\n",returnValue);
#endif
							return returnValue;
						}
						else
							break;
				}
				if(bValidManner)
				{
					dwStartOfNextSentenceElement=dwEndOfValidManner+1;
					//if(dwEndOfValidMiddle+1==dwEndOfValidManner
				}
			}
		}
		if(bPredicateWithActiveToBe && bValidManner)
			dwEndOfValidMiddle=dwEndOfValidMiddleWithActiveToBe;
		// wenn Prädikat vom Verb 'to be' und die Umstandsbestimmumg
		// der Art und Weise nicht direkt nach dem Mittelteil (Prädikat und so 
		// weiter) beginnt, dann die Umstandsbestimmung der Art und Weise
		// ungültig machen, weil sich nach dem Mittelteil noch ein Objekt
		// befinden kann, das Teile der Umstandsbestimmung der Art und Weise
		// enthält . zum Beispiel: "I am a good artist."
		// "good" wird zunächst als Umstandsbestimmung der Art und Weise
		// erkannt. "good" liegt aber nicht direkt hinter dem Mittelteil
		// (Prädikat und so weiter); das Objekt ist: "a good artist"		
		if(bValidManner && gv->m_strWords[0]==_T("sein") && bObjectVerb &&
			dwEndOfObject!=dwEndOfValidMiddle)
		{
#ifdef _DEBUG
			printf("bValidManner && gv->m_strWords[0]==_T(\"sein\") && \
dwEndOfObject!=dwEndOfValidMiddle)\n");
			printf("dwEndOfObject: %u dwEndOfValidMiddle: %u\n",dwEndOfObject,
				dwEndOfValidMiddle);
#endif
			bValidManner=FALSE;
			delete subsentence.m_pManner;
			subsentence.m_pManner=NULL;
			dwEndOfObject=0;
		}
		if(dwStartOfNextSentenceElement<psv.size() && 
      //Only when this is a PASSIVE clause a by-agent after the predicate
      //is valid.
      !subsentence.m_bActiveClause)
		{
			if(bValidManner || !bObjectVerb)
			{
				DWORD dwEndOfValidByAgent=0;
				for(DWORD dwRun=dwEndOfValidSentenceElement+1;dwRun<psv.size() && 
					g_bContinue;dwRun++)
				{
					ByAgent byAgent;
					if(ParseByAgent(byAgent,GetBetween(psv,dwStartOfNextSentenceElement,
						dwRun),dwFlags))
					{
						//dwEndOfValidObject=dwRun;
						TRACE("nach: if(ParseManner(manner,GetBetween(psv,dwStartOfNextSentenceElement,dwRun),TRUE))\n");
						dwEndOfValidByAgent=dwRun;
						bValidByAgent=TRUE;
						//bValidObject=TRUE;
						if(subsentence.m_pByAgent)
						{
							TRACE("subsentence.m_pByAgent\n");
							delete subsentence.m_pByAgent;
						}
						subsentence.m_pByAgent=new ByAgent;
						TRACE("nach: if(subsentence.m_pByAgent)\n");
						//TRACE("manner.m_vecMannerEnumerationElements.size(): %u\n",manner.m_vecMannerEnumerationElements.size());
						*subsentence.m_pByAgent=byAgent;
						//TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
						if(!dwStartOfFirstSentenceElementAfterMiddle)
							dwStartOfFirstSentenceElementAfterMiddle=
							dwStartOfNextSentenceElement;
					}
					if(bValidByAgent)
						dwStartOfNextSentenceElement=dwEndOfValidByAgent+1;
				}//if(ParseByAgent(...)
			}//if(bValidManner || !bObjectVerb)
			else
			{
        if(bObjectVerb
          ////Only when this is a PASSIVE clause a by-agent after the predicate
          ////is valid.
          //&& !subsentence.m_bActiveClause
          )
				{
					DWORD dwEndOfValidByAgent=0;
					for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && 
						g_bContinue;dwRun++)
					{
						for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
						{
							ByAgent byAgent;
							if(ParseByAgent(byAgent,GetBetween(psv,dwRun,dwRun2),dwFlags))
							{
								//dwEndOfValidObject=dwRun;
								TRACE("nach: if(ParseManner(manner,GetBetween(psv,dwRun,dwRun2),TRUE))\n");
								dwEndOfValidByAgent=dwRun2;
								bValidByAgent=TRUE;
								dwEndOfObject=dwRun-1;
								//bValidObject=TRUE;
								if(subsentence.m_pByAgent)
								{
									TRACE("subsentence.m_pByAgent\n");
									delete subsentence.m_pByAgent;
								}
								subsentence.m_pByAgent=new ByAgent;
								TRACE("nach: if(subsentence.m_pByAgent)\n");
								//TRACE("manner.m_vecMannerEnumerationElements.size(): %u\n",manner.m_vecMannerEnumerationElements.size());
								*subsentence.m_pByAgent=byAgent;
								if(!dwStartOfFirstSentenceElementAfterMiddle)
									dwStartOfFirstSentenceElementAfterMiddle=dwRun;
								//TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
							}//if(ParseByAgent(...)
						}
					}
					if(bValidByAgent)
						dwStartOfNextSentenceElement=dwEndOfValidByAgent+1;
				}
			}//else-branch of "if(bValidManner || !bObjectVerb)"; ParseByAgent(...)
		}//if(dwStartOfNextSentenceElement<psv.size())
#ifdef _DEBUG
		printf("psv.size: %u dwStartOfNextSentenceElement: %u\n",psv.size(),
			dwStartOfNextSentenceElement);
#endif
		std::vector<Range> vecRangeOfPlace;
		if(dwStartOfNextSentenceElement<psv.size())
		{
			if(bValidManner || bValidByAgent || !bObjectVerb)
			{
				DWORD dwEndOfValidPlace=0;
				for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && 
					g_bContinue;dwRun++)
				{
					Place place; // Umstandsbestimmung des Ortes
					std::vector<Range> vecRange;
					if(ParsePlace(place,GetBetween(psv,dwStartOfNextSentenceElement,
						dwRun),vecRange,dwFlags))
					{
						if(subsentence.m_pPlace)
							delete subsentence.m_pPlace;
						subsentence.m_pPlace=new Place;
						*subsentence.m_pPlace=place;
						bValidPlace=TRUE;
						dwEndOfValidPlace=dwRun;
						vecRangeOfPlace=vecRange;
						if(!dwStartOfFirstSentenceElementAfterMiddle)
							dwStartOfFirstSentenceElementAfterMiddle=
								dwStartOfNextSentenceElement;
					}
				}
				if(bValidPlace)
					dwStartOfNextSentenceElement=dwEndOfValidPlace+1;
			}
			else
			{
				if(bObjectVerb)
				{
					DWORD dwEndOfValidPlace=0;
					for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && g_bContinue;dwRun++)
					{
						for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
						{
							Place place; // Umstandsbestimmung des Ortes
							std::vector<Range> vecRange;
							if(ParsePlace(place,GetBetween(psv,dwRun,dwRun2),vecRange,dwFlags))
							{
								if(subsentence.m_pPlace)
									delete subsentence.m_pPlace;
								subsentence.m_pPlace=new Place;
								*subsentence.m_pPlace=place;
								bValidPlace=TRUE;
								dwEndOfValidPlace=dwRun2;
								dwEndOfObject=dwRun-1;
								vecRangeOfPlace=vecRange;
								if(!dwStartOfFirstSentenceElementAfterMiddle)
									dwStartOfFirstSentenceElementAfterMiddle=dwRun;
							}
						}
					}
					if(bValidPlace)
						dwStartOfNextSentenceElement=dwEndOfValidPlace+1;
				}
			}
		}
		if(bValidPlace)
		{
			for(DWORD dwRun=0;dwRun<vecRangeOfPlace.size();dwRun++)
			{
				rVecRange.push_back(vecRangeOfPlace.at(dwRun));
			}
		}
#ifdef _DEBUG
		printf("psv.size: %u dwStartOfNextSentenceElement: %u\n",psv.size(),
			dwStartOfNextSentenceElement);
		printf("vor der Prüfung auf eine Umstandsbestimmung der Zeit am Ende des\
 \"Untersatzes\"\n");
#endif
    LOG("19.05.2008 20.42\n");
		if(dwStartOfNextSentenceElement<psv.size())
		{
			if(!bObjectVerb || bValidManner || bValidByAgent || bValidPlace)
			{
				CString str=psv.at(dwStartOfNextSentenceElement).m_Str;
#ifdef _DEBUG
				printf("!bObjectVerb || bValidManner || bValidByAgent || bValidPlace\n");
#endif
				if(!(dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
					str.MakeLower();
				if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(str)) )
				{
					#ifdef _DEBUG
					printf("Adverb\n");
					#endif
					EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
					if(ea)
						if(ea->m_bType==5) // Adverb der Zeit ('today')
						{
							dwStartOfNextSentenceElement++;
							subsentence.m_bAdverbOfTimeAtTheBeginning=FALSE;
							#ifdef _DEBUG
							printf("Adverb der Zeitt\n");
							printf("psv.size: %u dwStartOfNextSentenceElement: %u\n",
								psv.size(),dwStartOfNextSentenceElement);
							#endif
							subsentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=
								pWordNode;
							bValidTime=TRUE;
							if(!dwStartOfFirstSentenceElementAfterMiddle)
								dwStartOfFirstSentenceElementAfterMiddle=
									dwStartOfNextSentenceElement;
						}
				}
			}
			else
			{
#ifdef _DEBUG
				printf("!(!bObjectVerb || bValidManner || bValidByAgent || bValidPlace)\n");
#endif
        //If a transitive verb, first parse for structures that may be RIGHT of an object
        //because an object may be a proper name.
				if(bObjectVerb)
				{
#ifdef _DEBUG
					printf("bObjectVerb\n");
#endif
					for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() 
						&& g_bContinue;dwRun++)
					{
						CString str=psv.at(dwRun).m_Str;
#ifdef _DEBUG
						printf("str: %s\n",str);
#endif
						if(!(dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
							str.MakeLower();
						if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(str)) )
						{
							#ifdef _DEBUG
							printf("Adverb\n");
							#endif
							EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
							if(ea)
								if(ea->m_bType==ADVERB_OF_TIME) // Adverb der Zeit ('today')
								{
									dwStartOfNextSentenceElement=dwRun+1;
									subsentence.m_bAdverbOfTimeAtTheBeginning=FALSE;
									dwEndOfObject=dwRun-1;
									bValidTime=TRUE;
									#ifdef _DEBUG
									printf("Adverb der Zeit: %s\n",ea->m_strWord);
									printf("psv.size: %u dwStartOfNextSentenceElement: %u\n",psv.size(),dwStartOfNextSentenceElement);
									#endif
									subsentence.
										m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=
										pWordNode;
									if(!dwStartOfFirstSentenceElementAfterMiddle)
										dwStartOfFirstSentenceElementAfterMiddle=dwRun;
									break; // wichtig, sonst wird dwStartOfNextSentenceElement mehrmals
									// inkrementiert
								}
						}
					}//for-loop
				}
			}
		}
#ifdef _DEBUG
		printf("bObjectVerb: %d bValidManner: %d gv->words[0]: %s \
dwStartOfNextSentenceElement: %u psv.size(): %u\n",bObjectVerb,
bValidManner,gv->m_strWords[0],dwStartOfNextSentenceElement,psv.size());
#endif
   LOG("19.05.2008 20.42.30\n");
		//if((bObjectVerb || (!bValidManner && gv->words[0]==_T("sein"))) && 
		//	dwStartOfNextSentenceElement<psv.size())
		//wenn das Verb ein Objekt ermöglicht und nicht das Hilfsverb "sein" 
		//ist oder es keine Umstandsbestimmung der Art und Weise gibt und
		//das Verb das Hilfsverb "sein" ist
		if((bObjectVerb && gv->m_strWords[0]!=_T("sein")) || (!bValidManner && 
			gv->m_strWords[0]==_T("sein")) && dwEndOfValidMiddle+1!=
			dwStartOfFirstSentenceElementAfterMiddle)
		{ // nur wenn das Prädikat ein Objekt verlangt, wenn
			// das Prädikät vom Verb "sein" ist, dann darf nicht bereits
			// eine Umstandsbestimmung der Art und Weise (!bValidManner, Manner=
			// deutsch: "Art und Weise") existieren,.
			//und dem Ende des "Mittelsteils" nicht direkt ein Satzglied folgt
			//zum Beispiel: "The number is incomplete."
			//                          ++ +--------+
			//													|			 |
			//                     Prädikat  Umstandsbestimmung der Art und Weise
			//               ("Mittelteil")  (Satzglied)
			//in diesem Beispiel folgt dem "Mittelteil" direkt ein Satzglied, es
			//wird also nicht die Gültigkeit (grammatikalische Überprüfung) eines 
			//Objektes durchgeführt
#ifdef _DEBUG
			printf("(bObjectVerb || (!bValidManner && gv->words[0]==_T(\"sein\"))) && \
dwStartOfNextSentenceElement<psv.size()\n");
#endif
			if(!dwEndOfObject)
				dwEndOfObject=psv.size()-1;
			#ifdef _DEBUG
			printf("dwEndOfValidMiddle+1: %u psv.size(): %u dwEndOfObject:  %u \
subsentence.m_vecPredicate.size(): %u\n",dwEndOfValidMiddle+1,psv.size(),
				dwEndOfObject,subsentence.m_vecPredicate.size());
			#endif
			if(dwEndOfValidMiddle+1<psv.size() && dwEndOfObject>=0 && 
				subsentence.m_vecPredicate.size()>0 && 
				(dwEndOfValidMiddle+1<=dwEndOfObject))
			{
				#ifdef _DEBUG
				printf("dwStartOfNextSentenceElement<sv.size() && \
(subsentence.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated\n || \
subsentence.m_pVocNodeWhereAnEnglishVerbIsEncapsulated)\n");
				#endif
				if(gv)
				{
					#ifdef _DEBUG
					printf("gv gv->m_strWords[0]: %s gv->Case: %u\n",gv->m_strWords[0],gv->m_bCase);
					if(subsentence.m_bActiveClause)
						printf("subsentence.m_bActiveClause\n");
					else
						printf("!subsentence.m_bActiveClause\n");
					#endif
					BOOL bValidObject=FALSE;
          LOG("23.05.2008 11.15.43\n");//AC=Active Clause
					if(subsentence.m_bActiveClause)
					{
						if(gv->m_bCase==1 || gv->m_bCase==2 || gv->m_bCase==4)
						{ // wenn das Prädikat den 1. , 3. oder 4. Fall verlangt
							// gv->Case==1  : 3. Fall
							// gv->Case==2  : 4. Fall
							// gv->Case==4  : 1. Fall
							BYTE bReturnOfParseObject=0;
							DWORD dwFlagsParameter=dwFlags;
							#ifdef _DEBUG
							printf("gv->m_bCase==1 || gv->m_bCase==2 || gv->m_bCase==4\n");
							printf("returnValue: %u\n",returnValue);
							#endif
							std::vector<Range> vecRange;
							Object object;
							if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
								//31tes Bit setzen
								dwFlagsParameter|=1073741824;
							if((bReturnOfParseObject=ParseObject(object,GetBetween(psv,
								dwEndOfValidMiddle+1,dwEndOfObject),TRUE,vecRange,
								dwFlagsParameter))==0)
							{
							#ifdef _DEBUG
								printf("BOOL ParseClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange) - return 0\n");
							#endif
								return 0;
							}
							if(bReturnOfParseObject<returnValue)
								returnValue=bReturnOfParseObject;
							if(subsentence.m_pObjectFirst)
								delete subsentence.m_pObjectFirst;
							subsentence.m_pObjectFirst=new Object(object);
							for(dwRun=0;dwRun<vecRange.size();dwRun++)
								rVecRange.push_back(vecRange.at(dwRun));
							if(!bValidManner && !bValidByAgent && !bValidPlace && !bValidTime)
								dwStartOfNextSentenceElement=dwEndOfObject+1;
						}
            LOG("19.05.2008 20.45\n");
						if(gv->m_bCase==DATIVE_AND_ACCUSATIVE)
						{ // wenn das Prädikat 2 Fälle (3. und 4. Fall) verlangt
							std::vector<Range> vecRange;
							std::vector<Range> vecRangeOfValidObject;
							DWORD dwEndOfValidObject=0;
							for(DWORD dwRun=dwEndOfValidMiddle+1;dwRun<dwEndOfObject+1 && 
								g_bContinue;dwRun++)
							{
								DWORD dwFlagsParameter=dwFlags;
								vecRange.clear();
								Object object;
								if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
									//31tes Bit setzen
									dwFlagsParameter|=1073741824;
								if(ParseObject(object,GetBetween(psv,dwEndOfValidMiddle+1,
									dwRun),TRUE,vecRange,dwFlagsParameter)==2)
								{
									dwEndOfValidObject=dwRun;
									//dwEndOfValidSentenceElement=dwRun;
									bValidObject=TRUE;
									if(subsentence.m_pObjectFirst)
										delete subsentence.m_pObjectFirst;
									subsentence.m_pObjectFirst=new Object(object);
									vecRangeOfValidObject.clear();
									vecRangeOfValidObject=vecRange;
								}
							}
							if(bValidObject)
							{
								dwStartOfNextSentenceElement=dwEndOfValidObject+1;
								dwEndOfValidSentenceElement=dwEndOfValidObject;
								for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
									rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
							}
						}
					}//if active clause
          LOG("23.05.2008 11.15.55\n");//AAC=After Active Clause
					if(subsentence.m_bActiveClause && gv->m_bCase==3)
					{ // wenn ein Verb, das 2 Fälle verlangt
						#ifdef _DEBUG
						printf("gv->m_bCase==3\n");
						#endif
						DWORD dwEndOfValidObject=0;
						if(bValidObject)
						{
							DWORD dwFlagsParameter=dwFlags;
							std::vector<Range> vecRange;
							Object object;
							if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
								//31tes Bit setzen
								dwFlagsParameter|=1073741824;
							ParseObject(object,GetBetween(psv,dwStartOfNextSentenceElement,
								dwEndOfObject),TRUE,vecRange,dwFlagsParameter);
							bValidObject=TRUE;
							if(subsentence.m_pObjectSecond)
								delete subsentence.m_pObjectSecond;
							subsentence.m_pObjectSecond=new Object(object);
							for(dwRun=0;dwRun<vecRange.size();dwRun++)
								rVecRange.push_back(vecRange.at(dwRun));
							if(!bValidManner && !bValidByAgent && !bValidPlace && !bValidTime)
								dwStartOfNextSentenceElement=dwEndOfObject+1;
						}
						else
						{
							//bValidObject=FALSE;
							std::vector<Range> vecRange;
							std::vector<Range> vecRangeOfValidObject;
							DWORD dwStartOfValidObject=0;
							for(DWORD dwRun=dwEndOfObject;dwRun>dwEndOfValidMiddle && g_bContinue;dwRun--)
							{
								DWORD dwFlagsParameter=dwFlags;
								vecRange.clear();
								Object object;
								if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
									//31tes Bit setzen
									dwFlagsParameter|=1073741824;
								if(ParseObject(object,GetBetween(psv,dwRun,dwEndOfObject),TRUE,
									vecRange,dwFlagsParameter)==2)
								{
									dwStartOfValidObject=dwRun;
									//dwEndOfValidSentenceElement=dwRun;
									bValidObject=TRUE;
									if(subsentence.m_pObjectSecond)
										delete subsentence.m_pObjectSecond;
									subsentence.m_pObjectSecond=new Object;
									*subsentence.m_pObjectSecond=object;
									//subsentence.m_pObject=object;
									vecRangeOfValidObject.clear();
									vecRangeOfValidObject=vecRange;
								}
							}
							for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
								rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
							Object object;
							if(bValidObject)
							{
								DWORD dwFlagsParameter=dwFlags;
								vecRange.clear();
								if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
									//31tes Bit setzen
									dwFlagsParameter|=1073741824;
								ParseObject(object,GetBetween(psv,dwStartOfNextSentenceElement,
									dwStartOfValidObject-1),TRUE,vecRange,dwFlagsParameter);
								if(subsentence.m_pObjectFirst)
									delete subsentence.m_pObjectFirst;
								subsentence.m_pObjectFirst=new Object(object);
								for(dwRun=0;dwRun<vecRange.size();dwRun++)
									rVecRange.push_back(vecRange.at(dwRun));
								if(!bValidManner && !bValidByAgent && !bValidPlace && !bValidTime)
									dwStartOfNextSentenceElement=dwEndOfObject+1;
							}
						}
					}//if active clause and 3rd Case in German.
          LOG("23.05.2008 11.16.00\n");//AAC3=After Active Clause and 3rd case
					//Passivsatz und 3.? Fall
					if(!subsentence.m_bActiveClause && gv->m_bCase==3)
					{
						#ifdef _DEBUG
						printf("!subsentence.m_bActiveClause && gv->m_bCase==3\n");
						#endif
						Subject subject;
						std::vector<Range> vecRange;
						ParseSubject(subject,GetBetween(psv,dwEndOfValidMiddle+1,
							dwEndOfObject),FALSE,vecRange,dwFlags,
							bContainsFirstTokenOfSentence);
						//dwEndOfValidSentenceElement=dwRun;
						if(subsentence.m_pPersonalSubject)
							delete subsentence.m_pPersonalSubject;
						subsentence.m_pPersonalSubject=new Subject;
						*subsentence.m_pPersonalSubject=subject;
						for(dwRun=0;dwRun<vecRange.size();dwRun++)
							rVecRange.push_back(vecRange.at(dwRun));
						if(!bValidManner && !bValidByAgent && !bValidPlace && !bValidTime)
							dwStartOfNextSentenceElement=dwEndOfObject+1;
					}
          LOG("23.05.2008 11.6.07\n");//ANAC3=After Not Active Clause and 3rd case
				}
				#ifdef _DEBUG
				else
				{
					printf("\n\n\nelse-zweig\n");
					if(!subsentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
						m_pWordNodeNext)
						printf("!subsentence.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated->\
m_pWordNodeNext\n");
					if(!subsentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext)
						printf("!subsentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->\
m_pWordNodeNext\n");
				}
				#endif
			}//If at least 1 token remaining,object,>=1 predicate
			else
			{
				if(subsentence.m_vecPredicate.size()==1)
				{
					Predicate predicate=subsentence.m_vecPredicate.at(0);
					if(predicate.m_bActiveClause)
					{
						returnValue=0;
#ifdef _DEBUG
						printf("bObjectVerb und kein Objekt!\n");
#endif
					}
				}
			}
		}//Objektverb,<>"sein" v ...
    LOG("23.05.2008 11.16.15\n");//AOV=After ObjectVerb
		if(dwStartOfNextSentenceElement==psv.size())
		{
			#ifdef _DEBUG
			printf("rVecRange.size(): %u\n",rVecRange.size());
			printf("dwStartOfNextSentenceElement==psv.size()\n");
			printf("BOOL ParseClause(Clause & subsentence,const \
PositionCStringVector & sv) - return returnValue (%u)\n",returnValue);
			#endif
      LOG("23.05.2008 11.16.24\n");//NMT=no more token
			return returnValue;
		}
		#ifdef _DEBUG
		else
		{
			printf("dwStartOfNextSentenceElement!=psv.size()\n");
		}
		#endif
	}//if(bValidMiddle || bValidImperativ)
	TRACE("ParseClause ENDE - return FALSE\n");
	#ifdef _DEBUG
	printf("rVecRange.size(): %u\n",rVecRange.size());
	printf("BYTE ParseClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
dwFlags,BYTE bContainsFirstTokenOfSentence - return FALSE\n");
	#endif
  LOG("23.05.2008 11.16.31\n");//PCE=ParseClause End
	return FALSE;
}

//@return 
// 2, wenn sich mindestens ein vermeintlicher Eigenname 
//  im Haupt- oder Nebensatz befindet, 
// 1 wenn sich keine Eigenname im Haupt- 
//  oder Nebensatz befindet, 0 wenn der Haupt- oder Nebensatz grammatikalisch
//inkorrekt ist
//mit "clause" ist ein Haupt- oder Nebensatz (mit Nebensatz ist 
//NICHT Relativsatz gemeint) /nicht-zusammengesetzer Satz gemeint,
//also NICHT mehrere Haupt- oder Nebensätze hintereinander
BYTE 
  //Soll ohne Subjekt und Objekt parsen, damit diese Funktion für Relativ-Sätze
  //(The man you liked yesterday: Rel-Satz: "you liked yesterday" und für Frage-Sätze, die nach einem
  //Objekt fragen(Who likes you on the table?), benutzt werden kann.
  Clause::ParseClauseWithoutSubjectAndObject(
  Clause & subsentence,
	const PositionCStringVector & psv,std::vector<Range> & rVecRange,
	const DWORD & dwFlags//,BYTE bContainsFirstTokenOfSentence
  )
{
	Subject subject;
  bool bContainsFirstTokenOfSentence = 
    (dwFlags & CONTAINS_FIRST_TOKEN_OF_SENTENCE) //!= 0
    ;
	BOOL bConjunction=FALSE;
	BOOL bMiddle=FALSE;
	BOOL bObject=FALSE;
	BOOL bObjectVerb=FALSE;
	BOOL bParsedSubject=FALSE;
	BOOL bParsedMiddle=FALSE;
	BOOL bPredicateWithActiveToBe=FALSE;
	BOOL bPredicateRest=FALSE;
	BOOL bVerb=FALSE;
	BOOL bValidByAgent=FALSE;
	BOOL bValidManner=FALSE;
	BOOL bValidMiddle=FALSE;
	BOOL bValidPlace=FALSE;
	BOOL bValidTime=FALSE;
	BOOL bValidSubject=FALSE;
	BOOL bValidObject=FALSE;
	BYTE returnValue=2;
	BYTE personIndex=0;
	BYTE bValidImperativ=FALSE;
	BYTE bObjectVerbPossible=TRUE;
	DWORD dwEndOfConjunction=0;
	DWORD dwEndOfObject=0;
	DWORD dwEndOfValidSentenceElement=0; // Ende eines gültigen Satzgliedes
	DWORD dwStartOfNextSentenceElement=0;
	DWORD dwEndOfValidManner=0;
	DWORD dwEndOfValidMiddle=0;
	DWORD dwEndOfValidMiddleWithActiveToBe=0;
	DWORD dwStartOfFirstSentenceElementAfterMiddle=0;
	DWORD dwStartOfValidMiddle=0;
	DWORD dwEndOfElementBeforeMiddle=dwStartOfNextSentenceElement;
	DWORD dwStart=0;
	DWORD dwEnd=0;
	DWORD dwEndOfValidSubject=0;
	DWORD dwEndOfValidObject=0;
	Gerund gerund;
	int start=0;
	int end=0;
	std::vector<BYTE> vecByte;
	std::vector<Range> vecRange;
	WordNode * pWordNode=NULL;
	//SubSentence * subsentence=new SubSentence();
#ifdef _DEBUG
	printf("BYTE ParseClause(Clause & subsentence,\
const PositionCStringVector & psv,std::vector<Range> & rVecRange,\
const DWORD & dwFlags,BYTE bContainsFirstTokenOfSentence(%u)) ANFANG\n",
	bContainsFirstTokenOfSentence);
	printf("g_bContinue: %u\n",g_bContinue);
	printf("subsentence.m_vecPredicate.size(): %u\n",
		subsentence.m_vecPredicate.size());
#endif
	subsentence.m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.clear();
	#ifdef _DEBUG
	for(DWORD i2=0;i2<psv.size();i2++)
	{
		printf("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
	}
	#endif
	subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated=NULL;
	//es können auch mehrere Konjunktionen hintereinander stehen Beispiel: 
	//"But if you are intelligent." (="Aber falls du intelligent bist)
	//Konjunktionen: "But", "if"
	for(DWORD dwRun=0;dwRun<psv.size() && g_bContinue;dwRun++)
	{
    LOG("23.05.2008 11.14.41 "<<dwRun<<"\n");
		BOOL bValidConjunction=FALSE;
		for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
		{
#ifdef _DEBUG
			printf("dwRun2: %u\n",dwRun2);
#endif
			CStringVector sv=GetBetweenAsCStringVector(psv,dwRun,dwRun2);
			if(!(dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)//wenn die Groß- und Kleinschreibung nicht relevant ist
				for(DWORD dwRun3=0;dwRun3<sv.size();dwRun3++)
					sv.at(dwRun3).MakeLower();
			else if(bContainsFirstTokenOfSentence)
				MakeFirstLetterLower(sv.at(0));
			if((pWordNode=GetPointerToWordNodeWhereAnEnglishConjunctionIsEncapsulated(sv)))
			{
				subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated=
					pWordNode;
				subsentence.m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.
					push_back(pWordNode);
				dwEndOfConjunction=dwRun2;
				bConjunction=TRUE;
				#ifdef _DEBUG
				printf("GetPointerToWordNodeWhereAnEnglishConjunctionIsEncapsula\
ted(GetBetween(psv,0,dwRun))==TRUE\n");
				#endif
				bValidConjunction=TRUE;
			}
		#ifdef _DEBUG
			else
			{
				printf("GetPointerToWordNodeWhereAnEnglishConjunctionIsEncapsula\
ted(GetBetween(psv,0,dwRun))==FALSE\n");
				if(pWordNode==NULL)
					printf("pWordNode==NULL\n");
			}
		#endif
		}
		if(bValidConjunction)
		{
			dwRun=dwEndOfConjunction;//dwRun2+1;
			dwStartOfNextSentenceElement=dwEndOfConjunction+1;
			dwEndOfValidSentenceElement=dwEndOfConjunction;
			bContainsFirstTokenOfSentence=FALSE;
		}
		else // die Konjunktionen dürfen nur am Satzbeginn stehen
			break;
	}
	#ifdef _DEBUG
	if(subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated)
		printf("subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated\n");
	else
		printf("!subsentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated\n");
	#endif
  LOG("19.05.2008 20.44\n");
//	//ein Gerundium kann das Subjekt eines Satzes sein.
//	//Beispiel: "Leaving your home country is a big step." 
//	//           --------Gerundium-------- -P -----O----  P:Prädikat O:Objekt
//	//-> "Deine Heimat zu verlassen ist ein großer Schritt."
//	if(ParseGerund(gerund,TRUE,FALSE,psv,vecRange,dwFlags,
//		bContainsFirstTokenOfSentence))
//	{
//		if(subsentence.m_pGerund)
//			delete subsentence.m_pGerund;
//		subsentence.m_pGerund=new Gerund(gerund);
//		#ifdef _DEBUG
//		printf("rVecRange.size(): %u\n",rVecRange.size());
//		printf("BOOL ParseClause(Clause & subsentence,const \
//PositionCStringVector & psv) - ENDE - return TRUE\n");
//		#endif
//		return TRUE;
//	}
	if(subsentence.m_pGerund)
		bContainsFirstTokenOfSentence=FALSE;
	//Ein "Teilsatz" kann am Anfang ein Adverb (Umstandsbestimmung) der 
	//Zeit beinhalten.
	//Beispiel: "Now you are great." -> "Jetzt bist du grossartig."
	// Adverb der Zeit: "Now"
	if(psv.size()>dwStartOfNextSentenceElement)
	{
		CString str=psv.at(dwStartOfNextSentenceElement).m_Str;
		if(bContainsFirstTokenOfSentence)
			MakeFirstLetterLower(str);
		if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
			str)) )
		{
			#ifdef _DEBUG
			printf("Adverb\n");
			#endif
			EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
			if(ea)
				if(ea->m_bType==5) // Adverb der Zeit ('today')
				{
					#ifdef _DEBUG
					printf("Adverb der Zeit\n");
					#endif
					subsentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=
						pWordNode;
					//subsentence.m_bWordOrder=PSO;
					dwStartOfNextSentenceElement++;
					bContainsFirstTokenOfSentence=FALSE;
				}
		}
	}
	//if(psv.size()>dwStartOfNextSentenceElement)
	//{
	//	//if(!bValidMiddle)
	//	CString str=psv.at(dwStartOfNextSentenceElement).m_Str;
	//	if(bContainsFirstTokenOfSentence)
	//		MakeFirstLetterLower(str);
	//		//wenn Imperativ
	//		if((pWordNode=GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(
	//			str)))
	//		{
	//			Predicate predicate;
	//			subsentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
	//			subsentence.m_bTense=1;
	//			bValidImperativ=TRUE;
	//			//predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb=
	//			//	pWordNode;
 //       //Im Englischen ist der Imperativ zwar identisch mit dem Infinitiv(
 //       //sieht man sehr gut beim Verb "to be", bei dem es viele verschiede 
 //       //gebeugte Verbformen gibt: "Be careful", im Deutschen: "Sei|seid vorsichtig".
	//			predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb=
	//				pWordNode;
	//			predicate.m_bTense=1;
	//			predicate.m_bActiveClause=1;
	//			predicate.m_bNegation=0;
	//			subsentence.m_vecPredicate.push_back(predicate);
	//			dwStartOfNextSentenceElement++;
	//			bContainsFirstTokenOfSentence=FALSE;
	//		}
	//}
	//Prüfung auf gültiges Mittelteil erst ab dem Token hinter 
	//dwStartOfNextSentenceElement, denn wenn das kein Aufforderungssatz 
	//(bValidImperativ==FALSE) ist, muss sich ein Subjekt direkt vor dem 
	//Mittelteil befinden. Ein Subjekt besteht aus mindestens einem Token.
	if(!bValidImperativ && psv.size()>dwStartOfNextSentenceElement+1)
	{
		BOOL bThisPredicateIsWithActiveToBe=FALSE;
		BYTE bValidPredicate=FALSE;
		BYTE bPersonIndex=0;
		DWORD dwEndOfSubjectWithPersonalNames=0;
		DWORD dwEndOfSubjectWithoutPersonalNames=0;
		DWORD dwEndOfSubjectWithRelativeClause=0;
		Predicate predicateToBe;
		Predicate predicateRest;
		Subject subjectWithPersonalNames;
		Subject subjectWithRelativeClause;
		Subject subjectWithoutPersonalNames;
		std::vector <DWORD> vectordwStartOfPredicate;
		std::vector <DWORD> vectordwEndOfPredicate;
		std::vector <Predicate> vectorPredicate;
		Predicate predicate,predicateValid;
#ifdef _DEBUG
		printf("!bValidImperativ && psv.size()>dwStartOfNextSentenceElement+1\n");
          if(psv.size()==7)
            bPersonIndex=bPersonIndex;
#endif
		//Es kann passieren, dass mehrere Prädikate in einem "Teilsatz" gefunden
		//werden. Dann kann es sein, dass ein oder mehrere Prädikate zu 
		//einem Relativsatz oder mehreren Relativsätzen gehört/gehören.
		//Wenn zum Beispiel 2 Prädikate existieren, kann das letzte zum 
		//Relativsatz gehören: 
    // "You are the man I hate." Relativsatz: "I hate"
    //
    //Es kann auch sein,dass 2 Prädikate Direkt hintereinander liegen:
    //"The number you have dialled is incomplete."
    //                -Prädikat 1- \/
    //                             2.Prädikat

    //Oder es kann sein, dass ein Wort, dass mit einer Verbform identisch ist, 
    //im Satz erscheint: Some people like men are intelligent.
    //                               \  /     \ /
    //ist identisch mit dem Verb "to like"    (wirkliches) Prädikat
    //(ZUNÄCHST vermeintliches/mögliches 
    //Prädikat)

    //Oder es kann sein, dass ein Wort, dass mit einer Verbform identisch ist, 
    //direkt hinter dem Prädikat liegt: This is like working.
    //                                       \/ \  /
    //                    (wirkliches) Prädikat  ist identisch mit dem Verb "to like"
    //                                           (ZUNÄCHST vermeintliches/mögliches 
    //                                           Prädikat)

    //Damit bei der Überprüfung, ob die gebeugte Verbform des Prädikats mit
		//dem Subjekt übereinstimmt, nur noch ein Prädikat übrigbleibt,
		//existiert eine verschachtelte Schleife, in der geprüft wird, ob ein
		//Prädikat zu einem Relativsatz gehört und - falls nicht - an einen neuen 
		//Prädikatvektor angehängt
		for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size();++dwRun)
		{
			BYTE bReturnOfParseMiddle;
			DWORD	dwEndOfPredicate=0;
		  bValidPredicate=FALSE;
			BYTE bPersonIndexValid;
			for(DWORD dwRun2=dwRun;dwRun2<psv.size();++dwRun2)
			{
#ifdef _LOG
				if(dwRun==1 && dwRun2==3)
					LOG("");
#endif
        //Reset the pointers(they may have been set from before 
        //and would have been overtaken->possible side effects).
        predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb=NULL;
        predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb=NULL;
        predicate.m_bActiveClause=true;
        predicate.m_bNegation=false;
				//das ParseMiddle kann TRUE zurückliefern, obwohl das Prädikat
				//nicht vollständig ist: "I am walking.": wenn ParseMiddle 'am'
				//als Parameter übermittelt bekommt, ist 'am' als Prädikat zwar
				//grundsätzlich richtig, doch das Prädikat in diesem Haupt- oder
				//Nebensatz schliesst noch 'walking' ein.
				//Deswegen kann man nicht gleich beim ersten Mal, wenn ParseMiddle
				//TRUE zurückliefert, aus der inneren Schleife ausbrechen
				//wenn das Prädikat grammatikalisch korrekt ist
				if((bReturnOfParseMiddle=ParseNonBendPartOfPredicate(predicate,TRUE,
					GetBetweenAsCStringVector(psv,dwRun,dwRun2),dwFlags)))
				{
#ifdef _DEBUG
					printf("(bReturnOfParseMiddle=ParseFullPredicate(predicate,TRUE,\
GetBetweenAsCStringVector(psv,dwRun,dwRun2),0,dwFlags))\n");
#endif
					if(vectordwEndOfPredicate.size()>0)
					{
#ifdef _DEBUG
						printf("vectordwEndOfPredicate.size()>0\n");
#endif
					//wenn der Anfang dieses Prädikats vor oder beim letzten Token des
					//letzten Prädikats beginnt, überschneiden sich die Prädikate und
					//dieses Prädikat darf dem Vektor nicht angehängt werden
					//Beispiel: "You have dialled a number."
					//               +----------+
					//						    |	  +-----+
					//								|			 |
					//	letztes Prädikat     dieses Prädikat
						if(dwRun>vectordwEndOfPredicate.at(vectordwEndOfPredicate.size()-1))
						{
							bValidPredicate=TRUE;
							bValidMiddle=TRUE;
							predicateValid=predicate;
							//die folgende Zeile auskommentiert und ausgetauscht gegen
							//die Zeile unterhalb, da bei Relativsätzen falsche
							//zulässive Flexionsparadigmen waren (vom Prädikat vom
							//Relativsatz anstatt vom Prädikat des umgebenden Satzes.
							//bPersonIndexValid=bPersonIndex;
							bPersonIndexValid=bReturnOfParseMiddle;
							//dwStartOfPredicate=dwRun;
							dwEndOfPredicate=dwRun2;
	#ifdef _DEBUG
							printf("dwRun2: %u\n",dwRun2);
	#endif
						}
					}
					//wenn vectordwEndOfPredicate.size() gleich 0 ist
					else
					{
#ifdef _DEBUG
						printf("!vectordwEndOfPredicate.size()>0\n");
#endif
						//erst hier zuweisen, da sonst zum Beispiel bei 
						//"has worked" das "worked" für gültige 
						//Personalpronomen genommen 
						//die gebeugte Verbform befindet sich ja am Anfang
						bPersonIndex=bReturnOfParseMiddle;
						bValidPredicate=TRUE;
						bValidMiddle=TRUE;
						predicateValid=predicate;
						bPersonIndexValid=bPersonIndex;
						dwEndOfPredicate=dwRun2;
	#ifdef _DEBUG
						printf("dwRun2: %u\n",dwRun2);
	#endif
						//vectordwEndOfPredicate.push_back(dwRun2);
					}
				}//if(ParseFullPredicate(...)
        LOG("23.05.2008 11.15.08 "<<(short)predicate.m_bTense<<" "<<(short)predicate.m_bActiveClause<<"\n");
        //if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
          //LOG("PC-W:"<<predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWordNodeNext->m_pWord
			}//for(DWORD dwRun2=dwRun;dwRun2<psv.size();++dwRun2)
#ifdef _DEBUG
			printf("nach dem Ende der for-Schleife, in der das \"Mittelteil\" \
geparst wird\n");
#endif
		  if(bValidPredicate)
			{
				vectorPredicate.push_back(predicateValid);
				vecByte.push_back(bPersonIndexValid);
				vectordwEndOfPredicate.push_back(dwEndOfPredicate);
				vectordwStartOfPredicate.push_back(dwRun);
			}
#ifdef _DEBUG
			printf("nach push_back\n");
#endif
		}//for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size();++dwRun)
#ifdef _DEBUG
		printf("nach der äußeren Schleife, in der das Mittelteil geparst wird\n");
		printf("vectorPredicate.size(): %d\n",vectorPredicate.size());
		printf("vectordwEndOfPredicate.size(): %d\n",vectordwEndOfPredicate.
size());
		printf("vectordwStartOfPredicate.size(): %d\n",vectordwStartOfPredicate.
size());
#endif
		BYTE bValidSubjectOrObject=FALSE;
		BYTE bHasObject=FALSE;
		if(vectorPredicate.size()==0)
		{
#ifdef _DEBUG
			printf("BYTE ParseClause(Clause & subsentence,\
const PositionCStringVector & psv,std::vector<Range> & rVecRange,\
const DWORD & dwFlags,BYTE bContainsFirstTokenOfSentence return 0-\
vectorPredicate.size()==0\n");
#endif
			return 0;
		}
#ifdef _DEBUG
		for(dwRun=0;dwRun<vectordwEndOfPredicate.size();++dwRun)
			printf("vectordwEndOfPredicate.at(%u): %u\n",dwRun,
				vectordwEndOfPredicate.at(dwRun));
		printf("vectorPredicate.size(): %u\n",vectorPredicate.size());
		for(dwRun=0;dwRun<vectorPredicate.size();dwRun++)
		{
			printf("vectorPredicate.at(dwRun).m_bNegation: %u \
vectorPredicate.at(dwRun).m_bActiveClause: %u \
vectorPredicate.at(dwRun).m_bTense: %u\n",
				vectorPredicate.at(dwRun).m_bNegation,
				vectorPredicate.at(dwRun).m_bActiveClause,
				vectorPredicate.at(dwRun).m_bTense);
			if(vectorPredicate.at(dwRun).
				m_pWordNodeWhereTheGermanTranslationIsABendVerb)
			{
				printf("vectorPredicate.at(dwRun).\
m_pWordNodeWhereAnEnglishVerbIsEncapsulated\n");
				if(typeid(vectorPredicate.at(dwRun).
					m_pWordNodeWhereTheGermanTranslationIsABendVerb)==
					typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
						(vectorPredicate.at(dwRun).
						m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
					if(eav)
						printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]);
				}
				if(typeid(vectorPredicate.at(dwRun).
					m_pWordNodeWhereTheGermanTranslationIsABendVerb)==
					typeid(EnglishVerb))
				{
					EnglishVerb * ev=dynamic_cast<EnglishVerb *>
						(vectorPredicate.at(dwRun).
						m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
					if(ev)
						printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
				}
			}
			if(vectorPredicate.at(dwRun).
				m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
			{
				printf("vectorPredicate.at(dwRun).\
m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated\n");
				if(typeid(*vectorPredicate.at(dwRun).
					m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord)==
					typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
						(vectorPredicate.at(dwRun).
						m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord);
					if(eav)
						printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]);
				}
				if(typeid(*vectorPredicate.at(dwRun).
					m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord)==
					typeid(EnglishVerb))
				{
					EnglishVerb * ev=dynamic_cast<EnglishVerb *>
						(vectorPredicate.at(dwRun).
						m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord);
					if(ev)
						printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
				}
			}
		}
#endif//_DEBUG
		//Wenn der Haupt- oder Nebensatz mehr als 1 Prädikat enthält.
		if(vectorPredicate.size()>1)
		{
		  BYTE bBreak2ndLoopFromInside=FALSE;
      DWORD dwStartOfPredicate;
			std::vector<BYTE> vecBYTESubSentence;
			//jetzt wird das Prädikat des Haupt oder Nebensatzes gesucht:
			//wenn vor sich vor dem Prädikat ein Artikel und ein Nomen befinden und
			//zwischen dem Artikel plus Nomen und dem Prädikat ein Subjekt ist, das
			//keine Eigennamen enthält, ist dieses Prädikat NICHT das Prädikat des 
			//Haupt- oder Nebensatzes, das gesucht wird, sondern das Prädikat eines
			//Relativsatzes
		  //maximal alle Prädikate durchlaufen
			//The number you have dialled is incomplete.
			//Art Nomen  SOE --Prädikat--
			//You are the air I am breathing.
			//        Art Nom E --Prädikat--
			//Art:Artikel  Nom:Nomen  SOE/E:Subjekt ohne Eigennamen
			//es wird mit dem letzten Prädikat begonnen
		  for(DWORD dwRun=vectorPredicate.size()-1;dwRun>=0 && g_bContinue;--dwRun)
		  {
				dwStartOfPredicate=vectordwStartOfPredicate.at(dwRun);
		    bBreak2ndLoopFromInside=FALSE;
#ifdef _DEBUG
				printf("Schleife 1 dwStartOfPredicate: %u\n",dwStartOfPredicate);
#endif
				//dwStartOfNextSentenceElement entspricht dem Index des Tokens hinter
				//einem Imperativ, dem Index eines Tokens hinter dem Adverb der Zeit
				//am Anfang des Satzes, dem Index eines Tokens hinter der letzten 
				//Konjunktion oder 0
		    if(dwStartOfPredicate>dwStartOfNextSentenceElement+2)
				{
			    for(DWORD dwRun2=dwStartOfPredicate-3;dwRun2>=
						dwStartOfNextSentenceElement && dwRun2!=4294967295 && g_bContinue;
						--dwRun2)
			    {
#ifdef _DEBUG
						printf("Schleife 2 dwRun2: %u dwStartOfNextSentenceElement: %u\n"
						,dwRun2,dwStartOfNextSentenceElement);
#endif
				    for(DWORD dwRun3=dwRun2+1;dwRun3<dwStartOfPredicate && g_bContinue;
							++dwRun3)
			      {
#ifdef _DEBUG
							printf("Schleife 3\n");
#endif
							EnglishNounRecord enr;
					    //eine Struktur aus einem bestimmten englischen Artikel und einem Nomen
			        //besteht immer mindestens aus 2 Token
					    //Beispiel: "the number"
							//ein Subjekt besteht immer mindestens aus einem 1 Token
					    //Deswegen soll nach einem Artikel plus Nomen erst 3 Token vor
						  //dem Anfang des Prädikats geparst werden
							//Beispiel: I like the number you dialled.
			        //                                +-----+
					    //                                   ^
							//                                   |
				      //                                Prädikat
					    //erste Prüfung auf ParseDefiniteArticleNoun: "you"
						  //zweite Prüfung auf ParseDefiniteArticleNoun: "number"
							//dritte Prüfung auf ParseDefiniteArticleNoun: "number you"
							//vierte Prüfung auf ParseDefiniteArticleNoun: "the"
			        //fünfte Prüfung auf ParseDefiniteArticleNoun: "the number"
					    //ParseDefiniteArticleNoun gibt TRUE zurück
				      //Prüfung auf ParseSubject: "you"
					    //ParseSubject gibt 1 zurück
							CStringVector sv = GetBetweenAsCStringVector(psv,dwRun2,dwRun3) ;
						  if(ParseDefiniteArticleNoun(enr,sv,dwFlags,FALSE))
							{
								std::vector<Range> vecRange;
					      //Von direkt nach "bestimmter Artikel+Nomen" bis direkt vor dem
                //vermuteten Prädikat nach einem Subjekt parsen.
                //Bei "I like the number you dialled." würde "you" geparst werden.
							  if(ParseSubject(subject,GetBetween(psv,dwRun3+1, 
					        dwStartOfPredicate-1),FALSE,vecRange,dwFlags,
                  //Wenn das Subjekt keine (vermeintlichen) Eigennamen enthält.
									bContainsFirstTokenOfSentence)==1)
									//das Prädikat gehört zu einem Relativsatz, also Flag setzen, 
									//um schnellstmöglich zum nächsten Prädikat zu kommen
					        bBreak2ndLoopFromInside=TRUE;
			        }
					    if(bBreak2ndLoopFromInside)
							  break;
			      }
					  if(bBreak2ndLoopFromInside)
							break;
					}//for-loop from begin of current predicate to the end of the last sentence element
				}//if(dwStartOfPredicate>dwStartOfNextSentenceElement+2)
				else//!dwStartOfPredicate>dwStartOfNextSentenceElement+2
				{//(=wenn weniger als 2 Token zwischen begin of current predicate to the end of the last sentence element)
#ifdef _DEBUG
					printf("!(dwStartOfPredicate>dwStartOfNextSentenceElement+2)\n");
#endif
					std::vector<Range> vecRange;
				  subsentence.m_pSubject=new Subject();
		      returnValue=ParseSubject(*subsentence.m_pSubject,GetBetween(psv,
						dwStartOfNextSentenceElement,dwStartOfPredicate-1),FALSE,vecRange,
						dwFlags,bContainsFirstTokenOfSentence);
					if(returnValue==2)
						returnValue=1;
					else if(returnValue==1)
						returnValue=2;
					for(DWORD dwRun2=0;dwRun2<vecRange.size();dwRun2++)
						rVecRange.push_back(vecRange.at(dwRun2));
#ifdef _DEBUG
					printf("vectordwEndOfPredicate.at(%u): %u\n",dwRun,
						vectordwEndOfPredicate.at(dwRun));
					printf("subsentence.m_pSubject->m_vecEnumerationElement.size(): %d\n",
						subsentence.m_pSubject->m_vecEnumerationElement.size());
#endif
					dwEndOfValidMiddle=vectordwEndOfPredicate.at(dwRun);
				  dwStartOfNextSentenceElement=vectordwEndOfPredicate.at(dwRun)+1;
					predicateRest=vectorPredicate.at(dwRun);
					//subsentence.m_vecPredicate.push_back(predicateRest);
					vecBYTESubSentence.push_back(vecByte.at(dwRun));
					bPredicateRest=TRUE;
					break;
				}//wenn weniger als 2 Token zwischen begin of current predicate to the end of the last sentence element
		    //wenn nicht: bestimmter Artikel plus Nomen und zwischen 
				//dem bestimmten Artikel
				//plus Nomen und einem Prädikat ein Subjekt ohne Eigennamen 
				//vorkam, dann ist
		    //das Prädikat das gesuchte Prädikat
        //(=Wenn das Prädikat nicht zu einem Relativsatz gehört.)
				if(!bBreak2ndLoopFromInside)
		    {
#ifdef _DEBUG
					printf("!bBreak2ndLoopFromInside\n");
#endif
					std::vector<Range> vecRange;
				  subsentence.m_pSubject=new Subject();
					returnValue=ParseSubject(*subsentence.m_pSubject,GetBetween(psv,
						dwStartOfNextSentenceElement,dwStartOfPredicate-1),FALSE,vecRange,
						dwFlags,bContainsFirstTokenOfSentence);
					if(returnValue==2)
						returnValue=1;
					else if(returnValue==1)
						returnValue=2;
					for(DWORD dwRun2=0;dwRun2<vecRange.size();dwRun2++)
						rVecRange.push_back(vecRange.at(dwRun2));
#ifdef _DEBUG
					printf("vectordwEndOfPredicate.at(%u): %u\n",dwRun,
						vectordwEndOfPredicate.at(dwRun));
					printf("subsentence.m_pSubject->m_enumerationElements.size(): %d\n",
						subsentence.m_pSubject->m_vecEnumerationElement.size());
					printf("subsentence.m_vecPredicate.size(): %u\n",subsentence.
						m_vecPredicate.size());
#endif
					dwEndOfValidMiddle=vectordwEndOfPredicate.at(dwRun);
		      dwStartOfNextSentenceElement=vectordwEndOfPredicate.at(dwRun)+1;
					predicateRest=vectorPredicate.at(dwRun);
					//subsentence.m_vecPredicate.push_back(predicateRest);
					vecBYTESubSentence.push_back(vecByte.at(dwRun));
					bPredicateRest=TRUE;
				  break;
		    }//if(!bBreak2ndLoopFromInside)
		  }//end of for-loop iterating through predicates
		  //if(!
			//die folgende Zeile auskommentiert und ausgetauscht gegen
			//die Zeile unterhalb, da bei Relativsätzen falsche
			//zulässive Flexionsparadigmen waren (vom Prädikat vom
			//Relativsatz anstatt vom Prädikat des umgebenden Satzes.
			//subsentence.m_vecPredicate.size() war nämlich 1 bei einem
			//Satz, der auf Übereinstimmung von Prädikat und Subjekt im
			//Flexionsparadigma geprüft hätte werden sollen
			//if(subsentence.m_vecPredicate.size()==1)
			if(vecBYTESubSentence.size()==1)
			{
				if(!((vecBYTESubSentence.at(0)>>subsentence.m_pSubject->m_personIndex)&1))
				{
          //See http://en.wikipedia.org/wiki/Agreement_%28linguistics%29
          subsentence.m_dwErrorCode=NoGrammaticalPersonAgreement;
	#ifdef _DEBUG
					printf("BYTE ParseClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
dwFlags,BYTE bContainsFirstTokenOfSentence - return FALSE - falscher Personenindex\n");
	#endif
					return FALSE;
				}
			}
		}//if(vectorPredicate.size()>1)/Wenn der Haupt- oder Nebensatz mehr als 1 Prädikat enthält.		
		else//Wenn die Anzahl der möglichen Prädikate <=1 ist.
		{
//			std::vector<Range> vecRange;
//			subsentence.m_pSubject=new Subject();
//		  returnValue=ParseSubject(*subsentence.m_pSubject,GetBetween(psv,
//				dwStartOfNextSentenceElement,vectordwStartOfPredicate.at(0)-1),
//				FALSE,vecRange,dwFlags,bContainsFirstTokenOfSentence);
//#ifdef _DEBUG
//			printf("bPersonIndex: %u subsentence.m_pSubject->m_personIndex: %u\n",
//				bPersonIndex,subsentence.m_pSubject->m_personIndex);
//			printf("(bPersonIndex>>subsentence.m_pSubject->m_personIndex)&1: %u\n",
//				(bPersonIndex>>subsentence.m_pSubject->m_personIndex)&1);
//#endif
//			//wenn der Personenindex des Prädikats nicht mit dem Personenindex des
//			//Subjekts übereinstimmt
//			//Beispiel, wenn er nicht übereinstimmt: "I likes you."
//			//8 Bit von bPersonIndex geben an, welcher Personenindex bei einem
//			//gebeugtem Verb möglich ist
//			//Beispiel: burns:
//			//Infinitiv: "burn" : stimmt nicht überein mit "burns"            0
//			//Personenindex 1: "I burn" : stimmt nicht überein mit "burns"    0
//			//Personenindex 2: "you burn" : stimmt nicht überein mit "burns"  0
//			//Personenindex 3: "he/she/it burns" : stimmt überein mit "burns" 1
//			//Personenindex 4: "we burn" : stimmt nicht überein mit "burns"   0
//			//Personenindex 5: "you burn" : stimmt nicht überein mit "burns"  0
//			//Personenindex 6: "they burn" : stimmt nicht überein mit "burns" 0
//			//                                   Bit 8 bleibt ungenutzt, also 0
//			//das resultierende Byte sieht so aus: 00010000 (dezimal: 16)
//			//wenn die Bits des Byte nun nach rechts verschoben werden mit der
//			//Anzahl der Stellen des Personenindex des Subjekts und das Ergebnis 
//			//bitweise-Und mit 1 verknüpft wird und ungleich
//			//"0" ist, stimmen der Personenindex des Subjekts und des Prädikats
//			//überein
//			//wenn Personenindex 3 (zum Beispiel "he"): 00001000 >> 3= 00000001
//			//00000001 & 00000001=00000001 (also ungleich "0")
//			if(!((bPersonIndex>>subsentence.m_pSubject->m_personIndex)&1))
//			{
//	#ifdef _DEBUG
//				printf("BYTE ParseClause(Clause & subsentence,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
//dwFlags,BYTE bContainsFirstTokenOfSentence - return FALSE - falscher Personenindex\n");
//	#endif
//				return FALSE;
//			}
//			if(returnValue==2)
//				returnValue=1;
//			else if(returnValue==1)
//				returnValue=2;
//			for(DWORD dwRun=0;dwRun<vecRange.size();++dwRun)
//				rVecRange.push_back(vecRange.at(dwRun));
		  dwStartOfNextSentenceElement=vectordwEndOfPredicate.at(0)+1;
			dwEndOfValidMiddle=vectordwEndOfPredicate.at(0);
			predicateRest=vectorPredicate.at(0);
			bPredicateRest=TRUE;
#ifdef _DEBUG
			printf("am Ende von else-Zweig der Bedingung if(vectorPredicate.size()>1)\n");
#endif
		}//Wenn nur ein Prädikat vorhanden ist.
		/*for(DWORD dwRun=dwStartOfNextSentenceElement+1;dwRun<psv.size() && 
			g_bContinue;dwRun++)
		{
			for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
			{
				Predicate predicate;
				bThisPredicateIsWithActiveToBe=FALSE;
				if(ParseMiddle(predicate,TRUE,GetBetweenAsCStringVector(psv,dwRun,
					dwRun2),0,dwFlags))
				{
					BYTE bReturnValueOfParseSubject=0;
#ifdef _DEBUG
					if(predicate.m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated)
					{
						printf("predicate.m_pVocNodeWhereAnEnglishInfinitiveIsEnca\
psulated\n");
						printf("typeid(predicate.m_pVocNodeWhereAnEnglishInfinitive\
IsEncapsulated %s\n",typeid(*predicate.
							m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated->voc));
					}
#endif
					vecRange.clear();
					bReturnValueOfParseSubject=ParseSubject(subject,GetBetween(psv,
						0,dwRun-1),TRUE,vecRange,dwFlags,bContainsFirstTokenOfSentence);
					if(bReturnValueOfParseSubject==0)
					{
						dwEndOfSubjectWithPersonalNames=dwRun-1;
						subjectWithPersonalNames=subject;
					}
					if(bReturnValueOfParseSubject==1)
					{
						dwEndOfSubjectWithoutPersonalNames=dwRun-1;
						subjectWithoutPersonalNames=subject;
					}
					if(bReturnValueOfParseSubject==2)
					{
						dwEndOfSubjectWithRelativeClause=dwRun-1;
						subjectWithRelativeClause=subject;
					}
					if(!bValidMiddle)
						dwStartOfValidMiddle=dwRun;
					dwStartOfValidMiddle=dwRun;
					dwEndOfValidSentenceElement=dwRun;
					bValidMiddle=TRUE;
					if(predicate.m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated)
						if(typeid(*predicate.
							m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated->voc)==
							typeid(EnglishAuxiliaryVerb))
						{
								EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
									(predicate.
									m_pVocNodeWhereAnEnglishInfinitiveIsEncapsulated->voc);
#ifdef _DEBUG
								printf("typeid(*predicate.m_pVocNodeWhereAnEnglishInfiniti\
veIsEncapsulated->voc)==typeid(EnglishAuxiliaryVerb)\n");
#endif
								if(eav->words[0]==_T("be") && predicate.m_bActiveClause)
								{
#ifdef _DEBUG
									printf("eav->words[0]==_T(\"be\") && predicate.m_bActiveClause\n");
#endif
									predicateToBe=Predicate(predicate);
									bThisPredicateIsWithActiveToBe=TRUE;
									bPredicateWithActiveToBe=TRUE;
									dwEndOfValidMiddleWithActiveToBe=dwRun2;
								}
						}
					if(!bThisPredicateIsWithActiveToBe)
					{
#ifdef _DEBUG
						printf("!bPredicateWithActiveToBe\n");
#endif
						bPredicateRest=TRUE;
						dwEndOfValidMiddle=dwRun2;
						predicateRest=predicate;
					}
				}
				//if(predicate.m_ActiveClause)
				//if(ParseMiddle(subsentence,TRUE,GetBetweenAsCStringVector(psv,dwRun,
				//	dwRun2),0,dwFlags))
				//{
				//	if(!bValidMiddle)
				//		dwStartOfValidMiddle=dwRun;
				//	dwEndOfValidMiddle=dwRun2;
				//	dwStartOfValidMiddle=dwRun;
				//	dwEndOfValidSentenceElement=dwRun;
				//	bValidMiddle=TRUE;
				//}
			}
/*			if(bValidMiddle)
			{
#ifdef _DEBUG
				printf("bValidMiddle\n");
#endif
				if(bPredicateRest)
				{
#ifdef _DEBUG
					printf("bPredicateRest\n");
#endif
					subsentence.m_vecPredicate.push_back(predicateRest);
				}
				if(bPredicateWithActiveToBe)
				{
#ifdef _DEBUG
					printf("bPredicateWithActiveToBe\n");
#endif
					subsentence.m_vecPredicate.push_back(predicateToBe);
				}
				dwStartOfNextSentenceElement=dwEndOfValidMiddle+1;
				//break;
			}
		}*/
		if(bValidMiddle)
		{
#ifdef _DEBUG
			printf("bValidMiddle\n");
			fflush(stdout);
#endif
			if(bPredicateRest)
			{
#ifdef _DEBUG
				printf("bPredicateRest\n");
				printf("dwEndOfValidMiddle: %u\n",dwEndOfValidMiddle);
#endif
				if(dwEndOfValidMiddle==dwEndOfValidMiddleWithActiveToBe)
					subsentence.m_vecPredicate.push_back(predicateRest);
				else
				{
					if(dwEndOfValidMiddle>dwEndOfValidMiddleWithActiveToBe)
						subsentence.m_vecPredicate.push_back(predicateRest);
				}
			}
			if(bPredicateWithActiveToBe)
			{
#ifdef _DEBUG
				printf("bPredicateWithActiveToBe\n");
				printf("dwEndOfValidMiddleWithActiveToBe: %u\n",
					dwEndOfValidMiddleWithActiveToBe);
#endif
				if(dwEndOfValidMiddle==dwEndOfValidMiddleWithActiveToBe)
					subsentence.m_vecPredicate.push_back(predicateToBe);
				else
				{
					if(dwEndOfValidMiddleWithActiveToBe>dwEndOfValidMiddle)
						subsentence.m_vecPredicate.push_back(predicateToBe);
				}
			}
			//dwStartOfNextSentenceElement=dwEndOfValidMiddle+1;
			/*if(dwEndOfSubjectWithRelativeClause>0 && 
				dwEndOfSubjectWithRelativeClause>
				dwEndOfSubjectWithoutPersonalNames)
			{
				subsentence.m_pSubject=new Subject(subjectWithRelativeClause);
			}
			else
				subsentence.m_pSubject=new Subject(subjectWithoutPersonalNames);
			*/
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
			{
				rVecRange.push_back(vecRange.at(dwRun));
#ifdef _DEBUG
				printf("vecRange.at(%lu).m_dwStart: %lu vecRange.at(%lu).\
m_dwEnd: %lu\n",dwRun,vecRange.at(dwRun).m_dwStart,dwRun,vecRange.at(dwRun).m_dwEnd);
#endif
			}
#ifdef _DEBUG
			printf("subsentence.m_vecPredicate.size(): %u\n",subsentence.
				m_vecPredicate.size());
#endif
		}
	}
#ifdef _DEBUG
	printf("subsentence.m_vecPredicate.size(): %u\n",
		subsentence.m_vecPredicate.size());
#endif
	if(bValidMiddle) // wegen: dwStartOfValidMiddle-1
	{
		/*Subject subject;
		std::vector<Range> vecRange;
		if(!ParseSubject(subject,GetBetween(psv,dwEndOfElementBeforeMiddle,
			dwStartOfValidMiddle-1),FALSE,vecRange,dwFlags,bContainsFirstTokenOfSentence))
		{
#ifdef _DEBUG
			printf("BOOL ParseClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange) - return FALSE \
- ungültiges Subjekt\n");
#endif
			return FALSE;
		}
		subsentence.m_pSubject=new Subject(subject);
		bContainsFirstTokenOfSentence=FALSE;
		//if(vecRange.size())
		//	rVecRange.insert(rVecRange.end()-1,vecRange.begin(),vecRange.end()-1);
		for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
			rVecRange.push_back(vecRange.at(dwRun));
		//subsentence.m_Subject=subject;*/
	}
//	else
//	{
		//#ifdef _DEBUG
		//printf("BOOL ParseClause(Clause & subsentence,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange) - return FALSE\
// - !bValidMiddle\n");
//		#endif
//	}
	if(bValidMiddle || bValidImperativ) // gültiges 'Mittelteil' (Prädikat, usw)
	{
		BOOL bBe=FALSE;
		DWORD dwStartOfManner=0;
		GermanVerb * gv=NULL;
		Predicate predicate;
		#ifdef _DEBUG
		printf("bValidMiddle || bValidImperativ\n");
		if(bValidMiddle)
			printf("bValidMiddle\n");
		else
			printf("bValidImperativ\n");
		#endif
#ifdef _DEBUG
		printf("psv.size: %u dwStartOfNextSentenceElement: %u vor ParseManner\n",
			psv.size(),dwStartOfNextSentenceElement);
#endif
		predicate=subsentence.m_vecPredicate.at(0);
#ifdef _DEBUG
		printf("nach: predicate=subsentence.m_vecPredicate.at(0)\n");
#endif
		if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
		{
#ifdef _DEBUG
			printf("predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb\n");
#endif
			if(typeid(*predicate.
				m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord)==
				typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
					(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
					m_pWord);
				if(eav)
				{
#ifdef _DEBUG
					printf("eav->m_strWords[0]:%s\n",eav->m_strWords[0]);
#endif
					if(eav->m_strWords[0]==_T("be"))
						bBe=TRUE;
				}
			}
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb->
				m_pWordNodeNext)
				gv=dynamic_cast<GermanVerb*>(predicate.
					m_pWordNodeWhereTheGermanTranslationIsABendVerb->
					m_pWordNodeNext->m_pWord);
		}
		if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
		{
#ifdef _DEBUG
			printf("predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb\n");
#endif
			if(typeid(*predicate.
				m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord)==
				typeid(EnglishAuxiliaryVerb))
			{
#ifdef _DEBUG
			printf("predicate.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated->\
m_pWord==typeid(EnglishAuxiliaryVerb)\n");
#endif
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
					(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
					m_pWord);
				if(eav)
				{
#ifdef _DEBUG
					printf("eav->m_strWords[0]:%s\n",eav->m_strWords[0]);
#endif
					if(eav->m_strWords[0]==_T("be"))
					{
						bBe=TRUE;
					}
				}
			}
			if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
				m_pWordNodeNext)
				gv=dynamic_cast<GermanVerb*>(predicate.
				m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->
				m_pWordNodeNext->m_pWord);
		}
		if(gv)
			if(gv->m_bCase==1 || gv->m_bCase==2 || gv->m_bCase==3 || gv->m_bCase==4)
				bObjectVerb=TRUE;
		if(dwStartOfNextSentenceElement<psv.size() || (bPredicateWithActiveToBe 
			&& bPredicateRest))
		{
			//if(
			//BOOL bBe=FALSE;
			DWORD dwEndOfValidManner=0;
			CString strBe("be") ;
			EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strBe);
		#ifdef _DEBUG
			printf("dwStartOfNextSentenceElement<psv.size() || \
(bPredicateWithActiveToBe && bPredicateRest)\n");
		#endif
			/*if(subsentence.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated)
				if(typeid(*subsentence.
					m_pVocNodeWhereAnEnglishPredicateIsEncapsulated->voc)==
					typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
						(subsentence.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated->voc);
					if(eav)
						if(eav->words[0]==_T("be"))
							bBe=TRUE;
				}*/
			//wenn das Verb kein Objekt verlangt
			if(!bObjectVerb//)// || bPredicateWithActiveToBe)
        //Wenn z.B. Relativsatz mit bei: "The man you hate surprisingly.", dann ...
        //                                        \      /
        //                                       Relativsatz
        //...muss/darf der Relativsatz kein Objekt beinhalten(, da sich 
        //der Relativsatz ja auf das Subjekt("The man") das Hauptsatzes bezieht).
        || dwFlags&LEAVE_OUT_1ST_OBJECT && bObjectVerb)
			{
		#ifdef _DEBUG
				printf("!bObjectVerb || bPredicateWithActiveToBe\n");
		#endif
				//DWORD dwRun=0;
				if(bPredicateWithActiveToBe)
					dwStartOfNextSentenceElement=dwEndOfValidMiddleWithActiveToBe+1;
				else
					dwStartOfNextSentenceElement=dwEndOfValidMiddle+1;
				for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && 
				//for(;dwRun<psv.size() && 
					g_bContinue;dwRun++)
				{
					Manner manner;
					if(ParseManner(manner,GetBetween(psv,
						dwStartOfNextSentenceElement,dwRun),bPredicateWithActiveToBe,dwFlags))
					{
						TRACE("nach: if(ParseManner(manner,GetBetween(psv,dwStartOfNextSentenceElement,dwRun),TRUE))\n");
						dwEndOfValidManner=dwRun;
						bValidManner=TRUE;
						//bValidObject=TRUE;
						if(subsentence.m_pManner)
						{
							TRACE("subsentence.m_pManner\n");
							delete subsentence.m_pManner;
						}
						subsentence.m_pManner=new Manner;
						*subsentence.m_pManner=manner;
						dwStartOfNextSentenceElement=dwRun+1;
						dwStartOfManner=dwRun;
						TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
						dwStartOfFirstSentenceElementAfterMiddle=dwStartOfNextSentenceElement;
					}
				}
				if(bValidManner)
					dwStartOfNextSentenceElement=dwEndOfValidManner+1;
			}
			//wenn das Verb ein Objekt verlangt
			else
			{
        //Strategie: zuerst alle Satz-Elemente, die zwischen einem Prädikat 
        //und einem Objekt befinden können, parsen. Denn falls das Objekt ein
        //Eigenname ist, weiß man dann den Beginn des Eigennamens.
#ifdef _DEBUG
				printf("bObjectVerb\n");
#endif
				BOOL bBreak=FALSE;
				//if(bPredicateWithActiveToBe)
				//	dwStartOfNextSentenceElement=dwEndOfValidMiddleWithActiveToBe+1;
				//else
				//	dwStartOfNextSentenceElement=dwEndOfValidMiddle+1;
        
        //Auch Verben, die ein Objekt verlangen, können eine Art und Weise beinhalten:
        //He hit her completely wrong." Art und Weise: completely wrong
				for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && 
					g_bContinue;dwRun++)
				{
					for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
					{
						//if(subsentence.predicate==eav->words[1] || subsentence.predicate==eav->words[2] ||  // am, are, is
						//	subsentence.predicate==eav->words[3] || subsentence.predicate==eav->words[7] || subsentence.predicate==eav->words[8]) // was, were
						//	bBe=TRUE;
						Manner manner;
						if(ParseManner(manner,GetBetween(psv,dwRun,dwRun2),
							bBe,dwFlags))
						{
							TRACE("nach: if(ParseManner(manner,GetBetween(psv,dwStartOfNextSentenceElement,dwRun),TRUE))\n");
							dwEndOfValidManner=dwRun2;
							bValidManner=TRUE;
							//bValidObject=TRUE;
							if(subsentence.m_pManner)
							{
								TRACE("subsentence.m_pManner\n");
								delete subsentence.m_pManner;
							}
							subsentence.m_pManner=new Manner;
							*subsentence.m_pManner=manner;
							//dwStartOfNextSentenceElement=dwRun2+1;
							dwEndOfObject=dwRun-1;
							TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
							dwStartOfNextSentenceElement=dwRun2+1;
							dwStartOfFirstSentenceElementAfterMiddle=dwRun;
						}
					}
					if(bValidManner) //if(bBreak)
						if(gv->m_bCase==4 && dwEndOfValidMiddle==dwEndOfObject && 
							dwStartOfNextSentenceElement==psv.size())
						{
#ifdef _DEBUG
							printf("ParseClause - return returnValue (%u)\n",returnValue);
#endif
							return returnValue;
						}
						else
							break;
				}
				if(bValidManner)
				{
					dwStartOfNextSentenceElement=dwEndOfValidManner+1;
					//if(dwEndOfValidMiddle+1==dwEndOfValidManner
				}
			}
		}
		if(bPredicateWithActiveToBe && bValidManner)
			dwEndOfValidMiddle=dwEndOfValidMiddleWithActiveToBe;
		// wenn Prädikat vom Verb 'to be' und die Umstandsbestimmumg
		// der Art und Weise nicht direkt nach dem Mittelteil (Prädikat und so 
		// weiter) beginnt, dann die Umstandsbestimmung der Art und Weise
		// ungültig machen, weil sich nach dem Mittelteil noch ein Objekt
		// befinden kann, das Teile der Umstandsbestimmung der Art und Weise
		// enthält . zum Beispiel: "I am a good artist."
		// "good" wird zunächst als Umstandsbestimmung der Art und Weise
		// erkannt. "good" liegt aber nicht direkt hinter dem Mittelteil
		// (Prädikat und so weiter); das Objekt ist: "a good artist"		
		if(bValidManner && gv->m_strWords[0]==_T("sein") && bObjectVerb &&
			dwEndOfObject!=dwEndOfValidMiddle)
		{
#ifdef _DEBUG
			printf("bValidManner && gv->m_strWords[0]==_T(\"sein\") && \
dwEndOfObject!=dwEndOfValidMiddle)\n");
			printf("dwEndOfObject: %u dwEndOfValidMiddle: %u\n",dwEndOfObject,
				dwEndOfValidMiddle);
#endif
			bValidManner=FALSE;
			delete subsentence.m_pManner;
			subsentence.m_pManner=NULL;
			dwEndOfObject=0;
		}
		if(dwStartOfNextSentenceElement<psv.size() && 
      //Only when this is a PASSIVE clause a by-agent after the predicate
      //is valid.
      !subsentence.m_bActiveClause)
		{
			if(bValidManner || !bObjectVerb)
			{
				DWORD dwEndOfValidByAgent=0;
				for(DWORD dwRun=dwEndOfValidSentenceElement+1;dwRun<psv.size() && 
					g_bContinue;dwRun++)
				{
					ByAgent byAgent;
					if(ParseByAgent(byAgent,GetBetween(psv,dwStartOfNextSentenceElement,
						dwRun),dwFlags))
					{
						//dwEndOfValidObject=dwRun;
						TRACE("nach: if(ParseManner(manner,GetBetween(psv,dwStartOfNextSentenceElement,dwRun),TRUE))\n");
						dwEndOfValidByAgent=dwRun;
						bValidByAgent=TRUE;
						//bValidObject=TRUE;
						if(subsentence.m_pByAgent)
						{
							TRACE("subsentence.m_pByAgent\n");
							delete subsentence.m_pByAgent;
						}
						subsentence.m_pByAgent=new ByAgent;
						TRACE("nach: if(subsentence.m_pByAgent)\n");
						//TRACE("manner.m_vecMannerEnumerationElements.size(): %u\n",manner.m_vecMannerEnumerationElements.size());
						*subsentence.m_pByAgent=byAgent;
						//TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
						if(!dwStartOfFirstSentenceElementAfterMiddle)
							dwStartOfFirstSentenceElementAfterMiddle=
							dwStartOfNextSentenceElement;
					}
					if(bValidByAgent)
						dwStartOfNextSentenceElement=dwEndOfValidByAgent+1;
				}//if(ParseByAgent(...)
			}//if(bValidManner || !bObjectVerb)
			else
			{
        if(bObjectVerb
          ////Only when this is a PASSIVE clause a by-agent after the predicate
          ////is valid.
          //&& !subsentence.m_bActiveClause
          )
				{
					DWORD dwEndOfValidByAgent=0;
					for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && 
						g_bContinue;dwRun++)
					{
						for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
						{
							ByAgent byAgent;
							if(ParseByAgent(byAgent,GetBetween(psv,dwRun,dwRun2),dwFlags))
							{
								//dwEndOfValidObject=dwRun;
								TRACE("nach: if(ParseManner(manner,GetBetween(psv,dwRun,dwRun2),TRUE))\n");
								dwEndOfValidByAgent=dwRun2;
								bValidByAgent=TRUE;
								dwEndOfObject=dwRun-1;
								//bValidObject=TRUE;
								if(subsentence.m_pByAgent)
								{
									TRACE("subsentence.m_pByAgent\n");
									delete subsentence.m_pByAgent;
								}
								subsentence.m_pByAgent=new ByAgent;
								TRACE("nach: if(subsentence.m_pByAgent)\n");
								//TRACE("manner.m_vecMannerEnumerationElements.size(): %u\n",manner.m_vecMannerEnumerationElements.size());
								*subsentence.m_pByAgent=byAgent;
								if(!dwStartOfFirstSentenceElementAfterMiddle)
									dwStartOfFirstSentenceElementAfterMiddle=dwRun;
								//TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
							}//if(ParseByAgent(...)
						}
					}
					if(bValidByAgent)
						dwStartOfNextSentenceElement=dwEndOfValidByAgent+1;
				}
			}//else-branch of "if(bValidManner || !bObjectVerb)"; ParseByAgent(...)
		}//if(dwStartOfNextSentenceElement<psv.size())
#ifdef _DEBUG
		printf("psv.size: %u dwStartOfNextSentenceElement: %u\n",psv.size(),
			dwStartOfNextSentenceElement);
#endif
		std::vector<Range> vecRangeOfPlace;
		if(dwStartOfNextSentenceElement<psv.size())
		{
			if(bValidManner || bValidByAgent || !bObjectVerb
        //Wenn z.B. Relativsatz mit bei: "The man you hate at school.", dann ...
        //                                        \                /
        //                                         --Relativsatz---
        //...muss/darf der Relativsatz kein Objekt beinhalten(, da sich 
        //der Relativsatz ja auf das Subjekt("The man") das Hauptsatzes bezieht).
        || dwFlags&LEAVE_OUT_1ST_OBJECT && bObjectVerb
        )
			{
				DWORD dwEndOfValidPlace=0;
				for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && 
					g_bContinue;dwRun++)
				{
					Place place; // Umstandsbestimmung des Ortes
					std::vector<Range> vecRange;
					if(ParsePlace(place,GetBetween(psv,dwStartOfNextSentenceElement,
						dwRun),vecRange,dwFlags))
					{
						if(subsentence.m_pPlace)
							delete subsentence.m_pPlace;
						subsentence.m_pPlace=new Place;
						*subsentence.m_pPlace=place;
						bValidPlace=TRUE;
						dwEndOfValidPlace=dwRun;
						vecRangeOfPlace=vecRange;
						if(!dwStartOfFirstSentenceElementAfterMiddle)
							dwStartOfFirstSentenceElementAfterMiddle=
								dwStartOfNextSentenceElement;
					}
				}
				if(bValidPlace)
					dwStartOfNextSentenceElement=dwEndOfValidPlace+1;
			}
			else
			{
				if(bObjectVerb)
				{
					DWORD dwEndOfValidPlace=0;
					for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() && g_bContinue;dwRun++)
					{
						for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
						{
							Place place; // Umstandsbestimmung des Ortes
							std::vector<Range> vecRange;
							if(ParsePlace(place,GetBetween(psv,dwRun,dwRun2),vecRange,dwFlags))
							{
								if(subsentence.m_pPlace)
									delete subsentence.m_pPlace;
								subsentence.m_pPlace=new Place;
								*subsentence.m_pPlace=place;
								bValidPlace=TRUE;
								dwEndOfValidPlace=dwRun2;
								dwEndOfObject=dwRun-1;
								vecRangeOfPlace=vecRange;
								if(!dwStartOfFirstSentenceElementAfterMiddle)
									dwStartOfFirstSentenceElementAfterMiddle=dwRun;
							}
						}
					}
					if(bValidPlace)
						dwStartOfNextSentenceElement=dwEndOfValidPlace+1;
				}
			}
		}
		if(bValidPlace)
		{
			for(DWORD dwRun=0;dwRun<vecRangeOfPlace.size();dwRun++)
			{
				rVecRange.push_back(vecRangeOfPlace.at(dwRun));
			}
		}
#ifdef _DEBUG
		printf("psv.size: %u dwStartOfNextSentenceElement: %u\n",psv.size(),
			dwStartOfNextSentenceElement);
		printf("vor der Prüfung auf eine Umstandsbestimmung der Zeit am Ende des\
 \"Untersatzes\"\n");
#endif
    LOG("19.05.2008 20.42\n");
		if(dwStartOfNextSentenceElement<psv.size())
		{
			if(!bObjectVerb || bValidManner || bValidByAgent || bValidPlace 
        //Wenn z.B. Relativsatz mit bei: "The man you hate today.", dann ...
        //                                        \            /
        //                                         Relativsatz-
        //...muss/darf der Relativsatz kein Objekt beinhalten(, da sich 
        //der Relativsatz ja auf das Subjekt("The man") das Hauptsatzes bezieht).
        || dwFlags&LEAVE_OUT_1ST_OBJECT && bObjectVerb)
			{
				CString str=psv.at(dwStartOfNextSentenceElement).m_Str;
#ifdef _DEBUG
				printf("!bObjectVerb || bValidManner || bValidByAgent || bValidPlace\n");
#endif
				if(!(dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
					str.MakeLower();
				if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(str)) )
				{
					#ifdef _DEBUG
					printf("Adverb\n");
					#endif
					EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
					if(ea)
						if(ea->m_bType==5) // Adverb der Zeit ('today')
						{
							dwStartOfNextSentenceElement++;
							subsentence.m_bAdverbOfTimeAtTheBeginning=FALSE;
							#ifdef _DEBUG
							printf("Adverb der Zeitt\n");
							printf("psv.size: %u dwStartOfNextSentenceElement: %u\n",
								psv.size(),dwStartOfNextSentenceElement);
							#endif
							subsentence.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=
								pWordNode;
							bValidTime=TRUE;
							if(!dwStartOfFirstSentenceElementAfterMiddle)
								dwStartOfFirstSentenceElementAfterMiddle=
									dwStartOfNextSentenceElement;
						}
				}
			}
			else
			{
#ifdef _DEBUG
				printf("!(!bObjectVerb || bValidManner || bValidByAgent || bValidPlace)\n");
#endif
        //If a transitive verb, first parse for structures that may be RIGHT of an object
        //because an object may be a proper name.
				if(bObjectVerb)
				{
#ifdef _DEBUG
					printf("bObjectVerb\n");
#endif
					for(DWORD dwRun=dwStartOfNextSentenceElement;dwRun<psv.size() 
						&& g_bContinue;dwRun++)
					{
						CString str=psv.at(dwRun).m_Str;
#ifdef _DEBUG
						printf("str: %s\n",str);
#endif
						if(!(dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
							str.MakeLower();
						if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(str)) )
						{
							#ifdef _DEBUG
							printf("Adverb\n");
							#endif
							EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
							if(ea)
								if(ea->m_bType==ADVERB_OF_TIME) // Adverb der Zeit ('today')
								{
									dwStartOfNextSentenceElement=dwRun+1;
									subsentence.m_bAdverbOfTimeAtTheBeginning=FALSE;
									dwEndOfObject=dwRun-1;
									bValidTime=TRUE;
									#ifdef _DEBUG
									printf("Adverb der Zeit: %s\n",ea->m_strWord);
									printf("psv.size: %u dwStartOfNextSentenceElement: %u\n",psv.size(),dwStartOfNextSentenceElement);
									#endif
									subsentence.
										m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=
										pWordNode;
									if(!dwStartOfFirstSentenceElementAfterMiddle)
										dwStartOfFirstSentenceElementAfterMiddle=dwRun;
									break; // wichtig, sonst wird dwStartOfNextSentenceElement mehrmals
									// inkrementiert
								}
						}
					}//for-loop
				}
			}
		}
#ifdef _DEBUG
		printf("bObjectVerb: %d bValidManner: %d gv->words[0]: %s \
dwStartOfNextSentenceElement: %u psv.size(): %u\n",bObjectVerb,
bValidManner,gv->m_strWords[0],dwStartOfNextSentenceElement,psv.size());
#endif
   LOG("19.05.2008 20.42.30\n");
		//if((bObjectVerb || (!bValidManner && gv->words[0]==_T("sein"))) && 
		//	dwStartOfNextSentenceElement<psv.size())
		//wenn das Verb ein Objekt ermöglicht und nicht das Hilfsverb "sein" 
		//ist oder es keine Umstandsbestimmung der Art und Weise gibt und
		//das Verb das Hilfsverb "sein" ist
		if((bObjectVerb && gv->m_strWords[0]!=_T("sein")) || (!bValidManner && 
			gv->m_strWords[0]==_T("sein")) && dwEndOfValidMiddle+1!=
			dwStartOfFirstSentenceElementAfterMiddle)
		{ // nur wenn das Prädikat ein Objekt verlangt, wenn
			// das Prädikät vom Verb "sein" ist, dann darf nicht bereits
			// eine Umstandsbestimmung der Art und Weise (!bValidManner, Manner=
			// deutsch: "Art und Weise") existieren,.
			//und dem Ende des "Mittelsteils" nicht direkt ein Satzglied folgt
			//zum Beispiel: "The number is incomplete."
			//                          ++ +--------+
			//													|			 |
			//                     Prädikat  Umstandsbestimmung der Art und Weise
			//               ("Mittelteil")  (Satzglied)
			//in diesem Beispiel folgt dem "Mittelteil" direkt ein Satzglied, es
			//wird also nicht die Gültigkeit (grammatikalische Überprüfung) eines 
			//Objektes durchgeführt
#ifdef _DEBUG
			printf("(bObjectVerb || (!bValidManner && gv->words[0]==_T(\"sein\"))) && \
dwStartOfNextSentenceElement<psv.size()\n");
#endif
			if(!dwEndOfObject)
				dwEndOfObject=psv.size()-1;
			#ifdef _DEBUG
			printf("dwEndOfValidMiddle+1: %u psv.size(): %u dwEndOfObject:  %u \
subsentence.m_vecPredicate.size(): %u\n",dwEndOfValidMiddle+1,psv.size(),
				dwEndOfObject,subsentence.m_vecPredicate.size());
			#endif
			if(dwEndOfValidMiddle+1<psv.size() && dwEndOfObject>=0 && 
				subsentence.m_vecPredicate.size()>0 && 
				(dwEndOfValidMiddle+1<=dwEndOfObject))
			{
				#ifdef _DEBUG
				printf("dwStartOfNextSentenceElement<sv.size() && \
(subsentence.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated\n || \
subsentence.m_pVocNodeWhereAnEnglishVerbIsEncapsulated)\n");
				#endif
				if(gv)
				{
					#ifdef _DEBUG
					printf("gv gv->m_strWords[0]: %s gv->Case: %u\n",gv->m_strWords[0],gv->m_bCase);
					if(subsentence.m_bActiveClause)
						printf("subsentence.m_bActiveClause\n");
					else
						printf("!subsentence.m_bActiveClause\n");
					#endif
					BOOL bValidObject=FALSE;
          LOG("23.05.2008 11.15.43\n");//AC=Active Clause
					if(subsentence.m_bActiveClause)
					{
            //Wenn das Prädikat den 1. , 3. oder 4. Fall verlangt.
							// gv->Case==1  : 3. Fall
							// gv->Case==2  : 4. Fall
							// gv->Case==4  : 1. Fall
						if(
              (gv->m_bCase==DATIVE || gv->m_bCase==ACCUSATIVE || gv->m_bCase==NOMINATIVE)
              //Wenn dieser Teilsatz ein zu parsender Relativ-Satz ist, dann
              //bezieht sich dieser auf ein Satzteil, z.B. "That man you hate."
              //                                                     \      /
              //                                                   Relativsatz
              && !(dwFlags&LEAVE_OUT_1ST_OBJECT)
              //Wenn dieser Teilsatz ein zu parsender Relativ-Satz ist, der ein Verb,
              //das 2 Objekte verlangt, enthält, dann soll nur für 1 Objekt des
              //Relativsatzes geparst werden, denn auf das 1te bezieht sich der 
              //Relativsatz ja: z.B. "That man you give a book."
              //                               \             /
              //                                -Relativsatz-
              || gv->m_bCase==DATIVE_AND_ACCUSATIVE && dwFlags&LEAVE_OUT_1ST_OBJECT
              )
						{ 
							BYTE bReturnOfParseObject=0;
							DWORD dwFlagsParameter=dwFlags;
							#ifdef _DEBUG
							printf("gv->m_bCase==1 || gv->m_bCase==2 || gv->m_bCase==4\n");
							printf("returnValue: %u\n",returnValue);
							#endif
							std::vector<Range> vecRange;
							Object object;
							if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
								//31tes Bit setzen
								dwFlagsParameter|=1073741824;
							if((bReturnOfParseObject=ParseObject(object,GetBetween(psv,
								dwEndOfValidMiddle+1,dwEndOfObject),TRUE,vecRange,
								dwFlagsParameter))==0)
							{
							#ifdef _DEBUG
								printf("BOOL ParseClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange) - return 0\n");
							#endif
								return 0;
							}
							if(bReturnOfParseObject<returnValue)
								returnValue=bReturnOfParseObject;
							if(subsentence.m_pObjectFirst)
								delete subsentence.m_pObjectFirst;
							subsentence.m_pObjectFirst=new Object(object);
							for(dwRun=0;dwRun<vecRange.size();dwRun++)
								rVecRange.push_back(vecRange.at(dwRun));
							if(!bValidManner && !bValidByAgent && !bValidPlace && !bValidTime)
								dwStartOfNextSentenceElement=dwEndOfObject+1;
						}
            LOG("19.05.2008 20.45\n");
						if(gv->m_bCase==DATIVE_AND_ACCUSATIVE 
              //Wenn dieser Teilsatz ein zu parsender Relativ-Satz ist, dann
              //bezieht sich dieser auf ein Satzteil, z.B. "That man you gave a book."
              //                                                     \             /
              //                                                      -Relativsatz-
              && !(dwFlags&LEAVE_OUT_1ST_OBJECT) )
						{ // wenn das Prädikat 2 Fälle (3. und 4. Fall) verlangt
							std::vector<Range> vecRange;
							std::vector<Range> vecRangeOfValidObject;
							DWORD dwEndOfValidObject=0;
							for(DWORD dwRun=dwEndOfValidMiddle+1;dwRun<dwEndOfObject+1 && 
								g_bContinue;dwRun++)
							{
								DWORD dwFlagsParameter=dwFlags;
								vecRange.clear();
								Object object;
								if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
									//31tes Bit setzen
									dwFlagsParameter|=1073741824;
								if(ParseObject(object,GetBetween(psv,dwEndOfValidMiddle+1,
									dwRun),TRUE,vecRange,dwFlagsParameter)==2)
								{
									dwEndOfValidObject=dwRun;
									//dwEndOfValidSentenceElement=dwRun;
									bValidObject=TRUE;
									if(subsentence.m_pObjectFirst)
										delete subsentence.m_pObjectFirst;
									subsentence.m_pObjectFirst=new Object(object);
									vecRangeOfValidObject.clear();
									vecRangeOfValidObject=vecRange;
								}
							}
							if(bValidObject)
							{
								dwStartOfNextSentenceElement=dwEndOfValidObject+1;
								dwEndOfValidSentenceElement=dwEndOfValidObject;
								for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
									rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
							}
						}
					}//If active clause.
          LOG("23.05.2008 11.15.55\n");//AAC=After Active Clause
					if(subsentence.m_bActiveClause && gv->m_bCase==DATIVE_AND_ACCUSATIVE
            //Wenn dieser Teilsatz ein zu parsender Relativ-Satz ist, dann
            //bezieht sich dieser auf ein Satzteil, z.B. "That man you gave a book."
            //                                                     \             /
            //                                                      -Relativsatz-
            && !(dwFlags&LEAVE_OUT_1ST_OBJECT)
            )
					{ // wenn ein Verb, das 2 Fälle verlangt
						#ifdef _DEBUG
						printf("gv->m_bCase==3\n");
						#endif
						DWORD dwEndOfValidObject=0;
						if(bValidObject)
						{
							DWORD dwFlagsParameter=dwFlags;
							std::vector<Range> vecRange;
							Object object;
							if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
								//31tes Bit setzen
								dwFlagsParameter|=1073741824;
							ParseObject(object,GetBetween(psv,dwStartOfNextSentenceElement,
								dwEndOfObject),TRUE,vecRange,dwFlagsParameter);
							bValidObject=TRUE;
							if(subsentence.m_pObjectSecond)
								delete subsentence.m_pObjectSecond;
							subsentence.m_pObjectSecond=new Object(object);
							for(dwRun=0;dwRun<vecRange.size();dwRun++)
								rVecRange.push_back(vecRange.at(dwRun));
							if(!bValidManner && !bValidByAgent && !bValidPlace && !bValidTime)
								dwStartOfNextSentenceElement=dwEndOfObject+1;
						}//if(bValidObject)
						else
						{
							//bValidObject=FALSE;
							std::vector<Range> vecRange;
							std::vector<Range> vecRangeOfValidObject;
							DWORD dwStartOfValidObject=0;
							for(DWORD dwRun=dwEndOfObject;dwRun>dwEndOfValidMiddle && g_bContinue;dwRun--)
							{
								DWORD dwFlagsParameter=dwFlags;
								vecRange.clear();
								Object object;
								if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
									//31tes Bit setzen
									dwFlagsParameter|=1073741824;
								if(ParseObject(object,GetBetween(psv,dwRun,dwEndOfObject),TRUE,
									vecRange,dwFlagsParameter)==2)
								{
									dwStartOfValidObject=dwRun;
									//dwEndOfValidSentenceElement=dwRun;
									bValidObject=TRUE;
									if(subsentence.m_pObjectSecond)
										delete subsentence.m_pObjectSecond;
									subsentence.m_pObjectSecond=new Object;
									*subsentence.m_pObjectSecond=object;
									//subsentence.m_pObject=object;
									vecRangeOfValidObject.clear();
									vecRangeOfValidObject=vecRange;
								}
							}
							for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
								rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
							Object object;
							if(bValidObject)
							{
								DWORD dwFlagsParameter=dwFlags;
								vecRange.clear();
								if(PredicateAllowsGerund(subsentence.m_vecPredicate.at(0)))
									//31tes Bit setzen
									dwFlagsParameter|=1073741824;
								ParseObject(object,GetBetween(psv,dwStartOfNextSentenceElement,
									dwStartOfValidObject-1),TRUE,vecRange,dwFlagsParameter);
								if(subsentence.m_pObjectFirst)
									delete subsentence.m_pObjectFirst;
								subsentence.m_pObjectFirst=new Object(object);
								for(dwRun=0;dwRun<vecRange.size();dwRun++)
									rVecRange.push_back(vecRange.at(dwRun));
								if(!bValidManner && !bValidByAgent && !bValidPlace && !bValidTime)
									dwStartOfNextSentenceElement=dwEndOfObject+1;
							}
						}//bValidObject==false
          //If active clause and German predicate/verb demands a 
          //3rd case object and a 4th case object and 2 objects to parse for.
					}
          LOG("23.05.2008 11.16.00\n");//AAC3=After Active Clause and 3rd case
					//Passivsatz und 3.? Fall
          //Z.B. I was given a book.
					if(!subsentence.m_bActiveClause && gv->m_bCase==DATIVE_AND_ACCUSATIVE
            //Wenn dieser Teilsatz ein PASSIVER zu parsender Relativ-Satz ist, dann
            //darf er kein Objekt beinhalten, z.B. "The man I was given."
            //                                              \         /
            //                                             -Relativsatz-
            && !(dwFlags&LEAVE_OUT_1ST_OBJECT)
            )
					{
						#ifdef _DEBUG
						printf("!subsentence.m_bActiveClause && gv->m_bCase==3\n");
						#endif
						Subject subject;
						std::vector<Range> vecRange;
						ParseSubject(subject,GetBetween(psv,dwEndOfValidMiddle+1,
							dwEndOfObject),FALSE,vecRange,dwFlags,
							bContainsFirstTokenOfSentence);
						//dwEndOfValidSentenceElement=dwRun;
						if(subsentence.m_pPersonalSubject)
							delete subsentence.m_pPersonalSubject;
						subsentence.m_pPersonalSubject=new Subject;
						*subsentence.m_pPersonalSubject=subject;
						for(dwRun=0;dwRun<vecRange.size();dwRun++)
							rVecRange.push_back(vecRange.at(dwRun));
						if(!bValidManner && !bValidByAgent && !bValidPlace && !bValidTime)
							dwStartOfNextSentenceElement=dwEndOfObject+1;
					}
          LOG("23.05.2008 11.6.07\n");//ANAC3=After Not Active Clause and 3rd case
				}//if(gv)
				#ifdef _DEBUG
				else
				{
					printf("\n\n\nelse-zweig\n");
					if(!subsentence.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated->
						m_pWordNodeNext)
						printf("!subsentence.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated->\
m_pWordNodeNext\n");
					if(!subsentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext)
						printf("!subsentence.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->\
m_pWordNodeNext\n");
				}
				#endif
			}//If at least 1 token remaining,object,>=1 predicate
			else
			{
				if(subsentence.m_vecPredicate.size()==1)
				{
					Predicate predicate=subsentence.m_vecPredicate.at(0);
					if(predicate.m_bActiveClause)
					{
						returnValue=0;
#ifdef _DEBUG
						printf("bObjectVerb und kein Objekt!\n");
#endif
					}
				}
			}
		}//Objektverb,<>"sein" v ...
    LOG("23.05.2008 11.16.15\n");//AOV=After ObjectVerb
		if(dwStartOfNextSentenceElement==psv.size())
		{
			#ifdef _DEBUG
			printf("rVecRange.size(): %u\n",rVecRange.size());
			printf("dwStartOfNextSentenceElement==psv.size()\n");
			printf("BOOL ParseClause(Clause & subsentence,const \
PositionCStringVector & sv) - return returnValue (%u)\n",returnValue);
			#endif
      LOG("23.05.2008 11.16.24\n");//NMT=no more token
			return returnValue;
		}
		#ifdef _DEBUG
		else
		{
			printf("dwStartOfNextSentenceElement!=psv.size()\n");
		}
		#endif
	}//if(bValidMiddle || bValidImperativ)
	TRACE("ParseClause ENDE - return FALSE\n");
	#ifdef _DEBUG
	printf("rVecRange.size(): %u\n",rVecRange.size());
	printf("BYTE ParseClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
dwFlags,BYTE bContainsFirstTokenOfSentence - return FALSE\n");
	#endif
  LOG("23.05.2008 11.16.31\n");//PCE=ParseClause End
	return FALSE;
}//Clause::ParseClauseWithoutSubjectAndObject

BYTE ParseDefiniteArticleNoun(EnglishNounRecord & enr,CStringVector & sv,
	const DWORD & dwFlags,BYTE bContainsFirstTokenOfSentence)
{
	TRACE("BYTE ParseDefiniteArticleNoun(EnglishNounRecord & enr,CStringVector sv) - ANFANG\n");
	#ifdef _DEBUG
	printf("BYTE ParseDefiniteArticleNoun(EnglishNounRecord & enr,CStringVector sv) - ANFANG\n");
	printf("lfghm\n");
	for(DWORD i2=0;i2<sv.size();i2++)
	{
		printf("sv.at(i2): %s\n",sv.at(i2).GetBuffer(sv.at(i2).GetLength()));	
	}
	#endif
	if(sv.size()>1)
	{
		CString str=sv.at(0);
		if(!(dwFlags&CASE_SENSITIVE)==1)
			str.MakeLower();
		else if(bContainsFirstTokenOfSentence)
		{
			MakeFirstLetterLower(str);
		}
		if(str==_T("the"))
		{
			CStringVector vecCStr=GetBetween(sv,1,sv.size());
			//if(!(dwFlags&CASE_SENSITIVE==1))
			if(!(dwFlags&CASE_SENSITIVE))
				for(DWORD dwRun=0;dwRun<vecCStr.size();dwRun++)
					vecCStr.at(dwRun).MakeLower();
			std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> ptvwaenie=
				IfIsEnglishNounThenReturnEnglishNounEncapsulatedInWord(vecCStr);
			if(ptvwaenie.size()>0)
			{
				enr.bDefiniteArticle=TRUE;
				enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated=ptvwaenie;
				if(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
					if(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).m_bSingular)
					{
#ifdef _DEBUG
						printf("BYTE ParseDefiniteArticleNoun(EnglishNounRecord & enr,CStringVector sv) - return 3\n");
#endif
						return 3;
					}
					else
					{
#ifdef _DEBUG
						printf("BYTE ParseDefiniteArticleNoun(EnglishNounRecord & enr,CStringVector sv) - return 3\n");
#endif
						return 6;
					}
			}
/*			if(IsEnglishSingular(GetBetween(sv,1,sv.size())))
			{
				Append(enr.m_Noun,GetBetween(sv,1,sv.size()));
				enr.bDefiniteArticle=TRUE;
				TRACE("BYTE ParseDefiniteArticleNoun(EnglishNounRecord & enr,CStringVector sv) - ENDE return 3\n");
				printf("BYTE ParseDefiniteArticleNoun(EnglishNounRecord & enr,CStringVector sv) - ENDE return 3\n");
				return 3;
			}*/
		}
	}
	//TRACE("personIndex: %u\n",personIndex);
	TRACE("BYTE ParseDefiniteArticleNoun(EnglishNounRecord & enr,CStringVector sv) - ENDE return 0\n");
	#ifdef _DEBUG
	printf("BYTE ParseDefiniteArticleNoun(EnglishNounRecord & enr,CStringVector sv) - ENDE return 0\n");
	#endif
	return 0;
}

BOOL ParseArticleObject(Object & object,const CStringVector & sv)
{
	TRACE("BOOL ParseArticleObject(Object & object,const CStringVector & sv) - ANFANG\n");
	//in MFC/Windows the TRACE macro has a variable # of args--I do 
	//not know how to enable variable numbers of args for the same macro name.
	//So I exclude it from non-Windows.
#ifdef _WINDOWS
	//Without this in non-DEBUG compile the loop also would consume time.
	#ifdef _DEBUG
		for(DWORD i=0;i<sv.size();i++)
			TRACE("sv.at(%d): %s\n",i,sv.at(i));
	#endif
#endif
	
	DWORD dwStart=0;
	for(DWORD dwRun=0;dwRun<sv.size();dwRun++)
	{
		CString token=sv.at(dwRun);
		if(token==_T("and"))
		{
			CStringVector svEE=GetBetween(sv,dwStart,dwRun);
			EnumerationElement ee;
			if(!ParseArticleObjectEnumerationElement(ee,svEE))
			{
				TRACE("BOOL ParseArticleObject(Object & object,const CStringVector & sv) - ENDE - return FALSE\n");
				return FALSE;
			}
			object.m_vecEnumerationElement.push_back(ee);
			dwStart=dwRun+1;
		}
		if(dwRun==sv.size()-1)
		{
			CStringVector svEE=GetBetween(sv,dwStart,dwRun);
			EnumerationElement ee;
			if(!ParseArticleObjectEnumerationElement(ee,svEE))
			{
				TRACE("BOOL ParseArticleObject(Object & object,const CStringVector & sv) - ENDE - return FALSE\n");
				return FALSE;
			}
			object.m_vecEnumerationElement.push_back(ee);
		}
	}
	TRACE("BOOL ParseArticleObject(Object & object,const CStringVector & sv) - ENDE - return TRUE\n");
	return TRUE;
}

BOOL ParseArticleObjectEnumerationElement(EnumerationElement & ee,CStringVector sv)
{
	TRACE("BOOL ParseArticleObjectEnumerationElement(EnumerationElement & ee,CStringVector sv) - ANFANG\n");
	if(sv.size()>1)
	{
		if(!(sv.at(0)==_T("the")))
			return FALSE;
		//in MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
#ifdef _WINDOWS
	//Without this in non-DEBUG compile the loop also would consume time.
	#ifdef _DEBUG 
		for(DWORD i=0;i<sv.size();i++)
			TRACE("sv.at(%d): %s\n",i,sv.at(i));
	#endif
#endif
		CStringVector svNoun;
		DWORD dwStart=0;
		for(DWORD dwRun=sv.size()-1;dwRun>0;dwRun--)
		{
			svNoun=GetBetween(sv,dwRun,sv.size()-1);
			if(IsEnglishSingular(svNoun) || IsEnglishPlural(svNoun))
				dwStart=dwRun;
		}
		if(dwStart)
		{
			Append(ee.m_Noun,svNoun);
			if(dwStart-1>0)
			{
				for(DWORD dwRun=dwStart-1;dwRun>1;dwRun--)
				{
					CString token=sv.at(dwRun);
					if(IsEnglishAdjective(token))
						ee.m_Adjectives.push_back(token);
					else
					{
						TRACE("BOOL ParseArticleObjectEnumerationElement(EnumerationElement & ee,CStringVector sv) - ENDE - return FALSE\n");
						return FALSE;
					}
				}
			}
			ee.m_bDefiniteArticle=TRUE;
			TRACE("BOOL ParseArticleObjectEnumerationElement(EnumerationElement & ee,CStringVector sv) - ENDE - return TRUE\n");
			return TRUE;
		}
	}
	TRACE("BOOL ParseArticleObjectEnumerationElement(EnumerationElement & ee,CStringVector sv) - ENDE - return FALSE\n");
	return FALSE;
}

//diese Funktion wird von ParseClause, ParseQuestionClause und 
//ParseRelativeClause aufgerufen (bessere Wartbarkeit)
//und liefert den Personalpronomen zurück, zu dem das gebeugte Verb gehört
//der Rückgabewert ist ein Byte, bei dem die Bits von 0 bis 6 angeben, für 
//welchen Personenindex das Prädikat korrekt ist
//Beispiel: "I like you." -> like passt für die folgenden Personenindizes:
//1 ("I like you.") ,2 ("You like me."), 4 ("We like you."), 5 ("You like you."),
//und 6 ("They like you.")
BYTE ParseBendPartOfPredicate(Predicate & predicate,BOOL bBe,const 
	CStringVector & sv,
	BYTE personIndex,const DWORD & dwFlags)
{
	//anfangs falsch, dann in den passenden Verzweigungen, falls
	//das Token dort gültig ist, auf "true" gesetzt;bei restlichen
	//Verzweigungen bleibt der Wert "false"
	//Um nur 1 "return" zu haben (am Ende der Funktion)
	bool bValid=false;
	bool bNonModalAuxiliaryExclusiveWill=false;
	bool bGetBitmaskForFlectiveParadigmas=false;
  bool bAdverbOfFrequencyFound=false;

	BYTE returnPersonIndex=0;
	CStringVector strToken;
	TRACE("BYTE ParseBendPartOfPredicate(Predicate & "
		"predicate,BOOL bBe,const CStringVector & sv,"
		"BYTE personIndex,const DWORD & dwFlags) ANFANG\n");
  LOG2N("23.05.2008 10.40.32","ParseBendPartOfPredicate");//=ParseBendPartOfPredicate
	#ifdef _DEBUG
		printf("BYTE ParseBendPartOfPredicate(Predicate & "
			"predicate,BOOL bBe,const CStringVector & sv,"
			"BYTE personIndex,const DWORD & dwFlags) ANFANG\n");
	#endif
	//ein WordNode-Zeiger, der ein englisches Verb, dessen deutsches Pendant
	//eine gebeugte Verbform ist, die für eine korrekte Übersetzung notwendig 
	//ist ("have" bei "I have been." ist zum Beispiel NICHT notwendig, da 
	//man durch die Zeitform "Perfekt" weiss, dass man eine gebeugte Verbform 
	//von "haben" im Deutschen dafür braucht), kapselt
	WordNode * pWordNodeWhereTheGermanTranslationIsABendVerb=NULL;
	//BYTE bEmpathicDo=FALSE;
	bool bNegation=false;
	BYTE byTense=0;
	//am Anfang aus den höchstmöglichen Wert setzen
	DWORD dwPositionOfBendAuxiliary=ULONG_MAX;
	WORD wFiniteVerbFormsAffectedbySearchString;

#ifdef _DEBUG
//	printf("ParseMiddle(Predicate & predicate,BOOL bBe,const CStringVector & sv,\
//BYTE personIndex,const DWORD & dwFlags) - ANFANG\n");
	printf("predicate.m_bTense: %u\n",predicate.m_bTense);
	fflush(stdout);
#endif
	//CString token;
	if(!(sv.size()>0))
	{
    LOG2N("23.05.2008 10.40.58","ParseBendPartOfPredicate 0 Token Return");//=ParseBendPartOfPredicate 0 Token Return
		TRACE("ParseMiddle ENDE - return FALSE\n");
#ifdef _DEBUG
		printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ENDE - return FALSE - sv.size()==0\n");
#endif
		return FALSE;
	}
	#ifdef _DEBUG
	{
		for(DWORD i=0;i<sv.size();i++)
		{
			CString token=sv.at(i);
			TRACE("sv.at(%d): %s\n",i,token);
			printf("sv.at(%d): %s\n",i,token);
		}
	}
	#endif
	//predicate.m_adverbs.clear();
	//for(int i=0;i<ss.m_adverbs.size();i++
	BOOL bAuxiliaryVerbOccured=FALSE;
	for(DWORD dwRun=0;dwRun<sv.size() && g_bContinue;++dwRun)
	{
		CString token=sv.at(dwRun);
		if(!(dwFlags&CASE_SENSITIVE)==1)
			token.MakeLower();
		//in MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
	#ifdef _WINDOWS
		TRACE("token: %s\n",token);
#endif
		#ifdef _DEBUG
		printf("token: %s\n",token);
		#endif
		WordNode * pWordNode=NULL;
		//wichtig!:für jeden Schleifendurchlauf erneut setzen
		bValid=false;
	//ein WordNode-Zeiger, der ein englisches Verb, dessen deutsches Pendant
	//eine gebeugte Verbform ist, die für eine korrekte Übersetzung notwendig 
	//ist ("have" bei "I have been." ist zum Beispiel NICHT notwendig, da 
	//man durch die Zeitform "Perfekt" weiss, dass man eine gebeugte Verbform 
	//von "haben" im Deutschen dafür braucht), kapselt
		if(pWordNodeWhereTheGermanTranslationIsABendVerb==NULL)
		{
			if((pWordNode=
				//GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(token)))
				GetPointerToWordNodeWhereABendEnglishVerbIsEncapsulated(
				token,wFiniteVerbFormsAffectedbySearchString)
				))
			{
				BOOL bAuxiliaryVerb=FALSE;
				TRACE("BendVerb found\n");
			#ifdef _DEBUG
				printf("pWordNode!=NULL\n");
			#endif
				if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))
				{
					//BOOL bValid=FALSE;
			#ifdef _DEBUG
					printf("EnglishAuxiliaryVerb\n");
			#endif
					strToken.push_back(token);
					bAuxiliaryVerb=TRUE;
					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
						(pWordNode->m_pWord);
					//wenn zu wenig Speicherplatz zur Verfügung stand, kann es sein,
					//dass kein Speicher für eav alloziert wurde und eav "NULL" ist
					if(eav)
					{
						//wenn modales Hilfsverb (zum beispiel "can", "may"):
						//modale Hilfsverbeb können nur im Präsens vorkommen
						if(!eav->m_bIntegral)
						{
							byTense=SIMPLE_PRESENT;
							bValid=true;
							//dem WordNode-Zeiger, der ein englisches Verb, 
							//dessen deutsches Pendant
							//eine gebeugte Verbform ist, die für eine 
							//korrekte Übersetzung notwendig 
							//ist ("have" bei "I have been." ist zum Beispiel 
							//NICHT notwendig, da 
							//man durch die Zeitform "Perfekt" weiss, dass 
							//man eine gebeugte Verbform 
							//von "haben" im Deutschen dafür braucht), 
							//kapselt, die Adresse des Zeigers "pWordNode" 
							//zuweisen
							pWordNodeWhereTheGermanTranslationIsABendVerb=
								pWordNode;
							//alle Flexions-Paradigmen dieses Verbes sind gleich
							returnPersonIndex=126;
						}
						else
						{
							if(eav->m_strWords[0]==_T("will"))
							{
								byTense=FUTURE;
								bValid=true;
								//dem WordNode-Zeiger, der ein englisches Verb, 
								//dessen deutsches Pendant
								//eine gebeugte Verbform ist, die für eine 
								//korrekte Übersetzung notwendig 
								//ist ("have" bei "I have been." ist zum Beispiel 
								//NICHT notwendig, da 
								//man durch die Zeitform "Perfekt" weiss, dass 
								//man eine gebeugte Verbform 
								//von "haben" im Deutschen dafür braucht), 
								//kapselt, die Adresse des Zeigers "pWordNode" 
								//zuweisen
								pWordNodeWhereTheGermanTranslationIsABendVerb=
									pWordNode;
								//alle Flexions-Paradigmen dieses Verbes sind gleich
								returnPersonIndex=126;
							}
							if(eav->m_strWords[0]==_T("be") || 
								eav->m_strWords[0]==_T("have"))
							{
								//bNonModalAuxiliaryExclusiveWill=true;
								bGetBitmaskForFlectiveParadigmas=true;
								bValid=true;
								//dem WordNode-Zeiger, der ein englisches Verb, 
								//dessen deutsches Pendant
								//eine gebeugte Verbform ist, die für eine 
								//korrekte Übersetzung notwendig 
								//ist ("have" bei "I have been." ist zum Beispiel 
								//NICHT notwendig, da 
								//man durch die Zeitform "Perfekt" weiss, dass 
								//man eine gebeugte Verbform 
								//von "haben" im Deutschen dafür braucht), 
								//kapselt, die Adresse des Zeigers "pWordNode" 
								//zuweisen
								pWordNodeWhereTheGermanTranslationIsABendVerb=
									pWordNode;
							}
							if(eav->m_strWords[0]==_T("do"))
							{
								//bNonModalAuxiliaryExclusiveWill=true;
								bGetBitmaskForFlectiveParadigmas=true;
								bValid=true;
								//dem WordNode-Zeiger, der ein englisches Verb, 
								//dessen deutsches Pendant
								//eine gebeugte Verbform ist, die für eine 
								//korrekte Übersetzung notwendig 
								//ist ("have" bei "I have been." ist zum Beispiel 
								//NICHT notwendig, da 
								//man durch die Zeitform "Perfekt" weiss, dass 
								//man eine gebeugte Verbform 
								//von "haben" im Deutschen dafür braucht), 
								//kapselt, die Adresse des Zeigers "pWordNode" 
								//zuweisen
								pWordNodeWhereTheGermanTranslationIsABendVerb=
									pWordNode;
							}
						}
					}
				}//if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))
				//wenn das gefundene Verb KEIN Hilfsverb ist (es 
				//bleibt nur noch Vollverb als Möglichkeit übrig)
				else//if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb)
				{
					bValid=true;
					//dem WordNode-Zeiger, der ein englisches Verb, 
					//dessen deutsches Pendant
					//eine gebeugte Verbform ist, die für eine 
					//korrekte Übersetzung notwendig 
					//ist ("have" bei "I have been." ist zum Beispiel 
					//NICHT notwendig, da 
					//man durch die Zeitform "Perfekt" weiss, dass 
					//man eine gebeugte Verbform 
					//von "haben" im Deutschen dafür braucht), 
					//kapselt, die Adresse des Zeigers "pWordNode" 
					//zuweisen
					pWordNodeWhereTheGermanTranslationIsABendVerb=
						pWordNode;
					//bNonModalAuxiliaryExclusiveWill=true;
					bGetBitmaskForFlectiveParadigmas=true;
				}
			}
			else//englisches Verb identisch dem Token gefunden
				if((pWordNode=
					GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(token))
					)
				{
					EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>
						(pWordNode->m_pWord);
					if(ea)
					{
						//wenn sich das Adverb zwischen Subjekt und Verb befinden darf
						//oder nach der finiten Form des Verbes 'to be' oder nach dem
						//ersten Hilfsverb wie z.B. Adverbien 
						//der Häufigkeit (always,never,often)
						if(ea->m_bType==BEFORE_FULL_VERB_OR_AFTER_FINITE_AUXILIARY_VERB && 
							//wenn noch kein Adverb, das sich zwischen Subjekt und Verb 
							//befinden darf
							//oder nach der finiten Form des Verbes 'to be' oder nach dem
							//ersten Hilfsverb wie z.B. Adverbien 
							//der Häufigkeit (always,never,often), gefunden wurde
							!bAdverbOfFrequencyFound)
						{
								//if(typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->
								//	m_pWord)==typeid(EnglishAuxiliaryVerb))
								//{
							bValid=true;
							predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated=
								pWordNode;
							bAdverbOfFrequencyFound=TRUE;
						}
						else if(
							//wenn ein Adverb, das ein Verb beschreibt
							ea->m_bType==1 && !predicate.
							m_pWordNodeWhereAnEnglishAdverbDescribingAVerbIsEncapsulated)
						{
							bValid=true;
						}
					}
				}
		}//if(pWordNodeWhereTheGermanTranslationIsABendVerb==NULL)
		else
			//pWordNodeWhereTheGermanTranslationIsABendVerb!=NULL
		{
			// wenn Negation (Verneinung)
			//Beispiel: "Do not you like me?"
      //"Haven't you seen me?": 4.880 google-Treffer
      //"Have you not seen me?": 12.500 google-Treffer
      //"Have not you seen me?: 1 google-Treffer
      //->Indiz, dass bei nicht-abgekürztem "not" "Have you not seen me?" richtig ist.
			//           \    /
			//durch diese Funktion überprüft
			if(token==_T("not") && 
				//wenn nicht schon bereits eine Negation vorliegt
				!bNegation)
			{
				bNegation=true;
				bValid=true;
			}
		}
		if(!bValid)
		{
			//0=Fehler: für Rückgabe benötigt
			returnPersonIndex=0;
			break;
		}
	}
	if(bValid)
	{
		if(//bNonModalAuxiliaryExclusiveWill
			bGetBitmaskForFlectiveParadigmas
			)
		{
			for(BYTE byRun=1;byRun<13;++byRun)
				//wenn das Bit an byRun-ter Stelle (vom niederwertigsten Bit an)
				//gesetzt ist
				if((wFiniteVerbFormsAffectedbySearchString>>byRun)&1)
					if(byRun<7)
					{
						returnPersonIndex|=//(BYTE)pow(2,b);
							(1<<byRun);
						byTense=SIMPLE_PRESENT;
					}
					else
					{
						returnPersonIndex|=//(BYTE)pow(2,b);
							//"-8":1.Person Singular im 2.-niederwertigsten Bit
							(1<<(byRun-6));
						if(byTense==SIMPLE_PRESENT)
							byTense=SIMPLE_PRESENT_OR_SIMPLE_PAST;
						else
							byTense=SIMPLE_PAST;
					}
		}
		//wenn das gefundene Verb das Hilfsverb "will" ist
		//else
		//	returnPersonIndex=118;

		//nur übernehmen, wenn das Prädikat gültig ist!
		predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb=
			pWordNodeWhereTheGermanTranslationIsABendVerb;
		predicate.m_bTense=byTense;
		predicate.m_bNegation=bNegation;
	}

#ifdef _DEBUG
		//wenn der Zeiger, der ein englisches Verb, dessen deutsches 
		//Pendant
		//eine gebeugte Verbform ist, die für eine korrekte 
		//Übersetzung notwendig 
		//ist ("have" bei "I have been." ist zum Beispiel NICHT 
		//notwendig, da 
		//man durch die Zeitform "Perfekt" weiss, dass man eine 
		//gebeugte Verbform 
		//von "haben" im Deutschen dafür braucht), kapselt, schon
		//zugewiesen wurde
		if(pWordNodeWhereTheGermanTranslationIsABendVerb)
		{
			printf("pWordNodeWhereTheGermanTranslationIsABendVerb\n");
			if(typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->
				m_pWord)==typeid(EnglishVerb))
			{
				printf("typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->\
m_pWord)==typeid(EnglishVerb)\n");
				EnglishVerb	* ev=dynamic_cast<EnglishVerb	*>
					(pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
				if(ev)
					printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
			}
			if(typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->
				m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				printf("typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->\
m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
				if(eav)
					printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]);
			}
		}
//#ifdef _DEBUG
		else
			printf("!pWordNodeWhereTheGermanTranslationIsABendVerb\n");
//#endif
		printf("returnPersonIndex: %u\n",returnPersonIndex);
		printf("Schleifenende\n");
#endif

	#ifdef _DEBUG
	printf("predicate.m_bTense: %u\n",predicate.m_bTense);
	if(predicate.m_bNegation)
		printf("predicate.m_bNegation\n");
	else
		printf("!predicate.m_bNegation\n");
	printf("returnPersonIndex: %u\n",returnPersonIndex);
	printf("ParseBendPartOfPredicate(Predicate & predicate,BOOL bBe,const CStringVector\
& sv,BYTE personIndex) - ENDE - return returnPersonIndex (%u)\n",
returnPersonIndex);
	#endif
  LOG2N("23.05.2008 10.41.13 "<<(short)returnPersonIndex<<(short)predicate.m_bActiveClause,
    "ParseBendPartOfPredicate Return "<<(short)returnPersonIndex<<(short)predicate.m_bActiveClause);//=ParseBendPartOfPredicate Return
	return returnPersonIndex;
}//BYTE ParseBendPartOfPredicate(Predicate & predicate,BOOL bBe,const CStringVector & sv,
//	BYTE personIndex,const DWORD & dwFlags)

//Parses for a by-agent that can follow a PASSIVE predicate.
BOOL ParseByAgent(ByAgent & byAgent,const PositionCStringVector & psv,const 
	DWORD & dwFlags)
{
	if(psv.size()>0)
	{
		if(psv.at(0).m_Str==_T("by"))
		{
			//if(ParseSubject(byAgent,GetBetween(sv,1,sv.size()))
			//	return TRUE;
			DWORD dwStart=0;
			for(DWORD dwRun=0;dwRun<psv.size();dwRun++)
			{
				CString token=psv.at(dwRun).m_Str;
				if(token==_T(",") || token==_T("and"))
				{
					ByAgentEnumerationElement baee;
					if(!ParseByAgentEnumerationElement(baee,GetBetween(psv,1,psv.size()),TRUE ))
						return FALSE;
					byAgent.m_enumerationElements.push_back(baee);
					dwStart=dwRun+1;
				}
				if(dwRun==psv.size()-1)
				{
					ByAgentEnumerationElement baee;
					if(ParseByAgentEnumerationElement(baee,GetBetween(psv,1,psv.size()),TRUE ))
					{
						byAgent.m_enumerationElements.push_back(baee);
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

BYTE ParseByAgentEnumerationElement(ByAgentEnumerationElement & ee,
	const PositionCStringVector & psv,const DWORD & dwFlags)
{
	TRACE("BYTE ParseByAgentEnumerationElement(ByAgentEnumerationElement & baee,const CStzringVector & sv) ANFANG\n");
	#ifdef _DEBUG
	printf("BYTE ParseByAgentEnumerationElement(ByAgentEnumerationElement & baee,const CStzringVector & sv) ANFANG\n");
	for(DWORD i=0;i<psv.size();i++)
		printf("psv.at(%d).m_Str: %s\n",i,psv.at(i).m_Str);
	#endif
	BYTE personIndex=0;
	if(psv.size()==1)
	{
		CString str=psv.at(0).m_Str;
			if(str=="me")
			{
				ee.m_personalPronoun=1;
				return 1;
			}
			if(str=="you")
			{
				ee.m_personalPronoun=2;
				return 2;
			}
			if(str=="him")
			{
				ee.m_personalPronoun=3;
				return 3;
			}
			if(str=="her")
			{
				ee.m_personalPronoun=4;
				return 3;
			}
			if(str=="it")
			{
				ee.m_personalPronoun=5;
				return 3;
			}
			if(str=="us")
			{
				ee.m_personalPronoun=6;
				return 4;
			}
			if(str=="them")
			{
				ee.m_personalPronoun=7;
				return 6;
			}
	}
	EnglishNounRecord enr;
	if((personIndex=ParsePersonalPronounOrNounStruct(enr,psv,dwFlags,FALSE)))
	{
		ee.m_pEnr=new EnglishNounRecord;
		*ee.m_pEnr=enr;
		#ifdef _DEBUG
		printf("BYTE ParseByAgentEnumerationElement(ByAgentEnumerationElement & baee,const CStzringVector & sv) ENDE !=0\n");
		#endif
		return personIndex;
	}
	BOOL bValidDefiniteArticleNoun=FALSE;
	DWORD dwValidDefiniteArticleNounEnd=0;
	for(DWORD dwRun=0;dwRun<psv.size();dwRun++)
	{
		BYTE ret=0;
		EnglishNounRecord enr;
		CStringVector sv = GetBetweenAsCStringVector(psv,0,dwRun) ;
		if((ret=ParseDefiniteArticleNoun(enr,sv,dwFlags,FALSE)))
		{ // z.B.: The school
			if(ee.m_pEnr)
				delete ee.m_pEnr;
			ee.m_pEnr=new EnglishNounRecord;
			*ee.m_pEnr=enr;
			personIndex=ret;
			bValidDefiniteArticleNoun=TRUE;
			dwValidDefiniteArticleNounEnd=dwRun;
			#ifdef _DEBUG
			printf("ee.m_pEnr->m_Noun.size(): %u\n",ee.m_pEnr->m_Noun.size());
			#endif
		}
	}
	if(bValidDefiniteArticleNoun && psv.size()>dwValidDefiniteArticleNounEnd+1)
	{
		OfPhrase op;
		if(ParseOfPhrase(op,GetBetween(psv,dwValidDefiniteArticleNounEnd+1,
			psv.size()),dwFlags))
		{ // z.B.: of my father
			if(ee.m_pOfPhrase)
				delete ee.m_pOfPhrase;
			ee.m_pOfPhrase=new OfPhrase;
			*ee.m_pOfPhrase=op;
			#ifdef _DEBUG
			printf("op.m_Enr->m_Noun.size(): %u\n",op.m_Enr.m_Noun.size());
			printf("ee.m_pOfPhrase->m_Enr->m_Noun.size(): %u\n",ee.m_pOfPhrase->
				m_Enr.m_Noun.size());
			printf("personIndex: %u\n",personIndex);
			printf("BYTE ParseByAgentEnumerationElement(ByAgentEnumerationElement\
				& baee,const CStzringVector & sv) ENDE !=0\n");
			#endif
			return personIndex;
		}
	}
	TRACE("BYTE ParseByAgentEnumerationElement(ByAgentEnumerationElement & baee,const CStzringVector & sv) ENDE - return TRUE\n");
	#ifdef _DEBUG
	printf("BYTE ParseByAgentEnumerationElement(ByAgentEnumerationElement & baee,const CStzringVector & sv) ENDE return 0\n");
	#endif
	return 0;//personIndex;
}

//1 Funktion, die von mehreren anderen Funktionen aufgerufen
//werden soll, um Wartungsaufwand (bei Änderungen im Quelltext)
//, Quellcodegröße und Binärgröße
//zu verringern;soll von ParseQuestionClause, ParseRelativeClause
//und ParseClause aufegrufen werden
//BYTE ParseClauseRightOfPredicate(Clause & ss,PositionCStringVector & psv,
//  std::vector<Range> & rVecRange)
//{
//		BOOL bValidSubject=FALSE; //gültiges Subjekt?
//    BOOL bValidManner=FALSE;
//		DWORD dwEndOfValidSubject=0;
//		dwStart=dwStartOfNextElement;
//		Subject subject;
//		std::vector<Range> vecRange;
//#ifdef _DEBUG
//		printf("dwStartOfObject: %u\n",dwStartOfObject);
//#endif
//		/*	if(dwStartOfNextElement==psv.size())
//			{
//				ss.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNodeWhereAnEnglishBendVerbIsEncapsulated;
//			}*/
//#ifdef _DEBUG
//		printf("vor: if(sv.size()>dwStartOfNextElement && !bValidObjectOccured)\n");
//#endif
//		dwEndOfObject=psv.size()-1; // falls keine Umstandsbestimmungen existieren, 
//		// geht das Objekt bis zum Ende des Vektors
//		GermanVerb * gv=NULL;
//    WordNode * pWordNodeWhereAnEnglishBendVerbIsEncapsulated=ss.
//      m_pWordNodeWhereAnEnglishPredicateIsEncapsulated;
//    WordNode * 
//      pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=ss.
//      m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated;
//		if(psv.size()>dwStartOfNextElement)// && !bValidObjectOccured)
//		{ // es kann nicht gleichzeitig ein Objekt und eine Umstandsbestimmnung
//			// der Art und Weise geben, denn sonst wären solche Sätze möglich:
//			// "Am I intelligent a car?" oder: "Am I a car intelligent?"
//#ifdef _DEBUG
//			printf("sv.size()>dwStartOfNextElement && !bValidObjectOccured\n");
//#endif
//			DWORD dwEndOfValidManner=0;
//			//BOOL bValidManner=FALSE;
//			BOOL bBe=FALSE;
//			if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
//			{
//				if(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)==
//					typeid(EnglishAuxiliaryVerb))
//				{
//					EnglishAuxiliaryVerb * eav=dynamic_cast
//						<EnglishAuxiliaryVerb *>
//						(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord);
//					if(eav)
//						if(eav->m_strWords[0]==_T("be"))
//							bBe=TRUE;
//				}
//				gv=dynamic_cast<GermanVerb *>
//					(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext->m_pWord);
//			}
//			if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
//			{ // bei Perfekt; Beispiel: "Have I BEEN?"
//				if(typeid(
//					*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//					m_pWord)==typeid(EnglishAuxiliaryVerb))
//				{
//					EnglishAuxiliaryVerb * eav=dynamic_cast
//						<EnglishAuxiliaryVerb *>
//						(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//						m_pWord);
//					if(eav)
//						if(eav->m_strWords[0]==_T("be"))
//							bBe=TRUE;
//				}
//				gv=dynamic_cast<GermanVerb *>
//					(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//					m_pWordNodeNext->m_pWord);
//			}
//#ifdef _DEBUG
//			printf("gv->m_bCase: %u\n",gv->m_bCase);
//#endif
//			if(gv->m_bCase==0) // wenn das Prädikat kein Objekt verlangt
//			{
//				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
//				{
//					Manner manner;
//					if(ParseManner(manner,GetBetween(psv,dwStartOfNextElement,dwRun),bBe,TRUE))
//					{
//						//dwEndOfValidObject=dwRun;
//						TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
//						dwEndOfValidManner=dwRun;
//						bValidManner=TRUE;
//						//bValidObject=TRUE;
//						if(ss.m_pManner)
//						{
//							TRACE("ss.m_pManner\n");
//							delete ss.m_pManner;
//						}
//						ss.m_pManner=new Manner(manner); // Standard-Kopierkonstruktor nutzen
//						dwEndOfObject=dwRun-1;
//					}
//					if(bValidManner)
//					{
//						//dwStartOfNextElement=dwEndOfValidManner+1;
//						if(psv.size()==(dwStartOfNextElement=dwEndOfValidManner+1))
//						{
//							ss.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=pWordNode;
//							ss.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=
//								pWordNodeWhereAnEnglishBendVerbIsEncapsulated;
//							ss.m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=
//								pWordNodeWhereAnEnglishQuestionWordIsEncapsulated;
//							ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=
//								pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated;
//							#ifdef _DEBUG
//							printf("ss.m_bTense: %u\n",ss.m_bTense);
//							printf("BOOL ParseQuestionClause(Clause & subsentence,\
//CStringVector sv) - ENDE - return TRUE\n");
//							#endif
//							TRACE("BOOL ParseQuestionClause(Clause & subsentence,\
//CStringVector sv) - ENDE - return TRUE\n");
//							return TRUE;
//						}
//					}
//				}
//				//if(bValidManner)
//				//	break;
//			}
//			else//if(gv->m_bCase==0)
//			// wenn das Prädikat mind. ein Objekt verlangt
//			{
//				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
//				{
//					for(DWORD dwRun2=dwRun;dwRun2<psv.size();dwRun2++)
//					{
//						Manner manner;
//						if(ParseManner(manner,GetBetween(psv,dwRun,dwRun2),bBe,TRUE))
//						{
//							//dwEndOfValidObject=dwRun;
//							TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
//							//dwEndOfValidManner=dwRun;
//							bValidManner=TRUE;
//							dwStartOfNextElement=dwRun2+1;
//							//bValidObject=TRUE;
//							if(ss.m_pManner)
//							{
//								TRACE("ss.m_pManner\n");
//								delete ss.m_pManner;
//							}
//							ss.m_pManner=new Manner(manner); // Standard-Kopierkonstruktor nutzen
//							dwEndOfObject=dwRun-1;
//						}
//					}
//					if(bValidManner)
//						break;
//				}
//			}
//		}
//		if(psv.size()>dwStartOfNextElement)
//		{
//			//wenn eine gültige Umstandsbestimmung der Art und Weise 
//			//gefunden wurde oder kein Fall
//			if(bValidManner || gv->m_bCase==0)
//			{
//			#ifdef _DEBUG
//				printf("bValidManner || gv->m_bCase==0\n");
//			#endif
//				DWORD dwBegin=dwStartOfNextElement;
//				for(DWORD dwRun=dwBegin;dwRun<psv.size();dwRun++)
//				{
//					Place place;
//					std::vector<Range> vecRange;
//					if(ParsePlace(place,GetBetween(psv,dwBegin,dwRun),vecRange,TRUE))
//					{
//						if(ss.m_pPlace)
//							delete ss.m_pPlace;
//						ss.m_pPlace=new Place;
//						*ss.m_pPlace=place;
//						//bValidObject=TRUE;
//						dwEndOfLastValidElement=dwRun;
//						dwStartOfNextElement=dwRun+1;
//						bValidPlace=TRUE;
//					}
//				}
//			}
//			//wenn nicht: "eine gültige Umstandsbestimmung der Art und Weise gefunden 
//			//wurde oder kein Fall", also wenn: "keine gültige Umstandsbestimmung 
//			//der Art und Weise gefunden wurde und das Prädikat mindestens ein Objekt
//			//verlangt"
//			else
//			{
//			#ifdef _DEBUG
//				printf("!(bValidManner || gv->m_bCase==0)\n");
//			#endif
//				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
//				{
//					#ifdef _DEBUG
//					printf("in for-Schleife\n");
//					#endif
//					for(DWORD dwRun2=dwRun;dwRun2<psv.size();dwRun2++)
//					{
//						Place place;
//						if(ParsePlace(place,GetBetween(psv,dwRun,dwRun2),vecRange,TRUE))
//						{
//							if(ss.m_pPlace)
//								delete ss.m_pPlace;
//							ss.m_pPlace=new Place;
//							*ss.m_pPlace=place;
//							//bValidObject=TRUE;
//							dwEndOfLastValidElement=dwRun;
//							dwStartOfNextElement=dwRun2+1;
//							bValidPlace=TRUE;
//							dwEndOfObject=dwRun-1;
//						}
//					}
//					if(bValidPlace)
//						break;
//				}
//			}
//		}
//		if(bValidPlace)
//		{
//			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//			{
//				rVecRange.push_back(vecRange.at(dwRun));
//			}
//		}
//		if(psv.size()>dwStartOfNextElement)
//		{
//			//wenn Umstandsbestimmung der Art und Weise oder Umstandsbestimmung
//			//des Ortes oder das Prädikat kein Objekt verlangt
//			if(bValidManner || bValidPlace || gv->m_bCase==0)
//			{
//        WordNode * pWordNode;
//				if((pWordNode=
//					GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
//					psv.at(dwStartOfNextElement).m_Str)) )
//				{
//					#ifdef _DEBUG
//					printf("Adverb\n");
//					#endif
//					EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
//					if(ea)
//						if(ea->m_bType==5) // Adverb der Zeit ('today')
//						{
//							#ifdef _DEBUG
//							printf("Adverb der Zeit: %s\n",psv.at(dwStartOfNextElement).m_Str);
//							#endif
//							dwStartOfNextElement++;
//							pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=pWordNode;
//							bValidTime=TRUE;
//						}
//				}
//			}
//			//wenn nicht: "Umstandsbestimmung der Art und Weise oder Umstandsbestimmung
//			//des Ortes oder das Prädikat kein Objekt verlangt", also wenn:
//			//"keine Umstandsbestimmung der Art und Weise und keine Umstandsbestimmung
//			//des Ortes und das Prädikat ein Objekt verlangt"
//			else
//			{
//        WordNode * pWordNode;
//				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
//				{
//					if((pWordNode=
//						GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
//						psv.at(dwRun).m_Str)) )
//					{
//						#ifdef _DEBUG
//						printf("Adverb\n");
//						#endif
//						EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
//						if(ea)
//							if(ea->m_bType==5) // Adverb der Zeit ('today')
//							{
//								#ifdef _DEBUG
//								printf("Adverb der Zeit: %s\n",psv.at(dwRun).m_Str);
//								#endif
//								pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=pWordNode;
//								dwEndOfObject=dwRun-1;
//								dwStartOfNextElement=dwRun+1;
//								bValidTime=TRUE;
//								break;
//							}
//					}
//				}
//			}
//		}
//		//wenn noch kein Subjekt existiert
//		//if(dwStartOfObject!=0 && !ss.m_pSubject)
//		if(!ss.m_pSubject)
//		{
//#ifdef _DEBUG
//			printf("dwStartOfObject!=0\n");
//#endif
//			//erstes ParseSubject in ParseQuestionClause
//#ifdef _DEBUG
//			printf("drittes ParseSubject in ParseQuestionClause\n");
//#endif
//			if(dwEndOfSubject==0)
//				dwEndOfSubject=psv.size()-1;
//			if(ParseSubject(subject,GetBetween(psv,dwStartOfSubject,
//				dwEndOfSubject),FALSE,vecRange,TRUE,FALSE))
//			{
//				if(dwStartOfNextElement<dwEndOfSubject+1)
//					dwStartOfNextElement=dwEndOfSubject+1;
//        //TODO bIndexes has to get values for the "person indices" that match
//        //within ParsePredicate().
//        BYTE bIndexes=255;
//				//wenn bIndexes um "subject.m_personIndex" Anzahl Bits nach rechts
//				//verschoben und bitweise-und mit 1 keine 1 ergibt
//				//(bIndexes setzt für jede gebeugte Verbform, die mit dem Verb 
//				//übereinstimmt ein Bit
//				if(!((bIndexes>>subject.m_personIndex)&1))
//				{
//#ifdef _DEBUG
//					printf("subject.m_personIndex: %u\n",subject.m_personIndex);
//					printf("BOOL ParseQuestionClause(Clause & ss,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
//& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
//#endif
//					return 0;
//				}
//				if(ss.m_pSubject)
//					delete ss.m_pSubject;
//				ss.m_pSubject=new Subject(subject);
//#ifdef _DEBUG
//#endif
//				bValidSubject=TRUE;
//				//dwEndOfValidSubject=dwRun;
//				//dwStartOfNextElement=dwRun+1;
//				for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//					rVecRange.push_back(vecRange.at(dwRun));
//				if(dwStartOfNextElement==psv.size())
//				{
//					if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
//					ss.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=
//						pWordNodeWhereAnEnglishBendVerbIsEncapsulated;
//						ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
//					//ss.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNodeWhereAnEnglishBendVerbIsEncapsulated;
//					ss.m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=
//						pWordNodeWhereAnEnglishQuestionWordIsEncapsulated;
//					ss.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=
//						pWordNodeWhereAnEnglishInfinitiveIsEncapsulated;
//					#ifdef _DEBUG
//					printf("ss.m_bTense: %u\n",ss.m_bTense);
//					printf("BOOL ParseQuestionClause(Clause & ss,const \
//CStringVector & sv) - return TRUE\n");
//					#endif
//					return TRUE;
//				}
//			}
//			else
//				return FALSE;
//		}
//			GermanVerb * gvPredicate=NULL;
//			EnglishAuxiliaryVerb * eavPredicate=NULL;
//			if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
//			{
//				if(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->
//					m_pWord)==typeid(EnglishAuxiliaryVerb))
//					eavPredicate=dynamic_cast<EnglishAuxiliaryVerb*>
//					(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord);
//				if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext)
//					gvPredicate=dynamic_cast<GermanVerb*>
//					(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext->
//					m_pWord);
//			}
//			if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
//			{
//				if(typeid(
//					*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//					m_pWord)==typeid(EnglishAuxiliaryVerb))
//					eavPredicate=dynamic_cast<EnglishAuxiliaryVerb*>
//					(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//					m_pWord);
//				if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//					m_pWordNodeNext)
//					gvPredicate=dynamic_cast<GermanVerb*>
//					(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//						m_pWordNodeNext->m_pWord);
//			}
//#ifdef _DEBUG
//		printf("bValidMiddle: %u bValidManner %u eavPredicate: %u \
//gvPredicate: %u dwStartOfObject: %u\n",
//			bValidMiddle,bValidManner,eavPredicate,gvPredicate,dwStartOfObject);
//#endif
//		//wenn kein "Mittelteil" (Teil des Prädikates nach dem Subjekt)
//		//z.B. : "Have I used a car?" "Mittelteil": "used" Subjekt: "I" und 
//		//keine Umstandsbestimmung der Art und Weise und eavPredicate ungleich
//		//NULL ist
//		if(gvPredicate && dwStartOfObject>0)
//		{
//			BOOL bValidObject=FALSE;
//			DWORD dwEndOfValidSentenceElement=0;
//			if(ss.m_bActiveClause)
//			{
//#ifdef _DEBUG
//				printf("gv: %u\n",gv);
//#endif
//				if(gvPredicate->m_bCase==1 || gvPredicate->m_bCase==2 || 
//					gvPredicate->m_bCase==4)
//				{ // wenn das Prädikat den 1. , 3. oder 4. Fall verlangt
//					// gv->Case==1  : 3. Fall
//					// gv->Case==2  : 4. Fall
//					// gv->Case==4  : 1. Fall
//					#ifdef _DEBUG
//					printf("gvPredicate->m_bCase==1 || gvPredicate->m_bCase==2 || \
//gvPredicate->m_bCase==3\n");
//					#endif
//					std::vector<Range> vecRange;
//					Object object;
//#ifdef _DEBUG
//					printf("dwStartOfObject: %u dwEndOfObject: %u\n",dwStartOfObject,
//						dwEndOfObject);
//#endif
//					if(ParseObject(object,GetBetween(psv,dwStartOfObject,dwEndOfObject),
//						TRUE,vecRange,TRUE)==0)
//					{
//					#ifdef _DEBUG
//						printf("BOOL ParseQuestionClause(Clause & subsentence,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange) - return FALSE\n");
//					#endif
//						return 0;
//					}
//					if(ss.m_pObjectFirst)
//						delete ss.m_pObjectFirst;
//					ss.m_pObjectFirst=new Object(object);
//					for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//						rVecRange.push_back(vecRange.at(dwRun));
//					if(!bValidManner && !bValidPlace && !bValidTime)
//						dwStartOfNextElement=dwEndOfObject+1;
//				}
//				if(gvPredicate->m_bCase==3)
//				{ // wenn das Prädikat 2 Fälle (3. und 4. Fall) verlangt
//					std::vector<Range> vecRange;
//					std::vector<Range> vecRangeOfValidObject;
//					DWORD dwEndOfValidObject=0;
//					for(DWORD dwRun=dwStartOfObject;dwRun<dwEndOfObject+1;dwRun++)
//					{
//						vecRange.clear();
//						Object object;
//						if(ParseObject(object,GetBetween(psv,dwStartOfObject,dwRun),
//							TRUE,vecRange,TRUE)==2)
//						{
//							dwEndOfValidObject=dwRun;
//							//dwEndOfValidSentenceElement=dwRun;
//							bValidObject=TRUE;
//							if(ss.m_pObjectFirst)
//								delete ss.m_pObjectFirst;
//							ss.m_pObjectFirst=new Object(object);
//							vecRangeOfValidObject.clear();
//							vecRangeOfValidObject=vecRange;
//						}
//					}
//					if(bValidObject)
//					{
//						dwStartOfNextElement=dwEndOfValidObject+1;
//						dwEndOfValidSentenceElement=dwEndOfValidObject;
//						for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
//							rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
//					}
//				}
//			}
//			if(ss.m_bActiveClause && gvPredicate->m_bCase==3)
//			{ // wenn ein Verb, das 2 Fälle verlangt
//				#ifdef _DEBUG
//				printf("gvPredicate->Case==3\n");
//				#endif
//				DWORD dwEndOfValidObject=0;
//				if(bValidObject)
//				{
//					std::vector<Range> vecRange;
//					Object object;
//					ParseObject(object,GetBetween(psv,dwStartOfNextElement,
//						dwEndOfObject),TRUE,vecRange,TRUE);
//					bValidObject=TRUE;
//					if(ss.m_pObjectSecond)
//						delete ss.m_pObjectSecond;
//					ss.m_pObjectSecond=new Object(object);
//					for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//						rVecRange.push_back(vecRange.at(dwRun));
//					if(!bValidManner && !bValidPlace && !bValidTime)
//						dwStartOfNextElement=dwEndOfObject+1;
//				}
//				else
//				{
//					std::vector<Range> vecRange;
//					std::vector<Range> vecRangeOfValidObject;
//					DWORD dwStartOfValidObject=0;
//					for(DWORD dwRun=dwEndOfObject;dwRun>dwStartOfObject-1;dwRun--)
//					{
//						vecRange.clear();
//						Object object;
//						if(ParseObject(object,GetBetween(psv,dwRun,dwEndOfObject),
//							TRUE,vecRange,TRUE)==2)
//						{
//							dwStartOfValidObject=dwRun+1;
//							//dwEndOfValidSentenceElement=dwRun;
//							bValidObject=TRUE;
//							if(ss.m_pObjectSecond)
//								delete ss.m_pObjectSecond;
//							ss.m_pObjectSecond=new Object;
//							*ss.m_pObjectSecond=object;
//							//subsentence.m_pObject=object;
//							vecRangeOfValidObject.clear();
//							vecRangeOfValidObject=vecRange;
//						}
//					}
//					for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
//						rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
//					Object object;
//					if(bValidObject)
//					{
//						vecRange.clear();
//						ParseObject(object,GetBetween(psv,dwStartOfObject,
//							dwStartOfValidObject-1),TRUE,vecRange,TRUE);
//						if(ss.m_pObjectFirst)
//							delete ss.m_pObjectFirst;
//						ss.m_pObjectFirst=new Object(object);
//						for(dwRun=0;dwRun<vecRange.size();dwRun++)
//							rVecRange.push_back(vecRange.at(dwRun));
//						if(!bValidManner && !bValidPlace && !bValidTime)
//							dwStartOfNextElement=dwEndOfObject+1;
//					}
//				}
//			}
//			if(!ss.m_bActiveClause && gvPredicate->m_bCase==3) // wenn Passivsatz
//			{ // und 2 Objekte pro Verb
//				#ifdef _DEBUG
//				printf("!subsentence.m_bActiveClause && gvPredicate->m_bCase==3\n");
//				#endif
//				Subject subject;
//				std::vector<Range> vecRange;
//				//fünftes ParseSubject in ParseQuestionClause
//#ifdef _DEBUG
//				printf("viertes ParseSubject in ParseQuestionClause\n");
//#endif
//				if(ParseSubject(subject,GetBetween(psv,dwEndOfValidMiddle+1,
//					dwEndOfObject),FALSE,vecRange,TRUE,FALSE))
//				{
//          //TODO bPersonIndexVerb has to get values.
//          BYTE bPersonIndexVerb=subject.m_personIndex;
//					if(subject.m_personIndex!=bPersonIndexVerb)
//					{
//#ifdef _DEBUG
//						printf("BOOL ParseQuestionClause(Clause & ss,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
//& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
//#endif
//						return 0;
//					}
//					if(ss.m_pPersonalSubject)
//						delete ss.m_pPersonalSubject;
//					ss.m_pPersonalSubject=new Subject;
//					*ss.m_pPersonalSubject=subject;
//					for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//						rVecRange.push_back(vecRange.at(dwRun));
//					if(!bValidManner && !bValidPlace && !bValidTime)
//						dwStartOfNextElement=dwEndOfObject+1;
//				}
//			}
//		}
//		if(!bValidMiddle && !bValidManner && eavPredicate)
//		{
//			
//			if(eavPredicate->m_strWords[0]==_T("be"))// || eavPredicate->
//			//	m_strWords[0]==_T("have"))
//			{
//				DWORD dwStartOfObjectWithoutPersonalNames=0;
//				BOOL bValidObjectWithoutPersonalNames=FALSE;
//#ifdef _DEBUG
//				printf("dwStartOfSubject: %u\n",dwStartOfSubject);
//#endif
//				//std::vector<Range> vecRangeOfObjectWithoutPersonalNames;
//				for(DWORD dwRun=psv.size()-1;dwRun>dwStartOfSubject;dwRun--)
//				{
//					Object object;
//					std::vector<Range> vecRange;
//					if(ParseObject(object,GetBetween(psv,dwRun,psv.size()-1),TRUE,
//						vecRange,TRUE)==2)
//					{ // wenn das Objekt keine(n) Eigennamen enthält
//						// zum Beispiel: "a man" aus dem Fragesatz: "Am I a man?"
//						if(ss.m_pObjectFirst)
//							delete ss.m_pObjectFirst;
//						ss.m_pObjectFirst=new Object(object);
//						dwStartOfObjectWithoutPersonalNames=dwRun;
//						bValidObjectWithoutPersonalNames=TRUE;
//						//vecRangeOfObjectWithoutPersonalNames=vecRange;
//					}
//				}
//				if(bValidObjectWithoutPersonalNames)
//				{
//					//for(dwRun=0;dwRun<vecRangeOfObjectWithoutPersonalNames.size();dwRun++)
//					//	rVecRange.push_back(vecRangeOfObjectWithoutPersonalNames.at(dwRun));
//#ifdef _DEBUG
//					printf("bValidObjectWithoutPersonalNames\n");
//					printf("dwStartOfObjectWithoutPersonalNames: %u rVecRange.size(): %u\n",
//						dwStartOfObjectWithoutPersonalNames,rVecRange.size());
//					printf("1. Prüfung auf Subjekt in ParseQuestionClause\n");
//#endif
//					Subject subject;
//					std::vector<Range> vecRange;
//					//zweites ParseSubject in ParseQuestionClause
//#ifdef _DEBUG
//					printf("fünftes ParseSubject in ParseQuestionClause\n");
//#endif
//					if(ParseSubject(subject,GetBetween(psv,dwStartOfSubject,
//						dwStartOfObjectWithoutPersonalNames-1),FALSE,vecRange,TRUE,FALSE))
//					{ // vor dem Objekt kann sich natürlich noch ein Subjekt befinden
//						// Es kann auch einen oder mehrere Eigennamen beinhalten, Hauptsache es
//						// ist gültig. Beispiel mit Eigennamen: "Is Stefan a man?"
//						if(subject.m_personIndex!=bPersonIndexVerb)
//						{
//#ifdef _DEBUG
//							printf("BOOL ParseQuestionClause(Clause & ss,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
//& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
//#endif
//							return 0;
//						}
//						if(ss.m_pSubject)
//							delete ss.m_pSubject;
//						ss.m_pSubject=new Subject(subject); //Standardkopierkonstruktur nutzen
//						for(dwRun=0;dwRun<vecRange.size();dwRun++)
//							rVecRange.push_back(vecRange.at(dwRun));
//						dwStartOfNextElement=psv.size();
//#ifdef _DEBUG
//						printf("Subjekt ist gültig\n");
//#endif
//					}
//				}
//			}
//		}
//		//wenn ein "Mittelteil" (Teil des Prädikates nach dem Subjekt)
//		//z.B. : "Have I used a car?" "Mittelteil": "used" Subjekt: "I" und/oder
//		//eine Umstandsbestimmung der Art und Weise und/oder eavPredicate gleich
//		//NULL ist
//		else
//		{
//			GermanVerb * gvPredicate=NULL;
//			if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
//				if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext)
//					gvPredicate=dynamic_cast<GermanVerb*>
//					(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext->
//						m_pWord);
//			if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
//				if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//					m_pWordNodeNext)
//					gvPredicate=dynamic_cast<GermanVerb*>
//						(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//						m_pWordNodeNext->m_pWord);
//#ifdef _DEBUG
//			printf("gvPredicate: %u ss.m_pSubject: %u\n",gvPredicate,ss.m_pSubject);
//#endif
//			if(gvPredicate && !ss.m_pSubject)
//			{
//#ifdef _DEBUG
//				printf("gvPredicate\n");
//#endif
//				if(gvPredicate->m_bCase==4)
//				{
//					//drittes ParseSubject in ParseQuestionClause
//#ifdef _DEBUG
//					printf("sechstes ParseSubject in ParseQuestionClause\n");
//#endif
//					if(ParseSubject(subject,GetBetween(psv,dwStartOfSubject,
//						dwEndOfObject),FALSE,vecRange,TRUE,FALSE))
//					{
//						if(subject.m_personIndex!=bPersonIndexVerb)
//						{
//#ifdef _DEBUG
//							printf("BOOL ParseQuestionClause(Clause & ss,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
//& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
//#endif
//							return 0;
//						}
//						*ss.m_pSubject=subject;
//						bValidSubject=TRUE;
//						for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//							rVecRange.push_back(vecRange.at(dwRun));
//						//dwEndOfValidSubject=dwRun;
//						//dwStartOfNextElement=dwRun+1;
//					}
//				}
//				if(gvPredicate->m_bCase==1 || gvPredicate->m_bCase==2 || 
//					gvPredicate->m_bCase==3)
//				{
//					//viertes ParseSubject in ParseQuestionClause
//#ifdef _DEBUG
//					printf("siebtes ParseSubject in ParseQuestionClause\n");
//#endif
//					if(ParseSubject(subject,GetBetween(psv,dwStartOfSubject,
//						dwEndOfSubject),FALSE,vecRange,TRUE,FALSE))
//					{
//						if(subject.m_personIndex!=bPersonIndexVerb)
//						{
//#ifdef _DEBUG
//							printf("BOOL ParseQuestionClause(Clause & ss,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
//& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
//#endif
//							return 0;
//						}
//						*ss.m_pSubject=subject;
//						bValidSubject=TRUE;
//						for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//							rVecRange.push_back(vecRange.at(dwRun));
//						//dwEndOfValidSubject=dwRun;
//						//dwStartOfNextElement=dwRun+1;
//						BOOL bValidObjectOccured=FALSE;
//						BOOL bValidObject=FALSE;
//						DWORD dwEndOfValidSentenceElement=0;
//						if(ss.m_bActiveClause)
//						{
//							if(gv->m_bCase==1 || gv->m_bCase==2 || gv->m_bCase==4)
//							{ // wenn das Prädikat den 1. , 3. oder 4. Fall verlangt
//								// gv->Case==1  : 3. Fall
//								// gv->Case==2  : 4. Fall
//								// gv->Case==4  : 1. Fall
//#ifdef _DEBUG
//								printf("gv->m_bCase==1 || gv->m_bCase==2 || gv->m_bCase==3\n");
//#endif
//								std::vector<Range> vecRange;
//								Object object;
//								if(ParseObject(object,GetBetween(psv,dwStartOfObject,dwEndOfObject),
//									TRUE,vecRange,TRUE)==0)
//								{
//#ifdef _DEBUG
//									printf("BOOL ParseQuestionClause(Clause & subsentence,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange) - return FALSE\n");
//#endif
//									return 0;
//								}
//								if(ss.m_pObjectFirst)
//									delete ss.m_pObjectFirst;
//								ss.m_pObjectFirst=new Object(object);
//								for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//									rVecRange.push_back(vecRange.at(dwRun));
//								if(!bValidManner && !bValidPlace && !bValidTime)
//									dwStartOfNextElement=dwEndOfObject+1;
//							}
//							if(gv->m_bCase==3)
//							{ // wenn das Prädikat 2 Fälle (3. und 4. Fall) verlangt
//								std::vector<Range> vecRange;
//								std::vector<Range> vecRangeOfValidObject;
//								DWORD dwEndOfValidObject=0;
//								for(DWORD dwRun=dwStartOfObject;dwRun<dwEndOfObject+1;dwRun++)
//								{
//									vecRange.clear();
//									Object object;
//									if(ParseObject(object,GetBetween(psv,dwStartOfObject,dwRun),
//										TRUE,vecRange,TRUE)==2)
//									{
//										dwEndOfValidObject=dwRun;
//										//dwEndOfValidSentenceElement=dwRun;
//										bValidObject=TRUE;
//										if(ss.m_pObjectFirst)
//											delete ss.m_pObjectFirst;
//										ss.m_pObjectFirst=new Object(object);
//										vecRangeOfValidObject.clear();
//										vecRangeOfValidObject=vecRange;
//									}
//								}
//								if(bValidObject)
//								{
//									dwStartOfNextElement=dwEndOfValidObject+1;
//									dwEndOfValidSentenceElement=dwEndOfValidObject;
//									for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
//										rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
//								}
//							}
//						}
//						if(ss.m_bActiveClause && gv->m_bCase==3)
//						{ // wenn ein Verb, das 2 Fälle verlangt
//#ifdef _DEBUG
//							printf("gv->Case==3\n");
//#endif
//							DWORD dwEndOfValidObject=0;
//							if(bValidObject)
//							{
//								std::vector<Range> vecRange;
//								Object object;
//								ParseObject(object,GetBetween(psv,dwStartOfNextElement,
//									dwEndOfObject),TRUE,vecRange,TRUE);
//								bValidObject=TRUE;
//								if(ss.m_pObjectSecond)
//									delete ss.m_pObjectSecond;
//								ss.m_pObjectSecond=new Object(object);
//								for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//									rVecRange.push_back(vecRange.at(dwRun));
//								if(!bValidManner && !bValidPlace && !bValidTime)
//									dwStartOfNextElement=dwEndOfObject+1;
//							}
//							else
//							{
//								std::vector<Range> vecRange;
//								std::vector<Range> vecRangeOfValidObject;
//								DWORD dwStartOfValidObject=0;
//								for(DWORD dwRun=dwEndOfObject;dwRun>dwStartOfObject-1;dwRun--)
//								{
//									vecRange.clear();
//									Object object;
//									if(ParseObject(object,GetBetween(psv,dwRun,dwEndOfObject),
//										TRUE,vecRange,TRUE)==2)
//									{
//										dwStartOfValidObject=dwRun+1;
//										//dwEndOfValidSentenceElement=dwRun;
//										bValidObject=TRUE;
//										if(ss.m_pObjectSecond)
//											delete ss.m_pObjectSecond;
//										ss.m_pObjectSecond=new Object;
//										*ss.m_pObjectSecond=object;
//										//subsentence.m_pObject=object;
//										vecRangeOfValidObject.clear();
//										vecRangeOfValidObject=vecRange;
//									}
//								}
//								for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
//									rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
//								Object object;
//								if(bValidObject)
//								{
//									vecRange.clear();
//									ParseObject(object,GetBetween(psv,dwStartOfObject,
//										dwStartOfValidObject-1),TRUE,vecRange,TRUE);
//									if(ss.m_pObjectFirst)
//										delete ss.m_pObjectFirst;
//									ss.m_pObjectFirst=new Object(object);
//									for(dwRun=0;dwRun<vecRange.size();dwRun++)
//										rVecRange.push_back(vecRange.at(dwRun));
//									if(!bValidManner && !bValidPlace && !bValidTime)
//										dwStartOfNextElement=dwEndOfObject+1;
//								}
//							}
//						}
//						if(!ss.m_bActiveClause && gv->m_bCase==3) // wenn Passivsatz
//						{ // und 2 Objekte pro Verb
//#ifdef _DEBUG
//							printf("!subsentence.m_bActiveClause && gv->m_bCase==3\n");
//#endif
//							Subject subject;
//							std::vector<Range> vecRange;
//							//fünftes ParseSubject in ParseQuestionClause
//#ifdef _DEBUG
//							printf("achtes ParseSubject in ParseQuestionClause\n");
//#endif
//							if(ParseSubject(subject,GetBetween(psv,dwEndOfValidMiddle+1,
//								dwEndOfObject),FALSE,vecRange,TRUE,FALSE))
//							{
//								if(subject.m_personIndex!=bPersonIndexVerb)
//								{
//#ifdef _DEBUG
//									printf("BOOL ParseQuestionClause(Clause & ss,const \
//PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
//& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
//#endif
//									return 0;
//								}
//								if(ss.m_pPersonalSubject)
//									delete ss.m_pPersonalSubject;
//								ss.m_pPersonalSubject=new Subject;
//								*ss.m_pPersonalSubject=subject;
//								for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
//									rVecRange.push_back(vecRange.at(dwRun));
//								if(!bValidManner && !bValidPlace && !bValidTime)
//									dwStartOfNextElement=dwEndOfObject+1;
//							}
//						}
//					}
//				}
//			}
//		}
//}

BOOL ParseClauseWithArticleObject(Clause & ss,CStringVector sv)
{ // Bsp.: She likes the school.
	TRACE("BOOL ParseClauseWithArticleObject(Clause & ss,CStringVector sv) - ANFANG\n");
	//in MFC/Windows the TRACE macro has a variable # of args--I do 
	//not know how to enable variable numbers of args for the same macro name.
	//So I exclude it from non-Windows.
#ifdef _WINDOWS
	//Without this in non-DEBUG compile the loop also would consume time.
	#ifdef _DEBUG
	for(DWORD i=0;i<sv.size();i++)
		TRACE("sv.at(%d): %s\n",i,sv.at(i));
	#endif
#endif
	Subject subject;
	DWORD dwEndofValidSubject=0;
	DWORD dwEndofValidMiddle=0;
	BOOL bValidMiddle=FALSE;
	BOOL bValidSubject=FALSE;
	//DWORD dwStart=0;
	for(DWORD dwRun=0;dwRun<sv.size();dwRun++)
	{
		CStringVector svSubject=GetBetween(sv,0,dwRun);
		/*if(ParseSubject(subject,svSubject,TRUE))
		{
			bValidSubject=TRUE;
			dwEndofValidSubject=dwRun;
			ss.m_Subject=subject;
		}*/
	}
	if(bValidSubject)
		if(dwEndofValidSubject+1<sv.size())
			for(DWORD dwRun=dwEndofValidSubject+1;dwRun<sv.size();dwRun++)
			{
				CStringVector svMiddle=GetBetween(sv,dwEndofValidSubject+1,dwRun);
				if(ParseMiddle(ss,TRUE,svMiddle,1,TRUE))
				{
					bValidMiddle=TRUE;
					dwEndofValidMiddle=dwRun;
				}
			}
	if(bValidMiddle)
		if(dwEndofValidMiddle+1<sv.size())
		{
			CStringVector svObject=GetBetween(sv,dwEndofValidMiddle+1,sv.size()-1);
			Object object;
			if(ParseArticleObject(object,svObject))
			{
				if(ss.m_pObjectFirst)
					delete ss.m_pObjectFirst;
				ss.m_pObjectFirst=new Object(object);
				TRACE("BOOL ParseClauseWithArticleObject(Clause & ss,CStringVector sv) - ENDE - return TRUE\n");
				return TRUE;
			}
		}
	TRACE("BOOL ParseClauseWithArticleObject(Clause & ss,CStringVector sv) - ENDE - return FALSE\n");
	return FALSE;
}

BOOL ParseClauseWithoutSubject(ClauseWithoutSubject & cws,CStringVector sv)
{
	TRACE("BOOL ParseClauseWithoutSubject(ClauseWithoutSubject & ss,CStringVector sv) - ANFANG\n");
	//in MFC/Windows the TRACE macro has a variable # of args--I do 
	//not know how to enable variable numbers of args for the same macro name.
	//So I exclude it from non-Windows.
#ifdef _WINDOWS
	//Without this in non-DEBUG compile the loop also would consume time.
	#ifdef _DEBUG
	for(DWORD i=0;i<sv.size();i++)
		TRACE("sv.at(%d): %s\n",i,sv.at(i));
	#endif
#endif
	Subject subject;
	//DWORD dwEndofValidSubject=0;
	DWORD dwEndofValidMiddle=0;
	BOOL bValidMiddle=FALSE;
	BOOL bValidSubject=FALSE;
	//DWORD dwStart=0;
	for(DWORD dwRun=0;dwRun<sv.size();dwRun++)
	{
		CStringVector svMiddle=GetBetween(sv,0,dwRun);
		if(ParseMiddle(cws,TRUE,svMiddle))
		{
			bValidMiddle=TRUE;
			dwEndofValidMiddle=dwRun;
		}
	}
	if(bValidMiddle)
		if(dwEndofValidMiddle+1<sv.size())
		{
			CStringVector svObject=GetBetween(sv,dwEndofValidMiddle+1,sv.size()-1);
			Object object;
			if(ParseArticleObject(object,svObject))
			{
				cws.m_Object=object;
				TRACE("BOOL ParseClauseWithoutSubject(ClauseWithoutSubject & ss,CStringVector sv) - ENDE - return TRUE\n");
				return TRUE;
			}
		}
		else // muss kein Objekt haben!!!
		{ // Bsp. The car you like sucks.
			// Hauptsatz: The car sucks. <- kein Objekt
			TRACE("BOOL ParseClauseWithoutSubject(ClauseWithoutSubject & ss,CStringVector sv) - ENDE - return TRUE\n");
			return TRUE;
		}
	TRACE("BOOL ParseClauseWithoutSubject(ClauseWithoutSubject & ss,CStringVector sv) - ENDE - return FALSE\n");
	return FALSE;
}

BYTE ParseComparison(Comparison & comparison,const PositionCStringVector & 
	psv,std::vector<Range> & rVecRange,const DWORD & dwFlags)
{
	DWORD dwStartOfObject=0;
	Object object;
	std::vector<Range> vecRange;
	WordNode * pWordNode=NULL;
#ifdef _DEBUG
	printf("BYTE ParseComparison(Comparison & comparison,\
const PositionCStringVector & psv,const DWORD & dwFlags) ANFANG\n");
	for(DWORD dwDebugRun=0;dwDebugRun<psv.size();++dwDebugRun)
		printf("psv.at(%u).m_Str: %s\n",dwDebugRun,psv.at(dwDebugRun).m_Str);
#endif
	if(psv.size()<3)
	{
#ifdef _DEBUG
		printf("BYTE ParseComparison(Comparison & comparison,\
const PositionCStringVector & psv,const DWORD & dwFlags) return 0 - \
(psv.size()<3)\n");
#endif
		return 0;
	}
	if(psv.at(1).m_Str==_T("than"))
	{
		if(!(pWordNode=GetPointerToWordNodeWhereAnEnglishComperativeIsEncapsulated(
			GetBetweenAsCStringVector(psv,0,0),dwFlags)))
		{
#ifdef _DEBUG
			printf("BYTE ParseComparison(Comparison & comparison,\
const PositionCStringVector & psv,const DWORD & dwFlags) return 0 - \
kein Komperativ\n");
#endif
			return 0;
		}
		dwStartOfObject=2;
	}
	else if(psv.at(2).m_Str==_T("than"))
	{
		if(!(pWordNode=GetPointerToWordNodeWhereAnEnglishComperativeIsEncapsulated(
			GetBetweenAsCStringVector(psv,0,1),dwFlags)))
		{
#ifdef _DEBUG
			printf("BYTE ParseComparison(Comparison & comparison,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
dwFlags) - return 0 - kein Komperativ\n");
#endif
			return 0;
		}
		dwStartOfObject=3;
	}
	else
	{
#ifdef _DEBUG
		printf("BYTE ParseComparison(Comparison & comparison,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
dwFlags) return 0 - \"than\" befindet ist weder 2. noch 3. Token\n");
#endif
		return 0;
	}
	comparison.m_pWordNodeWhereAnEnglishComperativeIsEncapsulated=pWordNode;
	if(!ParseObject(object,GetBetween(psv,dwStartOfObject,psv.size()),0,
		vecRange,dwFlags))
	{
#ifdef _DEBUG
		printf("BYTE ParseComparison(Comparison & comparison,\
const PositionCStringVector & psv,const DWORD & dwFlags) return 0 - \
!ParseObject\n");
#endif
		return 0;
	}
	for(DWORD dwRun=0;dwRun<vecRange.size();++dwRun)
		rVecRange.push_back(vecRange.at(dwRun));
	comparison.m_Object=object;
#ifdef _DEBUG
		printf("BYTE ParseComparison(Comparison & comparison,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
dwFlags) - return 1\n");
#endif
	return 1;
}

//bContainsFirstTokenOfSentence wird an alle Funktionen übergeben, die
//auf Konstruktionen prüfen, die am Anfang eines Aufzählungselementes 
//eines Subjektes stehen können
//@return diese Funktion liefert zurück, um welchen Index es sich handelt.
//Beispiel: Subjekt: "I" "m_personIndex": 1
BYTE ParseEnumerationElement(EnumerationElement & ee,	const 
	PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & 
	rVecRange,const DWORD & dwFlags,BYTE & bContainsFirstTokenOfSentence)
{
	BOOL bValidDefiniteArticleNoun=FALSE;
	BYTE personIndex=0;
	DWORD dwValidDefiniteArticleNounEnd=0;
	DWORD dwRun ;
	EnglishNounRecord enr;
	TRACE("ParseEnumerationElement ANFANG\n");
	#ifdef _DEBUG
	printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
		PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> &\
	rVecRange,const DWORD & dwFlags,BYTE & bContainsFirstTokenOfSentence) ANFANG\n");
	printf("bContainsFirstTokenOfSentence: %u\n",bContainsFirstTokenOfSentence);
	for(DWORD i=0;i<psv.size();i++)
		printf("psv.at(%d).m_Str: %s\n",i,psv.at(i).m_Str);
	#endif
	if(psv.size()==1)
	{
		//bExcludeGerund wird auf TRUE gesetzt, wenn keine Rekursion der
		//Funktion ParseGerund() erfolgen soll. Demnach wird bExcludeGerund
		//auch von der Funktion ParseGerund() auf TRUE gesetzt.
		//Wenn die Funktion ParseGerund() die Funktion ParseSubject(), die
		//die Funktion ParseEnumerationElement() aufruft, dann deswegen,
		//um nach einem persönlichen Subjekt (zum Beispiel: "him") zu suchen.
		//Beispielsatz für ein Gerundium mit persönlichem Subjekt:
		//"I like him working." ("Ich mag, dass er arbeitet")
		// S -P--   
    //        -Gerundium-
		//        -PS -Part.-
		//S=Subjekt   P=Prädikat   PS=persönliches Subjekt   Part.=Partizip
		if(bExcludeGerund)
		{
			CString str=psv.at(0).m_Str;
			//wenn das erste Bit von dwFlags gesetzt (1) ist, bedeutet das, dass
			//die Groß- und Kleinschreibung irrelevant ist und daher jedes Token
			//Groß- oder Kleinbuchstaben haben kann, daher muss das Token vorher
			//mittels MakeLower() kleingemacht werden.
			if(!(dwFlags&CASE_SENSITIVE)==1)
				str.MakeLower();
			//wenn psv das erste Token des Satzes beinhaltet, dann den ersten
			//Buchstaben von str mittels MakeFirstLetterUpper() klein machen
			else if(bContainsFirstTokenOfSentence)
				MakeFirstLetterLower(str);
			if(str==_T("me"))
			{
				ee.m_personalPronoun=1;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 1\n");
#endif
				return 1;
			}
			if(str==_T("you"))
			{
				ee.m_personalPronoun=2;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 2\n");
#endif
				return 2;
			}
			if(str==_T("him"))
			{
				ee.m_personalPronoun=3;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 3\n");
#endif
				return 3;
			}
			if(str==_T("her"))
			{
				ee.m_personalPronoun=4;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 3\n");
#endif
				return 3;
			}
			if(str==_T("it"))
			{
				ee.m_personalPronoun=5;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 3\n");
#endif
				return 3;
			}
			if(str==_T("us"))
			{
				ee.m_personalPronoun=6;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 4\n");
#endif
				return 4;
			}
			if(str==_T("them"))
			{
				ee.m_personalPronoun=7;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 6\n");
#endif
				return 6;
			}
		}
		else
		{
			CString str=psv.at(0).m_Str;
			//wenn das erste Bit von dwFlags gesetzt (1) ist, bedeutet das, dass
			//die Groß- und Kleinschreibung irrelevant ist und daher jedes Token
			//Groß- oder Kleinbuchstaben haben kann, daher muss das Token vorher
			//mittels MakeLower() kleingemacht werden.
			if(str!="I")
			{
				if(!(dwFlags&CASE_SENSITIVE)==1)
					str.MakeLower();
				else if(bContainsFirstTokenOfSentence)
					MakeFirstLetterLower(str);
			}
			if(str==_T("I"))
			{
				ee.m_personalPronoun=1;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 1\n");
#endif
				return 1;
			}
			if(str==_T("you"))
			{
				ee.m_personalPronoun=2;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 2\n");
#endif
				return 2;
			}
			if(str==_T("he"))
			{
				ee.m_personalPronoun=3;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 3\n");
#endif
				return 3;
			}
			if(str==_T("she"))
			{
				ee.m_personalPronoun=4;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 3\n");
#endif
				return 3;
			}
			if(str==_T("it"))
			{
				ee.m_personalPronoun=5;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 3\n");
#endif
				return 3;
			}
			if(str==_T("we"))
			{
				ee.m_personalPronoun=6;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 4\n");
#endif
				return 4;
			}
			if(str==_T("they"))
			{
				ee.m_personalPronoun=7;
				bContainsFirstTokenOfSentence=FALSE;
#ifdef _DEBUG
				printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,BOOL bExcludeGerund,std::vector<Range> & \
rVecRange,const DWORD & dwFlags) - return 6\n");
#endif
				return 6;
			}
		}
	}
	//ParseNoun()
	/*for(DWORD dwRun=0;dwRun<psv.size() && g_bContinue;dwRun++)
	{
		if((personIndex=ParsePersonalPronounOrNounStruct(enr,psv,dwFlags,
			bContainsFirstTokenOfSentence)))
		{
			if(ee.enr)
				delete ee.enr;
			ee.enr=new EnglishNounRecord;
			*ee.enr=enr;
			bValidEnglishNounRecord=TRUE;
		}
	}*/
	if((personIndex=ParsePersonalPronounOrNounStruct(enr,psv,dwFlags,
		bContainsFirstTokenOfSentence)))
	{
		ee.m_pEnr=new EnglishNounRecord;
		*ee.m_pEnr=enr;
		#ifdef _DEBUG
		printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv) ENDE !=0\n");
		#endif
		return personIndex;
	}
	//Überprüfen auf Konstruktion mit bestimmtem Artikel und Substantiv
	//Beispiel: "The school is great." -> "Die Schule ist großartig."
	//Konstruktion mit bestimmtem Artikel und Substantiv: "The school"
	for(//DWORD 
		dwRun=0;dwRun<psv.size() && g_bContinue;dwRun++)
	{
		BYTE ret=0;
		EnglishNounRecord enr;
		CStringVector sv = GetBetweenAsCStringVector(psv,
					0,dwRun) ;
		if((ret=ParseDefiniteArticleNoun(enr,sv,dwFlags,bContainsFirstTokenOfSentence)))
		{ // z.B.: The school
			if(ee.m_pEnr)
				delete ee.m_pEnr;
			ee.m_pEnr=new EnglishNounRecord;
			*ee.m_pEnr=enr;
			personIndex=ret;
			bValidDefiniteArticleNoun=TRUE;
			dwValidDefiniteArticleNounEnd=dwRun;
			#ifdef _DEBUG
			printf("ee.m_pEnr->m_Noun.size(): %u\n",ee.m_pEnr->m_Noun.size());
			#endif
		}
	}
	if(bValidDefiniteArticleNoun && psv.size()>dwValidDefiniteArticleNounEnd+1)
	{
		OfPhrase op;
		if(ParseOfPhrase(op,GetBetween(psv,dwValidDefiniteArticleNounEnd+1,
			psv.size()),dwFlags))
		{ // z.B.: of my father
			if(ee.m_pOfPhrase)
				delete ee.m_pOfPhrase;
			ee.m_pOfPhrase=new OfPhrase;
			*ee.m_pOfPhrase=op;
			#ifdef _DEBUG
			printf("op.m_Enr.m_Noun.size(): %u\n",op.m_Enr.m_Noun.size());
			printf("ee.m_OfPhrase.m_Enr->m_Noun.size(): %u\n",
				ee.m_pOfPhrase->m_Enr.m_Noun.size());
			printf("personIndex: %u\n",personIndex);
			printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv) ENDE !=0\n");
			#endif
			return personIndex;
		}
	}
	if(!bExcludeGerund)
	{
		std::vector<Range> vecRange;
		Gerund gerund;
		if(ParseGerund(gerund,FALSE,FALSE,psv,vecRange,dwFlags,
			bContainsFirstTokenOfSentence))
		{
			if(ee.m_pGerund)
				delete ee.m_pGerund;
			ee.m_pGerund=new Gerund;
			*ee.m_pGerund=gerund;
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
			#ifdef _DEBUG
			printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const PositionCStringVector & psv) ENDE return 3\n");
			#endif
			return 3;
		}
	}
	ToPlusInfinitive toPlusInfinitive;
	std::vector<Range> vecRange;
	if(ParseToPlusInfinitive(toPlusInfinitive,psv,vecRange,TRUE,
		bContainsFirstTokenOfSentence))
	{
		if(ee.m_pToPlusInfinitive)
			delete ee.m_pToPlusInfinitive;
		ee.m_pToPlusInfinitive=new ToPlusInfinitive(toPlusInfinitive);
		for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
			rVecRange.push_back(vecRange.at(dwRun));
		#ifdef _DEBUG
		printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const PositionCStringVector & psv) ENDE return 3\n");
		#endif
		return 3;
	}
	//dwFlags bitweise-Und 2147483648: nur wenn das 32. Bit gesetzt ist, ist 
	//das Ergebnis 2147483648. Dieses Ergebnis bedeutet, dass das Subjekt nicht 
	//nach Relativsätzen überprüft werden soll
	if(bValidDefiniteArticleNoun && psv.size() && (dwFlags&2147483648)!=
		2147483648>dwValidDefiniteArticleNounEnd+1)
	{
		BYTE bReturnOfParseRelativeClause=0;
		RelativeClause relativeClause;
		bReturnOfParseRelativeClause=ParseRelativeClause(relativeClause,
			GetBetween(psv,dwValidDefiniteArticleNounEnd+1,psv.size()),vecRange,
			dwFlags);
		if(bReturnOfParseRelativeClause!=0)
		{
			ee.m_pRelativeClause=new RelativeClause(relativeClause);
#ifdef _DEBUG
			printf("vecRange.size(): %d\n",vecRange.size());
#endif
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
#ifdef _DEBUG
			printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv) return personIndex\n");
#endif
			//bitweise-Oder verknüpfen: das 8. Bit setzen. Das bedeutet, dass 
			//sich im Aufzählungselement (EnumerationElement ein Relativsatz 
			//befand)
			return personIndex|ENUMERATION_ELEMENT_CONTAINS_REL_CLAUSE;
		}
	}
	#ifdef _DEBUG
	printf(" psv.size(): %u\n",psv.size());
	#endif
	for(//DWORD 
		dwRun=0;dwRun<psv.size();dwRun++)
	{
		ee.m_vecStrPlainText.push_back(psv.at(dwRun).m_Str);
		//if(dwRun>0 && dwRun<psv.size()-1)
		//	ee.m_plainText+=" ";
		//ee.m_plainText+=psv.at(dwRun).m_Str;
	}
	Range range;
	range.m_dwStart=psv.at(0).m_dwStart;
	range.m_dwEnd=psv.at(psv.size()-1).m_dwStart+psv.at(psv.size()-1).m_Str.GetLength();
	rVecRange.push_back(range);
	#ifdef _DEBUG
	printf("rVecRange.size(): %u range.m_dwStart: %u range.m_dwEnd: %u\n",
		rVecRange.size(),range.m_dwStart,range.m_dwEnd);
	printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv) ENDE return 3|128\n");
	#endif
	// nicht erkannt oder Eigenname, also einen Index von 3
	// zurückgeben (Eigennamen sind in der 3. Person Singular)
	// Beispiel: "Stefan is intelligent." ist gleichbedeutend mit:
	// "He is intelligent."
	//das 8. Bit des Rückgabewertes setzen: das bedeutet, dass das 
	//Aufzählungselement einen Eigennamen beinhaltet
	return THIRD_PERSON_SINGULAR|ENUMERATION_ELEMENT_CONTAINS_PROPER_NAME;
}

// bClause: gibt an, dass das Gerundium darauf geprüft werden soll, ob es ein Satzteil
// ist; bObject: gibt an, dass das Gerundium darauf geprüft werden soll, ob es ein 
// Objekt, dabei kann das Gerundium auch ein persönliches Subjekt haben, das vor 
// dem progressiven Partizip steht (zum Beispiel:"working")
// Beispielsatz für ein Gerundium, das ein Objekt ist: "I like him working hard."
// Gerundium: "him working hard"; "him" ist das persönliche Subjekt
BYTE ParseGerund(Gerund & gerund,BOOL bClause,BOOL bObject,
	const PositionCStringVector & psv,std::vector<Range> & rVecRange,
	const DWORD & dwFlags,BYTE bContainsFirstTokenOfSentence)
{
	#ifdef _DEBUG
	printf("BOOL ParseGerund(Gerund & gerund,BOOL bClause,BOOL bObject,const PositionCStringVector & psv) ANFANG\n");
	for(DWORD i=0;i<psv.size();i++)
		printf("psv.at(%d).m_Str: %s\n",i,psv.at(i).m_Str);
	#endif
	BOOL bPastParticiple=FALSE;
	BOOL bProgressive=FALSE;
	BYTE returnValue=2;
	CString strProgressive;
	DWORD dwStartOfNextElement=0;
	DWORD dwStartOfNegationOrProgressive=0;
	if(psv.size()<=0)
		return 0;
	WordNode * pWordNode=NULL;
	if(bClause)
	{
		for(DWORD dwRun=0;dwRun<psv.size() && g_bContinue;dwRun++)
		{
			//ein gültiges Gerundium, das einen Haupt- oder Nebensatz ersetzt,
			//muss eine Präposition am Anfang beinhalten
			CStringVector sv=GetBetweenAsCStringVector(psv,0,dwRun);
			if(bContainsFirstTokenOfSentence)
				MakeFirstLetterLower(sv.at(0));
			if((pWordNode=
				GetPointerToWordNodeWhereAnEnglishPrepositionIsEncapsulated(sv)))
			{
				dwStartOfNextElement=dwRun+1;
				gerund.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated=pWordNode;
			}
		}
		if(gerund.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated)
			bContainsFirstTokenOfSentence=FALSE;
	}
	if(bClause && !gerund.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated)
	{
		#ifdef _DEBUG
		printf("BOOL ParseGerund(Gerund & gerund,BOOL bClause,BOOL bObject,\
 const PositionCStringVector & psv) - return FALSE - keine Präposition\n");
		#endif
		return FALSE;
	}
	if(psv.size()>dwStartOfNextElement)
		if(bObject)
		{
			for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size() && g_bContinue;dwRun++)
			{
				CString str=psv.at(dwRun).m_Str;
				if(bContainsFirstTokenOfSentence)
					MakeFirstLetterLower(str);
				if(str==_T("not"))
				{
					gerund.m_bNegation=TRUE;
					dwStartOfNextElement=dwRun+1;
					dwStartOfNegationOrProgressive=dwRun;
					bContainsFirstTokenOfSentence=FALSE;
					break;
				}
			}
		}
		else
		{
			CString str=psv.at(dwStartOfNextElement).m_Str;
			if(bContainsFirstTokenOfSentence)
				MakeFirstLetterLower(str);
			if(str==_T("not"))
			{
				gerund.m_bNegation=TRUE;
				dwStartOfNegationOrProgressive=dwStartOfNextElement++;
				bContainsFirstTokenOfSentence=FALSE;
			}
		}
	//EnglishVerb ev;
	if(psv.size()>dwStartOfNextElement)
	{
		if(gerund.m_bNegation || !bObject)
		{
			CString str=psv.at(dwStartOfNextElement).m_Str;
#ifdef _DEBUG
			printf("gerund.m_bNegation || !bObject\n");
			printf("bContainsFirstTokenOfSentence: %u\n",
				bContainsFirstTokenOfSentence);
#endif
			if(bContainsFirstTokenOfSentence)
				MakeFirstLetterLower(str);
#ifdef _DEBUG
			printf("str: %s\n",str);
#endif
			if((pWordNode=
				GetPointerToWordNodeWhereAnEnglishProgressiveIsEncapsulated(
				str)))
			{
				bProgressive=TRUE;
				gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
				strProgressive=psv.at(dwStartOfNextElement).m_Str;
				bProgressive=TRUE;
				if(!gerund.m_bNegation)
					dwStartOfNegationOrProgressive=dwStartOfNextElement++;
				else
					dwStartOfNextElement++;
			}
		}
		else
		{
			for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
			{
				CString str=psv.at(dwRun).m_Str;
				if(bContainsFirstTokenOfSentence)
					MakeFirstLetterLower(str);
				if((pWordNode=
					GetPointerToWordNodeWhereAnEnglishProgressiveIsEncapsulated(
					str)))
				{
					bProgressive=TRUE;
					dwStartOfNextElement=dwRun+1;
					gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
					strProgressive=psv.at(dwRun).m_Str;
					dwStartOfNextElement=dwRun+1;
					if(!gerund.m_bNegation)
						dwStartOfNegationOrProgressive=dwRun;
					break;
				}
			}
		}
	}
	if(!bProgressive)
	{
		#ifdef _DEBUG
		printf("BOOL ParseGerund(Gerund & gerund,BOOL bClause,BOOL bObject,\
 const PositionCStringVector & psv) - return 0 - kein fortschreitendes Partizip\n");
		#endif
		return 0;
	}
	if(dwStartOfNegationOrProgressive>0)
	{
#ifdef _DEBUG
		printf("dwStartOfNegationOrProgressive: %u \n",dwStartOfNegationOrProgressive);
#endif
		Subject subject;
		std::vector<Range> vecRange;
		//auf persönliches Subjekt (zum Beispiel: "him") testen 
		if(!ParseSubject(subject,GetBetween(psv,0,dwStartOfNegationOrProgressive-1),
			TRUE,vecRange,TRUE,FALSE))
		{
		#ifdef _DEBUG
		printf("BOOL ParseGerund(Gerund & gerund,BOOL bClause,BOOL bObject,\
 const PositionCStringVector & psv) - return 0 - kein Subjekt\n");
		#endif
			return 0;
		}
		gerund.m_pPersonalSubject=new Subject(subject); // Standard-Kopierkonstruktor
		for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
			rVecRange.push_back(vecRange.at(dwRun));
	}
	if(psv.size()>dwStartOfNextElement)
	{
		#ifdef _DEBUG
		printf("pVocNode\n");
		#endif
		//gerund.m_predicate=ev.infinitive;
		if(strProgressive==_T("being"))
		{
			TRACE("psv.at(0)==_T(\"being\")\n");
			#ifdef _DEBUG
			printf("strProgressive==_T(\"being\") dwStartOfNextElement: %u\n",dwStartOfNextElement);
			#endif
			//pVocNode=NULL;
			if((pWordNode=GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(psv.at(dwStartOfNextElement).m_Str) ))
			{ // wenn Partizip Perfekt (zum Beispiel: "observed")
				TRACE("englisches Partizip Perfekt!!\n");
				#ifdef _DEBUG
				printf("englisches Partizip Perfekt!!\n");
				#endif
				dwStartOfNextElement++;
				gerund.m_bTense=SIMPLE_PRESENT;
				gerund.m_bActiveForm=FALSE;
				gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
				/*if(ParseByAgent(GetBetween(sv,2,sv.size()-1))
				return TRUE;*/
				#ifdef _DEBUG
				printf("BOOL ParseGerund(Gerund & gerund,const CStringVector & sv) ENDE return TRUE\n");
				#endif
				return returnValue;
			}
		}
		if(strProgressive==_T("having"))
		{
			#ifdef _DEBUG
			printf("psv.at(dwStartOfNextElement).m_Str==_T(\"having\")\n");
			#endif
			if(psv.at(dwStartOfNextElement).m_Str==_T("been"))
			{
				gerund.m_bActiveForm=FALSE; // Passivform
				dwStartOfNextElement++;
			}
			if(psv.size()>dwStartOfNextElement)
			{
				if((pWordNode=GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(psv.at(dwStartOfNextElement).m_Str)))
				{
					gerund.m_bTense=PRESENT_PERFECT;
					gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
					bPastParticiple=TRUE;
					dwStartOfNextElement++;
				}
				else
				{
					if(!gerund.m_bActiveForm)
					{
#ifdef _DEBUG
						printf("BYTE ParseGerund() - return 0\n");
#endif
						return 0;
					}
				}
			}
			/*if(!gerund.m_pVocNodeWhereAnEnglishVerbIsEncapsulated || dwStartOfNextElement+1!=psv.size())
			{
				#ifdef _DEBUG
				printf("BOOL ParseGerund(Gerund & gerund,const PositionCStringVector & psv) ENDE return FALSE - kein Verb\n");
				#endif
				return FALSE;
			}
			#ifdef _DEBUG
			printf("BOOL ParseGerund(Gerund & gerund,const PositionCStringVector & psv) ENDE return TRUE\n");
			#endif
			return returnValue;*/
		}
		#ifdef _DEBUG
		printf("nach: being und having\n");
		#endif
	}
//				dwStartOfNextElement++;
	if(typeid(*gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWord)==
		typeid(EnglishVerb) && psv.size()>dwStartOfNextElement)
	{
		#ifdef _DEBUG
		printf("Präposition testen\n");
		#endif
		EnglishVerb * ev=dynamic_cast<EnglishVerb *>(pWordNode->m_pWord);
		if(ev->m_strPreposition!=_T(""))
		{
			BOOL bValidPreposition=FALSE;
			CStringVector svPreposition=GetCStringVector2(ev->m_strPreposition);
			DWORD dwStart=0;
			for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size() && g_bContinue;dwRun++)
			{
				if(GetBetweenAsCStringVector(psv,dwStart,dwRun)==svPreposition)
				{
					dwStartOfNextElement=dwRun+1;
					bValidPreposition=TRUE;
				}
			}
			if(!bValidPreposition)
			{
				#ifdef _DEBUG
				printf("BOOL ParseGerund(Gerund & gerund,const CStringVector & sv) \
return FALSE - keine Präposition\n");
				#endif
				return FALSE;
			}
		}
	}
	if(psv.size()>dwStartOfNextElement)
	{
		#ifdef _DEBUG
		printf("Objekt testen\n");
		#endif
		if(gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated)
			if(gerund.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext)
			{
				GermanVerb * gv=dynamic_cast<GermanVerb *>(gerund.
					m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext->m_pWord);
				//GermanVerb * gv=GetGermanVocableOfEnglishVerb(ev.infinitive);
#ifdef _DEBUG
				printf("gv->m_bCase: %u\n",gv->m_bCase);
#endif
				if(gv->m_bCase==1 || gv->m_bCase==2)
				{
					DWORD dwEndOfValidObject=0;
					std::vector<Range> vecRangeOfValidObject;
					BOOL bValidObject=FALSE;
					for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size() && g_bContinue;dwRun++)
					{
						Object object;
						std::vector<Range> vecRange;
						if((returnValue=ParseObject(object,GetBetween(psv,
							dwStartOfNextElement,dwRun),TRUE,vecRange,TRUE)))
						{
							vecRangeOfValidObject=vecRange;
							if(gerund.m_pObject)
								delete gerund.m_pObject;
							gerund.m_pObject=new Object;
							*gerund.m_pObject=object;
							dwEndOfValidObject=dwRun;
							bValidObject=TRUE;
						}
					}
					if(bValidObject)
						dwStartOfNextElement=dwEndOfValidObject+1;
					if(dwEndOfValidObject==psv.size()-1)
					{
						for(DWORD dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
							rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
						#ifdef _DEBUG
						printf("rVecRange.size(): %u\n",rVecRange.size());
						printf("BOOL ParseGerund(Gerund & gerund,const CStringVector & sv,std::vector<Range> & rVecRange) ENDE return returnValue\n");
						#endif
						return returnValue;
					}
				}
			}
		//)==typeid
	}
	else
	{
		#ifdef _DEBUG
		printf("BOOL ParseGerund(Gerund & gerund,const CStringVector & sv) ENDE return returnValue (%u)\n",returnValue);
		#endif
		return returnValue;
	}
	if(psv.size()>dwStartOfNextElement) // ein Gerundium kann auch
	{ // eine Umstandsbestimmung des Ortes beinhalten
		// zum Beispiel: "I like living in Berlin." 
		// Gerundium: "living in Berlin"
		// Umstandsbestimmung des Ortes: "in Berlin"
		BOOL bValidPlace=FALSE;
		DWORD dwEndOfValidPlace=0;
		std::vector<Range> vecRangeOfValidPlace;
		BYTE returnValueOfValidPlace=0;
		for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size() && g_bContinue;dwRun++)
		{
			Place place;
			std::vector<Range> vecRange;
			BYTE ret=0;
			if((ret=ParsePlace(place,GetBetween(psv,dwStartOfNextElement,dwRun),vecRange,TRUE)))
			{
				vecRangeOfValidPlace=vecRange;
				if(gerund.m_pPlace)
					delete gerund.m_pPlace;
				gerund.m_pPlace=new Place(place);
				dwEndOfValidPlace=dwRun;
				bValidPlace=TRUE;
				returnValueOfValidPlace=ret;
			}
		}
		if(bValidPlace)
		{
			if(returnValueOfValidPlace==1)
				returnValue=1;
			dwStartOfNextElement=dwEndOfValidPlace+1;
			for(DWORD dwRun=0;dwRun<vecRangeOfValidPlace.size();dwRun++)
				rVecRange.push_back(vecRangeOfValidPlace.at(dwRun));
		}
		if(dwStartOfNextElement==psv.size())
		{
			#ifdef _DEBUG
			printf("rVecRange.size(): %u\n",rVecRange.size());
			printf("BOOL ParseGerund(Gerund & gerund,const CStringVector & sv,std::vector<Range> & rVecRange) ENDE return returnValue\n");
			#endif
			return returnValue;
		}
	}
	#ifdef _DEBUG
	printf("BOOL ParseGerund(Gerund & gerund,const CStringVector & sv) ENDE return FALSE\n");
	#endif
	return FALSE;
}

//Diese Funktion dient dazu, dass bei Frage-, Aufforderungs- 
//und Aussagesätzen
//die selbe Funktion aufgerufen wird, damit sich Änderungen in dieser
//Funktion auf Fragen UND Aussagen auswirkt (bessere Wartbarkeit).
//Diese Funktion wird von ParseClause, ParsQuestionClause und 
//ParseRelativeClause aufgerufen
//und liefert den Personalpronomen zurück, zu dem das gebeugte Verb gehört
//der Rückgabewert ist ein Byte, bei dem die Bits von 0 bis 6 angeben, für 
//welchen Personenindex das Prädikat korrekt ist. (alle Bits=0 =Fehler)
//Beispiel: "I like you." -> like passt für die folgenden Personenindizes:
//1 ("I like you.") ,2 ("You like me."), 4 ("We like you."), 5 ("You like you."),
//und 6 ("They like you.")
BYTE ParseNonBendPartOfPredicate(Predicate & predicate,BOOL bBe,
	const CStringVector & sv,	const DWORD & dwFlags//,const WordNode *
	//ein WordNode-Zeiger, der ein englisches Verb, dessen deutsches Pendant
	//eine gebeugte Verbform ist, die für eine korrekte Übersetzung notwendig 
	//ist ("have" bei "I have been." ist zum Beispiel NICHT notwendig, da 
	//man durch die Zeitform "Perfekt" weiss, dass man eine gebeugte Verbform 
	//von "haben" im Deutschen dafür braucht), kapselt
	//Wird zum Beispiel zugewiesen bei der Frage "Did you see him?":
	//dann wird auf die Vokabel "see" gezeigt
	//Wird eventuell von ParseBendPartOfPredicate zugewiesen
	//ergibt doch nur Sinn, wenn zugewiesen, oder?(ein Prädikat ist
	//doch sonst nicht gültig?)
	//pWordNodeWhereTheGermanTranslationIsABendVerb
	)
{
#ifdef _DEBUG
	printf("BYTE ParseNonBendPartOfPredicate(Predicate & "
		"predicate,BOOL bBe,const CStringVector & sv,const "
		"DWORD & dwFlags) - ANFANG\n");
	printf("predicate.m_bTense: %u\n",predicate.m_bTense);
	fflush(stdout);
#endif
	//Philosophie:anfangs falsch, dann in den passenden Verzweigungen, falls
	//das Token dort gültig ist, auf "true" gesetzt;bei restlichen
	//Verzweigungen bleibt der Wert "false"
	//Um nur 1 "return" zu haben (am Ende der Funktion)
	bool bValid=false;
	WordNode * pWordNodeWhereTheGermanTranslationIsABendVerb;
	pWordNodeWhereTheGermanTranslationIsABendVerb=predicate.
		m_pWordNodeWhereTheGermanTranslationIsABendVerb;
	//ergibt doch nur Sinn, wenn zugewiesen, oder?(ein Prädikat ist
	//doch sonst nicht gültig?)
	ASSERT(pWordNodeWhereTheGermanTranslationIsABendVerb!=NULL);
	//denn nach einem gebeugten Vollverb kann doch kein
	//Verb mehr folgen?!: He makes LIKE.
	if((sv.size()>0) && typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->
		m_pWord)==typeid(EnglishAuxiliaryVerb))
	{
    //Possible sentences at this point:
    //I will like you. :pWordNodeWhereTheGermanTranslationIsABendVerb== "will"
    //I can help you. :pWordNodeWhereTheGermanTranslationIsABendVerb== "can"
    //I am intelligent. :pWordNodeWhereTheGermanTranslationIsABendVerb== "am"
    //I am used. :pWordNodeWhereTheGermanTranslationIsABendVerb== "am"

		//VocNode * pVocNodeWhereAnEnglishConjunctionIsEncapsulated=NULL;
		//CStringVector strToken;
		//WordNode * pWordNodeWhereAnEnglishPredicateIsEncapsulated=NULL;
		//WordNode * pWordNodeWhereAnEnglishVerbIsEncapsulated=NULL;

		//WordNode * pWordNodeWhereTheGermanTranslationIsABendVerb=NULL;
		//ein WordNode-Zeiger, der ein englisches Verb, dessen deutsches Pendant
		//KEINE gebeugte Verbform ist, die für eine korrekte Übersetzung notwendig 
		//ist ("being" bei "I am being used." ist zum Beispiel NICHT notwendig, da 
		//man durch das Attribut "Aktivsatz" weiss, dass man eine gebeugte Verbform 
		//von "werden" im Deutschen dafür braucht), kapselt
		WordNode * pWordNodeWhereTheGermanTranslationIsANonBendVerb=NULL;
		//ss.tense=0;
    //bool bActiveClause=true;
		//predicate.m_MiddleEnumerationElements.clear(); // Vektor leeren (enthält möglicherweise 
		// bereits Elemente durch einen vorherigen Aufruf dieser Funktion
		bool bAdverbOfFrequencyFound=false;
		bool bPrepositionFound=false;
		//BYTE b3rdPerson=FALSE;
		//BYTE bAux=FALSE;
		//BYTE bDo=FALSE;
		//BYTE bAdverbOfFrequency=FALSE;
		BYTE bEmpathicDo=FALSE;
		//BYTE bHave=FALSE;
		BYTE bManner=FALSE;
		//BYTE bNegation=FALSE;
		//BYTE bNot=FALSE;
		//BYTE bVerb=FALSE;
		//BYTE bFuture=FALSE;
		BYTE bActiveClause=TRUE;
		//BYTE bTo=FALSE;
		BYTE byTense=predicate.m_bTense;
		BYTE returnPersonIndex=0//,personIndex
      ;
		CString strToken;
		//am Anfang aus den höchstmöglichen Wert setzen
		DWORD dwPositionOfBendAuxiliary=ULONG_MAX;
		EnglishAuxiliaryVerb * peavFinite;
		
		peavFinite=dynamic_cast<EnglishAuxiliaryVerb*>
			(pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
		//CString token;
		#ifdef _DEBUG
		{
			for(DWORD i=0;i<sv.size();i++)
			{
				CString token=sv.at(i);
				TRACE("sv.at(%d): %s\n",i,token);
				printf("sv.at(%d): %s\n",i,token);
			}
		}
		#endif
		//predicate.m_adverbs.clear();
		//for(int i=0;i<ss.m_adverbs.size();i++
		BOOL bAuxiliaryVerbOccured=FALSE;
		for(//CStringVector::const_iterator
			DWORD dwRun=0;dwRun<sv.size() && g_bContinue;++dwRun)
		{
			//strToken NICHT erst hier deklarieren (bessere 
			//Laufzeit, denn deklarieren kostet Zeit,denke ich)
			strToken=sv.at(dwRun);
			//wichtig!:für jeden Schleifendurchlauf erneut setzen
			bValid=false;
			if(!(dwFlags&CASE_SENSITIVE)==1)
				strToken.MakeLower();
			//in MFC/Windows the TRACE macro has a variable # of args--I do 
			//not know how to enable variable numbers of args for the same macro name.
			//So I exclude it from non-Windows.
		#ifdef _WINDOWS
			TRACE("strToken: %s\n",strToken);
		#endif
			#ifdef _DEBUG
			printf("strToken: %s\n",strToken);
      if(strToken==_T("dialled"))
        strToken=strToken;
			#endif
			WordNode * pWordNode=NULL;
			BOOL bValidToken=FALSE;
			BYTE byType;
			if((pWordNode=
				//GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(token)))
				GetPointerToWordNodeWhereANonBendEnglishVerbIsEncapsulated(
					strToken,byType)))
      {
        //Possible sentences at this point:
        // finite auxiliary verb followed by a 
        // non bend(e.g. (present|past) participle|infinitive) English verb:
        //I will like you. :pWordNode== "like"
        //I can help you. :pWordNode== "help"
        //I am going. :pWordNode== "going"
        //I am used. :pWordNode== "used", passive
				BOOL bAuxiliaryVerb=FALSE;
			#ifdef _DEBUG
				printf("pVocNode!=NULL\n");
			#endif
				if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))
				{
        //Possible sentences at this point:
        // finite auxiliary verb followed by a 
        // non bend(e.g. (present|past) participle|infinitive) English auxiliary verb:
        //I will be you. :pWordNode== "be"
        //I can can you. :pWordNode== "can" (false/incorrect sentence)
        //I am having. :pWordNode== "having"
        //I am will. :pWordNode== "will", (false/incorrect sentence)
			#ifdef _DEBUG
					printf("EnglishAuxiliaryVerb\n");
			#endif
					//strToken.push_back(token);
					EnglishAuxiliaryVerb * englishauxiliaryverbNotFinite=dynamic_cast
            <EnglishAuxiliaryVerb*>(pWordNode->m_pWord);
					if(englishauxiliaryverbNotFinite //&& 
						////denn nach einem gebeugten Vollverb kann doch kein
						////Verb mehr folgen?!: He makes LIKE.
						//typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->
						//m_pWord)==typeid(EnglishAuxiliaryVerb)
						)
					{
            //TODO use eavFinite from above instead of eavBend
						EnglishAuxiliaryVerb * eavBend=dynamic_cast<EnglishAuxiliaryVerb*>
							(pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
						if(eavBend)
						{
							////noch nicht zugewiesen
              //Wenn bereits ein nicht-gebeugtes(e.g. (present|past) participle|infinitive)
              //English in dieser Programm-Funktion gefunden wurde.
							if(pWordNodeWhereTheGermanTranslationIsANonBendVerb)
							{
                //Possible sentences at this point:
                // finite auxiliary verb followed by a
                // non bend(e.g. (present|past) participle|infinitive) English verb followed by a 
                // non bend English auxiliary verb:
                //I will be having you. :pWordNode== "having",
                //  pWordNodeWhereTheGermanTranslationIsANonBendVerb=="be"
                //I can can can you. :pWordNode== "can" (false/incorrect sentence)
                //I am having. :pWordNode== "having"
                //I was gone can. :pWordNode== "can", (false/incorrect sentence)

                //wenn das englische Verb, dessen 
								//deutsches Pendant
								//KEINE gebeugte Verbform ist, die für eine korrekte 
								//Übersetzung notwendig 
								//ist ("being" bei "I am being used." ist zum Beispiel 
								//NICHT notwendig, da 
								//man durch das Attribut "Aktivsatz" weiss, dass man 
								//eine gebeugte Verbform 
								//von "werden" im Deutschen dafür braucht) und von einem
								//WordNode-Zeiger gekapselt wird, ein Hilfsverb ist
								if(typeid(
									*pWordNodeWhereTheGermanTranslationIsANonBendVerb->
									m_pWord)==typeid(EnglishAuxiliaryVerb))
								{
                  //Possible sentences at this point:
                  // finite auxiliary verb followed by a
                  // non bend(e.g. (present|past) participle|infinitive) English auxiliary verb followed by a 
                  // non bend English auxiliary verb:
                  //I will be having you. :pWordNode== "having",
                  //  pWordNodeWhereTheGermanTranslationIsANonBendVerb=="be"
                  //I can can can you. :pWordNode== "can" (false/incorrect sentence)
                  //I can have been. :pWordNode== "having"
                  //I was will can. :pWordNode== "can", (false/incorrect sentence)
									EnglishAuxiliaryVerb * eavNotFinite=dynamic_cast
										<EnglishAuxiliaryVerb *>
										(pWordNodeWhereTheGermanTranslationIsANonBendVerb->
										m_pWord);
									if(eavNotFinite)
									{
										if(byTense==FUTURE && strToken=="been" &&
											eavNotFinite->m_strWords[EAV_ARRAY_INDEX_FOR_INFINITIVE]==_T("have"))
                    //If for instance: "I will have been."
										{
											byTense=FUTURE_PERFECT;
                      //NOW point to the word "been" if for instance: "I will have been."
                      //(because "have" is redundant because the tense is set yet.
											pWordNodeWhereTheGermanTranslationIsANonBendVerb=
												pWordNode;
											bValid=true;
										}
									}
								}
							}//pWordNodeWhereTheGermanTranslationIsANonBendVerb<>NULL
							else//if(pWordNodeWhereTheGermanTranslationIsANonBendVerb)
							//noch nicht zugewiesen
							//Zeitform ist bis jetzt "Präsens" oder "Präteritum"
							{
                //Possible sentences at this point:
                // finite auxiliary verb followed by a
                // non bend(e.g. (present|past) participle|infinitive) English auxiliary verb:
                //I will be you. :pWordNode== "having",
                //  pWordNodeWhereTheGermanTranslationIsANonBendVerb=="be"
                //I can can you. :pWordNode== "can" (false/incorrect sentence)
                //I am having. :pWordNode== "having"
                //I have been. :pWordNode== "can", (false/incorrect sentence)

                if(eavBend->m_strWords[EAV_ARRAY_INDEX_FOR_INFINITIVE]==_T("be"))
                {
									//z.B. "I am having fun.", "Were you being used?"
									if(strToken==_T("having")// || strToken==_T("being"
										)
									{
										bValid=true;
										pWordNodeWhereTheGermanTranslationIsANonBendVerb=
											pWordNode;
                    //Ex.: predicate till now: "am" (simple present)
										if(byTense==SIMPLE_PRESENT)
                      //Ex.: predicate now: "am having"
											byTense=PRESENT_PROGRESSIVE;
                    //Ex.: predicate till now: "was" (simple present)
										if(byTense==SIMPLE_PAST)
                      //Ex.: predicate now: "was having"
											byTense=PAST_PROGRESSIVE;
									}//if(strToken==_T("having")
                  //Use "else in order to avoid comparison(->faster) if strToken==_T("having")
                  //matched right before.
									else 
                    if(strToken==_T("being")
										  )
									  {
										  bValid=true;
                      //The predicate may be "am being"->passive construction.
                      bActiveClause=false;
										  pWordNodeWhereTheGermanTranslationIsANonBendVerb=
											  pWordNode;
                      //Ex.: predicate till now: "am" (simple present)
										  if(byTense==SIMPLE_PRESENT)
                        //Ex.: predicate now: "am having"
											  byTense=PRESENT_PROGRESSIVE;
                      //Ex.: predicate till now: "was" (simple present)
										  if(byTense==SIMPLE_PAST)
                        //Ex.: predicate now: "was having"
											  byTense=PAST_PROGRESSIVE;
									  }//if(strToken==_T("being")
                }//if(eavBend->m_strWords[EAV_ARRAY_INDEX_FOR_INFINITIVE]==_T("be"))
                //Use "else in order to avoid comparison(->faster) if the comparison
                //matched right before.
								else 
                  if(eavBend->m_strWords[EAV_ARRAY_INDEX_FOR_INFINITIVE]==_T("have"))
									  //z.B. I have had fun. Had you been used?
									  if(strToken==_T("had") || strToken==_T("been"))
									  {
										  bValid=true;
										  pWordNodeWhereTheGermanTranslationIsANonBendVerb=
											  pWordNode;
										  //have/has
										  if(byTense==SIMPLE_PRESENT)
											  //->have/has had/ have/has been
											  byTense=PRESENT_PERFECT;
										  //had
										  if(byTense==SIMPLE_PAST)
											  //->had had/ had been
											  byTense=PAST_PERFECT;
									  }//if(strToken==_T("had") || strToken==_T("been"))
								if(byTense==FUTURE && 
									//nach "will" muss ein Infinitiv folgen
									byType==INFINITIVE_OR_PAST_PARTICIPLE)
								{
									bValid=true;
									pWordNodeWhereTheGermanTranslationIsANonBendVerb=
										pWordNode;
								}
							}//pWordNodeWhereTheGermanTranslationIsANonBendVerb==NULL
						}//if(eavBend)
					}//if(englishauxiliaryverbNotFinite)
								//z.B. Did you have a party?
						//if(englishauxiliaryverbNotFinite->m_strWords[0]==_T("hav
				}//if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))
				else
					//aktuelles Token ist kein Hilfsverb, also Vollverb
				{
					EnglishVerb * ev=dynamic_cast<EnglishVerb*>
						(pWordNode->m_pWord);
					if(ev)
					{
						//wenn das englische Verb, dessen deutsches Pendant
						//eine gebeugte Verbform ist, die für eine korrekte 
						//Übersetzung notwendig 
						//ist ("have" bei "I have been." ist zum Beispiel NICHT 
						//notwendig, da 
						//man durch die Zeitform "Perfekt" weiss, dass man eine 
						//gebeugte Verbform 
						//von "haben" im Deutschen dafür braucht) und von einem
						//WordNode-Zeiger gekapselt wird, ein Hilfsverb ist
						//An dieser Stelle kann der Zeiger auch euf ein Vollverb zeigen:
						//z.B. bei I do not like you.:zeigt zuerst auf "do", dann
						//auf "like"
						//if(typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->
						//	m_pWord)==typeid(EnglishAuxiliaryVerb))
						{
							EnglishAuxiliaryVerb * eavFinite=dynamic_cast
								<EnglishAuxiliaryVerb *>
								(pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
							if(eavFinite)
							{

								//wenn der WordNode-Zeiger, der ein englisches Verb, dessen 
								//deutsches Pendant
								//KEINE gebeugte Verbform ist, die für eine korrekte 
								//Übersetzung notwendig 
								//ist ("being" bei "I am being used." ist zum Beispiel 
								//NICHT notwendig, da 
								//man durch das Attribut "Aktivsatz" weiss, dass man 
								//eine gebeugte Verbform 
								//von "werden" im Deutschen dafür braucht), kapselt,
								//schon zugewiesen wurde, ...
								if(pWordNodeWhereTheGermanTranslationIsANonBendVerb)
								{
									//...darf die Zeitform nur "Zukunft", "Perfekt" oder 
									//"Plusquamperfekt" sein,

									//wenn das englische Verb, dessen 
									//deutsches Pendant
									//KEINE gebeugte Verbform ist, die für eine korrekte 
									//Übersetzung notwendig 
									//ist ("being" bei "I am being used." ist zum Beispiel 
									//NICHT notwendig, da 
									//man durch das Attribut "Aktivsatz" weiss, dass man 
									//eine gebeugte Verbform 
									//von "werden" im Deutschen dafür braucht) und von einem
									//WordNode-Zeiger gekapselt wird, ein Hilfsverb ist
									if(typeid(
										*pWordNodeWhereTheGermanTranslationIsANonBendVerb->
										m_pWord)==typeid(EnglishAuxiliaryVerb))
									{
										EnglishAuxiliaryVerb * eavNotFinite=dynamic_cast
											<EnglishAuxiliaryVerb *>
											(pWordNodeWhereTheGermanTranslationIsANonBendVerb->
											m_pWord);
										if(eavNotFinite)
										{
											//wenn das nichtgebeugte Hilfsverb "be" ist
											if(eavNotFinite->m_strWords[0]==_T("be"))
											{

												//wenn das aktuelle Token identisch mit
												//dem Partizip Perfekt ist
												if(byType==PAST_PARTICIPLE || 
													//wenn der Infinitiv mit dem Partizip Perfekt 
													//identisch ist
													byType==INFINITIVE_OR_PAST_PARTICIPLE)
												{
													//am being,was being,will be,will have been,
													//have been, had been
													if((byTense==PRESENT_PROGRESSIVE || byTense==
														PAST_PROGRESSIVE || byTense==FUTURE || byTense==
														FUTURE_PERFECT || byTense==PRESENT_PERFECT || 
														byTense==PAST_PERFECT) && bActiveClause)
													{
														//TELEKOLLEG I Englisch Übungsband Seite 38:
														//Passiv
														//has been built
														//had been built
														//will be built
														//will have been built

														//Beispiel: "I am being hit."->"Ich werde (gerade) 
														//geschlagen." / "I was being hit."->"Ich wurde 
														//(gerade) geschlagen." / "I will be hit."->"Ich
														//werde geschlagen werden." / "I will have been hit."
														//->"Ich werde geschlagen worden sein." / "I have 
														//been hit."->"Ich bin geschlagen worden." / "I had 
														//been hit."->"Ich war geschlagen worden."
														bActiveClause=FALSE;
														//dem WordNode-Zeiger, der ein englisches Verb, 
														//dessen deutsches Pendant
														//KEINE gebeugte Verbform ist, die für eine 
														//korrekte Übersetzung notwendig 
														//ist ("being" bei "I am being used." ist zum 
														//Beispiel NICHT notwendig, da 
														//man durch das Attribut "Aktivsatz" weiss, 
														//dass man eine gebeugte Verbform 
														//von "werden" im Deutschen dafür braucht), 
														//kapselt,
														//die Adresse des Zeigers "pWordNode" zuweisen
														pWordNodeWhereTheGermanTranslationIsANonBendVerb=
															pWordNode;
														//bPredicateDidntChangeYet=FALSE;
														bValid=TRUE;
													}
													//if( 
													//wenn das Hilfsverb ein modales Hilfsverb ist
													//(modale Hilfsverben können nicht alleine stehen
													//und müssen direkt oder indirekt von einem 
													//Vollverb gefolgt werden)
													//	!eavFinite->m_bIntegral)
													{
														//z.B."can be" oder "am","is","was"
														if((byTense==SIMPLE_PRESENT || byTense==
															//PRESENT_PERFECT 
															SIMPLE_PAST) && bActiveClause)
														{
															//Beispiel: "I can be hit."->"Ich kann 
															//geschlagen werden." / "I can have been 
															//hit."->"Ich kann geschlagen worden sein."
															bActiveClause=FALSE;
															//dem WordNode-Zeiger, der ein 
															//englisches Verb, dessen 
															//deutsches Pendant
															//KEINE gebeugte Verbform ist, die für 
															//eine korrekte Übersetzung notwendig 
															//ist ("being" bei "I am being used." 
															//ist zum Beispiel NICHT notwendig, da 
															//man durch das Attribut "Aktivsatz" 
															//weiss, dass man eine gebeugte Verbform 
															//von "werden" im Deutschen dafür braucht), 
															//kapselt, die Adresse des Zeigers 
															//"pWordNode" zuweisen
															pWordNodeWhereTheGermanTranslationIsANonBendVerb=
																pWordNode;
															//bPredicateDidntChangeYet=FALSE;
															bValid=TRUE;
														}//simple present|past and active clause
													}
												}//matches past participle or inf. and past participle of verb

												//wenn das aktuelle Token identisch mit einem fortschreitendem 
												//Partizip (Bespiel: 'walkING') ist...
												if(byType==PARTICIPLE_PROGRESSIVE)
												{
													//die bisherige Zeitform war "Zukunft", die 
													//Wörter "will" und "be" sind im Prädikat
													//Beispiel: 
													//"I will be"
													if(byTense==FUTURE)
													{
														//daraus wird jetzt "fortschreitende Zukunft";
														// Beispiel: "I will be walking"
														byTense=FUTURE_CONTINOUS;
														bValid=TRUE;
													}
													//die bisherige Zeitform war "Perfekt";
													//Beispiel: "I have been"
													if(byTense==PRESENT_PERFECT)
													{
														//daraus wird jetzt "fortschreitendes Perfekt";
														// Beispiel: "I have been walking"
														byTense=PRESENT_PERFECT_PROGRESSIVE;
														bValid=TRUE;
													}
													//die bisherige Zeitform war "Plusquamperfekt";
													//Beispiel: "I had been"
													if(byTense==PAST_PERFECT)
													{
														//daraus wird jetzt "fortschreitendes Plusquamperfekt";
														// Beispiel: "I had been walking"
														byTense=PAST_PERFECT_PROGRESSIVE;
														bValid=TRUE;
													}
													if(bValid)
														//dem WordNode-Zeiger, der ein englisches Verb, dessen 
														//deutsches Pendant
														//KEINE gebeugte Verbform ist, die für eine korrekte 
														//Übersetzung notwendig 
														//ist ("being" bei "I am being used." ist zum Beispiel 
														//NICHT notwendig, da 
														//man durch das Attribut "Aktivsatz" weiss, dass man 
														//eine gebeugte Verbform 
														//von "werden" im Deutschen dafür braucht), kapselt,
														//die Adresse des Zeigers "pWordNode" zuweisen
														pWordNodeWhereTheGermanTranslationIsANonBendVerb=
															pWordNode;
												}//if(byType==PARTICIPLE_PROGRESSIVE)
											}//if(eavNotFinite->m_strWords[0]==_T("be"))
											else if(eavNotFinite->m_strWords[0]==_T("have"))
											{
												//wenn der Infitinitiv mit dem Partizip Perfekt
												//identisch ist
												if(byType==PAST_PARTICIPLE || 
													INFINITIVE_OR_PAST_PARTICIPLE)
												{
													if(byTense==FUTURE)
													{
														//Beispiel: "I will have hit."->"Ich werde geschlagen 
														//haben."

														//dem WordNode-Zeiger, der ein englisches Verb, 
														//dessen deutsches Pendant
														//KEINE gebeugte Verbform ist, die für eine 
														//korrekte Übersetzung notwendig 
														//ist ("being" bei "I am being used." ist zum 
														//Beispiel NICHT notwendig, da 
														//man durch das Attribut "Aktivsatz" weiss, 
														//dass man eine gebeugte Verbform 
														//von "werden" im Deutschen dafür braucht), 
														//kapselt,
														//die Adresse des Zeigers "pWordNode" zuweisen
														pWordNodeWhereTheGermanTranslationIsANonBendVerb=
															pWordNode;
														byTense=FUTURE_PERFECT;
														//bPredicateDidntChangeYet=FALSE;
														bValid=TRUE;
													}//if(byTense==FUTURE)
												  if(
													//wenn das Hilfsverb ein modales Hilfsverb ist
													//(modale Hilfsverben können nicht alleine stehen
													//und müssen direkt oder indirekt von einem 
													//Vollverb gefolgt werden)
														!eavFinite->m_bIntegral)
													{
														//z.B. "can have"
														if(byTense==SIMPLE_PRESENT)
														{
															//Beispiel: "I can have hit." ("Ich kann 
															//geschlagen haben.")

															//dem WordNode-Zeiger, der ein 
															//englisches Verb, dessen 
															//deutsches Pendant
															//KEINE gebeugte Verbform ist, die für 
															//eine korrekte Übersetzung notwendig 
															//ist ("being" bei "I am being used." 
															//ist zum Beispiel NICHT notwendig, da 
															//man durch das Attribut "Aktivsatz" 
															//weiss, dass man eine gebeugte Verbform 
															//von "werden" im Deutschen dafür braucht), 
															//kapselt, die Adresse des Zeigers 
															//"pWordNode" zuweisen
															pWordNodeWhereTheGermanTranslationIsANonBendVerb=
																pWordNode;
															byTense=PRESENT_PERFECT;
															//bPredicateDidntChangeYet=FALSE;
															bValid=TRUE;
														}//if(byTense==SIMPLE_PRESENT)
													}//if(!eavFinite->m_bIntegral)
												}//matches past participle or inf. and past participle of verb
											}//else if(eavNotFinite->m_strWords[0]==_T("have"))
										}//if(eavNotFinite)
									}//pWordNodeWhereTheGermanTranslationIsANonBendVerb is English aux. verb
								}//if(pWordNodeWhereTheGermanTranslationIsANonBendVerb)
								else
									//pWordNodeWhereTheGermanTranslationIsANonBendVerb==NULL
								{
                  LOG2N("23.05.2008 10.41.32 "<<byType,"PNBPOP 0 T"<<byType);
									if(byType==INFINITIVE || byType==
                    //If the infinitive and past participle equals each other.
										INFINITIVE_OR_PAST_PARTICIPLE)
									{
										if(byTense==FUTURE)//if tense till current token is future
										{
											//dem WordNode-Zeiger, der ein englisches Verb, 
											//dessen deutsches Pendant
											//KEINE gebeugte Verbform ist, die für eine 
											//korrekte Übersetzung notwendig 
											//ist ("being" bei "I am being used." ist zum 
											//Beispiel NICHT notwendig, da 
											//man durch das Attribut "Aktivsatz" weiss, 
											//dass man eine gebeugte Verbform 
											//von "werden" im Deutschen dafür braucht), 
											//kapselt,
											//die Adresse des Zeigers "pWordNode" zuweisen
											pWordNodeWhereTheGermanTranslationIsANonBendVerb=
												pWordNode;
											//bPredicateDidntChangeYet=FALSE;
											bValid=TRUE;
										}//if(byTense==FUTURE)
										
										if(
										//wenn das Hilfsverb ein modales Hilfsverb ist
										//(modale Hilfsverben können nicht alleine stehen
										//und müssen direkt oder indirekt von einem 
										//Vollverb gefolgt werden)
											!eavFinite->m_bIntegral)
										{
											//dem WordNode-Zeiger, der ein englisches Verb, dessen 
											//deutsches Pendant
											//KEINE gebeugte Verbform ist, die für eine korrekte 
											//Übersetzung notwendig 
											//ist ("being" bei "I am being used." ist zum Beispiel 
											//NICHT notwendig, da 
											//man durch das Attribut "Aktivsatz" weiss, dass man 
											//eine gebeugte Verbform 
											//von "werden" im Deutschen dafür braucht), kapselt,
											//die Adresse des Zeigers "pWordNode" zuweisen
											pWordNodeWhereTheGermanTranslationIsANonBendVerb=
												pWordNode;
											bValid=TRUE;
										}//if(!eavFinite->m_bIntegral)
									}//matches past participle or inf. and past participle of verb
									//wenn das Hilfsverb "be" ist
									if(eavFinite->m_strWords[0]==_T("be"))
									{
										//wenn der Infitinitiv mit dem Partizip Perfekt
										//identisch ist
										if(byType==//INFINITIVE
											PAST_PARTICIPLE|| byType==
											INFINITIVE_OR_PAST_PARTICIPLE)
										{
											//I am, I was, I am being, I was being
											if((byTense==SIMPLE_PRESENT || byTense==SIMPLE_PAST
												|| byTense==PRESENT_PROGRESSIVE || byTense==
													PAST_PROGRESSIVE) && bActiveClause)
											{
												//Beispiel: "I am hit."->"Ich werde geschlagen." /
												//"I was hit."->"Ich wurde geschlagen."
												bActiveClause=FALSE;
												//die gebeugte Verbform wird nicht mehr benötigt,
												//sie ergibt sich aus der Zeitform und ob es ein
												//aktives oder passives Prädikat ist:

												//dem WordNode-Zeiger, der ein englisches Verb, 
												//dessen deutsches Pendant
												//eine gebeugte Verbform ist, die für eine 
												//korrekte Übersetzung notwendig 
												//ist ("have" bei "I have been." ist zum Beispiel 
												//NICHT notwendig, da 
												//man durch die Zeitform "Perfekt" weiss, dass 
												//man eine gebeugte Verbform 
												//von "haben" im Deutschen dafür braucht), 
												//kapselt, den Wert "NULL" zuweisen
												//pWordNodeWhereTheGermanTranslationIsABendVerb=
												//	NULL;
												//dem WordNode-Zeiger, der ein englisches Verb, 
												//dessen deutsches Pendant
												//KEINE gebeugte Verbform ist, die für eine 
												//korrekte Übersetzung notwendig 
												//ist ("being" bei "I am being used." ist zum 
												//Beispiel NICHT notwendig, da 
												//man durch das Attribut "Aktivsatz" weiss, 
												//dass man eine gebeugte Verbform 
												//von "werden" im Deutschen dafür braucht), 
												//kapselt,
												//die Adresse des Zeigers "pWordNode" zuweisen
												pWordNodeWhereTheGermanTranslationIsANonBendVerb=
													pWordNode;
												//bPredicateDidntChangeYet=FALSE;
												bValid=TRUE;
											}
										}//matches past participle or inf. and past participle of verb
										//wenn das aktuelle Token identisch mit einem fortschreitendem 
										//Partizip (Bespiel: 'walkING') ist...
										if(byType==PARTICIPLE_PROGRESSIVE)
										{
											//die bisherige Zeitform war "Präsens"; Beispiel: "I am"
											if(byTense==SIMPLE_PRESENT)
											{
													//daraus wird jetzt "fortschreitendes Präsens";
													// Beispiel: "I am walking"
													byTense=PRESENT_PROGRESSIVE;
												bValid=TRUE;
											}
											//die bisherige Zeitform war "Präteritum";
											//Beispiel: "I was"
											if(byTense==SIMPLE_PAST)
											{
												//daraus wird jetzt "fortschreitendes Präteritum";
												// Beispiel: "I was walking"
												byTense=PAST_PROGRESSIVE;
												bValid=TRUE;
											}
											if(bValid)
												//dem WordNode-Zeiger, der ein englisches Verb, 
												//dessen deutsches Pendant
												//eine gebeugte Verbform ist, die für eine 
												//korrekte Übersetzung notwendig 
												//ist ("have" bei "I have been." ist zum Beispiel 
												//NICHT notwendig, da 
												//man durch die Zeitform "Perfekt" weiss, dass 
												//man eine gebeugte Verbform 
												//von "haben" im Deutschen dafür braucht), 
												//kapselt, die Adresse des Zeigers "pWordNode" 
												//zuweisen
												pWordNodeWhereTheGermanTranslationIsABendVerb=
													pWordNode;
										}//if(byType==PARTICIPLE_PROGRESSIVE)
									}//if(eavFinite->m_strWords[0]==_T("be"))
									//wenn das Hilfsverb "have" ist
									else if(eavFinite->m_strWords[0]==_T("have"))
									{
										//wenn der Infinitiv mit dem Partizip Perfekt
										//identisch ist
										if(byType==PAST_PARTICIPLE || byType==
											INFINITIVE_OR_PAST_PARTICIPLE)
										{
											//Wenn die aktuelle Zeit "Präsens" ist: z.B. "I have".
											if(byTense==SIMPLE_PRESENT)
											{
												//Beispiel: "I have hit." ("Ich habe geschlagen.")
												//die gebeugte Verbform wird nicht mehr benötigt,
												//sie ergibt sich aus der Zeitform und ob es ein
												//aktives oder passives Prädikat ist:

												//dem WordNode-Zeiger, der ein englisches Verb, 
												//dessen deutsches Pendant
												//eine gebeugte Verbform ist, die für eine 
												//korrekte Übersetzung notwendig 
												//ist ("have" bei "I have been." ist zum Beispiel 
												//NICHT notwendig, da 
												//man durch die Zeitform "Perfekt" weiss, dass 
												//man eine gebeugte Verbform 
												//von "haben" im Deutschen dafür braucht), 
												//kapselt, den Wert "NULL" zuweisen
												//pWordNodeWhereTheGermanTranslationIsABendVerb=
												//	NULL;
												//dem WordNode-Zeiger, der ein englisches Verb, 
												//dessen deutsches Pendant
												//KEINE gebeugte Verbform ist, die für eine 
												//korrekte Übersetzung notwendig 
												//ist ("being" bei "I am being used." ist zum 
												//Beispiel NICHT notwendig, da 
												//man durch das Attribut "Aktivsatz" weiss, 
												//dass man eine gebeugte Verbform 
												//von "werden" im Deutschen dafür braucht), 
												//kapselt,
												//die Adresse des Zeigers "pWordNode" zuweisen
												pWordNodeWhereTheGermanTranslationIsANonBendVerb=
													pWordNode;
												byTense=PRESENT_PERFECT;
												//bPredicateDidntChangeYet=FALSE;
												bValid=TRUE;
											}
											//Wenn die aktuelle Zeit "Präteritum" ist: z.B. "I had".
											if(byTense==SIMPLE_PAST)
											{
												//Beispiel: "I had hit." ("Ich hatte geschlagen.")
												//die gebeugte Verbform wird nicht mehr benötigt,
												//sie ergibt sich aus der Zeitform und ob es ein
												//aktives oder passives Prädikat ist:

												//dem WordNode-Zeiger, der ein englisches Verb, 
												//dessen deutsches Pendant
												//eine gebeugte Verbform ist, die für eine 
												//korrekte Übersetzung notwendig 
												//ist ("have" bei "I have been." ist zum Beispiel 
												//NICHT notwendig, da 
												//man durch die Zeitform "Perfekt" weiss, dass 
												//man eine gebeugte Verbform 
												//von "haben" im Deutschen dafür braucht), 
												//kapselt, den Wert "NULL" zuweisen
												//pWordNodeWhereTheGermanTranslationIsABendVerb=NULL;
												//dem WordNode-Zeiger, der ein englisches Verb, 
												//dessen deutsches Pendant
												//KEINE gebeugte Verbform ist, die für eine 
												//korrekte Übersetzung notwendig 
												//ist ("being" bei "I am being used." ist zum 
												//Beispiel NICHT notwendig, da 
												//man durch das Attribut "Aktivsatz" weiss, 
												//dass man eine gebeugte Verbform 
												//von "werden" im Deutschen dafür braucht), 
												//kapselt,
												//die Adresse des Zeigers "pWordNode" zuweisen
												pWordNodeWhereTheGermanTranslationIsANonBendVerb=
													pWordNode;
												byTense=PAST_PERFECT;
												//bPredicateDidntChangeYet=FALSE;
												bValid=TRUE;
											}//if(byTense==SIMPLE_PAST)
										}//matches past participle or inf. and past participle of verb
									}//if(eavFinite->m_strWords[0]==_T("have"))
									//wenn das Hilfsverb "do" ist
									if(eavFinite->m_strWords[0]==_T("do"))
									{
										//wenn das Prädikat NICHT verneint ist, ...
										if(!predicate.m_bNegation)
											//dann handelt es sich um ein empathisches "do"
											//"I do like you."->"Ich mag dich wirklich."
											bEmpathicDo=TRUE;
										//dem WordNode-Zeiger, der ein englisches Verb, 
										//dessen deutsches Pendant
										//eine gebeugte Verbform ist, die für eine 
										//korrekte Übersetzung notwendig 
										//ist ("have" bei "I have been." ist zum Beispiel 
										//NICHT notwendig, da 
										//man durch die Zeitform "Perfekt" weiss, dass 
										//man eine gebeugte Verbform 
										//von "haben" im Deutschen dafür braucht), 
										//kapselt, die Adresse des Zeigers "pWordNode" 
										//zuweisen
										pWordNodeWhereTheGermanTranslationIsABendVerb=
											pWordNode;
										bValid=TRUE;
									}//if(eavFinite->m_strWords[0]==_T("do"))
										//wenn das Hilfsverb "be" ist
									//if(eav->m_strWords[0]==_T("have"))
								}//pWordNodeWhereTheGermanTranslationIsANonBendVerb==NULL
							}//if(eavFinite)
						}//(no condition)
					}//if(ev)
				}//else-branch of "if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))"
			}//if the current token is a NonBendEnglishVerb
			else
			  //eine Preposition, die fest zu einem Verb gehört, darf
			  //nur 1x vorkommen
			  if(!bPrepositionFound)
			  {
				  const WordNode * pWordNodeEnglishVerb;
				  if(pWordNodeWhereTheGermanTranslationIsANonBendVerb)
					  pWordNodeEnglishVerb=
						  pWordNodeWhereTheGermanTranslationIsANonBendVerb;
				  else
					  //wenn der Zeiger, der ein englisches Verb, dessen deutsches 
					  //Pendant
					  //eine gebeugte Verbform ist, die für eine korrekte 
					  //Übersetzung notwendig 
					  //ist ("have" bei "I have been." ist zum Beispiel NICHT 
					  //notwendig, da 
					  //man durch die Zeitform "Perfekt" weiss, dass man eine 
					  //gebeugte Verbform 
					  //von "haben" im Deutschen dafür braucht), kapselt, schon
					  //zugewiesen wurde
					  if(pWordNodeWhereTheGermanTranslationIsABendVerb)
						  pWordNodeEnglishVerb=
							  pWordNodeWhereTheGermanTranslationIsABendVerb;
  				
				  if(pWordNodeEnglishVerb)
					  if(typeid(*pWordNodeEnglishVerb->
						  //wenn Vollverb
						  m_pWord)==typeid(EnglishVerb))
					  {
						  EnglishVerb * ev=dynamic_cast<EnglishVerb*>
							  (pWordNodeEnglishVerb->m_pWord);
						  if(ev)
						  {
							  if(strToken==ev->m_strPreposition)
							  {
								  bPrepositionFound=true;
								  bValid=TRUE;
							  }
						  }
						  //printf("ev->infinitive: %s\n",ev->infinitive);
					  }
			  }//if(!bPrepositionFound)
			  else
			    //da sich bestimmte Adverbien - so glaube ich - zwischen einer
			    //gebeugten Verbform und einem Partizip Perfekt befinden können,
			    //werden sie in dieser Funktion dem Prädikat hinzugefügt
			    //ist dieses Beispiel korrekt?: "I have always been here." Adverb:
			    //"always", gebeugte Verbform: "have" Partizip Perfekt: "been"
			    if(//ein Adverb, das sich auf ein Verb bezieht oder
				    //ein Häufigkeitsadverb darf sich nur vor einer
				    //nicht-gebeugten Verbform (oder nach dem Objekt) befinden:
				    //You can easily find the fault.
				    //We always drink tea in the afternoon.
				    //siehe ENGLISCH im TELEKOLLEG I Band 2 Seite 117
				    //wenn der Zeiger, der ein englisches Verb, dessen deutsches 
				    //Pendant
				    //eine nicht-gebeugte Verbform ist, die für eine korrekte 
				    //Übersetzung notwendig 
				    //ist ("have" bei "I have been." ist zum Beispiel NICHT 
				    //notwendig, da 
				    //man durch die Zeitform "Perfekt" weiss, dass man eine 
				    //gebeugte Verbform 
				    //von "haben" im Deutschen dafür braucht), kapselt, noch
				    //nicht zugewiesen wurde
				    !pWordNodeWhereTheGermanTranslationIsANonBendVerb &&
				    (pWordNode=
				    GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(strToken)))
			    {
				    EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
				    if(ea)
				    {
					    //wenn sich das Adverb zwischen Subjekt und Verb befinden darf
					    //oder nach der finiten Form des Verbes 'to be' oder nach dem
					    //ersten Hilfsverb wie z.B. Adverbien 
					    //der Häufigkeit (always,never,often)
					    if(ea->m_bType==0 && 
						    //wenn noch kein Adverb, das sich zwischen Subjekt und Verb 
						    //befinden darf
						    //oder nach der finiten Form des Verbes 'to be' oder nach dem
						    //ersten Hilfsverb wie z.B. Adverbien 
						    //der Häufigkeit (always,never,often), gefunden wurde
						    !bAdverbOfFrequencyFound)
					    {
							    //if(typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->
							    //	m_pWord)==typeid(EnglishAuxiliaryVerb))
							    //{
						    bValid=true;
						    predicate.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated=
							    pWordNode;
						    bAdverbOfFrequencyFound=TRUE;
					    }
					    else if(
						    //wenn ein Adverb, das ein Verb beschreibt
						    ea->m_bType==1 && !predicate.
						    m_pWordNodeWhereAnEnglishAdverbDescribingAVerbIsEncapsulated)
						    if(pWordNodeWhereTheGermanTranslationIsANonBendVerb)
						    {
							    if(typeid(
								    *pWordNodeWhereTheGermanTranslationIsANonBendVerb->
								    //wenn Hilfsverb
								    m_pWord)==typeid(EnglishAuxiliaryVerb)
								    )
							    {
								    EnglishAuxiliaryVerb * peavNotFinite=dynamic_cast<
									    EnglishAuxiliaryVerb *>(
									    pWordNodeWhereTheGermanTranslationIsANonBendVerb->
									    m_pWord);
								    if(peavNotFinite && 
									    //"I can be nicely." ist falsch z.B.
									    peavNotFinite->m_strWords[0]!=_T("be")
									    )
									    bValid=true;
							    }
							    else
							    {
								    bValid=true;
							    }
						    }//if(pWordNodeWhereTheGermanTranslationIsANonBendVerb)
						    else
							    if(peavFinite && 
							    //"I am nicely." ist falsch z.B.
								    peavFinite->m_strWords[0]!=_T("be")
							    )
							    {
								    bValid=true;
								    predicate.m_pWordNodeWhereAnEnglishAdverbDescribingAVerbIsEncapsulated=
									    pWordNode;
							    }						
					    //}
				    }//if(ea)
			    }//if current token is EnglishAdverb and no non-finite verb yet
			if(!bValid)
				break;
		}//end for
		if(byTense==FUTURE)
		{
			//wenn der WordNode-Zeiger, der ein englisches 
			//Verb, dessen deutsches Pendant
			//KEINE gebeugte Verbform ist, die für eine 
			//korrekte Übersetzung notwendig 
			//ist ("being" bei "I am being used." ist zum 
			//Beispiel NICHT notwendig, da 
			//man durch das Attribut "Aktivsatz" weiss, dass 
			//man eine gebeugte Verbform 
			//von "werden" im Deutschen dafür braucht), 
			//kapselt, den Wert "NULL" hat
			
			//Zukunft muss ein Infinitiv haben: "will run" "will be running
			if(!pWordNodeWhereTheGermanTranslationIsANonBendVerb)
			{
				//SE - Zukunft aber kein Infinitiv\n");
				bValid=false;
			}
		}//if(byTense==FUTURE)
		else
		{
			//wenn der Zeiger, der ein englisches Verb, dessen deutsches 
			//Pendant
			//eine gebeugte Verbform ist, die für eine korrekte 
			//Übersetzung notwendig 
			//ist ("have" bei "I have been." ist zum Beispiel NICHT 
			//notwendig, da 
			//man durch die Zeitform "Perfekt" weiss, dass man eine 
			//gebeugte Verbform 
			//von "haben" im Deutschen dafür braucht), kapselt, schon
			//zugewiesen wurde
			if(pWordNodeWhereTheGermanTranslationIsABendVerb)
			{
				if(typeid(*pWordNodeWhereTheGermanTranslationIsABendVerb->
					m_pWord)==typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
						(pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
					if(eav)
					{
						#ifdef _DEBUG
						printf("eav->m_bIntegral: %u\n",eav->m_bIntegral);
						#endif
						//wenn modales Hilfsverb und wenn der WordNode-Zeiger, 
						//der ein englisches 
						//Verb, dessen deutsches Pendant
						//KEINE gebeugte Verbform ist, die für eine 
						//korrekte Übersetzung notwendig 
						//ist ("being" bei "I am being used." ist zum 
						//Beispiel NICHT notwendig, da 
						//man durch das Attribut "Aktivsatz" weiss, dass 
						//man eine gebeugte Verbform 
						//von "werden" im Deutschen dafür braucht), 
						//kapselt, den Wert "NULL" hat
						if(!eav->m_bIntegral && 
							!pWordNodeWhereTheGermanTranslationIsANonBendVerb)
						{
							//modales Hilfsverb ohne Vollverb\n");
							bValid=false;
						}
					}//if(eav)
				}
			}//if(pWordNodeWhereTheGermanTranslationIsABendVerb)
		}//else-branch of "if(byTense==FUTURE)"
		predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb=
			pWordNodeWhereTheGermanTranslationIsABendVerb;
		predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb=
			pWordNodeWhereTheGermanTranslationIsANonBendVerb;
		//predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb=
		//	pWordNodeWhereTheGermanTranslationIsABendVerb;
		predicate.m_bActiveClause=bActiveClause;
		predicate.m_bTense=byTense;
		//predicate.m_bNegation=bNegation;
		//predicate.m_bNegation=FALSE;
		//ss.m_pVocNodeWhereAnEnglishConjunctionIsEncapsulated=pVocNodeWhereAnEnglishConjunctionIsEncapsulated;

		#ifdef _DEBUG
		printf("predicate.m_bTense: %u\n",predicate.m_bTense);
		if(predicate.m_bActiveClause)
			printf("predicate.m_bActiveClause\n");
		else
			printf("!predicate.m_bActiveClause\n");
		if(predicate.m_bNegation)
			printf("predicate.m_bNegation\n");
		else
			printf("!predicate.m_bNegation\n");
		printf("returnPersonIndex: %u\n",returnPersonIndex);
		printf("ParseNonBendPartOfPredicate(Predicate & predicate,BOOL bBe,const CStringVector\
	& sv,BYTE personIndex) - ENDE - return bValid (%u)\n",
	(short)bValid);
		#endif
	}
	return bValid;	

}//BYTE ParseNonBendPartOfPredicate(Predicate & predicate,BOOL bBe,const CStringVector & sv,
//	,const DWORD & dwFlags)

//BYTE ParseNounStruct(EnumerationElement & ee,CStringVector sv)
//{
//	BOOL bAdjective=FALSE;
//	BOOL bArticle=FALSE;
//	bool bArticleOccured=false;
//  bool bAdjectiveOccured=false;
//	BOOL bNoun=FALSE;
//	BOOL bPersonalPronoun=FALSE;
//	BYTE personIndex=0;
//	DWORD dwStart=0;
//	CStringVector noun;
//	CString strToken;
//	for(int i=0;i<sv.size();i++)
//	{
//		TRACE("sv.at(i): %s\n",sv.at(i));
//		strToken=sv.at(i);
//		if(strToken=="a" || strToken=="the" && !bArticleOccured
//			&& !bAdjectiveOccured) //&& !bPosse)
//		{
//			++SVIterator;
//			bValid=true;
//			//if(ee.m_bArticle || bAdjective || bPersonalPronoun)
//			//{
//			//	TRACE("ParseEnumerationElement ENDE - return FALSE\n");
//			//	return FALSE;
//			//}
//			//ee.m_bArticle=TRUE;
//			//dwStart=i+1;
//			bArticleOccured=true;
//		}
//		//um das Stützwort "one" zu ermöglichen
//		//TELEKOLLEG I ENGLISCH Band1,Seite 98
//		else if(strToken=="one" || strToken=="ones" //&& 
//			//Beispiel: "a wild one" ("ein Wilder")
//			&& bAdjectiveOccured && 
//			//Token, das es nur als Substantiv gibt
//			!bTokenThatIsExclusivelyNounOccured && 
//			//wenn letztes Token
//			SVIterator+1==sv.end())
//		{
//			bOneOrOnesOccured=true;
//			bValid=true;
//		}
//		else if(IsEnglishAdjective(strToken) && 
//			!bTokenThatIsExclusivelyNounOccured)
//		{
//			//if(bNoun || bPersonalPronoun)
//			//{
//			//	TRACE("ParseEnumerationElement ENDE - return FALSE\n");
//			//	return FALSE;
//			//}
//			//ee.m_Adjectives.push_back(token);
//			//bAdjective=TRUE;
//			++SVIterator;
//			bValid=true;
//			bAdjectiveOccured=true;
//		}
//		else
////			noun.push_back(token);
//		if(IsEnglishNoun(strToken))
//		{
//			if(bNoun || bPersonalPronoun)
//			{
//				TRACE("ParseEnumerationElement ENDE - return FALSE\n");
//				return FALSE;
//			}
//			if(IsEnglishSingular(sv.at(i)))
//				personIndex=3; // 3. Person Singular
//			if(IsEnglishPlural(sv.at(i)))
//				personIndex=6; // 3. Person Plural
//			ee.m_Noun.clear();
//			ee.m_Noun.push_back(strToken);
//			bNoun=TRUE;
//			bAdjective=FALSE;
//			bArticle=FALSE;
//		}
//		else if(IsEnglishPossessivePronoun(strToken))
//		{
//			if(bNoun)
//				return FALSE;
//			ee.m_possessivePronoun=GetIndexFromPossessivePronoun(strToken);
//		}
//		else
//		{
////			sv2.push_back(sv(i));
//			TRACE("else-Zweig\n");
//			noun.push_back(strToken);
//		}
//		ee.m_strWords.push_back(sv.at(i));
//	}
//	if(!bNoun && !bPersonalPronoun) // bis jetzt wurde noch kein Nomen (Substantiv) gefunden
//	{
//		if(!IsEnglishSingular(noun) && !IsEnglishPlural(noun))
//		{
//			TRACE("ParseEnumerationElement ENDE - return FALSE\n");
//			if(ee.m_Adjectives.size()>0)
//			{
//				if(!IsEnglishNoun(ee.m_Adjectives.at(0)))
//					return FALSE;
//				else
//				{
//					ee.m_Noun.push_back(ee.m_Adjectives.at(0));
//					ee.m_Adjectives.clear();
//				}
//			}
//			else
//				return FALSE;
//		}
//		else
//		{
//			ee.m_Noun=noun;
//			personIndex=3;
//		}
//	}
//	else
//	{
//		if(noun.size()>0)
//		{
//			TRACE("ParseEnumerationElement ENDE - return FALSE\n");
//			return FALSE;
//		}
//		//ee.m_Noun.push_back(;
//	}
//	TRACE("personIndex: %u\n",personIndex);
//	TRACE("ParseEnumerationElement ENDE - return TRUE\n");
//	return personIndex;
//}

BYTE ParseNumber(const CStringVector & sv,DWORD & dwNumber,const DWORD & dwFlags)
{
	CString strDigit[]={"zero","one","two","three","four","five","six","seven",
		"eight","nine","ten","eleven","twelf","thirteen","fourteen","fifteen",
		"sixteen","seventeen","eighteen","nineteen"};
	CString strNumbersBelow20[]={"zero","one","two","three","four","five","six","seven",
		"eight","nine","ten","eleven","twelf","thirteen","fourteen","fifteen",
		"sixteen","seventeen","eighteen","nineteen"};
	CString strNumeral[]={"twenty","thirty","fourty","fifty","sixty","seventy",
		"eighty","ninety","hundred","thousand","million","billion"};
	BOOL bDigit=FALSE; //wenn Ziffer ("one"..."nine")
	BYTE bHyphen=FALSE; //wenn Bidestrich ("-")
	BOOL bNumeral=FALSE; //wenn "twenty","thirty",...,"thousand",...usw
	BOOL bNumeralBelowHundred=FALSE; //wenn "twenty","thirty",...,"thousand",...usw
	BYTE bNumeralFromHundred=FALSE;
	WORD bFactor=0;
	for(DWORD dwRun=0;dwRun<sv.size();dwRun++)
	{
		if(bHyphen)
		{
			//von "one" bis "nine"
			for(DWORD dwRun2=1;dwRun2<10;dwRun2++) 
				if(sv.at(dwRun)==strNumbersBelow20[dwRun2])
				{
					bDigit=TRUE;
					bFactor+=dwRun2;
				}
			bHyphen=FALSE;
		}
		if(bNumeralBelowHundred)
		{
			//wenn "twenty" bis "ninety", dann kann noch eine Ziffer dahinter stehen
			if(sv.at(dwRun)==_T("-"))
			{
				bHyphen=TRUE;
			}
			bNumeralBelowHundred=FALSE;
		}
		if(bDigit)
		{
			for(DWORD dwRun2=8;dwRun2<11;dwRun2++)
				if(sv.at(dwRun)==strNumeral[dwRun2])
				{
					bNumeralFromHundred=TRUE;
				}
		}
		if(!bNumeral)
		{
			//von "twenty" bis "ninety"
			for(DWORD dwRun2=0;dwRun2<8;dwRun2++)
				if(sv.at(dwRun)==strNumeral[dwRun2])
				{
					bNumeralBelowHundred=TRUE;
					bFactor=(dwRun2+2)*10;
				}
			if(!bNumeralBelowHundred)
				for(DWORD dwRun2=1;dwRun2<19;dwRun2++) 
					if(sv.at(dwRun)==strNumbersBelow20[dwRun2])
					{
						bDigit=TRUE;
						bFactor=dwRun2;
					}

		}
		if(!bDigit && !bNumeral)
			return 0;
	}
	return 0;
}

BOOL ParseManner(Manner & manner,const PositionCStringVector & psv,BOOL bBe,
	const DWORD & dwFlags)
{
	#ifdef _DEBUG
	printf("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL \
bBe,const DWORD & dwFlags) - ANFANG\n");
	#endif
	TRACE("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL bBe)\n");
#ifdef _DEBUG
	for(DWORD i=0;i<psv.size();i++)
	{
		printf("psv.at(%d).m_Str: %s\n",i,psv.at(i).m_Str);
		//TRACE("
	}
#endif
	BOOL bAnd=FALSE;
	DWORD dwStart=0;
	for(DWORD dwRun=0;dwRun<psv.size();dwRun++)
	{
		CString token=psv.at(dwRun).m_Str;
		if(token==_T(","))
		{
			if(bAnd)
			{
				TRACE("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL bBe) return FALSE\n");
				#ifdef _DEBUG
				printf("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL \
bBe) return FALSE\n");
				#endif
				return FALSE;
			}
			MannerEnumerationElement mee;
			if(ParseMannerEnumerationElement(mee,GetBetween(psv,dwStart,dwRun-1),bBe,TRUE))
				manner.m_vecMannerEnumerationElement.push_back(mee);
			else
			{
				TRACE("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL bBe) return FALSE\n");
				#ifdef _DEBUG
				printf("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL \
bBe) return FALSE\n");
				#endif
				return FALSE;
			}
			dwStart=dwRun+1;
			//bColon=TRUE;
		}
		if(token==_T("and"))
		{
			bAnd=TRUE;
			MannerEnumerationElement mee;
			if(ParseMannerEnumerationElement(mee,GetBetween(psv,dwStart,dwRun-1),bBe,TRUE))
				manner.m_vecMannerEnumerationElement.push_back(mee);
			else
			{
				TRACE("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL bBe) return FALSE\n");
				#ifdef _DEBUG
				printf("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL \
bBe) return FALSE\n");
				#endif
				return FALSE;
			}
			dwStart=dwRun+1;
		}
		if(dwRun+1==psv.size())
		{
			MannerEnumerationElement mee;
			if(ParseMannerEnumerationElement(mee,GetBetween(psv,dwStart,
				psv.size()-1),bBe,TRUE))
				manner.m_vecMannerEnumerationElement.push_back(mee);
			else
			{
				TRACE("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL \
bBe,const DWORD & dwFlags) return FALSE\n");
				#ifdef _DEBUG
				printf("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL \
bBe,const DWORD & dwFlags) return FALSE\n");
				#endif
				return FALSE;
			}
		}
	}
	#ifdef _DEBUG
	printf("BOOL ParseManner(Manner & manner,const CStringVector & sv,BOOL bBe) \
return TRUE\n");
	#endif
	return TRUE;
}

BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,
	const PositionCStringVector & psv,BOOL bBe,const DWORD & dwFlags)
{
	BOOL bAdverbRefersToAdjectiveOrAdverb=FALSE;
	BOOL bAdverbOfManner=FALSE;
	BOOL bPositive=FALSE;
	Comparison comparison;
	Similarity similarity;
	std::vector<Range> rVecRange,vecRange;
	WordNode * pWordNode=NULL;
	TRACE("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,const CStringVector & sv,BOOL bBe) - ANFANG\n");
#ifdef _DEBUG
					printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - ANFANG\n");
	for(DWORD dw=0;dw<psv.size();dw++)
	{
		printf("psv.at(%u).m_Str: %s\n",dw,psv.at(dw).m_Str);
	}
#endif
	if(ParseSimilarity(similarity,psv,bBe,dwFlags))
	{
		mee.m_pSimilarity=new Similarity(similarity);
		return 1;
	}
	if(ParseComparison(comparison,psv,vecRange,dwFlags))
	{
		mee.m_pComparison=new Comparison(comparison);
		for(DWORD dwRun=0;dwRun<vecRange.size();++dwRun)
			rVecRange.push_back(vecRange.at(dwRun));
		return 1;
	}
	if(bBe)
	{
#ifdef _DEBUG
		printf("bBe\n");
#endif
		for(DWORD dwRun=0;dwRun<psv.size() && g_bContinue;dwRun++)
		{
			CString token=psv.at(dwRun).m_Str;
			if((pWordNode=
				GetWordNodeWhereAnEnglishAdverbRefersToAdjectiveOrAdverbIsEncapsulated(
				token)))
			{
				mee.m_pWordNodeAdverbRefersToAdjectiveOrAdverb=pWordNode;
				if(bPositive || bAdverbRefersToAdjectiveOrAdverb)
				{

					TRACE("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,const CStringVector & sv,BOOL bBe) - ENDE -return FALSE\n");
#ifdef _DEBUG
					printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - return FALSE\n");
#endif
					return FALSE;
				}
				//mee.m_strAdverbRefersToAdjectiveOrAdverb=token;
				bAdverbRefersToAdjectiveOrAdverb=TRUE;
			}
			else if((pWordNode=GetWordNodeWhereAnEnglishPositiveIsEncapsulated(token)))
			{
				if(bPositive)
				{
#ifdef _DEBUG
					printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - return FALSE\n");
#endif
					TRACE("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,const CStringVector & sv,BOOL bBe) - ENDE -return FALSE\n");
					return FALSE;
				}
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(pWordNode->m_pWord);
				if(ea)
				{
					//wenn das Adjektiv nur vor einem Substantiv stehen darf
					//Beispiel für ein Adjektiv, das nur vor einem Substantiv stehen
					//darf: "sick": "I am a sick boy."
					if(ea->m_bAllowedPlace==1)
					{
						#ifdef _DEBUG
						printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
	const CStringVector & sv)\n - return 0 - das Adjektiv darf nur \
	vor einem Substantiv stehen");
						#endif
						return 0;
					}	
				}	
				mee.m_pWordNodeAdverbOfManner=pWordNode;
			//mee.m_strAdverbOfManner=token;
				bPositive=TRUE;
			}
			else
			{
#ifdef _DEBUG
				printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - return FALSE\n");
#endif
				return FALSE;
			}
		}
		//wenn ein Adverb, dass sich auf ein Adjektiv beziehen muss, gefunden 
		//wurde aber kein Positiv (Grundform eines Adjektivs)
		if(bAdverbRefersToAdjectiveOrAdverb && !bPositive)
		{
#ifdef _DEBUG
			printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - return FALSE\n");
#endif
			return FALSE;
		}
	}
	else
	{
#ifdef _DEBUG
		printf("!bBe\n");
#endif
		for(DWORD dwRun=0;dwRun<psv.size() && g_bContinue;dwRun++)
		{
			CString token=psv.at(dwRun).m_Str;
			if((pWordNode=GetWordNodeWhereAnEnglishAdverbRefersToAdjectiveOrAdverbIsEncapsulated(token)))
			{
				mee.m_pWordNodeAdverbRefersToAdjectiveOrAdverb=pWordNode;
				if(bAdverbOfManner || bAdverbRefersToAdjectiveOrAdverb)
				{
					TRACE("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,const CStringVector & sv,BOOL bBe) - ENDE -return FALSE\n");
#ifdef _DEBUG
					printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - return FALSE\n");
#endif
					return FALSE;
				}
				//mee.m_strAdverbRefersToAdjectiveOrAdverb=token;
				bAdverbRefersToAdjectiveOrAdverb=TRUE;
			}
			else if((pWordNode=GetWordNodeWhereAnEnglishAdverbOfMannerIsEncapsulated(token)))
			{
				mee.m_pWordNodeAdverbOfManner=pWordNode;
				if(bAdverbOfManner)
				{
#ifdef _DEBUG
					printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - return FALSE\n");
#endif
					TRACE("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,const CStringVector & sv,BOOL bBe) - ENDE -return FALSE\n");
					return FALSE;
				}
			//mee.m_strAdverbOfManner=token;
				bAdverbOfManner=TRUE;
			}
			else
			{
				TRACE("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,const CStringVector & sv,BOOL bBe) - ENDE -return FALSE\n");
#ifdef _DEBUG
				printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - return FALSE\n");
#endif
				return FALSE;
			}
		}
	}
	if(!bAdverbOfManner && !bPositive)
	{
		TRACE("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,const CStringVector & sv,BOOL bBe) - ENDE -return FALSE\n");
#ifdef _DEBUG
		printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - return FALSE\n");
#endif
		return FALSE;
	}
	TRACE("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,const CStringVector & sv,BOOL bBe) - ENDE -return TRUE\n");
#ifdef _DEBUG
	printf("BOOL ParseMannerEnumerationElement(MannerEnumerationElement & mee,\
const CStringVector & sv,BOOL bBe,const DWORD & dwFlags) - return TRUE\n");
#endif
	return TRUE;
}

BOOL ParseMiddle(ClauseWithoutSubject & ss,BOOL bBe,const CStringVector & sv)
{
	TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ANFANG\n");
	ss.m_vecMiddleEnumerationElements.clear(); // Vektor leeren (enthält möglicherweise 
	// bereits Elemente durch einen vorherigen Aufruf dieser Funktion
	BOOL b3rdPerson=FALSE;
	BOOL bAux=FALSE;
	BOOL bDo=FALSE;
	BOOL bFrequency=FALSE;
	BOOL bHave=FALSE;
	BOOL bManner=FALSE;
	BOOL bNot=FALSE;
	BOOL bVerb=FALSE;
	BOOL bFuture=FALSE;
	DWORD i ;
	//CString token;
	if(!(sv.size()>0))
	{
		TRACE("ParseMiddle ENDE - return FALSE\n");
		return FALSE;
	}
	for(//DWORD 
		i=0;i<sv.size();i++)
	{
		CString token=sv.at(i);
		//in MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
	#ifdef _WINDOWS
		TRACE("sv.at(%d): %s\n",i,token);
	#endif
	}
	ss.m_vecEnglishAdverb.clear();
	//for(int i=0;i<ss.m_adverbs.size();i++
	for(//int 
		i=0;i<sv.size();i++)
	{
		CString token=sv.at(i);
		//in MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
	#ifdef _WINDOWS
		TRACE("token: %s\n",token);
	#endif
		if(IsEnglishVerb(token))
		{
			if(IsEnglishAuxiliaryVerb(token)) // englisches Hilfsverb (z.B. 'to be')
			{
				//EnglishAuxiliaryVerb
				CString strBe("be") ;
				EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strBe);
				if(token==eav->m_strWords[1] || token==eav->m_strWords[2] ||  // am, are, is
					token==eav->m_strWords[3] || token==eav->m_strWords[7] || 
					token==eav->m_strWords[8])
				{  // was, were
					bBe=TRUE;
					ss.m_strPredicate=token;//"be";
				}
				CString strHave("have") ;
				eav=GetEnglishAuxiliaryVerb(strHave);
				if(token==eav->m_strWords[1] || // have, has
					token==eav->m_strWords[3] || token==eav->m_strWords[7] || 
					token==eav->m_strWords[9])
				{  // was, were
					bHave=TRUE;
					ss.m_strPredicate=token;//"have";
				}
				CString strDo("do") ;
				eav=GetEnglishAuxiliaryVerb(strDo);
				if(token==eav->m_strWords[1] || // do, does
					token==eav->m_strWords[3] || token==eav->m_strWords[7])
				{  // did
					bDo=TRUE;
					ss.m_strPredicate=token;//"have";
				}
				if(token==_T("will"))
				{
					if(bFuture || bFrequency)
					{
						TRACE("ParseMiddle ENDE - return FALSE\n");
						return FALSE;
					}
					bFuture=TRUE;
				}
				if(bFrequency || bDo || ss.m_bNegation || bManner)
				{
					TRACE("ParseMiddle ENDE - return FALSE\n");
					return FALSE;
				}
				bAux=TRUE;
				ss.m_bBe=TRUE;
			}
			else
			{
				if(IsPresentProgressive(token))
				{
					if(bFuture || !bBe)
					{
						TRACE("ParseMiddle ENDE - return FALSE\n");
						return FALSE;
					}
					CString strBe("be") ;
					EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strBe);
					BYTE pi=ss.m_personIndex;
					//in MFC/Windows the TRACE macro has a variable # of args--I do 
					//not know how to enable variable numbers of args for the same macro name.
					//So I exclude it from non-Windows.
				#ifdef _WINDOWS
					TRACE("ss.m_personIndex: %u\n",ss.m_personIndex);
				#endif
					if(ss.m_strPredicate==eav->m_strWords[1] && pi==1 ||
						(ss.m_strPredicate==eav->m_strWords[2] && (pi==2 || pi==4 || 
						pi==5 || pi==6)) ||
						ss.m_strPredicate==eav->m_strWords[3] && pi==3)
					{ // am, are, is
						ss.m_bTense=PRESENT_PROGRESSIVE;
						if(token.GetLength()>3)
						{
							ss.m_strPredicate=token.Left(token.GetLength()-3);
						}
					}
					else
					{
						TRACE("ParseMiddle ENDE - return FALSE\n");
						return FALSE;
					}
				}
				if(IsPresentTense(token))
				{
					if(ss.m_strPredicate!="")
						return FALSE;
					if(bFuture)
						ss.m_bTense=FUTURE;
					else
						ss.m_bTense=SIMPLE_PRESENT;
					ss.m_strPredicate=token;
				}
				else if(IsSimplePast(token))
				{
					if(bFuture)
					{
						TRACE("ParseMiddle ENDE - return FALSE\n");
						return FALSE;
					}
					ss.m_bTense=SIMPLE_PAST;
				}
				if(IsPastParticiple(token))
				{
					CString strHave("have") ;
					EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strHave);
					BYTE pi=ss.m_personIndex;
					//In MFC/Windows the TRACE macro has a variable # of args--I do 
					//not know how to enable variable numbers of args for the same macro name.
					//So I exclude it from non-Windows.
				#ifdef _WINDOWS
					TRACE("ss.m_personIndex: %u\n",ss.m_personIndex);
				#endif
					if(ss.m_strPredicate==eav->m_strWords[1] && pi==1 ||
						(ss.m_strPredicate==eav->m_strWords[2] && (pi==2 || pi==4 || 
						pi==5 || pi==6)) ||
						ss.m_strPredicate==eav->m_strWords[3] && pi==3)
					{ // am, are, is
						ss.m_bTense=PRESENT_PERFECT;
						EnglishVerb * ev=GetEnglishVerb(token);
						ss.m_strPredicate=ev->m_strInfinitive;
					}
					else
					{
						TRACE("ParseMiddle ENDE - return FALSE\n");
						return FALSE;
					}
				}
				bVerb=TRUE;
			}
			if(IsEnglishVerb3rdPerson(token))
			{
				TRACE("IsEnglishVerb3rdPerson(token)==TRUE\n");
				b3rdPerson=TRUE;
			}
			if(ss.m_bTense==SIMPLE_PAST)
			{
//				EnglishVerb * ev=GetEnglishVocableOfEnglishVerb(token);
	//			if(token!=ev->infinitive)
		//			return FALSE;
			}
		}
		/*else if(token==_T("will"))
		{
			if(bFuture || bFrequency)
				return FALSE;
			bFuture=TRUE;
		}*/
		//else if(IsEnglishAdverb(token,'1'))
		else if(IsEnglishAdverbOfFrequency(token))
		{
			TRACE("IsEnglishAdverbOfFrequency(token)\n");
			if(bVerb || bManner || bFrequency)
			{
				TRACE("ParseMiddle ENDE - return FALSE\n");
				return FALSE;
			}
			bFrequency=TRUE;
			EnglishAdverb ea;
			ea.m_bType='1';
			ea.m_strWord=token;
			ss.m_vecEnglishAdverb.push_back(ea);
		}
		else if(IsEnglishAdverbRefersToAdjectiveOrAdverb(token) || 
			IsEnglishAdverbOfManner(token))
		{
			if(IsEnglishAdverbOfManner(token))
			{
				bManner=TRUE;
			}
//			if(i+1<sv.size())
	//		{
				DWORD dwStart=i;
				for(DWORD dwRun=i;dwRun<sv.size();dwRun++)
				{
					token=sv.at(dwRun);
					if(token=="," || token=="and")
					{
						MiddleEnumerationElement mee;
						CStringVector svMEE=GetBetween(sv,dwStart,dwRun-1);
						if(!ParseMiddleEnumerationElement(mee,svMEE,TRUE))
						{
							TRACE("ParseMiddle ENDE - return FALSE\n");
							return FALSE;
						}
						dwStart=dwRun+1;
						ss.m_vecMiddleEnumerationElements.push_back(mee);
					}
					if(dwRun+1==sv.size())
					{
						MiddleEnumerationElement mee;
						CStringVector svMEE=GetBetween(sv,dwStart,dwRun);
						if(!ParseMiddleEnumerationElement(mee,svMEE,TRUE))
						{
							TRACE("ParseMiddle ENDE - return FALSE\n");
							return FALSE;
						}
						//dwStart=dwRun+1;
						ss.m_vecMiddleEnumerationElements.push_back(mee);
						TRACE("ParseMiddle ENDE - return TRUE\n");
						return TRUE;
					}
				}
		//	}
			TRACE("IsEnglishAdverbOfManner(token)\n");
/*			if(bManner)
			{
				TRACE("ParseMiddle ENDE - return FALSE\n");
				return FALSE;
			}
			bManner=TRUE;
			EnglishAdverb ea;
			ea.type='3';
			ea.word=token;
			ss.m_adverbs.push_back(ea);*/
		}
		else if(IsEnglishAdverb(token,'3'))
		{
			bManner=TRUE;
			EnglishAdverb ea;
			ea.m_bType='1';
			ea.m_strWord=token;
			ss.m_vecEnglishAdverb.push_back(ea);
		}
		else if(token=="do" || token=="does" || token=="did")
		{
			if(bAux || bNot)
			{
				TRACE("ParseMiddle ENDE - return FALSE\n");
				return FALSE;
			}
			bDo=TRUE;
			if(token=="did")
				ss.m_bTense=SIMPLE_PAST;
		}
		else if(token=="not")
		{
			TRACE("token==\"not\"\n");
			if(bAux)
			{
				if(bFrequency || bManner || bDo)
				{
					TRACE("ParseMiddle ENDE - return FALSE\n");
					return FALSE;
				}
			}
			else
			{
				if(bManner || bVerb)
				{
					TRACE("ParseMiddle ENDE - return FALSE\n");
					return FALSE;
				}
			}
			TRACE("ss.bNegation=TRUE\n");
			ss.m_bNegation=TRUE;
		}
		else
		{
			TRACE("ParseMiddle ENDE - return FALSE\n");
			return FALSE;
		}
	}
	TRACE("ParseMiddle ENDE - return TRUE\n");
	return TRUE;
}

BOOL ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv)
{
	TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ANFANG\n");
	#ifdef _DEBUG
	printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - ANFANG\n");
	#endif
	ss.m_vecMiddleEnumerationElement.clear(); // Vektor leeren (enthält möglicherweise 
	// bereits Elemente durch einen vorherigen Aufruf dieser Funktion
	BOOL b3rdPerson=FALSE;
	BOOL bAux=FALSE;
	BOOL bDo=FALSE;
	BOOL bFrequency=FALSE;
	BOOL bHave=FALSE;
	BOOL bManner=FALSE;
	BOOL bNot=FALSE;
	BOOL bVerb=FALSE;
	BOOL bFuture=FALSE;
	//CString token;
	if(!(sv.size()>0))
	{
		TRACE("ParseMiddle ENDE - return FALSE\n");
		#ifdef _DEBUG
		printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - return FALSE - sv,.size()==0\n");
		#endif
		return FALSE;
	}
	for(DWORD i=0;i<sv.size();i++)
	{
		CString token=sv.at(i);
		//in MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
#ifdef _WINDOWS
		TRACE("sv.at(%d): %s\n",i,token);
#endif
		#ifdef _DEBUG
		printf("sv.at(%d): %s\n",i,token);
		#endif
	}
	ss.m_vecEnglishAdverb.clear();
	//for(int i=0;i<ss.m_adverbs.size();i++
	for(i=0;i<sv.size();i++)
	{
		CString token=sv.at(i);
		//In MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
#ifdef _WINDOWS
		TRACE("token: %s\n",token);
#endif
		if(IsEnglishVerb(token))
		{
			if(IsEnglishAuxiliaryVerb(token)) // englisches Hilfsverb (z.B. 'to be')
			{
				//EnglishAuxiliaryVerb
				CString strBe("be") ;
				EnglishAuxiliaryVerb * eav = ::GetEnglishAuxiliaryVerb(strBe);
				if(token==eav->m_strWords[1] || token==eav->m_strWords[2] ||  // am, are, is
					token==eav->m_strWords[3] || token==eav->m_strWords[7] || 
					token==eav->m_strWords[8])
				{  // was, were
					bBe=TRUE;
					if(ss.m_strPredicate!=_T(""))
					{
						#ifdef _DEBUG
						printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const \
CStringVector & sv) - return FALSE ss.predicate!=""\n");
						#endif
						return FALSE;
					}
					ss.m_strPredicate=token;//"be";
				}
				CString strHave("have") ;
				eav=GetEnglishAuxiliaryVerb(strHave);
				if(token==eav->m_strWords[1] || // have, has
					token==eav->m_strWords[3] || token==eav->m_strWords[7] || 
					token==eav->m_strWords[9])
				{  // was, were
					bHave=TRUE;
					if(ss.m_strPredicate!=_T(""))
					{
						#ifdef _DEBUG
						printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const \
CStringVector & sv) - return FALSE ss.predicate!=""\n");
						#endif
						return FALSE;
					}
					ss.m_strPredicate=token;//"have";
				}
				CString strDo("do") ;
				eav=GetEnglishAuxiliaryVerb(strDo);
				if(token==eav->m_strWords[1] || // do, does
					token==eav->m_strWords[3] || token==eav->m_strWords[7])
				{  // did
					bDo=TRUE;
					if(ss.m_strPredicate!=_T(""))
					{
						#ifdef _DEBUG
						printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const \
CStringVector & sv) - return FALSE ss.predicate!=""\n");
						#endif
						return FALSE;
					}
					ss.m_strPredicate=token;//"have";
				}
				if(token==_T("will"))
				{
					if(bFuture || bFrequency)
					{
						TRACE("ParseMiddle ENDE - return FALSE\n");
						#ifdef _DEBUG
						printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const \
CStringVector & sv) - return FALSE bFuture || bFrequency\n");
						#endif
						return FALSE;
					}
					bFuture=TRUE;
				}
				if(bFrequency || bDo || ss.m_bNegation || bManner)
				{
					TRACE("ParseMiddle ENDE - return FALSE\n");
					#ifdef _DEBUG
					printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector \
& sv) - return FALSE bFrequency || bDo || ss.bNegation || bManner\n");
					#endif
					return FALSE;
				}
				bAux=TRUE;
				ss.m_bBe=TRUE;
			}
			else
			{
				if(IsPresentProgressive(token))
				{
					if(bFuture || !bBe)
					{
						TRACE("ParseMiddle ENDE - return FALSE\n");
						#ifdef _DEBUG
						printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const \
CStringVector & sv) - return FALSE bFuture || !bBe\n");
						#endif
						return FALSE;
					}
					CString strBe("be") ;
					EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strBe);
					BYTE pi=ss.m_Subject.m_personIndex;
					//In MFC/Windows the TRACE macro has a variable # of args--I do 
					//not know how to enable variable numbers of args for the same macro name.
					//So I exclude it from non-Windows.
#ifdef _WINDOWS
					TRACE("ss.m_pSubject->m_personIndex: %u\n",ss.m_Subject.m_personIndex);
#endif
					if(ss.m_strPredicate==eav->m_strWords[1] && pi==1 ||
						(ss.m_strPredicate==eav->m_strWords[2] && (pi==2 || pi==4 || 
						pi==5 || pi==6)) ||
						ss.m_strPredicate==eav->m_strWords[3] && pi==3)
					{ // am, are, is
						ss.m_bTense=PRESENT_PROGRESSIVE;
						if(token.GetLength()>3)
						{
							ss.m_strPredicate=token.Left(token.GetLength()-3);
						}
					}
					else
					{
						TRACE("ParseMiddle ENDE - return FALSE\n");
						#ifdef _DEBUG
						printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const \
CStringVector & sv) - return FALSE nicht 'be'\n");
						#endif
						return FALSE;
					}
				}
				if(IsPresentTense(token))
				{
					if(ss.m_strPredicate!="")
					{
						#ifdef _DEBUG
						printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const \
CStringVector & sv) - return FALSE ss.predicate!=""\n");
						#endif
						return FALSE;
					}
					if(bFuture)
						ss.m_bTense=FUTURE;
					else
						ss.m_bTense=SIMPLE_PRESENT;
					ss.m_strPredicate=token;
				}
				else if(IsSimplePast(token))
				{
					if(bFuture)
					{
						TRACE("ParseMiddle ENDE - return FALSE\n");
						#ifdef _DEBUG
						printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const \
CStringVector & sv) - return FALSE bFuture\n");
						#endif
						return FALSE;
					}
					EnglishVerb * ev=GetEnglishVerb(token);
					ss.m_strPredicate=ev->m_strInfinitive;
					ss.m_bTense=SIMPLE_PAST;
				}
				if(IsPastParticiple(token))
				{
					CString strHave("have") ;
					EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strHave);
					BYTE pi=ss.m_Subject.m_personIndex;
					//In MFC/Windows the TRACE macro has a variable # of args--I do 
					//not know how to enable variable numbers of args for the same macro name.
					//So I exclude it from non-Windows.
#ifdef _WINDOWS
					TRACE("ss.m_pSubject->m_personIndex: %u\n",ss.m_Subject.m_personIndex);
#endif
					if(ss.m_strPredicate==eav->m_strWords[1] && pi==1 ||
						(ss.m_strPredicate==eav->m_strWords[2] && (pi==2 || pi==4 || 
						pi==5 || pi==6)) ||
						ss.m_strPredicate==eav->m_strWords[3] && pi==3)
					{ // am, are, is
						ss.m_bTense=PRESENT_PERFECT;
						EnglishVerb * ev=GetEnglishVerb(token);
						ss.m_strPredicate=ev->m_strInfinitive;
					}
					else
					{
						if(!IsSimplePast(token) && !IsPresentTense(token))
						{
							#ifdef _DEBUG
							printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const \
CStringVector & sv) - return FALSE nicht 'have'\n");
							#endif
							return FALSE;
						}
					}
				}
				bVerb=TRUE;
			}
			if(IsEnglishVerb3rdPerson(token))
			{
				TRACE("IsEnglishVerb3rdPerson(token)==TRUE\n");
				b3rdPerson=TRUE;
			}
			if(ss.m_bTense==SIMPLE_PAST)
			{
//				EnglishVerb * ev=GetEnglishVocableOfEnglishVerb(token);
	//			if(token!=ev->infinitive)
		//			return FALSE;
			}
		}
		/*else if(token==_T("will"))
		{
			if(bFuture || bFrequency)
				return FALSE;
			bFuture=TRUE;
		}*/
		//else if(IsEnglishAdverb(token,'1'))
		else if(IsEnglishAdverbOfFrequency(token))
		{
			TRACE("IsEnglishAdverbOfFrequency(token)\n");
			if(bVerb || bManner || bFrequency)
			{
				TRACE("ParseMiddle ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector \
& sv) - return FALSE bVerb || bManner || bFrequency\n");
				#endif
				return FALSE;
			}
			bFrequency=TRUE;
			EnglishAdverb ea;
			ea.m_bType='1';
			ea.m_strWord=token;
			ss.m_vecEnglishAdverb.push_back(ea);
		}
		else if(IsEnglishAdverbRefersToAdjectiveOrAdverb(token) || 
			IsEnglishAdverbOfManner(token))
		{
			if(IsEnglishAdverbOfManner(token))
			{
				bManner=TRUE;
			}
//			if(i+1<sv.size())
	//		{
				DWORD dwStart=i;
				for(DWORD dwRun=i;dwRun<sv.size();dwRun++)
				{
					token=sv.at(dwRun);
					if(token=="," || token=="and")
					{
						MiddleEnumerationElement mee;
						CStringVector svMEE=GetBetween(sv,dwStart,dwRun-1);
						if(!ParseMiddleEnumerationElement(mee,svMEE,TRUE))
						{
							TRACE("ParseMiddle ENDE - return FALSE\n");
							#ifdef _DEBUG
							printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - return FALSE !ParseMiddleEnumerationElement(mee,svMEE)\n");
							#endif
							return FALSE;
						}
						dwStart=dwRun+1;
						ss.m_vecMiddleEnumerationElement.push_back(mee);
					}
					if(dwRun+1==sv.size())
					{
						MiddleEnumerationElement mee;
						CStringVector svMEE=GetBetween(sv,dwStart,dwRun);
						if(!ParseMiddleEnumerationElement(mee,svMEE,TRUE))
						{
							TRACE("ParseMiddle ENDE - return FALSE\n");
							#ifdef _DEBUG
							printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - return FALSE !ParseMiddleEnumerationElement(mee,svMEE)\n");
							#endif
							return FALSE;
						}
						//dwStart=dwRun+1;
						ss.m_vecMiddleEnumerationElement.push_back(mee);
						TRACE("ParseMiddle ENDE - return TRUE\n");
						#ifdef _DEBUG
						printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - return FALSE\n");
						#endif
						return TRUE;
					}
				}
		//	}
			TRACE("IsEnglishAdverbOfManner(token)\n");
/*			if(bManner)
			{
				TRACE("ParseMiddle ENDE - return FALSE\n");
				return FALSE;
			}
			bManner=TRUE;
			EnglishAdverb ea;
			ea.type='3';
			ea.word=token;
			ss.m_adverbs.push_back(ea);*/
		}
		else if(IsEnglishAdverb(token,'3'))
		{
			bManner=TRUE;
			EnglishAdverb ea;
			ea.m_bType='1';
			ea.m_strWord=token;
			ss.m_vecEnglishAdverb.push_back(ea);
		}
		else if(token=="do" || token=="does" || token=="did")
		{
			if(bAux || bNot)
			{
				TRACE("ParseMiddle ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - return FALSE bAux || bNot\n");
				#endif
				return FALSE;
			}
			bDo=TRUE;
			if(token=="did")
				ss.m_bTense=SIMPLE_PAST;
		}
		else if(token=="not")
		{
			TRACE("token==\"not\"\n");
			if(bAux)
			{
				if(bFrequency || bManner || bDo)
				{
					TRACE("ParseMiddle ENDE - return FALSE\n");
					#ifdef _DEBUG
					printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - return FALSE bFrequency || bManner || bDo\n");
					#endif
					return FALSE;
				}
			}
			else
			{
				if(bManner || bVerb)
				{
					TRACE("ParseMiddle ENDE - return FALSE\n");
					#ifdef _DEBUG
					printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - return FALSE bManner || bVerb\n");
					#endif
					return FALSE;
				}
			}
			TRACE("ss.bNegation=TRUE\n");
			ss.m_bNegation=TRUE;
		}
		else
		{
			TRACE("ParseMiddle ENDE - return FALSE\n");
			#ifdef _DEBUG
			printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - return FALSE nicht von allen\n");
			#endif
			return FALSE;
		}
	}
	TRACE("ParseMiddle ENDE - return TRUE\n");
	#ifdef _DEBUG
	printf("ParseMiddle(RelativeClause & ss,BOOL bBe,const CStringVector & sv) - return TRUE\n");
	#endif
	return TRUE;
}

BOOL ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv,
	BYTE personIndex,const DWORD & dwFlags)
{
	BYTE returnPersonIndex=0;
	//VocNode * pVocNodeWhereAnEnglishConjunctionIsEncapsulated=NULL;
	TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ANFANG\n");
#ifdef _DEBUG
	printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv,\
BYTE personIndex,const DWORD & dwFlags) - ANFANG\n");
	printf("ss.m_bTense: %u\n",ss.m_bTense);
#endif
	CStringVector strToken;
	WordNode * pWordNodeWhereAnEnglishPredicateIsEncapsulated=NULL;
	WordNode * pWordNodeWhereAnEnglishVerbIsEncapsulated=NULL;
	//ss.tense=0;
	ss.m_vecMiddleEnumerationElement.clear(); // Vektor leeren (enthält möglicherweise 
	// bereits Elemente durch einen vorherigen Aufruf dieser Funktion
	BOOL b3rdPerson=FALSE;
	BOOL bAux=FALSE;
	BOOL bDo=FALSE;
	BOOL bFrequency=FALSE;
	BOOL bHave=FALSE;
	BOOL bManner=FALSE;
	BOOL bNegation=FALSE;
	BOOL bNot=FALSE;
	BOOL bVerb=FALSE;
	BOOL bFuture=FALSE;
	BOOL bActiveClause=TRUE;
	BOOL bTo=FALSE;
	BYTE tense=0;
	//CString token;
	if(!(sv.size()>0))
	{
		TRACE("ParseMiddle ENDE - return FALSE\n");
#ifdef _DEBUG
		printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ENDE - return FALSE - sv.size()==0\n");
#endif
		return FALSE;
	}
	#ifdef _DEBUG
	{
		for(DWORD i=0;i<sv.size();i++)
		{
			CString token=sv.at(i);
			TRACE("sv.at(%d): %s\n",i,token);
			printf("sv.at(%d): %s\n",i,token);
		}
	}
	#endif
	ss.m_vecEnglishAdverb.clear();
	//for(int i=0;i<ss.m_adverbs.size();i++
	BOOL bAuxiliaryVerbOccured=FALSE;
	for(DWORD i=0;i<sv.size() && g_bContinue;i++)
	{
		CString token=sv.at(i);
		if(!(dwFlags&CASE_SENSITIVE)==1)
			token.MakeLower();
		//In MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
#ifdef _WINDOWS
		TRACE("token: %s\n",token);
#endif
		#ifdef _DEBUG
		printf("token: %s\n",token);
		#endif
		WordNode * pWordNode=NULL;
		BOOL bValidToken=FALSE;
		if((pWordNode=GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(token))) //IsEnglishVerb(token))
		{
		#ifdef _DEBUG
			printf("pWordNode!=NULL\n");
		#endif
			BOOL bAuxiliaryVerb=FALSE;
			if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				BOOL bValid=FALSE;
		#ifdef _DEBUG
				printf("EnglishAuxiliaryVerb\n");
		#endif
				//strToken.push_back(token);
				bAuxiliaryVerb=TRUE;
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(pWordNode->m_pWord);
				if(token==eav->m_strWords[0]) // wenn Infinitiv
				{
					if(token==_T("will"))
					{
						if(bFuture || bFrequency)
						{
							TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ENDE - return FALSE\n");
							#ifdef _DEBUG
							printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ENDE - return FALSE - bFuture || bFrequency\n");
							#endif
							return FALSE;
						}
						bValid=TRUE;
						bFuture=TRUE;
						bAuxiliaryVerb=TRUE;
						bAuxiliaryVerbOccured=TRUE;
						tense=FUTURE;
						strToken.push_back(token);
						//pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNode;
					}
					if(pWordNodeWhereAnEnglishPredicateIsEncapsulated)
					{
						if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
							m_pWord)!=typeid(EnglishAuxiliaryVerb) && !bTo)
						{
							#ifdef _DEBUG
							printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - return FALSE - !typeid(*pWord\
NodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)==typeid(EnglishAuxil\
iaryVerb) && !bTo\n");
							#endif
							return FALSE;
						}
						bValid=TRUE;
						pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
					}
					else // Infinitiv und gebeugte Verbform können gleich sein!
					{ // Beispiel: to have - I have
						BYTE b ;
						for(//BYTE 
							b=1 ; b<7 ; b ++ )
							if(token==eav->m_strWords[b])
								if(personIndex)
								{
									if(personIndex==b)
									{
										if(token!=_T("will"))
										{
											tense=SIMPLE_PRESENT;
											pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
										}
										bValid=TRUE;
									}
								}
								else // personIndex==0
								{
									if(token!=_T("will"))
									{
										tense=SIMPLE_PRESENT;
										pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
									}
									bValid=TRUE;
								}
						for(//BYTE 
							b=7;b<13;b++)
							if(token==eav->m_strWords[b])
								if(personIndex)
								{
									if(personIndex+6==b)
									{
										if(token!=_T("will"))
										{
											tense=SIMPLE_PAST;
											pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
										}
										bValid=TRUE;
									}
								}
								else // personIndex==0
								{
									if(token!=_T("will"))
									{
										tense=SIMPLE_PAST;
										pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
									}
									bValid=TRUE;
								}
					}
				}
				else if(token==eav->m_strWords[13]) // wenn Partizip Perfekt
				{ // zum Beispiel: 'been' (von 'to be')
					if(pWordNodeWhereAnEnglishPredicateIsEncapsulated)
					{
						if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
							m_pWord)==typeid(EnglishAuxiliaryVerb))
						{
							EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
								(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
							if(eav->m_strWords[0]==_T("have"))
							{
								pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
								tense=PRESENT_PERFECT;
								bValid=TRUE;
							}
						}
					}
				}
				else
				{
					if(token==_T("being"))
					{
						pWordNodeWhereAnEnglishPredicateIsEncapsulated=NULL;
						tense=PRESENT_PROGRESSIVE;
						bActiveClause=FALSE;
						bValid=TRUE;
					}
					else
					{
						if(!pWordNodeWhereAnEnglishPredicateIsEncapsulated)
						{
							for(BYTE b=1;b<7;b++)
								if(token==eav->m_strWords[b])
									if(personIndex)
									{
										if(personIndex==b)
										{
											tense=SIMPLE_PRESENT;
											pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
											bValid=TRUE;
										}
									}
									else
									{
										tense=SIMPLE_PRESENT;
										pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
										bValid=TRUE;
									}
							for(b=7;b<13;b++)
								if(token==eav->m_strWords[b])
									if(personIndex)
									{
										if(personIndex+6==b)
										{
											tense=SIMPLE_PAST;
											pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
											bValid=TRUE;
										}
									}
									else
									{
										tense=SIMPLE_PAST;
										pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
										bValid=TRUE;
									}
						}
					}
				bAux=TRUE;
				ss.m_bBe=TRUE;
				}
				if(!bValid)
				{
#ifdef _DEBUG
					printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv,\
BYTE personIndex,const DWORD & dwFlags) - return FALSE\n");
#endif
					return FALSE;
				}
			}
			//}
			if(!bAuxiliaryVerb)//else // kein Hilfverb, also ein Vollverb
			{
				#ifdef _DEBUG
				printf("!bAuxiliaryVerb\n");
				#endif
				if(pWordNodeWhereAnEnglishPredicateIsEncapsulated) // wenn es bereits ein Prädikat
				{
#ifdef _DEBUG
					printf("pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
#endif
#ifdef _DEBUG
						printf("typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulate\
d->m_pWord): %s\n",typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord));
						if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)==
							typeid(EnglishVerb))
						{
							EnglishVerb * ev=dynamic_cast<EnglishVerb*>
								(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
							printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
						}
#endif
				}
				strToken.push_back(token);
				if(pWordNodeWhereAnEnglishVerbIsEncapsulated)
				{ // es können keine 2 Vollverben hintereinander stehen
#ifdef _DEBUG
					printf("pWordNodeWhereAnEnglishVerbIsEncapsulated\n");
#endif
				}
				if(pWordNode->m_pWordNodeNext)///IsSimplePast(token))
				{
					#ifdef _DEBUG
					printf("pWordNode->m_pWordNodeNext\n");
					#endif
					EnglishVerb * ev=dynamic_cast<EnglishVerb*>(pWordNode->m_pWord);
					//GermanVerb * gv=dynamic_cast<GermanVerb*>(pVocNode->next->voc);
					if(ev)
					{
						#ifdef _DEBUG
						printf("ev\n");
						printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
						#endif
						if(token==ev->m_strInfinitive+_T("ing")) // Bespiel: 'walkING'
						{
							#ifdef _DEBUG
							printf("token==ev->m_strInfinitive+_T(\"ing\")\n");
							#endif
							BOOL bValid=FALSE;
							if(pWordNodeWhereAnEnglishPredicateIsEncapsulated)
							{
								if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
									m_pWord)==typeid(EnglishAuxiliaryVerb))
								{
									EnglishAuxiliaryVerb * eav=dynamic_cast
										<EnglishAuxiliaryVerb *>
										(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
									if(eav->m_strWords[0]==_T("be"))
									{
										pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
										if(tense==SIMPLE_PRESENT) // I am.
											tense=PRESENT_PROGRESSIVE; // I am walking
										if(tense==SIMPLE_PAST) // I was.
											tense=PAST_PROGRESSIVE; // I was walking.
										if(tense==PRESENT_PERFECT) // I have been.
											tense=PRESENT_PERFECT_PROGRESSIVE; // I have been walking.
										bValid=TRUE;
									}
								}
								if(!bValid)
								{
									#ifdef _DEBUG
									printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - return FALSE - predicate nicht 'be'\n");
									#endif
									return FALSE;
								}
							}
							else
							{
								#ifdef _DEBUG
								printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - return FALSE - pVocNodeWhereAn\
EnglishPredicateIsEncapsulated==NULL\n");
								#endif
								return FALSE;
							}
						}
						if(token==ev->m_strInfinitive) // wenn das Verb im Infinitiv ist ('to walk')
						{
							#ifdef _DEBUG
							printf("token==ev->m_strInfinitive\n");
							#endif
							BOOL bValid=FALSE;
							if(pWordNodeWhereAnEnglishPredicateIsEncapsulated)
							{
								#ifdef _DEBUG
								printf("pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
								#endif
								if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
									m_pWord)==typeid(EnglishVerb))
								{
									if(!bTo)
									{
										#ifdef _DEBUG
										printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - return FALSE - kein \"to\"\n");
										#endif
										return FALSE;
									}
									pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
									bValid=TRUE;
								}
								if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
									m_pWord)==typeid(EnglishAuxiliaryVerb))
								{
									#ifdef _DEBUG
									printf("typeid(*pWordNodeWhereAnEnglishPredicateIsEncaps\
ulated->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
									#endif
									bValid=TRUE;
									EnglishAuxiliaryVerb * eav=dynamic_cast
										<EnglishAuxiliaryVerb *>
										(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
									#ifdef _DEBUG
									printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]);
									#endif
									if(eav->m_strWords[0]==_T("do") || eav->m_strWords[0]==
										_T("be") || eav->m_strWords[0]==_T("have"))
									// zum Beispiel: I do not like you.
										pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
									else
									{
									//pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNode;
										if(bTo)
										{
											#ifdef _DEBUG
											printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - return FALSE - kein \"to\"\n");
											#endif
											return FALSE;
										}
										pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
									}
								}
							}
							else // pVocNodeWhereAnEnglishPredicateIsEncapsulated==NULL
							{
							#ifdef _DEBUG
								printf("pWordNodeWhereAnEnglishPredicateIsEncapsulated==NULL\n");
							#endif
								if(personIndex==3) // 2. Person Singular ("you" englisch: "du")
								{
									#ifdef _DEBUG
									printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - ENDE - return FALSE- personIndex==2\n");
									#endif
									return FALSE;
								}
								if(tense==FUTURE)
									pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
								else
									pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
								bValid=TRUE;
								if(tense!=FUTURE && tense!=SIMPLE_PAST)
									tense=SIMPLE_PRESENT;
							}
							if(!bValid)
							{
								#ifdef _DEBUG
								printf("ParseMiddle Ende FALSE\n");
								#endif
								return FALSE;
							}
							//pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNode;
						}
						if(ev->m_strInfinitive.GetLength()>0)
							if(ev->m_strInfinitive.Right(1)==_T("o")) // wenn der letzte Buchstabe des
								// Verbes ein 'o' ist Beispiel: 'go'
							{
								if(token==ev->m_strInfinitive+_T("es")) // Wenn das Ende des Verbes
								{ // 'es' ist. Beispiel: 'goes' (Verb ist in der dritten Person 
									// Singular)
									#ifdef _DEBUG
									printf("token==ev->infinitive+_T(\"es\")\n");
									#endif
									if(personIndex && personIndex!=3)
									{
										#ifdef _DEBUG
										printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - ENDE - return FALSE- personIndex!=2\n");
										#endif
										return FALSE;
									}
									tense=SIMPLE_PRESENT;
									pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
								}
							}
							else if(token==ev->m_strInfinitive+_T("s")) // wenn das Verb in der 
							{ // dritten Person Singular (he/she/it) ist (Bespiel: 'plays')
								#ifdef _DEBUG
								printf("token==ev->m_strInfinitive+_T(\"s\")\n");
								#endif
								if(personIndex && personIndex!=3)
								{
									#ifdef _DEBUG
									printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - ENDE - return FALSE- personIndex!=2\n");
									#endif
									return FALSE;
								}
								if(pWordNodeWhereAnEnglishPredicateIsEncapsulated)
								{
									#ifdef _DEBUG
									printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - ENDE - return FALSE- 3. Person-'s' \
trotz gebeugten Verbes\n");
									#endif
									return FALSE;
								}
								tense=SIMPLE_PRESENT;
								pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
							}
						if(token==ev->m_strPastTense) // wenn Verb in der Vergangenheit ist
						{
							#ifdef _DEBUG
							printf("token==ev->m_strPastTense\n");
							#endif
							if(pWordNodeWhereAnEnglishPredicateIsEncapsulated) // wenn es bereits ein Prädikat
							{
								BOOL bValid=FALSE;
								// wenn das Prädikat ein Hilfsverb ist (Beispiel: 'do')
								if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
									m_pWord)==typeid(EnglishAuxiliaryVerb))
								{
									#ifdef _DEBUG
									printf("typeid(*pWordNodeWhereAnEnglishPredicateIsEn\
capsulated->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
									#endif
									EnglishAuxiliaryVerb * eav=dynamic_cast
										<EnglishAuxiliaryVerb*>
										(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
									if(eav->m_strWords[0]!=_T("be") && eav->m_strWords[0]!=
										_T("have") && eav->m_strWords[0]!=_T("do") && 
										token==ev->m_strPastParticiple)
									{ // wenn anderes Hilfsverb als 'be' oder 'have', also zum Beispiel:
										// can und wenn das Partizip Perfekt dem Präteritum entspricht
										// Bespiel: 'to make' - 'has made' 'made'
										//tense=SIMPLE_PAST;
										//pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNode;
										if(typeid(*pWordNodeWhereAnEnglishVerbIsEncapsulated->
											m_pWord)==typeid(EnglishAuxiliaryVerb))
										{
											EnglishAuxiliaryVerb * eavVerb=dynamic_cast
												<EnglishAuxiliaryVerb*>
												(pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWord);
											if(eavVerb->m_strWords[0]==_T("be")) // Infinitiv ist 'be'
											// das Mittelteil könnte somit zum Beispiel 'can be' sein
												bValid=TRUE;
										}
									}
									if(eav->m_strWords[0]==_T("be") || eav->m_strWords[0]==_T("have"))
									{
										if(token==ev->m_strPastParticiple) // wenn das Token
										// auch dem Partizip Perfekt entspricht
										// Bespiel: to make - MADE - have MADE
											bValid=TRUE;
									}
									if(!bValid)
									{
										#ifdef _DEBUG
										printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - ENDE - return FALSE - !bValid\n");
										#endif
										return FALSE;
									}
								}
								else
								{
									#ifdef _DEBUG
									printf("!typeid(*pWordNodeWhereAnEnglishPredicateIsEnc\
apsulated->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
									#endif
									tense=SIMPLE_PAST;
									pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
								}
								if(token==ev->m_strInfinitive && personIndex!=3)
								{ // wenn Präsens und Präteritum eines Verbes gleich sind (zum Beispiel
									// 'to cut') und wenn nicht 3. Person Singular, dann können das
									// 2 verschiedene Zeitformen (Präsens und Präteritum) sein.
									tense=SIMPLE_PRESENT_OR_SIMPLE_PAST;
								}
								else
									tense=SIMPLE_PAST;
							}
							else
							{
								if(tense!=PRESENT_PROGRESSIVE)
								{
									pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
									if(token==ev->m_strInfinitive && personIndex!=3)
									{ // wenn Präsens und Präteritum eines Verbes gleich sind (zum Beispiel
										// 'to cut') und wenn nicht 3. Person Singular, dann können das
										// 2 verschiedene Zeitformen (Präsens und Präteritum) sein.
										tense=SIMPLE_PRESENT_OR_SIMPLE_PAST;
									}
									else
										tense=SIMPLE_PAST;
								}
							}
						}
						if(token==ev->m_strPastParticiple) // Perfekt
						{
#ifdef _DEBUG
							printf("token==ev->m_strPastParticiple\n");
#endif
							if(pWordNodeWhereAnEnglishPredicateIsEncapsulated) // wenn es bereits ein Prädikat
							{
#ifdef _DEBUG
								printf("pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
								printf("typeid(*pWordNodeWhereAnEnglishPredicateIsEnca\
psulated->m_pWord): %s\n",typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
									m_pWord).name());
#endif
								if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)
									==typeid(EnglishAuxiliaryVerb))
								{ // wenn das Prädikat ein Hilfsverb ist (Beispiel: 'do')
									EnglishAuxiliaryVerb * eav=dynamic_cast
										<EnglishAuxiliaryVerb*>
										(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
#ifdef _DEBUG
									printf("typeid(*pWordNodeWhereAnEnglishPredicateIs\
Encapsulated->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
#endif
									if(/*eav->words[0]==_T("be") || */eav->m_strWords[0]==_T("have"))
									{
#ifdef _DEBUG
										//printf("eav->words[0]==_T(\"be\") || eav->words[0]==_T(\"have\")\n");
										printf("eav->words[0]==_T(\"have\")\n");
#endif
										pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
										tense=PRESENT_PERFECT;
									}
#ifdef _DEBUG
									printf("nach: if(eav->m_strWords[0]==_T(\"have\"))\n");
#endif
									if(eav->m_strWords[0]==_T("be") && tense==PRESENT_PERFECT || 
										tense==SIMPLE_PAST)
									{ // wenn: 'have been'
										#ifdef _DEBUG
										printf("eav->m_strWords[0]==_T(\"be\") && tense==PRESENT_PERFECT\
 || tense==SIMPLE_PAST\n");
										#endif
										bActiveClause=FALSE; // es ist ein Passivsatz
										pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
									}
#ifdef _DEBUG
									printf("if(eav->m_strWords[0]==_T(\"be\") && tense==\
PRESENT_PERFECT || tense==SIMPLE_PAST)\n");
#endif
									if(eav->m_strWords[0]!=_T("be") && eav->m_strWords[0]!=_T("have"))
									{ // wenn anderes Hilfsverb als 'be' oder 'have', also zum Beispiel:
										// can und wenn das Partizip Perfekt dem Präteritum entspricht
										// Bespiel: 'to make' - 'has made' 'made'
										//tense=SIMPLE_PAST;
										//pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNode;
										if(typeid(*pWordNodeWhereAnEnglishVerbIsEncapsulated->
											m_pWord)==typeid(EnglishAuxiliaryVerb))
										{
											EnglishAuxiliaryVerb * eavVerb=dynamic_cast
												<EnglishAuxiliaryVerb*>
												(pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWord);
											if(eavVerb->m_strWords[0]==_T("be")) // Infinitiv ist 'be'
											{ // das Mittelteil könnte somit zum Beispiel 'can be' sein
												// zusammen mit dem Partizip Perfekt kann das zum Beispiel
												// 'can be used' sein
												pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
												//tense=PRESENT_PERFECT;
												bActiveClause=FALSE;
											}
										}
									}
#ifdef _DEBUG
									printf("nach: if(eav->m_strWords[0]!=_T(\"be\") && \
eav->m_strWords[0]!=_T(\"have\"))\n");
#endif
									CString strBe("be") ;
									eav = GetEnglishAuxiliaryVerb(strBe) ;
									if(strToken.size()>0)
#ifdef _DEBUG
									{
										printf("strToken.size()>0\n");
										printf("strToken.at(0): %s\n",strToken.at(0));
#endif
										if(strToken.at(0)==eav->m_strWords[1] || // am
											strToken.at(0)==eav->m_strWords[2] || // are
											strToken.at(0)==eav->m_strWords[3] || // is
											strToken.at(0)==eav->m_strWords[7] || // was
											strToken.at(0)==eav->m_strWords[8]) // were
										{
											#ifdef _DEBUG
											printf("Form von 'to be'\n");
											#endif
											bActiveClause=FALSE;
											pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
											//ss.tense=SIMPLE_PRESENT_PERFECT;
										}
#ifdef _DEBUG
									}
									else
										printf("!strToken.size()>0\n");
#endif
									if(strToken.size()>1)
									{
										#ifdef _DEBUG
										printf("strToken.size()>1\n");
										#endif
										if(strToken.at(1)==eav->m_strWords[0]) // 'be'
										{ // zum Beispiel: can be used
											#ifdef _DEBUG
											printf("strToken.at(1)==eav->m_strWords[0]\n");
											#endif
											bActiveClause=FALSE;
											pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
										}
									}
								}
								if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
									m_pWord)==typeid(EnglishAuxiliaryVerb))
								{
									//if(ss.tense==FUTURE)
									bActiveClause=FALSE;
								}
								if(tense==PRESENT_PROGRESSIVE || tense==PAST_PROGRESSIVE)
								{
									#ifdef _DEBUG
									printf("tense==PRESENT_PROGRESSIVE || tense==PAST_PROGRESSIVE\n");
									#endif
									bActiveClause=FALSE;
									pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
								}
#ifdef _DEBUG
								if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
									m_pWord)==typeid(EnglishVerb))
								{ // wenn das Prädikat ein Vollverb ist
									EnglishVerb * ev=dynamic_cast<EnglishVerb*>
										(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
									printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
								}
#endif
							}
							else
							{
#ifdef _DEBUG
								printf("pWordNodeWhereAnEnglishPredicateIsEncapsulated==NULL\n");
#endif
								pWordNodeWhereAnEnglishPredicateIsEncapsulated=pWordNode;
							}
						}
					}//token
				}
				bVerb=TRUE;
			}
			if(token==_T("not")) // wenn Negation (Verneinung)
				if(!bAuxiliaryVerb) // wenn noch kein Hilfsverb
				{
					#ifdef _DEBUG
					printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector \
& sv,BYTE personIndex) - ENDE - return FALSE- !bAuxiliaryVerb\n");
					#endif
					return FALSE;
				}
				else
					bNegation=TRUE;
			if(IsEnglishVerb3rdPerson(token))
			{
				TRACE("IsEnglishVerb3rdPerson(token)==TRUE\n");
				b3rdPerson=TRUE;
			}
			if(tense==SIMPLE_PAST)
			{
//				EnglishVerb * ev=GetEnglishVocableOfEnglishVerb(token);
	//			if(token!=ev->infinitive)
		//			return FALSE;
			}
			bValidToken=TRUE;
		}
		/*else if(token==_T("will"))
		{
			if(bFuture || bFrequency)
				return FALSE;
			bFuture=TRUE;
		}*/
		//else if(IsEnglishAdverb(token,'1'))
		if(pWordNodeWhereAnEnglishPredicateIsEncapsulated)
			if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)==
				typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>
					(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
				if(token==ev->m_strPreposition)
					bValidToken=TRUE;
				//printf("ev->infinitive: %s\n",ev->infinitive);
			}
		if(IsEnglishAdverbOfFrequency(token))
		{
			TRACE("IsEnglishAdverbOfFrequency(token)\n");
			if(bVerb || bManner || bFrequency)
			{
				TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & \
sv) - ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & \
sv) - ENDE - return FALSE- bVerb || bManner || bFrequency\n");
				#endif
				return FALSE;
			}
			bFrequency=TRUE;
			EnglishAdverb ea;
			ea.m_bType='1';
			ea.m_strWord=token;
			ss.m_vecEnglishAdverb.push_back(ea);
			bValidToken=TRUE;
		}
		if(IsEnglishAdverb(token,'3'))
		{
			bManner=TRUE;
			EnglishAdverb ea;
			ea.m_bType='1';
			ea.m_strWord=token;
			ss.m_vecEnglishAdverb.push_back(ea);
			bValidToken=TRUE;
		}
		/*if(token=="do" || token=="does" || token=="did")
		{
			if(bAux || bNot)
			{
				TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ENDE - return FALSE\n");
				printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ENDE - return FALSE- bAux || bNot\n");
				return FALSE;
			}
			bDo=TRUE;
			if(token=="did")
				tense=SIMPLE_PAST;
			bValidToken=TRUE;
		}*/
		if(token==_T("not"))
		{
			TRACE("token==\"not\"\n");
			if(bAux)
			{
				if(bFrequency || bManner)// || bDo)
				{
					TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector \
& sv) - ENDE - return FALSE\n");
#ifdef _DEBUG
					printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector \
& sv) - ENDE - return FALSE - bFrequency || bManner || bDo\n");
#endif
					return FALSE;
				}
			}
			else
			{
				if(bManner || bVerb)
				{
					TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & \
sv) - ENDE - return FALSE\n");
					#ifdef _DEBUG
					printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector \
& sv) - ENDE - return FALSE-bManner || bVerb\n");
					#endif
					return FALSE;
				}
			}
			if(!pWordNodeWhereAnEnglishPredicateIsEncapsulated && tense!=FUTURE)
			{ // wenn noch kein gebeugtes Verb und Zeitform nicht Zukunft ist
				#ifdef _DEBUG
				printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & \
sv,BYTE personIndex) - return FALSE - !pWordNodeWhereAnEnglishPredicateIsEnc\
apsulated\n");
				#endif
				return FALSE;
			}
			else
			{
				if(pWordNodeWhereAnEnglishPredicateIsEncapsulated)
					if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->
						m_pWord)!=typeid(EnglishAuxiliaryVerb))
					{
						#ifdef _DEBUG
						printf("BOOL ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - return FALSE - !typeid(EnglishAuxiliaryVerb\n");
						#endif
						return FALSE;
					}
					/*else
					{
						EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>(pVocNodeWhereAnEnglishPredicateIsEncapsulated->voc);
						if(eav)
							if(eav->words[0]!=_T("do"))
							{
								#ifdef _DEBUG
								printf("BOOL ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv,BYTE personIndex) - return FALSE - eav->words[0]!=_T(\"do\")\n");
								#endif
								return FALSE;
							}
					}*/
			}
			TRACE("bNegation=TRUE\n");
			#ifdef _DEBUG
			printf("bNegation=TRUE;\n");
			#endif
			bNegation=TRUE;
			bValidToken=TRUE;
		}
		BOOL bAcceptsTo=TRUE;
		if(token==_T("to"))
		{
			
			if(pWordNodeWhereAnEnglishPredicateIsEncapsulated)
				if(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWordNodeNext)
				{
					GermanVerb * gv=dynamic_cast<GermanVerb *>
						(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWordNodeNext->m_pWord);
					if(gv->m_bCase!=0)
						bValidToken=TRUE;
				}
/*
			//if(pVocNodeWhereAnEnglishPredicateIsEncapsulated)
			//{
			//	if(typeid(*pVocNodeWhereAnEnglishPredicateIsEncapsulated->voc)==typeid(EnglishAuxiliaryVerb))
					// wenn das Prädikat ein Hilsverb ist
			//	{
			//		EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>(pVocNodeWhereAnEnglishPredicateIsEncapsulated->voc);
			//		if(eav)
			//			if(eav->m_bAcceptsTo)
			//				bValidToken=TRUE;
			//	}
			//	if(typeid(*pVocNodeWhereAnEnglishPredicateIsEncapsulated->voc)==typeid(EnglishVerb))
			//	{
			//		EnglishVerb * ev=dynamic_cast<EnglishVerb *>(pVocNodeWhereAnEnglishPredicateIsEncapsulated->voc);
			//		if(ev)
			//			if(ev->next)//m_bAcceptsTo)
			//				bValidToken=TRUE;
			//	}
			//}
			*/
			//if(bAcceptsTo)
			//	bValidToken=TRUE;
			bTo=TRUE;
		}
		if(!bValidToken)
		{
			TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & \
sv) - ENDE - return FALSE\n");
			#ifdef _DEBUG
			printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & \
sv) - ENDE - return FALSE - unbekanntes Wort\n");
			#endif
			return FALSE;
		}
#ifdef _DEBUG
		if(pWordNodeWhereAnEnglishPredicateIsEncapsulated)
		{
			printf("pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
			if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)==
				typeid(EnglishVerb))
				// wenn das Prädikat ein Vollverb ist
			{
				printf("typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->\
m_pWord)==typeid(EnglishVerb)\n");
				EnglishVerb	* ev=dynamic_cast<EnglishVerb	*>
					(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
				printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
			}
			if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)==
				typeid(EnglishAuxiliaryVerb))
			{
				printf("typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_\
pWord)==typeid(EnglishAuxiliaryVerb)\n");
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
				printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]);
			}
		}
		else
			printf("!pWordNodeWhereAnEnglishPredicateIsEncapsulated\n");
		if(pWordNodeWhereAnEnglishVerbIsEncapsulated)
		{
			printf("pWordNodeWhereAnEnglishVerbIsEncapsulated\n");
			if(typeid(*pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWord)==
				typeid(EnglishVerb))
				// wenn das Prädikat ein Vollverb ist
			{
				EnglishVerb	* ev=dynamic_cast<EnglishVerb	*>
					(pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWord);
				printf("ev->m_strInfinitive: %s\n",ev->m_strInfinitive);
			}
			if(typeid(*pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWord)==
				typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWord);
				printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]);
			}
		}
		else
			printf("!pWordNodeWhereAnEnglishVerbIsEncapsulated\n");
		printf("tense: %u\n",tense);
		printf("Schleifenende\n");
#endif
	}
	if(tense==FUTURE)
	{
		if(!pWordNodeWhereAnEnglishVerbIsEncapsulated)
		{
			#ifdef _DEBUG
			printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & \
sv,BYTE personIndex) - ENDE - return FALSE - kein Infinitiv\n");
			#endif
			return FALSE;
		}
	}
	else
	{
		if(!pWordNodeWhereAnEnglishPredicateIsEncapsulated)
		{
			#ifdef _DEBUG
			printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & \
sv,BYTE personIndex) - ENDE - return FALSE - kein Prädikat\n");
			#endif
			return FALSE;
		}
		else
		{
			if(typeid(*pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord)==
				typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
					(pWordNodeWhereAnEnglishPredicateIsEncapsulated->m_pWord);
				if(eav)
					if(eav->m_strWords[0]!=_T("be") && !pWordNodeWhereAnEnglishVerbIsEncapsulated)
					{
						#ifdef _DEBUG
						printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv,BYTE personIndex) - return FALSE - Hilfsverb außer 'to \
be' ohne Vollverb\n");
						#endif
						return FALSE;
					}
			}
		}
	}
	ss.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=
		pWordNodeWhereAnEnglishPredicateIsEncapsulated;
	ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=
		pWordNodeWhereAnEnglishVerbIsEncapsulated;
	ss.m_bActiveClause=bActiveClause;
	ss.m_bTense=tense;
	ss.m_bNegation=bNegation;

	//ss.m_pVocNodeWhereAnEnglishConjunctionIsEncapsulated=pVocNodeWhereAnEnglishConjunctionIsEncapsulated;

	#ifdef _DEBUG
	printf("ss.m_bTense: %u\n",ss.m_bTense);
	if(ss.m_bActiveClause)
		printf("ss.m_bActiveClause\n");
	else
		printf("!ss.m_bActiveClause\n");
	if(ss.m_bNegation)
		printf("ss.m_bNegation\n");
	else
		printf("!ss.m_bNegation\n");
	printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv,\
BYTE personIndex) - ENDE - return TRUE\n");
	#endif
	return TRUE;
}

//diese Funktion wird von ParseClause und ParseRelativeClause aufgerufen
//und liefert den Personalpronomen zurück, zu dem das gebeugte Verb gehört
//der Rückgabewert ist ein Byte, bei dem die Bits von 0 bis 6 angeben, für 
//welchen Personenindex das Prädikat korrekt ist
//Beispiel: "I like you." -> like passt für die folgenden Personenindizes:
//1 ("I like you.") ,2 ("You like me."), 4 ("We like you."), 5 ("You like you."),
//und 6 ("They like you.")
BYTE ParseFullPredicate(Predicate & predicate,BOOL bBe,const CStringVector & sv,
	BYTE personIndex,const DWORD & dwFlags)
{
	bool bFinitePartOfPredicateFound=false;
	BYTE byReturnPersonIndex=0;
	BYTE byPersonIndex;

	//VocNode * pVocNodeWhereAnEnglishConjunctionIsEncapsulated=NULL;
	CStringVector strToken;
	//am Anfang aus den höchstmöglichen Wert setzen
	DWORD dwPositionOfBendAuxiliary=ULONG_MAX;

#ifdef _DEBUG
	printf("ParseFullPredicate(Predicate & predicate,BOOL bBe,const CStringVector & sv,\
BYTE personIndex,const DWORD & dwFlags) - ANFANG\n");
	printf("predicate.m_bTense: %u\n",predicate.m_bTense);
	fflush(stdout);
#endif
	//CString token;
	if(!(sv.size()>0))
	{
		TRACE("ParseFullPredicate ENDE - return FALSE\n");
#ifdef _DEBUG
		printf("ParseFullPredicate(Clause & ss,BOOL bBe,const CStringVector & sv) - ENDE - return FALSE - sv.size()==0\n");
#endif
		return FALSE;
	}
	#ifdef _DEBUG
	{
		for(DWORD i=0;i<sv.size();i++)
		{
			CString token=sv.at(i);
			TRACE("sv.at(%d): %s\n",i,token);
			printf("sv.at(%d): %s\n",i,token);
		}
	}
	#endif
	//predicate.m_adverbs.clear();
	//for(int i=0;i<ss.m_adverbs.size();i++
	BOOL bAuxiliaryVerbOccured=FALSE;
	for(DWORD dwRun=0;dwRun<sv.size() && g_bContinue;++dwRun)
	{
		CString token=sv.at(dwRun);
		if(!(dwFlags&CASE_SENSITIVE)==1)
			token.MakeLower();
		//In MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
#ifdef _WINDOWS
		TRACE("token: %s\n",token);
#endif
		#ifdef _DEBUG
		printf("token: %s\n",token);
		#endif
		BOOL bValidToken=FALSE;
		if((byPersonIndex=ParseBendPartOfPredicate(predicate,bBe,
			GetBetween(sv,0,dwRun),
			personIndex,dwFlags)))
		{
			bFinitePartOfPredicateFound=true;
			byReturnPersonIndex=byPersonIndex;
		}
		else
			if(bFinitePartOfPredicateFound)
			break;
	}
	if(bFinitePartOfPredicateFound && 
		//zu parsender Vektor enthält mindestens noch ein weiteres Token
		dwRun<sv.size())
	{
		if(!ParseNonBendPartOfPredicate(predicate,bBe,
			GetBetween(sv,dwRun,sv.size()-1),
			dwFlags//,predicate.
			//m_pWordNodeWhereTheGermanTranslationIsABendVerb
			))
			byReturnPersonIndex=0;
		//bool bNotFinitePartOfPredicateFound=false;
		//DWORD dwBeginOfNotFinitePartOfPredicate=dwRun;
		//for(;dwRun<sv.size() && g_bContinue;++dwRun)
		//{
		//	CString token=sv.at(dwRun);
		//	if(!(dwFlags&1)==1)
		//		token.MakeLower();
		//	TRACE("token: %s\n",token);
		//	#ifdef _DEBUG
		//	printf("token: %s\n",token);
		//	#endif
		//	BOOL bValidToken=FALSE;
		//	if(ParseNonBendPartOfPredicate(predicate,bBe,
		//		GetBetween(sv,dwBeginOfNotFinitePartOfPredicate,dwRun),
		//		dwFlags//,predicate.
		//		//m_pWordNodeWhereTheGermanTranslationIsABendVerb
		//		))
		//		bNotFinitePartOfPredicateFound=true;
		//	else
		//		break;
		//}
		//der nicht-gebeugte Teil kann ja richtig sein, dann ist
		//byReturnPersonIndex zunächst ungleich 0
		//if(!bNotFinitePartOfPredicateFound)
		//	byReturnPersonIndex=0;
	}
  LOG2N("23.05.2008 10.42.44 "<<byReturnPersonIndex,
    "ParseFullPredicate return"<<byReturnPersonIndex);
	return byReturnPersonIndex;
}//BYTE ParseFullPredicate(Predicate & predicate,BOOL bBe,const CStringVector & sv,
//	BYTE personIndex,const DWORD & dwFlags)

BOOL ParseMiddleEnumerationElement(MiddleEnumerationElement & mee,
	const CStringVector & sv,const DWORD & dwFlags)
{
	TRACE("BOOL ParseMiddleEnumerationElement(MiddleEnumerationElement & mee,\
		const CStringVector & sv) - ANFANG\n");
#ifdef _DEBUG
	for(DWORD dw=0;dw<sv.size();dw++)
	{
		TRACE("sv.at(%u): %s=\n",dw,sv.at(dw));
	}
#endif
	BOOL bAdverbOfManner=FALSE;
	BOOL bAdverbRefersToAdjectiveOrAdverb=FALSE;
	for(DWORD dwRun=0;dwRun<sv.size();dwRun++)
	{
		CString token=sv.at(dwRun);
		if(IsEnglishAdverbRefersToAdjectiveOrAdverb(token))
		{
			mee.m_strAdverbRefersToAdjectiveOrAdverb=token;
			bAdverbRefersToAdjectiveOrAdverb=TRUE;
		}
		else if(IsEnglishAdverbOfManner(token))
		{
			mee.m_strAdverbOfManner=token;
			bAdverbOfManner=TRUE;
		}
		else
		{
			TRACE("BOOL ParseMiddleEnumerationElement(MiddleEnumerationElement & mee,const CStringVector & sv) - ENDE - return FALSE\n");
			return FALSE;
		}
	}
	if(!bAdverbOfManner)
	{
		TRACE("BOOL ParseMiddleEnumerationElement(MiddleEnumerationElement & mee,const CStringVector & sv) - ENDE - return FALSE\n");
		return FALSE;
	}
	TRACE("BOOL ParseMiddleEnumerationElement(MiddleEnumerationElement & mee,const CStringVector & sv) - ENDE - return TRUE\n");
	return TRUE;
}

BYTE ParseObject(Object & object,const PositionCStringVector & psv,BOOL bBe,
	std::vector<Range> & rVecRange,const DWORD & dwFlags)
{
	TRACE("ParseObject ANFANG\n");
	#ifdef _DEBUG
	printf("BOOL ParseObject(Object & object,const CStringVector & sv,BOOL bBe,std::vector<Range> & rVecRange) ANFANG\n");

	for(DWORD i2=0;i2<psv.size();i2++)
	{
		TRACE("psv.at(%d).m_Str: %s\n",i2,psv.at(i2).m_Str);
		printf("psv.at(%d).m_Str: %s\n",i2,psv.at(i2).m_Str);
	}
	#endif
	//Subject subject;
	BOOL bReturn=TRUE;
	BOOL bAnd=FALSE;
	BOOL bColon=FALSE;
	BYTE personIndex=0;
	BYTE returnValue=2;
	int first=0;
	for(DWORD i=0;i<psv.size() && g_bContinue;i++)
	{
		if(psv.at(i).m_Str==_T(","))
		{
			if(bAnd)
			{
				TRACE("ParseObject ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("rVecRange.size(): %u\n",rVecRange.size());
				printf("BYTE ParseObject(Object & object,const PositionCStringVector & psv,BOOL bBe,std::vector<Range> & rVecRange) - return 0 - bAnd\n");
				#endif
				return 0;
			}
			bColon=TRUE;
			//In MFC/Windows the TRACE macro has a variable # of args--I do 
			//not know how to enable variable numbers of args for the same macro name.
			//So I exclude it from non-Windows.
#ifdef _WINDOWS
			TRACE("first: %d,i: %d\n",first,i);
#endif
			PositionCStringVector eeVec=GetBetween(psv,first,i-1);
			EnumerationElement ee;
			std::vector<Range> vecRange;
			BYTE ret=0;
			if(!(ret=ParseObjectEnumerationElement(ee,eeVec,vecRange,dwFlags)))
			{
				TRACE("ParseObject ENDE - return FALSE\n");
				//for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				//	rVecRange.push_back(vecRange.at(dwRun));
				//object.m_enumerationElements.push_back(ee);
				bReturn=FALSE;
				returnValue=1;
				#ifdef _DEBUG
				//printf("BOOL ParseObject(Object & object,const PositionCStringVector & psv,BOOL bBe,std::vector<Range> & rVecRange) - return FALSE\n");
				#endif
			}
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
			//personIndex=GetPersonIndex(personIndex,ret);
			first=i+1;
			object.m_vecEnumerationElement.push_back(ee);
		}
		if(psv.at(i).m_Str==_T("and")) // wenn 'and'
		{ 
			TRACE("and\n");
			if(bAnd)
			{
				TRACE("ParseObject ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("rVecRange.size(): %u\n",rVecRange.size());
				printf("BYTE ParseObject(Object & object,const PositionCStringVector & psv,BOOL bBe,std::vector<Range> & rVecRange) - return 0 - bAnd\n");
				#endif
				return 0;
			}
			bAnd=TRUE;
			//In MFC/Windows the TRACE macro has a variable # of args--I do 
			//not know how to enable variable numbers of args for the same macro name.
			//So I exclude it from non-Windows.
#ifdef _WINDOWS
			TRACE("first: %d,i: %d\n",first,i);
#endif
			PositionCStringVector eeVec=GetBetween(psv,first,i-1);
			EnumerationElement ee;
			std::vector<Range> vecRange;
			BYTE ret=0;
			if(!(ret=ParseObjectEnumerationElement(ee,eeVec,vecRange,dwFlags)))
			{
				TRACE("ParseObject ENDE - return FALSE\n");
				//for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				//	rVecRange.push_back(vecRange.at(dwRun));
				//object.m_enumerationElements.push_back(ee);
				#ifdef _DEBUG
				//printf("BOOL ParseObject(Object & object,const PositionCStringVector & psv,BOOL bBe,std::vector<Range> & rVecRange) - return FALSE\n");
				#endif
				returnValue=1;
				bReturn=FALSE;
			}
			//personIndex=GetPersonIndex(personIndex,ret);
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
			first=i+1;
			object.m_vecEnumerationElement.push_back(ee);
		}
		if((i+1)==psv.size()) // wenn das Ende des Vektors erreicht wird
		{ // (letztes Vektor-Element)
			if(bColon && !bAnd)
			{
				#ifdef _DEBUG
				printf("rVecRange.size(): %u\n",rVecRange.size());
				printf("BYTE ParseObject(Object & object,const CStringVector & sv,BOOL bBe) - return 0 - bColon && !bAnd\n");
				#endif
				return 0;
			}
			//In MFC/Windows the TRACE macro has a variable # of args--I do 
			//not know how to enable variable numbers of args for the same macro name.
			//So I exclude it from non-Windows.
#ifdef _WINDOWS
			TRACE("(i+1)==sv.size()\n");
			TRACE("first: %d,i: %d\n",first,i);
#endif
//			TRACE("sv.at(4): %s\n",sv.at(4));
			PositionCStringVector eeVec=GetBetween(psv,first,i);
			EnumerationElement ee;
			std::vector<Range> vecRange;
			BYTE ret=0;
			if(!(ret=ParseObjectEnumerationElement(ee,eeVec,vecRange,dwFlags)))
			{
				TRACE("ParseObject ENDE - return FALSE\n");
				for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
					rVecRange.push_back(vecRange.at(dwRun));
				object.m_vecEnumerationElement.push_back(ee);
				#ifdef _DEBUG
				printf("rVecRange.size(): %u\n",rVecRange.size());
				printf("object.m_enumerationElements.size(): %u\n",object.m_vecEnumerationElement.size());
				printf("BYTE ParseObject(Object & object,const CStringVector & sv,BOOL bBe) - return 1\n");
				#endif
				return 1;
			}
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
			object.m_vecEnumerationElement.push_back(ee);
			TRACE("ParseObject ENDE - return TRUE\n");
			#ifdef _DEBUG
			printf("rVecRange.size(): %u\n",rVecRange.size());
			printf("BOOL ParseObject(Object & object,const CStringVector & sv,BOOL bBe) - return %u\n",returnValue);
			#endif
			return returnValue;
		}
	}
	if(!bAnd)
	{
		TRACE("ParseObject ENDE - return FALSE\n");
		#ifdef _DEBUG
		printf("rVecRange.size(): %u\n",rVecRange.size());
		printf("BYTE ParseObject(Object & object,const CStringVector & sv,BOOL bBe) - return 0 - !bAnd\n");
		#endif
		return 0;
	}
	TRACE("ParseObject ENDE - return TRUE\n");
	#ifdef _DEBUG
	printf("rVecRange.size(): %u\n",rVecRange.size());
	printf("BOOL ParseObject(Object & object,const CStringVector & sv,BOOL bBe) - return TRUE\n");
	#endif
	return TRUE;
}

BOOL ParseObjectEnumerationElement(EnumerationElement & ee,
	const PositionCStringVector & psv,std::vector<Range> & rVecRange,
	const DWORD & dwFlags)
{
	BYTE ret=0;
	DWORD dwRun ;
	Gerund gerund;
	std::vector<Range> vecRange;
	TRACE("BOOL ParseObjectEnumerationElement(EnumerationElement & ee,const CStringVector & sv) - ANFANG\n");
	#ifdef _DEBUG
	printf("BOOL ParseObjectEnumerationElement(EnumerationElement & ee,const PositionCStringVector & psv,std::vector<Range> & rVecRange) - ANFANG\n");
	for(DWORD i2=0;i2<psv.size();i2++)
	{
		TRACE("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
		printf("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
	}
	#endif
	BYTE returnValue=2;
	BYTE personIndex=0;
	if(psv.size()==0)
	{
		#ifdef _DEBUG
		printf("BOOL ParseObjectEnumerationElement(EnumerationElement & ee,const CStringVector & sv) - return FALSE -kein Token\n");
		#endif
		return FALSE;
	}
	if(psv.size()==1)
	{
		CString str=psv.at(0).m_Str;
		//if(!(dwFlags&CASE_SENSITIVE==1))
		if(!(dwFlags&CASE_SENSITIVE))
			str.MakeLower();
			if(str==_T("me"))
			{
				ee.m_personalPronoun=1;
				return returnValue;
			}
			if(str==_T("you"))
			{
				ee.m_personalPronoun=2;
				return returnValue;
			}
			if(str==_T("him"))
			{
				ee.m_personalPronoun=3;
				return returnValue;
			}
			if(str==_T("her"))
			{
				ee.m_personalPronoun=4;
				return returnValue;
			}
			if(str==_T("it"))
			{
				ee.m_personalPronoun=5;
				return returnValue;
			}
			if(str==_T("us"))
			{
				ee.m_personalPronoun=6;
				return returnValue;
			}
			if(str==_T("them"))
			{
				ee.m_personalPronoun=7;
				return returnValue;
			}
	}
	EnglishNounRecord enr;
	if(ParsePersonalPronounOrNounStruct(enr,psv,dwFlags,FALSE))
	{
		ee.m_pEnr=new EnglishNounRecord;
		*ee.m_pEnr=enr;
		#ifdef _DEBUG
		printf("BOOL ParseObjectEnumerationElement(EnumerationElement & ee,\
const CStringVector & sv) ENDE return TRUE\n");
		#endif
		return returnValue;
	}
	BOOL bValidDefiniteArticleNoun=FALSE;
	DWORD dwValidDefiniteArticleNounEnd=0;
	//die Schleife wird bis zum Ende des Vektors ausgeführt, weil auch wenn
	//man bereits ein gültiges Konstrukt aus bestimmtem Artikel und Nomen 
	//gefunden hat, kann es noch ein Nomen geben, das den oder die Token
	//des existierden Nomens beinhaltet: "the vacuum" "the vacuum cleaner"
	for(//DWORD 
		dwRun=0;dwRun<psv.size() && g_bContinue;dwRun++)
	{
		BYTE ret=0;
		EnglishNounRecord enr;
		CStringVector sv = GetBetweenAsCStringVector(psv,0,dwRun) ;
		if(ParseDefiniteArticleNoun(enr,sv,
			dwFlags,FALSE))
		{ // z.B.: The school
			if(ee.m_pEnr)
				delete ee.m_pEnr;
			ee.m_pEnr=new EnglishNounRecord;
			*ee.m_pEnr=enr;
			bValidDefiniteArticleNoun=TRUE;
			dwValidDefiniteArticleNounEnd=dwRun;
			#ifdef _DEBUG
			printf("ee.m_pEnr->m_Noun.size(): %u\n",ee.m_pEnr->m_Noun.size());
			#endif
		}
	}
	if(bValidDefiniteArticleNoun && psv.size()>dwValidDefiniteArticleNounEnd+1)
	{
		OfPhrase op;
		if(ParseOfPhrase(op,GetBetween(psv,dwValidDefiniteArticleNounEnd+1,
			psv.size()),dwFlags))
		{ // z.B.: of my father
			if(ee.m_pOfPhrase)
				delete ee.m_pOfPhrase;
			ee.m_pOfPhrase=new OfPhrase;
			*ee.m_pOfPhrase=op;
			#ifdef _DEBUG
			printf("op.m_Enr.m_Noun.size(): %u\n",op.m_Enr.m_Noun.size());
			printf("ee.m_pOfPhrase->m_Enr.m_Noun.size(): %u\n",ee.m_pOfPhrase->
				m_Enr.m_Noun.size());
			printf("personIndex: %u\n",personIndex);
			printf("BOOL ParseObjectEnumerationElement(EnumerationElement & ee,\
const CStringVector & sv) ENDE return returnValue(%u)\n",returnValue);
			#endif
			return returnValue;
		}
		//wenn kein Of-Phrase, dann ee.enr unbedingt löschen, denn sonst kann es 
		//passieren, dass das Aufzählungselement (EnumerationElement) gleichzeitig 
		//einen Eigennamen (ee.m_vecStrPlainText) und ein Substantiv mit bestimmtem
		//Artikel (ee.enr) enthält.
		//Beispiel: "I go to the cinema tomorrow." wird dann zu: 
		//"Ich gehe zu the cinema tomorrow dem Kino." wenn als einziges Wort
		//tomorrow unbekannt ist
		//else
		//{
		//	delete ee.enr;
		//	ee.enr=NULL;
		//}
	}
	//dwFlags bitweise-Und 2147483648: nur wenn das 32. Bit gesetzt ist, ist 
	//das Ergebnis 2147483648. Dieses Ergebnis bedeutet, dass das Subjekt nicht 
	//nach Relativsätzen überprüft werden soll
	if(bValidDefiniteArticleNoun && psv.size() && (dwFlags&2147483648)!=
		2147483648>dwValidDefiniteArticleNounEnd+1)
	{
		BYTE bReturnOfParseRelativeClause=0;
		RelativeClause relativeClause;
		bReturnOfParseRelativeClause=ParseRelativeClause(relativeClause,
			GetBetween(psv,dwValidDefiniteArticleNounEnd+1,psv.size()),vecRange,
			dwFlags);
		if(bReturnOfParseRelativeClause!=0)
		{
			//wenn der Rückgabewert von ParseGerund() 2 ist, bedeutet das, dass
			//das Subjekt des Gerundiums ein Eigenname ist.
			//wenn es im Aufzählungselement einen Relativsatz gibt, kann es nicht 
			//auch gleichzeitig ein Gerundium geben. Also muss das Gerundium 
			//gelöscht werden.
			//if(ret==2)
			//{
			//	delete ee.m_pGerund;
			//	ee.m_pGerund=NULL;
			//}
			ee.m_pRelativeClause=new RelativeClause(relativeClause);
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
#ifdef _DEBUG
			printf("BYTE ParseEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv) return personIndex\n");
#endif
			//bitweise-Oder verknüpfen: das 8. Bit setzen. Das bedeutet, dass 
			//sich im Aufzählungselement (EnumerationElement) ein Relativsatz 
			//befand
			return personIndex|128;
		}
	}
	//wenn das 31te Bit gesetzt ist (=wenn das Prädikat ein Gerundium erlaubt)
	if((dwFlags&PREDICATE_VERB_ALLOWS_GERUND)==PREDICATE_VERB_ALLOWS_GERUND)
		if((ret=ParseGerund(gerund,FALSE,TRUE,psv,vecRange,dwFlags,FALSE)))
		//if(ParseGerund(gerund,sv))
		{
			returnValue=ret;
			if(ee.m_pGerund)
				delete ee.m_pGerund;
			ee.m_pGerund=new Gerund;
			*ee.m_pGerund=gerund;
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
			//if(ret==1)
			//{
			#ifdef _DEBUG
				printf("BOOL ParseObjectEnumerationElement(EnumerationElement & ee,const CStringVector & sv) ENDE return TRUE\n");
			#endif
				return returnValue;
			//}
		}
	#ifdef _DEBUG
	printf(" psv.size(): %u\n",psv.size());
	#endif
	for(//DWORD 
		dwRun=0;dwRun<psv.size();dwRun++)
	{
		ee.m_vecStrPlainText.push_back(psv.at(dwRun).m_Str);
		//if(dwRun>0 && dwRun<psv.size()-1)
		//	ee.m_plainText+=" ";
		//ee.m_plainText+=psv.at(dwRun).m_Str;
	}
	Range range;
	range.m_dwStart=psv.at(0).m_dwStart;
	range.m_dwEnd=psv.at(psv.size()-1).m_dwStart+psv.at(psv.size()-1).m_Str.GetLength();
	rVecRange.push_back(range);
	#ifdef _DEBUG
	printf("rVecRange.size(): %u range.m_dwStart: %u range.m_dwEnd: %u\n",
		rVecRange.size(),range.m_dwStart,range.m_dwEnd);
	printf("BOOL ParseObjectEnumerationElement(EnumerationElement & ee,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange - return FALSE\n");
	#endif
	return FALSE; // nicht erkannt oder Eigenname, also einen Index von 3
}

BOOL ParseOfPhrase(OfPhrase & op,const PositionCStringVector & psv,
	const DWORD & dwFlags)
{
	TRACE("BOOL ParseOfPhrase(OfPhrase op,const CStringVector & sv) - ANFANG\n");
	#ifdef _DEBUG
	printf("BOOL ParseOfPhrase(OfPhrase op,const CStringVector & sv) - ANFANG\n");
	for(DWORD i2=0;i2<psv.size();i2++)
	{
		TRACE("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
		printf("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
	}
	#endif
	if(psv.size()>1)
	{
		CString str=psv.at(0).m_Str;
		//if(!(dwFlags&CASE_SENSITIVE==1))
		if(!(dwFlags&CASE_SENSITIVE))
			str.MakeLower();
		if(str==_T("of"))
		{
			if(ParsePersonalPronounOrNounStruct(op.m_Enr,GetBetween(psv,1,
				psv.size()),dwFlags,FALSE))
			{
				#ifdef _DEBUG
				printf("op.m_Enr->m_Noun.size(): %u\n",op.m_Enr.m_Noun.size());
				TRACE("BOOL ParseOfPhrase(OfPhrase op,const CStringVector & sv) - ENDE - TRUE\n");
				printf("BOOL ParseOfPhrase(OfPhrase op,const CStringVector & sv) - ENDE - TRUE\n");
				#endif
				return TRUE;
			}
		}
	}
	TRACE("BOOL ParseOfPhrase(OfPhrase op,const CStringVector & sv) - ENDE - FALSE\n");
	#ifdef _DEBUG
	printf("BOOL ParseOfPhrase(OfPhrase op,const CStringVector & sv) - ENDE - FALSE\n");
	#endif
	return FALSE;
}

BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const 
	PositionCStringVector & psv,const DWORD & dwFlags,BYTE 
	bContainsFirstTokenOfSentence)
{
	BOOL bAdjective=FALSE;
	BOOL bDefiniteArticle=FALSE;
	BOOL bPossessivePronoun=FALSE;
	BYTE personIndex=0;
	BOOL bEnglishNoun=FALSE;
	DWORD dwBeginOfEnglishNoun=0;
	TRACE("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const PositionCStringVector & psv) ANFANG\n");
	#ifdef _DEBUG
	printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const PositionCStringVector & psv) ANFANG\n");
	printf("psv.size(): %d\n",psv.size());
	for(DWORD i2=0;i2<psv.size();i2++)
	{
		printf("psv.at(%d).m_Str: %s\n",i2,psv.at(i2).m_Str);
	}
	#endif
	//enr=new EnglishNounRecord;
	for(int i=psv.size()-1;i>=0 && g_bContinue;i--)
	{
		//In MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
#ifdef _WINDOWS
		TRACE("psv.at(i).m_Str: %s\n",psv.at(i).m_Str);
#endif
		#ifdef _DEBUG
		printf("psv.size()-1: %d psv.at(%d).m_Str: %s\n",psv.size()-1,i,psv.at(i).m_Str);
		#endif
		#ifdef _DEBUG
		CStringVector sv2=GetBetweenAsCStringVector(psv,i,psv.size()-1);
		for(DWORD i2=0;i2<sv2.size();i2++)
			printf("sv2.at(%d): %s\n",i2,sv2.at(i2));
		#endif
		//if(i<sv.size()-1)
		//{
		CStringVector vecCStr=GetBetweenAsCStringVector(psv,i,psv.size()-1);
		if(!(dwFlags&CASE_SENSITIVE)==1)
			for(DWORD dwRun=0;dwRun<vecCStr.size();dwRun++)
				vecCStr.at(dwRun).MakeLower();
		else if(bContainsFirstTokenOfSentence)
			MakeFirstLetterLower(vecCStr.at(0));
		std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> ptvwaenie=
			GetVecPointerToWordNodeWhereAnEnglishNounIsEncapsulated(vecCStr,dwFlags);
		if(ptvwaenie.size()>0)
		{
			dwBeginOfEnglishNoun=i;
			bEnglishNoun=TRUE;
			if(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
				enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.clear();
			enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated=ptvwaenie;
			if(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).m_bSingular)
				personIndex=3;
			else
				personIndex=6;
		}
		#ifdef _DEBUG
		printf("enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size(): %u\n",
			enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size());
		#endif
	}
	if(bEnglishNoun) // nur wenn ein englisches Nomen gefunden wurde
	{
		BYTE bAdjectiveType=0;
		BYTE bComperative=0;
		BYTE bSuperlative=0;
		DWORD dwIndex ;
		#ifdef _DEBUG
		printf("bEnglishNoun\n");
		#endif
		//enr->m_Noun=
		//Append(enr.m_Noun,GetBetween(psv,dwBeginOfEnglishNoun,psv.size()));
		CString strIndefiniteArticle;
		WordNode * pWordNode;
		for(dwIndex=0;dwIndex<dwBeginOfEnglishNoun && g_bContinue;dwIndex++)
		{
			CString token=psv.at(dwIndex).m_Str;
			if(!(dwFlags&CASE_SENSITIVE)==1)
				token.MakeLower();
			else if(bContainsFirstTokenOfSentence)
			{
				bContainsFirstTokenOfSentence=FALSE;
				MakeFirstLetterLower(token);
			}
			#ifdef _DEBUG
			printf("token: %s\n",token);
			#endif
			if(token==_T("a") || token==_T("an"))
			{
				if(enr.bIndefiniteArticle || enr.bDefiniteArticle || 
				 	enr.m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated ||
					enr.m_possessivePronoun ||
					enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
					enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
				{
					#ifdef _DEBUG
					printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - enr.bIndefiniteArticle || \
enr.bDefiniteArticle || enr.m_PointerToVocNodeWhereAnEnglishPronounIsEncapsulated \
|| enr.m_possessivePronoun || \
enr.m_vecPointerToVocNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || \
enr.m_PointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated");
					#endif
					return 0;
				}
				strIndefiniteArticle=token;
				enr.bIndefiniteArticle=TRUE;
			}
			else if(token==_T("the"))
			{
				if(enr.bIndefiniteArticle || enr.bDefiniteArticle || 
					enr.m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated ||
					enr.m_possessivePronoun ||
					enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
					enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
				{
					#ifdef _DEBUG
					printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - enr.bIndefiniteArticle || \
enr.bDefiniteArticle || enr.m_PointerToVocNodeWhereAnEnglishPronounIsEncapsulated \
|| enr.m_possessivePronoun || \
enr.m_vecPointerToVocNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || \
enr.m_PointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated");
					#endif
					return 0;
				}
				enr.bDefiniteArticle=TRUE;
			}
			else if((pWordNode=
				GetPointerToWordNodeWhereAnEnglishPronounIsEncapsulated(token)))
			{
				if(enr.bIndefiniteArticle || enr.bDefiniteArticle || 
					enr.m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated ||
					enr.m_possessivePronoun ||
					enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
					enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
				{
					#ifdef _DEBUG
					printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - enr.bIndefiniteArticle || \
enr.bDefiniteArticle || enr.m_PointerToVocNodeWhereAnEnglishPronounIsEncapsulated \
|| enr.m_possessivePronoun || \
enr.m_vecPointerToVocNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || \
enr.m_PointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated");
					#endif
					return 0;
				}
				#ifdef _DEBUG
				printf("englisches Pronom gefunden\n");
				#endif
				enr.m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated=pWordNode;
			}
			else if((pWordNode=
				GetPointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated(
				token)))
			{
				if(enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
					enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
				{
					#ifdef _DEBUG
					printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - \
enr.m_vecPointerToVocNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || \
enr.m_PointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated");
					#endif
					return 0;
				}
				EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
				if(ea->m_strWord.GetLength()>1)
					if(IsVowel(ea->m_strWord.Left(1))) // wenn der Anfangsbuchstabe ein Vokal ist
					{
						if(strIndefiniteArticle==_T("a"))
						{
							#ifdef _DEBUG
							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Adverbs\
ist ein Vokal, aber der unbestimmte Artikel ist 'a'");
							#endif
							return 0;
						}
					}
					else // der Anfangsbuchstabe ist kein Vokal
					{
						if(strIndefiniteArticle==_T("an"))
						{
							#ifdef _DEBUG
							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Adverbs\
ist kein Vokal, aber der unbestimmte Artikel ist 'an'");
							#endif
							return 0;
						}
					}
				enr.
					m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated=
					pWordNode;
			}
			/*else if((pVocNode=GetPointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAnAdverbIsEncapsulated(token)))
			{
			}*/
			else if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated(
				token,dwFlags,bAdjectiveType)))//IsEnglishAdjective(token))
			{
				//enr.m_
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective *>
					(pWordNode->m_pWord);
				if(ea)
				{
					//wenn das Adjektiv nur als Umstandsbestimmung der Art und Weise
					//benutzt werden darf
					//Beispiel für ein Adjektiv, das nur als Umstandsbestimmung der Art und 
					//Weise benutzt werden darf: "ill": "I am ill."
					if(ea->m_bAllowedPlace==2)
					{
						#ifdef _DEBUG
						printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
	const CStringVector & sv)\n - return 0 - das Adjektiv darf nur als \
	Umstandsbestimmung der Art und Weise benutzt werden");
						#endif
						return 0;
					}	
					if(ea->m_strPositiv.GetLength()>1)
						if(IsVowel(ea->m_strPositiv.Left(1))) // wenn der Anfangsbuchstabe ein Vokal ist
						{
							if(strIndefiniteArticle==_T("a"))
							{
								#ifdef _DEBUG
								printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
	const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Adjektivs\
	ist ein Vokal, aber der unbestimmte Artikel ist 'a'");
								#endif
								return 0;
							}
						}
						else // der Anfangsbuchstabe ist kein Vokal
						{
							if(strIndefiniteArticle==_T("an"))
							{
								#ifdef _DEBUG
								printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
	const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Adjektivs\
	ist kein Vokal, aber der unbestimmte Artikel ist 'an'");
								#endif
								return 0;
							}
						}
				}
				if(token==_T("more"))
				{
					bComperative=3;
				}
				if(token==_T("most"))
				{
					bSuperlative=3;
				}	
				if(enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
					enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
				{
					if(bComperative || bSuperlative)
					{
						if(!ea->m_bMoreMost)
						{
							#ifdef _DEBUG
							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - more oder most aber ein \
Adjektiv, dessen Steigerung nicht durch more/most gebildet wird\n");
							#endif
							return 0;
						}
						enr.m_Adjectives.clear();
						enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.clear();
						enr.m_vecAdjectiveType.clear();
						bAdjectiveType=2;
					}
					else
					{
					#ifdef _DEBUG
					printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - \
enr.m_vecPointerToVocNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || \
enr.m_PointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverb\
IsEncapsulated und weder bComperative noch bSuperlative\n");
					#endif
					return 0;
					}
				}
				//wenn das Adjektiv im Superlativ ist und entweder kein bestimmter Artikel
				//oder ein unbestimmter Artikel (wenn Superlativ, dann muss vor dem 
				//Adjektiv ein bestimmter Artikel stehen)
				if(bSuperlative && (!enr.bDefiniteArticle || enr.bIndefiniteArticle))
				{
					#ifdef _DEBUG
					printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - Superlativ aber entweder kein \
bestimmter Artikel oder ein unbestimmter Artikel\n");
					#endif
					return 0;
				}
				if(bComperative)
					bComperative=1;
				if(bSuperlative)
					bSuperlative=1;
				enr.m_Adjectives.push_back(token);
				enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.
					push_back(pWordNode);
				enr.m_vecAdjectiveType.push_back(bAdjectiveType);
				bAdjective=TRUE;
			}
			else if(IsEnglishPossessivePronoun(token))
			{
				if(enr.bIndefiniteArticle || enr.bDefiniteArticle || 
					enr.m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated ||
					enr.m_possessivePronoun ||
					enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
					enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
				{
					#ifdef _DEBUG
					printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - enr.bIndefiniteArticle || \
enr.bDefiniteArticle || enr.m_PointerToVocNodeWhereAnEnglishPronounIsEncapsulated \
|| enr.m_possessivePronoun || \
enr.m_vecPointerToVocNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || \
enr.m_PointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated");
					#endif
					return 0;
				}
				enr.m_possessivePronoun=GetIndexFromPossessivePronoun(token);
			}
			else
			{
				TRACE("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const CStringVector & sv) ENDE return 0\n");
				#ifdef _DEBUG
				printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const CStringVector & sv) ENDE return 0 nichts von allem\n");
				#endif
				return 0;
			}
			if(bComperative==2 || bSuperlative==2)
			{
#ifdef _DEBUG
				printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const \
PositionCStringVector & psv,const DWORD & dwFlags,BYTE \
bContainsFirstTokenOfSentence - return 0 - more oder most aber kein Adjektiv \
direkt dahinter\n");
#endif
				return 0;
			}
			if(bComperative>1)
				--bComperative;
			if(bSuperlative>1)
				--bSuperlative;
		}
		if(enr.bIndefiniteArticle && !(
			enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
			enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated))
		{
			EnglishNoun * en=dynamic_cast<EnglishNoun *>
				(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				m_pWordNode->m_pWord);
			if(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
				m_bSingular)
			{
				if(en->m_strSingular.GetLength()>1)
					if(IsVowel(en->m_strSingular.Left(1))) // wenn der Anfangsbuchstabe ein Vokal ist
					{
						if(strIndefiniteArticle==_T("a"))
						{
							#ifdef _DEBUG
							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Nomens\
ist ein Vokal, aber der unbestimmte Artikel ist 'a'");
							#endif
							return 0;
						}
					}
					else // der Anfangsbuchstabe ist kein Vokal
					{
						if(strIndefiniteArticle==_T("an"))
						{
							#ifdef _DEBUG
							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Nomens\
ist kein Vokal, aber der unbestimmte Artikel ist 'an'");
							#endif
							return 0;
						}
					}
			}
			else
			{
				if(en->m_strPlural.GetLength()>1)
					if(IsVowel(en->m_strPlural.Left(1))) // wenn der Anfangsbuchstabe ein Vokal ist
					{
						if(strIndefiniteArticle==_T("a"))
						{
							#ifdef _DEBUG
							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Nomens\
ist ein Vokal, aber der unbestimmte Artikel ist 'a'");
							#endif
							return 0;
						}
					}
					else // der Anfangsbuchstabe ist kein Vokal
					{
						if(strIndefiniteArticle==_T("an"))
						{
							#ifdef _DEBUG
							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Nomens\
ist kein Vokal, aber der unbestimmte Artikel ist 'an'");
							#endif
							return 0;
						}
					}
			}
		}
	}
	else
	{
		#ifdef _DEBUG
		printf("!bEnglishNoun\n");
		#endif
		WordNode * pWordNode=NULL;
		if((pWordNode=GetPointerToWordNodeWhereAnEnglishPronounIsEncapsulated(
			GetBetweenAsCStringVector(psv,0,psv.size()))))
		{
			enr.m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated=pWordNode;
			#ifdef _DEBUG
			printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & \
enr,const CStringVector & sv) ENDE return 3\n");
			#endif
			return 3; // 3 = 3. Person Singular
		}
		if(psv.size()==1)
		{
			CString token=psv.at(0).m_Str;
			if(IsPersonalPronoun(token))
			{
				personIndex=GetPersonIndex(token);
			}
		}
	}
	//TRACE("ParseEnumerationElement ENDE - return TRUE\n");
	#ifdef _DEBUG
	printf("enr.m_Noun.size(): %u\n",enr.m_Noun.size());
	TRACE("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const CStringVector & sv) ENDE\n");
	printf("personIndex: %u\n",personIndex);
	printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const CStringVector & sv) ENDE return %u\n",personIndex);
	#endif
	return personIndex;
}

//BYTE ParsePersonalPronounOrNounStruct2(EnumerationElement & ee,const 
//	PositionCStringVector & psv,const DWORD & dwFlags,BYTE 
//	bContainsFirstTokenOfSentence)
//{
//	BOOL bAdjective=FALSE;
//	BOOL bEnglishNoun=FALSE;
//	BOOL bPossessivePronoun=FALSE;
//  bool bValid=false;
//	//"the" und "a"/"an" für das selbe Substantiv schließen sich aus--
//	//nur 1 Variable für alle Artikelarten.
//	BYTE byArticle=0;
//  BYTE bAdjectiveType=0;
//  BYTE bComperative=0;
//  BYTE bSuperlative=0;
//	BYTE personIndex=0;
//	//an dieser Stelle deklarieren, damit nicht bei jedem 
//	//Schleifendurchlauf angelegt wird
//	CStringVector vecCStr;
//	DWORD dwBeginOfEnglishNoun=0;
//	//an dieser Stelle deklarieren, damit nicht bei jedem 
//	//Schleifendurchlauf angelegt wird
//	std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> 
//		ptvwaenie;
//  //TODO this must be assigned to be persistent when leaving this function.
//  std::vector<WordNode *> vecpWordNodeWhereAnEnglishAdjectiveIsEncapsulated;
//  WordNode * pWordNode;
//	TRACE("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const PositionCStringVector & psv) ANFANG\n");
//
//	#ifdef _DEBUG
//	printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const PositionCStringVector & psv) ANFANG\n");
//	printf("psv.size(): %d\n",psv.size());
//	for(int i2=0;i2<psv.size();i2++)
//	{
//		printf("psv.at(%d).m_Str: %s\n",i2,psv.at(i2).m_Str);
//	}
//	#endif
//	//enr=new EnglishNounRecord;
//	for(int i=psv.size()-1;i>=0 && g_bContinue;i--)
//	{
//		TRACE("psv.at(i).m_Str: %s\n",psv.at(i).m_Str);
//		#ifdef _DEBUG
//		printf("psv.size()-1: %d psv.at(%d).m_Str: %s\n",psv.size()-1,i,psv.at(i).m_Str);
//		#endif
//		#ifdef _DEBUG
//		//CStringVector sv2=GetBetweenAsCStringVector(psv,i,psv.size()-1);
//		for(int i2=0;i2<sv2.size();i2++)
//			printf("sv2.at(%d): %s\n",i2,sv2.at(i2));
//		#endif
//
//		if(token==_T("a"))
//		{
//			if(!(byArticle || 
//				pWordNodeWhereAnEnglishPronounIsEncapsulated ||
//				possessivePronoun ||
//				vecpWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
//				pWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
//				)
//			{
//				bValid=true;
//				byArticle=A;
//			}
//		}
//		else if(token==_T("an"))
//		{
//			if(!(byArticle || 
//				pWordNodeWhereAnEnglishPronounIsEncapsulated ||
//				possessivePronoun ||
//				vecpWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
//				pWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
//				)
//			{
//				bValid=true;
//				byArticle=AN;
//			}
//		}
//		else if(token==_T("the"))
//		{
//			if(!(byArticle || 
//				pWordNodeWhereAnEnglishPronounIsEncapsulated ||
//				possessivePronoun ||
//				vecpWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
//				pWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
//				)
//			{
//				bValid=true;
//				byArticle=THE;
//			}
//		}
//		if(token==_T("more"))
//		{
//			//vor einer Steigerung, die sich auf ein Nomen bezieht, muss
//			//sich ein Artikel befinden: a more intelligent player, 
//			//the more intelligent player
//			//NICHT: He is more intelligent player
//			if(byArticle)
//			{
//				bValid=true;
//				bComperative=3;
//			}
//		}
//		if(token==_T("most"))
//		{
//			//vor einer Steigerung, die sich auf ein Nomen bezieht, muss
//			//sich ein bestimmter Artikel befinden: the most 
//			//intelligent player, 
//			//NICHT: He is most intelligent player,
//			//He is a most intelligent player
//			if(byArticle==THE)
//			{
//				bValid=true;
//				bSuperlative=3;
//			}
//		}	
//		if((pWordNode=
//			GetPointerToWordNodeWhereAnEnglishPronounIsEncapsulated(
//			strToken)))
//		{
//			if(!(byArticle || 
//				pWordNodeWhereAnEnglishPronounIsEncapsulated ||
//				possessivePronoun ||
//				vecpWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
//				pWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
//				)
//			{
//				bValid=true;
//				#ifdef _DEBUG
//				printf("englisches Pronom gefunden\n");
//				#endif
//				pWordNodeWhereAnEnglishPronounIsEncapsulated=pWordNode;
//			}
//		}
//		else if((pWordNode=
//			GetPointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated(
//			strToken)))
//		{
//			EnglishAdverb * ea;
//			if(!(vecpWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
//				pWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
//				)
//			{
//				bValid=true;
//			}
//			ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
//			if(ea && ea->m_strWord.GetLength()>1)
//				if(
//          //TODO im englischen geht es nach der AUSSPrache(des Beginn des Nomens), 
//          //und nicht der Rechtschreibung, ob der unbestimmte Artikel "an" oder "a" ist.
//					//Wenn der Anfangsbuchstabe ein Vokal ist.
//					IsVowel(ea->m_strWord.Left(1))
//          )
//				{
//					if(//und "an" als Artikel
//						byArticle==AN || byArticle==THE)
//					{
//						bValid=true;
//					}
//				}
//				else // der Anfangsbuchstabe ist kein Vokal
//				{
//					if(byArticle==A  || byArticle==THE)
//					{
//						bValid=true;
//					}
//				}
//			pWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated=
//				pWordNode;
//		}
//		/*else if((pVocNode=GetPointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAnAdverbIsEncapsulated(token)))
//		{
//		}*/
//		else if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated(
//			strToken,dwFlags,bAdjectiveType)))//IsEnglishAdjective(token))
//		{
//			//enr.m_
//			EnglishAdjective * ea=dynamic_cast<EnglishAdjective *>
//				(pWordNode->m_pWord);
//			if(ea)
//			{
//				//wenn das Adjektiv nur als Umstandsbestimmung der Art und Weise
//				//benutzt werden darf
//				//Beispiel für ein Adjektiv, das nur als Umstandsbestimmung der Art und 
//				//Weise benutzt werden darf: "ill": "I am ill."
//				if(!(ea->m_bAllowedPlace==2))
//				{
//          vecpWordNodeWhereAnEnglishAdjectiveIsEncapsulated.push_back(pWordNode);
//					bValid=true;
//				}	
//				if(ea->m_strPositiv.GetLength()>1)
//					if(IsVowel(ea->m_strPositiv.Left(1))) // wenn der Anfangsbuchstabe ein Vokal ist
//					{
//						if(bArticle==_T("a"))
//						{
//							#ifdef _DEBUG
//							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Adjektivs\
//ist ein Vokal, aber der unbestimmte Artikel ist 'a'");
//							#endif
//							return 0;
//						}
//					}
//					else // der Anfangsbuchstabe ist kein Vokal
//					{
//						if(strIndefiniteArticle==_T("an"))
//						{
//							#ifdef _DEBUG
//							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Adjektivs\
//ist kein Vokal, aber der unbestimmte Artikel ist 'an'");
//							#endif
//							return 0;
//						}
//					}
//			}
//			if(enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
//				enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
//			{
//				if(bComperative || bSuperlative)
//				{
//					if(!ea->m_bMoreMost)
//					{
//						#ifdef _DEBUG
//						printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - more oder most aber ein \
//Adjektiv, dessen Steigerung nicht durch more/most gebildet wird\n");
//						#endif
//						return 0;
//					}
//					enr.m_Adjectives.clear();
//					enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.clear();
//					enr.m_vecAdjectiveType.clear();
//					bAdjectiveType=2;
//				}
//				else
//				{
//				#ifdef _DEBUG
//				printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - \
//enr.m_vecPointerToVocNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || \
//enr.m_PointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverb\
//IsEncapsulated und weder bComperative noch bSuperlative\n");
//				#endif
//				return 0;
//				}
//			}
//			//wenn das Adjektiv im Superlativ ist und entweder kein bestimmter Artikel
//			//oder ein unbestimmter Artikel (wenn Superlativ, dann muss vor dem 
//			//Adjektiv ein bestimmter Artikel stehen)
//			if(bSuperlative && (!enr.bDefiniteArticle || enr.bIndefiniteArticle))
//			{
//				#ifdef _DEBUG
//				printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - Superlativ aber entweder kein \
//bestimmter Artikel oder ein unbestimmter Artikel\n");
//				#endif
//				return 0;
//			}
//			if(bComperative)
//				bComperative=1;
//			if(bSuperlative)
//				bSuperlative=1;
//			enr.m_Adjectives.push_back(token);
//			enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.
//				push_back(pWordNode);
//			enr.m_vecAdjectiveType.push_back(bAdjectiveType);
//			bAdjective=TRUE;
//		}
//		else if(IsEnglishPossessivePronoun(token))
//		{
//			if(enr.bIndefiniteArticle || enr.bDefiniteArticle || 
//				enr.m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated ||
//				enr.m_possessivePronoun ||
//				enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
//				enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated)
//			{
//				#ifdef _DEBUG
//				printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - enr.bIndefiniteArticle || \
//enr.bDefiniteArticle || enr.m_PointerToVocNodeWhereAnEnglishPronounIsEncapsulated \
//|| enr.m_possessivePronoun || \
//enr.m_vecPointerToVocNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || \
//enr.m_PointerToVocNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated");
//				#endif
//				return 0;
//			}
//			enr.m_possessivePronoun=GetIndexFromPossessivePronoun(token);
//		}
//		//if(i<sv.size()-1)
//		//{
//		//ein englisches Substantiv kann ja aus mehr als einem
//		//Token bestehen, deswegen ein Vektor aus strings
//		vecCStr=GetBetweenAsCStringVector(psv,i,psv.size()-1);
//		if(!(dwFlags&CASE_SENSITIVE)==1)
//			for(DWORD dwRun=0;dwRun<vecCStr.size();dwRun++)
//				vecCStr.at(dwRun).MakeLower();
//		else if(bContainsFirstTokenOfSentence)
//			MakeFirstLetterLower(vecCStr.at(0));
//		ptvwaenie=
//			GetVecPointerToWordNodeWhereAnEnglishNounIsEncapsulated(
//			vecCStr,dwFlags);
//		if(ptvwaenie.size()>0)
//		{
//			dwBeginOfEnglishNoun=i;
//			bEnglishNoun=TRUE;
//			if(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size()>0)
//				enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.clear();
//			enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated=ptvwaenie;
//			if(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).m_bSingular)
//				personIndex=3;
//			else
//				personIndex=6;
//		}
//		#ifdef _DEBUG
//		printf("enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size(): %u\n",
//			enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.size());
//		#endif
//	}
//	if(bEnglishNoun) // nur wenn ein englisches Nomen gefunden wurde
//	{
//		BYTE bAdjectiveType=0;
//		BYTE bComperative=0;
//		BYTE bSuperlative=0;
//		#ifdef _DEBUG
//		printf("bEnglishNoun\n");
//		#endif
//		//enr->m_Noun=
//		//Append(enr.m_Noun,GetBetween(psv,dwBeginOfEnglishNoun,psv.size()));
//		CString strIndefiniteArticle;
//		WordNode * pWordNode;
//		for(i=0;i<dwBeginOfEnglishNoun && g_bContinue;i++)
//		{
//			CString token=psv.at(i).m_Str;
//			if(!(dwFlags&CASE_SENSITIVE)==1)
//				token.MakeLower();
//			else if(bContainsFirstTokenOfSentence)
//			{
//				bContainsFirstTokenOfSentence=FALSE;
//				MakeFirstLetterLower(token);
//			}
//			#ifdef _DEBUG
//			printf("token: %s\n",token);
//			#endif
//
//
//			else
//			{
//				TRACE("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const CStringVector & sv) ENDE return 0\n");
//				#ifdef _DEBUG
//				printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const CStringVector & sv) ENDE return 0 nichts von allem\n");
//				#endif
//				return 0;
//			}
//			if(bComperative==2 || bSuperlative==2)
//			{
//#ifdef _DEBUG
//				printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const \
//PositionCStringVector & psv,const DWORD & dwFlags,BYTE \
//bContainsFirstTokenOfSentence - return 0 - more oder most aber kein Adjektiv \
//direkt dahinter\n");
//#endif
//				return 0;
//			}
//			if(bComperative>1)
//				--bComperative;
//			if(bSuperlative>1)
//				--bSuperlative;
//		}
//		if(enr.bIndefiniteArticle && !(
//			enr.m_vecPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated.size()>0 || 
//			enr.m_PointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated))
//		{
//			EnglishNoun * en=dynamic_cast<EnglishNoun *>
//				(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
//				m_pWordNode->m_pWord);
//			if(enr.m_vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.at(0).
//				m_bSingular)
//			{
//				if(en->m_strSingular.GetLength()>1)
//					if(IsVowel(en->m_strSingular.Left(1))) // wenn der Anfangsbuchstabe ein Vokal ist
//					{
//						if(strIndefiniteArticle==_T("a"))
//						{
//							#ifdef _DEBUG
//							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Nomens\
//ist ein Vokal, aber der unbestimmte Artikel ist 'a'");
//							#endif
//							return 0;
//						}
//					}
//					else // der Anfangsbuchstabe ist kein Vokal
//					{
//						if(strIndefiniteArticle==_T("an"))
//						{
//							#ifdef _DEBUG
//							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Nomens\
//ist kein Vokal, aber der unbestimmte Artikel ist 'an'");
//							#endif
//							return 0;
//						}
//					}
//			}
//			else
//			{
//				if(en->m_strPlural.GetLength()>1)
//					if(IsVowel(en->m_strPlural.Left(1))) // wenn der Anfangsbuchstabe ein Vokal ist
//					{
//						if(strIndefiniteArticle==_T("a"))
//						{
//							#ifdef _DEBUG
//							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Nomens\
//ist ein Vokal, aber der unbestimmte Artikel ist 'a'");
//							#endif
//							return 0;
//						}
//					}
//					else // der Anfangsbuchstabe ist kein Vokal
//					{
//						if(strIndefiniteArticle==_T("an"))
//						{
//							#ifdef _DEBUG
//							printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,\
//const CStringVector & sv)\n - return FALSE - der Anfangsbuchstabe des Nomens\
//ist kein Vokal, aber der unbestimmte Artikel ist 'an'");
//							#endif
//							return 0;
//						}
//					}
//			}
//		}
//	}
//	else
//	{
//		#ifdef _DEBUG
//		printf("!bEnglishNoun\n");
//		#endif
//		WordNode * pWordNode=NULL;
//		if((pWordNode=GetPointerToWordNodeWhereAnEnglishPronounIsEncapsulated(
//			GetBetweenAsCStringVector(psv,0,psv.size()))))
//		{
//			enr.m_PointerToWordNodeWhereAnEnglishPronounIsEncapsulated=pWordNode;
//			#ifdef _DEBUG
//			printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & \
//enr,const CStringVector & sv) ENDE return 3\n");
//			#endif
//			return 3; // 3 = 3. Person Singular
//		}
//		if(psv.size()==1)
//		{
//			CString token=psv.at(0).m_Str;
//			if(IsPersonalPronoun(token))
//			{
//				personIndex=GetPersonIndex(token);
//			}
//		}
//	}
//	//TRACE("ParseEnumerationElement ENDE - return TRUE\n");
//	#ifdef _DEBUG
//	printf("enr.m_Noun.size(): %u\n",enr.m_Noun.size());
//	TRACE("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const CStringVector & sv) ENDE\n");
//	printf("personIndex: %u\n",personIndex);
//	printf("BYTE ParsePersonalPronounOrNounStruct(EnglishNounRecord & enr,const CStringVector & sv) ENDE return %u\n",personIndex);
//	#endif
//	return personIndex;
//}

BYTE ParsePlace(Place & place,const PositionCStringVector & psv,
	std::vector<Range> & rVecRange,const DWORD & dwFlags)
{ // ist das eine Umstandsbestimmunmg des Ortes?
	#ifdef _DEBUG
	printf("BOOL ParsePlace(Place & place,const CStringVector & sv,std::vector<Range> & rVecRange) - ANFANG\n");
	for(DWORD i2=0;i2<psv.size();i2++)
	{
		TRACE("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
		printf("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
	}
	#endif
	BYTE returnValue=2;
	BOOL bAnd=FALSE;
	BOOL bColon=FALSE;
	BYTE personIndex=0;
	BOOL bValidPreposition=FALSE;
	DWORD dwEndOfValidPreposition=0;
	WordNode * pWordNode=NULL;
	for(DWORD dwRun=0;dwRun<psv.size();dwRun++)
	{
		CStringVector vecCStr=GetBetweenAsCStringVector(psv,0,dwRun);
		//if(!(dwFlags&CASE_SENSITIVE==1))
		if(!(dwFlags&CASE_SENSITIVE))
			for(DWORD dwRun2=0;dwRun2<vecCStr.size();dwRun2++)
				vecCStr.at(dwRun2).MakeLower();
		if((pWordNode=GetPointerToWordNodeWhereAnEnglishPrepositionIsEncapsulated(vecCStr)))
		{
			place.m_pWordNodeWhereAnEnglishPrepositionIsEncapsulated=pWordNode;
			bValidPreposition=TRUE;
			dwEndOfValidPreposition=dwRun;
		}
	}
	if(bValidPreposition && dwEndOfValidPreposition+1<psv.size())
	{
		DWORD dwStart=dwEndOfValidPreposition+1;
		for(dwRun=dwEndOfValidPreposition+1;dwRun<psv.size();dwRun++)
		{
			//In MFC/Windows the TRACE macro has a variable # of args--I do 
			//not know how to enable variable numbers of args for the same macro name.
			//So I exclude it from non-Windows.
#ifdef _WINDOWS
			TRACE("psv.at(%u).m_Str: %s\n",dwRun,psv.at(dwRun).m_Str);
#endif
			if(psv.at(dwRun).m_Str==",")
			{
				if(bAnd)
				{
					#ifdef _DEBUG
					printf("BYTE ParsePlace(Place & place,const CStringVector & sv,std::vector<Range> & rVecRange) - return 0 - bAnd\n");
					#endif
					return 0;
				}
				bColon=TRUE;
				PositionCStringVector eeVec=GetBetween(psv,dwStart,dwRun-1);
				EnumerationElement ee;
				std::vector<Range> vecRange;
				BYTE ret=0;
				if(!(ret=ParsePlaceEnumerationElement(ee,eeVec,vecRange,dwFlags)))
				{
					TRACE("ParseObject ENDE - return FALSE\n");
					#ifdef _DEBUG
					printf("BOOL ParsePlace(Place & place,const CStringVector & sv) - return 0\n");
					#endif
					return 0;
				}
				if(ret==1)
					returnValue=1;
				for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
					rVecRange.push_back(vecRange.at(dwRun));
				dwStart=dwRun+1;
				place.m_vecEnumerationElement.push_back(ee);
			}
			if(psv.at(dwRun).m_Str=="and") // wenn 'and'
			{ 
				TRACE("and\n");
				if(bAnd)
				{
					TRACE("ParseObject ENDE - return FALSE\n");
					#ifdef _DEBUG
					printf("BYTE ParsePlace(Place & place,const CStringVector & sv,std::vector<Range> & rVecRange) - return 0 - bAnd\n");
					#endif
					return 0;
				}
				bAnd=TRUE;
				PositionCStringVector eeVec=GetBetween(psv,dwStart,dwRun-1);
				EnumerationElement ee;
				std::vector<Range> vecRange;
				BYTE ret=0;
				if(!(ret=ParsePlaceEnumerationElement(ee,eeVec,vecRange,dwFlags)))
				{
					TRACE("ParseObject ENDE - return FALSE\n");
					#ifdef _DEBUG
					printf("BOOL ParsePlace(Place & place,const CStringVector & sv,std::vector<Range> & rVecRange) - return 0\n");
					#endif
					return 0;
				}
				for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
					rVecRange.push_back(vecRange.at(dwRun));
				if(ret==1)
					returnValue=1;
				dwStart=dwRun+1;
				place.m_vecEnumerationElement.push_back(ee);
			}
			if((dwRun+1)==psv.size()) // wenn das Ende des Vektors erreicht wird
			{ // (letztes Vektor-Element)

				TRACE("(fdwRun+1)==sv.size()\n");
				PositionCStringVector eeVec=GetBetween(psv,dwStart,dwRun);
				EnumerationElement ee;
				std::vector<Range> vecRange;
				BYTE ret=0;
				if(!(ret=ParsePlaceEnumerationElement(ee,eeVec,vecRange,dwFlags)))
				{
					TRACE("ParseObject ENDE - return FALSE\n");
					#ifdef _DEBUG
					printf("BOOL ParsePlace(Place & place,const CStringVector & sv,std::vector<Range> & rVecRange) - return 0\n");
					#endif
					return 0;
				}
				if(ret==1)
					returnValue=1;
				for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
					rVecRange.push_back(vecRange.at(dwRun));
				place.m_vecEnumerationElement.push_back(ee);
				//TRACE("ParseObject ENDE - return TRUE\n");
				//#ifdef _DEBUG
				//printf("BOOL ParsePlace(Place & place,const CStringVector & sv,std::vector<Range> & rVecRange) - return %u\n",returnValue);
				//#endif
				//return returnValue;
			}
		}
		if(bColon && !bAnd)
		{
			TRACE("ParseObject ENDE - return FALSE\n");
			#ifdef _DEBUG
			printf("BYTE ParsePlace(Place & place,const CStringVector & sv,std::vector<Range> & rVecRange) - return 0 - !bAnd\n");
			#endif
			return 0;
		}
		TRACE("ParseObject ENDE - return TRUE\n");
		#ifdef _DEBUG
		printf("BOOL ParsePlace(Place & place,const CStringVector & sv,std::vector<Range> & rVecRange) - return %u\n",returnValue);
		#endif
		return returnValue;
	}
	#ifdef _DEBUG
	printf("BOOL ParsePlace(Place & place,const CStringVector & sv,std::vector<Range> & rVecRange) - return 0\n");
	#endif
	return 0;
}

BYTE ParsePlaceEnumerationElement(EnumerationElement & ee,
	const PositionCStringVector & psv,std::vector<Range> & rVecRange,
	const DWORD & dwFlags)
{
	#ifdef _DEBUG
	printf("BYTE ParsePlaceEnumerationElement(EnumerationElement & ee,const PositionCStringVector & psv,std::vector<Range> & rVecRange) - ANFANG\n");
	for(DWORD i2=0;i2<psv.size();i2++)
	{
		TRACE("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
		printf("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
	}
	#endif
	BYTE returnValue=2;
	BYTE personIndex=0;
	if(psv.size()==0)
	{
		#ifdef _DEBUG
		printf("BYTE ParsePlaceEnumerationElement(EnumerationElement & ee,const PositionCStringVector & psv,std::vector<Range> & rVecRange) - return FALSE -kein Token\n");
		#endif
		return FALSE;
	}
	if(psv.size()==1)
	{
		CString str=psv.at(0).m_Str;
		//if(!(dwFlags&CASE_SENSITIVE==1))
		if(!(dwFlags&CASE_SENSITIVE))
			str.MakeLower();
			if(str=="me")
			{
				ee.m_personalPronoun=1;
				return returnValue;
			}
			if(str=="you")
			{
				ee.m_personalPronoun=2;
				return returnValue;
			}
			if(str=="him")
			{
				ee.m_personalPronoun=3;
				return returnValue;
			}
			if(str=="her")
			{
				ee.m_personalPronoun=4;
				return returnValue;
			}
			if(str=="it")
			{
				ee.m_personalPronoun=5;
				return returnValue;
			}
			if(str=="us")
			{
				ee.m_personalPronoun=6;
				return returnValue;
			}
			if(str=="them")
			{
				ee.m_personalPronoun=7;
				return returnValue;
			}
	}
	EnglishNounRecord enr;
	if(ParsePersonalPronounOrNounStruct(enr,psv,dwFlags,FALSE))
	{
		ee.m_pEnr=new EnglishNounRecord;
		*ee.m_pEnr=enr;
		#ifdef _DEBUG
		printf("BYTE ParsePlaceEnumerationElement(EnumerationElement & ee,const PositionCStringVector & psv,std::vector<Range> & rVecRange) ENDE return TRUE\n");
		#endif
		return returnValue;
	}
	BOOL bValidDefiniteArticleNoun=FALSE;
	DWORD dwValidDefiniteArticleNounEnd=0;
	for(DWORD dwRun=0;dwRun<psv.size() && g_bContinue;dwRun++)
	{
		BYTE ret=0;
		EnglishNounRecord enr;
		CStringVector sv = GetBetweenAsCStringVector(psv,0,dwRun) ;
		if(ParseDefiniteArticleNoun(enr,sv,
			dwFlags,FALSE))
		{ // z.B.: The school
			if(ee.m_pEnr)
				delete ee.m_pEnr;
			ee.m_pEnr=new EnglishNounRecord;
			*ee.m_pEnr=enr;
			bValidDefiniteArticleNoun=TRUE;
			dwValidDefiniteArticleNounEnd=dwRun;
			#ifdef _DEBUG
			printf("ee.m_pEnr->m_Noun.size(): %u\n",ee.m_pEnr->m_Noun.size());
			#endif
		}
	}
	if(bValidDefiniteArticleNoun && psv.size()>dwValidDefiniteArticleNounEnd+1)
	{
		OfPhrase op;
		if(ParseOfPhrase(op,GetBetween(psv,dwValidDefiniteArticleNounEnd+1,
			psv.size()),dwFlags))
		{ // z.B.: of my father
			if(ee.m_pOfPhrase)
				delete ee.m_pOfPhrase;
			ee.m_pOfPhrase=new OfPhrase;
			*ee.m_pOfPhrase=op;
			#ifdef _DEBUG
			printf("op.m_Enr->m_Noun.size(): %u\n",op.m_Enr.m_Noun.size());
			printf("ee.m_pOfPhrase->m_Enr->m_Noun.size(): %u\n",ee.m_pOfPhrase->
				m_Enr.m_Noun.size());
			printf("personIndex: %u\n",personIndex);
			printf("BYTE ParsePlaceEnumerationElement(EnumerationElement & ee,\
const PositionCStringVector & psv,std::vector<Range> & rVecRange) ENDE \
return TRUE\n");
			#endif
			return returnValue;
		}
	}
	for(dwRun=0;dwRun<psv.size();dwRun++)
	{
		ee.m_vecStrPlainText.push_back(psv.at(dwRun).m_Str);
		//if(dwRun>0 && dwRun<psv.size()-1)
		//	ee.m_plainText+=" ";
		//ee.m_plainText+=psv.at(dwRun).m_Str;
	}
	Range range;
	range.m_dwStart=psv.at(0).m_dwStart;
	range.m_dwEnd=psv.at(psv.size()-1).m_dwStart+psv.at(psv.size()-1).m_Str.GetLength();
	rVecRange.push_back(range);
	#ifdef _DEBUG
	printf("rVecRange.size(): %u range.m_dwStart: %u range.m_dwEnd: %u\n",rVecRange.size(),range.m_dwStart,range.m_dwEnd);
	printf("BYTE ParsePlaceEnumerationElement(EnumerationElement & ee,const PositionCStringVector & psv,std::vector<Range> & rVecRange) ENDE return 1\n");
	#endif
	return 1; // nicht erkannt oder Eigenname, also einen Index von 3
}


BOOL ParsePre()//EnumerationElement & ee,const CStringVector & sv)
{

	return TRUE;
}

BOOL ParseQuestion(Question & question,PositionCStringVector & psv,
	std::vector<Range> & rVecRange,const DWORD & dwFlags)
{
	#ifdef _DEBUG
	printf("BOOL ParseQuestion(Question & question,const CStringVector & sv) - ANFANG\n");
	//CString str
	printf("psv.size: %u\n",psv.size());
	#endif
	if(psv.size()<=0)
	{
		#ifdef _DEBUG
		printf("BOOL ParseQuestion(Sentence & sentence,CStringVector sv) - return FALSE - sv.size()<=0\n");
		#endif
		return FALSE;
	}
	if(!IsFirstLetterUpper(psv.at(0).m_Str)) // ein gültiger Satz muß mit einem
	{ // Großbuchstaben beginnen
		TRACE("BOOL ParseSentence(Sentence & sentence,CStringVector sv) - ENDE - return FALSE\n");
		return FALSE;
	}
	MakeFirstLetterLower(psv.at(0).m_Str); // ersten Buchstaben des ersten Tokens klein machen
	if(psv.size()>0)
	{
		Clause subsentence;
		std::vector<Range> vecRange;
		if(ParseQuestionClause(subsentence,GetBetween(psv,0,psv.size()),vecRange,TRUE))
		{
			subsentence.m_bWordOrder=SPO;
			//subsentence.tense=question.m_tense;
			//printf("
			question.m_vecClause.push_back(subsentence);
			#ifdef _DEBUG
			printf("BOOL ParseQuestion(Question & question,const CStringVector & \
sv) - ENDE - return TRUE\n");
			#endif
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
			return TRUE;
		}
	}
	#ifdef _DEBUG
	printf("BOOL ParseQuestion(Question & question,const CStringVector & sv) - ENDE - return FALSE\n");
	#endif
	return FALSE;
}

//return: 0=Fehler, 1=korrekter Satz
BOOL ParseQuestionClause(Clause & ss,const PositionCStringVector & psv,
	std::vector<Range> & rVecRange,const DWORD & dwFlags)
{
	#ifdef _DEBUG
	printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector & sv) - ANFANG\n");
	#endif
	if(psv.size()<1)
	{
#ifdef _DEBUG
		printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector \
& sv) - return FALSE - !(psv.size()<1)\n");
#endif
		return FALSE;
	}
		#ifdef _DEBUG
		printf("psv.size()>0\n");
		#endif
		BYTE bIndexes;
		BYTE bPersonIndexVerb=0;
		BYTE bTense;
		BYTE bTo=FALSE;
		BYTE bValidAdverbWhichIsSituatedBehindTheSubject=FALSE;
		BYTE bValidMiddle=FALSE; // gültiges 'Mittelteil'?
		BYTE bMiddle=FALSE;
		BYTE bValidManner=FALSE;
		BYTE bValidPlace=FALSE;
		BYTE bValidTime=FALSE;
		BYTE bTypeOfQuestionWord;
		DWORD dwEndOfValidMiddle=0; // Ende des
		DWORD dwEndOfLastValidElement=0;
		DWORD dwStart=0;
		DWORD dwStartOfNextElement=0;
		DWORD dwEndOfObject=0;
		DWORD dwEndOfSubject=0;
		DWORD dwStartOfObject=0;
		DWORD dwStartOfSubject=0;
		WordNode * pWordNode=NULL;
		WordNode * pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=NULL;
		WordNode * pWordNodeWhereAnEnglishBendVerbIsEncapsulated=NULL;
		WordNode * pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=NULL;
		WordNode * pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=NULL;
		WordNode * pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=NULL;
		#ifdef _DEBUG
		{
			DWORD dwRun=0;
			for(;dwRun<psv.size();dwRun++)
				printf("psv.at(%u).m_Str: %s\n",dwRun,psv.at(dwRun).m_Str);
		}
		#endif
		//gucken, ob ein Fragewort
		if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(psv.at(0).m_Str)) )
		{
			#ifdef _DEBUG
			printf("Adverb\n");
			#endif
			EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
			if(ea)
				// wenn das Adverb ein Fragewort ist, das nicht nach dem Subjekt(4) fragt
				// oder ein Fragewort ist, das nach einem Subjekt fragt (6)
				if(ea->m_bType==QUESTION_WORD || ea->m_bType==ASK_FOR_SUBJECT)
				{
					bTypeOfQuestionWord=ea->m_bType;
					//#ifdef _DEBUG
					//printf("1. Wort ist Fragewort\n");
					//#endif
          LOG2N("23.05.2008 10.43.23","1tesWort=FrageWort");//
					pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=pWordNode;
					dwStartOfNextElement++;
				}
		}
		//Prüfung, ob die Vektorgröße größer als der Start des nächsten Tokens ist
		//(also ob sich der Token innerhalb des gültigen Bereichs des Vektors "psv"
		//liegt)
		//Wenn nicht geprüft wird, kann Folgendes passieren: "Where?": es liegt 
		//ein Fragewort vor, daher wird dwStartOfNextElement um 1 erhöht
		//psv hat aber nur 1 Element, daher ist dwStartOfNextElement ein ungültiger
		//Index und es folgt ein Absturz des Programmes
		if(psv.size()>dwStartOfNextElement)
		{
			//das erste oder zweite (wenn sich davor ein Fragewort befand) Wort ist
			//üblicherweise ein Hilfsverb (bei Fragen, die NICHT nach einem Subjekt
			//fragen; Beispiel: "Where are you?", "Are you intelligent?") oder ein 
			//Vollverb (bei Fragen, die nach einem Subjekt fragen; Beispiel: "Who 
			//helped you?")
			if( (pWordNode=GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
				psv.at(dwStartOfNextElement).m_Str,bIndexes,bTense)) )
			{
				#ifdef _DEBUG
				printf("(pWordNode=GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(\
sv.at(dwStartOfNextElement)) )\n");
				#endif
				//wenn das Verb ein Hilfsverb ist (zum Beispiel "to be")
				if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))
				{
				#ifdef _DEBUG
					printf("typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
				#endif
					EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
						(pWordNode->m_pWord);
					if(eav)
					{
					#ifdef _DEBUG
						printf("eav\n");
					#endif
						BOOL bValidAuxiliary=FALSE;
						//wenn ein Fragewort existiert und dieses nach einem Subjekt fragt
						//und das erste gebeugte Verb "to do" ist, dann ist der Satz 
						//grammatikalisch falsch
						if(pWordNodeWhereAnEnglishQuestionWordIsEncapsulated && 
							bTypeOfQuestionWord==ASK_FOR_SUBJECT && eav->m_strWords[0]==_T("do"))
						{
#ifdef _DEBUG
							printf("BOOL ParseQuestionClause(Clause & ss,const \
CStringVector & sv) - return 0 - \"do\" bei einer Frage nach dem Subjekt\n");
#endif
							return 0;
						}
						//gebeugte Verbformen des Präsens mit dem aktuellen Token vergleichen
						for(BYTE bRun=1;bRun<7;bRun++)
							if(psv.at(dwStartOfNextElement).m_Str==eav->m_strWords[bRun])
							{
							#ifdef _DEBUG
								printf("sv.at(dwStartOfNextElement)==eav->m_strWords[bRun]\n");
							#endif
								if(eav->m_strWords[0]!=_T("do") && eav->m_strWords[0]!=_T("will"))
									pWordNodeWhereAnEnglishBendVerbIsEncapsulated=pWordNode;
								if(eav->m_strWords[0]==_T("will"))
									ss.m_bTense=FUTURE;
								else
								{
									ss.m_bTense=SIMPLE_PRESENT;
								#ifdef _DEBUG
									printf("ss.tense=SIMPLE_PRESENT;\n");
								#endif
								}
								bValidAuxiliary=TRUE;
								dwStartOfSubject=dwStartOfNextElement+1;
								bPersonIndexVerb=bRun;
								break;
							}
						//gebeugte Verbformen des Präteritums mit dem aktuellen Token vergleichen
						for(bRun=7;bRun<13;bRun++)
							if(psv.at(dwStartOfNextElement).m_Str==eav->m_strWords[bRun])
							{
								if(eav->m_strWords[0]!=_T("do") && eav->m_strWords[0]!=_T("will"))
									pWordNodeWhereAnEnglishBendVerbIsEncapsulated=pWordNode;
								ss.m_bTense=SIMPLE_PAST;
							#ifdef _DEBUG
								printf("ss.m_bTense=SIMPLE_PAST;\n");
							#endif
								bValidAuxiliary=TRUE;
								dwStartOfSubject=dwStartOfNextElement+1;
								bPersonIndexVerb=bRun-6;
								break;
							}
#ifdef _DEBUG
							printf("bPersonIndexVerb: %u\n",bPersonIndexVerb);
#endif
						if(!bValidAuxiliary)
						{
						#ifdef _DEBUG
							printf("BOOL ParseQuestion(Sentence & sentence,CStringVector sv) - return FALSE - kein gültiges Hilfsverb\n");
						#endif
							return FALSE;
						}
					}
					dwStartOfNextElement++;
				}
				//else // vor dem Subject muss ein Hilfsverb stehen!
				//	return FALSE;
				//wenn das Verb ein Vollverb ist und das Fragewort nach einem Subjekt fragt
				//Beispiel: "Who helped you?"
				if(typeid(*pWordNode->m_pWord)==typeid(EnglishVerb) && bTypeOfQuestionWord==6)
				{
					pWordNodeWhereAnEnglishBendVerbIsEncapsulated=pWordNode;
					dwStartOfNextElement++;
				}
			}
			else // vor dem Subjekt/Objekt muss ein Verb stehen!
				return FALSE;
		}
		#ifdef _DEBUG
		if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
			printf("pWordNodeWhereAnEnglishBendVerbIsEncapsulated\n");
		else
		{
			//printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector & sv)\n");
			//return FALSE;
		}
		#endif
		dwStartOfSubject=dwStartOfNextElement;
		if(psv.size()>dwStartOfNextElement)
		{
			//wenn das Token eine Verneinung ist und kein Fragewort existiert, das nach
			//einem Subjekt fragt und das gebeugte Verb kein Vollverb ist (Fragewörter,
			//die nach einem Subjekt fragen und als gebeugte Verbform ein Vollverb haben,
			//haben bei einer Verneinung eine gebeugte
			//Verbform von "to do": "Who does not like you").
			//"Who likes not you?": grammatikalisch falsch
			if(psv.at(dwStartOfNextElement).m_Str==_T("not") && 
				!(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)
				==typeid(EnglishVerb) && bTypeOfQuestionWord==6 &&
				pWordNodeWhereAnEnglishQuestionWordIsEncapsulated))
			{
				dwStartOfSubject=++dwStartOfNextElement;
				ss.m_bNegation=TRUE;
				//printf("
			}
		}
		//nach einem Adverb suchen, das sich direkt hinter dem Subjekt befinden darf
		//Beispiel: "always" ("Do I always like you?")
		if(psv.size()>dwStartOfNextElement)
			for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
			{
				if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
					psv.at(dwRun).m_Str) ))
				{
					#ifdef _DEBUG
					printf("pVocNode=GetPointerToVocNodeWhereAnEnglishAdverbIsEnc\
apsulated(sv.at(dwRun))\n");
					#endif
					EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
					if(ea)
						if(ea->m_bType==0) // wenn Adverb, das sich hinter dem Subjekt befindet
						{ // Beispiel: "always" ("Am I always intelligent?")
							if(dwRun==dwStartOfSubject) // es befindet sich kein einziges Token
							{ // zwischen dem gebeugten Hilfsverb und dem Adverb
#ifdef _DEBUG
								printf("ParseQuesionClause() - return FALSE\n");
#endif
								return FALSE;
							}
							ss.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated=pWordNode;
							dwStartOfNextElement=dwRun+1;
							dwEndOfSubject=dwRun-1;
							dwStartOfObject=dwRun+1;
							bValidAdverbWhichIsSituatedBehindTheSubject=TRUE;
							break;
						}
				}
			}
		//wenn das gebeugte Verb vom Verb "to do" ist (also 
		//pWordNodeWhereAnEnglishBendVerbIsEncapsulated noch nicht alloziert
		//wurde und ausgeschlossen ist, das die gebeugte Verbform vom Verb "to will"
		//stammt), dann nach einem Infinitiv suchen
		//Beispielsatz: "Do you like me?" Infinitiv: "like"
		if(psv.size()>dwStartOfNextElement && 
			!pWordNodeWhereAnEnglishBendVerbIsEncapsulated && ss.m_bTense!=FUTURE)
		{
			if(bValidAdverbWhichIsSituatedBehindTheSubject)
			{
				if((pWordNode=GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(
					psv.at(dwStartOfNextElement).m_Str) ))
				{
					pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
						pWordNode;
					dwStartOfNextElement++;
					dwStartOfObject=dwStartOfNextElement;
#ifdef _DEBUG
					printf("erstes ParseSubject in ParseQuestionClause\n");
#endif
					ParseSubject(*ss.m_pSubject,GetBetween(psv,dwStartOfSubject,
						dwEndOfSubject),0,rVecRange,dwFlags,FALSE);
				}				
			}
			else
			{
				//es soll erst ab dem übernächsten Token angefangen werden, da das 
				//Subjekt aus mindestens einem Token bestehen muss
				for(DWORD dwRun=dwStartOfNextElement+1;dwRun<psv.size();dwRun++)
				{
					if((pWordNode=GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(
						psv.at(dwRun).m_Str) ))
					{
						std::vector<Range> rVecRange;
						pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
							pWordNode;
						ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
						dwStartOfNextElement=dwRun+1;
						dwStartOfObject=dwStartOfNextElement;
						if(!ss.m_pSubject)
							ss.m_pSubject=new Subject();
#ifdef _DEBUG
						printf("zweites ParseSubject in ParseQuestionClause\n");
#endif
						ParseSubject(*ss.m_pSubject,GetBetween(psv,dwStartOfSubject,
							dwRun-1),0,rVecRange,dwFlags,FALSE);
						ss.m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=
							pWordNodeWhereAnEnglishQuestionWordIsEncapsulated;
						//dwEndOfSubject=dwRun-1;
						break;
					}
				}
			}
		}
		if(ss.m_pSubject && psv.size()==dwStartOfNextElement)
		{
#ifdef _DEBUG
			printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector & sv)\
- return 1(korrekter Satz)\n");
#endif
			return 1;
		}
		if(psv.size()>dwStartOfNextElement)
		{
#ifdef _DEBUG
			printf("Suche nach einem Infinitiv\n");
#endif
			//wenn ein Adverb, das hinter dem Subjekt steht, im Satz vorkommt 
			//und kein Fragewort existiert, das nach
			//einem Subjekt fragt und das gebeugte Verb kein Vollverb ist (bei 
			//Fragewörtern, die nach einem Subjekt fragen und als gebeugte Verbform 
			//ein Vollverb haben, gibt es kein zweites Verb im Infinitiv
			//Beispiel: "Who likes always love you": grammatikalisch falsch).
			if(bValidAdverbWhichIsSituatedBehindTheSubject && 
				!(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)
				==typeid(EnglishVerb) && bTypeOfQuestionWord==6 &&
				pWordNodeWhereAnEnglishQuestionWordIsEncapsulated))
			{
				if((pWordNode=GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
					psv.at(dwStartOfNextElement).m_Str) ))
				{
			#ifdef _DEBUG
					printf("(pVocNode=GetPointerToVocNodeWhereAnEnglishVerbIsEncapsulated(\
sv.at(dwStartOfNextElement)) )\n");
			#endif
					//wenn englisches Hilfsverb (zum Beispiel: "to do")
					if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))
					{
						#ifdef _DEBUG
						printf("typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
						#endif
						EnglishAuxiliaryVerb * eav=dynamic_cast
							<EnglishAuxiliaryVerb *>(pWordNode->m_pWord);
						if(eav)
						{
							// wenn Infinitiv
							if(psv.at(dwStartOfNextElement).m_Str==eav->m_strWords[0])
							{
								#ifdef _DEBUG
								printf("sv.at(dwStartOfNextElement)===eav->m_strWords[0]\n");
								#endif
								if(!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
								{
									#ifdef _DEBUG
									printf("!pWordNodeWhereAnEnglishInfinitiveOrPastParticip\
leIsEncapsulated\n");
									#endif
									//dwStartOfNextElement++;
									//dwStartOfMiddle=dwStartOfNextElement;
									dwEndOfSubject=dwStartOfNextElement-1;
									dwStartOfObject=dwStartOfNextElement+1;
									dwStartOfNextElement++;
									pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
										pWordNode;
								}
							}
						}
					}
					if(typeid(*pWordNode->m_pWord)==typeid(EnglishVerb))
					{
						#ifdef _DEBUG
						printf("typeid(*pWordNode->m_pWord)==typeid(EnglishVerb)\n");
						#endif
						EnglishVerb * ev=dynamic_cast<EnglishVerb *>(pWordNode->m_pWord);
						if(ev)
							if(psv.at(dwStartOfNextElement).m_Str==ev->m_strInfinitive)
							{
								#ifdef _DEBUG
								printf("sv.at(dwStartOfNextElement)==ev->m_strInfinitive\n");
								#endif
								if(!pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
								{ // wenn pVocNodeWhereAnEnglishBendVerbIsEncapsulated NULL ist
									#ifdef _DEBUG
									printf("!pWordNodeWhereAnEnglishBendVerbIsEncapsulated\n");
									#endif
									//dwStartOfNextElement++;
									//dwStartOfMiddle=dwStartOfNextElement;
									dwEndOfSubject=dwStartOfNextElement-1;
									dwStartOfObject=dwStartOfNextElement+1;
									//pVocNodeWhereAnEnglishBendVerbIsEncapsulated=pVocNode;
									dwStartOfNextElement++;
									pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
										pWordNode;
								}
								else if(!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
								{
									#ifdef _DEBUG
									printf("!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
									#endif
									//dwStartOfNextElement++;
									//dwStartOfMiddle=dwStartOfNextElement;
									dwStartOfNextElement++;
									pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
										pWordNode;
								}
							}
					}
				}
			}
			//wenn KEIN Adverb, das hinter dem Subjekt steht, im Satz vorkommt 
			//und kein Fragewort existiert, das nach
			//einem Subjekt fragt und das gebeugte Verb kein Vollverb ist (bei 
			//Fragewörtern, die nach einem Subjekt fragen und als gebugte Verbform 
			//ein Vollverb haben, gibt es kein zweites Verb im Infinitiv
			//Beispiel: "Who likes love you": grammatikalisch falsch).
			/*if(!bValidAdverbWhichIsSituatedBehindTheSubject && 
				!(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)
				==typeid(EnglishVerb) && bTypeOfQuestionWord==6 &&
				pWordNodeWhereAnEnglishQuestionWordIsEncapsulated))
			{
				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
				{
					if((pWordNode=GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
						psv.at(dwRun).m_Str) ))
					{
						if(typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb))
						{
							#ifdef _DEBUG
							printf("typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
							#endif
							EnglishAuxiliaryVerb * eav=dynamic_cast
								<EnglishAuxiliaryVerb *>(pWordNode->m_pWord);
							if(eav)
							{
								if(psv.at(dwRun).m_Str==eav->m_strWords[0])
								{ // wenn Infinitiv
									//EnglishAuxiliaryVerb * eav=dynamic_cast<
									//	EnglishAuxiliaryVerb*>
									//	pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord;
									#ifdef _DEBUG
									printf("sv.at(dwRun)===eav->m_strWords[0]\n");
									#endif
									//if(eav)
									//	if(eav->m_strWords[0]==_T("have"))
									//		//"Fragewort" "Verb 'to have' [...] "be" "?": falsch
									//		//Beispiel: "Where have you be?": falsch
									//		return FALSE;
									if(!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
									{
										#ifdef _DEBUG
										printf("!pWordNodeWhereAnEnglishInfinitiveOrPastPa\
rticipleIsEncapsulated\n");
										#endif
										//dwStartOfNextElement++;
										//dwStartOfMiddle=dwRun;
										dwEndOfSubject=dwRun-1;
										dwStartOfObject=dwRun+1;
										dwStartOfNextElement=dwRun+1;
										pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
											pWordNode;
										break;
									}
								}
							}
						}
						if(typeid(*pWordNode->m_pWord)==typeid(EnglishVerb))
						{
							#ifdef _DEBUG
							printf("typeid(*pWordNode->m_pWord)==typeid(EnglishVerb)\n");
							#endif
							EnglishVerb * ev=dynamic_cast<EnglishVerb *>(pWordNode->m_pWord);
							if(ev)
								if(psv.at(dwRun).m_Str==ev->m_strInfinitive)
								{
									#ifdef _DEBUG
									printf("psv.at(dwRun)==ev->m_strInfinitive\n");
									#endif
									if(!pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
									{ // wenn pVocNodeWhereAnEnglishBendVerbIsEncapsulated NULL ist
										#ifdef _DEBUG
										printf("!pWordNodeWhereAnEnglishBendVerbIsEncapsulated\n");
										#endif
										//dwStartOfNextElement++;
										//dwStartOfMiddle=dwRun;
										dwEndOfSubject=dwRun-1;
										dwStartOfObject=dwRun+1;
										//pVocNodeWhereAnEnglishBendVerbIsEncapsulated=pVocNode;
										dwStartOfNextElement=dwRun+1;
										pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
											pWordNode;
										break;
									}
									else if(!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
									{
										#ifdef _DEBUG
										printf("!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
										#endif
										//dwStartOfNextElement++;
										//dwStartOfMiddle=dwRun;
										dwEndOfSubject=dwRun-1;
										dwStartOfObject=dwRun+1;
										pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
											pWordNode;
										dwStartOfNextElement=dwRun+1;
										break;
									}
								}
						}
					}
				}

			}*/
		}
		if(psv.size()>dwStartOfNextElement)
		{
			if(bMiddle)
			{
			#ifdef _DEBUG
				printf("bMiddle\n");
			#endif
				if(psv.at(dwStartOfNextElement).m_Str==_T("to")) // zum Beispiel:
				{ // "Do you want TO go?"
			#ifdef _DEBUG
					printf("psv.at(dwStartOfNextElement).m_Str==_T(\"to\")\n");
			#endif
					bTo=TRUE;
					dwStartOfNextElement++;
				}
			}
			#ifdef _DEBUG
			else
				printf("!bMiddle\n");
			#endif
		}
		if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
		{
			#ifdef _DEBUG
			printf("pWordNodeWhereAnEnglishBendVerbIsEncapsulated\n");
			#endif
			if(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)==
			typeid(EnglishAuxiliaryVerb))
			{
				#ifdef _DEBUG
				printf("typeid(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)==\
typeid(EnglishAuxiliaryVerb)\n");
				#endif
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
				(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord);
				if(eav)
				{
					#ifdef _DEBUG
					printf("eav\n");
					printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]); // Is it used? -> Wird es benutzt?
					#endif
				}
			}
		}
		if(psv.size()>dwStartOfNextElement && bTo) // nur, wenn ein "to" direkt davor
		{ // vorkam, kann dahinter ein Infinitiv stehen
#ifdef _DEBUG
			printf("psv.size()>dwStartOfNextElement && bTo\n");
#endif
			if((pWordNode=GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
				psv.at(dwStartOfNextElement).m_Str) ))
			{
				if(typeid(*pWordNode->m_pWord)==typeid(EnglishVerb))
				{
					EnglishVerb * ev=dynamic_cast<EnglishVerb *>(pWordNode->m_pWord);
					if(ev)
						if(psv.at(dwStartOfNextElement).m_Str==ev->m_strInfinitive)
							//if(ss.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated)
							if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
							{
								//dwStartOfNextElement++;
								dwStartOfNextElement++;
								dwStartOfObject=dwStartOfNextElement+1;
								pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=pWordNode;
							}
							//pVocNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=pVocNode;
				}
			}
		}
		//Ist das Token identisch mit einem Partizip Perfekt (zum Beispiel "been")
		//oder mit einem Partizip Progressive (zum Beispiel "having")?
		if(psv.size()>dwStartOfNextElement)
		{
			#ifdef _DEBUG
			printf("sv.size()>dwStartOfNextElement\n");
			printf("erstes Past participle\n");
			#endif
			//Wenn unter den Token ein Adverb existiert, das sich direkt hinter dem
			//Subjekt befinden darf? Beispiel: "always" ("Do I always like you?")
			if(bValidAdverbWhichIsSituatedBehindTheSubject)
			{
			#ifdef _DEBUG
			printf("bValidAdverbWhichIsSituatedBehindTheSubject\n");
			#endif
				if((pWordNode=
					GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(
					psv.at(dwStartOfNextElement).m_Str)))
				{ // wenn Partizip Perfekt Beispiel: "Have I LIKED you?"
					// oder: Am I LIKED?
					#ifdef _DEBUG
					printf("past participle\n");
					#endif
					if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
					{
						#ifdef _DEBUG
						printf("pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
						#endif
						if(typeid(
							*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
							m_pWord)==typeid(EnglishAuxiliaryVerb))
						{
							EnglishAuxiliaryVerb * eav=dynamic_cast
								<EnglishAuxiliaryVerb *>
								(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
								m_pWord);
							if(eav)
							{
								//Must it have used? -> Muss es benutzt haben?
								if(eav->m_strWords[0]==_T("have"))
								{
									ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
									ss.m_bTense=PRESENT_PERFECT;
									dwEndOfSubject=dwStartOfNextElement-1;
									dwStartOfObject=dwStartOfNextElement+1;
									dwStartOfNextElement++;
								}
								if(eav->m_strWords[0]==_T("be")) // Must it be used? -> 
								{ // Muss es benutzt werden?
									if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
										ss.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=
											pWordNodeWhereAnEnglishBendVerbIsEncapsulated;
									// pVocNodeWhereAnEnglishBendVerbIsEncapsulated: englisches
									// Hilfsverb außer 'be' oder 'have'
									ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
									//ss.tense=PRESENT_PERFECT;
									ss.m_bActiveClause=FALSE;
									dwEndOfSubject=dwStartOfNextElement-1;
									dwStartOfObject=dwStartOfNextElement+1;
									dwStartOfNextElement++;
								}
							}
						}
						else // sonst: Vollverb
						{
							EnglishVerb * ev=dynamic_cast<EnglishVerb *>
								(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
								m_pWord);
							if(ev)
							{
								if(ev->m_strInfinitive==_T("have")) // Must it have used? -> Muss es benutzt haben?
								{
									ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
									ss.m_bTense=PRESENT_PERFECT;
									dwEndOfSubject=dwStartOfNextElement-1;
									dwStartOfObject=dwStartOfNextElement+1;
									dwStartOfNextElement++;
								}
								if(ev->m_strInfinitive==_T("be")) // Must it be used? -> Muss es benutzt werden?
								{
									#ifdef _DEBUG
									printf("ev->m_strInfinitive==_T(\"be\")\n");
									#endif
									ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
									//ss.tense=PRESENT_PERFECT;
									ss.m_bActiveClause=FALSE;
									dwEndOfSubject=dwStartOfNextElement-1;
									dwStartOfObject=dwStartOfNextElement+1;
									dwStartOfNextElement++;
								}
							}
						}
					}
					else
					{
						#ifdef _DEBUG
						printf("!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
						#endif
						if(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)==
							typeid(EnglishAuxiliaryVerb))
						{
							#ifdef _DEBUG
							printf("typeid(pWordNodeWhereAnEnglishBendVerbIsEncapsulated\
->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
							#endif
							EnglishAuxiliaryVerb * eav=dynamic_cast
								<EnglishAuxiliaryVerb *>
								(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord);
							if(eav)
							{
								#ifdef _DEBUG
								printf("eav\n");
								#endif
								if(eav->m_strWords[0]==_T("be")) // Is it used? -> Wird es benutzt?
								{
									#ifdef _DEBUG
									printf("eav->m_strWords[0]==_T(\"be\")\n");
									#endif
									//ss.tense=PRESENT_PERFECT;
									ss.m_bActiveClause=FALSE;
									pWordNodeWhereAnEnglishBendVerbIsEncapsulated=NULL;
									dwEndOfSubject=dwStartOfNextElement-1;
									dwStartOfObject=dwStartOfNextElement+1;
									dwStartOfNextElement++;
									pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
										pWordNode;
								}
								if(eav->m_strWords[0]==_T("have")) // Has it used? -> Hat es benutzt?
								{ // Has it been used? -> Ist es benutzt worden?
									#ifdef _DEBUG
									printf("eav->m_strWords[0]==_T(\"have\")\n");
									#endif
									ss.m_bTense=PRESENT_PERFECT;
									//ss.bActiveClause=FALSE;
									pWordNodeWhereAnEnglishBendVerbIsEncapsulated=NULL;
									dwEndOfSubject=dwStartOfNextElement-1;
									dwStartOfObject=dwStartOfNextElement+1;
									dwStartOfNextElement++;
									pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
										pWordNode;
								}
							}
						}
					}
				}
				else if((pWordNode=
					GetPointerToWordNodeWhereAnEnglishProgressiveIsEncapsulated(
					psv.at(dwStartOfNextElement).m_Str)) &&
					!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated &&
					pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
				{ // wenn progressives Präsens; Beispiel: "Am I WALKING?"
					#ifdef _DEBUG
					printf("pWordNode=GetPointerToVocNodeWhereAnEnglishProgress\
iveIsEncapsulated(sv.at(dwStartOfNextElement)\n");
					#endif
					if(ss.m_bTense==SIMPLE_PRESENT)
						ss.m_bTense=PRESENT_PROGRESSIVE;
					if(ss.m_bTense==SIMPLE_PAST)
						ss.m_bTense=PAST_PROGRESSIVE;
					pWordNodeWhereAnEnglishBendVerbIsEncapsulated=NULL;
					dwEndOfSubject=dwStartOfNextElement-1;
					dwStartOfObject=dwStartOfNextElement+1;
					dwStartOfNextElement++;
					pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
						pWordNode;
	
				//ss.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNodeWhereAnEnglishBendVerbIsEncapsulated;
				//dwStartOfNextElement++;
				}
			}//if(bValidAdverbWhichIsSituatedBehindTheSubject)
			//Wenn unter den Token KEIN Adverb existiert, das sich direkt hinter dem
			//Subjekt befinden darf? Beispiel: "always" ("Do I always like you?")
			else
			{
				//nach einem Partizip Perfekt (zum Beispiel: "been") suchen
#ifdef _DEBUG
				printf("nach einem Partizip Perfekt (zum Beispiel: \"been\") suchen\n");
#endif
				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
				{
#ifdef _DEBUG
					printf("psv.at(dwRun).m_Str: %s\n",psv.at(dwRun).m_Str);
#endif
					if((pWordNode=
						GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(
						psv.at(dwRun).m_Str)))
					{ // wenn Partizip Perfekt Beispiel: "Have I LIKED you?"
						// oder: Am I LIKED?
#ifdef _DEBUG
						printf("past participle\n");
#endif
            //Notlösung, da Absturz als auf pWordNodeWhereAnEnglishBendVerbIsEncapsulated
            //zugegriffen wurde und es NULL war.
            if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
            {
						  EnglishAuxiliaryVerb * eav=dynamic_cast
							  <EnglishAuxiliaryVerb *>
							  (pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord);
						  if(eav)
						  {
							  //wenn das Hilfsverb nicht "to have" ist
							  if(eav->m_strWords[0]!=_T("have"))
							  {
  #ifdef _DEBUG
								  printf("BOOL ParseQuestionClause(Clause & ss,const \
  PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD & \
  dwFlags) - return 0 - Hilfsverb nicht \"to have\"\n");
  #endif
								  return 0;
							  }
              }
            }
						if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
						{
#ifdef _DEBUG
							printf("pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
#endif
							if(typeid(
								*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
								m_pWord)==typeid(EnglishAuxiliaryVerb))
							{
								EnglishAuxiliaryVerb * eav=dynamic_cast
									<EnglishAuxiliaryVerb *>
									(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
									m_pWord);
								if(eav)
								{
									if(eav->m_strWords[0]==_T("have")) // Must it have used? -> Muss es benutzt haben?
									{
										ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
										ss.m_bTense=PRESENT_PERFECT;
										dwEndOfSubject=dwRun-1;
										dwStartOfObject=dwRun+1;
										dwStartOfNextElement=dwRun+1;
									}
									if(eav->m_strWords[0]==_T("be")) // Must it be used? -> 
									{ // Muss es benutzt werden?
										if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
											ss.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=
												pWordNodeWhereAnEnglishBendVerbIsEncapsulated;
										// pVocNodeWhereAnEnglishBendVerbIsEncapsulated: englisches
										// Hilfsverb außer 'be' oder 'have'
										ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
										//ss.tense=PRESENT_PERFECT;
										ss.m_bActiveClause=FALSE;
										dwEndOfSubject=dwRun-1;
										dwStartOfObject=dwRun+1;
										dwStartOfNextElement=dwRun+1;
									}
								}
							}
							else // sonst: Vollverb
							{
								EnglishVerb * ev=dynamic_cast<EnglishVerb *>
									(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
									m_pWord);
								if(ev)
								{
									if(ev->m_strInfinitive==_T("have")) // Must it have used? -> Muss es benutzt haben?
									{
										ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
										ss.m_bTense=PRESENT_PERFECT;
										dwEndOfSubject=dwRun-1;
										dwStartOfObject=dwRun+1;
										dwStartOfNextElement=dwRun+1;
									}
									if(ev->m_strInfinitive==_T("be")) // Must it be used? -> Muss es benutzt werden?
									{
#ifdef _DEBUG
										printf("ev->m_strInfinitive==_T(\"be\")\n");
#endif
										ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
										//ss.tense=PRESENT_PERFECT;
										ss.m_bActiveClause=FALSE;
										dwEndOfSubject=dwRun-1;
										dwStartOfObject=dwRun+1;
										dwStartOfNextElement=dwRun+1;
									}
								}
							}
						}
						//!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated
						else
						{
#ifdef _DEBUG
							printf("!pWordNodeWhereAnEnglishInfinitiveOrPastPartici\
pleIsEncapsulated\n");
#endif
							if(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->
								m_pWord)==typeid(EnglishAuxiliaryVerb))
							{
#ifdef _DEBUG
								printf("typeid(pWordNodeWhereAnEnglishBendVerbIsEncaps\
ulated->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
#endif
								EnglishAuxiliaryVerb * eav=dynamic_cast
									<EnglishAuxiliaryVerb *>
									(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord);
								if(eav)
								{
#ifdef _DEBUG
									printf("eav\n");
#endif
									if(eav->m_strWords[0]==_T("be")) // Is it used? -> Wird es benutzt?
									{
#ifdef _DEBUG
										printf("eav->m_strWords[0]==_T(\"be\")\n");
#endif
										//ss.tense=PRESENT_PERFECT;
										ss.m_bActiveClause=FALSE;
										pWordNodeWhereAnEnglishBendVerbIsEncapsulated=NULL;
										dwEndOfSubject=dwRun-1;
										dwStartOfObject=dwRun+1;
										dwStartOfNextElement=dwRun+1;
										pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
											pWordNode;
									}
									if(eav->m_strWords[0]==_T("have")) // Has it used? -> Hat es benutzt?
									{ // Has it been used? -> Ist es benutzt worden?
#ifdef _DEBUG
										printf("eav->m_strWords[0]==_T(\"have\")\n");
#endif
										ss.m_bTense=PRESENT_PERFECT;
										//ss.bActiveClause=FALSE;
										pWordNodeWhereAnEnglishBendVerbIsEncapsulated=NULL;
										dwEndOfSubject=dwRun-1;
										dwStartOfObject=dwRun+1;
										dwStartOfNextElement=dwRun+1;
										pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
											pWordNode;
									}
								}
							}
						}
					}//GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(psv.at(dwRun).m_Str)<>NULL
					//GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(
					//psv.at(dwRun).m_Str)==NULL
					else if((pWordNode=
						GetPointerToWordNodeWhereAnEnglishProgressiveIsEncapsulated(
						psv.at(dwRun).m_Str)) &&
						!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated &&
						pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
					{ // wenn progressives Präsens; Beispiel: "Am I WALKING?"
#ifdef _DEBUG
						printf("pWordNode=GetPointerToVocNodeWhereAnEnglishProgress\
iveIsEncapsulated(sv.at(dwRun)\n");
#endif
						if(ss.m_bTense==SIMPLE_PRESENT)
						{
							ss.m_bTense=PRESENT_PROGRESSIVE;
							bValidMiddle=TRUE;
						}
						if(ss.m_bTense==SIMPLE_PAST)
						{
							ss.m_bTense=PAST_PROGRESSIVE;
							bValidMiddle=TRUE;
						}
						pWordNodeWhereAnEnglishBendVerbIsEncapsulated=NULL;
						dwEndOfSubject=dwRun-1;
						dwStartOfObject=dwRun+1;
						dwStartOfNextElement=dwRun+1;
						pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
						pWordNode;
#ifdef _DEBUG
						printf("pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIs\
Encapsulated=pWordNode\n");
#endif
						//ss.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNodeWhereAnEnglishBendVerbIsEncapsulated;
						//dwStartOfNextElement++;
					}
				}
			}//if(bValidAdverbWhichIsSituatedBehindTheSubject)
		}
		if(!pWordNodeWhereAnEnglishBendVerbIsEncapsulated && 
			!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
		{
#ifdef _DEBUG
			printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector & sv)\
- return FALSE - !pWordNodeWhereAnEnglishBendVerbIsEncapsulated && \
!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
#endif
			return FALSE;
		}
		#ifdef _DEBUG
		printf("ss.m_bTense: %u\n",ss.m_bTense);
		printf("psv.size(): %u dwStartOfNextElement: %u\n",psv.size(),dwStartOfNextElement);
		#endif
		if(psv.size()>dwStartOfNextElement && ss.m_bTense==PRESENT_PERFECT || 
			ss.m_bTense==PRESENT_PROGRESSIVE || ss.m_bTense==PAST_PROGRESSIVE)
		{
			#ifdef _DEBUG
			printf("sv.size()>dwStartOfNextElement && ss.m_bTense==PRESENT_PERFECT\n");
			#endif
			if(1)//bMiddle || bAdverbWhich...)
			{
				if((pWordNode=
					GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(
					psv.at(dwStartOfNextElement).m_Str)))
				{ // wenn Partizip Perfekt; Beispiel: "Have I been hitten?"
					#ifdef _DEBUG
					printf("past participle\n");
					printf("zweites Past participle\n");
					#endif
					if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
					{
						#ifdef _DEBUG
						printf("pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
						#endif
						if(typeid(
							*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
							m_pWord)==
							typeid(EnglishAuxiliaryVerb))
						{
							EnglishAuxiliaryVerb * eav=dynamic_cast
								<EnglishAuxiliaryVerb *>
								(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
								m_pWord);
							if(eav)
							{
								if(eav->m_strWords[0]==_T("be")) // Has it been used? -> 
								{ // Ist es benutzt worden?
									// pVocNodeWhereAnEnglishBendVerbIsEncapsulated: englisches
									// Hilfsverb außer 'be' oder 'have'
									//ss.tense=PRESENT_PERFECT;
									#ifdef _DEBUG
									printf("eav->m_strWords[0]==_T(\"be\")\n");
									#endif
									ss.m_bActiveClause=FALSE;
									dwStartOfObject=dwStartOfNextElement+1;
									dwStartOfNextElement++;
									pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
										pWordNode;
								}
							}
						}
					}
				}
			}
			else
			{
				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
				{
					if((pWordNode=
						GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(
						psv.at(dwRun).m_Str)))
					{ // wenn Partizip Perfekt Beispiel: "Have I LIKED you?"
						// oder: Am I LIKED?
						#ifdef _DEBUG
						printf("past participle\n");
						printf("zweites Past participle\n");
						#endif
						if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
						{
							#ifdef _DEBUG
							printf("pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
							#endif
							if(typeid(
								*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
								m_pWord)==
								typeid(EnglishAuxiliaryVerb))
							{
								EnglishAuxiliaryVerb * eav=dynamic_cast
									<EnglishAuxiliaryVerb *>
									(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
									m_pWord);
								if(eav)
								{
									if(eav->m_strWords[0]==_T("be")) // Has it been used? -> 
									{ // Ist es benutzt worden?
										// pVocNodeWhereAnEnglishBendVerbIsEncapsulated: englisches
										// Hilfsverb außer 'be' oder 'have'
										//ss.tense=PRESENT_PERFECT;
										#ifdef _DEBUG
										printf("eav->m_strWords[0]==_T(\"be\")\n");
										#endif
										ss.m_bActiveClause=FALSE;
										dwStartOfObject=dwStartOfNextElement+1;
										dwStartOfNextElement=dwRun+1;
										pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=
											pWordNode;
									}
								}
							}
						}
					}
				}
			}
		}
		if(psv.size()>dwStartOfNextElement)
		{
#ifdef _DEBUG
			printf("psv.at(dwStartOfNextElement).m_Str: %s \n",
				psv.at(dwStartOfNextElement).m_Str);
#endif
			if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
			{
#ifdef _DEBUG
				printf("pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
#endif
				if(typeid(
					*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
					m_pWord)==typeid(EnglishVerb))
				{
					EnglishVerb * ev=dynamic_cast<EnglishVerb*>
						(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
						m_pWord);
					if(ev)
						if(ev->m_strPreposition!="")
							if(ev->m_strPreposition==psv.at(dwStartOfNextElement).m_Str)
							{
#ifdef _DEBUG
								printf("ev->m_strPreposition: %s\n",ev->m_strPreposition);
#endif
								dwStartOfNextElement++;
								dwStartOfObject=dwStartOfNextElement;
							}
							else
							{
#ifdef _DEBUG
								printf("ParseQuestionClause return FALSE\n");
#endif
								return FALSE;
							}
				}
			}
		}
		BOOL bValidSubject=FALSE; //gültiges Subjekt?
		DWORD dwEndOfValidSubject=0;
		dwStart=dwStartOfNextElement;
		Subject subject;
		std::vector<Range> vecRange;
#ifdef _DEBUG
		printf("dwStartOfObject: %u\n",dwStartOfObject);
#endif
		/*	if(dwStartOfNextElement==psv.size())
			{
				ss.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNodeWhereAnEnglishBendVerbIsEncapsulated;
			}*/
#ifdef _DEBUG
		printf("vor: if(sv.size()>dwStartOfNextElement && !bValidObjectOccured)\n");
#endif
		dwEndOfObject=psv.size()-1; // falls keine Umstandsbestimmungen existieren, 
		// geht das Objekt bis zum Ende des Vektors
		GermanVerb * gv=NULL;
		if(psv.size()>dwStartOfNextElement)// && !bValidObjectOccured)
		{ // es kann nicht gleichzeitig ein Objekt und eine Umstandsbestimmnung
			// der Art und Weise geben, denn sonst wären solche Sätze möglich:
			// "Am I intelligent a car?" oder: "Am I a car intelligent?"
#ifdef _DEBUG
			printf("sv.size()>dwStartOfNextElement && !bValidObjectOccured\n");
#endif
			DWORD dwEndOfValidManner=0;
			//BOOL bValidManner=FALSE;
			BOOL bBe=FALSE;
			if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
			{
				if(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)==
					typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast
						<EnglishAuxiliaryVerb *>
						(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord);
					if(eav)
						if(eav->m_strWords[0]==_T("be"))
							bBe=TRUE;
				}
				gv=dynamic_cast<GermanVerb *>
					(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext->m_pWord);
			}
			if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
			{ // bei Perfekt; Beispiel: "Have I BEEN?"
				if(typeid(
					*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
					m_pWord)==typeid(EnglishAuxiliaryVerb))
				{
					EnglishAuxiliaryVerb * eav=dynamic_cast
						<EnglishAuxiliaryVerb *>
						(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
						m_pWord);
					if(eav)
						if(eav->m_strWords[0]==_T("be"))
							bBe=TRUE;
				}
				gv=dynamic_cast<GermanVerb *>
					(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
					m_pWordNodeNext->m_pWord);
			}
#ifdef _DEBUG
			printf("gv->m_bCase: %u\n",gv->m_bCase);
#endif
			if(gv->m_bCase==0) // wenn das Prädikat kein Objekt verlangt
			{
				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
				{
					Manner manner;
					if(ParseManner(manner,GetBetween(psv,dwStartOfNextElement,dwRun),bBe,TRUE))
					{
						//dwEndOfValidObject=dwRun;
						TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
						dwEndOfValidManner=dwRun;
						bValidManner=TRUE;
						//bValidObject=TRUE;
						if(ss.m_pManner)
						{
							TRACE("ss.m_pManner\n");
							delete ss.m_pManner;
						}
						ss.m_pManner=new Manner(manner); // Standard-Kopierkonstruktor nutzen
						dwEndOfObject=dwRun-1;
					}
					if(bValidManner)
					{
						//dwStartOfNextElement=dwEndOfValidManner+1;
						if(psv.size()==(dwStartOfNextElement=dwEndOfValidManner+1))
						{
							ss.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=pWordNode;
							ss.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=
								pWordNodeWhereAnEnglishBendVerbIsEncapsulated;
							ss.m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=
								pWordNodeWhereAnEnglishQuestionWordIsEncapsulated;
							ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=
								pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated;
							#ifdef _DEBUG
							printf("ss.m_bTense: %u\n",ss.m_bTense);
							printf("BOOL ParseQuestionClause(Clause & subsentence,\
CStringVector sv) - ENDE - return TRUE\n");
							#endif
							TRACE("BOOL ParseQuestionClause(Clause & subsentence,\
CStringVector sv) - ENDE - return TRUE\n");
							return TRUE;
						}
					}
				}
				//if(bValidManner)
				//	break;
			}
			else
			{
				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
				{
					for(DWORD dwRun2=dwRun;dwRun2<psv.size();dwRun2++)
					{
						Manner manner;
						if(ParseManner(manner,GetBetween(psv,dwRun,dwRun2),bBe,TRUE))
						{
							//dwEndOfValidObject=dwRun;
							TRACE("nach: if(ParseManner(manner,GetBetween(sv,dwEndOfValidSentenceElement+1,dwRun),TRUE))\n");
							//dwEndOfValidManner=dwRun;
							bValidManner=TRUE;
							dwStartOfNextElement=dwRun2+1;
							//bValidObject=TRUE;
							if(ss.m_pManner)
							{
								TRACE("ss.m_pManner\n");
								delete ss.m_pManner;
							}
							ss.m_pManner=new Manner(manner); // Standard-Kopierkonstruktor nutzen
							dwEndOfObject=dwRun-1;
						}
					}
					if(bValidManner)
						break;
				}
			}
		}
		if(psv.size()>dwStartOfNextElement)
		{
			//wenn eine gültige Umstandsbestimmung der Art und Weise 
			//gefunden wurde oder kein Fall
			if(bValidManner || gv->m_bCase==0)
			{
			#ifdef _DEBUG
				printf("bValidManner || gv->m_bCase==0\n");
			#endif
				DWORD dwBegin=dwStartOfNextElement;
				for(DWORD dwRun=dwBegin;dwRun<psv.size();dwRun++)
				{
					Place place;
					std::vector<Range> vecRange;
					if(ParsePlace(place,GetBetween(psv,dwBegin,dwRun),vecRange,TRUE))
					{
						if(ss.m_pPlace)
							delete ss.m_pPlace;
						ss.m_pPlace=new Place;
						*ss.m_pPlace=place;
						//bValidObject=TRUE;
						dwEndOfLastValidElement=dwRun;
						dwStartOfNextElement=dwRun+1;
						bValidPlace=TRUE;
					}
				}
			}
			//wenn nicht: "eine gültige Umstandsbestimmung der Art und Weise gefunden 
			//wurde oder kein Fall", also wenn: "keine gültige Umstandsbestimmung 
			//der Art und Weise gefunden wurde und das Prädikat mindestens ein Objekt
			//verlangt"
			else
			{
			#ifdef _DEBUG
				printf("!(bValidManner || gv->m_bCase==0)\n");
			#endif
				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
				{
					#ifdef _DEBUG
					printf("in for-Schleife\n");
					#endif
					for(DWORD dwRun2=dwRun;dwRun2<psv.size();dwRun2++)
					{
						Place place;
						if(ParsePlace(place,GetBetween(psv,dwRun,dwRun2),vecRange,TRUE))
						{
							if(ss.m_pPlace)
								delete ss.m_pPlace;
							ss.m_pPlace=new Place;
							*ss.m_pPlace=place;
							//bValidObject=TRUE;
							dwEndOfLastValidElement=dwRun;
							dwStartOfNextElement=dwRun2+1;
							bValidPlace=TRUE;
							dwEndOfObject=dwRun-1;
						}
					}
					if(bValidPlace)
						break;
				}
			}
		}
		if(bValidPlace)
		{
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
			{
				rVecRange.push_back(vecRange.at(dwRun));
			}
		}
		if(psv.size()>dwStartOfNextElement)
		{
			//wenn Umstandsbestimmung der Art und Weise oder Umstandsbestimmung
			//des Ortes oder das Prädikat kein Objekt verlangt
			if(bValidManner || bValidPlace || gv->m_bCase==0)
			{
				if((pWordNode=
					GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
					psv.at(dwStartOfNextElement).m_Str)) )
				{
					#ifdef _DEBUG
					printf("Adverb\n");
					#endif
					EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
					if(ea)
						if(ea->m_bType==5) // Adverb der Zeit ('today')
						{
							#ifdef _DEBUG
							printf("Adverb der Zeit: %s\n",psv.at(dwStartOfNextElement).m_Str);
							#endif
							dwStartOfNextElement++;
							pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=pWordNode;
							bValidTime=TRUE;
						}
				}
			}
			//wenn nicht: "Umstandsbestimmung der Art und Weise oder Umstandsbestimmung
			//des Ortes oder das Prädikat kein Objekt verlangt", also wenn:
			//"keine Umstandsbestimmung der Art und Weise und keine Umstandsbestimmung
			//des Ortes und das Prädikat ein Objekt verlangt"
			else
			{
				for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
				{
					if((pWordNode=
						GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
						psv.at(dwRun).m_Str)) )
					{
						#ifdef _DEBUG
						printf("Adverb\n");
						#endif
						EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
						if(ea)
							if(ea->m_bType==5) // Adverb der Zeit ('today')
							{
								#ifdef _DEBUG
								printf("Adverb der Zeit: %s\n",psv.at(dwRun).m_Str);
								#endif
								pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=pWordNode;
								dwEndOfObject=dwRun-1;
								dwStartOfNextElement=dwRun+1;
								bValidTime=TRUE;
								break;
							}
					}
				}
			}
		}
		//wenn noch kein Subjekt existiert
		//if(dwStartOfObject!=0 && !ss.m_pSubject)
		if(!ss.m_pSubject)
		{
#ifdef _DEBUG
			printf("dwStartOfObject!=0\n");
#endif
			//erstes ParseSubject in ParseQuestionClause
#ifdef _DEBUG
			printf("drittes ParseSubject in ParseQuestionClause\n");
#endif
			if(dwEndOfSubject==0)
				dwEndOfSubject=psv.size()-1;
			if(ParseSubject(subject,GetBetween(psv,dwStartOfSubject,
				dwEndOfSubject),FALSE,vecRange,TRUE,FALSE))
			{
				if(dwStartOfNextElement<dwEndOfSubject+1)
					dwStartOfNextElement=dwEndOfSubject+1;
				//wenn bIndexes um "subject.m_personIndex" Anzahl Bits nach rechts
				//verschoben und bitweise-und mit 1 keine 1 ergibt
				//(bIndexes setzt für jede gebeugte Verbform, die mit dem Verb 
				//übereinstimmt ein Bit
				if(!((bIndexes>>subject.m_personIndex)&1))
				{
#ifdef _DEBUG
					printf("subject.m_personIndex: %u\n",subject.m_personIndex);
					printf("BOOL ParseQuestionClause(Clause & ss,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
#endif
					return 0;
				}
				if(ss.m_pSubject)
					delete ss.m_pSubject;
				ss.m_pSubject=new Subject(subject);
#ifdef _DEBUG
#endif
				bValidSubject=TRUE;
				//dwEndOfValidSubject=dwRun;
				//dwStartOfNextElement=dwRun+1;
				for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
					rVecRange.push_back(vecRange.at(dwRun));
				if(dwStartOfNextElement==psv.size())
				{
					if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
					ss.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=
						pWordNodeWhereAnEnglishBendVerbIsEncapsulated;
						ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
					//ss.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated=pVocNodeWhereAnEnglishBendVerbIsEncapsulated;
					ss.m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=
						pWordNodeWhereAnEnglishQuestionWordIsEncapsulated;
					ss.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=
						pWordNodeWhereAnEnglishInfinitiveIsEncapsulated;
					#ifdef _DEBUG
					printf("ss.m_bTense: %u\n",ss.m_bTense);
					printf("BOOL ParseQuestionClause(Clause & ss,const \
CStringVector & sv) - return TRUE\n");
					#endif
					return TRUE;
				}
			}
			else
				return FALSE;
		}
			GermanVerb * gvPredicate=NULL;
			EnglishAuxiliaryVerb * eavPredicate=NULL;
			if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
			{
				if(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->
					m_pWord)==typeid(EnglishAuxiliaryVerb))
					eavPredicate=dynamic_cast<EnglishAuxiliaryVerb*>
					(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord);
				if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext)
					gvPredicate=dynamic_cast<GermanVerb*>
					(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext->
					m_pWord);
			}
			if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
			{
				if(typeid(
					*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
					m_pWord)==typeid(EnglishAuxiliaryVerb))
					eavPredicate=dynamic_cast<EnglishAuxiliaryVerb*>
					(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
					m_pWord);
				if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
					m_pWordNodeNext)
					gvPredicate=dynamic_cast<GermanVerb*>
					(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
						m_pWordNodeNext->m_pWord);
			}
#ifdef _DEBUG
		printf("bValidMiddle: %u bValidManner %u eavPredicate: %u \
gvPredicate: %u dwStartOfObject: %u\n",
			bValidMiddle,bValidManner,eavPredicate,gvPredicate,dwStartOfObject);
#endif
		//wenn kein "Mittelteil" (Teil des Prädikates nach dem Subjekt)
		//z.B. : "Have I used a car?" "Mittelteil": "used" Subjekt: "I" und 
		//keine Umstandsbestimmung der Art und Weise und eavPredicate ungleich
		//NULL ist
		if(gvPredicate && dwStartOfObject>0)
		{
			BOOL bValidObject=FALSE;
			DWORD dwEndOfValidSentenceElement=0;
			if(ss.m_bActiveClause)
			{
#ifdef _DEBUG
				printf("gv: %u\n",gv);
#endif
				if(gvPredicate->m_bCase==1 || gvPredicate->m_bCase==2 || 
					gvPredicate->m_bCase==4)
				{ // wenn das Prädikat den 1. , 3. oder 4. Fall verlangt
					// gv->Case==1  : 3. Fall
					// gv->Case==2  : 4. Fall
					// gv->Case==4  : 1. Fall
					#ifdef _DEBUG
					printf("gvPredicate->m_bCase==1 || gvPredicate->m_bCase==2 || \
gvPredicate->m_bCase==3\n");
					#endif
					std::vector<Range> vecRange;
					Object object;
#ifdef _DEBUG
					printf("dwStartOfObject: %u dwEndOfObject: %u\n",dwStartOfObject,
						dwEndOfObject);
#endif
					if(ParseObject(object,GetBetween(psv,dwStartOfObject,dwEndOfObject),
						TRUE,vecRange,TRUE)==0)
					{
					#ifdef _DEBUG
						printf("BOOL ParseQuestionClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange) - return FALSE\n");
					#endif
						return 0;
					}
					if(ss.m_pObjectFirst)
						delete ss.m_pObjectFirst;
					ss.m_pObjectFirst=new Object(object);
					for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
						rVecRange.push_back(vecRange.at(dwRun));
					if(!bValidManner && !bValidPlace && !bValidTime)
						dwStartOfNextElement=dwEndOfObject+1;
				}
				if(gvPredicate->m_bCase==3)
				{ // wenn das Prädikat 2 Fälle (3. und 4. Fall) verlangt
					std::vector<Range> vecRange;
					std::vector<Range> vecRangeOfValidObject;
					DWORD dwEndOfValidObject=0;
					for(DWORD dwRun=dwStartOfObject;dwRun<dwEndOfObject+1;dwRun++)
					{
						vecRange.clear();
						Object object;
						if(ParseObject(object,GetBetween(psv,dwStartOfObject,dwRun),
							TRUE,vecRange,TRUE)==2)
						{
							dwEndOfValidObject=dwRun;
							//dwEndOfValidSentenceElement=dwRun;
							bValidObject=TRUE;
							if(ss.m_pObjectFirst)
								delete ss.m_pObjectFirst;
							ss.m_pObjectFirst=new Object(object);
							vecRangeOfValidObject.clear();
							vecRangeOfValidObject=vecRange;
						}
					}
					if(bValidObject)
					{
						dwStartOfNextElement=dwEndOfValidObject+1;
						dwEndOfValidSentenceElement=dwEndOfValidObject;
						for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
							rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
					}
				}
			}
			if(ss.m_bActiveClause && gvPredicate->m_bCase==3)
			{ // wenn ein Verb, das 2 Fälle verlangt
				#ifdef _DEBUG
				printf("gvPredicate->Case==3\n");
				#endif
				DWORD dwEndOfValidObject=0;
				if(bValidObject)
				{
					std::vector<Range> vecRange;
					Object object;
					ParseObject(object,GetBetween(psv,dwStartOfNextElement,
						dwEndOfObject),TRUE,vecRange,TRUE);
					bValidObject=TRUE;
					if(ss.m_pObjectSecond)
						delete ss.m_pObjectSecond;
					ss.m_pObjectSecond=new Object(object);
					for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
						rVecRange.push_back(vecRange.at(dwRun));
					if(!bValidManner && !bValidPlace && !bValidTime)
						dwStartOfNextElement=dwEndOfObject+1;
				}
				else
				{
					std::vector<Range> vecRange;
					std::vector<Range> vecRangeOfValidObject;
					DWORD dwStartOfValidObject=0;
					for(DWORD dwRun=dwEndOfObject;dwRun>dwStartOfObject-1;dwRun--)
					{
						vecRange.clear();
						Object object;
						if(ParseObject(object,GetBetween(psv,dwRun,dwEndOfObject),
							TRUE,vecRange,TRUE)==2)
						{
							dwStartOfValidObject=dwRun+1;
							//dwEndOfValidSentenceElement=dwRun;
							bValidObject=TRUE;
							if(ss.m_pObjectSecond)
								delete ss.m_pObjectSecond;
							ss.m_pObjectSecond=new Object;
							*ss.m_pObjectSecond=object;
							//subsentence.m_pObject=object;
							vecRangeOfValidObject.clear();
							vecRangeOfValidObject=vecRange;
						}
					}
					for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
						rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
					Object object;
					if(bValidObject)
					{
						vecRange.clear();
						ParseObject(object,GetBetween(psv,dwStartOfObject,
							dwStartOfValidObject-1),TRUE,vecRange,TRUE);
						if(ss.m_pObjectFirst)
							delete ss.m_pObjectFirst;
						ss.m_pObjectFirst=new Object(object);
						for(dwRun=0;dwRun<vecRange.size();dwRun++)
							rVecRange.push_back(vecRange.at(dwRun));
						if(!bValidManner && !bValidPlace && !bValidTime)
							dwStartOfNextElement=dwEndOfObject+1;
					}
				}
			}
			if(!ss.m_bActiveClause && gvPredicate->m_bCase==3) // wenn Passivsatz
			{ // und 2 Objekte pro Verb
				#ifdef _DEBUG
				printf("!subsentence.m_bActiveClause && gvPredicate->m_bCase==3\n");
				#endif
				Subject subject;
				std::vector<Range> vecRange;
				//fünftes ParseSubject in ParseQuestionClause
#ifdef _DEBUG
				printf("viertes ParseSubject in ParseQuestionClause\n");
#endif
				if(ParseSubject(subject,GetBetween(psv,dwEndOfValidMiddle+1,
					dwEndOfObject),FALSE,vecRange,TRUE,FALSE))
				{
					if(subject.m_personIndex!=bPersonIndexVerb)
					{
#ifdef _DEBUG
						printf("BOOL ParseQuestionClause(Clause & ss,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
#endif
						return 0;
					}
					if(ss.m_pPersonalSubject)
						delete ss.m_pPersonalSubject;
					ss.m_pPersonalSubject=new Subject;
					*ss.m_pPersonalSubject=subject;
					for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
						rVecRange.push_back(vecRange.at(dwRun));
					if(!bValidManner && !bValidPlace && !bValidTime)
						dwStartOfNextElement=dwEndOfObject+1;
				}
			}
		}
		if(!bValidMiddle && !bValidManner && eavPredicate)
		{
			
			if(eavPredicate->m_strWords[0]==_T("be"))// || eavPredicate->
			//	m_strWords[0]==_T("have"))
			{
				DWORD dwStartOfObjectWithoutPersonalNames=0;
				BOOL bValidObjectWithoutPersonalNames=FALSE;
#ifdef _DEBUG
				printf("dwStartOfSubject: %u\n",dwStartOfSubject);
#endif
				//std::vector<Range> vecRangeOfObjectWithoutPersonalNames;
				for(DWORD dwRun=psv.size()-1;dwRun>dwStartOfSubject;dwRun--)
				{
					Object object;
					std::vector<Range> vecRange;
					if(ParseObject(object,GetBetween(psv,dwRun,psv.size()-1),TRUE,
						vecRange,TRUE)==2)
					{ // wenn das Objekt keine(n) Eigennamen enthält
						// zum Beispiel: "a man" aus dem Fragesatz: "Am I a man?"
						if(ss.m_pObjectFirst)
							delete ss.m_pObjectFirst;
						ss.m_pObjectFirst=new Object(object);
						dwStartOfObjectWithoutPersonalNames=dwRun;
						bValidObjectWithoutPersonalNames=TRUE;
						//vecRangeOfObjectWithoutPersonalNames=vecRange;
					}
				}
				if(bValidObjectWithoutPersonalNames)
				{
					//for(dwRun=0;dwRun<vecRangeOfObjectWithoutPersonalNames.size();dwRun++)
					//	rVecRange.push_back(vecRangeOfObjectWithoutPersonalNames.at(dwRun));
#ifdef _DEBUG
					printf("bValidObjectWithoutPersonalNames\n");
					printf("dwStartOfObjectWithoutPersonalNames: %u rVecRange.size(): %u\n",
						dwStartOfObjectWithoutPersonalNames,rVecRange.size());
					printf("1. Prüfung auf Subjekt in ParseQuestionClause\n");
#endif
					Subject subject;
					std::vector<Range> vecRange;
					//zweites ParseSubject in ParseQuestionClause
#ifdef _DEBUG
					printf("fünftes ParseSubject in ParseQuestionClause\n");
#endif
					if(ParseSubject(subject,GetBetween(psv,dwStartOfSubject,
						dwStartOfObjectWithoutPersonalNames-1),FALSE,vecRange,TRUE,FALSE))
					{ // vor dem Objekt kann sich natürlich noch ein Subjekt befinden
						// Es kann auch einen oder mehrere Eigennamen beinhalten, Hauptsache es
						// ist gültig. Beispiel mit Eigennamen: "Is Stefan a man?"
						if(subject.m_personIndex!=bPersonIndexVerb)
						{
#ifdef _DEBUG
							printf("BOOL ParseQuestionClause(Clause & ss,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
#endif
							return 0;
						}
						if(ss.m_pSubject)
							delete ss.m_pSubject;
						ss.m_pSubject=new Subject(subject); //Standardkopierkonstruktur nutzen
						for(dwRun=0;dwRun<vecRange.size();dwRun++)
							rVecRange.push_back(vecRange.at(dwRun));
						dwStartOfNextElement=psv.size();
#ifdef _DEBUG
						printf("Subjekt ist gültig\n");
#endif
					}
				}
			}
		}
		//wenn ein "Mittelteil" (Teil des Prädikates nach dem Subjekt)
		//z.B. : "Have I used a car?" "Mittelteil": "used" Subjekt: "I" und/oder
		//eine Umstandsbestimmung der Art und Weise und/oder eavPredicate gleich
		//NULL ist
		else
		{
			GermanVerb * gvPredicate=NULL;
			if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
				if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext)
					gvPredicate=dynamic_cast<GermanVerb*>
					(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWordNodeNext->
						m_pWord);
			if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
				if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
					m_pWordNodeNext)
					gvPredicate=dynamic_cast<GermanVerb*>
						(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
						m_pWordNodeNext->m_pWord);
#ifdef _DEBUG
			printf("gvPredicate: %u ss.m_pSubject: %u\n",gvPredicate,ss.m_pSubject);
#endif
			if(gvPredicate && !ss.m_pSubject)
			{
#ifdef _DEBUG
				printf("gvPredicate\n");
#endif
				if(gvPredicate->m_bCase==4)
				{
					//drittes ParseSubject in ParseQuestionClause
#ifdef _DEBUG
					printf("sechstes ParseSubject in ParseQuestionClause\n");
#endif
					if(ParseSubject(subject,GetBetween(psv,dwStartOfSubject,
						dwEndOfObject),FALSE,vecRange,TRUE,FALSE))
					{
						if(subject.m_personIndex!=bPersonIndexVerb)
						{
#ifdef _DEBUG
							printf("BOOL ParseQuestionClause(Clause & ss,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
#endif
							return 0;
						}
						*ss.m_pSubject=subject;
						bValidSubject=TRUE;
						for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
							rVecRange.push_back(vecRange.at(dwRun));
						//dwEndOfValidSubject=dwRun;
						//dwStartOfNextElement=dwRun+1;
					}
				}
				if(gvPredicate->m_bCase==1 || gvPredicate->m_bCase==2 || 
					gvPredicate->m_bCase==3)
				{
					//viertes ParseSubject in ParseQuestionClause
#ifdef _DEBUG
					printf("siebtes ParseSubject in ParseQuestionClause\n");
#endif
					if(ParseSubject(subject,GetBetween(psv,dwStartOfSubject,
						dwEndOfSubject),FALSE,vecRange,TRUE,FALSE))
					{
						if(subject.m_personIndex!=bPersonIndexVerb)
						{
#ifdef _DEBUG
							printf("BOOL ParseQuestionClause(Clause & ss,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
#endif
							return 0;
						}
						*ss.m_pSubject=subject;
						bValidSubject=TRUE;
						for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
							rVecRange.push_back(vecRange.at(dwRun));
						//dwEndOfValidSubject=dwRun;
						//dwStartOfNextElement=dwRun+1;
						BOOL bValidObjectOccured=FALSE;
						BOOL bValidObject=FALSE;
						DWORD dwEndOfValidSentenceElement=0;
						if(ss.m_bActiveClause)
						{
							if(gv->m_bCase==1 || gv->m_bCase==2 || gv->m_bCase==4)
							{ // wenn das Prädikat den 1. , 3. oder 4. Fall verlangt
								// gv->Case==1  : 3. Fall
								// gv->Case==2  : 4. Fall
								// gv->Case==4  : 1. Fall
#ifdef _DEBUG
								printf("gv->m_bCase==1 || gv->m_bCase==2 || gv->m_bCase==3\n");
#endif
								std::vector<Range> vecRange;
								Object object;
								if(ParseObject(object,GetBetween(psv,dwStartOfObject,dwEndOfObject),
									TRUE,vecRange,TRUE)==0)
								{
#ifdef _DEBUG
									printf("BOOL ParseQuestionClause(Clause & subsentence,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange) - return FALSE\n");
#endif
									return 0;
								}
								if(ss.m_pObjectFirst)
									delete ss.m_pObjectFirst;
								ss.m_pObjectFirst=new Object(object);
								for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
									rVecRange.push_back(vecRange.at(dwRun));
								if(!bValidManner && !bValidPlace && !bValidTime)
									dwStartOfNextElement=dwEndOfObject+1;
							}
							if(gv->m_bCase==3)
							{ // wenn das Prädikat 2 Fälle (3. und 4. Fall) verlangt
								std::vector<Range> vecRange;
								std::vector<Range> vecRangeOfValidObject;
								DWORD dwEndOfValidObject=0;
								for(DWORD dwRun=dwStartOfObject;dwRun<dwEndOfObject+1;dwRun++)
								{
									vecRange.clear();
									Object object;
									if(ParseObject(object,GetBetween(psv,dwStartOfObject,dwRun),
										TRUE,vecRange,TRUE)==2)
									{
										dwEndOfValidObject=dwRun;
										//dwEndOfValidSentenceElement=dwRun;
										bValidObject=TRUE;
										if(ss.m_pObjectFirst)
											delete ss.m_pObjectFirst;
										ss.m_pObjectFirst=new Object(object);
										vecRangeOfValidObject.clear();
										vecRangeOfValidObject=vecRange;
									}
								}
								if(bValidObject)
								{
									dwStartOfNextElement=dwEndOfValidObject+1;
									dwEndOfValidSentenceElement=dwEndOfValidObject;
									for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
										rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
								}
							}
						}
						if(ss.m_bActiveClause && gv->m_bCase==3)
						{ // wenn ein Verb, das 2 Fälle verlangt
#ifdef _DEBUG
							printf("gv->Case==3\n");
#endif
							DWORD dwEndOfValidObject=0;
							if(bValidObject)
							{
								std::vector<Range> vecRange;
								Object object;
								ParseObject(object,GetBetween(psv,dwStartOfNextElement,
									dwEndOfObject),TRUE,vecRange,TRUE);
								bValidObject=TRUE;
								if(ss.m_pObjectSecond)
									delete ss.m_pObjectSecond;
								ss.m_pObjectSecond=new Object(object);
								for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
									rVecRange.push_back(vecRange.at(dwRun));
								if(!bValidManner && !bValidPlace && !bValidTime)
									dwStartOfNextElement=dwEndOfObject+1;
							}
							else
							{
								std::vector<Range> vecRange;
								std::vector<Range> vecRangeOfValidObject;
								DWORD dwStartOfValidObject=0;
								for(DWORD dwRun=dwEndOfObject;dwRun>dwStartOfObject-1;dwRun--)
								{
									vecRange.clear();
									Object object;
									if(ParseObject(object,GetBetween(psv,dwRun,dwEndOfObject),
										TRUE,vecRange,TRUE)==2)
									{
										dwStartOfValidObject=dwRun+1;
										//dwEndOfValidSentenceElement=dwRun;
										bValidObject=TRUE;
										if(ss.m_pObjectSecond)
											delete ss.m_pObjectSecond;
										ss.m_pObjectSecond=new Object;
										*ss.m_pObjectSecond=object;
										//subsentence.m_pObject=object;
										vecRangeOfValidObject.clear();
										vecRangeOfValidObject=vecRange;
									}
								}
								for(dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
									rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
								Object object;
								if(bValidObject)
								{
									vecRange.clear();
									ParseObject(object,GetBetween(psv,dwStartOfObject,
										dwStartOfValidObject-1),TRUE,vecRange,TRUE);
									if(ss.m_pObjectFirst)
										delete ss.m_pObjectFirst;
									ss.m_pObjectFirst=new Object(object);
									for(dwRun=0;dwRun<vecRange.size();dwRun++)
										rVecRange.push_back(vecRange.at(dwRun));
									if(!bValidManner && !bValidPlace && !bValidTime)
										dwStartOfNextElement=dwEndOfObject+1;
								}
							}
						}
						if(!ss.m_bActiveClause && gv->m_bCase==3) // wenn Passivsatz
						{ // und 2 Objekte pro Verb
#ifdef _DEBUG
							printf("!subsentence.m_bActiveClause && gv->m_bCase==3\n");
#endif
							Subject subject;
							std::vector<Range> vecRange;
							//fünftes ParseSubject in ParseQuestionClause
#ifdef _DEBUG
							printf("achtes ParseSubject in ParseQuestionClause\n");
#endif
							if(ParseSubject(subject,GetBetween(psv,dwEndOfValidMiddle+1,
								dwEndOfObject),FALSE,vecRange,TRUE,FALSE))
							{
								if(subject.m_personIndex!=bPersonIndexVerb)
								{
#ifdef _DEBUG
									printf("BOOL ParseQuestionClause(Clause & ss,const \
PositionCStringVector & psv,std::vector<Range> & rVecRange,const DWORD \
& dwFlags) - return 0 - subject.m_personIndex!=bPersonIndexVerb\n");
#endif
									return 0;
								}
								if(ss.m_pPersonalSubject)
									delete ss.m_pPersonalSubject;
								ss.m_pPersonalSubject=new Subject;
								*ss.m_pPersonalSubject=subject;
								for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
									rVecRange.push_back(vecRange.at(dwRun));
								if(!bValidManner && !bValidPlace && !bValidTime)
									dwStartOfNextElement=dwEndOfObject+1;
							}
						}
					}
				}
			}
		}
#ifdef _DEBUG
			printf("dwStartOfNextElement: %u psv.size(): %u\n",
				dwStartOfNextElement,psv.size());
#endif
			if(dwStartOfNextElement==psv.size())
			{
				if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
					ss.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=
						pWordNodeWhereAnEnglishBendVerbIsEncapsulated;
				if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
					ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=
						pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated;
				ss.m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=
					pWordNodeWhereAnEnglishQuestionWordIsEncapsulated;
				ss.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=
					pWordNodeWhereAnEnglishInfinitiveIsEncapsulated;
				ss.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=
					pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated;
#ifdef _DEBUG
				printf("ss.m_bTense: %u\n",ss.m_bTense);
				printf("BOOL ParseQuestionClause(Clause & subsentence,\
CStringVector sv,std::vector<Range> & rVecRange) - return TRUE\n");
#endif
				return TRUE;
			}
	//}
	#ifdef _DEBUG
	printf("ss.m_bTense: %u\n",ss.m_bTense);
	printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector \
& sv) - ENDE - return FALSE\n");
	#endif
	return FALSE;
}

//Kann auch Bestandteil eines Nicht-Fragesatzes sein:
//He asked me who had seen you. 
//            --Fragesatz-----
//"who had seen you":dependant (indirect) question:
// see lecture "35G" ENGLISCH im TELEKOLLEG I Band 3.
// But 1 thing is impossible/different: "do" / "does" fällt weg.
BYTE Clause::ParseQuestionClause2(const PositionCStringVector & psv,
	std::vector<Range> & rVecRange,const DWORD & dwFlags)
{
  bool bValid=false;
	//BYTE bIndexes;
	BYTE bPersonIndexVerb=0;
	//BYTE bTense;
	BYTE bTo=FALSE;
	BYTE bValidAdverbWhichIsSituatedBehindTheSubject=FALSE;
	BYTE bValidMiddle=FALSE; // gültiges 'Mittelteil'?
	BYTE bMiddle=FALSE;
	BYTE bValidManner=FALSE;
	BYTE bValidPlace=FALSE;
	BYTE bValidTime=FALSE;
	//zuerst auf "0" setzen, damit man später weiß, ob ein Wert 
	//zugewiesen wurde
	BYTE bTypeOfQuestionWord=0;
	DWORD dwEndOfValidMiddle=0; // Ende des
	DWORD dwEndOfLastValidElement=0;
	DWORD dwStart=0;
	DWORD dwStartOfNextElement=0;
	DWORD dwEndOfObject=0;
	DWORD dwEndOfSubject=0;
	DWORD dwStartOfObject=0;
	//wenn 0 nach dem Überprüfen des Prädikats, verlangt der
	//Teilsatz kein Subjekt ->also Frage nach dem Subjekt 
	//("Who likes you?")
	DWORD dwStartOfSubject=0;
	DWORD dwNumberOfTokens;
	WordNode * pWordNode=NULL;
	WordNode * pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=NULL;
	WordNode * pWordNodeWhereAnEnglishBendVerbIsEncapsulated=NULL;
	WordNode * pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=NULL;
	WordNode * pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=NULL;
	WordNode * pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=NULL;
	#ifdef _DEBUG
	printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector & sv) - ANFANG\n");
	#endif
	if(psv.size()<1)
	{
#ifdef _DEBUG
		printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector \
& sv) - return FALSE - !(psv.size()<1)\n");
#endif
		return FALSE;
	}
	#ifdef _DEBUG
	printf("psv.size()>0\n");
	#endif
	#ifdef _DEBUG
	{
		DWORD dwRun=0;
		for(;dwRun<psv.size();dwRun++)
			printf("psv.at(%u).m_Str: %s\n",dwRun,psv.at(dwRun).m_Str);
	}
	#endif
	if(psv.size()>0)
	{
		bool bFinitePartOfPredicateFound=false;
		typedef PositionCStringVector::const_iterator ItPSV;
		ItPSV end = psv.end();
		//gucken, ob ein Fragewort
    //Ein Fragewort ist für einen Fragesatz nicht unbedingt nötig, wie z.B. bei: 
    //"Have you been there?"
		if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
			psv.at(0).m_Str)) )
		{
			#ifdef _DEBUG
			printf("Adverb\n");
			#endif
			EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
			if(ea)
				// wenn das Adverb ein Fragewort ist, das nicht nach dem Subjekt(4) fragt
				// oder ein Fragewort ist, das nach einem Subjekt fragt (6)
				if(ea->m_bType==QUESTION_WORD || ea->m_bType==ASK_FOR_SUBJECT)
				{
					bTypeOfQuestionWord=ea->m_bType;
					#ifdef _DEBUG
					printf("Fragewort\n");
					#endif
					pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=pWordNode;
					dwStartOfNextElement++;
				}
		}
		dwNumberOfTokens=psv.size();
		for(ItPSV itpsvRun=psv.begin()+dwStartOfNextElement;
			itpsvRun != end ; ++itpsvRun)
		{
			bValid=false;
    ////z.B. "Who has helped him?"
      if(bTypeOfQuestionWord==ASK_FOR_SUBJECT)
    //    //Es darf kein Adverb der Zeit vorne stehen bei Fragen:
    //    // "Who now helped you?" ist falsch.
    //    ParseQuestionOrStatementClauseExcludingSubject(...);
        ;
      else
      {
        //ParseBendPartOfPredicate(...);
    //    ParseNonBendPartOfPredicate(...);
    //    //TODO some question require a question word? 
    //    // E.g. "Have you been?" is false?
    //    //if(
    //    ParseSubject();
      }
//			if(!bFinitePartOfPredicateFound)
//			{
//				BYTE byReturnPersonIndex=0;
//				BYTE byPersonIndex;
//
//				CStringVector strToken;
//				//am Anfang aus den höchstmöglichen Wert setzen
//				DWORD dwPositionOfBendAuxiliary=ULONG_MAX;
//				Predicate predicate;
//			#ifdef _DEBUG
//				printf("predicate.m_bTense: %u\n",predicate.m_bTense);
//				fflush(stdout);
//			#endif
//				#ifdef _DEBUG
//				{
//					for(int i=0;i<sv.size();i++)
//					{
//						CString token=sv.at(i);
//						TRACE("sv.at(%d): %s\n",i,token);
//						printf("sv.at(%d): %s\n",i,token);
//					}
//				}
//				#endif
//				//predicate.m_adverbs.clear();
//				//for(int i=0;i<ss.m_adverbs.size();i++
//				for(DWORD dwRun=dwStartOfNextElement;dwRun<sv.size() && 
//					g_bContinue;++dwRun)
//					//itpsvRun != end && g_bContinue; ++itpsvRun)
//				{
//					CString token=sv.at(dwRun);
//					if(!(dwFlags&CASE_SENSITIVE)==1)
//						token.MakeLower();
//					TRACE("token: %s\n",token);
//					#ifdef _DEBUG
//					printf("token: %s\n",token);
//					#endif
//					if((byPersonIndex=ParseBendPartOfPredicate(predicate,bBe,
//						GetBetweenAsCStringVector(psv,dwStartOfNextElement,dwRun),
//						personIndex,dwFlags)))
//					{
//						bFinitePartOfPredicateFound=true;
//						byReturnPersonIndex=byPersonIndex;
//					}
//					else
//						if(bFinitePartOfPredicateFound)
//						{
//							//wenn das Fragewort nach einem Subjekt fragt,
//							if(bTypeOfQuestionWord==ASK_FOR_SUBJECT)
//								//dann fügt sich der unkonjugierte Teil des
//								//Prädikats direkt an den gebeugten
//								//z.B."Who has seen you?"
//								//dwRun ist an dieser Stelle der Index für einen 
//								//ungültigen "gebeugten Teil des Prädikats", und
//								//da es bereits einen gültigen teil gibt 
//								//(bFinitePartOfPredicateFound==true), ist es der Index 
//								//vom Token direkt dahinter
//								dwStartOfNextElement=dwRun;
//							else
//							{
//								//ansonsten muss sich zwischen konfugiertem und nicht
//								//konjugiertem Teil des Prädikats ein Subjekt mit
//								//mindestens 1 Token befinden, deswegen +1
//								//dwRun ist an dieser Stelle der Index für einen 
//								//ungültigen "gebeugten Teil des Prädikats", und
//								//da es bereits einen gültigen teil gibt 
//								//(bFinitePartOfPredicateFound==true), ist es der Index 
//								//vom Token direkt dahinter
//								dwStartOfNextElement=dwRun+1;
//								dwStartOfSubject=dwRun;
//							}
//							//wenn das Verb ein Hilfsverb ist (zum Beispiel "to be")
//							if(typeid(predicate.
//								m_pWordNodeWhereTheGermanTranslationIsABendVerb->
//								m_pWord)==typeid(EnglishAuxiliaryVerb))
//							{
//							#ifdef _DEBUG
//								printf("typeid(*pWordNode->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
//							#endif
//								EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
//									(predicate.
//									m_pWordNodeWhereTheGermanTranslationIsABendVerb->
//									m_pWord);
//								//wenn Speicher alloziert
//								if(eav)
//								{
//								#ifdef _DEBUG
//									printf("eav\n");
//								#endif
//									BOOL bValidAuxiliary=FALSE;
//									//wenn ein Fragewort existiert und dieses nach einem Subjekt fragt
//									//und das erste gebeugte Verb "to do" ist, dann ist der Satz 
//									//grammatikalisch falsch
//									if(pWordNodeWhereAnEnglishQuestionWordIsEncapsulated && 
//										bTypeOfQuestionWord==6 && eav->m_strWords[0]==_T("do"))
//									{
//			#ifdef _DEBUG
//										printf("BOOL ParseQuestionClause(Clause & ss,const \
//			CStringVector & sv) - return 0 - \"do\" bei einer Frage nach dem Subjekt\n");
//			#endif
//										//return 0;
//									}
//			#ifdef _DEBUG
//										printf("bPersonIndexVerb: %u\n",bPersonIndexVerb);
//			#endif
//								}
//								bValid=true;
//							}
//							//wenn das Verb ein Vollverb ist und das Fragewort 
//							//nach einem Subjekt fragt Beispiel: "Who helped you?"
//							if(typeid(predicate.
//								m_pWordNodeWhereTheGermanTranslationIsABendVerb->
//								m_pWord)==typeid(EnglishVerb) && bTypeOfQuestionWord==6)
//							{
//								pWordNodeWhereAnEnglishBendVerbIsEncapsulated=pWordNode;
//								//dwStartOfNextElement++;
//								bValid=true;
//							}
//							break;
//						}//if(bFinitePartOfPredicateFound)
//				}//for-Schleife für gebeugten Teil des Prädikats
//				//wenn an dieser Stelle kein gebeugter Teil des Prädikats
//				//gefunden, dann ist es kein vollständiger Satz
//				if(!bFinitePartOfPredicateFound)
//				{
//					//aus der for-Schleife für alle Token ausbrechen
//					break;
//				}
//				else
//				{
//					if(
//						//zu parsender Vektor enthält mindestens noch ein weiteres Token
//						dwRun<dwNumberOfTokens)
//					{
//						//if(!ParseNonBendPartOfPredicate(predicate,bBe,
//						//	GetBetween(sv,dwRun,sv.size()-1),
//						//	dwFlags//,predicate.
//						//	//m_pWordNodeWhereTheGermanTranslationIsABendVerb
//						//	))
//						//	byReturnPersonIndex=0;
//						bool bNotFinitePartOfPredicateFound=false;
//						//wenn das Fragewort nach einem Subjekt fragt,
//						if(bTypeOfQuestionWord==ASK_FOR_SUBJECT)
//						{
//							for(DWORD dwEndOfNotFinitePartOfPredicate=
//								dwStartOfNextElement;dwEndOfNotFinitePartOfPredicate<
//								dwNumberOfTokens && 
//								g_bContinue;++dwEndOfNotFinitePartOfPredicate)
//							{
//									CString token=sv.at(dwRun);
//									if(!(dwFlags&1)==1)
//										token.MakeLower();
//									TRACE("token: %s\n",token);
//									#ifdef _DEBUG
//									printf("token: %s\n",token);
//									#endif
//								BOOL bValidToken=FALSE;
//								if(ParseNonBendPartOfPredicate(predicate,bBe,
//									GetBetweenAsCStringVector(psv,
//									dwStartOfNextElement,
//									dwEndOfNotFinitePartOfPredicate),
//									dwFlags//,predicate.
//									//m_pWordNodeWhereTheGermanTranslationIsABendVerb
//									))
//									bNotFinitePartOfPredicateFound=true;
//								else
//									break;
//							}
//							dwStartOfNextClauseElement=
//								dwEndOfNotFinitePartOfPredicate;
//						}
//						else//if(bTypeOfQuestionWord==ASK_FOR_SUBJECT)
//						{
//							//DWORD dwBeginOfNotFinitePartOfPredicate=dwRun;
//							//der nicht-gebeugte Teil eines Prädikats beginnt hinter
//							//einem SUbjekt, falls das Fragewort nicht nach einem
//							//Subjekt fragt; da das Subjekt aber auch ein Eigenname
//							//sein kann, suche ich zuerst nach einem gültigen 
//							//nicht-gebeugten Teil des Prädikats.
//							//Weil der nicht-gebeugte Teil des Prädikats mehr als
//							//1 Token lang sein kann, eine verschachtelte Schleifen-
//							//konstruktion
//							for(DWORD dwStartOfNotFinitePartOfPredicate=
//								dwStartOfNextElement;dwStartOfNotFinitePartOfPredicate<
//								dwNumberOfTokens && 
//								g_bContinue;++dwStartOfNotFinitePartOfPredicate)
//							{
//								for(DWORD dwEndOfNotFinitePartOfPredicate=
//									dwStartOfNotFinitePartOfPredicate;
//									dwEndOfNotFinitePartOfPredicate<
//									dwNumberOfTokens && 
//									g_bContinue;++dwEndOfNotFinitePartOfPredicate)
//								{
//									CString token=sv.at(dwRun);
//									if(!(dwFlags&1)==1)
//										token.MakeLower();
//									TRACE("token: %s\n",token);
//									#ifdef _DEBUG
//									printf("token: %s\n",token);
//									#endif
//									BOOL bValidToken=FALSE;
//									if(ParseNonBendPartOfPredicate(predicate,bBe,
//										GetBetweenAsCStringVector(psv,
//										dwStartOfNotFinitePartOfPredicate,
//										dwEndOfNotFinitePartOfPredicate),
//										dwFlags//,predicate.
//										//m_pWordNodeWhereTheGermanTranslationIsABendVerb
//										))
//									{
//										dwEndOfSubject=dwStartOfNotFinitePartOfPredicate;
//										bNotFinitePartOfPredicateFound=true;
//									}
//									else
//									{
//										//um aus der äußeren Schleife auszubrechen
//										//(siehe Abbruchbedingung)
//										dwStartOfNotFinitePartOfPredicate=
//											dwNumberOfTokens;
//										dwStartOfNextClauseElement=
//											dwEndOfNotFinitePartOfPredicate;
//										break;
//									}
//								}
//							}
//						//der nicht-gebeugte Teil kann ja richtig sein, dann ist
//						//byReturnPersonIndex zunächst ungleich 0
//						//if(!bNotFinitePartOfPredicateFound)
//						//	byReturnPersonIndex=0;
//						}
//					}
//					if(bNotFinitePartOfPredicateFound)
//						;
//					else
//						//aus der for-Schleife für alle Token ausbrechen
//						break;
//					//return byReturnPersonIndex;
//				}
//			}//if(!bFinitePartOfPredicateFound)
//			else
//			{
//			}
		}//end of for-loop
//
//		//Prüfung, ob die Vektorgröße größer als der Start des nächsten Tokens ist
//		//(also ob sich der Token innerhalb des gültigen Bereichs des Vektors "psv"
//		//liegt)
//		//Wenn nicht geprüft wird, kann Folgendes passieren: "Where?": es liegt 
//		//ein Fragewort vor, daher wird dwStartOfNextElement um 1 erhöht
//		//psv hat aber nur 1 Element, daher ist dwStartOfNextElement ein ungültiger
//		//Index und es folgt ein Absturz des Programmes
//		if(dwNumberOfTokens>dwStartOfNextElement)
//		{
//
//		}
//		#ifdef _DEBUG
//		if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
//			printf("pWordNodeWhereAnEnglishBendVerbIsEncapsulated\n");
//		else
//		{
//			//printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector & sv)\n");
//			//return FALSE;
//		}
//		#endif
//		dwStartOfSubject=dwStartOfNextElement;
//		//nach einem Adverb suchen, das sich direkt hinter dem Subjekt befinden darf
//		//Beispiel: "always" ("Do I always like you?")
//		if(dwNumberOfTokens>dwStartOfNextElement)
//			for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size();dwRun++)
//			{
//				if((pWordNode=GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
//					psv.at(dwRun).m_Str) ))
//				{
//					#ifdef _DEBUG
//					printf("pVocNode=GetPointerToVocNodeWhereAnEnglishAdverbIsEnc\
//apsulated(sv.at(dwRun))\n");
//					#endif
//					EnglishAdverb * ea=dynamic_cast<EnglishAdverb *>(pWordNode->m_pWord);
//					if(ea)
//						if(ea->m_bType==0) // wenn Adverb, das sich hinter dem Subjekt befindet
//						{ // Beispiel: "always" ("Am I always intelligent?")
//							if(dwRun==dwStartOfSubject) // es befindet sich kein einziges Token
//							{ // zwischen dem gebeugten Hilfsverb und dem Adverb
//#ifdef _DEBUG
//								printf("ParseQuesionClause() - return FALSE\n");
//#endif
//								return FALSE;
//							}
//							ss.m_pWordNodeWhereAnEnglishAdverbIsEncapsulated=pWordNode;
//							dwStartOfNextElement=dwRun+1;
//							dwEndOfSubject=dwRun-1;
//							dwStartOfObject=dwRun+1;
//							bValidAdverbWhichIsSituatedBehindTheSubject=TRUE;
//							break;
//						}
//				}
//			}
//
//		}
//		//wenn kein Fragewort im Satz vorkommt, das 
//		//nach einem Subjekt fragt
//		if(bTypeOfQuestionWord!=ASK_FOR_SUBJECT)
//			ParseSubject(*ss.m_pSubject,GetBetween(psv,dwStartOfSubject,
//				dwEndOfSubject),0,rVecRange,dwFlags,FALSE);
//		if(ss.m_pSubject && psv.size()==dwStartOfNextElement)
//		{
//#ifdef _DEBUG
//			printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector & sv)\
//- return 1\n");
//#endif
//			return 1;
//		}
//		if(psv.size()>dwStartOfNextElement)
//		{
//#ifdef _DEBUG
//			printf("Suche nach einem Infinitiv\n");
//#endif
//		}
//		if(psv.size()>dwStartOfNextElement)
//		{
//			if(bMiddle)
//			{
//			#ifdef _DEBUG
//				printf("bMiddle\n");
//			#endif
//				if(psv.at(dwStartOfNextElement).m_Str==_T("to")) // zum Beispiel:
//				{ // "Do you want TO go?"
//			#ifdef _DEBUG
//					printf("psv.at(dwStartOfNextElement).m_Str==_T(\"to\")\n");
//			#endif
//					bTo=TRUE;
//					dwStartOfNextElement++;
//				}
//			}
//			#ifdef _DEBUG
//			else
//				printf("!bMiddle\n");
//			#endif
//		}
//		if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
//		{
//			#ifdef _DEBUG
//			printf("pWordNodeWhereAnEnglishBendVerbIsEncapsulated\n");
//			#endif
//			if(typeid(*pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)==
//			typeid(EnglishAuxiliaryVerb))
//			{
//				#ifdef _DEBUG
//				printf("typeid(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord)==\
//typeid(EnglishAuxiliaryVerb)\n");
//				#endif
//				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
//				(pWordNodeWhereAnEnglishBendVerbIsEncapsulated->m_pWord);
//				if(eav)
//				{
//					#ifdef _DEBUG
//					printf("eav\n");
//					printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]); // Is it used? -> Wird es benutzt?
//					#endif
//				}
//			}
//		}
//		if(psv.size()>dwStartOfNextElement && bTo) // nur, wenn ein "to" direkt davor
//		{ // vorkam, kann dahinter ein Infinitiv stehen
//#ifdef _DEBUG
//			printf("psv.size()>dwStartOfNextElement && bTo\n");
//#endif
//			if((pWordNode=GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
//				psv.at(dwStartOfNextElement).m_Str) ))
//			{
//				if(typeid(*pWordNode->m_pWord)==typeid(EnglishVerb))
//				{
//					EnglishVerb * ev=dynamic_cast<EnglishVerb *>(pWordNode->m_pWord);
//					if(ev)
//						if(psv.at(dwStartOfNextElement).m_Str==ev->m_strInfinitive)
//							//if(ss.m_pVocNodeWhereAnEnglishPredicateIsEncapsulated)
//							if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
//							{
//								//dwStartOfNextElement++;
//								dwStartOfNextElement++;
//								dwStartOfObject=dwStartOfNextElement+1;
//								pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=pWordNode;
//							}
//							//pVocNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated=pVocNode;
//				}
//			}
//		}
//		if(!pWordNodeWhereAnEnglishBendVerbIsEncapsulated && 
//			!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
//		{
//#ifdef _DEBUG
//			printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector & sv)\
//- return FALSE - !pWordNodeWhereAnEnglishBendVerbIsEncapsulated && \
//!pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
//#endif
//			return FALSE;
//		}
//		#ifdef _DEBUG
//		printf("ss.m_bTense: %u\n",ss.m_bTense);
//		printf("psv.size(): %u dwStartOfNextElement: %u\n",psv.size(),dwStartOfNextElement);
//		#endif
//
//		if(psv.size()>dwStartOfNextElement)
//		{
//#ifdef _DEBUG
//			printf("psv.at(dwStartOfNextElement).m_Str: %s \n",
//				psv.at(dwStartOfNextElement).m_Str);
//#endif
//			if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
//			{
//#ifdef _DEBUG
//				printf("pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated\n");
//#endif
//				if(typeid(
//					*pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//					m_pWord)==typeid(EnglishVerb))
//				{
//					EnglishVerb * ev=dynamic_cast<EnglishVerb*>
//						(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated->
//						m_pWord);
//					if(ev)
//						if(ev->m_strPreposition!="")
//							if(ev->m_strPreposition==psv.at(dwStartOfNextElement).m_Str)
//							{
//#ifdef _DEBUG
//								printf("ev->m_strPreposition: %s\n",ev->m_strPreposition);
//#endif
//								dwStartOfNextElement++;
//								dwStartOfObject=dwStartOfNextElement;
//							}
//							else
//							{
//#ifdef _DEBUG
//								printf("ParseQuestionClause return FALSE\n");
//#endif
//								return FALSE;
//							}
//				}
//			}
//		}
//		ParseClauseRightOfPredicate(ss,rVecRange);
//#ifdef _DEBUG
//			printf("dwStartOfNextElement: %u psv.size(): %u\n",
//				dwStartOfNextElement,psv.size());
//#endif
//			if(dwStartOfNextElement==psv.size())
//			{
//				if(pWordNodeWhereAnEnglishBendVerbIsEncapsulated)
//					ss.m_pWordNodeWhereAnEnglishPredicateIsEncapsulated=
//						pWordNodeWhereAnEnglishBendVerbIsEncapsulated;
//				if(pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated)
//					ss.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=
//						pWordNodeWhereAnEnglishInfinitiveOrPastParticipleIsEncapsulated;
//				ss.m_pWordNodeWhereAnEnglishQuestionWordIsEncapsulated=
//					pWordNodeWhereAnEnglishQuestionWordIsEncapsulated;
//				ss.m_pWordNodeWhereAnEnglishInfinitiveIsEncapsulated=
//					pWordNodeWhereAnEnglishInfinitiveIsEncapsulated;
//				ss.m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated=
//					pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated;
//#ifdef _DEBUG
//				printf("ss.m_bTense: %u\n",ss.m_bTense);
//				printf("BOOL ParseQuestionClause(Clause & subsentence,\
//CStringVector sv,std::vector<Range> & rVecRange) - return TRUE\n");
//#endif
//				return TRUE;
//			}
	}//if(psv.size()>0)
	#ifdef _DEBUG
	//printf("ss.m_bTense: %u\n",ss.m_bTense);
	printf("BOOL ParseQuestionClause(Clause & ss,const CStringVector \
& sv) - ENDE - return FALSE\n");
	#endif
	return FALSE;
}

BOOL ParseQuestionMiddle(Clause & ss,const CStringVector & sv)
{
	TRACE("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ANFANG\n");
	#ifdef _DEBUG
	printf("BOOL ParseQuestionMiddle(Clause & ss,const CStringVector & sv) - ANFANG\n");
	#endif
	ss.m_vecMiddleEnumerationElement.clear(); // Vektor leeren (enthält möglicherweise 
	// bereits Elemente durch einen vorherigen Aufruf dieser Funktion
	BOOL b3rdPerson=FALSE;
	BOOL bAux=FALSE;
	BOOL bDo=FALSE;
	BOOL bFrequency=FALSE;
	BOOL bHave=FALSE;
	BOOL bManner=FALSE;
	BOOL bNot=FALSE;
	BOOL bVerb=FALSE;
	BOOL bFuture=FALSE;
	BOOL bBe=FALSE;
	//CString token;
	if(!(sv.size()>0))
	{
		TRACE("ParseMiddle ENDE - return FALSE\n");
		#ifdef _DEBUG
		printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) - ENDE - return FALSE - sv.size()==0\n");
		#endif
		return FALSE;
	}
	#ifdef _DEBUG
	{
		for(DWORD i=0;i<sv.size();i++)
		{
			CString token=sv.at(i);
			TRACE("sv.at(%d): %s\n",i,token);
			printf("sv.at(%d): %s\n",i,token);
		}
	}
	#endif
	ss.m_vecEnglishAdverb.clear();
	//for(int i=0;i<ss.m_adverbs.size();i++
	for(DWORD i=0;i<sv.size();i++)
	{
		CString token=sv.at(i);
#ifdef _WINDOWS
		TRACE("token: %s\n",token);
#endif
		if(IsEnglishInfinitive(token))
		{
			if(IsEnglishAuxiliaryVerb(token)) // englisches Hilfsverb (z.B. 'to be')
			{
				//EnglishAuxiliaryVerb
				CString strBe("be") ;
				EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strBe);
				if(token==eav->m_strWords[1] || token==eav->m_strWords[2] ||  // am, are, is
					token==eav->m_strWords[3] || token==eav->m_strWords[7] || 
					token==eav->m_strWords[8])
				{  // was, were
					bBe=TRUE;
					ss.m_strPredicate=token;//"be";
					ss.m_bTense=SIMPLE_PRESENT;
					if(bFrequency)
					{
						#ifdef _DEBUG
						printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv) - ENDE - return FALSE - bFrequency\n");
						#endif
						return FALSE;
					}
				}
				CString strHave("have") ;
				eav = GetEnglishAuxiliaryVerb(strHave);
				if(token==eav->m_strWords[1] || // have, has
					token==eav->m_strWords[3] || token==eav->m_strWords[7] || 
					token==eav->m_strWords[9])
				{  // was, were
					bHave=TRUE;
					ss.m_strPredicate=token;//"have";
					ss.m_bTense=SIMPLE_PRESENT;
				}
				CString strDo("do") ; 
				eav=GetEnglishAuxiliaryVerb(strDo);
				if(token==eav->m_strWords[1] || // do, does
					token==eav->m_strWords[3] || token==eav->m_strWords[7])
				{  // did
					bDo=TRUE;
					ss.m_strPredicate=token;//"have";
					ss.m_bTense=SIMPLE_PRESENT;
				}
				if(token==_T("will"))
				{
					if(bFuture || bFrequency)
					{
						#ifdef _DEBUG
						printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv) - ENDE - return FALSE - bFuture || bFrequency\n");
						#endif
						return FALSE;
					}
					bFuture=TRUE;
				}
				if(bDo || ss.m_bNegation || bManner)
				{
					#ifdef _DEBUG
					printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector \
& sv) - ENDE - return FALSE - bDo || ss.bNegation || bManner\n");
					#endif
					return FALSE;
				}
				bAux=TRUE;
				ss.m_bBe=TRUE;
			}
			else // kein Hilfverb
			{
				if(IsPresentProgressive(token))
				{
					if(bFuture || !bBe)
					{
						#ifdef _DEBUG
						printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv) - ENDE - return FALSE - bFuture || !bBe\n");
						#endif
						return FALSE;
					}
					CString strBe = CString("be") ;
					EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strBe);
					BYTE pi=ss.m_pSubject->m_personIndex;
					if(ss.m_strPredicate==eav->m_strWords[1] && pi==1 ||
						(ss.m_strPredicate==eav->m_strWords[2] && (pi==2 || pi==4 || 
						pi==5 || pi==6)) ||
						ss.m_strPredicate==eav->m_strWords[3] && pi==3)
					{ // am, are, is
						ss.m_bTense=PRESENT_PROGRESSIVE;
						if(token.GetLength()>3)
						{
							ss.m_strPredicate=token.Left(token.GetLength()-3);
						}
					}
					else
					{
						#ifdef _DEBUG
						printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv) - ENDE - return FALSE - keine Übereinstimmung\n");
						#endif
						return FALSE;
					}
				}
				if(IsPresentTense(token))
				{
					if(bFuture)
						ss.m_bTense=FUTURE;
					else
						ss.m_bTense=SIMPLE_PRESENT;
					ss.m_strPredicate=token;
				}
				else if(IsSimplePast(token))
				{
					if(bFuture)
					{
						#ifdef _DEBUG
						printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv) - ENDE - return FALSE\n");
						#endif
						return FALSE;
					}
					ss.m_bTense=SIMPLE_PAST;
				}
				if(IsPastParticiple(token))
				{
					CString strHave("have") ;
					EnglishAuxiliaryVerb * eav=GetEnglishAuxiliaryVerb(strHave);
					BYTE pi=ss.m_pSubject->m_personIndex;
#ifdef _WINDOWS
					TRACE("ss.m_pSubject->m_personIndex: %u\n",ss.m_pSubject->m_personIndex);
#endif
					if(ss.m_strPredicate==eav->m_strWords[1] && pi==1 ||
						(ss.m_strPredicate==eav->m_strWords[2] && (pi==2 || pi==4 || 
						pi==5 || pi==6)) ||
						ss.m_strPredicate==eav->m_strWords[3] && pi==3)
					{ // am, are, is
						ss.m_bTense=PRESENT_PERFECT;
						EnglishVerb * ev=GetEnglishVerb(token);
						ss.m_strPredicate=ev->m_strInfinitive;
					}
					else
					{
						#ifdef _DEBUG
						printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv) - ENDE - return FALSE\n");
						#endif
						return FALSE;
					}
				}
				bVerb=TRUE;
			}
			if(IsEnglishVerb3rdPerson(token))
			{
				TRACE("IsEnglishVerb3rdPerson(token)==TRUE\n");
				b3rdPerson=TRUE;
			}
			if(ss.m_bTense==SIMPLE_PAST)
			{
//				EnglishVerb * ev=GetEnglishVocableOfEnglishVerb(token);
	//			if(token!=ev->infinitive)
		//			return FALSE;
			}
		}
		/*else if(token==_T("will"))
		{
			if(bFuture || bFrequency)
				return FALSE;
			bFuture=TRUE;
		}*/
		//else if(IsEnglishAdverb(token,'1'))
		else if(IsEnglishAdverbOfFrequency(token))
		{
			TRACE("IsEnglishAdverbOfFrequency(token)\n");
			if(bVerb || bManner || bFrequency)
			{
				#ifdef _DEBUG
				printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector \
& sv) - ENDE - return FALSE- bVerb || bManner || bFrequency\n");
				#endif
				return FALSE;
			}
			bFrequency=TRUE;
			EnglishAdverb ea;
			ea.m_bType='1';
			ea.m_strWord=token;
			ss.m_vecEnglishAdverb.push_back(ea);
		}
		else if(IsEnglishAdverbRefersToAdjectiveOrAdverb(token) || 
			IsEnglishAdverbOfManner(token))
		{
			if(IsEnglishAdverbOfManner(token))
			{
				bManner=TRUE;
			}
//			if(i+1<sv.size())
	//		{
				DWORD dwStart=i;
				for(DWORD dwRun=i;dwRun<sv.size();dwRun++)
				{
					token=sv.at(dwRun);
					if(token=="," || token=="and")
					{
						MiddleEnumerationElement mee;
						CStringVector svMEE=GetBetween(sv,dwStart,dwRun-1);
						if(!ParseMiddleEnumerationElement(mee,svMEE,TRUE))
						{
							#ifdef _DEBUG
							printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv) - ENDE - return FALSE - !ParseMiddleEnumeration\
Element(mee,svMEE)\n");
							#endif
							return FALSE;
						}
						dwStart=dwRun+1;
						ss.m_vecMiddleEnumerationElement.push_back(mee);
					}
					if(dwRun+1==sv.size())
					{
						MiddleEnumerationElement mee;
						CStringVector svMEE=GetBetween(sv,dwStart,dwRun);
						if(!ParseMiddleEnumerationElement(mee,svMEE,TRUE))
						{
							#ifdef _DEBUG
							printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv) - ENDE - return FALSE- !ParseMiddleEnumeration\
Element(mee,svMEE)\n");
							#endif
							return FALSE;
						}
						//dwStart=dwRun+1;
						ss.m_vecMiddleEnumerationElement.push_back(mee);
						#ifdef _DEBUG
						printf("ParseMiddle(Clause & ss,BOOL bBe,const \
CStringVector & sv) - ENDE - return TRUE\n");
						#endif
						return TRUE;
					}
				}
		//	}
			TRACE("IsEnglishAdverbOfManner(token)\n");
/*			if(bManner)
			{
				TRACE("ParseMiddle ENDE - return FALSE\n");
				return FALSE;
			}
			bManner=TRUE;
			EnglishAdverb ea;
			ea.type='3';
			ea.word=token;
			ss.m_adverbs.push_back(ea);*/
		}
		else if(IsEnglishAdverb(token,'3'))
		{
			bManner=TRUE;
			EnglishAdverb ea;
			ea.m_bType='1';
			ea.m_strWord=token;
			ss.m_vecEnglishAdverb.push_back(ea);
		}
		else if(token==_T("do") || token==_T("does") || token==_T("did"))
		{
			if(bAux || bNot)
			{
				#ifdef _DEBUG
				printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector \
& sv) - ENDE - return FALSE- bAux || bNot\n");
				#endif
				return FALSE;
			}
			bDo=TRUE;
			if(token==_T("did"))
				ss.m_bTense=SIMPLE_PAST;
		}
		else if(token==_T("not"))
		{
			TRACE("token==\"not\"\n");
			if(bAux)
			{
				if(bFrequency || bManner || bDo)
				{
					#ifdef _DEBUG
					printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector \
& sv) - ENDE - return FALSE - bFrequency || bManner || bDo\n");
					#endif
					return FALSE;
				}
			}
			else
			{
				if(bManner || bVerb)
				{
					#ifdef _DEBUG
					printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector \
& sv) - ENDE - return FALSE-bManner || bVerb\n");
					#endif
					return FALSE;
				}
			}
			ss.m_bNegation=TRUE;
		}
		else
		{
			#ifdef _DEBUG
			printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & \
sv) - ENDE - return FALSE\n");
			#endif
			return FALSE;
		}
	}
	#ifdef _DEBUG
	printf("ParseMiddle(Clause & ss,BOOL bBe,const CStringVector & sv) \
- ENDE - return TRUE\n");
	#endif
	return TRUE;
}

BOOL ParseRelativeClause(RelativeClause & rc,CStringVector sv)
{
	BOOL bValidSubject=FALSE;
	BOOL bValidMiddle=FALSE;
	Subject subject;
	DWORD dwEndOfValidSubject=0;
	DWORD dwEndOfValidMiddle=0;
	#ifdef _DEBUG
	printf("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) \
- ANFANG\n");
	for(DWORD dwRun2=0;dwRun2<sv.size();dwRun2++)
	{
		printf("sv.at(%u): %s\n",dwRun2,sv.at(dwRun2));
	}
	#endif
	for(DWORD dwRun=0;dwRun<sv.size();dwRun++)
	{
		CStringVector svSubject=GetBetween(sv,0,dwRun);
		/*if(ParseSubject(subject,svSubject,TRUE))
		{
			//dwEnd=dwRun;
			dwEndOfValidSubject=dwRun;
			bValidSubject=TRUE;
			rc.m_Subject=subject;
		}*/
	}
	if(bValidSubject)
	{
		#ifdef _DEBUG
		printf("bValidSubject\n");
		#endif
		if(dwEndOfValidSubject+1<sv.size())
		{
			#ifdef _DEBUG
			printf("dwEndOfValidSubject+1<sv.size()\n");
			#endif
			for(DWORD dwRun=dwEndOfValidSubject+1;dwRun<sv.size();dwRun++)
			{
				CStringVector svMiddle=GetBetween(sv,dwEndOfValidSubject+1,dwRun);
				Clause ss;
				#ifdef _DEBUG
				printf("vor: ParseMiddle(rc,TRUE,svMiddle)\n");
				#endif
				if(ParseMiddle(rc,TRUE,svMiddle))
				{
					bValidMiddle=TRUE;
					dwEndOfValidMiddle=dwRun;
				}
			}
		}
		else
			TRACE("!dwEnd+1<sv.size()-1\n");
	}
	//EnglishVerb * ev=GetEnglishVerb(rc.predicate);
	GermanVerb * gv=GetGermanWordOfEnglishVerb(rc.m_strPredicate);
	if(gv)
		if(gv->m_bCase==3)
		{
			if(bValidMiddle)
			{
				Object object;
				/*vecRange,
				if(ParseObject(object,GetBetween(sv,dwEndOfValidMiddle+1,sv.size()-1),TRUE,vecRange,TRUE))
				{
					rc.m_Object=object;
					TRACE("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) - ENDE - return TRUE\n");
					#ifdef _DEBUG
					printf("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) - ENDE - return TRUE\n");
					#endif
					return TRUE;
				}*/
			}
		}
		else
		{
			if(bValidMiddle && dwEndOfValidMiddle+1==sv.size())
			{
				TRACE("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) - ENDE - return TRUE\n");
				#ifdef _DEBUG
				printf("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) - ENDE - return TRUE\n");
				#endif
				return TRUE;
			}
		}
	TRACE("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) - ENDE - return FALSE\n");
	#ifdef _DEBUG
	printf("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) - ENDE - return FALSE\n");
	#endif
	return FALSE;
}

//diese Funktion wird von ParseClause aufgerufen
BYTE ParseRelativeClause(RelativeClause & relativeClause,const 
	PositionCStringVector & psv,std::vector<Range> & rVecRange,DWORD dwFlags)
{
	BOOL bValidPredicate=FALSE;
	BOOL bReturnOfParseSubject=0;
	BYTE bPersonIndex;
	BYTE bPreviousPersonIndex;
	BYTE bPreviousPredicateWasValid=0;
	BYTE bPreviousPredicateIsMoreProbably=FALSE;
	DWORD dwBeginOfValidMiddle=0,dwEndOfValidMiddle=0;
	Predicate predicateValid;
	Subject subject;
	//std::vector<Range> vecRange;
#ifdef _DEBUG
	printf("BYTE ParseRelativeClause(RelativeClause & relativeClause,const \
PositionCStringVector & psv,const std::vector<Range> & rVecRange,DWORD \
dwFlags) ANFANG\n");
	for(DWORD dwRun2=0;dwRun2<psv.size();dwRun2++)
	{
		printf("psv.at(%u): %s\n",dwRun2,psv.at(dwRun2).m_Str);
	}
#endif
	for(DWORD dwRun=1;dwRun<psv.size() && g_bContinue;dwRun++)
	{
		for(DWORD dwRun2=dwRun;dwRun2<psv.size() && g_bContinue;dwRun2++)
		{
			BYTE bReturnOfParseMiddle;
			Predicate predicate;
			if((bReturnOfParseMiddle=ParseFullPredicate(predicate,TRUE,
				GetBetweenAsCStringVector(psv,dwRun,dwRun2),0,dwFlags)))
			{
				bPreviousPredicateIsMoreProbably=FALSE;
				if(bPreviousPredicateWasValid)
          //Wenn die zuvor als korrektes Prädikat geparste Token-Kette mehr Token hat.
          //Bsp.: zuvor: "have dialled", jetzt: "dialled"
					if(dwBeginOfValidMiddle<dwRun && dwEndOfValidMiddle==dwRun)
					{
						bPreviousPredicateIsMoreProbably=TRUE;
					}
				if(!bPreviousPredicateIsMoreProbably)
				{
#ifdef _DEBUG
					printf("!bPreviousPredicateIsMoreProbably\n");
#endif
					dwBeginOfValidMiddle=dwRun;
					dwEndOfValidMiddle=dwRun2;
					predicateValid=predicate;
					bValidPredicate=TRUE;
					//dwBeginOfValidMiddle=dwRun;
					//dwEndOfValidMiddle=dwRun2;
					bPreviousPredicateWasValid=1;
					//bPersonIndex=bPreviousPersonIndex;
					bPersonIndex=bReturnOfParseMiddle;
				}
#ifdef _DEBUG
				else
					printf("bPreviousPredicateIsMoreProbably\n");
#endif
				bPreviousPersonIndex=bPersonIndex;
			}
			else
				bPreviousPredicateWasValid=FALSE;
		}
	}
	if(!bValidPredicate)
	{
#ifdef _DEBUG
		printf("BYTE ParseRelativeClause(RelativeClause & relativeClause,const \
PositionsCStringVector & psv,const std::vector<Range> vecRange,dwFlags) - \
return 0 - \n");
#endif
		return 0;
	}
	//32. Bit setzen (das bedeutet, dass das Subjekt nicht nach Relativsätzen 
	//überprüft werden soll)
	dwFlags|=DO_NOT_PARSE_SUBJECT_FOR_REL_CLAUSES; //binär: 10000000000000000000000000000000
	bReturnOfParseSubject=ParseSubject(subject,GetBetween(psv,0,
		dwBeginOfValidMiddle-1),FALSE,rVecRange,dwFlags,FALSE);
	if(bReturnOfParseSubject==0)
	{
#ifdef _DEBUG
		printf("BYTE ParseRelativeClause(RelativeClause & relativeClause,const \
PositionsCStringVector & psv,const std::vector<Range> vecRange,dwFlags) - \
return 0\n");
#endif
		return 0;
	}
#ifdef _DEBUG
	printf("bPersonIndex: %u\n",bPersonIndex);
	printf("relativeClause.m_Subject.m_personIndex: %u\n",subject.
		m_personIndex);
#endif
	//wenn der Personenindex des Prädikats nicht mit dem Personenindex des
	//Subjekts übereinstimmt
	//Beispiel, wenn er nicht übereinstimmt: "I likes you."
	//8 Bit von bPersonIndex geben an, welcher Personenindex bei einem
	//gebeugtem Verb möglich ist
	//Beispiel: burns:
	//Infinitiv: "burn" : stimmt nicht überein mit "burns"            0
	//Personenindex 1: "I burn" : stimmt nicht überein mit "burns"    0
	//Personenindex 2: "you burn" : stimmt nicht überein mit "burns"  0
	//Personenindex 3: "he/she/it burns" : stimmt überein mit "burns" 1
	//Personenindex 4: "we burn" : stimmt nicht überein mit "burns"   0
	//Personenindex 5: "you burn" : stimmt nicht überein mit "burns"  0
	//Personenindex 6: "they burn" : stimmt nicht überein mit "burns" 0
	//                                   Bit 8 bleibt ungenutzt, also 0
	//das resultierende Byte sieht so aus: 00010000 (dezimal: 16)
	//wenn die Bits des Byte nun nach rechts verschoben werden mit der
	//Anzahl der Stellen des Personenindex des Subjekts und das Ergebnis 
	//bitweise-Und mit 1 verknüpft wird und ungleich
	//"0" ist, stimmen der Personenindex des Subjekts und des Prädikats
	//überein
	//wenn Personenindex 3 (zum Beispiel "he"): 00001000 >> 3= 00000001
	//00000001 & 00000001=00000001 (also ungleich "0")
	if(!((bPersonIndex>>subject.m_personIndex)&1))
	{
	#ifdef _DEBUG
		printf("BYTE ParseRelativeClause(RelativeClause & relativeClause,const \
PositionsCStringVector & psv,const std::vector<Range> vecRange,dwFlags) - \
return 0 - falscher Personenindex\n");
	#endif
		return 0;
	}
	relativeClause.m_Subject=subject;
	if(dwEndOfValidMiddle+1==psv.size())
	{
		relativeClause.m_Predicate=predicateValid;
#ifdef _DEBUG
		printf("BYTE ParseRelativeClause(RelativeClause & relativeClause,const \
PositionsCStringVector & psv,const std::vector<Range> vecRange,dwFlags) - \
return bReturnOfParseSubject(%u)\n",bReturnOfParseSubject);
#endif
		return bReturnOfParseSubject;
	}
	/*if(predicateValid.m_pVocNodeWhereAnEnglishVerbIsEncapsulated)
	{
		if(predicateValid.m_pVocNodeWhereAnEnglishVerbIsEncapsulated->next)
		{
			GermanVerb * gv=dynamic_cast<predicateValid.
			m_pVocNodeWhereAnEnglishVerbIsEncapsulated->next->voc>;
			if(gv->m_type>
		}
	}
	if(dwEndOfValidMiddle*/
#ifdef _DEBUG
		printf("BYTE ParseRelativeClause(RelativeClause & relativeClause,const \
PositionsCStringVector & psv,const std::vector<Range> vecRange,dwFlags) - \
return 0\n");
#endif
	return 0;
}

BOOL ParseRelativeClause(Clause & ss,CStringVector sv)
{
#ifdef _WINDOWS
	TRACE("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) - ANFANG\n");
	for(DWORD dwRun2=0;dwRun2<sv.size();dwRun2++)
	{
		TRACE("sv.at(%u): %s\n",dwRun2,sv.at(dwRun2));
	}
#endif
	BOOL bValidSubject=FALSE;
	DWORD dwEnd=0;
	for(DWORD dwRun=0;dwRun<sv.size();dwRun++)
	{
		CStringVector svSubject=GetBetween(sv,0,dwRun);
		Subject subject;
		//if(ParseSubject(subject,svSubject,TRUE))
		//	dwEnd=dwRun;
	}
	if(dwEnd)
		if(dwEnd+1<sv.size())
		{
			CStringVector svMiddle=GetBetween(sv,dwEnd+1,sv.size()-1);
			if(ParseMiddle(ss,TRUE,svMiddle,1,TRUE))
			{
				TRACE("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) - ENDE - return TRUE\n");
				return TRUE;
			}
		}
	TRACE("BOOL ParseRelativeClause(Clause & ss,CStringVector sv) - ENDE - return FALSE\n");
	return FALSE;
}

//mit "Sentence" ist ein einfacher und/oder zusammengetzter Satz gemeint
BOOL ParseSentence(Sentence & sentence,
// psv soll eine Kopie sein (deswegen steht auch kein "&"
	// davor), damit die Veränderung durch die Funktion "MakeFirstLetterUpper"
	// sich nicht auf das Original auswirkt
									 PositionCStringVector & psv,
	std::vector<Range> & rVecRange,const DWORD & dwFlags)
{ 
	//Sentence sentence;
	TRACE("BOOL ParseSentence(Sentence & sentence,CStringVector sv) - ANFANG\n");
#ifdef _DEBUG
	printf("BOOL ParseSentence(Sentence & sentence,PositionCStringVector psv) - ANFANG\n");
	for(DWORD i2=0;i2<psv.size();i2++)
	{
		TRACE("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
		printf("psv.at(i2).m_Str: %s\n",psv.at(i2).m_Str);
	}
#endif
	g_dwSentenceStatus=0;
	if(psv.size()<=0)
	{
#ifdef _DEBUG
		printf("BOOL ParseSentence(Sentence & sentence,CStringVector sv) - return FALSE - sv.size()<=0\n");
#endif
		return FALSE;
	}
	if(!((dwFlags&ALLOW_LOW_LETTER_AT_THE_BEGINNING_OF_SENTENCE)==
		ALLOW_LOW_LETTER_AT_THE_BEGINNING_OF_SENTENCE))//wenn ein Satz mit einem Großbuchstaben beginnen muss
		if(!IsFirstLetterUpper(psv.at(0).m_Str)) // ein gültiger Satz muß mit einem
		{ // Großbuchstaben beginnen
			TRACE("BOOL ParseSentence(Sentence & sentence,CStringVector sv) - ENDE - return FALSE\n");
			return FALSE;
		}
	//if(psv.at(0).m_Str!="I") // wenn das erste Token nicht "I" ist, dann den
	//	MakeFirstLetterLower(psv.at(0).m_Str); // ersten Buchstaben des ersten Tokens
	// kleiner machen

	Clause subsentence;
	DWORD dwStart=0;
	DWORD dwEnd=0;
	DWORD dwEndOfValidAdverbRefersToWholeSentence=0;
	BOOL bClause=FALSE;
	#ifdef _DEBUG
	printf("vor: if(psv.size()>1)\n");
	#endif
	if(psv.size()>1)
	{
		CString str=psv.at(0).m_Str;
		if(!(dwFlags&CASE_SENSITIVE)==1)//wenn die Groß- und Kleinschreibung nicht relevant ist
			str.MakeLower();
#ifdef _INSERT_INTO_HASH_TREE
    Parse(psv) ;
#endif
		if(IsEnglishAdverbRefersToWholeSentence(str))
		{
			TRACE("IsEnglishAdverbRefersToWholeSentence(str)\n");
			if(psv.at(1).m_Str==",")
			{
				dwStart=2;
				dwEndOfValidAdverbRefersToWholeSentence=2;
				EnglishAdverb ea=GetEnglishAdverb(str);
				sentence.m_vecEnglishAdverb.push_back(ea);
			}
		}
		//TRACE("nach: if(sv.size()>1)\n");
		if(psv.at(psv.size()-1).m_Str==_T("too") && psv.at(psv.size()-2).m_Str==_T(","))
		{
			dwEnd=psv.size()-2;
		}
		else
			dwEnd=psv.size();
	}
#ifdef _DEBUG
	printf("nach: if(psv.size()>1)\n");
#endif
	CString conjunction;
	BOOL bBreak=FALSE;
	BOOL bPreviousClauseWasSOP=FALSE;
	BOOL bValidClause=FALSE;
	BOOL bValidClauseIfItContainsPersonalNames=FALSE;
	DWORD dwEndOfValidClause=0;
	DWORD dwEndOfValidClauseIfItContainsPersonalNames=0;
	DWORD dwStartOfNextClause=dwStart;
	DWORD dwEndOfPreviousValidClauseWithoutPersonalNamesInTheObject=0;
	DWORD dwEndOfPreviousValidClauseWithPersonalNamesInTheObject=0;
	DWORD dwEndOfValidClauseWithoutPersonalNamesInTheObject=0;
	DWORD dwEndOfValidClauseWithPersonalNamesInTheObject=0;
	BYTE returnValueOfPreviousClause=0;
	//Clause previousValidClause;
	Clause previousSubSentenceWithoutPersonalNamesInTheObject;
	Clause previousSubSentenceWithPersonalNamesInTheObject;
	Clause subSentenceWithoutPersonalNamesInTheObject;
	Clause subSentenceWithPersonalNamesInTheObject;
	BOOL bPreviousValidClauseWithPersonalNamesInTheObject=FALSE;
	BOOL bPreviousValidClauseWithoutPersonalNamesInTheObject=FALSE;
	std::vector<Range> vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject;
	std::vector<Range> vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject;
	std::vector<Range> vecRangeOfValidClauseWithPersonalNamesInTheObject;
	std::vector<Range> vecRangeOfValidClauseWithoutPersonalNamesInTheObject;
	BOOL bValidSentence=FALSE;
	//bFirstValidClause wird erst nach der zweiten for-Schleife auf TRUE gesetzt
	BOOL bFirstValidClause=FALSE;
	BYTE returnValue=0;
	BYTE previousSyntax=0;
	for(DWORD dwRun=dwStart;dwRun<dwEnd && g_bContinue;dwRun++)
	{
#ifdef _DEBUG
		printf("dwRun vor ParseClause: %u\n",dwRun);
#endif
    LOG("23.05.2008 11.10.43 "<<dwRun<<"\n");
		bValidClause=FALSE;
		bValidClauseIfItContainsPersonalNames=FALSE;
		if(psv.at(dwRun).m_Str==_T(","))
			dwRun++;
		Clause preferredSubSentence;
		std::vector<Range> vecRange;
		Clause ssValidClauseIfItContainsPersonalNames;
		BYTE ret=0;
		DWORD dwEndOfValidClause=0;
		for(DWORD dwRun2=dwRun;dwRun2<dwEnd && g_bContinue;++dwRun2)
		{
      LOG("23.05.2008 11.11.11 "<<dwRun2<<"\n");
			//if(dwRun2+1==dwEnd)
			//	bBreak=TRUE;
			g_dwStatus=dwRun2-dwRun;
#ifdef _DEBUG
			printf("dwRun2 vor ParseClause: %u\n",dwRun2);
#endif
			//vecRange.clear();
			Clause subSentence;
			std::vector<Range> parameterVecRange;
      LOGN("23.05.2008 11.11.19 "<<dwRun<<" "<<dwRun2);//BCC=Before Checking Clause
			//wenn der Rückgabewert von ParseClause() ungleich 0 ist
			if((ret=ParseClause(subSentence,GetBetween(psv,dwRun,dwRun2),
				parameterVecRange,dwFlags,(BYTE)!bFirstValidClause)))
			{
				//dwEndOfClause=dwRun;
				//dwStartOfNextClause=dwRun2+1;
				//wenn das der erste Haupt-/Nebensatz ist und dieser nicht direkt 
				//am Beginn der Folge der Token von psv liegt
				//if(dwEndOfValidClause==0 && dwRun>0)

				dwEndOfValidClause=dwRun2;
				returnValue=ret;
				if(ret==CLAUSE_CONTAINS_AT_LEAST_1_UNKNOWN_WORD)
				{
					if(!bFirstValidClause)
					{
						bPreviousValidClauseWithoutPersonalNamesInTheObject=TRUE;
						dwEndOfPreviousValidClauseWithoutPersonalNamesInTheObject=dwRun2;
						//preferredClause=subSentence;
						previousSubSentenceWithoutPersonalNamesInTheObject=subSentence;
						vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject=parameterVecRange;
						returnValueOfPreviousClause=ret;
					}
					else
					{
						dwEndOfValidClauseWithoutPersonalNamesInTheObject=dwRun2;
						bValidClause=TRUE;
						vecRangeOfValidClauseWithoutPersonalNamesInTheObject=parameterVecRange;
						subSentenceWithoutPersonalNamesInTheObject=subSentence;
					}
				}
				if(ret==CLAUSE_EXCLUSIVELY_CONTAINS_KNOWN_WORDS)
				{
					if(!bFirstValidClause)
					{
						dwEndOfPreviousValidClauseWithPersonalNamesInTheObject=dwRun2;
						bPreviousValidClauseWithPersonalNamesInTheObject=TRUE;
						previousSubSentenceWithPersonalNamesInTheObject=subSentence;
						vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject=parameterVecRange;
						returnValueOfPreviousClause=ret;
					}
					else
					{
						bValidClauseIfItContainsPersonalNames=TRUE;
						dwEndOfValidClauseWithPersonalNamesInTheObject=dwRun2;
						vecRangeOfValidClauseWithPersonalNamesInTheObject=parameterVecRange;
						//if(dwRun2+1==dwEnd)
						//	ssValidClauseIfItContainsPersonalNames=subSentence;
						subSentenceWithPersonalNamesInTheObject=subSentence;
					}
				}
				//bValidClause=TRUE;
				#ifdef _DEBUG
				if(preferredSubSentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated)
					printf("preferredSubSentence.m_pVocNodeWhereAnEnglishConjunctionIsEncapsulated\n");
				else
					printf("!preferredSubSentence.m_pVocNodeWhereAnEnglishConjunctionIsEncapsulated\n");
				#endif
        LOGN("23.05.2008 11.11.33 "<<dwRun<<" "<<dwRun2<<" "<<(short)ret);//ACC=After Checking Clause
			}
			#ifdef _DEBUG
			else
				printf("ret==0\n");
			if(preferredSubSentence.m_pWordNodeWhereAnEnglishConjunctionIsEncapsulated)
				printf("preferredSubSentence.m_pVocNodeWhereAnEnglishConjunctionIsEncapsulated\n");
			else
				printf("!preferredSubSentence.m_pVocNodeWhereAnEnglishConjunctionIsEncapsulated\n");
			#endif
		}
		//wenn bValidClause==TRUE (bValidClause ist beim ersten Mal nach der 
		//zweiten for-Schleife immer FALSE)
		if(bValidClause)
		{
      LOG("23.05.2008 11.11.43\n");//VC=Valid Clause
#ifdef _DEBUG
			printf("bValidClause\n");
#endif
			//ohne Eigennamen
			if(bPreviousValidClauseWithoutPersonalNamesInTheObject)
			{
				if(previousSubSentenceWithoutPersonalNamesInTheObject.
					m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
					for(DWORD dwRun3=0;dwRun3<
						previousSubSentenceWithoutPersonalNamesInTheObject.
						m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun3++)
					{
						GermanConjunction * gc=dynamic_cast<GermanConjunction *>
							(previousSubSentenceWithoutPersonalNamesInTheObject.
							m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.
							at(dwRun3)->m_pWordNodeNext->m_pWord);
						if(gc)
							if(gc->m_bWordOrder==1) // Satzbau ist: Subjekt Objekt Prädikat 
								previousSubSentenceWithoutPersonalNamesInTheObject.
								m_bWordOrder=SOP;
					}
				else if((sentence.m_vecEnglishAdverb.size()>0 && 
					sentence.m_vecClause.size()==0) || previousSyntax==SOP)
					previousSubSentenceWithoutPersonalNamesInTheObject.m_bWordOrder=PSO;
				else
					previousSubSentenceWithoutPersonalNamesInTheObject.m_bWordOrder=SPO;
				sentence.m_vecClause.push_back(
					previousSubSentenceWithoutPersonalNamesInTheObject);
				//g_dwStatus=dwRun2;
				g_dwSentenceStatus=
					dwEndOfPreviousValidClauseWithoutPersonalNamesInTheObject;
				previousSyntax=previousSubSentenceWithoutPersonalNamesInTheObject.
					m_bWordOrder;
				for(DWORD dwRun3=0;dwRun3<
					vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject.
					size();dwRun3++)
					rVecRange.push_back(
					vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject.
					at(dwRun3));
				returnValueOfPreviousClause=2;
				vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject=
					vecRangeOfValidClauseWithoutPersonalNamesInTheObject;
				dwEndOfPreviousValidClauseWithoutPersonalNamesInTheObject=
					dwEndOfValidClauseWithoutPersonalNamesInTheObject;
				previousSubSentenceWithoutPersonalNamesInTheObject=
					subSentenceWithoutPersonalNamesInTheObject;
			}
			//wenn mit Eigennamen
			else if(bPreviousValidClauseWithPersonalNamesInTheObject)
			{
				if(previousSubSentenceWithPersonalNamesInTheObject.
					m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
					for(DWORD dwRun3=0;dwRun3<
						previousSubSentenceWithPersonalNamesInTheObject.
						m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun3++)
					{
						GermanConjunction * gc=dynamic_cast<GermanConjunction *>
							(previousSubSentenceWithPersonalNamesInTheObject.
							m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.
							at(dwRun3)->m_pWordNodeNext->m_pWord);
						if(gc)
							if(gc->m_bWordOrder==1) // Satzbau ist: Subjekt Objekt Prädikat 
								previousSubSentenceWithPersonalNamesInTheObject.
								m_bWordOrder=SOP;
					}
				else if((sentence.m_vecEnglishAdverb.size()>0 && sentence.m_vecClause.
					size()==0) || previousSyntax==SOP)
					previousSubSentenceWithPersonalNamesInTheObject.m_bWordOrder=PSO;
				else
					previousSubSentenceWithPersonalNamesInTheObject.m_bWordOrder=SPO;
			#ifdef _DEBUG
				printf("preferredSubSentence.m_bWordOrder: %u\n",preferredSubSentence.m_bWordOrder);
			#endif
				sentence.m_vecClause.push_back(
					previousSubSentenceWithPersonalNamesInTheObject);
				//g_dwStatus=dwRun2;
				g_dwSentenceStatus=dwEndOfPreviousValidClauseWithPersonalNamesInTheObject;
				previousSyntax=previousSubSentenceWithPersonalNamesInTheObject.
					m_bWordOrder;
				for(DWORD dwRun3=0;dwRun3<
					vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject.
					size();dwRun3++)
					rVecRange.push_back(
					vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject.
					at(dwRun3));
				returnValueOfPreviousClause=2;
				vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject=
					vecRangeOfValidClauseWithPersonalNamesInTheObject;
				dwEndOfPreviousValidClauseWithoutPersonalNamesInTheObject=
					dwEndOfValidClauseWithPersonalNamesInTheObject;
				previousSubSentenceWithoutPersonalNamesInTheObject=
					subSentenceWithPersonalNamesInTheObject;
			}
			dwRun=dwEndOfValidClauseWithoutPersonalNamesInTheObject; // dwRun entspricht Index des letzten Tokens eines gültigen
			bPreviousValidClauseWithoutPersonalNamesInTheObject=TRUE;
			bPreviousValidClauseWithPersonalNamesInTheObject=FALSE;
		}//if(validclause)
		//wenn bValidClause==FALSE (bValidClause ist beim ersten Mal nach der 
		//zweiten for-Schleife immer FALSE) und 
		//bValidClauseIfItContainsPersonalNames==TRUE 
		//(bValidClauseIfItContainsPersonalNames ist beim sten Mal nach der 
		//zweiten for-Schleife auch immer FALSE), also der "Untersatz" 
		//Eigennamen beinhaltet
		else if(bValidClauseIfItContainsPersonalNames)
		{
#ifdef _DEBUG
			printf("bValidClauseIfItContainsPersonalNames\n");
#endif
			if(bPreviousValidClauseWithoutPersonalNamesInTheObject)
			{
				if(previousSubSentenceWithoutPersonalNamesInTheObject.
					m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
					for(DWORD dwRun3=0;dwRun3<
						previousSubSentenceWithoutPersonalNamesInTheObject.
						m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun3++)
					{
						GermanConjunction * gc=dynamic_cast<GermanConjunction *>
							(previousSubSentenceWithoutPersonalNamesInTheObject.
							m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.at(dwRun3)->
							m_pWordNodeNext->m_pWord);
						if(gc)
							if(gc->m_bWordOrder==1) // Satzbau ist: Subjekt Objekt Prädikat 
								previousSubSentenceWithoutPersonalNamesInTheObject.
								m_bWordOrder=SOP;
					}
				else if((sentence.m_vecEnglishAdverb.size()>0 && sentence.m_vecClause.
					size()==0) || previousSyntax==SOP)
					previousSubSentenceWithoutPersonalNamesInTheObject.m_bWordOrder=PSO;
				else
					previousSubSentenceWithoutPersonalNamesInTheObject.m_bWordOrder=SPO;
				sentence.m_vecClause.push_back(
					previousSubSentenceWithoutPersonalNamesInTheObject);
				//g_dwStatus=dwRun2;
				g_dwSentenceStatus=dwEndOfPreviousValidClauseWithoutPersonalNamesInTheObject;
				previousSyntax=previousSubSentenceWithoutPersonalNamesInTheObject.
					m_bWordOrder;
				for(DWORD dwRun3=0;dwRun3<
					vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject.
					size();dwRun3++)
					rVecRange.push_back(
					vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject.at(dwRun3));
				returnValueOfPreviousClause=1;
				vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject=
					vecRangeOfValidClauseWithoutPersonalNamesInTheObject;
				dwEndOfPreviousValidClauseWithPersonalNamesInTheObject=
					dwEndOfValidClauseWithoutPersonalNamesInTheObject;
				previousSubSentenceWithPersonalNamesInTheObject=
					subSentenceWithoutPersonalNamesInTheObject;
			}
			//if(vecRange.size())
			//	rRange.insert(rRange.end()-1,vecRange.begin(),vecRange.end()-1);
			else if(bPreviousValidClauseWithPersonalNamesInTheObject)
			{
				if(previousSubSentenceWithPersonalNamesInTheObject.
					m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
					for(DWORD dwRun3=0;dwRun3<
						previousSubSentenceWithPersonalNamesInTheObject.
						m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun3++)
					{
						GermanConjunction * gc=dynamic_cast<GermanConjunction *>
							(previousSubSentenceWithPersonalNamesInTheObject.
							m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.at(dwRun3)->
							m_pWordNodeNext->m_pWord);
						if(gc)
							if(gc->m_bWordOrder==1) // Satzbau ist: Subjekt Objekt Prädikat 
								previousSubSentenceWithPersonalNamesInTheObject.
								m_bWordOrder=SOP;
					}
				else if((sentence.m_vecEnglishAdverb.size()>0 && sentence.m_vecClause.
					size()==0) || previousSyntax==SOP)
					previousSubSentenceWithPersonalNamesInTheObject.m_bWordOrder=PSO;
				else
					previousSubSentenceWithPersonalNamesInTheObject.m_bWordOrder=SPO;
				sentence.m_vecClause.push_back(
					previousSubSentenceWithPersonalNamesInTheObject);
				//g_dwStatus=dwRun2;
				g_dwSentenceStatus=dwEndOfPreviousValidClauseWithPersonalNamesInTheObject;
				previousSyntax=previousSubSentenceWithPersonalNamesInTheObject.
					m_bWordOrder;
				for(DWORD dwRun3=0;dwRun3<
					vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject.
					size();dwRun3++)
					rVecRange.push_back(
					vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject.at(dwRun3));
				returnValueOfPreviousClause=1;
				vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject=
					vecRangeOfValidClauseWithPersonalNamesInTheObject;
				dwEndOfPreviousValidClauseWithPersonalNamesInTheObject=
					dwEndOfValidClauseWithPersonalNamesInTheObject;
				//previousSubSentenceWithoutPersonalNamesInTheObject.satzbau=SPO;
				//sentence.m_subSentences.push_back(previousSubSentenceWithoutPersonalNamesInTheObject);
				previousSubSentenceWithPersonalNamesInTheObject=
					subSentenceWithPersonalNamesInTheObject;
			}
			//for(DWORD dwRun3=0;dwRun3<vecRange.size();dwRun3++)
			//	rVecRange.push_back(vecRange.at(dwRun3));
			dwRun=dwEndOfValidClauseWithPersonalNamesInTheObject; // dwRun entspricht Index des letzten Tokens eines gültigen
			bPreviousValidClauseWithoutPersonalNamesInTheObject=FALSE;
			bPreviousValidClauseWithPersonalNamesInTheObject=TRUE;

			// 'Teilsatzes' und wird am Schleifenende nochmal inkrementiert
		}//if(bValidClauseIfItContainsPersonalNames)
		//wenn bValidClause==FALSE (bValidClause ist beim ersten Mal nach der 
		//zweiten for-Schleife immer FALSE) und 
		//bValidClauseIfItContainsPersonalNames==FALSE 
		//(bValidClauseIfItContainsPersonalNames ist beim ersten Mal nach der 
		//zweiten for-Schleife auch immer FALSE), also der "Untersatz" keine
		//Eigennamen beinhaltet
		else
		{
			//wenn es noch keinen gültigen "Untersatz" gab
			if(!bFirstValidClause)
			{
				//wenn es einen gültigen "Untersatz" ohne Eigennamen im Objekt
				//gab
				if(bPreviousValidClauseWithoutPersonalNamesInTheObject)
					dwRun=dwEndOfPreviousValidClauseWithoutPersonalNamesInTheObject;
				else
					dwRun=dwEndOfPreviousValidClauseWithPersonalNamesInTheObject;
			}
			//wenn es bereits mindestens einen gültigen "Untersatz" (mit oder ohne 
			//Eigennamen) gab
			else
			{
 // ungültiger 'Teilsatz'
#ifdef _DEBUG
				printf("ungültiger 'Teilsatz' returnValueOfPreviousClause: %u\n",
					returnValueOfPreviousClause);
				printf("sentence.m_vecClause.size(): %u\n",sentence.m_vecClause.size());
#endif
				//wenn der Rückgabewert des letzten gültigen "Untersatzes", der am 
				//Ende der ersten for-Schleife zugeweisen wird,  eine '1' war
				//(gibt an, dass es ein "Untersatz" mit Eigennamen im Objekt war)
				if(returnValueOfPreviousClause==1)
				{
					//wenn das Ende des letzten gültigen "Untersatzes" mit Eigennamen 
					//im Objekt, der am Ende der ersten for-Schleife zugeweisen wird, 
					//mit dem Ende des ganzen Satzes abschliesst (es bleiben also keine
					//weiteren Token mehr übrig)
					if(dwEndOfPreviousValidClauseWithPersonalNamesInTheObject+1==dwEnd)
					{
						if(previousSubSentenceWithPersonalNamesInTheObject.
							m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
							for(DWORD dwRun3=0;dwRun3<
								previousSubSentenceWithPersonalNamesInTheObject.
								m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun3++)
							{
								GermanConjunction * gc=dynamic_cast<GermanConjunction *>
									(previousSubSentenceWithPersonalNamesInTheObject.
									m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.
									at(dwRun3)->m_pWordNodeNext->m_pWord);
								if(gc)
									if(gc->m_bWordOrder==1) // Satzbau ist: Subjekt Objekt Prädikat 
										previousSubSentenceWithPersonalNamesInTheObject.
										m_bWordOrder=SOP;
							}
						else if((sentence.m_vecEnglishAdverb.size()>0 && sentence.m_vecClause.
							size()==0) || previousSyntax==SOP)
							previousSubSentenceWithPersonalNamesInTheObject.m_bWordOrder=PSO;
						else
							previousSubSentenceWithPersonalNamesInTheObject.m_bWordOrder=SPO;
						sentence.m_vecClause.push_back(
							previousSubSentenceWithPersonalNamesInTheObject);
						previousSyntax=previousSubSentenceWithPersonalNamesInTheObject.
							m_bWordOrder;
						for(DWORD dwRun3=0;dwRun3<
							vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject.
							size();dwRun3++)
							rVecRange.push_back(
							vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject.
							at(dwRun3));
						bValidSentence=TRUE;
					}	
				//ssValidClauseIfItContainsPersonalNames;
				//sentence.push_back(previousSubSentenceWithPersonalNamesInTheObject);
				}
				//aus der ersten for-Schleife ausbrechen
				break;
			}
		}
    LOG("23.05.2008 11.12.09\n");//AVC=After Valid Clause
		returnValueOfPreviousClause=ret;
		bFirstValidClause=TRUE;
#ifdef _DEBUG
		printf("sentence.m_vecClause.size(): %u\n",sentence.m_vecClause.size());
#endif
		//if(bBreak)
		//	break;
	}//endo fo loop "for(DWORD dwRun=dwStart;dwRun<dwEnd && g_bContinue;dwRun++)"
	//wenn bei der letzten Überprüfung auf einen "Untersatz" ein
	//gültiger "Untersatz" mit oder ohne Eigennamen festgestellt wurde.
	//Wenn nicht, war der letzte "Untersatz" ungültig
	if(bValidClauseIfItContainsPersonalNames || bValidClause)
	{
    LOG("23.05.2008 11.12.20\n");//VIICPNOVCC=ValidClauseIfItContainsPersonalNames Or ValidClause
#ifdef _DEBUG
		printf("sentence.m_vecClauses.size(): %u\n",sentence.
			m_vecClause.size());
		printf("letzter 'Teilsatz' grammatikalisch korrekt, aber noch nicht \
angehängt; returnValue: %u\n",returnValue);
#endif
		//wenn der letzte Rückgabewert der Funktion ParseClause() bei der Über-
		//prüfung auf einen "Untersatz" die '1' war (eine 1 gibt an, dass es ein 
		//gültiger "Untersatz" mit Eigennamen war)
		if(returnValue==1)
		{
			if(dwEndOfValidClauseWithPersonalNamesInTheObject+1==dwEnd)
			{
				bValidSentence=TRUE;
				if(subSentenceWithPersonalNamesInTheObject.
					m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
					for(DWORD dwRun3=0;dwRun3<subSentenceWithPersonalNamesInTheObject.
						m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun3++)
					{
						GermanConjunction * gc=dynamic_cast<GermanConjunction *>
							(subSentenceWithPersonalNamesInTheObject.
							m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.at(dwRun3)->
							m_pWordNodeNext->m_pWord);
						if(gc)
							if(gc->m_bWordOrder==1) // Satzbau ist: Subjekt Objekt Prädikat 
								subSentenceWithPersonalNamesInTheObject.m_bWordOrder=SOP;
					}
				else if((sentence.m_vecEnglishAdverb.size()>0 && sentence.m_vecClause.
					size()==0) || 
					previousSyntax==SOP)
					subSentenceWithPersonalNamesInTheObject.m_bWordOrder=PSO;
				else
					subSentenceWithPersonalNamesInTheObject.m_bWordOrder=SPO;
				sentence.m_vecClause.push_back(
					subSentenceWithPersonalNamesInTheObject);
				//previousSyntax=previousSubSentenceWithPersonalNamesInTheObject.satzbau;
				for(DWORD dwRun3=0;dwRun3<
					vecRangeOfValidClauseWithPersonalNamesInTheObject.size();dwRun3++)
					rVecRange.push_back(
					vecRangeOfValidClauseWithPersonalNamesInTheObject.at(dwRun3));
			}
		}
		//wenn der letzte Rückgabewert der Funktion ParseSubSentence() bei der Über-
		//prüfung auf einen "Untersatz" die '2' war (eine 2 gibt an, dass es ein 
		//gültiger "Untersatz" ohne Eigennamen war)
		if(returnValue==2)
		{
#ifdef _DEBUG
			printf("dwEndOfValidClauseWithoutPersonalNamesInTheObject: %u\n",
				dwEndOfValidClauseWithoutPersonalNamesInTheObject);
#endif
			if(dwEndOfValidClauseWithoutPersonalNamesInTheObject+1==dwEnd)
			{
#ifdef _DEBUG
				printf("dwEndOfValidClauseWithoutPersonalNamesInTheObject+1==dwEnd\n");
#endif
				bValidSentence=TRUE;
				if(subSentenceWithoutPersonalNamesInTheObject.
					m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
					for(DWORD dwRun3=0;dwRun3<subSentenceWithoutPersonalNamesInTheObject.
						m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun3++)
					{
						GermanConjunction * gc=dynamic_cast<GermanConjunction *>
							(subSentenceWithoutPersonalNamesInTheObject.
							m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.at(dwRun3)->
							m_pWordNodeNext->m_pWord);
						if(gc)
							if(gc->m_bWordOrder==1) // Satzbau ist: Subjekt Objekt Prädikat 
								subSentenceWithoutPersonalNamesInTheObject.m_bWordOrder=SOP;
					}
				else if((sentence.m_vecEnglishAdverb.size()>0 && 
					sentence.m_vecClause.size()==0) || previousSyntax==SOP)
					subSentenceWithoutPersonalNamesInTheObject.m_bWordOrder=PSO;
				else
					subSentenceWithoutPersonalNamesInTheObject.m_bWordOrder=SPO;
				sentence.m_vecClause.push_back(
					subSentenceWithoutPersonalNamesInTheObject);
				//previousSyntax=previousSubSentenceWithoutPersonalNamesInTheObject.satzbau;
				for(DWORD dwRun3=0;dwRun3<
					vecRangeOfValidClauseWithoutPersonalNamesInTheObject.size();dwRun3++)
					rVecRange.push_back(
					vecRangeOfValidClauseWithoutPersonalNamesInTheObject.at(dwRun3));
			}
		}
	}//if(bValidClauseIfItContainsPersonalNames || bValidClause)
	//wenn dem Satz-Objekt noch überhaupt keine gültigen "Untersätze "
	//hinzugefügt wurden (wenn der Satz nur einen gültigen "Untersatz enthält")
	else if(sentence.m_vecClause.size()==0)
	{
    LOG("23.05.2008 11.12.25\n");//CS0=Clause Size 0
		//wenn der letzte gültige "Untersatz" ein gültiger "Untersatz" mit Eigennamen war
		if(returnValueOfPreviousClause==CLAUSE_WITH_PROPER_NAME)
		{
			//wenn cdas Ende des letzten gültigen "Untersatzes" mit Eigennamen im 
			//Objekt mit dem Ende des ganzen Satzes abschliesst (es bleiben keine 
			//Token mehr übrig)
			if(dwEndOfPreviousValidClauseWithPersonalNamesInTheObject+1==dwEnd)
			{
				//dann ist der ganze Satz gültig
				bValidSentence=TRUE;
				if(previousSubSentenceWithPersonalNamesInTheObject.
					m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
					for(DWORD dwRun3=0;dwRun3<
						previousSubSentenceWithPersonalNamesInTheObject.
						m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun3++)
					{
						GermanConjunction * gc=dynamic_cast<GermanConjunction *>
							(previousSubSentenceWithPersonalNamesInTheObject.
							m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.
							at(dwRun3)->m_pWordNodeNext->m_pWord);
						if(gc)
							if(gc->m_bWordOrder==1) // Satzbau ist: Subjekt Objekt Prädikat 
								previousSubSentenceWithPersonalNamesInTheObject.
								m_bWordOrder=SOP;
					}
				else if
          (
            (
              //TODO This branch seems to be the same as in the case below 
              //WITHOUT proper name.

              //E.g. "Unfortunatly Stefan does not like you."->
              // "Unglücklicherweise mag Stefan dich nicht."
              //                      P     S     O
              sentence.m_vecEnglishAdverb.size()>0 
              || 
			        (
                //E.g. "Now Stefan likes you."->
                // "Jetzt mag Stefan dich."
                //   Zeit  P     S     O
                previousSubSentenceWithoutPersonalNamesInTheObject.
			          m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated &&
                previousSubSentenceWithoutPersonalNamesInTheObject.
                m_bAdverbOfTimeAtTheBeginning
              )
              && 
              sentence.m_vecClause.size()==0
            ) 
            || 
            previousSyntax==SOP
          )
					previousSubSentenceWithPersonalNamesInTheObject.m_bWordOrder=PSO;
				else
					previousSubSentenceWithPersonalNamesInTheObject.m_bWordOrder=SPO;
				sentence.m_vecClause.push_back(
					previousSubSentenceWithPersonalNamesInTheObject);
				previousSyntax=previousSubSentenceWithPersonalNamesInTheObject.
					m_bWordOrder;
				for(DWORD dwRun3=0;dwRun3<
					vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject.
					size();dwRun3++)
					rVecRange.push_back(
					vecRangeOfPreviousValidClauseWithPersonalNamesInTheObject.at(dwRun3));
			}
		}
		//wenn der letzte gültige "Untersatz" ein gültiger "Untersatz" ohne Eigennamen war
		if(returnValueOfPreviousClause==CLAUSE_WITHOUT_PROPER_NAME)
		{
#ifdef _DEBUG
			printf("dwEndOfValidClauseWithoutPersonalNamesInTheObject: %u\n",
				dwEndOfValidClauseWithoutPersonalNamesInTheObject);
#endif
			if(dwEndOfPreviousValidClauseWithoutPersonalNamesInTheObject+1==dwEnd)
			{
#ifdef _DEBUG
				printf("dwEndOfValidClauseWithoutPersonalNamesInTheObject+1==dwEnd\n");
#endif
				bValidSentence=TRUE;
        //Wenn Haupt- oder Nebensatz mit Eigennamen und mind. 1 Konjunktion.
				if(previousSubSentenceWithoutPersonalNamesInTheObject.
					m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size()>0)
					for(DWORD dwRun3=0;dwRun3<
						previousSubSentenceWithoutPersonalNamesInTheObject.
						m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.size();dwRun3++)
					{
						GermanConjunction * gc=dynamic_cast<GermanConjunction *>
							(previousSubSentenceWithoutPersonalNamesInTheObject.
							m_vecpWordNodeWhereAnEnglishConjunctionIsEncapsulated.at(dwRun3)->
							m_pWordNodeNext->m_pWord);
						if(gc)
							if(gc->m_bWordOrder==1) // Satzbau ist: Subjekt Objekt Prädikat 
								previousSubSentenceWithoutPersonalNamesInTheObject.
								m_bWordOrder=SOP;
					}
				//Wenn ein Adverb der Zeit am Anfang des Untersatzes steht,
				//ist der Satzbau im Deutschen automatisch PSO (Prädikat Subjekt 
				//[Objekt])
        //Beispiel: "Now I close my eyes." -> 
				// "Jetzt schliesse ich meine Augen."
				//  -ADZ- -Prädikat -S- ---Objekt--
				//  ADZ: Adverb der Zeit   S: Subjekt
        //Hingegen: "I close my eyes now."-> "Ich schließe jetzt meine Augen" (SPO)
				else if
          (
            (
              (
                //E.g. "Unfortunatly I do not like you."->
                // "Unglücklicherweise mag ich dich nicht."
                //                      P   S   O
                sentence.m_vecEnglishAdverb.size()>0 
                || 
				        (
                  //E.g. "Now he likes you."->
                  // "Jetzt mag er dich."
                  //   Zeit  P   S   O
                  previousSubSentenceWithoutPersonalNamesInTheObject.
				          m_pWordNodeWhereAnEnglishAdverbOfTimeIsEncapsulated &&
                  previousSubSentenceWithoutPersonalNamesInTheObject.
                  m_bAdverbOfTimeAtTheBeginning
                )
              ) 
              && 
              sentence.m_vecClause.size()==0
            ) 
            || 
            previousSyntax==SOP
          )
					previousSubSentenceWithoutPersonalNamesInTheObject.m_bWordOrder=PSO;
				else
					previousSubSentenceWithoutPersonalNamesInTheObject.m_bWordOrder=SPO;
				sentence.m_vecClause.push_back(
					previousSubSentenceWithoutPersonalNamesInTheObject);
				previousSyntax=previousSubSentenceWithoutPersonalNamesInTheObject.
					m_bWordOrder;
				for(DWORD dwRun3=0;dwRun3<
					vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject.size();dwRun3++)
					rVecRange.push_back(
					vecRangeOfPreviousValidClauseWithoutPersonalNamesInTheObject.at(dwRun3));
			}
		}
	}//if(sentence.m_vecClause.size()==0)
#ifdef _DEBUG
	printf("sentence.m_vecClause.size(): %u\n",sentence.m_vecClause.
		size());
#endif
	if(bValidSentence) // wenn ALLE Teilsätze gültige Teilsätze sind, Funktion beenden
	{
    LOG("23.05.2008 11.13.28\n");//VSR=Valid Sentence Return
	#ifdef _DEBUG
		printf("bValidSentence\n");
		printf("rVecRange.size(): %u\n",rVecRange.size());
		printf("ParseSentence() - return TRUE\n");
		#endif
		return TRUE;
	}
	else
	{
#ifdef _DEBUG
		printf("!bValidSentence\n");
		printf("ParseSentence() - return FALSE\n");
#endif
		return FALSE;
	}
	dwStart=dwEndOfValidAdverbRefersToWholeSentence;
  LOG("23.05.2008 11.13.36\n");//BC=Before Continue
	if(g_bContinue)//!bSubSentence)
	{ // Es gibt NICHT mehrere Teilsätze, die durch eine Konjunktion
		// (Bindewort) oder durch ein Komma getrennt sind (der Satz kann
		// dennoch korrekt sein, enthält dann aber exakt EINEN Teilsatz
		//BOOL b
		TRACE("!bSubSentence\n");
		#ifdef _DEBUG
		printf("!bSubSentence\n");
		#endif
		{
			Clause subsentence;
			PositionCStringVector vecSub=GetBetween(psv,dwStart,dwEnd);
			std::vector<Range> vecRange;
			if(ParseClause(subsentence,vecSub,vecRange,TRUE,FALSE))
			{
				bClause=TRUE;
				sentence.m_vecClause.push_back(subsentence);
				TRACE("ParseSentence ENDE - return TRUE\n");
				return TRUE;
			}
		}
		Clause ss;
		BOOL bValidSubject=FALSE;
		DWORD dwEndOfSubject=0;
		// Bsp.: She likes the school her brother hates.
		TRACE("Bsp.: She likes the school her brother hates.\n");
		for(dwRun=dwStart;dwRun<dwEnd;dwRun++)
		{ // Bsp.: She 
			PositionCStringVector svClause=GetBetween(psv,dwStart,dwRun);
			Subject subject;
			std::vector<Range> vecRange;
			if(ParseSubject(subject,svClause,FALSE,vecRange,TRUE,FALSE))
			{ // ArticleObject: Artikel und Nomen
				bValidSubject=TRUE;
				dwEndOfSubject=dwRun;
				*ss.m_pSubject=subject;
			}
		}
		DWORD dwEndOfValidMiddle=0;
		if(bValidSubject)
		{ // Bsp.: likes
			#ifdef _DEBUG
			printf("dwEndOfSubject\n");
			#endif
			TRACE("dwEndOfSubject\n");
			if(dwEndOfSubject+1<psv.size())
			{
				for(dwRun=dwEndOfSubject+1;dwRun<dwEnd;dwRun++)
				{
					if(ParseMiddle(ss,TRUE,GetBetweenAsCStringVector(psv,
						dwEndOfSubject+1,dwRun),1,TRUE))
					{
						dwEndOfValidMiddle=dwRun;
					}
				}
			}
		}
		else
			TRACE("! dwEndOfClause\n");
		DWORD dwEndOfValidObject=0;
		if(dwEndOfValidMiddle)
		{
			#ifdef _DEBUG
			printf("dwEndOfValidMiddle\n");
			#endif
			ss.m_pObjectFirst->m_vecEnumerationElement.push_back(EnumerationElement());
			for(dwRun=dwEndOfValidMiddle+1;dwRun<dwEnd;dwRun++)
			{ // Bsp.: the school
				DefiniteArticleNoun dan;
				BYTE personIndex=0;
			}
		}
		if(dwEndOfValidObject)
		{
			#ifdef _DEBUG
			printf("dwEndOfValidObject\n");
			#endif
			RelativeClause rc;
			if(ParseRelativeClause(rc,GetBetweenAsCStringVector(psv,
				dwEndOfValidObject+1,dwEnd)))
			{ // Bsp.: her brother hates
				if(ss.m_pRelativeClause)
					delete ss.m_pRelativeClause;
				ss.m_pRelativeClause=new RelativeClause;
				*ss.m_pRelativeClause=rc;
				ss.m_pRelativeClause->m_bRefersToSubject=FALSE; // relativsatz bezieht sich auf Object
				sentence.m_vecClause.push_back(ss);
#ifdef _DEBUG
				printf("ParseSentence - return TRUE\n");
#endif
				return TRUE;
			}
		}
		Subject * subject=new Subject;
		RelativeClause * rc=new RelativeClause;
		/*DWORD*/ dwEndOfSubject=0;
		DWORD dwEndOfRelativeClause=0;
		bValidSubject=FALSE;
		BOOL bValidRelativeClause=FALSE;
		// Bsp.: The school she likes is great.
		TRACE("Bsp.: The school she likes is great.\n");
		#ifdef _DEBUG
		printf("Bsp.: The school she likes is great.\n");
		#endif
		{
		Clause ss;
		ss.m_bWordOrder=SPO;
		ss.m_pSubject->m_vecEnumerationElement.push_back(EnumerationElement());
		for(dwRun=dwStart;dwRun<dwEnd;dwRun++)
		{ // Bsp.: her brother hates
			PositionCStringVector svClause=GetBetween(psv,dwStart,dwRun);
			//EnglishNounRecord enr;
			DefiniteArticleNoun dan;
			BYTE personIndex=0;
		}
		if(bValidSubject)
		{
			TRACE("bValidSubject\n");
			#ifdef _DEBUG
			printf("valid subject!\n");
			#endif
			for(dwRun=dwEndOfSubject+1;dwRun<dwEnd;dwRun++)
			{
				CStringVector svClause=GetBetweenAsCStringVector(psv,
					dwEndOfSubject+1,dwRun);
				RelativeClause rc;
				if(ParseRelativeClause(rc,svClause))
				{ // Bsp.: she likes 
					if(ss.m_pRelativeClause)
						delete ss.m_pRelativeClause;
					ss.m_pRelativeClause=new RelativeClause;
					*ss.m_pRelativeClause=rc;
					bValidRelativeClause=TRUE;
					dwEndOfRelativeClause=dwRun;
				}
			}
		}
		BOOL bValidMiddle=FALSE;
		//DWORD
		dwEndOfValidMiddle=0;
		DWORD dwEndOfValidSentenceElement=0;
		if(bValidRelativeClause)
		{
			#ifdef _DEBUG
			printf("valid relative clause!\n");
			#endif
			TRACE("bValidRelativeClause\n");
			ClauseWithoutSubject * cws=new ClauseWithoutSubject;
			//CStringVector svClause=GetBetween(sv,dwEndOfRelativeClause+1,dwEnd);
			for(dwRun=dwEndOfRelativeClause+1;dwRun<dwEnd;dwRun++)
			{
				if(ParseMiddle(ss,TRUE,GetBetweenAsCStringVector(psv,
					dwEndOfRelativeClause+1,dwRun),1,TRUE))
				{ // Bsp.: is
					bValidMiddle=TRUE;
					dwEndOfValidMiddle=dwRun;
					dwEndOfValidSentenceElement=dwRun;
				}
			}
			if(bValidMiddle && dwEndOfValidMiddle+1<dwEnd)
			{
				#ifdef _DEBUG
				printf("valid middle!\n");
				#endif
				for(DWORD dwRun;dwRun<dwEnd;dwRun++)
				{
					Object object;
					/*if(ParseObject(object,GetBetween(psv,dwEndOfValidMiddle+1,dwRun),TRUE,TRUE))
					{ // Bsp.: great
						#ifdef _DEBUG
						printf("valid object!\n");
						#endif
						dwEndOfValidSentenceElement=dwRun;
						ss.m_Object=object;
						TRACE("ParseSentence ENDE - return TRUE\n");
						ss.m_rc->m_bRefersToSubject=TRUE;
						sentence.m_subSentences.push_back(ss);
						return TRUE;
					}*/
				}
				Manner manner;
        LOG("23.05.2008 11.14.21\n");//BParseManner=Before ParseManner
				if(ParseManner(manner,GetBetween(psv,dwEndOfValidSentenceElement+1,
					dwEnd),TRUE,TRUE))
				{ // Bsp.: great
					#ifdef _DEBUG
					printf("valid object!\n");
					#endif
					if(ss.m_pManner)
						delete ss.m_pManner;
					ss.m_pManner=new Manner;
					*ss.m_pManner=manner;
					dwEndOfValidSentenceElement=dwEnd;
					TRACE("ParseSentence ENDE - return TRUE\n");
					ss.m_pRelativeClause->m_bRefersToSubject=TRUE;
					sentence.m_vecClause.push_back(ss);
#ifdef _DEBUG
					printf("ParseSentence() - return TRUE\n");
#endif
					return TRUE;
				}
				if(dwEndOfValidSentenceElement==dwEnd)
				{
					ss.m_pRelativeClause->m_bRefersToSubject=TRUE;
					sentence.m_vecClause.push_back(ss);
#ifdef _DEBUG
					printf("ParseSentence ENDE - return TRUE\n");
#endif
					return TRUE;
				}
			}
			else
			{
				ss.m_pRelativeClause->m_bRefersToSubject=TRUE;
				sentence.m_vecClause.push_back(ss);
				#ifdef _DEBUG
				printf("ParseSentence ENDE - return TRUE\n");
				#endif
				return TRUE;
			}
		}
		}
	}
	if(!bClause)
	{
		TRACE("ParseSentence ENDE - return FALSE\n");
		return FALSE;
	}
	#ifdef _DEBUG
	printf("rVecRange.size(): %u\n",rVecRange.size());
	printf("ParseSentence ENDE - return TRUE\n");
	#endif
	return TRUE;
}//ParseSentence

//überprüft "as ... as"
BYTE ParseSimilarity(Similarity & similarity,
	const PositionCStringVector & psv,BOOL bBe,const DWORD & dwFlags)
{
	Object object;
	std::vector<Range> vecRange;
	WordNode * pWordNode=NULL;
	if(psv.size()<4)
		return 0;
	if(psv.at(0).m_Str!=_T("as") || psv.at(2).m_Str!=_T("as"))
		return 0;
	if(!(pWordNode=GetPointerToWordNodeWhereAnEnglishPositiveIsEncapsulated(
		psv.at(1).m_Str,dwFlags)))
		return 0;
	similarity.m_pWordNodeWhereAnEnglishPositiveIsEncapsulated=pWordNode;
	if(!ParseObject(object,GetBetween(psv,3,psv.size()),bBe,vecRange,dwFlags))
		return 0;
	similarity.m_pObject=new Object(object);
	return 1;
}

//liefert 0 zurück, wenn das/die übergebene(n) Token kein grammatikalisch
//korrektes Subjekt ergibt/ergeben, 1 wenn das/die übergebene(n) Token ein 
//grammatikalisch korrektes Subjekt ergibt/ergeben und kein(e) Eigennamen
//beinhaltet/beinhalten, 2 wenn das/die übergebene(n) Token ein 
//grammatikalisch korrektes Subjekt ergibt/ergeben und mindestens einen 
//vermeintlichen Eigennamen beinhaltet/beinhalten
//bContainsFirstTokenOfSentence: gibt an, ob psv das erste Token im Satz 
//beinhaltet. bContainsFirstTokenOfSentence existiert, damit Eigennamen, die
//am Anfang eines Satzes stehen, in der Übersetzung nicht immer mit einem
//Kleinbuchstaben beginnen
//diese Funktion speichert in der Membervariable "m_personIndex"  des Objekts
//"subject", um welchen Index es sich handelt. Beispiel: Subjekt: "I"
//"m_personIndex": 1
BOOL ParseSubject(Subject & subject,const PositionCStringVector & psv,
	BOOL bExcludeGerund,std::vector<Range> & rVecRange,const DWORD & 
	dwFlags,BYTE bContainsFirstTokenOfSentence)
{
	BOOL bAnd=FALSE;
	BOOL bColon=FALSE;
	BOOL bReturn=1;
	BYTE personIndex=0;
	int first=0;
	TRACE("BOOL ParseSubject(Subject & subject,const CStringVector sv) - ANFANG\n");
#ifdef _DEBUG
	printf("BOOL ParseSubject(Subject & subject,const PositionCStringVector &\n\
psv,BOOL bExcludeGerund,std::vector<Range> & rVecRange,const DWORD & \n\
dwFlags,BYTE bContainsFirstTokenOfSentence(%u)) - ANFANG\n",
bContainsFirstTokenOfSentence);
	printf("g_bContinue: %u\n",g_bContinue);
	fflush(stdout);
#endif
	if(psv.size()<1)//wenn kein Token, dann ist es naturlich KEIN Subjekt
	{
#ifdef _DEBUG
		printf("BOOL ParseSubject(Subject & subject,const PositionCStringVector\n\
& psv) - return 0 - psv.size()<1\n");
		fflush(stdout);
#endif
		return 0;
	}
	subject.m_vecEnumerationElement.clear();
	#ifdef _DEBUG
	for(DWORD i2=0;i2<psv.size();i2++)
	{
		printf("psv.at(%d).m_Str: %s\n",i2,psv.at(i2).m_Str);
	}
	#endif
	//Subject subject;
	for(DWORD i=0;i<psv.size() && g_bContinue;i++)
	{
#ifdef _WINDOWS
		TRACE("psv.at(%d): %s\n",i,psv.at(i).m_Str);
#endif
		if(psv.at(i).m_Str==",")
		{
			if(bAnd)
			{
				TRACE("ParseSubject ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("BOOL ParseSubject(Subject & subject,const PositionCStringVector & psv) - ENDE - return FALSE\n");
				#endif
				return FALSE;
			}
			bColon=TRUE;
#ifdef _WINDOWS
			TRACE("first: %d,i: %d\n",first,i);
#endif
			PositionCStringVector eeVec=GetBetween(psv,first,i-1);
			EnumerationElement ee;
			std::vector<Range> vecRange;
			BYTE ret=0;
			if(!(ret=ParseEnumerationElement(ee,eeVec,bExcludeGerund,vecRange,
				dwFlags,bContainsFirstTokenOfSentence)))
			{
				TRACE("ParseSubject ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("BOOL ParseSubject(Subject & subject,const PositionCStringVector & psv) - ENDE - return FALSE\n");
				#endif
				return FALSE;
			}
			//if(vecRange.size())
			//	rVecRange.insert(rVecRange.end()-1,vecRange.begin(),vecRange.end()-1);
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
			personIndex=GetPersonIndex(personIndex,ret);
			first=i+1;
			//ret bitweise-Und 128: wenn das Ergebnis 128 ist, dann ist das 
			//Aufzählungselement (EnumerationElement) ein Eigenname
			if((ret&=128)==128)
				bReturn=2;
			subject.m_vecEnumerationElement.push_back(ee);
		}
		if(psv.at(i).m_Str==_T("and")) // wenn die englische Konjunktion 'and'
		{ 
			TRACE("and\n");
			if(bAnd)
			{
				TRACE("ParseSubject ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("BOOL ParseSubject(Subject & subject,const PositionCStringVector & psv) - ENDE - return FALSE\n");
				#endif
				return FALSE;
			}
			bAnd=TRUE;
#ifdef _WINDOWS
			TRACE("first: %d,i: %d\n",first,i);
#endif
			PositionCStringVector eeVec=GetBetween(psv,first,i-1);
			EnumerationElement ee;
			std::vector<Range> vecRange;
			BYTE ret=0;
			if(!(ret=ParseEnumerationElement(ee,eeVec,bExcludeGerund,vecRange,
				dwFlags,bContainsFirstTokenOfSentence)))
			{
				TRACE("ParseSubject ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("BOOL ParseSubject(Subject & subject,const PositionCStringVector & psv) - ENDE - return FALSE\n");
				#endif
				return FALSE;
			}
			//if(vecRange.size())
			//	rVecRange.insert(rVecRange.end()-1,vecRange.begin(),vecRange.end()-1);
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
			personIndex=GetPersonIndex(personIndex,ret);
			first=i+1;
			//ret bitweise-Und 128: wenn das Ergebnis 128 ist, dann ist das 
			//Aufzählungselement (EnumerationElement) ein Eigenname
			if((ret&=128)==128)
				bReturn=2;
			subject.m_vecEnumerationElement.push_back(ee);
		}
		if(i==psv.size()-1) // wenn das Ende des Vektors erreicht wird
		{
		//In MFC/Windows the TRACE macro has a variable # of args--I do 
		//not know how to enable variable numbers of args for the same macro name.
		//So I exclude it from non-Windows.
#ifdef _WINDOWS
			TRACE("(i+1)==psv.size()\n");
			TRACE("first: %d,i: %d\n",first,i);
#endif//#ifdef _WINDOWS
//			TRACE("sv.at(4): %s\n",sv.at(4));
			PositionCStringVector eeVec=GetBetween(psv,first,i);
			EnumerationElement ee;
			std::vector<Range> vecRange;
			BYTE ret=0;
			if(!(ret=ParseEnumerationElement(ee,eeVec,bExcludeGerund,vecRange,
				dwFlags,bContainsFirstTokenOfSentence)))
			{
				TRACE("ParseSubject ENDE - return FALSE\n");
				#ifdef _DEBUG
				printf("BOOL ParseSubject(Subject & subject,const CStringVector sv) - ENDE - return FALSE\n");
				#endif
				return FALSE;
			}
			#ifdef _DEBUG
			printf("vecRange.size(): %u\n",vecRange.size());
			#endif
			//if(vecRange.size())
			//	rVecRange.insert(vecRange.begin(),rVecRange.end(),rVecRange.end());
			for(DWORD dwRun=0;dwRun<vecRange.size();dwRun++)
				rVecRange.push_back(vecRange.at(dwRun));
			#ifdef _DEBUG
			printf("rVecRange.size(): %u\n",rVecRange.size());
			#endif
			subject.m_vecEnumerationElement.push_back(ee);
			if(personIndex!=0)
			{
				personIndex=GetPersonIndex(personIndex,ret);
				subject.m_personIndex=personIndex;
			}
			else
			//den Rückgabewert von ParseEnumerationElement bitweise-Oder mit
			//191 verknüpfen, um das 8. Bit, das angibt, ob sich im
			//Aufzählungselement (EnumerationElement) ein Eigenname befindet,
			//außer Acht zu lassen
				subject.m_personIndex=ret&127; //191 binär: 01111111
			//subject.personIndex=personIndex;
			//ret bitweise-Und 128: wenn das Ergebnis 128 ist, dann ist das 
			//Aufzählungselement (EnumerationElement) ein Eigenname
			if((ret&=ENUMERATION_ELEMENT_CONTAINS_PROPER_NAME)==
        ENUMERATION_ELEMENT_CONTAINS_PROPER_NAME)
				bReturn=2;
			//TRACE("ParseSubject ENDE - return TRUE\n");
			//return TRUE;
		}
	}
	if(bColon && !bAnd)
	{
		TRACE("ParseSubject ENDE - return FALSE\n");
		#ifdef _DEBUG
		printf("BOOL ParseSubject(Subject & subject,const CStringVector sv) - ENDE - \
return FALSE\n");
		#endif
		return FALSE;
	}
	//wenn das Subjekt ein Eigenname ist
	if(bReturn==2 && subject.m_vecEnumerationElement.size()==1)
	{
		if(subject.m_vecEnumerationElement.at(0).m_vecStrPlainText.size()>0)
		{
		//dann alle Objekte (also Klasseninstanzen) löschen, da sonst dieser
		//Effekt auftreten kann: "The number yfou have dialled is incomplete."
		//-> The number yfou have dialled Die Nummer ist unvollständig.
		//(es wurde ein Substantiv mit einem bestimmten Artikel erkannt,
		//daher das Objekt "m_pEnr" mit dem Schlüsselwort "new" dynamisch erstellt
		//aber nicht gelöscht, als fest stand, dass "yfou have dialled" kein
		//grammatikalisch korrekter Relativsatz ist. Daher wurde "m_pEnr" mit
		//übersetzt)
		for(DWORD dwRun=0;dwRun<subject.m_vecEnumerationElement.size();dwRun++)
		{
			if(subject.m_vecEnumerationElement.at(dwRun).m_pDan)
			{
				delete subject.m_vecEnumerationElement.at(dwRun).m_pDan;
				subject.m_vecEnumerationElement.at(dwRun).m_pDan=NULL;
			}
			if(subject.m_vecEnumerationElement.at(dwRun).m_pEnr)
			{
				delete subject.m_vecEnumerationElement.at(dwRun).m_pEnr;
				subject.m_vecEnumerationElement.at(dwRun).m_pEnr=NULL;
			}
			if(subject.m_vecEnumerationElement.at(dwRun).m_pGerund)
			{
				delete subject.m_vecEnumerationElement.at(dwRun).m_pGerund;
				subject.m_vecEnumerationElement.at(dwRun).m_pGerund=NULL;
			}
			if(subject.m_vecEnumerationElement.at(dwRun).m_pOfPhrase)
			{
				delete subject.m_vecEnumerationElement.at(dwRun).m_pOfPhrase;
				subject.m_vecEnumerationElement.at(dwRun).m_pOfPhrase=NULL;
			}
			if(subject.m_vecEnumerationElement.at(dwRun).m_pRelativeClause)
			{
				delete subject.m_vecEnumerationElement.at(dwRun).m_pRelativeClause;
				subject.m_vecEnumerationElement.at(dwRun).m_pRelativeClause=NULL;
			}
			if(subject.m_vecEnumerationElement.at(dwRun).m_pToPlusInfinitive)
			{
				delete subject.m_vecEnumerationElement.at(dwRun).m_pToPlusInfinitive;
				subject.m_vecEnumerationElement.at(dwRun).m_pToPlusInfinitive=NULL;
			}
		}
		}
	}
#ifdef _DEBUG
	if(subject.m_vecEnumerationElement.size()>0)
		if(subject.m_vecEnumerationElement.at(0).m_pOfPhrase)
			printf("subject.m_vecEnumerationElement.at(0).m_pOfPhrase->m_Enr.\
m_Noun.size(): %u\n",subject.m_vecEnumerationElement.at(0).\
m_pOfPhrase->m_Enr.m_Noun.size());
	TRACE("ParseSubject ENDE - return TRUE\n");
	//subject.m_personIndex=personIndex;
	printf("rVecRange.size(): %u\n",rVecRange.size());
	printf("subject.m_personIndex: %u\n",subject.m_personIndex);
	printf("BOOL ParseSubject(Subject & subject,const PositionCStringVector\n\
& psv,BOOL bExcludeGerund,std::vector<Range> & rVecRange,const DWORD & \n\
dwFlags,BYTE bContainsFirstTokenOfSentence) - return bReturn (%d)\n",bReturn);
#endif
	return bReturn;
}//ParseSubject

BYTE ParseToPlusInfinitive(ToPlusInfinitive & toPlusInfinitive,
	const PositionCStringVector & psv,std::vector<Range> & rVecRange,
	const DWORD & dwFlags,BYTE bFirstTokenOfSentence)
{
	#ifdef _DEBUG
	printf("BYTE ParseToPlusInfinitive(ToPlusInfinitive & toPlusInfinitive,const PositionCStringVector & psv,std::vector<Range> & rVecRange) ANFANG\n");
	for(DWORD i=0;i<psv.size();i++)
		printf("psv.at(%d).m_Str: %s\n",i,psv.at(i).m_Str);
	#endif
	BOOL bInfinitive=FALSE;
	BOOL bPastParticiple=FALSE;
	BOOL bTo=FALSE;
	BYTE returnValue=2;
	CString strProgressive;
	DWORD dwStartOfNextElement=0;
	DWORD dwStartOfNegationOrProgressive=0;
	if(psv.size()<=0)
		return 0;
	WordNode * pWordNode=NULL;
	if(psv.size()>dwStartOfNextElement)
		if(psv.at(dwStartOfNextElement).m_Str==_T("not"))
		{
			toPlusInfinitive.m_bNegation=TRUE;
			dwStartOfNegationOrProgressive=dwStartOfNextElement++;
		}
	if(psv.size()>dwStartOfNextElement)
		if(psv.at(dwStartOfNextElement).m_Str==_T("to"))
		{
			dwStartOfNextElement++;
			bTo=TRUE;
		}
	if(!bTo)
	{
		#ifdef _DEBUG
		printf("BYTE ParseToPlusInfinitive(ToPlusInfinitive & toPlusInfinitive,\
const PositionCStringVector & psv,std::vector<Range> & rVecRange) ENDE \
return 0 - 'to' fehlt\n");
		#endif
		return 0;
	}
	//EnglishVerb ev;
	if(psv.size()>dwStartOfNextElement)
	{
		if((pWordNode=GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
			psv.at(dwStartOfNextElement).m_Str))) //IsPresentProgressive(sv.at(0)))
		{
			toPlusInfinitive.m_pWordNodeWhereAnEnglishVerbIsEncapsulated=pWordNode;
			if(typeid(*pWordNode)==typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb *>
					(pWordNode->m_pWord);
				if(eav)
					if(eav->m_strWords[0]==psv.at(dwStartOfNextElement).m_Str)
						bInfinitive=TRUE;
			}
			if(typeid(*pWordNode)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb *>(pWordNode->m_pWord);
				if(ev)
					if(ev->m_strInfinitive==psv.at(dwStartOfNextElement).m_Str)
						bInfinitive=TRUE;
			}
		}
	}
	if(!bInfinitive)
	{
		#ifdef _DEBUG
		printf("BYTE ParseToPlusInfinitive(ToPlusInfinitive & toPlusInfinitive,\
const PositionCStringVector & psv,std::vector<Range> & rVecRange) ENDE \
return 0 - Infinitiv fehlt\n");
		#endif
		return 0;
	}
	dwStartOfNextElement++;
//				dwStartOfNextElement++;
	if(typeid(*toPlusInfinitive.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
		m_pWord)==typeid(EnglishVerb) && psv.size()>dwStartOfNextElement)
	{
		#ifdef _DEBUG
		printf("Präposition testen\n");
		#endif
		EnglishVerb * ev=dynamic_cast<EnglishVerb *>(toPlusInfinitive.
			m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWord);
		if(ev->m_strPreposition!="")
		{
			BOOL bValidPreposition=FALSE;
			CStringVector svPreposition=GetCStringVector2(ev->m_strPreposition);
			DWORD dwStart=0;
			for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size() && g_bContinue;dwRun++)
			{
				if(GetBetweenAsCStringVector(psv,dwStart,dwRun)==svPreposition)
				{
					dwStartOfNextElement=dwRun+1;
					bValidPreposition=TRUE;
				}
			}
			if(!bValidPreposition)
			{
				#ifdef _DEBUG
				printf("BYTE ParseToPlusInfinitive(ToPlusInfinitive & toPlusInfinitive,\
const PositionCStringVector & psv,std::vector<Range> & rVecRange) return \
FALSE - keine Präposition\n");
				#endif
				return FALSE;
			}
		}
	}
	if(psv.size()>dwStartOfNextElement)
	{
		#ifdef _DEBUG
		printf("Objekt testen\n");
		#endif
		if(toPlusInfinitive.m_pWordNodeWhereAnEnglishVerbIsEncapsulated)
			if(toPlusInfinitive.m_pWordNodeWhereAnEnglishVerbIsEncapsulated->
				m_pWordNodeNext)
			{
				GermanVerb * gv=dynamic_cast<GermanVerb *>(toPlusInfinitive.
					m_pWordNodeWhereAnEnglishVerbIsEncapsulated->m_pWordNodeNext->m_pWord);
				//GermanVerb * gv=GetGermanVocableOfEnglishVerb(ev.infinitive);
#ifdef _DEBUG
				printf("gv->m_bCase: %u\n",gv->m_bCase);
#endif
				if(gv->m_bCase==1 || gv->m_bCase==2)
				{
					DWORD dwEndOfValidObject=0;
					std::vector<Range> vecRangeOfValidObject;
					BOOL bValidObject=FALSE;
					for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size() && 
						g_bContinue;dwRun++)
					{
						Object object;
						std::vector<Range> vecRange;
						if((returnValue=ParseObject(object,GetBetween(psv,
							dwStartOfNextElement,dwRun),TRUE,vecRange,TRUE)))
						{
							vecRangeOfValidObject=vecRange;
							if(toPlusInfinitive.m_pObject)
								delete toPlusInfinitive.m_pObject;
							toPlusInfinitive.m_pObject=new Object(object);
							dwEndOfValidObject=dwRun;
							bValidObject=TRUE;
						}
					}
					if(bValidObject)
						dwStartOfNextElement=dwEndOfValidObject+1;
					if(dwEndOfValidObject==psv.size()-1)
					{
						for(DWORD dwRun=0;dwRun<vecRangeOfValidObject.size();dwRun++)
							rVecRange.push_back(vecRangeOfValidObject.at(dwRun));
						#ifdef _DEBUG
						printf("rVecRange.size(): %u\n",rVecRange.size());
						printf("BYTE ParseToPlusInfinitive(ToPlusInfinitive & \
toPlusInfinitive,const PositionCStringVector & psv,std::vector<Range> & \
rVecRange) ENDE return returnValue\n");
						#endif
						return returnValue;
					}
				}
			}
		//)==typeid
	}
	else
	{
		#ifdef _DEBUG
		printf("BYTE ParseToPlusInfinitive(ToPlusInfinitive & toPlusInfinitive,\
const PositionCStringVector & psv,std::vector<Range> & rVecRange) ENDE \
return returnValue (%u)\n",returnValue);
		#endif
		return returnValue;
	}
	if(psv.size()>dwStartOfNextElement) // ein Gerundium kann auch
	{ // eine Umstandsbestimmung des Ortes beinhalten
		// zum Beispiel: "I like living in Berlin." 
		// Gerundium: "living in Berlin"
		// Umstandsbestimmung des Ortes: "in Berlin"
		BOOL bValidPlace=FALSE;
		DWORD dwEndOfValidPlace=0;
		std::vector<Range> vecRangeOfValidPlace;
		BYTE returnValueOfValidPlace=0;
		for(DWORD dwRun=dwStartOfNextElement;dwRun<psv.size() && g_bContinue;dwRun++)
		{
			Place place;
			std::vector<Range> vecRange;
			BYTE ret=0;
			if((ret=ParsePlace(place,GetBetween(psv,dwStartOfNextElement,dwRun),vecRange,TRUE)))
			{
				vecRangeOfValidPlace=vecRange;
				if(toPlusInfinitive.m_pPlace)
					delete toPlusInfinitive.m_pPlace;
				toPlusInfinitive.m_pPlace=new Place(place);
				dwEndOfValidPlace=dwRun;
				bValidPlace=TRUE;
				returnValueOfValidPlace=ret;
			}
		}
		if(bValidPlace)
		{
			if(returnValueOfValidPlace==1)
				returnValue=1;
			dwStartOfNextElement=dwEndOfValidPlace+1;
			for(DWORD dwRun=0;dwRun<vecRangeOfValidPlace.size();dwRun++)
				rVecRange.push_back(vecRangeOfValidPlace.at(dwRun));
		}
		if(dwStartOfNextElement==psv.size())
		{
			#ifdef _DEBUG
			printf("rVecRange.size(): %u\n",rVecRange.size());
			printf("BYTE ParseToPlusInfinitive(ToPlusInfinitive & toPlusInfinitive,const PositionCStringVector & psv,std::vector<Range> & rVecRange) ENDE return returnValue\n");
			#endif
			return returnValue;
		}
	}
	#ifdef _DEBUG
	printf("BYTE ParseToPlusInfinitive(ToPlusInfinitive & toPlusInfinitive,const PositionCStringVector & psv,std::vector<Range> & rVecRange) ENDE return FALSE\n");
	#endif
	return FALSE;
}//ParseToPlusInfinitive

BYTE PredicateAllowsGerund(const Predicate & predicate)
{
#ifdef _DEBUG
	printf("BYTE PredicateAllowsGerund(const Predicate & predicate) - ANFANG\n");
#endif
	if(predicate.m_pWordNodeWhereTheGermanTranslationIsANonBendVerb)
	{
		//wenn ein Infinitiv im Prädikat ist, ist dieses auf jeden Fall
		//ein Vollverb
		EnglishVerb * ev=dynamic_cast<EnglishVerb *>(predicate.
			m_pWordNodeWhereTheGermanTranslationIsANonBendVerb->m_pWord);
		if(ev)
			if(ev->m_bAllowsIngForm)
#ifdef _DEBUG
			{
printf("BYTE PredicateAllowsGerund(const Predicate & predicate) - return 1\n");
#endif
				return 1;
#ifdef _DEBUG
			}
#endif
	}
	else
	{
		if(predicate.m_pWordNodeWhereTheGermanTranslationIsABendVerb)
			if(typeid (predicate.
				m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord)==
				typeid(EnglishVerb *))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb *>(predicate.
					m_pWordNodeWhereTheGermanTranslationIsABendVerb->m_pWord);
				if(ev)
					if(ev->m_bAllowsIngForm)
	#ifdef _DEBUG
					{
	printf("BYTE PredicateAllowsGerund(const Predicate & predicate) - return 1\n");
	#endif
						return 1;
	#ifdef _DEBUG
					}
	#endif
			}
	}
#ifdef _DEBUG
	printf("BYTE PredicateAllowsGerund(const Predicate & predicate) - return 0\n");
#endif
	return 0;
}
