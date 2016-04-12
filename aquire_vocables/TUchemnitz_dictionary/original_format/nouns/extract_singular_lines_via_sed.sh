#from http://de.wikipedia.org/wiki/Sed_%28Unix%29
#http://de.wikipedia.org/wiki/Sed_%28Unix%29#Ersetzung_mehrerer_bzw._aller_Vorkommen_innerhalb_einer_Zeile


#http://www.unix.com/shell-programming-scripting/134502-using-sed-perform-multiple-substitutions.html
#http://stackoverflow.com/questions/9541867/sed-delete-lines-not-containing-specific-string

#expected format: original TU Chemnitz dictionary with German::English order

#if regex between leftmost "/" and "/" matches: substite between 2nd "/" and "/" by 
# "/.*::.*{[fmn]}.*/!d": delete lines not containing "{f}", "{m}" or "{n}"
# "\1": German singular form of noun
# "\2": genus of noun
# "\3": English singular form of noun
# "|" separates plural from singular

#sed '/.*{[fmn]}.*::.*/!d;/.*{[fmn]}.*/ s/\([^{]*\)\({[fmn]}\).*:: \([^|]*\).*/\3::\1\2/g'

#see https://de.wikipedia.org/wiki/Sed_%28Unix%29#Verzweigungen
sed '/.*{[fmn]}.*::.*/!d'

#http://stackoverflow.com/questions/5410757/sed-delete-a-line-containing-a-specific-string
#sed '/{adj}/!d'
#sed '/.*::.*{adj}.*/!d'
