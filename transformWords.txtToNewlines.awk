awk -F 0 ' {for (i = 1; i<=NF ; i++)
	{ # nicht "print", sondern "printf" verwenden, da "print" immer ein newline ausgibt
	printf $i #alle Vokabeln stehen in einer einzigen Zeile (=keine Newlines)
	if( i % 2 == 0 ) # Ende einer deutschen Vokabel
    printf "\n"
  else # Ende einer englischen Vokabel
    printf "0"
	}
	  }'

