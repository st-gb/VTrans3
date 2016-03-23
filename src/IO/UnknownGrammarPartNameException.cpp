/*
 * UnknownGrammarPartNameException.cpp
 *
 *  Created on: 08.12.2012
 *      Author: Stefan
 */

#include "UnknownGrammarPartNameException.hpp"

namespace VTrans
{

  UnknownGrammarPartNameException::UnknownGrammarPartNameException(
    const std::string & c_r_std_strGrammarPartPath)
    : m_std_strGrammarPartPath(c_r_std_strGrammarPartPath)
  {
    // TODO Auto-generated constructor stub

  }

  UnknownGrammarPartNameException::~UnknownGrammarPartNameException()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace VTrans */
