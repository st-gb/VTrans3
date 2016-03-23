/*
 * CharMappedDictionary.hpp
 *
 *  Created on: 22.01.2013
 *      Author: Stefan
 */

#ifndef CHARMAPPEDDICTIONARY_HPP_
#define CHARMAPPEDDICTIONARY_HPP_

#include "IVocabularyInMainMem.hpp" //class IVocabularyInMainMem

#define MAPPING_ARRAY_SIZE 255

namespace VTrans
{
  /** Base class of trie classes. Provides a mapping from a character to a
   * number to minimize the character array size.  */
  class CharMappedDictionary
    : public IVocabularyInMainMem
  {
  protected:
    //BYTE * m_arbyCharValueToArrayIndex ;
    BYTE m_arbyCharValueToArrayIndex[MAPPING_ARRAY_SIZE] ;
    BYTE m_byArrayIndexRespSize ;
  public:
    CharMappedDictionary();
    virtual
    ~CharMappedDictionary();

    /** The sense of mapping is to allow the array of the direct child of a
    * node to be have less than 255 elements.*/
    void createMapping() ;

    void addToCharValueToArrayIndexMapping(//CHAR
      BYTE chFrom, //CHAR
      BYTE chTo )
    {
      for(//CHAR
        BYTE chIndex = chFrom ; chIndex <= chTo ; ++ chIndex )
        //m_vecch.push_back(ch);
        //Sanity check.
        if(chIndex < 255 )
          m_arbyCharValueToArrayIndex[chIndex] = m_byArrayIndexRespSize ++ ;
    }
    void addToCharValueToArrayIndexMapping(//CHAR
      BYTE ch)
    {
      //m_vecch.push_back(ch);
      //Sanity check.
      if(ch < 255 )
        m_arbyCharValueToArrayIndex[ch] = m_byArrayIndexRespSize ++ ;
    }
  };

} /* namespace VTrans */
#endif /* CHARMAPPEDDICTIONARY_HPP_ */
