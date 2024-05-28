#!/bin/bash
# 'chmod +x execute_lab04.sh' to make the script executable

# Start server
echo "Starting server..."
python3 lab04.py 25000 5000 0 &

# Sleep for a short duration to ensure the server has started
sleep 1

# Start clients
for ((i = 1; i <= 16; i++)); do
    echo "Starting client $i..."
    python3 lab04.py 25000 $((5000 + i)) 1 &
done

# Wait for all clients to finish
wait

echo "All clients finished."
