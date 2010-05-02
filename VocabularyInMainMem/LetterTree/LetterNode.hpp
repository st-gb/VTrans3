#pragma once

//#include "../IVocabularyInMainMem.hpp"

//declare forward for the LetterNode constructor.
class LetterTree ;
//class IVocabularyInMainMem ;

//!This class stands for a single character of a word if it
//!is not the root node.
class LetterNode
  //: public IVocabularyInMainMem
{
public:
  //wenn eine Vokabel mehr als einen String beinhaltet, muss/ sollte
  //angegeben werden, f�r welches dieser Strings die Knoten geh�ren.
  //Beispiel: Substantiv-Vokabel: dann muss angegeben werden, ob es
  //Singular (SINGULAR) oder Plural (PLURAL) ist.
  BYTE m_byWordType ;
  //If this node is a leaf, that means this is the last node and
  //no further node, then arpnode1LevelLower is NULL
  //and pdirdate is allocated (=not NULL).
  //A character of the name (1st the last name,
  //then the first name) is
  //described as an allocated Node of * arpnode1LevelLower.

  //!Declared as a pointer to a pointer because it should be
  //!a dynamically created(because the size may vary) array of
  //!pointers to CPersonEntryNode
  //!objects.(1 asterisk stands for dynamically created array)
  //!If a CPersonEntryNode pointer at a specific index within
  //!the m_appersonentrynode1LevelLower array is not NULL then
  //!this CPersonEntryNode pointer is allocated and thus a further
  //!character of the name follows.
  LetterNode ** m_arpletternode1LevelLower;
  //!pdirdate is NULL (set to NULL in the constructor of THIS class)
  //!if it is unused.
  //!pdirdate is used(allocated) if this node is a leaf of the tree structure
  //!OR if this node ends at a name that is a substring that
  //!begins at the first character of the last name of another name
  //!(example:"Gebauer Stefan" is a substing of "Gebauer Stefanie"),
  //!so pdirdate would be allocated at the node that stands for the
  //!'n' of "Stefan" and at the node that stands for the last "e" of
  //!"Stefanie".
  //struct DirDate * m_pdirdate;

  ////A container because there may more than 1 vocabulary with the same spelling.
  //std::map<Word> * m_pmapWord;
  //std::map<Word> * m_pmapWordTranslation;
  ////TODO Or:
  ////Saves the index within a designated container(or:
  ////for a pointer value in conjuction with a heap manager that ensures this
  ////little value range of 2^(3*8)= ~16 million bytes ) for
  ////this vocabulary as 3 byte integer (because there may be a lot of this class'
  ////nodes and this is less than for a 4 byte pointer).
  //BYTE [] arby[3] ;
  //Or:
  //std::set<VocabularyAndTranslation> * m_psetvocabularyandtranslation ;
  std::set<
    //As pointer because: for a single vocabulary often exist multiple words:
    //e.g. for the vocabulary "man": "man" and "men".
    //So for the last LetterNode of "man" and "men" the pointer to the same
    //VocabularyAndTranslation should be added.
    VocabularyAndTranslation *> * m_psetpvocabularyandtranslation ;

  std::set<VocabularyAndTranslation *> *
    m_psetpvocabularyandtranslationDeletedYet ;

  LetterNode(BYTE byArraySize, LetterTree * plt ) ;

  bool NotDeletedYet(VocabularyAndTranslation * pvoc)
  {
    //bool b = false ;
    return
      //If the pointer was NOT found within the set->not deleted yet/
      //only pointers to deleted VocabularyAndTranslation are in
      //this set.
      m_psetpvocabularyandtranslationDeletedYet->find(pvoc) ==
      m_psetpvocabularyandtranslationDeletedYet->end() ;
  }

  ~LetterNode();

  bool ContainsAtLeast1Vocable()
  {
    //return m_psetvocabularyandtranslation != NULL &&
      //m_psetvocabularyandtranslation->size() > 0 ;
    return m_psetpvocabularyandtranslation != NULL &&
      m_psetpvocabularyandtranslation->size() > 0 ;
  }

  void insert(VocabularyAndTranslation * const pvocabularyandtranslation)
  {
    //if(m_psetvocabularyandtranslation)
      //m_psetvocabularyandtranslation->insert(*pvocabularyandtranslation);
    if(m_psetpvocabularyandtranslation)
      m_psetpvocabularyandtranslation->insert(pvocabularyandtranslation);
  }
};
//Keep a newline at the end of the file to avoid a g++ warning.
