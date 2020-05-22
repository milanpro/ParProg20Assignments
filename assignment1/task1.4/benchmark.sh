#!/bin/bash

file="measurement.csv"
if [ -e $file ] ; then
  rm $file
fi

touch $file
echo "strategy,nthreads,npoints,runtime" >> $file

strategies=(1 2 3)
nthreads=(1 2 3 4 6 8 16 32 64)
npoints=(10000000 100000000 1000000000)

for strat in "${strategies[@]}" ; do
  for threadcount in "${nthreads[@]}" ; do
    for pointcount in "${npoints[@]}" ; do
      for ((i = 0; i < 4; i++)) ; do
        # Redirect stderr to stdout and throw original stdout away
        result=$(/usr/bin/time --format="%e" ./mcpi "$pointcount" "$threadcount" "$strat" 2>&1 > /dev/null)
        echo "$strat","$threadcount","$pointcount","$result" >> $file
      done
    done
  done
done
