#!/usr/bin/bash

echo "Enter the filename you want to tail:"
read FILENAME

if [ -f "$FILENAME" ]; then
    echo "Tail of '$FILENAME':"
    tail -f  "$FILENAME"
else
    echo "File '$FILENAME' does not exist."
fi
