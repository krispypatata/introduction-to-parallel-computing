#!/bin/bash
# 'chmod +x kill_ports.sh' to make the script executable
max_port=16

# Function to kill processes using a specific port
function kill_processes_using_port() {
    local port=$1
    lsof -ti ":$port" | xargs kill -9 >/dev/null 2>&1
}

# Kill processes for ports 5000 to 500${max_port}
for ((i = 0; i <= max_port; i++)); do
    port=$((5000 + i))
    echo "Killing processes using port $port..."
    kill_processes_using_port "$port"
done

echo "Processes killed."
