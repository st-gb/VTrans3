/*
 * PositionString.hpp
 *
 *  Created on: 31.03.2012
 *      Author: Stefan
 */

#ifndef POSITIONSTRING_HPP_
#define POSITIONSTRING_HPP_

  #include <vector> //class std::vector
  #include <Controller/string_type.hpp> //typedef VTrans::string_type
  #include <windef.h> //for types DWORD, BYTE (,...)

  class PositionString
  {
  public:
    BYTE m_color;
    DWORD m_dwLength;
    DWORD m_dwStart;
    VTrans::string_type m_Str;
    PositionString(VTrans::string_type str,DWORD dwStart,DWORD dwLength)
    {
      m_Str = str;
      m_dwStart = dwStart;
      m_dwLength = dwLength;
    };
  };

  typedef std::vector<PositionString> PositionStringVector;

#endif /* POSITIONSTRING_HPP_ */
