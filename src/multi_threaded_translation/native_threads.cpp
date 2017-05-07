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
#include <hardware/CPU/atomic/memory_barrier.h>

extern TranslationControllerBase * g_p_translationcontrollerbase;
static long int s_indexOfEndingThread;

namespace VTrans3 {
  
  MultiThreadedTranslation::MultiThreadedTranslation(fastestUnsignedDataType number)
    : m_numThreads(number)
  {
    threads = new nativeThread_type[number];
    threadStates = new long int[number];
    memset( threadStates, idle, sizeof(long int) * number );
    
    pthread_cond_init(& m_pthread_cond_t, NULL);
    pthread_cond_init(& m_pthread_cond_tFinishingThreads, NULL);
    pthread_mutex_init(& m_pthread_mutex_t, NULL);
    
//    CreateAndStartThreads();
  }
  MultiThreadedTranslation::~MultiThreadedTranslation() {
    delete [] threads;
    delete [] threadStates;
    pthread_cond_destroy(& m_pthread_cond_t);
    pthread_cond_destroy(& m_pthread_cond_tFinishingThreads);
    pthread_mutex_destroy(& m_pthread_mutex_t);
  }
  
  void MultiThreadedTranslation::WaitForThreadBecomingIdle()
  {
    //Wait for the 1st thread becoming to terminate.
    // This equals the "WaitForMultipleObjects" function in Windows API.
    
//    if( threads[0].WaitForTermination() )
  }

  inline void MultiThreadedTranslation::LockCriticalSection(
    char * strMessage,
    int threadNumber/* = -1*/ )
  {
    std::ostringstream oss,oss2;
    oss << "Before locking critical section " << & m_pthread_mutex_t 
      << " for " << strMessage;
    if( threadNumber > -1 )
      oss << " for thread #" << threadNumber; 
    LOGN_DEBUG(oss.str() )
    //TODO: use pthread_mutex_trylock instead?
    //pthread_mutex_t.__data.__owner is thread ID of owner
    //pthread_mutex_t.__data.__lock is lock state
    int returnValue = pthread_mutex_lock(& m_pthread_mutex_t);
    
    oss2 << "After locking critical section " 
      << & m_pthread_mutex_t << " for " << strMessage;
    if( threadNumber > -1 )
      oss2 << " for thread #" << threadNumber;
    LOGN_DEBUG( oss2.str() )
    if( returnValue != 0 )
      returnValue++;
  }
  
  inline void MultiThreadedTranslation::UnlockCriticalSection(
    char * str, 
    int threadNumber /* = -1*/ )
  {
    LOGN_DEBUG("Before UNlocking critical section " << 
      & m_pthread_mutex_t << " for \"" << str 
      << "\" for thread #" << threadNumber )
    int returnValue = pthread_mutex_unlock(& m_pthread_mutex_t);
    LOGN_DEBUG("After UNlocking critical section " 
      << & m_pthread_mutex_t << " for \"" << str << "\" for thread #" <<
      threadNumber << " return value:" << returnValue)
    if( returnValue != 0 )
      returnValue++;
  }

  inline void LockForSignallingThisThreadFinishedWork(
    MultiThreadedTranslation * p_multiThreadedTranslation  )
  {
    p_multiThreadedTranslation->LockCriticalSection( 
      (char *) "signalling this thread finished work");
  }

  void MultiThreadedTranslation::Signal(
    const char * message, 
    pthread_cond_t * p_pthread_cond_t,
    int threadIndex)
  {
    LOGN_DEBUG("Before signalling " << message << " for thread #" << 
      threadIndex )
    //pthread_cond_signal increases the lock count (__data.__lock) of mutex by 1
    int returnValue = pthread_cond_signal(p_pthread_cond_t);
    LOGN_DEBUG("After signalling " << message << " for thread #" << threadIndex)
    if( returnValue != 0 )
      returnValue++;
  }
  
