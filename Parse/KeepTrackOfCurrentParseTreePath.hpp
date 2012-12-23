/*
 * KeepTrackOfCurrentParseTreePath.hpp
 *
 *  Created on: 13.12.2012
 *      Author: Stefan
 */

#ifndef KEEPTRACKOFCURRENTPARSETREEPATH_HPP_
#define KEEPTRACKOFCURRENTPARSETREEPATH_HPP_

#include <vector> //class std::vector
#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <Parse/GrammarPart.hpp> //class GrammarPart

namespace ParseTreeTraverser
{
  /** Implementation of DirectingLeavesMultipleIterTraverser that keeps track
   * of the current parse tree path. */
  class KeepTrackOfCurrentParseTreePath
    : public DirectingLeavesMultipleIterTraverser
  {
  protected:
    std::vector<WORD> m_vec_wGrammarPartIDcurrentParsePath ;
    std::vector<GrammarPart *> m_stdvector_p_grammarpartCurrentParseTreePath ;
  public:
    KeepTrackOfCurrentParseTreePath(
      const GrammarPart * p_grammarpartStartNode
      , ParseByRise & r_parsebyrise
//      , TranslateParseByRiseTree & r_translateparsebyrisetree
      );
    virtual
    ~KeepTrackOfCurrentParseTreePath();
    void BeforeBeginAtRoot()
    {
      //Important. else pathes with previous node(s) (->too long) are created.
      m_vec_wGrammarPartIDcurrentParsePath.clear() ;
      m_stdvector_p_grammarpartCurrentParseTreePath.clear() ;
    }
    void CurrentNodeIsLastAddedRightChild()
    {
      //http://www.cplusplus.com/reference/stl/vector/resize/:
      //"If sz is smaller than the current vector size, the content is reduced
      //to its first sz elements, the rest being dropped."
      m_vec_wGrammarPartIDcurrentParsePath.resize(
        //if the right node was at parse level 1 (2nd level), then 1 element
        //should remain.
        m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel ) ;
      m_stdvector_p_grammarpartCurrentParseTreePath.resize(
        //if the right node was at parse level 1 (2nd level), then 1 element
        //should remain.
        m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel ) ;
      m_vec_wGrammarPartIDcurrentParsePath.push_back(
        m_grammarpartpointer_and_parselevelCurrent.
          m_p_grammarpart->m_wGrammarPartID ) ;
      m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
        m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;
    }
    void ParseTreePathAdded()
    {
      m_vec_wGrammarPartIDcurrentParsePath.push_back(
        m_grammarpartpointer_and_parselevelCurrent.
          m_p_grammarpart->m_wGrammarPartID ) ;
      m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
        m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart) ;
//  #ifdef _DEBUG
//      std::string stdstr = m_r_parsebyrise.GetPathAs_std_string(
//        m_vec_wGrammarPartIDcurrentParsePath) ;
//  #endif
    }
    void ParseTreePathPopped()
    {
      m_stdvector_p_grammarpartCurrentParseTreePath.pop_back() ;
      m_vec_wGrammarPartIDcurrentParsePath.pop_back() ;
    }
  };

} /* namespace ParseTreeTraverser */
#endif /* KEEPTRACKOFCURRENTPARSETREEPATH_HPP_ */
