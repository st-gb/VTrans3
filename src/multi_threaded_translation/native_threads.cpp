#include "nativeThreads.hpp"
#include "Controller/TranslationControllerBase.hpp"
#include "hardware/CPU/atomic/atomicIncrement.h"
#include <unistd.h> //usleep(...)
#include <Controller/character_string/convertFromAndToStdString.hpp>
#include <OperatingSystem/GetErrorMessageFromLastErrorCode.hpp>
#include <hardware/CPU/atomic/AtomicExchange.h>
#include <hardware/CPU/atomic/memory_barrier.h>
#include <Controller/time/GetTickCount.hpp>

extern TranslationControllerBase * g_p_translationcontrollerbase;
static long int s_indexOfEndingThread;

namespace VTrans3 {
  
  MultiThreadedTranslation::MultiThreadedTranslation()
    : m_numThreads(0)
    , killAllThreads(0)
    , m_jobNumber(0)
  {
    
  }
  
  MultiThreadedTranslation::MultiThreadedTranslation(fastestUnsignedDataType number)
    : m_numThreads(number)
    , killAllThreads(0)
    , m_jobNumber(0)
  {
    if( m_numThreads )
    {
      AllocateAndInitializeResources();
//    CreateAndStartThreads();
    }
  }

  void MultiThreadedTranslation::AllocateAndInitializeResources()
  {
    m_numThreadsExecJobs = 0;
    pthread_cond_init(& m_pthread_cond_t, NULL);
    pthread_cond_init(& m_pthread_cond_tFinishingThreads, NULL);
    pthread_mutex_init(& m_pthread_mutex_t, NULL);
      
    threads = new nativeThread_type[m_numThreads];
    threadStates = new long int[m_numThreads];
    m_threadTimes = new long double[m_numThreads];
    memset( threadStates, idle, sizeof(long int) * m_numThreads );
    
    m_pthread_cond_tNewJobOrEndAllThreads = new pthread_cond_t[m_numThreads];
    for(int i = 0; i < m_numThreads; i++)
      pthread_cond_init(& m_pthread_cond_tNewJobOrEndAllThreads[i], NULL);
    killAllThreads = 0;
  }
  
  void MultiThreadedTranslation::WaitForTerminationOfAllThreads()
  {
    LOGN_INFO("begin")
    for(fastestUnsignedDataType threadIndex = 0; threadIndex < m_numThreads ;
         threadIndex ++)
    {
      LOGN_INFO("waiting for termination of thread # " << threadIndex)
      threads[threadIndex].WaitForTermination();
    }
    LOGN_INFO("end")
  }
  
  void MultiThreadedTranslation::DeleteAllThreadsAndThreadStates()
  {
    if( m_numThreads )
    {
      EnsureAllThreadsFinishedJob();
      BroadcastThreadEndToAllThreads();
      WaitForTerminationOfAllThreads();
      
      pthread_cond_destroy(& m_pthread_cond_t);
      pthread_cond_destroy(& m_pthread_cond_tFinishingThreads);
      pthread_mutex_destroy(& m_pthread_mutex_t);
      
      delete [] threads;
      //TODO use atomic exchange function here?
      m_allTranslationThreadsCreated = false;
      delete [] m_threadTimes;
      delete [] threadStates;
      for(int i = 0; i < m_numThreads; i++)
        pthread_cond_destroy(& m_pthread_cond_tNewJobOrEndAllThreads[i]);
      delete [] m_pthread_cond_tNewJobOrEndAllThreads;
    }
  }
  
