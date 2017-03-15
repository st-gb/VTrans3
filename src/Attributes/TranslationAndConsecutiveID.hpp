/* 
 * File:   TranslationAndConsecutiveID.hpp
 * Author: stefan
 *
 * Created on May 8, 2013, 12:26 PM
 */

#ifndef TRANSLATIONANDCONSECUTIVEID_HPP
#define	TRANSLATIONANDCONSECUTIVEID_HPP

#include <hardware/CPU/fastest_data_type.h> //typedef fastestUnsignedDataType

typedef unsigned short WORD ;

class TranslationAndConsecutiveID
{
public:
  typedef /*WORD*/ fastestUnsignedDataType ID_data_type;
  
  TranslationAndConsecutiveID(
    std::string & r_stdstrTranslation ,
    ID_data_type consecutiveID
    )
  {
    m_stdstrTranslation = r_stdstrTranslation ;
    m_wConsecutiveID = consecutiveID ;
  }
  std::string m_stdstrTranslation ;
  ID_data_type m_wConsecutiveID ;
} ;

#endif	/* TRANSLATIONANDCONSECUTIVEID_HPP */

