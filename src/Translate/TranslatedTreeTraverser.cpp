/** TranslatedTreeTraverser.cpp
 *  Created on: 13.08.2011
 *      Author: Stefan */

#include "TranslatedTreeTraverser.hpp"
#include <Parse/GrammarPart.hpp> //class GrammarPart

//TODO just for testing if log statements affect the parallel translation
#ifdef LOGN_DEBUG
  #undef LOGN_DEBUG
  #define LOGN_DEBUG(to_ostream) ; /*empty->do not log*/
#endif

namespace ParseTreeTraverser
{
  void TranslatedTreeTraverser::LeaveFound()
  {
    GrammarPart * p_grammarpart =
      m_grammarpartpointer_and_parselevelCurrent.m_p_grammarpart;
    if( p_grammarpart )
    {
      m_std_vector_p_grammarpartTranslation.push_back(p_grammarpart);
      m_std_vector_translationandgrammarpart.push_back(
        TranslationAndGrammarPart(
          p_grammarpart->m_stdstrTranslation,
          p_grammarpart
          )
        );
#ifdef _DEBUG
      const std::string & stdstrTranslation = p_grammarpart->m_stdstrTranslation;
      m_std_strWholeTranslation += stdstrTranslation + " ";
#else
      m_std_strWholeTranslation += p_grammarpart->m_stdstrTranslation + " ";
#endif
    }
  }
}
