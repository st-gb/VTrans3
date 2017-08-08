/*
 * EachAttributeInSingleLine.hpp
 *
 *  Created on: 03.01.2014
 *      Author: mr.sys
 */

#ifndef EACHATTRIBUTEINSINGLELINE_HPP_
#define EACHATTRIBUTEINSINGLELINE_HPP_

#include "../BinarySearchInDictData.hpp" //base class
//#include <vector> //class std::vector
//#include <string>

class VocabularyAndTranslation;

//namespace VTrans3
//{
  namespace DictionaryReader
  {
    namespace TUchemnitz
    {
    namespace EngWordSorted1st
    {
      namespace EachAttributeInSingleLine
      {
        class BinarySearchInDictData
          : public DictionaryReader::TUchemnitz::EngWordSorted1st::BinarySearchInDictData
        {
        public:
          BinarySearchInDictData(IVocabularyInMainMem &);
          virtual
          ~BinarySearchInDictData();

          void AddGermanAttributes(
            std::map<unsigned, VocabularyAndTranslation *> & voc_containerVocsCreated,
            std::vector< std::vector <std::string> > & germanVocables);
          IVocabularyInMainMem::voc_container_type * AddVocable(
            const std::vector<std::string> & englishVocableWords,
            enum TUchemnitzDictionary::wordKinds wordKind,
    //        enum EnglishWord::English_word_class word_class
            VocabularyAndTranslation *& p_vocabularyandtranslation
            );
          PositionStringVector::cmp ContainsEnglishWord(
            const PositionStringVector & psvStringToSearch,
            DWORD & r_dwTokenIndex,
            const fastestUnsignedDataType numToken,
    //        std::set<fastestUnsignedDataType> & byteOffsetsOfVocData,
            fastestUnsignedDataType & closestBeforeNonMatchOffset
            );
        };
      }
    } //EngWordSorted1st
    } //TUchemnitz
  } /* namespace DictionaryReader */
//} /* namespace VTrans3 */

#endif /* EACHATTRIBUTEINSINGLELINE_HPP_ */
