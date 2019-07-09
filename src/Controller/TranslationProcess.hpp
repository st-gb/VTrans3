/** File:   TranslationProcess.hpp
 * Author: sg
 * Created on 5. November 2017, 14:54  */

#ifndef TRANSLATIONPROCESS_HPP
#define TRANSLATIONPROCESS_HPP

#include <OperatingSystem/multithread/nativeCriticalSectionType.hpp>
#include "CurrentStatus.hpp"
#include <data_structures/ByteArray.hpp>
#include <Attributes/CollectDictionaryStatisticsStatus.hpp>

class TranslationProcess {
  nativeCriticalSection_type m_critSecStatus;
  //TODO check if volatile is sufficient or better use atomic function Compare and swap
  volatile bool m_vbContinue;
  VTrans::CurrentStatus m_currentStatus;
public:
  TranslationProcess();
  virtual ~TranslationProcess();
  
  CollectDictionaryStatisticsStatus m_collectDictionaryStatisticsStatus;
  /** (Sub class) implementation may set title of a window to the status passed. */
//  virtual void SetStatus(enum VTrans::StatusCode, /*const std::string & str*/
//    const char * const);
  enum VTrans::StatusCode GetStatus( /*const std::string & str*/
    std::string &, struct tm &);
  unsigned GetStatus2( /*const std::string & str*/
    const std::string & str, struct tm & time, ByteArray & byteArray
    /*void * pData*/);
  void SetStatus(
    enum VTrans::StatusCode statusCode,
  //  const std::string & str
    const char * const pch,
    signed progress
    );
  void SetContinue(bool b) { 
    //TODO is all memory freed when the translation is cancelled?
    m_vbContinue = b;
    }
  bool Continue() const { return m_vbContinue; }
private:

};

#endif /* TRANSLATIONPROCESS_HPP */

