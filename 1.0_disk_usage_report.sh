#!/bin/bash
# Disk Space Usage Reporter

echo -e "\n--- System Disk Usage Report ---"
echo "Target: Root File System (/)"


echo -e "\n-----------------------------------------------------"
printf "%-10s %-10s %-10s %-10s\n" "TOTAL" "USED" "AVAILABLE" "USAGE %"
echo "-----------------------------------------------------"

df -h | grep ' /$' | awk '{printf "%-10s %-10s %-10s %-10s\n", $2, $3, $4, $5}'

echo -e "-----------------------------------------------------\n"

USAGE_PERCENT=$(df / | tail -1 | awk '{print $5}' | sed 's/%//g')

if [ "$USAGE_PERCENT" -gt 90 ]; then
    echo "WARNING: Disk usage is over 90%. Consider cleaning up old files!"
elif [ "$USAGE_PERCENT" -gt 75 ]; then
    echo "ALERT: Disk usage is over 75%. Monitor closely."
else
    echo "STATUS: Disk space is healthy."
fi
