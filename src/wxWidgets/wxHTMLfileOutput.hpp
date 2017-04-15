/** wxHTMLfileOutput.h
 *  Created on: Jun 2, 2010
 *      Author: Stefan  */

#ifndef WXHTMLFILEOUTPUT_H_
#define WXHTMLFILEOUTPUT_H_

#include <set>//class std::set
#include <vector>//class std::vector
#include <string>//class std::string
#include <windef.h> //WORD
#include <Attributes/TranslationResult.hpp> //WordCompoundsAtSameTokenIndex

class TranslationAndConsecutiveID ;
class TranslationAndGrammarPart ;

class wxHTMLfileOutput
{
//  const std::vector<std::vector<TranslationAndConsecutiveID> > &
//    mcr_stdvec_stdvecTranslationAndConsecutiveID ;
  //TODO doesn't it need to be of type "TranslationResult"?
  const WordCompoundsAtSameTokenIndex & mcr_wordCompoundsAtSameTokenIndex ;
public:
  wxHTMLfileOutput(
//    const std::vector<std::vector<TranslationAndConsecutiveID> > &
//      cr_stdvec_stdvecTranslationAndConsecutiveID
    const WordCompoundsAtSameTokenIndex &
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
    const WordCompoundsAtSameTokenIndex &
    , wxString wxstrFile
  ) ;

  void writeFile(
    std::vector<std::string> & r_stdvec_stdstrWholeTransl
    , wxString wxstrFile
    ) ;
};

#endif /* WXHTMLFILEOUTPUT_H_ */
