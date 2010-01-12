#pragma once //incl. guard

#include <string>

  class IVocabularyInMainMem
  {
  public:
    //Make methods pure virtual ("virtual [...] = 0" ) to avoid
    //linker error "LNK 2001" in MSVC++.
    virtual BYTE GetGermanArticle() = 0 ;
    virtual std::string GetGermanPlural() = 0 ;
    virtual WORD GetNumberOfNouns() = 0 ;
    virtual BYTE IsSingular() = 0 ;
    virtual BYTE GetTranslationType() = 0 ;
    virtual std::string GetGermanSingular() = 0 ;
    virtual void NextNoun() = 0 ;
  };
