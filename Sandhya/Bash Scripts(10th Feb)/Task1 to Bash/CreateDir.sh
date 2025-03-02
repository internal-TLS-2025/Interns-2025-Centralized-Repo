#!/usr/bin/bash

echo "Directory Name?"
read DIR
#DIR="TestDir"

if [ ! -d "$DIR" ]; then
       mkdir "$DIR"
    echo "Directory $DIR created."
else
    echo "Directory $DIR already exists."
fi

