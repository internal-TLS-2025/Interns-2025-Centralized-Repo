#!/usr/bin/bash

echo "Enter the source filename:"
read SOURCE

echo "Enter the destination filename:"
read DESTINATION

if [ -f "$SOURCE" ]; then
    cp "$SOURCE" "$DESTINATION"
    echo "File '$SOURCE' copied to '$DESTINATION' successfully."
else
    echo "Source file '$SOURCE' does not exist."
fi
