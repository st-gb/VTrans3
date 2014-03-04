#from http://de.wikipedia.org/wiki/Sed_%28Unix%29
#http://de.wikipedia.org/wiki/Sed_%28Unix%29#Ersetzung_mehrerer_bzw._aller_Vorkommen_innerhalb_einer_Zeile


#http://www.unix.com/shell-programming-scripting/134502-using-sed-perform-multiple-substitutions.html
#http://stackoverflow.com/questions/9541867/sed-delete-lines-not-containing-specific-string

#expected format: TU Chemnitz dictionary with English<->German exchanged order

#if regex between leftmost "/" and "/" matches: substite between 2nd "/" and "/" by 
# "/.*::.*{adj}.*/!d": delete lines not containing "{adj}"
# "\1": English singular form of noun
# "\2": German singular form of noun
# "\3": genus of noun
sed '/.*::.*{[fmn]}.*/!d;/.*{[fmn]}.*/ s/\([^|]*\).*::\([^{]*\)\({[fmn]}\).*/\1::\2\3/g'

#http://stackoverflow.com/questions/5410757/sed-delete-a-line-containing-a-specific-string
#sed '/{adj}/!d'
#sed '/.*::.*{adj}.*/!d'
