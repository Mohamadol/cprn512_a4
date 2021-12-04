#!/bin/bash



#for N in 8 64 512 1024 4096
#do
N=8

mkdir results/N$N
echo "Experiment"
make main.elf && ./main.elf > results/N$N.txt


#done
