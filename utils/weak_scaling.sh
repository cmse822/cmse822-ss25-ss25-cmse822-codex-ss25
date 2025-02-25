#!/bin/bash
#
# This script conducts a weak scaling study with agoge_run.
# For a fixed number of zones per process (baseline), the global grid dimensions
# are computed from the processor count by factoring np into Px, Py, and Pz.
# Each process will then get exactly (baseline)^3 zones.
# The Sedov.yaml file is updated accordingly.
#
# Usage: ./weak_scaling.sh

# Set variables for paths
YAML_FILE="/mnt/home/scouch/cmse822/cmse822-codex-private/agoge/problems/Sedov.yaml"
EXECUTABLE="/mnt/home/scouch/cmse822/cmse822-codex-private/agoge/agoge_run"
LOG_DIR="/mnt/home/scouch/cmse822/cmse822-codex-private/agoge/scaling_logs"

# Create log directory if it doesn't exist
mkdir -p "${LOG_DIR}"

# Backup the original Sedov.yaml
cp "${YAML_FILE}" "${YAML_FILE}.bak"

# List of baseline resolutions (zones per process along one dimension)
baselines=(8 16 32 64 128 256 512)
# List of process counts (in powers of 2)
procs=(1 2 4 8 16 32 64 128)

for baseline in "${baselines[@]}"; do
    echo "====== Baseline zones per process: ${baseline} ======"
    for np in "${procs[@]}"; do
        # Compute a factorization of np into (Px, Py, Pz) using a Python snippet.
        factors=$(python3 -c "import math, sys
np_val = int(sys.argv[1])
cube = int(round(np_val ** (1/3)))
Px = 1
for i in range(cube, 0, -1):
    if np_val % i == 0:
        Px = i
        break
rem = np_val // Px
sq = int(round(math.sqrt(rem)))
Py = 1
for i in range(sq, 0, -1):
    if rem % i == 0:
        Py = i
        break
Pz = rem // Py
print(Px, Py, Pz)" "$np")
        read Px Py Pz <<< "$factors"
        # Compute global grid dimensions so that each process receives exactly baseline zones in each direction.
        NX=$(( baseline * Px ))
        NY=$(( baseline * Py ))
        NZ=$(( baseline * Pz ))
        
        LOG_FILE="${LOG_DIR}/weakScaling_${NX}x${NY}x${NZ}_${np}procs.log"
        echo "-----------------------------------------------" > "${LOG_FILE}"
        echo "Global Grid: ${NX}x${NY}x${NZ}, Processors: ${np}" >> "${LOG_FILE}"
        echo "Baseline zones/process: ${baseline}" >> "${LOG_FILE}"
        echo "Decomposition: Px=${Px}, Py=${Py}, Pz=${Pz}" >> "${LOG_FILE}"
        echo "Timestamp: $(date)" >> "${LOG_FILE}"
        echo "-----------------------------------------------" >> "${LOG_FILE}"
        
        echo "Running grid ${NX}x${NY}x${NZ} on ${np} procs. Output will be saved to ${LOG_FILE}"
        # Update the Sedov.yaml file with the new global grid dimensions.
        sed -i -E "s/^(nx:)[[:space:]]*[0-9]+/\1 ${NX}/" "${YAML_FILE}"
        sed -i -E "s/^(ny:)[[:space:]]*[0-9]+/\1 ${NY}/" "${YAML_FILE}"
        sed -i -E "s/^(nz:)[[:space:]]*[0-9]+/\1 ${NZ}/" "${YAML_FILE}"
    
        mpiexec -n "${np}" "${EXECUTABLE}" "${YAML_FILE}" >> "${LOG_FILE}" 2>&1
    done
done

# Restore the original Sedov.yaml
mv "${YAML_FILE}.bak" "${YAML_FILE}"
echo "Weak scaling study complete. Logs are in ${LOG_DIR}."