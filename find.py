#!/usr/bin/python3

# 2016, Georg Sauthoff <mail@georg.so>, GPLv3+

import sys

n = int(sys.argv[1])
for i in range(n):
  with open(sys.argv[2], 'r') as f:
    for line in f:
      print(line.find('|'))
