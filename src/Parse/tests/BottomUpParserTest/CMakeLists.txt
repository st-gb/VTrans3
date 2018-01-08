#from https://cmake.org/cmake-tutorial/
cmake_minimum_required (VERSION 2.8)
project (BottomUpParserTest)
#from https://cmake.org/Wiki/CMake/Testing_With_CTest
#enable_testing()

#https://stackoverflow.com/questions/19207721/build-android-ndk-project-with-cmake

if( NOT DEFINED COMMON_SOURCECODE_ROOT_PATH )
  #By default "common_sourcecode" lies/lays 1 level above VTrans3/src
  set(COMMON_SOURCECODE_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../common_sourcecode)
  message("setting COMMON_SOURCECODE_ROOT_PATH to ${COMMON_SOURCECODE_ROOT_PATH}")
endif( NOT DEFINED COMMON_SOURCECODE_ROOT_PATH )
if( NOT DEFINED VTRANS_ROOT_PATH )
  #Do not use (VTRANS3) a number (3) in the variable name
  set(VTRANS_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../")
  message("setting VTRANS_ROOT_PATH to ${VTRANS_ROOT_PATH}")
endif()
#set(USE_ADDRESS_SANITIZER TRUE)
if( DEFINED USE_ADDRESS_SANITIZER )
  message("should use address sanitizer")
  #from  https://github.com/gayanpathirage/SanitizersReviewTestSuite
  set(GCC_MINIMAL_STATIC_LIBASAN_VERSION 4.9.3)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GCC_MINIMAL_STATIC_LIBASAN_VERSION)
    #https://gist.github.com/kwk/4171e37f4bcdf7705329
    #https://stackoverflow.com/questions/44320465/whats-the-proper-way-to-enable-addresssanitizer-in-cmake-that-works-in-xcode
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fno-omit-frame-pointer -lasan")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
  else()
    message("GCC version > 4.9.3")
    #from  https://github.com/gayanpathirage/SanitizersReviewTestSuite
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} -static-libstdc++ -static-libasan -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libasan -fsanitize=address -fno-omit-frame-pointer")
  endif()
endif()

set(VTRANS3_ROOT_PATH ${PROJECT_SOURCE_DIR} )

set(INCLUDE_DIRS
  ${INCLUDE_DIRS}
  ${VTRANS_ROOT_PATH}
  ${COMMON_SOURCECODE_ROOT_PATH}
)

if(NOT WIN32)
  #Important: under Windows the original files should be included, else errors.
  set( INCLUDE_DIRS
    ${INCLUDE_DIRS}
    ${COMMON_SOURCECODE_ROOT_PATH}/Controller/MSVC_adaption
    )
endif(NOT WIN32)

#from http://stackoverflow.com/questions/12896988/passing-the-argument-to-cmake-via-command-prompt
# on command line: "-DEXE_TYPE:STRING=GUI"
#SET(EXE_TYPE "wxGUI" CACHE STRING "Some user-specified option")
message("EXE_TYPE: ${EXE_TYPE}")

message("CXX flags: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")

#see http://stackoverflow.com/questions/8934295/add-source-in-a-subdirectory-to-a-cmake-project
set(SOURCE_FILE_PATHS
  ${SOURCE_FILE_PATHS}
  ${PROJECT_SOURCE_DIR}/main.cpp

  ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/multithread/I_Thread.cpp

  ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/Linux/EnglishMessageFromErrorCode/EnglishMessageFromErrorCode.cpp
  ${VTRANS_ROOT_PATH}/UserInterface/ConsoleUserInterface.cpp

  ${VTRANS_ROOT_PATH}/Attributes/EnglishWord.cpp
  ${VTRANS_ROOT_PATH}/Attributes/PositionString.cpp

  ${VTRANS_ROOT_PATH}/Controller/TranslationProcess.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInDictFile.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/TUchemnitzEngWordSorted1st/BinarySearchInRAM.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/IVocabularyInMainMem.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/CharStringStdMap/CharStringStdMap.cpp
  ${VTRANS_ROOT_PATH}/Attributes/GermanWord.cpp

  ${VTRANS_ROOT_PATH}/IO/dictionary/DictionaryReaderBase.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/OpenDictFileException.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInDictData.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInDictFile.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/eachAttributeInSingleLine/BinarySearchInRAM.cpp
  ${VTRANS_ROOT_PATH}/IO/dictionary/TUchemnitz/EngWordSorted1st/BinarySearchInDictData.cpp
  ${VTRANS_ROOT_PATH}/Attributes/Word.cpp

  ${VTRANS_ROOT_PATH}/Controller/DictReaderAndVocAccess/TUchemnitzEngWordSorted1stAndBinarySearch.cpp
  ${VTRANS_ROOT_PATH}/Controller/DictReaderAndVocAccess/TUchemnitzEngWordSorted1stAndBinarySearchInRAM.cpp
  ${VTRANS_ROOT_PATH}/IO/UnknownGrammarPartNameException.cpp
  ${VTRANS_ROOT_PATH}/Parse/GrammarPart.cpp
  #${VTRANS_ROOT_PATH}/Parse/ParseByRise_GenerateXMLtree.cpp
  ${VTRANS_ROOT_PATH}/Parse/ParseByRise.cpp
  ${VTRANS_ROOT_PATH}/Parse/ParseByRise_BuildParseTree.cpp
  #Needed for building up parse trees
  ${VTRANS_ROOT_PATH}/ParseTree/DuplicateParseTree.cpp
  ${VTRANS_ROOT_PATH}/ParseTree/DirectingLeavesMultipleIterTraverser.cpp
  ${VTRANS_ROOT_PATH}/ParseTree/KeepTrackOfCurrentParseTreePath.cpp

  ${VTRANS_ROOT_PATH}/Parse/ParseByRise_GrammarRule.cpp
  ${VTRANS_ROOT_PATH}/VocabularyInMainMem/VocabularyAndTranslation.cpp
)
message("${VTRANS3_ROOT_PATH}/Parse/ParseByRise.cpp")

