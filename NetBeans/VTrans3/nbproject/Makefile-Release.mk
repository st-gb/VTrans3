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
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/StatusDialog.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/MainFrm.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Parse/sequential.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterNode.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/VTransView.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/WordEditorBar.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/simple_clause/simple_clause_c_pp.tab.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordNode.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/ParseEnglish.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/GetAndTestWord.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/ErrorCodeFromGetLastErrorToString.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/1LinePerWordPair.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/UnknownWordsBar.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/sizecbar.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/VTransDoc.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Translate.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Text.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/WordDockingWindow.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/VTrans.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/Translate.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordList.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO/writeXML.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/SplitterFrame.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/GermanView.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Token.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/LoadStatusDialog.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/SearchFormBar.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/OptionsDialog.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/charsetconv.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/allWordsIn1SingleLine.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word_2003_08_Aug_14.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/xmlwriter.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/VocabularyAndTranslation.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/StdAfx.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterTree.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/EnglishView.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/ChildFrm.o

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
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/Cygwin_4.x-Windows/vtrans3.exe

dist/Release/Cygwin_4.x-Windows/vtrans3.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Release/Cygwin_4.x-Windows
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vtrans3 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/StatusDialog.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/StatusDialog.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/StatusDialog.o ../../MFC/StatusDialog.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/MainFrm.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/MainFrm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/MainFrm.o ../../MFC/MainFrm.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Parse/sequential.o: nbproject/Makefile-${CND_CONF}.mk ../../Parse/sequential.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Parse
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Parse/sequential.o ../../Parse/sequential.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterNode.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/LetterTree/LetterNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterNode.o ../../VocabularyInMainMem/LetterTree/LetterNode.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/VTransView.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/VTransView.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/VTransView.o ../../MFC/VTransView.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/WordEditorBar.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/WordEditorBar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/WordEditorBar.o ../../MFC/WordEditorBar.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/simple_clause/simple_clause_c_pp.tab.o: nbproject/Makefile-${CND_CONF}.mk ../../simple_clause/simple_clause_c_pp.tab.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/simple_clause
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/simple_clause/simple_clause_c_pp.tab.o ../../simple_clause/simple_clause_c_pp.tab.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordNode.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/DoublyLinkedList/WordNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordNode.o ../../VocabularyInMainMem/DoublyLinkedList/WordNode.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/ParseEnglish.o: nbproject/Makefile-${CND_CONF}.mk ../../ParseEnglish.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/ParseEnglish.o ../../ParseEnglish.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/GetAndTestWord.o: nbproject/Makefile-${CND_CONF}.mk ../../GetAndTestWord.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/GetAndTestWord.o ../../GetAndTestWord.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/ErrorCodeFromGetLastErrorToString.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/ErrorCodeFromGetLastErrorToString.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/ErrorCodeFromGetLastErrorToString.o ../../MFC/ErrorCodeFromGetLastErrorToString.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/1LinePerWordPair.o: nbproject/Makefile-${CND_CONF}.mk ../../1LinePerWordPair.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/1LinePerWordPair.o ../../1LinePerWordPair.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/UnknownWordsBar.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/UnknownWordsBar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/UnknownWordsBar.o ../../MFC/UnknownWordsBar.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/sizecbar.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/sizecbar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/sizecbar.o ../../MFC/sizecbar.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/VTransDoc.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/VTransDoc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/VTransDoc.o ../../MFC/VTransDoc.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Translate.o: nbproject/Makefile-${CND_CONF}.mk ../../Translate.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Translate.o ../../Translate.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Text.o: nbproject/Makefile-${CND_CONF}.mk ../../Text.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Text.o ../../Text.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/WordDockingWindow.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/WordDockingWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/WordDockingWindow.o ../../MFC/WordDockingWindow.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/VTrans.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/VTrans.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/VTrans.o ../../MFC/VTrans.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/Translate.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/LetterTree/Translate.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/Translate.o ../../VocabularyInMainMem/LetterTree/Translate.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordList.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/DoublyLinkedList/WordList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/DoublyLinkedList/WordList.o ../../VocabularyInMainMem/DoublyLinkedList/WordList.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word.o: nbproject/Makefile-${CND_CONF}.mk ../../Word.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word.o ../../Word.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO/writeXML.o: nbproject/Makefile-${CND_CONF}.mk ../../IO/writeXML.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO/writeXML.o ../../IO/writeXML.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/SplitterFrame.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/SplitterFrame.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/SplitterFrame.o ../../MFC/SplitterFrame.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/GermanView.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/GermanView.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/GermanView.o ../../MFC/GermanView.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Token.o: nbproject/Makefile-${CND_CONF}.mk ../../Token.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Token.o ../../Token.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/LoadStatusDialog.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/LoadStatusDialog.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/LoadStatusDialog.o ../../MFC/LoadStatusDialog.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/SearchFormBar.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/SearchFormBar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/SearchFormBar.o ../../MFC/SearchFormBar.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/OptionsDialog.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/OptionsDialog.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/OptionsDialog.o ../../MFC/OptionsDialog.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/charsetconv.o: nbproject/Makefile-${CND_CONF}.mk ../../charsetconv.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/charsetconv.o ../../charsetconv.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/allWordsIn1SingleLine.o: nbproject/Makefile-${CND_CONF}.mk ../../allWordsIn1SingleLine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/allWordsIn1SingleLine.o ../../allWordsIn1SingleLine.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO.o: nbproject/Makefile-${CND_CONF}.mk ../../IO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/IO.o ../../IO.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word_2003_08_Aug_14.o: nbproject/Makefile-${CND_CONF}.mk ../../Word_2003_08\(Aug\)14.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Word_2003_08_Aug_14.o ../../Word_2003_08\(Aug\)14.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/xmlwriter.o: nbproject/Makefile-${CND_CONF}.mk ../../xmlwriter.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/xmlwriter.o ../../xmlwriter.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/VocabularyAndTranslation.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/LetterTree/VocabularyAndTranslation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/VocabularyAndTranslation.o ../../VocabularyInMainMem/LetterTree/VocabularyAndTranslation.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/StdAfx.o: nbproject/Makefile-${CND_CONF}.mk ../../StdAfx.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/StdAfx.o ../../StdAfx.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterTree.o: nbproject/Makefile-${CND_CONF}.mk ../../VocabularyInMainMem/LetterTree/LetterTree.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/VocabularyInMainMem/LetterTree/LetterTree.o ../../VocabularyInMainMem/LetterTree/LetterTree.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/EnglishView.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/EnglishView.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/EnglishView.o ../../MFC/EnglishView.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/ChildFrm.o: nbproject/Makefile-${CND_CONF}.mk ../../MFC/ChildFrm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/MFC/ChildFrm.o ../../MFC/ChildFrm.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/Cygwin_4.x-Windows/vtrans3.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
