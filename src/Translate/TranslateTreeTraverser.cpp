/*
 * DoTranslateTreeTraverser.cpp
 *
 *  Created on: May 17, 2010
 *      Author: Stefan
 */

#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN()
#include <Translate/TranslateTreeTraverser.hpp>
#include <Translate/SummarizePersonIndex.hpp>
#include <Translate/SetSameConsecutiveIDforLeaves.hpp>
//SUPPRESS_UNUSED_VARIABLE_WARNING(...)
#include <compiler/GCC/suppress_unused_variable.h>

namespace ParseTreeTraverser
{

  DoTranslateTreeTraverser::DoTranslateTreeTraverser(
    const GrammarPart * p_grammarpartStartNode
    , ParseByRise & r_parsebyrise
    , TranslateParseByRiseTree & r_translateparsebyrisetree
    )
    :
     ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
      p_grammarpartStartNode ,
      & r_parsebyrise
      )
    , m_r_parsebyrise (r_parsebyrise)
    , mr_translateparsebyrisetree( r_translateparsebyrisetree )
    , m_wConsecutiveID(0)
  {
    LOGN_DEBUG(/*"TranslateTreeTraverser()"*/ "")
//    m_wSubjectGrammarPartID ;
    if( ! mp_parsebyrise->GetGrammarPartID("subject", m_wSubjectGrammarPartID)
      )
      m_wSubjectGrammarPartID = 65535 ;
    if( ! mp_parsebyrise->GetGrammarPartID("article_singular",
      m_wArticleSingularGrammarPartID)
      )
      m_wArticleSingularGrammarPartID = 65535 ;
    if( ! mp_parsebyrise->GetGrammarPartID("plural_noun" ,
      m_wPluralNounGrammarPartID)
      )
      m_wPluralNounGrammarPartID = 65535 ;
  }

  DoTranslateTreeTraverser::~DoTranslateTreeTraverser()
  {
    LOGN_DEBUG(/*"~TranslateTreeTraverser()"*/ "")
  }

  void DoTranslateTreeTraverser::BeforeBeginAtRoot()
  {
    //Important. else pathes with previous node(s) (->too long) are created.
    m_vec_wGrammarPartIDcurrentParsePath.clear() ;
    m_stdvector_p_grammarpartCurrentParseTreePath.clear() ;
  }

  //TODO obsolete since the grammar rules determine the person index?
  // (3rd pers sing must match 3rd pers sing finite verb form e.g."he workS")
  void DoTranslateTreeTraverser::HandlePossibleSubject()
  {
    LOGN_DEBUG("begin")
    GrammarPart * p_grammarpart =
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ;
    if( //mp_grammarpartStartNode
        p_grammarpart->
        m_wGrammarPartID == m_wSubjectGrammarPartID
//        || p_grammarpart->m_wGrammarPartID == m_wArticleSingularGrammarPartID
//        || p_grammarpart->m_wGrammarPartID == m_wPluralNounGrammarPartID
        )
      //Get the person index bitfield, i.e.
    {
      LOGN_DEBUG("m_wGrammarPartID == m_wSubjectGrammarPartID")
      //  "I" has person index "0",
      //  "you" has person indices "1" (singular, German: "du" ),
      //     "4" (plural, German: "ihr"), "6" (German: "man")
  //          mp_parsebyrise->GetSubjectPersonIndex( p_grammarpart);
      ParseTreeTraverser::SummarizePersonIndex spi(
        p_grammarpart ,
        mp_parsebyrise ,
        mr_translateparsebyrisetree
        ) ;
      spi.Traverse() ;
//      DEBUG_COUT(
      LOGN_DEBUG(
        "result of getting person index:"
        << (WORD) p_grammarpart->m_byPersonIndex )
  //      //The nodes directing leaves have been processed by SummarizePersonIndex
  //      // yet, so do not do this again.
  //      m_stdset_p_grammarpartProcessedYet.insert( p_grammarpart ) ;
    }
  }

  void DoTranslateTreeTraverser::LeaveFound()
  {
    LOGN_DEBUG( /*FULL_FUNC_NAME <<*/ "current parse tree "
      "path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
    BYTE byPersonIndex ;
    std::string stdstrTranslation ;
    //TODO value is not set for this variable
    WORD wConsecutiveID = 0;
    //const GrammarPart * p_grammarpart ;
    GrammarPart * p_grammarpartWithConsecutiveID ;
    if( mr_translateparsebyrisetree.TranslationRuleApplies(
        stdstrTranslation ,
        byPersonIndex ,
        m_vec_wGrammarPartIDcurrentParsePath ,
        m_stdvector_p_grammarpartCurrentParseTreePath ,
//        wConsecutiveID
        p_grammarpartWithConsecutiveID
        )
      )
    {
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
        m_stdstrTranslation = stdstrTranslation;

//      DEBUG_COUTN( "LeaveFound--translation rule applies. translation:" <<
//        stdstrTranslation ) ;
      LOGN_DEBUG( //"TranslateTreeTraverser::LeaveFound(): " <<
        m_r_parsebyrise.GetPathAs_std_string(
          m_vec_wGrammarPartIDcurrentParsePath)
        << "--translation rule applies. "
        "translation:" << stdstrTranslation ) ;
      m_stdstrWholeTranslation += stdstrTranslation + " " ;
      //The consecutive ID is set to the grammar part AFTER we arrive here.
      //Because this ID is needed, store the grammar part, too.
      m_stdvecTranslationAndGrammarPart.push_back(
        TranslationAndGrammarPart(stdstrTranslation,
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart
//          p_grammarpartWithConsecutiveID
          )
        ) ;
      m_stdvec_translation_and_consecutive_id.push_back(
        TranslationAndConsecutiveID( stdstrTranslation,
          wConsecutiveID)
        ) ;
      //TODO combine the person indices _until_ the parent is "subject"
      // because:
      // If clause with relative clause:
      // The man   that you fear    sucks.
      // ---S--- -relative clause-
      //                -S- --P-    --P--
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
        m_byPersonIndex = byPersonIndex ;
    }
  }

  /** The traverser traverses the tree until 1 or 2 processed node(s) was/ were
  * found: e.g.
  *
  *    def_noun    <-when arrived here: "the" and "car" were processed-> call
  *     / \           "UnprocessedHighestLevelNodeFound"
  *   the car  <-both were processed yet. */
  void DoTranslateTreeTraverser::UnprocessedHighestLevelNodeFound()
  {
    std::string stdstrTranslation ;
    BYTE byPersonIndex ;
//    WORD wConsecutiveID ;
    //const GrammarPart * p_grammarpart ;
    GrammarPart * p_grammarpart ;
    if( m_vec_wGrammarPartIDcurrentParsePath.size() > 0 )
    {
      WORD wGrammarPartIDcurrentParsePath =
          m_vec_wGrammarPartIDcurrentParsePath.back() ;
      std::string stdstr = mp_parsebyrise->GetGrammarPartName(
        wGrammarPartIDcurrentParsePath) ;
      if( stdstr == "article_singular"
          || stdstr == "definite_article_plural" )
      {
#ifdef _DEBUG
        //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//        #pragma GCC diagnostic ignored  "-Wunused"
        GrammarPart * p_grammarpart =
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ;
        SUPPRESS_UNUSED_VARIABLE_WARNING(p_grammarpart)
//        SetSameConsecutiveIDforLeaves(p_grammarpart) ;
#endif
      }
    }
//    if( m_vec_wGrammarPartIDcurrentParsePath)
    if( mr_translateparsebyrisetree.TranslationRuleApplies(
        stdstrTranslation ,
        byPersonIndex ,
        m_vec_wGrammarPartIDcurrentParsePath ,
        m_stdvector_p_grammarpartCurrentParseTreePath ,
//        wConsecutiveID
        p_grammarpart
        )
      )
    {
      //DEBUG_COUT( "UnprocessedHighestLevelNodeFound--"
      LOGN_DEBUG(
        "translation rule applies\n" ) ;
      m_stdstrWholeTranslation += stdstrTranslation + " " ;
      //TODO combine the person indices _until_ the parent is "subject"
      // because:
      // If clause with relative clause:
      // The man   that you fear    sucks.
      // ---S--- -relative clause-
      //                -S- --P-    --P--
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart->
        m_byPersonIndex = byPersonIndex ;
    }
  }

  void DoTranslateTreeTraverser::ParseTreePathAdded()
  {
    m_vec_wGrammarPartIDcurrentParsePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent.
        m_p_grammarpart->m_wGrammarPartID ) ;
    m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart) ;
