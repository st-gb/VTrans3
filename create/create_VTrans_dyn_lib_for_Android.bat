@echo off
echo works with CMake toolchain file from https://github.com/taka-no-me/android-cmake
echo and if executed from within NetBeans with working directory: "taka-no-me_android-cmake-master path"
echo and command line "path to this batch file" "taka-no-me_android-cmake-master path"
set ANDROID_CMAKE_DIR=%1
set BUILD_DIR=%2
echo changing to directory %ANDROID_CMAKE_DIR%
echo current working directory: %cd%
cd %ANDROID_CMAKE_DIR%
REM cmake -G "MinGW Makefiles" ^
cmake -G "Unix Makefiles" ^
-DCMAKE_TOOLCHAIN_FILE=android.toolchain.cmake ^
-DANDROID_NDK=E:\Android_NDK\android-ndk-r15c ^
-DCMAKE_BUILD_TYPE=Release ^
-DANDROID_ABI="armeabi-v7a with NEON" ^
-DEXE_TYPE:STRING=Android ^
-DANDROID_API_LEVEL=15 ^
-DMINI_XML_SOURCE=E:/MiniXML/mxml-release-2.10 ^
C:\Users\Tillmann\SourceCodeManagement\VTrans3\src
@REM cmake --build C:\Users\Tillmann\SourceCodeManagement\VTrans3\src
cmake --build .
REM mingw32-make -f Makefile -j