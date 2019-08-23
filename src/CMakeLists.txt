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
#SET(EXE_TYPE "wxGUI" CACHE STRING "Some user-specified option")
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
  ${MINI_XML_ROOT_DIR} #e.g. from build.gradle or a shell script/command line
  ${COMMON_SOURCECODE_ROOT_PATH}
  ${PROJECT_SOURCE_DIR}
  )

#from https://stackoverflow.com/questions/6902149/listing-include-directories-in-cmake
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "incl.dir='${dir}'")
endforeach()

if(NOT WIN32)
  #Important: under Windows the original files should be included, else errors.
  set( INCLUDE_DIRS
    ${INCLUDE_DIRS}
    #"/usr/include" #for MiniXML's mxml.h for Android
    ${COMMON_SOURCECODE_ROOT_PATH}/Controller/MSVC_adaption
    )
endif(NOT WIN32)
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
  #-H #show includes hierarchy
  -DCOMPILE_LOGGER_MULTITHREAD_SAFE
  #-D__WXDEBUG__
  -DUSE_OWN_LOGGER
  -DCOMPILE_WITH_LOG
  #-DCOMPILE_AS_EXECUTABLE #not as dynamic library but as standalone executable
  -DUNICODE
)

#see https://codeyarns.com/2015/05/14/build-types-in-cmake/
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
 set( CXX_DEFINITIONS
  ${CXX_DEFINITIONS}
  -D_DEBUG
  )
endif()

if( DEFINED EVALUATE_PROCESSING)
  #if({EVALUATE_PROCESSING} STREQUAL "YES" )
  #According to Chandler Carruth in 
  # "CppCon 2015: Chandler Carruth 
  # "Tuning C++: Benchmarks, and CPUs, and Compilers! Oh My!"
  # 1:17:00"-fno-omit-frame-pointer" is better then dwarf unwinding for
  #call stack information
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer")
#endif( DEFINED EVALUATE_PROCESSING AND ${EVALUATE_PROCESSING} STREQUAL "YES" )
endif( DEFINED EVALUATE_PROCESSING )

if( DEFINED COMPILE_WITH_OPENMP )
 if( ${COMPILE_WITH_OPENMP} STREQUAL "YES")
  message("COMPILE_WITH_OPENMP defined:" ${COMPILE_WITH_OPENMP} )
  #from https://stackoverflow.com/questions/12399422/how-to-set-linker-flags-for-openmp-in-cmakes-try-compile-function
  find_package(OpenMP)
  if (OPENMP_FOUND)
      message("OpenMP found")
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
      #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
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
endif( DEFINED COMPILE_WITH_OPENMP )

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
  #For Android NDK there's no "pthread.a" to link to ?!
  if( ${EXE_TYPE} STREQUAL "Android" )
  else( ${EXE_TYPE} STREQUAL "Android" )
  set( LIBRARIES
    ${LIBRARIES}
    pthread
  )
  endif( ${EXE_TYPE} STREQUAL "Android" )
  set( BASIC_TRANSLATION_SOURCE_FILES
    ${BASIC_TRANSLATION_SOURCE_FILES}
    ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/POSIX/multithread/pthreadBasedI_Thread.cpp
    ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/POSIX/multithread/Event.cpp
    ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/Linux/EnglishMessageFromErrorCode/EnglishMessageFromErrorCode.cpp
    ${COMMON_SOURCECODE_ROOT_PATH}/OperatingSystem/Linux/FileSystem/GetCurrentWorkingDir/GetCurrentWorkingDir.cpp
  )
endif (UNIX)
if(WIN32)
    set(BASIC_TRANSLATION_SOURCE_FILES
      ${BASIC_TRANSLATION_SOURCE_FILES}
      ${COMMON_SOURCECODE_ROOT_PATH}/Windows/Logger/LogEntryOutputter.cpp
      ${COMMON_SOURCECODE_ROOT_PATH}/Windows/FileSystem/File/File.cpp
      ${COMMON_SOURCECODE_ROOT_PATH}/Windows/ErrorCode/GetErrorMessageFromErrorCode.cpp
      ${COMMON_SOURCECODE_ROOT_PATH}/Windows/ErrorCode/GetErrorMessageFromLastErrorCode.cpp
      ${COMMON_SOURCECODE_ROOT_PATH}/Windows/ErrorCode/LocalLanguageMessageFromErrorCode.cpp
      )
    set( LIBRARIES
      ${LIBRARIES}
      Shlwapi #PathIsRelativeA(...), PathFileExistsA(...)
      )
else(WIN32)
    set(BASIC_TRANSLATION_SOURCE_FILES
      ${BASIC_TRANSLATION_SOURCE_FILES}
      ${COMMON_SOURCECODE_ROOT_PATH}/Controller/Logger/OutputHandler/StdOfStreamLogWriter.cpp )
endif(WIN32)

#set( LIBRARIES 
#  ${LIBRARIES}
#  boost_system #for boost::threadpool
#  boost_thread #for boost::threadpool
# )

