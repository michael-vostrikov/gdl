#!/bin/bash

rm parser; g++ -O3 -g main.cpp -o parser 2>&1 | more && ./parser
