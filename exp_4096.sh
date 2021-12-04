#!/bin/bash



#for N in 8 64 512 1024 4096
#do

N=4096

echo "Experiment"
make main_4096.elf && ./main_4096.elf > results/N$N.txt


#done
