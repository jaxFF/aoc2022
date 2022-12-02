#!/bin/sh

gcc shared_scratch.c -Ishared_code -DAOC_BUILD_EXE -FC -O2 -static -o shared_scratch;gcc -fPIC -Ishared_code -FC -O2 -static -c code/day1.c;gcc -DAOC_BUILD_EXE -Ishared_code -static day1.o code/runner.c -o runner
