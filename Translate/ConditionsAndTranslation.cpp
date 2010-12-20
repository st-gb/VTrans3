/*
 * ConditionsAndTranslation.cpp
 *
 *  Created on: Jun 7, 2010
 *      Author: Stefan
 */
#include "ConditionsAndTranslation.hpp"
#include <Translate/AttributeTypeAndPosAndSize.hpp>
#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp>
#include <VocabularyInMainMem/LetterTree/VocabularyAndTranslation.hpp>
#include <map> //std::map
//SUPPRESS_UNUSED_VARIABLE_WARNING(...)
#include <preprocessor_macros/suppress_unused_variable.h>

bool ConditionsAndTranslation::AllConditionsMatch(
  // So it can be used with data from outside this class.
  //, const std::vector<WORD> & r_stdvec_wGrammarPartPath
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
  ) const
{
  bool bAllConditionsMatch = false ;
//  int i = 0 ;
  //TODO implement this
//        if( r_cnt.conditions.m_byCompareType == ConditionsAndTranslation::equals
//          && AttributeTypeAndPosAndSize::English && rightof("==") equals(
//          grammarpart.string &&
//          mp_parsebyrise->psv.getTokens(
//              grammarpart.index, index2) )
  if( m_conditions.size() > 0 )
  {
    GrammarPart * p_grammarpartLeaf ;
    bAllConditionsMatch = true ;
//    const Condition & cr_condition =
//      r_conditions_and_translation.m_conditions.front() ;
    std::vector<Condition> r_vec_condition = m_conditions ;
    for( std::vector<Condition>::const_iterator c_iter =
        r_vec_condition.begin() ;
        c_iter != r_vec_condition.end() ;
        ++ c_iter
        )
    {
      const Condition & cr_condition = *c_iter ;
      //if( r_cond.m_syntaxtreepath.IsPartOf(m_stdvec_wCurrentGrammarPartPath) )
      //TODO get pointer to leaf node of r_cond.m_syntaxtreepath
      p_grammarpartLeaf = cr_condition.m_syntaxtreepath.GetLeaf(
        r_stdvec_p_grammarpartPath ) ;
      if( p_grammarpartLeaf &&
        p_grammarpartLeaf->m_pvocabularyandtranslation )
      {
        //if( p_grammarpartLeaf )
        {
          DEBUG_COUT("grammar part leaf found:" <<
            sp_parsebyrise->GetGrammarPartName(
            p_grammarpartLeaf->m_wGrammarPartID) << "\n")
        }
        std::map<std::string,AttributeTypeAndPosAndSize>::const_iterator
          c_iter_stdstrSttrName2atapas =
              sp_stdmap_AttrName2VocAndTranslAttrDef->find(
            cr_condition.m_stdstrAttributeName ) ;

        //The attribute name (e.g. "German_singular" exists in the map
        if( c_iter_stdstrSttrName2atapas !=
          sp_stdmap_AttrName2VocAndTranslAttrDef->end()
          )
        {
          //p_grammarpartLeaf->m_psetpvocabularyandtranslation  ;
          //if( p_grammarpartLeaf->m_psetpvocabularyandtranslation )
          {
            const AttributeTypeAndPosAndSize & r_atapas =
              c_iter_stdstrSttrName2atapas->second ;
            switch( r_atapas.m_byAttrDataType )
            {
              case AttributeTypeAndPosAndSize::string :
              {
                if( r_atapas.m_byLanguage ==
                  AttributeTypeAndPosAndSize::English )
                {
                  std::string & r_stdstrAttrVal = p_grammarpartLeaf->
                    m_pvocabularyandtranslation->m_arstrEnglishWord[
                    r_atapas.m_wIndex ] ;
                  //std::string & r_stdstrTextTokens =
                  std::string stdstrTextTokens ;
                    sp_parsebyrise->GetTokensAsSpaceSeparatedString(
                    p_grammarpartLeaf->m_dwLeftmostIndex,
                    p_grammarpartLeaf->m_dwRightmostIndex ,
                    stdstrTextTokens
                    ) ;
                  if( stdstrTextTokens != ""  && stdstrTextTokens ==
                    r_stdstrAttrVal )
                  {
                    //bIdentical = true ;
                  }
                  else
                  {
                    bAllConditionsMatch = false ;
                    //break ;
                    return false ;
                  }
                }
              }
              break ;
              case AttributeTypeAndPosAndSize::bit :
                if( r_atapas.m_byLanguage ==
                  AttributeTypeAndPosAndSize::German )
                {
                  BYTE by = p_grammarpartLeaf->
                    m_pvocabularyandtranslation->m_arbyAttribute[
                    r_atapas.m_wIndex ] ;
                  if( cr_condition.m_byAttributeValue != by )
                    //bAllConditionsMatch = false ;
                    return false ;
                }

            }//switch
          }
          //if( r_cond.m_byCompareType == Condition::equals )
        }
      }
      else
        return false ;
    } //loop over all conditions
  }
  else
    //0 conditions, e.g. for "3rd person singular finite verb" grammar part
    bAllConditionsMatch = true ;
  return bAllConditionsMatch ;
}

