#from https://cmake.org/cmake-tutorial/
cmake_minimum_required (VERSION 2.8)
project (ExeCallingVTransDynLib)

set(COMMON_SOURCECODE_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../common_sourcecode)
set(VTRANS_SOURCE_ROOT_PATH ../../..)

set( INCLUDE_DIRS 
  ${VTRANS_SOURCE_ROOT_PATH}
  ${COMMON_SOURCECODE_ROOT_PATH}
  )

include_directories( ${INCLUDE_DIRS} )

add_executable(${PROJECT_NAME} 
  ExeCallingVTransDynlib.cpp
  ${VTRANS_SOURCE_ROOT_PATH}/IO/dictionary/OpenDictFileException.cpp)
target_link_libraries(${PROJECT_NAME} dl)
