#!/bin/bash
#
# This script conducts a strong scaling study with agoge_run.
# It updates problems/Sedov.yaml for grid resolutions from 8^3 to 512^3
# and then runs mpiexec jobs for process counts (1, 2, 4, …, 128).
# Standard output (with metadata) from each run is saved to corresponding log files.
#
# Usage: ./strong_scaling.sh

# set variables for paths
YAML_FILE="/mnt/home/scouch/cmse822/cmse822-codex-private/agoge/problems/Sedov.yaml"
EXECUTABLE="/mnt/home/scouch/cmse822/cmse822-codex-private/agoge/agoge_run"
LOG_DIR="/mnt/home/scouch/cmse822/cmse822-codex-private/agoge/scaling_logs"

# Create log directory if it doesn't exist
mkdir -p "${LOG_DIR}"

# Backup the original Sedov.yaml
cp "${YAML_FILE}" "${YAML_FILE}.bak"

# List of grid resolutions (cube root values) to test
resolutions=(8 16 32 64 128 256 512)
# List of process counts (in powers of 2)
procs=(1 2 4 8 16 32 64 128)

for res in "${resolutions[@]}"; do
    echo "====== Updating grid resolution to ${res}x${res}x${res} ======"
    # Use sed to update nx, ny, nz lines in the YAML file
    sed -i -E "s/^(nx:)[[:space:]]*[0-9]+/\1 ${res}/" "${YAML_FILE}"
    sed -i -E "s/^(ny:)[[:space:]]*[0-9]+/\1 ${res}/" "${YAML_FILE}"
    sed -i -E "s/^(nz:)[[:space:]]*[0-9]+/\1 ${res}/" "${YAML_FILE}"
    
    for np in "${procs[@]}"; do
        LOG_FILE="${LOG_DIR}/scaling_${res}x${res}x${res}_${np}procs.log"
        echo "-----------------------------------------------" > "${LOG_FILE}"
        echo "Grid: ${res}^3, Processors: ${np}" >> "${LOG_FILE}"
        echo "Timestamp: $(date)" >> "${LOG_FILE}"
        echo "-----------------------------------------------" >> "${LOG_FILE}"
        
        echo "Running grid ${res}x${res}x${res} with ${np} procs. Output will be saved to ${LOG_FILE}"
        mpiexec -n "${np}" "${EXECUTABLE}" "${YAML_FILE}" >> "${LOG_FILE}" 2>&1
    done
done

# Restore the original Sedov.yaml
mv "${YAML_FILE}.bak" "${YAML_FILE}"
echo "Scaling study complete. Logs are in ${LOG_DIR}."
