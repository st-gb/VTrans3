#!/bin/bash
#from http://de.wikipedia.org/wiki/Sed_%28Unix%29
#http://de.wikipedia.org/wiki/Sed_%28Unix%29#Ersetzung_mehrerer_bzw._aller_Vorkommen_innerhalb_einer_Zeile

dictionaryFilePath="../../../dictionaries/TUchemnitz/en-de/en-de.txt"
#echo $dictionaryFilePath

#http://linuxcommand.org/wss0130.php
#http://stackoverflow.com/questions/4223294/using-or-in-shell-script
if [ "$1" = "" -o "$1" = " " ]; then
	echo "no input file specified as command line parameter->using default dict file path "$dictionaryFilePath
else
	dictionaryFilePath=$1
	echo "using "$dictionaryFilePath" as dict file path"
fi
#http://www.unix.com/shell-programming-scripting/134502-using-sed-perform-multiple-substitutions.html
#http://stackoverflow.com/questions/9541867/sed-delete-lines-not-containing-specific-string

#expected format: TU Chemnitz dictionary with English<->German exchanged order
# sed 's/alt/NEU/' inputfile

#if regex between leftmost "/" and "/" matches: substitute between 2nd "/" and "/" by 
# "/.*::.*{pl}.*/!d": delete lines not containing "{pl}"
# "\1": English singular form of noun
# "\2": German singular form of noun
# "\3": genus of noun
# "Afrodeutsche {m,f}"
sed '/.*|.*::.*{pl}.*/!d' $dictionaryFilePath > $2

#http://stackoverflow.com/questions/5410757/sed-delete-a-line-containing-a-specific-string
#sed '/{adj}/!d'
#sed '/.*::.*{adj}.*/!d'
