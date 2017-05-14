
#Use MiniXML for reading XML configuration files because:
#-I wasn't able to compile Apache Xerces for Android
#-no reported memory leaks by valgrind (in contrast to Apache Xerces)
add_subdirectory(MiniXML
  #EXCLUDE_FROM_ALL
  )
if( ANDROID_NDK )
else ( ANDROID_NDK )
  set( CXX_DEFINITIONS
    ${CXX_DEFINITIONS}
    -DCOMPILE_AS_EXECUTABLE
  )
endif( ANDROID_NDK )

set( CONFIGURATION_READER_SOURCE_FILES
  #${MINI_XML_SOURCE_FILES}
  ${CONFIGURATION_READER_SOURCE_FILES}
  ${VTRANS3_ROOT_PATH}/IO/configuration/MainConfigFileReaderBase.cpp
  ${VTRANS3_ROOT_PATH}/IO/rules/TranslationRuleFileReaderBase.cpp
	${VTRANS3_ROOT_PATH}/IO/GenerateXMLtreeFromParseTree.cpp
	${VTRANS3_ROOT_PATH}/IO/ParseTree2XMLtreeTraverser.cpp
  PARENT_SCOPE
)

set( LIBRARIES
  ${LIBRARIES}
  PARENT_SCOPE
)
