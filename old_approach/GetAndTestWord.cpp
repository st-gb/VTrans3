//Ohne das folgende include: fatal error C1010: Unerwartetes Dateiende während der Suche nach dem vorkompilierten Header.
#include "StdAfx.h"
#include "GetAndTestWord.h" //for IsConsonant(...) etc.
#include "VocabularyInMainMem/DoublyLinkedList/WordNode.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"
#include "rest.h" //for CStringVector, #defines
#include "Token.h" //for GetCStringVector2 etc.
#include "Word.hpp"
#include <vector>
#include <math.h> //for pow()

extern WordList wordList ;

CString GetArticle(GermanNoun * gn)
{
	if(gn->m_bArticle=='1')
		return "der";
	if(gn->m_bArticle=='2')
		return "die";
	if(gn->m_bArticle=='3')
		return "das";
	return "";
}

EnglishAuxiliaryVerb * GetEnglishAuxiliaryVerb(CString & str)
{
	TRACE("EnglishAuxiliaryVerb * GetEnglishAuxiliaryVerb(CString & str) - ANFANG\n");
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			//TRACE("in Schleife\n");
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
				TRACE("englisches Nomen\n");
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				TRACE("EnglishAuxiliaryVerb!\n");
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					for(int i=0;i<15;i++)
					{
						//TRACE("eav->words[i]: %s\n",eav->words[i]);
						if(str==eav->m_strWords[i])
						{
							TRACE("EnglishAuxiliaryVerb * GetEnglishAuxiliaryVerb(CString & str) - ENDE - return eav\n");
							return eav;
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("EnglishAuxiliaryVerb * GetEnglishAuxiliaryVerb(CString & str) - ENDE - return NULL\n");
	return NULL;
}

EnglishNoun * GetEnglishNoun(CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					if(str==en->m_strSingular || str==en->m_strPlural)
					{
						return en;
					}
				}
			}
			if(current->m_pWordNodeNext=NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;	
}

std::vector <EnglishNoun> GetEnglishNoun(const CStringVector & svNoun)
{
	std::vector <EnglishNoun> vecEnglishNoun;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					CStringVector cstrvecEN=GetCStringVector2(en->m_strSingular);
					if(Compare(svNoun,cstrvecEN))
					{
						vecEnglishNoun.push_back(*en);
					}
					cstrvecEN=GetCStringVector2(en->m_strPlural);
					if(Compare(svNoun,cstrvecEN))
					{
						vecEnglishNoun.push_back(*en);
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return vecEnglishNoun;
}

EnglishVerb * GetEnglishVerb(CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * pWordNodeCurrent=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*pWordNodeCurrent->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(pWordNodeCurrent->m_pWord);
				if(ev!=NULL)
				{
					if(str==ev->m_strInfinitive || str==ev->m_strInfinitive+_T("s") || 
						str==ev->m_strPastTense || str==ev->m_strPastParticiple)
					{
						return ev;
					}
				}
			}
			if(pWordNodeCurrent->m_pWordNodeNext==NULL)
				break;
			else
				pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
		}
	}
	return NULL;	
}

