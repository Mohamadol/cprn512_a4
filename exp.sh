#!/bin/bash

rm -rf results
mkdir results

for N in 8 64 512 1024 4096
do

    mkdir results/N$N

    echo "Experiment"
    make main.elf N=$N && ./main.elf > results/N$N.txt


done
