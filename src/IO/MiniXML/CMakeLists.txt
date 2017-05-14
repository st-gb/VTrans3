
set(MINI_XML_SOURCE_FILES
  ${VTRANS3_ROOT_PATH}/IO/MiniXML/MiniXMLconfigReader.cpp
  ${VTRANS3_ROOT_PATH}/IO/MiniXML/GrammarRuleFileReader.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/MainConfigFileReader.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/TranslationRuleFileReader.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/TranslationRuleFile_EndXMLelement.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/VocAttributeDefintionHandler.cpp
	${VTRANS3_ROOT_PATH}/IO/MiniXML/MiniXMLconfigReader.cpp
)

set( CONFIGURATION_READER_SOURCE_FILES
  ${MINI_XML_SOURCE_FILES}
  PARENT_SCOPE #to propagate changes to file that included THIS file
)

set( LIBRARIES
  mxml
  PARENT_SCOPE #to propagate changes to file that included THIS file
)
