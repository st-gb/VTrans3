#from http://de.wikipedia.org/wiki/Sed_%28Unix%29
#http://de.wikipedia.org/wiki/Sed_%28Unix%29#Ersetzung_mehrerer_bzw._aller_Vorkommen_innerhalb_einer_Zeile


#http://www.unix.com/shell-programming-scripting/134502-using-sed-perform-multiple-substitutions.html
#http://stackoverflow.com/questions/9541867/sed-delete-lines-not-containing-specific-string

#expected format: TU Chemnitz dictionary with English<->German exchanged order

#if regex between leftmost "/" and "/" matches: substite between 2nd "/" and "/" by 
# "/RegEx/!d": delete lines not containing the regular expression "RegEx"
# "\1": positive form of adjective
# "\3": after wordkind, e.g. a preposition: "schädlich {adj} (für)" -> "schädlich (für)"
# "to work{work, wrought; worked, wrought}"
# "| he/she works |"   "| er/sie arbeitet |"
#sed '/.*she\ \([^s]*\)s[\ ;].*::.*{v[irt]}.*/!d;s/.*she\ \([^s]*\)s[\ ;].*::.*\({v[irt]\).*\ er\/sie\([^|]*\).*er\/sie hat.*/\1s::\2\3_3rdPersSingPres}/g'
sed '/to\ .*::.*{v[irt]}.*/!d;/.*{v[irt]}.*/ s/to\ \([^|;:{]*\).*::\([^{]*\)\({v[irt]\).*/\1::\2\3_3rdPersSingPres}/g'

#http://stackoverflow.com/questions/5410757/sed-delete-a-line-containing-a-specific-string
#sed '/{adj}/!d'
#sed '/.*::.*{adj}.*/!d'
