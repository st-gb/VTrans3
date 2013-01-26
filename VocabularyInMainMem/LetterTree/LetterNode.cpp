//Ohne das folgende include/wenn Pfad in den Eigenschaften dieser Datei 
//unter C/C++->vorkompilierte Header->"PCH durch..." falsch ist:
//"fatal error C1010: Unerwartetes Dateiende während der Suche nach dem
// vorkompilierten Header.[...]"
#include "../../StdAfx.h"
#include "LetterTree.hpp" //class LetterTree
#include <stdlib.h> //calloc(), free()

LetterNode::LetterNode(BYTE byArraySize, LetterTree * p_lettertree )
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
//      TRACE("m_arpletternode1LevelLower[%u]: %u" , i ,
//        m_arpletternode1LevelLower[i]);
//#endif
  m_psetpvocabularyandtranslationDeletedYet = & p_lettertree->
    m_setpvocabularyandtranslationDeletedYet ;
}

void LetterNode::FreeMemory()
{
}

LetterNode::~LetterNode()
{
//  FreeMemory() ;
  //Use "free" because "calloc(..)" was used for allocating memory.
  free(m_arpletternode1LevelLower) ;
  if( m_psetpvocabularyandtranslation )
  {
    VocabularyAndTranslation * p_c_vocabularyandtranslation;
    for(std::set<VocabularyAndTranslation *>::iterator 
      iter_p_vocabularyandtranslation =
      m_psetpvocabularyandtranslation->begin() ; 
      iter_p_vocabularyandtranslation !=
      m_psetpvocabularyandtranslation->end() ; 
      ++ iter_p_vocabularyandtranslation )
  #ifdef _DEBUG_FREEING_MEM
    {
  #endif
      p_c_vocabularyandtranslation = * iter_p_vocabularyandtranslation;
      //1 and the same vocandtrans must not be deleted twice!
      //(more than 1 letter node may contain the same
      //vocandtrans in its set.)
      if( NotDeletedYet( p_c_vocabularyandtranslation) )
      {
        //Memorize the immediately deleted address of vocandtrans
        //because more than 1 letter node may contain the same
        //vocandtrans in its set.
        m_psetpvocabularyandtranslationDeletedYet->insert(
          p_c_vocabularyandtranslation ) ;
        TRACE("will free vocabularyandtranslation mem at \"%x\" immediately\n",
          p_c_vocabularyandtranslation) ;
        //TODO the VocAndTransl object must not be created on the heap, else:
        //letter node 1 deletes the Voc and transl obj while letternode 2 still
        //refers to it an afterwards will try to delete the already deleted
        //VocAndTrans. obj.
        delete p_c_vocabularyandtranslation ;
      }
      else
        TRACE("~LetterNode()--alreaded deleted vocabularyandtranslation at "
          "\"%x\"\n", p_c_vocabularyandtranslation) ;
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
