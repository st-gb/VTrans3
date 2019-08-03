
if( NOT ${MINI_XML_ROOT_DIR} STREQUAL "")
  #message("MINI_XML_ROOT_DIR is defined: ${MINI_XML_ROOT_DIR}")
 if(EXISTS "${MINI_XML_ROOT_DIR}/config.h")
 else()
 message("calling MiniXML's configure")
 #generates needed "config.h"
 execute_process(COMMAND "./configure" WORKING_DIRECTORY ${MINI_XML_ROOT_DIR})
 endif()
else()
  message("MINI_XML_ROOT_DIR is not defined-> assuming installed header and libs")
  SET(USE_MINI_XML_STATIC_LIBRARY 1)
endif()

#If only parsing/creating the parse tree is needed.
set(MINI_XML_SOURCE_FILES
  ${VTRANS3_ROOT_PATH}/IO/MiniXML/GrammarRuleFileReader.cpp
  ${VTRANS3_ROOT_PATH}/IO/MiniXML/MainConfigFileReader.cpp
  ${VTRANS3_ROOT_PATH}/IO/MiniXML/MiniXMLconfigReader.cpp
)

if( TRANSLATE )#If also translating the parse tree should be possible.
  set(MINI_XML_SOURCE_FILES
    ${MINI_XML_SOURCE_FILES}
    ${VTRANS3_ROOT_PATH}/IO/MiniXML/ReadTranslationRuleFile.cpp
    ${VTRANS3_ROOT_PATH}/IO/MiniXML/TranslationRuleFile_EndXMLelement.cpp
    ${VTRANS3_ROOT_PATH}/IO/MiniXML/TranslationRuleFileReader.cpp
    ${VTRANS3_ROOT_PATH}/IO/MiniXML/VocAttributeDefintionHandler.cpp
    )
endif()

if( ANDROID_NDK )#TODO change to ~ "if(USE_TRANSLATE_STAGE)"
  set(MINI_XML_SOURCE_FILES
    ${MINI_XML_SOURCE_FILES}
    #Not needed for parsing.
    ${VTRANS3_ROOT_PATH}/IO/MiniXML/ReadTranslationRuleFile.cpp
   )
endif()

#With MinGW/MSYS the static libary could only be created with the console from 
# >>MinGW dir<<\msys\1.0\msys.bat
if( DEFINED USE_MINI_XML_STATIC_LIBRARY)
  iF(WIN32)
  set( LIBRARIES
    ${LIBRARIES}
    ${MINI_XML_SOURCE}/libmxml.a
    PARENT_SCOPE #to propagate changes to file that included THIS file
    )
  else(WIN32)
  set( LIBRARIES 
    mxml #Linux automatically adds prefix "lib" and suffix ".a"
    PARENT_SCOPE #to propagate changes to file that included THIS file
  )
  endif(WIN32)

else()
#for MINI_XML_LIB_SRC_FILE_PATHS
include(${COMMON_SOURCECODE_ROOT_PATH}/libraries/XML/MiniXML/srcFiles.cmake)
message("in MiniXML CMakeLists:" ${MINI_XML_LIB_SRC_FILE_PATHS} )
endif(DEFINED USE_MINI_XML_STATIC_LIBRARY)

set( CONFIGURATION_READER_SOURCE_FILES
  ${MINI_XML_SOURCE_FILES}
  ${MINI_XML_LIB_SRC_FILE_PATHS}
  PARENT_SCOPE #to propagate changes to file that included THIS file
)

#link_directories(${MINI_XML_SOURCE})
#if(WIN32)
  set(SOURCE_FILE_PATHS
    ${SOURCE_FILE_PATHS} 
    ${MINI_XML_SOURCE_FILE_PATHS}
    PARENT_SCOPE #to propagate changes to file that included THIS file
    )

#endif(WIN32)
