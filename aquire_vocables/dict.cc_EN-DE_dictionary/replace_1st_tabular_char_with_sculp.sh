# replace 1st tabulator with sculp ("::") 
#sed 's/\([^\t]*\)\t/\1::/'

# replaces the 1st tabulator with sculp ("::"), " [" with "["
#//TODO: replace ALL " [" in line with "[" ? ("g" option) 
sed 's/\t/::/' | sed 's/\ \[/\[/'
