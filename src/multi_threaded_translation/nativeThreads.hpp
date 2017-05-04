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
  
  struct ParallelParams{
    TranslateParseByRiseTree::ProcessParseTree_type processParseTreeFunction;
    TranslateParseByRiseTree * p_translateParseByRiseTree;
    GrammarPart * p_GrammarPart;
    fastestUnsignedDataType translThreadID;
//    nativeCriticalSection_type * p_threadAssignCritSec;
//    nativeEvent_type * p_threadAllocEvent;
    pthread_mutex_t * p_pthread_mutex_t;
    pthread_cond_t * p_pthread_cond_t;
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
  pthread_mutex_t m_pthread_mutex_t;
  pthread_cond_t m_pthread_cond_t;
  fastestUnsignedDataType m_numThreads;
public:
  MultiThreadedTranslation(fastestUnsignedDataType );
  ~MultiThreadedTranslation();
  void WaitForThreadBecomingIdle();
  void EnsureAllThreadsEnded();
  void execute(
    TranslateParseByRiseTree::ProcessParseTree_type, 
    TranslateParseByRiseTree *,
    GrammarPart *
    );
  void StartNewThread(
    const ProcessParseTreeParams & processParseTreeParams, 
    fastestUnsignedDataType threadIndex);
  void WaitForEndingThreadToSignal();
};
}

#endif /* NATIVETHREADS_HPP */

