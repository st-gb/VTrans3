/*
 * EachAttributeInSingleLine.hpp
 *
 *  Created on: 03.01.2014
 *      Author: mr.sys
 */

#ifndef EACHATTRIBUTEINSINGLELINE_HPP_
#define EACHATTRIBUTEINSINGLELINE_HPP_

#include "../BinarySearchInDictFile.hpp" //base class
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
        class BinarySearchInDictFile
          : public DictionaryReader::TUchemnitz::EngWordSorted1st::BinarySearchInDictFile
        {
        public:
          BinarySearchInDictFile(IVocabularyInMainMem &);
          virtual
          ~BinarySearchInDictFile();

          void AddGermanAttributes(
            std::map<unsigned, VocabularyAndTranslation *> & voc_containerVocsCreated,
            std::vector< std::vector <std::string> > & germanVocables);
          IVocabularyInMainMem::voc_container_type * AddVocable(
            const std::vector<std::string> & englishVocableWords,
            enum TUchemnitzDictionary::wordKinds wordKind,
    //        enum EnglishWord::English_word_class word_class
            VocabularyAndTranslation *& p_vocabularyandtranslation
            );
        };
      }
    } //EngWordSorted1st
    } //TUchemnitz
  } /* namespace DictionaryReader */
//} /* namespace VTrans3 */

#endif /* EACHATTRIBUTEINSINGLELINE_HPP_ */
