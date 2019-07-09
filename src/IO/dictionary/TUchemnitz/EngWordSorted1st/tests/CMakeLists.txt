#from https://cmake.org/cmake-tutorial/
cmake_minimum_required (VERSION 2.8)
project (trie_test)

if( NOT DEFINED VTRANS_ROOT_PATH )
  #Do not use (VTRANS3) a number (3) in the variable name
  set(VTRANS_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../")
  message("setting VTRANS_ROOT_PATH to ${VTRANS_ROOT_PATH}")
endif( NOT DEFINED VTRANS_ROOT_PATH )

if( NOT DEFINED COMMON_SOURCECODE_ROOT_PATH )
  #By default "common_sourcecode" lies/lays 1 level above VTrans3/src
  set(COMMON_SOURCECODE_ROOT_PATH ${VTRANS_ROOT_PATH}/../../common_sourcecode)
endif( NOT DEFINED COMMON_SOURCECODE_ROOT_PATH )

#see http://stackoverflow.com/questions/8934295/add-source-in-a-subdirectory-to-a-cmake-project
set(SOURCE_FILE_PATHS
  ${SOURCE_FILE_PATHS}
  ${PROJECT_SOURCE_DIR}/Trie_test.cpp
  ${VTRANS_ROOT_PATH}/Attributes/EnglishWord.cpp
  ${VTRANS_ROOT_PATH}/Attributes/GermanWord.cpp
  ${VTRANS_ROOT_PATH}/Attributes/PositionString.cpp
  ${VTRANS_ROOT_PATH}/Attributes/Word.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/DictionaryReaderBase.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/BinarySearchInDictData.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/IVocabularyInMainMem.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/VocabularyAndTranslation.cpp
  ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/multithread/I_Thread.cpp
)

if(WIN32)
  set(SOURCE_FILE_PATHS
    ${COMMON_SOURCECODE_ROOT_PATH}/Windows/ErrorCode/LocalLanguageMessageFromErrorCode.cpp
    ${COMMON_SOURCECODE_ROOT_PATH}/Windows/ErrorCode/GetErrorMessageFromErrorCode.cpp
  )
else()
  #Important: under Windows the original files should be included, else errors.
  set( INCLUDE_DIRS
    ${INCLUDE_DIRS}
    ${COMMON_SOURCECODE_ROOT_PATH}/Controller/MSVC_adaption
    )
endif()
if(UNIX)
  set(SOURCE_FILE_PATHS
    ${SOURCE_FILE_PATHS}
    ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/Linux/EnglishMessageFromErrorCode/EnglishMessageFromErrorCode.cpp
  )
endif()

set( INCLUDE_DIRS
  ${INCLUDE_DIRS}
  ${VTRANS_ROOT_PATH}
  ${COMMON_SOURCECODE_ROOT_PATH}
)

include_directories( ${INCLUDE_DIRS} )

add_executable(${PROJECT_NAME} ${SOURCE_FILE_PATHS})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

