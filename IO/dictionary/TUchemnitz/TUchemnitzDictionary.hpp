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
    //TODO rename "vi" to "intransitiveVerb"
    //     rename "vt" to "transitiveVerb"
    //     rename "vr" to "reflexiveVerb"
    vi_3singPres, vi_3singPast,
    vt_3singPres, vt_3singPast,
    vr_3singPres, vr_3singPast,
    adjPositive
  };
}

#endif /* TUCHEMNITZDICTIONARY_HPP_ */
