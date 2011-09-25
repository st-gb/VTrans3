/*
 * GenerateXMLtreeFromParseTree.hpp
 *
 *  Created on: 25.09.2011
 *      Author: Stefan
 */

#ifndef GENERATEXMLTREEFROMPARSETREE_HPP_
#define GENERATEXMLTREEFROMPARSETREE_HPP_

#include <string> //class std::string

class ParseByRise;

void GenerateXMLtreeFromParseTree(ParseByRise * p_parsebyrise,
  std::string & std_strXML);

#endif /* GENERATEXMLTREEFROMPARSETREE_HPP_ */
