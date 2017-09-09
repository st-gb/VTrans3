#from https://cmake.org/cmake-tutorial/
cmake_minimum_required (VERSION 2.8)
project (VTrans)
#from https://cmake.org/Wiki/CMake/Testing_With_CTest
#enable_testing()

#https://stackoverflow.com/questions/19207721/build-android-ndk-project-with-cmake

if( NOT DEFINED COMMON_SOURCECODE_ROOT_PATH )
  #By default "common_sourcecode" lies/lays 1 level above VTrans3/src
  set(COMMON_SOURCECODE_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../common_sourcecode)
endif( NOT DEFINED COMMON_SOURCECODE_ROOT_PATH )
set(VTRANS3_ROOT_PATH ${PROJECT_SOURCE_DIR} )

#from http://stackoverflow.com/questions/12896988/passing-the-argument-to-cmake-via-command-prompt
# on command line: "-DEXE_TYPE:STRING=GUI"
SET(EXE_TYPE "wxGUI" CACHE STRING "Some user-specified option")
message("EXE_TYPE: ${EXE_TYPE}")

message("CXX flags: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")

if( ${EXE_TYPE} STREQUAL "wxGUI")
  #https://cmake.org/cmake/help/v3.0/command/add_subdirectory.html
  add_subdirectory(wxWidgets 
    #EXCLUDE_FROM_ALL
    )
  #message("CXX_DEFINITIONS after wxWidgets/CMAKELists.txt: ${CXX_DEFINITIONS}")
endif( ${EXE_TYPE} STREQUAL "wxGUI")

if( ${EXE_TYPE} STREQUAL "console")
  set(SOURCE_FILE_PATHS
    ${PROJECT_SOURCE_DIR}/Controller/ConsoleTranslationController.cpp
  )
endif( ${EXE_TYPE} STREQUAL "console")

#set(<variable> <value>  ... <valueN>)
#/home/sg/Downloads/boost_threadpool-0_2_5-src/threadpool
set( INCLUDE_DIRS 
  ${INCLUDE_DIRS}
  ${MINI_XML_SOURCE}
 ${COMMON_SOURCECODE_ROOT_PATH}
  ${COMMON_SOURCECODE_ROOT_PATH}/Controller/MSVC_adaption
  ${PROJECT_SOURCE_DIR}
c)

#set(PROJECT_BINARY_DIR /home/sg/SourceCodeManagement/VTrans3/build)
#http://stackoverflow.com/questions/6594796/how-do-i-make-cmake-output-into-a-bin-dir
#set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/../build)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")
#set(CMAKE_CURRENT_BINARY_DIRECTORY /home/sg/SourceCodeManagement/VTrans3/bin)

message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")

set( CXX_DEFINITIONS
  ${CXX_DEFINITIONS}
  -DCOMPILE_LOGGER_MULTITHREAD_SAFE
  #-D__WXDEBUG__
  -DUSE_OWN_LOGGER
  -DCOMPILE_WITH_LOG
  -DCOMPILE_AS_EXECUTABLE
  -DUNICODE
)


if( ${EVALUATE_PROCESSING} STREQUAL "YES" )
  #According to Chandler Carruth in 
  # "CppCon 2015: Chandler Carruth 
  # "Tuning C++: Benchmarks, and CPUs, and Compilers! Oh My!"
  # 1:17:00"-fno-omit-frame-pointer" is better then dwarf unwinding for
  #call stack information
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer")
endif( ${EVALUATE_PROCESSING} STREQUAL "YES" )

if( ${COMPILE_WITH_OPENMP} STREQUAL "YES")
  message("COMPILE_WITH_OPENMP defined:" ${COMPILE_WITH_OPENMP} )
  #from https://stackoverflow.com/questions/12399422/how-to-set-linker-flags-for-openmp-in-cmakes-try-compile-function
  find_package(OpenMP)
  if (OPENMP_FOUND)
      message("OpenMP found")
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()
  set( CXX_DEFINITIONS
    ${CXX_DEFINITIONS}
    -DCOMPILE_WITH_OPENMP)
else( ${COMPILE_WITH_OPENMP} STREQUAL "YES" )
  set(BASIC_TRANSLATION_SOURCE_FILES
    ${BASIC_TRANSLATION_SOURCE_FILES}
#    ${VTRANS3_ROOT_PATH}/multi_threaded_translation/TranslateThreadProc.cpp
#    ${VTRANS3_ROOT_PATH}/multi_threaded_translation/native_threads.cpp 
  )
endif( ${COMPILE_WITH_OPENMP} STREQUAL "YES" )

add_subdirectory(./IO
  #EXCLUDE_FROM_ALL
  )
if( ANDROID_NDK )
else( ANDROID_NDK )
  #add_subdirectory(Xerces
  #EXCLUDE_FROM_ALL
  #)
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
    ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/POSIX/multithread/pthreadBasedI_Thread.cpp
    ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/POSIX/multithread/Event.cpp
    ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/Linux/EnglishMessageFromErrorCode/EnglishMessageFromErrorCode.cpp
    ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/Linux/FileSystem/GetCurrentWorkingDir/GetCurrentWorkingDir.cpp
  )
endif (UNIX)

