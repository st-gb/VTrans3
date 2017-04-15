/* TranslationResult.hpp
 *  Created on: 03.04.2014
 *      Author: mr.sys  */

#ifndef TRANSLATIONRESULT_HPP_
#define TRANSLATIONRESULT_HPP_

#include "TranslationAndGrammarPart.hpp"
#include <vector> //base class std::vector

/** 1 or multiple words that are connected by nodes of a parse tree:
 *  e.g. "the man walks".
 *  each TranslationAndGrammarPart object represents a word. */
//class //TranslationResult
//  WordCompound
//  : public std::vector<TranslationAndGrammarPart>
//{
//public:
//  WordCompound();
//  virtual
//  ~WordCompound();
//};
typedef std::vector<TranslationAndGrammarPart> WordCompound;

/** The same parse tree can be translated in multiple ways: "the car" ->
  *  "das Auto", "der Pkw". This is why it is a vector of word compounds. */
typedef std::vector<WordCompound> WordCompoundsAtSameTokenIndex;

/** A vector of sentences that begin at the same token index
* (sentences that begin at the same token index:
* vector of sentences that each contains a vector of words). */
//TODO exchange "std::vector<ele>" by a class that is derived from std::vector
class TranslationResult
  : public std::vector<WordCompoundsAtSameTokenIndex>
{
};
//Alternative to class definition: typedef 
// std::vector<WordCompoundsAtSameTokenIndex> TranslationResult;

#endif /* TRANSLATIONRESULT_HPP_ */
