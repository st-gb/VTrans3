
set(MINI_XML_SOURCE_FILES
  ${VTRANS3_ROOT_PATH}/IO/MiniXML/MiniXMLconfigReader.cpp
  ${VTRANS3_ROOT_PATH}/IO/MiniXML/GrammarRuleFileReader.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/MainConfigFileReader.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/TranslationRuleFileReader.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/TranslationRuleFile_EndXMLelement.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/VocAttributeDefintionHandler.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/MiniXMLconfigReader.cpp
)

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

else ( DEFINED USE_MINI_XML_STATIC_LIBRARY)
set(MINI_XML_LIBRARY_SOURCE_FILE_PATHS
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
  )
message("in MiniXML CMakeLists:" ${MINI_XML_LIBRARY_SOURCE_FILE_PATHS} )
endif(DEFINED USE_MINI_XML_STATIC_LIBRARY)

set( CONFIGURATION_READER_SOURCE_FILES
  ${MINI_XML_SOURCE_FILES}
  ${MINI_XML_LIBRARY_SOURCE_FILE_PATHS}
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
