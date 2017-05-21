
#https://web.stanford.edu/class/cs107/guide_callgrind.html
#http://valgrind.org/docs/manual/cl-manual.html
# --separate-threads=yes
NUM_ARGS_NEEDED=1
echo 1st param: $1
echo 2nd param: $2
if [ $# -ge $NUM_ARGS_NEEDED ]; then
  valgrind --tool=callgrind --simulate-cache=yes ./src/VTrans_console "$1" $2
else
  echo "no input text specified"
fi
