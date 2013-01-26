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
    enum Init_return_codes
    {
      success = 0,
      vocabularyFilePathIsEmpty,
      loadingVocabularyFileFailed,
      creatingLogFileFailed
    };
  }
}

#endif /* TRANSLATECONTROLLERBASERETURNCODES_H_ */
