#!/usr/bin/bash

echo "Enter the name of the file or directory you want to remove:"
read ITEM

if [ -e "$ITEM" ]; then
    rm -r "$ITEM"
    echo "'$ITEM' has been removed successfully."
else
    echo "File or directory '$ITEM' does not exist."
fi
