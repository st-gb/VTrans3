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

/** A vector of sentences that begin at the same token index
* (sentences that begin at the same token index:
* vector of sentences that each contains a vector of words). */
//TODO exchange "std::vector<ele>" by a class that is derived from std::vector
class TranslationResult
  : public std::vector <std::vector <std::vector <TranslationAndGrammarPart> > >
{
  
};

#endif /* TRANSLATIONRESULT_HPP_ */
