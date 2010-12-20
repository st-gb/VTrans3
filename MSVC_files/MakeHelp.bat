@echo off
REM -- Zuerst Zuordnungsdatei der mit Microsoft Visual C++ erzeugten resource.h erstellen
echo // Von MAKEHELP.BAT erzeugte Hilfe-Zuordnungsdatei. Wird verwendet von VTRANS.HPJ. >"hlp\VTrans.hm"
echo. >>"hlp\VTrans.hm"
echo // Befehle (ID_* und IDM_*) >>"hlp\VTrans.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\VTrans.hm"
echo. >>"hlp\VTrans.hm"
echo // Eingabeaufforderungen (IDP_*) >>"hlp\VTrans.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\VTrans.hm"
echo. >>"hlp\VTrans.hm"
echo // Ressourcen (IDR_*) >>"hlp\VTrans.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\VTrans.hm"
echo. >>"hlp\VTrans.hm"
echo // Dialogfelder (IDD_*) >>"hlp\VTrans.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\VTrans.hm"
echo. >>"hlp\VTrans.hm"
echo // Rahmen-Steuerelemente (IDW_*) >>"hlp\VTrans.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\VTrans.hm"
REM -- Hilfe erstellen für Projekt VTRANS


echo Erstelle Win32-Hilfedateien
start /wait hcw /C /E /M "hlp\VTrans.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\VTrans.hlp" goto :Error
if not exist "hlp\VTrans.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\VTrans.hlp" Debug
if exist Debug\nul copy "hlp\VTrans.cnt" Debug
if exist Release\nul copy "hlp\VTrans.hlp" Release
if exist Release\nul copy "hlp\VTrans.cnt" Release
echo.
goto :done

:Error
echo hlp\VTrans.hpj(1) : Fehler: Problem beim Erstellen der Hilfedatei festgestellt

:done
echo.
