/*
 * SyntaxTreePath.hpp
 *
 *  Created on: 03.04.2010
 *      Author: Stefan
 */

#ifndef SYNTAXTREEPATH_HPP_
#define SYNTAXTREEPATH_HPP_

#include <string>
#include <vector>
#include <rest.h> //DEBUG_COUT(...)

typedef unsigned short WORD ;
class ParseByRise ;
class GrammarPart ;

//Used to compare whether a condition of a translation rule applies.
//Therefore it created a syntax tree path as an ID array from the
//grammar part name(s) to compare faster.
//This class encapsulates both the ID array and its size because the size
//is necessary for knowing the end of the array.
class SyntaxTreePath
{
  //unsigned char  m_byOtherReferencesToGrammarPartIDArray ;
  bool m_bReferringOthersGrammarPartIDArray ;
public:
  ParseByRise * mp_parsebyrise ;
  WORD * m_ar_wElements ;
  WORD m_wNumberOfElements ;
  SyntaxTreePath()
    : //m_byOtherReferencesToGrammarPartIDArray( 0)
    //,
    m_bReferringOthersGrammarPartIDArray( false )
  {
    m_ar_wElements = NULL ;
    //Important to set for < operator.
    m_wNumberOfElements = 0 ;
  }

  //The copy contructor needs an own implementation because an array is created
  //on the heap: in order to NOT release memory for this array in the destructor
  //we simply set a flag that we are referring this array.
  SyntaxTreePath( const SyntaxTreePath & stpToCopyFrom ) ;
  ~SyntaxTreePath() ;
  SyntaxTreePath(std::string & r_stdstrSyntaxTreePath
      ,     ParseByRise * p_parsebyrise ) ;
  void CreateGrammarPartIDArray(
      const std::string & r_stdstrSyntaxTreePath
      ,     ParseByRise * p_parsebyrise ) ;
  GrammarPart * GetLeaf(
    std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath) const ;
  bool operator < ( const SyntaxTreePath & r) const ;
  bool IsPartOf(std::vector<WORD> & r_stdvec_wGrammarPartPath ) ;
};

#endif /* SYNTAXTREEPATH_HPP_ */
