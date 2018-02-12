#include "ConsoleUserInterface.hpp"
#include <iostream> //class std::cout

void ConsoleUserInterface::Message(const std::string & cr_stdstr )
{
  std::cout << cr_stdstr << std::endl;
}

void ConsoleUserInterface::Message(const std::wstring & cr_stdwstr )
{
  std::wcout << cr_stdwstr << std::endl;  
}

void ConsoleUserInterface::ShowInvalidVocabularyFileFormatMessage(
  const VTrans::string_type & strWordFile,
  DWORD dwOffsetOfBeginOfEntry,
  DWORD dwOffset
  )
{
  std::cout << "in file \"" << strWordFile << "\": invalid format at index " 
    << dwOffsetOfBeginOfEntry << std::endl;    
}


