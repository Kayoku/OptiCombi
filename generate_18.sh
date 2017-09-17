#!/bin/bash
set -e

cd cmake/bin
k=2
select=( "first" "best" )
neighbour=( "insert" "swap" "exchange" )
init=( "rnd" "edd" "mdd")

for s in 'first' 'best'
do
 for n in 'exchange' 'swap' 'insert'
 do
  for i in 'rnd' 'edd' 'mdd'
  do
  echo "Start of "$s" "$n" "$i
   ./climbing_smtwtp_main 100 ../../instances/wt100.txt $k $s"-"$n"-"$i".csv" $s $n $i
  done
 done
done