set(BASIC_TRANSLATION_SOURCE_FILES
  ${BASIC_TRANSLATION_SOURCE_FILES}
	${COMMON_SOURCECODE_ROOT_PATH}/Controller/character_string/stdtstr.cpp
  ${VTRANS3_ROOT_PATH}/multi_threaded_translation/TranslateThreadProc.cpp
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

	${COMMON_SOURCECODE_ROOT_PATH}/data_structures/Trie/byteTrie/ByteTrie.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/data_structures/Trie/byteTrie/Trie.cpp

  ${COMMON_SOURCECODE_ROOT_PATH}/InputOutput/ReadFileContent/ReadFileContent.cpp

  ${PROJECT_SOURCE_DIR}/Controller/TranslationControllerBase.cpp

	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/CharMappedDictionary.cpp
	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/CharStringStdMap/CharStringStdMap.cpp
	${VTRANS3_ROOT_PATH}/VocabularyInMainMem/I_WordSearch.cpp
)

SET(VTRANS_TRANSLATE_STAGE_SRC_FILE_PATHS
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
)

#for VTrans3allSrcFilesNeededToParse.cmake
SET( VTRANS_ROOT_PATH  ${VTRANS3_ROOT_PATH} )
#for VTRANS_PARSE_SRC_FILE_PATHS, TU_CHEMNITZ_DICT_ACCESS_FILE_PATHS
include(${VTRANS3_ROOT_PATH}/VTrans3allSrcFilesNeededToParse.cmake)

if ( ${EXE_TYPE} STREQUAL "dynlib" )
  set(CREATE_DYN_LIB 1)
endif( ${EXE_TYPE} STREQUAL "dynlib" )

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
  set(TRANSLATE 1)
endif( ${EXE_TYPE} STREQUAL "wxGUI" )

if( ${EXE_TYPE} STREQUAL "console")
  set(EXE_NAME ${PROJECT_NAME}_console)
  set(TRANSLATE 1)
endif( ${EXE_TYPE} STREQUAL "console" )

#Compiles all files in ${SOURCE_FILE_PATHS} ?!

if (ANDROID)
    message(STATUS "Hello from taka-no-me/android-cmake")
endif()
if( ${EXE_TYPE} STREQUAL "Android")
  set( CMAKE_BINARY_DIR
    ${ANDROID_DIR}/libs)
  set(CREATE_DYN_LIB 1)
  if( NOT DEFINED MINI_XML_SOURCE )
   message( WARNING "MINI_XML_SOURCE not defined")
   SET( USE_MINI_XML_STATIC_LIBRARY TRUE) 
  endif(NOT DEFINED MINI_XML_SOURCE)
  set(SOURCE_FILE_PATHS
    ${SOURCE_FILE_PATHS}
    ${VTRANS3_ROOT_PATH}/Controller/DynLib/vtrans_dynlib_VTransDynLibJNI.cpp
    ${COMMON_SOURCECODE_ROOT_PATH}/Controller/character_string/format_as_string.cpp
    )
  add_definitions(-D__ANDROID__)

  set(EXE_NAME ${PROJECT_NAME} )
  message("ANDROID_NDK: ${ANDROID_NDK}")
  message("DCMAKE_CXX_COMPILER: ${DCMAKE_CXX_COMPILER}")
  message("ANDROID_ABI: ${ANDROID_ABI}")
  message("ANDROID_NATIVE_API_LEVEL: ${ANDROID_NATIVE_API_LEVEL}")
  message("ANDROID_TOOLCHAIN_NAME: ${ANDROID_TOOLCHAIN_NAME}")
  message("ANDROID_STL: ${ANDROID_STL}")
  message("CMAKE_ANDROID_ARCH: ${CMAKE_ANDROID_ARCH}")
  message("CMAKE_ANDROID_NDK: ${CMAKE_ANDROID_NDK}")
  message("CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION: ${CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION}")
  message("CMAKE_ANDROID_STL_TYPE: ${CMAKE_ANDROID_STL_TYPE}")
  #set(EXE_TYPE "dynlib")
  message( "SYSROOT: ${SYSROOT}" )
  #see https://stackoverflow.com/questions/16795583/android-error-cannot-open-crtbegin-dynamic-o-no-such-file-or-directory
  message( WARNING "for taka-no-me's Android CMake toolchain file CMAKE_CXX_FLAGS may not be set MANUALLY, else errors like: undefined reference to crtbegin.so")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -ffor-scope -fno-rtti -pipe -ffunction-sections -fdata-sections -ffast-math -Wnon-virtual-dtor -Wreorder -Wsign-promo -fvisibility=hidden -fvisibility-inlines-hidden -Wstrict-null-sentinel -Os -funroll-all-loops -fpeel-loops -ftree-vectorize")
  #TODO: are these flags used at all?
  set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,--as-needed -Wl,--gc-sections -Wl,--no-undefined -Wl,--strip-all -Wl,-rpath-link=${ANDROID_NDK_SYSROOT}/usr/lib/ -L${ANDROID_NDK_SYSROOT}/usr/lib/")
  message( "ANDROID_NDK_SYSROOT: ${ANDROID_NDK_SYSROOT}" )
  message( "DISCORDIA_ANDROID_CMAKE_HOME: ${DISCORDIA_ANDROID_CMAKE_HOME}")
  #include("${DISCORDIA_ANDROID_CMAKE_HOME}/android.apk.cmake" REQUIRED)
