/*
 * StdUnorderedMap.hpp
 *
 *  Created on: 22.01.2013
 *      Author: Stefan
 */

#ifndef STDUNORDEREDMAP_HPP_
#define STDUNORDEREDMAP_HPP_

#include "IVocabularyInMainMem.hpp"
#define _GLIBCXX_PROFILE
#define __GXX_EXPERIMENTAL_CXX0X__ //for std::unordered_map
#include <unordered_map> //class std::unordered_map
#include <set>

//namespace DictionaryInMainMem
//{
  class Std_Unordered_Map :
    public IVocabularyInMainMem
  {
    std::unordered_map<std::string, std::set<VocabularyAndTranslation *> > word;
  public:
    Std_Unordered_Map();
    virtual
    ~Std_Unordered_Map();
  };

//} /* namespace DictionaryInMainMem */
#endif /* STDUNORDEREDMAP_HPP_ */