CString GetGermanNoun(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					if(str==en->m_strSingular)
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanNoun))
							{
								GermanNoun * gn=dynamic_cast<GermanNoun*>(current->m_pWord);
								if(gn!=NULL)
								{
									return gn->m_strSingular;
								}
							}
						}
					}
					if(str==en->m_strPlural)
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanNoun))
							{
								GermanNoun * gn=dynamic_cast<GermanNoun*>(current->m_pWord);
								if(gn!=NULL)
								{
									return gn->m_strPlural;
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return "";
}

/*GermanNoun * gn=GetGermanNounOfPointerToEnglishNoun(EnglishNoun * pEN)
{
	if(pEN->m_
}*/

EnglishAdverb GetEnglishAdverb(CString str)
{
	EnglishAdverb * ea;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				ea=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(ea!=NULL)
				{
					if(str==ea->m_strWord)
					{
						return *ea;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	ea=new EnglishAdverb;
	return *ea;
}

Word * GetGermanAdjectiveOrGermanAdverbOfEnglishAdverb(const CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective)) // das Adverb der
			{ // Art und Weise wird in einem Object der Klasse EnglishAdjective
				// gespeichert (Member-Variable 'CString adverb'), und nicht in
				// einem Objekt der Klasse EnglishAdverb
				// Deswegen müssen wir auch die englischen Adjective nach einem
				// Adverb durchsuchen
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
				if(ea!=NULL)
				{
					if(str==ea->m_strAdverb)
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanAdjective))
							{
								GermanAdjective * ga=dynamic_cast<GermanAdjective*>
									(current->m_pWord);
								if(ga!=NULL)
								{
									return ga;
								}
							}
						}
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * ea=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(ea!=NULL)
				{
					if(str==ea->m_strWord)
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanAdverb))
							{
								GermanAdverb * ga=dynamic_cast<GermanAdverb*>(current->m_pWord);
								if(ga!=NULL)
								{
									return ga;
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;
}

CString GetGermanAdverb(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord)
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanAdverb))
							{
								GermanAdverb * gav=dynamic_cast<GermanAdverb*>
									(current->m_pWord);
								if(gav!=NULL)
								{
									return gav->m_strWord;
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return "";
}

GermanNoun * GetGermanWordOfEnglishNoun(const CStringVector & cstrvec)
{
	GermanNoun * gn;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					CStringVector cstrvecEN=GetCStringVector2(en->m_strSingular);
					//TRACE("cstrvecEN.size(): %d cstrvec.size(): %d\n",cstrvecEN.size(),cstrvec.size());
					if(Compare(cstrvec,cstrvecEN))
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanNoun))
							{
								gn=dynamic_cast<GermanNoun*>(current->m_pWord);
								if(gn!=NULL)
								{
									//TRACE("gn->article: %c\n",gn->article);
									//TRACE("GermanNoun * GetGermanVocableOfEnglishNoun(const CStringVector & cstrvec) ENDE\n");
									return gn;
								}
							}
						}
					}
					cstrvecEN=GetCStringVector2(en->m_strPlural);
					if(Compare(cstrvec,cstrvecEN))
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanNoun))
							{
								gn=dynamic_cast<GermanNoun*>(current->m_pWord);
								if(gn!=NULL)
								{
									//TRACE("GermanNoun * GetGermanVocableOfEnglishNoun(const CStringVector & cstrvec) ENDE\n");
									return gn;
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	gn=new GermanNoun();
	return gn;
}

std::vector <GermanNoun> GetGermanWordOfEnglishNoun2(const 
	CStringVector & cstrvec)
{
	std::vector <GermanNoun > vecGermanNoun;
	GermanNoun * gn;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					CStringVector cstrvecEN=GetCStringVector2(en->m_strSingular);
					//TRACE("cstrvecEN.size(): %d cstrvec.size(): %d\n",cstrvecEN.size(),cstrvec.size());
					if(Compare(cstrvec,cstrvecEN))
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanNoun))
							{
								gn=dynamic_cast<GermanNoun*>(current->m_pWord);
								if(gn!=NULL)
								{
									//TRACE("gn->article: %c\n",gn->article);
									//TRACE("GermanNoun * GetGermanVocableOfEnglishNoun(const CStringVector & cstrvec) ENDE\n");
									vecGermanNoun.push_back(*gn);
								}
							}
						}
					}
					cstrvecEN=GetCStringVector2(en->m_strPlural);
					if(Compare(cstrvec,cstrvecEN))
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanNoun))
							{
								gn=dynamic_cast<GermanNoun*>(current->m_pWord);
								if(gn!=NULL)
								{
									//TRACE("GermanNoun * GetGermanVocableOfEnglishNoun(const CStringVector & cstrvec) ENDE\n");
									//return gn;
									vecGermanNoun.push_back(*gn);
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	//gn=new GermanNoun();
	return vecGermanNoun;
	//return gn;
}

GermanNoun * GetGermanWordOfEnglishNoun(CString str)
{
	GermanNoun * gn;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					if(str==en->m_strSingular || str==en->m_strPlural)
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanNoun))
							{
								gn=dynamic_cast<GermanNoun*>(current->m_pWord);
								if(gn!=NULL)
								{
									return gn;
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	gn=new GermanNoun();
	return gn;
}

GermanVerb * GetGermanWordOfEnglishVerb(CString str)
{
	GermanVerb * gv;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(str==ev->m_strInfinitive || str==ev->m_strInfinitive+"s" || 
						str==ev->m_strPastTense || str==ev->m_strInfinitive+"ing")
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanVerb))
							{
								gv=dynamic_cast<GermanVerb*>(current->m_pWord);
								if(gv!=NULL)
								{
									return gv;
								}
							}
						}
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				TRACE("EnglishAuxiliaryVerb!\n");
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					for(int i=0;i<15;i++)
					{
						//TRACE("eav->words[i]: %s\n",eav->words[i]);
						if(str==eav->m_strWords[i])
						{
							if(current->m_pWordNodeNext!=NULL)
							{
								current=current->m_pWordNodeNext;
								if(typeid(*current->m_pWord)==typeid(GermanVerb))
								{
									gv=dynamic_cast<GermanVerb*>(current->m_pWord);
									if(gv!=NULL)
									{
										return gv;
									}
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;	
}

GermanAdjective * GetGermanWordOfEnglishAdjective(CString str)
{
	GermanAdjective * ga;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>
					(current->m_pWord);
				if(ea!=NULL)
				{
					if(str==ea->m_strPositiv || str==ea->m_strComperativ || 
						str==ea->m_strSuperlativ)
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanAdjective))
							{
								ga=dynamic_cast<GermanAdjective*>(current->m_pWord);
								if(ga!=NULL)
								{
									return ga;
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;
}

GermanAdverb * GetGermanWordOfEnglishAdverb(CString str)
{
	GermanAdverb * gav;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>
					(current->m_pWord);
				if(ea!=NULL)
				{
					if(str==ea->m_strPositiv+"ly")
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanAdjective))
							{
								GermanAdjective * ga=dynamic_cast<GermanAdjective*>
									(current->m_pWord);
								if(ga!=NULL)
								{
									gav=new GermanAdverb;
									gav->m_strWord=ga->m_strPositiv;
									return gav;
								}
							}
						}
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord)
					{
						if(current->m_pWordNodeNext!=NULL)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanAdverb))
							{
								gav=dynamic_cast<GermanAdverb*>(current->m_pWord);
								if(gav!=NULL)
								{
									return gav;
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;
}

GermanNoun * GetPointerToGermanNounFromWordNodeWhereAnEnglishNounIsEncapsulated(
	WordNode * pWordNode)
{
	if(pWordNode->m_pWordNodeNext)
	{
		if(typeid(*pWordNode->m_pWordNodeNext->m_pWord)==typeid(GermanNoun)) // wenn englisches Substantiv
		{
			return dynamic_cast<GermanNoun*>(pWordNode->m_pWordNodeNext->m_pWord);
		}
	}
	return NULL;
}

WordNode * GetWordNodeWhereAnEnglishPositiveIsEncapsulated(const CString & str) // Positiv von englischem Adjektiv
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>
					(current->m_pWord);
				if(ea!=NULL)
				{
					//TRACE("eav->word: %s   eav->type: %d  %u   %c\n",eav->word,eav->type,eav->type,eav->type);
					if(str==ea->m_strPositiv)
					{
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated(
	const CString & str,const DWORD & dwFlags)
{
#ifdef _DEBUG
	printf("VocNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - ANFANG\n");
	if((dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
		printf("(dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE\n");
	else
		printf("!(dwFlags&1)==1\n");
	printf("dwFlags: %u\n",dwFlags);
#endif
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>
					(current->m_pWord);
				if(ea!=NULL)
				{
					//TRACE("eav->word: %s   eav->type: %d  %u   %c\n",eav->word,eav->type,eav->type,eav->type);
					//wenn die Gross- und Kleinschreibung beachtet werden soll
					if((dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
					{
						if(str==ea->m_strPositiv || str==ea->m_strComperativ || 
							str==ea->m_strSuperlativ)
						{
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
					}
					else
					{
						CString str2=ea->m_strPositiv;
						str2.MakeLower();
						if(str==str2)
						{
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
						str2=ea->m_strComperativ;
						str2.MakeLower();
						if(str==str2)
						{
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
						str2=ea->m_strSuperlativ;
						str2.MakeLower();
						if(str==str2)
						{
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return NULL\n");
#endif
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEncapsulated(
	const CString & str,const DWORD & dwFlags,BYTE & bType)
{
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - ANFANG\n");
	if((dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
		printf("(dwFlags&1)==1\n");
	else
		printf("!(dwFlags&1)==1\n");
	printf("dwFlags: %u\n",dwFlags);
#endif
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>
					(current->m_pWord);
				if(ea!=NULL)
				{
					//TRACE("eav->word: %s   eav->type: %d  %u   %c\n",eav->word,eav->type,eav->type,eav->type);
					//wenn die Gross- und Kleinschreibung beachtet werden soll
					if((dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
					{
						if(str==ea->m_strPositiv)
						{
							bType=0;
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
						if(str==ea->m_strComperativ)
						{
							bType=1;
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
						if(str==ea->m_strSuperlativ)
						{
							bType=2;
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
					}
					else
					{
						CString str2=ea->m_strPositiv;
						str2.MakeLower();
						if(str==str2)
						{
							bType=0;
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
						str2=ea->m_strComperativ;
						str2.MakeLower();
						if(str==str2)
						{
							bType=1;
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
						str2=ea->m_strSuperlativ;
						str2.MakeLower();
						if(str==str2)
						{
							bType=2;
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishAdjectiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return NULL\n");
#endif
	return NULL;
}

WordNode * GetWordNodeWhereAnEnglishAdverbOfMannerIsEncapsulated(
	const CString & str) // englisches Adverb der Art und Weise
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>
					(current->m_pWord);
				if(ea!=NULL)
				{
					//TRACE("eav->word: %s   eav->type: %d  %u   %c\n",eav->word,eav->type,eav->type,eav->type);
					if(str==ea->m_strAdverb)
					{
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;
}

WordNode * 
	GetWordNodeWhereAnEnglishAdverbRefersToAdjectiveOrAdverbIsEncapsulated(
	const CString & str)
{
	TRACE("BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str) - ANFANG\n");
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord && eav->m_bType==1)
					{
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishAdverbIsEncapsulated(
	const CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord)
					{
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str) - ENDE - return FALSE\n");
	return NULL;
}

WordNode * 
	GetPointerToWordNodeWhereAnEnglishAdverbWhichRefersToAnAdjectiveOrAdverbIsEncapsulated(
	const CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord && eav->m_bType==1)
					{
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str) - ENDE - return FALSE\n");
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishComperativeIsEncapsulated(const 
	CStringVector & sv,const DWORD & dwFlags)
{
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishComperativeIsEnca\
psulated(CStringVector & sv,const DWORD & dwFlags) - ANFANG\n");
	if((dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
		printf("(dwFlags&1)==1\n");
	else
		printf("!(dwFlags&1)==1\n");
	printf("dwFlags: %u\n",dwFlags);
#endif
	//ein englisches Komperativ darf nicht mehr als 2 Token beinhalten
	if(sv.size()==0 || sv.size()>2)
	{
#ifdef _DEBUG
		printf("WordNode * GetPointerToWordNodeWhereAnEnglishComperativeIsEnca\
psulated(CStringVector & sv,const DWORD & dwFlags) - return NULL - \
sv.size()==0 || sv.size()>2\n");
#endif
		return NULL;
	}
	if(sv.size()==1)
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
				if(ea!=NULL)
				{
					//TRACE("eav->word: %s   eav->type: %d  %u   %c\n",eav->word,eav->type,eav->type,eav->type);
					//wenn die Gross- und Kleinschreibung beachtet werden soll
					if((dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
					{
						if(sv.at(0)==ea->m_strComperativ)
						{
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishPositiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
					}
					else
					{
						CString str2=ea->m_strPositiv;
						str2.MakeLower();
						if(sv.at(0)==str2)
						{
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishPositiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	if(sv.size()==2)
	{
#ifdef _DEBUG
		printf("sv.size()==2\n");
#endif
		if(sv.at(0)!=_T("more"))
		{
#ifdef _DEBUG
			printf("WordNode * GetPointerToWordNodeWhereAnEnglishComperativeIsEnca\
psulated(CStringVector & sv,const DWORD & dwFlags) - return NULL - \
sv.at(0)!=_T(\"more\")\n");
#endif
			return NULL;
		}
		if(wordList.m_pWordNodeFirst!=NULL)
		{
			WordNode * current=wordList.m_pWordNodeFirst;
			while(1)
			{
				if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
				{
					EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
					if(ea!=NULL)
					{
						if(ea->m_bMoreMost)
						{
						//TRACE("eav->word: %s   eav->type: %d  %u   %c\n",eav->word,eav->type,eav->type,eav->type);
						//wenn die Gross- und Kleinschreibung beachtet werden soll
							if((dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
							{
								if(sv.at(1)==ea->m_strPositiv)
								{
#ifdef _DEBUG
			printf("WordNode * GetPointerToWordNodeWhereAnEnglishComperativeIsEnca\
psulated(CStringVector & sv,const DWORD & dwFlags) - return current\n");
#endif
									return current;
								}
							}
							else
							{
								CString str2=ea->m_strPositiv;
								str2.MakeLower();
								if(sv.at(1)==str2)
								{
#ifdef _DEBUG
									printf("WordNode * GetPointerToWordNodeWhereAnEnglishComperativeIsEnca\
psulated(CStringVector & sv,const DWORD & dwFlags) - return current\n");
#endif
									return current;
								}
							}
						}
					}
				}
				if(current->m_pWordNodeNext==NULL)
					break;
				else
					current=current->m_pWordNodeNext;
			}
		}
	}
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishComperativeIsEnca\
psulated(CStringVector & sv,const DWORD & dwFlags) - return NULL\n");
#endif
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishConjunctionIsEncapsulated(const 
	CStringVector & sv)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishConjunction))
			{
				EnglishConjunction * ec=dynamic_cast<EnglishConjunction*>
					(current->m_pWord);
				if(ec!=NULL)
				{
					//TRACE("ec->m_strWord: %s\n",ep->m_strWord);
					CStringVector cstrvecEN=GetCStringVector2(ec->m_strWord);
					//TRACE("cstrvecEN.size(): %d cstrvec.size(): %d\n",cstrvecEN.size(),cstrvec.size());
					if(cstrvecEN.size()==sv.size())
					{
						//TRACE("Die Größen sind gleich.\n");
						BOOL bEqual=TRUE;
						for(DWORD i=0;i<sv.size();i++)
						{
							//TRACE("cstrvec.at(i): %s cstrvecEN.at(i): %s\n",cstrvec.at(i),cstrvecEN.at(i));
							if(sv.at(i)!=cstrvecEN.at(i))
							{
								bEqual=FALSE;
								break;
							}
						}
						if(bEqual)
						{
							return current;
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str) - ENDE - return FALSE\n");
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(
	const CString & str)
{
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(\
const CString & str) - ANFANG\n");
	printf("str: %s\n",str);
#endif

	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==eav->m_strWords[0])
					{
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(\
const CString & str) - return current\n");
#endif
						return current;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==ev->m_strInfinitive)
					{
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(\
const CString & str) - return current\n");
#endif
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishInfinitiveIsEncapsulated(\
const CString & str) - return NULL\n");
#endif
	return NULL;
}

std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> 
	GetVecPointerToWordNodeWhereAnEnglishNounIsEncapsulated(
	const CStringVector & cstrvec,const DWORD & dwFlags)
{
	#ifdef _DEBUG
	printf("std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> \
IfIsEnglishNounThenReturnEnglishNounEncapsulatedInVocable(const \
CStringVector & cstrvec) ANFANG\n");
	#endif
	std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> 
		vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					CStringVector cstrvecEN=GetCStringVector2(en->m_strSingular);
					if(cstrvecEN.size()==cstrvec.size())
					{
						BOOL bEqual=TRUE;
						for(DWORD i=0;i<cstrvec.size();i++)
						{
							//if(!(dwFlags&CASE_SENSITIVE==CASE_SENSITIVE))
							if(!(dwFlags&CASE_SENSITIVE))
								cstrvecEN.at(i).MakeLower();
							if(cstrvec.at(i)!=cstrvecEN.at(i))
							{
								bEqual=FALSE;
								break;
							}
						}
						if(bEqual && (en->m_bTranslationType==0 || en->m_bTranslationType==1))
						{
							PointerToWordNodeWhereAnEnglishNounIsEncapsulated ptvwaenie;
							ptvwaenie.m_pWordNode=current;
							vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
								push_back(ptvwaenie);
							goto afterPlural;
						}
					}
						cstrvecEN.clear();
						cstrvecEN=GetCStringVector2(en->m_strPlural);
						if(cstrvecEN.size()==cstrvec.size())
						{
							//TRACE("Die Größen sind gleich.\n");
							BOOL bEqual=TRUE;
							for(DWORD i=0;i<cstrvec.size();i++)
							{
							//TRACE("cstrvec.at(i): %s cstrvecEN.at(i): %s\n",cstrvec.at(i),cstrvecEN.at(i));
								//if(!(dwFlags&CASE_SENSITIVE==CASE_SENSITIVE))
								if(!(dwFlags&CASE_SENSITIVE))
									cstrvecEN.at(i).MakeLower();
								if(cstrvec.at(i)!=cstrvecEN.at(i))
								{
									bEqual=FALSE;
									break;
								}
							}
							if(bEqual && (en->m_bTranslationType==0 || en->m_bTranslationType==2))
							{
								#ifdef _DEBUG
								printf("Plural\n");
								#endif
								PointerToWordNodeWhereAnEnglishNounIsEncapsulated ptvwaenie;
								//ptvwaenie.m_pEnglishNoun=en;
								ptvwaenie.m_pWordNode=current;
								ptvwaenie.m_bSingular=FALSE;
								vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
									push_back(ptvwaenie);
							//TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return TRUE\n");
							//return TRUE;
							}
						}
afterPlural:;
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	#ifdef _DEBUG
	printf("std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> \
IfIsEnglishNounThenReturnEnglishNounEncapsulatedInVocable(const \
CStringVector & cstrvec) ENDE\n");
	#endif
	return vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated;
}

WordNode * GetPointerToWordNodeWhereAnEnglishPastParticipleIsEncapsulated(
	const CString & str)
{
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishPastParticiple\
IsEncapsulated(const CString & str) - ANFANG\n");
#endif
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		//CString str=str;
		//if(str=="your")
		//	str="having";
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
#ifdef _DEBUG
			printf("in der while-Schleife\n");
			fflush(stdout);
#endif
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
#ifdef _DEBUG
				printf("typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
				fflush(stdout);
#endif
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==eav->m_strWords[13])
					{
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishPastParticiple\
IsEncapsulated(const CString & str) - return current\n");
						fflush(stdout);
#endif
						return current;
					}
				}
			}
#ifdef _DEBUG
			printf("vor: if(typeid(*current->m_pWord)==typeid(EnglishVerb))\n");
			fflush(stdout);
#endif
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
#ifdef _DEBUG
				printf("typeid(*current->m_pWord)==typeid(EnglishVerb)\n");
				fflush(stdout);
#endif
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==ev->m_strPastParticiple)
					{
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishPastParticiple\
IsEncapsulated(const CString & str) - return current\n");
						fflush(stdout);
#endif
						return current;
					}
				}
			}
#ifdef _DEBUG
			printf("vor: if(current->m_pWordNodeNext==NULL)\n");
			fflush(stdout);
#endif
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishPastParticiple\
IsEncapsulated(const CString & str) - return NULL\n");
	fflush(stdout);
#endif
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishPositiveIsEncapsulated(
	const CString & str,const DWORD & dwFlags)
{
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishPositiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - ANFANG\n");
	if((dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
		printf("(dwFlags&1)==1\n");
	else
		printf("!(dwFlags&1)==1\n");
	printf("dwFlags: %u\n",dwFlags);
#endif
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
				if(ea!=NULL)
				{
					//TRACE("eav->word: %s   eav->type: %d  %u   %c\n",eav->word,eav->type,eav->type,eav->type);
					//wenn die Gross- und Kleinschreibung beachtet werden soll
					if((dwFlags&CASE_SENSITIVE)==CASE_SENSITIVE)
					{
						if(str==ea->m_strPositiv)
						{
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishPositiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
					}
					else
					{
						CString str2=ea->m_strPositiv;
						str2.MakeLower();
						if(str==str2)
						{
#ifdef _DEBUG
							printf("WordNode * GetPointerToWordNodeWhereAnEnglishPositiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return current\n");
#endif
							return current;
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishPositiveIsEnca\
psulated(const CString & str,BOOL dwFlags) - return NULL\n");
#endif
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishPrepositionIsEncapsulated(
	const CStringVector & sv)
{
	#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishPrepositionIs\
Encapsulated(const CStringVector & sv) - ANFANG\n");
	#endif
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishPreposition))
			{
				EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>
					(current->m_pWord);
				if(ep!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					CStringVector	sv2=GetCStringVector2(ep->m_strWord);
					if(sv==sv2)
					{
						#ifdef _DEBUG
						printf("WordNode * GetPointerToWordNodeWhereAnEnglish\
PrepositionIsEncapsulated(const CStringVector & sv) - return current\n");
						#endif
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	#ifdef _DEBUG
	printf("VocNode * GetPointerToVocNodeWhereAnEnglishPrepositionIsEncapsulated(const CStringVector & sv) - return FALSE\n");
	#endif
	return NULL;
}

//die fortschreitenden Partizipien werden nach dem Buch "ENGLISCH im 
//TELEKOLLEG I Übungsband Seite 13 gebildet
WordNode * GetPointerToWordNodeWhereAnEnglishProgressiveIsEncapsulated(
	const CString & rstrToken)
{
	#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishProgressiveIs\
Encapsulated(const CString & rstrToken) - ANFANG\n");
	printf("rstrToken: %s\n",rstrToken);
	#endif
	WordNode * pWordNodeCurrent=wordList.m_pWordNodeFirst;
	while(pWordNodeCurrent)
	{
		if(typeid(*pWordNodeCurrent->m_pWord)==typeid(EnglishAuxiliaryVerb))
		{
			EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
				(pWordNodeCurrent->m_pWord);
#ifdef _DEBUG
			printf("typeid(*pWordNodeCurrent->m_pWord)==typeid(EnglishAuxiliaryVerb)\n");
#endif
			//wenn eav NICHT NULL ist
			if(eav)
			{
#ifdef _DEBUG
				printf("eav!=NULL\n");
				printf("eav->m_strWords[0]: %s\n",eav->m_strWords[0]);
#endif
				//wenn der Infinitiv aus mindestens 3 Zeichen besteht (damit "be" 
				//nicht eingeschlossen wird), das zweite 
				//Zeichen von rechts ein Konsonant ist und das Zeichen ganz rechts 
				//ein "e" ist. Beispiel: "have"
				if(eav->m_strWords[0].GetLength()>2 && IsConsonant(eav->
					m_strWords[0].Right(2).Left(1)) && eav->m_strWords[0].
					Right(1)==_T("e"))
				{
					//wenn die Zeichenkette mit dem gebildeten 
					//fortschreitenden Partizip übereinstimmt. Beispiel für ein 
					//gebildetes forschreitendes Partizip:
					//"having"
					if (rstrToken==eav->m_strWords[0].Left(eav->m_strWords[0].
						GetLength()-1)+_T("ing"))
						return pWordNodeCurrent;
				}
				//wenn der Infinitiv aus mindestens 2 Zeichen besteht,
				//das zweite Zeichen von rechts ein Vokal ist und das Zeichen
				//ganz rechts ein Konsonant ist.
				if(eav->m_strWords[0].GetLength()>1 && IsVowel(eav->m_strWords[0].
					Right(2).Left(1)) && IsConsonant(eav->m_strWords[0].Right(1)))
				{
					//wenn die Zeichenkette mit dem gebildeten 
					//fortschreitenden Partizip übereinstimmt.
					if(rstrToken==eav->m_strWords[0]+eav->m_strWords[0].
						Right(1)+_T("ing"))
						return pWordNodeCurrent;
				}
				//wenn die Zeichenkette mit dem gebildeten 
				//fortschreitenden Partizip übereinstimmt.
				if(rstrToken==eav->m_strWords[0]+_T("ing"))
					return pWordNodeCurrent;
			}
		}
		if(typeid(*pWordNodeCurrent->m_pWord)==typeid(EnglishVerb))
		{
			EnglishVerb * englishVerb=dynamic_cast<EnglishVerb*>(
				pWordNodeCurrent->m_pWord);
			//wenn englishVerb NICHT NULL ist
			if(englishVerb)
			{
				//wenn der Infinitiv aus mindestens 2 Zeichen besteht
				if(englishVerb->m_strInfinitive.GetLength()>1)
				{
					//wenn das zweite Zeichen von rechts ein Konsonant ist und das Zeichen
					//ganz rechts ein "e" ist. Beispiel: "make", "like", "love"
					if(IsConsonant(englishVerb->m_strInfinitive.Right(2).Left(1)) && 
						englishVerb->m_strInfinitive.Right(1)==_T("e"))
					{
						//wenn die Zeichenkette mit dem gebildeten 
						//fortschreitenden Partizip übereinstimmt. Beispiel für ein 
						//gebildetes forschreitendes Partizip:
						//"making","liking","loving"
						if(rstrToken==englishVerb->m_strInfinitive.Left(
							englishVerb->m_strInfinitive.GetLength()-1)+_T("ing"))
							return pWordNodeCurrent;
					}
					//wenn das zweite Zeichen von rechts ein Vokal ist und das Zeichen
					//ganz rechts ein Konsonant ist. Beispiel: "put", "prefer", "begin"
					if(IsVowel(englishVerb->m_strInfinitive.Right(2).Left(1)) && 
						IsConsonant(englishVerb->m_strInfinitive.Right(1)))
					{
						//wenn die Zeichenkette mit dem gebildeten 
						//fortschreitenden Partizip übereinstimmt. Beispiel für ein 
						//gebildetes forschreitendes Partizip:
						//"putting","preferring","beginning"
						if(rstrToken==englishVerb->m_strInfinitive+englishVerb->
							m_strInfinitive.Right(1)+_T("ing"))
							return pWordNodeCurrent;
					}
				}
				if(rstrToken==englishVerb->m_strInfinitive+_T("ing"))
					return pWordNodeCurrent;
			}
		}
		pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	}
	#ifdef _DEBUG
	printf("WordNode * GetPointerToWordNodeWhereAnEnglishProgressiveIs\
Encapsulated(const CString & str) - return NULL\n");
	#endif
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishPronounIsEncapsulated(
	const CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishPronoun))
			{
				EnglishPronoun * ep=dynamic_cast<EnglishPronoun*>(current->m_pWord);
				if(ep!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==ep->m_strSingular || str==ep->m_strPlural)
					{
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishPronounIsEncapsulated(
	const CStringVector & sv)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishPronoun))
			{
				EnglishPronoun * ep=dynamic_cast<EnglishPronoun*>(current->m_pWord);
				if(ep!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					CStringVector svPreposition=GetCStringVector2(ep->m_strSingular);
					if(svPreposition==sv)
					{
						return current;
					}
					svPreposition=GetCStringVector2(ep->m_strPlural);
					if(svPreposition==sv)
					{
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;
}

WordNode * 
	GetPointerToWordNodeWhereABendEnglishVerbIsEncapsulated(
	const CString & str,WORD & rwFiniteVerbFormsAffectedbySearchString
	//die Menge der Formen eines Wortes, die das 
	//Flexionsparadigma von Verben oder Substantiven bilden; 
	//Konjugation beziehungsweise Deklination.
	)
{
	bool bFound=false;
	rwFiniteVerbFormsAffectedbySearchString=0;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL //&& 
					//wenn kein modales Hilfsverb
					//eav->m_bIntegral==FALSE
					)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					//gebeugte Verbformen von 1. Person Singular Präsens bis
					//3. Person Plural Präteritum=12 Verbformen
					for(BYTE byRun=1;byRun<13;++byRun)
					{
						if(str==eav->m_strWords[byRun])
						{
							rwFiniteVerbFormsAffectedbySearchString|=(1<<byRun);
							bFound=true;
						}
					}
					if(bFound)
						return current;
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==ev->m_strInfinitive)
					{
						//bei einem Vollverb sind alle gebeugten Verbformen
						//von 1. Person Singular bis 3. Person Plural 
						//identisch außer 3. Person Singular!
						//Person:  1 2  4  5  6 
						//       ->2+4+16+32+64=118
						rwFiniteVerbFormsAffectedbySearchString=118;
						return current;
					}
					if(str==ev->m_strPastTense)
					{
						//alle gebeugten Verbformen bei einem VOOLverb 
						//im Präteritum sind identisch!
						//binär:1111110000000
						rwFiniteVerbFormsAffectedbySearchString=8064;
						return current;
					}
					if(ev->m_strInfinitive.GetLength()>0)
					{
						//z.B. "go"
						if(ev->m_strInfinitive.Right(1)==_T("o"))
						{
							if(ev->m_strInfinitive+_T("es")==str)
							{
								//3. Person Singular
								rwFiniteVerbFormsAffectedbySearchString=8;
								return current;
							}
						}
						else
							if(ev->m_strInfinitive+_T("s")==str)
							{
								//3. Person Singular
								rwFiniteVerbFormsAffectedbySearchString=8;
								return current;
							}
						//if(
					}
					/*if(ev->infinitive.GetLength()>0)
						if(ev->infinitive.Right(1)==_T("o"))
						{
							if(ev->infinitive+_T("es")==str)
								return current;
						}*/
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str) - ENDE - return FALSE\n");
	return NULL;
}

CString GetCorrespondingParticipleProgressive(const CString & 
	strInfinitive)
{
	CString strCorrespondingParticipleProgressive;
	if(strInfinitive.GetLength()>2)
	{
		CString str2RightmostChars=strInfinitive.Right(2);
		//z.B. haVE, liKE
		if(str2RightmostChars.Right(1)==_T("e") && 
			IsConsonant(str2RightmostChars.Left(1))
			)
			strCorrespondingParticipleProgressive=
				strInfinitive.Left(strInfinitive.GetLength()-1)+_T("ing");
		else
			//z.B. fuck
			strCorrespondingParticipleProgressive=
				strInfinitive+_T("ing");
	}
	return strCorrespondingParticipleProgressive;
}

WordNode * 
	GetPointerToWordNodeWhereANonBendEnglishVerbIsEncapsulated(
	const CString & str,BYTE & rbyType
	//die Menge der Formen eines Wortes, die das 
	//Flexionsparadigma von Verben oder Substantiven bilden; 
	//Konjugation beziehungsweise Deklination.
	)
{
	bool bFound=false;
	rbyType=0;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL && 
//Warnung	17	warning C4805: '==': unsichere Kombination von Typ 'BYTE' mit Typ 'bool' in einer Operation	
#pragma warning(disable: 4805)
					//wenn kein modales Hilfsverb (denn modale 
					//Hilsfverben sind immer gebeugt)
					eav->m_bIntegral==true//FALSE
#pragma warning(restore: 4805)
          )
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==eav->m_strWords[EAV_ARRAY_INDEX_FOR_INFINITIVE])
					{
						rbyType=INFINITIVE;
					}
					if(str==eav->m_strWords[EAV_ARRAY_INDEX_FOR_PAST_PARTICIPLE])
					{
						if(rbyType==INFINITIVE)
              //The infintive and past participle of some English verbs(e.g. "hit") are identical.
							rbyType=INFINITIVE_OR_PAST_PARTICIPLE;
						else
							rbyType=PAST_PARTICIPLE;
					}
					if( 
						//z.B.: I am HAVING fun.
						str==GetCorrespondingParticipleProgressive(
						  eav->m_strWords[EAV_ARRAY_INDEX_FOR_INFINITIVE]))
					{
						rbyType=PARTICIPLE_PROGRESSIVE;
					}
					//erst an dieser Stelle zurückspringen, da Infinitiv und
					//Partizip Perfekt eines Verbes identisch sein können:
					//to hit-I have hit.
					if(rbyType!=0)
						return current;
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==ev->m_strInfinitive)
						rbyType=INFINITIVE;
					//if(str==ev->m_strPastTense)
					if(str==ev->m_strPastParticiple)
						if(rbyType==INFINITIVE)
							rbyType=INFINITIVE_OR_PAST_PARTICIPLE;
						else
							rbyType=PAST_PARTICIPLE;
					if(str==GetCorrespondingParticipleProgressive(
						ev->m_strInfinitive)
						)
					{
						rbyType=PARTICIPLE_PROGRESSIVE;
					}
					//erst an dieser Stelle zurückspringen, da Infinitiv und
					//Partizip Perfekt eines Verbes identisch sein können
					if(rbyType!=0)
						return current;
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str) - ENDE - return FALSE\n");
	return NULL;
}

WordNode * GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
	const CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==eav->m_strWords[0]+_T("ing"))
						return current;
					for(DWORD dwRun=0;dwRun<15;dwRun++)
					{
						if(str==eav->m_strWords[dwRun])
							return current;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==ev->m_strInfinitive || str==ev->m_strPastTense || 
						str==ev->m_strPastParticiple)
					{
						return current;
					}
					if(str.GetLength()>0)
						if(ev->m_strInfinitive.Right(1)==_T("e")) // wenn das Zeichen ganz rechts ein 'e'
						{ // ist (z.B. bei 'to like')
								//printf("ev->infinitive
							if(str==ev->m_strInfinitive.Left(ev->m_strInfinitive.
								GetLength()-1)+_T("ing"))
								return current;
						}
						else
						{
							if(str==ev->m_strInfinitive+_T("ing"))
								return current;
						}
					//if(str==ev->infinitive+_T("ing"))
					//	return current;
					if(ev->m_strInfinitive.GetLength()>0)
					{
						if(ev->m_strInfinitive.Right(1)==_T("o"))
						{
							if(ev->m_strInfinitive+_T("es")==str)
								return current;
						}
						else
							if(ev->m_strInfinitive+_T("s")==str)
								return current;
						if(ev->m_strInfinitive.Right(1)==_T("e")) // make -> making
							if(str==ev->m_strInfinitive.Left(str.GetLength()-1)+_T("ing"))
								return current;
						//if(
					}
					/*if(ev->infinitive.GetLength()>0)
						if(ev->infinitive.Right(1)==_T("o"))
						{
							if(ev->infinitive+_T("es")==str)
								return current;
						}*/
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str) - ENDE - return FALSE\n");
	return NULL;
}

//die Bits in bIndexes geben an, ob eine gebeugte Verbform eine Person
//mit der übergebenen Zeichenkette "str" übereinstimmen
//Beispiel: str=="have" -> "to have"       -> 1. Bit setzen
//                         "I have"        -> 2. Bit setzen
//                         "you have"      -> 3. Bit setzen
//                         "he/she/it has" -> 4. Bit nicht setzen
//                         "we have"       -> 5. Bit setzen
//                         "you have       -> 6. Bit setzen
//                         "they have"     -> 7. Bit setzen
//demnach sieht bIndexes binär so aus: 11101110
WordNode * GetPointerToWordNodeWhereAnEnglishVerbIsEncapsulated(
	const CString & str,BYTE & bIndexes,BYTE & bTense)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					BYTE bEquals=0;
					bIndexes=0;
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==eav->m_strWords[0]+_T("ing"))
						return current;
					for(BYTE bRun=0;bRun<15;bRun++)
					{
						if(str==eav->m_strWords[bRun])
						{
							bEquals=1;
							if(bRun<7)
							{
								bTense=SIMPLE_PRESENT;
								bIndexes|=(BYTE)pow(2.0,bRun);
							}
							if(bRun>6 && bRun<14)
							{
								bTense=SIMPLE_PAST;
								bIndexes|=(BYTE)pow(2.0,bRun-6);
							}
							if(bRun==14)
								bTense=PRESENT_PERFECT;
						}
					}
					if(bEquals)
						return current;
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					//TRACE("ep->m_strWord: %s\n",ep->m_strWord);
					if(str==ev->m_strInfinitive || str==ev->m_strPastTense || 
						str==ev->m_strPastParticiple)
					{
						return current;
					}
					if(str.GetLength()>0)
						if(ev->m_strInfinitive.Right(1)==_T("e")) // wenn das Zeichen ganz rechts ein 'e'
						{ // ist (z.B. bei 'to like')
								//printf("ev->infinitive
							if(str==ev->m_strInfinitive.Left(ev->m_strInfinitive.
								GetLength()-1)+_T("ing"))
								return current;
						}
						else
						{
							if(str==ev->m_strInfinitive+_T("ing"))
								return current;
						}
					//if(str==ev->infinitive+_T("ing"))
					//	return current;
					if(ev->m_strInfinitive.GetLength()>0)
					{
						if(ev->m_strInfinitive.Right(1)==_T("o"))
						{
							if(ev->m_strInfinitive+_T("es")==str)
								return current;
						}
						else
							if(ev->m_strInfinitive+_T("s")==str)
								return current;
						if(ev->m_strInfinitive.Right(1)==_T("e")) // make -> making
							if(str==ev->m_strInfinitive.Left(str.GetLength()-1)+_T("ing"))
								return current;
						//if(
					}
					/*if(ev->infinitive.GetLength()>0)
						if(ev->infinitive.Right(1)==_T("o"))
						{
							if(ev->infinitive+_T("es")==str)
								return current;
						}*/
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str) - ENDE - return FALSE\n");
	return NULL;
}

std::vector<PointerToEnglishNoun> IfIsEnglishNounThenGetVectorOfPointerToEnglishNoun(const CStringVector & cstrvec)
{
	TRACE("IsEnglishSingular(CStringVector cstrvec) - ANFANG\n");
	std::vector<PointerToEnglishNoun> vecPointerToEnglishNoun;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					//TRACE("en->singular: %s\n",en->singular);
					CStringVector cstrvecEN=GetCStringVector2(en->m_strSingular);
					//TRACE("cstrvecEN.size(): %d cstrvec.size(): %d\n",cstrvecEN.size(),cstrvec.size());
					if(cstrvecEN.size()==cstrvec.size())
					{
						//TRACE("Die Größen sind gleich.\n");
						BOOL bEqual=TRUE;
						for(DWORD i=0;i<cstrvec.size();i++)
						{
							//TRACE("cstrvec.at(i): %s cstrvecEN.at(i): %s\n",cstrvec.at(i),cstrvecEN.at(i));
							if(cstrvec.at(i)!=cstrvecEN.at(i))
							{
								bEqual=FALSE;
								break;
							}
						}
						if(bEqual)
						{
							PointerToEnglishNoun pten;
							pten.m_pEnglishNoun=en;
							//pten.m_bSingular=TRUE;
							vecPointerToEnglishNoun.push_back(pten);
							//TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return TRUE\n");
							//return TRUE;
						}
					}
					else
					{
						cstrvecEN=GetCStringVector2(en->m_strSingular);
						if(cstrvecEN.size()==cstrvec.size())
						{
							//TRACE("Die Größen sind gleich.\n");
							BOOL bEqual=TRUE;
							for(DWORD i=0;i<cstrvec.size();i++)
							{
								//TRACE("cstrvec.at(i): %s cstrvecEN.at(i): %s\n",cstrvec.at(i),cstrvecEN.at(i));
								if(cstrvec.at(i)!=cstrvecEN.at(i))
								{
									bEqual=FALSE;
									break;
								}
							}
							if(bEqual)
							{
								PointerToEnglishNoun pten;
								pten.m_pEnglishNoun=en;
								pten.m_bSingular=FALSE;
								vecPointerToEnglishNoun.push_back(pten);
								//TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return TRUE\n");
								//return TRUE;
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return vecpEnglishNoun\n");
	return vecPointerToEnglishNoun;
}

std::vector<EnglishNoun*> IfIsEnglishNounThenGetVectorOfPointersToEnglishNoun(
	const CStringVector & cstrvec)
{
	std::vector<EnglishNoun*> vecpEnglishNoun;
	TRACE("IsEnglishSingular(CStringVector cstrvec) - ANFANG\n");
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					//TRACE("en->singular: %s\n",en->singular);
					CStringVector cstrvecEN=GetCStringVector2(en->m_strSingular);
					//TRACE("cstrvecEN.size(): %d cstrvec.size(): %d\n",cstrvecEN.size(),cstrvec.size());
					if(cstrvecEN.size()==cstrvec.size())
					{
						//TRACE("Die Größen sind gleich.\n");
						BOOL bEqual=TRUE;
						for(DWORD i=0;i<cstrvec.size();i++)
						{
							//TRACE("cstrvec.at(i): %s cstrvecEN.at(i): %s\n",cstrvec.at(i),cstrvecEN.at(i));
							if(cstrvec.at(i)!=cstrvecEN.at(i))
							{
								bEqual=FALSE;
								break;
							}
						}
						if(bEqual)
						{
							vecpEnglishNoun.push_back(en);
							//TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return TRUE\n");
							//return TRUE;
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return vecpEnglishNoun\n");
	return vecpEnglishNoun;
}

std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> 
	IfIsEnglishNounThenReturnEnglishNounEncapsulatedInWord(const 
	CStringVector & cstrvec)
{
	#ifdef _DEBUG
	printf("std::vector<PointerToVocNodeWhereAnEnglishNounIsEncapsulated> \
IfIsEnglishNounThenReturnEnglishNounEncapsulatedInVocable(const \
CStringVector & cstrvec) ANFANG\n");
	#endif
	std::vector<PointerToWordNodeWhereAnEnglishNounIsEncapsulated> 
		vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					//TRACE("en->singular: %s\n",en->singular);
					CStringVector cstrvecEN=GetCStringVector2(en->m_strSingular);
					//TRACE("cstrvecEN.size(): %d cstrvec.size(): %d\n",cstrvecEN.size(),cstrvec.size());
					if(cstrvecEN.size()==cstrvec.size())
					{
						//TRACE("Die Größen sind gleich.\n");
						BOOL bEqual=TRUE;
						for(DWORD i=0;i<cstrvec.size();i++)
						{
							//TRACE("cstrvec.at(i): %s cstrvecEN.at(i): %s\n",cstrvec.at(i),cstrvecEN.at(i));
							if(cstrvec.at(i)!=cstrvecEN.at(i))
							{
								bEqual=FALSE;
								break;
							}
						}
						if(bEqual && (en->m_bTranslationType==0 || en->m_bTranslationType==1))
						{
							PointerToWordNodeWhereAnEnglishNounIsEncapsulated ptvwaenie;
							ptvwaenie.m_pWordNode=current;
							//pten.m_pEnglishNoun=en;
							//pten.m_bSingular=TRUE;
							vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.push_back(ptvwaenie);
							//TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return TRUE\n");
							//return TRUE;
							goto afterPlural;
						}
					}
						cstrvecEN.clear();
						cstrvecEN=GetCStringVector2(en->m_strPlural);
						if(cstrvecEN.size()==cstrvec.size())
						{
							//TRACE("Die Größen sind gleich.\n");
							BOOL bEqual=TRUE;
							for(DWORD i=0;i<cstrvec.size();i++)
							{
							//TRACE("cstrvec.at(i): %s cstrvecEN.at(i): %s\n",cstrvec.at(i),cstrvecEN.at(i));
								if(cstrvec.at(i)!=cstrvecEN.at(i))
								{
									bEqual=FALSE;
									break;
								}
							}
							if(bEqual && (en->m_bTranslationType==0 || en->m_bTranslationType==2))
							{
								#ifdef _DEBUG
								printf("Plural\n");
								#endif
								PointerToWordNodeWhereAnEnglishNounIsEncapsulated ptvwaenie;
								//ptvwaenie.m_pEnglishNoun=en;
								ptvwaenie.m_pWordNode=current;
								ptvwaenie.m_bSingular=FALSE;
								vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated.
									push_back(ptvwaenie);
							//TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return TRUE\n");
							//return TRUE;
							}
						}
afterPlural:;
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	#ifdef _DEBUG
	printf("std::vector<PointerToVocNodeWhereAnEnglishNounIsEncapsulated> \
IfIsEnglishNounThenReturnEnglishNounEncapsulatedInVocable(const \
CStringVector & cstrvec) ENDE\n");
	#endif
	return vecPointerToWordNodeWhereAnEnglishNounIsEncapsulated;
}

//liefert zurück, ob das erste Zeichen der Zeichenkette ein Konsonant ist
//sollte KEINE Referenz sein, da str verändert wird
BYTE IsConsonant(CString str)
{
	char ch;
	str.MakeLower();
	ch=str[0];
	return ch=='b' || ch=='c' || ch=='d' || ch=='f' || ch=='g' || ch=='h'
		|| ch=='j' || ch=='k' || ch=='l' || ch=='m' || ch=='n' || ch=='p'
		|| ch=='q' || ch=='r' || ch=='s' || ch=='t' || ch=='v' || ch=='w'
		|| ch=='x' || ch=='y' || ch=='z';
}

BYTE IsConsonant(TCHAR ch)
{
  //Convert character to lowercase.
  ch = tolower(ch) ;
	return ch=='b' || ch=='c' || ch=='d' || ch=='f' || ch=='g' || ch=='h'
		|| ch=='j' || ch=='k' || ch=='l' || ch=='m' || ch=='n' || ch=='p'
		|| ch=='q' || ch=='r' || ch=='s' || ch=='t' || ch=='v' || ch=='w'
		|| ch=='x' || ch=='y' || ch=='z';
}

BOOL IsEnglishAdverbOfFrequency(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord && eav->m_bType==0)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishAdverbOfManner(CString str) // englisches Adverb der Art und Weise
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>
					(current->m_pWord);
				if(ea!=NULL)
				{
					//TRACE("eav->word: %s   eav->type: %d  %u   %c\n",eav->word,eav->type,eav->type,eav->type);
					if(str==ea->m_strAdverb)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord && eav->m_bType==1)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishAdverbRefersToAdjectiveOrAdverb(const CString & str) - ENDE - return FALSE\n");
	return FALSE;
}

BOOL IsEnglishAdverbRefersToWholeSentence(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord && eav->m_bType==2)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishPlural(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					if(str==en->m_strPlural)
						return TRUE;
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishPlural(const CStringVector & cstrvec)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				/*EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->voc);
				if(en!=NULL)
				{
					//TRACE("en->singular: %s\n",en->singular);
					CStringVector cstrvecEN=GetCStringVector2(en->singular);
					//TRACE("cstrvecEN.size(): %d cstrvec.size(): %d\n",cstrvecEN.size(),cstrvec.size());
					if(cstrvecEN.size()==cstrvec.size())
					{
						//TRACE("Die Größen sind gleich.\n");
						BOOL bEqual=TRUE;
						for(int i=0;i<cstrvec.size();i++)
						{
							//TRACE("cstrvec.at(i): %s cstrvecEN.at(i): %s\n",cstrvec.at(i),cstrvecEN.at(i));
							if(cstrvec.at(i)!=cstrvecEN.at(i))
							{
								bEqual=FALSE;
								break;
							}
						}
						if(bEqual)
						{
							TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return TRUE\n");
							return TRUE;
						}
					}
				}*/
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					CStringVector cstrvecEN=GetCStringVector2(en->m_strPlural);
					if(cstrvecEN.size()==cstrvec.size())
					{
						BOOL bEqual=TRUE;
						for(DWORD i=0;i<cstrvec.size();i++)
						{
							if(cstrvec.at(i)!=cstrvecEN.at(i))
							{
								bEqual=FALSE;
								break;
							}
						}
						if(bEqual)
							return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishObjectVerb(CString str)
{
	TRACE("BOOL IsEnglishObjectVerb(CString str) - ANFANG\n");
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(str==ev->m_strInfinitive || str==ev->m_strInfinitive+"s" || 
						str==ev->m_strPastTense)
					{
						if(current->m_pWordNodeNext)
						{
							current=current->m_pWordNodeNext;
							if(typeid(*current->m_pWord)==typeid(GermanVerb))
							{
								GermanVerb * gv=dynamic_cast<GermanVerb*>(current->m_pWord);
								if(gv)
								{
									if(gv->m_bCase>0)
									{
										return TRUE;
									}
								}
							}
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishObjectVerb(CString str) - ENDE - return FALSE\n");
	return FALSE;
}

BOOL IsEnglishNoun(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					if(str==en->m_strSingular)
					{
						return TRUE;
					}
					if(str==en->m_strPlural)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishSingular(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					if(str==en->m_strSingular)
						return TRUE;
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishSingular(CStringVector & cstrvec)
{
	TRACE("IsEnglishSingular(CStringVector cstrvec) - ANFANG\n");
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					//TRACE("en->singular: %s\n",en->singular);
					CStringVector cstrvecEN=GetCStringVector2(en->m_strSingular);
					//TRACE("cstrvecEN.size(): %d cstrvec.size(): %d\n",cstrvecEN.size(),cstrvec.size());
					if(cstrvecEN.size()==cstrvec.size())
					{
						//TRACE("Die Größen sind gleich.\n");
						BOOL bEqual=TRUE;
						for(DWORD i=0;i<cstrvec.size();i++)
						{
							//TRACE("cstrvec.at(i): %s cstrvecEN.at(i): %s\n",cstrvec.at(i),cstrvecEN.at(i));
							if(cstrvec.at(i)!=cstrvecEN.at(i))
							{
								bEqual=FALSE;
								break;
							}
						}
						if(bEqual)
						{
							TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return TRUE\n");
							return TRUE;
						}
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("IsEnglishSingular(CStringVector cstrvec) - ENDE -return FALSE\n");
	return FALSE;
}

BOOL IsEnglishVerb(CString str)
{
	TRACE("BOOL IsEnglishVerb(CString str) - ANFANG\n");
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(str==ev->m_strInfinitive || str==ev->m_strInfinitive+"s" || 
						str==ev->m_strPastTense)
					{
						//delete ev;
						TRACE("BOOL IsEnglishVerb(CString str) - ENDE - return TRUE\n");
						return TRUE;
					}
					//delete ev;
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				TRACE("EnglishAuxiliaryVerb!\n");
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					for(int i=0;i<15;i++)
					{
						//TRACE("eav->words[i]: %s\n",eav->words[i]);
						if(str==eav->m_strWords[i])
						{
							//delete *eav;
							return TRUE;
						}
					}
					//delete eav;
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	if(IsPresentProgressive(str))
	{
		TRACE("BOOL IsEnglishVerb(CString str) - ENDE - return TRUE\n");
		return TRUE;
	}
	TRACE("BOOL IsEnglishVerb(CString str) - ENDE - return FALSE\n");
	return FALSE;
}

//ist die übergebene Zeichenkette ein fortschreitendes Partizip ("working"
//bei "I am working.","making" bei "I was "making a bread.","putting" bei
//"I have been putting.","preferring" bei "I had been preferring.",
//"beginning" bei "I will be beginning.")
//die fortschreitenden Partizipien werden nach dem Buch "ENGLISCH im 
//TELEKOLLEG I Übungsband Seite 13 gebildet
BYTE IsParticipleProgressive(const CString & rstrToken,const EnglishVerb 
	& rEnglishVerb)
{
	//wenn der Infinitiv aus mindestens 2 Zeichen besteht
	if(rEnglishVerb.m_strInfinitive.GetLength()>1)
	{
		//wenn das zweite Zeichen von rechts ein Konsonant ist und das Zeichen
		//ganz rechts ein "e" ist. Beispiel: "make", "like", "love"
		if(IsConsonant(rEnglishVerb.m_strInfinitive.Right(2).Left(1)) && 
			rEnglishVerb.m_strInfinitive.Right(1)==_T("e"))
		{
			//zurückliefern, ob die Zeichenkette mit dem gebildeten 
			//fortschreitenden Partizip übereinstimmt. Beispiel für ein 
			//gebildetes forschreitendes Partizip:
			//"making","liking","loving"
			return rstrToken==rEnglishVerb.m_strInfinitive.Left(rEnglishVerb.
				m_strInfinitive.GetLength()-1)+_T("ing");
		}
		//wenn das zweite Zeichen von rechts ein Vokal ist und das Zeichen
		//ganz rechts ein Konsonant ist. Beispiel: "put", "prefer", "begin"
		if(IsVowel(rEnglishVerb.m_strInfinitive.Right(2).Left(1)) && 
			IsConsonant(rEnglishVerb.m_strInfinitive.Right(1)))
		{
			//zurückliefern, ob die Zeichenkette mit dem gebildeten 
			//fortschreitenden Partizip übereinstimmt. Beispiel für ein 
			//gebildetes forschreitendes Partizip:
			//"putting","preferring","beginning"
			return rstrToken==rEnglishVerb.m_strInfinitive+rEnglishVerb.
				m_strInfinitive.Right(1)+_T("ing");
		}
	}
	return rstrToken==rEnglishVerb.m_strInfinitive+_T("ing");
}

BYTE IsPresentProgressive(const CString & token)
{
	WordNode * pWordNodeCurrent=wordList.m_pWordNodeFirst;
	while(pWordNodeCurrent)
	{
		if(typeid(*pWordNodeCurrent->m_pWord)==typeid(EnglishVerb))
		{
			EnglishVerb * ev=dynamic_cast<EnglishVerb*>(pWordNodeCurrent->m_pWord);
			//wenn ev NICHT NULL ist
			if(ev)
			{
				if(token.GetLength()>0)
					//wenn das Zeichen ganz rechts ein 'e' ist (z.B. bei 'to like')
					if(ev->m_strInfinitive.Right(1)==_T("e"))
					{
						#ifdef _DEBUG
						printf("ev->m_strInfinitive.Left(ev->m_strInfinitive.\
GetLength()-1)+_T(\"ing\"): %s\n",ev->m_strInfinitive.Left(
							ev->m_strInfinitive.GetLength()-1)+_T("ing"));
						#endif
						//printf("ev->infinitive
						if(token==ev->m_strInfinitive.Left(ev->m_strInfinitive.
							GetLength()-1)+_T("ing"))
						{
							return TRUE;
						}
					}
					else
					{
						if(token==ev->m_strInfinitive+_T("ing"))
						{
							return TRUE;
						}
					}
				
			}
		}
		pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	}
	return FALSE;
}

BOOL IsPresentProgressiveCopyVocable(EnglishVerb & rEV,const CString & token)
{
	TRACE("BOOL IsPresentProgressive(EnglishVerb & rEV,const CString & token) - ANFANG\n");
	#ifdef _DEBUG
	printf("BOOL IsPresentProgressive(EnglishVerb & rEV,const CString & token) - ANFANG\n");
	#endif
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(token.GetLength()>0)
					{
						if(ev->m_strInfinitive.Right(1)==_T("e")) // wenn das Zeichen ganz rechts ein 'e'
						{ // ist (z.B. bei 'to like')
							#ifdef _DEBUG
							printf("ev->m_strInfinitive.Left(ev->m_strInfinitive.GetLength()-1)+\
_T(\"ing\"): %s\n",ev->m_strInfinitive.Left(ev->m_strInfinitive.GetLength()-1)+_T("ing"));
							#endif
							//printf("ev->infinitive
							if(token==ev->m_strInfinitive.Left(ev->m_strInfinitive.GetLength()-1)+_T("ing"))
							{
								rEV=*ev;
								TRACE("BOOL IsPresentProgressive(const CString & token) - ENDE - return TRUE\n");
								#ifdef _DEBUG
								printf("BOOL IsPresentProgressive(EnglishVerb & rEV,const CString & token) - return TRUE\n");
								#endif
								return TRUE;
							}
						}
					}
					if(token==ev->m_strInfinitive+_T("ing"))
					{
						rEV=*ev;
						TRACE("BOOL IsPresentProgressive(const CString & token) - ENDE - return TRUE\n");
						#ifdef _DEBUG
						printf("BOOL IsPresentProgressive(EnglishVerb & rEV,const CString & token) - return TRUE\n");
						#endif
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsPresentProgressive(const CString & token) - ENDE - return FALSE\n");
	#ifdef _DEBUG
	printf("BOOL IsPresentProgressive(EnglishVerb & rEV,const CString & token) - return FALSE\n");
	#endif
	return FALSE;
}

BOOL IsPresentTense(CString & str)
{
	TRACE("BOOL IsPresentTense(CString str) - ANFANG\n");
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * pWordNodeCurrent=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*pWordNodeCurrent->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(pWordNodeCurrent->m_pWord);
				if(ev!=NULL)
				{
					if(str==ev->m_strInfinitive || str==ev->m_strInfinitive+_T("s"))
					{
						//delete ev;
						TRACE("BOOL IsPresentTense(CString str) - ENDE - return TRUE\n");
						return TRUE;
					}
					//delete ev;
				}
			}
			if(typeid(*pWordNodeCurrent->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				TRACE("EnglishAuxiliaryVerb!\n");
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>(
					pWordNodeCurrent->m_pWord);
				if(eav!=NULL)
				{
					for(int i=1;i<7;i++)
					{
						//TRACE("eav->words[i]: %s\n",eav->words[i]);
						if(str==eav->m_strWords[i])
						{
							//delete *eav;
							//In MFC/Windows the TRACE macro has a variable # of args--I do 
							//not know how to enable variable numbers of args for the same macro name.
							//So I exclude it from non-Windows.
					#ifdef _WINDOWS
							TRACE("BOOL IsPresentTense(CString str) - ENDE - return TRUE %s\n",
								eav->m_strWords[i]);
					#endif //#ifdef _WINDOWS
							return TRUE;
						}
					}
					//delete eav;
				}
			}
			if(pWordNodeCurrent->m_pWordNodeNext==NULL)
				break;
			else
				pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsPresentTense(CString & str) - ENDE - return FALSE\n");
	return FALSE;
}

BOOL IsSimplePast(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(str==ev->m_strPastTense)
					{
						return TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				TRACE("EnglishAuxiliaryVerb!\n");
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					for(int i=8;i<15;i++)
					{
						//TRACE("eav->words[i]: %s\n",eav->words[i]);
						if(str==eav->m_strWords[i])
							return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishAuxiliaryVerb(CString & str)
{
	TRACE("BOOL IsEnglishAuxiliaryVerb(CString & str) - ANFANG\n");
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				TRACE("EnglishAuxiliaryVerb!\n");
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					for(int i=0;i<15;i++)
					{
						//TRACE("eav->words[i]: %s\n",eav->words[i]);
						if(str==eav->m_strWords[i])
							return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	TRACE("BOOL IsEnglishAuxiliaryVerb(CString & str) - ENDE - return FALSE\n");
	return FALSE;
}

BOOL IsEnglishInfinitive(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(str==ev->m_strInfinitive)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishPersonalPronoun(CString & token)
{
	if(token=="I" || token=="you" || token=="he" || token=="she" || 
		token=="it" || token=="is" || token=="we" || token=="they")
		return TRUE;
	return FALSE;
}

BYTE IsEnglishVerb3rdPerson(const CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(str==ev->m_strInfinitive+"s")
					{
						return TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWords[3])
						return TRUE;
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

//ist die übergebene Zeichenkette eine gebeugte Verbform der dritten Person
//Singular ("works","watches","carries","does","says")?
BYTE IsEnglishVerb3rdPersonSingular(const CString & rstrToken,const 
	EnglishVerb & rEnglishVerb)
{
	//wenn das Infinitiv aus mindestens 2 Zeichen besteht
	if(rEnglishVerb.m_strInfinitive.GetLength()>1)
	{
		//wenn die zwei Zeichen ganz rechts "ch"s sind. Beispiel: "watch","match"
		if(rEnglishVerb.m_strInfinitive.Right(2)==_T("ch"))
		{
			//zurückliefern, ob die Zeichenkette mit der gebildeten gebeugten
			//Verbform der dritten Person Singular übereinstimmt. Beispiel für 
			//eine gebildete gebeugte Verbform der dritten Person Singular:
			//"watches","matches"
			return rstrToken==rEnglishVerb.m_strInfinitive+_T("es");
		}
		//wenn das zweite Zeichen von rechts ein Konsonant ist und das Zeichen
		//ganz rechts ein "y" ist. Beispiel: "carry"
		if(IsConsonant(rEnglishVerb.m_strInfinitive.Right(2).Left(1)) && 
			rEnglishVerb.m_strInfinitive.Right(1)==_T("y"))
		{
			//zurückliefern, ob die Zeichenkette mit der gebildeten gebeugten
			//Verbform der dritten Person Singular übereinstimmt. Beispiel für 
			//eine gebildete gebeugte Verbform der dritten Person Singular:
			//"carries"
			return rstrToken==rEnglishVerb.m_strInfinitive.Left(rEnglishVerb.
				m_strInfinitive.GetLength()-1)+_T("ies");
		}
	}
	//wenn das Infinitiv aus mindestens 1 Zeichen besteht
	if(rEnglishVerb.m_strInfinitive.GetLength()>0)
	{
		//wenn das Zeichen ganz rechts ein "o" ist. Beispiel: "go"
		if(rEnglishVerb.m_strInfinitive.Right(1)==_T("o"))
		{
			//zurückliefern, ob die Zeichenkette mit der gebildeten gebeugten
			//Verbform der dritten Person Singular übereinstimmt. Beispiel für 
			//eine gebildete gebeugte Verbform der dritten Person Singular:
			//"goes"
			return rstrToken==rEnglishVerb.m_strInfinitive+_T("es");
		}
	}
	//zurückliefern, ob die Zeichenkette mit der gebildeten gebeugten
	//Verbform der dritten Person Singular übereinstimmt. Beispiel für 
	//eine gebildete gebeugte Verbform der dritten Person Singular:
	//"works", "says"
	return rstrToken==rEnglishVerb.m_strInfinitive+_T("s");
}

//diese Funktion liefert zurück, ob die übergebene Zeichenkette ein dem 
//Programm bekannte
//englische Vokabel ist TRUE/1=bekannt FALSE/0=unbekannt
BYTE IsEnglishWord(const CString & strWord)
{
	//wenn ein Personalpronom (Fürwort)
	if(strWord==_T("I") || strWord==_T("you") || strWord==_T("we") || 
		strWord==_T("they"))
		return TRUE;
	//wenn ein Personalpronom (Fürwort)
	if(strWord==_T("he") || strWord==_T("she") || strWord==_T("it"))
		return TRUE;
	//wenn ein Possesivpronom (besitzanzeigendes Fürwort)
	if(strWord==_T("my") || strWord==_T("your") || strWord==_T("his") || 
		strWord==_T("her") || strWord==_T("its") || strWord==_T("our") || 
		strWord==_T("their"))
		return TRUE;
	//wenn englische Verneinung
	if(strWord==_T("not"))
		return TRUE;
	//wenn englischer Artikel
	if(strWord==_T("a") || strWord==_T("an") || strWord==_T("the"))
		return TRUE;
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
				if(ea!=NULL)
				{
					if(strWord==ea->m_strPositiv || strWord==ea->m_strPositiv+_T("ly") || 
						strWord==ea->m_strComperativ || strWord==ea->m_strSuperlativ || 
						strWord==ea->m_strAdverb)
					{
						return TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(strWord==eav->m_strWord)
					{
						return TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				TRACE("EnglishAuxiliaryVerb!\n");
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					for(int i=0;i<15;i++)
					{
						//TRACE("eav->words[i]: %s\n",eav->words[i]);
						if(strWord==eav->m_strWords[i])
							return TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishConjunction))
			{
				EnglishConjunction * ec=dynamic_cast<EnglishConjunction*>
					(current->m_pWord);
				if(ec!=NULL)
				{
					if(strWord==ec->m_strWord)
						return TRUE;
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishNoun))
			{
				EnglishNoun * en=dynamic_cast<EnglishNoun*>(current->m_pWord);
				if(en!=NULL)
				{
					if(strWord==en->m_strSingular)
					{
						return TRUE;
					}
					if(strWord==en->m_strPlural)
					{
						return TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishPreposition))
			{
				EnglishPreposition * ep=dynamic_cast<EnglishPreposition*>
					(current->m_pWord);
				if(ep!=NULL)
				{
					if(strWord==ep->m_strWord)
						return TRUE;
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishPronoun))
			{
				EnglishPronoun * ep=dynamic_cast<EnglishPronoun*>(current->m_pWord);
				if(ep!=NULL)
				{
					if(strWord==ep->m_strPlural || strWord==ep->m_strSingular)
						return TRUE;
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(strWord==ev->m_strInfinitive || strWord==ev->m_strPastTense ||
						strWord==ev->m_strPastParticiple)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}//Ende von: BYTE IsEnglishWord(const CString & strWord)

BOOL IsEnglishAdjective(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
				if(ea!=NULL)
				{
					if(str==ea->m_strPositiv || str==ea->m_strComperativ || str==
						ea->m_strSuperlativ)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

WordNode * IsEnglishAdjective2(CString & str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
				if(ea!=NULL)
				{
					if(str==ea->m_strPositiv || str==ea->m_strComperativ || str==
						ea->m_strSuperlativ)
					{
						return current;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return NULL;
}

BOOL IsEnglishAdverb(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective))
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
				if(ea!=NULL)
				{
					if(str==ea->m_strPositiv+_T("ly"))
					{
						return TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishAdverb(CString str,BYTE type)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishAdjective) && type=='1')
			{
				EnglishAdjective * ea=dynamic_cast<EnglishAdjective*>(current->m_pWord);
				if(ea!=NULL)
				{
					if(str==ea->m_strPositiv+_T("ly"))
					{
						return TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAdverb))
			{
				EnglishAdverb * eav=dynamic_cast<EnglishAdverb*>(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWord && type==eav->m_bType)
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsEnglishPronoun(CString str)
{
	if(str=="I" || str=="you" || str=="we" || str=="they")
		return TRUE;
	return FALSE;
}

BOOL IsEnglishPronoun3rdPerson(CString str)
{
	if(str=="he" || str=="she" || str=="it")
		return TRUE;
	return FALSE;
}

BOOL IsEnglishPossessivePronoun(CString str)
{
	if(str=="my" || str=="your" || str=="his" || str=="her" || str=="its" || str=="our" || str=="their")
		return TRUE;
	return FALSE;
}

BOOL IsObjectPersonalpronoun(CString token)
{
	if(token=="me" || token=="you" || token=="him" || token=="her" || 
		token=="it" || token=="us" || token=="them")
		return TRUE;
	return FALSE;
}

BOOL IsPastParticiple(CString str)
{
	if(wordList.m_pWordNodeFirst!=NULL)
	{
		WordNode * current=wordList.m_pWordNodeFirst;
		while(1)
		{
			if(typeid(*current->m_pWord)==typeid(EnglishVerb))
			{
				EnglishVerb * ev=dynamic_cast<EnglishVerb*>(current->m_pWord);
				if(ev!=NULL)
				{
					if(str==ev->m_strPastTense)
					{
						//In MFC/Windows the TRACE macro has a variable # of args--I do 
						//not know how to enable variable numbers of args for the same macro name.
						//So I exclude it from non-Windows.
				#ifdef _WINDOWS
						TRACE("str: %s\n",str);
				#endif //#ifdef _WINDOWS
						return TRUE;
					}
				}
			}
			if(typeid(*current->m_pWord)==typeid(EnglishAuxiliaryVerb))
			{
				EnglishAuxiliaryVerb * eav=dynamic_cast<EnglishAuxiliaryVerb*>
					(current->m_pWord);
				if(eav!=NULL)
				{
					if(str==eav->m_strWords[14])
					{
						return TRUE;
					}
				}
			}
			if(current->m_pWordNodeNext==NULL)
				break;
			else
				current=current->m_pWordNodeNext;
		}
	}
	return FALSE;
}

BOOL IsPersonalPronoun(CString token)
{
	if(token=="I" || token=="you" || token=="he" || token=="she" || 
		token=="it" || token=="we" || token=="they")
		return TRUE;
	return FALSE;
}

//liefert zurück, ob das erste Zeichen der Zeichenkette ein Vokal ist
//sollte KEINE Referenz sein, da str verändert wird
BYTE IsVowel(CString str)
{
	char ch;
	str.MakeLower();
	ch=str[0];
	return ch=='a' || ch=='e' || ch=='i' || ch=='o'|| ch=='u';
}

//liefert zurück, ob das erste Zeichen der Zeichenkette ein Vokal ist
//sollte KEINE Referenz sein, da str verändert wird
BYTE IsVowel(TCHAR ch)
{
	ch = tolower(ch);
	return ch=='a' || ch=='e' || ch=='i' || ch=='o'|| ch=='u';
}
