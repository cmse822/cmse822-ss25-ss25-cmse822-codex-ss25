"""
@file vds_builder.py
@brief Builds an HDF5 Virtual Data Set (VDS) that aggregates output files from MPI ranks.
Uses h5py's VDS API.
"""

import os
import h5py
import numpy as np
import glob

def get_metadata(file_path):
    with h5py.File(file_path, 'r') as f:
        grid = f["/grid"]
        local_dims = np.array(grid.attrs["local_dimensions"], dtype=int)
        subdomain = np.array(grid.attrs["subdomain"], dtype=int)
    return local_dims, subdomain

def build_vds(input_dir, output_vds_file, field="rho"):
    # Assume file naming pattern: "output/<problem>_out_XXXX/rank_XXXX.h5"
    file_pattern = os.path.join(input_dir, "*", "rank_*.h5")
    rank_files = glob.glob(file_pattern)
    if not rank_files:
        raise RuntimeError("No rank HDF5 files found in {}".format(input_dir))
    
    # Read a representative file for global metadata (assumed identical in all files).
    with h5py.File(rank_files[0], 'r') as f:
        grid = f["/grid"]
        global_dims = np.array(grid.attrs["global_dimensions"], dtype=int)
    global_shape = tuple(global_dims)  # (Nx, Ny, Nz)
    
    # Create virtual layout. In our files, data is stored as shape (Nz, Ny, Nx)
    layout = h5py.VirtualLayout(shape=(global_shape[2], global_shape[1], global_shape[0]), dtype='float64')
    
    # Iterate over rank files and map them into the layout.
    for file in rank_files:
        local_dims, subdomain = get_metadata(file)
        # Compute offset in global dataset for each rank.
        # Assume a consistent subdomain-to-global mapping:
        # For example, offset_x = subdomain[0] * local_dims[0]
        # (extend this to y and z accordingly)
        offset = ( subdomain[0] * local_dims[0],
                   subdomain[1] * local_dims[1],
                   subdomain[2] * local_dims[2] )
        # VirtualSource shape matches the local interior field shape,
        # but note: our rank files store data as (Nz, Ny, Nx)
        vsource = h5py.VirtualSource(file, field, shape=(local_dims[2], local_dims[1], local_dims[0]))
        layout[offset[2]:offset[2]+local_dims[2],
               offset[1]:offset[1]+local_dims[1],
               offset[0]:offset[0]+local_dims[0]] = vsource

    # Create the VDS file and write the layout.
    with h5py.File(output_vds_file, 'w', libver='latest') as f:
        f.create_virtual_dataset(field, layout)
    
    print("VDS created at:", output_vds_file)

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description="Build HDF5 Virtual Data Set from MPI rank files.")
    parser.add_argument("--input-dir", required=True, help="Directory containing output rank files.")
    parser.add_argument("--output", default="global_vds.h5", help="Output VDS file name")
    parser.add_argument("--field", default="rho", help="Field name to map (default: rho)")
    args = parser.parse_args()
    build_vds(args.input_dir, args.output, args.field)
