
# https://cmake.org/cmake/help/v3.7/manual/cmake-toolchains.7.html
#CMAKE_BINARY_DIR : https://cmake.org/Wiki/CMake_Useful_Variables

#
#  -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
#  -DCMAKE_BINARY_DIR=./build \
#  -DEXE_TYPE:STRING=Android \
#  -DCMAKE_ANDROID_STANDALONE_TOOLCHAIN=/home/sg/SourceCodeManagement/Android_toolchain \

cmake \
  -DCMAKE_SYSTEM_NAME=Android \
  ./src #where to finde the CMakeLists.txt

