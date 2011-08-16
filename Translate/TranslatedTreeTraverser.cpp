/*
 * TranslatedTreeTraverser.cpp
 *
 *  Created on: 13.08.2011
 *      Author: Stefan
 */

#include "TranslatedTreeTraverser.hpp"
#include <Parse/GrammarPart.hpp> //class GrammarPart

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
      m_std_strWholeTranslation += p_grammarpart->m_stdstrTranslation + " ";
    }
  }
}
