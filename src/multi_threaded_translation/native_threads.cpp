/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "nativeThreads.hpp"
#include "Controller/TranslationControllerBase.hpp"
#include <unistd.h> //usleep(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>
#include <OperatingSystem/GetErrorMessageFromLastErrorCode.hpp>
#include <hardware/CPU/atomic/AtomicExchange.h>

extern TranslationControllerBase * g_p_translationcontrollerbase;
static long int s_indexOfEndingThread;

namespace VTrans3 {
  
  MultiThreadedTranslation::MultiThreadedTranslation(fastestUnsignedDataType number)
    : m_numThreads(number)
  {
    threads = new nativeThread_type[number];
    
    pthread_cond_init(& m_pthread_cond_t, NULL);
    pthread_mutex_init(& m_pthread_mutex_t, NULL);
  }
  MultiThreadedTranslation::~MultiThreadedTranslation() {
    delete [] threads;
    pthread_cond_destroy(& m_pthread_cond_t);
    pthread_mutex_destroy(& m_pthread_mutex_t);
  }
  
  void MultiThreadedTranslation::WaitForThreadBecomingIdle()
  {
    //Wait for the 1st thread becoming to terminate.
    // This equals the "WaitForMultipleObjects" function in Windows API.
    
//    if( threads[0].WaitForTermination() )
  }

  inline void LockForSignallingThisThreadEnds(pthread_mutex_t * p_pthread_mutex_t )
  {
    LOGN_DEBUG("Before locking for signalling this thread (" << 
      p_parallelParams->translThreadID << " ended")
    int returnValue = pthread_mutex_lock(p_pthread_mutex_t);
    LOGN_DEBUG("After locking for signalling this thread (" << 
      p_parallelParams->translThreadID << ") ended")
    if( returnValue != 0 )
      returnValue++;
  }

  inline void SignallingThisThreadEnds(pthread_cond_t * p_pthread_cond_t)
  {
    LOGN_DEBUG("Before signalling this thread (" << 
      p_parallelParams->translThreadID << " ended")
    int returnValue = pthread_cond_broadcast(p_pthread_cond_t);  
    LOGN_DEBUG("After signalling this thread (" << 
      p_parallelParams->translThreadID << ") ended")
    if( returnValue != 0 )
      returnValue++;
  }
  
  inline void UnlockAfterSignallingThisThreadEnds(pthread_mutex_t * p_pthread_mutex_t)
  {
    LOGN_DEBUG("Before unlocking after signalling this thread (" << 
      p_parallelParams->translThreadID << ") ended")
    returnValue = pthread_mutex_unlock(p_parallelParams->p_pthread_mutex_t);
    LOGN_DEBUG("After unlocking after signalling this thread (" << 
      p_parallelParams->translThreadID << ") ended")
    if( returnValue != 0 )
      returnValue++;
  }
  
  DWORD THREAD_FUNCTION_CALLING_CONVENTION execThreadFunction(void * p)
  {
    LOGN_DEBUG("begin, transl. index:" << p_parallelParams->translThreadID)
    ParallelParams * p_parallelParams = (ParallelParams *) p;
    if( p_parallelParams )
    {
      /** A thread name can only be set in the thread function under Linux (
       *  With "pthread_create(...)" there is no possibility to determine it.*/
      I_Thread::SetThreadName(
        ("parTransl#" + convertToStdString(p_parallelParams->translThreadID) ).c_str(), 
        GetCurrentThreadNumber() );
      WordCompoundsAtSameTokenIndex wordCompoundsAtSameTokenIndex;
      (p_parallelParams->p_translateParseByRiseTree->*
        p_parallelParams->processParseTreeFunction)(
        p_parallelParams->p_GrammarPart, 
        wordCompoundsAtSameTokenIndex);
      
//      p_parallelParams->p_threadAssignCritSec->Enter();
//      p_parallelParams->p_threadAllocEvent->Broadcast();
//      p_parallelParams->p_threadAssignCritSec->Leave();
      
      //TODO POSIX-specific:
      LockForSignallingThisThreadEnds(p_parallelParams->p_pthread_mutex_t);
      
      AtomicExchange(& s_indexOfEndingThread, p_parallelParams->translThreadID);
      
      SignallingThisThreadEnds(p_parallelParams->p_pthread_cond_t);
      
      UnlockAfterSignallingThisThreadEnds(p_parallelParams->p_pthread_mutex_t);
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
      threads[index].WaitForTermination(); //TODO SIGSEV here
    }
    //TODO all changes of all threads need to be updated in main translation 
    // thread?! -> i.e. memory barrier?? 
  }
    
  void MultiThreadedTranslation::WaitForEndingThreadToSignal()
  {
//        p_parallelParams->condition.wait();
//        threadAllocEvent.Wait();
//        pthread_mutex_lock( & m_pthread_mutex_t);
    LOGN_DEBUG("Before waiting for ending thread to signal")
    /** "pthread_cond_wait" unlocks the mutex?! */
    int returnValue = pthread_cond_wait( & m_pthread_cond_t, & m_pthread_mutex_t);
    LOGN_DEBUG("after waiting for ending thread to signal--return value:" 
      << returnValue )
    if( returnValue != 0 )
      returnValue = errno;
  }
  
