#!/bin/bash

# Access and display environment variables
echo "Current User: $USER"
echo "Home Directory: $HOME"
echo "Current Shell: $SHELL"
echo "Current Working Directory: $PWD"

# Check disk usage in the home directory
echo "Disk Usage in Home Directory:"
du -sh "$HOME"

# Set a custom environment variable
export MY_VARIABLE="Hello, $USER! Welcome to your home directory."

# Access the custom environment variable
echo "$MY_VARIABLE"
