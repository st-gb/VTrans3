/*
 * TranslationAndGrammarPart.hpp
 *
 *  Created on: Jan 18, 2011
 *      Author: Stefan
 */

#ifndef TRANSLATIONANDGRAMMARPART_HPP_
#define TRANSLATIONANDGRAMMARPART_HPP_

#include <string> //class std::string

class GrammarPart;

class TranslationAndGrammarPart
{
public:
  TranslationAndGrammarPart(
    std::string & r_stdstrTranslation ,
    const GrammarPart * p_grammarpart
    )
  {
    m_stdstrTranslation = r_stdstrTranslation ;
    mp_grammarpart = p_grammarpart ;
  }
  std::string m_stdstrTranslation ;
  const GrammarPart * mp_grammarpart ;
} ;

#endif /* TRANSLATIONANDGRAMMARPART_HPP_ */
