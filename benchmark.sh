#!/bin/bash

# 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

set -eu


exes_mini='find_find find_memchr
      find_naive find_musl find_uclibc find_unroll
      '
exes_x86='find_uclibcx86'
exes_sse='find_diet find_sse'
exes_avx='find_avx2_overflow
      find_avx2_nozero
      find_avx2
      find_avx2_overflow_ext find_avx2_align find_avx2_align2
      find_avx2_more
      find_avx2_loop find_avx2_memcpy'
exes=$exes_mini

time=/usr/bin/time
in='in'
ref='ref'
t='tmp'
#brep=5
brep=20
frep=30000
result=result.csv
targets=all-mini
have_avx=0

function gen_input
{
  if [ -f "$in".gz ] ; then
    zcat "$in".gz > "$in"
    return 0
  fi
  aspell -d en dump master | aspell -l en expand \
    | paste '-d,,,,|,,'  - - - - - - - - > "$in"
  gzip -c "$in" > "$in".gz

}
function configure
{
  if [ -f /proc/cpuinfo ]; then
    if grep '^flags.*avx2' /proc/cpuinfo > /dev/null ; then
      targets+=' 'all-avx
      have_avx=1
      exes+=' '$exes_avx
    fi
    if grep '^flags.*sse2' /proc/cpuinfo > /dev/null ; then
      targets+=' 'all-sse
      exes+=' '$exes_sse
    fi
    cpu=$(uname -p)
    if [ "${cpu:0:3}" = x86 ]; then
      targets+=' 'all-x86
      exes+=' '$exes_x86
    fi
  fi
}
function setup
{
  rm -f "$t"
  rm -f "$ref"
  make clean
  if [ "$have_avx" = 1 ]; then
    make find_avx2 CXXFLAGS_NOZERO='-mno-vzeroupper'
    mv find_avx2 find_avx2_nozero
    make clean
  fi
  make $targets
}

function check
{
  ./find_naive 1 "$in" > $ref
  for exe in $exes; do
    ./"$exe" 1 "$in" > "$t"
    cmp "$t" "$ref" \
      || { echo "$exe produced deviating output" >&2; exit 1; }
  done
}
function benchmark
{                   
  local f=$(mktemp)
  local n=$1
  shift
  {
    echo all,user,sys,rss >&2
    for i in $(seq $n); do
      "$time" -f '%e,%U,%S,%M' "$@" >/dev/null
      sleep 50
    done
  } 2> "$f"
  Rscript --vanilla -e "b=read.csv(file='$f');summary(b);sapply(b, sd);q()"
  rm -f "$f"
}
# all: min 1stQ medi mean 3rdQ max sd
function measure
{
  printf 'exe,min,1stQ,median,mean,3rdQ,max,sdev\n'
  for exe in $exes; do
    printf "$exe,"
    benchmark "$brep" ./"$exe" "$frep" "$in" \
      | sed 's/^[^:]*://' | awk '{print $1}' | grep -v all \
      | paste -s - -d ,
  done
}

gen_input
configure
exes=find_unroll2
setup
check
measure | tee "$result"