#ifdef _DEBUG
    std::string stdstr = m_r_parsebyrise.GetPathAs_std_string(
      m_vec_wGrammarPartIDcurrentParsePath) ;
#endif
    if( m_vec_wGrammarPartIDcurrentParsePath.size() > 0 )
    {
      WORD wGrammarPartIDcurrentParsePath =
          m_vec_wGrammarPartIDcurrentParsePath.back() ;
      std::string stdstr = mp_parsebyrise->GetGrammarPartName(
        wGrammarPartIDcurrentParsePath) ;
      if( stdstr == "article_singular"
          || stdstr == "definite_article_plural" )
      {
        GrammarPart * p_grammarpart =
          m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ;
        p_grammarpart->m_wConsecutiveID = m_wConsecutiveID ++ ;
      }
    }
//    HandlePossibleSubject();//not neccessary anymore? (see method documentat.)
  }

  void DoTranslateTreeTraverser::ParseTreePathPopped()
  {
    m_stdvector_p_grammarpartCurrentParseTreePath.pop_back() ;
    m_vec_wGrammarPartIDcurrentParsePath.pop_back() ;
    LOGN_DEBUG(//"TranslateTreeTraverser::ParseTreePathPopped()--"
      "current parse tree "
      "path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
  }

  void DoTranslateTreeTraverser::CurrentNodeIsLastAddedRightChild(
    //WORD wCurrentParseTreeLevel
    )
  {
//    m_stdvector_p_grammarpartCurrentParseTreePath.push_back(
//      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;
//    m_vec_wGrammarPartIDcurrentParsePath.push_back(
//      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart ) ;
    LOGN_DEBUG(//"TranslateTreeTraverser::CurrentNodeIsLastAddedRightChild() "
      "begin"
      "--current parse tree path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
#ifdef _DEBUG
    std::string stdstr = m_r_parsebyrise.GetPathAs_std_string(
      m_vec_wGrammarPartIDcurrentParsePath) ;
#endif
//    m_map_grammarpartRightUnprocessedChild2wParseTreeLevel.insert(
//      std::pair<mcp_grammarpartCurrent,wCurrentParseTreeLevel> ) ;

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
#ifdef _DEBUG
    stdstr = m_r_parsebyrise.GetPathAs_std_string(
      m_vec_wGrammarPartIDcurrentParsePath) ;
    LOGN_DEBUG(//"TranslateTreeTraverser::CurrentNodeIsLastAddedRightChild() "
      "end"
      "--current parse tree path: " << m_r_parsebyrise.GetPathAs_std_string(
        m_stdvector_p_grammarpartCurrentParseTreePath)
      )
#endif
//    HandlePossibleSubject();//not neccessary anymore? (see method documentat.)
  }

  /** This is needed for _many_ dropdown lists to select the same indices. */
  void DoTranslateTreeTraverser::SetSameConsecutiveIDforLeaves(
    const GrammarPart * p_grammarpart)
  {
    ParseTreeTraverser::SetSameConsecutiveIDforLeaves trav(
      p_grammarpart ,
      mp_parsebyrise ,
      ++ m_wConsecutiveID
      ) ;
    trav.Traverse() ;
  }
}
