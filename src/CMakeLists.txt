#from https://cmake.org/cmake-tutorial/

cmake_minimum_required (VERSION 2.6)
project (VTrans)

set(COMMON_SOURCECODE_ROOT_PATH /home/sg/SourceCodeManagement/common_sourcecode)
set(WXWIDGETS_SOURCECODE_ROOT_PATH )
set(VTRANS3_ROOT_PATH ${PROJECT_SOURCE_DIR} )
#see http://accountingplusplus.blogspot.de/2010/12/cmake-wxwidgets.html :
#Set wxWidgets_USE_DEBUG to "ON" to link against WxWidgets debug libraries.
set( wxWidgets_USE_DEBUG ON)

#set(<variable> <value>  ... <valueN>)
set( INCLUDE_DIRS 
  ${COMMON_SOURCECODE_ROOT_PATH}
  ${COMMON_SOURCECODE_ROOT_PATH}/Controller/MSVC_adaption
  /usr/lib/i386-linux-gnu/wx/include/gtk2-unicode-debug-2.8
  /media/sg/devel/devel/MiniXML/mxml-2.8
  ${PROJECT_SOURCE_DIR}
  /usr/include/wx-2.8
)
message("include directories: ${INCLUDE_DIRS}")

message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message("wxWidgets_USE_DEBUG: ${wxWidgets_USE_DEBUG}")

set( CXX_DEFINITIONS
  -DCOMPILE_LOGGER_MULTITHREAD_SAFE
  #-D__WXDEBUG__
  -DUSE_OWN_LOGGER
  -DCOMPILE_WITH_LOG
  -DCOMPILE_AS_EXECUTABLE
  -DUNICODE
)

#https://cmake.org/cmake/help/v3.0/command/add_subdirectory.html
add_subdirectory(wxWidgets 
  #EXCLUDE_FROM_ALL
  )
add_subdirectory(./IO
  #EXCLUDE_FROM_ALL
  )
if( ANDROID_NDK )
else( ANDROID_NDK )
  add_subdirectory(Xerces
  #EXCLUDE_FROM_ALL
  )
  set(SOURCE_FILE_PATHS
    ${SOURCE_FILE_PATHS}
    ${XERCES_SOURCE_FILES}
  )
endif ( ANDROID_NDK )

#include_directories("${PROJECT_BINARY_DIR}")

#https://www.johnlamp.net/cmake-tutorial-4-libraries-and-subdirectories.html
include_directories( ${INCLUDE_DIRS} )
#if( DEBUG)
#  /usr/lib/i386-linux-gnu/wx/include/gtk2-unicode-debug-2.8

#"Add directories to the end of this directory’s include paths."
#https://cmake.org/Wiki/CMake_Useful_Variables for "ENV
#include_directories($ENV{COMMON_SOURCECODE_DIR})

#https://cmake.org/cmake/help/v3.0/command/add_executable.html
#add_executable(VTrans ${PROJECT_SOURCE_DIR})

#http://stackoverflow.com/questions/17653738/recursive-cmake-search-for-header-and-source-files
#file(GLOB_RECURSE SOURCE_FILE_PATHS *.cpp)

#http://stackoverflow.com/questions/16449676/remove-specific-file-from-cmake-build
#list(REMOVE_ITEM SOURCE_FILE_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
#LIST(FILTER <list> <regex> [<regex> ...] [OUTPUT_VARIABLE <variable>])
#set(SOURCE_FILE_PATHS_TEMP ${SOURCE_FILE_PATHS})
#list(REMOVE_ITEM SOURCE_FILE_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/testing.cpp)

set( ATTRIBUTE_DATA_SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/Attributes/EnglishWord.cpp
  ${PROJECT_SOURCE_DIR}/Attributes/GermanWord.cpp
  ${PROJECT_SOURCE_DIR}/Attributes/PositionString.cpp
  ${PROJECT_SOURCE_DIR}/Attributes/Word.cpp
)

if (UNIX)
  set( LIBRARIES
    ${LIBRARIES}
    pthread
  )
  set( BASIC_TRANSLATION_SOURCE_FILES
    ${BASIC_TRANSLATION_SOURCE_FILES}
    ${COMMON_SOURCECODE_ROOT_PATH}/Linux/multithread/pthreadBasedI_Thread.cpp
  	${COMMON_SOURCECODE_ROOT_PATH}/Linux/EnglishMessageFromErrorCode/EnglishMessageFromErrorCode.cpp
	  ${COMMON_SOURCECODE_ROOT_PATH}/Linux/FileSystem/GetCurrentWorkingDir/GetCurrentWorkingDir.cpp
  )
endif (UNIX)

