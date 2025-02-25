#!/bin/bash
# This script extracts the "Zone Updates per Second (M)" metric from each scaling log file
# and writes the results to a CSV file.
# For weak scaling logs, it also extracts the baseline resolution (zones per process)
# from a dedicated log line "Baseline zones/process:".
# Usage: ./extract_zone_updates.sh

LOG_DIR="/mnt/home/scouch/cmse822/cmse822-codex-private/agoge/scaling_logs"
# Use a different output file for weak scaling study
OUTPUT_FILE="${LOG_DIR}/zone_updates_weak.csv"

# CSV header
echo "Baseline,Processors,ZoneUpdates(M)" > "${OUTPUT_FILE}"

for log_file in "${LOG_DIR}"/weakScaling_*.log; do
    # Extract processor count from the filename.
    # Filename format: weakScaling_<globalNx>x<globalNy>x<globalNz>_<n>procs.log
    base=$(basename "${log_file}")
    procs=$(echo "${base}" | cut -d'_' -f3 | sed 's/procs\.log//')
    
    # Extract the baseline resolution from the log file.
    # Look for a line like: "Baseline zones/process: <number>"
    baseline=$(grep "Baseline zones/process:" "${log_file}" | sed -E 's/.*Baseline zones\/process:\s*//')
    
    # Fallback: if not found, try to extract from filename (using first dimension)
    if [[ -z "${baseline}" ]]; then
        resolution_with_x=$(echo "${base}" | cut -d'_' -f2)
        baseline=$(echo "${resolution_with_x}" | cut -d'x' -f1)
    fi

    # Extract the "Zone Updates per Second" value.
    zone_updates=$(grep "Zone Updates per Second" "${log_file}" | sed -E 's/.*Zone Updates per Second \(M\):\s*//')
    
    # Write a CSV record if zone_updates was found.
    if [[ -n "${zone_updates}" && -n "${baseline}" ]]; then
        echo "${baseline},${procs},${zone_updates}" >> "${OUTPUT_FILE}"
    fi
done

echo "CSV file created at: ${OUTPUT_FILE}"
