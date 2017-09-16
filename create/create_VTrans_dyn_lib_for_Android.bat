@echo off
echo works with CMake toolchain file from https://github.com/taka-no-me/android-cmake
echo and if executed from within NetBeans with working directory: "taka-no-me_android-cmake-master path"
echo and command line "path to this batch file" "taka-no-me_android-cmake-master path"

REM from https://stackoverflow.com/questions/1291941/batch-files-number-of-command-line-arguments
set numberOfArguments=0
set numberOfNeededArguments=4
for %%x in (%*) do Set /A numberOfArguments+=1
echo number of batch file arguments: %numberOfArguments%

IF %numberOfArguments% LSS %numberOfNeededArguments% (
  echo error: at least %numberOfNeededArguments% command line arguments are needed: 
  echo "Android NDK path" "(taka-no-me) android-cmake path" "Android APK root path" "Mini XML source path"
  echo "Android APK root path" : path where the "asset", "libs" etc. directories reside
  GOTO END
)
set ANDROID_NDK_PATH=%1
echo Android Native Development Kit path: %ANDROID_NDK_PATH%
set ANDROID_CMAKE_DIR=%2
echo (taka-no-me) android-cmake path: %ANDROID_CMAKE_DIR%
set ANDROID_DIR=%3
echo "Android APK root path": %ANDROID_DIR%
Set MiniXMLsourcePath=%4
echo MiniXMLsourcePath: %MiniXMLsourcePath%
REM set BUILD_DIR=%2
REm set ANDROID_DIR=C:/Users/Tillmann/SourceCodeManagement/VTransdroid
REM echo changing to directory %ANDROID_CMAKE_DIR%
REM cd %ANDROID_CMAKE_DIR%
echo current working directory: %cd%
REM cmake -G "MinGW Makefiles" ^
cmake -G "Unix Makefiles" ^
-DCMAKE_TOOLCHAIN_FILE=%ANDROID_CMAKE_DIR%/android.toolchain.cmake ^
-DANDROID_NDK=%ANDROID_NDK_PATH% ^
-DCMAKE_BUILD_TYPE=Release ^
-DANDROID_ABI="armeabi-v7a with NEON" ^
-DEXE_TYPE:STRING=Android ^
-DANDROID_API_LEVEL=15 ^
-DMINI_XML_SOURCE=%MiniXMLsourcePath% ^
-DDISCORDIA_ANDROID_CMAKE_HOME=C:/Users/Tillmann/SourceCodeManagement/github.com_Discordia_android-cmake ^
-DANDROID_DIR=%ANDROID_DIR% ^
src
REM C:\Users\Tillmann\SourceCodeManagement\VTrans3\src
@REM cmake --build C:\Users\Tillmann\SourceCodeManagement\VTrans3\src
REM cmake --build %ANDROID_DIR%/libs
REM mingw32-make -f Makefile -j
:END