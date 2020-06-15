#!/bin/bash

rm parser2; g++ -O3 -g main2.cpp -o parser2 2>&1 | more && ./parser2