if(WIN32)
  set(LOGGER_SOURCE_FILES
    ${LOGGER_SOURCE_FILES}
    ${COMMON_SOURCECODE_ROOT_PATH}/Windows/Logger/LogEntryOutputter.cpp )
  set(SOURCE_FILE_PATHS
    ${SOURCE_FILE_PATHS}
    ${COMMON_SOURCECODE_ROOT_PATH}/Windows/FileSystem/File/File.cpp
    )
endif()
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  #https://stackoverflow.com/questions/6003368/unresolved-externals-in-c-when-using-vectors-and-find
  message("for Debug builds use flag /MTd for Multi-threaded Debug DLL")
  # Add "MTd" (Multi-threaded Debug DLL) flag to avoid linker errors for Debug builds.
  # Seems to be a bug of CMake, see https://gitlab.kitware.com/cmake/cmake/issues/16712
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

  #see http://microsoft.public.de.vc.narkive.com/NcREf3PC/haltepunkt-wird-nicht-erreicht-fur-dieses-dokument-wurden-keine-symbole-geladen :
  # add C/C++ General/Options -> Debug Information Format : Program Database for Edit & Continue (/ZI)
  # for ability to halt at breakpoints
  # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
  message("CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()
#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
if(WIN32)
else()
  set( LIBRARIES
    ${LIBRARIES}
    # http://csweb.cs.wfu.edu/~torgerse/Kokua/More_SGI/007-2579-009/sgi_html/ch09.html
    #malloc_ss
    )
endif()

set(SOURCE_FILE_PATHS
  ${SOURCE_FILE_PATHS}
  ${MINI_XML_SOURCE_FILES}
  ${LOGGER_SOURCE_FILES}
)

set( CXX_DEFINITIONS
  ${CXX_DEFINITIONS}
  #-H #show includes hierarchy
  -DCOMPILE_AS_EXECUTABLE
  -DUNICODE #both "UNICODE" and "_UNICODE" are needed for Microsoft Visual C++
  -D_UNICODE #in order to define TCHAR types and functions using these types
  -D_DEBUG
)

add_definitions(${CXX_DEFINITIONS})

#With MinGW/MSYS the static libary could only be created with the console from 
# >>MinGW dir<<\msys\1.0\msys.bat
if( DEFINED USE_MINI_XML_STATIC_LIBRARY)
  iF(WIN32)
  else(WIN32)
  set( LIBRARIES 
    pthread #Linux automatically adds prefix "lib" and suffix ".a"
  )
  endif(WIN32)
else ( DEFINED USE_MINI_XML_STATIC_LIBRARY)
endif(DEFINED USE_MINI_XML_STATIC_LIBRARY)

message( "INCLUDE_DIRS: ${INCLUDE_DIRS}" )
#https://www.johnlamp.net/cmake-tutorial-4-libraries-and-subdirectories.html
include_directories( ${INCLUDE_DIRS} )

  set(EXE_NAME ${PROJECT_NAME}_console)
  add_executable(${EXE_NAME} ${SOURCE_FILE_PATHS})
  message("linking LIBRARIES:${LIBRARIES}")
  target_link_libraries(${EXE_NAME} ${LIBRARIES})

