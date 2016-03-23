/*
 * TranslationAndConcatenationID.hpp
 *
 *  Created on: 14.08.2011
 *      Author: Stefan
 */

#ifndef TRANSLATIONANDCONCATENATIONID_HPP_
#define TRANSLATIONANDCONCATENATIONID_HPP_

class TranslationAndConcatenationID
{
public:
  std::string m_std_strTranslation;
  uint32_t m_ui32ConcatenationID;
  TranslationAndConcatenationID(
    const std::string & c_r_std_strTranslation,
    uint32_t ui32ConcatenationID
    )
    : m_std_strTranslation(c_r_std_strTranslation)
    , m_ui32ConcatenationID(ui32ConcatenationID)
  {

  }

  inline bool OrderByString1stAndThenByID(
    const TranslationAndConcatenationID & c_r_CompareWith) const
  {
//    bool bLess = false;
    int nStrCmp = c_r_CompareWith.m_std_strTranslation.
      //"Returns an integer < 0 if this string is ordered before @a str"
      compare(m_std_strTranslation);
    if( nStrCmp < 0)
      return true;
    else if( nStrCmp > 0)
      return false;
    else
    {
        if( c_r_CompareWith.m_ui32ConcatenationID < m_ui32ConcatenationID )
          return true;
        else return false;
    }
  }
  inline bool OrderByConcatenationID1st(
    const TranslationAndConcatenationID & c_r_CompareWith) const
  {
//    bool bLess = false;
    if( c_r_CompareWith.m_ui32ConcatenationID < m_ui32ConcatenationID )
      return true;
    else if( c_r_CompareWith.m_ui32ConcatenationID > m_ui32ConcatenationID)
      return false;
    else // c_r_CompareWith.m_ui32ConcatenationID = m_ui32ConcatenationID
    {
      int nStrCmp = c_r_CompareWith.m_std_strTranslation.
        //"Returns an integer < 0 if this string is ordered before @a str"
        compare(m_std_strTranslation);
      if( nStrCmp < 0)
        return true;
      else
        return false;
    }
  }
  //For inserting into STL container.
  //Should be ordered by concatenation ID because they should be inserted into
  //the choice controls in this order.
  bool operator < (const TranslationAndConcatenationID & rhs) const
  {
    return //OrderByString1stAndThenByID(rhs);
        OrderByConcatenationID1st(rhs);
//    if( rhs.m_std_strTranslation < m_std_strTranslation )
//    return rhs.m_std_strTranslation < m_std_strTranslation
//      &&  ;
//    return bLess;
  }
};

#endif /* TRANSLATIONANDCONCATENATIONID_HPP_ */
