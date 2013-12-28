#$1 ~ /::/ { print $3 }

# NF=number of fields
#awk -F 0 ' {for (i = 1; i<=NF ; i++)
#http://de.wikibooks.org/wiki/Awk:_Grundlagen:_Aktionen#gsub.28regexp.2C_replacement_.5B.2C_target.5D.29: "Wie sub, allerdings wird global ersetzt."
awk ' BEGIN { ORS = "" } { gsub( "\n", "" ); gsub( /\n$/, "" );
    #
	prevCharIsSpecial = 0;
	german = 0;
	numEnglishPipeChars = 0;
	for (tokenIndex = 1; tokenIndex<=NF ; tokenIndex++)
	{ # nicht "print", sondern "printf" verwenden, da "print" immer ein newline ausgibt
		# printf $i #alle Vokabeln stehen in einer einzigen Zeile (=keine Newlines)
		#if($i~/^c/) {printf $i} # Ende einer deutschen Vokabel
	
		if(tokenIndex==1)
		{
			if(tokenIndex+1 < NF)
				printf "%s",$(tokenIndex)
		}
		else
		{
			#use printf "%s " to avoid "awk: run time error: not enough arguments passed to printf("3% ")" when field value is "3%" 
			# printf "%s",$(tokenIndex+1)
			#http://de.wikibooks.org/wiki/Awk:_Grundlagen:_Aktionen#index.28string.2C_find.29
			indexOfOpeningBrace = index($tokenIndex, "{")
			indexOfOpeningBracket = index($tokenIndex, "(")
			indexOfOpeningSquareBracket = index($tokenIndex, "[")
			if( $tokenIndex == "::" )
			{
				german = 1;
			}
			if( $tokenIndex == "{pl}" )
				englishPlural = tokenIndex -
			if ( $tokenIndex == "|" && german == 0)
			{
			   numEnglishPipeChars += 1;
			}
			if(  indexOfOpeningBracket > 0 || indexOfOpeningSquareBracket > 0 || indexOfOpeningBrace > 0 ) {
				prevCharIsSpecial=1
				printf "%s",$(tokenIndex)
			}
			else
			{
				if(prevCharIsSpecial==1)
				{
					printf "%s",$(tokenIndex)
					prevCharIsSpecial=0
				}
				else
					printf " %s",$(tokenIndex)
			}
			#use printf "%s " to avoid "awk: run time error: not enough arguments passed to printf("3% ")" when field value is "3%" 
			#printf "%s",$(tokenIndex)
		}
	} # Ende eines Vokabel-Paares
	#printf $i
  #else # Ende einer englischen Vokabel
    #printf "0"
	#if($1=="car") printf $1
	#}
	#print
printf "\n"
	  }'

