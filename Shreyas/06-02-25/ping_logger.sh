#!/bin/bash


LOG_FILE="$HOME/Shreyas/06-02-25/ping_monitor.log"
SERVER="10.113.50.100"

while true; do

TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')

if ping -c 1 "$SERVER" > /dev/null 2>&1; then
echo "$TIMESTAMP: $SERVER is reachable." >> "$LOG_FILE"
else
echo "$TIMESTAMP: $SERVER is down!" >> "$LOG_FILE"
fi
sleep 10
done