  inline void LockForCheckAllThreadOrWaitForThreadEnd( 
    pthread_mutex_t * p_pthread_mutex_t )
  {
    LOGN_DEBUG("Before locking for atomic: check all threads or wait for thread end")
//      threadAssignCritSec.Enter();
    int returnValue = pthread_mutex_lock( p_pthread_mutex_t);
    LOGN_DEBUG("After locking for atomic: check all threads or wait for thread end."
      "return value:" << returnValue)
    if( returnValue != 0 )
      returnValue++;
  }
  
  inline void UnlockForCheckAllThreadOrWaitForThreadEnd( 
    pthread_mutex_t * p_pthread_mutex_t )
  {
//      threadAssignCritSec.Leave();
    LOGN_DEBUG("Before Unlocking after atomic: check all threads or wait for thread end")
    int returnValue = pthread_mutex_unlock( p_pthread_mutex_t);
    LOGN_DEBUG("After Unlocking after atomic : check all threads or wait for thread end " << returnValue)
    if( returnValue != 0)
      returnValue = errno;
  }
  
  void MultiThreadedTranslation::StartNewThread(
    const ProcessParseTreeParams & processParseTreeParams, 
    fastestUnsignedDataType threadIndex)
  {
    ParallelParams * p_parallelParams = new ParallelParams {
      processParseTreeParams.processParseTreeFunction, 
      processParseTreeParams.p_translateParseByRiseTree, 
      processParseTreeParams.p_GrammarPart,
      threadIndex,
//            & threadAssignCritSec,
//            & threadAllocEvent
      & m_pthread_mutex_t,
      & m_pthread_cond_t
      };
    LOGN_DEBUG("Starting new thread at thread index " << threadIndex)
    int startRet = threads[threadIndex].start(execThreadFunction, p_parallelParams 
      //, ("parTransl#" + convertToStdString(index)).c_str() 
      );
    if( startRet > 0 )
      g_p_translationcontrollerbase->Message("Starting a translation thread failed:"
        + OperatingSystem::GetErrorMessageFromErrorCodeA(startRet) );
//        scheduled = true;
  }
  
  inline void WaitForTerminationOfEndingThread(nativeThread_type & thread)
  {
    LOGN_DEBUG("Waiting for termination of thread index " << 
      s_indexOfEndingThread)
    thread.WaitForTermination();
    LOGN_DEBUG("After waiting for termination of thread index " << 
      s_indexOfEndingThread)
  }
  
  inline void BusyWaiting()
  {
//        nanosleep(1000);
        //TODO this is busy waiting->the thread calling this function may get
        // 100% CPU time for the CPU core it runs on.
//        usleep(1000);
  }

  /** Implement/simulate n:1 one:one-signalling (TU Berlin, lecture "OS design",
   * 2015/16, slide 19) as the "WaitForMultipleObjects(..., bWaitAll=false, ) 
   *  Windows API function does. */
  void MultiThreadedTranslation::execute(
    TranslateParseByRiseTree::ProcessParseTree_type processParseTreeFunction,
    TranslateParseByRiseTree * p_translateParseByRiseTree,
    GrammarPart * p_GrammarPart
    )
  {
    ProcessParseTreeParams processParseTreeParams {processParseTreeFunction, 
      p_translateParseByRiseTree, p_GrammarPart };
//    ParallelParams * p_parallelParams = NULL;
    bool noNonRunningThreadAvailable = true;
//    do
//    {
      /** Lock mutex so we can't miss a "thread ended" signal before placing
       * ourselves into a "wait for condition " state */
      LockForCheckAllThreadOrWaitForThreadEnd(& m_pthread_mutex_t);
      for( fastestUnsignedDataType threadIndex = 0; threadIndex < m_numThreads; 
          threadIndex++)
      {
        if( ! threads[threadIndex].IsRunning() )
        {
          StartNewThread(processParseTreeParams, threadIndex);
          noNonRunningThreadAvailable = false;
          break;
        }
      }
      if( /*! p_parallelParams*/ noNonRunningThreadAvailable )
      {
//        BusyWaiting();
        WaitForEndingThreadToSignal();
        
        /** Let other threads be able to terminate and signal in "execThreadFunction" */
        UnlockForCheckAllThreadOrWaitForThreadEnd(& m_pthread_mutex_t);
        
        WaitForTerminationOfEndingThread(threads[s_indexOfEndingThread]);

        StartNewThread(processParseTreeParams, s_indexOfEndingThread);
        
//        noNonRunningThreadAvailable = false;
      }
      else 
        UnlockForCheckAllThreadOrWaitForThreadEnd(& m_pthread_mutex_t);
//    }while( noNonRunningThreadAvailable);
  }
}
