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

//If e.g. definite_article_plural.definite_article it is important if
// a direct or indirect parent node is "object" so that it is translated to
// e.g. either "die" or "den" (if dative).
//The syntax tree path may vary because between
//  "definite_article_plural.definite_article" and "object" can be other nodes
// that e.g. are used for conjunctions:
//I  trust           the         men.
// \ mainVerbInf1Obj defArticle  pluralNoun
//  \  /                   \   /
// clause1Obj       defArticlePlural
//        \           object
//         \          /
//         ClauseWith1Obj
//
//I  trust           the        men       and the          women.
// \ mainVerbInf1Obj defArticle pluralNoun /  DefArticle pluralNoun
//  \  /                      \   /       /            \  /
//   clause1Obj       defArticlePlural   / DefArticlePlural
//            \                      \  / /
//             \     defArticlePluralAnd /
//              \                     \ /
//               \                object
//                \              /
//                 ClauseWith1Obj
// So the "Kleene star operator" is useful for these situations:
// "definite_article_plural.definite_article.*.object"
#define KLEENE_STAR_OPERATOR 65535

class ParseByRise ;
class GrammarPart ;

//Used to compare whether a condition of a translation rule applies.
//Therefore it created a syntax tree path as an ID array from the
//grammar part name(s) to compare faster.
//A syntax tree path in the syntax tree
//  the         car
// def_article  noun
//           \  /
// def_article_noun
//
//is e.g. "def_article_noun(3)"->"def_article(1)", as IDs e.g. "3->1"
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
  std::string GetAs_std_string() const ;
  GrammarPart * GetLeaf(
    const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath) const ;
  bool operator < ( const SyntaxTreePath & r) const ;
  bool IsPartOf(std::vector<WORD> & r_stdvec_wGrammarPartPath ) ;
};

#endif /* SYNTAXTREEPATH_HPP_ */
