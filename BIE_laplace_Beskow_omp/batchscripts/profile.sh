#!/bin/bash -l

FILENAME=../output/batch_inlining2/

export OMP_NUM_THREADS=32

pat_build -u -D trace-text-size=50 -Drtenv=PAT_RT_PERFCTR=default -f ./../api/main_exe

echo 'Running the pat file'
sbatch runpat.sh
unset FileXF
while [ -z $FileXF ]
do
    FileXF=$(grep *.xf my_output_file)
    if [ -z $FileXF ]
    then
        sleep 10
    fi
done

echo " File $FileXF  created"
echo " Running pat_report"
pat_report $FileXF > samp.prof
echo " Running pat_report"
pat_report -O profile+hwpc $FileXF > tracing.prof


mkdir "$FILENAME"
mv error_file.e "$FILENAME"
mv main_exe+* "$FILENAME"
mv my_output_file "$FILENAME"
mv output_file.o "$FILENAME"
mv samp.prof "$FILENAME"
mv tracing.prof "$FILENAME"
echo " Files moved to folder"

