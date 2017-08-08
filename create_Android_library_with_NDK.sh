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
#from https://cmake.org/cmake/help/v3.7/manual/cmake-toolchains.7.html#cross-compiling-for-android
#-G "Ninja" \
cmake ./src \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_SYSTEM_NAME=Android \
-DCMAKE_SYSTEM_VERSION=14 \
-DCMAKE_ANDROID_ARCH=ARM \
-DCMAKE_ANDROID_ARCH_ABI=armeabi \
-DCMAKE_ANDROID_NDK=$ANDROID_NDK_PATH \
-DCMAKE_ANDROID_STL_TYPE=gnustl_static \
-DCMAKE_C_FLAGS_DEBUG="-g3 -gdwarf-2 --warn" \
-DCMAKE_CXX_FLAGS_DEBUG="-g3 -gdwarf-2" \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DCMAKE_CXX_FLAGS="-pg -O3" \
-DEXE_TYPE:STRING=Android
else
  echo "ERROR:at least" $NUM_ARGS_NEEDED " args needed:"
  echo $0 " <ANDROID_NDK_PATH>"
  echo " ANDROID_NDK_PATH: e.g. \"/devel/android-ndk-r10d\""
fi

