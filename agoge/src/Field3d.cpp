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

/// @brief Determine MPI tags for sending and receiving data between neighboring
/// ranks.
///
/// This function helps to avoid tag collisions by assigning unique tags based
/// on the ranks of the current process (myRank) and its neighbor
/// (neighborRank). The base value is used to differentiate between different
/// types of communications (e.g., x, y, z directions).
///
/// @param myRank The rank of the current process.
/// @param neighborRank The rank of the neighboring process.
/// @param base The base value for the tag to differentiate communication types.
/// @return A pair of integers representing the send and receive tags.
// inline std::pair<int, int> getTags(int myRank, int neighborRank, int base) {
//     // ...obsolete code, remove...
// }

// Simple symmetric tag computation:
//   base = direction_offset + phase_offset + (min(myRank,neighborRank)*10 +
//   max(myRank,neighborRank))
// where for example, for 'Z': direction_offset = 20000, phase0 -> phase_offset
// = 0, phase1 -> phase_offset = 100. Then if (myRank < neighborRank) return
// {base+1, base+2}; else return {base+2, base+1};
inline std::pair<int, int> computeSymmetricTagPair(int myRank, int neighborRank,
                                                   char direction, int phase) {
    int directionOffset = 0;
    if (direction == 'X') {
        directionOffset = 0;
    } else if (direction == 'Y') {
        directionOffset = 10000;
    } else if (direction == 'Z') {
        directionOffset = 20000;
    }
    int phaseOffset = (phase == 0) ? 0 : 100;
    int base =
        directionOffset + phaseOffset +
        (std::min(myRank, neighborRank) * 10 + std::max(myRank, neighborRank));
    if (myRank < neighborRank) {
        return {base + 1, base + 2};  // lower rank sends with (base+1), higher
                                      // receives with (base+2)
    } else if (myRank > neighborRank) {
        return {base + 2, base + 1};  // higher rank sends with (base+2), lower
                                      // receives with (base+1)
    } else {
        return {base + 1, base + 1};  // self-communication (unlikely)
    }
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

    // Remove old x-direction exchange; replace with ring-shift approach.

    // Helper lambdas for packing/unpacking x-direction data
    auto packX = [&](std::vector<double> &buf, bool sendPlus) {
        int cnt = 0;
        for (int k = 0; k < NzGhost; k++) {
            for (int j = 0; j < NyGhost; j++) {
                int iStart = sendPlus ? (NxGhost - 2 * nghost) : nghost;
                int iEnd = sendPlus ? (NxGhost - nghost) : (2 * nghost);
                for (int i = iStart; i < iEnd; i++) {
                    buf[cnt++] = rho[index(i, j, k)];
                    buf[cnt++] = rhou[index(i, j, k)];
                    buf[cnt++] = rhov[index(i, j, k)];
                    buf[cnt++] = rhow[index(i, j, k)];
                    buf[cnt++] = E[index(i, j, k)];
                    buf[cnt++] = phi[index(i, j, k)];
                }
            }
        }
    };
    auto unpackX = [&](const std::vector<double> &buf, bool plusSide) {
        int cnt = 0;
        for (int k = 0; k < NzGhost; k++) {
            for (int j = 0; j < NyGhost; j++) {
                int iStart = plusSide ? (NxGhost - nghost) : 0;
                int iEnd = plusSide ? NxGhost : nghost;
                for (int i = iStart; i < iEnd; i++) {
                    rho[index(i, j, k)] = buf[cnt++];
                    rhou[index(i, j, k)] = buf[cnt++];
                    rhov[index(i, j, k)] = buf[cnt++];
                    rhow[index(i, j, k)] = buf[cnt++];
                    E[index(i, j, k)] = buf[cnt++];
                    phi[index(i, j, k)] = buf[cnt++];
                }
            }
        }
    };

    // X-direction using MPI_Sendrecv with two buffers
    if (rankMinusX != MPI_PROC_NULL || rankPlusX != MPI_PROC_NULL) {
        const int numArrays = 6;
        int planeSize = NyGhost * NzGhost * nghost;
        int totalCount = planeSize * numArrays;
        std::vector<double> sendBuf(totalCount), recvBuf(totalCount);
        int tag = 0;
        // Phase 1: send plus-side data, receive from minus-side
        packX(sendBuf, true);
        MPI_Sendrecv(sendBuf.data(), totalCount, MPI_DOUBLE, rankPlusX, tag,
                     recvBuf.data(), totalCount, MPI_DOUBLE, rankMinusX, tag,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        unpackX(recvBuf, false);

        // Phase 2: send minus-side data, receive from plus-side
        packX(sendBuf, false);
        MPI_Sendrecv(sendBuf.data(), totalCount, MPI_DOUBLE, rankMinusX, tag,
                     recvBuf.data(), totalCount, MPI_DOUBLE, rankPlusX, tag,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        unpackX(recvBuf, true);
    }

    // --- Y-DIRECTION HALO EXCHANGE USING MPI_Sendrecv ---
    if (rankMinusY != MPI_PROC_NULL || rankPlusY != MPI_PROC_NULL) {
        // Define packY and unpackY lambdas for y-direction
        auto packY = [&](std::vector<double> &buf, bool sendPlus) {
            int cnt = 0;
            for (int k = 0; k < NzGhost; k++) {
                for (int i = 0; i < NxGhost; i++) {
                    int jStart = sendPlus ? (NyGhost - 2 * nghost) : nghost;
                    int jEnd = sendPlus ? (NyGhost - nghost) : (2 * nghost);
                    for (int j = jStart; j < jEnd; j++) {
                        buf[cnt++] = rho[index(i, j, k)];
                        buf[cnt++] = rhou[index(i, j, k)];
                        buf[cnt++] = rhov[index(i, j, k)];
                        buf[cnt++] = rhow[index(i, j, k)];
                        buf[cnt++] = E[index(i, j, k)];
                        buf[cnt++] = phi[index(i, j, k)];
                    }
                }
            }
        };
        auto unpackY = [&](const std::vector<double> &buf, bool plusSide) {
            int cnt = 0;
            for (int k = 0; k < NzGhost; k++) {
                for (int i = 0; i < NxGhost; i++) {
                    int jStart = plusSide ? (NyGhost - nghost) : 0;
                    int jEnd = plusSide ? NyGhost : nghost;
                    for (int j = jStart; j < jEnd; j++) {
                        rho[index(i, j, k)] = buf[cnt++];
                        rhou[index(i, j, k)] = buf[cnt++];
                        rhov[index(i, j, k)] = buf[cnt++];
                        rhow[index(i, j, k)] = buf[cnt++];
                        E[index(i, j, k)] = buf[cnt++];
                        phi[index(i, j, k)] = buf[cnt++];
                    }
                }
            }
        };

        const int numArrays = 6;
        int planeSize = NxGhost * NzGhost * nghost;
        int totalCount = planeSize * numArrays;
        std::vector<double> sendBuf(totalCount), recvBuf(totalCount);
        int tag = 0;

        // Phase 1: send plus-side data, receive minus-side
        packY(sendBuf, true);
        MPI_Sendrecv(sendBuf.data(), totalCount, MPI_DOUBLE, rankPlusY, tag,
                     recvBuf.data(), totalCount, MPI_DOUBLE, rankMinusY, tag,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        unpackY(recvBuf, false);

        // Phase 2: send minus-side data, receive plus-side
        packY(sendBuf, false);
        MPI_Sendrecv(sendBuf.data(), totalCount, MPI_DOUBLE, rankMinusY, tag,
                     recvBuf.data(), totalCount, MPI_DOUBLE, rankPlusY, tag,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        unpackY(recvBuf, true);
    }

    // --- Z-DIRECTION HALO EXCHANGE USING MPI_Irecv + MPI_Isend ---
    if (rankMinusZ != MPI_PROC_NULL || rankPlusZ != MPI_PROC_NULL) {
        // Define packZ and unpackZ lambdas for z-direction
        auto packZ = [&](std::vector<double> &buf, bool sendPlus) {
            int cnt = 0;
            for (int j = 0; j < NyGhost; j++) {
                for (int i = 0; i < NxGhost; i++) {
                    int kStart = sendPlus ? (NzGhost - 2 * nghost) : nghost;
                    int kEnd = sendPlus ? (NzGhost - nghost) : (2 * nghost);
                    for (int k = kStart; k < kEnd; k++) {
                        buf[cnt++] = rho[index(i, j, k)];
                        buf[cnt++] = rhou[index(i, j, k)];
                        buf[cnt++] = rhov[index(i, j, k)];
                        buf[cnt++] = rhow[index(i, j, k)];
                        buf[cnt++] = E[index(i, j, k)];
                        buf[cnt++] = phi[index(i, j, k)];
                    }
                }
            }
        };
        auto unpackZ = [&](const std::vector<double> &buf, bool plusSide) {
            int cnt = 0;
            for (int j = 0; j < NyGhost; j++) {
                for (int i = 0; i < NxGhost; i++) {
                    int kStart = plusSide ? (NzGhost - nghost) : 0;
                    int kEnd = plusSide ? NzGhost : nghost;
                    for (int k = kStart; k < kEnd; k++) {
                        rho[index(i, j, k)] = buf[cnt++];
                        rhou[index(i, j, k)] = buf[cnt++];
                        rhov[index(i, j, k)] = buf[cnt++];
                        rhow[index(i, j, k)] = buf[cnt++];
                        E[index(i, j, k)] = buf[cnt++];
                        phi[index(i, j, k)] = buf[cnt++];
                    }
                }
            }
        };

        const int numArrays = 6;
        int planeSize = NxGhost * NyGhost * nghost;
        int totalCount = planeSize * numArrays;

        std::vector<double> sendBuf(totalCount), recvBuf(totalCount);

        // Phase 1: send to plus-side, receive from minus-side
        packZ(sendBuf, true);
        int tag = 0;
        MPI_Sendrecv(sendBuf.data(), totalCount, MPI_DOUBLE, rankPlusZ, tag,
                     recvBuf.data(), totalCount, MPI_DOUBLE, rankMinusZ, tag,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        unpackZ(recvBuf, false);

        // Phase 2: send minus-side data, receive plus-side data
        packZ(sendBuf, false);
        MPI_Sendrecv(sendBuf.data(), totalCount, MPI_DOUBLE, rankMinusZ, tag,
                     recvBuf.data(), totalCount, MPI_DOUBLE, rankPlusZ, tag,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        unpackZ(recvBuf, true);
    }
}

}  // namespace agoge
