/*
 * KeepTrackOfCurrentParseTreePath.cpp
 *
 *  Created on: 13.12.2012
 *      Author: Stefan
 */

#include "Parse/KeepTrackOfCurrentParseTreePath.hpp"

namespace ParseTreeTraverser
{

  KeepTrackOfCurrentParseTreePath::KeepTrackOfCurrentParseTreePath(
    const GrammarPart * p_grammarpartStartNode
    , ParseByRise & r_parsebyrise
//    , TranslateParseByRiseTree & r_translateparsebyrisetree
    )
    :
    ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
     p_grammarpartStartNode ,
     & r_parsebyrise
     )
  {
  }

  KeepTrackOfCurrentParseTreePath::~KeepTrackOfCurrentParseTreePath()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace ParseTreeTraverser */
