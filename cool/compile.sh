#!/bin/bash

# Arguments: 
# $1 = output file 

particle compile electron *.ino *.cpp *.h
if [ -z "$1" ]; then 
    mv electron* firmware.bin
else
    mv electron* $1
fi
