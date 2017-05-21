#http://stackoverflow.com/questions/19479383/c-time-measurement-of-function
#http://sam.zoy.org/writings/programming/gprof.html

#TranslateParseTree

#http://www.thegeekstuff.com/2012/08/gprof-tutorial/
echo 1st param: $1
echo `pwd`
EXE=./src/VTrans_console
./src/VTrans_console "$1" $2
gprof $EXE gmon.out > gprof_analysis.txt
