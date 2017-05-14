#http://valgrind.org/docs/manual/hg-manual.html 

NUM_ARGS_NEEDED=1
ENGLISH_TEXT="the car works"
if [ $# -ge $NUM_ARGS_NEEDED ]; then
  echo first arg: $1
  valgrind --tool=helgrind src/VTrans_console "$1"
fi

