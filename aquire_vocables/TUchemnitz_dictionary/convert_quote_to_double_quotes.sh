
#from http://de.wikipedia.org/wiki/Sed_%28Unix%29
#http://de.wikipedia.org/wiki/Sed_%28Unix%29#Ersetzung_mehrerer_bzw._aller_Vorkommen_innerhalb_einer_Zeile

#"\1" means: 1st between "( )"
#sed -e '/.*{.*/ s/\([^\{]*\)\({[^\}]*\)\(.*\)/\1\3/g'
#"s = substitute" ?
# see https://stackoverflow.com/questions/15212489/sqlite3-import-with-quotes
sed -e 's/"/""/g'
