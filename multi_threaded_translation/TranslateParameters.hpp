/* 
 * File:   TranslateParameters.hpp
 * Author: mr.sys
 *
 * Created on 13. Mai 2014, 18:42
 */

#ifndef TRANSLATEPARAMETERS_HPP
#define	TRANSLATEPARAMETERS_HPP

#include <string> //std::string
//#include <Attributes/TranslationResult.hpp>

//fwd decl.
class TranslationControllerBase;
class I_UserInterface;
class TranslationResult;

namespace VTrans
{
  namespace multiThreadedTranslation
  {
    class TranslateParameters
    {
    public:
      std::string m_std_strWholeInputText;
      TranslationControllerBase * m_p_translationControllerBase;
      I_UserInterface * m_p_i_userInterface;
      long double m_translationDurationInSeconds;
      TranslationResult * m_p_translationResult;
      
      /** @param i_userInterface : pass as reference, else the pointer is falsely 
           assigned (bad pointer address). */
      TranslateParameters(
        const std::string & std_strWholeInputText,
        TranslationControllerBase * p_translationControllerBase,
        TranslationResult * p_translationResult,
        I_UserInterface & i_userInterface
        )
        :
        m_std_strWholeInputText(std_strWholeInputText),
        m_p_translationControllerBase(p_translationControllerBase),
        m_p_translationResult(p_translationResult),
        m_p_i_userInterface( //(I_UserInterface * ) p_translationControllerBase
          & i_userInterface)
      {
      }
    };
  }
}

#endif	/* TRANSLATEPARAMETERS_HPP */

