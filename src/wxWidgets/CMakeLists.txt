
#see http://stackoverflow.com/questions/8934295/add-source-in-a-subdirectory-to-a-cmake-project

set(SOURCE_FILE_PATHS
  ${SOURCE_FILE_PATHS}
  ${CMAKE_CURRENT_SOURCE_DIR}/VTransApp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/DictionaryStatisticsWindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/DrawGrammarPartAttributes.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/DrawParseTreeTraverser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/RulesListCtrl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserInterface/ShowRulesDialogBase.cpp
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
  PARENT_SCOPE
)

set(INCLUDE_DIRS
  ${INCLUDE_DIRS}
  #/usr/lib/i386-linux-gnu/wx/include/gtk2-unicode-debug-2.8
  /usr/include/wx-2.8
  PARENT_SCOPE
)

#include_directories( ${INCLUDE_DIRS} )

#https://wiki.wxwidgets.org/CMake
#set(wxWidgets_CONFIGURATION mswu)
#find_package(wxWidgets COMPONENTS core base REQUIRED)
#include(${wxWidgets_USE_FILE})

set( LIBRARIES
  ${LIBRARIES}
  ${wxWidgets_USE_FILE}
  PARENT_SCOPE
)

if (WIN32)
  add_definitions(-D__WXMSW__)
  set(CXX_DEFINITIONS
    ${CXX_DEFINITIONS}
    -D__WXMSW__
    PARENT_SCOPE
  )
endif (WIN32)

if (UNIX)
  #add_definitions(-D__WXGTK__)
  set(CXX_DEFINITIONS
    ${CXX_DEFINITIONS}
    -D__WXGTK__
    PARENT_SCOPE
  )
  set( LIBRARIES
    ${LIBRARIES}
    #libwx_gtk2ud_core-2.8.a
    #wx_baseud-2.8
    #wx_gtk2ud_adv-2.8
    PARENT_SCOPE
  )
endif (UNIX)

#add_executable(VTrans ${SOURCE_FILE_PATHS})
