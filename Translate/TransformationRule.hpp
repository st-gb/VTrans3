/*
 * TransformationRule.hpp
 *
 *  Created on: Dec 21, 2010
 *      Author: Stefan
 */

#ifndef TRANSFORMATIONRULE_HPP_
#define TRANSFORMATIONRULE_HPP_

#include <string> //class std::string

class TransformationRule
{
public:
  bool m_bInsertLeftChild ;
  std::string m_stdstrParseTreePathWhereToInsert ;
  TransformationRule();
  virtual
  ~TransformationRule();
};

#endif /* TRANSFORMATIONRULE_HPP_ */
