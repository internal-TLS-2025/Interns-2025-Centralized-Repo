#!/usr/bin/bash

# Define the log file path
LOG_FILE="/home/t0314996/sandhya_doc/07-02-25/monitorUsageOutput.log"

# Get current timestamp
timestamp=$(date '+%Y-%m-%d %H:%M:%S')

# Get CPU usage
cpuUsage=$(top -bn1 | awk '/Cpu/ { print $2 }')

# Get Memory usage
memUsage=$(free -m | awk '/Mem/{print $3}')

# Get Disk Space usage
diskUsage=$(df -h | grep '^/dev/' | awk '{print $1, $3, $5, $6}')

# Log the information to the log file
echo "$timestamp - CPU Usage: ${cpuUsage}%; Memory Usage: ${memUsage} MB; Disk Space Usage: $diskUsage" >> "$LOG_FILE"
