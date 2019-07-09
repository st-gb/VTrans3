#parts of NetBeans config: -DCMAKE_C_COMPILER=${IDE_CC} -DCMAKE_CXX_COMPILER=${IDE_CXX}
#to specify build dir some of these options may work: ../build -B../build -DCMAKE_BINARY_DIR=../build --build ../build -DCMAKE_CURRENT_BINARY_DIRECTORY=/home/sg/SourceCodeManagement/VTrans3/bin
#-DWXWIDGETS=ON

#Edit the following variables if needed!

NUM_ARGS_NEEDED=2

#IDEA: pass as args: <NAME=VALUE>, e.g. COMMON_SOURCECODE_ROOT_PATH=../common_sourcecode
if [ $# -ge $NUM_ARGS_NEEDED ]; then
  #If using a non-absolute path when setting the C Compiler causes this: http://public.kitware.com/Bug/view.php?id=13756
  #So use an absolute path, as http://public.kitware.com/Bug/view.php?id=14294 proposes
  #C_COMPILER=C:/MinGW/5.3.0-2/bin/gcc.exe
  #CPP_COMPILER=C:/MinGW/5.3.0-2/bin/g++.exe
  CPP_COMPILER=$1
  C_COMPILER=$2

# "-pg" option for "gprof" profiling
# "gdwarf-2"
# -fsanitize=address -fno-omit-frame-pointer -static-libasan
# -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address" \
#https://github.com/gayanpathirage/SanitizersReviewTestSuite
cmake -G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_C_COMPILER=${C_COMPILER} \
-DCMAKE_CXX_COMPILER=${CPP_COMPILER} \
-DUSE_ADDRESS_SANITIZER="YES" \
-DCMAKE_C_FLAGS_DEBUG="-g" \
-DCMAKE_CXX_FLAGS_DEBUG="-O0 -g3" \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
. \
-DEXE_TYPE:STRING=console
else
  echo "ERROR:at least" $NUM_ARGS_NEEDED " args needed:"
  echo "specifiy options for this script:<C++ compiler path> <C compiler path> <Mini XML path>"
  echo "e.g.:"$0 C:/MinGW/5.3.0-2/bin/g++.exe C:/MinGW/5.3.0-2/bin/gcc.exe
fi

