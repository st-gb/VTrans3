/*
 * LetterArrayTree.hpp
 *
 *  Created on: 22.01.2013
 *      Author: Stefan
 */

#ifndef LETTERARRAYTREE_HPP_
#define LETTERARRAYTREE_HPP_

#include <stddef.h> //NULL
#include <set> //class std::set

namespace VTrans
{
  class LetterArray
  {
  public:
//    bool stringEnd;
    std::set<VocabularyAndTranslation> * m_p_;
    LetterArray * ar;
    LetterArray()
      : //stringEnd(false),
      m_p_(NULL),
      ar(NULL)
    {
//      ar = new LetterArray[numChars];
    }

    void Create(unsigned numChars)
    {
      ar = new LetterArray[numChars];
    }
//    CharExists()
//    {
//      ar[]
//    }
    void SetStringEnd()
    {
      if( ! m_p_ )
        m_p_ = new std::set<VocabularyAndTranslation *>;
    }
  };

  class LetterArrayTree : public /*IVocabularyInMainMem*/ VTrans::CharMappedDictionary
  {
    LetterArray m_root;
    unsigned m_numChars;
    unsigned m_numArray;
  public:
    LetterArrayTree(unsigned numChars);
    virtual
    ~LetterArrayTree();

    /*VocabularyAndTranslation * */
    std::set<VocabularyAndTranslation> * find(char * string)
    {
      LetterArray * p_letterarray = & m_root;
      BYTE char_ = * string;
      while( char_)
      {
//        p_letterarray->ar[ char_] = new LetterArray();
        if( p_letterarray->ar )
          p_letterarray = p_letterarray->ar[ char_];
        ++ string;
        char_ = * string;
      }
      if( p_letterarray->m_p_ )
      {
        return p_letterarray->m_p_;
      }
      return NULL;
    }

    void Insert(char * string)
    {
      LetterArray * p_letterarray = & m_root;
      BYTE char_ = * string;
      while( char_)
      {
//        p_letterarray->ar[ char_] = new LetterArray();
        p_letterarray->Create(m_numChars);
        ++ string;
        char_ = * string;
        if( char_)
          p_letterarray = p_letterarray->ar[ char_];
      }
//      p_letterarray->stringEnd = true;
      p_letterarray->SetStringEnd();
    }
  };

} /* namespace VTrans */
#endif /* LETTERARRAYTREE_HPP_ */
