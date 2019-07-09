awk ' 
function outputGermanPart(FirstGermanTokenIndex, LastGermanTokenIndex)
{
  printf "::"
  for (i = FirstGermanTokenIndex; i<=LastGermanTokenIndex ; i++)
  {
    printf $i
  }
}

function outputEnglishSynonyms(FirstGermanTokenIndex,LastGermanTokenIndex,EnglishWordBegin,englishSynonymSeperatorCharIndices)
{
          printf FirstGermanTokenIndex " " LastGermanTokenIndex " " EnglishWordBegin
        for (arrayIndex in englishSynonymSeperatorCharIndices)
        {
          englishSynonymSeperatorCharIndex = englishSynonymSeperatorCharIndices[arrayIndex]
          #printf "arrayIndex|:" englishSingularSynonymSeperatorCharIndex " "

        #output each token of a word (a word may have multiple tokens, e.g. "handy vacuum cleaner"
        for (i = EnglishWordBegin; i<=englishSynonymSeperatorCharIndex ; i++)
        {
          if( $i ~ /.*;/ )
          {
            printf substr($i,1, length($i) - 1)
            i = englishSynonymSeperatorCharIndex + 1
          }
          else
          {
            if( $i !~ /\|/ )
            {
             printf $i #output Englisch singular token
             if( i < englishSynonymSeperatorCharIndex - 1)
              printf " " 
            }
          }
        }
          EnglishWordBegin = englishSynonymSeperatorCharIndex + 1
        outputGermanPart(FirstGermanTokenIndex, LastGermanTokenIndex)
          printf "\n"
        }
}

{ wordkind = unassigned; englishGermanSeperatorFieldIndex = -1; englishSynonymSeperatorCharIndex = 0
        englishSingularSynonymSeperatorArrayIndex = 0;
        englishAttributeSeperatorChar = 0;
        GermanAttributeSeperatorChar = 0
        wordkind = ""
        englishAttributeNumber = 0
        englishPluralSynonymSeperatorArrayIndex = 0
        GermanSingularEndTokenIndex = 0
        GermanPluralEndTokenIndex = 0
        GermanAttributeNumber = 0
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
          if(GermanAttributeNumber == 0)
            GermanSingularEndTokenIndex = i - 1
          if(GermanAttributeNumber == 1)
            GermanPluralEndTokenIndex = i - 1
          #printf " englishGermanSeperatorFieldIndex: " englishGermanSeperatorFieldIndex
        }
	if( $i ~ /\|/ ) # "|" is seperator char for attributes of the same vocable, like a seperation between singular and plural
        {
          if( englishGermanSeperatorFieldIndex > 0)
          {
            #englishAttributeNumber = englishAttributeNumber + 1
            if( englishAttributeSeperatorChar == 0 )
            {
            englishAttributeSeperatorChar = i
            englishSingularSynonymSeperatorCharIndices[englishSingularSynonymSeperatorArrayIndex] = i
            }
            englishAttributeNumber = englishAttributeNumber + 1
          }
          else
          {
          if(GermanAttributeNumber == 0)
          {
            GermanAttributeSeperatorChar = i
            GermanSingularEndTokenIndex = i - 1
          }
          if(GermanAttributeNumber == 1)
            GermanPluralEndTokenIndex = i - 1
            
            GermanAttributeNumber = GermanAttributeNumber + 1
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
              englishSingularSynonymSeperatorCharIndices[englishSingularSynonymSeperatorArrayIndex] = i + 1
              #printf "array[" englishSingularSynonymSeperatorArrayIndex "]:" englishSingularSynonymSeperatorCharIndices[englishSingularSynonymSeperatorArrayIndex] " "
              englishSingularSynonymSeperatorArrayIndex = englishSingularSynonymSeperatorArrayIndex + 1
            }
            if( englishAttributeNumber == 1) # plural
            {
              englishPluralSynonymSeperatorCharIndices[englishPluralSynonymSeperatorArrayIndex] = i + 1
              englishPluralSynonymSeperatorArrayIndex = englishPluralSynonymSeperatorArrayIndex + 1
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

        if( wordkind ~ /singular_noun/ ) # no plural
        {
          if( englishAttributeSeperatorChar == 0 ) {
          #printf "englishAttributeSeperatorChar == 0"
          englishSingularSynonymSeperatorCharIndices[englishSingularSynonymSeperatorArrayIndex] = NF + 1
          #printf englishSingularSynonymSeperatorArrayIndex "=" NF
          englishAttributeSeperatorChar = NF + 1
          }
          if(englishAttributeNumber == 1)
            englishPluralSynonymSeperatorCharIndices[englishPluralSynonymSeperatorArrayIndex] = NF + 1
        }
        EnglishWordBegin = englishGermanSeperatorFieldIndex + 1
        #http://porthos.ist.utl.pt/docs/gawk/gawk_9.html
        #if( englishSingularSynonymSeperatorArrayIndex > 0 )
        if( wordkind ~ /singular_noun/ )
        {
          outputEnglishSynonyms(1, GermanSingularEndTokenIndex, EnglishWordBegin, englishSingularSynonymSeperatorCharIndices)
          #EnglishWordBegin = 
          outputEnglishSynonyms(GermanSingularEndTokenIndex + 2, GermanPluralEndTokenIndex, englishAttributeSeperatorChar, englishPluralSynonymSeperatorCharIndices)
        #for (arrayIndex in englishPluralSynonymSeperatorCharIndices)
        #{
         # englishPluralSynonymSeperatorCharIndex = englishPluralSynonymSeperatorCharIndices[arrayIndex]
        #}
        } # if( wordkind )  
	  }'
