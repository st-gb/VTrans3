@REM suppress output of commands with the following line
@echo off
echo pass file paths with slashes (/) instead of backslashes (\) for MinGW/MSYS
@REM https://stackoverflow.com/questions/1291941/batch-files-number-of-command-line-arguments
@REM set numCmdLineArgs=0
@REM for %%x in (%*) do Set /A numCmdLineArgs+=1


IF "%~1"=="" GOTO CALL_CMAKE
  echo "pass Mini XML source dir as 1st command line parameter"
  @Set MiniXMLsourcePath=%1
  @echo MiniXMLsourcePath: %MiniXMLsourcePath%

:CALL_CMAKE
@Set MiniXMLsourcePath=%1

@REM see/from https://stackoverflow.com/questions/69068/long-commands-split-over-multiple-lines-in-windows-vista-batch-bat-file
cmake -G "MSYS Makefiles" ^
-DCMAKE_BUILD_TYPE=Debug ^
-DCMAKE_C_COMPILER=gcc ^
-DCMAKE_CXX_COMPILER=g++ ^
-DCMAKE_C_FLAGS_DEBUG="-g3 -O0" ^
-DCMAKE_CXX_FLAGS_DEBUG="-g3 -O0" ^
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
-DCMAKE_CXX_FLAGS="-g3 -O0" ^
-DCOMPILE_WITH_OPENMP=NO ^
-DEVALUATE_PROCESSING=YES ^
-DMINI_XML_SOURCE:STRING=%MiniXMLsourcePath% ^
-DEXE_TYPE:STRING=dynlib ^
./src"
