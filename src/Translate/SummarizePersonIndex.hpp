/*
 * SummarizePersonIndex.hpp
 *
 *  Created on: May 6, 2010
 *      Author: Stefan
 */

#ifndef SUMMARIZEPERSONINDEX_HPP_
#define SUMMARIZEPERSONINDEX_HPP_

#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <Translate/TranslateParseByRiseTree.hpp>
#include <map>
#include <vector>

//avoid including "windef.h" for just 1 typedef
typedef unsigned short WORD ;

class GrammarPart ;

namespace ParseTreeTraverser
{

  class SummarizePersonIndex
    : public DirectingLeavesMultipleIterTraverser
  {
  public:
    ParseByRise & mr_parsebyrise ;
    TranslateParseByRiseTree * mp_translateparsebyrisetree ;
    std::map<GrammarPart *,WORD>
      m_map_grammarpartRightUnprocessedChild2wParseTreeLevel ;
    std::vector<WORD> m_vecGrammarPartID ;
    std::vector<GrammarPart *> m_stdvec_p_grammarpartPath ;
    SummarizePersonIndex(
      const GrammarPart * p_grammarpart ,
      ParseByRise * p_parsebyrise ,
      TranslateParseByRiseTree & r_translateparsebyrisetree
      );
    virtual
    ~SummarizePersonIndex();
    void BeforeBeginAtRoot()
    {
      //Important. else pathes with previous node(s) (->too long) are created.
      m_vecGrammarPartID.clear() ;
      m_stdvec_p_grammarpartPath.clear() ;
    }
    //In order to keep track of the current parse tree path.
    void CurrentNodeIsLastAddedRightChild() ;
    void LeaveFound() ;
    //In order to keep track of the current parse tree path.
    void ParseTreePathAdded() ;
//    void RightChildAdded(WORD wCurrentParseTreeLevel) ;
    void UnprocessedHighestLevelNodeFound() ;
  };
}

#endif /* SUMMARIZEPERSONINDEX_HPP_ */
