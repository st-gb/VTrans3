/*
 * TranslatedTreeTraverser.hpp
 *
 *  Created on: 13.08.2011
 *      Author: Stefan
 */

#ifndef TRANSLATEDTREETRAVERSER_HPP_
#define TRANSLATEDTREETRAVERSER_HPP_

#include <string> //class std::string
#include <vector> //class std::vector
//class TranslationAndGrammarPart
#include <Attributes/TranslationAndGrammarPart.hpp>
#include <Parse/DirectingLeavesMultipleIterTraverser.hpp> //Base class

namespace ParseTreeTraverser
{
  //Traverses the translated tree (when translating the parse tree may be
  //modified by inserting or moving nodes) for getting the word/ token from
  //right to left.
  class TranslatedTreeTraverser
    : public DirectingLeavesMultipleIterTraverser
  {
  public:
    std::string m_std_strWholeTranslation;
    std::vector<GrammarPart *> m_std_vector_p_grammarpartTranslation ;
    std::vector<TranslationAndGrammarPart>
      m_std_vector_translationandgrammarpart;

    TranslatedTreeTraverser(
      const GrammarPart * p_grammarpartStartNode
      , ParseByRise * p_parsebyrise
      )
      :
      DirectingLeavesMultipleIterTraverser(
        p_grammarpartStartNode
        , p_parsebyrise
      )
    {
      mp_grammarpartStartNode = p_grammarpartStartNode;
    }
    //Callback function from superclass.
    void LeaveFound();
  };
}

#endif /* TRANSLATEDTREETRAVERSER_HPP_ */