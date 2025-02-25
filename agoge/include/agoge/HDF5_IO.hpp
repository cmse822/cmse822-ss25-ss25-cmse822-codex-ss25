#pragma once

#include "Field3d.hpp"
#include <string>

/**
 * @file HDF5_IO.hpp
 * @brief Declarations for HDF5 read/write routines in the Agoge project.
 *
 * Extra metadata (global_bbox, global_dimensions, mpi_decomposition, 
 * subdomain, local_dimensions, and cell_size) are stored in the /grid attribute group.
 */

namespace agoge {
namespace io {

/**
 * @brief Write the fields (`rho`, `rhou`, `rhov`, `rhow`, `E`, `phi`) from a
 * Field3D object to an HDF5 file.
 *
 * The data is written as 3D datasets of shape (Nz, Ny, Nx) in row-major
 * order, meaning index (k, j, i) is contiguous in the innermost dimension i.
 *
 * Optionally, domain-size attributes (Nx, Ny, Nz, dx, dy, dz) are also stored.
 *
 * @param[in] Q         The Field3D data structure to write.
 * @param[in] filename  The output HDF5 file name.
 */
void writeFieldHDF5(const Field3D &Q, const std::string &filename);

/**
 * @brief (Optional) Read the fields (`rho`, `rhou`, `rhov`, `rhow`, `E`, `phi`)
 * from an HDF5 file into a Field3D object.
 *
 * This function expects the same shape (Nz, Ny, Nx) that was used when writing.
 * If domain-size attributes are present, it can optionally check them.
 *
 * @param[out] Q         The Field3D data structure to populate.
 * @param[in]  filename  The input HDF5 file name.
 */
void readFieldHDF5(Field3D &Q, const std::string &filename);

/**
 * @brief Performs field I/O using HDF5, automatically naming the output
 *        directory and file using zero-padded epoch and rank.
 *
 * @param Q The Field3D data to write.
 * @param problemName The problem name.
 * @param rank The MPI process rank.
 */
void performFieldIO(const Field3D &Q, const std::string &problemName, int rank);

} // namespace io
} // namespace agoge
