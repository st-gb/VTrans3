#pragma once //Include guard

class Word ;
class AutomDelWord ;

//This class should be implemented by classes that store vocabulary entries
//AND also should enable a word lookup for multiple entries with the same string
//(e.g. if there are 2 nouns with string "love" (noun and verb), then both entries
//should be retrievable.
class I_WordSearch
{
  //If e.g. someone searches inside the GUI for words.
  //virtual BYTE GetNextOccurance(const CString & str ) = 0 ;
  virtual //void 
    //0=failure (no vocable with the string found)
    BYTE GetFirstOccurance(
    const std::string & cr_stdstr
    //, Word & r_wordEnglish
    //, Word & r_wordGerman
    //, Word * & p_wordEnglish
    //, Word * & p_wordGerman
    , AutomDelWord & r_automdelwordEng
    , AutomDelWord & r_automdelwordGer
    ) = 0 ;
  virtual //BYTE 
    Word * GetNextOccurance(
    //const CString & str
    const std::string & str
    , Word ** p_wordEnglish
    , Word ** p_wordGerman
    ) = 0 ;
  virtual //BYTE 
    Word * GetPreviousOccurance(
    //const CString & str
    const std::string & str
    , Word ** p_wordEnglish
    , Word ** p_wordGerman
    ) = 0 ;
};
