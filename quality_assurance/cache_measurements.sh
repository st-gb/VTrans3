echo 1st param: $1
valgrind --tool=cachegrind ./src/VTrans_console "$1" $2 $3
