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
//#include <iostream> //std::cout

  class PositionString
  {
  public:
    BYTE m_color;
    DWORD m_dwLength;
    DWORD m_dwStart;
    VTrans::string_type m_Str;
    PositionString(const VTrans::string_type & str, DWORD dwStart, DWORD dwLength)
    {
      m_Str = str;
      m_dwStart = dwStart;
      m_dwLength = dwLength;
    };
  };

//  typedef std::vector<PositionString> PositionStringVector;

  class PositionStringVector
    : public std::vector<PositionString>
  {
  public:
//    PositionStringVector(const VTrans::string_type & str);
    enum cmp { notSet, fullMatchOf2nd, match, lower, greater,
      tooFewTokensInComparisonVector, prefixMatch };
    static const char * const s_comparisonResultString [];
    /** @see http://msdn.microsoft.com/en-us/library/1z2f6c2k.aspx */
    friend std::ostream & operator << (std::ostream& os, const PositionStringVector &);
    enum cmp Compare(const char * const p_ch) const
    {
      const_iterator c_iter = begin();
      while( c_iter != end() )
      {
//        while( (* c_iter).m_Str. )
        {

        }
        ++ c_iter;
      }
      return match;
    }
    enum cmp Compare(const PositionStringVector & psvCompare) const
    {
      const_iterator c_iter = begin(), c_iterCmp = psvCompare.begin();
      while( c_iter != end() && c_iterCmp != psvCompare.end() )
      {
        if( (* c_iter).m_Str == c_iterCmp->m_Str )
        {
          ++ c_iter;
          ++ c_iterCmp;
        }
        else
        {
          bool isGreater = (* c_iter).m_Str > c_iterCmp->m_Str;
          return isGreater ? greater : lower ;
        }
      }
      if( c_iter == end() )
      {
        if(c_iterCmp == psvCompare.end() )
          return match;
        else return lower;
      }
      else
        if(c_iterCmp == psvCompare.end() )
          return greater;
      return match;
    }
    enum cmp Compare(const PositionStringVector & psvCompare,
      const unsigned indexOf1stToken,
      //TODO make parameter const?!
      unsigned numTokens) const;
    PositionStringVector GetBetween(
//      const PositionStringVector & sv,
      const int first, const int last ) const;
    std::string GetBetweenAsStdString(const int first, const int last) const;
  };


#endif /* POSITIONSTRING_HPP_ */
