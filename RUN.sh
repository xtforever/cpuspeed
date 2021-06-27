#!/bin/bash

make >/dev/null || exit 1
cpu=$(grep -m 1 model\ name /proc/cpuinfo)
speed=$(./svar2-test)
today=$(date +%s)

echo $today : $speed : $cpu | tee -a speed-test.txt

