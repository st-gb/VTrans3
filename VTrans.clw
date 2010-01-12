; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=EnglishView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "vtrans.h"
LastPage=0

ClassCount=12
Class1=CChildFrame
Class2=EnglishView
Class3=CMainFrame
Class4=COptionsDialog
Class5=CMyBar
Class6=CSizingControlBar
Class7=StatusDialog
Class8=CVTransApp
Class9=CAboutDlg
Class10=CVTransDoc
Class11=CVTransView

ResourceCount=6
Resource1=IDR_MAINFRAME
Resource2=IDR_VTRANSTYPE
Resource3=IDD_ABOUTBOX
Resource4=IDD_OPTIONS_DIALOG
Resource5=IDD_STATUS_DIALOG
Class12=CLoadStatusDialog
Resource6=IDD_FILE_OPEN_DIALOG

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
LastObject=CChildFrame
Filter=M
VirtualFilter=mfWC

[CLS:EnglishView]
Type=0
BaseClass=CView
HeaderFile=EnglishView.h
ImplementationFile=EnglishView.cpp
LastObject=EnglishView
Filter=C
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame
Filter=T
VirtualFilter=fWC

[CLS:COptionsDialog]
Type=0
BaseClass=CDialog
HeaderFile=OptionsDialog.h
ImplementationFile=OptionsDialog.cpp
Filter=D
VirtualFilter=dWC
LastObject=COptionsDialog

[CLS:CMyBar]
Type=0
HeaderFile=SearchFormBar.h
ImplementationFile=VocableEditorBar.cpp

[CLS:CSizingControlBar]
Type=0
BaseClass=baseCSizingControlBar
HeaderFile=sizecbar.h
ImplementationFile=sizecbar.cpp
LastObject=CSizingControlBar

[CLS:StatusDialog]
Type=0
BaseClass=CDialog
HeaderFile=StatusDialog.h
ImplementationFile=StatusDialog.cpp

[CLS:CVTransApp]
Type=0
BaseClass=CWinApp
HeaderFile=VTrans.h
ImplementationFile=VTrans.cpp
LastObject=CVTransApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=VTrans.cpp
ImplementationFile=VTrans.cpp

[CLS:CVTransDoc]
Type=0
BaseClass=CDocument
HeaderFile=VTransDoc.h
ImplementationFile=VTransDoc.cpp
LastObject=CVTransDoc

[CLS:CVTransView]
Type=0
BaseClass=CView
HeaderFile=VTransView.h
ImplementationFile=VTransView.cpp
LastObject=CVTransView

[DLG:IDD_OPTIONS_DIALOG]
Type=1
Class=COptionsDialog
ControlCount=46
Control1=IDC_CHECK1,button,1342242819
Control2=IDC_ENSURE_USER_WANTS_MODIFICATION_CHECK,button,1342242819
Control3=IDC_ENSURE_USER_WANTS_DELETION_CHECK,button,1342242819
Control4=IDC_CASE_SENSITIVE_CHECK,button,1342242819
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_MAX_CHARS_PER_LINE_EDIT,edit,1350631552
Control8=IDOK,button,1342242817
Control9=IDCANCEL,button,1342242816
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_CLAUSE_PROGRESS_MODE_GRAPHIC_CHECK,button,1342242819
Control13=IDC_CLAUSE_PROGRESS_MODE_NUMERIC_CHECK,button,1342242819
Control14=IDC_PROGNOSTICED_REMAINING_TIME_CHECK,button,1342242819
Control15=IDC_SENTENCE_PROGRESS_MODE_GRAPHIC_CHECK,button,1342242819
Control16=IDC_SENTENCE_PROGRESS_MODE_NUMERIC_CHECK,button,1342242819
Control17=IDC_TEXT_PROGRESS_MODE_GRAPHIC_CHECK,button,1342242819
Control18=IDC_TEXT_PROGRESS_MODE_NUMERIC_CHECK,button,1342242819
Control19=IDC_BUFFER_OUTPUT_CHECK,button,1342251011
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,button,1342177287
Control23=IDC_ROUGH_GRAPHIC_CLAUSE_PROGRESS_RADIO,button,1342308361
Control24=IDC_SMOOTH_GRAPHIC_CLAUSE_PROGRESS_RADIO,button,1342177289
Control25=IDC_ROUGH_GRAPHIC_SENTENCE_PROGRESS_RADIO,button,1342308361
Control26=IDC_SMOOTH_GRAPHIC_SENTENCE_PROGRESS_RADIO,button,1342177289
Control27=IDC_ROUGH_GRAPHIC_TEXT_PROGRESS_RADIO,button,1342308361
Control28=IDC_SMOOTH_GRAPHIC_TEXT_PROGRESS_RADIO,button,1342177289
Control29=IDC_THREAD_PRIORITY_SLIDER,msctls_trackbar32,1342242817
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,button,1342177287
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,static,1342308352
Control36=IDC_STATIC,button,1342177287
Control37=IDC_ALLOW_LOW_LETTER_AT_THE_BEGINNING_OF_SENTENCE_CHECK,button,1342242819
Control38=IDC_ADOPT_GRAMMATICALLY_INCORRECT_SENTENCES_CHECK,button,1342242819
Control39=IDC_AUTOMATIC_FORMATTING_CHECK,button,1342242819
Control40=IDC_STATIC,static,1342177294
Control41=IDC_STATIC,static,1342177294
Control42=IDC_STATIC,static,1342177294
Control43=IDC_STATIC,static,1342177294
Control44=IDC_STATIC,static,1342177294
Control45=IDC_CHECK2,button,1342242819
Control46=IDC_CHECK3,button,1342242819

