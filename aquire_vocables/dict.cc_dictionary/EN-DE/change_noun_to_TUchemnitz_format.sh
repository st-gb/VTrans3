#see https://de.wikipedia.org/wiki/Sed_%28Unix%29#Zusammengesetzte_Kommandos

#http://linuxcommand.org/wss0130.php
#http://stackoverflow.com/questions/4223294/using-or-in-shell-script
#if [ "$1" = "" -o "$1" = " " ]; then
#	echo "Error: no input file specified as command line parameter"
#else
# replace 'tabulator,"noun"' with empty string
# replace 1st ".* {" with ".*{" (remove space character)
sed '/.*noun/ { #
  s/\tnoun// # 
  s/\(.*\)\ {/\1{/ #
  }' #$1
  
#fi

