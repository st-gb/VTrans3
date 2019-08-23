#parts of NetBeans config: -DCMAKE_C_COMPILER=${IDE_CC} -DCMAKE_CXX_COMPILER=${IDE_CXX}
#to specify build dir some of these options may work: ../build -B../build -DCMAKE_BINARY_DIR=../build --build ../build -DCMAKE_CURRENT_BINARY_DIRECTORY=/home/sg/SourceCodeManagement/VTrans3/bin
#-DWXWIDGETS=ON

#Edit the following variables if needed!

#If using a non-absolute path when setting the C Compiler causes this: http://public.kitware.com/Bug/view.php?id=13756
#So use an absolute path, as http://public.kitware.com/Bug/view.php?id=14294 proposes
C_COMPILER=/usr/bin/gcc
CPP_COMPILER=/usr/bin/g++

NUM_ARGS_NEEDED=3

#IDEA: pass as args: <NAME=VALUE>, e.g. COMMON_SOURCECODE_ROOT_PATH=../common_sourcecode
if [ $# -ge $NUM_ARGS_NEEDED ]; then
  BUILD_SYSTEM_GENERATOR=$1
  echo "build generator: $BUILD_SYSTEM_GENERATOR"
  WXWIDGETS_INCLUDE_DIR=$2
  WXWIDGETS_SETUP_H_PATH=$3

# "-pg" option for "gprof" profiling

cmake -G "$BUILD_SYSTEM_GENERATOR" \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_C_COMPILER=${C_COMPILER} \
-DCMAKE_CXX_COMPILER=${CPP_COMPILER} \
-DCMAKE_C_FLAGS_DEBUG="-g3 -gdwarf-2 -pg" \
-DCMAKE_CXX_FLAGS="-pg" \
-DCMAKE_CXX_FLAGS_DEBUG="-g3 -gdwarf-2 -pg" \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DEXE_TYPE:STRING=wxGUI \
-DWXWIDGETS_INCLUDE_DIR:STRING=$WXWIDGETS_INCLUDE_DIR \
-DWXWIDGETS_SETUP_H_PATH:STRING=$WXWIDGETS_SETUP_H_PATH \
./../../src
else
  echo "ERROR:at least" $NUM_ARGS_NEEDED " args needed:"
  echo "specifiy options for this script:<WXWIDGETS_INCLUDE_DIR> <WXWIDGETS_SETUP_H_PATH> <COMMON_SOURCECODE_ROOT_PATH>"
  echo "e.g.:"$0 /usr/include/wx-3.0 /usr/lib/i386-linux-gnu/wx/include/gtk2-unicode-debug-3.0 /home/sg/SourceCodeManagement/common_sourcecode
fi

