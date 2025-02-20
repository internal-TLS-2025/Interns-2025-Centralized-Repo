#!/usr/bin/env bash

echo "Which directory do you want to visit?"
read DIR

if [ ! -d "$DIR" ]; then
    echo "No such directory"
else
    cd "$DIR" || echo "Failed to change directory"
    pwd
fi
