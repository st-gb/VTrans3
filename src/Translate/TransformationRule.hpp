/*
 * TransformationRule.hpp
 *
 *  Created on: Dec 21, 2010
 *      Author: Stefan
 */

#ifndef TRANSFORMATIONRULE_HPP_
#define TRANSFORMATIONRULE_HPP_

#include <string> //class std::string
#include <Translate/SyntaxTreePath.hpp> //class SyntaxTreePath

class TransformationRule
{
public:
  bool m_bInsertLeftChild ;
  std::string m_stdstrParseTreePathWhereToInsert ;
  std::string m_stdstrNameOfGrammarPartToInsert;
  SyntaxTreePath m_syntaxtreepath2nd;
  SyntaxTreePath m_syntaxtreepathWhereToInsert;
  TransformationRule();
  virtual
  ~TransformationRule();
};

#endif /* TRANSFORMATIONRULE_HPP_ */