  MultiThreadedTranslation::~MultiThreadedTranslation() {
    if( m_numThreads )
    {
      DeleteAllThreadsAndThreadStates();
    }
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
      << ",owner:" << m_pthread_mutex_t.__data.__owner << " for " << strMessage;
    if( threadNumber > -1 )
      oss << " for thread #" << threadNumber; 
    LOGN_INFO(oss.str() )
    //TODO: use pthread_mutex_trylock instead?
    //pthread_mutex_t.__data.__owner is thread ID of owner
    //pthread_mutex_t.__data.__lock is lock state
    int returnValue = pthread_mutex_lock(& m_pthread_mutex_t);
    
    oss2 << "After locking critical section " << & m_pthread_mutex_t <<
      ",owner:" << m_pthread_mutex_t.__data.__owner << " for " << strMessage;
    if( threadNumber > -1 )
      oss2 << " for thread #" << threadNumber;
    LOGN_INFO( oss2.str() )
    if( returnValue != 0 )
      returnValue++;
  }
  
  inline void MultiThreadedTranslation::UnlockCriticalSection(
    char * str, 
    int threadNumber /* = -1*/ )
  {
    LOGN_INFO("Before UNlocking critical section " <<
      & m_pthread_mutex_t << " for \"" << str 
      << "\" for thread #" << threadNumber )
    int returnValue = pthread_mutex_unlock(& m_pthread_mutex_t);
    LOGN_INFO("After UNlocking critical section "
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

  void MultiThreadedTranslation::Broadcast(
    const char * message, 
    pthread_cond_t * p_pthread_cond_t,
    int threadIndex)
  {
    LOGN_DEBUG("Before broadcasting " << message << " for thread #" << 
      threadIndex )
    //pthread_cond_signal increases the lock count (__data.__lock) of mutex by 1
    int returnValue = pthread_cond_broadcast(p_pthread_cond_t);
    LOGN_DEBUG("After broadcasting " << message << " for thread #" << threadIndex)
    if( returnValue != 0 )
      returnValue++;
  }
  
  void MultiThreadedTranslation::SetNumberOfThreads(fastestUnsignedDataType num)
  {
    m_asyncThreadStarterThread.WaitForTermination();
    DeleteAllThreadsAndThreadStates();
    m_numThreads = num;
    AllocateAndInitializeResources();
//    CreateAndStartThreads();
  }
  
  void MultiThreadedTranslation::Signal(
    const char * message, 
    pthread_cond_t * p_pthread_cond_t,
    int threadIndex)
  {
    LOGN_INFO("Before signalling " << message << " for thread #" <<
      threadIndex << " for condition " << p_pthread_cond_t << ",# waiters:"
      << p_pthread_cond_t->__data.__nwaiters)
    /** Ensure the other thread is waiting, else it doesn't get the signal! */
    if( threadIndex > -1 && ! p_pthread_cond_t->__data.__nwaiters)
    {
      LOGN_WARNING("no thread is waiting for the signal")
      while(! p_pthread_cond_t->__data.__nwaiters)
        /** memory fence to update "__nwaiters" to from the other thread when
        * put in waiting mode : necessary at all? */
        __sync_synchronize();
//      usleep(1000000);
    }
    //pthread_cond_signal increases the lock count (__data.__lock) of mutex by 1
    int returnValue = pthread_cond_signal(p_pthread_cond_t);
    LOGN_INFO("After signalling " << message << " for thread #" << threadIndex
      << " return value:" << returnValue)
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
  
  fastestUnsignedDataType MultiThreadedTranslation::WaitForNewJobOrThreadEndSignal(
    const int threadIndex)
  {
    char * message = (char*) "waiting on signal for new job/thread end";
    /** https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.bpxbd00/ptcwait.htm :
     * It must be called with mutex locked by the calling thread, or undefined behavior will result. */
//    LockCriticalSection( message, threadIndex );
//    WaitForSignal("new job/thread end", & m_pthread_cond_t, threadIndex);
    WaitForSignal("new job/thread end", & m_pthread_cond_tNewJobOrEndAllThreads[threadIndex], threadIndex);
    fastestUnsignedDataType jobNumber;
    if( ! killAllThreads )
    {
//      LOGN_INFO("pointer to New Job:" << (void *) newJob )
      ProcessParseTreeParams * p_processParseTreeParams = 
        (ProcessParseTreeParams *) /*newJob*/ threadStates[threadIndex];
      LOGN_INFO("New Job:" <<  (void *) p_processParseTreeParams
        << " grammar part:" << p_processParseTreeParams->p_GrammarPart << " "
        << p_processParseTreeParams->p_translateParseByRiseTree << " "
        << "function pointer:" << (void *) p_processParseTreeParams->processParseTreeFunction )
      jobNumber = p_processParseTreeParams->jobNumber;
  //    OperatingSystem::GetTimeCountInSeconds(m_threadTimes[threadIndex]);
      OperatingSystem::GetTimeCountInSeconds(m_jobNumber2time[jobNumber]);
    }
//    assert()
    //TODO is the new thread state visible for the dispatcher thread?
    //->TU Berlin, "parallel programming" , lecture 10
    // http://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g
//    AtomicExchange( & threadStates[threadIndex], newJob);
    
//     InterlockedExchangePointer();
    
    /** mutex is locked by pthread_cond_wait(...) before returning:
     *  /** https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.bpxbd00/ptcwait.htm :
     * "The pthread_cond_wait() function 
     *  releases this mutex 
     *  before suspending the thread and obtains it again before returning."
     *  May unlock now-> execute may continue because job is copied. */
    UnlockCriticalSection( message, threadIndex);
    return jobNumber;
  }
  
  //TODO member variables of class "TranslateParseByRiseTree" may be overwritten
  // in parallel translation
  //TODO runtime error:
//  *** Error in `src/VTrans_console': corrupted double-linked list: 0xb6900918 ***
//  Abgebrochen (Speicherabzug geschrieben)

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
      fastestUnsignedDataType jobNumber = p_multiThreadedTranslation->
        WaitForNewJobOrThreadEndSignal(threadIndex);
      
      while( p_multiThreadedTranslation->killAllThreads == 0 )
      {
        ProcessParseTreeParams * p_processParseTreeParams = 
          (ProcessParseTreeParams *) threadStates[threadIndex];
        //TODO assertion failed once(in version with 1 new job signal for all threads?!)
//        assert( p_processParseTreeParams->processParseTreeFunction == 
//          & TranslateParseByRiseTree::DummyTranslateParseTree ); 
        LOGN_INFO("before executing worker function")
        //TODO crashed here (in version with 1 new job signal for all threads?!): 
        // p_processParseTreeParams was 0?!: 
        //  "Access not within mapped region at address 0x0"
        (p_processParseTreeParams->p_translateParseByRiseTree->*
          p_processParseTreeParams->processParseTreeFunction)(
          p_processParseTreeParams->p_GrammarPart, 
          wordCompoundsAtSameTokenIndex);
        LOGN_INFO("after executing worker function")

  //      p_parallelParams->p_threadAssignCritSec->Enter();
  //      p_parallelParams->p_threadAllocEvent->Broadcast();
  //      p_parallelParams->p_threadAssignCritSec->Leave();
        //TODO POSIX-specific:  
        /** This lock is needed so that EnsureAllThreadsEnded() and dispatcher 
         *  function "executeThread" that use the same lock do not miss 
         *  -setting this thread's state to idle
         *  -this/a "thread finfishes" signal event or the . */
        LockForSignallingThisThreadFinishedWork(p_multiThreadedTranslation);
        //TODO statistic times here 
        long double d;
        OperatingSystem::GetTimeCountInSeconds(d);
        p_multiThreadedTranslation->m_threadTimes[threadIndex] = 
          d - p_multiThreadedTranslation->m_threadTimes[threadIndex];
        
        p_multiThreadedTranslation->m_jobNumber2time[jobNumber] =
          d - p_multiThreadedTranslation->m_jobNumber2time[jobNumber];
//          0.06d;

        LOGN_INFO("freeing memory for address:" << (void *) p_processParseTreeParams)
//        std::cout << "freeing memory" << threadStates[threadIndex] << std::endl;
        /** Has been dynamically allocated in "execute(...)" */
//        memory_barrier();
        //TODO crashes here (in version with 1 new job signal for all threads?!)
        // (SIGABRT) if running
        // -without being halted by breakpoints
        // -without logging
        delete p_processParseTreeParams;
        AtomicExchange(& threadStates[threadIndex], MultiThreadedTranslation::idle);
        p_multiThreadedTranslation->m_numThreadsExecJobs --;
        LOGN_INFO("after decrementing m_numThreadsExecJobs:" <<
          p_multiThreadedTranslation->m_numThreadsExecJobs)
//        AtomicExchange( & p_multiThreadedTranslation->m_numThreadsExecJobs,
//          p_multiThreadedTranslation->m_numThreadsExecJobs - 1);

        AtomicExchange(& s_indexOfEndingThread, threadIndex);
        /** Must use other signal objects than for "wait for new job or end signal",
        *  else worker threads waiting for a job get signals from worker
        *  threads signalising their end. */
        p_multiThreadedTranslation->SignalThisThreadFinishedWork();
        //This happened (worker thread got signalled before placing itself into
        // wait mode)
        // -> does "pthread_cond_signal" unlock a mutex???
        // because LockCriticalSection in dispatcher thread succeeds after 
        //  signalling
//        2017.06.17&nbsp;00:44:38s564ms INFO [parTransl#0-13336] VTrans3::MultiThreadedTranslation Signal After signalling this thread finished work for thread #-1
//        2017.06.17&nbsp;00:44:38s564ms INFO [Thread-13334] VTrans3::MultiThreadedTranslation LockCriticalSection After locking critical section 0xbfdc25f8 for atomic:check all threads or wait for thread finished work
//        2017.06.17&nbsp;00:44:38s564ms INFO [Thread-13334] VTrans3::MultiThreadedTranslation AssignNewJobToThread New Job:0x8c148a8 grammar part:0x8c19fb8 0xbfdc298c function pointer:0x80f7430thread index0
//        2017.06.17&nbsp;00:44:38s564ms INFO [Thread-13334] VTrans3::MultiThreadedTranslation Signal Before signalling send "new job" signal for thread #-1 for condition 0x8bf2098
//        2017.06.17&nbsp;00:44:38s564ms INFO [Thread-13334] VTrans3::MultiThreadedTranslation Signal After signalling send "new job" signal for thread #-1
//        2017.06.17&nbsp;00:44:38s565ms INFO [parTransl#1-13337] VTrans3 execThreadFunction before executing worker function
//        2017.06.17&nbsp;00:44:38s565ms INFO [parTransl#0-13336] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0x8bf2098 for new job/thread end for thread #0        p_multiThreadedTranslation->SignalThisThreadFinishedWork();

        if( p_multiThreadedTranslation->killAllThreads )
          p_multiThreadedTranslation->UnlockCriticalSection( (char *) "end execThreadFunction", threadIndex);
        else
          jobNumber = p_multiThreadedTranslation->WaitForNewJobOrThreadEndSignal(threadIndex);      

//        UnlockAfterSignallingThisThreadEnds(p_multiThreadedTranslation);
      }
      delete p_parallelParams;
    }
  }

  /** Idea: creating threads costs ca. 7 ms/thread. So create them in another
   *  thread in parallel. Therefore use this function start routine. */
  DWORD AsyncStartThreadsThreadProc(void * p_v)
  {
    MultiThreadedTranslation * p_multiThreadedTranslation =
      (MultiThreadedTranslation *) p_v;
    if( p_multiThreadedTranslation)
    {
      p_multiThreadedTranslation->CreateAndStartThreads();
    }
  }

  void MultiThreadedTranslation::StartThreadsAsync()
  {
    //TODO must be finished before using the threads in the translation
    // this can be ensured via m_asyncThreadStarterThread.WaitForTermination();
    m_asyncThreadStarterThread.start(AsyncStartThreadsThreadProc, this);
  }
  /** As thread creation costs ca. 7 ms/thread for POSIX threads 
   *  pthread_create(...) this function should be called from a separate thread */
  void MultiThreadedTranslation::CreateAndStartThreads()
  {
    LOGN_INFO("begin")
//#define SET_AFF_MASK
#ifdef SET_AFF_MASK
    cpu_set_t cpuset;
    int numCPUcores = 8;
#endif //#ifdef SET_AFF_MASK
    for(fastestUnsignedDataType threadIndex = 0; threadIndex < m_numThreads;
        threadIndex ++ )
    {
#ifdef SET_AFF_MASK
      CPU_ZERO(&cpuset);
//      CPU_SET( numCPUcores / (threadIndex + 1) - 1, &cpuset);
      CPU_SET( threadIndex, &cpuset);
#endif //#ifdef SET_AFF_MASK
      
      ParallelParams * p_parallelParams = new ParallelParams{ threadIndex, this };
      LOGN_DEBUG("Starting new thread at thread index " << threadIndex)
      int startRet = threads[threadIndex].start(execThreadFunction, p_parallelParams 
        //, ("parTransl#" + convertToStdString(index)).c_str() 
        );
#ifdef SET_AFF_MASK
      pthread_t threadHandle = * (pthread_t *) threads[threadIndex].GetThreadHandle();
//      if( m_numThreads < numCPUcores )
      // http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
//      int s = pthread_setaffinity_np( threadHandle, sizeof(cpu_set_t), &cpuset);
//      if (s != 0)
//        LOGN_ERROR("CPUsez");
#endif //#ifdef SET_AFF_MASK
      if( startRet > 0 )
        g_p_translationcontrollerbase->Message("Starting a translation thread failed:"
          + OperatingSystem::GetErrorMessageFromErrorCodeA(startRet) );
    }
    AtomicExchange( (long int *) & m_allTranslationThreadsCreated, true);
    LOGN_INFO("end")
  }
  
  
//  void MultiThreadedTranslation::CreateThreads(fastestUnsignedDataType number)
//  {
//    
////    for()
//    {
//      nativeThread_type;
//    }
//  }
  
  void MultiThreadedTranslation::BroadcastThreadEndToAllThreads()
  {
    char * message = "kill all threads";
    AtomicExchange( & killAllThreads, 1);
    LockCriticalSection(message);
//    Broadcast(message, & m_pthread_cond_t, -1);
    for(int i = 0; i < m_numThreads; i++)
      Signal(message, & m_pthread_cond_tNewJobOrEndAllThreads[i]);

    UnlockCriticalSection(message);
  }
  
  /** Waits for all threads to terminate. This function may be executed at the
   *  latest possible point (before the data structures that are used by the 
   *  threads are destroyed. */
  void MultiThreadedTranslation::EnsureAllThreadsFinishedJob()
  {
//    AtomicExchange( & killAllThreads, 1);
//    for( fastestUnsignedDataType index = 0; index < m_numThreads; index++)
//    {
//      //threads[index].WaitForTermination(); //TODO SIGSEV here
//      Signal("end threads")
//    }
    LOGN_INFO("begin")
    LockCriticalSection( (char *) "all threads finished");
//    AtomicExchange( & killAllThreads, 1);
    fastestUnsignedDataType numRunningThreadsAvailable = 0;
//    for( fastestUnsignedDataType threadIndex = 0; threadIndex < m_numThreads; 
//        threadIndex++)
//    {
//      if( threadStates[threadIndex] != MultiThreadedTranslation::idle )
//      {
//        numRunningThreadsAvailable ++;
//      }
//    }
//    if( m_numThreadsExecJobs )
    
    /* NOTE: any other thread waiting to aquire the lock may get it BEFORE
     *  pthread_cond_wait() returns after signalling! 
     * So pthread_cond_wait may miss a signal if 2 threads are signalling,
     *  and the 2nd signalling thread aquires the lock BEFORE pthread_cond_wait() 
     *  reaquires it! 
     *  So we have to ask m_numThreadsExecJobs var whose access is guarded by 
     *  locks. */
    while( /*numRunningThreadsAvailable --*/ m_numThreadsExecJobs )
    {
//      LOGN_INFO("numRunningThreadsAvailable:" << numRunningThreadsAvailable + 1)
      LOGN_INFO("# threads executing jobs:" << m_numThreadsExecJobs)
      //TODO sometimes was in wait state after the last worker thread signalled his
      // end-> did not receive the signal (->non-deterministic/race condition?)
      // this may be due to this fact that pthread_cond_wait(...) :
      // -releases the mutex before waiting before putting itself into wait mode
      // -then the worker threads may get the lock and 
      /* Log1:
  2017-05-14 13:04:41,777 INFO [Thread-16187] VTrans3::MultiThreadedTranslation EnsureAllThreadsFinishedJob begin
  2017-05-14 13:04:41,777 INFO [Thread-16187] VTrans3::MultiThreadedTranslation EnsureAllThreadsFinishedJob numRunningThreadsAvailable:2
  2017-05-14 13:04:41,777 INFO [Thread-16187] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0xbfe7aa70 for finishing thread to signal for thread #-1
  [...]
  2017-05-14 13:04:41,800 INFO [parTransl#0-16188] VTrans3 execThreadFunction after executing worker function
  2017-05-14 13:04:41,800 INFO [parTransl#0-16188] VTrans3 execThreadFunction freeing memory for address:0x9d68a50
  2017-05-14 13:04:41,800 INFO [parTransl#0-16188] VTrans3::MultiThreadedTranslation Signal Before signalling this thread finished work for thread #-1
  2017-05-14 13:04:41,800 INFO [parTransl#1-16189] VTrans3 execThreadFunction after executing worker function
  2017-05-14 13:04:41,800 INFO [parTransl#0-16188] VTrans3::MultiThreadedTranslation Signal After signalling this thread finished work for thread #-1
  2017-05-14 13:04:41,800 INFO [parTransl#0-16188] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0x9d5f220 for new job/thread end for thread #0
  2017-05-14 13:04:41,800 INFO [parTransl#1-16189] VTrans3 execThreadFunction freeing memory for address:0x9d67cc0
  2017-05-14 13:04:41,800 INFO [parTransl#1-16189] VTrans3::MultiThreadedTranslation Signal Before signalling this thread finished work for thread #-1
  2017-05-14 13:04:41,800 INFO [parTransl#1-16189] VTrans3::MultiThreadedTranslation Signal After signalling this thread finished work for thread #-1
  2017-05-14 13:04:41,800 INFO [parTransl#1-16189] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0x9d5f250 for new job/thread end for thread #1
  2017-05-14 13:04:41,800 INFO [Thread-16187] VTrans3::MultiThreadedTranslation WaitForSignal after waiting for condition 0xbfe7aa70 for finishing thread to signal for thread #-1 return value:0
  2017-05-14 13:04:41,800 INFO [Thread-16187] VTrans3::MultiThreadedTranslation EnsureAllThreadsFinishedJob numRunningThreadsAvailable:1
  2017-05-14 13:04:41,800 INFO [Thread-16187] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0xbfe7aa70 for finishing thread to signal for thread #-1       *  
       * 
       * log:
2017-05-14 13:14:18,713 INFO [Thread-16649] VTrans3::MultiThreadedTranslation EnsureAllThreadsFinishedJob begin
2017-05-14 13:14:18,713 INFO [Thread-16649] VTrans3::MultiThreadedTranslation LockCriticalSection Before locking critical section 0xbfa75e44 for all threads finished
2017-05-14 13:14:18,713 INFO [Thread-16649] VTrans3::MultiThreadedTranslation LockCriticalSection After locking critical section 0xbfa75e44 for all threads finished
2017-05-14 13:14:18,713 INFO [Thread-16649] VTrans3::MultiThreadedTranslation EnsureAllThreadsFinishedJob numRunningThreadsAvailable:2
2017-05-14 13:14:18,713 INFO [Thread-16649] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0xbfa75e90 for finishing thread to signal for thread #-1
       * [...]
2017-05-14 13:14:18,734 INFO [parTransl#1-16651] VTrans3::MultiThreadedTranslation LockCriticalSection Before locking critical section 0xbfa75e44 for signalling this thread finished work
       * [...]
2017-05-14 13:14:18,734 INFO [parTransl#0-16650] VTrans3::MultiThreadedTranslation LockCriticalSection Before locking critical section 0xbfa75e44 for signalling this thread finished work
2017-05-14 13:14:18,734 INFO [parTransl#1-16651] VTrans3::MultiThreadedTranslation LockCriticalSection After locking critical section 0xbfa75e44 for signalling this thread finished work
[...]
2017-05-14 13:14:18,734 INFO [parTransl#1-16651] VTrans3::MultiThreadedTranslation Signal Before signalling this thread finished work for thread #-1
2017-05-14 13:14:18,734 INFO [parTransl#1-16651] VTrans3::MultiThreadedTranslation Signal After signalling this thread finished work for thread #-1
2017-05-14 13:14:18,734 INFO [parTransl#1-16651] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0x9d4b250 for new job/thread end for thread #1
2017-05-14 13:14:18,734 INFO [parTransl#0-16650] VTrans3::MultiThreadedTranslation LockCriticalSection After locking critical section 0xbfa75e44 for signalling this thread finished work
[...]
2017-05-14 13:14:18,734 INFO [parTransl#0-16650] VTrans3::MultiThreadedTranslation Signal Before signalling this thread finished work for thread #-1
2017-05-14 13:14:18,735 INFO [parTransl#0-16650] VTrans3::MultiThreadedTranslation Signal After signalling this thread finished work for thread #-1
2017-05-14 13:14:18,735 INFO [parTransl#0-16650] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0x9d4b220 for new job/thread end for thread #0
2017-05-14 13:14:18,735 INFO [Thread-16649] VTrans3::MultiThreadedTranslation WaitForSignal after waiting for condition 0xbfa75e90 for finishing thread to signal for thread #-1 return value:0
2017-05-14 13:14:18,735 INFO [Thread-16649] VTrans3::MultiThreadedTranslation EnsureAllThreadsFinishedJob numRunningThreadsAvailable:1
2017-05-14 13:14:18,735 INFO [Thread-16649] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0xbfa75e90 for finishing thread to signal for thread #-1   
       *  
       * another log:
        2017-05-14 12:53:21,868 INFO [Thread-16059] VTrans3::MultiThreadedTranslation EnsureAllThreadsFinishedJob numRunningThreadsAvailable:2
        2017-05-14 12:53:21,868 INFO [Thread-16059] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0xbfa62bb0 for finishing thread to signal for thread #-1
       * [...]
        2017-05-14 12:53:21,874 INFO [parTransl#6-16066] VTrans3::MultiThreadedTranslation Signal Before signalling this thread finished work for thread #-1
        2017-05-14 12:53:21,874 INFO [parTransl#6-16066] VTrans3::MultiThreadedTranslation Signal After signalling this thread finished work for thread #-1
        2017-05-14 12:53:21,874 INFO [parTransl#6-16066] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0x8a4c3e8 for new job/thread end for thread #6
        2017-05-14 12:53:21,874 INFO [Thread-16059] VTrans3::MultiThreadedTranslation WaitForSignal after waiting for condition 0xbfa62bb0 for finishing thread to signal for thread #-1 return value:0
        2017-05-14 12:53:21,874 INFO [Thread-16059] VTrans3::MultiThreadedTranslation EnsureAllThreadsFinishedJob numRunningThreadsAvailable:1
        2017-05-14 12:53:21,875 INFO [Thread-16059] VTrans3::MultiThreadedTranslation WaitForSignal Before waiting for condition 0xbfa62bb0 for finishing thread to signal for thread #-1 */
              
      
      WaitForEndingThreadToSignal();
      /** Let other threads signal */
      UnlockCriticalSection( (char *) "all threads finished");
      /** Let finishing thread gain the lock inbetween. */
      LockCriticalSection( (char *) "all threads finished");
    }
    UnlockCriticalSection( (char *) "all threads finished");
    //TODO all changes of all threads need to be updated in main translation 
    // thread?! -> i.e. memory barrier?? 
     LOGN_INFO("end")
 }
  
  void MultiThreadedTranslation::WaitForSignal(
    const char * const strMessage,
    pthread_cond_t * p_condition,
    int threadIndex)
  {
    pthread_mutex_t * p_pthread_mutex_t = & m_pthread_mutex_t;
    LOGN_INFO("Before waiting for condition " << p_condition 
      << " for " << strMessage << //" for thread #" << threadIndex
      ",mutex:" << p_pthread_mutex_t << " mutex owner:"
      << p_pthread_mutex_t->__data.__owner << "(releases the mutex)")
    /** https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.bpxbd00/ptcwait.htm :
      * "The pthread_cond_wait() function 
     *  releases this mutex 
     *  before suspending the thread and obtains it again before returning." 
     * WARNING : any other thread waiting to aquire the lock may get it BEFORE
     *  pthread_cond_wait() returns after signalling!
     */
    int returnValue = pthread_cond_wait( p_condition, p_pthread_mutex_t);
    /** Returns here after "pthread_cond_signal" & mutex unlock  in "MultiThreadedTranslation::execute" */
    //crashed here with SIGSEV.
    LOGN_INFO("after waiting for condition " << p_condition << " for "
      << strMessage << //" for thread #" << threadIndex <<
      ",mutex:" << p_pthread_mutex_t << " return value:" << returnValue
      << "(mutex is locked)")
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
//    AtomicExchange( & newJob, (long int) p_processParseTreeParams);
    AtomicExchange( & threadStates[threadIndex], 
      (long int) p_processParseTreeParams);
    
//    TranslateParseByRiseTree & translateParseByRiseTree = * p_processParseTreeParams->p_translateParseByRiseTree;
    TranslateParseByRiseTree::ProcessParseTree_type processParseTreeFunction =
      p_processParseTreeParams->processParseTreeFunction;
    LOGN_INFO("New Job:" << (void *) p_processParseTreeParams 
      << " grammar part:" << p_processParseTreeParams->p_GrammarPart << " "
      << p_processParseTreeParams->p_translateParseByRiseTree
      //http://stackoverflow.com/questions/2402579/function-pointer-to-member-function
      << " function pointer:" << (void *) processParseTreeFunction
      << " thread index" << threadIndex)
//    Signal(message, & m_pthread_cond_t);
    Signal(message, & m_pthread_cond_tNewJobOrEndAllThreads[threadIndex],
       threadIndex);
    
    m_numThreadsExecJobs ++;
    LOGN_INFO("m_numThreadsExecJobs:" << m_numThreadsExecJobs)
    m_jobNumber++;
//    atomicIncrement( (long *) & m_jobNumber);
    
    //TODO when run with helgrind and 1 condition variable _for all threads_?: 
    //  EnsureAllThreadsFinishedJob() from 
    //  "TranslateParseByRiseTree::ProcessParseTree" followed after unlocking
    //  before any WaitForNewJobOrThreadEndSignal -> SIGSEGV
    //  4 jobs were signal, but only 1 got
    //TDDO when run with helgrind and 1 condition variable _for all threads_?: sent 3 jobs before the 1st 
    //  "WaitForNewJobOrThreadEndSignal" printed out the job-> 2 jobs got lost,
    //  3 threads got the same (the 3rd) job assigned
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
      p_translateParseByRiseTree, p_GrammarPart, m_jobNumber };
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
        AssignNewJobToThread(p_processParseTreeParams, s_indexOfEndingThread);
//        noNonRunningThreadAvailable = false;
      }
//      Broadcast("end all threads");
//      UnlockCriticalSection( message);

//      else 
//        UnlockForCheckAllThreadOrWaitForThreadFinishes(this);
//    }while( noNonRunningThreadAvailable);
  }
}
