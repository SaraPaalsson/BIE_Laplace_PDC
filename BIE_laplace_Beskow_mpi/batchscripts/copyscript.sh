#!/bin/bash

FILENAME=../output/batch1/

mkdir "$FILENAME"

mv error_file.e "$FILENAME"
mv main_exe+* "$FILENAME"
mv my_output_file "$FILENAME"
mv output_file.o "$FILENAME"
mv samp.prof "$FILENAME"
mv tracing.prof "$FILENAME"
~                               
