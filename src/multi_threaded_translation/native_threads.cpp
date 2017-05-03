/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "nativeThreads.hpp"
#include "Controller/TranslationControllerBase.hpp"
#include <unistd.h> //usleep(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>

extern TranslationControllerBase * g_p_translationcontrollerbase;

namespace VTrans3 {
  
  MultiThreadedTranslation::MultiThreadedTranslation(fastestUnsignedDataType number)
    : m_numThreads(number)
  {
    threads = new nativeThread_type[number];
  }
  MultiThreadedTranslation::~MultiThreadedTranslation() {
    delete [] threads;
  }
  
  void MultiThreadedTranslation::WaitForThreadBecomingIdle()
  {
    //Wait for the 1st thread becoming to terminate.
    // This equals the "WaitForMultipleObjects" function in Windows API.
    
//    if( threads[0].WaitForTermination() )
  }
  
  DWORD THREAD_FUNCTION_CALLING_CONVENTION execThreadFunction(void * p)
  {
    ParallelParams * p_parallelParams = (ParallelParams *) p;
    if( p_parallelParams )
    {
      /** A thread name can only be set in the thread function under Linux (
       *  With "pthread_create(...)" there is no possibility to determine it.*/
      I_Thread::SetThreadName(
        ("parTransl#" + convertToStdString(index)).c_str(), 
        GetCurrentThreadNumber() );
      WordCompoundsAtSameTokenIndex wordCompoundsAtSameTokenIndex;
      (p_parallelParams->p_translateParseByRiseTree->*
        p_parallelParams->processParseTreeFunction)(
        p_parallelParams->p_GrammarPart, 
        wordCompoundsAtSameTokenIndex);
      delete p_parallelParams;
      
      
    }
  }
  
//  void MultiThreadedTranslation::CreateThreads(fastestUnsignedDataType number)
//  {
//    
////    for()
//    {
//      nativeThread_type;
//    }
//  }
  
  /** Waits for all threads to terminate. This function may be executed at the
   *  latest possible point (before the data structures that are used by the 
   *  threads are destroyed. */
  void MultiThreadedTranslation::EnsureAllThreadsEnded()
  {
    for( fastestUnsignedDataType index = 0; index < m_numThreads; index++)
    {
      threads[index].WaitForTermination();
    }
    //TODO all changes of all threads need to be updated in main translation 
    // thread?! -> i.e. memory barrier?? 
  }
  
  /** implement/simulate n:1 one:one-signalling (TU Berlin, lecture "OS design",
   * 2015/16, slide 19) as the "WaitForMultipleObjects(..., bWaitAll=false, ) 
   *  Windows API function does. */
  void MultiThreadedTranslation::execute(
    TranslateParseByRiseTree::ProcessParseTree_type processParseTreeFunction,
    TranslateParseByRiseTree * p_translateParseByRiseTree,
    GrammarPart * p_GrammarPart
    )
  {
//    bool scheduled = false;
    ParallelParams * p_parallelParams = NULL;
    do
    {
      for( fastestUnsignedDataType index = 0; index < m_numThreads; index++)
      {
        if( ! threads[index].IsRunning() )
        {
          p_parallelParams = new ParallelParams {
            processParseTreeFunction, 
            p_translateParseByRiseTree, 
            p_GrammarPart,
            index
            };
          if( threads[index].start(execThreadFunction, p_parallelParams 
            //, ("parTransl#" + convertToStdString(index)).c_str() 
            ) > 0 )
            g_p_translationcontrollerbase->Message("Starting a translation thread failed:");
  //        scheduled = true;
          break;
        }
      }
      if( ! p_parallelParams )
      {
//        nanosleep(1000);
        //TODO this is busy waiting->the thread calling this function may get
        // 100% CPU time for the CPU core it runs on.
        usleep(1000);
      }
    }while(! p_parallelParams);
  }
}
