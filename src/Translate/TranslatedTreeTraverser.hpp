/** TranslatedTreeTraverser.hpp
 *  Created on: 13.08.2011
 *      Author: Stefan */

#ifndef TRANSLATEDTREETRAVERSER_HPP_
#define TRANSLATEDTREETRAVERSER_HPP_

#include <string> //class std::string
#include <vector> //class std::vector
//class TranslationAndGrammarPart
#include <Attributes/TranslationAndGrammarPart.hpp>
#include <ParseTree/DirectingLeavesMultipleIterTraverser.hpp> //Base class

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
      , VTrans3::BottomUpParser * p_bottomUpParser
      )
      :
      DirectingLeavesMultipleIterTraverser(
        p_grammarpartStartNode
        , p_bottomUpParser
      )
    {
      mp_grammarpartStartNode = p_grammarpartStartNode;
    }
    /** Override callback function from superclass.*/
    void LeaveFound();
  };
}

#endif /* TRANSLATEDTREETRAVERSER_HPP_ */
