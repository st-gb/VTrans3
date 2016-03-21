
add_subdirectory(MiniXML
  #EXCLUDE_FROM_ALL
  )

set( CONFIGURATION_READER_SOURCE_FILES
  #${MINI_XML_SOURCE_FILES}
  ${CONFIGURATION_READER_SOURCE_FILES}
  ${VTRANS3_ROOT_PATH}/IO/MiniXMLconfigReader.cpp
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
