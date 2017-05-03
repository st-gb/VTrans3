#cd ..
# http://valgrind.org/docs/manual/faq.html#faq.deflost
#  --show-possibly-lost=no : to avoid Xerces memory leaks output

# "Reachable blocks (those to which a pointer was found) are not shown.
# To see them, rerun with: --leak-check=full --show-leak-kinds=all"
valgrind --leak-check=yes --show-possibly-lost=no --leak-check=full ./src/VTrans
