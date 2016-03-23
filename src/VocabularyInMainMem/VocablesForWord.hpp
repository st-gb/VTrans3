/*
 * VocablesForWord.hpp
 *
 *  Created on: Sep 8, 2013
 *      Author: stefan
 */

#ifndef VOCABLESFORWORD_HPP_
#define VOCABLESFORWORD_HPP_

//Forward decl.
class VocabularyAndTranslation;

/** A word may have more than 1 vocable: "love": to love, the love. This class
 *  handles/ stores these possibilities.
 * This class may be used as the value type of an object of a "std::map" class.
 * */
class VocablesForWord
{
//  VocabularyAndTranslation * m_p_vocabularyandtranslation;
public:
  typedef std::set<
    //As pointer because: for a single vocabulary often exist multiple words:
    //e.g. for the vocabulary "man": "man" and "men".
    //So for the last LetterNode of "man" and "men" the pointer to the same
    //VocabularyAndTranslation should be added.
    VocabularyAndTranslation *> voc_container_type;
//  voc_container_type * m_p_setpvocabularyandtranslation ;
  voc_container_type m_std_set_p_vocabularyandtranslation ;
  VocablesForWord()
//    : m_p_setpvocabularyandtranslation(NULL)
  {

  }
  void insert(VocabularyAndTranslation * const pvocabularyandtranslation);
};

std::ostream & operator << (std::ostream &,
  const VocablesForWord::voc_container_type &);


#endif /* VOCABLESFORWORD_HPP_ */
