
#include "TranslateParameters.hpp"
#include "TranslateThreadProc.hpp"

#include <Controller/TranslationControllerBase.hpp>
#include <UserInterface/I_UserInterface.hpp>

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
        /** A vector of sentences that begin at the same token index
        * (sentences that begin at the same token index:
        * vector of sentences that each contains a vector of words). */
        //TODO exchange "std::vector<ele>" by a class that is derived from std::vector
//        std::vector <std::vector <std::vector <TranslationAndGrammarPart>
//          /* WordCompound */ > >
//          stdvec_stdvec_stdvecTranslationAndGrammarPart;
        
        p_translateParameters->m_p_translationControllerBase->Translate(
          p_translateParameters->m_std_strWholeInputText ,
          stdvec_stdstrWholeTransl ,
        //    stdvec_stdvecTranslationAndGrammarPart
          //stdvec_stdvec_stdvecTranslationAndGrammarPart
          * p_translateParameters->m_p_translationResult
          );
        
        p_translateParameters->//p_translationControllerBase
          m_p_i_userInterface->
          UpdateAfterTranslation();

        /** Should have been allocated on heap before. */
        delete p_translateParameters;
      }
    }
}
