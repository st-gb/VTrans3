/*
 * SetSameConsecutiveIDforLeaves.h
 *
 *  Created on: Jun 2, 2010
 *      Author: Stefan
 */

#ifndef SETSAMECONSECUTIVEIDFORLEAVES_H_
#define SETSAMECONSECUTIVEIDFORLEAVES_H_

#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>

class GrammarPart ;

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
      ParseByRise * p_parsebyrise ,
      WORD wConsecutiveID );
    virtual
    ~SetSameConsecutiveIDforLeaves();
  };

}

#endif /* SETSAMECONSECUTIVEIDFORLEAVES_H_ */
