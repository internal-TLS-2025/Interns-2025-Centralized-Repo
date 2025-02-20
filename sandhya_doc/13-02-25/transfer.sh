#!/bin/bash

# Define Variables
DSU_HOST="10.113.50.100"
PDL_HOST="root.pdl"
LOCAL_USER="t0314996"
LOCAL_MACHINE="10.113.53.85"
LOCAL_PATH="/home/t0314996"
TEMP_PATH="/tmp"
OUTPUT_FILE="ot.txt"

# Step 1: SSH into the DSU Host
ssh "$LOCAL_USER"@"$DSU_HOST" << EOF
    # Step 2: Switch to root user
    su - << EOR
        # Step 3: Enter DSU and create a file
        DSU1
        ifconfig > $OUTPUT_FILE
        
        # Step 4: Transfer the file from DSU to PDL
        scp ./$OUTPUT_FILE $PDL_HOST:$TEMP_PATH
        
        # Step 5: Exit to go back to the normal user
        exit
    EOR
EOF

# Step 6: Transfer file from PDL to local machine
scp "$PDL_HOST:$TEMP_PATH/$OUTPUT_FILE" "$LOCAL_USER"@"$LOCAL_MACHINE:$LOCAL_PATH"

# Optional: Notify the user that the process is complete
echo "File transfer completed. The file is now in $LOCAL_PATH on your local machine."
