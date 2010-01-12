#pragma once

class Word ;
//class IVocabularyInMainMem ;
//class I_WordSearch ;

class WordNode
{
public:
	Word * m_pWord;
	WordNode(){m_pWord = NULL; }
	WordNode(Word *);
	~WordNode()
  {
    TRACE("~WordNode\n");
    if(m_pWord)
      delete m_pWord;
  };
	WordNode * m_pWordNodePrevious;
	WordNode * m_pWordNodeNext;
};
