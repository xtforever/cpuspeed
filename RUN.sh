#!/bin/bash
grep -m 1 model\ name /proc/cpuinfo 
make >/dev/null && ./svar2-test