  inline void MultiThreadedTranslation::SignalThisThreadFinishedWork()
  {
    Signal("this thread finished work", & m_pthread_cond_tFinishingThreads);
  }  
  
  inline void UnlockAfterSignallingThisThreadEnds(MultiThreadedTranslation * p_multiThreadedTranslation)
  {
    LOGN_DEBUG("Before unlocking after signalling this thread (" << 
       ") ended")
    int returnValue = pthread_mutex_unlock(& p_multiThreadedTranslation->m_pthread_mutex_t);
    LOGN_DEBUG("After unlocking after signalling this thread (" << 
       ") ended")
    if( returnValue != 0 )
      returnValue++;
  }
  
  void MultiThreadedTranslation::WaitForNewJobOrThreadEndSignal(int threadIndex)
  {
    char * message = (char*) "waiting on signal for new job/thread end";
    /** https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.bpxbd00/ptcwait.htm :
     * It must be called with mutex locked by the calling thread, or undefined behavior will result. */
//    LockCriticalSection( message, threadIndex );
    WaitForSignal("new job/thread end", & m_pthread_cond_t, threadIndex);
    AtomicExchange( & threadStates[threadIndex], newJob);
    /** mutex is locked by pthread_cond_wait(...) before returning:
     *  /** https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.bpxbd00/ptcwait.htm :
     * "The pthread_cond_wait() function 
     *  releases this mutex 
     *  before suspending the thread and obtains it again before returning."
     *  May unlock now-> execute may continue because job is copied. */
    UnlockCriticalSection( message, threadIndex);
  }
  
  //TODO member variables of class "TranslateParseByRiseTree" may be overwritten
  // in parallel translation
  DWORD THREAD_FUNCTION_CALLING_CONVENTION execThreadFunction(void * p)
  {
    ParallelParams * p_parallelParams = (ParallelParams *) p;
    if( p_parallelParams )
    {
      fastestUnsignedDataType threadIndex = p_parallelParams->translThreadID;
      LOGN_DEBUG("transl. index:" << threadIndex)
      /** A thread name can only be set in the thread function under Linux (
       *  With "pthread_create(...)" there is no possibility to determine it.*/
      I_Thread::SetThreadName(
        ("parTransl#" + convertToStdString(threadIndex) ).c_str(), 
        GetCurrentThreadNumber() );
      WordCompoundsAtSameTokenIndex wordCompoundsAtSameTokenIndex;
      
      MultiThreadedTranslation * p_multiThreadedTranslation = 
        p_parallelParams->p_MultiThreadedTranslation;
      long int * threadStates = p_multiThreadedTranslation->threadStates;
      
      p_multiThreadedTranslation->LockCriticalSection( 
        (char *) "waiting on signal for new job/thread end");
      p_multiThreadedTranslation->WaitForNewJobOrThreadEndSignal(threadIndex);
      
      while( p_multiThreadedTranslation->killAllThreads == 0 )
      {
        ProcessParseTreeParams * p_processParseTreeParams = 
          (ProcessParseTreeParams *) threadStates[threadIndex];
        (p_processParseTreeParams->p_translateParseByRiseTree->*
          p_processParseTreeParams->processParseTreeFunction)(
          p_processParseTreeParams->p_GrammarPart, 
          wordCompoundsAtSameTokenIndex);

  //      p_parallelParams->p_threadAssignCritSec->Enter();
  //      p_parallelParams->p_threadAllocEvent->Broadcast();
  //      p_parallelParams->p_threadAssignCritSec->Leave();
        //TODO POSIX-specific:  
        /** This lock is needed so that EnsureAllThreadsEnded() and dispatcher 
         *  function "executeThread" that use the same lock do not miss 
         *  -setting this thread's state to idle
         *  -this/a "thread finfishes" signal event or the . */
        LockForSignallingThisThreadFinishedWork(p_multiThreadedTranslation);

        LOGN_DEBUG("freeing memory" << threadStates[threadIndex])
        /** Has been dynamically allocated in "execute(...)" */
//        memory_barrier();
        //TODO crashes here (SIGABRT) if running
        // -without being halted by breakpoints
        // -without logging
        delete (ProcessParseTreeParams *) threadStates[threadIndex];
        AtomicExchange(& threadStates[threadIndex], MultiThreadedTranslation::idle);

        AtomicExchange(& s_indexOfEndingThread, threadIndex);
        //Must use other signal objects than for "wait for new job or end signal", 
        //else worker threads waiting for a job get signals from worker 
        //threads signalising their end.
        p_multiThreadedTranslation->SignalThisThreadFinishedWork();
        if( p_multiThreadedTranslation->killAllThreads )
          p_multiThreadedTranslation->UnlockCriticalSection( "sd", threadIndex);
        else
          p_multiThreadedTranslation->WaitForNewJobOrThreadEndSignal(threadIndex);      

//        UnlockAfterSignallingThisThreadEnds(p_multiThreadedTranslation);
      }
      delete p_parallelParams;
    }
  }

