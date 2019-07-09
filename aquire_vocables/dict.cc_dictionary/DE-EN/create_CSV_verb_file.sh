#!/bin/sh

#see https://de.wikipedia.org/wiki/Sed_%28Unix%29#Zusammengesetzte_Kommandos

#http://linuxcommand.org/wss0130.php
#http://stackoverflow.com/questions/4223294/using-or-in-shell-script
if [ "$1" = "" -o "$1" = " " ]; then
	echo "Error: no input file specified as command line parameter"
else
  OUTPUT_FILE_PATH=$1
  #see https://de.wikipedia.org/wiki/CSV_(Dateiformat)
  echo "German,English,case\n" > $OUTPUT_FILE_PATH

sed '/.*verb$/ {
  s/\([^\t]*\)\t\([^\t]*\)verb/\1,\2/ # "
  }' #$1
  
fi

