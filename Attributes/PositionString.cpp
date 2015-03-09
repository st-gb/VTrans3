/*
 * PositionString.cpp
 *
 *  Created on: 18.12.2013
 *      Author: mr.sys
 */
#include "PositionString.hpp" //class PositionStringVector
#include <iostream> //class std::ostream
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)
// SUPPRESS_UNUSED_VARIABLE_WARNING(...)
#include <compiler/GCC/suppress_unused_variable.h>

  const char * const PositionStringVector::s_comparisonResultString [] = {
    "not set", "fullMatchOfCompareVector", "match", "lower", "greater", "tooFewTokens",
    "prefixMatch"
  };

//  inline void OutputAndIncrement(PositionStringVector::const_iterator & c_iter,
//    std::ostream & os )
//  {
//    const PositionString & ps = * c_iter;
//    os << ps.m_Str;
//    ++ c_iter;
//  }

  /** @see http://msdn.microsoft.com/en-us/library/1z2f6c2k.aspx */
  std::ostream & operator << (std::ostream & std_ostream,
      const PositionStringVector & psv)
  {
    PositionStringVector::const_iterator c_iter = psv.begin();
    if( c_iter != psv.end() )
    {
      const PositionString & positionString = * c_iter;
      std_ostream << positionString.m_Str;
      ++ c_iter;
      while( c_iter != psv.end() )
      {
        const PositionString & positionString = * c_iter;
        std_ostream  << " " << positionString.m_Str;
        ++ c_iter;
      }
    }
    return std_ostream;
  }

  /** @return greater: if token inside _this_ object would appear after
   *   @see psvCompare in a dictionary. */
  enum PositionStringVector::cmp PositionStringVector::Compare(
    const PositionStringVector & psvCompare,
    const unsigned indexOf1stToken,
    unsigned numTokens
    )
    const
  {
//    fastestUnsignedDataType numTokens = indexOfLastToken - indexOf1stToken + 1;
//#ifdef _DEBUG
    const int ownSize = size();
    const int cmpSize = psvCompare.size();
    LOGN_DEBUG( "comparing \"" << *this << "\" beginning from token #"
      << indexOf1stToken //<< "to " << indexOfLastToken
      << "(# tokens:"
      << numTokens << "); and \"" << psvCompare << "\"")
//#endif
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
    if( numTokens) /** comparison ended before comparing all tokens */
    {
      /** e.g. from token #3 (2 tokens) "the car still works" , dict: "works pension" */
      if( c_iter == end() )
        return lower;
      else /** # token to compare is greateer than comparison vector */
        return tooFewTokensInComparisonVector;
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

  PositionStringVector PositionStringVector::GetBetween(
//    const PositionStringVector & pcstrv,
    const int first,
    const int last
    ) const
  {
  //  DEBUG_COUT("GetBetween ANFANG\n");
//  #ifdef _DEBUG
    const int size = this->size() ;
    SUPPRESS_UNUSED_VARIABLE_WARNING(size)
//  #endif
    PositionStringVector retPCStrVec;
          #ifdef _DEBUG
  //      printf("first: %d\t last: %d\n",first,last);
  //      printf("last-first: %d\n",last-first);
  #endif
  //      DEBUG_COUT("pcstrv.size():" << pcstrv.size() << "\n")
    PositionStringVector::const_iterator pos_strCurrentElement =
      //see http://stackoverflow.com/questions/671423/c-stl-vectors-get-iterator-from-index
//      begin() + first;
      begin();
    if( first < size )
      //see http://stackoverflow.com/questions/671423/c-stl-vectors-get-iterator-from-index
      std::advance( pos_strCurrentElement, first );
//    for(DWORD i = first; i < pcstrv.size() && i <= (DWORD) last;)
    int currentIndex = first;
    while( pos_strCurrentElement != end() && currentIndex <= last)
    {
//              #ifdef _DEBUG
//              printf("pcstrv.at(i): %s\n",pcstrv.at(i).m_Str.c_str() );
//    #endif
//      PositionString pcstr = pcstrv.at( i++);
      retPCStrVec.push_back(/*pcstr*/ (* pos_strCurrentElement) );
      ++ pos_strCurrentElement;
      ++ currentIndex;
    }
//      DEBUG_COUT("PositionCStringVector GetBetween(const PositionCStringVector & ")
//    "pcstrv,int first,int last) ENDE\n");
    return retPCStrVec;
  }

std::string PositionStringVector::GetBetweenAsStdString(
  const int first, const int last ) const
{
  std::string stdstr ;
  const PositionStringVector pstrv = //GetBetween(rc_pstrv,first,last) ;
    GetBetween(first,last);
//    last = pstrv.size() ;
  PositionStringVector::const_iterator c_iter = pstrv.begin() ;
  if( c_iter != pstrv.end() )
  {
    stdstr += c_iter->m_Str ;
    ++ c_iter ;
  }
  while( c_iter != pstrv.end() )
//    for( first = 0 ; first < last ; ++ first )
  {
    stdstr += " " + c_iter->m_Str ;
    ++ c_iter ;
  }
  return stdstr ;
}
