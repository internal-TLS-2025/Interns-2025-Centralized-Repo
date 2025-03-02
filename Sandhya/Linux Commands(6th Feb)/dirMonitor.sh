!/bin/bash
 
  # Directory to monitor
  MONITOR_DIR="/home/t0314996/sandhya_doc/"
  # Log file to store changes
 LOG_FILE="/home/t0314996/sandhya_doc/06-02-25/dirMonitorOutput.log"
 
  # Check if the directory exists
  if [ ! -d "$MONITOR_DIR" ]; then
      echo "Directory $MONITOR_DIR does not exist."
      exit 1
  fi
 
  # Start monitoring
  echo "Monitoring directory: $MONITOR_DIR"
  echo "Logging changes to: $LOG_FILE"
 
  # Use inotifywait to monitor the directory for new files
  inotifywait -m -r -e create "$MONITOR_DIR" | while read path action file; do
      echo "$(date +'%Y-%m-%d %H:%M:%S') - New file created: $file in $path" >> "$LOG_FILE"
  done