  /** As thread creation costs ca. 7 ms/thread for POSIX threads 
   *  pthread_create(...) this function should be called from a separate thread */
  void MultiThreadedTranslation::CreateAndStartThreads()
  {
    for(fastestUnsignedDataType threadIndex = 0; threadIndex < m_numThreads;
        threadIndex ++ )
    {
      ParallelParams * p_parallelParams = new ParallelParams{ threadIndex, this };
      LOGN_DEBUG("Starting new thread at thread index " << threadIndex)
      int startRet = threads[threadIndex].start(execThreadFunction, p_parallelParams 
        //, ("parTransl#" + convertToStdString(index)).c_str() 
        );
      if( startRet > 0 )
        g_p_translationcontrollerbase->Message("Starting a translation thread failed:"
          + OperatingSystem::GetErrorMessageFromErrorCodeA(startRet) );
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
//    AtomicExchange( & killAllThreads, 1);
//    for( fastestUnsignedDataType index = 0; index < m_numThreads; index++)
//    {
//      //threads[index].WaitForTermination(); //TODO SIGSEV here
//      Signal("end threads")
//    }
    
    LockCriticalSection("all threads finished");
//    AtomicExchange( & killAllThreads, 1);
    fastestUnsignedDataType numRunningThreadsAvailable = 0;
    for( fastestUnsignedDataType threadIndex = 0; threadIndex < m_numThreads; 
        threadIndex++)
    {
      if( threadStates[threadIndex] != MultiThreadedTranslation::idle )
      {
        numRunningThreadsAvailable ++;
      }
    }
    while( numRunningThreadsAvailable --)
    {
      WaitForEndingThreadToSignal();
      /** Let other threads signal */
//      UnlockCriticalSection( (char *) "all threads finished");
      /** Let finishing thread gain the lock inbetween. */
//      LockCriticalSection( (char *) "all threads finished");
    }
    UnlockCriticalSection( (char *) "all threads finished");
    //TODO all changes of all threads need to be updated in main translation 
    // thread?! -> i.e. memory barrier?? 
  }
  
  void MultiThreadedTranslation::WaitForSignal(
    const char * const strMessage,
    pthread_cond_t * p_condition,
    int threadIndex)
  {    
    LOGN_DEBUG("Before waiting for condition " << & m_pthread_cond_t 
      << " for " << strMessage << " for thread #" << threadIndex)
    /** https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.bpxbd00/ptcwait.htm :
      * "The pthread_cond_wait() function 
     *  releases this mutex 
     *  before suspending the thread and obtains it again before returning." */
    int returnValue = pthread_cond_wait( p_condition, & m_pthread_mutex_t);
    /** Returns here after "pthread_cond_signal", mutex unlock  in "MultiThreadedTranslation::execute" */
    //crashed here with SIGSEV.
    LOGN_DEBUG("after waiting for condition " << & m_pthread_cond_t << " for "
      << strMessage << " for thread #" << threadIndex << " return value:" << returnValue )
    if( returnValue != 0 )
      returnValue = errno;
  }

  void MultiThreadedTranslation::WaitForEndingThreadToSignal()
  {
//        p_parallelParams->condition.wait();
//        threadAllocEvent.Wait();
//        pthread_mutex_lock( & m_pthread_mutex_t);
    WaitForSignal("finishing thread to signal", & m_pthread_cond_tFinishingThreads );    
  }
  
  inline void LockForCheckAllThreadOrWaitForThreadFinishes( 
    MultiThreadedTranslation * p_multiThreadedTranslation )
  {
    p_multiThreadedTranslation->LockCriticalSection(
      (char * )"atomic:check all threads or wait for thread finished work");
  }
  
  inline void UnlockForCheckAllThreadOrWaitForThreadFinishes( 
    MultiThreadedTranslation * p_multiThreadedTranslation )
  {
    p_multiThreadedTranslation->UnlockCriticalSection(
      (char *) "atomic:check all threads or wait for thread end");
  }
  
  void MultiThreadedTranslation::StartNewThread(
    const ProcessParseTreeParams & processParseTreeParams, 
    fastestUnsignedDataType threadIndex)
  {
    ParallelParams * p_parallelParams = new ParallelParams {
//      processParseTreeParams.processParseTreeFunction, 
//      processParseTreeParams.p_translateParseByRiseTree, 
//      processParseTreeParams.p_GrammarPart,
      threadIndex,
//            & threadAssignCritSec,
//            & threadAllocEvent
//      & m_pthread_mutex_t,
//      & m_pthread_cond_t
      this
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
  
  void MultiThreadedTranslation::AssignNewJobToThread(
    ProcessParseTreeParams * p_processParseTreeParams, 
    int threadIndex)
  {
    char * message = (char*) "send \"new job\" signal";
//    Should be already lock inside "execute"
//    LockCriticalSection( message );
//    AtomicExchange( & threadStates[threadIndex], 
//      (long int) p_processParseTreeParams);
    AtomicExchange( & newJob, (long int) p_processParseTreeParams);
    
    Signal(message, & m_pthread_cond_t);
    UnlockCriticalSection( message);
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
    ProcessParseTreeParams * p_processParseTreeParams = new 
      ProcessParseTreeParams{processParseTreeFunction, 
      p_translateParseByRiseTree, p_GrammarPart };
//    ParallelParams * p_parallelParams = NULL;
    bool noNonRunningThreadAvailable = true;
//    do
//    {
      /** Lock mutex so we can't miss a "thread ended" signal before placing
       * ourselves into a "wait for condition " state */
      LockForCheckAllThreadOrWaitForThreadFinishes(this);
      for( fastestUnsignedDataType threadIndex = 0; threadIndex < m_numThreads; 
          threadIndex++)
      {
        if( //! threads[threadIndex].IsRunning() )
          threadStates[threadIndex] == MultiThreadedTranslation::idle )
        {
//          StartNewThread(processParseTreeParams, threadIndex);
          AssignNewJobToThread(p_processParseTreeParams, threadIndex);
          noNonRunningThreadAvailable = false;
          break;
        }
      }
      if( /*! p_parallelParams*/ noNonRunningThreadAvailable )
      {
//        BusyWaiting();
        WaitForEndingThreadToSignal();
        
        /** Let other threads be able to terminate and signal in "execThreadFunction" */
        UnlockForCheckAllThreadOrWaitForThreadFinishes(this);
        
        //Creating a thread takes too long (ca. 6 ms).
//        WaitForTerminationOfEndingThread(threads[s_indexOfEndingThread]);
//        StartNewThread(processParseTreeParams, s_indexOfEndingThread);
        AssignNewJobToThread(p_processParseTreeParams, -1);
//        noNonRunningThreadAvailable = false;
      }
//      Broadcast("end all threads");
//      UnlockCriticalSection( message);

//      else 
//        UnlockForCheckAllThreadOrWaitForThreadFinishes(this);
//    }while( noNonRunningThreadAvailable);
  }
}
