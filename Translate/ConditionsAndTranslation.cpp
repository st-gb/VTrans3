/*
 * ConditionsAndTranslation.cpp
 *
 *  Created on: Jun 7, 2010
 *      Author: Stefan
 */
#include "ConditionsAndTranslation.hpp"
//#include <Attributes/EnglishWord.hpp> //class EnglishWord
#include <Parse/GrammarPart.hpp>
#include <Parse/ParseByRise.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN()
#include <Translate/AttributeTypeAndPosAndSize.hpp>
#include <UserInterface/I_UserInterface.hpp> //class I_UserInterface
#include <VocabularyInMainMem/VocabularyAndTranslation.hpp>
#include <map> //std::map
//SUPPRESS_UNUSED_VARIABLE_WARNING(...)
#include <preprocessor_macros/suppress_unused_variable.h>

//A static variable also needs to be defined.
std::map<std::string, pfnTransformString> ConditionsAndTranslation::
  s_std_mapFunctionName2Function;

bool ConditionsAndTranslation::AllConditionsMatch(
  // So it can be used with data from outside this class.
  //, const std::vector<WORD> & r_stdvec_wGrammarPartPath
  const std::vector<GrammarPart *> & r_stdvec_p_grammarpartPath
  ) const
{
  bool bAllConditionsMatch = false ;
  LOGN_DEBUG("# of conditions for current translation rule:" <<
    m_conditions.size() )
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
    for( std::vector<Condition>::const_iterator c_ConditionIterator =
        r_vec_condition.begin() ;
        c_ConditionIterator != r_vec_condition.end() ;
        ++ c_ConditionIterator
        )
    {
      const Condition & cr_condition = *c_ConditionIterator ;
      std::string std_stringSyntaxTreePathOfCondition = cr_condition.
        m_syntaxtreepath.GetAs_std_string();
      LOGN_DEBUG("current condition's syntax tree path:" <<
        std_stringSyntaxTreePathOfCondition )
      //if( r_cond.m_syntaxtreepath.IsPartOf(m_stdvec_wCurrentGrammarPartPath) )
      //TODO get pointer to leaf node of r_cond.m_syntaxtreepath
      p_grammarpartLeaf = cr_condition.m_syntaxtreepath.GetLeaf(
        r_stdvec_p_grammarpartPath ) ;
      if( p_grammarpartLeaf &&
        p_grammarpartLeaf->m_pvocabularyandtranslation )
      {
        //if( p_grammarpartLeaf )
//        {
//          DEBUG_COUT
          LOGN_DEBUG("grammar part leaf found:" <<
            sp_parsebyrise->GetGrammarPartName(
            p_grammarpartLeaf->m_wGrammarPartID)
            //<< "\n"
            )
//        }
        std::map<std::string,AttributeTypeAndPosAndSize>::const_iterator
          c_iter_stdstrSttrName2atapas =
              sp_stdmap_AttrName2VocAndTranslAttrDef->find(
            cr_condition.m_stdstrAttributeName ) ;

        //The attribute name (e.g. "German_singular" exists in the map
        if( c_iter_stdstrSttrName2atapas !=
          sp_stdmap_AttrName2VocAndTranslAttrDef->end()
          )
        {
          LOGN_DEBUG( //"ConditionsAndTranslation::AllConditionsMatch(...)--"
            "\"" << cr_condition.m_stdstrAttributeName
            << "\" could be found in the attribute names container.")
          //p_grammarpartLeaf->m_psetpvocabularyandtranslation  ;
          //if( p_grammarpartLeaf->m_psetpvocabularyandtranslation )
          {
            const AttributeTypeAndPosAndSize & r_atapas =
              c_iter_stdstrSttrName2atapas->second ;
            switch( r_atapas.m_byAttrDataType )
            {
              case AttributeTypeAndPosAndSize::string :
              {
                LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                  "attribute type is string" )
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
                    LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                      "attribute value \"" <<
                      stdstrTextTokens << "\" matches leaf's value" )
                  }
                  else
                  {
                    LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                      "attribute value \"" <<
                      stdstrTextTokens << "\" MISmatches leaf's value" )
                    bAllConditionsMatch = false ;
                    //break ;
                    //return false ;
                  }
                }
              }
              break ;
              case AttributeTypeAndPosAndSize::bit :
                LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                  "attribute type is bit" )
                if( r_atapas.m_byLanguage ==
                  AttributeTypeAndPosAndSize::German )
                {
                  LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                    "attribute refers to the German "
                    "language" )
                  BYTE by = p_grammarpartLeaf->
                    m_pvocabularyandtranslation->m_arbyAttribute[
                    r_atapas.m_wIndex ] ;
                  LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                    "attribute value is "
                    << (WORD) cr_condition.m_byAttributeValue
                    << ", leaf's value is: " << (WORD) by )
                  if( cr_condition.m_byAttributeValue != by )
                  {
                    LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                      "attribute value mismatches "
                      "leaf's value" )
                    bAllConditionsMatch = false ;
                  }
