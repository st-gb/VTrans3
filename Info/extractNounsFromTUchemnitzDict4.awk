#line endings are very relevant: if wrong line endings, it is not executed correctly.
awk ' { \
english = 0 ; \
plural = 0 ; \
germanPlural = "" ; \
for (i = 1; i<=NF ; i++) { #NF = Number of Fields:die Anzahl der Felder(Leerzeichen-getrennt) fuer aktuelle Zeile
#if( i == 2 ) break ; #  {*}* : z.B. "{m}" , "{f};"  \
#if ( i == 2 && $i ~ /{m}/ ) \
if ( $i ~ /::/ ) { \
	#TU chemnitz dict entry (either German or English) starts with singular 
	plural = 0 ; \
	english = 1 ; \
	#print "separator for german-english" ; \
	}
else {
	if( english == 1 )  {
		#creates a line in the VTrans dict format: first char: "1" (=noun), "0": end of English or German entry
		line = "1" $i "991110" germanWord "9" germanPlural "9" german_article; \
		#line = germanWord ; \
		#print "eng=1" , "1", $i , "901", germanWord , "9" ; \
		print line ; \
		break ; 
	}
	else {
	#	print "eng=0", $i ;
		#if( plural == 1 && germanPlural == "" )
		#print "current token:" $i "plural :" plural  "current german pl:" germanPlural ;
		if( plural == 1 && length(germanPlural) == 0 ) {
			#print "german pl:" $i length(germanPlural) ;
			germanPlural = $i ;
			}
		}
}
#if( plural == 1 )
#	germanPlural = $ i ;
# "|" in TU chemnitz dict = seperator betwenn singular and plural
if ( $i == "|" ) plural = 1 ;
if ( i == 2 ) {
	#print "i==2" ; \
	if( $i ~ /{m}/ ) {
		german_article = "1" ; \
		#print "der" ;
		}
	if( $i ~ /{f}/ )
		german_article = "2" ;
	if( $i ~ /{n}/)
		german_article = "3" ;
   #print $1 ; #  break ; if( $i ~ /[a-zäöüß]*/ ) print $i ;} } # Speichern in Vektor
   germanWord = $1 ;
}
#end of for-loop
} \
}
END {#for (k in zaehler)
#printf("%10s %5i \n", k, zaehler[k]);
}'
