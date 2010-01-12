#if !defined (ONELINEPERWORDPAIR_H_INCLUDED)
	    #define ONELINEPERWORDPAIR_H_INCLUDED

#include "Word.hpp"

namespace OneLinePerWordPair
{
  void LoadWords(  const std::string & cr_stdstrDictionaryFile
     , WordNode * pWordNodeCurrent) ;
}

#endif //ONELINEPERWORDPAIR_H_INCLUDED