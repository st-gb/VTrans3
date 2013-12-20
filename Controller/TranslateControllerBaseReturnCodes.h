/*
 * TranslateControllerBaseReturnCodes.h
 *
 *  Created on: Jan 18, 2011
 *      Author: Stefan
 */

#ifndef TRANSLATECONTROLLERBASERETURNCODES_H_
#define TRANSLATECONTROLLERBASERETURNCODES_H_

namespace TranslationControllerBaseClass
{
  namespace InitFunction
  {
//    /*const*/ char ** /*const*/ retCodeDescriptions;
//    = { "success", "vocabularyFilePathIsEmpty",
//      "loadingVocabularyFileFailed", "creatingLogFileFailed",
//      "loadingMainConfigFileFailed" };
    enum Init_return_codes
    {
      success = 0,
      vocabularyFilePathIsEmpty,
      loadingVocabularyFileFailed,
      creatingLogFileFailed,
      loadingMainConfigFileFailed
    };
  }
}

#endif /* TRANSLATECONTROLLERBASERETURNCODES_H_ */
