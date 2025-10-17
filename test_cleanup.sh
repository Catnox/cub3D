#!/bin/bash
./cub3D maps/test_simple.cub &
PID=$!
sleep 1
# Sende ESC-Taste
xdotool search --pid $PID key Escape 2>/dev/null
sleep 0.5
wait $PID
