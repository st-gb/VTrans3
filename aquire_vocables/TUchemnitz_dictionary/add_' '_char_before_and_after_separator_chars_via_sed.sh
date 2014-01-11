#from http://de.wikipedia.org/wiki/Sed_%28Unix%29
#http://de.wikipedia.org/wiki/Sed_%28Unix%29#Ersetzung_mehrerer_bzw._aller_Vorkommen_innerhalb_einer_Zeile
#sed 's/.*\([;\{]\).*/ \1/g'

# ";"->" ; " , "{"->" { " , "|"->" | " , "::"->" :: " 
sed 's/\([;{|]\|::\)/ \1 /g'
#sed 's/\([[:alpha:] ]\+\)/\1 /g'
#sed '/.*;.*/ s/;/ ;/'
#sed '/.*{.*/ s/{/ {/'