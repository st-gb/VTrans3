/** File:   newfile.h
 * Author: mr.sys
 * Created on 1. Februar 2015, 14:32   */

#ifndef CollectDictStatsThreadFuncParams_H
#define	CollectDictStatsThreadFuncParams_H

#include <map> //class std::map
#include <Attributes/EnglishWord.hpp>

/** Forward declarations */
class I_UserInterface;
class IVocabularyInMainMem;

struct CollectDictStatsThreadFuncParams
{
  I_UserInterface * pUserInterface;
  IVocabularyInMainMem * pIVocabularyInMainMem;
  /*volatile*/ std::map<enum EnglishWord::English_word_class, unsigned> 
    englishWordClass2CounterMap;
};

#endif	/* CollectDictStatsThreadFuncParams_H */

