/*
 * IterableWords.hpp
 *
 *  Created on: 31.07.2011
 *      Author: Stefan
 */

#ifndef ITERABLEWORDS_HPP_
#define ITERABLEWORDS_HPP_

typedef unsigned char BYTE;
#include <string> //class std::string

class IterableWords
{
public:
  BYTE m_byIndex ;
  IterableWords()
    : m_byIndex(0) {};
  /** avoid g++ warning : "virtual method but non-virtual d'tor" */
  virtual ~IterableWords() { }
  void InitGetNextString()
  {
    m_byIndex = 0 ;
  }
  //For ability to iterate over all of the word's strings.
  virtual bool GetNextString( std::string & r_stdstr ) //= 0 ;
    { return false; }
};

#endif /* ITERABLEWORDS_HPP_ */
