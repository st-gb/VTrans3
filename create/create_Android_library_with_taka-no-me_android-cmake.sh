#!/bin/sh

echo works with CMake toolchain file from https://github.com/taka-no-me/android-cmake

numberOfArguments=$#
NUM_MIN_ARGS_NEEDED=3
NUM_MAX_ARGS_NEEDED=5

DIRECTORY_OF_THIS_SHELL_SCRIPT=$(dirname "$0")
echo "directory of this shell script: $DIRECTORY_OF_THIS_SHELL_SCRIPT"

if [ $numberOfArguments -ge $NUM_MIN_ARGS_NEEDED ]; then
  ANDROID_NDK_PATH=$1
  echo "Android Native Development Kit (NDK) path:" $ANDROID_NDK_PATH
  ANDROID_CMAKE_DIR=$2
  echo "(taka-no-me) android-cmake path:" $ANDROID_CMAKE_DIR
  MiniXMLsourcePath=$3
  echo "MiniXMLsourcePath:" $MiniXMLsourcePath

if [ $numberOfArguments -ge $NUM_MAX_ARGS_NEEDED ]; then
  DiscordiaAndroidCmakePath=$4
  echo "Discordia Android CMake path:" $DiscordiaAndroidCmakePath
  ANDROID_DIR=$5
  echo "Android APK root path:" $ANDROID_DIR
fi

#echo changing to directory %ANDROID_CMAKE_DIR%
#cd %ANDROID_CMAKE_DIR%
#echo current working directory: %cd%

#parts of NetBeans config: -DCMAKE_C_COMPILER=${IDE_CC} -DCMAKE_CXX_COMPILER=${IDE_CXX}
#to specify build dir some of these options may work: ../build -B../build -DCMAKE_BINARY_DIR=../build --build ../build -DCMAKE_CURRENT_BINARY_DIRECTORY=/home/sg/SourceCodeManagement/VTrans3/bin

#Edit the following variables if needed!

#If using a non-absolute path when setting the C Compiler causes this: http://public.kitware.com/Bug/view.php?id=13756
#So use an absolute path, as http://public.kitware.com/Bug/view.php?id=14294 proposes
#C_COMPILER=/usr/bin/gcc
#CPP_COMPILER=/usr/bin/g++

#IDEA: pass as args: <NAME=VALUE>, e.g. COMMON_SOURCECODE_ROOT_PATH=../common_sourcecode

# "-pg" (DCMAKE_CXX_FLAGS) option for "gprof" profiling

#ANDROID_ABI: "armeabi-v7a with NEON", "x86"
#cd src
#use "-O3" in DCMAKE_CXX_FLAGS to optimize (for speed)
#from https://github.com/taka-no-me/android-cmake
#-G "Ninja" \
cmake $DIRECTORY_OF_THIS_SHELL_SCRIPT/../src \
-DCMAKE_TOOLCHAIN_FILE=$ANDROID_CMAKE_DIR/android.toolchain.cmake \
-DANDROID_NDK=$ANDROID_NDK_PATH \
-DCMAKE_BUILD_TYPE=Release \
-DANDROID_NO_UNDEFINED=ON \
-DANDROID_ABI="x86"  \
-DDISCORDIA_ANDROID_CMAKE_HOME=$DiscordiaAndroidCmakePath \
-DANDROID_DIR=$ANDROID_DIR \
-DMINI_XML_SOURCE=$MiniXMLsourcePath \
-DANDROID_API_LEVEL=15 \
-DEXE_TYPE:STRING=Android
else
  echo "error: at least" $NUM_MIN_ARGS_NEEDED "command line arguments are needed:"
  echo "\"Android NDK root path\" \"(taka-no-me) android-cmake path\" \"Mini XML source path\""
  echo "possible additional 2 arguments \"Android APK root path\" \"Discordia Android CMake path\""
  echo "Android NDK root path: e.g. \"/devel/android-ndk-r10d\""
  echo "Android APK root path : path where the \"asset\", \"libs\" etc. directories reside"
fi
