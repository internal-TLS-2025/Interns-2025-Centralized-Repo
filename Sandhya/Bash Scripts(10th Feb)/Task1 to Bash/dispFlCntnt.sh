#!/usr/bin/bash

echo "Enter the filename you want to display the content of:"
read FILENAME

if [ -f "$FILENAME" ]; then
    echo "Displaying content of '$FILENAME':"
    cat "$FILENAME"
else
    echo "File '$FILENAME' does not exist."
fi