std::string ConditionsAndTranslation::GetTranslationEquivalent(
  //A leaf contain the needed pointer to a VocabularyAndTranslation object.
  const GrammarPart * const p_grammarpartLeaf
  ) const
{
  bool bAllConditionsMatch = false ;
  std::string stdstr ;
  DEBUGN("ConditionsAndTranslation::GetTranslationEquivalent(...)"
    << "german transltation " << m_stdstrGermanTranslation )
//  int i = 0 ;
  //TODO implement this
//        if( r_cnt.conditions.m_byCompareType == ConditionsAndTranslation::equals
//          && AttributeTypeAndPosAndSize::English && rightof("==") equals(
//          grammarpart.string &&
//          mp_parsebyrise->psv.getTokens(
//              grammarpart.index, index2) )

  //If no simple replacement (like use "die" for "the" + "English_plural"
  if( m_stdstrGermanTranslation ==  "" )
  {
    DEBUGN("German translation string is empty -> use attribute value"
      << "p_grammarpartLeaf:" << p_grammarpartLeaf )
#ifdef COMPILE_WITH_LOG
    if( p_grammarpartLeaf )
      DEBUGN("p_grammarpartLeaf->m_pvocabularyandtranslation:"
        << p_grammarpartLeaf->m_pvocabularyandtranslation )
#endif //#ifdef COMPILE_WITH_LOG
    bAllConditionsMatch = true ;
    //if( r_cond.m_syntaxtreepath.IsPartOf(m_stdvec_wCurrentGrammarPartPath) )
    //TODO get pointer to leaf node of r_cond.m_syntaxtreepath
//    GrammarPart * p_grammarpartLeaf = r_condition_and_translation.
//        m_syntaxtreepath.GetLeaf( r_stdvec_p_grammarpartPath) ;
    if( p_grammarpartLeaf &&
      p_grammarpartLeaf->m_pvocabularyandtranslation )
    {
      //if( p_grammarpartLeaf )
      {
//        DEBUG_COUTN
        DEBUGN("GetTranslationEquivalent()--grammar part leaf found:" <<
          sp_parsebyrise->GetGrammarPartName(
          p_grammarpartLeaf->m_wGrammarPartID) )
      }
      std::map<std::string,AttributeTypeAndPosAndSize>::const_iterator
        c_iter_stdstrSttrName2atapas =
            sp_stdmap_AttrName2VocAndTranslAttrDef->find(
          m_stdstrAttributeName ) ;
      if( c_iter_stdstrSttrName2atapas ==
          sp_stdmap_AttrName2VocAndTranslAttrDef->end()
        )
//        DEBUG_COUTN
        DEBUGN("GetTranslationEquivalent()--no AttributeTypeAndPosAndSize "
          "for attribute name" << m_stdstrAttributeName << " found" )
      else
      {
        DEBUGN("AttributeTypeAndPosAndSize for attribute name "
          << m_stdstrAttributeName << " found ")
        //p_grammarpartLeaf->m_psetpvocabularyandtranslation  ;
        //if( p_grammarpartLeaf->m_psetpvocabularyandtranslation )
        {
          const AttributeTypeAndPosAndSize & r_atapas =
            c_iter_stdstrSttrName2atapas->second ;
          switch( r_atapas.m_byAttrDataType )
          {
            case AttributeTypeAndPosAndSize::string :
            {
//              DEBUG_COUTN
              DEBUGN("data type for choosing attribute value is string")
              switch( r_atapas.m_byLanguage )
              {
                case AttributeTypeAndPosAndSize::English :
                {
                  DEBUGN("language for choosing attribute value is "
                    "English, index:" << r_atapas.m_wIndex )
                  std::string & r_stdstrAttrVal = p_grammarpartLeaf->
                    m_pvocabularyandtranslation->m_arstrEnglishWord[
                    r_atapas.m_wIndex ] ;
                  //std::string & r_stdstrTextTokens =
                  return r_stdstrAttrVal ;
                }
                  break ;
                case AttributeTypeAndPosAndSize::German :
                  {
//                  DEBUG_COUTN
                  DEBUGN("language for choosing attribute value is "
                    "German, index:" << r_atapas.m_wIndex
                    << "m_pfn_TransformString:" << m_pfn_TransformString )
                  std::string & r_stdstrAttrVal = p_grammarpartLeaf->
                    m_pvocabularyandtranslation->m_arstrGermanWord[
                    r_atapas.m_wIndex ] ;
                  if( m_pfn_TransformString )
                  {
                    //Use a copy, else the VocAndTransl's string is modified
                    //by the function
                    std::string stdstrAttrVal = r_stdstrAttrVal ;
                    m_pfn_TransformString(stdstrAttrVal) ;
                    return stdstrAttrVal ;
                  }
                  else
                    //std::string & r_stdstrTextTokens =
                    return r_stdstrAttrVal ;
                  }
                  break ;
              }
            }
            break ;
            case AttributeTypeAndPosAndSize::bit :
              if( r_atapas.m_byLanguage ==
                AttributeTypeAndPosAndSize::German )
              {
                //see http://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html:
//                #pragma GCC diagnostic ignored  "-Wunused"
                BYTE by =
                  p_grammarpartLeaf->
                  m_pvocabularyandtranslation->m_arbyAttribute[
                  r_atapas.m_wIndex ] ;
                SUPPRESS_UNUSED_VARIABLE_WARNING(by)
              }
          }//switch
        }
        //if( r_cond.m_byCompareType == Condition::equals )
      }
    }
  }
  else
  {
    stdstr = m_stdstrGermanTranslation ;
  }
  return stdstr ;
}
