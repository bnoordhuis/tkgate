#!/bin/sh

VERGA=../../src/verga/verga

PASS=0
FAIL=0
for f in *.v; do
  
  echo -n "checking $f ... "
  $VERGA -q $f > verga.out
  if diff -q verga.out ${f%.v}.out 1> /dev/null; then
    echo "ok"
    PASS=`expr $PASS + 1`
  else
    echo "failed"
    FAIL=`expr $FAIL + 1`
  fi
done
echo ">>> $PASS tests passed.  $FAIL tests failed."
