#pragma once //Include guard.

#include <string> //for class std::string
#include <Controller/string_type.hpp> //for typedef VTrans::string_type

//typedef unsigned long DWORD;
#include <windef.h>

class I_UserInterface
{
public:
  I_UserInterface()
    : s_numberOfVocabularyPairs(0),
      m_dictionaryFileLineNumber(0)
  {

  }
  /** avoid g++ warning : "virtual method but non-virtual d'tor" */
  virtual ~I_UserInterface() { }
  unsigned s_numberOfVocabularyPairs;
  volatile unsigned m_dictionaryFileLineNumber;
  virtual void Message(const std::string & cr_stdstr ) = 0;
  virtual void Message(const std::wstring & cr_stdwstr ) = 0;
  virtual void ShowInvalidVocabularyFileFormatMessage(
    const VTrans::string_type & strWordFile,
    DWORD dwOffsetOfBeginOfEntry,
    DWORD dwOffset
    ) = 0;
  /** Intension: for multithreaded translation: called from the translation
   *   thread after translating.
   *  Sample implementation: send a message to the GUI thread to show the
   *   translation result in GUI thread. */
  virtual void UpdateAfterTranslation() {};
//  virtual void DictionaryFileLine(unsigned );
  virtual void EndTimer() { };
  virtual void StartTimer() { };
} ;
//Keep a newline at the end of the file to avoid a g++ warning.
