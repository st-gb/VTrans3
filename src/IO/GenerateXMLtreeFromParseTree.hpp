/*
 * GenerateXMLtreeFromParseTree.hpp
 *
 *  Created on: 25.09.2011
 *      Author: Stefan
 */

#ifndef GENERATEXMLTREEFROMPARSETREE_HPP_
#define GENERATEXMLTREEFROMPARSETREE_HPP_

//#include <string> //class std::string
//#include <data_structures/ByteArray.hpp>

/** Forward decl.*/
class ParseByRise;
class ByteArray;

namespace IO
{
  void GenerateXMLtreeFromParseTree(ParseByRise * p_parsebyrise,
    /*std::string & std_strXML*/ ByteArray & byteArray);
}

#endif /* GENERATEXMLTREEFROMPARSETREE_HPP_ */
