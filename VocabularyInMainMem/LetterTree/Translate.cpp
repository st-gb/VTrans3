//Ohne das folgende include/wenn Pfad in den Eigenschaften dieser Datei 
//unter C/C++->vorkompilierte Header->"PHC durch..." falsch ist: 
//"fatal error C1010: Unerwartetes Dateiende w�hrend der Suche nach dem vorkompilierten Header.[...]"
#include "../../StdAfx.h"
#include "../../SentenceElement.h"
#include "../../Translate.h" //for ::TranslateENR(...)
//#include "Translate.h"
#include <vector>

void NSLetterTree::Translate(
  //May not be const because the last param of "TranslateENR" is not const.
  //const 
  Subject & subject,
  std::vector<std::vector<std::string>> & vecvecstr
  )
{
  BYTE by1 = 1 , 
    by0 = 0, 
    byFirstTokenInSentence = (BYTE)(vecvecstr.size() == 0) ;
  EnglishNounRecord enr ;
  int n0 = 0 ;
  std::vector<EnumerationElement *>:://const_iterator 
    iterator iterenumele ;
  std::vector<VTrans_string_typeVector>::iterator iterveccstring ;
  std::vector<CString>::iterator itercstring ;
  std::vector<VTrans_string_typeVector> vecveccstring ;
  for(iterenumele = subject.m_vecpenumerationelement.begin() ;
    iterenumele != subject.m_vecpenumerationelement.end(); iterenumele++)
  {
    if(//iterenumele->m_engnounslettertree.size() > 0 && 
      (*iterenumele)->m_engnounslettertree.m_psetpvocandtransl )
    {
      //EngNounsLetterTree engnounlt ;
      vecveccstring = ::TranslateENR(
        &enr,
        0,//FALSE,
        false,
        false,
        by1,
        //May the enum ele the first token in sentence?.
	      //by0,
        byFirstTokenInSentence ,
        by0,
        n0,
        (*iterenumele)->m_engnounslettertree) ;
      for(iterveccstring = vecveccstring.begin() ;
        iterveccstring != vecveccstring.end(); iterveccstring ++)
      {
        std::vector<string> vecstr ;
        for(itercstring = iterveccstring->begin() ;
          itercstring != iterveccstring->end(); itercstring ++)
        {
          vecstr.push_back((LPCSTR)*itercstring) ;
        }
        vecvecstr.push_back(vecstr);
      }
    }
  }
}

  //TODO implement this method:
//NSLetterTree::Translate(const Object & object,BYTE Case,
//	BOOL bNegation,BYTE & bFirstTokenInSentence,//EnglishView * pEV,
//  std::vector<IntPair> & rvecintpair,int & nStartID)
//{
//
//}

void NSLetterTree::Translate(
  const Clause & clause,
  std::vector<std::vector<std::string>> & vecvecstr
  )
{
  if(clause.m_pSubject)
  {
    Translate(*clause.m_pSubject,vecvecstr) ;
  }
}

void NSLetterTree::Translate(
  const Sentence & sentence, 
  std::vector<std::vector<std::string>> & vecvecstr
  )
{
  std::vector<Clause>::const_iterator iter ;
  for(iter = sentence.m_vecClause.begin() ;iter != sentence.
    m_vecClause.end(); iter++)
  {
    Translate(*iter,vecvecstr) ;
  }
}