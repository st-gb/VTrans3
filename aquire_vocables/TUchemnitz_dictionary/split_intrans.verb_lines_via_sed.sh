#from http://de.wikipedia.org/wiki/Sed_%28Unix%29
#http://de.wikipedia.org/wiki/Sed_%28Unix%29#Ersetzung_mehrerer_bzw._aller_Vorkommen_innerhalb_einer_Zeile
#sed 's/.*\([;\{]\).*/ \1/g'
#sed 's/\([;{|]\|::\)/ \1/g'

#"{vi}" means intransitive verb
#sed '/.*{.*/ s/\([^\{]*\)\({[[:alpha:] ]\+}\).*::.*{vi} /MATCH\1 \2/g'
#sed '/.*{.*/ s/\([^\{\:]*\)\({[[:alpha:] ]\+}\)/BEG1\1END BEG2\2/g'
#sed -e '/.*{.*/ s/\([^\{]*\)\({[[:alpha:] ]\+}\).*::.*/BEG1\1END BEG2\2/g'

#until "{"
#sed -e '/.*{.*/ s/\([^\{]*\)\({[^\}]*\).*::.*/BEG1\1END BEG2\2END2/g'

#sed -e '/.*{.*/ s/\([^\{]*\)\({[^\}]*\)\(.*\)/BEG1\1END BEG2\2END2BEG3\3/g'

sed -e '/.*{.*/ s/\([^\{]*\)\({[^\}]*\)\(.*\)/\1\3/g'

#sed -e '/.*{.*/ s/.*\({[[:alpha:] ]\+}\).*::.*/BEG1\1END BEG2/g'

#sed '/.*;.*/ s/;/ ;/'
#sed '/.*{.*/ s/{/ {/'