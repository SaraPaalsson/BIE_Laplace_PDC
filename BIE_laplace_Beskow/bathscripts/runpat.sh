#!/bin/bash -l



#SBATCH -A summer-2016

#SBATCH -J specQuad

# time 
#SBATCH -t 0:30:00

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH -e error_file.e
#SBATCH -o output_file.o

aprun -n 1 ./main_exe+pat > my_output_file 2>&1
~                                                   
