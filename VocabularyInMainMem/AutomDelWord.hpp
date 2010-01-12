#pragma once

#include "Word.hpp"

class AutomDelWord
{
  Word * mp_word ;
public:
  AutomDelWord()
    : mp_word (NULL)
  {
  }

  ~AutomDelWord()
  {
    if( mp_word )
      delete mp_word ;
  }
  
  Word * GetWord()
  {
    return mp_word ;
  }
  void SetWord(Word * p_word )
  {
    mp_word = p_word ;
  }
};
