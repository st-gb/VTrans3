#see/from https://github.com/taka-no-me/android-cmake

cmake -DCMAKE_TOOLCHAIN_FILE=/home/sg/SourceCodeManagement/taka-no-me_android-cmake.git/android-cmake/android.toolchain.cmake \
      -DANDROID_NDK=/media/sg/devel/devel/Android/android-ndk-r10e_Linux32bit                       \
      -DCMAKE_BUILD_TYPE=Release                     \
      -DANDROID_ABI="mips"          \
      ./src
cmake --build ./src
