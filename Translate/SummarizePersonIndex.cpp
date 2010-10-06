/*
 * SummarizePersonIndex.cpp
 *
 *  Created on: May 6, 2010
 *      Author: Stefan
 */

#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
#include <Parse/GrammarPart.hpp>
#ifdef _DEBUG //for getting a string from an grammar part ID vector
  #include <Parse/ParseByRise.hpp>
#endif
#include <Translate/SummarizePersonIndex.hpp>
#include <Translate/TranslateParseByRiseTree.hpp>
//for "personindex" enum "German_main_verb"
#include <VocabularyInMainMem/LetterTree/VocabularyAndTranslation.hpp>

namespace ParseTreeTraverser
{

  SummarizePersonIndex::SummarizePersonIndex(
    const GrammarPart * p_grammarpartStartNode ,
    ParseByRise * p_parsebyrise ,
    TranslateParseByRiseTree & r_translateparsebyrisetree
    )
    : ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
      p_grammarpartStartNode ,
      p_parsebyrise
      )
    , mr_parsebyrise( * p_parsebyrise )
    , mp_translateparsebyrisetree ( & r_translateparsebyrisetree )
  {
    // TODO Auto-generated constructor stub

  }

  SummarizePersonIndex::~SummarizePersonIndex()
  {
    // TODO Auto-generated destructor stub
  }

  void SummarizePersonIndex::LeaveFound()
  {
    BYTE byPersonIndex ;
    std::string stdstrTranslation ;
#ifdef _DEBUG
    std::string stdstr = mr_parsebyrise.GetPathAs_std_string(
      m_vecGrammarPartID) ;
#endif
//    WORD wConsecutiveID ;
    //const GrammarPart * p_grammarpart ;
    GrammarPart * p_grammarpart ;
    //If the leave is e.g. a noun then for the person index we need to know
    //whether it is singular or plural. This is what TranslationRuleApplies()
    //does.
    if( mp_translateparsebyrisetree->TranslationRuleApplies(
        //stdstrTranslation
        m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->m_stdstrTranslation
        , byPersonIndex
        , m_vecGrammarPartID
        , m_stdvec_p_grammarpartPath
//        , wConsecutiveID
        , p_grammarpart
        )
      )
    {
//      DEBUG_COUT( "translation rule applies\n" ) ;
//      stdstrWholeTransl += stdstrTranslation + " " ;
      //TODO combine the person indices _until_ the parent is "subject"
      // because:
      // If clause with relative clause:
      // The man   that you fear    sucks.
      // ---S--- -relative clause-
      //                -S- --P-    --P--
//      p_grammarpart->m_byPersonIndex = byPersonIndex ;
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->m_byPersonIndex =
          byPersonIndex ;
//      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->m_
    }
//    mcp_grammarpartCurrent
  }

  void SummarizePersonIndex::ParseTreePathAdded()
  {
    m_vecGrammarPartID.push_back( m_grammarpartpointer_and_parselevelCurrent.
        m_p_grammarpart->m_wGrammarPartID ) ;
    m_stdvec_p_grammarpartPath.push_back( m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart) ;
#ifdef _DEBUG
    std::string stdstr = mr_parsebyrise.GetPathAs_std_string(
      m_vecGrammarPartID) ;
#endif
  }

  void SummarizePersonIndex::CurrentNodeIsLastAddedRightChild(
    //WORD wCurrentParseTreeLevel
    )
  {
#ifdef _DEBUG
    std::string stdstr = mr_parsebyrise.GetPathAs_std_string(
      m_vecGrammarPartID) ;
#endif
//    m_map_grammarpartRightUnprocessedChild2wParseTreeLevel.insert(
//      std::pair<mcp_grammarpartCurrent,wCurrentParseTreeLevel> ) ;
    //http://www.cplusplus.com/reference/stl/vector/resize/:
    //"If sz is smaller than the current vector size, the content is reduced
    //to its first sz elements, the rest being dropped."
    m_vecGrammarPartID.resize(
      //if the right node was at parse level 1 (2nd level), then 1 element
      //should remain.
      m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel ) ;
    m_stdvec_p_grammarpartPath.resize(
      //if the right node was at parse level 1 (2nd level), then 1 element
      //should remain.
      m_grammarpartpointer_and_parselevelCurrent.m_wParseLevel ) ;
    m_vecGrammarPartID.push_back( m_grammarpartpointer_and_parselevelCurrent.
        m_p_grammarpart->m_wGrammarPartID ) ;
    m_stdvec_p_grammarpartPath.push_back( m_grammarpartpointer_and_parselevelCurrent.
      m_p_grammarpart ) ;
#ifdef _DEBUG
    stdstr = mr_parsebyrise.GetPathAs_std_string(
      m_vecGrammarPartID) ;
#endif
  }

  //Is being called if both children of the current node were processed yet.
  void SummarizePersonIndex::UnprocessedHighestLevelNodeFound()
  {
#ifdef _DEBUG
    std::string stdstr = mr_parsebyrise.GetPathAs_std_string(
      m_vecGrammarPartID) ;
#endif
    GrammarPart * p_grammarpart =
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ;
    //May be NULL (if the superordinate grammar part is just a superclass):
    //   the car
    //    \  /
    // def_noun
    //    /
    // subj_or_obj_enum_ele
    if( p_grammarpart->mp_grammarpartRightChild )
    {
      switch(
          p_grammarpart->mp_grammarpartLeftChild->m_byPersonIndex )
      {
      //Undefined person index (e.g. "the" for "the car").
      case VocabularyAndTranslation::infinitive :
        p_grammarpart->m_byPersonIndex =
            p_grammarpart->mp_grammarpartRightChild->m_byPersonIndex ;
        break ;
      case VocabularyAndTranslation::first_person_singular :
        //I and I -> we  I and you -> we    I and he->we I and we -> we
        // I and you(plur)-> we    I and they->we
        p_grammarpart->m_byPersonIndex =
            VocabularyAndTranslation::first_person_plural ;
        break ;
        //"you" is 2nd person sing. ("du"), 3rd perosn ("man") and plural ("ihr")
      case VocabularyAndTranslation::second_person_singular :
        //"you and I" ->   "we"
        //"you(singular) and you" -> "you"(plural)
        //"you and he"->   "you"(plural)
        //you and we -> we
        // I and you(plur)-> we    I and they->we
  //      switch(p_grammarpart->mp_grammarpartRightChild )
  //      {
  //      case
  //      }
  //      p_grammarpart->m_byPersonIndex =
  //          VocabularyAndTranslation::first_person_plural ;
        break ;
      case VocabularyAndTranslation::third_person_singular :

        //"he and he" ->   "they"
        //"he and they" -> "they"
        switch(p_grammarpart->mp_grammarpartRightChild->m_byPersonIndex )
        {
        case VocabularyAndTranslation::first_person_singular : //"he and I"->"we"
        case VocabularyAndTranslation::first_person_plural : //"he and we"->"we"
          p_grammarpart->m_byPersonIndex =
              VocabularyAndTranslation::first_person_plural ;
          break ;
          //"he and you(singular)" -> "you"(plural)
        case VocabularyAndTranslation::second_person_singular :
          //"he and you(plural)" -> "you"(plural)
        case VocabularyAndTranslation::second_person_plural :
          p_grammarpart->m_byPersonIndex =
              VocabularyAndTranslation::second_person_plural ;
          break ;
        }
      }
    }
    else
      p_grammarpart->m_byPersonIndex =
          p_grammarpart->mp_grammarpartLeftChild->m_byPersonIndex ;
  }
}
