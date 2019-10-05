/*
 * dictReaderAndVocAccess_type.hpp
 *
 *  Created on: 11.12.2013
 *      Author: mr.sys
 */

#ifndef DICTREADERANDVOCACCESS_TYPE_HPP_
#define DICTREADERANDVOCACCESS_TYPE_HPP_

//#include "TUchemnitzEngWordSorted1stAndBinarySearch.hpp"
//typedef VTrans3::TUchemnitzEngWordSorted1stAndBinarySearchInDictFile
//  dictReaderAndVocAccess_type;

///Pass the dictionary type via preprocessor definitions from the build system
/// (Makefile etc.) so it needn't be changed here.

#ifdef USE_TU_CHEMNITZ_DICT
#include "TUchemnitzEngWordSorted1stAndBinarySearchInRAM.hpp"
typedef VTrans3::TUchemnitzEngWordSorted1stAndBinarySearchInRAM
  dictReaderAndVocAccess_type;

#elif (defined USE_DICT_CC_DICT)
#include "Original_dict_cc_file_formatAndGermanTranslationByteOffset.hpp"
typedef VTrans3::Original_dict_cc_EN_DE_AndGermanTranslationByteOffset 
  dictReaderAndVocAccess_type;#
#endif

#endif /* DICTREADERANDVOCACCESS_TYPE_HPP_ */
