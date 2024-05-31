#!/bin/bash

# Define the filename and matrix size as variables
FILENAME="lab04_parallel.py"
MATRIX_SIZE=30000

for port in {5009..5016}
do
  gnome-terminal -- bash -c "python3 $FILENAME $MATRIX_SIZE $port 1; exec bash"
done
