#include "StdAfx.h"
#include "WordNode.hpp"
#include <Attributes/Word.hpp>
#include "../../rest.h" //for CStringVector
#include <Attributes/Token.h> //for GetCStringVector2(...)
#include <string>

extern WordNode * m_first ;

WordNode::WordNode(Word * pWord)
{
//	if(typeid(*pVoc)==typeid(EnglishAuxiliaryVerb))
	//	voc=new EnglishAuxiliaryVerb(*pVoc);
	m_pWord = new Word(*pWord);
}

WordNode::~WordNode()
{
  //TRACE("~WordNode\n");
  if(m_pWord)
    delete m_pWord;
};
