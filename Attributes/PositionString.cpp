/*
 * PositionString.cpp
 *
 *  Created on: 18.12.2013
 *      Author: mr.sys
 */
#include "PositionString.hpp" //class PositionStringVector
#include <iostream> //class std::ostream
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)

  const char * const PositionStringVector::s_comparisonResultString [] = {
    "fullMatchOfCompareVector", "match", "lower", "greater", "tooFewTokens"
  };

  /** @see http://msdn.microsoft.com/en-us/library/1z2f6c2k.aspx */
  std::ostream & operator << (std::ostream& os, const PositionStringVector & psv)
  {
    PositionStringVector::const_iterator c_iter = psv.begin();
    while( c_iter != psv.end() )
    {
      const PositionString & ps = * c_iter;
      os << ps.m_Str << " ";
      ++ c_iter;
    }
    return os;
  }

  enum PositionStringVector::cmp PositionStringVector::Compare(
    const PositionStringVector & psvCompare,
    const unsigned indexOf1stToken,
    unsigned numTokens
    )
    const
  {
//    fastestUnsignedDataType numTokens = indexOfLastToken - indexOf1stToken + 1;
#ifdef _DEBUG
    const int ownSize = size();
    const int cmpSize = psvCompare.size();
    LOGN_DEBUG( "comparing \"" << *this << "\" beginning from token #"
      << indexOf1stToken //<< "to " << indexOfLastToken
      << "(# tokens:"
      << numTokens << "); and \"" << psvCompare << "\"")
#endif
    const_iterator c_iter = begin() + indexOf1stToken, c_iterCmp = psvCompare.begin();
    bool prefixMatches = false;
    while( c_iter != end() && c_iterCmp != psvCompare.end() && numTokens)
    {
      if( (* c_iter).m_Str == c_iterCmp->m_Str )
      {
        ++ c_iter;
        ++ c_iterCmp;
        prefixMatches = true;
      }
      else
      {
        bool isGreater = (* c_iter).m_Str > c_iterCmp->m_Str;
        return isGreater ? greater : lower ;
      }
      -- numTokens;
    }
    if( numTokens)
    {
      /** e.g. from token #3 (2 tokens) "the car still works" , dict: "works pension" */
      if( c_iter == end() )
        return lower;
      else
        return tooFewTokens;
    }
    else
    {
      if( c_iter == end() )
      {
        /** Reached the end of the comparison vector. */
        if(c_iterCmp == psvCompare.end() )
          return match;
        else return lower;
      }
      else
      {
        /** Reached the end of the comparison vector. */
        if(c_iterCmp == psvCompare.end() )
        {
          if( prefixMatches )
            return match;
          else
            return greater;
        }
        else
        {
          if( ! numTokens ) /** e.g. 1 from token 1 from "the car still works", in dict:
            "car production" */
            return lower; /** e.g. "car" is < "car production" */
          /** e.g. "car" in "the car still works" matches "car production" */
          if(prefixMatches)
            return prefixMatch;
        }
      }
      return match;
    }
  }
