#parts of NetBeans config: -DCMAKE_C_COMPILER=${IDE_CC} -DCMAKE_CXX_COMPILER=${IDE_CXX}
#to specify build dir some of these options may work: ../build -B../build -DCMAKE_BINARY_DIR=../build --build ../build -DCMAKE_CURRENT_BINARY_DIRECTORY=/home/sg/SourceCodeManagement/VTrans3/bin

#Edit the following variables if needed!

#If using a non-absolute path when setting the C Compiler causes this: http://public.kitware.com/Bug/view.php?id=13756
#So use an absolute path, as http://public.kitware.com/Bug/view.php?id=14294 proposes
C_COMPILER=/usr/bin/gcc
CPP_COMPILER=/usr/bin/g++

NUM_ARGS_NEEDED=1

BASEDIR=$(pwd)
echo "this shell script's dir: $BASEDIR"

#IDEA: pass as args: <NAME=VALUE>, e.g. COMMON_SOURCECODE_ROOT_PATH=../common_sourcecode
if [ $# -ge $NUM_ARGS_NEEDED ]; then
  BUILD_SYSTEM_GENERATOR=$1
  echo "BUILD_SYSTEM_GENERATOR: $BUILD_SYSTEM_GENERATOR"

# "-pg" (DCMAKE_CXX_FLAGS) option for "gprof" profiling

#cd src
#use "-O3" in DCMAKE_CXX_FLAGS to optimize (for speed)

EXEC="cmake -G \"$BUILD_SYSTEM_GENERATOR\" \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_C_COMPILER=${C_COMPILER} \
-DCMAKE_CXX_COMPILER=${CPP_COMPILER} \
-DCMAKE_C_FLAGS_DEBUG=\"-g3 -gdwarf-2\" \
-DCMAKE_CXX_FLAGS_DEBUG=\"-g3 -gdwarf-2\" \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DCMAKE_CXX_FLAGS=\"-pg -O3\" \
-DCOMPILE_WITH_OPENMP=NO \
. \
-DEXE_TYPE:STRING=console \
./src" #where to finde the CMakeLists.txt
echo "should execute: $EXEC"
eval $EXEC
else
  echo "ERROR:at least" $NUM_ARGS_NEEDED " args needed:"
  echo $0 " <build system>"
  echo " build system: e.g. \"Unix Makefiles\""
fi

