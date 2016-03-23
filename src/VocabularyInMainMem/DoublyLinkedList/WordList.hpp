#pragma once

#include <string>
#include "../I_WordSearch.hpp"

class WordNode ;

class WordList 
  : public //IVocabularyInMainMem
  I_WordSearch
{
private : 
  WordNode * m_pwordnodeCurrentOccurance ;
public:
	WordNode * m_pWordNodeFirst;
	WordNode * m_pWordNodeLast;
	void Append(Word *);
  void Delete() ;
  //void 
  BYTE GetFirstOccurance(
    const std::string & cr_stdstr
    //, Word & r_wordEnglish
    //, Word & r_wordGerman
    , AutomDelWord & r_automdelwordEng
    , AutomDelWord & r_automdelwordGer
    ) 
  {
    return 0 ;
  }
  //BYTE 
  Word * GetFirstOccurance(
    const std::string & str 
    , Word ** p_wordEnglish
    , Word ** p_wordGerman
    ) ;
  Word * GetPreviousOccurance(
    const std::string & str
    , Word ** p_wordEnglish
    , Word ** p_wordGerman
    ) ;
  //BYTE 
  Word * GetNextOccurance(
    //const CString & str 
    const std::string & str
    //, Word & * p_wordEnglish
    //, Word * p_wordGerman
    , Word ** p_wordEnglish
    , Word ** p_wordGerman
    ) ;
	WordList()
  {
    m_pWordNodeFirst = NULL;
    m_pWordNodeLast = NULL;
    m_pwordnodeCurrentOccurance = NULL ;
  }
  ~WordList();
};
