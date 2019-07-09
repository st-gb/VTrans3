#see https://de.wikipedia.org/wiki/Sed_%28Unix%29#Zusammengesetzte_Kommandos

#http://linuxcommand.org/wss0130.php
#http://stackoverflow.com/questions/4223294/using-or-in-shell-script
#if [ "$1" = "" -o "$1" = " " ]; then
#	echo "Error: no input file specified as command line parameter"
#else
# replace "jdn./etw." with empty string
# replace "jdn." with empty string
# replace "verb" with "{vaI}"
sed '/.*\tjdn\..*verb/ { # jemanden -> 4ter Fall/Akkusativ
  s/jdn\.\/etw\.\ // # "jdn./etw. " -> "" empty string
  s/jdn\.\ // # "jdn " -> "" empty string
  s/\ sb\.\/sth\.// # replace "sb./sth." with "" (empty string)
  s/\(.*\)\tverb/\1{vaI}/ # "vaI" = "verb accusative Infinitive"
  }' #$1
  
#fi

