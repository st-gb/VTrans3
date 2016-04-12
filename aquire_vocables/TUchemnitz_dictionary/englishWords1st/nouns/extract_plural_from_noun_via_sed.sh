#from http://de.wikipedia.org/wiki/Sed_%28Unix%29
#http://de.wikipedia.org/wiki/Sed_%28Unix%29#Ersetzung_mehrerer_bzw._aller_Vorkommen_innerhalb_einer_Zeile


#http://www.unix.com/shell-programming-scripting/134502-using-sed-perform-multiple-substitutions.html
#http://stackoverflow.com/questions/9541867/sed-delete-lines-not-containing-specific-string

#expected format: TU Chemnitz dictionary with English<->German exchanged order

#if regex between leftmost "/" and "/" matches: substite between 2nd "/" and "/" by 
# "/.*::.*{pl}.*/!d": delete lines not containing "{pl}"
# "\1": English singular form of noun
# "\2": German singular form of noun
# "\3": genus of noun
# "Afrodeutsche {m,f}"
sed '/.*|.*::.*{pl}.*/!d;/.*|.*{[fmn]}.*/ s/\([^|]*\).*| \([^|]*\)::\([^{]*\).*|\([^|{]*\) {pl}.*/\2::\4 {pl}/g'

#http://stackoverflow.com/questions/5410757/sed-delete-a-line-containing-a-specific-string
#sed '/{adj}/!d'
#sed '/.*::.*{adj}.*/!d'
