/*
 * TUchemnitzDictionary.hpp
 *
 *  Created on: 25.12.2013
 *      Author: mr.sys
 */

#ifndef TUCHEMNITZDICTIONARY_HPP_
#define TUCHEMNITZDICTIONARY_HPP_

namespace TUchemnitzDictionary
{
  enum wordKinds { not_set, adj, adv, mascNoun, femNoun, neutralNoun,
    pluralNoun, intransitiveVerb, transitiveVerb, reflexiveVerb,
    /** The case information for a verb is not contained in the TU Chemnitz 
     *  dictionary, but in a dict.cc dictionary that is converted to a format 
     *  that can be read by the DictionaryReader::TUchemnitz::EngWordSorted1st
     *  ::EachAttributeInSingleLine::BinarySearchInDictData reader */
    accusativeVerb, dativeVerb,
    vi_3singPres, vi_3singPast,
    vt_3singPres, vt_3singPast,
    vr_3singPres, vr_3singPast,
    adjPositive
  };
}

#endif /* TUCHEMNITZDICTIONARY_HPP_ */
