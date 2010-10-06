#pragma once //Include guard.

#include <string> //for class std::string

class I_UserInterface
{
public:
  virtual void Message(const std::string & cr_stdstr ) = 0;
  virtual void Message(const std::wstring & cr_stdwstr ) = 0;
} ;
//Keep a newline at the end of the file to avoid a g++ warning.
