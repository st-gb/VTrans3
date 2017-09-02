#see https://de.wikipedia.org/wiki/Sed_%28Unix%29#Zusammengesetzte_Kommandos

#http://linuxcommand.org/wss0130.php
#http://stackoverflow.com/questions/4223294/using-or-in-shell-script
#if [ "$1" = "" -o "$1" = " " ]; then
#	echo "Error: no input file specified as command line parameter"
#else
# replace "jdm./etw." with empty string
sed '/.*\tjdm\..*verb/ { # jemandem -> 4ter Fall/Akkusativ
  s/jdm\.\/etw\.\ // # "jdm./etw. " -> "" empty string
  s/\ sb\.\/sth\.// # replace "sb./sth." with "" (empty string)
  s/jdm\.\ // # "jdm. " -> "" empty string
  s/\(.*\)\tverb/\1{vdI}/ # "vdI" = "verb dative Infinitive"
  }' #$1
  
#fi

