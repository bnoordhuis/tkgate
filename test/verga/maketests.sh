#!/bin/sh

for f in *.v; do
  
  echo "making $f ... "
  ../verga -q $f > ${f%.v}.out
done
