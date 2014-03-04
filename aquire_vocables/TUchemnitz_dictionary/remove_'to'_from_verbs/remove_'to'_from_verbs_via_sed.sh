

#replace "to " by "" (empty string)
# ensure "{v" -> word class "verb" is right of "::"
sed '/.*::.*{v.*/ s/to //'
