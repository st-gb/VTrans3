#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc-4.exe
CCC=g++-4.exe
CXX=g++-4.exe
FC=
AS=

# Macros
CND_PLATFORM=Cygwin_4.x-Windows
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate/TranslateParseByRiseTree.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/IO.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/I_WordSearch.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/VocabularyAndTranslation.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate/SyntaxTreePath.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate/TranslationRule.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterTree.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets/wxTextInputDlg.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/IVocabularyInMainMem.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets/VTransApp.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordNode.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Word.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/StdAfx.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets/wxParseTreePanel.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterNode.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/allWordsIn1SingleLine.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Parse/ParseByRise.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L${WX_CYG_ANSI_DBG_STAT_LIB_PATH} -lwx_mswd_core-2.8 -lwx_mswd_adv-2.8 -lwx_based-2.8 -lgdi32 -lole32 -lcomdlg32 -lcomctl32 -luuid -loleaut32

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/Cygwin_4.x-Windows/vtrans3.exe

dist/Debug/Cygwin_4.x-Windows/vtrans3.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/Cygwin_4.x-Windows
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vtrans3 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate/TranslateParseByRiseTree.o: nbproject/Makefile-${CND_CONF}.mk ../../../Translate/TranslateParseByRiseTree.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate/TranslateParseByRiseTree.o ../../../Translate/TranslateParseByRiseTree.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/IO.o: nbproject/Makefile-${CND_CONF}.mk ../../../IO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/IO.o ../../../IO.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/I_WordSearch.o: nbproject/Makefile-${CND_CONF}.mk ../../../VocabularyInMainMem/I_WordSearch.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/I_WordSearch.o ../../../VocabularyInMainMem/I_WordSearch.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/VocabularyAndTranslation.o: nbproject/Makefile-${CND_CONF}.mk ../../../VocabularyInMainMem/LetterTree/VocabularyAndTranslation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/VocabularyAndTranslation.o ../../../VocabularyInMainMem/LetterTree/VocabularyAndTranslation.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate/SyntaxTreePath.o: nbproject/Makefile-${CND_CONF}.mk ../../../Translate/SyntaxTreePath.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate/SyntaxTreePath.o ../../../Translate/SyntaxTreePath.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate/TranslationRule.o: nbproject/Makefile-${CND_CONF}.mk ../../../Translate/TranslationRule.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Translate/TranslationRule.o ../../../Translate/TranslationRule.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterTree.o: nbproject/Makefile-${CND_CONF}.mk ../../../VocabularyInMainMem/LetterTree/LetterTree.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterTree.o ../../../VocabularyInMainMem/LetterTree/LetterTree.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets/wxTextInputDlg.o: nbproject/Makefile-${CND_CONF}.mk ../../../wxWidgets/wxTextInputDlg.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets/wxTextInputDlg.o ../../../wxWidgets/wxTextInputDlg.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/IVocabularyInMainMem.o: nbproject/Makefile-${CND_CONF}.mk ../../../VocabularyInMainMem/IVocabularyInMainMem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/IVocabularyInMainMem.o ../../../VocabularyInMainMem/IVocabularyInMainMem.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets/VTransApp.o: nbproject/Makefile-${CND_CONF}.mk ../../../wxWidgets/VTransApp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets/VTransApp.o ../../../wxWidgets/VTransApp.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordNode.o: nbproject/Makefile-${CND_CONF}.mk ../../../VocabularyInMainMem/DoublyLinkedList/WordNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordNode.o ../../../VocabularyInMainMem/DoublyLinkedList/WordNode.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Word.o: nbproject/Makefile-${CND_CONF}.mk ../../../Word.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Word.o ../../../Word.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/StdAfx.o: nbproject/Makefile-${CND_CONF}.mk ../../../StdAfx.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/StdAfx.o ../../../StdAfx.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets/wxParseTreePanel.o: nbproject/Makefile-${CND_CONF}.mk ../../../wxWidgets/wxParseTreePanel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/wxWidgets/wxParseTreePanel.o ../../../wxWidgets/wxParseTreePanel.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterNode.o: nbproject/Makefile-${CND_CONF}.mk ../../../VocabularyInMainMem/LetterTree/LetterNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterNode.o ../../../VocabularyInMainMem/LetterTree/LetterNode.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/allWordsIn1SingleLine.o: nbproject/Makefile-${CND_CONF}.mk ../../../allWordsIn1SingleLine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/allWordsIn1SingleLine.o ../../../allWordsIn1SingleLine.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Parse/ParseByRise.o: nbproject/Makefile-${CND_CONF}.mk ../../../Parse/ParseByRise.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Parse
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -D_DEBUG -DNOMINMAX -I../../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/Parse/ParseByRise.o ../../../Parse/ParseByRise.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/Cygwin_4.x-Windows/vtrans3.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
