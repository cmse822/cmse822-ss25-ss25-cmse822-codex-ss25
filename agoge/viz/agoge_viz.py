import glob
import h5py
import numpy as np
import os
import math
import matplotlib.pyplot as plt
import argparse

def read_directory(directory, field_name="rho"):
    """
    Reads all HDF5 files in the given directory, reconstructs the global field 
    using the global and subdomain metadata stored in the /grid group.
    Assumes each file has attributes:
      - "global_bbox": [xmin, xmax, ymin, ymax, zmin, zmax]
      - "global_dimensions": [global_Nx, global_Ny, global_Nz]
      - "cell_size": [dx, dy, dz]
      - "Px", "Py", "Pz": integers
      - "subdomain": [sx, sy, sz] (the subdomain indices for this file)
      - "local_dimensions": [local_Nx, local_Ny, local_Nz] (the interior sizes for this rank)
    """
    # If a VDS file exists (e.g., global_vds.h5), use that.
    vds_file = os.path.join(directory, "global_vds.h5")
    if os.path.exists(vds_file):
        print("Using VDS file:", vds_file)
        with h5py.File(vds_file, 'r') as f:
            dataset = f[field_name]
            global_field = np.array(dataset)
            # Read global attributes from the VDS if stored, or set defaults.
            # For simplicity, assume global dimensions are in the dataset shape.
            global_Nz, global_Ny, global_Nx = global_field.shape
            # Dummy coordinate arrays (you may want to read these from metadata)
            xmin, xmax = 0, global_Nx
            ymin, ymax = 0, global_Ny
            zmin, zmax = 0, global_Nz
            dx = (xmax - xmin) / global_Nx
            dy = (ymax - ymin) / global_Ny
            dz = (zmax - zmin) / global_Nz
            x_coords = np.array([xmin + (i + 0.5) * dx for i in range(global_Nx)])
            y_coords = np.array([ymin + (j + 0.5) * dy for j in range(global_Ny)])
            z_coords = np.array([zmin + (k + 0.5) * dz for k in range(global_Nz)])
        return {"field": global_field, "x": x_coords, "y": y_coords, "z": z_coords}
    
    # Fallback: scan individual HDF5 files in directory.
    file_list = glob.glob(os.path.join(directory, "*.h5"))
    if not file_list:
        raise RuntimeError("No HDF5 files found in the directory.")

    # Read metadata from the first file (assumed identical global settings)
    with h5py.File(file_list[0], 'r') as f:
        grid = f["/grid"]
        global_bbox = np.array(grid.attrs["global_bbox"], dtype=float)
        global_dims = np.array(grid.attrs["global_dimensions"], dtype=int)
        cell_size = np.array(grid.attrs["cell_size"], dtype=float)
        Px = int(grid.attrs["Px"])
        Py = int(grid.attrs["Py"])
        Pz = int(grid.attrs["Pz"])
    global_Nx, global_Ny, global_Nz = global_dims
    # Pre-allocate global array
    global_field = np.zeros((global_Nz, global_Ny, global_Nx))

    # Compute remainder for offsets (assume uniform division with remainders)
    remX = global_Nx % Px
    remY = global_Ny % Py
    remZ = global_Nz % Pz
    baseX = global_Nx // Px
    baseY = global_Ny // Py
    baseZ = global_Nz // Pz

    for filename in file_list:
        with h5py.File(filename, 'r') as f:
            grid = f["/grid"]
            # Local field data is stored in dataset <field_name> in root group
            local_field = np.array(f[field_name])
            # Get local dimensions from attribute "local_dimensions" 
            local_dims = np.array(grid.attrs["local_dimensions"], dtype=int)
            lx, ly, lz = local_dims  # local interior size along x, y, z
            # Get subdomain indices [sx, sy, sz]
            subdomain = np.array(grid.attrs["subdomain"], dtype=int)
            sx, sy, sz = subdomain

            # Compute offset in global array:
            offset_x = sx * baseX + min(sx, remX)
            offset_y = sy * baseY + min(sy, remY)
            offset_z = sz * baseZ + min(sz, remZ)
            # local_field is assumed ordered as (lz, ly, lx)
            global_field[offset_z:offset_z+lz, offset_y:offset_y+ly, offset_x:offset_x+lx] = local_field

    # Compute coordinate arrays using global_bbox and cell_size
    xmin, xmax, ymin, ymax, zmin, zmax = global_bbox
    dx, dy, dz = cell_size
    x_coords = np.array([xmin + (i + 0.5) * dx for i in range(global_Nx)])
    y_coords = np.array([ymin + (j + 0.5) * dy for j in range(global_Ny)])
    z_coords = np.array([zmin + (k + 0.5) * dz for k in range(global_Nz)])

    return {"field": global_field, "x": x_coords, "y": y_coords, "z": z_coords}