[DLG:IDD_STATUS_DIALOG]
Type=1
Class=StatusDialog
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_AMOUNT_STATIC,static,1342308352
Control6=IDC_PARSING_PROGRESS,msctls_progress32,1350565888
Control7=IDC_SENTENCE_PROGRESS,msctls_progress32,1350565888
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_TEXT_PROGRESS,msctls_progress32,1350565888
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=17
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_HOMEPAGE_EDIT,edit,1350633600
Control8=IDC_STATIC,static,1342308352
Control9=IDC_REGISTRY_KEY_EDIT,edit,1350633600
Control10=IDC_EMAIL_EDIT,edit,1350633600
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_CONFIGURATION_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352

[MNU:IDR_VTRANSTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_BEARBEITEN_LSCHEN
Command16=ID_EDIT_MARK_ALL
Command17=ID_VIEW_TOOLBAR
Command18=ID_VIEW_STATUS_BAR
Command19=ID_VIEW_VOCABLEEDITOR
Command20=ID_VIEW_SEARCH_FORM
Command21=ID_EXTRAS_COUNT_WORDS
Command22=ID_EXTRAS_OPTIONEN
Command23=ID_WINDOW_NEW
Command24=ID_WINDOW_CASCADE
Command25=ID_WINDOW_TILE_HORZ
Command26=ID_WINDOW_ARRANGE
Command27=ID_HELP_FINDER
Command28=ID_APP_ABOUT
CommandCount=28

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_VIEW_VOCABLEEDITOR
Command9=ID_EXTRAS_COUNT_WORDS
Command10=ID_EXTRAS_OPTIONEN
Command11=ID_HELP_FINDER
Command12=ID_APP_ABOUT
CommandCount=12

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_VIEW_VOCABLEEDITOR
Command9=ID_TRANSLATE_BUTTON
Command10=ID_TRANSLATE_AUTOMATICALLY_IF_TEXTMODIFICATION_BUTTON
Command11=ID_ADD_VOCABLE_BUTTON
Command12=ID_OVERWRITE_VOCABLE_BUTTON
Command13=ID_DELETE_VOCABLE_BUTTON
Command14=ID_PREVIOUS_VOCABLE_BUTTON
Command15=ID_NEXT_VOCABLE_BUTTON
Command16=ID_COMBOBOX_BUTTON
Command17=ID_APP_ABOUT
Command18=ID_CONTEXT_HELP
CommandCount=18

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_MARK_ALL
Command2=ID_EDIT_COPY
Command3=ID_FILE_NEW
Command4=ID_FILE_OPEN
Command5=ID_FILE_PRINT
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_HELP
Command11=ID_CONTEXT_HELP
Command12=ID_TRANSLATE_BUTTON
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_EDIT_COPY
Command16=ID_EDIT_PASTE
Command17=ID_EDIT_CUT
Command18=ID_EDIT_UNDO
CommandCount=18

[DLG:IDD_FILE_OPEN_DIALOG]
Type=1
Class=CLoadStatusDialog
ControlCount=7
Control1=IDCANCEL,button,1342242816
Control2=IDC_LOAD_PROGRESS,msctls_progress32,1350565889
Control3=IDC_ACTION_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_BYTES_INSERTED_STATIC,static,1342308352
Control7=IDC_BYTES_TOTAL_STATIC,static,1342308352

[CLS:CLoadStatusDialog]
Type=0
HeaderFile=LoadStatusDialog.h
ImplementationFile=LoadStatusDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CLoadStatusDialog
VirtualFilter=dWC