//                    return false ;
                }
                break;
            }//switch
            if( ! bAllConditionsMatch )
            {
              LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                "breaking conditions loop because of "
                "value MISmatch." )
              break; //"for"-loop
            }
          }
          //if( r_cond.m_byCompareType == Condition::equals )
        }
        else //Not found in the vocable attribute defintions.
        {
          LOGN_DEBUG( //"ConditionsAndTranslation::AllConditionsMatch(...)--"
            "\"" << cr_condition.m_stdstrAttributeName
            << "\" could not be found in the attribute names container.")
          bAllConditionsMatch = false ;
        }
      }
      else
      {
        LOGN_DEBUG("NO grammar part leaf found.")
        bAllConditionsMatch = false;
//        return false ;
      }
    } //loop over all conditions
  }
  else
    //0 conditions, e.g. for "3rd person singular finite verb" grammar part
    bAllConditionsMatch = true ;
  LOGN_DEBUG( //FULL_FUNC_NAME << "--"
    "returning " << bAllConditionsMatch )
  return bAllConditionsMatch ;
}

std::string ConditionsAndTranslation::GetTranslationEquivalent(
  //A leaf contain the needed pointer to a VocabularyAndTranslation object.
  const GrammarPart * const p_grammarpartLeaf
  ) const
{
  bool bAllConditionsMatch = false ;
  std::string stdstr ;
  LOGN_DEBUG( //"ConditionsAndTranslation::GetTranslationEquivalent(...)"
    //FULL_FUNC_NAME << "--"
    "German translation: \"" << m_stdstrGermanTranslation << "\"")
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
    LOGN_DEBUG( //FULL_FUNC_NAME << "--"
      "German translation string is empty -> use "
      "attribute value " << "p_grammarpartLeaf:" << p_grammarpartLeaf )
#ifdef COMPILE_WITH_LOG
    if( p_grammarpartLeaf )
      LOGN_DEBUG("p_grammarpartLeaf->m_pvocabularyandtranslation:"
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
        LOGN_DEBUG( //"GetTranslationEquivalent()"
          //FULL_FUNC_NAME << "--"
          "grammar part leaf found: \"" <<
          sp_parsebyrise->GetGrammarPartName(
          p_grammarpartLeaf->m_wGrammarPartID) << "\"")
      }
      std::map<std::string,AttributeTypeAndPosAndSize>::const_iterator
        c_iter_stdstrSttrName2atapas =
            sp_stdmap_AttrName2VocAndTranslAttrDef->find(
          m_stdstrAttributeName ) ;
      if( c_iter_stdstrSttrName2atapas ==
          sp_stdmap_AttrName2VocAndTranslAttrDef->end()
        )
//        DEBUG_COUTN
        LOGN_DEBUG(//"GetTranslationEquivalent()"
          //FULL_FUNC_NAME << "--"
          "no AttributeTypeAndPosAndSize "
          "for attribute name \"" << m_stdstrAttributeName << "\" found" )
      else
      {
        LOGN_DEBUG("AttributeTypeAndPosAndSize for attribute name \""
          << m_stdstrAttributeName << "\" found ")
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
              LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                "data type for choosing attribute value is string")
              switch( r_atapas.m_byLanguage )
              {
                case AttributeTypeAndPosAndSize::English :
                {
                  return p_grammarpartLeaf->m_pvocabularyandtranslation->
                    GetEnglishString( (BYTE) r_atapas.m_wIndex);
                }
                  break ;
                case AttributeTypeAndPosAndSize::German :
                {
//                  LOGN_DEBUG( //FULL_FUNC_NAME << "--"
//                    "language is German")
                  const VocabularyAndTranslation * const
                    p_vocabularyandtranslation =
                    p_grammarpartLeaf->m_pvocabularyandtranslation;

                  LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                    "\"voc and translation pointer\" is:" <<
                    p_vocabularyandtranslation )

                  std::string stdstrAttrVal = p_vocabularyandtranslation->
                    GetGermanString( (BYTE) r_atapas.m_wIndex);

                  LOGN_INFO( //FULL_FUNC_NAME << "--"
                    "attribute value at voc&transl's"
                    << r_atapas.GetLanguageAsString() << " string index "
                    << r_atapas.m_wIndex << " is \""
                    << stdstrAttrVal << "\""
                    << " for current leaf"
                    )

                  LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                    "m_pfn_TransformString for STP "
                    << m_syntaxtreepath.GetAs_std_string() << " is \"" <<
                    (void *) m_pfn_TransformString << "\"")
                  //If function that modifies as string (like for
                  //"I trust the children." : for "Kinder" ->
                  //"Kindern")
                  //TODO possibly use this for getting verb attributes like
                  //past participle/ finite verb forms from an infinitive.
                  if( m_pfn_TransformString )
                  {
                    //Use a copy, else the VocAndTransl's string is modified
                    //by the function
//                    std::string stdstrAttrVal = r_stdstrAttrVal ;
                    m_pfn_TransformString(stdstrAttrVal) ;
                    LOGN_DEBUG( //FULL_FUNC_NAME << "--"
                      "attribute value after transforming: \""
                      << stdstrAttrVal << "\"")
                    return stdstrAttrVal ;
                  }
                  else
                    //std::string & r_stdstrTextTokens =
                    return stdstrAttrVal ;
                }
                break ;
              }
            }
            break ;
            case AttributeTypeAndPosAndSize::bit :
              if( r_atapas.m_byLanguage ==
                AttributeTypeAndPosAndSize::German )
              {
                p_grammarpartLeaf->m_pvocabularyandtranslation->
                  GetAttributeValue( (BYTE) r_atapas.m_wIndex);
              }
              break ;
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
