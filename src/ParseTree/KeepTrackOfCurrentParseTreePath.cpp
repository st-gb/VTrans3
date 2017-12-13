/** KeepTrackOfCurrentParseTreePath.cpp
 *  Created on: 13.12.2012
 *      Author: Stefan   */

#include <ParseTree/KeepTrackOfCurrentParseTreePath.hpp>
#include <preprocessor_macros/logging_preprocessor_macros.h>

namespace ParseTreeTraverser
{

  KeepTrackOfCurrentParseTreePath::KeepTrackOfCurrentParseTreePath(
    const GrammarPart * p_grammarpartStartNode
    , VTrans3::BottomUpParser & r_bottomUpParser
//    , TranslateParseByRiseTree & r_translateparsebyrisetree
    )
    :
    ParseTreeTraverser::DirectingLeavesMultipleIterTraverser(
     p_grammarpartStartNode ,
     & r_bottomUpParser
     )
  {
  }

  //TODO double free or corruption in member variable std::vector<GrammarPart *>
  // for the input text from farkastranslations.com, "The Trial" and the dict.cc
  // dictionary:
  // "Someone must have been telling lies about Josef K., he knew he had done 
  // nothing wrong but, one [...]"
  KeepTrackOfCurrentParseTreePath::~KeepTrackOfCurrentParseTreePath()
  {
    LOGN_DEBUG("")
  }

} /* namespace ParseTreeTraverser */
