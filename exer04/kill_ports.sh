#!/bin/bash
# 'chmod +x kill_ports.sh' to make the script executable
max_port=16

# Function to kill processes using a specific port
function kill_processes_using_port() {
    local port=$1
    pids=$(lsof -ti ":$port")
    if [ -n "$pids" ]; then
        echo "Found processes using port $port: $pids"
        echo "$pids" | xargs kill -9
        echo "Processes using port $port killed."
    else
        echo "No processes found using port $port."
    fi
}

# Alternative method using pgrep and pkill
function kill_processes_using_port_alternative() {
    local port=$1
    pkill -f ":$port"
}

# Kill processes for ports 5000 to 500${max_port}
for ((i = 0; i <= max_port; i++)); do
    port=$((5000 + i))
    echo "Attempting to kill processes using port $port..."
    kill_processes_using_port "$port"
    # Uncomment the following line to use the alternative method
    # kill_processes_using_port_alternative "$port"
done

echo "Processes killed."
