#pragma once

#include <stddef.h> //for NULL

class Word ;
//class IVocabularyInMainMem ;
//class I_WordSearch ;

class WordNode
{
public:
	Word * m_pWord;
	WordNode(){m_pWord = NULL; }
	WordNode(Word *);
  ~WordNode() ;
	WordNode * m_pWordNodePrevious;
	WordNode * m_pWordNodeNext;
};
