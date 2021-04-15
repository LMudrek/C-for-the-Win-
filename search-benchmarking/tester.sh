#!/bin/bash
declare -a input
input=(100 10000 100000 1000000 100000000 1000000000 2000000000 3000000000 4000000000 4294967295)

for i in "${input[@]}"
do 
  time ./search.exe b $i
done