/** SetSameConsecutiveIDforLeaves.h
 *  Created on: Jun 2, 2010
 *      Author: Stefan */

#ifndef SETSAMECONSECUTIVEIDFORLEAVES_H_
#define SETSAMECONSECUTIVEIDFORLEAVES_H_

#include <ParseTree/DirectingLeavesMultipleIterTraverser.hpp>//base class

/** Forward declararions */
class GrammarPart;
namespace VTrans3 { class BottomUpParser; }

namespace ParseTreeTraverser
{
  class SetSameConsecutiveIDforLeaves
    : public DirectingLeavesMultipleIterTraverser
  {
  public:
    WORD m_wConsecutiveID ;
    void LeaveFound() ;
    SetSameConsecutiveIDforLeaves(
      const GrammarPart * p_grammarpart ,
      VTrans3::BottomUpParser *,
      WORD wConsecutiveID );
    virtual
    ~SetSameConsecutiveIDforLeaves();
  };

}

#endif /* SETSAMECONSECUTIVEIDFORLEAVES_H_ */
