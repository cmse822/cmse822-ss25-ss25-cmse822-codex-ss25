/// @file Field3d.cpp
/// @brief Implementation of the Field3D class for the Agoge application.
///
/// This file provides method definitions for spatial coordinate calculations,
/// boundary conditions application, and MPI halo exchange for 3D fields.

#include "Field3d.hpp"

#include <mpi.h>

#include <algorithm>
#include <iostream>

namespace agoge {

Field3D::Field3D(int nx, int ny, int nz, const BoundingBox &bbox_in, int ghost)
    : Nx(nx),
      Ny(ny),
      Nz(nz),
      nghost(ghost),
      dx((bbox_in.xmax - bbox_in.xmin) / nx),
      dy((bbox_in.ymax - bbox_in.ymin) / ny),
      dz((bbox_in.zmax - bbox_in.zmin) / nz),
      bbox(bbox_in),  // Initialize BoundingBox member
      bc_xmin(config::BoundaryCondition::PERIODIC),
      bc_xmax(config::BoundaryCondition::PERIODIC),
      bc_ymin(config::BoundaryCondition::PERIODIC),
      bc_ymax(config::BoundaryCondition::PERIODIC),
      bc_zmin(config::BoundaryCondition::PERIODIC),
      bc_zmax(config::BoundaryCondition::PERIODIC) {
    NxGhost = Nx + 2 * nghost;
    NyGhost = Ny + 2 * nghost;
    NzGhost = Nz + 2 * nghost;

    size_t totalSize = NxGhost * NyGhost * NzGhost;
    rho.resize(totalSize, 0.0);
    rhou.resize(totalSize, 0.0);
    rhov.resize(totalSize, 0.0);
    rhow.resize(totalSize, 0.0);
    E.resize(totalSize, 0.0);
    phi.resize(totalSize, 0.0);
}

//======================================================
// Spatial coordinate methods
//======================================================
// The left edge of the interior domain is at the start of cell i=0 =>
// if you want a global offset X0, define it as a member.
// For now, assume X0=0.
// Cell iIn's center => (iIn+0.5)*dx.
// We do NOT add nghost here because that's an array index offset only.
// If the physical domain truly starts at X0, then:
//   x= X0 + (iIn+0.5)*dx
// That ensures iIn in [0..Nx-1] => x in [0.5*dx.. Nx-0.5]*dx.

double Field3D::xCenter(int iIn) const { return bbox.xmin + (iIn + 0.5) * dx; }
double Field3D::xLeftEdge(int iIn) const { return bbox.xmin + iIn * dx; }
double Field3D::xRightEdge(int iIn) const { return bbox.xmin + (iIn + 1) * dx; }

double Field3D::yCenter(int jIn) const { return bbox.ymin + (jIn + 0.5) * dy; }
double Field3D::yLeftEdge(int jIn) const { return bbox.ymin + jIn * dy; }
double Field3D::yRightEdge(int jIn) const { return bbox.ymin + (jIn + 1) * dy; }

double Field3D::zCenter(int kIn) const { return bbox.zmin + (kIn + 0.5) * dz; }
double Field3D::zLeftEdge(int kIn) const { return bbox.zmin + kIn * dz; }
double Field3D::zRightEdge(int kIn) const { return bbox.zmin + (kIn + 1) * dz; }

/// @brief copy boundary or wrap for periodic
static inline void copyCell(std::vector<double> &arr, int dst, int src) {
    arr[dst] = arr[src];
}

void Field3D::applyBCs() {
    // Apply standard physical boundary conditions for each face.
    // (These remain intact for MPI sub-domains on global boundaries.)
    auto fillBC_x = [&](std::vector<double> &arr) {
        // x-min physical BC
        for (int k = 0; k < NzGhost; k++) {
            for (int j = 0; j < NyGhost; j++) {
                for (int iGhost = 0; iGhost < nghost; iGhost++) {
                    int iInterior = nghost;
                    int ghostIdx = index(iGhost, j, k);
                    if (bc_xmin == config::BoundaryCondition::PERIODIC) {
                        int iMirror = Nx + iGhost;
                        if (iMirror >= Nx) iMirror -= Nx;
                        int src = index(iMirror + nghost, j, k);
                        // Physical periodic on global boundary
                        copyCell(arr, ghostIdx, src);
                    } else if (bc_xmin == config::BoundaryCondition::OUTFLOW) {
                        int src = index(iInterior, j, k);
                        copyCell(arr, ghostIdx, src);
                    }
                }
            }
        }
        // x-max physical BC
        for (int k = 0; k < NzGhost; k++) {
            for (int j = 0; j < NyGhost; j++) {
                for (int iGhost = Nx; iGhost < NxGhost; iGhost++) {
                    int iInterior = NxGhost - nghost - 1;
                    int ghostIdx = index(iGhost, j, k);
                    if (bc_xmax == config::BoundaryCondition::PERIODIC) {
                        int iMirror = (iGhost - Nx) + nghost;
                        int src = index(iMirror, j, k);
                        copyCell(arr, ghostIdx, src);
                    } else if (bc_xmax == config::BoundaryCondition::OUTFLOW) {
                        int src = index(iInterior, j, k);
                        copyCell(arr, ghostIdx, src);
                    }
                }
            }
        }
    };
    // Similarly define fillBC_y and fillBC_z for the physical BCs.
    auto fillBC_y = [&](std::vector<double> &arr) {
        // y-min
        for (int k = 0; k < NzGhost; k++) {
            for (int i = 0; i < NxGhost; i++) {
                for (int jGhost = 0; jGhost < nghost; jGhost++) {
                    int jInterior = nghost;
                    int ghostIdx = index(i, jGhost, k);

                    if (bc_ymin == config::BoundaryCondition::PERIODIC) {
                        int jMirror = Ny + jGhost;
                        if (jMirror >= Ny) jMirror -= Ny;
                        int src = index(i, jMirror + nghost, k);
                        copyCell(arr, ghostIdx, src);
                    } else if (bc_ymin == config::BoundaryCondition::OUTFLOW) {
                        int src = index(i, jInterior, k);
                        copyCell(arr, ghostIdx, src);
                    }
                }
            }
        }
        // y-max
        for (int k = 0; k < NzGhost; k++) {
            for (int i = 0; i < NxGhost; i++) {
                for (int jGhost = nghost + Ny; jGhost < NyGhost; jGhost++) {
                    int jInterior = nghost + Ny - 1;
                    int ghostIdx = index(i, jGhost, k);
                    if (bc_ymax == config::BoundaryCondition::PERIODIC) {
                        int jMirror = jGhost - Ny;
                        int src = index(i, jMirror, k);
                        copyCell(arr, ghostIdx, src);
                    } else if (bc_ymax == config::BoundaryCondition::OUTFLOW) {
                        int src = index(i, jInterior, k);
                        copyCell(arr, ghostIdx, src);
                    }
                }
            }
        }
    };

    auto fillBC_z = [&](std::vector<double> &arr) {
        // z-min
        for (int j = 0; j < NyGhost; j++) {
            for (int i = 0; i < NxGhost; i++) {
                for (int kGhost = 0; kGhost < nghost; kGhost++) {
                    int kInterior = nghost;
                    int ghostIdx = index(i, j, kGhost);
                    if (bc_zmin == config::BoundaryCondition::PERIODIC) {
                        int kMirror = Nz + kGhost;
                        if (kMirror >= Nz) kMirror -= Nz;
                        int src = index(i, j, kMirror + nghost);
                        copyCell(arr, ghostIdx, src);
                    } else if (bc_zmin == config::BoundaryCondition::OUTFLOW) {
                        int src = index(i, j, kInterior);
                        copyCell(arr, ghostIdx, src);
                    }
                }
            }
        }
        // z-max
        for (int j = 0; j < NyGhost; j++) {
            for (int i = 0; i < NxGhost; i++) {
                for (int kGhost = nghost + Nz; kGhost < NzGhost; kGhost++) {
                    int kInterior = nghost + Nz - 1;
                    int ghostIdx = index(i, j, kGhost);
                    if (bc_zmax == config::BoundaryCondition::PERIODIC) {
                        int kMirror = kGhost - Nz;
                        int src = index(i, j, kMirror);
                        copyCell(arr, ghostIdx, src);
                    } else if (bc_zmax == config::BoundaryCondition::OUTFLOW) {
                        int src = index(i, j, kInterior);
                        copyCell(arr, ghostIdx, src);
                    }
                }
            }
        }
    };

    fillBC_x(rho);
    fillBC_x(rhou);
    fillBC_x(rhov);
    fillBC_x(rhow);
    fillBC_x(E);
    fillBC_x(phi);
    fillBC_y(rho);
    fillBC_y(rhou);
    fillBC_y(rhov);
    fillBC_y(rhow);
    fillBC_y(E);
    fillBC_y(phi);
    fillBC_z(rho);
    fillBC_z(rhou);
    fillBC_z(rhov);
    fillBC_z(rhow);
    fillBC_z(E);
    fillBC_z(phi);

    // --- Begin MPI halo exchange ---
    // The following applies if there is a valid MPI neighbor,
    // while for global boundaries (neighbor rank == -1) physical BCs remain.

    // Helper lambdas for packing/unpacking x-direction data
    auto packX = [&](std::vector<double> &buf, bool sendPlus) {
        // Hint: Iterate over k and j indices.
        // For each (k, j), determine iStart and iEnd based on sendPlus:
        //   int iStart = sendPlus ? (NxGhost - 2 * nghost) : nghost;
        //   int iEnd = sendPlus ? (NxGhost - nghost) : (2 * nghost);
        // Then, sequentially pack rho, rhou, rhov, rhow, E, and phi into buf.
    };
    auto unpackX = [&](const std::vector<double> &buf, bool plusSide) {
        // Hint: Iterate over k and j indices.
        // For each (k, j), determine iStart and iEnd based on plusSide:
        //   int iStart = plusSide ? (NxGhost - nghost) : 0;
        //   int iEnd = plusSide ? NxGhost : nghost;
        // Then, sequentially unpack the six field values from buf into the corresponding arrays.
    };

    // X-direction using MPI_Sendrecv with two buffers
    if (rankMinusX != MPI_PROC_NULL || rankPlusX != MPI_PROC_NULL) {
        // Hint: Compute totalCount = NyGhost * NzGhost * nghost * 6.
        // Allocate send and receive buffers of that size.
        // First, pack the plus-side data, send it to rankPlusX and receive from rankMinusX,
        // then use unpackX with plusSide set to false.
        // Next, pack the minus-side data, send it to rankMinusX and receive from rankPlusX,
        // then call unpackX with plusSide set to true.
    }

    // --- Y-DIRECTION HALO EXCHANGE USING MPI_Sendrecv ---
    if (rankMinusY != MPI_PROC_NULL || rankPlusY != MPI_PROC_NULL) {
        // Define packY and unpackY lambdas for y-direction
        auto packY = [&](std::vector<double> &buf, bool sendPlus) {
            // Hint: Iterate over k and i indices.
            // Determine jStart and jEnd based on sendPlus:
            //   int jStart = sendPlus ? (NyGhost - 2 * nghost) : nghost;
            //   int jEnd = sendPlus ? (NyGhost - nghost) : (2 * nghost);
            // Pack rho, rhou, rhov, rhow, E, and phi, sequentially for each (k, i) over j range.
        };
        auto unpackY = [&](const std::vector<double> &buf, bool plusSide) {
            // Hint: Iterate over k and i indices.
            // Determine jStart and jEnd based on plusSide:
            //   int jStart = plusSide ? (NyGhost - nghost) : 0;
            //   int jEnd = plusSide ? NyGhost : nghost;
            // Unpack the six field values from buf into the corresponding (k, i) cells.
        };

        // Hint: Compute totalCount = NxGhost * NzGhost * nghost * 6.
        // Allocate send and receive buffers of that size.
        // First, pack plus-side data, use MPI_Sendrecv to exchange with rankPlusY and rankMinusY,
        // then call unpackY with plusSide set to false.
        // Next, pack minus-side data, exchange accordingly, then call unpackY with plusSide set to true.
    }

    // --- Z-DIRECTION HALO EXCHANGE USING MPI_Irecv + MPI_Isend ---
    if (rankMinusZ != MPI_PROC_NULL || rankPlusZ != MPI_PROC_NULL) {
        // Define packZ and unpackZ lambdas for z-direction
        auto packZ = [&](std::vector<double> &buf, bool sendPlus) {
            // Hint: Iterate over i and j indices.
            // Determine kStart and kEnd based on sendPlus:
            //   int kStart = sendPlus ? (NzGhost - 2 * nghost) : nghost;
            //   int kEnd = sendPlus ? (NzGhost - nghost) : (2 * nghost);
            // Pack sequentially the six field values (rho, rhou, rhov, rhow, E, phi) for each (i, j).
        };
        auto unpackZ = [&](const std::vector<double> &buf, bool plusSide) {
            // Hint: Iterate over i and j indices.
            // Determine kStart and kEnd based on plusSide:
            //   int kStart = plusSide ? (NzGhost - nghost) : 0;
            //   int kEnd = plusSide ? NzGhost : nghost;
            // Unpack the six field values from buf into the appropriate (i, j) cells.
        };

        // Hint: Compute totalCount = NxGhost * NyGhost * nghost * 6.
        // Allocate send and receive buffers of that size.
        // First, pack plus-side data, perform MPI_Sendrecv sending to rankPlusZ and receiving from rankMinusZ,
        // then call unpackZ with plusSide set to false.
        // Next, pack minus-side data, exchange with rankMinusZ and rankPlusZ using MPI_Sendrecv,
        // then call unpackZ with plusSide set to true.
    }
}

}  // namespace agoge
