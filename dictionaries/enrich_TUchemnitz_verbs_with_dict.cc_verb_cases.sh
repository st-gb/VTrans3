
echo "1st argument: TU Chemnitz dictionary "

#if grep /.*{vt}.*/

sed '/.*{vt}/ {
  s/\tnoun// # 
  s/\(.*\)\ {/\1{/
  }' #$1

#algorithm:
# 1. read line from text file 
# 2. 

#grep dict.cc
