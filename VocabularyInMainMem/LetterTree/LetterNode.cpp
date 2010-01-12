//Ohne das folgende include/wenn Pfad in den Eigenschaften dieser Datei 
//unter C/C++->vorkompilierte Header->"PHC durch..." falsch ist: 
//"fatal error C1010: Unerwartetes Dateiende während der Suche nach dem vorkompilierten Header.[...]"
#include "../../StdAfx.h"
#include "LetterTree.hpp"

LetterNode::LetterNode(BYTE byArraySize, LetterTree * plt )
{
  //m_psetvocabularyandtranslation = NULL ; 
  m_psetpvocabularyandtranslation = NULL ; 
  //m_arpletternode1LevelLower = new LetterNode * [
  //  VALUES_RANGE_FOR_SINGLE_CHARACTER_OF_NAME ] ;

  //Call/use calloc() because initially all elements should
  //have the value zero(0/NULL)
  m_arpletternode1LevelLower=
	  (LetterNode **)calloc(
	  //Amount of elements
	  //VALUES_RANGE_FOR_SINGLE_CHARACTER_OF_NAME,
    byArraySize,
	  //The size of each element is the size of a pointer.
	  sizeof(LetterNode *));
//#ifdef _DEBUG
//    for(int i = 0 ; i < byArraySize ; i ++ )
//      TRACE("m_arpletternode1LevelLower[%u]: %u" , i , m_arpletternode1LevelLower[i]);
//#endif
  m_psetpvocabularyandtranslationDeletedYet = & plt->
    m_setpvocabularyandtranslationDeletedYet ;
}

LetterNode::~LetterNode()
{
  //Use "free" because "calloc(..)" was used for allocating memory.
  free(m_arpletternode1LevelLower) ;
  if( m_psetpvocabularyandtranslation )
  {
    for(std::set<VocabularyAndTranslation *>::iterator 
      isetpvocabularyandtranslation = 
      m_psetpvocabularyandtranslation->begin() ; 
      isetpvocabularyandtranslation !=
      m_psetpvocabularyandtranslation->end() ; 
      ++ isetpvocabularyandtranslation )
#ifdef _DEBUG_FREEING_MEM
    {
#endif
      //1 and the same vocandtrans must not be deleted twice!
      //(more than 1 letter node may contain the same
      //vocandtrans in its set.)
      if(NotDeletedYet(*isetpvocabularyandtranslation) )
      {
        //Memorize the immedialtely deleted address of vocandtrans 
        //because more than 1 letter node may contain the same
        //vocandtrans in its set.
        m_psetpvocabularyandtranslationDeletedYet->insert(
          *isetpvocabularyandtranslation ) ;
        TRACE("will free vocabularyandtranslation mem at \"%x\" immediately\n",
          *isetpvocabularyandtranslation) ;
        delete *isetpvocabularyandtranslation ;
      }
      else
        TRACE("~LetterNode()--alreaded deleted vocabularyandtranslation at \"%x\"\n",
          *isetpvocabularyandtranslation) ;
#ifdef _DEBUG_FREEING_MEM
    }
#endif
    //m_psetpvocabularyandtranslation->clear() ;
    delete m_psetpvocabularyandtranslation ;
  }
#ifdef _DEBUG_FREEING_MEM
  else
    TRACE("~LetterNode()--no voc std::set\n") ;
#endif
}
