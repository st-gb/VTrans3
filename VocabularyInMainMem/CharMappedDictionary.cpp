/*
 * CharMappedDictionary.cpp
 *
 *  Created on: 22.01.2013
 *      Author: Stefan
 */

#include "VocabularyInMainMem/CharMappedDictionary.hpp"
#include <string.h> //memset(...)

namespace VTrans
{

  CharMappedDictionary::CharMappedDictionary()
  //Initializations .
  : m_byArrayIndexRespSize(0)
  {
    //createMapping() must be called first because it sets / determines the
    //the mapping array size.
    createMapping() ;
  }

  CharMappedDictionary::~CharMappedDictionary()
  {
    // TODO Auto-generated destructor stub
  }

  /** The sense of mapping is to allow the array of the direct child of a
  * node to be have less than 255 elements. */
  void CharMappedDictionary::createMapping()
  {
    //Initially set all array values to "255".
    ::memset(m_arbyCharValueToArrayIndex, 255, MAPPING_ARRAY_SIZE) ;
    //m_arbyCharValueToArrayIndex = new BYTE[255 ] ;
    addToCharValueToArrayIndexMapping(' ') ; //" " in "vacumm cleaner"
    addToCharValueToArrayIndexMapping('.') ; //e.g. "Allrad..."
    addToCharValueToArrayIndexMapping(',') ; //e.g.
    addToCharValueToArrayIndexMapping(':') ; //e.g. "Edition: Unpublished"
    //e.g. investor's confidence (Anlegervertrauen)
    addToCharValueToArrayIndexMapping('\'') ; // '
    addToCharValueToArrayIndexMapping('/') ; // / "water/steam"
    addToCharValueToArrayIndexMapping('\"') ; // "
    //"-" as in "A-bewertet" {adj} :: A-weighted
    addToCharValueToArrayIndexMapping('-') ; // "
    addToCharValueToArrayIndexMapping('+') ; // "as a matter of fact + do"
    addToCharValueToArrayIndexMapping('~') ; // "Carthaginian ~ Carthage"
    addToCharValueToArrayIndexMapping('&'); //"black & white triller"
    addToCharValueToArrayIndexMapping('?'); //"?" as in "What's my line?"
    addToCharValueToArrayIndexMapping('!'); //"!" as in "Duh!"
    //addToCharValueToArrayIndexMapping('(') ;
    //addToCharValueToArrayIndexMapping(')') ;
    addToCharValueToArrayIndexMapping('0','9') ; // (e.g. "4x4 drive")
    addToCharValueToArrayIndexMapping('A','Z') ;
    addToCharValueToArrayIndexMapping('a','z') ;
    //"è" as in "Apr_è_s Ski"; value from de.wikipedia.org/wiki/Codepage_850
    addToCharValueToArrayIndexMapping(0x8A) ;
    addToCharValueToArrayIndexMapping(228) ; //"ae" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(246) ; //"oe" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(252) ; //"ue" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(196) ; //"Ae" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(214) ; //"Oe" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(220) ; //"Ue" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping(223) ; //"sz" (falls wieder anderer Zeichensatz)
    addToCharValueToArrayIndexMapping('-') ; //f�r "passers-by" etc.
    //mappingToArray();
  }
} /* namespace VTrans */
