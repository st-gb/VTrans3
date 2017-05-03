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
#include <Translate/TranslateParseByRiseTree.hpp>

namespace VTrans3 {
  
  struct ParallelParams{
    TranslateParseByRiseTree::ProcessParseTree_type processParseTreeFunction;
    TranslateParseByRiseTree * p_translateParseByRiseTree;
    GrammarPart * p_GrammarPart;
    fastestUnsignedDataType translThreadID;
  };
  
class MultiThreadedTranslation
{
  nativeThread_type * threads;
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
};
}

#endif /* NATIVETHREADS_HPP */