#set( LIBRARIES 
#  ${LIBRARIES}
#  boost_system #for boost::threadpool
#  boost_thread #for boost::threadpool
# )

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
	${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/multithread/I_Thread.cpp
  ${COMMON_SOURCECODE_ROOT_PATH}/InputOutput/ReadFileContent/ReadFileContent.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/DictionaryReaderBase.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/OpenDictFileException.cpp
	${VTRANS3_ROOT_PATH}/Controller/DictReaderAndVocAccess/TUchemnitzEngWordSorted1stAndBinarySearch.cpp
	${VTRANS3_ROOT_PATH}/Controller/DictReaderAndVocAccess/TUchemnitzEngWordSorted1stAndBinarySearchInRAM.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/BinarySearchInDictData.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInDictFile.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInRAM.cpp
	${VTRANS3_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInDictData.cpp
  ${PROJECT_SOURCE_DIR}/Controller/TranslationControllerBase.cpp
  ${VTRANS3_ROOT_PATH}/IO/UnknownGrammarPartNameException.cpp
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
	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInRAM.cpp
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

if(DEFINED WXWIDGETS)
  #https://wiki.wxwidgets.org/CMake
  set(wxWidgets_CONFIGURATION mswud) #TODO wrong type
  SET(wxWidgets_USE_LIBS core base adv)
  find_package(wxWidgets COMPONENTS core base adv REQUIRED)
  include(${wxWidgets_USE_FILE})
  message("wxWidgets_LIBRARIES:${wxWidgets_LIBRARIES}")
endif (DEFINED WXWIDGETS)

message("include directories:${INCLUDE_DIRS}")

if( ${EXE_TYPE} STREQUAL "wxGUI")
  set(EXE_NAME wx${PROJECT_NAME}_GUI)
endif( ${EXE_TYPE} STREQUAL "wxGUI" )

if( ${EXE_TYPE} STREQUAL "console")
  set(EXE_NAME ${PROJECT_NAME}_console)
endif( ${EXE_TYPE} STREQUAL "console" )

message("CXX_DEFINITIONS for compiling:${CXX_DEFINITIONS}")
#Compiles all files in ${SOURCE_FILE_PATHS} ?!

if( ${EXE_TYPE} STREQUAL "Android")
  if( NOT DEFINED MINI_XML_SOURCE )
   warning("MINI_XML_SOURCE not defined")
  endif(NOT DEFINED MINI_XML_SOURCE)
  add_definitions(-D__ANDROID__)
  set(MINI_XML_SOURCE_FILE_PATHS
    ${MINI_XML_SOURCE}/mxml-attr.c
    ${MINI_XML_SOURCE}/mxmldoc.c
    ${MINI_XML_SOURCE}/mxml-entity.c
    ${MINI_XML_SOURCE}/mxml-file.c
    ${MINI_XML_SOURCE}/mxml-get.c
    ${MINI_XML_SOURCE}/mxml-index.c
    ${MINI_XML_SOURCE}/mxml-node.c
    ${MINI_XML_SOURCE}/mxml-private.c
    ${MINI_XML_SOURCE}/mxml-search.c
    ${MINI_XML_SOURCE}/mxml-set.c
    ${MINI_XML_SOURCE}/mxml-string.c
    ${MINI_XML_SOURCE}/testmxml.c
    )

  #link_directories(${MINI_XML_SOURCE})
  #set(SOURCE_FILE_PATHS
  #  ${SOURCE_FILE_PATHS} 
  #  ${MINI_XML_SOURCE_FILE_PATHS} )

  set(EXE_NAME ${PROJECT_NAME} )
  message("ANDROID_NDK: ${ANDROID_NDK}")
  message("ANDROID_ABI: ${ANDROID_ABI}")
  message("ANDROID_NATIVE_API_LEVEL: ${ANDROID_NATIVE_API_LEVEL}")
  message("ANDROID_TOOLCHAIN_NAME: ${ANDROID_TOOLCHAIN_NAME}")
  message("ANDROID_STL: ${ANDROID_STL}")
  message("CMAKE_ANDROID_ARCH: ${CMAKE_ANDROID_ARCH}")
  message("CMAKE_ANDROID_NDK: ${CMAKE_ANDROID_NDK}")
  message("CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION: ${CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION}")
  message("CMAKE_ANDROID_STL_TYPE: ${CMAKE_ANDROID_STL_TYPE}")
  set(EXE_TYPE "dynlib")
endif( ${EXE_TYPE} STREQUAL "Android")

if ( ${EXE_TYPE} STREQUAL "dynlib" )
  set(EXE_NAME ${PROJECT_NAME} )
  set(SOURCE_FILE_PATHS 
    ${SOURCE_FILE_PATHS} 
    ${PROJECT_SOURCE_DIR}/Controller/DynLib/dynlib_main.cpp
    )
  #https://stackoverflow.com/questions/11783932/how-to-add-linker-or-compile-flag-in-cmake-file
  #"--no-undefined" reports "undefined reference to" -errors while compiling
  # else these errors are reported when linking at first
  #see https://stackoverflow.com/questions/2356168/force-gcc-to-notify-about-undefined-references-in-shared-libraries
  SET( CMAKE_CXX_FLAGS "-Wl,--no-undefined")
  add_library(${EXE_NAME} SHARED ${SOURCE_FILE_PATHS})
  message ("should link libraries \"${LIBRARIES}\" to executable")
  target_link_libraries(${EXE_NAME} ${LIBRARIES})
endif( ${EXE_TYPE} STREQUAL "dynlib")

if(DEFINED WXWIDGETS)
  add_executable(${EXE_NAME} ${SOURCE_FILE_PATHS})
  target_link_libraries(${EXE_NAME} ${LIBRARIES})
  #https://wiki.wxwidgets.org/CMake
  message ("should link wxWidgets libraries ${wxWidgets_LIBRARIES} to executable")
  #This line is necessary, else linker errors: undefined reference to "wx..."
  target_link_libraries(${EXE_NAME} ${wxWidgets_LIBRARIES})
  message ("wxWidgets type: ${wxWidgets_CONFIGURATION}")
endif (DEFINED WXWIDGETS)

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
