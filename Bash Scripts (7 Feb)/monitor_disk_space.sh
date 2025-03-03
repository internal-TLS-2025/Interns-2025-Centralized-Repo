#!/bin/bash

   # Log file to store disk space usage
   LOG_FILE="/home/t0315000/SHREYA/07-02-2025/disk_space_monitor.log"  # Change to your desired path

   # Log the start time
   echo "Monitoring disk space usage. Logging to $LOG_FILE."
   echo "Start time: $(date)" >> "$LOG_FILE"

   # Continuous monitoring loop
  # while true; do
       # Get current date/time for logging
       TIMESTAMP=$(date +'%Y-%m-%d %H:%M:%S')

       # Capture disk space usage
       DISK_USAGE=$(df -h)

       # Log the results
       echo "$TIMESTAMP" >> "$LOG_FILE"
       echo "$DISK_USAGE" >> "$LOG_FILE"
       echo "----------------------------------------" >> "$LOG_FILE"

       # Wait for a specific duration (e.g., 1 hour) before the next check
   #    sleep 3600
   # done

