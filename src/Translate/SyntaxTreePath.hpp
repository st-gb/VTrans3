/*
 * SyntaxTreePath.hpp
 *
 *  Created on: 03.04.2010
 *      Author: Stefan
 */

#ifndef SYNTAXTREEPATH_HPP_
#define SYNTAXTREEPATH_HPP_

#include <string> //class std::string
#include <vector> //class std::vector
//#include <rest.h> //DEBUG_COUT(...)
#include <windef.h> //for BYTE
#include <hardware/CPU/fastest_data_type.h> //typedef fastestUnsignedDataType
//#include <exception>//class std::exception
#include <Parse/ParseTreeNode_ID_type.hpp>///typedef PTN_IDtype

using namespace VTrans3;///Scoping for PTN_IDtype

class GetGrammarPartIDexception
//  : public std::exception
{
public:
  std::string m_stdstr ;
  GetGrammarPartIDexception( const std::string & cr_stdstr )
  {
    m_stdstr = cr_stdstr ;
  }
//  const char* what() const //throw()
//  {
//    return m_stdstr ;
//  }
} ;

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
//TODO the value is dependent on the data type used for parse tree node IDs
// Change to "const indexDataType KLEENE_STAR_OPERATOR = std::numeric_limits<
// indexDataType>::max();"?
#define KLEENE_STAR_OPERATOR 65535

/** Forward declarations */
class GrammarPart ;
class I_UserInterface ;
namespace VTrans3
{
  class BottomUpParser ;
}
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
  typedef /*WORD*/ fastestUnsignedDataType number_type;
  ///PTN_ID=ParseTreeNodeIDentifier
  typedef std::vector<PTN_IDtype> PTN_IDpathContainer_type;
  enum CreateGrammarPartIDArray_return_values
  {
    unknown_grammar_part_name = 0 ,
    all_grammar_parts_resolved ,
    no_grammar_part_resolved
  };
  //for showing an error message when getting ID from grammar part name failed.
  static I_UserInterface * sp_userinterface ;
  VTrans3::BottomUpParser * mp_bottomUpParser;
  PTN_IDtype * m_ar_wElements ;
  WORD m_wNumberOfElements ;

  void copy(const SyntaxTreePath & c_r_syntaxtreepathToCopyFrom);

  SyntaxTreePath()
    : //m_byOtherReferencesToGrammarPartIDArray( 0)
    //,
    m_bReferringOthersGrammarPartIDArray( false )
  {
    m_ar_wElements = NULL ;
    //Important to set for < operator.
    m_wNumberOfElements = 0 ;
  }
  /** The copy constructor needs an own implementation because an array is created
  * on the heap: in order to NOT release memory for this array in the destructor
  * we simply set a flag that we are referring this array. */
  SyntaxTreePath( const SyntaxTreePath & stpToCopyFrom ) ;
  SyntaxTreePath(
    const std::string & r_stdstrSyntaxTreePath
    , VTrans3::BottomUpParser *
    ) ;
  ~SyntaxTreePath() ;

  GrammarPart * GetCommonNode(
    const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath) const;
  BYTE CreateGrammarPartIDArray(
    const std::string & r_stdstrSyntaxTreePath
    , VTrans3::BottomUpParser *
    , std::string & r_std_strUnknownGrammarPartID
    ) ;
  std::string GetAs_std_string() const ;
  static void GetAsGrammarPartIDvector(
    std::vector<GrammarPart *> & r_stdvector_p_grammarpartParseTreePath ,
    PTN_IDpathContainer_type & PTN_IDpathContainer) ;
  GrammarPart * GetLeaf(
    const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath) const ;
  number_type GetNumberOfElements() const { return m_wNumberOfElements; }
  SyntaxTreePath & operator = ( const SyntaxTreePath & r) ;
  bool operator < ( const SyntaxTreePath & r) const ;
  bool IsPartOf(PTN_IDpathContainer_type & PTN_IDpathContainer ) ;
 static bool Matches(
    PTN_IDtype * ar_wElements ,
    fastestUnsignedDataType numberOfElements ,
    const PTN_IDpathContainer_type & PTN_IDpathContainer
    ) ;
//  static bool Matches(
//    const std::vector<WORD> & cr_stdvec_wGrammarPartPath ,
//    const std::vector<GrammarPart *> & cr_stdvec_grammarpartPath
//    ) const ;
  bool Matches(
    const PTN_IDpathContainer_type & r_stdvec_wCurrentGrammarPartPath
    ) const ;
}; //end class

#endif /* SYNTAXTREEPATH_HPP_ */
