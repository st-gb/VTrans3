# NF=number of fields
#awk -F 0 ' {for (i = 1; i<=NF ; i++)
awk ' BEGIN { ORS = "" } { gsub( "\n", "" ); gsub( /\n$/, "" );
	for (tokenIndex = 1; tokenIndex<=NF ; tokenIndex++)
	{ # nicht "print", sondern "printf" verwenden, da "print" immer ein newline ausgibt
	# printf $i #alle Vokabeln stehen in einer einzigen Zeile (=keine Newlines)
	#if($i~/^c/) {printf $i} # Ende einer deutschen Vokabel
	if ($tokenIndex!="to") {
		#printf $(tokenIndex+1) " "
		#use printf "%s " to avoid "awk: run time error: not enough arguments passed to printf("3% ")" when field value is "3%" 
		printf "%s ",$tokenIndex
	} # Ende einer deutschen Vokabel
	#printf $i
  #else # Ende einer englischen Vokabel
    #printf "0"
	#if($1=="car") printf $1
	}
	#print
printf "\n"
	  }'
