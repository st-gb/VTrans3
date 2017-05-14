
 #see http://accountingplusplus.blogspot.de/2010/12/cmake-wxwidgets.html :
 #Set wxWidgets_USE_DEBUG to "ON" to link against WxWidgets debug libraries.
 set( wxWidgets_USE_DEBUG ON
  PARENT_SCOPE #To propagate changes to file that included THIS file
  )
 set(WXWIDGETS ON 
    PARENT_SCOPE #To propagate changes to file that included THIS file
  )
message("wxWidgets_USE_DEBUG: ${wxWidgets_USE_DEBUG}")

set( CXX_DEFINITIONS
  ${CXX_DEFINITIONS}
  -DCOMPILE_AS_GUI
# Must not use "PARENT_SCOPE" HERE, else this definition and the one at the end
#  of this file are not both propagated to the file that includes this file
#  (reason for this is unknown)
#  PARENT_SCOPE #To propagate changes to file that included THIS file
  )

#see http://stackoverflow.com/questions/8934295/add-source-in-a-subdirectory-to-a-cmake-project
set(SOURCE_FILE_PATHS
  ${SOURCE_FILE_PATHS}
	${VTRANS3_ROOT_PATH}/Translate/TestIfTranslationRuleApplies.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/VTransApp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/DictionaryStatisticsWindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/DrawGrammarPartAttributes.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/DrawParseTreeTraverser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/RulesListCtrl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/ShowRulesDialogBase.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/SettingsDialog.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/UserInterface.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/wxGermanTranslationPanel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/wxParseTreePanel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/MainWindow/EventHandlerFunctions.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/MainWindow/MainFrame.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/MainWindow/MainFrame_AddInOutCtrls.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/MainWindow/wxTextInputDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/MainWindow/MainWindowBase.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/ParseRules/ParseRulesListCtrl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/ParseRules/ShowParseRulesDialog.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/TranslationRules/ShowTranslationRulesDialog.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/TranslationRules/TranslationRulesListCtrl.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/wxWidgets/Controller/character_string/wxStringHelper.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/wxWidgets/UserInterface/LogEntries/LogEntriesDialog.cpp
	${COMMON_SOURCECODE_ROOT_PATH}/wxWidgets/UserInterface/LogEntries/LogEntriesListCtrl.cpp
  PARENT_SCOPE #To propagate changes to file that included THIS file
)

if( NOT DEFINED WXWIDGETS_SETUP_H_PATH )
  message("warning: WXWIDGETS_SETUP_H_PATH is NOT defined->compiling will fail"
   "on Linux it may be like: /usr/lib/i386-linux-gnu/wx/include/gtk2-unicode-debug-2.8")
endif( NOT DEFINED WXWIDGETS_SETUP_H_PATH )
if( NOT DEFINED WXWIDGETS_INCLUDE_DIR )
  message("warning: WXWIDGETS_INCLUDE_DIR is NOT defined->compiling will fail"
   "on Linux it may be like: /usr/include/wx-2.8")
endif( NOT DEFINED WXWIDGETS_INCLUDE_DIR )

set(INCLUDE_DIRS
  ${INCLUDE_DIRS}
  ${WXWIDGETS_SETUP_H_PATH}
  ${WXWIDGETS_INCLUDE_DIR}
  PARENT_SCOPE #To propagate changes to file that included THIS file
)

#include_directories( ${INCLUDE_DIRS} )

#https://wiki.wxwidgets.org/CMake
#set(wxWidgets_CONFIGURATION mswu)
#find_package(wxWidgets COMPONENTS core base REQUIRED)
#include(${wxWidgets_USE_FILE})

set( LIBRARIES
  ${LIBRARIES}
  ${wxWidgets_USE_FILE}
  PARENT_SCOPE #To propagate changes to file that included THIS file
)

if (WIN32)
  add_definitions(-D__WXMSW__)
  set(CXX_DEFINITIONS
    ${CXX_DEFINITIONS}
    -D__WXMSW__
    PARENT_SCOPE #To propagate changes to file that included THIS file
  )
endif (WIN32)

if (UNIX)
  #add_definitions(-D__WXGTK__)
  set(CXX_DEFINITIONS
    ${CXX_DEFINITIONS}
    -D__WXGTK__
    PARENT_SCOPE #To propagate changes to file that included THIS file
  )
endif (UNIX)

#add_executable(VTrans ${SOURCE_FILE_PATHS})
