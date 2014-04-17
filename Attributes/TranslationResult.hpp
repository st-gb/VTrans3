/*
 * TranslationResult.hpp
 *
 *  Created on: 03.04.2014
 *      Author: mr.sys
 */

#ifndef TRANSLATIONRESULT_HPP_
#define TRANSLATIONRESULT_HPP_

#include "TranslationAndGrammarPart.hpp"
#include <vector> //base class std::vector

/** 1 or multiple words that are connected by nodes of a parse tree. */
class //TranslationResult
  WordCompound
  : public std::vector<TranslationAndGrammarPart>
{
public:
  WordCompound();
  virtual
  ~WordCompound();
};

#endif /* TRANSLATIONRESULT_HPP_ */