set(BASIC_TRANSLATION_SOURCE_FILES
  ${BASIC_TRANSLATION_SOURCE_FILES}
	${COMMON_SOURCECODE_ROOT_PATH}/Controller/character_string/stdtstr.cpp
  ${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/preprocessor_logging_macros.cpp
  ${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/Logger.cpp
  ${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/LogLevel.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/Appender/FormattedLogEntryProcessor.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/Appender/RollingFileOutput.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/Formatter/I_LogFormatter.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/Formatter/HTMLlogFormatter.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/Formatter/Log4jFormatter.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/OutputHandler/I_LogEntryOutputter.cpp
  ${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/OutputHandler/StdCoutLogWriter.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/OutputHandler/StdOfStreamLogWriter.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/data_structures/Trie/byteTrie/ByteTrie.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/data_structures/Trie/byteTrie/Trie.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/multithread/I_Thread.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/DictionaryReaderBase.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/OpenDictFileException.cpp
	${VTRANS3_ROOT_PATH}/Controller/DictReaderAndVocAccess/TUchemnitzEngWordSorted1stAndBinarySearch.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/BinarySearchInDictFile.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/EachAttributeInSingleLine.cpp
  ${PROJECT_SOURCE_DIR}/Controller/TranslationControllerBase.cpp
  ${VTRANS3_ROOT_PATH}/IO/UnknownGrammarPartNameException.cpp
  ${VTRANS3_ROOT_PATH}/multi_threaded_translation/TranslateThreadProc.cpp
  ${PROJECT_SOURCE_DIR}/Parse/DirectingLeavesInSingleIterationTraverser.cpp
  ${PROJECT_SOURCE_DIR}/Parse/DirectingLeavesMultipleIterTraverser.cpp
  ${PROJECT_SOURCE_DIR}/Parse/DuplicateParseTree.cpp
  ${PROJECT_SOURCE_DIR}/Parse/GrammarPart.cpp
  ${PROJECT_SOURCE_DIR}/Parse/KeepTrackOfCurrentParseTreePath.cpp
  ${PROJECT_SOURCE_DIR}/Parse/ParseByRise.cpp
  ${PROJECT_SOURCE_DIR}/Parse/ParseByRise_BuildParseTree.cpp
  ${PROJECT_SOURCE_DIR}/Parse/ParseByRise_GrammarRule.cpp
	${VTRANS3_ROOT_PATH}/Translate/AddVocAndTranslDefinitions.cpp
	${VTRANS3_ROOT_PATH}/Translate/AttributeTypeAndPosAndSize.cpp
	${VTRANS3_ROOT_PATH}/Translate/ConditionsAndTranslation.cpp
	${VTRANS3_ROOT_PATH}/Translate/InsertIntoTreeTransverser.cpp
	${VTRANS3_ROOT_PATH}/Translate/SetSameConsecutiveIDforLeaves.cpp
	${VTRANS3_ROOT_PATH}/Translate/SummarizePersonIndex.cpp
	${VTRANS3_ROOT_PATH}/Translate/SyntaxTreePath.cpp
	${VTRANS3_ROOT_PATH}/Translate/TestIfTranslationRuleApplies.cpp
	${VTRANS3_ROOT_PATH}/Translate/TransformationRule.cpp
	${VTRANS3_ROOT_PATH}/Translate/TransformTreeTraverser.cpp
	${VTRANS3_ROOT_PATH}/Translate/TranslatedTreeTraverser.cpp
	${VTRANS3_ROOT_PATH}/Translate/TranslateParseByRiseTree.cpp
	${VTRANS3_ROOT_PATH}/Translate/TranslateTreeTraverser.cpp
	${VTRANS3_ROOT_PATH}/Translate/TranslationRule.cpp
	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/CharMappedDictionary.cpp
	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/CharStringStdMap/CharStringStdMap.cpp
	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/IVocabularyInMainMem.cpp
	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/I_WordSearch.cpp
	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInDictFile.cpp
	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/VocabularyAndTranslation.cpp
)

set(SOURCE_FILE_PATHS
  ${SOURCE_FILE_PATHS}
  ${BASIC_TRANSLATION_SOURCE_FILES}
  ${MINI_XML_SOURCE_FILES}
  ${CONFIGURATION_READER_SOURCE_FILES}
  ${ATTRIBUTE_DATA_SOURCE_FILES}
)

add_definitions(${CXX_DEFINITIONS})
#add_library(${LIBRARIES} )
#specify which libraries an executable target links against.
#TARGET_LINK_LIBRARIES( ${LIBRARIES})

#target_link_libraries(${LIBRARIES})

#https://wiki.wxwidgets.org/CMake
set(wxWidgets_CONFIGURATION mswud)
SET(wxWidgets_USE_LIBS core base adv)
find_package(wxWidgets COMPONENTS core base adv REQUIRED)
include(${wxWidgets_USE_FILE})

message("wxWidgets:: ${wxWidgets_LIBRARIES}")

add_executable(VTrans ${SOURCE_FILE_PATHS})
#https://wiki.wxwidgets.org/CMake
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
target_link_libraries(VTrans ${LIBRARIES})
message (STATUS "wxWidgets type: ${wxWidgets_CONFIGURATION}")

#target_link_libraries (VTrans3 wxWidgets)

#http://stackoverflow.com/questions/8934295/add-source-in-a-subdirectory-to-a-cmake-project

#https://cmake.org/pipermail/cmake/2006-October/011548.html
set(CPACK_SOURCE_IGNORE_FILES
"Makefile\\\\.in$"
"~$"
"^${PROJECT_SOURCE_DIR}.*/CVS/"
"^${PROJECT_SOURCE_DIR}/debian/"
"^${PROJECT_SOURCE_DIR}/old/"
)
