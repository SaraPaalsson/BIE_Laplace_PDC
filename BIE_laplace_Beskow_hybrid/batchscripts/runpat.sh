#!/bin/bash -l



#SBATCH -A summer-2016

#SBATCH -J specQuad

# time 
#SBATCH -t 01:00:00

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH -e error_file.e
#SBATCH -o output_file.o

export OMP_NUM_THREADS=2

aprun -n 16 -N 16 -d 2 -cc none ./main_exe+pat > my_output_file 2>&1
~                                                   
