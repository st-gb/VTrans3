/*
 * wxHTMLfileOutput.h
 *
 *  Created on: Jun 2, 2010
 *      Author: Stefan
 */

#ifndef WXHTMLFILEOUTPUT_H_
#define WXHTMLFILEOUTPUT_H_

#include <vector>
#include <set>
#include <string>

class TranslationAndConsecutiveID ;
class TranslationAndGrammarPart ;

class wxHTMLfileOutput
{
//  const std::vector<std::vector<TranslationAndConsecutiveID> > &
//    mcr_stdvec_stdvecTranslationAndConsecutiveID ;
  const std::vector<std::vector<TranslationAndGrammarPart> > &
    mcr_stdvec_stdvecTranslationAndGrammarPart ;
public:
  wxHTMLfileOutput(
//    const std::vector<std::vector<TranslationAndConsecutiveID> > &
//      cr_stdvec_stdvecTranslationAndConsecutiveID
    const std::vector<std::vector<TranslationAndGrammarPart> > &
      cr_stdvec_stdvecTranslationAndGrammarPart
    );
  virtual
  ~wxHTMLfileOutput();

  void GetTokenIndicesForConsecutiveID(
    WORD wConsecutiveID ,
    WORD wTokenIndexToExclude ,
    std::set<WORD> & r_stdset_wTokenIndex
    ) ;
  void writeFile(
//    const std::vector<std::vector<TranslationAndConsecutiveID> > &
//      cr_stdvec_stdvecTranslationAndConsecutiveID
    const std::vector<std::vector<TranslationAndGrammarPart> > &
      cr_stdvec_stdvecTranslationAndGrammarPart
    , wxString wxstrFile
  ) ;

  void writeFile(
    std::vector<std::string> & r_stdvec_stdstrWholeTransl
    , wxString wxstrFile
    ) ;
};

#endif /* WXHTMLFILEOUTPUT_H_ */
