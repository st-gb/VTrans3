awk ' 
function outputGermanPart(GermanAttributeSeperatorChar)
{
  printf "::"
  for (i = 1; i<GermanAttributeSeperatorChar ; i++)
  {
    printf $i
  }
}

{ wordkind = unassigned; englishGermanSeperatorFieldIndex = -1; englishSynonymSeperatorCharIndex = 0
        englishSingularSynonymSeperatorArrayIndex = 0;
        englishAttributeSeperatorChar = 0;
        GermanAttributeSeperatorChar = 0
        wordkind = ""
        #englishSingularSynonymSeperatorCharIndices
        #print "arrayIndex" arrayIndex;
        for (i = 1; i<=NF ; i++) #NF=Number of Fields in current line
	{ # nicht "print", sondern "printf" verwenden, da "print" immer ein newline ausgibt
	#printf $i
	if( $i ~ /::/ ) #
        {
          englishGermanSeperatorFieldIndex = i
          if( GermanAttributeSeperatorChar == 0)
            GermanAttributeSeperatorChar = i
          #printf " englishGermanSeperatorFieldIndex: " englishGermanSeperatorFieldIndex
        }
	if( $i ~ /\|/ ) # "|" is seperator char for attributes of the same vocable, like a seperation between singular and plural
        {
          if(GermanAttributeSeperatorChar == 0)
            GermanAttributeSeperatorChar = i
          if( englishGermanSeperatorFieldIndex > 0 && englishAttributeSeperatorChar == 0 )
          {
            englishAttributeSeperatorChar = i
            englishSingularSynonymSeperatorCharIndices[englishSingularSynonymSeperatorArrayIndex] = i
          }
        }
	if( $i ~ /{f/ ) # feminine singular noun
        {
          wordkind = "singular_noun"
          #printf " feminine singular noun at: " i
        }
	if( $i ~ /{m/ ) # feminine singular noun
          wordkind = "singular_noun"
	if( $i ~ /{n/ ) # feminine singular noun        
          wordkind = "singular_noun"
        if( $i ~ /.*;/ )
          if( i > englishGermanSeperatorFieldIndex ) { #ensure being in English part
            #printf " English semicolon at " i
            if( englishAttributeSeperatorChar == 0 )
            {
              englishSingularSynonymSeperatorCharIndices[englishSingularSynonymSeperatorArrayIndex] = i
              #printf "array[" englishSingularSynonymSeperatorArrayIndex "]:" englishSingularSynonymSeperatorCharIndices[englishSingularSynonymSeperatorArrayIndex] " "
              englishSingularSynonymSeperatorArrayIndex = englishSingularSynonymSeperatorArrayIndex + 1
              
            }
            #array[subscript]
          }
          #else
          #  printf " German semicolon at " i
         #if( i==NF)
           #printf "\n"
	}
        #print "arrayIndex" arrayIndex
        #if( englishSynonymSeperatorCharIndex > 0 )
        #if( englishSingularSynonymSeperatorArrayIndex > 0 )
        #arrLen = length(englishSingularSynonymSeperatorCharIndices)
        #printf "arrLen" arrLen
        #for( arrayindex = englishGermanSeperatorFieldIndex; englishSingularSynonymSeperatorArrayIndex[arrLen - 1]; arrayindex )

        if( wordkind ~ /singular_noun/ && englishAttributeSeperatorChar == 0 )
        {
          #printf "englishAttributeSeperatorChar == 0"
          englishSingularSynonymSeperatorCharIndices[englishSingularSynonymSeperatorArrayIndex] = NF + 1
          #printf englishSingularSynonymSeperatorArrayIndex "=" NF
          englishAttributeSeperatorChar = NF + 1
        }
        EnglishWordBegin = englishGermanSeperatorFieldIndex + 1
        #http://porthos.ist.utl.pt/docs/gawk/gawk_9.html
        #if( englishSingularSynonymSeperatorArrayIndex > 0 )
        if( wordkind ~ /singular_noun/ )
        for (arrayIndex in englishSingularSynonymSeperatorCharIndices)
        {
          englishSingularSynonymSeperatorCharIndex = englishSingularSynonymSeperatorCharIndices[arrayIndex]
          #printf "arrayIndex|:" englishSingularSynonymSeperatorCharIndex " "
        for (i = EnglishWordBegin; i<=englishSingularSynonymSeperatorCharIndex ; i++)
        {
          if( $i ~ /.*;/ )
          {
            printf substr($i,1, length($i) - 1)
          }
          else
          {
            if( $i !~ /\|/ )
            {
             printf $i #output Englisch singular token
             if( i < englishAttributeSeperatorChar - 1)
              printf " " 
            }
          }
        }
          EnglishWordBegin = englishSingularSynonymSeperatorCharIndex + 1
        outputGermanPart(GermanAttributeSeperatorChar)
          printf "\n"
        }

        
	  }'
