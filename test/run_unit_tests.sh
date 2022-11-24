#!/bin/bash

set -eu

trap "rm -rf unit_test" EXIT SIGINT

g++ unit_test.cpp -o unit_test -O2 -D LOCAL -std=c++0x -D_GLIBCXX_DEBUG -fsanitize=address
time ./unit_test
