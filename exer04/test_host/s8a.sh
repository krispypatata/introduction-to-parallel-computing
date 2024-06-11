#!/bin/bash

# Define the filename and matrix size as variables
FILENAME="lab05_serial.py"
MATRIX_SIZE=20000

for port in {5001..5008}
do
  gnome-terminal --window -- bash -c "python3 $FILENAME $MATRIX_SIZE $port 1; exec bash"
done
