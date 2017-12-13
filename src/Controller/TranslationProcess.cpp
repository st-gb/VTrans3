/** File:   TranslationProcess.cpp
 * Author: sg
 * Created on 5. November 2017, 14:54  */

#include "TranslationProcess.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h>

TranslationProcess::TranslationProcess() 
  : m_vbContinue(true)
{
}

TranslationProcess::~TranslationProcess() {
}

enum VTrans::StatusCode TranslationProcess::GetStatus( /*const std::string & str*/
  std::string & str, struct tm & time)
{
  LOGN_DEBUG("begin")
  VTrans::StatusCode statusCode;

  m_critSecStatus.Enter();
  if( str == "dictionaryStatistics" )
  {

  }
  else
  {
    m_currentStatus.GetItem(str);
    statusCode = m_currentStatus.GetCode();
    m_currentStatus.GetTime(time);
  }
  m_critSecStatus.Leave();
  LOGN_DEBUG("return status code:" << statusCode << " " << str)
  return statusCode;
}

unsigned TranslationProcess::GetStatus2( /*const std::string & str*/
  const std::string & str, struct tm & time, ByteArray & byteArray
  //void * pData
  )
{
  LOGN_DEBUG("begin")
  VTrans::StatusCode statusCode;

  if( str == "dictionaryStatistics" )
  {
    const unsigned sizeInBytes = sizeof(m_collectDictionaryStatisticsStatus);
    unsigned char * bytes = new unsigned char[sizeInBytes];
    m_critSecStatus.Enter();
    memcpy(bytes, & m_collectDictionaryStatisticsStatus, sizeInBytes);
    m_critSecStatus.Leave();
    byteArray.add(bytes, sizeInBytes);
  }
  else
  {
    m_critSecStatus.Enter();
//    m_currentStatus.GetItem(str);
    statusCode = m_currentStatus.GetCode();
    m_currentStatus.GetTime(time);
    m_critSecStatus.Leave();
  }
  LOGN_DEBUG("return status code:" << statusCode << " " << str)
  return statusCode;
}

void TranslationProcess::SetStatus(
  enum VTrans::StatusCode statusCode,
//  const std::string & str
  const char * const pch
  )
{
  LOGN_DEBUG("begin--status code:" << statusCode << " " << pch)
  m_critSecStatus.Enter();
  m_currentStatus.Set(statusCode, /*str*/ pch);
  m_critSecStatus.Leave();
}
