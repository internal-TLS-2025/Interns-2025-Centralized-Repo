#!/bin/bash

# Directory to monitor
MONITOR_DIR="$HOME/Shreyas/06-02-25"
# Log file path
LOG_FILE="$HOME/Shreyas/06-02-25/directory_monitor.log"

# Ensure the log file exists
touch "$LOG_FILE"
chmod 644 "$LOG_FILE"

# Function to monitor the directory
monitor_directory() {
    inotifywait -m -r -e create "$MONITOR_DIR" | while read path action file; do
        TIMESTAMP=$(date "+%Y-%m-%d %H:%M:%S")
        echo "$TIMESTAMP | New file created: $file in $path" >> "$LOG_FILE"
    done
}

# Start monitoring
monitor_directory
