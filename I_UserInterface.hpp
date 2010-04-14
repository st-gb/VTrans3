#pragma once 

#include <string> //for class std::string

class I_UserInterface
{
public:
  virtual void Message(std::string str ) = 0;
} ;
//Keep a newline at the end of the file to avoid a g++ warning.
