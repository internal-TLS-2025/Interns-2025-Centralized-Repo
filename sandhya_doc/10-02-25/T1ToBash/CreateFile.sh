#!/usr/bin/bash

echo "Enter the name of the file you want to create:"
read FILENAME

touch "$FILENAME"
echo "File '$FILENAME' created successfully."
