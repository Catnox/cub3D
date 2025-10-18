#!/bin/bash
# Simplified script to extract valgrind suppressions without gawk dependencies
# Usage: ./gawkscript.sh <valgrind_log_file>

# Initialize variables
in_suppression=0
current_suppression=""
declare -A seen_suppressions

# Check if input file is provided
if [[ $# -eq 0 ]]; then
    echo "Usage: $0 <valgrind_log_file>"
    exit 1
fi

if [[ ! -f "$1" ]]; then
    echo "Error: File '$1' not found"
    exit 1
fi

# Read the input file
while IFS= read -r line; do
    # Check if line starts with '{'
    if [[ "$line" =~ ^{.*$ ]]; then
        in_suppression=1
        current_suppression="$line"$'\n'
    # Check if line starts with '}'
    elif [[ "$line" =~ ^}.*$ ]] && [[ $in_suppression -eq 1 ]]; then
        in_suppression=0
        current_suppression+="$line"
        
        # Generate a simple hash for deduplication
        hash=$(echo -n "$current_suppression" | md5sum | cut -d' ' -f1)
        
        # If we haven't seen this suppression before, print it
        if [[ -z "${seen_suppressions[$hash]}" ]]; then
            seen_suppressions[$hash]=1
            echo -e "$current_suppression"
            echo  # Add empty line between suppressions
        fi
        
        current_suppression=""
    # If we're in a suppression, add the line
    elif [[ $in_suppression -eq 1 ]]; then
        current_suppression+="$line"$'\n'
    fi
done < "$1"