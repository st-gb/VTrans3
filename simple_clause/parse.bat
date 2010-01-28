REM nicht zeigen (simple_clause_c_pp.txt wird bei Änderungen automatisch kopiert noch goexchange.de), 
dass das eine ypp-Datei ist.

ren simple_clause_c_pp.txt simple_clause_c_pp.ypp
C:\Program Files (x86)\GnuWin32\bin\bison.exe simple_clause_c_pp.ypp
ren simple_clause_c_pp.ypp simple_clause_c_pp.txt

REM bison bennent die Dateien falsch. Deshalb gibt es ohne Umbennen
REM den Compiler-Fehler: kann Datei "location.hh" nicht finden in der 
REM ...tab.hpp-Datei
REM die evtl. existierende Datei muss geloescht werden, sonst Fehler beim Umbenennen

REM del location.hh
REM ren b4_dir_prefixlocation.hh location.hh

REM die evtl. existierende Datei muss geloescht werden, sonst Fehler beim Umbenennen
REM del position.hh
REM ren b4_dir_prefixposition.hh position.hh