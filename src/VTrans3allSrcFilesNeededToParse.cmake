
SET(VTRANS_PARSE_SRC_FILE_PATHS 
  ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/multithread/I_Thread.cpp

  ${VTRANS_ROOT_PATH}/Attributes/EnglishWord.cpp
  ${VTRANS_ROOT_PATH}/Attributes/GermanWord.cpp
  ${VTRANS_ROOT_PATH}/Attributes/PositionString.cpp
  ${VTRANS_ROOT_PATH}/Attributes/Word.cpp

  ${VTRANS_ROOT_PATH}/IO/GenerateXMLtreeFromParseTree.cpp

  ${VTRANS_ROOT_PATH}/Controller/TranslationProcess.cpp
  #${VTRANS_ROOT_PATH}/Controller/TranslationControllerBase.cpp

  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/IVocabularyInMainMem.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/CharStringStdMap/CharStringStdMap.cpp

  ${VTRANS_ROOT_PATH}/IO/ConfigurationReader.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/DictionaryReaderBase.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/OpenDictFileException.cpp

  #${VTRANS_ROOT_PATH}/Translate/ConditionsAndTranslation.cpp
  #${VTRANS_ROOT_PATH}/Translate/SyntaxTreePath.cpp
  #${VTRANS_ROOT_PATH}/Translate/AttributeTypeAndPosAndSize.cpp

  ${VTRANS_ROOT_PATH}/IO/UnknownGrammarPartNameException.cpp
  ${VTRANS_ROOT_PATH}/IO/ParseTree2XMLtreeTraverser.cpp

  ${VTRANS_ROOT_PATH}/ParseTree/DirectingLeavesMultipleIterTraverser.cpp
  ${VTRANS_ROOT_PATH}/ParseTree/DirectingLeavesInSingleIterationTraverser.cpp
  ${VTRANS_ROOT_PATH}/ParseTree/DuplicateParseTree.cpp
  ${VTRANS_ROOT_PATH}/Parse/GrammarPart.cpp
  ${VTRANS_ROOT_PATH}/Parse/ParseByRise_GenerateXMLtree.cpp
  ${VTRANS_ROOT_PATH}/ParseTree/KeepTrackOfCurrentParseTreePath.cpp
  ${VTRANS_ROOT_PATH}/Parse/ParseByRise.cpp
  ${VTRANS_ROOT_PATH}/Parse/ParseByRise_BuildParseTree.cpp
  ${VTRANS_ROOT_PATH}/Parse/ParseByRise_GrammarRule.cpp

  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/VocabularyAndTranslation.cpp
)

SET(TU_CHEMNITZ_DICT_ACCESS_FILE_PATHS
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInDictFile.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInRAM.cpp

  ${VTRANS_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInDictData.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInDictFile.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInRAM.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/BinarySearchInDictData.cpp

  ${VTRANS_ROOT_PATH}/Controller/DictReaderAndVocAccess/TUchemnitzEngWordSorted1stAndBinarySearch.cpp
  ${VTRANS_ROOT_PATH}/Controller/DictReaderAndVocAccess/TUchemnitzEngWordSorted1stAndBinarySearchInRAM.cpp
)

SET(DICT_CC_DICT_ACCESS_SRC_FILE_PATHS
  ${VTRANS_ROOT_PATH}/IO/dictionary/dict.cc/EN_DE/GermanTranslationByteOffset.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/dict.cc/EN_DE/GermanTranslationByteOffset.cpp
)
