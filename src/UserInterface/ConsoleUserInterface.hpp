/** File:   ConsoleUserInterface.hpp
 * Author: sg
 * Created on 15. November 2017, 19:58   */

#ifndef CONSOLEUSERINTERFACE_HPP
#define CONSOLEUSERINTERFACE_HPP

#include "I_UserInterface.hpp"

class ConsoleUserInterface
  : public I_UserInterface
{
public:
//  ConsoleUserInterface() {}
//  virtual ~ConsoleUserInterface() {}
  
  void Message(const std::string & cr_stdstr );
  void Message(const std::wstring & cr_stdwstr );
  void ShowInvalidVocabularyFileFormatMessage(
    const VTrans::string_type & strWordFile,
    DWORD dwOffsetOfBeginOfEntry,
    DWORD dwOffset
    );
};

#endif /* CONSOLEUSERINTERFACE_HPP */