def plot_field(field_data, x, y, z, axis='z', index=None, field_name="Field", vmin=None, vmax=None):
    if axis.lower() == 'z':
        if index is None:
            index = field_data.shape[0] // 2
        slice_data = field_data[index, :, :]
        X, Y = np.meshgrid(x, y, indexing='xy')
        plt.figure()
        plt.contourf(X, Y, slice_data, cmap="viridis", vmin=vmin, vmax=vmax)
        plt.xlabel("x")
        plt.ylabel("y")
        plt.title(f"{field_name} at z-slice index {index}")
        plt.colorbar()
    elif axis.lower() == 'y':
        if index is None:
            index = field_data.shape[1] // 2
        slice_data = field_data[:, index, :]
        X, Z = np.meshgrid(x, z, indexing='xy')
        plt.figure()
        plt.contourf(X, Z, slice_data, cmap="viridis", vmin=vmin, vmax=vmax)
        plt.xlabel("x")
        plt.ylabel("z")
        plt.title(f"{field_name} at y-slice index {index}")
        plt.colorbar()
    elif axis.lower() == 'x':
        if index is None:
            index = field_data.shape[2] // 2
        slice_data = field_data[:, :, index]
        Y, Z = np.meshgrid(y, z, indexing="xy")
        plt.figure()
        plt.contourf(Y, Z, slice_data, cmap="viridis", vmin=vmin, vmax=vmax)
        plt.xlabel("y")
        plt.ylabel("z")
        plt.title(f"{field_name} at x-slice index {index}")
        plt.colorbar()
    else:
        raise ValueError("Axis must be one of 'x', 'y', or 'z'")
    plt.show()

def plot_line(x, data_list, labels, title="", xlabel="x", ylabel="Field Value", styles=None):
    """
    Plot 1D line data where x is common for each dataset.

    Parameters:
        x (array-like): 1D coordinate array.
        data_list (list of array-like): List of 1D data arrays to plot.
        labels (list of str): Labels for each line.
        title (str): Plot title.
        xlabel (str): x-axis label.
        ylabel (str): y-axis label.
        styles (list of str, optional): Matplotlib style formats (e.g., 'r-', 'bo--').
    """
    plt.figure(figsize=(8, 5))
    for i, data in enumerate(data_list):
        if styles is not None and i < len(styles):
            plt.plot(x, data, styles[i], linewidth=2, label=labels[i])
        else:
            plt.plot(x, data, linewidth=2, label=labels[i])
    plt.title(title, fontsize=14)
    plt.xlabel(xlabel, fontsize=12)
    plt.ylabel(ylabel, fontsize=12)
    plt.legend(fontsize=10)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.tight_layout()
    plt.show()

# New: Compare global datasets from two directories.
def compare_global_datasets(dir1, dir2, field="rho"):
    data1 = read_directory(dir1, field_name=field)
    data2 = read_directory(dir2, field_name=field)
    field1 = data1["field"]
    field2 = data2["field"]

    # Compute absolute error
    abs_error = np.abs(field1 - field2)
    # Compute relative error with epsilon to avoid division by zero
    epsilon = 1e-12
    rel_error = abs_error / (np.maximum(np.abs(field1), np.abs(field2)) + epsilon)

    print("Comparison of global datasets for field '%s':" % field)
    print("Max absolute error: ", np.max(abs_error))
    print("Mean absolute error:", np.mean(abs_error))
    print("Max relative error: ", np.max(rel_error))
    print("Mean relative error:", np.mean(rel_error))

    return abs_error, rel_error

def main():
    parser = argparse.ArgumentParser(
        description="Reconstruct and visualize global Agoge field data."
    )
    parser.add_argument("directory", nargs="?", help="Directory containing HDF5 files (or global_vds.h5)")
    parser.add_argument("--compare", nargs=2, metavar=("DIR1", "DIR2"), 
                        help="Compare global fields from two directories")
    parser.add_argument("--field", default="rho", help="Field name to visualize or compare")
    parser.add_argument("--axis", default="z", choices=["x", "y", "z"], help="Slicing axis")
    parser.add_argument("--index", type=int, default=None, help="Slice index")
    args = parser.parse_args()

    # New: If compare flag is provided, perform comparison and exit.
    if args.compare:
        compare_global_datasets(args.compare[0], args.compare[1], field=args.field)
        return

    if not args.directory:
        print("Error: Directory not specified.")
        return

    data = read_directory(args.directory, field_name=args.field)
    global_field = data["field"]
    x_coords = data["x"]
    y_coords = data["y"]
    z_coords = data["z"]

    if global_field is None:
        print("Error: Could not reconstruct global field data.")
        return

    plot_field(global_field, x_coords, y_coords, z_coords,
               axis=args.axis, index=args.index, field_name=args.field)

if __name__ == "__main__":
    main()