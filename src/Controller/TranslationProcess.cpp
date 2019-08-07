/** File:   TranslationProcess.cpp
 * Author: sg
 * Created on 5. November 2017, 14:54  */

#include "TranslationProcess.hpp"
//#include "BilingualCorpusProcessingBase.hpp"
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
    statusCode = m_currentStatus.GetCode();
    m_critSecStatus.Leave();
    byteArray.add(bytes, sizeInBytes);
    delete [] bytes;
  }
  else  if( str == "loadDictionary" )
  {
    const unsigned sizeInBytes = sizeof(int);
    unsigned char * bytes = new unsigned char[sizeInBytes];
    m_critSecStatus.Enter();
    //TODO does not get updated progress value when written from another thread
    const int progress = m_currentStatus.GetProgress();
    memcpy(bytes, & progress, sizeInBytes);
//    NcursesUI::s_p_NcursesUI->GetBottumUpParser().s_dictReaderAndVocAccess.m_dictReader.
    statusCode = m_currentStatus.GetCode();
    m_critSecStatus.Leave();
    byteArray.add(bytes, sizeInBytes);
    delete [] bytes;
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
  ///Concrete step/more detailled description of \param statusCode.
  const char * const pch,
  ///E.g. for creating the parse tree: the number of parse rules applied.
  signed progress
  )
{
  LOGN_DEBUG("begin--status code:" << statusCode << " (as string:" <<
    VTrans::g_statusMessages[statusCode] << ") state:" << progress << pch)
  m_critSecStatus.Enter();
  m_currentStatus.Set(statusCode, /*str*/ pch, progress);
  m_critSecStatus.Leave();
}
