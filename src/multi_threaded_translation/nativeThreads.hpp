/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   nativeThreads.hpp
 * Author: sg
 *
 * Created on 1. Mai 2017, 19:00
 */

#ifndef NATIVETHREADS_HPP
#define NATIVETHREADS_HPP

#include <hardware/CPU/fastest_data_type.h> //fastestUnsignedDataType
#include <OperatingSystem/multithread/nativeThreadType.hpp>
#include <OperatingSystem/multithread/nativeCriticalSectionType.hpp>
#include <OperatingSystem/multithread/nativeEvent_type.hpp>
#include <Translate/TranslateParseByRiseTree.hpp>


namespace VTrans3 {

  class MultiThreadedTranslation;
  
  struct ParallelParams{
//    TranslateParseByRiseTree::ProcessParseTree_type processParseTreeFunction;
//    TranslateParseByRiseTree * p_translateParseByRiseTree;
//    GrammarPart * p_GrammarPart;
    fastestUnsignedDataType translThreadID;
//    nativeCriticalSection_type * p_threadAssignCritSec;
//    nativeEvent_type * p_threadAllocEvent;
//    pthread_mutex_t * p_pthread_mutex_t;
//    pthread_cond_t * p_pthread_cond_t;
    MultiThreadedTranslation * p_MultiThreadedTranslation;
  };
  struct ProcessParseTreeParams{
    TranslateParseByRiseTree::ProcessParseTree_type processParseTreeFunction;
    TranslateParseByRiseTree * p_translateParseByRiseTree;
    GrammarPart * p_GrammarPart;
  };
  
class MultiThreadedTranslation
{
  nativeThread_type * threads;
//  nativeCriticalSection_type threadAssignCritSec;
//  nativeEvent_type threadAllocEvent;
  fastestUnsignedDataType m_numThreads;
public:
  MultiThreadedTranslation();
  MultiThreadedTranslation(fastestUnsignedDataType );
  ~MultiThreadedTranslation();
  
  void AllocateAndInitializeResources();
  void DeleteAllThreadsAndThreadStates();
  void BroadcastThreadEndToAllThreads();
  void AssignNewJobToThread(ProcessParseTreeParams *, int threadIndex);
  void CreateAndStartThreads();
  void WaitForTerminationOfAllThreads();
  void WaitForThreadBecomingIdle();
  void EnsureAllThreadsFinishedJob();
  fastestUnsignedDataType GetNumberOfThreads() { return m_numThreads; }
  void execute(
    TranslateParseByRiseTree::ProcessParseTree_type, 
    TranslateParseByRiseTree *,
    GrammarPart *
    );
  inline void LockCriticalSection(
    char * str,
    int threadNumber = -1);
  inline void UnlockCriticalSection(
    char * str, 
    int threadNumber = -1);
  void StartNewThread(
    const ProcessParseTreeParams & processParseTreeParams, 
    fastestUnsignedDataType threadIndex);
  void Signal(const char * message, pthread_cond_t *, int threadIndex = -1);
  void SetNumberOfThreads(fastestUnsignedDataType num);
  void Broadcast(
    const char * message, 
    pthread_cond_t * p_pthread_cond_t,
    int threadIndex);
  void SignalThisThreadFinishedWork();
  void WaitForEndingThreadToSignal();
  inline void WaitForNewJobOrThreadEndSignal(int threadIndex);
  inline void WaitForSignal(const char * const strMessage, 
    pthread_cond_t * condition, int threadIndex = -1);
  enum ThreadState { idle = 0, running};
  //TODO exchange all uses with this typedef
  typedef long int atomic_functions_type;
  long int * threadStates;
  long int killAllThreads;
  pthread_mutex_t m_pthread_mutex_t;
  pthread_cond_t * m_pthread_cond_tNewJobOrEndAllThreads;
  pthread_cond_t m_pthread_cond_t;
  pthread_cond_t m_pthread_cond_tFinishingThreads;
  long int newJob;
};
}

#endif /* NATIVETHREADS_HPP */

