#!/bin/bash

# Log file paths

CPU_MEM_LOG_FILE="$HOME/Shreyas/07-02-25/cpu_mem_usage.log"

DISK_LOG_FILE="$HOME/Shreyas/07-02-25/disk_usage.log"

# Ensure the log files exist and are writable
touch "$CPU_MEM_LOG_FILE"
touch "$DISK_LOG_FILE"
chmod 644 "$CPU_MEM_LOG_FILE"
chmod 644 "$DISK_LOG_FILE"

# Function to log CPU and memory usage
log_cpu_mem_usage() {
    TIMESTAMP=$(date "+%Y-%m-%d %H:%M:%S")
    
    CPU_USAGE=$(top -bn1 | grep "Cpu(s)" | awk '{print $2 + $4}')  # User + System CPU usage
    MEM_USED=$(free -m | awk 'NR==2{print $3}')   # Used memory in MB
    MEM_TOTAL=$(free -m | awk 'NR==2{print $2}')  # Total memory in MB

    echo "$TIMESTAMP | CPU Usage: $CPU_USAGE% | Memory: $MEM_USED MB used / $MEM_TOTAL MB total" >> "$CPU_MEM_LOG_FILE"
}

# Function to log disk usage
log_disk_usage() {
    TIMESTAMP=$(date "+%Y-%m-%d %H:%M:%S")
    DISK_USAGE=$(df -h / | awk 'NR==2 {print $5}')  # Disk usage percentage
    DISK_TOTAL=$(df -h / | awk 'NR==2 {print $2}')  # Total disk space
    DISK_USED=$(df -h / | awk 'NR==2 {print $3}')   # Used disk space

    echo "$TIMESTAMP | Disk Usage: $DISK_USED of $DISK_TOTAL ($DISK_USAGE)" >> "$DISK_LOG_FILE"
}

# Run the functions at set intervals
while true; do
    log_cpu_mem_usage        # Log CPU and Memory usage every cycle
    log_disk_usage           # Log Disk usage every cycle
    sleep 30                 # Sleep for 30 seconds before the next cycle
done


