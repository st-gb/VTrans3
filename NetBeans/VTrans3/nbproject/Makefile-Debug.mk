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
AS=as.exe

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
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Parse/sequential.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterNode.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/simple_clause/simple_clause_c_pp.tab.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordNode.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/wxWidgets/noname.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/ParseEnglish.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/GetAndTestWord.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/1LinePerWordPair.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Translate.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Text.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/wxWidgets/wxTextInputDlg.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/Translate.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordList.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO/writeXML.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Token.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/charsetconv.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/allWordsIn1SingleLine.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word_2003_08_Aug_14.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/xmlwriter.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/VocabularyAndTranslation.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/StdAfx.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterTree.o

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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/Cygwin_4.x-Windows/vtrans3.exe

dist/Debug/Cygwin_4.x-Windows/vtrans3.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/Cygwin_4.x-Windows
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vtrans3 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Parse/sequential.o: nbproject/Makefile-${CND_CONF}.mk ../../Parse/sequential.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Parse
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Parse/sequential.o ../../Parse/sequential.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterNode.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/LetterTree/LetterNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterNode.o ../../VocabularyInMainMem/LetterTree/LetterNode.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/simple_clause/simple_clause_c_pp.tab.o: nbproject/Makefile-${CND_CONF}.mk ../../simple_clause/simple_clause_c_pp.tab.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/simple_clause
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/simple_clause/simple_clause_c_pp.tab.o ../../simple_clause/simple_clause_c_pp.tab.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordNode.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/DoublyLinkedList/WordNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordNode.o ../../VocabularyInMainMem/DoublyLinkedList/WordNode.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/wxWidgets/noname.o: nbproject/Makefile-${CND_CONF}.mk ../../wxWidgets/noname.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/wxWidgets
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/wxWidgets/noname.o ../../wxWidgets/noname.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/ParseEnglish.o: nbproject/Makefile-${CND_CONF}.mk ../../ParseEnglish.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/ParseEnglish.o ../../ParseEnglish.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/GetAndTestWord.o: nbproject/Makefile-${CND_CONF}.mk ../../GetAndTestWord.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/GetAndTestWord.o ../../GetAndTestWord.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/1LinePerWordPair.o: nbproject/Makefile-${CND_CONF}.mk ../../1LinePerWordPair.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/1LinePerWordPair.o ../../1LinePerWordPair.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Translate.o: nbproject/Makefile-${CND_CONF}.mk ../../Translate.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Translate.o ../../Translate.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Text.o: nbproject/Makefile-${CND_CONF}.mk ../../Text.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Text.o ../../Text.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/wxWidgets/wxTextInputDlg.o: nbproject/Makefile-${CND_CONF}.mk ../../wxWidgets/wxTextInputDlg.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/wxWidgets
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/wxWidgets/wxTextInputDlg.o ../../wxWidgets/wxTextInputDlg.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/Translate.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/LetterTree/Translate.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/Translate.o ../../VocabularyInMainMem/LetterTree/Translate.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordList.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/DoublyLinkedList/WordList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordList.o ../../VocabularyInMainMem/DoublyLinkedList/WordList.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO/writeXML.o: nbproject/Makefile-${CND_CONF}.mk ../../IO/writeXML.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO/writeXML.o ../../IO/writeXML.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word.o: nbproject/Makefile-${CND_CONF}.mk ../../Word.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word.o ../../Word.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Token.o: nbproject/Makefile-${CND_CONF}.mk ../../Token.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Token.o ../../Token.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/charsetconv.o: nbproject/Makefile-${CND_CONF}.mk ../../charsetconv.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/charsetconv.o ../../charsetconv.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/allWordsIn1SingleLine.o: nbproject/Makefile-${CND_CONF}.mk ../../allWordsIn1SingleLine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/allWordsIn1SingleLine.o ../../allWordsIn1SingleLine.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO.o: nbproject/Makefile-${CND_CONF}.mk ../../IO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO.o ../../IO.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word_2003_08_Aug_14.o: nbproject/Makefile-${CND_CONF}.mk ../../Word_2003_08\(Aug\)14.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word_2003_08_Aug_14.o ../../Word_2003_08\(Aug\)14.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/xmlwriter.o: nbproject/Makefile-${CND_CONF}.mk ../../xmlwriter.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/xmlwriter.o ../../xmlwriter.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/VocabularyAndTranslation.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/LetterTree/VocabularyAndTranslation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/VocabularyAndTranslation.o ../../VocabularyInMainMem/LetterTree/VocabularyAndTranslation.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/StdAfx.o: nbproject/Makefile-${CND_CONF}.mk ../../StdAfx.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/StdAfx.o ../../StdAfx.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterTree.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/LetterTree/LetterTree.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -g -D__WXMSW__ -DNOMINMAX -D__cplusplus -D_DEBUG -D_WINDOWS -D__WINDOWS__ -I../.. -I${WXWIDGETS_CYGWIN_PATH}/include -I${WX_ANSI_DBG_STATIC_SETUP_H_CYG_PATH} -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterTree.o ../../VocabularyInMainMem/LetterTree/LetterTree.cpp

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
