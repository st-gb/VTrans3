set( XERCES_SOURCE_FILES
  #${MINI_XML_SOURCE_FILES}
  ${VTRANS3_ROOT_PATH}/Xerces/ReadViaSAX2.cpp
  ${VTRANS3_ROOT_PATH}/Xerces/SAX2configReader.cpp
  ${VTRANS3_ROOT_PATH}/Xerces/SAX2GrammarRuleHandler.cpp
  ${VTRANS3_ROOT_PATH}/Xerces/SAX2MainConfigHandler.cpp
  ${VTRANS3_ROOT_PATH}/Xerces/SAX2TransformationRuleHandler.cpp
  ${VTRANS3_ROOT_PATH}/Xerces/SAX2TranslationRuleHandler.cpp
  ${VTRANS3_ROOT_PATH}/Xerces/SAX2VocAttributeDefintionHandler.cpp
  ${COMMON_SOURCECODE_ROOT_PATH}/Xerces/XercesAttributesHelper.cpp
  ${COMMON_SOURCECODE_ROOT_PATH}/Xerces/XercesString.cpp
  ${COMMON_SOURCECODE_ROOT_PATH}/Xerces/ConvertXercesStringToStdWstring.cpp
  PARENT_SCOPE
)

set( LIBRARIES
  xerces-c
  PARENT_SCOPE
)
