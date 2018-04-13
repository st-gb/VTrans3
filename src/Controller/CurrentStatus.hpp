/** File:   CurrentStatus.hpp
 * Author: sg
 * Created on 5. November 2017, 15:51  */

#ifndef CURRENTSTATUS_HPP
#define CURRENTSTATUS_HPP

#include <OperatingSystem/time/GetCurrentTime.hpp>
#include <string> //class std::string

namespace VTrans
{
  enum StatusCode {not_set, lookUpWordInDictBeginningFromTokenIndex,
	translationRules, transformParseTree, generateXMLtreeFromParseTree,
        loadDictionary
    };
  const char * const g_statusMessages [] = { "not set", 
    "lookUpWordInDictBeginningFromTokenIndex", "translationRules", 
    "transformParseTree", "generateXMLtreeFromParseTree", "load dictionary"};
  class CurrentStatus
  {
    private:
    StatusCode m_code;
    /** the current item, e.g. for looking up in dictionary the word that is
      *  looked up. */
    std::string m_item;
    struct tm m_time;
    /** The percentage of the progress is calculated by m_progress:MAX_INT 
      *  If the progress can't be determined it should be set to < 0. */
    signed m_progress;
    public:
    CurrentStatus() : m_code(not_set)
    {

    }
    StatusCode GetCode() { return m_code; }
    void GetItem(std::string & str ) { str = m_item; }
    void GetTime(struct tm & time ) { time = m_time; }
    void Set(StatusCode code, const char * const pch, signed progress)
    {
      m_item = std::string(pch);
      m_code = code;
      m_progress = progress;
      OperatingSystem::GetCurrentTime(m_time);
    }
  };
}

#endif /* CURRENTSTATUS_HPP */
