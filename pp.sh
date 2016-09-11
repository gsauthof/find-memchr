#!/bin/bash

# 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

r=$1

: ${r:=result.csv}

set -eu


x=$(awk -F, '$1 == "find_naive" { print $4 }' $r)



< $r \
   sort -t, -k4,4 -g \
 | sed 's/\([0-9]\.[0-9][0-9]\)[0-9]\+$/\1/' \
 | cut -d, -f 1-2,4-5,7-8  \
 | awk -F, 'BEGIN {OFS=","} NR==1 { print $0, "speedup"} NR> 1 { printf("%s,%.2f\n", $0, '"$x"'/$3) }' \
 | column -s',' -t -o ' | ' \
 | awk 'NR==1 {print;gsub("[^|]", "-");print} NR>1 {print}' 
