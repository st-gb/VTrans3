/*
 * DupliceParseTree.cpp
 *
 *  Created on: 21.03.2014
 *      Author: mr.sys
 */

#include <Parse/DuplicateParseTree.hpp>
#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp>

namespace ParseTreeTraverser
{

  DuplicateParseTree::DuplicateParseTree(
      const GrammarPart * p_grammarpartStartNode
      //TODO make const?
      , ParseByRise & r_parsebyrise
  //    , TranslateParseByRiseTree & r_translateparsebyrisetree
      )
    :
      ParseTreeTraverser::KeepTrackOfCurrentParseTreePath(
       p_grammarpartStartNode ,
       r_parsebyrise
       )
  {
    m_p_rootOfDuplicatedSubTree = new GrammarPart( * p_grammarpartStartNode);
    m_p_currentParent = m_p_rootOfDuplicatedSubTree;
  }

  DuplicateParseTree::~DuplicateParseTree()
  {
    // TODO Auto-generated destructor stub
  }

//  void DuplicateParseTree::ParseTreePathAdded()
//  {
////    m_p_rootOfDuplicatedSubTree->SetLeftChild();
////    m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
//  }

//  void DuplicateParseTree::RightChildAdded( unsigned short)
//  {
//  }

  void DuplicateParseTree::EnteredLeftChild()
  {
    GrammarPart * gpLeftChild = new GrammarPart(
      * m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart);
    m_p_currentParent->SetLeftChild( * gpLeftChild);
    m_p_currentParent = gpLeftChild;
  }
  void DuplicateParseTree::EnteredRightChild()
  {
    GrammarPart * gp = new GrammarPart(
      * m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart);
    m_p_currentParent->SetRightChild( * gp);
  }

  void DuplicateParseTree::CurrentNodeIsLastAddedRightChild()
  {
    if( ! m_stdvec_p_grammarpartRightNodeCopyToProcess.empty() )
    {
      m_p_currentParent =
        m_stdvec_p_grammarpartRightNodeCopyToProcess.
        //"Returns a read/write reference to the data at the last
        //*  element of the %vector."
        back() ;
      m_stdvec_p_grammarpartRightNodeCopyToProcess.pop_back();
    }
  }

  void DuplicateParseTree::RightChildAdded(
      WORD wParseTreeLevelRightChild)
  {
    GrammarPart * p_grammarpartRightChildOrig =
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
      mp_grammarpartRightChild ;
    GrammarPart * p_grammarpartRightChildCopy =
      new GrammarPart( * p_grammarpartRightChildOrig);
    m_p_currentParent->SetRightChild( * p_grammarpartRightChildCopy);
    m_stdvec_p_grammarpartRightNodeCopyToProcess.push_back(
      p_grammarpartRightChildCopy);
  }

  void DuplicateParseTree::MemorizedRightChildToProcess()
  {

  }
} /* namespace ParseTreeTraverser */
