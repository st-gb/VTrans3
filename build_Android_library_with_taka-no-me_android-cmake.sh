#!/bin/sh

if [ "$1" = "" -o "$1" = " " ]; then
	echo "Error: no paths specified a command line parameters"
else

echo "param1: taka-no-me android-cmake directory"
echo "param2: Android NDK direcory"

#ARCHITECTURE=\"armeabi-v7a with NEON\"
#NATIVE_API_LEVEL=$1
ANDROID_CMAKE_PATH=$1

#delete CMakeCache.txt if following or similar error:
#make[1]: CMakeFiles/Makefile2: Datei oder Verzeichnis nicht gefunden
#make[1]: *** Keine Regel, um „CMakeFiles/Makefile2“ zu erstellen.  Schluss.

#see https://github.com/taka-no-me/android-cmake

ORIGINAL_PWD=`pwd`
echo "current working dir:" $ORIGINAL_PWD
echo "changing directory to" $ANDROID_CMAKE_PATH
cd $ANDROID_CMAKE_PATH

#      -ANDROID_STL=stlport_static \
cmake -DCMAKE_TOOLCHAIN_FILE=android.toolchain.cmake \
      -DANDROID_NDK=\"$2\" \
      -DCMAKE_BUILD_TYPE=Release                     \
      -DANDROID_ABI="armeabi-v7a with NEON"          \
      -DANDROID_NATIVE_API_LEVEL=15 \
-DCOMPILE_WITH_OPENMP=YES \
-DEVALUATE_PROCESSING=NO \
-DEXE_TYPE:STRING=Android \
-DMINI_XML_SOURCE=/home/stefan/Downloads/mxml-2.10 \
      $ORIGINAL_PWD/src
#cmake --build ../src 
make -f $ORIGINAL_PWD/src/Makefile -j4

fi
