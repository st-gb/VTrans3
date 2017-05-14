#cd ..
# http://valgrind.org/docs/manual/faq.html#faq.deflost
#  --show-possibly-lost=no : to avoid Xerces memory leaks output

# "Reachable blocks (those to which a pointer was found) are not shown.
# To see them, rerun with: --leak-check=full --show-leak-kinds=all"
echo 1st param: $1

#valgrind --leak-check=yes --show-possibly-lost=no --leak-check=full ./src/VTrans_console $1
valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./src/VTrans_console $1
