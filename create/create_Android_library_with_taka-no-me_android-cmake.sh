#parts of NetBeans config: -DCMAKE_C_COMPILER=${IDE_CC} -DCMAKE_CXX_COMPILER=${IDE_CXX}
#to specify build dir some of these options may work: ../build -B../build -DCMAKE_BINARY_DIR=../build --build ../build -DCMAKE_CURRENT_BINARY_DIRECTORY=/home/sg/SourceCodeManagement/VTrans3/bin

#Edit the following variables if needed!

#If using a non-absolute path when setting the C Compiler causes this: http://public.kitware.com/Bug/view.php?id=13756
#So use an absolute path, as http://public.kitware.com/Bug/view.php?id=14294 proposes
#C_COMPILER=/usr/bin/gcc
#CPP_COMPILER=/usr/bin/g++

NUM_ARGS_NEEDED=1

#IDEA: pass as args: <NAME=VALUE>, e.g. COMMON_SOURCECODE_ROOT_PATH=../common_sourcecode
if [ $# -ge $NUM_ARGS_NEEDED ]; then
  ANDROID_NDK_PATH=$1
  echo "ANDROID_NDK_PATH: $ANDROID_NDK_PATH"

# "-pg" (DCMAKE_CXX_FLAGS) option for "gprof" profiling

#cd src
#use "-O3" in DCMAKE_CXX_FLAGS to optimize (for speed)
#from https://github.com/taka-no-me/android-cmake
#-G "Ninja" \
cmake ./src \
-DCMAKE_TOOLCHAIN_FILE=/home/sg/SourceCodeManagement/pixellight_Android_cmake/android.toolchain.cmake \
-DANDROID_NDK=$ANDROID_NDK_PATH \
-DCMAKE_BUILD_TYPE=Release \
-DANDROID_NO_UNDEFINED=ON \
-DANDROID_ABI="armeabi-v7a with NEON"  \
-DDISCORDIA_ANDROID_CMAKE_HOME=/home/sg/SourceCodeManagement/Discordia_android-cmake/android-cmake \
-DUSE_MINI_XML_STATIC_LIBRARY=1 \
-DANDROID_API_LEVEL=15 \
-DEXE_TYPE:STRING=Android
else
  echo "ERROR:at least" $NUM_ARGS_NEEDED " args needed:"
  echo $0 " <ANDROID_NDK_PATH>"
  echo " ANDROID_NDK_PATH: e.g. \"/devel/android-ndk-r10d\""
fi

