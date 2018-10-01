#!/bin/bash

## This is to run the first program graphgen.java

if [ -e "graphgen.class" ]; then
    java graphgen $1
else
    echo "First run compile.sh to compile the code"
fi