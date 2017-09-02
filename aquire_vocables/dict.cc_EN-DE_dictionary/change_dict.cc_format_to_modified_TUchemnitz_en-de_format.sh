#http://linuxcommand.org/wss0130.php
#http://stackoverflow.com/questions/4223294/using-or-in-shell-script
if [ "$1" = "" -o "$1" = " " ]; then
  echo "Error: no input file specified as command line parameter"
else

#the tabulator character must/should be replaced _after_ sorting
cat $1 | ./remove_\'to\'_from_verbs.sh | ./change_accusative_verb_to_TUchemnitz_format.sh | ./change_dative_verb_to_TUchemnitz_format.sh | ./change_noun_to_TUchemnitz_format.sh | ./sort_until_tabulator_char.sh | ./replace_1st_tabular_char_with_sculp.sh
fi
