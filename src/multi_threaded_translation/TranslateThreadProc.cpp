
#include "TranslateParameters.hpp"
#include "TranslateThreadProc.hpp"

#include <Controller/TranslationControllerBase.hpp>
#include <FileSystem/File/FileException.hpp>
#include <IO/dictionary/DictionaryFileAccessException.hpp>
#include <UserInterface/I_UserInterface.hpp>
//#include <OperatingSystem/time/GetCurrentTime.hpp>
//OperatingSystem::GetTimeCountInSeconds()
#include <Controller/time/GetTickCount.hpp>

namespace VTrans
{
    THREAD_FUNCTION_CALLING_CONVENTION DWORD TranslateThreadProc(void * p_vThreadParams)
    {
    	I_Thread::SetCurrentThreadName("Translate");
     LOGN_DEBUG("begin")
      VTrans::multiThreadedTranslation::TranslateParameters * 
        p_translateParameters =
        (VTrans::multiThreadedTranslation::TranslateParameters * ) 
        p_vThreadParams;
      if( p_translateParameters )
      {
        try
        {
        std::vector<std::string> stdvec_stdstrWholeTransl ;
        
        long double timeCountInSecondsBeforeTranslation;
        OperatingSystem::GetTimeCountInSeconds(timeCountInSecondsBeforeTranslation);
        p_translateParameters->m_p_translationControllerBase->Translate(
          p_translateParameters->m_std_strWholeInputText ,
          stdvec_stdstrWholeTransl ,
//          * p_translateParameters->m_p_translationResult
          & TranslateParseByRiseTree::TranslateParseTree,
          1
          );
        
        if( p_translateParameters->m_p_translationControllerBase->m_vbContinue )
        {
          long double timeCountInSecondsAfterTranslation;
          OperatingSystem::GetTimeCountInSeconds(timeCountInSecondsAfterTranslation);
          p_translateParameters->m_p_translationControllerBase->
            m_translationDurationInSeconds = timeCountInSecondsAfterTranslation -
            timeCountInSecondsBeforeTranslation;
          p_translateParameters->//p_translationControllerBase
            m_p_i_userInterface->
            UpdateAfterTranslation();
        }
        else
          p_translateParameters->m_p_translationControllerBase->m_vbContinue = true;

        /** Should have been allocated on heap before. */
        delete p_translateParameters;
      }
        catch(const DictionaryFileAccessException & dictionaryFileAccessException)
        {
          p_translateParameters->//p_translationControllerBase
            m_p_i_userInterface->Message(dictionaryFileAccessException.GetErrorMessageA());
        }
      catch(const FileException & fe)
      {
        LOGN_ERROR("file exception occurred")
        p_translateParameters->//p_translationControllerBase
          m_p_i_userInterface->Message("file exception occurred while translating");
      }
      catch(...)
      {
        LOGN_ERROR("exception occurred")
        p_translateParameters->//p_translationControllerBase
          m_p_i_userInterface->Message("unknown exception occurred while translating");
      }
      }
     LOGN_DEBUG("end")
    }
}