endif( ${EXE_TYPE} STREQUAL "Android")

if( CREATE_DYN_LIB )#Android, native dyn lib
  set(TRANSLATE 1)
  set( CXX_DEFINITIONS
    ${CXX_DEFINITIONS}
    -DNO_USER_INTERFACE
    )
endif()
if( TRANSLATE )
  set( CXX_DEFINITIONS
    ${CXX_DEFINITIONS}
    -DUSE_TRANSLATION_RULES
    )
endif()

message("MINI_XML_ROOT_DIR: ${MINI_XML_ROOT_DIR}")
SET(MINI_XML_SRC_DIR ${MINI_XML_ROOT_DIR})
add_subdirectory(./IO
  #EXCLUDE_FROM_ALL
  )

set(SOURCE_FILE_PATHS
  ${SOURCE_FILE_PATHS}
  ${VTRANS_PARSE_SRC_FILE_PATHS}
  ${TU_CHEMNITZ_DICT_ACCESS_FILE_PATHS}
  ${DICT_CC_DICT_ACCESS_SRC_FILE_PATHS}
  ${VTRANS_TRANSLATE_STAGE_SRC_FILE_PATHS}
  ${BASIC_TRANSLATION_SOURCE_FILES}
  ${MINI_XML_SOURCE_FILES}
  ${CONFIGURATION_READER_SOURCE_FILES}
  ${ATTRIBUTE_DATA_SOURCE_FILES}
)

message("CXX_DEFINITIONS for compiling:${CXX_DEFINITIONS}")
add_definitions(${CXX_DEFINITIONS})

if ( CREATE_DYN_LIB )
  message("should create dyn lib")
  set(EXE_NAME ${PROJECT_NAME} )
  set(SOURCE_FILE_PATHS 
    ${SOURCE_FILE_PATHS} 
    ${PROJECT_SOURCE_DIR}/Controller/DynLib/dynlib_main.cpp
    )
  #https://stackoverflow.com/questions/11783932/how-to-add-linker-or-compile-flag-in-cmake-file
  #"--no-undefined" reports "undefined reference to" -errors while compiling
  # else these errors are reported when linking at first
  #see https://stackoverflow.com/questions/2356168/force-gcc-to-notify-about-undefined-references-in-shared-libraries
  #SET( CMAKE_CXX_FLAGS 
  #  ${CMAKE_CXX_FLAGS}
  #  "-Wl,--no-undefined -fPIC")
  message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
  message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
  message( "COMPILE_DEFINITIONS = ${COMPILE_DEFINITIONS}" )
  add_library(${EXE_NAME} SHARED ${SOURCE_FILE_PATHS})
  message ("should link libraries \"${LIBRARIES}\" to executable")
  target_link_libraries(${EXE_NAME} ${LIBRARIES})
  message ("library name: " $<TARGET_FILE_NAME:${EXE_NAME}> )
  #execute_process("file " $<TARGET_FILE:${EXE_NAME}>)
  #problems when building for Android with "cmd.exe" that does not know the "file" command
  if(UNIX)
    add_custom_command( 
      TARGET ${EXE_NAME} 
      POST_BUILD 
      COMMAND "file" $<TARGET_FILE:${EXE_NAME}> )
  endif(UNIX)
  #if( ${EXE_TYPE} STREQUAL "Android")
    string(COMPARE EQUAL "${ANDROID_DIR}" "" result)
    if(result)
      message("creating apk file--ANDROID_DIR:${ANDROID_DIR}")
    #from https://github.com/Discordia/android-cmake-example/blob/master/CMakeLists.txt
#    android_create_apk(
#      ${EXE_NAME}
#      "de.it_gebauer.VTrans"
#      "${ANDROID_DIR}/apk" #where to create the apk directory
#      "${ANDROID_DIR}/libs" #dynamic library (*.so) files
#      "${ANDROID_DIR}" #where the AndroidManifest.xml resides
#      "${ANDROID_DIR}/assets" #assets like the dictionary
#      "E:/Android/sdk"
#      )
    else()
      message("not creating apk file because ANDROID_DIR is empty")
    endif()
  #endif()
else(CREATE_DYN_LIB)

  add_executable(${EXE_NAME} ${SOURCE_FILE_PATHS})
  target_link_libraries(${EXE_NAME} ${LIBRARIES})
if(DEFINED WXWIDGETS)
  #https://wiki.wxwidgets.org/CMake
  message ("should link wxWidgets libraries ${wxWidgets_LIBRARIES} to executable")
  #This line is necessary, else linker errors: undefined reference to "wx..."
  target_link_libraries(${EXE_NAME} ${wxWidgets_LIBRARIES})
  message ("wxWidgets type: ${wxWidgets_CONFIGURATION}")
endif (DEFINED WXWIDGETS)
endif(CREATE_DYN_LIB)

message("EXE_NAME:${EXE_NAME}")

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
