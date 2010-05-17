#include "StdAfx.h"
#include "Word.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordNode.hpp"
#include "VocabularyInMainMem/DoublyLinkedList/WordList.hpp"

#include <typeinfo> //for typeid()
#include "IO.h" //for "::LoadWords(...)"
//#include "MainFrm.h"
#ifdef _WINDOWS
	#include <typeinfo.h>
#endif

//"ofstreamLogFile" should be declared in the module that starts the translation:
//Either the TransApp constructor or a DLL module.
extern std::ofstream ofstreamLogFile;
//extern VocNode * m_first;
extern WordNode * m_first ;
extern WordList wordList;

BOOL Word::operator == (Word * pWordCompare)
{
	if(typeid(*this) == typeid(*pWordCompare))
	{
		if(typeid(*this)==typeid(EnglishNoun))
		{
			EnglishNoun * en1 = dynamic_cast<EnglishNoun*>(this);
			EnglishNoun * en2 = dynamic_cast<EnglishNoun*>(pWordCompare);
			if(en1 != NULL && en2 != NULL )
			{
				if(en1->m_strSingular == en2->m_strSingular)
				{
					if(en1->m_strPlural == en2->m_strPlural)
					{
						return TRUE;
					}
				}
			}
		}
		if(typeid(*this)==typeid(EnglishVerb))
		{
			EnglishVerb * ev1=dynamic_cast<EnglishVerb*>(this);
			EnglishVerb * ev2=dynamic_cast<EnglishVerb*>(pWordCompare);
			if(ev1 !=NULL && ev2!=NULL)
			{
				if(ev1->m_strInfinitive==ev2->m_strInfinitive)
				{
					if(ev1->m_strPastTense==ev2->m_strPastTense)
					{
						if(ev1->m_strPastParticiple==ev2->m_strPastParticiple)
						{
							return TRUE;
						}
					}
				}
			}
		}
		if(typeid(*this)==typeid(EnglishAdjective))
		{
			EnglishAdjective * ea1=dynamic_cast<EnglishAdjective*>(this);
			EnglishAdjective * ea2=dynamic_cast<EnglishAdjective*>(pWordCompare);
			if(ea1!=NULL && ea2!=NULL)
			{
				if(ea1->m_strPositiv==ea2->m_strPositiv && ea1->m_strComperativ==
					ea2->m_strComperativ &&	ea1->m_strSuperlativ==ea2->m_strSuperlativ 
					&& ea1->m_strAdverb==ea2->m_strAdverb)
					return TRUE;
			}
		}
		if(typeid(*this)==typeid(EnglishAdverb))
		{
			EnglishAdverb * ea1=dynamic_cast<EnglishAdverb*>(this);
			EnglishAdverb * ea2=dynamic_cast<EnglishAdverb*>(pWordCompare);
			if(ea1 !=NULL && ea2!=NULL)
			{
				if(ea1->m_strWord==ea2->m_strWord && ea1->m_bType==ea2->m_bType)
				{
					return TRUE;
				}
			}
		}
		if(typeid(*this)==typeid(GermanNoun))
		{
			GermanNoun * gn1=dynamic_cast<GermanNoun*>(this);
			GermanNoun * gn2=dynamic_cast<GermanNoun*>(pWordCompare);
			if(gn1 !=NULL && gn2!=NULL)
			{
				if(gn1->m_strSingular==gn2->m_strSingular && gn1->m_strPlural==
					gn2->m_strPlural &&	gn1->m_bArticle==gn2->m_bArticle)
					return TRUE;
			}
		}
		if(typeid(*this)==typeid(GermanVerb))
		{
			GermanVerb * gv1=dynamic_cast<GermanVerb*>(this);
			GermanVerb * gv2=dynamic_cast<GermanVerb*>(pWordCompare);
			if(gv1!=NULL && gv2!=NULL)
			{
				for(BYTE b=0;b<16;b++)
					if(gv1->m_strWords[b]!=gv2->m_strWords[b])
						return FALSE;
				if(gv1->m_bAuxiliaryVerb!=gv2->m_bAuxiliaryVerb)
					return FALSE;
				if(gv1->m_bBe!=gv2->m_bBe)
					return FALSE;
				if(gv1->m_bCase!=gv2->m_bCase)
					return FALSE;
				if(gv1->m_bMove!=gv2->m_bMove)
					return FALSE;
				return TRUE;
			}
		}
		if(typeid(*this)==typeid(GermanAdjective))
		{
			GermanAdjective * ga1=dynamic_cast<GermanAdjective*>(this);
			GermanAdjective * ga2=dynamic_cast<GermanAdjective*>(pWordCompare);
			if(ga1!=NULL && ga2!=NULL)
			{
				if(ga1->m_strPositiv==ga2->m_strPositiv && ga1->m_strComperativ==
					ga2->m_strComperativ &&	ga1->m_strSuperlativ==ga2->m_strSuperlativ 
					&& ga1->m_strWortstamm==ga2->m_strWortstamm)
					return TRUE;
			}
		}
		if(typeid(*this)==typeid(GermanAdverb))
		{
			GermanAdverb * ga1=dynamic_cast<GermanAdverb*>(this);
			GermanAdverb * ga2=dynamic_cast<GermanAdverb*>(pWordCompare);
			if(ga1!=NULL && ga2!=NULL)
			{
				if(ga1->m_strWord==ga2->m_strWord)
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//EnglishAuxiliaryVerb::EnglishAuxiliaryVerb(){m_bIntegral=FALSE;}

EnglishAuxiliaryVerb::EnglishAuxiliaryVerb( 
  //const CString & str,
  const VTrans::string_type & str,
	bool bModalAuxiliary
  )
{
	int i=0;
	int start=0;
	WORD wIndex=0;
	m_bIntegral=TRUE;
	while( i < str.length() )
	{
		if( str[i] == '\n' )
		{
			m_strWords[wIndex++] = //str.Mid(start,i-start);
        str.substr( start, i - start ) ;
			start = i + 1 ;
		}
		i++ ;
	}
	//ein modales Hilfsverb wird NICHT f�r die �berstzung 
	//unbedingt ben�tigt f�r einige Zeitformen (im Gegensatz
	//zu "to be" f�r fortschreitenden Pr�sens ("I am working.")
	m_bIntegral = ! bModalAuxiliary ;
}

//void EnglishAuxiliaryVerb::InitGetNextString( )
//{
//  m_byIndex = 0 ;
//}

bool EnglishAuxiliaryVerb::GetNextString( std::string & r_stdstr )
{
  bool bSucc = false ;
  if( m_byIndex < NUMBER_OF_ENGLISH_AUX_STRINGS )
  {
    m_strWords [ m_byIndex ++ ] ;
    bSucc = true ;
  }
  return bSucc ;
}

BYTE EnglishAuxiliaryVerb::GetWordClass()
{
  return auxiliary_verb ;
}

//inline
void EnglishVerb::Get3rdPersonForm(
  VTrans::string_type & r_vtransstr )
{
  WORD wStringSize = r_vtransstr.size() ;
  if ( wStringSize > 2 )
  {
    VTrans::string_type vtransstrEnding = r_vtransstr.substr(
      wStringSize - 2 ) ;
   if( vtransstrEnding == //"wish->wishes"
       "sh" || vtransstrEnding ==
           //"switch->switches"
       "ch"
     )
   {
     r_vtransstr += "es" ;
     return ;
   }
  }
  else
    if ( wStringSize > 1 )
    {
      VTrans::string_type vtransstrEnding = r_vtransstr.substr(
        wStringSize - 1 ) ;
      if( vtransstrEnding ==
          //"go->goes"
          "o"
        )
      {
        r_vtransstr += "es" ;
        return ;

      }
    }
  r_vtransstr += "s" ;
}

GermanAuxiliaryVerb::GermanAuxiliaryVerb(const VTrans::string_type & str)
{
	int i=0;
	int start=0;
	WORD wIndex=0;
	m_bIntegral=TRUE;
	while( i < str.length() )
	{
		if(str[i]=='\n')
		{
			m_strWords[wIndex++] = //str.Mid(start,i-start);
        str.substr( start ,i - start );
			start = i + 1;
		}
		i++;
	}
}

void GermanVerb::CreateFromString(const VTrans::string_type & str)
{
	int i=0;
	int start=0;
	WORD wIndex=0;
	m_bIntegral=TRUE;
	while( i < str.length() )
	{
		if( wIndex == 14 )
			m_bCase = str[i] - 50 ;
		if( str[i] == '\n' )
		{
			m_strWords[wIndex++] = //str.Mid(start,i-start);
        str.substr(start,i-start);
			start=i+1;
		}
		i++;
	}
	m_bAuxiliaryVerb=TRUE;
	m_bReflexive=FALSE;
}

GermanVerb::GermanVerb(const VTrans::string_type & str)
{
  CreateFromString(str) ;
}

GermanVerb::GermanVerb(const VTrans::string_type & str , e_case cas)
{
  CreateFromString(str) ;
  m_bCase = cas ;
}

void InitDictionary()
{
	WordNode * pWordNodeCurrent=NULL;
	WordNode * pWordNodePrevious=NULL;
	Word * pWord=NULL;
#ifdef _DEBUG
	printf("void LoadWords(WordNode * pWordNode) ANFANG\n");
#endif
//	//zuerst die integralen Vokabeln der verketteten Liste hinzuf�gen, Anfang
//	wordList.m_pWordNodeFirst = new WordNode();
//	wordList.m_pWordNodeFirst->m_pWord = new EnglishAuxiliaryVerb(
//		"be\nam\nare\nis\nare\nare\nare\nwas\nwere\n"
//		"was\nwere\nwere\nwere\nbeen\n",false);
//
//	//current=voclist.m_first;
//	wordList.m_pWordNodeFirst->m_pWordNodePrevious=NULL;
//	pWordNodeCurrent=wordList.m_pWordNodeFirst;
	//delete voc;
	//voc=new GermanVerb("sein\nbin\nbist\nist\nsind\nseid\nsind\nwar\nwarst\nwar\nwaren\nward\nwaren\ngewesen\n");
	//voclist.Append(voc);
	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	GermanVerb gv=GermanVerb("sein\nbin\nbist\nist\nsind\nseid\nsind\nwar\nwarst\nwar\nwaren\nward\nwaren\ngewesen\n");
	gv.m_bCase=4;
	gv.m_bMove=0;
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new GermanVerb(gv);
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;
	//delete voc;
	//voc=new EnglishAuxiliaryVerb("have\nhave\nhave\nhas\nhave\nhave\nhave\nhad\nhad\nhad\nhad\nhad\nhad\nhad\n");
	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new 
		EnglishAuxiliaryVerb("have\nhave\nhave\nhas\nhave\nhave\n"
		"have\nhad\nhad\nhad\nhad\nhad\nhad\nhad\n",false);
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new GermanVerb("haben\nhabe\nhast\nhat\nhaben\nhabt\nhaben\nhatte\nhattest\nhatte\nhatten\nhattet\nhatten\ngehabt\n4");
	//pWordNodeCurrent->m_pWordNodeNext->m_pWord->m_bMove=0;
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new 
		EnglishAuxiliaryVerb("will\nwill\nwill\nwill\nwill\nwill\n"
		"will\nwill\nwill\nwill\nwill\nwill\nwill\nwill\n",false);
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new GermanVerb(
		"werden\nwerde\nwirst\nwird\nwerden\nwerdet\nwerden\nwurde\n"
		"wurdest\nwurde\nwurden\nwurdet\nwurden\ngeworden\n");
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new 
		EnglishAuxiliaryVerb("do\ndo\ndo\ndoes\ndo\ndo\ndo\ndid\n"
		"did\ndid\ndid\ndid\ndid\ndone\n",false);
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

	pWordNodePrevious=pWordNodeCurrent;
	pWordNodeCurrent->m_pWordNodeNext=new WordNode();
	pWordNodeCurrent->m_pWordNodeNext->m_pWord=new 
		GermanVerb("machen\nmache\nmachst\nmacht\nmachen\nmacht\n"
		"machen\nmachte\nmachtest\nmachte\nmachten\nmachtet\n"
		"machten\ngemacht\n");
	pWordNodeCurrent=pWordNodeCurrent->m_pWordNodeNext;
	pWordNodeCurrent->m_pWordNodePrevious=pWordNodePrevious;

  //::LoadWords(pWordNodeCurrent) ;
}
