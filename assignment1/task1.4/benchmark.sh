#!/bin/bash

FILE="measurement.csv"
if [ -e $FILE ] ; then
  rm $FILE
fi

touch $FILE
echo "strategy,nthreads,npoints,runtime" >> $FILE

STRATEGIES=(1 2 3)
NTHREADS=(1 2 3 4 6 8 16 32 64)
NPOINTS=(10000000 100000000 1000000000)

for strat in "${STRATEGIES[@]}" ; do
  for threadcount in "${NTHREADS[@]}" ; do
    for pointcount in "${NPOINTS[@]}" ; do
      for ((i = 0; i < 4; i++)) ; do
        # Redirect stderr to stdout and throw original stdout away
        RESULT=$(/usr/bin/time --format="%e" ./mcpi "$pointcount" "$threadcount" "$strat" 2>&1 > /dev/null)
        echo "$strat","$threadcount","$pointcount","$RESULT" >> $FILE
      done
    done
  done
done
