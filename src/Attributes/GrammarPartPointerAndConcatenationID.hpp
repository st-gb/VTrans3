/*
 * GrammarPartPointerAndConcatenationID.hpp
 *
 *  Created on: 17.08.2011
 *      Author: Stefan
 */

#ifndef GRAMMARPARTPOINTERANDCONCATENATIONID_HPP_
#define GRAMMARPARTPOINTERANDCONCATENATIONID_HPP_

#include <Parse/GrammarPart.hpp> //class GrammarPart
#include <stdint.h> //for uint32_t

class GrammarPartPointerAndConcatenationID
{
public:
  //TODO maybe exchange integer type to fastestUnsignedDataType for performance reasons
  /** all IDs with this value should not be connected. */
  static const uint32_t s_defaultConcatenationID = 0;
  const GrammarPart * m_p_grammarpartToken;
  uint32_t m_ui32ConcatenationID;

  GrammarPartPointerAndConcatenationID(
    const GrammarPart * c_p_grammarpartToken,
    uint32_t ui32ConcatenationID
    )
    : m_p_grammarpartToken(c_p_grammarpartToken)
    , m_ui32ConcatenationID(ui32ConcatenationID)
  {

  }
  inline bool OrderByConcatenationID1st(
    const GrammarPartPointerAndConcatenationID & c_r_CompareWith) const
  {
//    bool bLess = false;
    if( c_r_CompareWith.m_ui32ConcatenationID < m_ui32ConcatenationID )
      return true;
    else if( c_r_CompareWith.m_ui32ConcatenationID > m_ui32ConcatenationID)
      return false;
    else // c_r_CompareWith.m_ui32ConcatenationID = m_ui32ConcatenationID
    {
      int nStrCmp = c_r_CompareWith.m_p_grammarpartToken->m_stdstrTranslation.
        //"Returns an integer < 0 if this string is ordered before @a str"
        compare(m_p_grammarpartToken->m_stdstrTranslation);
      if( nStrCmp < 0)
        return true;
      else
        return false;
    }
  }
  //For inserting into STL container.
  //Should be ordered by concatenation ID because they should be inserted into
  //the choice controls in this order.
  bool operator < (const GrammarPartPointerAndConcatenationID & rhs) const
  {
    return //OrderByString1stAndThenByID(rhs);
        OrderByConcatenationID1st(rhs);
//    if( rhs.m_std_strTranslation < m_std_strTranslation )
//    return rhs.m_std_strTranslation < m_std_strTranslation
//      &&  ;
//    return bLess;
  }
};

#endif /* GRAMMARPARTPOINTERANDCONCATENATIONID_HPP_ */
