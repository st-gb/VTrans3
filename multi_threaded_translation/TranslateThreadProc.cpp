
#include "TranslateParameters.hpp"
#include "TranslateThreadProc.hpp"

#include <Controller/TranslationControllerBase.hpp>
#include <UserInterface/I_UserInterface.hpp>
//#include <OperatingSystem/time/GetCurrentTime.hpp>
//OperatingSystem::GetTimeCountInSeconds()
#include <Controller/time/GetTickCount.hpp>

namespace VTrans
{
    THREAD_FUNCTION_CALLING_CONVENTION DWORD TranslateThreadProc(void * p_vThreadParams)
    {
      VTrans::multiThreadedTranslation::TranslateParameters * 
        p_translateParameters =
        (VTrans::multiThreadedTranslation::TranslateParameters * ) 
        p_vThreadParams;
      if( p_translateParameters )
      {
        std::vector<std::string> stdvec_stdstrWholeTransl ;
      //  std::vector<std::vector<TranslationAndGrammarPart> >
      //    stdvec_stdvecTranslationAndGrammarPart ;
        
        long double timeCountInSecondsBegin;
        OperatingSystem::GetTimeCountInSeconds(timeCountInSecondsBegin);
        p_translateParameters->m_p_translationControllerBase->Translate(
          p_translateParameters->m_std_strWholeInputText ,
          stdvec_stdstrWholeTransl ,
        //    stdvec_stdvecTranslationAndGrammarPart
          //stdvec_stdvec_stdvecTranslationAndGrammarPart
          * p_translateParameters->m_p_translationResult
          );
        
        if( p_translateParameters->m_p_translationControllerBase->m_vbContinue )
        {
          long double timeCountInSeconds;
          OperatingSystem::GetTimeCountInSeconds(timeCountInSeconds);
          p_translateParameters->m_p_translationControllerBase->
            m_translationDurationInSeconds = timeCountInSeconds -
            timeCountInSecondsBegin;
          p_translateParameters->//p_translationControllerBase
            m_p_i_userInterface->
            UpdateAfterTranslation();
        }
        else
          p_translateParameters->m_p_translationControllerBase->m_vbContinue = true;

        /** Should have been allocated on heap before. */
        delete p_translateParameters;
      }
    }
}
