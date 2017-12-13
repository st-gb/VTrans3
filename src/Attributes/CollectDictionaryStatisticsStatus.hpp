/** File:   CollectDictionaryStatisticsStatus.hpp
 * Author: sg
 * Created on 5. November 2017, 18:59  */

#ifndef COLLECTDICTIONARYSTATISTICSSTATUS_HPP
#define COLLECTDICTIONARYSTATISTICSSTATUS_HPP

#include <hardware/CPU/atomic/AtomicExchange.h>

class CollectDictionaryStatisticsStatus
{
private:
  long m_numberOfBytesCurrentlyProcessed;
  long m_fileSizeInBytes;
public:
  void SetNumberOfBytesCurrentlyProcessed(
    const fastestUnsignedDataType numberOfBytesCurrentlyProcessed)
  {
    /** May be read and written to at the same time->use atomic functions. */
    AtomicExchange( & m_numberOfBytesCurrentlyProcessed, 
      numberOfBytesCurrentlyProcessed);
//    m_numberOfBytesCurrentlyProcessed = numberOfBytesCurrentlyProcessed;
  }
};

#endif /* COLLECTDICTIONARYSTATISTICSSTATUS_HPP */
